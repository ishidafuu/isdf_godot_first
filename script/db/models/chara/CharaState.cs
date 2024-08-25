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

    public CharaLive Live { get; set; } = new();
    public CharaDamage Damage { get; set; } = new();
    public CharaLevel Level { get; set; } = new();
    public CharaPosition Position { get; set; } = new();
    public CharaMove Move { get; set; } = new();
    public CharaAir Air { get; set; } = new();
    public CharaView View { get; set; } = new();
    public CharaCatch Catch { get; set; } = new();
    public CharaShoot Shoot { get; set; } = new();
    public CharaDodge Dodge { get; set; } = new();
    public CharaCourt Court { get; set; } = new();
    public CharaPass Pass { get; set; } = new();
    public CharaDashman Dashman { get; set; } = new();
    public CharaCom Com { get; set; } = new();
    public CharaBallEffect BallEffect { get; set; } = new();

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