using System;
using db.masters;

namespace db;

/// <summary>
/// COM思考メンバー全員に関するステータス
/// </summary>
public class ComMemberState
{
    //パスを呼んでる
    public bool CallingPa_f { get; set; }

    //ガードを呼んでる
    public bool CallingGd_f { get; set; }

    //リーダー化
    public bool CallingRd_f { get; set; }

    public void Initialize()
    {
        CallingPa_f = false;
        CallingGd_f = false;
        CallingRd_f = false;
    }
}