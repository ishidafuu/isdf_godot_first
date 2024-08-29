#include "dbLyHmCBBA.h"
#include "dbSnHome.h"
#include "dbUGPrev.h"

namespace db
{

    enum enPage
    {
        pg_Visit,
        pg_Menu,
        pg_ShEdit,
        pg_RenameShot,
        pg_ShotInv,
        pg_ShotInvEnd,
        pg_BaraMenu,
        pg_Bara,
        pg_ShFilter,
        pg_ShPrev,
        pg_BuyShPrev,
        pg_BaraEnd,
        pg_BuySozaiMenu,
        pg_BuySozaiType,
        pg_BuySozai,
        //pg_BuySozaiEnd,
        pg_SellMenu,
        pg_SellShot,
        pg_Out,
    };

    enum enBBAMenu
    {
        bbm_ShEdit,
        bbm_ShotInv,
        bbm_Bara,
        bbm_SellShot,
        bbm_BuySozai,
        bbmEND,
    };


    enum enConnect
    {
        cn_LockShot,
        cn_RenameShot,
        cn_AddShotInv,
        //cn_BaraLast,
        cn_BaraAll,
        cn_SellShot,
        cn_BuySozai,
    };

    const s32 INVMAX = 1000;

    //コンストラクタ
    TLyHmCBBA::TLyHmCBBA(TSnHome* pScene)
        :TLyHmGen(pScene)
        , ugbba_(this)
        , ugdrShot_(this)
        , uglistMenu_(this)
        , uglistShotInv_(this)
        , uglistBuySozai_(this)
        , uglistBaraMenu_(this)
        , uglistSellMenu_(this)
        , uglistBuySozaiMenu_(this)
        , uglistSellShot_(this)
        , uglistBuySozaiType_(this)
        , uglistShFilter_(this)
        , ugshot_(this)
    {
        stCBBA_.Init();
    }

    TLyHmCBBA::~TLyHmCBBA()
    {
    }

    void TLyHmCBBA::DoLoad()
    {
        //ばばあフィールド全生成
        ugbba_.MakeGroup(field_, &pScene_->lyHmCenter_->ugkantoku_,
            &pScene_->lyHmCenter_->ugcap_,
            &pScene_->lyHmCenter_->ugcl_bba_,
            &pScene_->lyHmCenter_->ugdoor_bba_);

        //メニューリスト
        uglistMenu_.MakeGroup(field_, FALSE);
        //ばらメニューリスト
        uglistBaraMenu_.MakeGroup(field_, FALSE);
        //売却メニューリスト
        uglistSellMenu_.MakeGroup(field_, FALSE);

        //素材購入価格帯
        uglistBuySozaiMenu_.MakeGroup(field_, FALSE);
        //素材購入ジャンル
        uglistBuySozaiType_.MakeGroup(field_, FALSE);

        //シュートインベントリリスト
        uglistShotInv_.MakeGroup(field_, FALSE);
        //素材購入リスト
        uglistBuySozai_.MakeGroup(field_, FALSE);
        //シュート売却リスト
        uglistSellShot_.MakeGroup(field_, FALSE);
        //シュートフィルター
        uglistShFilter_.MakeGroup(field_, TRUE);

        //ドラム
        ugdrShot_.MakeGroup(field_, FALSE, &uglistShFilter_);

        listvec_.clear();
        listvec_.push_back(&uglistMenu_);
        listvec_.push_back(&uglistBaraMenu_);
        listvec_.push_back(&uglistSellMenu_);
        listvec_.push_back(&uglistShotInv_);
        listvec_.push_back(&uglistBuySozaiMenu_);
        listvec_.push_back(&uglistBuySozai_);
        listvec_.push_back(&uglistSellShot_);
        listvec_.push_back(&uglistBuySozaiType_);
        listvec_.push_back(&uglistShFilter_);

        uglistShotInv_.Refresh(enShopType_ShotInv);
        uglistBuySozaiMenu_.Refresh();
        uglistBuySozaiType_.Refresh();

        //購入表示用シュートカード
        ugshot_.MakeGroup(field_);
        ugshot_.SetDraw(FALSE);
        ugshot_.SetDepth(UIDP_04HEADER);
        ugshot_.MakeUra();

        //メニューリスト各レコード
        for (s32 i = 0; i < bbmEND; ++i)
        {
            TUGRcNormal* newrecord = (TUGRcNormal*)uglistMenu_.MakeObj_Record();
            switch (i)
            {
            case bbm_ShEdit:newrecord->ugname_.SetNameIcon("しゅーとかんり", ugi_Ball); break;
            case bbm_ShotInv:newrecord->ugname_.SetNameIcon("しゅーとわく＿ついか", ugi_ShotInv); break;
            case bbm_Bara:newrecord->ugname_.SetNameIcon("ミックス＿ばらし", ugi_Bara);  break;
            case bbm_BuySozai:newrecord->ugname_.SetNameIcon("そざい＿こうにゅう", ugi_BuySozai);  break;
            case bbm_SellShot:newrecord->ugname_.SetNameIcon("しゅーと＿ばいきゃく", ugi_SellSozai);  break;
            }

        }

        //シュートインベントリリスト各レコード7

        const s32 BARAMENUNUM = 2;
        for (s32 i = 0; i < BARAMENUNUM; ++i)
        {
            TUGRcNormal* newrecord = (TUGRcNormal*)uglistBaraMenu_.MakeObj_Record();
            s32 price = pmgEO_->mgCoM_.mdm_Supply_.GetPKDt(SPL_BARASTAR + i, mdm_supply_price);
            s32 pricetype = pmgEO_->mgCoM_.mdm_Supply_.GetPKDt(SPL_BARASTAR + i, mdm_supply_priceType);

#ifdef __K_MAKE_BBA_PRICE_DOWN__
			s32 salePrice = 0;
			s32 salePercent = 0;

			salePrice = 90;//price - price * 0.1; // ここをどこかのテーブルから取得するようにする
			salePercent = 100 - (f32)(salePrice) / (f32)(price) * 100;

			// 価格が違うときは割引表記
			if (price != salePrice)
			{
				std::string saleStr = "ＬＩＳＴ＿ＰＲＩＣＥ＿";
				saleStr += lib_str::IntToStr(price);
				saleStr += "＿ＳＡＬＥ＿";
				saleStr += lib_str::IntToStr(salePercent);
				saleStr += "％＿ＯＦＦ";
				newrecord->SetSubName(saleStr.c_str());

				price = salePrice;
			}
#endif

            if (pricetype == enPriceType_Star)
            {
                newrecord->SetValue_Star(price);
                newrecord->ugname_.SetNameIcon("スターでばらす", ugi_Bara);
            }
            else
            {
                newrecord->SetValue_Todd(price);
                newrecord->ugname_.SetNameIcon("トッドでばらす", ugi_Bara);
            }
        }

        const s32 SELLMENUNUM = 5;
        for (s32 i = 0; i < SELLMENUNUM; ++i)
        {
            TUGRcNormal* newrecord = (TUGRcNormal*)uglistSellMenu_.MakeObj_Record();
            switch (i)
            {
            case 0:newrecord->ugname_.SetNameIcon("Ｄランクいかから", ugi_RarityD); break;
            case 1:newrecord->ugname_.SetNameIcon("Ｃランクいかから", ugi_RarityC); break;
            case 2:newrecord->ugname_.SetNameIcon("Ｂランクいかから", ugi_RarityB); break;
            case 3:newrecord->ugname_.SetNameIcon("Ａランクいかから", ugi_RarityA);  break;
            case 4:newrecord->ugname_.SetNameIcon("すべてのシュートから", ugi_RarityAll); break;
            }
        }

    }

    //入店時の処理
    void TLyHmCBBA::DoReady()
    {
        stCBBA_.Init();
        ugbba_.Ready();
        DrawList(NULL);
        ugbtn_ok_->SetFont("かう！");
        ugbtn_back_->SetFont("もどる");
        ugbtn_prev_->SetFont("まえのページ");
        ugbtn_next_->SetFont("つぎのページ");
        ugbtn_mid_->SetFont("プレビュー");
        uglistShFilter_.Ready(enShotFilTypeS_All);
        ugdrShot_.ReadyFilter();
        pmgEO_->stSeq_.PushNeedOmakase();//おまかせリフレッシュフラグ
    }
    void TLyHmCBBA::DoDown()
    {
        ugbba_.Down();
        pScene_->lyHmCenter_->Ready();//退出
    }

    void TLyHmCBBA::DrawList(TUGLiGen* drawlist)
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
    }
    void TLyHmCBBA::ResetObj()
    {
        ugdrShot_.SetDraw(FALSE);//ドラム消しとく
        ugdrShot_.SelectDrumEnd(FALSE);//カード末端けす
        ugshot_.SetDraw(FALSE);
        ugbtn_ok_->SetDraw(FALSE);
        ugbtn_prev_->SetDraw(FALSE);
        ugbtn_next_->SetDraw(FALSE);
        ugbtn_mid_->SetDraw(FALSE);
        ugbtn_tsh_->SetDraw(FALSE);

        ugbtn_back_->SetDraw(TRUE);
    }

    BOOL TLyHmCBBA::RefreshPageBtn(BOOL init_f, s32 mv)
    {
        BOOL res = FALSE;

        if (init_f)
        {
            stCBBA_.listpageNo_ = 0;
            ugbtn_prev_->SetFont("まえのページ", FC2_normal);
            ugbtn_next_->SetFont("つぎのページ", FC2_normal);
        }
        else
        {
            if (mv > 0)
            {
                if (stCBBA_.listpageNo_ < uglistSellShot_.GetMaxPage())
                {
                    ++stCBBA_.listpageNo_;
                    res = TRUE;
                }
            }
            else if (mv < 0)
            {
                if (stCBBA_.listpageNo_ > 0)
                {
                    --stCBBA_.listpageNo_;
                    res = TRUE;
                }
            }
        }


        if (uglistSellShot_.GetMaxPage() == 0)
        {
            ugbtn_next_->SetDraw(FALSE);
            ugbtn_prev_->SetDraw(FALSE);
        }
        else
        {
            ugbtn_next_->SetDraw(stCBBA_.listpageNo_ < uglistSellShot_.GetMaxPage());
            ugbtn_prev_->SetDraw(stCBBA_.listpageNo_ > 0);
        }

        return res;
    }


    void TLyHmCBBA::RefreshSellBtn(BOOL init_f)
    {
        if (uglistSellShot_.GetSelectNum() == 0)//選択なしになった
        {
            ugshot_.SetDraw(FALSE);
            ugbba_.ugserifu_.SetDraw(TRUE);
            RefreshPageBtn(init_f, 0);
            ugbtn_ok_->SetDraw(FALSE);
        }
        else if (uglistSellShot_.GetIDShotLast() != 0)//何か選択
        {
            ugshot_.SetShotDtSell(uglistSellShot_.GetIDShotLast());
            ugshot_.SetUra(FALSE);
            ugshot_.SetLRPos(TRUE);
            ugshot_.SetDraw(TRUE);
            ugbtn_prev_->SetFont("くわしく", FC2_normal);
            ugbtn_next_->SetFont("プレビュー", FC2_normal);
            ugbtn_prev_->SetDraw(TRUE);
            ugbtn_next_->SetDraw(TRUE);
            ugbba_.ugserifu_.SetDraw(FALSE);
        }
        else
        {
            ugshot_.SetDraw(FALSE);
            ugbba_.ugserifu_.SetDraw(TRUE);
            ugbtn_prev_->SetDraw(FALSE);
            ugbtn_next_->SetDraw(FALSE);
        }
    }


    void TLyHmCBBA::DoChangePage(s32 pageno)
    {
        pScene_->lyHmCenter_->RefreshSoukoNum();
        switch (pageno)
        {
        case db::pg_Visit:
            DrawList(&uglistMenu_);//メニュー
            break;
        case db::pg_Menu:
            DrawList(&uglistMenu_);//メニュー
            //全シュート
            ugbba_.SetBBAMsg(enBBAMsg_Menu);
            ugbba_.ugserifu_.SetDraw(TRUE);
            break;
        case db::pg_ShEdit:
            DrawList(&uglistShFilter_);
            ugbba_.ugserifu_.SetDraw(FALSE);
            ugdrShot_.SetDraw(TRUE);
            //管理ページのみおきにいりとなまえ
            ugbtn_prev_->SetFont("くわしく", FC2_normal);
            ugbtn_prev_->SetDraw(TRUE);
            ugbtn_next_->SetFont("プレビュー");
            ugbtn_next_->SetDraw(TRUE);
            ugbtn_mid_->SetFont("おきにいり", FC2_normal);
            ugbtn_mid_->SetDraw(TRUE);
            ugbtn_ok_->SetFont("なまえつける", FC2_normal);
            ugbtn_ok_->SetDraw(TRUE);
            //全シュート
            uglistShFilter_.Ready(enShotFilTypeS_All);
            ugdrShot_.ReadyFilter();
            ResetPrevBtn();
            break;
        case db::pg_RenameShot:
            DrawList(NULL);
            frame_->SetHudInput(SHOTNAMEMAX, FALSE);
            ugbtn_back_->SetDraw(FALSE);
            break;
            //シュートインベントリ
        case db::pg_ShotInv:
            DrawList(&uglistShotInv_);//シュートインベントリリスト表示
            ugbba_.SetBBAMsg(enBBAMsg_ShotInv);
            uglistShotInv_.ChangeListEnd(FALSE);//インベントリリストエンドけす
            ugbtn_tsh_->SetDraw(TRUE);
            ugr_star_->Refresh();
            break;
        case db::pg_ShotInvEnd:
            DrawList(&uglistShotInv_);
            uglistShotInv_.ChangeListEnd(TRUE);//インベントリリストエンド表示
            ugbba_.SetBBAMsg(enBBAMsg_OK);
            ugbtn_tsh_->SetDraw(TRUE);
            break;
            //シュートばらし
        case db::pg_BaraMenu:
            DrawList(&uglistBaraMenu_);//シュートインベントリリスト表示
            ugbtn_tsh_->SetDraw(TRUE);
            ugr_star_->RefreshMid();
            ugr_todd_->Refresh();
            break;
        case db::pg_Bara:
            DrawList(NULL);
            ugbba_.SetBBAMsg(enBBAMsg_Bara);
            ugdrShot_.SetDraw(TRUE);//シュートドラム表示
            ugbtn_ok_->SetFont("これをばらす", FC2_normal);
            ugbtn_ok_->SetDraw(TRUE);
            ugbtn_prev_->SetFont("ならびかえ", FC2_normal);
            ugbtn_prev_->SetDraw(TRUE);
            ugbtn_next_->SetFont("くわしく", FC2_normal);
            ugbtn_next_->SetDraw(TRUE);
            ugbtn_mid_->SetFont("プレビュー");
            ugbtn_mid_->SetDraw(TRUE);
            BaraPriceType();
            ResetPrevBtn();
            break;
        case db::pg_ShPrev:
        {
            TPrevParam param;
            param.id_shot = stCBBA_.id_shot_;
            param.nonehavemixbtn_f = TRUE;

            DrawList(NULL);
            ugbba_.ugserifu_.SetDraw(FALSE);
            SetHudPrev(param);
            ugbtn_prev_->SetDraw(FALSE);
            ugbtn_next_->SetDraw(FALSE);
            ugbtn_back_->SetDraw(FALSE);
            ugbtn_ok_->SetDraw(FALSE);
        }
        break;
        case db::pg_ShFilter:
            DrawList(&uglistShFilter_);
            ugbba_.ugserifu_.SetDraw(FALSE);
            ugdrShot_.SetDraw(TRUE);
            break;
        case db::pg_BaraEnd:
            DrawList(NULL);//シュートリスト表示
            ugdrShot_.SetDraw(TRUE);//シュートドラム表示
            //ugdrShot_.SelectDrumEnd(TRUE);//カード末端表示
            ugbba_.SetBBAMsg(enBBAMsg_OK);
            ugbtn_ok_->SetFont("ばらす！", FC2_normal);
            ugbtn_ok_->SetDraw(TRUE);
            ugbtn_tsh_->SetDraw(TRUE);
            //ugr_star_->Refresh();
            BaraPriceType();
            break;
            //シュート売却
        case db::pg_SellMenu:
            DrawList(&uglistSellMenu_);//シュートリスト表示
            ugbba_.SetBBAMsg(enBBAMsg_SellMenu);
            ugr_todd_->Refresh();
            break;
        case db::pg_SellShot:
            DrawList(&uglistSellShot_);//売却シュートリスト表示
            //RefreshPageBtn(TRUE, FALSE);
            ugbba_.SetBBAMsg(enBBAMsg_Sell);
            ugr_todd_->Refresh();
            ugbtn_ok_->SetFont("うる！", FC2_normal);
            RefreshSellBtn(TRUE);
            break;
            //シュート販売価格帯
        case db::pg_BuySozaiMenu:
            DrawList(&uglistBuySozaiMenu_);//シュートリスト表示
            ugbba_.SetBBAMsg(enBBAMsg_BuyMenu);
            break;
            //シュート販売
        case db::pg_BuySozaiType:
            DrawList(&uglistBuySozaiType_);//販売素材リスト表示
            ugbba_.SetBBAMsg(enBBAMsg_Buy);
            ugshot_.SetDraw(FALSE);
            BuyPriceType();
            break;
        case db::pg_BuySozai:
            DrawList(&uglistBuySozai_);//販売素材リスト表示
            ugbtn_ok_->SetFont("かう！", FC2_normal);
            ugbtn_prev_->SetFont("くわしく", FC2_normal);
            ugbtn_next_->SetFont("プレビュー", FC2_normal);
            BuyPriceType();
            break;
        case db::pg_BuyShPrev:
        {
            TPrevParam param;
            param.sozaivec = &buyprevvec_;
            param.nonehavemixbtn_f = TRUE;

            DrawList(NULL);
            ugbba_.ugserifu_.SetDraw(FALSE);
            SetHudPrev(param);
            ugbtn_back_->SetDraw(FALSE);
        }
        break;
        case db::pg_Out:
            UndrawBtn();
            ugbba_.SetBBAMsg(enBBAMsg_Out);
            break;
        }
    }


    //入店
    void TLyHmCBBA::MvPage_Visit()
    {
        if (ugbtn_back_->IsAct())
        {
            ChangePage(pg_Out);
        }
        else if (ugbba_.ugserifu_.IsActRes(enUGSerifuAct_WaitEnd))
        {
            ChangePage(pg_Menu);
            ugbba_.SetBBAMsg(enBBAMsg_Menu);
            ugdrShot_.StopSwipe(FALSE);//スワイプきくように
        }
        else if (uglistMenu_.IsAct())//次のページへ
        {
            ActListMenu();
        }
    }
    //メニュー
    void TLyHmCBBA::MvPage_Menu()
    {
        //出る
        if (ugbtn_back_->IsAct())
        {
            ChangePage(pg_Out);
        }
        else if (uglistMenu_.IsAct())//次のページへ
        {
            ActListMenu();
        }
    }

    //シュート管理
    void TLyHmCBBA::MvPage_ShEdit()
    {
        ResetPrevBtn();

        if (ugbtn_back_->IsAct())//おーけー
        {
            //POST：シュートロック
            if (CheckPostLockShot(stCBBA_.shfilterPage_) == FALSE)
            {
                ChangePage(stCBBA_.shfilterPage_);
                ugbba_.ugserifu_.SetDraw(TRUE);
            }
        }
        else if (ugbtn_prev_->IsAct())//裏面
        {
            stCBBA_.id_shot_ = ugdrShot_.GetIDShot();//有効な中の番号
            ugdrShot_.ura_.SetShotUraDt(stCBBA_.id_shot_);
            ugdrShot_.RevUra();
        }
        else if (ugbtn_next_->IsAct())//プレビュー
        {
            stCBBA_.id_shot_ = ugdrShot_.GetIDShot();
            ChangePage(pg_ShPrev);
        }
        else if (ugbtn_mid_->IsAct())//シュートロック
        {
            stCBBA_.id_shot_ = ugdrShot_.GetIDShot();//有効な中の番号
            pmgEO_->mgCoU_.SwitchShLock(stCBBA_.id_shot_);
            ugdrShot_.RefreshSelected();//鍵リフレッシュ
            //とりあえず40ぐらいの保存でいったん送る
            if (pmgEO_->mgCoU_.GetShLockNum() > 40)
            {
                //POST：シュートロック
                CheckPostLockShot(NGNUM);
            }
        }
        else if (ugbtn_ok_->IsAct())//リネーム
        {
            stCBBA_.id_shot_ = ugdrShot_.GetIDShot();//有効な中の番号
            //POST：シュートロック
            if (CheckPostLockShot(pg_RenameShot) == FALSE)
            {
                ChangePage(pg_RenameShot);
            }
        }
        else if (uglistShFilter_.IsActRes2(enUGLiShFilter_Filter))
        {
            ugdrShot_.ReadyFilter();
            //POST：シュートロック
            CheckPostLockShot(NGNUM);
        }
        else if (uglistShFilter_.IsActRes2(enUGLiShFilter_Sort))
        {
            ugdrShot_.ReadySort();
        }
    }
    //リネーム
    void TLyHmCBBA::MvPage_RenameShot()
    {
        if (frame_->IsHudCancel())
        {
            ChangePage(pg_ShEdit);//もどる
        }
        else if (frame_->IsHudOK())//確定ボタンにかえる
        {
            inputname_ = frame_->HudOutputString();
            //POSTリネーム
            Post_RenameShot();
        }
    }

    //シュートインベントリ
    void TLyHmCBBA::MvPage_ShotInv()
    {
        //もどる
        if (ugbtn_back_->IsAct())
        {
            ChangePage(pg_Menu);
        }
        else if (uglistShotInv_.IsAct())//次のページへ
        {
            //インベントリ商品番号
            stCBBA_.mid_spsyouhin_ = uglistShotInv_.GetCommonID();
            uglistShotInv_.SetListEnd(uglistShotInv_.GetActIndex());
            ChangePage(pg_ShotInvEnd);
        }
    }
    //インベントリ購入
    void TLyHmCBBA::MvPage_ShotInvEnd()
    {
        //もどる
        if (ugbtn_back_->IsAct())
        {
            ChangePage(pg_ShotInv);
        }
        else if (uglistShotInv_.uglistend_.IsAct())
        {
            SetHudMsgStarSupply(pmgEO_->mgCoM_.mdm_SpSyouhin_.GetPKDt(stCBBA_.mid_spsyouhin_, mdm_spsyouhin_itemID), FALSE);
        }
        else if (IsHudOK())
        {
            //POST：シュートインベントリ拡張
            Post_AddShotInv();
        }
    }

    //ばらしメニュー
    void TLyHmCBBA::MvPage_BaraMenu()
    {
        //もどる
        if (ugbtn_back_->IsAct())
        {
            ChangePage(pg_Menu);
        }
        else if (uglistBaraMenu_.IsAct())//次のページへ
        {
            stCBBA_.barastar_f_ = (uglistBaraMenu_.GetActIndex() == 0);
            //stCBBA_.baraall_f_ = TRUE;// (uglistBaraMenu_.GetActIndex() == 1);//すべてばらすか
            //ばらし可能フィルタ
            uglistShFilter_.Ready(enShotFilTypeS_Mixed);
            ugdrShot_.ReadyFilter();
            ChangePage(pg_Bara);
        }
    }

    //シュートばらしひとつはずす
    void TLyHmCBBA::MvPage_Bara()
    {
        ResetPrevBtn();
        //もどる
        if (ugbtn_back_->IsAct())
        {
            ChangePage(pg_BaraMenu);
        }
        else if (ugbtn_prev_->IsAct())//ならびかえ
        {
            ChangePage(pg_ShFilter);
            stCBBA_.shfilterPage_ = stLy_.lastpageNo_;
        }
        else if (ugbtn_next_->IsAct())//裏面
        {
            ugdrShot_.ura_.SetShotUraDt(ugdrShot_.GetIDShot());
            ugdrShot_.RevUra();
        }
        else if (ugbtn_mid_->IsAct())//プレビュー
        {
            stCBBA_.id_shot_ = ugdrShot_.GetIDShot();//有効な中の番号
            ChangePage(pg_ShPrev);
        }
        else  if (ugbtn_ok_->IsAct())//次のページへ
        {
            stCBBA_.id_shot_ = ugdrShot_.GetIDShot();
            //if (stCBBA_.baraall_f_)
            {
                if (pmgEO_->mgCoU_.GetShotNum_Emp() < pmgEO_->mgCoU_.mdShot_.GetPKDt(ugdrShot_.GetIDShot(), mdshot_useslot))
                {
                    ugbba_.SetBBAMsg(enBBAMsg_NoneEmp);//空き枠足りない
                }
                else
                {
                    ugdrShot_.ReadyType_BaraAll(ugdrShot_.GetIDShot());
                }
            }
            //else
            //{
            //  if (pmgEO_->mgCoU_.GetShotNum_Emp() < 1)
            //  {
            //    ugbba_.SetBBAMsg(enBBAMsg_NoneEmp);//空き枠足りない
            //  }
            //  else
            //  {
            //    ugdrShot_.ReadyType_BaraLast(ugdrShot_.GetIDShot());
            //  }
            //}

            ChangePage(pg_BaraEnd);
        }
    }
    void TLyHmCBBA::MvPage_BaraEnd()
    {
        //もどる
        if (ugbtn_back_->IsAct())
        {
            //合成済み（ばらし用）
            uglistShFilter_.Ready(enShotFilTypeS_Mixed);
            ugdrShot_.ReadyFilter();
            ChangePage(pg_Bara);
        }
        else if (ugbtn_next_->IsAct())//裏面
        {
            ugdrShot_.ura_.SetShotUraDt(ugdrShot_.GetIDShot());
            ugdrShot_.RevUra();
        }
        else if (ugbtn_ok_->IsAct())
        {

            //スター消費メッセージ
            //if (stCBBA_.baraall_f_)
            {

                STRVECTOR strvec = { "ミックスを＿すべて＿はずします", "よろしいですか？" };
                if (stCBBA_.barastar_f_)
                {
                    s32 price = pmgEO_->mgCoM_.mdm_Supply_.GetPKDt(SPL_BARASTAR, mdm_supply_price);
                    SetHudMsgStar(&strvec, price);
                }
                else
                {
                    s32 price = pmgEO_->mgCoM_.mdm_Supply_.GetPKDt(SPL_BARATODD, mdm_supply_price);
                    SetHudMsgTodd(&strvec, price, FALSE);
                }
            }
            //else
            //{
            //  STRVECTOR strvec = { "ミックスを＿ひとつ＿はずします", "よろしいですか？" };
            //  if (barastar_f_)
            //  {
            //    SetHudMsgStar(&strvec, price);
            //  }
            //  else
            //  {
            //    SetHudMsgTodd(&strvec, price, FALSE);
            //  }
            //}
        }
        else if (IsHudOK())
        {
            Post_BaraAll();

            ////POST：シュートばらしひとつ
            //if (stCBBA_.baraall_f_)
            //{
            //  Post_BaraAll();
            //}
            //else
            //{
            //  Post_BaraLast();
            //}
        }
    }

    //シュート売却メニュー
    void TLyHmCBBA::MvPage_SellMenu()
    {
        //もどる
        if (ugbtn_back_->IsAct())
        {
            ChangePage(pg_Menu);
        }
        else if (uglistSellMenu_.IsAct())//次のページへ
        {
            stCBBA_.sellfilter_ = (enUGLiSellShotType)uglistSellMenu_.GetActIndex();
            //売却可能フィルタ
            switch (stCBBA_.sellfilter_)
            {
            case db::enUGLiSellShotType_UnderD: uglistShFilter_.Ready(enShotFilTypeS_SellD); break;
            case db::enUGLiSellShotType_UnderC: uglistShFilter_.Ready(enShotFilTypeS_SellC); break;
            case db::enUGLiSellShotType_UnderB:uglistShFilter_.Ready(enShotFilTypeS_SellB); break;
            case db::enUGLiSellShotType_UnderA:uglistShFilter_.Ready(enShotFilTypeS_SellA); break;
            case db::enUGLiSellShotType_All: uglistShFilter_.Ready(enShotFilTypeS_SellALL); break;
            }

            ugdrShot_.ReadyFilter();

            if (uglistSellShot_.Refresh(0, &(uglistShFilter_.id_shotvec_)))
            {
                ChangePage(pg_SellShot);
            }
            else
            {
                ugbba_.SetBBAMsg(enBBAMsg_NoneSell);
            }
        }
    }
    void TLyHmCBBA::MvPage_SellShot()
    {
        //もどる
        if (ugbtn_back_->IsAct())
        {
            ChangePage(pg_SellMenu);
        }
        else if (ugbtn_ok_->IsAct() && (uglistSellShot_.GetSelectNum() > 0))
        {
            //売却するシュートリスト
            sellidvec_ = uglistSellShot_.GetIDSellShot();

            s32 sellprice = 0;
            for (s32 i = 0; i < sellidvec_.size(); ++i)
            {
                sellprice += pmgEO_->mgCoU_.GetShotPrice(sellidvec_[i]);
            }
            STRVECTOR strvec = { lib_str::IntToStr(sellidvec_.size()) + "この＿しゅーとを", "ばいきゃくします" };
            SetHudMsgTodd(&strvec, sellprice, TRUE);
        }
        else if (IsHudOK())//確認ＯＫ
        {
            //POST：シュート売却
            Post_SellShot();
        }
        else if (uglistSellShot_.IsAct())
        {
            //全選択
            if (uglistSellShot_.IsActRes(enUGLiAct_SubA))
            {
                //ugshot_.SetDraw(FALSE);
                //ugbba_.ugserifu_.SetDraw(TRUE);
                //RefreshPageBtn(TRUE, FALSE);
                RefreshSellBtn(FALSE);
            }
            else if (uglistSellShot_.IsActRes(enUGLiAct_SubB))//全キャンセル
            {
                //ugshot_.SetDraw(FALSE);
                //ugbba_.ugserifu_.SetDraw(TRUE);
                //RefreshPageBtn(TRUE, FALSE);
                RefreshSellBtn(FALSE);
            }
            else if (uglistSellShot_.IsActRes(enUGLiAct_Record))//レコード
            {
                RefreshSellBtn(FALSE);
            }

            //うるボタン
            ugbtn_ok_->SetDraw((uglistSellShot_.GetSelectNum() != 0));
        }
        else
        {
            //１つ以上選択されてるとき
            if (uglistSellShot_.GetSelectNum() > 0)
            {
                if (ugbtn_prev_->IsAct())//裏面
                {
                    ugshot_.SetShotDtSell(uglistSellShot_.GetIDShotLast());
                    ugshot_.SetLRPos(TRUE);
                    ugshot_.SetDraw(TRUE);
                    ugbba_.ugserifu_.SetDraw(FALSE);
                    ugshot_.SetUra(TRUE);
                }
                else if (ugbtn_next_->IsAct())//プレビュー
                {
                    stCBBA_.id_shot_ = uglistSellShot_.GetIDShotLast();//有効な中の番号
                    ChangePage(pg_ShPrev);
                }
            }
            else//一つも選択されてないとき
            {
                //前ページ
                if (ugbtn_prev_->IsAct())
                {
                    if (RefreshPageBtn(FALSE, -1))
                    {
                        uglistSellShot_.Refresh(stCBBA_.listpageNo_, &(uglistShFilter_.id_shotvec_));
                    }
                    ugshot_.SetDraw(FALSE);
                    ugbba_.ugserifu_.SetDraw(TRUE);
                }
                else if (ugbtn_next_->IsAct())//次ページ
                {
                    if (RefreshPageBtn(FALSE, +1))
                    {
                        uglistSellShot_.Refresh(stCBBA_.listpageNo_, &(uglistShFilter_.id_shotvec_));
                    }
                    ugshot_.SetDraw(FALSE);
                    ugbba_.ugserifu_.SetDraw(TRUE);
                }
            }
        }
    }

    //シュート購入メニュー価格帯
    void TLyHmCBBA::MvPage_BuySozaiMenu()
    {
        //もどる
        if (ugbtn_back_->IsAct())
        {
            ChangePage(pg_Menu);
        }
        else if (uglistBuySozaiMenu_.IsAct())//次のページへ
        {
            ugshot_.SetDraw(FALSE);
            stCBBA_.mid_bbabuy_ = uglistBuySozaiMenu_.GetCommonID();
            uglistBuySozaiType_.Refresh(stCBBA_.mid_bbabuy_);
            ChangePage(pg_BuySozaiType);
        }
    }

    //シュート購入
    void TLyHmCBBA::MvPage_BuySozaiType()
    {
        //もどる
        if (ugbtn_back_->IsAct())
        {
            ChangePage(pg_BuySozaiMenu);
        }
        else if (uglistBuySozaiType_.IsAct())//次のページへ
        {
            ugshot_.SetDraw(FALSE);
            stCBBA_.mid_spsyouhin_ = 0;
            stCBBA_.mid_sozai_ = 0;
            uglistBuySozai_.Refresh(uglistBuySozaiType_.GetType(), uglistBuySozaiType_.GetNextType(), stCBBA_.mid_bbabuy_);
            ChangePage(pg_BuySozai);
        }
    }
    void TLyHmCBBA::MvPage_BuySozai()
    {
        //もどる
        if (ugbtn_back_->IsAct())
        {
            if (stCBBA_.mid_spsyouhin_ == 0)
            {
                ChangePage(pg_BuySozaiType);
            }
            else
            {
                uglistBuySozai_.SetSelect(NGNUM);
                stCBBA_.mid_spsyouhin_ = 0;
                stCBBA_.mid_sozai_ = 0;
                ugbtn_ok_->SetDraw(FALSE);
                ugshot_.SetDraw(FALSE);
                ugbba_.ugserifu_.SetDraw(TRUE);
                ugbtn_prev_->SetDraw(FALSE);
                ugbtn_next_->SetDraw(FALSE);
            }
        }
        else if (ugbtn_ok_->IsAct())
        {
            std::string name = pmgEO_->mgDt_.dtShElm_.GetDtStr(stCBBA_.mid_sozai_ / SOZAIMID, szs_name);
            s32 pricetype = pmgEO_->mgCoM_.mdm_BBABuy_.GetPKDt(stCBBA_.mid_bbabuy_, mdm_bbabuy_priceType);
            s32 price = pmgEO_->mgCoM_.mdm_BBABuy_.GetPKDt(stCBBA_.mid_bbabuy_, mdm_bbabuy_price);
            //s32 price = pmgEO_->mgCoM_.mdm_Rarity_.GetLineDt((stCBBA_.mid_sozai_ % SOZAIMID), mdm_rarity_priceBuy);
            STRVECTOR strvec = { name + "を", "こうにゅうします" };
            if (pricetype == enPriceType_Star)
            {
                SetHudMsgStar(&strvec, price);
            }
            else
            {
                SetHudMsgTodd(&strvec, price, FALSE);
            }

        }
        else if (IsHudOK())//確認ＯＫ
        {
            //POST：素材購入
            Post_BuySozai();
        }
        else if (ugbtn_prev_->IsAct())//裏面
        {
            ugshot_.SetDraw(TRUE);
            ugbba_.ugserifu_.SetDraw(FALSE);
            //ugshot_.SetUra(!ugshot_.IsUra());
            ugshot_.SetUra(TRUE);

        }
        else if (ugbtn_next_->IsAct())//プレビュー
        {
            buyprevvec_.clear();
            TSozaiData sozai;
            sozai.SetMidSozai(stCBBA_.mid_sozai_);
            buyprevvec_.push_back(sozai);
            ChangePage(pg_BuyShPrev);
        }
        else if (uglistBuySozai_.IsAct())//次のページへ
        {
            stCBBA_.mid_spsyouhin_ = uglistBuySozai_.GetCommonID();
            stCBBA_.mid_sozai_ = pmgEO_->mgCoM_.mdm_SpSyouhin_.GetPKDt(stCBBA_.mid_spsyouhin_, mdm_spsyouhin_itemID);
            ugshot_.SetItemGet(stCBBA_.mid_sozai_, NGNUM, NGNUM);
            ugshot_.SetDraw(TRUE);
            ugshot_.SetLRPos(TRUE);
            ugbtn_ok_->SetDraw(TRUE);
            ugbtn_prev_->SetDraw(TRUE);
            ugbtn_next_->SetDraw(TRUE);
            ugbba_.ugserifu_.SetDraw(FALSE);
        }
    }
    void TLyHmCBBA::MvPage_BuyShPrev()
    {
        if (IsHudPrevBack())//おーけー
        {
            ChangePage(pg_BuySozai);
            ugshot_.SetDraw(TRUE);
            ugbtn_ok_->SetDraw(TRUE);
            ugbtn_prev_->SetDraw(TRUE);
            ugbtn_next_->SetDraw(TRUE);
        }
        else if (IsHudPrevMix())//ランダムボタン
        {
            SetHudPrevMixSozai(stCBBA_.mid_sozai_, TRUE);
        }
        else if (IsHudPrevMixClear())
        {
            SetHudPrevMixSozai(stCBBA_.mid_sozai_, FALSE);

        }
    }

    BOOL TLyHmCBBA::CheckPostLockShot(s32 nextpage)
    {
        BOOL res = FALSE;
        shsetvec_ = pmgEO_->mgCoU_.GetShLockVec();
        if (shsetvec_.size() > 0)
        {
            stCBBA_.nextPage_ = nextpage;
            Post_LockShot();
            res = TRUE;
        }
        return res;
    }

    //フィルター
    void TLyHmCBBA::MvPage_ShFilter()
    {
        if (ugbtn_back_->IsAct())//おーけー
        {
            ChangePage(stCBBA_.shfilterPage_);
            ugbba_.ugserifu_.SetDraw(TRUE);
        }
        else if (ugbtn_prev_->IsAct())//裏面
        {
            stCBBA_.id_shot_ = ugdrShot_.GetIDShot();//有効な中の番号
            ugdrShot_.ura_.SetShotUraDt(stCBBA_.id_shot_);
            ugdrShot_.RevUra();
        }
        else if (ugbtn_next_->IsAct())//プレビュー
        {
            stCBBA_.id_shot_ = ugdrShot_.GetIDShot();
            ChangePage(pg_ShPrev);
        }
        else if (uglistShFilter_.IsActRes2(enUGLiShFilter_Filter))
        {
            ugdrShot_.ReadyFilter();
            //POST：シュートロック
            CheckPostLockShot(NGNUM);
        }
        else if (uglistShFilter_.IsActRes2(enUGLiShFilter_Sort))
        {
            ugdrShot_.ReadySort();
        }
    }
    //プレビュー
    void TLyHmCBBA::MvPage_ShPrev()
    {
        if (IsHudPrevBack())//おーけー
        {
            ChangePage(stLy_.lastpageNo_);
        }
        else if (IsHudPrevMix())
        {
            SetHudPrevMixShot(stCBBA_.id_shot_, TRUE);
        }
        else if (IsHudPrevMixClear())
        {
            SetHudPrevMixShot(stCBBA_.id_shot_, FALSE);
        }
    }
    void TLyHmCBBA::MvPage_Out()
    {
        if (ugbba_.IsAct())
        {
            Down();
        }
    }

    //POST：シュートロック
    void TLyHmCBBA::Post_LockShot()
    {
        StartConnect(cn_LockShot);
        pmgEO_->mgPo_.poHmTeam_.Ready_shlock(&shsetvec_);
    }
    //POST：シュート名変更
    void TLyHmCBBA::Post_RenameShot()
    {
        StartConnect(cn_RenameShot);
        pmgEO_->mgPo_.poHmTeam_.Ready_renameshot(stCBBA_.id_shot_, inputname_);
    }

    //POST：シュートインベントリ拡張
    void TLyHmCBBA::Post_AddShotInv()
    {
        StartConnect(cn_AddShotInv);
        pmgEO_->mgPo_.poHmCenter_.Ready_addshotinv(stCBBA_.mid_spsyouhin_);
    }
    ////POST：シュートばらしひとつ
    //void TLyHmCBBA::Post_BaraLast()
    //{
    //  StartConnect(cn_BaraLast);
    //  pmgEO_->mgPo_.poHmCenter_.Ready_bara(stCBBA_.id_shot_, TRUE);
    //}
    //POST：シュートばらしすべて
    void TLyHmCBBA::Post_BaraAll()
    {
        StartConnect(cn_BaraAll);
        pmgEO_->mgPo_.poHmCenter_.Ready_bara(stCBBA_.id_shot_, !(stCBBA_.barastar_f_));
    }
    //POST：シュート売却
    void TLyHmCBBA::Post_SellShot()
    {
        StartConnect(cn_SellShot);
        pmgEO_->mgPo_.poHmCenter_.Ready_sellshot(&sellidvec_);
    }
    //POST：シュート購入
    void TLyHmCBBA::Post_BuySozai()
    {
        StartConnect(cn_BuySozai);
        pmgEO_->mgPo_.poHmCenter_.Ready_buysozai(stCBBA_.mid_spsyouhin_);
    }


    void TLyHmCBBA::Connect_LockShot()
    {
        if (stCBBA_.nextPage_ != NGNUM)
        {
            ChangePage(stCBBA_.nextPage_);
        }
        else
        {
            uglistShFilter_.Ready(enShotFilTypeS_All);
        }
    }
    void TLyHmCBBA::Connect_RenameShot()
    {
        ugdrShot_.RefreshSelected();
        ChangePage(pg_ShEdit);
    }

    //接続：シュートインベントリ拡張
    void TLyHmCBBA::Connect_AddShotInv()
    {
        if (pmgEO_->mgCoU_.GetShotNum_Inv() >= INVMAX)
        {
            ChangePage(pg_Menu);
        }
        else
        {
            ChangePage(pg_ShotInv);
            ugbba_.SetBBAMsg(enBBAMsg_Thanks);
        }
    }
    ////接続：シュートばらし
    //void TLyHmCBBA::Connect_BaraLast()
    //{
    //  uglistShFilter_.Ready(enShotFilTypeS_Mixed);
    //  if (uglistShFilter_.IsHaveMixed() && (pmgEO_->mgCoU_.GetShotNum_Emp() > 0))
    //  {
    //    ChangePage(pg_BaraMenu);
    //    ugbba_.SetBBAMsg(enBBAMsg_Thanks);
    //  }
    //  else
    //  {
    //    ChangePage(pg_Menu);
    //  }
    //}
    //接続：シュートばらし
    void TLyHmCBBA::Connect_BaraAll()
    {
        uglistShFilter_.Ready(enShotFilTypeS_Mixed);
        if (uglistShFilter_.IsHaveMixed() && (pmgEO_->mgCoU_.GetShotNum_Emp() > 0))
        {
            ChangePage(pg_BaraMenu);
            ugbba_.SetBBAMsg(enBBAMsg_Thanks);
        }
        else
        {
            ChangePage(pg_Menu);
        }
    }
    //接続：シュート売却
    void TLyHmCBBA::Connect_SellShot()
    {
        ChangePage(pg_SellMenu);
        ugbba_.SetBBAMsg(enBBAMsg_Thanks);
        ugbba_.ugserifu_.SetDraw(TRUE);
    }
    //接続：素材購入
    void TLyHmCBBA::Connect_BuySozai()
    {
        ChangePage(pg_BuySozai);
        ugbba_.SetBBAMsg(enBBAMsg_Thanks);
        ugbba_.ugserifu_.SetDraw(TRUE);
    }

    //接続完了
    void TLyHmCBBA::DoConnect(BOOL rqok_f)
    {
        if (rqok_f)//ＯＫのみ
        {
            //code0
            if (pmgEO_->mgPo_.IsCodeZero())
            {
                switch (stLy_.connect_)
                {
                case cn_LockShot: Connect_LockShot(); break;
                case cn_RenameShot: Connect_RenameShot(); break;
                case cn_AddShotInv: Connect_AddShotInv(); break;
                    //case cn_BaraLast: Connect_BaraLast(); break;
                case cn_BaraAll: Connect_BaraAll(); break;
                case cn_SellShot: Connect_SellShot(); break;
                case cn_BuySozai: Connect_BuySozai(); break;
                }
            }
            else
            {
                //エラーメッセージ
                EndConnectNG(pmgEO_->mgPo_.GetCode(), pmgEO_->mgPo_.GetMsg().c_str());
                //ChangePage(pg_Menu);

                /* 2015/06/17 Koike Modified */
                // エラーならタイトルへ戻る
                Down();
                pScene_->lyHmBar_->SetFilterAll(FALSE);
                pScene_->SetGotoTitle();
            }
        }
        else
        {
            //通信エラー
            //ChangePage(pg_Menu);

            /* 2015/06/17 Koike Modified */
            // エラーならタイトルへ戻る
            Down();
            pScene_->lyHmBar_->SetFilterAll(FALSE);
            pScene_->SetGotoTitle();
        }
    }

    //lp_Shopping
    void TLyHmCBBA::DoUpdate(BOOL draw_f)
    {
        switch (stLy_.pageNo_)
        {
        case db::pg_Out:
            ugbba_.Update(draw_f);
            break;
        default:
            ugbba_.Update(draw_f);
            ugdrShot_.Update(draw_f);
            uglistMenu_.Update(draw_f);
            uglistShotInv_.Update(draw_f);
            uglistBaraMenu_.Update(draw_f);
            uglistBuySozaiMenu_.Update(draw_f);
            uglistBuySozai_.Update(draw_f);
            uglistSellMenu_.Update(draw_f);
            uglistSellShot_.Update(draw_f);
            uglistBuySozaiType_.Update(draw_f);
            uglistShFilter_.Update(draw_f);
            ugshot_.Update(draw_f);
            break;
        }


        if (draw_f == FALSE)
        {
            switch (stLy_.pageNo_)
            {
            case db::pg_Visit:MvPage_Visit(); break;
            case db::pg_Menu:MvPage_Menu(); break;
            case db::pg_ShEdit:MvPage_ShEdit(); break;
            case db::pg_RenameShot:MvPage_RenameShot(); break;
            case db::pg_ShotInv:MvPage_ShotInv(); break;
            case db::pg_ShotInvEnd: MvPage_ShotInvEnd(); break;
            case db::pg_BaraMenu:MvPage_BaraMenu(); break;
            case db::pg_Bara:MvPage_Bara(); break;
            case db::pg_ShFilter:MvPage_ShFilter(); break;
            case db::pg_ShPrev:MvPage_ShPrev(); break;
            case db::pg_BaraEnd:MvPage_BaraEnd(); break;
            case db::pg_BuySozaiMenu:MvPage_BuySozaiMenu(); break;
            case db::pg_BuySozaiType:MvPage_BuySozaiType(); break;
            case db::pg_BuySozai:MvPage_BuySozai(); break;
            case db::pg_BuyShPrev:MvPage_BuyShPrev(); break;
            case db::pg_SellMenu:MvPage_SellMenu(); break;
            case db::pg_SellShot:MvPage_SellShot(); break;
            case db::pg_Out:MvPage_Out(); break;
            }
        }
    }

    void TLyHmCBBA::ActListMenu()
    {
        switch ((enBBAMenu)uglistMenu_.GetActIndex())
        {
        case bbm_ShEdit:
            ChangePage(pg_ShEdit);
            stCBBA_.shfilterPage_ = stLy_.lastpageNo_;
            break;
        case bbm_ShotInv:
            //インベントリ数
            if (pmgEO_->mgCoU_.GetShotNum_Inv() >= INVMAX)
            {
                ugbba_.SetBBAMsg(enBBAMsg_InvMax);
            }
            else
            {
                ChangePage(pg_ShotInv);
            }
            break;
        case bbm_Bara:

			if (pmgEO_->stSeq_.IsNoSv())
			{
				if (uglistShFilter_.IsHaveMixed())
				{
					ChangePage(pg_BaraMenu);
				}
				return;
			}

            if (pmgEO_->mgCoU_.GetShotNum_Emp() == 0)
            {
                ugbba_.SetBBAMsg(enBBAMsg_NoneEmp);
            }
            else if (uglistShFilter_.IsHaveMixed())
            {
                ChangePage(pg_BaraMenu);
            }
            else
            {
                ugbba_.SetBBAMsg(enBBAMsg_NoneBara);
            }
            break;
        case bbm_SellShot:
            ChangePage(pg_SellMenu);

            break;
        case bbm_BuySozai:
#ifdef __K_DEBUG_ROOT__
            ugbtn_ok_->SetFont("かう！", FC2_normal);
            ChangePage(pg_BuySozaiMenu);
#else
            if (pmgEO_->mgCoU_.GetShotNum_Emp() == 0)
            {
                ugbba_.SetBBAMsg(enBBAMsg_NoneEmp);
            }
            else
            {
                ugbtn_ok_->SetFont("かう！", FC2_normal);
                ChangePage(pg_BuySozaiMenu);
            }
#endif
            break;
        }
    }



    //ドラムがないときくわしくとプレビューけす
    void TLyHmCBBA::ResetPrevBtn()
    {
        ugbtn_next_->SetDraw(ugdrShot_.IsData());
        ugbtn_prev_->SetDraw(ugdrShot_.IsData());
        ugbtn_mid_->SetDraw(ugdrShot_.IsData());
        ugbtn_ok_->SetDraw(ugdrShot_.IsData());
    }

    void TLyHmCBBA::BuyPriceType()
    {
        s32 pricetype = pmgEO_->mgCoM_.mdm_BBABuy_.GetPKDt(stCBBA_.mid_bbabuy_, mdm_bbabuy_priceType);
        if (pricetype == enPriceType_Star)
        {
            ugr_star_->Refresh();
        }
        else
        {
            ugr_todd_->Refresh();
        }
    }

    void TLyHmCBBA::BaraPriceType()
    {
        if (stCBBA_.barastar_f_)
        {
            ugr_star_->Refresh();
        }
        else
        {
            ugr_todd_->Refresh();
        }
    }



}
