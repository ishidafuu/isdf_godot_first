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
    /// <param name="passTarget">パスターゲットタイプ</param>
    /// <returns>オーダーインデックス</returns>
    private OrderIndexType ConvertToOrderIndex(PassTargetType passTarget)
    {
        foreach (var chara in GetTeamCharas())
        {
            if (chara == null) continue;
            if (Order.IsInfield ? chara.GetNaiyaTarget() == passTarget : chara.GetGaiyaTag() == passTarget)
            {
                return chara.Order.OrderIndex;
            }
        }

        return OrderIndexType.Disabled;
    }

    /// <summary>
    /// キー入力状態からパスターゲットを取得します
    /// </summary>
    private PassTargetType GetPassTargetFromKeyState(PassTargetKeyState keyState, PositionState positionState)
    {
        // ダッシュマンパス
        if (keyState.isNeutralKey)
        {
            return PassTargetType.Dashman;
        }

        // 通常パス
        if (keyState.isLeftKey || keyState.isRightKey)
        {
            return PassTargetType.Side;
        }
        if (keyState.isUpKey)
        {
            return PassTargetType.Front;
        }
        if (keyState.isDownKey)
        {
            return PassTargetType.Back;
        }

        return PassTargetType.Dashman; // デフォルトはダッシュマン
    }

    /// <summary>
    /// シュートターゲットを設定します
    /// </summary>
    /// <param name="orderIndex">設定するオーダーインデックス</param>
    private void SetShootTarget(OrderIndexType orderIndex)
    {
        if (IsNGPassTag(orderIndex))
        {
            if (MyOrderIndex != OrderIndexType.Outfield4 && IsNGPassTag(OrderIndexType.Outfield4) == false)
            {
                orderIndex = OrderIndexType.Outfield4;
            }
            else if (MyOrderIndex != OrderIndexType.Outfield3 && IsNGPassTag(OrderIndexType.Outfield3) == false)
            {
                orderIndex = OrderIndexType.Outfield3;
            }
            else if (MyOrderIndex != OrderIndexType.Outfield2 && IsNGPassTag(OrderIndexType.Outfield2) == false)
            {
                orderIndex = OrderIndexType.Outfield2;
            }
            else
            {
                if (MyTeam.Position.PassAbleCount > 0
                    && MyOrderIndex != MyTeam.Position.Postman
                    && IsNGPassTag(MyTeam.Position.Postman) == false)
                {
                    orderIndex = MyTeam.Position.Postman;
                }
                else
                {
                    orderIndex = OrderIndexType.Outfield4;
                }
            }
        }

        Ball.CallChangePassTarget(MySideIndex, orderIndex);
    }

    /// <summary>
    /// パスターゲットを設定します
    /// </summary>
    /// <param name="orderIndex">設定するオーダーインデックス</param>
    private void SetPassTarget(OrderIndexType orderIndex)
    {
        if (IsNGPassTag(orderIndex))
        {
            if (MyOrderIndex != OrderIndexType.Outfield4 && IsNGPassTag(OrderIndexType.Outfield4) == false)
            {
                orderIndex = OrderIndexType.Outfield4;
            }
            else if (MyOrderIndex != OrderIndexType.Outfield3 && IsNGPassTag(OrderIndexType.Outfield3) == false)
            {
                orderIndex = OrderIndexType.Outfield3;
            }
            else if (MyOrderIndex != OrderIndexType.Outfield2 && IsNGPassTag(OrderIndexType.Outfield2) == false)
            {
                orderIndex = OrderIndexType.Outfield2;
            }
            else
            {
                if (MyTeam.Position.PassAbleCount > 0
                    && MyOrderIndex != MyTeam.Position.Postman
                    && IsNGPassTag(MyTeam.Position.Postman) == false)
                {
                    orderIndex = MyTeam.Position.Postman;
                }
                else
                {
                    orderIndex = OrderIndexType.Outfield4;
                }
            }
        }

        Ball.CallChangePassTarget(MySideIndex, orderIndex);
    }

    /// <summary>
    /// パスカットターゲットを設定します
    /// </summary>
    /// <param name="orderIndex">設定するオーダーインデックス</param>
    private void PaCtTagSet(OrderIndexType orderIndex)
    {
        //パスカットキャラセット
        var passTargetChara = CharaBehaviorManager.Instance.GetOrderChara(MySideIndex, orderIndex);
        var passTargetCharaX = passTargetChara.Coordinate.X;
        var passTargetCharaZ = passTargetChara.Coordinate.Z;

        var passCutOrderIndex = OrderIndexType.Disabled;
        var maxDist = int.MaxValue;

        for (var order = 0; order < Defines.DBMEMBER_INF; ++order)
        {
            var targetX = passTargetCharaX - EnemySideOrders[order].Coordinate.X;
            var targetZ = Defines.PercentageOf(passTargetCharaZ - EnemySideOrders[order].Coordinate.Z, Defines.ZPER);
            var dist = Defines.Hypot(targetX, targetZ);

            if (dist >= maxDist)
            {
                continue;
            }

            maxDist = dist;
            passCutOrderIndex = (OrderIndexType)order;
        }

        Ball.CallChangePassTarget(MySideIndex, passCutOrderIndex);
    }

    /// <summary>
    /// パスターゲットとして無効かどうかをチェックします
    /// 指定されたオーダーがパスターゲットとして有効かを判定します
    /// </summary>
    /// <param name="order">チェック対象のオーダー</param>
    /// <returns>パスターゲットとして無効な場合はtrue</returns>
    private bool IsNGPassTag(OrderIndexType order)
    {
        if (order == MyOrderIndex || order == OrderIndexType.Disabled)
        {
            return true; //パス不可
        }

        var chara = CharaBehaviorManager.Instance.GetOrderChara(MySideIndex, order);

        //★ダッシュマンはだいじょぶ
        if (chara.Composite.IsDashman)
        //&& ((st_.pmgMyTm_.st_.pmgMyCh_[pos].MyCoordinate.dY >= (-XYMAG))
        //  || (MyMirPass_c > 0)))//下降ではない
        {
            return false;
        }

        //空中の人はパスタゲにならないように
        return chara.Composite.IsFree(true) == false || chara.Motion.HasFlag(CharaMotionFlag.Ar);
    }

    /// <summary>
    /// パスターゲットとして無効かどうかをチェックします（int版）
    /// </summary>
    /// <param name="order">チェック対象のオーダー番号</param>
    /// <returns>パスターゲットとして無効な場合はtrue</returns>
    private bool IsNGPassTag(int order)
    {
        return IsNGPassTag((OrderIndexType)order);
    }

    /// <summary>
    /// 指定された角度が現在の向きに対して有効かチェックします
    /// キャラクターの向きと目標位置の角度関係を判定します
    /// </summary>
    /// <param name="targetX">目標X座標</param>
    /// <param name="targetZ">目標Z座標</param>
    /// <returns>角度が有効でない場合はtrue</returns>
    private bool IsCheckNoAgl(int targetX, int targetZ)
    {
        var isInAngle = Coordinate.DirectionZ switch
        {
            DirectionZType.Forward => targetZ < Coordinate.Z,
            DirectionZType.Neutral => true,
            DirectionZType.Backward => targetZ > Coordinate.Z,
            _ => throw new ArgumentOutOfRangeException(),
        };

        if (isInAngle)
        {
            isInAngle = Coordinate.DirectionX switch
            {
                DirectionXType.Left => targetX < Coordinate.X,
                DirectionXType.Neutral => true,
                DirectionXType.Right => targetX > Coordinate.X,
                _ => throw new ArgumentOutOfRangeException(),
            };
        }

        return isInAngle == false;
    }

    /// <summary>
    /// 敵コートに着地するかどうかをチェックします
    /// キャラクターの着地位置が敵コートになるかを判定します
    /// </summary>
    /// <param name="order">チェック対象のオーダーインデックス</param>
    /// <returns>敵コートに着地する場合はtrue</returns>
    private bool IsCheckLandEnPos(int order)
    {
        var chara = CharaBehaviorManager.Instance.GetOrderChara(MySideIndex, order);

        var tLandX = MySideIndex == 0
            ? chara.Air.LandX
            : Defines.DBCRT_CL - (chara.Air.LandX - Defines.DBCRT_CL);

        return chara.Order.IsInfield && tLandX > Defines.DBCRT_CLI;
    }

    /// <summary>
    /// 向きから角度を取得します
    /// 現在の向きから12時方向の角度を計算します
    /// </summary>
    /// <returns>12時方向の角度</returns>
    private int GetMukiAglFromDirection()
    {
        switch (Order.GetOrderFieldType())
        {
            case OrderFieldType.Infield:
                if (Coordinate.DirectionX == DirectionXType.Right)
                {
                    if (Coordinate.DirectionZ == DirectionZType.Forward)
                    {
                        return 11; //11 0 1 2
                    }

                    if (Coordinate.DirectionZ == DirectionZType.Backward)
                    {
                        return 3; //3 4 5 6
                    }
                    return 1; //1 2 3 4
                }

                if (Coordinate.DirectionX == DirectionXType.Left)
                {
                    if (Coordinate.DirectionZ == DirectionZType.Forward)
                    {
                        return 9; //9 10 11 0
                    }

                    if (Coordinate.DirectionZ == DirectionZType.Backward)
                    {
                        return 5; //5 6 7 8
                    }
                    return 7; //7 8 9 10
                }
                break;
            case OrderFieldType.Outfield2:
                if (Coordinate.DirectionX == DirectionXType.Left)
                {
                    return 5; //5678
                }

                if (Coordinate.DirectionX == DirectionXType.Right)
                {
                    return 3; //3456
                }
                break;
            case OrderFieldType.Outfield3:
                if (Coordinate.DirectionX == DirectionXType.Left)
                {
                    return 9; //9 10 11 0
                }

                if (Coordinate.DirectionX == DirectionXType.Right)
                {
                    return 11; //11 0 1 2
                }
                break;
            case OrderFieldType.Outfield4:
                if (MySideIndex == 0)
                {
                    if (Coordinate.DirectionZ == DirectionZType.Forward)
                    {
                        return 9; //9 10 11 0
                    }

                    if (Coordinate.DirectionZ == DirectionZType.Backward)
                    {
                        return 5; //5 6 7 8
                    }
                    return 7; //7 8 9 10
                }

                if (Coordinate.DirectionZ == DirectionZType.Forward)
                {
                    return 11; //11 0 1 2
                }

                if (Coordinate.DirectionZ == DirectionZType.Backward)
                {
                    return 3; //3 4 5 6
                }
                return 1; //1 2 3 4
            default:
                throw new ArgumentOutOfRangeException();
        }

        // 来ない想定
        return 0;
    }
}