using isdf;

namespace db;

public partial class CharaBehavior : BaseBehavior
{
    private int EnemySideIndex => MySideIndex == 0 ? 1 : 0;
    private int MyMemberIndex => RawState.Index.MemberIndex;

    private CharaState RawState => CharaStateManager.Instance.Get(MySideIndex, MyMemberIndex);
    private CharaState[] MySideOrders => CharaStateManager.Instance.GetSideOrders(MySideIndex);
    private CharaState[] EnemySideOrders => CharaStateManager.Instance.GetSideOrders(EnemySideIndex);
    private IBallStateGetter BallState => BallStateManager.Instance.BallState;
    private IRefereeStateGetter RefereeState => RefereeStateManager.Instance.RefereeState;
    private TeamBehavior MyTeam => TeamBehaviorManager.Instance.Get(MySideIndex);
    private ITeamAiActionStateGetter MyTeamAiAction => MyTeam.ComAction(MyMemberIndex);
    private TeamBehavior EnemyTeam => TeamBehaviorManager.Instance.Get(EnemySideIndex);
    private IPad Pad => PadBehaviorManager.Instance.Get(MySideIndex).Pad;
    private readonly CharaCompositeState _rawComposite = new();
    
    /// <summary>
    /// Getアクセスインターフェイス
    /// </summary>

    public int MySideIndex => RawState.Index.SideIndex;

    public OrderIndexType MyOrderIndex => RawState.Order.OrderIndex;
    public ICharaIndexStateGetter Index => RawState.Index;
    public ICharaOrderStateGetter Order => RawState.Order;
    public ICharaInputStateGetter Input => RawState.Input;
    public ICharaLiveStateGetter Live => RawState.Live;
    public ICoordinateStateGetter Coordinate => RawState.Coordinate;
    public ICharaMotionStateGetter Motion => RawState.Motion;
    public ICharaAutoStateGetter Auto => RawState.Auto;
    public ICharaAutoStateGetter NextAuto => RawState.NextAuto;
    public ICharaComAutoStateGetter ComAuto => RawState.ComAuto;
    public ICharaGouseiStateGetter Gousei => RawState.Gousei;
    public ICharaDamageStateGetter Damage => RawState.Damage;
    public ICharaLevelStateGetter Level => RawState.Level;
    public ICharaDistanceStateGetter Distance => RawState.Distance;
    public ICharaMoveStateGetter Move => RawState.Move;
    public ICharaAirStateGetter Air => RawState.Air;
    public ICharaViewStateGetter View => RawState.View;
    public ICharaCatchStateGetter Catch => RawState.Catch;
    public ICharaShootStateGetter Shoot => RawState.Shoot;
    public ICharaCourtStateGetter Court => RawState.Court;
    public ICharaPassStateGetter Pass => RawState.Pass;
    public ICharaDashmanStateGetter Dashman => RawState.Dashman;
    public ICharaComOnlyStateGetter ComOnly => RawState.ComOnly;
    public ICharaBallEffectStateGetter BallEffect => RawState.BallEffect;
    public ICompositeStateGetter Composite => _rawComposite;

    /// <summary>
    /// Setアクセスインターフェイス
    /// </summary>

    private ICharaIndexStateSetter IndexSet => RawState.Index;
    private ICharaOrderStateSetter OrderSet => RawState.Order;
    private ICharaInputStateSetter InputSet => RawState.Input;
    private ICharaLiveStateSetter LiveSet => RawState.Live;
    private ICoordinateStateSetter CoordinateSet => RawState.Coordinate;
    private ICharaMotionStateSetter MotionSet => RawState.Motion;
    private ICharaAutoStateSetter AutoSet => RawState.Auto;
    private ICharaAutoStateSetter NextAutoSet => RawState.NextAuto;
    private ICharaComAutoStateSetter ComAutoSet => RawState.ComAuto;
    private ICharaGouseiStateSetter GouseiSet => RawState.Gousei;
    private ICharaDamageStateSetter DamageSet => RawState.Damage;
    private ICharaLevelStateSetter LevelSet => RawState.Level;
    private ICharaDistanceStateSetter DistanceSet => RawState.Distance;
    private ICharaMoveStateSetter MoveSet => RawState.Move;
    private ICharaAirStateSetter AirSet => RawState.Air;
    private ICharaViewStateSetter ViewSet => RawState.View;
    private ICharaCatchStateSetter CatchSet => RawState.Catch;
    private ICharaShootStateSetter ShootSet => RawState.Shoot;
    private ICharaCourtStateSetter CourtSet => RawState.Court;
    private ICharaPassStateSetter PassSet => RawState.Pass;
    private ICharaDashmanStateSetter DashmanSet => RawState.Dashman;
    private ICharaComOnlyStateSetter ComOnlySet => RawState.ComOnly;
    private ICharaBallEffectStateSetter BallEffectSet => RawState.BallEffect;

    public CharaBehavior()
    {
    }
}
