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
    public ICharaIndexState MyIndex => MyState.Index;
    public ICharaOrderState MyOrder => MyState.Order;
    public ICharaInputState MyInput => MyState.Input;
    public ICharaLiveState MyLive => MyState.Live;
    public ICoordinateState MyCoordinate => MyState.Coordinate;
    public ICharaMotionState MyMotion => MyState.Motion;
    public ICharaAutoState MyAuto => MyState.Auto;
    public ICharaAutoState MyNextAuto => MyState.NextAuto;
    public ICharaComAutoState MyComAuto => MyState.ComAuto;
    public ICharaGouseiState MyGousei => MyState.Gousei;
    public ICharaDamageState MyDamage => MyState.Damage;
    public ICharaLevelState MyLevel => MyState.Level;
    public ICharaDistanceState MyDistance => MyState.Distance;
    public ICharaMoveState MyMove => MyState.Move;
    public ICharaAirState MyAir => MyState.Air;
    public ICharaViewState MyView => MyState.View;
    public ICharaCatchState MyCatch => MyState.Catch;
    public ICharaShootState MyShoot => MyState.Shoot;
    public ICharaCourtState MyCourt => MyState.Court;
    public ICharaPassState MyPass => MyState.Pass;
    public ICharaDashmanState MyDashman => MyState.Dashman;
    public ICharaComOnlyState MyComOnly => MyState.ComOnly;
    public ICharaBallEffectState MyBallEffect => MyState.BallEffect;

    public CharaBehavior()
    {
    }
}