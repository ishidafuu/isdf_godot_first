#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGLabel.h"
#include "dbUGRIcon.h"
namespace db
{

  struct TUGButtonSt
  {
    BOOL enterAct_f_;//リリースアクションではなくエンターアクション
    base::TGrp* basegrp_;
    base::TGrp* basegrp2_;
    BOOL part_f_;
    s32 partno_;//パーツ番号
    s32 slpartno_;//選択時パーツ番号
    BOOL disable_f_;
    void Init()
    {
      Zeromem(this);
    }
  };

  class TUGButton : public TUGGen
  {
  protected:
    static const s32 TABDX = 64;
    void GenMakeGroup(TUIObj* parent, BOOL center_f);
    void DoAction();
    void DoUpdate(BOOL draw_f);
    virtual void DoUpdateSub(BOOL draw_f){}
    virtual void DoActionSub(){}
    virtual void DoMakeGroupSub(TUIObj* parent){}
  public:   
    TUGRIcon ugricon_;
    TUGButtonSt stBtn_;
    //コンストラクタ
    explicit TUGButton(TUIGenLayer* player);
    virtual ~TUGButton(void);
    void MakeGroup(TUIObj* parent, base::TGrp* basegrp);
    void MakeGroup(TUIObj* parent, enUGGrp basegrp);
    void MakeGroup(TUIObj* parent, enUGGrp basegrp, enUGGrp basegrp2);
    void MakeGroup(TUIObj* parent, enUGGrp basegrp, BOOL center_f);
    void MakeGroup(TUIObj* parent, enUGGrPart basegrp, s32 partno);
    void MakeGroup_Otoiawase(TUIObj* parent);
    void MakeGroup_Headder(TUIObj* parent, enUGGrp basegrp, enUGGrp basegrp2);
    void MakeGroup_Footer(TUIObj* parent, s32 pos);
    void MakeGroup_FooterTokusyou(TUIObj* parent);
    void MakeGroup_FtPosBack(TUIObj* parent);
    void MakeGroup_FtPos(TUIObj* parent, s32 pos, const char* msg);
    void MakeGroup_FtPosTokusyou(TUIObj* parent);
    void MakeGroup_Tokusyou(TUIObj* parent);
		/* 2015/07/02 Koike Added */
		void MakeGroup_FtPosCancel(TUIObj* parent);
    
    void MakeGroup_Balloon(TUIObj* parent, const char* msg);
    void MakeGroup_Radio(TUIObj* parent, base::TGrp* basegrp, base::TGrp* basegrp2);
    void MakeGroup_Radio(TUIObj* parent, enUGGrp basegrp, enUGGrp basegrp2);
    void MakeGroup_Radio(TUIObj* parent, enUGGrPart basegrp, s32 partno, s32 slpartno);


    void SetEnterAct(BOOL enteract_f);
    void SetFont(const char* msg);
    void SetFont(const char* msg, s32 fontcol);
    void SetFont(const char* msg, s32 fontcol, s32 fx, s32 fy);
    void SetPos(s32 posx, s32 posy);
    void SetSelect(BOOL select_f);
    void SetDisable(BOOL disable_f);
  };

 
}
