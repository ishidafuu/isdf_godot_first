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
    /// MyState ショートカット
    /// </summary>
    public CharaIndexState MyIndex => MyState.Index;

    public CharaOrderState MyOrder => MyState.Order;
    public CharaInputState MyInput => MyState.Input;
    public CharaLiveState MyLive => MyState.Live;
    public CoordinateState MyCoordinate => MyState.Coordinate;
    public CharaMotionState MyMotion => MyState.Motion;
    public CharaAutoState MyAuto => MyState.Auto;
    public CharaAutoState MyNextAuto => MyState.NextAuto;
    public CharaComAutoState MyComAuto => MyState.ComAuto;
    public CharaGouseiState MyGousei => MyState.Gousei;
    public CharaDamageState MyDamage => MyState.Damage;
    public CharaLevelState MyLevel => MyState.Level;
    public CharaDistanceState MyDistance => MyState.Distance;
    public CharaMoveState MyMove => MyState.Move;
    public CharaAirState MyAir => MyState.Air;
    public CharaViewState MyView => MyState.View;
    public CharaCatchState MyCatch => MyState.Catch;
    public CharaShootState MyShoot => MyState.Shoot;
    // public CharaDodgeState MyDodge => MyState.Dodge;
    public CharaCourtState MyCourt => MyState.Court;
    public CharaPassState MyPass => MyState.Pass;
    public CharaDashmanState MyDashman => MyState.Dashman;
    public CharaComOnlyState MyComOnly => MyState.ComOnly;
    public CharaBallEffectState MyBallEffect => MyState.BallEffect;

    public CharaBehavior()
    {
    }
}