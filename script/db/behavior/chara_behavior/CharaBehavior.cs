using isdf;

namespace db;

public partial class CharaBehavior
{
    private int EnemySideIndex => MySideIndex == 0 ? 1 : 0;
    private int MyMemberIndex => RawState2.Index.MemberIndex;

    private CharaState RawState2 => CharaStateManager.Instance.Get(MySideIndex, MyMemberIndex);
    private CharaState[] MySideOrders => CharaStateManager.Instance.GetSideOrders(MySideIndex);
    private CharaState[] EnemySideOrders => CharaStateManager.Instance.GetSideOrders(EnemySideIndex);
    private BallState BallState => BallStateManager.Instance.BallState;
    private RefereeState RefereeState => RefereeStateManager.Instance.RefereeState;
    private TeamState MyTeamState => TeamStateManager.Instance.Get(MySideIndex);
    private ComState MyComState => ComStateManager.Instance.Get(MySideIndex);
    private TeamState EnemyTeamState => TeamStateManager.Instance.Get(EnemySideIndex);
    private TeamBehavior MyTeamBehavior => TeamBehaviorManager.Instance.Get(MySideIndex);
    private IPad MyPad => PadBehaviorManager.Instance.Get(MySideIndex).Pad;

    /// <summary>
    /// Getアクセスインターフェイス
    /// </summary>

    public int MySideIndex => RawState2.Index.SideIndex;

    public OrderIndexType MyOrderIndex => RawState2.Order.OrderIndex;
    public ICharaIndexStateGetter Index => RawState2.Index;
    public ICharaOrderStateGetter Order => RawState2.Order;
    public ICharaInputStateGetter Input => RawState2.Input;
    public ICharaLiveStateGetter Live => RawState2.Live;
    public ICoordinateStateGetter Coordinate => RawState2.Coordinate;
    public ICharaMotionStateGetter Motion => RawState2.Motion;
    public ICharaAutoStateGetter Auto => RawState2.Auto;
    public ICharaAutoStateGetter NextAuto => RawState2.NextAuto;
    public ICharaComAutoStateGetter ComAuto => RawState2.ComAuto;
    public ICharaGouseiStateGetter Gousei => RawState2.Gousei;
    public ICharaDamageStateGetter Damage => RawState2.Damage;
    public ICharaLevelStateGetter Level => RawState2.Level;
    public ICharaDistanceStateGetter Distance => RawState2.Distance;
    public ICharaMoveStateGetter Move => RawState2.Move;
    public ICharaAirStateGetter Air => RawState2.Air;
    public ICharaViewStateGetter View => RawState2.View;
    public ICharaCatchStateGetter Catch => RawState2.Catch;
    public ICharaShootStateGetter Shoot => RawState2.Shoot;
    public ICharaCourtStateGetter Court => RawState2.Court;
    public ICharaPassStateGetter Pass => RawState2.Pass;
    public ICharaDashmanStateGetter Dashman => RawState2.Dashman;
    public ICharaComOnlyStateGetter ComOnly => RawState2.ComOnly;
    public ICharaBallEffectStateGetter BallEffect => RawState2.BallEffect;

    /// <summary>
    /// Setアクセスインターフェイス
    /// </summary>

    private ICharaIndexStateSetter SetIndex => RawState2.Index;

    private ICharaOrderStateSetter SetOrder => RawState2.Order;
    private ICharaInputStateSetter SetInput => RawState2.Input;
    private ICharaLiveStateSetter SetLive => RawState2.Live;
    private ICoordinateStateSetter SetCoordinate => RawState2.Coordinate;
    private ICharaMotionStateSetter SetMotion => RawState2.Motion;
    private ICharaAutoStateSetter SetAuto => RawState2.Auto;
    private ICharaAutoStateSetter SetNextAuto => RawState2.NextAuto;
    private ICharaComAutoStateSetter SetComAuto => RawState2.ComAuto;
    private ICharaGouseiStateSetter SetGousei => RawState2.Gousei;
    private ICharaDamageStateSetter SetDamage => RawState2.Damage;
    private ICharaLevelStateSetter SetLevel => RawState2.Level;
    private ICharaDistanceStateSetter SetDistance => RawState2.Distance;
    private ICharaMoveStateSetter SetMove => RawState2.Move;
    private ICharaAirStateSetter SetAir => RawState2.Air;
    private ICharaViewStateSetter SetView => RawState2.View;
    private ICharaCatchStateSetter SetCatch => RawState2.Catch;
    private ICharaShootStateSetter SetShoot => RawState2.Shoot;
    private ICharaCourtStateSetter SetCourt => RawState2.Court;
    private ICharaPassStateSetter SetPass => RawState2.Pass;
    private ICharaDashmanStateSetter SetDashman => RawState2.Dashman;
    private ICharaComOnlyStateSetter SetComOnly => RawState2.ComOnly;
    private ICharaBallEffectStateSetter SetBallEffect => RawState2.BallEffect;

    public CharaBehavior()
    {
    }
}