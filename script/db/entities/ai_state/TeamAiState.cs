namespace db;

public class TeamAiState
{
    public int SideIndex { get; }
    public TeamAiMainState Main { get; } = new TeamAiMainState();
    public TeamAiMemberState Member { get; } = new TeamAiMemberState();
    public TeamAiPatternState Pattern { get; } = new TeamAiPatternState();
    public TeamAiAttackState Attack { get; } = new TeamAiAttackState();
    public TeamAiActionState[] Action { get; } = new TeamAiActionState[Defines.MemberCount];
    public TeamAiInfieldState[] Infield { get; } = new TeamAiInfieldState[Defines.InfieldCount];

    public TeamAiState(int sideIndex)
    {
        SideIndex = sideIndex;

        for (var i = 0; i < Action.Length; i++)
        {
            Action[i] = new TeamAiActionState();
        }

        for (var i = 0; i < Infield.Length; i++)
        {
            Infield[i] = new TeamAiInfieldState();
        }

        Initialize();
    }

    public void Initialize()
    {
    }

}
