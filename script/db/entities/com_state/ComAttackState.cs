namespace db;

/// <summary>
/// COMアクション思考に関するステータス
/// </summary>
public class ComAttackState : IComAttackStateGetter, IComAttackStateSetter
{
    //キャラの手に渡った時点で初期化する系
    public int DmpawaitC { get; set; } // ダッシュマンパスインターバル用
    public bool QuickF { get; set; } // クイックシュート行動
    public bool VnnojpF { get; set; } // 至近距離ジャンプ無しシュート行動
    public bool ShF { get; set; } // シュート行動
    public bool PaF { get; set; } // パス行動
    public bool JpF { get; set; } // ジャンプ行動
    public bool DsF { get; set; } // ダッシュ行動
    public bool OnestepF { get; set; } // １歩ダッシュ
    public bool TossF { get; set; } // トスパス
    public bool DsposF { get; set; } // ダッシュ開始位置
    public int WaitmoveC { get; set; } // ウエイト動作
    public bool DmpassF { get; set; } // ダッシュマンパス
    public int Getstep { get; set; } // ボール取得時の歩数
    public int DsTagZ { get; set; } // ダッシュ目標
    public int DsTagZF { get; set; } // ダッシュ目標常にサーチ

    public OrderIndexType PaTag { get; set; } // シングルパスタゲ

    public void Initialize()
    {
    }
}

public interface IComAttackStateGetter
{
    int DmpawaitC { get; }
    bool QuickF { get; }
    bool VnnojpF { get; }
    bool ShF { get; }
    bool PaF { get; }
    bool JpF { get; }
    bool DsF { get; }
    bool OnestepF { get; }
    bool TossF { get; }
    bool DsposF { get; }
    int WaitmoveC { get; }
    bool DmpassF { get; }
    int Getstep { get; }
    int DsTagZ { get; }
    int DsTagZF { get; }
    OrderIndexType PaTag { get; }
}

public interface IComAttackStateSetter
{
    int DmpawaitC { set; }
    bool QuickF { set; }
    bool VnnojpF { set; }
    bool ShF { set; }
    bool PaF { set; }
    bool JpF { set; }
    bool DsF { set; }
    bool OnestepF { set; }
    bool TossF { set; }
    bool DsposF { set; }
    int WaitmoveC { set; }
    bool DmpassF { set; }
    int Getstep { set; }
    int DsTagZ { set; }
    int DsTagZF { set; }
    OrderIndexType PaTag { set; }

    void Initialize();
}