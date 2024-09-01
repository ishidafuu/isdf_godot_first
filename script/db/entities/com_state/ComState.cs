namespace db;

public class ComState
{
    public int SideIndex { get; }
    public ComActionState ActionState { get; } = new ComActionState();
    public ComAttackState AttackState { get; } = new ComAttackState();
    public ComInfieldState[] ComInfieldState { get; } = new ComInfieldState[Defines.InfieldCount];
    public ComMainState MainState { get; } = new ComMainState();
    public ComMemberState MemberState { get; } = new ComMemberState();
    public ComPatternState PatternState { get; } = new ComPatternState();

    public ComState(int sideIndex)
    {
        SideIndex = sideIndex;

        for (var i = 0; i < ComInfieldState.Length; i++)
        {
            ComInfieldState[i] = new ComInfieldState();
        }

        Initialize();
    }

    public void Initialize()
    {
    }

}