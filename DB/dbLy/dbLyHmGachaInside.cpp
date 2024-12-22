#include "dbLyHmGachaInside.h"
#include "dbSnHome.h"
#include "dbUGPrev.h"

namespace db
{

  enum enPage
  {
    pg_Visit,
    pg_SelectType,
    pg_SelectNaBox,
    pg_SelectNaBoxNum,
    pg_DecideNaBox,
    pg_NaBoxCnt,
    pg_NaBoxCntEnd,
		pg_NaBoxPrev,
    pg_SelectGacha,
    pg_DecideGacha,
    pg_TousenGrade,
    pg_Tousen,
    pg_TousenEnd,
    pg_Prob,
    pg_ProbCnt,
    pg_ProbCntEnd,
    pg_NaBoxRoll,
    pg_GachaRoll,
    pg_NaBoxResult,
    pg_GachaResult,
    pg_Out,
  };

  enum enConnect
  {
    cn_Gacha,
    cn_Tousen,
    cn_NaBox,
    cn_NaBoxOtona,
  };

  enum enGatyaType
  {
    gt_Star,
    gt_Box,
    gt_Todd,
    gtEND,
  };

  const s32 INTRO_TIMER_NUM = 90;	// 90フレームだけイントロ再生

  //コンストラクタ
  TLyHmGachaInside::TLyHmGachaInside(TSnHome* pScene)
    :TLyHmGen(pScene)
    , ugsp_gacha_(this)
    , uglistMenu_(this)
    , uglistGacha_(this)
    , uglistProbCnt_(this)
    , uglistNaBox_(this)
    , uglistNaBoxNum_(this)
    , uglistNaBoxCnt_(this)
    , uglistProb_(this)
    , uglistTousenGrade_(this)
    , uglistTousen_(this)
    , ugprof_(this)
    , introBGMID_(-1)
    , selectListNo_(-1)
  {
    stGacha_.Init();
  }

  TLyHmGachaInside::~TLyHmGachaInside()
  {

  }


  // メニューの中から開始
  // 配列数→階層数
  // 配列の中身→メニュー番号
  void TLyHmGachaInside::SetupMenu(void)
  {
    INTVECTOR v = pmgEO_->stJump_.GetJumpData();
	s32 msgNo = NGNUM;

	  for (s32 i = 0; i < v.size(); i++)
	  {
		  if (v[i] <= 0){ break; }

		  // 1階層目
		  if (i == 0)
		  {
			  // とりあえず1階層バージョンだけ
			  stGacha_.gahcatype_ = v[0]-1;
			  if (stGacha_.gahcatype_ == gt_Box)
			  {
				  ChangePage(pg_SelectNaBox);
				  s32 fromTab = pmgEO_->stJump_.GetFromTab();
				  switch (fromTab)
				  {
				  default:
				  case footer_id_home:
					  msgNo = gamt_JumpInNaBoxFromHome;
					  break;
				  case footer_id_team:
					  msgNo = gamt_JumpInNaBoxFromTeam;
					  break;
				  }
			  }
			  else
			  {
				  if (stGacha_.gahcatype_ == gt_Todd)
				  {
					  stGacha_.todd_f_ = TRUE;
					  msgNo = gamt_JumpInTodd;
				  }
				  else
				  {
					  stGacha_.todd_f_ = FALSE;
					  msgNo = gamt_JumpInStar;
				  }
				  ChangePage(pg_SelectGacha);
			  }
		  }
		  // 2階層目
		  else
		  {
			  if (stGacha_.gahcatype_ == gt_Box)
			  {
				  //s32 recordidx = v[1] - 1;
				  //stGacha_.mid_gacha_ = uglistNaBox_.records_[recordidx]->GetCommonID();//なつかしＭＩＤ
				  stGacha_.mid_gacha_ = v[1];
				  ChangePage(pg_SelectNaBoxNum);
			  }
			  else
			  {
				  s32 recordidx = v[1]-1;
				  uglistGacha_.SetListEnd(recordidx);
				  stGacha_.mid_gacha_ = uglistGacha_.records_[recordidx]->GetCommonID();//ガチャＭＩＤ
				  ChangePage(pg_DecideGacha);
				  if (stGacha_.todd_f_)
				  {
					  //ugsp_gacha_.SetMsg(gamt_DecideStar);
				  }
				  else
				  {
					  //ugsp_gacha_.SetMsg(gamt_DecideTodd);
				  }
			  }
		  }
	  }

	  if (msgNo != NGNUM)
	  {
		  ugsp_gacha_.SetMsg((enGachaMsgType)msgNo);
	  }
  }


  void TLyHmGachaInside::DoLoad()
  {
    //ショップフィールド全生成
    ugsp_gacha_.MakeGroup();

    uglistMenu_.MakeGroup(field_, FALSE);
    uglistGacha_.MakeGroup(field_, FALSE);
    uglistProbCnt_.MakeGroup(field_, FALSE);

    uglistNaBox_.MakeGroup(field_, FALSE);
    uglistNaBoxCnt_.MakeGroup(field_, FALSE);
    uglistNaBoxNum_.MakeGroup(field_, FALSE);

    uglistProb_.MakeGroup(field_, FALSE);
    uglistTousenGrade_.MakeGroup(field_, FALSE);
    uglistTousen_.MakeGroup(field_, FALSE);

    ugprof_.MakeGroup(field_, TRUE);
    //ugprof_.SetProfMe();

    listvec_.clear();
    listvec_.push_back(&uglistMenu_);
    listvec_.push_back(&uglistGacha_);
    listvec_.push_back(&uglistProbCnt_);
    listvec_.push_back(&uglistNaBox_);
    listvec_.push_back(&uglistNaBoxNum_);
    listvec_.push_back(&uglistNaBoxCnt_);
    listvec_.push_back(&uglistProb_);
    listvec_.push_back(&uglistTousenGrade_);
    listvec_.push_back(&uglistTousen_);

    //各レコード
    uglistNaBox_.Refresh();

    //トップメニュー
    for (s32 i = 0; i < gtEND; ++i)
    {
      TUGRcNormal* newrecord = (TUGRcNormal*)uglistMenu_.MakeObj_Record();
      switch (i)
      {
      case gt_Star: newrecord->SetNameIcon("スターガチャ", ugi_StarGacha); break;
      case gt_Box: newrecord->SetNameIcon("なつかしＢＯＸ", ugi_NaBox); break;
      case gt_Todd: newrecord->SetNameIcon("トッドガチャ", ugi_Gacha); break;
      }
    }

    ////当選者
    //const s32 GRADENUM = 4;
    //const std::string GRADE[] =
    //{
    //  "ＳＳＳランク",
    //  "ＳＳランク",
    //  "Ｓランク",
    //  "Ａランク",
    //};
    //for (s32 i = 0; i < GRADENUM; ++i)
    //{
    //  TUGRcNormal* newrecord = (TUGRcNormal*)uglistTousenGrade_.MakeObj_Record();
    //  s32 grade = RARITYNUM - i - 1;
    //  newrecord->SetCommonID(grade);//等級
    //  newrecord->SetName(GRADE[i].c_str());
    //  newrecord->ugname_.ugicon_.SetIconCup(grade);
    //}

    ResetObj();
  }
  //入店時の処理
  void TLyHmGachaInside::DoReady()
  {
    stGacha_.Init();
    pmgEO_->mgNSF_.Play(nsf_shop_gatya);//BGM
    ugsp_gacha_.Ready();
    //ugbtn_back_->SetFont("もどる");
	/* 2015/06/12 Koike Modified */
	//「みせをでる」に変更
	ugbtn_back_->SetFont("みせをでる");
    ugbtn_mid_->SetFont("ガチャかくりつ");
    ugbtn_ok_->SetFont("とうせんしゃ");
    DrawList(NULL);

    introBGMID_ = -1;
    selectListNo_ = -1;
  }
  void TLyHmGachaInside::DoDown()
  {
    pScene_->lyHmShop_->OutShop();
    pmgEO_->mgNSF_.Play(nsf_home);//BGM
  }

  //リストを全て閉じる
  void TLyHmGachaInside::DrawList(TUGLiGen* drawlist)
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
  //当選ランクリフレッシュ
  void TLyHmGachaInside::RefreshTousenGrade(BOOL todd_f)
  {
    uglistTousenGrade_.ClearRecord();
    if (todd_f)
    {
      TUGRcNormal* newrecord = (TUGRcNormal*)uglistTousenGrade_.MakeObj_Record();
      s32 grade = 3;
      newrecord->SetCommonID(grade);//等級
      newrecord->SetName("Ａランク");
      newrecord->ugname_.ugicon_.SetIconCup(grade);
    }
    else
    {
      //当選者
      const s32 GRADENUM = 3;
      const std::string GRADE[] =
      {
//        "ＳＳＳランク",
        "ＳＳランク",
        "Ｓランク",
        "Ａランク",
      };
      for (s32 i = 0; i < GRADENUM; ++i)
      {
        TUGRcNormal* newrecord = (TUGRcNormal*)uglistTousenGrade_.MakeObj_Record();
        s32 grade = RARITYNUM - i - 2;
        newrecord->SetCommonID(grade);//等級
        newrecord->SetName(GRADE[i].c_str());
        newrecord->ugname_.ugicon_.SetIconCup(grade);
      }
    }
  }

  // チーム取得時にステータス更新を行う関数
  void TLyHmGachaInside::CheckGetLogTeam(void)
  {
	  s32 num = pmgEO_->mgCoU_.mdlog_ItemGet_.GetLineNum();
	  LONGVECTOR checkEndList;
	  checkEndList.clear();
	  for (s32 i = 0; i < num; i++)
	  {
		  // チームじゃなければ次へ
		  enItemType itemtype = (enItemType)pmgEO_->mgCoU_.mdlog_ItemGet_.GetLineDt(i, mdlog_itemget_itemType);
		  if (itemtype != enItemType_Team){ continue; }

		  s64 itemid = pmgEO_->mgCoU_.mdlog_ItemGet_.GetLineDt(i, mdlog_itemget_itemID);
		  BOOL isCheckEnd = FALSE;
		  for (s32 j = 0; j < checkEndList.size(); j++)
		  {
			  // すでにステータス更新済みチーム
			  if (checkEndList[j] == itemid)
			  {
				  isCheckEnd = TRUE;
				  break;
			  }
		  }
		  // 更新済みチームだったら次へ
		  if (isCheckEnd){ continue; }

		  // チームのステータス更新
		  pmgEO_->RefreshStateTbl(itemid);
		  checkEndList.push_back(itemid);
	  }
  }


  //ボタンを全て閉じる
  void TLyHmGachaInside::ResetObj()
  {
    ugprof_.SetDraw(FALSE);
    ugbtn_mid_->SetDraw(FALSE);
    ugbtn_ok_->SetDraw(FALSE);
    ugbtn_tsh_->SetDraw(FALSE);
    ugbtn_back_->SetDraw(TRUE);
  }

  //ページ切り替え
  void TLyHmGachaInside::DoChangePage(s32 pageno)
  {
    switch ((enPage)pageno)
    {
    case db::pg_Visit:
      DrawList(&uglistMenu_);
	  ugbtn_back_->SetFont("みせをでる");
      break;
    case db::pg_SelectType:
      DrawList(&uglistMenu_);
	  ugbtn_back_->SetFont("みせをでる");
	  break;
    case db::pg_SelectNaBox:
      DrawList(&uglistNaBox_);
      uglistNaBox_.Refresh();
      ugbtn_tsh_->SetDraw(TRUE);
      ugr_star_->Refresh();
	  ugbtn_back_->SetFont("もどる");
      break;
    case db::pg_SelectNaBoxNum:
      DrawList(&uglistNaBoxNum_);
      uglistNaBoxNum_.Refresh(stGacha_.mid_gacha_);
      uglistNaBoxNum_.ChangeListEnd(FALSE);
      //ugbtn_mid_->SetFont("ＢＯＸなかみ", FC2_normal);
      //ugbtn_mid_->SetDraw(TRUE);
      ugbtn_tsh_->SetDraw(TRUE);
      ugr_star_->Refresh();
      break;
	case db::pg_DecideNaBox:
		DrawList(&uglistNaBoxNum_);
		uglistNaBoxNum_.SetListEnd(stGacha_.otona_f_);
		uglistNaBoxNum_.ChangeListEnd(TRUE);
		ugbtn_tsh_->SetDraw(TRUE);
		ugr_star_->Refresh();
      break;
    case db::pg_NaBoxCnt:
      DrawList(&uglistNaBoxCnt_);
      uglistNaBoxCnt_.Refresh_NaBox(stGacha_.mid_gacha_);
      ugbtn_back_->SetDraw(TRUE);
      break;
	case db::pg_NaBoxCntEnd:
	{
		DrawList(NULL);
		isChangeBGM_ = FALSE;
		introBGMID_ = -1;
		ugbtn_back_->SetDraw(FALSE);
		
		// 選択してるアイテムを調べる
		enItemType itemtype = (enItemType)pmgEO_->mgCoM_.mdm_NaBoxCnt_.GetPKDt(selectListNo_, mdm_naboxcnt_itemType);
		if (itemtype == enItemType_Sozai)
		{
			ugbtn_mid_->SetFont("プレビュー", FC2_normal);
			ugbtn_mid_->SetDraw(TRUE);
		}

	}
      break;
  case db::pg_NaBoxPrev:
  {
    TPrevParam param;
		param.nonehavemixbtn_f = TRUE;

    DrawList(NULL);
    SetHudPrev(param);
    SetHudPrevChangeShot(prevvec_);

  }
		break;
    case db::pg_SelectGacha:
      DrawList(&uglistGacha_);
      uglistGacha_.Refresh(stGacha_.todd_f_);
      uglistGacha_.ChangeListEnd(FALSE);
      ugbtn_tsh_->SetDraw(TRUE);
      if (stGacha_.todd_f_)
      {
        ugr_todd_->Refresh();
        ugsp_gacha_.uggacha_.SetGachaType(enUGGachaType_GachaTodd);
      }
      else
      {
        ugr_star_->Refresh();
        ugsp_gacha_.uggacha_.SetGachaType(enUGGachaType_GachaStar);
      }
	  ugbtn_back_->SetFont("もどる");
      break;
    case db::pg_DecideGacha:
      DrawList(&uglistGacha_);
      uglistGacha_.ChangeListEnd(TRUE);
      ugbtn_mid_->SetFont("ガチャかくりつ", FC2_normal);
      ugbtn_mid_->SetDraw(TRUE);
      ugbtn_ok_->SetDraw(TRUE);
      ugbtn_tsh_->SetDraw(TRUE);
      if (stGacha_.todd_f_)
      {
        ugr_todd_->Refresh();
      }
      else
      {
        if (pmgEO_->mgCoU_.IsGachaKenUse(stGacha_.mid_gacha_))
        {
          ugr_gacha_->RefreshGacha(stGacha_.mid_gacha_);
        }
        else
        {
          ugr_star_->Refresh();
        }
      }
      break;
    case db::pg_TousenGrade:
      DrawList(&uglistTousenGrade_);
      RefreshTousenGrade(stGacha_.todd_f_);
      break;
    case db::pg_Tousen:
      DrawList(&uglistTousen_);
      pmgEO_->mgCoU_.MakeChestEnemy();//敵アイコン作成
      uglistTousen_.Refresh_Gacha();
      uglistTousen_.ChangeListEnd(FALSE);
      if (uglistTousen_.records_.size() > 0)
      {
        ugprof_.SetDraw(TRUE);
        ugprof_.SetProfEnemy(0);
      }
      break;
    case db::pg_TousenEnd:
      ugprof_.SetDraw(TRUE);
      uglistTousen_.SetListEnd(uglistTousen_.GetActIndex());
      uglistTousen_.ChangeListEnd(TRUE);
      break;
    case db::pg_Prob:
      DrawList(&uglistProb_);
      uglistProb_.Refresh(stGacha_.mid_gacha_);
      break;
    case db::pg_ProbCnt:
      DrawList(&uglistProbCnt_);
      ugbtn_back_->SetDraw(TRUE);
      uglistProbCnt_.ChangeListEnd(FALSE);
      break;
    case db::pg_ProbCntEnd:
      DrawList(NULL);
      ugbtn_back_->SetDraw(FALSE);
      break;
    case db::pg_NaBoxRoll:
      DrawList(NULL);
      ugbtn_back_->SetDraw(FALSE);
      break;
    case db::pg_GachaRoll:
      DrawList(NULL);
      ugbtn_back_->SetDraw(FALSE);
      break;
    case db::pg_NaBoxResult:
	{
      DrawList(NULL);
      SetHudItemView_GetLog(FALSE, FALSE, TRUE);
	  CheckGetLogTeam();
	}
      break;
    case db::pg_GachaResult:
	{
      DrawList(NULL);
      SetHudItemView_GetLog(FALSE, FALSE, TRUE);
	  CheckGetLogTeam();
	}
      break;
    case db::pg_Out:
      UndrawBtn();
      break;
    default:
      break;
    }
  }

  void TLyHmGachaInside::ActListMenu()
  {
    stGacha_.gahcatype_ = uglistMenu_.GetActIndex();

    if (stGacha_.gahcatype_ == gt_Box)
    {
      ChangePage(pg_SelectNaBox);
      ugsp_gacha_.SetMsg(gamt_SelectNaBox);
    }
    else
    {
      if (stGacha_.gahcatype_ == gt_Todd)
      {
        stGacha_.todd_f_ = TRUE;
        ugsp_gacha_.SetMsg(gamt_SelectTodd);
      }
      else
      {
        stGacha_.todd_f_ = FALSE;
        ugsp_gacha_.SetMsg(gamt_SelectStar);
      }
      ChangePage(pg_SelectGacha);
    }
  }
  //来店
  void TLyHmGachaInside::MvPage_Visit()
  {
    if (ugbtn_back_->IsAct())
    {
      OutShop();
    }
    else if (ugsp_gacha_.ugserifu_.IsActRes(enUGSerifuAct_WaitEnd))
    {
      ChangePage(pg_SelectType);
      ugsp_gacha_.SetMsg(gamt_Menu);
    }
    else if (uglistMenu_.IsAct())//レコード選択
    {
      ActListMenu();
    }
  }
  //ガチャタイプ選択
  void TLyHmGachaInside::MvPage_SelectType()
  {
    if (ugbtn_back_->IsAct())
    {
      OutShop();
    }
    else if (uglistMenu_.IsAct())//レコード選択
    {
      ActListMenu();
    }
  }
  //懐かしボックス選択
  void TLyHmGachaInside::MvPage_SelectNaBox()
  {
    if (ugbtn_back_->IsAct())
    {
      ChangePage(pg_SelectType);
    }
    else if (uglistNaBox_.IsAct())//レコード選択
    {
      stGacha_.mid_gacha_ = uglistNaBox_.GetCommonID();//なつかしＭＩＤ
      ChangePage(pg_SelectNaBoxNum);
    }
  }
  //なつかしの通常／大人買い選択
  void TLyHmGachaInside::MvPage_SelectNaBoxNum()
  {
    if (ugbtn_back_->IsAct())
    {
      ChangePage(pg_SelectNaBox);
    }
	//else if (ugbtn_mid_->IsAct())//なつかし中身
    //{
    // stGacha_.backtopage_ = pg_SelectNaBoxNum;
    //  ChangePage(pg_NaBoxCnt);
    //}
    else if (uglistNaBoxNum_.IsAct())//レコード選択
    {
		s32 idx = uglistNaBoxNum_.GetActIndex();
		// BOXなかみ
		if (idx == 0)
		{
			stGacha_.backtopage_ = pg_SelectNaBoxNum;
			ChangePage(pg_NaBoxCnt);
			return;
		}
      stGacha_.otona_f_ = (idx != 1);
      ChangePage(pg_DecideNaBox);
      ugsp_gacha_.SetMsg(gamt_DecideNaBox);
    }
  }
  //なつかし決定
  void TLyHmGachaInside::MvPage_DecideNaBox()
  {
    if (ugbtn_back_->IsAct())
    {
      ChangePage(pg_SelectNaBoxNum);
    }
    else if (ugbtn_mid_->IsAct())//なつかし中身
    {
      stGacha_.backtopage_ = pg_DecideNaBox;
      ChangePage(pg_NaBoxCnt);
    }
    else  if (uglistNaBoxNum_.uglistend_.IsActRes(enUGListEndAct_ok))
    {
      //スター消費メッセージ
      SetHudMsgStarNaBox(stGacha_.mid_gacha_, stGacha_.otona_f_);
    }
    else if (IsHudOK())
    {
      //通信に入る前に筐体のリフレッシュを掛ける
      ugsp_gacha_.NaBoxRefresh(stGacha_.mid_gacha_);

      if (stGacha_.otona_f_)
      {
        Post_NaBoxOtona();//なつかしおとな開始通信
      }
      else
      {
        Post_NaBox();//なつかし開始通信
      }
    }
  }
  //なつかし中身リスト
  void TLyHmGachaInside::MvPage_NaBoxCnt()
  {
    if (ugbtn_back_->IsAct())
    {
      ChangePage(stGacha_.backtopage_);
    }
    else if (uglistNaBoxCnt_.IsAct())//レコード選択
    {
      selectListNo_ = uglistNaBoxCnt_.GetMIDNaBoxCnt(uglistNaBoxCnt_.GetActIndex());
      SetHudItemView_NaBoxCnt(selectListNo_);
      ChangePage(pg_NaBoxCntEnd);
    }
  }
  //なつかし中身リストアイテムビュー
  void TLyHmGachaInside::MvPage_NaBoxCntEnd()
  {
	// 選択してるアイテムを調べる
	enItemType itemtype = (enItemType)pmgEO_->mgCoM_.mdm_NaBoxCnt_.GetPKDt(selectListNo_, mdm_naboxcnt_itemType);


    //if (ugbtn_back_->IsAct())
    if (IsHudItemView())
    {
      if (itemtype == enItemType_BGM)
      {


        // 曲が変わってないということはイントロ再生すらしてない
        // なので何もしない
        if (!isChangeBGM_)
        {
        }
        // まだイントロ流れてる
        else if (introBGMID_ != -1)
        {
          // 止める
          selectListNo_ = -1;
          introBGMID_ = -1;
          pmgEO_->mgSound_.StopBGM();

          // 出るときにお店のBGMを流しておく
          //pmgEO_->mgNSF_.Play(nsf_shop_gatya);
          //pmgEO_->mgNSF_.PlayLast();//連続して鳴らすと直前サウンドが指し変わってしまう
          pmgEO_->mgNSF_.Play(nsf_shop_gatya);//BGM
        }
        // イントロ終わって無音状態
        else
        {
          // 出るときにお店のBGMを流しておく
          //pmgEO_->mgNSF_.PlayLast();
          pmgEO_->mgNSF_.Play(nsf_shop_gatya);//BGM
        }
      }
      ChangePage(pg_NaBoxCnt);
      return;
    }
    // BGMのイントロ再生
    else if (IsHudIntro())
    {
      pmgEO_->mgSound_.StopBGM();
      introBGMID_ = pmgEO_->mgCoM_.mdm_NaBoxCnt_.GetPKDt(selectListNo_, mdm_naboxcnt_itemID);
      pmgEO_->mgNSF_.PlayID_GetBGM(introBGMID_);
      introTimer_ = INTRO_TIMER_NUM;
      isChangeBGM_ = TRUE;
      return;
    }

    // イントロタイマー
    if (introBGMID_ != -1)
    {
      introTimer_--;
      if (introTimer_ < 0)
      {
        introBGMID_ = -1;
        pmgEO_->mgSound_.StopBGM();
      }
    }




	// 素材
	if (itemtype == enItemType_Sozai)
	{
		// シュートプレビュー
		if (ugbtn_mid_->IsAct())
		{
			prevvec_.clear();
			TSozaiData sozai;
			s64 sozaiID = pmgEO_->mgCoM_.mdm_NaBoxCnt_.GetPKDt(selectListNo_, mdm_naboxcnt_itemID);
			sozai.SetMidSozai(sozaiID);
			prevvec_.push_back(sozai);
			SetHudEnd();
			ChangePage(pg_NaBoxPrev);
			return;
		}
	}
  }
  // プレビュー
  void TLyHmGachaInside::MvPage_Preview()
  {
	  if (IsHudPrevBack())//おーけー
	  {
		  SetHudItemView_NaBoxCnt(selectListNo_);
		  ChangePage(pg_NaBoxCntEnd);
	  }
	  else if (IsHudPrevMix())
	  {
		  SetHudPrevMixSozai(prevvec_[0].GetSozaiR(), TRUE);
	  }
	  else if (IsHudPrevMixClear())
	  {
		  SetHudPrevMixSozai(prevvec_[0].GetSozaiR(), FALSE);
	  }
  }
  //ガチャ個数選択
  void TLyHmGachaInside::MvPage_SelectGacha()
  {
    if (ugbtn_back_->IsAct())
    {
      ChangePage(pg_SelectType);
    }
    else if (uglistGacha_.IsAct())//レコード選択
    {
      s32 recordidx = uglistGacha_.GetActIndex();
      uglistGacha_.SetListEnd(recordidx);
      stGacha_.mid_gacha_ = uglistGacha_.GetCommonID();//ガチャＭＩＤ
      ChangePage(pg_DecideGacha);
      if (stGacha_.todd_f_)
      {
        ugsp_gacha_.SetMsg(gamt_DecideStar);
      }
      else
      {
        ugsp_gacha_.SetMsg(gamt_DecideTodd);
      }
    }
  }
  //ガチャ買う画面
  void TLyHmGachaInside::MvPage_DecideGacha()
  {
    if (ugbtn_back_->IsAct())
    {
      ChangePage(pg_SelectGacha);
    }
    else if (ugbtn_ok_->IsAct())//とうせんしゃ
    {
      //Post_Tousen();//当選者ログを取りに行く
      ChangePage(pg_TousenGrade);
    }
    else if (ugbtn_mid_->IsAct())//ガチャ確率
    {
      ChangePage(pg_Prob);
    }
    else if (uglistGacha_.uglistend_.IsActRes(enUGListEndAct_ok))
    {
      if (stGacha_.todd_f_)
      {
        Post_Gacha();//ガチャ開始通信
      }
      else
      {
        //チケット
        if (pmgEO_->mgCoU_.IsGachaKenUse(stGacha_.mid_gacha_))
        {
          Post_Gacha();//ガチャ開始通信
        }
        else
        {
          //スター消費メッセージ
          SetHudMsgStarGacha(stGacha_.mid_gacha_);
        }
      }
    }
    else if (IsHudOK())
    {
      Post_Gacha();//ガチャ開始通信
    }
  }

  //当選ランク
  void TLyHmGachaInside::MvPage_TousenGrade()
  {
    if (ugbtn_back_->IsAct())
    {
      ChangePage(pg_DecideGacha);
    }
    else if (uglistTousenGrade_.IsAct())//レコード選択
    {
      stGacha_.grade_ = uglistTousenGrade_.GetCommonID();//等級
      stGacha_.tousenpage_ = 0;//ページ
      Post_Tousen();//ガチャ開始通信
    }
  }
  //当選者表示
  void TLyHmGachaInside::MvPage_Tousen()
  {
    if (ugbtn_back_->IsAct())
    {
      //監督画像が共用なので元に戻す
      ugprof_.SetProfMe();
      ChangePage(pg_TousenGrade);
    }
    else if (uglistTousen_.IsAct())//レコード選択
    {
      //表示書き換え//変数更新（テクスチャ読み込み）
      ugprof_.SetProfEnemy(uglistTousen_.GetActIndex());
      //ChangePage(pg_TousenEnd);
    }
  }
  //当選者ＥＮＤ
  void TLyHmGachaInside::MvPage_TousenEnd()
  {
    if (ugbtn_back_->IsAct())
    {
      ChangePage(pg_Tousen);
    }
  }
  //確率表示
  void TLyHmGachaInside::MvPage_Prob()
  {
    if (ugbtn_back_->IsAct())
    {
      ChangePage(pg_DecideGacha);
    }
    else if (uglistProb_.IsAct())//レコード選択
    {
      uglistProbCnt_.Refresh(uglistProb_.GetCntGrpNo(uglistProb_.GetActIndex()));
      ChangePage(pg_ProbCnt);
    }
  }
  //確率中身
  void TLyHmGachaInside::MvPage_ProbCnt()
  {
    if (ugbtn_back_->IsAct())
    {
      ChangePage(pg_Prob);
    }
    else if (uglistProbCnt_.IsAct())//レコード選択
    {
      SetHudItemView_GachaCnt(uglistProbCnt_.GetCommonID());
      ChangePage(pg_ProbCntEnd);
    }
  }
  //確率ＥＮＤ
  void TLyHmGachaInside::MvPage_ProbCntEnd()
  {
    //if (ugbtn_back_->IsAct())
    if (IsHudItemView())
    {
      ChangePage(pg_ProbCnt);
    }
  }
  //ガチャロール
  void TLyHmGachaInside::MvPage_GachaRoll()
  {
    //ガチャアニメーション終了
    if (ugsp_gacha_.IsActRes(enUGSpGachaAct_CupsuleOut))
    {
      SetHudItemView_GachaGet(ugsp_gacha_.GetActIndex());
    }
    else if (ugsp_gacha_.IsActRes(enUGSpGachaAct_GachaEnd))
    {
      ChangePage(pg_GachaResult);
    }
    //画面タップ
    else if (field_->sc_IsCamEnter2())
    {
      ugsp_gacha_.TapGachaField();
    }
  }
  //なつかしロール
  void TLyHmGachaInside::MvPage_NaBoxRoll()
  {
    //ガチャアニメーション終了
    if (ugsp_gacha_.IsAct())
    {
      ChangePage(pg_NaBoxResult);
    }
    //画面タップ
    else if (field_->sc_IsCamEnter())
    {
      ugsp_gacha_.TapBoxGachaField();
    }
  }
  //なつかし結果
  void TLyHmGachaInside::MvPage_NaBoxResult()
  {
    if (IsHudItemView())
    {
      s32 id_nabox = pmgEO_->mgCoU_.GetNaBox_id(stGacha_.mid_gacha_);
      s64 opennum = pmgEO_->mgCoU_.mdNaBox_.GetPKDt(id_nabox, mdnabox_openNum);
      s32 boxsize = pmgEO_->mgCoM_.mdm_NaBox_.GetPKDt(stGacha_.mid_gacha_, mdm_nabox_boxSize);
      s32 restnum = (boxsize - opennum);
      if (restnum > 0)
      {
        ChangePage(pg_SelectNaBoxNum);
      }
      else
      {
        ChangePage(pg_SelectNaBox);//のこり0のときはメニューに戻る
      }
      ugsp_gacha_.SetMsg(gamt_Buy);
      ugbtn_back_->SetDraw(TRUE);
    }
  }
  //ガチャ結果
  void TLyHmGachaInside::MvPage_GachaResult()
  {
    if (IsHudItemView())
    {
      ChangePage(pg_SelectGacha);
      ugsp_gacha_.SetMsg(gamt_Buy);
      ugbtn_back_->SetDraw(TRUE);
    }
  }
  //退出
  void TLyHmGachaInside::MvPage_Out()
  {
    lib_num::AprTo0(&stGacha_.wait_c_);
    if (stGacha_.wait_c_ == 0)
    {
      Down();
    }
  }

  //POST：ガチャ
  void TLyHmGachaInside::Post_Gacha()
  {
    StartConnect(cn_Gacha);
    pmgEO_->mgPo_.poHmShop_.Ready_gacha(stGacha_.mid_gacha_);

    if (pmgEO_->stSeq_.IsNoSv()) Connect_Gacha();
  }
  //POST：当選者取得
  void TLyHmGachaInside::Post_Tousen()
  {
    StartConnect(cn_Tousen);
    pmgEO_->mgPo_.poHmShop_.Ready_tousengacha(stGacha_.mid_gacha_, stGacha_.grade_, stGacha_.tousenpage_);

    if (pmgEO_->stSeq_.IsNoSv()) Connect_Tousen();
  }
  //POST：なつかしＢＯＸ
  void TLyHmGachaInside::Post_NaBox()
  {
    StartConnect(cn_NaBox);
    pmgEO_->mgPo_.poHmShop_.Ready_nabox(stGacha_.mid_gacha_);

    if (pmgEO_->stSeq_.IsNoSv()) Connect_NaBox();
  }
  //POST：なつかしＢＯＸ大人買い
  void TLyHmGachaInside::Post_NaBoxOtona()
  {
    StartConnect(cn_NaBoxOtona);
    pmgEO_->mgPo_.poHmShop_.Ready_nabox_otona(stGacha_.mid_gacha_);

    if (pmgEO_->stSeq_.IsNoSv()) Connect_NaBoxOtona();
  }
  //通信：ガチャ
  void TLyHmGachaInside::Connect_Gacha()
  {
    //ガチャアニメーションへ移行
    ChangePage(pg_GachaRoll);
    ugsp_gacha_.SetMsg(gamt_Gacha);
    SetHudEnd();
  }
  //通信：当選者
  void TLyHmGachaInside::Connect_Tousen()
  {
    //当選者表示ページへ移行
    ChangePage(pg_Tousen);
    SetHudEnd();
  }
  //通信：なつかしボックス
  void TLyHmGachaInside::Connect_NaBox()
  {
    //なつかしボックス
    ChangePage(pg_NaBoxRoll);
    ugsp_gacha_.SetMsg(gamt_NaBox);
    SetHudEnd();
  }
  //通信：なつかしボックス大人買い
  void TLyHmGachaInside::Connect_NaBoxOtona()
  {
    //なつかしボックス大人買い
    ChangePage(pg_NaBoxRoll);
    ugsp_gacha_.SetMsg(gamt_NaBoxOtona);
    SetHudEnd();
  }

  void TLyHmGachaInside::DoConnect(BOOL rqok_f)
  {
    if (rqok_f)//ＯＫのみ
    {
      //code0
      if (pmgEO_->mgPo_.IsCodeZero())
      {
        switch (stLy_.connect_)
        {
        case cn_Gacha: Connect_Gacha(); break;
        case cn_Tousen: Connect_Tousen(); break;
        case cn_NaBox: Connect_NaBox(); break;
        case cn_NaBoxOtona: Connect_NaBoxOtona(); break;
        }
      }
      else
      {
        //エラーメッセージ
        EndConnectNG(pmgEO_->mgPo_.GetCode(), pmgEO_->mgPo_.GetMsg().c_str());
        ChangePage(pg_SelectType);
      }
    }
    else
    {
      //通信エラー
      ChangePage(pg_SelectType);
    }
  }

  void TLyHmGachaInside::DoUpdate(BOOL draw_f)
  {
    switch (stLy_.pageNo_)
    {
    case pg_Out:
      ugsp_gacha_.Update(draw_f);
      break;
    default:
      uglistMenu_.Update(draw_f);
      uglistGacha_.Update(draw_f);
      uglistProb_.Update(draw_f);
      uglistProbCnt_.Update(draw_f);
      uglistNaBox_.Update(draw_f);
      uglistNaBoxNum_.Update(draw_f);
      uglistNaBoxCnt_.Update(draw_f);
      uglistTousenGrade_.Update(draw_f);
      uglistTousen_.Update(draw_f);
      ugsp_gacha_.Update(draw_f);
      ugprof_.Update(draw_f);
      break;
    }

    if (draw_f == FALSE)
    {
      switch (stLy_.pageNo_)
      {
      case pg_Visit: MvPage_Visit(); break;//品物選択
      case pg_SelectType: MvPage_SelectType(); break;//ガチャタイプ選択
      case pg_SelectNaBox: MvPage_SelectNaBox(); break;//なつかしＢＯＸ選択
      case pg_SelectNaBoxNum: MvPage_SelectNaBoxNum(); break;//なつかしの通常／大人買い選択
      case pg_DecideNaBox: MvPage_DecideNaBox(); break;//なつかしＢＯＸ決定
      case pg_NaBoxCnt: MvPage_NaBoxCnt(); break;//なつかし中身
      case pg_NaBoxCntEnd: MvPage_NaBoxCntEnd(); break;//懐かし中身ＥＮＤ
	  case pg_NaBoxPrev: MvPage_Preview(); break;//懐かし中身(素材だま)プレビュー
      case pg_SelectGacha: MvPage_SelectGacha(); break;//ガチャ回数選択
      case pg_DecideGacha: MvPage_DecideGacha(); break;//決定画面
      case pg_TousenGrade: MvPage_TousenGrade(); break;//当選者
      case pg_Tousen: MvPage_Tousen(); break;//当選者
      case pg_TousenEnd: MvPage_TousenEnd(); break;//当選者エンド
      case pg_Prob: MvPage_Prob(); break;//確率
      case pg_ProbCnt: MvPage_ProbCnt(); break;//確率中身
      case pg_ProbCntEnd: MvPage_ProbCntEnd(); break;//ボックス中身
      case pg_GachaRoll: MvPage_GachaRoll(); break;//ガチャ
      case pg_NaBoxRoll: MvPage_NaBoxRoll(); break;//BOXガチャ
      case pg_GachaResult: MvPage_GachaResult(); break;//ガチャ結果
      case pg_NaBoxResult: MvPage_NaBoxResult(); break;//BOXガチャ結果
      case pg_Out: MvPage_Out(); break;
      }
    }
  }

  void TLyHmGachaInside::OutShop()
  {
    stGacha_.msgset_f_ = FALSE;
    stGacha_.wait_c_ = 60;
    ChangePage(pg_Out);
    ugsp_gacha_.SetMsg(gamt_Out);
  }

}
