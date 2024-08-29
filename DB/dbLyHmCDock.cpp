#include "dbLyHmCDock.h"
#include "dbSnHome.h"
#include "dbUGPrev.h"
#include "dbUGMixPrev.h"

namespace db
{

  enum enPage
  {
    pg_Visit,
    pg_Base,
    pg_Add,
    pg_Add2,
    pg_OK,
    pg_Get,
    pg_Bara,
    pg_ShFilter,
    pg_ShPrev,
    pg_GetPrev,
    pg_Out,
		/* 2015/06/02 Koike Added */
		pg_MixPrev,
		pg_CustumPrevIn,
		pg_CustumPrev,
		pg_CustumPrevDrum,
		pg_CustumPrevOK,
  };

  enum enConnect
  {
    cn_gousei,
    cn_okgousei,
    cn_nggousei,
  };


  //コンストラクタ
	TLyHmCDock::TLyHmCDock(TSnHome* pScene)
		:TLyHmGen(pScene)
		, uggousei_(this)
		, ugdrShot_(this)
		, uglistShFilter_(this)
		, ugshot_(this)
		, ugdrMixPrevShot_(this)
		, uglistMixPrevShFilter_(this)
		, ugmixprevshot_(this)
  {
    stCDock_.Init();
  }

  TLyHmCDock::~TLyHmCDock()
  {
  }

  void TLyHmCDock::DoLoad()
  {
    //ショップフィールド全生成
    uggousei_.MakeGroup(field_, &pScene_->lyHmCenter_->ugkantoku_, &pScene_->lyHmCenter_->ugcap_);

    uglistShFilter_.MakeGroup(field_, FALSE);
    //ドラム
    ugdrShot_.MakeGroup(field_, TRUE, &uglistShFilter_);

    ugshot_.MakeGroup(field_);
    ugshot_.SetDraw(FALSE);
    ugshot_.SetDepth(UIDP_04HEADER);
    ugshot_.MakeUra();

		uglistMixPrevShFilter_.MakeGroup(field_, FALSE);
		//ドラム
		ugdrMixPrevShot_.MakeGroup(field_, TRUE, &uglistMixPrevShFilter_);
		ugdrMixPrevShot_.SetDepth(UIDP_03_6);

		ugmixprevshot_.MakeGroup(field_);
		ugmixprevshot_.SetDraw(FALSE);
		ugmixprevshot_.SetDepth(UIDP_03_6);
		ugmixprevshot_.MakeUra();
  }

  //入店時の処理
  void TLyHmCDock::ReadyDock(TUGDock* ugdock, s32 dockno)
  {
    ugdock_ = ugdock;
    stCDock_.Init();
    stCDock_.id_dock_ = pmgEO_->mgCoU_.mdDock_.GetLineDt(ugdock->GetDockNo(), mddock_pk);
    uggousei_.Ready(ugdock, dockno);
    //シュートドラムタイプ：ベース可能
    uglistShFilter_.Ready(enShotFilTypeS_Baseable);
    ugdrShot_.ReadyFilter();


    switch (dockno)
    {
    case 0: ugshot_.GetField()->z_SetPos(8, -68); break;
    case 1: ugshot_.GetField()->z_SetPos(8, -68); break;
    case 2: ugshot_.GetField()->z_SetPos(8, -28); break;
    case 3: ugshot_.GetField()->z_SetPos(8, -28); break;
    }

		uglistMixPrevShFilter_.Ready(enShotFilTypeS_Baseable);
		ugdrMixPrevShot_.GetField()->z_SetPos(20, -150);
		ugdrMixPrevShot_.SetModal(TRUE);
		ugdrMixPrevShot_.ReadyFilter();

    ResetObj();

    Ready();

    pmgEO_->stSeq_.PushNeedOmakase();//おまかせリフレッシュフラグ
  }
  void TLyHmCDock::DoReady()
  {
    //ReadyDockを用意しているので書かない
  }
  void TLyHmCDock::DoDown()
  {
    uggousei_.Down();
    pScene_->lyHmCenter_->Ready();//退出
    pScene_->lyHmCenter_->OutDock();
  }

  void TLyHmCDock::ResetObj()
  {
    SetHudEnd();
    ugdrShot_.SetDraw(FALSE);
    ugdrShot_.SelectUra(FALSE);//裏返ってるときは元に戻す
    ugdrShot_.SelectDrumEnd(FALSE);//合成結果カードの消去
    ugshot_.SetDraw(FALSE);
		ugdrMixPrevShot_.SetDraw(FALSE);
		ugdrMixPrevShot_.SelectUra(FALSE);//裏返ってるときは元に戻す
		ugdrMixPrevShot_.SelectDrumEnd(FALSE);//合成結果カードの消去
		ugmixprevshot_.SetDraw(FALSE);
    ugbtn_prev_->SetDraw(FALSE);
    ugbtn_next_->SetDraw(FALSE);
    ugbtn_mid_->SetDraw(FALSE);
    ugbtn_ok_->SetDraw(FALSE);
    ugbtn_back_->SetDraw(TRUE);

    uglistShFilter_.SetDraw(FALSE);
		uglistMixPrevShFilter_.SetDraw(FALSE);
  }
  void TLyHmCDock::DoChangePage(s32 pageno)
  {
    switch ((enPage)pageno)
    {
    case db::pg_Visit:
      break;
    case db::pg_Base:
      ResetObj();
      ugdrShot_.SetDraw(TRUE);
      stCDock_.id_shot_base_ = NGNUM;
      uggousei_.SetGouseiMsg(enGouseiMsg_Base);
      ugbtn_back_->SetFont("もどる");
      ugbtn_prev_->SetFont("ならびかえ");
      ugbtn_next_->SetFont("くわしく");
      ugbtn_mid_->SetFont("プレビュー");
      ugbtn_ok_->SetFont("ベースにする");
      ugbtn_prev_->SetDraw(TRUE);//くわしく
      ugbtn_next_->SetDraw(TRUE);//ならびかえ
      ugbtn_mid_->SetDraw(TRUE);//プレビュー
      ugbtn_ok_->SetDraw(TRUE);//OK
			stCDock_.have_random_ = FALSE;
			uglistShFilter_.Ready(enShotFilTypeS_Baseable);
			ugdrShot_.ReadyFilter();
      ResetPrevBtn();
      break;
    case db::pg_Add:
      ResetObj();
      ugdrShot_.SetDraw(TRUE);
      ugshot_.SetDraw(TRUE);
      //uggousei_.SetBaseID(stCDock_.id_shot_base_);
      uggousei_.SetGouseiMsg(enGouseiMsg_Sozai);
      ugbtn_prev_->SetFont("ならびかえ");
      ugbtn_next_->SetFont("くわしく");
      ugbtn_mid_->SetFont("プレビュー");
      ugbtn_prev_->SetDraw(TRUE);//くわしく
      ugbtn_next_->SetDraw(TRUE);//ならびかえ
      ugbtn_mid_->SetDraw(TRUE);//プレビュー
      ugbtn_ok_->SetDraw(TRUE);//OK
      ugbtn_ok_->SetFont("これをまぜる", FC2_normal);
			stCDock_.have_random_ = TRUE;
      ResetPrevBtn();
      break;
    case db::pg_Add2:
      ResetObj();
      ugdrShot_.SetDraw(TRUE);
      ugshot_.SetDraw(TRUE);
      uggousei_.SetGouseiMsg(enGouseiMsg_Sozai);
      ugbtn_prev_->SetFont("ならびかえ");
      ugbtn_next_->SetFont("くわしく");
      ugbtn_mid_->SetFont("プレビュー");
      ugbtn_prev_->SetDraw(TRUE);//くわしく
      ugbtn_next_->SetDraw(TRUE);//ならびかえ
      ugbtn_mid_->SetDraw(TRUE);//プレビュー
      ugbtn_ok_->SetDraw(TRUE);//OK
      ugbtn_ok_->SetFont("これもついか", FC2_normal);
      ugbtn_back_->SetDraw(TRUE);//OK
      ugbtn_back_->SetFont("かくてい", FC2_normal);
      ResetPrevBtn();
      break;
    case db::pg_OK:
      ResetObj();
      ugdrShot_.SetDraw(TRUE);
      //uggousei_.SetAddID(stCDock_.id_shot_add_);//素材を入れる
      uggousei_.SetGouseiMsg(enGouseiMsg_OK);
      ugdrShot_.drumend_.SetShotDtGousei(&id_shot_mixvec_);//最終カードの下地
      ugdrShot_.SelectDrumEnd(TRUE);//合成結果カードの表示
      ugbtn_prev_->SetFont("くわしく");
      ugbtn_prev_->SetDraw(TRUE);//くわしく
      ugbtn_ok_->SetFont("ミックス！", FC2_normal);
      ugbtn_ok_->SetDraw(TRUE);//OK
      ugbtn_back_->SetFont("もどる", FC2_normal);
      ugbtn_back_->SetDraw(TRUE);
			/* 2015/06/02 Koike Added */
			ugbtn_mid_->SetFont("プレビュー");
			ugbtn_mid_->SetDraw(TRUE);//プレビュー
      break;
    case db::pg_Get:
      ResetObj();
      //ugdrShot_.SelectDrumEnd(TRUE);//合成結果カードの表示
      //ugdrShot_.SetDraw(TRUE);
      ugshot_.SetDraw(TRUE);
      ugbtn_ok_->SetDraw(TRUE);
      ugbtn_prev_->SetFont("くわしく");
      ugbtn_prev_->SetDraw(TRUE);//くわしく
      ugbtn_next_->SetFont("プレビュー", FC2_normal);
      ugbtn_next_->SetDraw(TRUE);//ならびかえをプレビューに変える
      //ugbtn_mid_->SetDraw(TRUE);//プレビュー
      break;
    case db::pg_Bara:
    {
      ResetObj();
      
      STRVECTOR strvec = { "こんかいの＿ミックスを", "もとにもどしますか？" };
      SetHudMsg(TRUE, FALSE, &strvec);
      ugbtn_back_->SetDraw(FALSE);
      ugbtn_ok_->SetDraw(FALSE);
    }
    break;
    case db::pg_ShFilter:
      ResetObj();
      ugbtn_prev_->SetFont("くわしく");
      ugbtn_next_->SetFont("プレビュー");
      ugbtn_back_->SetFont("もどる");
      ugbtn_prev_->SetDraw(TRUE);//くわしく
      ugbtn_next_->SetDraw(TRUE);//プレビュー
      uglistShFilter_.SetDraw(TRUE);
      ugdrShot_.SetDraw(TRUE);
      uggousei_.SetSerifuDraw(FALSE);
      break;
    case db::pg_ShPrev:
    {
      TPrevParam param;
      param.id_shot = stCDock_.id_shot_prev_;
			param.nonehavemixbtn_f = stCDock_.have_random_;
      ResetObj();
      SetHudPrev(param);
      ugbtn_back_->SetDraw(FALSE);
      ugbtn_ok_->SetDraw(FALSE);
    }
      break;
    case db::pg_GetPrev:
      ResetObj();
      SetHudPrev_Mixed(&id_shot_mixvec_);
      ugbtn_back_->SetDraw(FALSE);
      ugbtn_ok_->SetDraw(FALSE);
      break;
    case db::pg_Out:
      UndrawBtn();
      //uggousei_.SetGouseiMsg(enGouseiMsg_Out);
      pScene_->lyHmCenter_->RefreshSoukoNum();
      break;
	  /* 2015/06/02 Koike Added */
		case db::pg_MixPrev:
		{
			ResetObj();
			SetHudPrev_Mixed(&id_shot_mixvec_);
			ugbtn_back_->SetDraw(FALSE);
			ugbtn_ok_->SetDraw(FALSE);
		}
		  break;
		case db::pg_CustumPrevIn:
		{
			TMixPrevParam param;
			param.id_shot = stCDock_.id_shot_prev_;
			param.nonemixbtn_f = FALSE;
			param.nonehavemixbtn_f = stCDock_.have_random_;
			ResetObj();
			SetHudMixPrev(param);
			ugdrMixPrevShot_.SetDraw(FALSE);
			ugbtn_back_->SetDraw(FALSE);
			ugbtn_ok_->SetDraw(FALSE);
		}
			break;
		case db::pg_CustumPrev:
		{
			// ここでは特に何もしない。
			ugdrMixPrevShot_.SetDraw(FALSE);
		}
		 break;
		case db::pg_CustumPrevDrum:
		{
			ugdrMixPrevShot_.SetDraw(TRUE);
			ugdrMixPrevShot_.SetLRPos(FALSE);

			//uglistMixPrevShFilter_.Ready(enShotFilTypeS_Baseable);
			//ugdrMixPrevShot_.ReadyFilter();
		}
		 break;
		case db::pg_CustumPrevOK:
		{
			ResetObj();
			ugdrShot_.SetDraw(TRUE);
			//uggousei_.SetAddID(stCDock_.id_shot_add_);//素材を入れる
			uggousei_.SetGouseiMsg(enGouseiMsg_OK);
			ugdrShot_.drumend_.SetShotDtGousei(&id_shot_mixvec_);//最終カードの下地
			ugdrShot_.SelectDrumEnd(TRUE);//合成結果カードの表示
			ugbtn_prev_->SetFont("くわしく");
			ugbtn_prev_->SetDraw(TRUE);//くわしく
			ugbtn_ok_->SetFont("ミックス！", FC2_normal);
			ugbtn_ok_->SetDraw(TRUE);//OK
			ugbtn_back_->SetFont("もどる", FC2_normal);
			ugbtn_back_->SetDraw(TRUE);
			/* 2015/06/02 Koike Added */
			ugbtn_mid_->SetFont("プレビュー");
			ugbtn_mid_->SetDraw(TRUE);//プレビュー
		}
		 break;
    default:
      break;
    }
  }

  //入店
  void TLyHmCDock::MvPage_Visit()
  {
    if (ugbtn_back_->IsAct())
    {
      uggousei_.SetGouseiMsg(enGouseiMsg_Out);
      ChangePage(pg_Out);
    }
    else if (uggousei_.ugserifu_.IsActRes(enUGSerifuAct_WaitEnd))
    {
      ugbtn_back_->SetDraw(TRUE);

      //空き
      if (pmgEO_->mgCoU_.mdDock_.GetPKDt(stCDock_.id_dock_, mddock_state) == 0)
      {
        ChangePage(pg_Base);

        stCDock_.id_shot_base_ = NGNUM;
        uggousei_.SetGouseiMsg(enGouseiMsg_Base);
        ugdrShot_.StopSwipe(FALSE);//スワイプきくように
        ugdrShot_.SetDraw(TRUE);
        ugbtn_back_->SetFont("もどる", FC2_normal);
        ugbtn_ok_->SetFont("ベースにする", FC2_normal);
      }
      else//合成中
      {
        ChangePage(pg_Get);

        stCDock_.id_shot_get_ = pmgEO_->mgCoU_.mdDock_.GetPKDt(stCDock_.id_dock_, mddock_id_shot0);
        uggousei_.SetGouseiMsg(enGouseiMsg_Get);
        id_shot_mixvec_.clear();
        for (s32 i = 0; i < HSELEMMAX; ++i)
        {
          enmdDock shotno = (enmdDock)(mddock_id_shot0 + i);
          s64 id_shot = pmgEO_->mgCoU_.mdDock_.GetPKDt(stCDock_.id_dock_, shotno);
          if (id_shot == 0) break;
          id_shot_mixvec_.push_back(id_shot);
        }

        ugshot_.SetLRPos(TRUE);
        ugshot_.SetShotDtGousei(&id_shot_mixvec_);
        ugshot_.SetDraw(TRUE);
        //ugdrShot_.drumend_.SetShotDtGousei(&id_shot_mixvec_);//最終カードの下地
        //ugdrShot_.drumend_.SetShotDt(stCDock_.id_shot_get_, 0, TRUE, pmgEO_->mgCoU_.GetSortShot(stCDock_.id_shot_get_));//最終カードの下地
        ugbtn_back_->SetFont("おーけー！", FC2_normal);
        ugbtn_ok_->SetFont("ダメダメ！", FC2_normal);
      }
    }
  }

  //ベース選択
  void TLyHmCDock::MvPage_Base()
  {
    //ドラムからの時ならびかえ消す
    ResetPrevBtn();

    if (ugbtn_back_->IsAct())
    {
      uggousei_.SetGouseiMsg(enGouseiMsg_Out);
      ChangePage(pg_Out);
    }
    else if (ugbtn_prev_->IsAct())
    {
      ChangePage(pg_ShFilter);
      stCDock_.backpage_filter_ = stLy_.lastpageNo_;
    }
    else if (ugbtn_next_->IsAct())//裏面
    {
      ugdrShot_.ura_.SetShotUraDt(ugdrShot_.GetIDShot());//てきとう
      ugdrShot_.RevUra();
    }
    else if (ugbtn_mid_->IsAct())//裏面
    {
      stCDock_.id_shot_prev_ = ugdrShot_.GetIDShot();// すべての中から
      ChangePage(pg_ShPrev);
      stCDock_.backpage_prev_ = stLy_.lastpageNo_;
    }
    else if (ugbtn_ok_->IsAct())
    {
      /*id_shot_mixvec_.clear();
      id_shot_mixvec_.push_back(ugdrShot_.GetIDShot());// すべての中から
			stCDock_.id_shot_prev_ = ugdrShot_.GetIDShot();// すべての中から

      s32 empslot = (ugshot_.SetShotDtGousei(&id_shot_mixvec_));
      if (empslot == 0)
      {
        ChangePage(pg_OK);
      }
      else
      {
        enShotFilTypeS filtype = (enShotFilTypeS)(enShotFilTypeS_Under1 + (empslot - 1));
        if (uglistShFilter_.Ready(filtype, &id_shot_mixvec_))
        {
          STRVECTOR strvec = { "このベースに＿ミックスできる", "そざいが＿ありません" };
          SetHudMsg(FALSE, FALSE, &strvec);
        }
        else
        {
          ugdrShot_.ReadyFilter();
          ChangePage(pg_Add);
					stCDock_.backpage_prev_ = stLy_.lastpageNo_;
        }
      }*/
			id_shot_mixvec_.clear();
			id_shot_mixvec_.push_back(ugdrShot_.GetIDShot());// すべての中から
			stCDock_.id_shot_prev_ = ugdrShot_.GetIDShot();// すべての中から

			s32 empslot = (ugshot_.SetShotDtGousei(&id_shot_mixvec_));
			if (empslot == 0)
			{
				ChangePage(pg_OK);
			}
			else
			{
				enShotFilTypeS filtype = (enShotFilTypeS)(enShotFilTypeS_Under1 + (empslot - 1));
				if (uglistMixPrevShFilter_.Ready(filtype, &id_shot_mixvec_))
				{
					STRVECTOR strvec = { "このベースに＿ミックスできる", "そざいが＿ありません" };
					SetHudMsg(FALSE, FALSE, &strvec);
				}
				else
				{
					ugdrMixPrevShot_.ReadyFilter();
					ChangePage(pg_CustumPrevIn);
					stCDock_.backpage_prev_ = stLy_.lastpageNo_;
				}
			}
    }
  }

	//追加素材選択
  void TLyHmCDock::MvPage_Add()
  {
    //ドラムからの時ならびかえ消す
    ResetPrevBtn();

    //もどる
    if (ugbtn_back_->IsAct())
    {
      uglistShFilter_.Ready(enShotFilTypeS_Baseable);
      ugdrShot_.ReadyFilter();
      ChangePage(pg_Base);
    }
    else  if (ugbtn_prev_->IsAct())//裏面
    {
      ChangePage(pg_ShFilter);
      stCDock_.backpage_filter_ = stLy_.lastpageNo_;
    }
    else if (ugbtn_next_->IsAct())
    {
      ugdrShot_.ura_.SetShotUraDt(ugdrShot_.GetIDShot());//てきとう
      ugdrShot_.RevUra();
    }
    else  if (ugbtn_mid_->IsAct())
    {
      stCDock_.id_shot_prev_ = ugdrShot_.GetIDShot(); //素材のみの中から
      ChangePage(pg_ShPrev);
      stCDock_.backpage_prev_ = stLy_.lastpageNo_;
    }
    else if (ugbtn_ok_->IsAct())
    {
      s32 addid = ugdrShot_.GetIDShot();// すべての中から
      id_shot_mixvec_.push_back(addid);

      s32 empslot = (ugshot_.SetShotDtGousei(&id_shot_mixvec_));
      if (empslot == 0)
      {
        ChangePage(pg_OK);
      }
      else
      {
        enShotFilTypeS filtype = (enShotFilTypeS)(enShotFilTypeS_Under1 + (empslot - 1));
        if (uglistShFilter_.Ready(filtype, &id_shot_mixvec_))
        {
          //もう素材がない
          ChangePage(pg_OK);
          //id_shot_mixvec_.pop_back();
          //STRVECTOR strvec = { "このベースに＿ミックスできる", "そざいが＿ありません" };
          //SetHudMsg(FALSE, FALSE, &strvec);
        }
        else
        {
          ugdrShot_.ReadyFilter();
          ChangePage(pg_Add2);
        }
      }
    }
  }

  //追加素材選択
  void TLyHmCDock::MvPage_Add2()
  {
    //決定
    if (ugbtn_back_->IsAct())
    {
      ChangePage(pg_OK);
    }
    else
    {
      MvPage_Add();
    }
  }

  //オーケー
  void TLyHmCDock::MvPage_OK()
  {
    //もどる
    if (ugbtn_back_->IsAct())
    {
      uglistShFilter_.Ready(enShotFilTypeS_Baseable);
      ugdrShot_.ReadyFilter();
      ChangePage(pg_Base);
    }
    else if (ugbtn_prev_->IsAct())//裏面
    {
      //合成結果の情報（まだ実際に持ってはいない）
      ugdrShot_.ura_.SetShotUraDt(&id_shot_mixvec_);
      ugdrShot_.RevUra();
    }
    else if (ugbtn_ok_->IsAct())
    {
      //POST：合成
      Post_Gousei();
    }
		/* 2015/06/02 Koike Added プレビューを追加 */
		else if (ugbtn_mid_->IsAct())
		{
			//合成結果のプレビュー（まだ実際に持ってはいない）
			SetHudPrev_Mixed(&id_shot_mixvec_);
			ChangePage(pg_MixPrev);
			stCDock_.backpage_prev_ = stLy_.lastpageNo_;
		}
  }
	//オーケー（改修版用）
	void TLyHmCDock::MyPage_CustomPrevOK()
	{
		//もどる
		if (ugbtn_back_->IsAct())
		{
			uglistShFilter_.Ready(enShotFilTypeS_Baseable);
			ugdrShot_.ReadyFilter();
			ChangePage(pg_Base);
		}
		else if (ugbtn_prev_->IsAct())//裏面
		{
			//合成結果の情報（まだ実際に持ってはいない）
			ugdrShot_.ura_.SetShotUraDt(&id_shot_mixvec_);
			ugdrShot_.RevUra();
		}
		else if (ugbtn_ok_->IsAct())
		{
			//POST：合成
			Post_Gousei();
		}
		/* 2015/06/02 Koike Added プレビューを追加 */
		else if (ugbtn_mid_->IsAct())
		{
			//合成結果のプレビュー（まだ実際に持ってはいない）
			//SetHudPrev_Mixed(&id_shot_mixvec_);
			SetHudPrev_Mixed(&id_shot_mixvec_);
			ChangePage(pg_MixPrev);
			stCDock_.backpage_prev_ = stLy_.lastpageNo_;
		}
	}
  //プレビュー
  void TLyHmCDock::MvPage_Prev()
  {
    if (IsHudPrevBack())
    {
      ChangePage(stCDock_.backpage_prev_);
    }
		else if (IsHudPrevMix())
		{
			SetHudPrevMixShot(stCDock_.id_shot_prev_, TRUE);
		}
		else if (IsHudPrevMixClear())
		{
			SetHudPrevMixShot(stCDock_.id_shot_prev_, FALSE);
		}
		/* 2015/06/19 Koike Added */
		else if (IsHudPrevHaveMix())
		{
			MixMyShot(); //手持ちのシュートをランダムで追加する
			SetHudPrev_Have_Mixed(&id_shot_mixvec_);
		}
  }
  //プレビュー
  void TLyHmCDock::MvPage_GetPrev()
  {
    if (IsHudPrevBack())
    {
      ChangePage(pg_Get);
    }
  }

  /* 2015/06/02 Koike Added */
  void TLyHmCDock::MvPage_MixPrev()
  {
	  if (IsHudPrevBack())
	  {
			uglistShFilter_.Ready(enShotFilTypeS_Baseable);
			ugdrShot_.ReadyFilter();
		  ChangePage(stCDock_.backpage_prev_);
	  }
  }

	void TLyHmCDock::MyPage_CustumPrevIn()
	{
		// この中では特に何もしない。
		ChangePage(pg_CustumPrev);
	}

	void TLyHmCDock::MyPage_CustumPrev()
	{
		if (IsHudPrevBack())
		{
			ChangePage(stCDock_.backpage_prev_);
		}
		else if (IsHudPrevHaveMix())
		{
			MixMyShot_CustumPrev(); //手持ちのシュートをランダムで追加する
			SetHudMixPrev_Have_Mixed(&id_shot_mixvec_);
		}
		else if (IsHudMixPrevSelectShot())
		{
			id_shot_mixvec_ = GetHudMixPrevChangeVec();
			ChangePage(pg_CustumPrevOK);
		}
		else if (IsHudMixPrevUndo())
		{
			if (IsUndoShot()) SetHudMixPrev_Have_Mixed(&id_shot_mixvec_);
			else
			{
				// ベースに戻る
				uglistShFilter_.Ready(enShotFilTypeS_Baseable);
				ugdrShot_.ReadyFilter();
				ChangePage(pg_Base);
			}
		}
		else if (IsHudMixPrevBall_0())
		{
			SetActionBallIcon(0);
		}
		else if (IsHudMixPrevBall_1())
		{
			SetActionBallIcon(1);
		}
		else if (IsHudMixPrevBall_2())
		{
			SetActionBallIcon(2);
		}
		else if (IsHudMixPrevBall_3())
		{
			SetActionBallIcon(3);
		}
		else if (IsHudMixPrevBall_4())
		{
			SetActionBallIcon(4);
		}
		else if (IsHudMixPrevBall_5())
		{
			SetActionBallIcon(5);
		}
		else if (IsHudMixPrevBall_6())
		{
			SetActionBallIcon(6);
		}
		else if (IsHudMixPrevBall_7())
		{
			SetActionBallIcon(7);
		}
		else if (IsHudMixPrevSelectBall())
		{
			ChangePage(pg_CustumPrevDrum);
		}
	}

	void TLyHmCDock::MyPage_CustumPrevDrum()
	{
		if (IsHudMixPrevCancel())
		{
			ChangePage(pg_CustumPrev);
		}
		if (IsHudMixPrevSelectShot())
		{
			s32 addid = ugdrMixPrevShot_.GetIDShot();// すべての中から
			id_shot_mixvec_.push_back(addid);
			SetHudMixPrevSelectShot(addid);

			s32 empslot = (ugmixprevshot_.SetShotDtGousei(&id_shot_mixvec_));
			if (empslot == 0)
			{
				ChangePage(pg_CustumPrev);
			}
			else
			{
				enShotFilTypeS filtype = (enShotFilTypeS)(enShotFilTypeS_Under1 + (empslot - 1));
				if (uglistMixPrevShFilter_.Ready(filtype, &id_shot_mixvec_))
				{
					//もう素材がない
					ChangePage(pg_CustumPrev);
					//id_shot_mixvec_.pop_back();
					//STRVECTOR strvec = { "このベースに＿ミックスできる", "そざいが＿ありません" };
					//SetHudMsg(FALSE, FALSE, &strvec);
				}
				else
				{
					ugdrMixPrevShot_.ReadyFilter();
					ChangePage(pg_CustumPrev);
				}
			}
		}
	}

  void TLyHmCDock::MvPage_ShFilter()
  {

    if (ugbtn_back_->IsAct())//おーけー
    {
      ChangePage(stCDock_.backpage_filter_);
      uggousei_.SetSerifuDraw(TRUE);
    }
    else if (ugbtn_prev_->IsAct())//裏面
    {
      stCDock_.id_shot_prev_ = ugdrShot_.GetIDShot();//有効な中の番号
      ugdrShot_.ura_.SetShotUraDt(stCDock_.id_shot_prev_);
      ugdrShot_.RevUra();
    }
    else if (ugbtn_next_->IsAct())//プレビュー
    {
      stCDock_.id_shot_prev_ = ugdrShot_.GetIDShot();
      ChangePage(pg_ShPrev);
      stCDock_.backpage_prev_ = pg_ShFilter;
    }
    else if (uglistShFilter_.IsActRes2(enUGLiShFilter_Filter))
    {
      ugdrShot_.ReadyFilter();
    }
    else if (uglistShFilter_.IsActRes2(enUGLiShFilter_Sort))
    {
      ugdrShot_.ReadySort();
    }
  }

  void TLyHmCDock::MvPage_Get()
  {
    //合成ＯＫ
    if (ugbtn_back_->IsAct())
    {
      //POST：合成ＯＫ
      Post_OKGousei();
    }
    else if (ugbtn_ok_->IsAct())
    {
      ChangePage(pg_Bara);
    }
    else  if (ugbtn_prev_->IsAct())//裏面
    {
      ugshot_.SetUra(TRUE);
      //ugshot_.ura_.SetShotUraDt(&id_shot_mixvec_);
      //ugshot_.RevUra();
    }
    else if (ugbtn_next_->IsAct())//ここはプレビューを一つ左によせる
    {
      //SetHudPrev_Mixed(&id_shot_mixvec_);
      ChangePage(pg_GetPrev);
    }

  }
  void TLyHmCDock::MvPage_Bara()
  {
    if (IsHudOK())//おーけー
    {
      //POST：合成ＮＧ
      Post_NGGousei();
    }
    else if (IsHudCancel())//きゃんせる
    {
      ChangePage(pg_Get);
    }
  }

  //退出
  void TLyHmCDock::MvPage_Out()
  {
    if (uggousei_.IsAct()) Down();
  }

  //POST：合成
  void TLyHmCDock::Post_Gousei()
  {
    StartConnect(cn_gousei);
    pmgEO_->mgPo_.poHmCenter_.Ready_gousei(stCDock_.id_dock_, &id_shot_mixvec_);
  }
  //POST：合成ＯＫ
  void TLyHmCDock::Post_OKGousei()
  {
    StartConnect(cn_okgousei);
    pmgEO_->mgPo_.poHmCenter_.Ready_okgousei(stCDock_.id_dock_);
  }
  //POST：合成ＮＧ
  void TLyHmCDock::Post_NGGousei()
  {
    StartConnect(cn_nggousei);
    pmgEO_->mgPo_.poHmCenter_.Ready_nggousei(stCDock_.id_dock_);
  }
  //接続：合成
  void TLyHmCDock::Connect_Gousei()
  {
    //合成開始
    uggousei_.SetGouseiMsg(enGouseiMsg_Out_Mix);
    ChangePage(pg_Out);
  }
  //接続：合成
  void TLyHmCDock::Connect_OKGousei()
  {
    //合成ＯＫ
    uggousei_.SetGouseiMsg(enGouseiMsg_Out_OK);
    ChangePage(pg_Out);
  }
  //接続：合成
  void TLyHmCDock::Connect_NGGousei()
  {
    //合成ＮＧ
    uggousei_.SetGouseiMsg(enGouseiMsg_Out_NG);
    ChangePage(pg_Out);
  }

  //接続完了
  void TLyHmCDock::DoConnect(BOOL rqok_f)
  {
    if (rqok_f)//ＯＫのみ
    {
      //code0
      if (pmgEO_->mgPo_.IsCodeZero())
      {
        switch (stLy_.connect_)
        {
        case cn_gousei: Connect_Gousei(); break;
        case cn_okgousei: Connect_OKGousei(); break;
        case cn_nggousei: Connect_NGGousei(); break;
        }
      }
      else
      {
        //エラーメッセージ
        EndConnectNG(pmgEO_->mgPo_.GetCode(), pmgEO_->mgPo_.GetMsg().c_str());
        //uggousei_.SetGouseiMsg(enGouseiMsg_Out);
        //ChangePage(pg_Out);

        //通信エラー
        switch (stLy_.connect_)
        {
        case cn_gousei: break;
        case cn_okgousei: break;
        case cn_nggousei:  break;
        default:
          uggousei_.SetGouseiMsg(enGouseiMsg_Out);
          ChangePage(pg_Out);
          break;
        }
      }
    }
    else
    {

      //エラーメッセージ
      EndConnectNG(pmgEO_->mgPo_.GetCode(), pmgEO_->mgPo_.GetMsg().c_str());
      //通信エラー
      switch (stLy_.connect_)
      {
      case cn_gousei: break;
      case cn_okgousei: break;
      case cn_nggousei:  break;
      default:
        uggousei_.SetGouseiMsg(enGouseiMsg_Out);
        ChangePage(pg_Out);
        break;
      }
    }
  }

  void TLyHmCDock::DoUpdate(BOOL draw_f)
  {
    switch (stLy_.pageNo_)
    {
    case pg_Out:
      uggousei_.Update(draw_f);
      break;
    default:
      uggousei_.Update(draw_f);
      ugdrShot_.Update(draw_f);
      uglistShFilter_.Update(draw_f);
      ugshot_.Update(draw_f);
			ugdrMixPrevShot_.Update(draw_f);
			uglistMixPrevShFilter_.Update(draw_f);
			ugmixprevshot_.Update(draw_f);
      break;
    }

    if (draw_f == FALSE)
    {
      switch (stLy_.pageNo_)
      {
			case pg_Visit: MvPage_Visit(); break;
			case pg_Base: MvPage_Base(); break;
			case pg_Add: MvPage_Add(); break;
			case pg_Add2: MvPage_Add2(); break;
			case pg_OK: MvPage_OK(); break;
			case pg_Get: MvPage_Get(); break;
			case pg_Bara: MvPage_Bara(); break;
			case pg_ShFilter: MvPage_ShFilter(); break;
			case pg_ShPrev: MvPage_Prev(); break;
			case pg_GetPrev: MvPage_GetPrev(); break;
			case pg_Out: MvPage_Out(); break;
			/* 2015/06/02 Koike Added */
			case pg_MixPrev: MvPage_MixPrev(); break;
			case pg_CustumPrevIn: MyPage_CustumPrevIn(); break;
			case pg_CustumPrev: MyPage_CustumPrev(); break;
			case pg_CustumPrevDrum: MyPage_CustumPrevDrum(); break;
			case pg_CustumPrevOK: MyPage_CustomPrevOK(); break;
      }
    }
  }
  //ドラムがないときくわしくとプレビューけす
  void TLyHmCDock::ResetPrevBtn()
  {
    ugbtn_next_->SetDraw(ugdrShot_.IsData());
    ugbtn_prev_->SetDraw(ugdrShot_.IsData());
    ugbtn_mid_->SetDraw(ugdrShot_.IsData());
  }

	/* 2015/06/18 Koike Added */
	// 手持ちのシュートをランダムで合成する処理
	void TLyHmCDock::MixMyShot()
	{
		// まずは初期化
		id_shot_mixvec_.clear();
		s32 addid = stCDock_.id_shot_prev_;// 現在選んでいるID
		id_shot_mixvec_.push_back(addid); // これがまずベースになる

		// 空きスロットを確認
		s32 empslot = (ugshot_.SetShotDtGousei(&id_shot_mixvec_));
		//s32 restshot = ugdrShot_.id_shotvec_.size();
		// ここから先は素材をくっつける
		// ここから先は素材をくっつける
		while (0 < empslot)
		{
			enShotFilTypeS filtype = (enShotFilTypeS)(enShotFilTypeS_Under1 + (empslot - 1));
			if (uglistShFilter_.Ready(filtype, &id_shot_mixvec_))
			{
				ugdrShot_.ReadyFilter();
				return;
			}
			else
			{
				// まだ行ける場合はそのまま素材を足す
				ugdrShot_.ReadyFilter();
				addid = mid::midGetRandMn(ugdrShot_.id_shotvec_.size());
				id_shot_mixvec_.push_back(ugdrShot_.id_shotvec_[addid]); // 素材を足す
				empslot = (ugshot_.SetShotDtGousei(&id_shot_mixvec_)); // 空き状態とセットを確認
			}
		}
		/*if (empslot == 0 || restshot <= 1)
		{
			return; // 空きスロットと他に素材が無い場合はそのまま抜ける
		}
		else
		{
			// ここから先は素材をくっつける
			while (empslot != 0)
			{
				enShotFilTypeS filtype = (enShotFilTypeS)(enShotFilTypeS_Under1 + (empslot - 1));
				uglistShFilter_.HaveFilter(enShotFilType_All, filtype);
				// まだ行ける場合はそのまま素材を足す
				ugdrShot_.ReadyFilter();
				addid = mid::midGetRandMn(ugdrShot_.id_shotvec_.size());
				id_shot_mixvec_.push_back(ugdrShot_.id_shotvec_[addid]); // 素材を足す
				empslot = (ugshot_.SetShotDtGousei(&id_shot_mixvec_)); // 空き状態とセットを確認
			}
		}*/
	}
	void TLyHmCDock::MixMyShot_CustumPrev()
	{
		// まずは初期化
		id_shot_mixvec_.clear();
		s32 addid = stCDock_.id_shot_prev_;// 現在選んでいるID
		id_shot_mixvec_.push_back(addid); // これがまずベースになる

		// 空きスロットを確認
		s32 empslot = (ugshot_.SetShotDtGousei(&id_shot_mixvec_));
		// ここから先は素材をくっつける
		while (0 < empslot)
		{
			enShotFilTypeS filtype = (enShotFilTypeS)(enShotFilTypeS_Under1 + (empslot - 1));
			//if(uglistMixPrevShFilter_.HaveFilter(enShotFilType_All, filtype))
			if (uglistMixPrevShFilter_.Ready(filtype, &id_shot_mixvec_))
			{
				ugdrMixPrevShot_.ReadyFilter();
				return;
			}
			// まだ行ける場合はそのまま素材を足す
			else
			{
				ugdrMixPrevShot_.ReadyFilter();
				addid = mid::midGetRandMn(ugdrMixPrevShot_.id_shotvec_.size());
				id_shot_mixvec_.push_back(ugdrMixPrevShot_.id_shotvec_[addid]); // 素材を足す
				empslot = (ugshot_.SetShotDtGousei(&id_shot_mixvec_)); // 空き状態とセットを確認
			}
		}
		/*if (empslot == 0 || restshot <= 1)
		{
			return; // 空きスロットと他に素材が無い場合はそのまま抜ける
		}
		else
		{
			// ここから先は素材をくっつける
			while (empslot != 0 || restshot <= 1)
			{
				enShotFilTypeS filtype = (enShotFilTypeS)(enShotFilTypeS_Under1 + (empslot - 1));
				//if(uglistMixPrevShFilter_.HaveFilter(enShotFilType_All, filtype))
				if (uglistMixPrevShFilter_.Ready(filtype, &id_shot_mixvec_))
				{
					return;
				}
				// まだ行ける場合はそのまま素材を足す
				{
					ugdrMixPrevShot_.ReadyFilter();
					addid = mid::midGetRandMn(ugdrMixPrevShot_.id_shotvec_.size());
					id_shot_mixvec_.push_back(ugdrMixPrevShot_.id_shotvec_[addid]); // 素材を足す
					empslot = (ugshot_.SetShotDtGousei(&id_shot_mixvec_)); // 空き状態とセットを確認
				}
			}*/
	}

	// シュートの素材の最後を外す処理
	BOOL TLyHmCDock::IsUndoShot()
	{
		if (id_shot_mixvec_.size() > 1)
		{
			id_shot_mixvec_.pop_back();
			// 消した場合は問題ないことを確認
			s32 empslot = (ugshot_.SetShotDtGousei(&id_shot_mixvec_));
			enShotFilTypeS filtype = (enShotFilTypeS)(enShotFilTypeS_Under1 + (empslot - 1));
			if (!uglistMixPrevShFilter_.Ready(filtype, &id_shot_mixvec_))
			{
				ugdrMixPrevShot_.ReadyFilter();
				return TRUE;
			}
		}
		return FALSE;
	}

	// 手持ち選択のシュートをセット
	void TLyHmCDock::SelectMyShot()
	{
		s32 addid = ugdrMixPrevShot_.GetIDShot();// すべての中から
		//GetHudMixPrevChangeVec().push_back(addid);
		id_shot_mixvec_.push_back(addid);

		s32 empslot = (ugmixprevshot_.SetShotDtGousei(&id_shot_mixvec_));
		if (empslot == 0)
		{
			ChangePage(pg_CustumPrev);
		}
		else
		{
			enShotFilTypeS filtype = (enShotFilTypeS)(enShotFilTypeS_Under1 + (empslot - 1));
			if (uglistMixPrevShFilter_.Ready(filtype, &id_shot_mixvec_))
			{
				//もう素材がない
				ChangePage(pg_OK);
			}
			else
			{
				ugdrMixPrevShot_.ReadyFilter();
			}
		}
	}

	void TLyHmCDock::SetActionBallIcon(s32 iconno)
	{
		s32 empslot = (ugshot_.SetShotDtGousei(&id_shot_mixvec_));
		enShotFilTypeS filtype = (enShotFilTypeS)(enShotFilTypeS_Under1 + (empslot - 1));
		if (uglistMixPrevShFilter_.Ready(filtype, &id_shot_mixvec_))
		{
			if (frame_->SetHudMixPrevTouchMode(&id_shot_mixvec_, iconno) == enUGMixPrevSelectMode_select)
			{
				SetHudResetField(TRUE);
			}
		}
		else
		{
			if (frame_->SetHudMixPrevTouchMode(&id_shot_mixvec_, iconno) == enUGMixPrevSelectMode_select)
			{
				ChangePage(pg_CustumPrevDrum);
			}
		}
	}
}