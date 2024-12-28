using System;
using isdf;

namespace db;

public partial class CharaBehavior
{
    /// <summary>
    /// パスのターゲットを表す列挙型
    /// </summary>
    private enum PassTargetType
    {
        /// <summary>ダッシュマン</summary>
        Dashman,
        /// <summary>サイド</summary>
        Side,
        /// <summary>前方</summary>
        Front,
        /// <summary>後方</summary>
        Back
    }

    // 再利用する配列（内部処理用）
    private static readonly PassTargetType[] EmptyPassTags = Array.Empty<PassTargetType>();
    private static readonly PassTargetType[] SinglePassTag = new PassTargetType[1];
    private static readonly PassTargetType[] MultiPassTags = new PassTargetType[4];
    private static readonly float[] DistanceBuffer = new float[4];
    private static readonly CharaBehavior[] TeamCharas = new CharaBehavior[Defines.DBMEMBER_INF];

    // 再利用する配列（結果用）
    private static readonly OrderIndexType[] ResultPassTags = new OrderIndexType[4];
    private static readonly OrderIndexType[] ResultSingleTag = new OrderIndexType[1];

    /// <summary>
    /// キー入力の状態を管理する構造体
    /// </summary>
    private struct PassTargetKeyState
    {
        /// <summary>左キーが押されているかどうか</summary>
        public bool isLeftKey;
        /// <summary>右キーが押されているかどうか</summary>
        public bool isRightKey;
        /// <summary>上キーが押されているかどうか</summary>
        public bool isUpKey;
        /// <summary>下キーが押されているかどうか</summary>
        public bool isDownKey;
        /// <summary>方向キーが押されていない状態かどうか</summary>
        public bool isNeutralKey;
    }

    /// <summary>
    /// プレイヤーの位置状態を管理する構造体
    /// </summary>
    private struct PositionState
    {
        /// <summary>最前列のポジションにいるかどうか</summary>
        public bool isTopPosition;
        /// <summary>最前方のポジションにいるかどうか</summary>
        public bool isFrontPosition;
        /// <summary>最後方のポジションにいるかどうか</summary>
        public bool isBackwardPosition;
    }

    /// <summary>
    /// キー入力の状態を取得します
    /// </summary>
    /// <returns>現在のキー入力状態</returns>
    private PassTargetKeyState GetPassTargetKeyState()
    {
        var (isLeftKey, isRightKey, isUpKey, isDownKey) = GetKeyInputs();
        if (Composite.IsSelfControl)
        {
            isLeftKey = Pad.KeyLeft.IsPressed;
            isRightKey = Pad.KeyRight.IsPressed;
            isUpKey = Pad.KeyUp.IsPressed;
            isDownKey = Pad.KeyDown.IsPressed;
        }

        return new PassTargetKeyState
        {
            isLeftKey = isLeftKey,
            isRightKey = isRightKey,
            isUpKey = isUpKey,
            isDownKey = isDownKey,
            isNeutralKey = !(isLeftKey || isRightKey || isUpKey || isDownKey)
        };
    }

    /// <summary>
    /// キー入力の状態を取得します
    /// </summary>
    /// <returns>各方向キーの入力状態をタプルで返します</returns>
    private (bool isLeftKey, bool isRightKey, bool isUpKey, bool isDownKey) GetKeyInputs()
    {
        if (!Composite.IsSelfControl)
        {
            return (false, false, false, false);
        }

        return (
            Pad.KeyLeft.IsPressed,
            Pad.KeyRight.IsPressed,
            Pad.KeyUp.IsPressed,
            Pad.KeyDown.IsPressed
        );
    }

    /// <summary>
    /// 内野方向を判定します
    /// </summary>
    /// <param name="passDirectionX">パスのX方向</param>
    /// <returns>内野方向の場合はtrue</returns>
    private bool GetInfieldDirection(DirectionXType passDirectionX)
    {
        return MySideIndex == 0
            ? passDirectionX == DirectionXType.Left
            : passDirectionX == DirectionXType.Right;
    }

    /// <summary>
    /// キャラクターの位置状態を更新します
    /// </summary>
    /// <param name="chara">更新対象のキャラクター</param>
    /// <param name="state">更新する位置状態</param>
    private void UpdatePositionState(CharaBehavior chara, ref PositionState state)
    {
        if (chara.Composite.LeftCourtX > Composite.LeftCourtX)
        {
            state.isTopPosition = false;
        }

        if (chara.Coordinate.Z > Coordinate.Z)
        {
            state.isBackwardPosition = false;
        }

        if (chara.Coordinate.Z < Coordinate.Z)
        {
            state.isFrontPosition = false;
        }
    }

    /// <summary>
    /// 位置状態を初期化します
    /// </summary>
    /// <returns>初期化された位置状態</returns>
    private PositionState InitializePositionState()
    {
        return new PositionState
        {
            isTopPosition = true,
            isFrontPosition = true,
            isBackwardPosition = true
        };
    }

    /// <summary>
    /// ソート用の対象配列を準備します
    /// </summary>
    /// <param name="isSelectTarget">準備対象のターゲット配列</param>
    /// <returns>準備されたターゲット配列</returns>
    private PassTargetType[] PrepareTargetsForSort(PassTargetType[] isSelectTarget)
    {
        if (isSelectTarget.Length == 0) return EmptyPassTags;
        if (isSelectTarget.Length == 1)
        {
            SinglePassTag[0] = isSelectTarget[0];
            return SinglePassTag;
        }

        Array.Copy(isSelectTarget, MultiPassTags, isSelectTarget.Length);
        return MultiPassTags;
    }

    /// <summary>
    /// 対象をソートします
    /// </summary>
    /// <param name="sortedTargets">ソート対象のターゲット配列</param>
    /// <param name="distances">距離の配列</param>
    private void SortTargets(PassTargetType[] sortedTargets, float[] distances)
    {
        var length = sortedTargets.Length;
        if (length <= 1) return;

        for (var i = 0; i < length - 1; i++)
        {
            for (var j = 0; j < length - 1 - i; j++)
            {
                if (CalculateSortValue(sortedTargets[j], distances[j]) >
                    CalculateSortValue(sortedTargets[j + 1], distances[j + 1]))
                {
                    (sortedTargets[j], sortedTargets[j + 1]) = (sortedTargets[j + 1], sortedTargets[j]);
                    (distances[j], distances[j + 1]) = (distances[j + 1], distances[j]);
                }
            }
        }
    }

    /// <summary>
    /// ソート値を計算します
    /// </summary>
    /// <param name="tag">計算対象のターゲット</param>
    /// <param name="distance">距離</param>
    /// <returns>計算されたソート値</returns>
    private float CalculateSortValue(PassTargetType tag, float distance)
    {
        return tag switch
        {
            PassTargetType.Dashman => distance * 0.5f,
            _ => distance
        };
    }

    /// <summary>
    /// チームのキャラクターを取得します
    /// </summary>
    /// <returns>チームのキャラクター配列</returns>
    private CharaBehavior[] GetTeamCharas()
    {
        for (var order = 0; order < Defines.DBMEMBER_INF; ++order)
        {
            if ((OrderIndexType)order == MyOrderIndex)
            {
                TeamCharas[order] = null;
                continue;
            }

            TeamCharas[order] = CharaBehaviorManager.Instance.GetOrderChara(MySideIndex, order);
        }
        return TeamCharas;
    }

    /// <summary>
    /// 角度チェックを行います
    /// </summary>
    /// <param name="x">X座標</param>
    /// <param name="z">Z座標</param>
    /// <returns>角度チェックがNGの場合はtrue</returns>
    private bool IsCheckNoAgl(float x, float z)
    {
        return IsCheckLandEnPos((int)MyOrderIndex) || IsCheckLandEnPos((int)OrderIndexType.Disabled);
    }

    /// <summary>
    /// 12時方向の角度を取得します
    /// </summary>
    /// <param name="x">X座標</param>
    /// <param name="z">Z座標</param>
    /// <returns>12時方向の角度</returns>
    private int GetTagAgl2(float x, float z)
    {
        const float PI2 = MathF.PI * 2;
        var angle = MathF.Atan2(z, x);
        if (angle < 0) angle += PI2;
        return (int)(angle * 6 / MathF.PI);
    }

    /// <summary>
    /// パスターゲットタイプをオーダーインデックスに変換します
    /// </summary>
    /// <param name="passTargets">パスターゲットタイプの配列</param>
    /// <returns>オーダーインデックスの配列</returns>
    private OrderIndexType[] ConvertToOrderIndex(PassTargetType[] passTargets)
    {
        if (passTargets.Length == 0) return Array.Empty<OrderIndexType>();

        var count = 0;
        foreach (var target in passTargets)
        {
            foreach (var chara in GetTeamCharas())
            {
                if (chara.GetNaiyaTarget() == target)
                {
                    ResultPassTags[count++] = chara.Order.OrderIndex;
                    break;
                }
            }
        }

        if (count == 0) return Array.Empty<OrderIndexType>();
        if (count == 1)
        {
            ResultSingleTag[0] = ResultPassTags[0];
            return ResultSingleTag;
        }

        var result = new OrderIndexType[count];
        Array.Copy(ResultPassTags, result, count);
        return result;
    }

    /// <summary>
    /// キー入力状態からパスターゲットを取得します
    /// </summary>
    private PassTargetType[] GetPassTargetsFromKeyState(PassTargetKeyState keyState, PositionState positionState)
    {
        var count = 0;

        // ダッシュマンパス
        if (keyState.isNeutralKey)
        {
            MultiPassTags[count++] = PassTargetType.Dashman;
        }

        // 通常パス
        if (keyState.isLeftKey || keyState.isRightKey)
        {
            MultiPassTags[count++] = PassTargetType.Side;
        }
        if (keyState.isUpKey)
        {
            MultiPassTags[count++] = PassTargetType.Front;
        }
        if (keyState.isDownKey)
        {
            MultiPassTags[count++] = PassTargetType.Back;
        }

        if (count == 0) return EmptyPassTags;
        if (count == 1)
        {
            SinglePassTag[0] = MultiPassTags[0];
            return SinglePassTag;
        }

        return MultiPassTags;
    }
}