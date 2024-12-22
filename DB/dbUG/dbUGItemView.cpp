#include "dbUGItemView.h"

namespace db
{
  enum enViewType
  {
    vt_single,
    vt_list,
    vt_listw,
	vt_listSet,
	vt_listSetW,
  };
  //コンストラクタ
  TUGItemView::TUGItemView(TUIGenLayer* player) :TUGGen(player)
	  , ugbtn_back_(player)
	  , ugbtn_intro_(player)
	  , ugliget_(player)
	  , ugligetw_(player)
	  , ugliset_(player)
	  , uglisetw_(player)
	  , ugteam_(player)
	  , ugTmStPlate_(player)
	  , ugshot_(player)
	  , ughaikei_(player)
	  , ugitem_(player)
	  , ugtitle_(player)
	  , uglisozai_(player)
    , isDrawFilter_(TRUE)	// デフォはフィルター使う
  {
    stIV_.Init();
	viewItemParam_.Init();
  }

  TUGItemView::~TUGItemView()
  {
  }

  void TUGItemView::MakeGroup(TUIObj* parent)
  {
    const s32 BARW = 512;

    SetParent(parent);
    ////ヘッダフッタ間フィルタ
    //field_ = MakeObj();
    //field_->o_SetParent(parent);

    filter_ = MakeObj();
    filter_->o_SetParent(field_);
    filter_->g_SetCenter(TRUE);//フィルタはフィルタサイズが小さいためセンタリングが正常に効かない
    filter_->o_SetObjFilterGrp(BARW, BARW, 0xFF, 0, 0, 0);
    filter_->g_SetAlp(FALSE, 0x80);
    filter_->g_SetDepth(UIDP_08MSGFILTER);

    ugtitle_.MakeGroup(field_, TRUE);
    ugtitle_.SetMessage("アイテムかくとく");
    //title_ = MakeObj();
    //title_->o_SetParent(field_);
    //title_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_itemget_title));
    //ugtitle.SetCenter();
    //ugtitle.SetDepth(UIDP_09MSGBOX);
    //title_->f_MakeFont("アイテムかくとく", FC2_normal);
    //title_->f_SetCenter(TRUE);
    //title_->z_SetPos_Full(232 + 76, 44 + 12);

    ugliget_.MakeGroup(field_, FALSE);
    ugliget_.SetDepth(UIDP_09MSGBOX);
    ugligetw_.MakeGroup(field_);
    ugligetw_.SetDepth(UIDP_09MSGBOX);

	ugliset_.MakeGroup(field_, FALSE);
	ugliset_.SetDepth(UIDP_09MSGBOX);
	uglisetw_.MakeGroup(field_);
	uglisetw_.SetDepth(UIDP_09MSGBOX);

	uglisozai_.MakeGroup(field_, FALSE);
	uglisozai_.SetDepth(UIDP_09MSGBOX);

    ugbtn_back_.MakeGroup_FtPosBack(field_);
    ugbtn_back_.SetDepth(UIDP_09MSGBOX);//プライオリティあげて下地のもどるを隠す

    ugbtn_intro_.MakeGroup_FtPos(field_, 1, "イントロ");
    ugbtn_intro_.GetField()->z_SetPos_Full(352 - 64, 223);
    //ugbtn_intro_.SetFont("イントロ", FC2_btn_normal);
    ugbtn_intro_.SetDepth(UIDP_09MSGBOX);

    ugteam_.MakeGroup(field_, SIDE0);
    ugteam_.SetDepth(UIDP_09MSGBOX);
    ugteam_.SetDraw(FALSE);
	ugTmStPlate_.MakeGroup(field_);
	ugTmStPlate_.SetDepth(UIDP_09MSGBOX);
	ugTmStPlate_.SetDraw(FALSE);

    ugshot_.MakeGroup(field_);
    ugshot_.SetDepth(UIDP_09MSGBOX);
    ugshot_.SetDraw(FALSE);
    ughaikei_.MakeGroup(field_);
    ughaikei_.SetDepth(UIDP_09MSGBOX);
    ughaikei_.SetDraw(FALSE);

    ugitem_.MakeGroup(field_);
    ugitem_.SetDepth(UIDP_09MSGBOX);
    ugitem_.SetDraw(FALSE);


    //とりあえず右位置がデフォ
    SetLRPos(FALSE);

    SetDraw(FALSE);
  }
  //ガチャ開始の時クリア
  void TUGItemView::ClearGachaLv()
  {
    gachalv_.clear();
  }
  //取得時に追加
  s32 TUGItemView::IncGachaLv(s32 id_team)
  {
    if (gachalv_.count(id_team) > 0)
    {
      ++gachalv_[id_team];
    }
    else
    {
      gachalv_[id_team] = 1;
    }

    return gachalv_.count(id_team);//現在のレベルアップ量
  }
  void TUGItemView::SetDrawFilter(BOOL draw_f)
  {
    if (filter_)
    {
      isDrawFilter_ = draw_f;
    }
  }
  void TUGItemView::UndrawSingleObj()
  {
    ugteam_.SetDraw(FALSE);
	ugTmStPlate_.SetDraw(FALSE);
    ugshot_.SetDraw(FALSE);
    ughaikei_.SetDraw(FALSE);
    ugitem_.SetDraw(FALSE);
    ugbtn_intro_.SetDraw(FALSE);
  }
  void TUGItemView::UndrawListFilter()
  {
    filter_->g_SetDraw(FALSE);
    ugtitle_.SetDraw(FALSE);
    ugliget_.SetDraw(FALSE);
    ugligetw_.SetDraw(FALSE);
	ugliset_.SetDraw(FALSE);
	uglisetw_.SetDraw(FALSE);
	uglisozai_.SetDraw(FALSE);
  }
  void TUGItemView::SetLRPos(BOOL leftpos_f)
  {
    stIV_.leftpos_f_ = leftpos_f;
    ugtitle_.SetLRPos(leftpos_f);
    ugliget_.SetLRPos(leftpos_f);
	ugliset_.SetLRPos(leftpos_f);
	ugteam_.SetLRPosByGet(leftpos_f);
	ugTmStPlate_.SetLRPos(leftpos_f);
    ugshot_.SetLRPos(leftpos_f);
    ughaikei_.SetLRPos(leftpos_f);
    ugitem_.SetLRPos(leftpos_f);
	uglisozai_.SetLRPos(leftpos_f);
  }

  void TUGItemView::SingleRefreshByListSet(enItemType itemtype, s64 itemid, s64 itemnum, s32 tokkouNum, s32 tokkouMag, BOOL get_f, s32 etcnum, BOOL leftpos_f)
  {
	  BOOL leftPosByList_f = !leftpos_f;
	  SingleRefresh(itemtype, itemid, itemnum, tokkouNum, tokkouMag, get_f, etcnum, leftpos_f);
	  
	  // 元に戻す
	  ugliset_.SetLRPos(leftPosByList_f);
	  stIV_.leftpos_f_ = leftPosByList_f;
  }
  void TUGItemView::SingleRefresh(enItemType itemtype, s64 itemid, s64 itemnum, s32 tokkouNum, s32 tokkouMag, BOOL get_f, s32 etcnum, BOOL leftpos_f)
  {

    SetLRPos(leftpos_f);

    switch (itemtype)
    {
    case enItemType_Team:
      if (get_f)
      {
        ugteam_.SetItemGet(itemid, etcnum, leftpos_f);
		ugTmStPlate_.SetSt(itemid, etcnum, leftpos_f);
		ugTmStPlate_.SetDraw(TRUE);
      }
      else
      {
        ugteam_.SetItemCnt(itemid, leftpos_f);
      }
      break;
	case enItemType_Sozai:
	{
		s64 pow = (etcnum == NGNUM) ? NGNUM : (etcnum / 10) % 10;
		s64 spin = (etcnum == NGNUM) ? NGNUM : etcnum % 10;
		ugshot_.SetItemGet(itemid, pow, spin);
	}
      break;
    case enItemType_Shot:
      break;
    case enItemType_BGM:
      ugitem_.SetDataBGM(itemid);
      if (get_f) pmgEO_->mgNSF_.PlayID_GetBGM(itemid);//取得時BGM再生
      break;
    case enItemType_Haikei:
      ughaikei_.SetHaikeiDt_ItemView(itemid);
      break;
    case enItemType_Supply:
      ugitem_.SetDataSupplyEvent(itemid, itemnum, tokkouNum, tokkouMag);
      break;
    case enItemType_Todd:
      ugitem_.SetDataTodd(itemnum);
      break;
    case enItemType_Star:
      ugitem_.SetDataStar(itemnum);
      break;
    case enItemType_Pack:
      ugitem_.SetDataPack(itemid, itemnum);
      break;
    case enItemType_Huku:
      ugitem_.SetDataHuku(itemnum);
      break;
    }
  }

  //獲得ログからリスト
  void TUGItemView::Refresh_GetLog(BOOL msgok_f, BOOL leftpos_f, BOOL filter_f)
  {
    if (msgok_f)
    {
      ugbtn_back_.SetFont("おーけー");
    }
    else
    {
      ugbtn_back_.SetFont("もどる");
    }

    SetLRPos(leftpos_f);

    s32 num = pmgEO_->mgCoU_.mdlog_ItemGet_.GetLineNum();

    //まずぜんぶけす
    UndrawSingleObj();
    UndrawListFilter();

    //フィルタオン
    BOOL isF = filter_f;
    isF = isDrawFilter_ ? isF : FALSE;
    filter_->g_SetDraw(isF);
    ugbtn_back_.SetDraw(TRUE);
    //if (pmgEO_->stSeq_.IsNoSv()) num = 10;

    if (num == 1)
    {
      stIV_.viewtype_ = vt_single;
    }
    else if (num <= 5)
    {
      stIV_.viewtype_ = vt_list;
    }
    else
    {
      stIV_.viewtype_ = vt_listw;
    }


    switch (stIV_.viewtype_)
    {
    case vt_single:
    {
      ugtitle_.SetDraw(TRUE);
      enItemType itemtype = (enItemType)pmgEO_->mgCoU_.mdlog_ItemGet_.GetLineDt(0, mdlog_itemget_itemType);
      s64 itemid = pmgEO_->mgCoU_.mdlog_ItemGet_.GetLineDt(0, mdlog_itemget_itemID);
      s64 itemnum = pmgEO_->mgCoU_.mdlog_ItemGet_.GetLineDt(0, mdlog_itemget_itemNum);
	  s64 etcnum = NGNUM;
	  if(itemtype == enItemType_Sozai)
	  {
		  // 獲得ログのgetInfo4に個体値が入ってるかと思います
		  etcnum = pmgEO_->mgCoU_.mdlog_ItemGet_.GetLineDt(0, mdlog_itemget_getInfo4);
	  }
	  SingleRefresh(itemtype, itemid, itemnum, NGNUM, NGNUM, TRUE, etcnum, stIV_.leftpos_f_);
    }
    break;
    case vt_list:
      ugtitle_.SetDraw(FALSE);
      ugliget_.SetDraw(TRUE);
      ugliget_.SetLRPos(stIV_.leftpos_f_);
      ugliget_.Refresh();
      break;
    case vt_listw:
      ugtitle_.SetDraw(FALSE);
      ugligetw_.SetDraw(TRUE);
      ugligetw_.Refresh();
      break;
    }
  }

  //獲得ログ（番号指定１つ表示）
  void TUGItemView::Refresh_GetLogSingle(s32 logno, BOOL msgok_f, BOOL leftpos_f)
  {
    UndrawSingleObj();
    UndrawListFilter();

    filter_->g_SetDraw(isDrawFilter_);
    ugtitle_.SetDraw(TRUE);
    //title_->f_MakeFont("アイテムかくとく", FC2_normal);

    stIV_.viewtype_ = vt_single;
    enItemType itemtype = (enItemType)pmgEO_->mgCoU_.mdlog_ItemGet_.GetLineDt(logno, mdlog_itemget_itemType);
    s64 itemid = pmgEO_->mgCoU_.mdlog_ItemGet_.GetLineDt(logno, mdlog_itemget_itemID);
    s64 itemnum = pmgEO_->mgCoU_.mdlog_ItemGet_.GetLineDt(logno, mdlog_itemget_itemNum);
	s32 etcnum = NGNUM;
	if (itemtype == enItemType_Sozai)
	{
		// 獲得ログのgetInfo4に個体値が入ってるかと思います
		etcnum = pmgEO_->mgCoU_.mdlog_ItemGet_.GetLineDt(logno, mdlog_itemget_getInfo4);
	}
	SingleRefresh(itemtype, itemid, itemnum, NGNUM, NGNUM, TRUE, etcnum, leftpos_f);

    if (msgok_f)
    {
      //ugbtn_back_.SetFont("おーけー");
	  /* 2015/06/12 Koike Modified */
	  // 「うけとる」に変更
	  ugbtn_back_.SetFont("うけとる");
    }
    else
    {
      ugbtn_back_.SetFont("もどる");
    }
  }
  void TUGItemView::Refresh_GachaCnt(s64 mid_gachacnt)
  {
    //フィルタと獲得バー消す
    UndrawSingleObj();
    UndrawListFilter();

    enItemType itemtype = (enItemType)pmgEO_->mgCoM_.mdm_GachaCnt_.GetPKDt(mid_gachacnt, mdm_gachacnt_itemType);
    s64 itemid = pmgEO_->mgCoM_.mdm_GachaCnt_.GetPKDt(mid_gachacnt, mdm_gachacnt_itemID);
    s64 itemnum = pmgEO_->mgCoM_.mdm_GachaCnt_.GetPKDt(mid_gachacnt, mdm_gachacnt_itemNum);
    SingleRefresh(itemtype, itemid, itemnum, NGNUM, NGNUM, FALSE, 0, FALSE);
  }
  void TUGItemView::Refresh_GachaGet(s32 logno)
  {
    

    UndrawSingleObj();
    UndrawListFilter();

    //filter_->g_SetDraw(TRUE);
    ugtitle_.SetDraw(TRUE);
    //title_->f_MakeFont("アイテムかくとく", FC2_normal);

    stIV_.viewtype_ = vt_single;
    enItemType itemtype = (enItemType)pmgEO_->mgCoU_.mdlog_ItemGet_.GetLineDt(logno, mdlog_itemget_itemType);
    s64 itemid = pmgEO_->mgCoU_.mdlog_ItemGet_.GetLineDt(logno, mdlog_itemget_itemID);
    //s32 lv = IncGachaLv(itemid);
    s32 lv = NGNUM;
    s64 itemnum = pmgEO_->mgCoU_.mdlog_ItemGet_.GetLineDt(logno, mdlog_itemget_itemNum);
    SingleRefresh(itemtype, itemid, itemnum, NGNUM, NGNUM, TRUE, lv, FALSE);
    ugbtn_back_.SetDraw(FALSE);
  }

  void TUGItemView::Refresh_NaBoxCnt(s64 mid_naboxcnt)
  {
    //フィルタと獲得バー消す
    UndrawSingleObj();
    UndrawListFilter();

    enItemType itemtype = (enItemType)pmgEO_->mgCoM_.mdm_NaBoxCnt_.GetPKDt(mid_naboxcnt, mdm_naboxcnt_itemType);
    s64 itemid = pmgEO_->mgCoM_.mdm_NaBoxCnt_.GetPKDt(mid_naboxcnt, mdm_naboxcnt_itemID);
    s64 itemnum = pmgEO_->mgCoM_.mdm_NaBoxCnt_.GetPKDt(mid_naboxcnt, mdm_naboxcnt_itemNum);
    SingleRefresh(itemtype, itemid, itemnum, NGNUM, NGNUM, FALSE, 0, FALSE);


    // BGMだったらイントロボタン表示
    if ((enItemType_BGM == itemtype)
      /*&& (pmgEO_->mgDt_.dtBGM_.GetDtLine(itemid, enDtBGM_loop_f) != 0)*/)	//ループしないのもイントロ聞かせる
    {
      ugbtn_intro_.SetDraw(TRUE);
      //pmgEO_->mgNSF_.PlayID_GetBGM(itemid);//取得時BGM再生
    }
  }


  void TUGItemView::Refresh_HukuCnt(s64 mid_hukucnt)
  {
    //フィルタと獲得バー消す
    UndrawSingleObj();
    UndrawListFilter();

    enItemType itemtype = (enItemType)pmgEO_->mgCoM_.mdm_HukuCnt_.GetPKDt(mid_hukucnt, mdm_hukucnt_itemType);
    s64 itemid = pmgEO_->mgCoM_.mdm_HukuCnt_.GetPKDt(mid_hukucnt, mdm_hukucnt_itemID);
    s64 itemnum = pmgEO_->mgCoM_.mdm_HukuCnt_.GetPKDt(mid_hukucnt, mdm_hukucnt_itemNum);
    SingleRefresh(itemtype, itemid, itemnum, NGNUM, NGNUM, FALSE, 0, FALSE);
  }

  void TUGItemView::Refresh_Reward(s64 mid_ranking, s32 rank, BOOL msgok_f, BOOL leftpos_f, BOOL filter_f)
  {
	  if (msgok_f)
	  {
		  ugbtn_back_.SetFont("おーけー");
	  }
	  else
	  {
		  ugbtn_back_.SetFont("もどる");
	  }

	  SetLRPos(leftpos_f);

	  //まずぜんぶけす
	  UndrawSingleObj();
	  UndrawListFilter();

	  //フィルタオン
	  BOOL isF = filter_f;
	  isF = isDrawFilter_ ? isF : FALSE;
	  filter_->g_SetDraw(isF);
	  ugbtn_back_.SetDraw(TRUE);
	  //if (pmgEO_->stSeq_.IsNoSv()) num = 10;


	  enItemType itemType = (enItemType)NGNUM;
	  s64 itemID = NGNUM;
	  s32 itemNum = NGNUM;

	  for (s32 i = 0; i < pmgEO_->mgCoM_.mdm_RkReward_.GetLineNum(); i++)
	  {
		  // 該当のイベントではない
		  if (pmgEO_->mgCoM_.mdm_RkReward_.GetLineDt(i, enmdm_RkReward::mdm_rkreward_id_m_ranking) != mid_ranking) { continue; }

		  // 該当の順位
		  s32 top = pmgEO_->mgCoM_.mdm_RkReward_.GetLineDt(i, enmdm_RkReward::mdm_rkreward_top);
		  s32 bottom = pmgEO_->mgCoM_.mdm_RkReward_.GetLineDt(i, enmdm_RkReward::mdm_rkreward_bottom);
		  if (rank >= top && bottom >= rank)
		  {
			  itemType = (enItemType)pmgEO_->mgCoM_.mdm_RkReward_.GetLineDt(i, enmdm_RkReward::mdm_rkreward_itemType);
			  itemID = pmgEO_->mgCoM_.mdm_RkReward_.GetLineDt(i, enmdm_RkReward::mdm_rkreward_itemID);
			  itemNum = pmgEO_->mgCoM_.mdm_RkReward_.GetLineDt(i, enmdm_RkReward::mdm_rkreward_itemNum);
			  break;
		  }
	  }


	  if (itemType == NGNUM){ return; }

	  // アイテムパックのときはさらに中身チェック
	  LONGVECTOR2 data;
	  data.clear();
	  if (itemType == enItemType_Pack)
	  {
		  // 中身展開
		  for (s32 i = 0; i < pmgEO_->mgCoM_.mdm_ItemPackSet_.GetLineNum(); i++)
		  {
			  if (itemID == pmgEO_->mgCoM_.mdm_ItemPackSet_.GetLineDt(i, enmdm_ItemPackSet::mdm_itempackset_id_m_itempack))
			  {
				  LONGVECTOR v;
				  v.clear();
				  v.push_back(pmgEO_->mgCoM_.mdm_ItemPackSet_.GetLineDt(i, enmdm_ItemPackSet::mdm_itempackset_itemType));
				  v.push_back(pmgEO_->mgCoM_.mdm_ItemPackSet_.GetLineDt(i, enmdm_ItemPackSet::mdm_itempackset_itemID));
				  v.push_back(pmgEO_->mgCoM_.mdm_ItemPackSet_.GetLineDt(i, enmdm_ItemPackSet::mdm_itempackset_itemNum));
				  data.push_back(v);
			  }
		  }
	  }
	  else
	  {
		  LONGVECTOR v;
		  v.clear();
		  v.push_back(itemType);
		  v.push_back(itemID);
		  v.push_back(itemNum);
		  data.push_back(v);
	  }

	  // 中身の数に合わせて表示
	  if (data.size() == 1)
	  {
		  stIV_.viewtype_ = vt_single;

		  ugtitle_.SetDraw(FALSE);
		  SingleRefresh(itemType, itemID, itemNum, NGNUM, NGNUM, TRUE, NGNUM, stIV_.leftpos_f_);
	  }
	  else if (data.size() <= 5)
	  {
		  stIV_.viewtype_ = vt_listSet;

		  ugtitle_.SetDraw(FALSE);
		  
		  ugliset_.SetDraw(TRUE);
		  ugliset_.SetLRPos(stIV_.leftpos_f_);
		  ugliset_.Refresh(data);
	  }
	  else
	  {
		  stIV_.viewtype_ = vt_listSetW;
		  ugtitle_.SetDraw(FALSE);
		 
		  uglisetw_.SetDraw(TRUE);
		  uglisetw_.Refresh(data);
	  }
  }


  //非獲得アイテム表示（フィルタとタイトル不要～
  void TUGItemView::Refresh_View(s64 itemType, s64 itemID, s64 itemNum, BOOL msgok_f, BOOL leftpos_f)
  {
    UndrawListFilter();
    UndrawSingleObj();
    stIV_.viewtype_ = vt_single;
    SingleRefresh((enItemType)itemType, itemID, itemNum, NGNUM, NGNUM, FALSE, 0, leftpos_f);

	if (msgok_f)
	{
//		ugbtn_back_.SetFont("おーけー");
		/* 2015/06/12 Koike Modified */
		// 未取得アイテムの場合は文字を「うけとる」に変更
		ugbtn_back_.SetFont("うけとる");
	}
	else
	{
//		ugbtn_back_.SetFont("もどる");
		/* 2015/06/12 Koike Modified */
		// 予告の場合は「おーけー」にする
		ugbtn_back_.SetFont("おーけー");
	}
  }

  void TUGItemView::Refresh_Shot(SOZAIDTVECTOR sozai, BOOL leftpos_f)
  {
	  UndrawListFilter();
	  UndrawSingleObj();

	  SetLRPos(leftpos_f);

	  ugshot_.MakeUra();
	  ugshot_.SetShotDtGousei(sozai);
	  ugshot_.SetDraw(TRUE);
  }
  void TUGItemView::Refresh_SozaiList(SOZAIDTVECTOR sozai, BOOL leftpos_f)
  {
	  SetLRPos(leftpos_f);

	  //まずぜんぶけす
	  UndrawSingleObj();
	  UndrawListFilter();

	  ugtitle_.SetDraw(FALSE);
	  uglisozai_.SetDraw(TRUE);
	  uglisozai_.SetLRPos(leftpos_f);
	  uglisozai_.SetData(sozai);
  }

  //コンプアイテム表示
  void TUGItemView::Refresh_Drop(s64 itemType, s64 itemID, s64 itemNum, s32 tokkouNum, s32 tokkouMag, BOOL comp_f)
  {
    //まずぜんぶけす
    UndrawSingleObj();
    UndrawListFilter();
    //フィルタオン
    filter_->g_SetDraw(isDrawFilter_);
    stIV_.viewtype_ = vt_single;
    ugtitle_.SetDraw(TRUE);
    if (comp_f)
    {
      ugtitle_.SetMessage("コンプボーナス");
    }
    else
    {
      ugtitle_.SetMessage("アイテムかくとく");
      //title_->f_MakeFont("アイテムかくとく", FC2_normal);
    }
	s32 etcnum = 1;
	if (itemType == enItemType_Team)
	{
		// 試合終了後などにチームカードドロップ表示している時
		// 実際にはリザルト画面の次の暗転の通信でチームカードを取得しているので
		// ここでは現在のローカルのユーザーデータからチームのoverlapを取得してその値+1と表示する。
		// ユーザーデータに無いチームなら新規取得なので１固定
		// 追記：と思ったらここの部分は-1した値で渡すので初取得時は0、それ以外はoverlapそのままを使う
		for(s32 i = 0; i < pmgEO_->mgCoU_.mdTeam_.GetLineNum(); i++)
		{
			// 持っているものと一致するものを探す
			if(itemID != pmgEO_->mgCoU_.mdTeam_.GetLineDt(i, enmdTeam::mdteam_id_m_team)){ continue; }
			
			// 見つけた時は現在のoverlap + 1(今回取得分)をセット
			etcnum = pmgEO_->mgCoU_.mdTeam_.GetLineDt(i, enmdTeam::mdteam_overlap) + 1;
		}
	}
	SingleRefresh((enItemType)itemType, itemID, itemNum, tokkouNum, tokkouMag, TRUE, etcnum, FALSE);
  }

  TUGItemViewParam TUGItemView::GetSelectViewItem(void)
  {
	  return viewItemParam_;
  }


  void TUGItemView::DoUpdate(BOOL draw_f)
  {
    field_->Update(draw_f);
    filter_->Update(draw_f);
    ugtitle_.Update(draw_f);

    ugliget_.Update(draw_f);
    ugligetw_.Update(draw_f);
	ugliset_.Update(draw_f);
	uglisetw_.Update(draw_f);
	ugbtn_back_.Update(draw_f);
    ugbtn_intro_.Update(draw_f);

	uglisozai_.Update(draw_f);

    ugteam_.Update(draw_f);
	ugTmStPlate_.Update(draw_f);
    ugshot_.Update(draw_f);
    ughaikei_.Update(draw_f);

    ugitem_.Update(draw_f);
  }

  void TUGItemView::DoAction()
  {
    //リストの中を見てるとき
    if (stIV_.openlist_f_)
    {

      if (ugbtn_back_.IsAct())
      {
        UndrawSingleObj();

        if (stIV_.viewtype_ == vt_list)
        {
          ugliget_.SetDraw(TRUE);
        }
        else if (stIV_.viewtype_ == vt_listw)
        {
          ugligetw_.SetDraw(TRUE);
        }
		else if (stIV_.viewtype_ == vt_listSet)
		{
			ugliset_.SetDraw(TRUE);
			viewItemParam_.Clear();
		}
		else if (stIV_.viewtype_ == vt_listSetW)
		{
			uglisetw_.SetDraw(TRUE);
			ugbtn_back_.SetFont("もどる");
			viewItemParam_.Clear();
		}
        stIV_.openlist_f_ = FALSE;

        pmgEO_->mgNSF_.PlayLast();//BGMもどす
      }

    }
    else
    {

      if (ugbtn_back_.IsAct())
      {
        pmgEO_->mgNSF_.PlayLast();//BGM
        SetAct(0);
		viewItemParam_.Clear();
      }
      else//リストの中みる
      {
        s64 id_getlog = NGNUM;
		s64 id_set = NGNUM;
        s32 etcnum = 0;
		LONGVECTOR data;
		data.clear();

        if (ugliget_.IsAct())
        {
          id_getlog = ugliget_.GetCommonID();
          etcnum = ugliget_.GetEtcNum();
          ugliget_.SetDraw(FALSE);
        }
        else if (ugligetw_.IsAct())
        {
          id_getlog = ugligetw_.GetCommonID();
          etcnum = ugligetw_.GetEtcNum();
          ugligetw_.SetDraw(FALSE);
        }
		else if (ugliset_.IsAct())
		{
			id_set = ugliset_.GetCommonID();
			etcnum = ugliset_.GetEtcNum();
			data = ugliset_.GetData();
			//ugliset_.SetDraw(FALSE);
		}
		else if (uglisetw_.IsAct())
		{
			id_set = uglisetw_.GetCommonID();
			etcnum = uglisetw_.GetEtcNum();
			data = uglisetw_.GetData();
			uglisetw_.SetDraw(FALSE);
		}

        if (id_getlog != NGNUM)
        {
          enItemType itemtype = (enItemType)pmgEO_->mgCoU_.mdlog_ItemGet_.GetLineDt(id_getlog, mdlog_itemget_itemType);
          s64 itemid = pmgEO_->mgCoU_.mdlog_ItemGet_.GetLineDt(id_getlog, mdlog_itemget_itemID);
          s64 itemnum = pmgEO_->mgCoU_.mdlog_ItemGet_.GetLineDt(id_getlog, mdlog_itemget_itemNum);
          if (pmgEO_->stSeq_.IsNoSv())
          {
            itemtype = enItemType_Team;
            itemid = 1;
          }
		  
		  if (itemtype == enItemType_Sozai)
		  {
			  // 獲得ログのgetInfo4に個体値が入ってるかと思います
			  etcnum = pmgEO_->mgCoU_.mdlog_ItemGet_.GetLineDt(id_getlog, mdlog_itemget_getInfo4);
		  }
          SingleRefresh(itemtype, itemid, itemnum, NGNUM, NGNUM, TRUE, etcnum, stIV_.leftpos_f_);
          stIV_.openlist_f_ = TRUE;
        }
		else if (id_set != NGNUM)
		{
			enum paramType
			{
				ptType = 0,
				ptID,
				ptNum,
			};

			enItemType itemtype = (enItemType)data[ptType];
			s64 itemid = data[ptID];
			s64 itemnum = data[ptNum];

			if (ugliset_.IsAct())
			{
				UndrawSingleObj();
				SingleRefreshByListSet(itemtype, itemid, itemnum, NGNUM, NGNUM, TRUE, etcnum, !stIV_.leftpos_f_);

				viewItemParam_.type_ = itemtype;
				viewItemParam_.id_ = itemid;
				viewItemParam_.num_ = itemnum;
			}
			else
			{
				ugbtn_back_.SetFont("もどる");
				SingleRefresh(itemtype, itemid, itemnum, NGNUM, NGNUM, TRUE, etcnum, stIV_.leftpos_f_);
				stIV_.openlist_f_ = TRUE;

				viewItemParam_.type_ = itemtype;
				viewItemParam_.id_ = itemid;
				viewItemParam_.num_ = itemnum;
			}
		}
      }
    }
  }


}
