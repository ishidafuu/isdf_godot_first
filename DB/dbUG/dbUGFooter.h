#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGRadio.h"
#include "dbUGButton.h"
#include "dbUGBtnTokusyou.h"
#include "dbUGBtnSupply.h"
namespace db
{
	enum enFooterTabID
	{
		footer_id_home = 0,
		footer_id_shiai,
		footer_id_team,
		footer_id_center,
		footer_id_shop,
		footer_id_event,

		footer_id_help,		// フッターのタブとして存在しないがバナーのジャンプ先として存在
	};

  enum enFooterState
  {
    foot_None,
    foot_Tab,
    foot_Btn,
  };

  struct TUGFooterSt
  {
    enFooterState laststate_;
    enFooterState state_;
    s32 tabno_;
    BOOL hide_f_;
    void Init()
    {
      Zeromem(this);
    }
  };


  class TUGFooter : public TUGGen
  {
  protected:
    void DoUpdate(BOOL draw_f);
    void DoAction();
  public:
    TUGFooterSt stFooter_;
    TUGRadio ugradio_;
    std::vector<TUGButton*> tabbtn_;
    std::vector<TUGButton*> lybtn_;

    TUGButton ugbtn_back_;
    TUGButton ugbtn_ok_;
    TUGButton ugbtn_mid_;
    TUGButton ugbtn_prev_;
    TUGButton ugbtn_next_;
    TUGBtnTokusyou ugbtn_tsh_;
    TUGBtnSupply ugbtn_mgk_;
    TUGBtnSupply ugbtn_fkt_;

    //コンストラクタ
    explicit TUGFooter(TUIGenLayer* player);
    virtual ~TUGFooter(void);

    void MakeGroup(TUIObj* parent);
    BOOL IsTab();
    void SetEnable();
    void SetDisable();
    void UndrawBtn();
    void SetFoot_Tab();
    void SetFoot_Btn();
    void SetFoot_None();
    void HideBtn(BOOL hide_f);
    BOOL IsHideBtn();
  };
 
}
