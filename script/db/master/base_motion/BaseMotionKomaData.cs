using System;

namespace db;

public class BaseMotionKomaData
{
    private readonly int[] _data;

    // 絵番号
    public int BmpNo => _data[(int)enBMCommonDt.bmpNo];

    // Ｘ位置
    public int PosX => _data[(int)enBMCommonDt.posX];

    // Ｙ位置
    public int PosY => _data[(int)enBMCommonDt.posY];

    // SE
    public int Se => _data[(int)enBMCommonDt.se];

    // ループ回数
    public int LoopNum => _data[(int)enBMCommonDt.loopNum];

    // デフォフレーム数
    public int DefFrm => _data[(int)enBMCommonDt.DefFrm];

    // ループ設定
    public bool LoopSt => _data[(int)enBMCommonDt.loopSt] != 0;

    // ループ時SEも鳴らし直す
    public bool SeLoopF => _data[(int)enBMCommonDt.seloop_f] != 0;

    // アクションポイント
    public bool IsActionPoint => _data[(int)enBMCommonDt.ActP_f] != 0;

    public BaseMotionKomaData()
    {
        _data = new int[Enum.GetValues<enBMCommonDt>().Length];
    }

    public void Load()
    {
    }

    //     //読み込み
    //     void LoadDt(const char *CSVDir, const char *CSVFilename_ys, const char *CSVFilename_rs);
    //     //取得
    //     s32 GetDt(enDtComAct dtNo, s32 ptn, BOOL ressei_f);//参照CDtの取得
}