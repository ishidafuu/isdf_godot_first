using isdf;

namespace db;

public partial class CharaBehavior
{
    public int MySideIndex => MyState.Index.SideIndex;
    private int EnemySideIndex => MySideIndex == 0 ? 1 : 0;
    private int MemberIndex => MyState.Index.MemberIndex;
    public OrderIndexType MyOrderIndex => MyState.Order.OrderIndex;

    private CharaState MyState => CharaStateManager.Instance.Get(MySideIndex, MemberIndex);

    // private CharaState[] MySideCharas => CharaStateManager.Instance.GetSideCharas(MySideIndex);
    private CharaState[] MySideOrders => CharaStateManager.Instance.GetSideOrders(MySideIndex);

    // private CharaState[] EnemySideCharas => CharaStateManager.Instance.GetSideCharas(EnemySideIndex);
    private CharaState[] EnemySideOrders => CharaStateManager.Instance.GetSideOrders(EnemySideIndex);
    private BallState BallState => BallStateManager.Instance.BallState;
    private RefereeState RefereeState => RefereeStateManager.Instance.RefereeState;
    private TeamState MyTeamState => TeamStateManager.Instance.Get(MySideIndex);
    private ComState MyComState => ComStateManager.Instance.Get(MySideIndex);
    private TeamState EnemyTeamState => TeamStateManager.Instance.Get(EnemySideIndex);
    private TeamBehavior MyTeamBehavior => TeamBehaviorManager.Instance.Get(MySideIndex);
    private IPad MyPad => PadBehaviorManager.Instance.Get(MySideIndex).Pad;

    /// <summary>
    /// MyState
    /// Getアクセスインターフェイス
    /// </summary>
    public ICharaIndexStateGetter MyIndex => MyState.Index;
    public ICharaOrderStateGetter MyOrder => MyState.Order;
    public ICharaInputStateGetter MyInput => MyState.Input;
    public ICharaLiveStateGetter MyLive => MyState.Live;
    public ICoordinateState MyCoordinate => MyState.Coordinate;
    public ICharaMotionStateGetter MyMotion => MyState.Motion;
    public ICharaAutoStateGetter MyAuto => MyState.Auto;
    public ICharaAutoStateGetter MyNextAuto => MyState.NextAuto;
    public ICharaComAutoStateGetter MyComAuto => MyState.ComAuto;
    public ICharaGouseiStateGetter MyGousei => MyState.Gousei;
    public ICharaDamageStateGetter MyDamage => MyState.Damage;
    public ICharaLevelStateGetter MyLevel => MyState.Level;
    public ICharaDistanceStateGetter MyDistance => MyState.Distance;
    public ICharaMoveStateGetter MyMove => MyState.Move;
    public ICharaAirStateGetter MyAir => MyState.Air;
    public ICharaViewStateGetter MyView => MyState.View;
    public ICharaCatchStateGetter MyCatch => MyState.Catch;
    public ICharaShootStateGetter MyShoot => MyState.Shoot;
    public ICharaCourtStateGetter MyCourt => MyState.Court;
    public ICharaPassStateGetter MyPass => MyState.Pass;
    public ICharaDashmanStateGetter MyDashman => MyState.Dashman;
    public ICharaComOnlyStateGetter MyComOnly => MyState.ComOnly;
    public ICharaBallEffectStateGetter MyBallEffect => MyState.BallEffect;

    public CharaBehavior()
    {
    }
}