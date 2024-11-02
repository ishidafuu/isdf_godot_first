namespace db;

public class TeamComState
{
    public int SideIndex { get; }
    public TeamComMainState MainState { get; } = new TeamComMainState();
    public TeamComMemberState MemberState { get; } = new TeamComMemberState();
    public TeamComPatternState PatternState { get; } = new TeamComPatternState();
    public TeamComAttackState AttackState { get; } = new TeamComAttackState();
    public TeamComActionState[] ActionState { get; } = new TeamComActionState[Defines.MemberCount];
    public TeamComInfieldState[] InfieldState { get; } = new TeamComInfieldState[Defines.InfieldCount];

    public TeamComState(int sideIndex)
    {
        SideIndex = sideIndex;

        for (var i = 0; i < ActionState.Length; i++)
        {
            ActionState[i] = new TeamComActionState();
        }

        for (var i = 0; i < InfieldState.Length; i++)
        {
            InfieldState[i] = new TeamComInfieldState();
        }

        Initialize();
    }

    public void Initialize()
    {
    }

}