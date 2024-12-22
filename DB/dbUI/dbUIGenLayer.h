#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"


namespace db
{
  class TMgEngineObj;
  class TMgUI;
  class TGame;
  class TScene;

  class TUIGenLayer
  {
  public:
    TScene* pGenScene_;
    //ゲームクラスポインタ
    TGame* pGame_;
    //スクリプトクラスポインタ
    TMgUI* pmgUI_;
    //エンジンオブジェクトポインタ
    TMgEngineObj* pmgEO_;

    //コンストラクタ
    explicit TUIGenLayer(TScene* pScene_);
    virtual ~TUIGenLayer(void);
    base::TGrp* LoadGrp(const char* dirname, const char* filename);
    base::TGrp* LoadGrp_NoColKey(const char* dirname, const char* filename);
    base::TGrPart* LoadGrpPart(const char* dirname, const char* filename, s32 www, s32 hhh);
    base::TGrTile* LoadGrpTile(const char* dirname, const char* filename);

    //オブジェ作成
    TUIObj* MakeObj();

    void ClearGrp();//クリア
    void Clear();//クリア
    void GotoTitle();//タイトルへ戻る
    void ReloadTex();

    std::vector<base::TGrp*> GetPgrVec(void){ return pgrvec_; }


  protected:
    std::vector<mid::TBmpObj*> pbovec_;//構造体のベクタにすると次のpgrvecの領域をぶっこわすのでポインタに
    std::vector<base::TGrp*> pgrvec_;//bmpobjのポインタを持つ
    std::vector<TUIObj*> puiovec_;//TUIObjのポインタを持つ
    //画像読み込み
    s32 MakeFontBmo(const char* msg, u32 draw_fs);
    s32 LoadBmo(const char* dirname, const char* filename, BOOL nocolkey_f);
  };

}
