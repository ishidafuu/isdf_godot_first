﻿namespace db;

/// <summary>
/// COM思考メンバー全員に関するステータス
/// </summary>
public class ComMemberState : IComMemberStateGetter, IComMemberStateSetter
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

public interface IComMemberStateGetter
{
    bool CallingPa_f { get; }
    bool CallingGd_f { get; }
    bool CallingRd_f { get; }
}

public interface IComMemberStateSetter
{
    bool CallingPa_f { set; }
    bool CallingGd_f { set; }
    bool CallingRd_f { set; }

    void Initialize();
}