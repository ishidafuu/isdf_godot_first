namespace db;

public class TeamComState
{
    public int SideIndex { get; }
    public TeamComMainState Main { get; } = new TeamComMainState();
    public TeamComMemberState Member { get; } = new TeamComMemberState();
    public TeamComPatternState Pattern { get; } = new TeamComPatternState();
    public TeamComAttackState Attack { get; } = new TeamComAttackState();
    public TeamComActionState[] Action { get; } = new TeamComActionState[Defines.MemberCount];
    public TeamComInfieldState[] Infield { get; } = new TeamComInfieldState[Defines.InfieldCount];

    public TeamComState(int sideIndex)
    {
        SideIndex = sideIndex;

        for (var i = 0; i < Action.Length; i++)
        {
            Action[i] = new TeamComActionState();
        }

        for (var i = 0; i < Infield.Length; i++)
        {
            Infield[i] = new TeamComInfieldState();
        }

        Initialize();
    }

    public void Initialize()
    {
    }

}