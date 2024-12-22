#include "dbLyTechno.h"
#include "dbLyBarGen.h"

namespace db
{

    enum enRoomList
    {
        ml_BuyTechno,
        ml_BuyStar,
        ml_Info,
        ml_BuyLog,
        ml_UseLog,
        mlEND,
    };

    enum enPage
    {

        pg_Menu,
        pg_BuyTechno,
        pg_BuyStar,
        pg_Help,
        pg_BuyStarOK,
        pg_BuyStarTransaction,
        pg_BuyStarClose,
        pg_BuyStarCancel,
        pg_BuyStarResult,
        pg_Info,
        pg_BuyLog,
        //pg_BuyLogEnd,
        pg_UseLog,
        //pg_UseLogEnd,
    };

    enum enConnect
    {
        cn_GetBalance_BuyStarReady,
        cn_GetBalance_BuyTechnoResult,
        cn_GetBalance_BuyStarResult,
        cn_BankCreate,
        cn_Transaction,
        cn_BankClose,
        cn_BankCancel,
        cn_BuyLogRedy,
        cn_BuyLog,
        cn_UseLogReady,
        cn_UseLog,
    };

    //コンストラクタ
    TLyTechno::TLyTechno(TScene* pScene, TUIFrame* frame, TLyBarGen* lybar) :TUILayer(pScene, frame)
        , uglistMenu_(this)
        , uglistSpTechno_(this)
        , uglistBuyLog_(this)
        , uglistUseLog_(this)
        , ugbtn_back_tec_(this)
        , ugbtn_ok_tec_(this)
        //, ugbtn_mid_tec_(this)
        , ugbtn_next_tec_(this)
        , ugbtn_prev_tec_(this)
        , ugbtn_tsh_tec_(this)
        , ugallcredit_(this)
    {
        lybar_ = lybar;
        stTechno_.Init();
        stLy_.hedderlayer_f_ = TRUE;//ヘッダ用レイヤ
    }

    TLyTechno::~TLyTechno()
    {

    }
    void TLyTechno::DoLoad()
    {
        field_->g_SetCenter(TRUE);//フィルタはフィルタサイズが小さいためセンタリングが正常に効かない
        field_->o_SetObjFilterGrp(512, 512, 0xFF, 0, 0, 0);
        field_->g_SetAlp(FALSE, 0x80);
        field_->g_SetDepth(UIDP_06BUTTONHI);
        field_->g_SetDraw(FALSE);

        ugallcredit_.MakeGroup(field_);

        uglistMenu_.MakeGroup(field_, FALSE);
        uglistMenu_.SetDepth(UIDP_07BUTTONTOP);
        uglistSpTechno_.MakeGroup(field_, FALSE);
        uglistSpTechno_.SetDepth(UIDP_07BUTTONTOP);
        uglistBuyLog_.MakeGroup(field_, FALSE);
        uglistBuyLog_.SetDepth(UIDP_07BUTTONTOP);
        uglistUseLog_.MakeGroup(field_, FALSE);
        uglistUseLog_.SetDepth(UIDP_07BUTTONTOP);
        uglistSpTechno_.Refresh();

        listvec_.clear();
        listvec_.push_back(&uglistMenu_);
        listvec_.push_back(&uglistSpTechno_);
        listvec_.push_back(&uglistBuyLog_);
        listvec_.push_back(&uglistUseLog_);


        ugbtn_back_tec_.MakeGroup_FtPos(field_, 0, "もどる");
        ugbtn_ok_tec_.MakeGroup_FtPos(field_, 1, "かう");
        //ugbtn_mid_tec_.MakeGroup_FtPos(field_, 2, "もどる");
        ugbtn_next_tec_.MakeGroup_FtPos(field_, 3, "つぎのページ");
        ugbtn_prev_tec_.MakeGroup_FtPos(field_, 4, "まえのページ");
        ugbtn_tsh_tec_.MakeGroup_FtPosTokusyou(field_);
        ugbtn_back_tec_.SetDepth(UIDP_09MSGBOX);
        ugbtn_ok_tec_.SetDepth(UIDP_09MSGBOX);
        //ugbtn_mid_tec_.SetDepth(UIDP_09MSGBOX);
        ugbtn_next_tec_.SetDepth(UIDP_09MSGBOX);
        ugbtn_prev_tec_.SetDepth(UIDP_09MSGBOX);



        //メニューレコード
        for (s32 i = 0; i < mlEND; ++i)
        {
            TUGRcNormal* newrecord = (TUGRcNormal*)uglistMenu_.MakeObj_Record();
            switch (i)
            {
            case ml_BuyTechno:
                if (mid::midIsADR())
                {
                    newrecord->ugname_.SetNameIcon("モバコインこうにゅう", ugi_BuyMbCoin);
                }
                else
                {
                    newrecord->ugname_.SetNameIcon("テクノこうにゅう", ugi_BuyTechno);
                }
                break;
            case ml_BuyStar:newrecord->ugname_.SetNameIcon("スターチャージ", ugi_BuyStar);  break;
            case ml_Info:newrecord->ugname_.SetNameIcon("スターしょうさい", ugi_CheckStar); break;
            case ml_BuyLog:newrecord->ugname_.SetNameIcon("スターチャージログ", ugi_StarLog); break;
            case ml_UseLog:newrecord->ugname_.SetNameIcon("スターしょうひログ", ugi_StarLog); break;
            }
        }
    }

    //入店時の処理
    void TLyTechno::DoReady()
    {
        mid::midMobageGetBalance();
        stTechno_.Init();
        //ugbtn_back_tec_.SetFont("もどる");
        //ugbtn_ok_tec_.SetFont("かう");
        //ugbtn_prev_tec_.SetFont("まえのページ");
        //ugbtn_next_tec_.SetFont("つぎのページ");
        ugbtn_back_tec_.SetDraw(TRUE);
        field_->g_SetDraw(TRUE);

        ugallcredit_.Refresh();
    }
    void TLyTechno::DoDown()
    {
        field_->g_SetDraw(FALSE);
        lybar_->CloseHeader();
    }

    void TLyTechno::DrawList(TUGLiGen* drawlist)
    {
        for (s32 i = 0; i < listvec_.size(); ++i)
        {
            listvec_[i]->SetDraw(FALSE);
        }
        if (drawlist != NULL)
        {
            drawlist->SetDraw(TRUE);
            //drawlist->SetZeroPos();
        }
        ResetObj();
        ugbtn_back_tec_.SetDraw(TRUE);
    }
    void TLyTechno::ResetObj()
    {
        ugbtn_prev_tec_.SetDraw(FALSE);
        ugbtn_next_tec_.SetDraw(FALSE);
        ugbtn_ok_tec_.SetDraw(FALSE);
        ugbtn_tsh_tec_.SetDraw(FALSE);
    }
    void TLyTechno::SwitchLogPageBtn()
    {
        ugbtn_prev_tec_.SetDraw((stTechno_.logpage_ > 0));
        ugbtn_next_tec_.SetDraw((stTechno_.logpage_ < stTechno_.endpage_));
    }
    void TLyTechno::DoChangePage(s32 pageno)
    {
        switch ((enPage)pageno)
        {
        case db::pg_Menu:
            DrawList(&uglistMenu_);
            ugallcredit_.Refresh();
            uglistMenu_.ChangeListEnd(FALSE);
            ugbtn_tsh_tec_.SetDraw(FALSE);
            break;
        case db::pg_BuyTechno:
            mid::midMobageIAP();
            break;
        case db::pg_BuyStar:
            DrawList(&uglistSpTechno_);
            uglistSpTechno_.ChangeListEnd(FALSE);
            ugbtn_back_tec_.SetDraw(TRUE);
            ugbtn_tsh_tec_.SetDraw(TRUE);
            break;
        case db::pg_Help:
        {
            //資金決済法
            s32 ostype = mid::midIsIOS()
                ? 1
                : 2;
            mid::midModalPanel_HelpList(&pmgEO_->mgCoM_.mdm_Help_.model_, ostype);//パネル表示
        }
        break;
        case db::pg_BuyStarOK:
            DrawList(&uglistSpTechno_);
            uglistSpTechno_.ChangeListEnd(TRUE);
            ugbtn_tsh_tec_.SetDraw(TRUE);
            break;
        case db::pg_BuyStarTransaction:
            DrawList(&uglistSpTechno_);
            break;
        case db::pg_BuyStarClose:
            DrawList(&uglistSpTechno_);
            break;
        case db::pg_BuyStarResult:
            DrawList(&uglistSpTechno_);
            break;
        case db::pg_Info:
            DrawList(&uglistMenu_);
            uglistMenu_.uglistend_.SetDataInfoStar();
            uglistMenu_.ChangeListEnd(TRUE);
            break;
        case db::pg_BuyLog:
            DrawList(&uglistBuyLog_);
            uglistBuyLog_.Refresh();
            SetHudEnd();
            break;
            //case db::pg_BuyLogEnd:
            //  uglistBuyLog_.ChangeListEnd(TRUE);//リストエンドに切り替える
            //  ResetObj();
            //  break;
        case db::pg_UseLog:
            DrawList(&uglistUseLog_);
            stTechno_.bstar_f_ = TRUE;
            stTechno_.fstar_f_ = TRUE;
            uglistUseLog_.Refresh(stTechno_.bstar_f_, stTechno_.fstar_f_);
            SetHudEnd();
            break;
            //case db::pg_UseLogEnd:
            //  //uglistUseLog_.ChangeListEnd(TRUE);//リストエンドに切り替える
            //  ResetObj();
            //  break;
        }
        //ページ切り替え
        stTechno_.pageno_ = pageno;
    }

    //メニュー
    void TLyTechno::MvPage_Menu()
    {
        if (frame_->ugfooter_->IsHideBtn() == FALSE) frame_->ugfooter_->HideBtn(TRUE);

        if (mid::midIsRefreshBalance())
        {
            stTechno_.wait_c_ = 120;
            stTechno_.wait_c2_ = 300;
        }
        if (stTechno_.wait_c_ > 0) if (lib_num::AprTo0(&stTechno_.wait_c_))ugallcredit_.Refresh();
        if (stTechno_.wait_c2_ > 0) if (lib_num::AprTo0(&stTechno_.wait_c2_))ugallcredit_.Refresh();

        if (ugbtn_back_tec_.IsAct())
        {
            Down();//もどる
        }
        else if (ugbtn_tsh_tec_.IsAct())
        {
            mid::midTokusyouhou();//特賞法
        }
        else if (uglistMenu_.IsAct())
        {
            switch ((enRoomList)uglistMenu_.GetActIndex())
            {
            case db::ml_BuyTechno:
                ChangePage(pg_BuyTechno);
                break;
            case db::ml_BuyStar:
                stTechno_.techno_ = mid::midResBalance();
                ChangePage(pg_BuyStar);
                break;
            case db::ml_Info:ChangePage(pg_Info); break;
            case db::ml_BuyLog:
                stTechno_.logpage_ = 0;
                Post_BuyLogReady();
                break;
            case db::ml_UseLog:
                stTechno_.logpage_ = 0;
                Post_UseLogReady();
                break;
            }
        }
    }

    //テクノ購入
    void TLyTechno::MvPage_BuyTechno()
    {
        ChangePage(pg_Menu);//メニューに戻る
    }

    //スター購入
    void TLyTechno::MvPage_BuyStar()
    {
        if (ugbtn_back_tec_.IsAct())
        {
            ChangePage(pg_Menu);//メニューに戻る
        }
        else if (ugbtn_tsh_tec_.IsAct())
        {
            mid::midTokusyouhou();//特賞法
        }
        else if (uglistSpTechno_.IsAct())//次のページへ
        {
            s32 mid_spsyouhin = uglistSpTechno_.GetCommonID();
            if (mid_spsyouhin == NGNUM)
            {
                ChangePage(pg_Help);//ヘルプ
            }
            else
            {
                //インベントリ商品番号
                stTechno_.mid_supply_ = pmgEO_->mgCoM_.mdm_SpSyouhin_.GetPKDt(mid_spsyouhin, mdm_spsyouhin_itemID);
                uglistSpTechno_.SetListEnd(uglistSpTechno_.GetActIndex());
                ChangePage(pg_BuyStarOK);
            }

        }
    }
    //ヘルプ
    void TLyTechno::MvPage_Help()
    {
        //モーダルパネルが消えたらメニューに戻る
        if (mid::midIsDrawModalPanel() == FALSE)
        {
            ChangePage(pg_BuyStar);//メニューに戻る
        }
    }

    void TLyTechno::MvPage_BuyStarOK()
    {
        if (ugbtn_back_tec_.IsAct())
        {
            ChangePage(pg_BuyStar);//戻る
        }
        else if (ugbtn_tsh_tec_.IsAct())
        {
            mid::midTokusyouhou();//特賞法
        }
        else if (uglistSpTechno_.uglistend_.IsAct())
        {
            Post_BankCreate();
        }

    }
    void TLyTechno::MvPage_BuyStarTransaction()
    {
        switch (mid::midResTransaction())
        {
        case NGNUM:
            //処理中
            break;
        case 0:
            //OK
            Post_BankClose();
            break;
        case 1:
            //キャンセル
            //        Post_BankCancel();//キャンセルのゲームサーバー通信は不要っぽい
            ChangePage(pg_BuyStarResult);
            SetHudEnd();
            break;
        case 2:
            //NG
            //エラーメッセージ
            EndConnectNG(-1, "トランザクションエラーです");
            ChangePage(pg_Menu);
            break;
        }
    }
    void TLyTechno::MvPage_BuyStarClose()
    {
        Post_BankClose();
    }
    void TLyTechno::MvPage_BuyStarCancel()
    {
        Post_BankCancel();
    }
    void TLyTechno::MvPage_BuyStarResult()
    {
        mid::midMobageGetBalance();//テクノ更新
        ChangePage(pg_Menu);//メニューに戻る
    }

    void TLyTechno::MvPage_Info()
    {
        if (ugbtn_back_tec_.IsAct())//おーけー
        {
            ChangePage(pg_Menu);//メニューに戻る
        }
    }

    //スター購入履歴
    void TLyTechno::MvPage_BuyLog()
    {
        if (ugbtn_back_tec_.IsAct())
        {
            ChangePage(pg_Menu);//メニューに戻る
        }
        //else if (uglistBuyLog_.IsActRes(enUGLiAct_Record))
        //{
        //  //獲得詳細
        //  ChangePage(pg_BuyLogEnd);
        //}
        else if ((stTechno_.logpage_ > 0) && ugbtn_prev_tec_.IsAct())//前ページ
        {
            //アイテム＿かくとくりれき
            --stTechno_.logpage_;
            Post_BuyLog();
        }
        else if (ugbtn_next_tec_.IsAct())//次ページ
        {
            //アイテム＿かくとくりれき
            ++stTechno_.logpage_;
            Post_BuyLog();
        }
    }
    void TLyTechno::MvPage_BuyLogEnd()
    {
        if (ugbtn_back_tec_.IsAct())
        {
            ChangePage(pg_BuyLog);//もどる
        }
    }
    //スター使用履歴
    void TLyTechno::MvPage_UseLog()
    {
        if (ugbtn_back_tec_.IsAct())
        {
            ChangePage(pg_Menu);//メニューに戻る
        }
        //else if (uglistUseLog_.IsActRes(enUGLiAct_Record))
        //{
        //  //獲得詳細
        //  ChangePage(pg_UseLogEnd);
        //}
        else if ((stTechno_.logpage_ > 0) && ugbtn_prev_tec_.IsAct())//前ページ
        {
            //アイテム＿かくとくりれき
            --stTechno_.logpage_;
            Post_UseLog();
        }
        else if (ugbtn_next_tec_.IsAct())//次ページ
        {
            //アイテム＿かくとくりれき
            ++stTechno_.logpage_;
            Post_UseLog();
        }
    }

    ///////////////////////////////////////
    void TLyTechno::Post_GetBalance_BuyStarReady()
    {
        //テクノ取得
        StartConnect(cn_GetBalance_BuyStarReady);
        mid::midMobageGetBalance();
    }
    void TLyTechno::Post_GetBalance_BuyTechnoResult()
    {
        //テクノ取得
        StartConnect(cn_GetBalance_BuyTechnoResult);
        mid::midMobageGetBalance();
    }
    void TLyTechno::Post_GetBalance_BuyStarResult()
    {
        //テクノ取得
        StartConnect(cn_GetBalance_BuyStarResult);
        mid::midMobageGetBalance();
    }
    void TLyTechno::Post_BankCreate()
    {
        if (pmgEO_->stSeq_.IsNoSv())
        {
            Connect_BankCreate();
        }
        else
        {
            StartConnect(cn_BankCreate);
            pmgEO_->mgPo_.poBank_.Ready_create(stTechno_.mid_supply_, stTechno_.techno_);
        }
    }
    void TLyTechno::Post_Transaction()
    {
        mid::midMobageContinueTransaction(tid_.c_str());

        ChangePage(pg_BuyStarTransaction);
    }
    void TLyTechno::Post_BankClose()
    {
        if (pmgEO_->stSeq_.IsNoSv())
        {
            Connect_BankClose();
        }
        else
        {
            StartConnect(cn_BankClose);
            pmgEO_->mgPo_.poBank_.Ready_close(tid_.c_str());
        }
    }
    void TLyTechno::Post_BankCancel()
    {
        if (pmgEO_->stSeq_.IsNoSv())
        {
            Connect_BankCancel();
        }
        else
        {
            StartConnect(cn_BankCancel);
            pmgEO_->mgPo_.poBank_.Ready_cancel(tid_.c_str());
        }
    }

    //POSTかくとくログ（最初）
    void TLyTechno::Post_BuyLogReady()
    {
        if (pmgEO_->stSeq_.IsNoSv())
        {
            Connect_BuyLogReady();
        }
        else
        {
            StartConnect(cn_BuyLogRedy);
            //通信はページの更新と変わらない
            pmgEO_->mgPo_.poHome_.Ready_buystarlog(stTechno_.logpage_);
        }
    }
    //POSTかくとくログページの更新
    void TLyTechno::Post_BuyLog()
    {
        if (pmgEO_->stSeq_.IsNoSv())
        {
            Connect_BuyLog();
        }
        else
        {
            StartConnect(cn_BuyLog);
            pmgEO_->mgPo_.poHome_.Ready_buystarlog(stTechno_.logpage_);
        }
    }
    //POST使用ログ（最初）
    void TLyTechno::Post_UseLogReady()
    {
        if (pmgEO_->stSeq_.IsNoSv())
        {
            Connect_UseLogReady();
        }
        else
        {
            StartConnect(cn_UseLogReady);
            pmgEO_->mgPo_.poHome_.Ready_usestarlog(stTechno_.logpage_, stTechno_.bstar_f_, stTechno_.fstar_f_);//とりあえず課金無料両方取ってくる
        }
    }
    //POST使用ログ
    void TLyTechno::Post_UseLog()
    {
        if (pmgEO_->stSeq_.IsNoSv())
        {
            Connect_UseLog();
        }
        else
        {
            StartConnect(cn_UseLog);
            pmgEO_->mgPo_.poHome_.Ready_usestarlog(stTechno_.logpage_, stTechno_.bstar_f_, stTechno_.fstar_f_);
        }
    }

    void TLyTechno::SetEndPage()
    {
        stTechno_.endpage_ = pmgEO_->mgPo_.GetValue("endpage");
    }

    //接続
    void TLyTechno::Connect_GetBalance_BuyStarReady()
    {
        stTechno_.techno_ = mid::midResBalance();
        ChangePage(pg_BuyStar);
    }
    void TLyTechno::Connect_GetBalance_BuyTechnoResult()
    {
        stTechno_.techno_ = mid::midResBalance();
        ChangePage(pg_Menu);
    }
    void TLyTechno::Connect_GetBalance_BuyStarResult()
    {
        stTechno_.techno_ = mid::midResBalance();
        ChangePage(pg_Menu);
    }
    void TLyTechno::Connect_BankCreate()
    {
        //トランザクションＩＤ
        tid_ = pmgEO_->mgPo_.GetValueStr("tid");
        Post_Transaction();
        //    SetHudEnd();
    }
    void TLyTechno::Connect_Transaction()
    {
        Post_BankClose();
        ChangePage(pg_BuyStarClose);
        SetHudEnd();
    }
    void TLyTechno::Connect_BankClose()
    {

        if (mid::midIsShinsa()
//        || mid::midIsADR()
        )
        {
            s32 price = pmgEO_->mgCoM_.mdm_Supply_.GetPKDt(stTechno_.mid_supply_, mdm_supply_price);
            std::string name = pmgEO_->mgCoM_.mdm_Supply_.GetPKDtStr(stTechno_.mid_supply_, mdm_supply_name_c32);
            mid::midSetPartyTrack(price, name.c_str());
        }

        ChangePage(pg_BuyStarResult);
        SetHudEnd();
    }
    void TLyTechno::Connect_BankCancel()
    {
        ChangePage(pg_Menu);
        SetHudEnd();
    }
    //接続獲得ログ（最初）
    void TLyTechno::Connect_BuyLogReady()
    {
        SetEndPage();
        //１ページ目と全体数読み込めたら表示ページに移る
        ChangePage(pg_BuyLog);
        SwitchLogPageBtn();
        SetHudEnd();
    }
    //接続獲得ログ
    void TLyTechno::Connect_BuyLog()
    {
        //リストリフレッシュ
        uglistBuyLog_.Refresh();
        SwitchLogPageBtn();
        SetHudEnd();
    }
    //接続使用ログ（最初）
    void TLyTechno::Connect_UseLogReady()
    {
        SetEndPage();
        //１ページ目と全体数読み込めたら表示ページに移る
        ChangePage(pg_UseLog);
        SwitchLogPageBtn();
        SetHudEnd();
    }
    //接続使用ログ
    void TLyTechno::Connect_UseLog()
    {
        uglistUseLog_.Refresh(stTechno_.bstar_f_, stTechno_.fstar_f_);
        SwitchLogPageBtn();
        SetHudEnd();
        //リストリフレッシュ
    }

    //接続完了
    void TLyTechno::DoConnect(BOOL rqok_f)
    {
        if (rqok_f)//ＯＫのみ
        {
            BOOL post_f = FALSE;
            switch (stLy_.connect_)
            {
            case cn_GetBalance_BuyStarReady: Connect_GetBalance_BuyStarReady(); break;
            case cn_GetBalance_BuyTechnoResult: Connect_GetBalance_BuyTechnoResult(); break;
            case cn_GetBalance_BuyStarResult: Connect_GetBalance_BuyStarResult(); break;
            case cn_Transaction:
                Connect_Transaction();
                break;
            default: post_f = TRUE; break;
            }

            if (post_f)
            {
                //code0
                if (pmgEO_->mgPo_.IsCodeZero())
                {
                    switch (stLy_.connect_)
                    {
                    case cn_BankCreate: Connect_BankCreate(); break;
                    case cn_BankClose: Connect_BankClose(); break;
                    case cn_BuyLogRedy: Connect_BuyLogReady(); break;
                    case cn_BuyLog: Connect_BuyLog(); break;
                    case cn_UseLogReady: Connect_UseLogReady(); break;
                    case cn_UseLog: Connect_UseLog(); break;
                    }
                }
                else
                {
                    ChangePage(pg_Menu);
                    //エラーメッセージ
                    EndConnectNG(pmgEO_->mgPo_.GetCode(), pmgEO_->mgPo_.GetMsg().c_str());
                }
            }

        }
        else
        {
            //通信エラー
            ChangePage(pg_Menu);
            //エラーメッセージ
            EndConnectNG(pmgEO_->mgPo_.GetCode(), pmgEO_->mgPo_.GetMsg().c_str());
        }
    }

    void TLyTechno::DoUpdate(BOOL draw_f)
    {
        ugallcredit_.Update(draw_f);
        uglistMenu_.Update(draw_f);
        uglistSpTechno_.Update(draw_f);
        uglistBuyLog_.Update(draw_f);
        uglistUseLog_.Update(draw_f);

        ugbtn_tsh_tec_.Update(draw_f);
        ugbtn_back_tec_.Update(draw_f);
        ugbtn_ok_tec_.Update(draw_f);
        //ugbtn_mid_tec_.Update(draw_f);
        ugbtn_next_tec_.Update(draw_f);
        ugbtn_prev_tec_.Update(draw_f);

        if (draw_f == FALSE)
        {
            switch (stTechno_.pageno_)
            {
            case pg_Menu:MvPage_Menu(); break;
            case pg_Info:MvPage_Info(); break;
            case pg_BuyTechno:MvPage_BuyTechno(); break;
            case pg_BuyStar:MvPage_BuyStar(); break;
            case pg_Help:MvPage_Help(); break;
            case pg_BuyStarOK: MvPage_BuyStarOK(); break;
            case pg_BuyStarTransaction: MvPage_BuyStarTransaction(); break;
            case pg_BuyStarClose: MvPage_BuyStarClose(); break;
            case pg_BuyStarCancel: MvPage_BuyStarCancel(); break;
            case pg_BuyStarResult: MvPage_BuyStarResult(); break;
            case pg_BuyLog:MvPage_BuyLog(); break;
            case pg_UseLog:MvPage_UseLog(); break;
            }
        }
    }

}
