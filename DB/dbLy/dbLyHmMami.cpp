#include "dbLyHmMami.h"
#include "dbSnHome.h"
#include "dbUGPrev.h"

namespace db
{
#ifdef __K_DEBUG_ROOT__
	static s32 debugLoginNum = 1;
#endif
  enum enPage
  {
    pg_Visit,
    pg_Menu,

    pg_Logbo,
    pg_LogboSp,
    pg_LogboSpYokoku,
    pg_LogboResult,

		pg_LogboShot,
		pg_LogboSpShot,
		pg_LogboSpShotYokoku,
		pg_LogboShotResult,

    pg_GiftBox,
    pg_GiftBoxResult,
    pg_UpGrade,
    pg_Out,
  };

  enum enRoomList
  {
    ml_GiftBox,
    //    ml_ShotBox,
    //    ml_Dock,
    //    ml_Gacha,
    //ml_UpGrade,
    mlEND,
  };

  enum enConnect
  {
    cn_Logbo,
    cn_CheckGiftBox,
    cn_OpenGiftBox,
  };

  /* 2015/06/16 Koike Added ログボでBランクシュートだった場合の表記 */
  const char* LOGBO_SPSHOT_MSG = "ほんじつのベースはＢランク";

  //コンストラクタ
  TLyHmMami::TLyHmMami(TSnHome* pScene)
    :TLyHmGen(pScene)
    , ugsp_mami_(this)
    , uglistMenu_(this)
    , uglistGiftBox_(this)
	/* 2015/06/16 Koike Added Bランクシュート用のタイトルを追加 */
	, ugtitle_spshot_(this)
  {
    stMami_.Init();
  }

  TLyHmMami::~TLyHmMami()
  {
  }

  void TLyHmMami::DoLoad()
  {
    //ショップフィールド全生成
    ugsp_mami_.MakeGroup(&pScene_->lyHmHome_->ugmami_);
    //ここの設定はuglistにうつす
    uglistMenu_.MakeGroup(field_, TRUE);
    uglistGiftBox_.MakeGroup(field_, TRUE);
    listvec_.clear();
    listvec_.push_back(&uglistMenu_);
    listvec_.push_back(&uglistGiftBox_);

    //メニューレコード
    for (s32 i = 0; i < mlEND; ++i)
    {
      TUGRcNormal* newrecord = (TUGRcNormal*)uglistMenu_.MakeObj_Record();
      switch (i)
      {
      case ml_GiftBox:newrecord->ugname_.SetNameIcon("おくりものあずかり", ugi_GiftBox); break;
        //      case ml_ShotBox:newrecord->ugname_.SetName("シュートあずかり");  break;
        //      case ml_Dock:newrecord->ugname_.SetName("ミックスかんりょう");  break;
        //      case ml_Gacha:newrecord->ugname_.SetName("みしようガチャコイン"); break;
        //case ml_UpGrade:newrecord->ugname_.SetName("かいいんアップグレード"); break;
      }
    }


    //ログボミックス用ランダム素材
    syouhinvec_add_.clear();
    for (s32 i = 0; i < pmgEO_->mgCoM_.mdm_SpSyouhin_.GetLineNum(); ++i)
    {
      if (pmgEO_->mgCoM_.mdm_SpSyouhin_.GetLineDt(i, mdm_spsyouhin_shopType) == enShopType_BuySozai)
      {
        s32 mid_spsyouhin = pmgEO_->mgCoM_.mdm_SpSyouhin_.GetLineDt(i, mdm_spsyouhin_pk);
        syouhinvec_add_.push_back(mid_spsyouhin);
      }
    }

	/* 2015/06/16 Koike Added Bランクシュート配布用メッセージ追加 */
	ugtitle_spshot_.MakeGroup(field_, FALSE);
	ugtitle_spshot_.SetMessage(LOGBO_SPSHOT_MSG);
	ugtitle_spshot_.SetLRPos_BRank(TRUE);
	ugtitle_spshot_.SetDraw(FALSE);

  }

  //入店時の処理
  void TLyHmMami::DoReady()
  {
    stMami_.Init();

    uglogbo_ = &pScene_->lyHmHome_->uglogbo_;

    ugsp_mami_.SetMsg(mmm_Visit);
    DrawList(NULL);
    ugbtn_back_->SetFont("もどる");
    ugbtn_prev_->SetFont("まえのページ");
    ugbtn_next_->SetFont("つぎのページ");
    ugbtn_ok_->SetFont("おーけー");
    ugbtn_back_->SetDraw(FALSE);
    ugbtn_ok_->SetDraw(FALSE);
  }

  void TLyHmMami::DoDown()
  {
    pScene_->lyHmHome_->Ready();

	// ここでうけとれるものがあるかチェックする
	s32 uketoriNum = pmgEO_->mgCoU_.mdGiftbox_.GetLineNum();
	if (uketoriNum == 0)
	{
		pScene_->lyHmHome_->SetOutMami();
	}
  }

  void TLyHmMami::DrawList(TUGLiGen* drawlist)
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
    ugbtn_back_->SetDraw(TRUE);
    ResetObj();
  }
  void TLyHmMami::ResetObj()
  {
    ugbtn_prev_->SetDraw(FALSE);
    ugbtn_next_->SetDraw(FALSE);
    ugbtn_ok_->SetDraw(FALSE);
  }
  void TLyHmMami::SwitchGiftPageBtn()
  {
    ugbtn_prev_->SetDraw((stMami_.giftpageNo_ > 0));
    ugbtn_next_->SetDraw((stMami_.giftpageNo_ < uglistGiftBox_.GetMaxPage()));
  }
  void TLyHmMami::DoChangePage(s32 pageno)
  {
    switch ((enPage)pageno)
    {
    case db::pg_Visit:
      if (uglogbo_->IsLogobo() == FALSE)
      {
        DrawList(&uglistMenu_);
      }
      break;
	case db::pg_Logbo:
	{
		DrawList(NULL);
		ugsp_mami_.SetMsg(mmm_Logbo);
		SetHudEnd();
		s64 type = pmgEO_->mgCoM_.mdm_Logbo_.GetPKDt(0, enmdm_Logbo::mdm_logbo_itemType);
		s64 id = pmgEO_->mgCoM_.mdm_Logbo_.GetPKDt(0, enmdm_Logbo::mdm_logbo_itemID);
		s64 num = pmgEO_->mgCoM_.mdm_Logbo_.GetPKDt(0, enmdm_Logbo::mdm_logbo_itemNum);
		SetHudItemView_View(type, id, num, TRUE, TRUE);
		ugbtn_back_->SetDraw(FALSE);
	}
      break;
	case db::pg_LogboSp:
	{
		ugsp_mami_.SetMsg(mmm_LogboSp);
		SetHudEnd();
		s64 type = pmgEO_->mgCoM_.mdm_Logbo_.GetPKDt(1, enmdm_Logbo::mdm_logbo_itemType);
		s64 id = pmgEO_->mgCoM_.mdm_Logbo_.GetPKDt(1, enmdm_Logbo::mdm_logbo_itemID);
		s64 num = pmgEO_->mgCoM_.mdm_Logbo_.GetPKDt(1, enmdm_Logbo::mdm_logbo_itemNum);
		SetHudItemView_View(type, id, num, TRUE, TRUE);
		
		// チームのステータス更新
		pmgEO_->RefreshStateTbl(id);

		ugbtn_back_->SetDraw(FALSE);
	}
      break;
	case db::pg_LogboSpYokoku:
	{
#ifdef __K_DEBUG_ROOT__
		s32 loginnum = (5 - (debugLoginNum % 10));
#else
		s32 loginnum = (5 - (pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_loginNum) % 10));
#endif
		ugsp_mami_.SetMsg(mmm_LogboSpYokoku, loginnum);
		SetHudEnd();

		s64 type = pmgEO_->mgCoM_.mdm_Logbo_.GetPKDt(1, enmdm_Logbo::mdm_logbo_itemType);
		s64 id = pmgEO_->mgCoM_.mdm_Logbo_.GetPKDt(1, enmdm_Logbo::mdm_logbo_itemID);
		s64 num = pmgEO_->mgCoM_.mdm_Logbo_.GetPKDt(1, enmdm_Logbo::mdm_logbo_itemNum);
		/* 2015/06/12 Koike modified */
		// ここでボタンメッセージを変更する
		SetHudItemView_View(type, id, num, FALSE, TRUE);
	}
      break;
    case db::pg_LogboResult:
      ugsp_mami_.SetMsg(mmm_LogboResult);
      break;
	case db::pg_LogboShot:
		ugsp_mami_.SetMsg(mmm_LogboShot);
		subPage_ = 0;
		break;
	case db::pg_LogboSpShot:
		ugsp_mami_.SetMsg(mmm_LogboSpShot);
		subPage_ = 0;
		break;
	case db::pg_LogboSpShotYokoku:
	{
#ifdef __K_DEBUG_ROOT__
		s32 loginnum = (10 - (debugLoginNum % 10));
#else
		s32 loginnum = (10 - (pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_loginNum) % 10));
#endif
		ugsp_mami_.SetMsg(mmm_LogboSpShotYokoku, loginnum);
		SetHudEnd();

		// レアリティだけをセットしたいので
		// １０の位は適当、１の位はＢランクの２
		// 謎シュートを表示するためにマイナスの数値
		s32 itemID = -12;
//		SetHudItemView_View(enItemType_Sozai, itemID, 1, TRUE, TRUE);
		/* 2015/06/12 Koike modified */
		// ここでボタンメッセージを変更する
		SetHudItemView_View(enItemType_Sozai, itemID, 1, FALSE, TRUE);
	}
		break;
	case db::pg_LogboShotResult:
		SetHudEnd();
		if (!isCancel_)
		{
			// 空き枠チェック
			if (pmgEO_->mgCoU_.GetShotNum_Emp() > 0 )
			{
				SetHudItemView_Shot(prevsozaivec_, TRUE);
				ugsp_mami_.SetMsg(mmm_LogboShotResult);
			}
			// 空き枠無し
			else
			{
				SetHudItemView_SozaiList(prevsozaivec_, TRUE);
				ugsp_mami_.SetMsg(mmm_LogboShotResult2);
			}
		}
		else
		{
			s64 type;
			s64 id;
			s64 num;

			// もらわなかったらトッドを表示
			if (!isSpShot_)
			{
				// Dランク時なら40トッド
				// pk = 2
				type = pmgEO_->mgCoM_.mdm_Logbo_.GetPKDt(2, enmdm_Logbo::mdm_logbo_itemType);
				id = pmgEO_->mgCoM_.mdm_Logbo_.GetPKDt(2, enmdm_Logbo::mdm_logbo_itemID);
				num = pmgEO_->mgCoM_.mdm_Logbo_.GetPKDt(2, enmdm_Logbo::mdm_logbo_itemNum);
			}
			else
			{
				// Bランク時なら80トッド
				// pk = 3
				type = pmgEO_->mgCoM_.mdm_Logbo_.GetPKDt(3, enmdm_Logbo::mdm_logbo_itemType);
				id = pmgEO_->mgCoM_.mdm_Logbo_.GetPKDt(3, enmdm_Logbo::mdm_logbo_itemID);
				num = pmgEO_->mgCoM_.mdm_Logbo_.GetPKDt(3, enmdm_Logbo::mdm_logbo_itemNum);
			}
			SetHudItemView_View(type, id, num, TRUE, TRUE);
			ugsp_mami_.SetMsg(mmm_LogboShotResult);
		}
		break;
	case db::pg_Menu:
      DrawList(&uglistMenu_);
      ugsp_mami_.SetMsg(mmm_Menu);
      ugbtn_back_->SetDraw(TRUE);
      break;
    case db::pg_GiftBox:
      DrawList(&uglistGiftBox_);
      stMami_.giftpageNo_ = 0;
      uglistGiftBox_.Refresh(stMami_.giftpageNo_);
      ugsp_mami_.SetMsg(mmm_GiftBox);
      ugbtn_ok_->SetDraw(TRUE);
      SwitchGiftPageBtn();
      break;
    case db::pg_GiftBoxResult:
      DrawList(NULL);
      stMami_.giftpageNo_ = 0;
      uglistGiftBox_.Refresh(0);
      SetHudEnd();
      SetHudItemView_GetLog(FALSE, TRUE, TRUE);
      //ugbtn_ok_->SetDraw(TRUE);
      break;
    case db::pg_UpGrade:
      DrawList(NULL);
      mid::midMobageUpGrade2();//現状を観てアップグレード内容を決める0~2
      //ugsp_mami_.SetMsg(mmm_);
      //mid::midModalPanel(&pmgEO_->mgCoInfo_.mdm_Info_.model_);//お知らせパネル表示
      break;
    case db::pg_Out:
      UndrawBtn();
      ugsp_mami_.SetMsg(mmm_Out);
      break;
    default:
      break;
    }
  }
  void TLyHmMami::MvPage_Visit()
  {
    if (ugbtn_back_->IsAct())
    {
      ChangePage(pg_Out);
    }
    else if (ugsp_mami_.ugserifu_.IsActRes(enUGSerifuAct_WaitEnd))
    {
      if (uglogbo_->IsLogobo())
      {
		  ChangePage(pg_Logbo);
      }
      else
      {
        ugsp_mami_.SetMsg(mmm_Menu);
        ugbtn_back_->SetDraw(TRUE);
        ChangePage(pg_Menu);
      }
    }
    else if (uglistMenu_.IsAct())
    {
      ActListMenu();
    }
  }
  //ログボ
  void TLyHmMami::MvPage_Logbo()
  {
	  //アイテム獲得ボタン
	  if (IsHudItemView())
	  {
		  uglogbo_->Refresh();

		  // ログイン回数取得
		  s32 loginnum = pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_loginNum);
#ifdef __K_DEBUG_ROOT__
		  loginnum = debugLoginNum;
#endif

		  // ランダムシュートをプレゼントするが
		  // 10回以上ログイン以降10回ごとにＢランク、それ以外はＤランクを配る
		  isSpShot_ = FALSE;

		  // Ｂランクを渡すときは
		  // ログイン回数を10で割った時に余りが0の時
		  if ((loginnum % 10) == 0)
		  {
			  isSpShot_ = TRUE;
		  }

		  // Bランク
		  if (isSpShot_)
		  {
			  INTVECTOR v;
			  v.clear();
			  for (s32 i = 0; i < pmgEO_->mgCoM_.mdm_LogboSozai_.GetLineNum(); i++)
			  {
				  if (pmgEO_->mgCoM_.mdm_LogboSozai_.GetLineDt(i, enmdm_LogboSozai::mdm_logbosozai_logboType) == 1)
				  {
					  v.push_back(pmgEO_->mgCoM_.mdm_LogboSozai_.GetLineDt(i, enmdm_LogboSozai::mdm_logbosozai_pk));
				  }
			  }
			  // 監督ID＋本日の日付を、抽選ベースシュート総数で割って、余りで配布するシュートを決める
			  s64 mypk = pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_pk);
			  s64 date = pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_logbo_date);
			  s32 tableSize = v.size();
			  s32 no = (mypk + date) % tableSize;
			  mid_logbosozai_ = v[no];
			  ChangePage(pg_LogboSpShot);
		  }
		  // Dランク
		  else
		  {
			  INTVECTOR v;
			  v.clear();
			  for (s32 i = 0; i < pmgEO_->mgCoM_.mdm_LogboSozai_.GetLineNum(); i++)
			  {
				  if (pmgEO_->mgCoM_.mdm_LogboSozai_.GetLineDt(i, enmdm_LogboSozai::mdm_logbosozai_logboType) == 0)
				  {
					  v.push_back(pmgEO_->mgCoM_.mdm_LogboSozai_.GetLineDt(i, enmdm_LogboSozai::mdm_logbosozai_pk));
				  }
			  }
			  // 監督ID＋本日の日付を、抽選ベースシュート総数で割って、余りで配布するシュートを決める
			  s64 mypk = pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_pk);
			  s64 date = pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_logbo_date);
			  s32 tableSize = v.size();
			  s32 no = (mypk + date) % tableSize;
			  mid_logbosozai_ = v[no];
			  ChangePage(pg_LogboShot);
		  }
	  }
  }
  //スペシャルログボ
  void TLyHmMami::MvPage_LogboSp()
  {
	  //アイテム獲得ボタン
	  if (IsHudItemView())
	  {
		  // チームをもらったので
		  // 予告はシュート
		  ChangePage(pg_LogboSpShotYokoku);
	  }
  }
  void TLyHmMami::MvPage_LogboSpYokoku()
  {
	  if (IsHudItemView())
	  {
		  Post_Logbo();
	  }
  }
  //スペシャルログボ
  void TLyHmMami::MvPage_LogboResult()
  {
	  if (ugbtn_back_->IsAct())
	  {
		  if (pmgEO_->mgCoU_.mdGiftbox_.GetLineNum())
		  {
			  ChangePage(pg_Menu);
		  }
		  else
		  {
			  ChangePage(pg_Out);
		  }
	  }
  }
  //素材リセット
  void TLyHmMami::ResetSozaiVec()
  {
	  prevsozaivec_.clear();
	  spsyouhinvec_.clear();
	  //spsyouhinvec_.push_back(syouhinvec_add_[mid::midGetRandMn(syouhinvec_add_.size())]);
	  //spsyouhinvec_.push_back(syouhinvec_add_[mid::midGetRandMn(syouhinvec_add_.size())]);
	  //spsyouhinvec_.push_back(syouhinvec_add_[mid::midGetRandMn(syouhinvec_add_.size())]);

	  TSozaiData sozai_base;
	  sozai_base.SetMidSozai(pmgEO_->mgCoM_.mdm_LogboSozai_.GetPKDt(mid_logbosozai_, mdm_logbosozai_itemID));
	  prevsozaivec_.push_back(sozai_base);

	  for (s32 i = 0; i < sozai_base.rarity_ + 1; i++)
	  {
		  spsyouhinvec_.push_back(syouhinvec_add_[mid::midGetRandMn(syouhinvec_add_.size())]);
	  }	  

	  for (s32 i = 0; i < spsyouhinvec_.size(); ++i)
	  {
		  TSozaiData sozai;
		  sozai.SetMidSozai(pmgEO_->mgCoM_.mdm_SpSyouhin_.GetPKDt(spsyouhinvec_[i], mdm_spsyouhin_itemID));
		  prevsozaivec_.push_back(sozai);
	  }
  }
  void TLyHmMami::MvPage_LogboShot()
  {
	  enum
	  {
		  subPage_Top = 0,
		  subPage_Top2,
		  subPage_Setup,
		  subPage_Preview,
	  };

	  switch (subPage_)
	  {
		  // プレビュー画面前(まみのメッセージ送り前)
	  case subPage_Top:
		  if (ugsp_mami_.IsAct())
		  {
			  // もらわなかったらトッドを表示
			  // Dランク時なら40トッド
			  // pk = 2
			  s32 num = pmgEO_->mgCoM_.mdm_Logbo_.GetPKDt(2, enmdm_Logbo::mdm_logbo_itemNum);
			  ugsp_mami_.SetMsg(mmm_LogboShot2, num);
			  subPage_++;
		  }
		  break;

	  case subPage_Top2:
		  if (ugsp_mami_.IsAct())
		  {
			  DrawList(NULL);
			  ugbtn_back_->SetDraw(FALSE);

			  ugbtn_back_->SetFont("うけとる");
			  ugbtn_ok_->SetFont("イマイチ");
			  ugbtn_prev_->SetFont("いらない");
			  ugbtn_prev_->SetDraw(TRUE);

			  SOZAIDTVECTOR v;
			  v.clear();
			  TSozaiData sozai;
			  // ここにシュートの素材ベースを決める処理を書く
			  sozai.SetMidSozai(pmgEO_->mgCoM_.mdm_LogboSozai_.GetPKDt(mid_logbosozai_, mdm_logbosozai_itemID));
			  v.push_back(sozai);

			  TPrevParam param;
			  param.nonebackbtn_f = TRUE;
			  param.nonemixbtn_f = TRUE;
				param.nonehavemixbtn_f = TRUE;
			  param.sozaivec = &v;
			  SetHudPrev(param);
			  isCancel_ = FALSE;

			  subPage_++;
		  }
		  break;

		  // プレビューの準備
	  case subPage_Setup:
	  {
		  // 素材をベースにランダムセット
		  ResetSozaiVec();
		  SetHudPrevChangeShot(prevsozaivec_);
		  //SetHudPrevMixSozai(sozai_id, TRUE);
		  s64 sozai = pmgEO_->mgCoM_.mdm_LogboSozai_.GetPKDt(mid_logbosozai_, mdm_logbosozai_itemID) / 10;// レアリティが入っているので10で割って消す
		  ugsp_mami_.SetMsg(mmm_LogboShotPrev, sozai);
			// Dランク場合は非表示に
			ugtitle_spshot_.SetDraw(FALSE);
		  subPage_++;
	  }
	  break;

	  // プレビュー中
	  case subPage_Preview:
		  // 敵に当たった
		  if (IsHudPrevHit())
		  {
			  // おーけー、ダメダメボタン表示
			  if (!ugbtn_back_->IsDraw())
			  {
				  ugbtn_back_->SetDraw(TRUE);
				  ugbtn_ok_->SetDraw(TRUE);
				  ugsp_mami_.SetMsg(mmm_LogboShotPrev2);
			  }
		  }

		  // おーけーボタン
		  if (ugbtn_back_->IsAct())
		  {
			  // 文字を元に戻す
			  ugbtn_back_->SetFont("もどる");
			  ugbtn_ok_->SetFont("うけとる");
			  ugbtn_prev_->SetFont("まえのページ");

			  //POSTする内容
			  ugbtn_back_->SetDraw(FALSE);
			  ugbtn_ok_->SetDraw(FALSE);
			  ugbtn_prev_->SetDraw(FALSE);
				ugtitle_spshot_.SetDraw(FALSE);
			  ChangePage(pg_LogboShotResult);
			  //Post_LogboShot();
		  }
		  // ダメダメボタン
		  else if (ugbtn_ok_->IsAct())
		  {
			  ugbtn_back_->SetDraw(FALSE);
			  ugbtn_ok_->SetDraw(FALSE);
			  subPage_ = subPage_Setup;
		  }
		  // いらないボタン
		  else if(ugbtn_prev_->IsAct())
		  {
			  // 文字を元に戻す
			  ugbtn_back_->SetFont("もどる");
			  ugbtn_ok_->SetFont("うけとる");
			  ugbtn_prev_->SetFont("まえのページ");

			  ugbtn_back_->SetDraw(FALSE);
			  ugbtn_ok_->SetDraw(FALSE);
			  ugbtn_prev_->SetDraw(FALSE);
				ugtitle_spshot_.SetDraw(FALSE);

			  isCancel_ = TRUE;

			  ChangePage(pg_LogboShotResult);
			  //Post_LogboShot();
		  }
	  }
  }
  void TLyHmMami::MvPage_LogboSpShot()
  {
	  enum
	  {
		  subPage_Top = 0,
		  subPage_Top2,
		  subPage_Setup,
		  subPage_Preview,
	  };

	  switch (subPage_)
	  {
		  // プレビュー画面前(まみのメッセージ送り前)
	  case subPage_Top:
		  MvPage_LogboShot();
		  if (subPage_ == subPage_Top2)
		  {
			  // もらわなかったらトッドを表示
			  // Bランク時なら80トッド
			  // pk = 3
			  s32 num = pmgEO_->mgCoM_.mdm_Logbo_.GetPKDt(3, enmdm_Logbo::mdm_logbo_itemNum);
			  ugsp_mami_.SetMsg(mmm_LogboShot2, num);
		  }
		  break;

	  case subPage_Top2:
		  MvPage_LogboShot();
		  break;

		  // プレビューの準備
	  case subPage_Setup:
		{
			MvPage_LogboShot();
			s64 sozai = pmgEO_->mgCoM_.mdm_LogboSozai_.GetPKDt(mid_logbosozai_, mdm_logbosozai_itemID) / 10; // レアリティが入っているので10で割って消す
			ugsp_mami_.SetMsg(mmm_LogboShotPrev, sozai);
			/* 2015/06/16 Koike Added */
			// Bランクシュートの場合はメッセージを表示
			ugtitle_spshot_.SetDraw(TRUE);
		}
		  break;
		  // プレビュー中
	  case subPage_Preview:
		  MvPage_LogboShot();
		  break;
	  }
  }
  void TLyHmMami::MvPage_LogboSpShotYokoku()
  {
	  if (IsHudItemView())
	  {
		  Post_Logbo();
	  }
  }
  void TLyHmMami::MvPage_LogboShotResult()
  {
	  if (IsHudItemView())
	  {
		  // ログイン回数取得
		  s32 loginnum = pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_loginNum);
#ifdef __K_DEBUG_ROOT__
		  loginnum = debugLoginNum;
#endif
		  // 余りを計算
		  s32 remainder = loginnum % 10;

		  //---------------------------------------------------------
		  // ログインボーナスチームを渡す条件は
		  // ログイン回数を10で割った時に余りが5の時
		  if (remainder == 5)
		  {
			  ChangePage(pg_LogboSp);
		  }
		  //---------------------------------------------------------
		  // ここからはどちらの予告を出すかのチェック
		  // チームの予告は
		  // ログイン回数を10で割った時に余りが0～4の時
		  else if (remainder >= 0 && remainder < 5)
		  {
			  ChangePage(pg_LogboSpYokoku);
		  }
		  // Bランクシュートの予告は
		  // ログイン回数を10で割った時に余りが5～9の時
		  else
		  {
			  ChangePage(pg_LogboSpShotYokoku);
		  }
	  }
  }
  void TLyHmMami::MvPage_Menu()
  {
    if (ugbtn_back_->IsAct())
    {
      ChangePage(pg_Out);
    }
    else if (uglistMenu_.IsAct())
    {
      ActListMenu();
    }
  }
  void TLyHmMami::MvPage_GiftBox()
  {
    //出る
    if (ugbtn_back_->IsAct())
    {
      ChangePage(pg_Menu);
    }
    else if (ugbtn_prev_->IsAct() && (stMami_.giftpageNo_ > 0))
    {
      --stMami_.giftpageNo_;
      //Post_CheckGiftBox();
      uglistGiftBox_.Refresh(stMami_.giftpageNo_);
      SwitchGiftPageBtn();
    }
    else if ((ugbtn_next_->IsAct() && (stMami_.giftpageNo_ < uglistGiftBox_.GetMaxPage())))
    {
      ++stMami_.giftpageNo_;
      //Post_CheckGiftBox();
      uglistGiftBox_.Refresh(stMami_.giftpageNo_);
      SwitchGiftPageBtn();
    }
    else if (ugbtn_ok_->IsAct() && (uglistGiftBox_.GetSelectNum() > 0))
    {
      openidvec_ = uglistGiftBox_.GetIDOpen();

      //POST：素材受け取り
      Post_OpenGiftBox();
    }
  }
  void TLyHmMami::MvPage_GiftBoxResult()
  {
    //アイテム獲得ボタン
    if (IsHudItemView())
    {
      ChangePage(pg_Menu);//もどる
    }
  }
  void TLyHmMami::MvPage_UpGrade()
  {
    //出る
    if (ugbtn_back_->IsAct())
    {
      ChangePage(pg_Menu);
    }
  }
  void TLyHmMami::MvPage_Out()
  {
    if (ugsp_mami_.IsAct())
    {
      Down();
    }
  }
  //*******************
  //POSTログボ
  void TLyHmMami::Post_Logbo()
  {
	  if (pmgEO_->stSeq_.IsNoSv())
	  {
		  Connect_Logbo();
	  }
	  else
	  {
		  StartConnect(cn_Logbo);
      pmgEO_->mgPo_.poHome_.Ready_logbo(isCancel_, mid_logbosozai_, &spsyouhinvec_);
	  }
  }

  //*******************
  //POST贈り物ＢＯＸ最初とページ切り替え
  void TLyHmMami::Post_CheckGiftBox()
  {
    StartConnect(cn_CheckGiftBox);
    pmgEO_->mgPo_.poHome_.Ready_checkgiftbox();

    if (pmgEO_->stSeq_.IsNoSv()) Connect_CheckGiftBox();
  }
  //POST贈り物ＢＯＸ
  void TLyHmMami::Post_OpenGiftBox()
  {
    StartConnect(cn_OpenGiftBox);
    pmgEO_->mgPo_.poHome_.Ready_opengiftbox(&openidvec_);

    if (pmgEO_->stSeq_.IsNoSv()) Connect_OpenGiftBox();
  }

  //*******************
  //接続ログボ
  void TLyHmMami::Connect_Logbo()
  {
    //贈り物オープンにより真美止まるかのフラグ更新
    pmgEO_->stSeq_.SetDrawFlag(
      pmgEO_->mgCoU_.mdre_Refresh_.GetLineDt(0, mdre_refresh_mami_f), 
      pmgEO_->mgCoU_.mdre_Refresh_.GetLineDt(0, mdre_refresh_maruka_f));

	if (pmgEO_->mgCoU_.mdGiftbox_.GetLineNum())
	{
		ChangePage(pg_Menu);
	}
	else
	{
		ChangePage(pg_Out);
	}
  }

  //接続贈り物ＢＯＸ（最初）
  void TLyHmMami::Connect_CheckGiftBox()
  {
    //一応一回で良い
    stMami_.checked_f_ = TRUE;
    SetEndPage();
    //１ページ目と全体数読み込めたら表示ページに移る
    ChangePage(pg_GiftBox);
    SwitchGiftPageBtn();
  }
  //接続贈り物ＢＯＸ
  void TLyHmMami::Connect_OpenGiftBox()
  {
    //贈り物ＢＯＸは数が変わるので毎回セット
    ChangePage(pg_GiftBoxResult);

    //贈り物オープンにより真美止まるかのフラグ更新
    pmgEO_->stSeq_.SetDrawFlag(
      pmgEO_->mgCoU_.mdre_Refresh_.GetLineDt(0, mdre_refresh_mami_f),
      pmgEO_->mgCoU_.mdre_Refresh_.GetLineDt(0, mdre_refresh_maruka_f));
  }
  //接続完了
  void TLyHmMami::DoConnect(BOOL rqok_f)
  {
    if (rqok_f)//ＯＫのみ
    {
      //code0
      if (pmgEO_->mgPo_.IsCodeZero())
      {
        switch (stLy_.connect_)
        {
        case cn_Logbo: Connect_Logbo(); break;
        case cn_CheckGiftBox: Connect_CheckGiftBox(); break;
        case cn_OpenGiftBox: Connect_OpenGiftBox(); break;
        }
      }
      else
      {
        switch (stLy_.connect_)
        {
        case cn_Logbo: ChangePage(pg_Menu); break;
        case cn_CheckGiftBox: ChangePage(pg_Menu); break;
        case cn_OpenGiftBox: ChangePage(pg_Menu); break;
        }
        //エラーメッセージ(ここでstLy_.connect_のクリアが入ってしまう)
        EndConnectNG(pmgEO_->mgPo_.GetCode(), pmgEO_->mgPo_.GetMsg().c_str());
      }
    }
    else
    {
      //通信エラー
      SetHudEnd();
      ChangePage(pg_Menu);
    }
  }
  //POSTの結果から最終ページ番号取得
  void TLyHmMami::SetEndPage()
  {
    //stMami_.endpage_ = pmgEO_->mgPo_.GetValue("endpage");
  }

  //*******************

  void TLyHmMami::DoUpdate(BOOL draw_f)
  {
#if 0
#ifdef __K_DEBUG_ROOT__
	  if (kdebug::DebugController::GetInstance()->IsTriggerButton(XINPUT_BUTTON_LEFT_SHOULDER))
	  {
		  debugLoginNum--;
		  if (debugLoginNum <= 1){ debugLoginNum = 1; }
	  }
	  else if (kdebug::DebugController::GetInstance()->IsTriggerButton(XINPUT_BUTTON_RIGHT_SHOULDER))
	  {
		  debugLoginNum++;
	  }

	  kdebug::DebugText::GetInstance()->SetColor(kdebug::COL_RED);
	  char str_c[256];
	  sprintf(str_c, "LoginNum:%d", debugLoginNum);
	  kdebug::DebugText::GetInstance()->SetDebugText(305, 10, str_c);

	  BOOL isSkip = FALSE;
	  std::string stateStr;
	  switch (stLy_.pageNo_)
	  {
	  default:
		  isSkip = TRUE;
		  break;
	  case pg_Logbo: stateStr = "pg_Logbo"; break;
	  case pg_LogboSp: stateStr = "pg_LogboSp"; break;
	  case pg_LogboSpYokoku: stateStr = "pg_LogboSpYokoku"; break;
	  case pg_LogboResult: stateStr = "pg_LogboResult"; break;
	  case pg_LogboShot: stateStr = "pg_LogboShot"; break;
	  case pg_LogboSpShot: stateStr = "pg_LogboSpShot"; break;
	  case pg_LogboSpShotYokoku: stateStr = "pg_LogboSpShotYokoku"; break;
	  case pg_LogboShotResult: stateStr = "pg_LogboShotResult"; break;
	  }
	  if (!isSkip)
	  {
		  sprintf(str_c, "state:%s", stateStr.c_str());
		  kdebug::DebugText::GetInstance()->SetDebugText(305, 18, str_c);
	  }
#endif
#endif
    switch (stLy_.pageNo_)
    {
    case pg_Out:
      ugsp_mami_.Update(draw_f);
      break;
    default:
      ugsp_mami_.Update(draw_f);
      uglistMenu_.Update(draw_f);
      uglistGiftBox_.Update(draw_f);
			/* 2015/06/16 Koike Added Bランクシュート用メッセージ */
			ugtitle_spshot_.Update(draw_f);
      break;
    }

    if (draw_f == FALSE)
    {
      switch (stLy_.pageNo_)
      {
      case pg_Visit: MvPage_Visit(); break;
      case pg_Logbo: MvPage_Logbo(); break;
      case pg_LogboSp: MvPage_LogboSp(); break;
      case pg_LogboSpYokoku: MvPage_LogboSpYokoku(); break;
      case pg_LogboResult: MvPage_LogboResult(); break;
			case pg_LogboShot:MvPage_LogboShot(); break;
			case pg_LogboSpShot:MvPage_LogboSpShot(); break;
			case pg_LogboSpShotYokoku: MvPage_LogboSpShotYokoku(); break;
			case pg_LogboShotResult:MvPage_LogboShotResult(); break;
      case pg_Menu: MvPage_Menu(); break;
      case pg_GiftBox: MvPage_GiftBox(); break;
      case pg_GiftBoxResult: MvPage_GiftBoxResult(); break;
      case pg_UpGrade: MvPage_UpGrade(); break;
      case pg_Out: MvPage_Out(); break;
      }
    }
  }

  void TLyHmMami::ActListMenu()
  {
    switch ((enRoomList)uglistMenu_.GetActIndex())
    {
    case db::ml_GiftBox:
      stMami_.giftpageNo_ = 0;
      if (stMami_.checked_f_)
      {
        ChangePage(pg_GiftBox);
      }
      else
      {
        Post_CheckGiftBox();
      }
      break;
      //      case db::ml_ShotBox:ChangePage(pg_GiftBox); break;
      //      case db::ml_Dock:ChangePage(pg_GiftBox); break;
      //      case db::ml_Gacha:ChangePage(pg_GiftBox); break;
      //      case db::ml_UpGrade:ChangePage(pg_UpGrade); break;
    }
  }

}
