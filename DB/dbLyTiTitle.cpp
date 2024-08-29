#include "dbLyTiTitle.h"
#include "dbSnTitle.h"

namespace db
{
    enum enTitleStep
    {
        pg_Retry,//失敗したらここにもどる
        pg_VerCheck,//クライアントバージョン確認＆メンテチェック
        pg_MbgLogin,//モバゲーログイン
        pg_MbgAuth,//モバゲー認証
        pg_CoInfo,//おしらせアップデート
        pg_CoClmaster,//CLマスターアップデート
        pg_CoSvmaster,//SVマスターアップデート
        pg_CoBannerBmp,//バナーアップデート
        pg_CoEventBmp,//イベントアップデート
        pg_CoUser,//監督チェックアウト
        pg_CoWorld,//ワールドチェックアウト
        pg_Title,//タイトルタップ待ち
        pg_Kiyaku,//規約
        pg_KiyakuYN,//規約
        pg_GotoItunes,//itunesに跳ばす
    };
    enum enConnect
    {
        cn_VerCheck,
        cn_Kiyaku,
    };

    //コンストラクタ
    TLyTiTitle::TLyTiTitle(TSnTitle* pScene)
        :TLyTiGen(pScene)
        , ugfilter_(this)
        , ugbtn_dev_(this)
        , ugbtn_devdbg_(this)
        , ugbtn_start_(this)
        //, ugbtn_kuniodb_(this)
        , ugbtn_stg_(this)
        //, ugbtn_test2_(this)
        //, ugbtn_test1dbg_(this)
        //, ugbtn_test2dbg_(this)
        , ugbtn_oto_(this)
    {
        stTitle_.Init();
    }

    TLyTiTitle::~TLyTiTitle()
    {

    }

    void TLyTiTitle::DoLoad()
    {
        field_->o_SetObjGrp(tigrp_->title_bg_);
        field_->g_SetCenter(TRUE);
        copywrite_ = MakeObj();//コピーライト表示2014 Million Co.,Ltd / Marvelous Inc.
        copywrite_->f_MakeFont("著Ｍｉｌｌｉｏｎ＿Ｃｏ．，Ｌｔｄ＿／＿Ｍａｒｖｅｌｏｕｓ＿Ｉｎｃ．", FC2_btn_normal);
        copywrite_->z_SetPos(0, 88 - 16);
        copywrite_->f_SetCenter(TRUE);

        cver_ = MakeObj();//CLビルドVer
        s32 ver = mid::midGetClVersion();
        s32 v0 = (ver / 10000);
        s32 v1 = (ver / 100) % 100;
        s32 v2 = (ver % 100);
        std::string strver = "ＢＵＩＬＤ＿" + lib_str::IntToStr(v0) + "．" + lib_str::IntToStr(v1) + "．" + lib_str::IntToStr(v2);
        cver_->f_MakeFont_Mini(strver.c_str(), FC2_normal);
        cver_->z_SetPos(-40, 96);

        clmver_ = MakeObj();//CLマスターVer
        SetClmVerFont();
        svmver_ = MakeObj();//SVマスターVer
        SetSvmVerFont();

        ugfilter_.MakeGroup(field_);
        ugbtn_start_.MakeGroup(field_, ug_btn_ok2);
        ugbtn_start_.SetFont("ゲームスタート", FC2_btn_normal);
        ugbtn_start_.GetField()->z_SetPos(0, 40 - 16);


        ugbtn_stg_.MakeGroup(field_, ug_btn_ok);
        ugbtn_stg_.SetFont("ステージング", FC2_btn_normal);
        ugbtn_stg_.GetField()->z_SetPos(120, 40 + 16);


        ugbtn_dev_.MakeGroup(field_, ug_btn_ok);
        ugbtn_dev_.SetFont("かいはつさば", FC2_btn_normal);
        ugbtn_dev_.GetField()->z_SetPos(-120, 40 - 16);

        ugbtn_devdbg_.MakeGroup(field_, ug_btn_ok);
        ugbtn_devdbg_.SetFont("でばっぐさば", FC2_btn_normal);
        ugbtn_devdbg_.GetField()->z_SetPos(-120, 40 + 16);

        ugbtn_oto_.MakeGroup_Otoiawase(field_);
        ugbtn_stg_.SetDraw(!mid::midIsShinsa());
        ugbtn_dev_.SetDraw(!mid::midIsShinsa());
        ugbtn_devdbg_.SetDraw(!mid::midIsShinsa());

    }
    void TLyTiTitle::DoReady()
    {
        //本番をデフォに
        mid::midUseMarvKuniodbURL();
        mid::midSetSandbox(FALSE);
        stTitle_.cdn_f_ = TRUE;

        if (mid::midIsDXL())
        {
            pmgEO_->stSeq_.SetNoSv();
        }

        //マップクリア
        pmgEO_->mgCoU_.CleanAll();
    }
    void TLyTiTitle::DoDown()
    {

    }
    void TLyTiTitle::DoChangePage(s32 pageno)
    {
        stTitle_.postinit_f_ = FALSE;
    }

    void TLyTiTitle::SetClmVerFont()
    {
        std::string cvstr = "ＣＬＭＶＥＲ＿" + base::UnixtimeToDateString(mid::midGetClMasterVer() * 100, TRUE);
        clmver_->f_MakeFont_Mini(cvstr.c_str(), FC2_btn_normal);
        clmver_->z_SetPos(-160, 96);
    }

    void TLyTiTitle::SetSvmVerFont()
    {
        std::string svstr = "ＳＶＭＶＥＲ＿" + base::UnixtimeToDateString(mid::midGetSvMasterVer() * 100, TRUE);
        svmver_->f_MakeFont_Mini(svstr.c_str(), FC2_btn_normal);
        svmver_->z_SetPos(-160, 88);
    }

    //111111111111111111
    //クライアントバージョンチェック＆メンテチェック
    void TLyTiTitle::MvPage_VerCheck()
    {
        if (stTitle_.postinit_f_ == FALSE)
        {
            mid::midLog("MvPage_VerCheck");

            //モバゲーログインチェッククリア
            mid::midMobageClearLoginState();

            StartConnect(cn_VerCheck);
            pmgEO_->mgPo_.poVerCheck_.Ready_vercheck();
            stTitle_.postinit_f_ = TRUE;
        }
    }
    void TLyTiTitle::Connect_VerCheck()
    {
        if (pmgEO_->mgPo_.poVerCheck_.IsCodeZero())
        {
            //マスターZIPのバージョン
            stTitle_.re_clmver_ = pmgEO_->mgPo_.poVerCheck_.GetClMasterVer();
            stTitle_.re_svmver_ = pmgEO_->mgPo_.poVerCheck_.GetSvMasterVer();
            stTitle_.re_bnver_ = pmgEO_->mgPo_.poVerCheck_.GetBannerVer();
            stTitle_.re_evver_ = pmgEO_->mgPo_.poVerCheck_.GetEventVer();
            //サーバーとの時刻あわせ
            mid::midSetSvTime(pmgEO_->mgPo_.poVerCheck_.GetSvTime());

            switch (pmgEO_->mgPo_.poVerCheck_.GetConnect())
            {
            case 0://バージョン　サーバー＝クライアント　そのまま本番サーバーへ
                ChangePage(pg_MbgLogin);//モバゲーログインへ
                break;
            case 1:
            {
                //STRVECTOR strvec = { "アップルしんさ", "ステージングたいおう" };
                //SetHudMsg(FALSE, FALSE, &strvec);

                //バージョン　サーバー＜クライアント　接続先をレビューに切り替える
                mid::midUseMarvReviewURL();
                pmgEO_->stSeq_.SetReview();//ステージングへ
                stTitle_.cdn_f_ = FALSE;//そこのＺＩＰを取る
                stTitle_.review_f_ = TRUE;
                ChangePage(pg_MbgLogin);//モバゲーログインへ
            }
            break;
            case 2:
            {
                //バージョン　サーバー＞クライアント　itunesに跳ばす
                STRVECTOR strvec = { "さいしんバージョンが＿あります", "アプリを＿こうしんしてください" };
                SetHudMsg(FALSE, FALSE, &strvec);
                ChangePage(pg_GotoItunes);
            }
            break;
            }
        }

    }
    //222222222222222222222
    //モバゲーログイン
    void TLyTiTitle::MvPage_MbgLogin()
    {
        if (stTitle_.postinit_f_ == FALSE)
        {
            mid::midLog("MvPage_Login");
            mid::midMobageCheckLoginStatus();
            stTitle_.postinit_f_ = TRUE;
            frame_->SetAniStart_Login();//モーダル
        }

        s32 loginst = mid::midMobageGetLoginState();
        switch (loginst)
        {
        case 1://成功
            frame_->SetAniEnd();
            ChangePage(pg_MbgAuth);//認証へ
            break;
        case 2://ログイン失敗
            frame_->SetAniEnd();
            mid::midAlert("ログインエラー", "ログインできませんでした。");
            ChangePage(pg_Retry);//リトライへ
            stTitle_.retry_f_ = TRUE;
            break;
        }
    }

    //33333333333333333333333
    void TLyTiTitle::MvPage_MbgAuth()
    {
        if (stTitle_.postinit_f_ == FALSE)
        {
            mid::midLog("MvPage_MbgAuth");
            mid::midMobageStartAuth();
            stTitle_.postinit_f_ = TRUE;
            frame_->SetAniStart_Auth();//モーダル
        }


        s32 authst = mid::midMobageGetAuthState();
        switch (authst)
        {
        case 1://成功
            frame_->SetAniEnd();
            ChangePage(pg_CoInfo);//おしらせ取得へ
            break;
        case 2://ログイン失敗
            frame_->SetAniEnd();
            mid::midAlert("Mobage認証エラー", "認証できませんでした。");
            ChangePage(pg_Retry);//リトライへ
            stTitle_.retry_f_ = TRUE;
            break;
        }
    }

    //お知らせ
    void TLyTiTitle::MvPage_CoInfo()
    {
        if (stTitle_.postinit_f_ == FALSE)
        {
            //一応空にするか
            mid::midLog("MvPage_CoInfo");
            StartCheckOut(coInfo);
            pmgEO_->mgCoInfo_.StartConnect();
            stTitle_.postinit_f_ = TRUE;
        }
    }
    void TLyTiTitle::CheckOut_CoInfo()
    {
        ChangePage(pg_CoClmaster);//Zip取得へ
    }
    //クライアントマスター
    void TLyTiTitle::MvPage_CoClmaster()
    {
        if (stTitle_.postinit_f_ == FALSE)
        {

            s32 clmver = mid::midGetClMasterVer();
//                 clmver = 0;//強制更新

            //更新必要
            if ((clmver < stTitle_.re_clmver_) || stTitle_.review_f_)
            {
                mid::midLog("MvPage_coClmaster");
                StartCheckOut(coClmaster);
                pmgEO_->mgCoClmaster_.StartConnect(stTitle_.re_clmver_, stTitle_.cdn_f_);
                stTitle_.postinit_f_ = TRUE;
                stTitle_.reloadclm_f_ = TRUE;
            }
            else
            {
                mid::midLog("MvPage_GetClMaster_None");
                ChangePage(pg_CoSvmaster);
                return;
            }
        }
    }
    void TLyTiTitle::CheckOut_CoClmaster()
    {
        SetClmVerFont();
        ChangePage(pg_CoSvmaster);//Svマスターチェックアウトへ
    }
    //サーバーマスター
    void TLyTiTitle::MvPage_CoSvmaster()
    {
        if (stTitle_.postinit_f_ == FALSE)
        {
            s32 svmver = mid::midGetSvMasterVer();
            //      svmver = 0;//強制更新
            //更新必要
            if ((svmver < stTitle_.re_svmver_) || stTitle_.review_f_)
            {
                mid::midLog("MvPage_coSvmaster");
                StartCheckOut(coSvmaster);
                pmgEO_->mgCoSvmaster_.StartConnect(stTitle_.re_svmver_, stTitle_.cdn_f_);
                stTitle_.postinit_f_ = TRUE;
            }
            else
            {
                mid::midLog("MvPage_CoMaster_Load");
                ChangePage(pg_CoBannerBmp);
                return;
            }
        }
    }
    void TLyTiTitle::CheckOut_CoSvmaster()
    {
        SetSvmVerFont();
        ChangePage(pg_CoBannerBmp);//バナー更新へ
    }

    //バナー画像
    void TLyTiTitle::MvPage_CoBannerBmp()
    {
        if (stTitle_.postinit_f_ == FALSE)
        {
            s32 bnver = mid::midGetBannerVer();
            //      bnver = 0;
            //      svmver = 0;//強制更新
            //更新必要
            if ((bnver < stTitle_.re_bnver_) || stTitle_.review_f_)
            {
                mid::midLog("MvPage_banner");
                StartCheckOut(coBannerBmp);
                pmgEO_->mgCoBannerBmp_.StartConnect(stTitle_.re_bnver_, stTitle_.cdn_f_);
                stTitle_.postinit_f_ = TRUE;
            }
            else
            {
                mid::midLog("MvPage_CoBannerBmp_Load");
                ChangePage(pg_CoEventBmp);
                return;
            }
        }
    }
    void TLyTiTitle::CheckOut_CoBannerBmp()
    {
        //SetSvmVerFont();
        ChangePage(pg_CoEventBmp);//イベント画像チェックアウト
    }


    //イベント画像
    void TLyTiTitle::MvPage_CoEventBmp()
    {
        if (stTitle_.postinit_f_ == FALSE)
        {
            s32 evver = mid::midGetEventVer();
            //      bnver = 0;
            //      svmver = 0;//強制更新
            //更新必要
            if ((evver < stTitle_.re_evver_) || stTitle_.review_f_)
            {
                mid::midLog("MvPage_event");
                StartCheckOut(coEventBmp);
                pmgEO_->mgCoEventBmp_.StartConnect(stTitle_.re_evver_, stTitle_.cdn_f_);
                stTitle_.postinit_f_ = TRUE;
            }
            else
            {
                mid::midLog("MvPage_CoEventBmp_Load");
                ChangePage(pg_CoUser);
                return;
            }
        }
    }
    void TLyTiTitle::CheckOut_CoEventBmp()
    {
        //SetSvmVerFont();
        pmgUI_->LoadEvIcon();
        ChangePage(pg_CoUser);//監督チェックアウトへ
    }

    void TLyTiTitle::MvPage_CoUser()
    {
        if (stTitle_.postinit_f_ == FALSE)
        {
            mid::midLog("MvPage_CoUser");
            StartCheckOut(coUser);
            pmgEO_->mgCoU_.StartConnect();//ユーザーチェックアウト
            stTitle_.postinit_f_ = TRUE;
        }
    }
    void TLyTiTitle::CheckOut_CoUser()
    {
        pmgEO_->mgCoU_.SetMyPK();
        ChangePage(pg_CoWorld);
    }

    void TLyTiTitle::MvPage_CoWorld()
    {
        if (stTitle_.postinit_f_ == FALSE)
        {
            mid::midLog("MvPage_CoWorld");
            StartCheckOut(coWorld);
            pmgEO_->mgCoU_.StartConnectSub("co_world");//追加チェックアウト
            stTitle_.postinit_f_ = TRUE;
        }
    }
    void TLyTiTitle::CheckOut_CoWorld()
    {
        //マスター読み込み
        pmgEO_->mgCoM_.LoadDt();

        if (pmgEO_->mgCoU_.IsTutoClear(enTutoBitType_Kiyaku))
        {
            ChangePage(pg_Title);
        }
        else
        {
            ChangePage(pg_Kiyaku);
        }
    }

    void TLyTiTitle::MvPage_Kiyaku()
    {
        if (stTitle_.postinit_f_ == FALSE)
        {
            mid::midLog("MvPage_Kiyaku");
            mid::midModalPanel_Kiyaku(&pmgEO_->mgCoM_.mdm_Kiyaku_.model_);
            stTitle_.postinit_f_ = TRUE;
        }
        else if (mid::midIsDrawModalPanel() == FALSE)
        {
            STRVECTOR strvec = { "りようきやくに", "どういしますか？" };
            SetHudMsg(TRUE, FALSE, &strvec);
            ChangePage(pg_KiyakuYN);
        }
    }
    void TLyTiTitle::MvPage_KiyakuYN()
    {
        if (IsHudOK())//おーけー
        {
            StartConnect(cn_Kiyaku);
            pmgEO_->mgPo_.poTuto_.Ready_SetBit(enTutoBitType_Kiyaku);
        }
        else if (IsHudCancel())//きゃんせる
        {
            ChangePage(pg_Retry);//リトライへ
        }
    }
    void TLyTiTitle::Connect_Kiyaku()
    {
        ChangePage(pg_Title);
    }
    void TLyTiTitle::MvPage_Title()
    {
        if (stTitle_.postinit_f_ == FALSE)
        {
            if (pmgEO_->stSeq_.IsNoSv() == FALSE)
            {
                mid::midLog("MvPage_Title");
                INTVECTOR infovec = pmgEO_->mgCoInfo_.GetSortInfo();
                mid::midModalPanel_Info(&pmgEO_->mgCoInfo_.mdm_Info_.model_, &infovec);
                //        stTitle_.waitdialog_f_ = TRUE;
            }

            stTitle_.postinit_f_ = TRUE;
        }
        //    else if (stTitle_.waitdialog_f_)
        //    {
        //      if (mid::midIsDrawModalPanel())
        //      {
        //        stTitle_.waitdialog_f_ = FALSE;
        //      }
        //    }
        else if (mid::midIsDrawModalPanel() == FALSE)
        {
            if (stTitle_.filter_f_ == FALSE)
            {
                //        if (mid::midIsIOS() == FALSE)
                if (mid::midIsDXL())
                {
                    if (field_->sc_IsCamEnter())
                    {
                        //一度フィルタフラッシュを使っておかないと暗転が壊れる（おそらくOPENGLのカラーフィルタ原因）
                        stTitle_.filter_f_ = TRUE;
                        ugfilter_.OnFilter(2, FALSE, TRUE);
                    }
                }
                else
                {
                    stTitle_.filter_f_ = TRUE;
                    ugfilter_.OnFilter(2, FALSE, TRUE);
                }
            }
            else
            {
                if (ugfilter_.IsAct())
                {
                    //アップデート反映するためここで読み込みでOKか
                    //とりあえずデータのみ（画像はのちほど）
                    if (stTitle_.reloadclm_f_) pmgEO_->LoadData();

                    //マスター読み込み
                    if (pmgEO_->stSeq_.IsNoSv())
                    {
                        pmgEO_->mgCoM_.LoadDt();
                        pmgEO_->mgCoInfo_.LoadDt();
                    }


                    
                mid::midSetGooglePlayID(pmgEO_->mgDt_.dtClient_.IsAccessGooglePlayByIOS(), pmgEO_->mgDt_.dtClient_.GooglePlayID().c_str());
                  
                  



#ifdef __K_DEBUG_USER_DATA__
                    kdebug::DebugUDManager::GetInstance()->Setup();
#endif	// #ifdef __K_DEBUG_USER_DATA__

                    //所持スター数の確認
                    mid::midMobageGetBalance();

                    //ホームへ
                    pScene_->GoToHome();
                }
            }
        }
    }

    //バージョンが古いのでitunesにとばす
    void TLyTiTitle::MvPage_GotoItunes()
    {
        if (IsHudOK())
        {
            mid::midJumpiTunes();
            ChangePage(pg_Retry);
        }
    }
    void TLyTiTitle::MvPage_Retry()
    {

        //ウインドウズ起動
        if (pmgEO_->stSeq_.IsNoSv())
        {
            ChangePage(pg_Title);
            return;
        }

        if (stTitle_.postinit_f_ == FALSE)
        {
            if (mid::midMobageGetLoginState() != 0)
            {
                mid::midMobageInit();
            }
            ugbtn_start_.GetField()->b_SetBtn(TRUE);
            stTitle_.postinit_f_ = TRUE;
        }

        //ゲームスタート
        if (ugbtn_start_.IsAct())
        {

            mid::midUseMarvKuniodbURL();
//            if (mid::midIsADR())
//            {
//                //        mid::midSetSandbox(FALSE);
////                mid::midUseMarvReviewURL();
//                mid::midSetSandbox(TRUE);//審査終わるまで
//                stTitle_.cdn_f_ = FALSE;//ＣＤＮ
//                stTitle_.cdn_f_ = TRUE;
//            }
//            else
            {
                mid::midSetSandbox(FALSE);
                stTitle_.cdn_f_ = TRUE;//ＣＤＮ
            }
            //      mid::midSetSandbox(FALSE);

            if (stTitle_.retry_f_ || mid::midIsADR()) mid::midMobageInit();
            ChangePage(pg_VerCheck);
            ugbtn_start_.GetField()->b_SetBtn(FALSE);
        }

        if (ugbtn_oto_.IsAct())
        {
            //      mid::midPlayJGL(0, 0.5);
            //if (mid::midIsDebug())
            //{
            //  mid::midStartAuthorizationUI();
            //}
            //else
            {
                mid::midOtoiawase();//おといあわせ
            }
        }



        if (mid::midIsShinsa() == FALSE)
        {
            if (ugbtn_dev_.IsAct())//marv開発さば
            {
                mid::midUseMarvDevURL();
                mid::midSetSandbox(TRUE);
                stTitle_.cdn_f_ = FALSE;
                mid::midMobageInit();
                ChangePage(pg_VerCheck);
            }
            else if (ugbtn_devdbg_.IsAct())//marv開発デバッグ
            {
                mid::midUseMarvDevDbgURL();
                mid::midSetSandbox(TRUE);
                stTitle_.cdn_f_ = FALSE;
                mid::midMobageInit();
                ChangePage(pg_VerCheck);
            }
            else if (ugbtn_stg_.IsAct())//てすとさば
            {
                mid::midUseMarvStgURL();//marvステージング
                mid::midSetSandbox(mid::midIsStgSandbox());
                stTitle_.cdn_f_ = FALSE;
                mid::midMobageInit();
                ChangePage(pg_VerCheck);
            }

        }
    }

    void TLyTiTitle::DoConnect(BOOL rqok_f)
    {
        if (rqok_f)//ＯＫのみ
        {
            //code0
            if (pmgEO_->mgPo_.IsCodeZero())
            {
                switch (stLy_.connect_)
                {
                case cn_VerCheck: Connect_VerCheck(); break;
                case cn_Kiyaku: Connect_Kiyaku(); break;
                }
            }
            else
            {
                //エラーメッセージ
                EndConnectNG(pmgEO_->mgPo_.GetCode(), pmgEO_->mgPo_.GetMsg().c_str());
                //通信エラー
                ChangePage(pg_Retry);//リトライへ
            }
        }
        else
        {
            //通信エラー
            ChangePage(pg_Retry);//リトライへ
        }
    }

    void TLyTiTitle::DoCheckOut(BOOL rqok_f)
    {
        if (rqok_f)
        {
            switch (stLy_.checkout_)
            {
            case coInfo: CheckOut_CoInfo(); break;
            case coClmaster: CheckOut_CoClmaster(); break;
            case coSvmaster: CheckOut_CoSvmaster(); break;
            case coBannerBmp: CheckOut_CoBannerBmp(); break;
            case coEventBmp: CheckOut_CoEventBmp(); break;
            case coUser: CheckOut_CoUser(); break;
            case coWorld: CheckOut_CoWorld(); break;
            }
        }
        else
        {
            ChangePage(pg_Retry);//リトライへ
        }
    }

    void TLyTiTitle::DoUpdate(BOOL draw_f)
    {
        ugfilter_.Update(draw_f);
        copywrite_->Update(draw_f);
        svmver_->Update(draw_f);
        clmver_->Update(draw_f);
        cver_->Update(draw_f);

        if (stLy_.pageNo_ != pg_KiyakuYN)
        {
            ugbtn_oto_.Update(draw_f);

            ugbtn_start_.Update(draw_f);
            if (mid::midIsShinsa() == FALSE)
            {
                ugbtn_dev_.Update(draw_f);
                ugbtn_devdbg_.Update(draw_f);
                ugbtn_stg_.Update(draw_f);
            }
        }


        //ugbtn_dev_.Update(draw_f);
        //ugbtn_kuniodb_.Update(draw_f);
        //ugbtn_test2_.Update(draw_f);
        //ugbtn_test1dbg_.Update(draw_f);
        //ugbtn_test2dbg_.Update(draw_f);
        //ughud_.Update(draw_f);

        if (draw_f == FALSE)
        {
            switch (stLy_.pageNo_)
            {
            case pg_VerCheck: MvPage_VerCheck(); break;
            case pg_MbgLogin: MvPage_MbgLogin(); break;
            case pg_MbgAuth: MvPage_MbgAuth(); break;
            case pg_Title: MvPage_Title(); break;
            case pg_CoInfo: MvPage_CoInfo(); break;
            case pg_CoClmaster: MvPage_CoClmaster(); break;
            case pg_CoSvmaster: MvPage_CoSvmaster(); break;
            case pg_CoBannerBmp: MvPage_CoBannerBmp(); break;
            case pg_CoEventBmp: MvPage_CoEventBmp(); break;
            case pg_CoUser: MvPage_CoUser(); break;
            case pg_CoWorld: MvPage_CoWorld(); break;
            case pg_Kiyaku: MvPage_Kiyaku(); break;
            case pg_KiyakuYN: MvPage_KiyakuYN(); break;
            case pg_Retry: MvPage_Retry(); break;
            case pg_GotoItunes: MvPage_GotoItunes(); break;
            }
        }
    }
}
