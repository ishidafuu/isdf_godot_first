namespace db;

public partial class TeamState
{
    /// <summary>
    /// 全員アウト
    /// </summary>
    public bool IsAllOut()
    {
        foreach (var mySideChara in MySideCharas)
        {
            if (mySideChara.IsOut() == false)
            {
                return false;
            }
        }

        return true;
    }

    public bool IsCom()
    {
        // セミオート時のみオート扱いor COMサイド

        return (SemiAutoState.SemiF || MainState.ManSideF == false);
    }

}