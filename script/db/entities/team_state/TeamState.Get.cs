namespace db;

public partial class TeamState
{
    /// <summary>
    /// 全員アウト
    /// </summary>
    public bool IsAllOut
    {
        get
        {
            foreach (var mySideChara in MySideCharas)
            {
                if (mySideChara.IsOut == false)
                {
                    return false;
                }
            }

            return true;
        }
    }

    public bool IsCom => SemiAutoState.SemiF || MainState.ManSideF == false;
    
    

}