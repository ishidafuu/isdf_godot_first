/* // using System;
// using isdf;

// namespace db;

// public partial class CharaBehavior
// {
//     /// <summary>
//     /// 内野のパスターゲットを取得します
//     /// 内野プレイヤーの中から適切なパスターゲットを選択します
//     /// </summary>
//     /// <returns>選択されたパスターゲットのオーダーインデックス</returns>
//     private OrderIndexType GetNaiyaPassTag()
//     {
//         var state = InitializePassTagState();
//         var keyState = GetPassTagKeyState();
//         var positionState = InitializePositionState();

//         // ダッシュマンパス処理
//         if (state.isDashmanPass)
//         {
//             if (ProcessDashmanPass(state, keyState, positionState))
//             {
//                 return GetDashmanPassTarget(state, keyState, positionState);
//             }
//         }

//         // 通常パス処理
//         if (ProcessNormalPass(state, keyState, positionState))
//         {
//             return GetOutfieldPassTarget(state, keyState, positionState);
//         }

//         return GetSortedPassTarget(state, keyState);
//     }

//     /// <summary>
//     /// パスターゲットの状態を管理する構造体
//     /// </summary>
//     private struct PassTagState
//     {
//         /// <summary>完全にターゲットが存在しない状態</summary>
//         public bool isNoneTarget;
//         /// <summary>向き方向にターゲットが存在しない状態</summary>
//         public bool isNoneAngleTarget;
//         /// <summary>ダッシュマンへのパス状態</summary>
//         public bool isDashmanPass;
//         /// <summary>パスの方向（X軸）</summary>
//         public DirectionXType passDirectionX;
//         /// <summary>パスの方向（Z軸）</summary>
//         public DirectionZType passDirectionZ;
//         /// <summary>内野方向を向いているかどうか</summary>
//         public bool isInfieldDirection;
//         /// <summary>敵コート方向のキー入力があるかどうか</summary>
//         public bool isEnemyCourtKey;
//     }

//     /// <summary>
//     /// キー入力の状態を管理する構造体
//     /// </summary>
//     private struct PassTagKeyState
//     {
//         /// <summary>左キーが押されているかどうか</summary>
//         public bool isLeftKey;
//         /// <summary>右キーが押されているかどうか</summary>
//         public bool isRightKey;
//         /// <summary>上キーが押されているかどうか</summary>
//         public bool isUpKey;
//         /// <summary>下キーが押されているかどうか</summary>
//         public bool isDownKey;
//         /// <summary>方向キーが押されていない状態かどうか</summary>
//         public bool isNeutralKey;
//     }

//     /// <summary>
//     /// プレイヤーの位置状態を管理する構造体
//     /// </summary>
//     private struct PositionState
//     {
//         /// <summary>最前列のポジションにいるかどうか</summary>
//         public bool isTopPosition;
//         /// <summary>最前方のポジションにいるかどうか</summary>
//         public bool isFrontPosition;
//         /// <summary>最後方のポジションにいるかどうか</summary>
//         public bool isBackwardPosition;
//     }

//     /// <summary>
//     /// パスターゲットの状態を初期化します
//     /// </summary>
//     /// <returns>初期化されたパスターゲットの状態</returns>
//     private PassTagState InitializePassTagState()
//     {
//         return new PassTagState
//         {
//             isNoneTarget = true,
//             isNoneAngleTarget = true,
//             isDashmanPass = MyTeam.Position.DashmanNum > 0,
//             passDirectionX = Coordinate.DirectionX,
//             passDirectionZ = Coordinate.DirectionZ,
//             isInfieldDirection = GetInfieldDirection(Coordinate.DirectionX),
//             isEnemyCourtKey = (MySideIndex == 0 && Pad.KeyRight.IsPressed) || (MySideIndex == 1 && Pad.KeyLeft.IsPressed)
//         };
//     }

//     /// <summary>
//     /// キー入力の状態を取得します
//     /// </summary>
//     /// <returns>現在のキー入力状態</returns>
//     private PassTagKeyState GetPassTagKeyState()
//     {
//         var (isLeftKey, isRightKey, isUpKey, isDownKey) = GetKeyInputs();
//         if (Composite.IsSelfControl)
//         {
//             isLeftKey = Pad.KeyLeft.IsPressed;
//             isRightKey = Pad.KeyRight.IsPressed;
//             isUpKey = Pad.KeyUp.IsPressed;
//             isDownKey = Pad.KeyDown.IsPressed;
//         }

//         return new PassTagKeyState
//         {
//             isLeftKey = isLeftKey,
//             isRightKey = isRightKey,
//             isUpKey = isUpKey,
//             isDownKey = isDownKey,
//             isNeutralKey = !(isLeftKey || isRightKey || isUpKey || isDownKey)
//         };
//     }

//     /// <summary>
//     /// 位置状態を初期化します
//     /// </summary>
//     /// <returns>初期化された位置状態</returns>
//     private PositionState InitializePositionState()
//     {
//         return new PositionState
//         {
//             isTopPosition = true,
//             isFrontPosition = true,
//             isBackwardPosition = true
//         };
//     }

//     /// <summary>
//     /// ダッシュマンパスの処理を行います
//     /// </summary>
//     /// <param name="state">パスターゲットの状態</param>
//     /// <param name="keyState">キー入力の状態</param>
//     /// <param name="positionState">位置の状態</param>
//     /// <returns>ダッシュマンパスが可能な場合はtrue</returns>
//     private bool ProcessDashmanPass(PassTagState state, PassTagKeyState keyState, PositionState positionState)
//     {
//         TmpStateManager.Instance.TmpState.Clear();
//         var isSelectTarget = TmpStateManager.Instance.TmpState.isSelectTarget;

//         for (var order = 0; order < Defines.DBMEMBER_INF; ++order)
//         {
//             if ((OrderIndexType)order == MyOrderIndex)
//             {
//                 continue;
//             }

//             var chara = CharaBehaviorManager.Instance.GetOrderChara(MySideIndex, order);

//             if (!chara.Composite.IsDashman)
//             {
//                 continue;
//             }

//             isSelectTarget[order] = enNaiyaTag.TGOK;
//             state.isNoneAngleTarget = false;

//             UpdatePositionState(chara, ref positionState);
//         }

//         return !state.isNoneAngleTarget;
//     }

//     /// <summary>
//     /// キャラクターの位置状態を更新します
//     /// </summary>
//     /// <param name="chara">更新対象のキャラクター</param>
//     /// <param name="state">更新する位置状態</param>
//     private void UpdatePositionState(CharaBehavior chara, ref PositionState state)
//     {
//         if (chara.Composite.LeftCourtX > Composite.LeftCourtX)
//         {
//             state.isTopPosition = false;
//         }

//         if (chara.Coordinate.Z > Coordinate.Z)
//         {
//             state.isBackwardPosition = false;
//         }

//         if (chara.Coordinate.Z < Coordinate.Z)
//         {
//             state.isFrontPosition = false;
//         }
//     }

//     /// <summary>
//     /// 通常パスの処理を行います
//     /// </summary>
//     /// <param name="state">パスターゲットの状態</param>
//     /// <param name="keyState">キー入力の状態</param>
//     /// <param name="positionState">位置の状態</param>
//     /// <returns>外野パスが必要な場合はtrue</returns>
//     private bool ProcessNormalPass(PassTagState state, PassTagKeyState keyState, PositionState positionState)
//     {
//         TmpStateManager.Instance.TmpState.Clear();
//         var isSelectTarget = TmpStateManager.Instance.TmpState.isSelectTarget;

//         for (var order = 0; order < Defines.DBMEMBER_INF; ++order)
//         {
//             if (IsNGPassTag(order))
//             {
//                 isSelectTarget[order] = enNaiyaTag.TGNG;
//                 continue;
//             }

//             state.isNoneTarget = false;
//             var chara = CharaBehaviorManager.Instance.GetOrderChara(MySideIndex, order);

//             isSelectTarget[order] = IsCheckNoAgl(chara.Coordinate.X, chara.Coordinate.Z)
//                 ? enNaiyaTag.TGNOAGL
//                 : enNaiyaTag.TGOK;

//             if (isSelectTarget[order] == enNaiyaTag.TGOK)
//             {
//                 state.isNoneAngleTarget = false;
//             }

//             UpdatePositionState(chara, ref positionState);
//         }

//         return state.isNoneTarget || ((state.isInfieldDirection == false && positionState.isTopPosition));
//     }

//     /// <summary>
//     /// ダッシュマンへのパスターゲットを取得します
//     /// </summary>
//     /// <param name="state">パスターゲットの状態</param>
//     /// <param name="keyState">キー入力の状態</param>
//     /// <param name="positionState">位置の状態</param>
//     /// <returns>選択されたパスターゲットのオーダーインデックス</returns>
//     private OrderIndexType GetDashmanPassTarget(PassTagState state, PassTagKeyState keyState, PositionState positionState)
//     {
//         if (positionState.isTopPosition && state.isEnemyCourtKey)
//         {
//             if (keyState.isUpKey)
//             {
//                 return OrderIndexType.Outfield2;
//             }

//             if (keyState.isDownKey)
//             {
//                 return OrderIndexType.Outfield3;
//             }

//             var distO2 = Math.Abs(Coordinate.Z - Defines.DBCRT_BL);
//             var distO3 = Math.Abs(Coordinate.Z - Defines.DBCRT_FL);
//             return distO2 < distO3 ? OrderIndexType.Outfield2 : OrderIndexType.Outfield3;
//         }

//         if (positionState.isFrontPosition && keyState.isDownKey)
//         {
//             return OrderIndexType.Outfield3;
//         }

//         if (positionState.isBackwardPosition && keyState.isUpKey)
//         {
//             return OrderIndexType.Outfield2;
//         }

//         return GetSortedPassTarget(state, keyState);
//     }

//     /// <summary>
//     /// 外野へのパスターゲットを取得します
//     /// </summary>
//     /// <param name="state">パスターゲットの状態</param>
//     /// <param name="keyState">キー入力の状態</param>
//     /// <param name="positionState">位置の状態</param>
//     /// <returns>選択されたパスターゲットのオーダーインデックス</returns>
//     private OrderIndexType GetOutfieldPassTarget(PassTagState state, PassTagKeyState keyState, PositionState positionState)
//     {
//         if (Motion.MotionType == CharaMotionType.Ds)
//         {
//             if (Composite.IsSelfControl)
//             {
//                 if (keyState.isUpKey) return OrderIndexType.Outfield2;
//                 if (keyState.isDownKey) return OrderIndexType.Outfield3;
//                 if (state.isEnemyCourtKey) return OrderIndexType.Outfield4;

//                 var distO2 = Math.Abs(Coordinate.Z - Defines.DBCRT_BL);
//                 var distO3 = Math.Abs(Coordinate.Z - Defines.DBCRT_FL);
//                 return distO2 < distO3 ? OrderIndexType.Outfield2 : OrderIndexType.Outfield3;
//             }

//             return Auto.DirectionZ switch
//             {
//                 DirectionZType.Backward => OrderIndexType.Outfield2,
//                 DirectionZType.Forward => OrderIndexType.Outfield3,
//                 _ => OrderIndexType.Outfield4,
//             };
//         }

//         return GetOutfieldPassTargetByDirection(state);
//     }

//     /// <summary>
//     /// 方向に基づいて外野パスターゲットを取得します
//     /// </summary>
//     /// <param name="state">パスターゲットの状態</param>
//     /// <returns>選択されたパスターゲットのオーダーインデックス</returns>
//     private OrderIndexType GetOutfieldPassTargetByDirection(PassTagState state)
//     {
//         switch (state.passDirectionZ)
//         {
//             case DirectionZType.Backward:
//                 return OrderIndexType.Outfield2;
//             case DirectionZType.Forward:
//                 return OrderIndexType.Outfield3;
//             default:
//                 if (state.isEnemyCourtKey)
//                 {
//                     return OrderIndexType.Outfield4;
//                 }

//                 var distO2 = Math.Abs(Coordinate.Z - Defines.DBCRT_BL);
//                 var distO3 = Math.Abs(Coordinate.Z - Defines.DBCRT_FL);
//                 return distO2 < distO3 ? OrderIndexType.Outfield2 : OrderIndexType.Outfield3;
//         }
//     }

//     /// <summary>
//     /// ソートされたパスターゲットを取得します
//     /// </summary>
//     /// <param name="state">パスターゲットの状態</param>
//     /// <param name="keyState">キー入力の状態</param>
//     /// <returns>優先順位が最も高いパスターゲットのオーダーインデックス</returns>
//     private OrderIndexType GetSortedPassTarget(PassTagState state, PassTagKeyState keyState)
//     {
//         TmpStateManager.Instance.TmpState.Clear();
//         var targetDist = TmpStateManager.Instance.TmpState.targetDist;
//         var isSelectTarget = TmpStateManager.Instance.TmpState.isSelectTarget;
//         var targetOrder = TmpStateManager.Instance.TmpState.targetOrder;
//         var sortValue = TmpStateManager.Instance.TmpState.sortValue;

//         CalculateTargetDistances(targetDist);
//         var targetCount = PrepareTargetsForSort(state, keyState, isSelectTarget, targetOrder, sortValue);
//         SortTargets(targetCount, targetOrder, sortValue);

//         return targetOrder[0];
//     }

//     /// <summary>
//     /// ターゲットまでの距離を計算します
//     /// </summary>
//     /// <param name="targetDist">距離を格納する配列</param>
//     private void CalculateTargetDistances(int[] targetDist)
//     {
//         for (var order = 0; order < Defines.DBMEMBER_INF; ++order)
//         {
//             if ((OrderIndexType)order == MyOrderIndex)
//             {
//                 continue;
//             }

//             targetDist[order] = Coordinate.DistanceXZ(MySideOrders[order].Coordinate);
//         }
//     }

//     /// <summary>
//     /// ソート用のターゲットを準備します
//     /// </summary>
//     /// <param name="state">パスターゲットの状態</param>
//     /// <param name="keyState">キー入力の状態</param>
//     /// <param name="isSelectTarget">選択可能なターゲットの配列</param>
//     /// <param name="targetOrder">ターゲットの順序配列</param>
//     /// <param name="sortValue">ソート用の値配列</param>
//     /// <returns>有効なターゲットの数</returns>
//     private int PrepareTargetsForSort(PassTagState state, PassTagKeyState keyState, enNaiyaTag[] isSelectTarget, OrderIndexType[] targetOrder, int[] sortValue)
//     {
//         var targetCount = 0;

//         for (var order = 0; order < Defines.DBMEMBER_INF; ++order)
//         {
//             if (isSelectTarget[order] != enNaiyaTag.TGOK && (!state.isNoneAngleTarget || isSelectTarget[order] == enNaiyaTag.TGNG))
//             {
//                 targetOrder[targetCount] = OrderIndexType.Disabled;
//                 continue;
//             }

//             var chara = CharaBehaviorManager.Instance.GetOrderChara(MySideIndex, order);
//             sortValue[order] = CalculateSortValue(keyState, chara, order);
//             targetOrder[targetCount++] = (OrderIndexType)order;
//         }

//         return targetCount;
//     }

//     /// <summary>
//     /// ターゲットのソート値を計算します
//     /// </summary>
//     /// <param name="keyState">キー入力の状態</param>
//     /// <param name="chara">対象のキャラクター</param>
//     /// <param name="order">キャラクターの順序</param>
//     /// <returns>計算されたソート値</returns>
//     private int CalculateSortValue(PassTagKeyState keyState, CharaBehavior chara, int order)
//     {
//         if (keyState.isNeutralKey)
//         {
//             return TmpStateManager.Instance.TmpState.targetDist[order];
//         }

//         var tgZ = chara.Coordinate.Z;
//         var value = 0;

//         if (keyState.isUpKey)
//         {
//             value = -tgZ;
//         }
//         else if (keyState.isDownKey)
//         {
//             value = +tgZ;
//         }

//         if (keyState.isLeftKey)
//         {
//             value += chara.Coordinate.X;
//         }
//         else if (keyState.isRightKey)
//         {
//             value -= chara.Coordinate.X;
//         }

//         return value;
//     }

//     /// <summary>
//     /// ターゲットをソートします
//     /// </summary>
//     /// <param name="targetCount">ソート対象の数</param>
//     /// <param name="targetOrder">ターゲットの順序配列</param>
//     /// <param name="sortValue">ソート用の値配列</param>
//     private void SortTargets(int targetCount, OrderIndexType[] targetOrder, int[] sortValue)
//     {
//         for (var i = 0; i < targetCount - 1; ++i)
//         {
//             for (var i2 = 0; i2 < targetCount - 1; i2++)
//             {
//                 if (i == i2
//                     || targetOrder[i] == OrderIndexType.Disabled
//                     || targetOrder[i2] == OrderIndexType.Disabled)
//                 {
//                     continue;
//                 }

//                 if (sortValue[(int)targetOrder[i]] < sortValue[(int)targetOrder[i2]])
//                 {
//                     (targetOrder[i2], targetOrder[i]) = (targetOrder[i], targetOrder[i2]);
//                 }
//             }
//         }
//     }

//     /// <summary>
//     /// キー入力の状態を取得します
//     /// </summary>
//     /// <returns>各方向キーの入力状態をタプルで返します</returns>
//     private (bool isLeftKey, bool isRightKey, bool isUpKey, bool isDownKey) GetKeyInputs()
//     {
//         if (!Composite.IsSelfControl)
//         {
//             return (false, false, false, false);
//         }

//         return (
//             Pad.KeyLeft.IsPressed,
//             Pad.KeyRight.IsPressed,
//             Pad.KeyUp.IsPressed,
//             Pad.KeyDown.IsPressed
//         );
//     }

//     /// <summary>
//     /// 内野方向を判定します
//     /// </summary>
//     /// <param name="passDirectionX">パスのX方向</param>
//     /// <returns>内野方向の場合はtrue</returns>
//     private bool GetInfieldDirection(DirectionXType passDirectionX)
//     {
//         return MySideIndex == 0
//             ? passDirectionX == DirectionXType.Left
//             : passDirectionX == DirectionXType.Right;
//     }

//     /// <summary>
//     /// 外野のパスターゲットを取得します
//     /// 外野プレイヤーの中から適切なパスターゲットを選択します
//     /// </summary>
//     /// <returns>選択されたパスターゲットのオーダーインデックス</returns>
//     private OrderIndexType GetGaiyaPassTag()
//     {
//         var state = InitializeGaiyaPassTagState();
//         var keyState = GetPassTagKeyState();

//         // 通常パスターゲットの処理
//         var passTarget = GetInitialGaiyaPassTarget(state, keyState);

//         // ダッシュマンパス処理
//         if (!state.isDashmanPass || (passTarget != state.postMan && !keyState.isNeutralKey))
//         {
//             return passTarget;
//         }

//         return GetDashmanGaiyaPassTarget(state, keyState, passTarget);
//     }

//     /// <summary>
//     /// 外野パスターゲットの状態を管理する構造体
//     /// </summary>
//     private struct GaiyaPassTagState
//     {
//         /// <summary>内野方向のX座標</summary>
//         public DirectionXType infieldDirectionX;
//         /// <summary>ダッシュマンへのパス状態</summary>
//         public bool isDashmanPass;
//         /// <summary>パスの方向（X軸）</summary>
//         public DirectionXType passDirectionX;
//         /// <summary>パスの方向（Z軸）</summary>
//         public DirectionZType passDirectionZ;
//         /// <summary>最後のX方向キー入力</summary>
//         public DirectionXType lastXKey;
//         /// <summary>ポストマンの位置</summary>
//         public OrderIndexType postMan;
//     }

//     /// <summary>
//     /// 外野パスターゲットの状態を初期化します
//     /// </summary>
//     /// <returns>初期化された外野パスターゲットの状態</returns>
//     private GaiyaPassTagState InitializeGaiyaPassTagState()
//     {
//         return new GaiyaPassTagState
//         {
//             infieldDirectionX = MySideIndex == 0 ? DirectionXType.Left : DirectionXType.Right,
//             isDashmanPass = MyTeam.Position.DashmanNum > 0,
//             passDirectionX = Coordinate.DirectionX,
//             passDirectionZ = Coordinate.DirectionZ,
//             lastXKey = Input.LastXKey,
//             postMan = MyTeam.Position.Postman
//         };
//     }

//     /// <summary>
//     /// 初期の外野パスターゲットを取得します
//     /// </summary>
//     /// <param name="state">外野パスターゲットの状態</param>
//     /// <param name="keyState">キー入力の状態</param>
//     /// <returns>選択されたパスターゲットのオーダーインデックス</returns>
//     private OrderIndexType GetInitialGaiyaPassTarget(GaiyaPassTagState state, PassTagKeyState keyState)
//     {
//         var isInfieldKey = (MySideIndex == 0 && keyState.isLeftKey) || (MySideIndex == 1 && keyState.isRightKey);
//         var isNoneZKey = !keyState.isUpKey && !keyState.isDownKey;

//         switch (Order.OrderIndex)
//         {
//             case OrderIndexType.Outfield2:
//                 return GetOutfield2PassTarget(state, keyState);
//             case OrderIndexType.Outfield3:
//                 return GetOutfield3PassTarget(state, keyState);
//             case OrderIndexType.Outfield4:
//                 return GetOutfield4PassTarget(state, keyState, isInfieldKey, isNoneZKey);
//             default:
//                 return OrderIndexType.Disabled;
//         }
//     }

//     /// <summary>
//     /// 外野2番手のパスターゲットを取得します
//     /// </summary>
//     private OrderIndexType GetOutfield2PassTarget(GaiyaPassTagState state, PassTagKeyState keyState)
//     {
//         if (state.lastXKey == DirectionXType.Neutral)
//         {
//             if (keyState.isDownKey && !keyState.isLeftKey && !keyState.isRightKey)
//             {
//                 return OrderIndexType.Outfield3;
//             }

//             return state.passDirectionX == state.infieldDirectionX
//                 ? state.postMan
//                 : OrderIndexType.Outfield4;
//         }

//         return state.lastXKey == state.infieldDirectionX
//             ? state.postMan
//             : OrderIndexType.Outfield4;
//     }

//     /// <summary>
//     /// 外野3番手のパスターゲットを取得します
//     /// </summary>
//     private OrderIndexType GetOutfield3PassTarget(GaiyaPassTagState state, PassTagKeyState keyState)
//     {
//         if (state.lastXKey == DirectionXType.Neutral)
//         {
//             if (keyState.isUpKey && !keyState.isLeftKey && !keyState.isRightKey)
//             {
//                 return OrderIndexType.Outfield2;
//             }

//             return state.passDirectionX == state.infieldDirectionX
//                 ? state.postMan
//                 : OrderIndexType.Outfield4;
//         }

//         return state.lastXKey == state.infieldDirectionX
//             ? state.postMan
//             : OrderIndexType.Outfield4;
//     }

//     /// <summary>
//     /// 外野4番手のパスターゲットを取得します
//     /// </summary>
//     private OrderIndexType GetOutfield4PassTarget(GaiyaPassTagState state, PassTagKeyState keyState, bool isInfieldKey, bool isNoneZKey)
//     {
//         if (isInfieldKey && isNoneZKey)
//         {
//             return state.postMan;
//         }

//         return GetOutfield4DirectionalTarget(state);
//     }

//     /// <summary>
//     /// 外野4番手の方向に基づいたパスターゲットを取得します
//     /// </summary>
//     private OrderIndexType GetOutfield4DirectionalTarget(GaiyaPassTagState state)
//     {
//         switch (state.passDirectionZ)
//         {
//             case DirectionZType.Backward:
//                 return OrderIndexType.Outfield2;
//             case DirectionZType.Forward:
//                 return OrderIndexType.Outfield3;
//             default:
//                 var distO2 = Math.Abs(Coordinate.Z - Defines.DBCRT_BL);
//                 var distO3 = Math.Abs(Coordinate.Z - Defines.DBCRT_FL);
//                 return distO2 < distO3 ? OrderIndexType.Outfield2 : OrderIndexType.Outfield3;
//         }
//     }

//     /// <summary>
//     /// ダッシュマン用の外野パスターゲットを取得します
//     /// </summary>
//     private OrderIndexType GetDashmanGaiyaPassTarget(GaiyaPassTagState state, PassTagKeyState keyState, OrderIndexType defaultTarget)
//     {
//         var dashmanTargets = PrepareDashmanTargets(keyState);
//         if (dashmanTargets.isNoneTarget)
//         {
//             return defaultTarget;
//         }

//         return GetSortedDashmanTarget(dashmanTargets, keyState);
//     }

//     /// <summary>
//     /// ダッシュマンターゲットの情報を管理する構造体
//     /// </summary>
//     private struct DashmanTargetInfo
//     {
//         public bool isNoneTarget;
//         public int[] targetX;
//         public int[] targetDist;
//         public enNaiyaTag[] isSelectTarget;
//         public OrderIndexType[] targetOrder;
//         public int[] sortValue;
//     }

//     /// <summary>
//     /// ダッシュマンターゲットの準備を行います
//     /// </summary>
//     private DashmanTargetInfo PrepareDashmanTargets(PassTagKeyState keyState)
//     {
//         TmpStateManager.Instance.TmpState.Clear();
//         var info = new DashmanTargetInfo
//         {
//             isNoneTarget = true,
//             targetX = TmpStateManager.Instance.TmpState.targetX,
//             targetDist = TmpStateManager.Instance.TmpState.targetDist,
//             isSelectTarget = TmpStateManager.Instance.TmpState.isSelectTarget,
//             targetOrder = TmpStateManager.Instance.TmpState.targetOrder,
//             sortValue = TmpStateManager.Instance.TmpState.sortValue
//         };

//         for (var order = 0; order < Defines.DBMEMBER_INF; ++order)
//         {
//             var chara = CharaBehaviorManager.Instance.GetOrderChara(MySideIndex, order);

//             if (IsDashmanTargetValid(chara, order))
//             {
//                 info.isSelectTarget[order] = enNaiyaTag.TGOK;
//                 info.isNoneTarget = false;
//             }
//             else
//             {
//                 info.isSelectTarget[order] = enNaiyaTag.TGNG;
//             }

//             CalculateTargetDistances(chara, order, info);
//         }

//         return info;
//     }

//     /// <summary>
//     /// ダッシュマンターゲットが有効かどうかを判定します
//     /// </summary>
//     private bool IsDashmanTargetValid(CharaBehavior chara, int order)
//     {
//         return chara.Composite.IsDashman
//             && (OrderIndexType)order != MyOrderIndex
//             && !IsCheckLandEnPos(order);
//     }

//     /// <summary>
//     /// ターゲットまでの距離を計算します
//     /// </summary>
//     private void CalculateTargetDistances(CharaBehavior chara, int order, DashmanTargetInfo info)
//     {
//         info.targetX[order] = chara.Composite.LeftCourtX;
//         var targetZ = Math.Abs(chara.Coordinate.Z - Coordinate.Z);
//         info.targetDist[order] = Defines.Hypot(info.targetX[order], targetZ);
//     }

//     /// <summary>
//     /// ソートされたダッシュマンターゲットを取得します
//     /// </summary>
//     private OrderIndexType GetSortedDashmanTarget(DashmanTargetInfo info, PassTagKeyState keyState)
//     {
//         InitializeTargetOrder(info.targetOrder);
//         var targetCount = PrepareTargetsForDashmanSort(info, keyState);
//         SortDashmanTargets(targetCount, info);
//         return info.targetOrder[0];
//     }

//     /// <summary>
//     /// ターゲットの順序を初期化します
//     /// </summary>
//     private void InitializeTargetOrder(OrderIndexType[] targetOrder)
//     {
//         for (var order = 0; order < Defines.DBMEMBER_INF; ++order)
//         {
//             targetOrder[order] = OrderIndexType.Disabled;
//         }
//     }

//     /// <summary>
//     /// ダッシュマンターゲットのソート準備を行います
//     /// </summary>
//     private int PrepareTargetsForDashmanSort(DashmanTargetInfo info, PassTagKeyState keyState)
//     {
//         var targetCount = 0;

//         for (var order = 0; order < Defines.DBMEMBER_INF; ++order)
//         {
//             info.sortValue[order] = 0;

//             if (info.isSelectTarget[order] != enNaiyaTag.TGOK)
//             {
//                 continue;
//             }

//             if (keyState.isNeutralKey)
//             {
//                 info.sortValue[order] = -info.targetX[order];
//             }
//             else
//             {
//                 CalculateDashmanSortValue(order, info, keyState);
//             }

//             info.targetOrder[targetCount++] = (OrderIndexType)order;
//         }

//         return targetCount;
//     }

//     /// <summary>
//     /// ダッシュマンターゲットのソート値を計算します
//     /// </summary>
//     private void CalculateDashmanSortValue(int order, DashmanTargetInfo info, PassTagKeyState keyState)
//     {
//         var chara = CharaBehaviorManager.Instance.GetOrderChara(MySideIndex, order);
//         var tgZ = chara.Dashman.TargetZ;

//         if (keyState.isUpKey)
//         {
//             info.sortValue[order] = -tgZ;
//         }
//         else if (keyState.isDownKey)
//         {
//             info.sortValue[order] = +tgZ;
//         }

//         if (keyState.isLeftKey)
//         {
//             info.sortValue[order] += chara.Coordinate.X;
//         }
//         else if (keyState.isRightKey)
//         {
//             info.sortValue[order] -= chara.Coordinate.X;
//         }
//     }

//     /// <summary>
//     /// ダッシュマンターゲットをソートします
//     /// </summary>
//     private void SortDashmanTargets(int targetCount, DashmanTargetInfo info)
//     {
//         for (var i = 0; i < targetCount - 1; ++i)
//         {
//             for (var i2 = 0; i2 < targetCount - 1; i2++)
//             {
//                 if (ShouldSkipSort(i, i2, info.targetOrder))
//                 {
//                     continue;
//                 }

//                 if (ShouldSwapTargets(i, i2, info))
//                 {
//                     (info.targetOrder[i2], info.targetOrder[i]) = (info.targetOrder[i], info.targetOrder[i2]);
//                 }
//             }
//         }
//     }

//     /// <summary>
//     /// ソートをスキップすべきかどうかを判定します
//     /// </summary>
//     private bool ShouldSkipSort(int i, int i2, OrderIndexType[] targetOrder)
//     {
//         return i == i2
//             || targetOrder[i] == OrderIndexType.Disabled
//             || targetOrder[i2] == OrderIndexType.Disabled;
//     }

//     /// <summary>
//     /// ターゲットを交換すべきかどうかを判定します
//     /// </summary>
//     private bool ShouldSwapTargets(int i, int i2, DashmanTargetInfo info)
//     {
//         var orderA = (int)info.targetOrder[i];
//         var orderB = (int)info.targetOrder[i2];
//         var dist = Math.Abs(info.sortValue[orderA] - info.sortValue[orderB]);

//         if (dist <= Defines.Percent)
//         {
//             return info.targetDist[orderA] < info.targetDist[orderB];
//         }

//         return info.sortValue[orderA] < info.sortValue[orderB];
//     }

//     /// <summary>
//     /// COMダッシュマンのパスターゲットを設定します
//     /// 現在のダッシュマンの状態に基づいて最適なパスターゲットを決定します
//     /// </summary>
//     /// <param name="nowOKonly_f">現在パス可能な対象のみを選択するかどうか</param>
//     /// <returns>選択されたパスターゲットの順序インデックス</returns>
//     private OrderIndexType GetCOMDMPassTag(bool nowOKonly_f)
//     {
//         TmpStateManager.Instance.TmpState.Clear();
//         var targetDist = TmpStateManager.Instance.TmpState.targetDist;
//         var isSelectTarget = TmpStateManager.Instance.TmpState.isSelectTarget;
//         var targetOrder = TmpStateManager.Instance.TmpState.targetOrder;
//         var sortValue = TmpStateManager.Instance.TmpState.sortValue;

//         bool topord_f = true; // 最上位オーダーフラグ
//         bool bottom_f = true; // 最後尾フラグ

//         bool Notag_f = true; // 完全にパスタゲが居ない

//         // パスのタイプ
//         var patype = (enCOMDMPassType)MyTeam.AiPattern.GetPlanPattern(AiPlanType.comDMPaTag);

//         // 触ってない人だけ
//         bool NTOnly_f = MyTeam.AiPattern.GetPlanPattern(AiPlanType.comDMPaNum) == 0;

//         // 内野全員との距離を取る
//         for (var order = 0; order < Defines.DBMEMBER_INF; ++order)
//         {
//             if ((OrderIndexType)order == MyOrderIndex)
//             {
//                 continue;
//             }

//             var chara = CharaBehaviorManager.Instance.GetOrderChara(MySideIndex, order);
//             targetDist[order] = Coordinate.DistanceXZ(chara.Coordinate);
//         }

//         // ダッシュマンパススピード
//         int paspd = GetSettingPass(SettingPassType.DMPaSpd);

//         // 内野全員との角度を取る
//         for (var order = 0; order < Defines.DBMEMBER_INF; ++order)
//         {
//             if (order == (int)MyOrderIndex) // 自分
//             {
//                 isSelectTarget[order] = enNaiyaTag.TGNG;
//                 continue;
//             }

//             var chara = CharaBehaviorManager.Instance.GetOrderChara(MySideIndex, order);

//             if (!chara.Composite.IsDashman) // 現在ダッシュマンでない
//             {
//                 isSelectTarget[order] = enNaiyaTag.TGNG;
//                 continue;
//             }

//             const int YOYUU = 10; // 投げモーションと相手が離れていく分
//             int reachtime = (int)(targetDist[order] / paspd + YOYUU);


//             if (!chara.Composite.IsCom // マニュアル以外
//                 && chara.Dashman.EnabledPassCount.Value < reachtime) // おそらくとどかない
//             {
//                 isSelectTarget[order] = enNaiyaTag.TGNG;
//                 continue;
//             }

//             if (chara.Coordinate.VelocityY < 0) // 下降に入ってる
//             {
//                 isSelectTarget[order] = enNaiyaTag.TGNG;
//                 continue;
//             }

//             if (!NTOnly_f // 一人一回ではない
//                 || !MyTeam.AiAction((OrderIndexType)order).BallTouchedF) // まだ触ってない
//             {
//                 isSelectTarget[order] = enNaiyaTag.TGOK;
//             }
//             else
//             {
//                 isSelectTarget[order] = enNaiyaTag.TGNG;
//             }

//             if (isSelectTarget[order] != enNaiyaTag.TGNG)
//             {
//                 if (chara.Composite.LeftCourtX < Composite.LeftCourtX) // 後ろに人がいる
//                 {
//                     bottom_f = false; // 最後尾ではない
//                 }

//                 if (MyOrderIndex > (OrderIndexType)order) // 上位に人がいる
//                 {
//                     topord_f = false; // 先頭ではない
//                 }

//                 Notag_f = false;
//             }
//         }

//         if (Notag_f) // タゲが居ない
//         {
//             return OrderIndexType.Disabled;
//         }

//         var f = 0;

//         for (var order = 0; order < Defines.DBMEMBER_INF; ++order)
//         {
//             sortValue[order] = 0; // 初期化

//             if (isSelectTarget[order] != enNaiyaTag.TGOK)
//             {
//                 targetOrder[f] = OrderIndexType.Disabled;
//                 continue;
//             }

//             // 値が少ないほど優先
//             switch (patype)
//             {
//                 case enCOMDMPassType.cpmUpOrder: // 一つオーダー上位（上位なら最下位）
//                     if (topord_f) // 上位に人がいない
//                     {
//                         sortValue[order] = -order; // 最下位オーダー
//                     }
//                     else
//                     {
//                         sortValue[order] = order > (int)MyOrderIndex // タゲの方が前にいる
//                             ? Defines.DBMEMBER_INF
//                             : (int)MyOrderIndex - order; // 自分より上位で一番近い人
//                     }
//                     break;
//                 case enCOMDMPassType.cpmNear: // 一番近い人
//                     sortValue[order] = targetDist[order];
//                     break;
//                 case enCOMDMPassType.cpmFar: // 一番Ｚ軸が遠い人
//                     sortValue[order] = -Math.Abs(CharaBehaviorManager.Instance.GetOrderChara(MySideIndex, order).Coordinate.Z);
//                     break;
//                 case enCOMDMPassType.cpmRear: // 一つ後ろの人(居なければ一番前の人)
//                     if (bottom_f) // 一番後ろ
//                     {
//                         sortValue[order] = -(int)CharaBehaviorManager.Instance.GetOrderChara(MySideIndex, order).Composite.LeftCourtX; // 一番前にいる人
//                     }
//                     else
//                     {
//                         var chara = CharaBehaviorManager.Instance.GetOrderChara(MySideIndex, order);
//                         sortValue[order] = chara.Composite.LeftCourtX > Composite.LeftCourtX // タゲの方が前にいる
//                             ? Defines.DBCRT_W
//                             : (int)Composite.LeftCourtX - (int)chara.Composite.LeftCourtX; // 自分より後ろで一番近い人
//                     }
//                     break;
//             }

//             targetOrder[f++] = (OrderIndexType)order;
//         }

//         // ソート
//         for (var i = 0; i < Defines.DBMEMBER_INF - 1; ++i)
//         {
//             for (var i2 = 0; i2 < Defines.DBMEMBER_INF - 1; i2++)
//             {
//                 if (i == i2) continue; // 同じ

//                 if (targetOrder[i] != OrderIndexType.Disabled && targetOrder[i2] != OrderIndexType.Disabled)
//                 {
//                     if (sortValue[(int)targetOrder[i]] < sortValue[(int)targetOrder[i2]]) // 小さい方優先
//                     {
//                         (targetOrder[i2], targetOrder[i]) = (targetOrder[i], targetOrder[i2]);
//                     }
//                 }
//             }
//         }

//         // ソート１位
//         var res = targetOrder[0];

//         // 現状ＯＫなやつしか駄目な場合
//         if (nowOKonly_f)
//         {
//             if (isSelectTarget[(int)res] != enNaiyaTag.TGOK)
//             {
//                 res = OrderIndexType.Disabled;
//             }
//         }

//         return res;
//     }

//     //シュートタゲセット
//     //isIgnoreDirection:向きを反映しない（強制的に内野タゲ）
//     private OrderIndexType GetShootTarget(int angle, bool isIgnoreDirection)
//     {
//         TmpStateManager.Instance.TmpState.Clear();
//         var targetX = TmpStateManager.Instance.TmpState.targetX;
//         var targetY = TmpStateManager.Instance.TmpState.targetY;
//         var targetDist = TmpStateManager.Instance.TmpState.targetDist;
//         var isSelectTarget = TmpStateManager.Instance.TmpState.isSelectTarget;

//         //敵内野全員との角度を取る
//         for (var order = 0; order < Defines.DBMEMBER_INF; ++order)
//         {
//             var enemyChara = CharaBehaviorManager.Instance.GetOrderChara(EnemySideIndex, order);

//             if (enemyChara.Composite.IsEnableShootTarget == false)
//             {
//                 continue;
//             }

//             isSelectTarget[order] = enNaiyaTag.TGOK;
//             targetX[order] = enemyChara.Coordinate.X - Coordinate.X;
//             var targetZ = enemyChara.Coordinate.Z - Coordinate.Z;
//             targetY[order] = enemyChara.Coordinate.Y - Coordinate.Y;
//             targetDist[order] = Defines.Hypot(targetX[order], targetZ); //距離

//             // 角度を無視しない場合は角度チェックを行う
//             if (isIgnoreDirection == false)
//             {
//                 const int angle12 = 12;
//                 var targetAngle = GetTagAgl2(targetX[order], targetZ); //新12時法

//                 //メインアングルチェック
//                 if (targetAngle != (angle + 0) % angle12
//                     && targetAngle != (angle + 1) % angle12
//                     && targetAngle != (angle + 2) % angle12
//                     && targetAngle != (angle + 3) % angle12)
//                 {
//                     isSelectTarget[order] = enNaiyaTag.TGNG;
//                 }
//             }
//         }

//         var targetOrderIndex = OrderIndexType.Disabled;
//         var subTargetOrderIndex = OrderIndexType.Disabled;
//         var nearTargetDist = 0;
//         var subNearTargetDist = 0;

//         //距離による優先順位
//         for (var order = 0; order < Defines.DBMEMBER_INF; ++order)
//         {
//             if (isSelectTarget[order] == enNaiyaTag.TGNG)
//             {
//                 continue;
//             }

//             // 第二候補
//             if (subNearTargetDist == 0
//                 || targetDist[order] < subNearTargetDist)
//             {
//                 subTargetOrderIndex = (OrderIndexType)order;
//                 subNearTargetDist = targetDist[order];
//             }

//             // 近すぎ
//             if (targetDist[order] < Defines.SHTAG_NEARDIST)
//             {
//                 continue;
//             }

//             // Y離れすぎ
//             if (Defines.PercentageOf(targetDist[order], Defines.SHTAG_FARDIST_Y) < Math.Abs(targetY[order]))
//             {
//                 continue;
//             }

//             if (nearTargetDist == 0
//                 || targetDist[order] < nearTargetDist)
//             {
//                 targetOrderIndex = (OrderIndexType)order;
//                 nearTargetDist = targetDist[order];
//             }
//         }

//         return targetOrderIndex == OrderIndexType.Disabled
//             ? subTargetOrderIndex
//             : targetOrderIndex;
//     }

//     /// <summary>
//     /// シュートターゲットの存在をチェックします
//     /// 有効なシュートターゲットが存在するかを確認します
//     /// </summary>
//     /// <returns>シュートターゲットが存在しない場合はtrue</returns>
//     private bool IsShTag()
//     {
//         return Ball.Main.ShotTargetSide != MySideIndex
//                || Ball.Main.ShotTargetOrder == OrderIndexType.Disabled;
//     }

//     /// <summary>
//     /// シュートターゲットの設定を行います
//     /// 現在の角度に基づいてシュートターゲットを設定します
//     /// </summary>
//     /// <param name="isIgnoreDirection">向きを無視するかどうか</param>
//     private void SetShTagFromMyShootAngle12(bool isIgnoreDirection)
//     {
//         Ball.CallChangeShootTarget(EnemySideIndex, GetShootTarget(Shoot.Angle12, isIgnoreDirection));
//     }
// }
 */