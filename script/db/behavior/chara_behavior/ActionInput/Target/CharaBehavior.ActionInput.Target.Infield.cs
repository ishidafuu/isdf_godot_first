using System;
using System.Collections.Generic;
using System.Linq;
using isdf;

namespace db;

public partial class CharaBehavior
{
    /// <summary>
    /// パスのタグ状態を管理する構造体
    /// </summary>
    private struct PassTargetState
    {
        /// <summary>ダッシュマンパスの対象ターゲット</summary>
        public PassTargetType[] dashmanPassTargets;
        /// <summary>通常パスの対象ターゲット</summary>
        public PassTargetType[] normalPassTargets;
        /// <summary>ソート済みのパス対象タグ</summary>
        public PassTargetType[] sortedPassTargets;
    }

    /// <summary>
    /// 内野ターゲットを取得します
    /// </summary>
    /// <returns>内野ターゲット</returns>
    private PassTargetType GetNaiyaTarget()
    {
        if (Composite.IsDashman)
        {
            return PassTargetType.Dashman;
        }

        var positionZ = Coordinate.Z;
        if (positionZ > Defines.DBCRT_BL)
        {
            return PassTargetType.Back;
        }
        if (positionZ < Defines.DBCRT_FL)
        {
            return PassTargetType.Front;
        }

        return PassTargetType.Side;
    }

    /// <summary>
    /// 内野パスのターゲットを取得します
    /// </summary>
    /// <returns>内野パスの対象ターゲット配列</returns>
    private PassTargetType[] GetNaiyaPassTarget()
    {
        var state = InitializePassTargetState();
        var keyState = GetPassTargetKeyState();

        ProcessDashmanPass(ref state, keyState);
        ProcessNormalPass(ref state, keyState);

        return state.sortedPassTargets;
    }

    /// <summary>
    /// パスターゲットの状態を初期化します
    /// </summary>
    /// <returns>初期化されたパスターゲット状態</returns>
    private PassTargetState InitializePassTargetState()
    {
        return new PassTargetState
        {
            dashmanPassTargets = EmptyPassTags,
            normalPassTargets = EmptyPassTags,
            sortedPassTargets = EmptyPassTags
        };
    }

    /// <summary>
    /// ダッシュマンパスの処理を行います
    /// </summary>
    /// <param name="state">パスターゲット状態</param>
    /// <param name="keyState">キー入力状態</param>
    private void ProcessDashmanPass(ref PassTargetState state, PassTargetKeyState keyState)
    {
        if (keyState.isNeutralKey)
        {
            state.dashmanPassTargets = GetDashmanPassTarget();
        }
    }

    /// <summary>
    /// 通常パスの処理を行います
    /// </summary>
    /// <param name="state">パスターゲット状態</param>
    /// <param name="keyState">キー入力状態</param>
    private void ProcessNormalPass(ref PassTargetState state, PassTargetKeyState keyState)
    {
        if (!keyState.isNeutralKey)
        {
            state.normalPassTargets = GetOutfieldPassTarget(keyState);
        }

        var isSelectTarget = state.dashmanPassTargets.Length > 0
            ? state.dashmanPassTargets
            : state.normalPassTargets;

        state.sortedPassTargets = GetSortedPassTarget(isSelectTarget);
    }

    /// <summary>
    /// ダッシュマンパスの対象を取得します
    /// </summary>
    /// <returns>ダッシュマンパスの対象ターゲット配列</returns>
    private PassTargetType[] GetDashmanPassTarget()
    {
        var positionState = InitializePositionState();

        foreach (var chara in GetTeamCharas())
        {
            if (!chara.Composite.IsDashman)
            {
                continue;
            }

            UpdatePositionState(chara, ref positionState);
        }

        if (!positionState.isTopPosition)
        {
            return EmptyPassTags;
        }

        SinglePassTag[0] = PassTargetType.Dashman;
        return SinglePassTag;
    }

    /// <summary>
    /// 外野パスの対象を取得します
    /// </summary>
    /// <param name="keyState">キー入力状態</param>
    /// <returns>外野パスの対象ターゲット配列</returns>
    private PassTargetType[] GetOutfieldPassTarget(PassTargetKeyState keyState)
    {
        var positionState = InitializePositionState();

        foreach (var chara in GetTeamCharas())
        {
            UpdatePositionState(chara, ref positionState);
        }

        return GetOutfieldPassTargetByDirection(keyState, positionState);
    }

    /// <summary>
    /// 方向に基づいて外野パスの対象を取得します
    /// </summary>
    /// <param name="keyState">キー入力状態</param>
    /// <param name="positionState">位置状態</param>
    /// <returns>外野パスの対象ターゲット配列</returns>
    private PassTargetType[] GetOutfieldPassTargetByDirection(PassTargetKeyState keyState, PositionState positionState)
    {
        var count = 0;

        if (keyState.isLeftKey || keyState.isRightKey)
        {
            if (positionState.isTopPosition)
            {
                MultiPassTags[count++] = PassTargetType.Side;
            }
        }

        if (keyState.isUpKey)
        {
            if (positionState.isFrontPosition)
            {
                MultiPassTags[count++] = PassTargetType.Front;
            }
        }

        if (keyState.isDownKey)
        {
            if (positionState.isBackwardPosition)
            {
                MultiPassTags[count++] = PassTargetType.Back;
            }
        }

        if (count == 0) return EmptyPassTags;
        if (count == 1)
        {
            SinglePassTag[0] = MultiPassTags[0];
            return SinglePassTag;
        }
        return MultiPassTags;
    }

    /// <summary>
    /// パス対象をソートして取得します
    /// </summary>
    /// <param name="isSelectTarget">ソート対象のターゲット配列</param>
    /// <returns>ソート済みのターゲット配列</returns>
    private PassTargetType[] GetSortedPassTarget(PassTargetType[] isSelectTarget)
    {
        if (isSelectTarget.Length == 0)
        {
            return Array.Empty<PassTargetType>();
        }

        var distances = CalculateTargetDistances(isSelectTarget);
        var sortedTargets = PrepareTargetsForSort(isSelectTarget);
        SortTargets(sortedTargets, distances);

        return sortedTargets;
    }

    /// <summary>
    /// 対象との距離を計算します
    /// </summary>
    /// <param name="isSelectTarget">距離計算対象のターゲット配列</param>
    /// <returns>距離の配列</returns>
    private float[] CalculateTargetDistances(PassTargetType[] isSelectTarget)
    {
        var length = isSelectTarget.Length;
        if (length == 0) return DistanceBuffer;

        for (var i = 0; i < length; i++)
        {
            DistanceBuffer[i] = float.MaxValue;
            var targetTag = isSelectTarget[i];

            foreach (var chara in GetTeamCharas())
            {
                if (chara.GetNaiyaTarget() != targetTag) continue;
                var distance = chara.Coordinate.DistanceXZ(Coordinate);
                if (distance < DistanceBuffer[i])
                {
                    DistanceBuffer[i] = distance;
                }
            }
        }

        return DistanceBuffer;
    }
}