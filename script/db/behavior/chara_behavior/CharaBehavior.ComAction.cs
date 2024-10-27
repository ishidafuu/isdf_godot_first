using isdf;

namespace db;

public partial class CharaBehavior
{
    /// <summary>
    /// //ＣＯＭ思考行動（パス、シュート）★★
    /// </summary>
    private void ComAction()
    {
        //アクション中は来ない
        //ボールマンだけ
        if (MyMotion.HasFlag(CharaMotionFlag.Act)
            || IsBallHolder == false)
        {
            return;
        }

        if (MyTeamState.SemiAutoState.SemiShotF
            || MyTeamState.MainState.ComOverTimeF)
        {
            COMShootAct();
            MyTeamState.SemiAutoState.SemiShotF = false;
        }
        else if (MyComState.MainState.Isdm())
        {
            COMAction_DM();
        }
        else
        {
            COMAction_Std();
        }
    }

    void COMShootAct()
    {
    }

    void COMAction_DM()
    {
    }

    void COMAction_Std()
    {
    }
}