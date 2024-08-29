#include "dbUIGenLayer.h"
#include "dbMgEngineObj.h"
#include "dbMgUI.h"
#include "dbGame.h"
#include "dbScene.h"
namespace db
{
  //コンストラクタ
  TUIGenLayer::TUIGenLayer(TScene* pScene) : 
    pGenScene_(pScene),
    pGame_(pScene->pGame_),
    pmgEO_(pScene->pmgEO_), 
    pmgUI_(pScene->pmgUI_)
  {

  }

  TUIGenLayer::~TUIGenLayer()
  {
    Clear();//全オブジェ解放
  }

  void TUIGenLayer::ClearGrp()//クリア
  {
    for (s32 i = 0; i < pbovec_.size(); ++i)
    {
      pbovec_[i]->DeleteTex();
      Freemem(pbovec_[i]);
    }
    //グラフィック解放
    for (s32 i = 0; i < pgrvec_.size(); ++i) Freemem(pgrvec_[i]);
    pbovec_.clear();
    pgrvec_.clear();
  }
  //全オブジェ解放
  void TUIGenLayer::Clear()
  {
    ClearGrp();//画像クリア

    //オブジェ解放
    for (s32 i = 0; i < puiovec_.size(); ++i) Freemem(puiovec_[i]);
    puiovec_.clear();
  }

  //PNGファイルから直接テクスチャ作成
  s32 TUIGenLayer::LoadBmo(const char* dirname, const char* filename, BOOL nocolkey_f)//bovec_.push_back(bmo)が本体なので、ハンドル番号を返す方がよい
  {
    mid::TBmpObj* pbmo = new mid::TBmpObj();
    pbmo->Init();
    if (nocolkey_f)
    {
    mid::midLoadCI_NoColKey(pbmo, dirname, filename);
    }
    else
    {
      mid::midLoadCI(pbmo, dirname, filename);
    }
    pbovec_.push_back(pbmo);
    return (pbovec_.size() - 1);
  }
  //グラフィック作成（グラフィック番号を返す）
  base::TGrp* TUIGenLayer::LoadGrp(const char* dirname, const char* filename)
  {
    s32 bmono = LoadBmo(dirname, filename, FALSE);
    base::TGrp* pgr = new base::TGrp(&(pmgEO_->mgOrder_), (pbovec_[bmono]));
    pgrvec_.push_back(pgr);
    return pgr;
  }
  base::TGrp* TUIGenLayer::LoadGrp_NoColKey(const char* dirname, const char* filename)
  {
    s32 bmono = LoadBmo(dirname, filename, TRUE);
    base::TGrp* pgr = new base::TGrp(&(pmgEO_->mgOrder_), (pbovec_[bmono]));
    pgrvec_.push_back(pgr);
    return pgr;
  }
  //グラフィック作成（パーツ）
  base::TGrPart* TUIGenLayer::LoadGrpPart(const char* dirname, const char* filename, s32 www, s32 hhh)
  {
    s32 bmono = LoadBmo(dirname, filename, FALSE);
    base::TGrPart* pgr = new base::TGrPart(&(pmgEO_->mgOrder_), (pbovec_[bmono]), www, hhh);
    pgrvec_.push_back((base::TGrp*)pgr);
    return pgr;
  }
  //グラフィック作成（パーツ）
  base::TGrTile* TUIGenLayer::LoadGrpTile(const char* dirname, const char* filename)
  {
    s32 bmono = LoadBmo(dirname, filename, FALSE);
    base::TGrTile* pgr = new base::TGrTile(&(pmgEO_->mgOrder_), (pbovec_[bmono]), filename);
    pgrvec_.push_back((base::TGrp*)pgr);
    return pgr;
  }
  //オブジェ作成（オブジェ番号を返す）
  TUIObj* TUIGenLayer::MakeObj()
  {
    TUIObj* pso = new TUIObj(pmgEO_, pmgUI_);
    puiovec_.push_back(pso);
    return pso;
  }

  void TUIGenLayer::GotoTitle()
  {
    pGenScene_->GotoTitleFilter();
  }
  void TUIGenLayer::ReloadTex()
  {
//    mid::midLog2("puiovec_.size()%d",(s32)puiovec_.size());
    //ファイルからの読み込み
    for (s32 i = 0; i < pbovec_.size(); ++i)
    {
      pbovec_[i]->ReloadTex();
    }
  }

}
