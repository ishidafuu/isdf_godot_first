using System;
using System.Collections.Generic;
using System.Linq;
using isdf;

namespace db;

public partial class CharaBehavior
{
    /// <summary>
    /// 外野ターゲットを取得します
    /// </summary>
    /// <returns>外野ターゲット</returns>
    private PassTargetType GetGaiyaTag()
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
    /// 外野パスのターゲットを取得します
    /// </summary>
    /// <returns>外野パスの対象ターゲット配列</returns>
    private PassTargetType[] GetGaiyaPassTarget()
    {
        var state = InitializeGaiyaPassTargetState();
        var keyState = GetPassTargetKeyState();

        ProcessGaiyaDashmanPass(ref state, keyState);
        ProcessGaiyaNormalPass(ref state, keyState);

        return state.sortedPassTargets;
    }

    /// <summary>
    /// 外野パスターゲットの状態を初期化します
    /// </summary>
    /// <returns>初期化された外野パスターゲット状態</returns>
    private PassTargetState InitializeGaiyaPassTargetState()
    {
        return new PassTargetState
        {
            dashmanPassTargets = EmptyPassTags,
            normalPassTargets = EmptyPassTags,
            sortedPassTargets = EmptyPassTags
        };
    }

    /// <summary>
    /// 外野ダッシュマンパスの処理を行います
    /// </summary>
    /// <param name="state">外野パスターゲット状態</param>
    /// <param name="keyState">キー入力状態</param>
    private void ProcessGaiyaDashmanPass(ref PassTargetState state, PassTargetKeyState keyState)
    {
        if (keyState.isNeutralKey)
        {
            state.dashmanPassTargets = GetGaiyaDashmanPassTarget();
        }
    }

    /// <summary>
    /// 外野通常パスの処理を行います
    /// </summary>
    /// <param name="state">外野パスターゲット状態</param>
    /// <param name="keyState">キー入力状態</param>
    private void ProcessGaiyaNormalPass(ref PassTargetState state, PassTargetKeyState keyState)
    {
        if (!keyState.isNeutralKey)
        {
            state.normalPassTargets = GetGaiyaOutfieldPassTarget(keyState);
        }

        var isSelectTarget = state.dashmanPassTargets.Length > 0
            ? state.dashmanPassTargets
            : state.normalPassTargets;

        state.sortedPassTargets = GetGaiyaSortedPassTarget(isSelectTarget);
    }

    /// <summary>
    /// 外野ダッシュマンパスの対象を取得します
    /// </summary>
    /// <returns>外野ダッシュマンパスの対象ターゲット配列</returns>
    private PassTargetType[] GetGaiyaDashmanPassTarget()
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
    private PassTargetType[] GetGaiyaOutfieldPassTarget(PassTargetKeyState keyState)
    {
        var positionState = InitializePositionState();

        foreach (var chara in GetTeamCharas())
        {
            UpdatePositionState(chara, ref positionState);
        }

        return GetGaiyaOutfieldPassTargetByDirection(keyState, positionState);
    }

    /// <summary>
    /// 方向に基づいて外野パスの対象を取得します
    /// </summary>
    /// <param name="keyState">キー入力状態</param>
    /// <param name="positionState">位置状態</param>
    /// <returns>外野パスの対象ターゲット配列</returns>
    private PassTargetType[] GetGaiyaOutfieldPassTargetByDirection(PassTargetKeyState keyState, PositionState positionState)
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
    /// 外野パス対象をソートして取得します
    /// </summary>
    /// <param name="isSelectTarget">ソート対象のターゲット配列</param>
    /// <returns>ソート済みのターゲット配列</returns>
    private PassTargetType[] GetGaiyaSortedPassTarget(PassTargetType[] isSelectTarget)
    {
        if (isSelectTarget.Length == 0)
        {
            return Array.Empty<PassTargetType>();
        }

        var distances = CalculateGaiyaTargetDistances(isSelectTarget);
        var sortedTargets = PrepareTargetsForSort(isSelectTarget);
        SortTargets(sortedTargets, distances);

        return sortedTargets;
    }

    /// <summary>
    /// 外野対象との距離を計算します
    /// </summary>
    /// <param name="isSelectTarget">距離計算対象のターゲット配列</param>
    /// <returns>距離の配列</returns>
    private float[] CalculateGaiyaTargetDistances(PassTargetType[] isSelectTarget)
    {
        var length = isSelectTarget.Length;
        if (length == 0) return DistanceBuffer;

        for (var i = 0; i < length; i++)
        {
            DistanceBuffer[i] = float.MaxValue;
            var targetTag = isSelectTarget[i];

            foreach (var chara in GetTeamCharas())
            {
                if (chara.GetGaiyaTag() != targetTag) continue;
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