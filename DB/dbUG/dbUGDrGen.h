#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGGen.h"
#include "dbUGButton.h"
namespace db
{

  enum enUGDrumAct
  {
    enUGDrGenAct_Roll,
    enUGDrGenAct_Stop,
    enUGDrGenAct_Start,
  };


  //ドラムのレコードオブジェの数
  const s32 DRRECNUM = 12;

  struct TDrGenSt
  {
    BOOL USEOK_F_;
    BOOL USEEYE_F_;
    BOOL USEETC_F_;
    BOOL TOPFRONT_F_;
    s32 REVY_;
    s32 DISTY_;
    s32 LOOKPOS_;
    s32 SLIDEY_;
    s32 CARDH_;
    s32 EYEX_;
    s32 EYEY_;
    s32 OKX_;
    s32 OKY_;
    s32 ETCX_;
    s32 ETCY_;

    s32 dtnum_;
    f32 dfx_;
    f32 dfy_;
    s32 recno_;//ドラムの何番を見ているか
    BOOL move_f_;//ドラムの進行
    s32 move_c_;//ロールカウンタ
    s32 swpos_;//スワイプ情報
    s32 swpos2_;//スワイプ情報
    s32 dtno_;//recnoがモデルの何番か
    s32 drrecposno_[DRRECNUM];
    s32 drrecdtno_[DRRECNUM];
    BOOL drrecloop_f_[DRRECNUM];//周回しての表示か

    BOOL useura_f_;
    BOOL ura_f_;

    BOOL usedrumend_f_;
    BOOL drumend_f_;
    BOOL noneswipe_f_;
    //BOOL actionwait_f_;

    BOOL roll_f_;//このフレーム回ったか
    BOOL stop_f_;//このフレームに留まったか(loading)
    BOOL start_f_;//このフレームに動き始めたか

    BOOL notroll_f_;//ボタンが押されてる（ロールしない）
    BOOL loop_f_;
    void Init()
    {
      Zeromem(this);
    }
  };

  //ドラム
  class TUGDrGen :public TUGGen
  {
  private:
    std::vector<TUGGen*> gen_records_;//これもカードの基礎の方が良いか　表示だけなのでべつにいいか
    TUGGen* gen_ura_;
    TUGGen* gen_drumend_;
    
  protected:
    void GenMakeGroup(TUIObj* parent, BOOL leftpos_f);

    void GenSetUramen(TUGGen* pug);//裏面オブジェ
    void GenSetDrumEnd(TUGGen* pug);//決定オブジェ

    void SetSwipeSize();
    void GenDrumRoll(BOOL first_f, s32 dtno);
    void DrumStart();
    void DrumStop();
    void GenMakeGroup(TUIObj* parent);
    void ShiftOKPos(BOOL toppos_f);
    void GenAddRecord(TUGGen* pug);//レコードオブジェ
    void SetDontRoll();

    void DoUpdate(BOOL draw_f);//更新
    void DoAction();

//    virtual void DoRollRefresh();
    virtual void DoStop() = 0;
    virtual void DoRoll() = 0;
    virtual void DoReady() = 0;
	virtual void DoStart() = 0;
    virtual void DoUpdateSub(BOOL draw_f){};//更新
    virtual void DoActionSub(){};
    virtual BOOL DoSelectUra(BOOL ura_f){ return ura_f; };
  public:
    TDrGenSt stDrGen_;

    //コンストラクタ
    explicit TUGDrGen(TUIGenLayer* player);
    virtual ~TUGDrGen(void);
    void Ready(s32 dtnum);
    void SetLoop(BOOL loop_f);
    void SetSwipeHeight(s32 fdtno);
    void StopSwipe(BOOL stop_f);
    void DrumRoll_LookDt(s32 dtno);
    void DrumRoll();
    BOOL IsStop();
    s32 GetDataNo();
    s32 GetLookRecord();
    void SelectDrumEnd(BOOL drumend_f);
    void SelectUra(BOOL ura_f);
    //void RevUra();
    BOOL IsUra();
    BOOL IsData();

    BOOL IsActRes(enUGDrumAct res);
	void SetLRPos(BOOL left_f);
  };
 
}
