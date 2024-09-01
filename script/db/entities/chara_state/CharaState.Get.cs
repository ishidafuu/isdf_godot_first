namespace db;

public partial class CharaState
{
    /// <summary>
    /// アウト扱い
    /// </summary>
    public bool IsOut()
    {
        return Live.Hp == 0 || Live.IsAngel;
    }
    
    
}