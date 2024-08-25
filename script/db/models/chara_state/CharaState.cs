namespace db;

public class CharaState
{
    public CoordinateState Coordinate { get; set; } = new();
    public CharaMotionState Motion { get; set; } = new();
    public CharaAnimeState Anime { get; set; } = new();
    public CharaAutoState Auto { get; set; } = new();
    public CharaAutoState NextAuto { get; set; } = new();
    public CharaComAutoState ComAuto { get; set; } = new();
    public CharaPadState Pad { get; set; } = new();
    public CharaGouseiState Gousei { get; set; } = new();
    public CharaLiveState Live { get; set; } = new();
    public CharaDamageState Damage { get; set; } = new();
    public CharaLevelState Level { get; set; } = new();
    public CharaPositionState Position { get; set; } = new();
    public CharaMoveState Move { get; set; } = new();
    public CharaAirState Air { get; set; } = new();
    public CharaViewState View { get; set; } = new();
    public CharaCatchState Catch { get; set; } = new();
    public CharaShootState Shoot { get; set; } = new();
    public CharaDodgeState Dodge { get; set; } = new();
    public CharaCourtState Court { get; set; } = new();
    public CharaPassState Pass { get; set; } = new();
    public CharaDashmanState Dashman { get; set; } = new();
    public CharaComState Com { get; set; } = new();
    public CharaBallEffectState BallEffect { get; set; } = new();

    public void Initialize()
    {
        Coordinate.Initialize();
        Motion.Initialize();
        Anime.Initialize();
        Auto.Initialize();
        NextAuto.Initialize();
        ComAuto.Initialize();
        Pad.Initialize();
        Gousei.Initialize();
        Live.Initialize();
        Damage.Initialize();
        Level.Initialize();
        Position.Initialize();
        Move.Initialize();
        Air.Initialize();
        View.Initialize();
        Catch.Initialize();
        Shoot.Initialize();
        Dodge.Initialize();
        Court.Initialize();
        Pass.Initialize();
        Dashman.Initialize();
        Com.Initialize();
        BallEffect.Initialize();
    }
}