#pragma once
#include "dbInclude.h"
#include "dbUILayer.h"
#include "dbUGHeadder.h"
#include "dbUGFooter.h"
#include "dbUGFilter.h"
#include "dbUGShiai.h"
namespace db
{
  class TSnHome;
  class TUIFrame;


  struct TTLyBarGenSt
  {
    s32 filter_c_;
    s32 filter2_c_;
    s32 nexttabNo_;
    s32 tabNo_;
    s32 refresh_c_;
    BOOL barmove_f_;
    BOOL action_f_;
    BOOL shiai_f_;
    BOOL autooff_f_;

    void Init()
    {
      Zeromem(this);
    }
  };


  //ヘッダフッタ////////////////////////////////
  class TLyBarGen :public TUILayer
  {
  public:
    //コンストラクタ
    explicit TLyBarGen(TScene* pScene, TUIFrame* frame);
    virtual ~TLyBarGen(void);
    void RefreshHeadder();
    void SetFilterAll(BOOL gototitle_f);
    void SetFilterAll_AutoOff();
    BOOL IsBarMove();
    BOOL IsBarAction();
    void CloseHeader();
    void JumpHeader(s32 pageno);
    void SetNoneBar(BOOL nonebar_f);
    void SetMakeShiai();
	void JumpFooter(s32 tabno);

    TTLyBarGenSt stBar_;
    std::vector<TUILayer*> tabs_;
    TUGFilter ugfilter_;
    TUGHeadder ugheadder_;
    TUGFooter ugfooter_;
    TUGShiai ugshiai_;
  protected:
    //オーバーロード
    virtual void DoOpenHeader() = 0;
    virtual void DoCloseHeader() = 0;
    virtual void DoJumpHeader() = 0;
    virtual void DoOpenTechno() = 0;
    virtual void DoOpenProfile() = 0;
    virtual void DoLoadSub(){}
    virtual void DoUpdateSub(BOOL draw_f){}
    virtual void DoMoveFilter(){}
    virtual void DoMoveFilterAll(){}
    virtual void DoBackBtn(){}

    void DoLoad();
    void DoChangePage(s32 pageno);
    void DoConnect(BOOL rqok_f);
    void DoUpdate(BOOL draw_f);

    void MvPage_Bar();
    void MvPage_Profile();
    void MvPage_Techno();
    void MvPage_GGZ();
    void MvPage_Filter();
    void MvPage_FilterAll();
    void MvPage_GotoTitle();
    void MvPage_Error();

    void GenOpenHeader();
    void OpenTechno();
    void OpenProfile();
    void OpenGGZ();
    void Post_GGZ();
    void Post_GGZQuick();
    void Connect_GGZ();
    void Connect_GGZQuick();
  };

}
