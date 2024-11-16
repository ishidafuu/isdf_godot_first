using isdf;

namespace db;

public partial class CharaBehavior : BaseBehavior
{
    private int EnemySideIndex => MySideIndex == 0 ? 1 : 0;
    private int MyMemberIndex => RawState2.Index.MemberIndex;

    private CharaState RawState2 => CharaStateManager.Instance.Get(MySideIndex, MyMemberIndex);
    private CharaState[] MySideOrders => CharaStateManager.Instance.GetSideOrders(MySideIndex);
    private CharaState[] EnemySideOrders => CharaStateManager.Instance.GetSideOrders(EnemySideIndex);
    private BallState BallState => BallStateManager.Instance.BallState;
    private RefereeState RefereeState => RefereeStateManager.Instance.RefereeState;
    private TeamBehavior MyTeam => TeamBehaviorManager.Instance.Get(MySideIndex);
    private TeamBehavior EnemyTeam => TeamBehaviorManager.Instance.Get(EnemySideIndex);
    private IPad Pad => PadBehaviorManager.Instance.Get(MySideIndex).Pad;

    
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

    private ICharaIndexStateSetter IndexSet => RawState2.Index;

    private ICharaOrderStateSetter OrderSet => RawState2.Order;
    private ICharaInputStateSetter InputSet => RawState2.Input;
    private ICharaLiveStateSetter LiveSet => RawState2.Live;
    private ICoordinateStateSetter CoordinateSet => RawState2.Coordinate;
    private ICharaMotionStateSetter MotionSet => RawState2.Motion;
    private ICharaAutoStateSetter AutoSet => RawState2.Auto;
    private ICharaAutoStateSetter NextAutoSet => RawState2.NextAuto;
    private ICharaComAutoStateSetter ComAutoSet => RawState2.ComAuto;
    private ICharaGouseiStateSetter GouseiSet => RawState2.Gousei;
    private ICharaDamageStateSetter DamageSet => RawState2.Damage;
    private ICharaLevelStateSetter LevelSet => RawState2.Level;
    private ICharaDistanceStateSetter DistanceSet => RawState2.Distance;
    private ICharaMoveStateSetter MoveSet => RawState2.Move;
    private ICharaAirStateSetter AirSet => RawState2.Air;
    private ICharaViewStateSetter ViewSet => RawState2.View;
    private ICharaCatchStateSetter CatchSet => RawState2.Catch;
    private ICharaShootStateSetter ShootSet => RawState2.Shoot;
    private ICharaCourtStateSetter CourtSet => RawState2.Court;
    private ICharaPassStateSetter PassSet => RawState2.Pass;
    private ICharaDashmanStateSetter DashmanSet => RawState2.Dashman;
    private ICharaComOnlyStateSetter ComOnlySet => RawState2.ComOnly;
    private ICharaBallEffectStateSetter BallEffectSet => RawState2.BallEffect;

    public CharaBehavior()
    {
    }
}