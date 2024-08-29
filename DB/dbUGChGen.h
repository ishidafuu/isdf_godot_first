#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbMgEngineObj.h"
#include "dbUGGen.h"
namespace db
{
  enum enUGChGenAct
  {
    enUGChGenAct_MotihonEnd,
    enUGChGenAct_ActPoint,
    enUGChGenAct_MoveStop,
  };

  enum enUGChGenMuki
  {
    cmk_None,
    cmk_Left,
    cmk_Right,
  };


  //描画情報
  struct TCharStMotion
  {
    s32 mtype_;//モーション
    s32 m_c_;//そのモーション全体のカウンタ
    s32 frameNo_;//BMおよびTECEditorのアニメーション番号
    s32 ani_c_;//アニメーション内のカウンタ
    s32 loop_c_;//ループ回数
    s32 loopStNo_;//ループ始点
    BOOL actP_f_;//アクションポイントフラグ
    void Init()
    {
      Zeromem(this);
    }
  };
  struct TGenCharSt
  {
    TCharStMotion mt_;

    //汎用行動番号
    s32 moveNo_;
    BOOL mirror_f_;
    enUGChGenMuki muki_;	// 立ち止まった時の向き -1:無効な値 0:反転なし 1:反転
    void Init()
    {
      Zeromem(this);
    }
  };

  //リスト
  class TUGChGen :public TUGGen
  {
  protected:
    TUIObj* itemobj_;
    TBMData* bmdt_;
    void GenMakeGroup(TUIObj* parent, TMgCharGrp* pmgchar, TBMData* bmdt, s32 bmpoffset);
    void Count();
    void MotionEnd();
    void NextFrame();
    void IncMotion(BOOL goto_f);//goto_f:強制的に進める
    s32 NowKoma(enBMCommonDt dtNo);
    BOOL NowKomaBOOL(enBMCommonDt dtNo);
    BOOL IsLastKoma();
    void SetFrameData(BOOL InLoop_f);
    void SetGenMType(s32 mtype);
    void SetItemPos();
    virtual void DoActionSub() = 0;

    void DoUpdate(BOOL draw_f);
    void DoAction();
  public:
    TGenCharSt stChar_;

    //コンストラクタ
    explicit TUGChGen(TUIGenLayer* player);
    virtual ~TUGChGen(void);

    void SetMirror(BOOL mirror_f);
    void SetItem(TUIObj* itemobj);
    void ReleaseItem();
    BOOL IsItem();

	BOOL IsMirror(void);
    BOOL IsActRes(enUGChGenAct res);//返値ゲット
  };
 
}
