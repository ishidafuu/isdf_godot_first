#pragma once
#include "dbLyHmGen.h"
#include "dbUGDrShot.h"
#include "dbUGDrTeam.h"
#include "dbUGLiMenu.h"
#include "dbUGLiMember.h"
#include "dbUGLiSoubi.h"
#include "dbUGLiShSet.h"
#include "dbUGLiTmFilter.h"
#include "dbUGLiShFilter.h"
#include "dbUGLiShSetMenu.h"
#include "dbUGLiSoubiAll.h"
#include "dbUGLiShSetSelectAll.h"
#include "dbUGLiSyn.h"

namespace db
{

  struct THmTmSelectSt
  {
    s64 id_team_;//保持チームＩＤ
    s32 mid_team_;//チームマスターＩＤ
    s32 posno_;
    s32 slot_;
    s64 shotid_look_;
    s64 id_shot_;
    s32 soubirecid_[STEPNUM];
    s64 id_shset_;
    BOOL changed_f_;
    s32 nextPage_;
    s32 shfilterPage_;
    s32 char0_;
    s32 char1_;
    s32 char2_;
    s32 char3_;
    s32 char4_;


    enTeamFilType filtype_;
    enTeamSrtType srttype_;

    void Init()
    {
      Zeromem(this);
      id_team_ = NGNUM;
    }
  };


  class TLyHmTmSelect :public TLyHmGen
  {
  public:
    //コンストラクタ
    explicit TLyHmTmSelect(TSnHome* pScene);
    virtual ~TLyHmTmSelect(void);
  protected:
    //オーバーロード
    void DoLoad();
    void DoReady();
    void DoDown();
    void DoChangePage(s32 pageno);
    void DoConnect(BOOL rqok_f);
    void DoUpdate(BOOL draw_f);
    void MvPage_Menu();
    void MvPage_Omakase();
    void MvPage_ShSet();
    void MvPage_ShSetRename();
    void MvPage_TmFilter();
    void MvPage_Sort();
    void MvPage_TeamSelect();
    void MvPage_ShSetSelect();
    void MvPage_MemberSelect();
    void MvPage_MemSoubi();
    void MvPage_AllSoubi();
    void MvPage_Syn();
    void MvPage_ShFilter();
    void MvPage_Prev();
    void MvPage_PrevSyn();
    void Post_Omakase();
    void Post_AttachCustom();
    void Post_Detach();
    void Post_AttachShSet();
    void Post_SwitchShSet();
    void Post_RenameShSet();
    void Connect_Omakase();
    void Connect_MGKQuick();
    void Connect_AttachCustom();
    void Connect_AttachShSet();
    void Connect_SwitchShSet();
    void Connect_RenameShSet();
    void DrawList(TUGLiGen* drawlist);
    void ResetObj();
    void RefreshMenuList();
    void ShSetAtattch(BOOL dettach_f);
    void SoubiAtattch(BOOL dettach_f);
    void SoubiAllAtattch(BOOL dettach_f);
    void ClearListSoubi(BOOL init_f);
    void SelectShSet();
    void ResetPrevBtn();
    void ShSetToAll(s32 shsetno);
    THmTmSelectSt stSelect_;
    TUGLiMenu uglistMenu_;//メニュー
    TUGLiShSetSelectAll uglistShSetSelect_;
    //TUGLiMenu uglistTeamSelect_;//チームセレクト部分メニュー
    TUGLiShSetMenu uglistShSetMenu_;//シュートセットメニュー
    TUGLiShSet uglistShSet_;//シュートセット
    TUGLiTmFilter uglistTmFilter_;//チームフィルタ
    TUGLiShFilter uglistShFilter_;//シュートフィルタ
    TUGDrTeam ugdrTeam_;//チームドラム
    TUGDrShot ugdrShot_;//ショットドラム
    TUGLiMember uglistMember_;//メンバーリスト
    TUGLiSoubi uglistSoubi_;//そうび
    TUGLiSoubiAll uglistSoubiAll_;//そうびＡＬＬ
    TUGLiSyn uglistSyn_;//レシピ

    std::vector<TUGLiGen*> listvec_;
    SORTTEAMVECTOR teamvec_;
    std::map<s64, s64> teamshsetmap_;
    std::string inputname_;

    LONGVECTOR shsetvec_;

    LONGVECTOR swshsetvec_;
    LONGVECTOR swteamvec_;

  };
 
}
