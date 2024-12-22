#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGRcGen.h"
//#include "dbUGButton.h"
#include "dbUGRadio.h"
#include "dbUGRName.h"
#include "dbUGBtnLabel.h"
namespace db
{
  enum enUGLiAct
  {
    enUGLiAct_SubA,
    enUGLiAct_SubB,
    enUGLiAct_SubC,
    enUGLiAct_SubD,
    enUGLiAct_SubE,
    enUGLiAct_Record,
    enUGLiAct_RecordBtn,
  };

  struct TUGListSt
  {
    
    f32 overh_;
    s32 leftx_;
    s32 bottomy_;
    s32 marker_c_;
    BOOL listend_f_;
    BOOL slide_f_;
    void Init()
    {
      Zeromem(this);
    }
  };


  //リスト
  class TUGLiGen :public TUGGen
  {
  private:

  protected:
    std::vector<TUGRcGen*> genrecords_;
    TUIObj* slider_;//実際に移動するフィールド
    TUIObj* marker_;//マーカー
    f32 sliderx_;
    f32 slidery_;
    void GenMakeObj_Record(TUGRcGen* newrecord, s32 rh);
    void SetRecordPos();
    void SetSelect();
    void SetSliderDisable(BOOL disable_f);
    
    INTVECTOR GetSelectedRec();
    void GenSetListEnd(BOOL listend_f);
    TUGBtnLabel* MakeSubBtn(const std::string& label, enUGPIcon iconno);

    virtual void DoActionSub();
    virtual void DoUpdateSub(BOOL draw_f);
    virtual void DoMakeGroupSub();
    virtual void DoClearRecord() = 0;//必須

    void DoUpdate(BOOL draw_f);
    void DoAction();
  public:
    TUGListSt stList_;
    //TUGButton ugbtn_ok_;//ＯＫボタン
    std::vector<TUGBtnLabel*> ugbtn_sub_;
    
    TUGRadio ugradio_sub_;
    TUGRName ugrname_;

    //コンストラクタ
    explicit TUGLiGen(TUIGenLayer* player);
    virtual ~TUGLiGen(void);
    void MakeGroup(TUIObj* parent, BOOL leftpos_f);
    void ClearRecord();
    void SetZeroPos();
	s32 GetSelect();
	s32 GetSelectNum();
    void SetSelect(s32 idx);
    BOOL IsActRes(enUGLiAct res);
    void SetNamePage(s32 nowpage, s32 maxpage);
    void SetLRPos(BOOL leftpos_f);
  };
 
}
