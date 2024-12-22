#include "dbUGBgmList.h"
//=====================================================================================================
//	
//
//
//=====================================================================================================
namespace db
{
    const s32 INTRO_TIMER_NUM = 90;	// 90フレームだけイントロ再生

    //--------------------------------------------------------------------------------------------
    // 
    //--------------------------------------------------------------------------------------------
    TUGBgmList::TUGBgmList(TUIGenLayer* player)
        : TUGGen(player)
        , uglistAlbum_(player)
        , uglistTape_(player)
    {
    }

    //--------------------------------------------------------------------------------------------
    // 
    //--------------------------------------------------------------------------------------------
    TUGBgmList::~TUGBgmList()
    {
    }

    //--------------------------------------------------------------------------------------------
    // 
    //--------------------------------------------------------------------------------------------
    void TUGBgmList::MakeGroup(TUIObj* parent)
    {
        field_ = MakeObj();
        field_->o_SetParent(parent);

        stBGM_.Init();

        filter_ = MakeObj();
        filter_->z_SetPos_SafeLF(0, 0);
        filter_->o_SetObjFilterGrp(512, 512, 0xc0, 0x00, 0x00, 0x00);

        //ヘッダタイトル
        nowtitle_ = MakeObj();
        nowtitle_->o_SetParent(field_);
        nowtitle_->g_SetDepth(UIDP_05BUTTON);
        nowtitle_->z_SetPos_Full(mid::midFullDotL_Hf(), 34);
        nowtitle_->f_SetCenter(TRUE);
        //フッタタイトル
        nowBGM_ = MakeObj();
        nowBGM_->o_SetParent(field_);
        nowBGM_->g_SetDepth(UIDP_05BUTTON);
        nowBGM_->z_SetPos_FullBtm(mid::midFullDotL_Hf() - 156 - 4, -42);

        //タイトルリスト
        uglistAlbum_.MakeGroup(field_, TRUE);
        uglistAlbum_.Refresh();

        //ＢＧＭリスト
        uglistTape_.MakeGroup(field_, FALSE);

        SetTitle(0);


#ifdef __K_DEBUG_USER_DATA__
        kdebug::DebugUDManager::GetInstance()->GetAllBGM();
#endif
    }

    //--------------------------------------------------------------------------------------------
    // 
    //--------------------------------------------------------------------------------------------
    void TUGBgmList::RefreshIcon(void)
    {
        uglistTape_.RefreshIcon();
    }

    //--------------------------------------------------------------------------------------------
    // 
    //--------------------------------------------------------------------------------------------
    BOOL TUGBgmList::IsAct(void)
    {
        return (GetActRes() == enUGBgmAct_Select);
    }

    //--------------------------------------------------------------------------------------------
    // 
    //--------------------------------------------------------------------------------------------
    void TUGBgmList::Ready(void)
    {
        Ready(NGNUM);
    }
    //--------------------------------------------------------------------------------------------
    // 
    //--------------------------------------------------------------------------------------------
    void TUGBgmList::Ready(s64 mid_event)
    {
        mid_event_ = mid_event;
        stBGM_.Init();
        uglistAlbum_.Refresh(mid_event_);
        SetTitle(0);

        pmgEO_->mgSound_.StopBGM();
    }

    //--------------------------------------------------------------------------------------------
    // 
    //--------------------------------------------------------------------------------------------
    void TUGBgmList::Down()
    {
        pmgEO_->mgNSF_.PlayID(nsf_home);
    }


    //--------------------------------------------------------------------------------------------
    //ゲームタイトル変更
    //--------------------------------------------------------------------------------------------
    void TUGBgmList::SetTitle(s32 recordno)
    {
        s32 titleid = uglistAlbum_.records_[recordno]->GetCommonID();
        s32 iconno = pmgEO_->mgDt_.dtTitle_.GetDtPK(titleid, enDtTitle_iconno);

        if (stBGM_.titleid_ == titleid) return;

        stBGM_.titleid_ = titleid;
        stBGM_.iconno_ = iconno;
        bgmline_.clear();

        //ヘッダタイトル
        nowtitle_->f_MakeFont_Ht(pmgEO_->mgDt_.dtTitle_.GetStrPK(titleid, enDtTitleStr_name).c_str(), FC2_normal);
        uglistTape_.Refresh(titleid, mid_event_);

        stBGM_.mid_bgm_ = NGNUM;
    }

    //--------------------------------------------------------------------------------------------
    // 
    //--------------------------------------------------------------------------------------------
    void TUGBgmList::MvPage_BGM()
    {




        if (uglistAlbum_.IsAct())//タイトルりすと
        {
            stBGM_.wait_ = 0;
            //レコードタイトル検索
            SetTitle(uglistAlbum_.GetActIndex());
        }
        else if (uglistTape_.IsActRes(enUGLiAct_Record))//BGMりすと
        {
            //とりあえず再生か
            stBGM_.mid_bgm_ = uglistTape_.records_[uglistTape_.GetActIndex()]->GetCommonID();// bgmline_[uglistTape_.GetActIndex()];

            // 持ってる
            if (pmgEO_->mgCoU_.IsBGM(stBGM_.mid_bgm_))
            {
                stBGM_.mid_bgm_intro_ = NGNUM;
                stBGM_.introTimer_ = NGNUM;
            }
            // 持ってない
            else
            {
                stBGM_.mid_bgm_intro_ = stBGM_.mid_bgm_;
                stBGM_.introTimer_ = INTRO_TIMER_NUM;
            }

            // まず停止
            pmgEO_->mgNSF_.StopBGM();
            nowBGM_->f_MakeFont(pmgEO_->mgDt_.dtBGM_.GetStrLine(stBGM_.mid_bgm_, enDtBGMStr_name).c_str(), FC2_normal);
            nowBGM_->g_SetDraw(TRUE);

            //フリーズしないための待ち時間
            stBGM_.wait_ = 30;

        }
        else
        {
            //フリーズしないための待ち時間
            if ((stBGM_.wait_ > 0) && (stBGM_.mid_bgm_ != 0))
            {
                if (lib_num::AprTo0(&stBGM_.wait_))
                {
                    // 曲再生
                    pmgEO_->mgNSF_.PlayID(stBGM_.mid_bgm_);
                }
            }
        }



        // イントロタイマー
        if (stBGM_.mid_bgm_intro_ != NGNUM)
        {
            stBGM_.introTimer_--;
            if (stBGM_.introTimer_ < 0)
            {
                stBGM_.mid_bgm_intro_ = NGNUM;
                stBGM_.introTimer_ = NGNUM;
                pmgEO_->mgSound_.StopBGM();
                nowBGM_->g_SetDraw(FALSE);
            }
        }
    }

    //--------------------------------------------------------------------------------------------
    // 
    //--------------------------------------------------------------------------------------------
    void TUGBgmList::DoUpdate(BOOL draw_f)
    {
        nowtitle_->Update(draw_f);
        nowBGM_->Update(draw_f);
        uglistAlbum_.Update(draw_f);
        uglistTape_.Update(draw_f);

        if (draw_f == FALSE)
        {
            MvPage_BGM();
        }
    }

    //--------------------------------------------------------------------------------------------
    // 
    //--------------------------------------------------------------------------------------------
    void TUGBgmList::DoAction(void)
    {
        if (uglistTape_.IsActRes(enUGLiAct_Record))
        {
            SetAct(enUGBgmAct_Select);
        }
    }
}
