#include "dbMgCoMaster.h"
#include "dbMgEngineObj.h"
#include <time.h>
namespace db
{

  TMgCoMaster::TMgCoMaster(TMgEngineObj* pmgEO) :TMgCoGen(pmgEO, "co_master")
  {
    mdvec_.clear();

    mdvec_.push_back(&mdm_Help_);//マスターヘルプ
    mdvec_.push_back(&mdm_Kiyaku_);//マスター規約
    mdvec_.push_back(&mdm_Client_);//クライアント状態
    mdvec_.push_back(&mdm_Timer_);//マスタータイム管理
    mdvec_.push_back(&mdm_Connect_);//通信設定
    mdvec_.push_back(&mdm_Team_);//マスターチーム
    mdvec_.push_back(&mdm_Slot_);//マスタースロットオープン
    mdvec_.push_back(&mdm_Supply_);//マスター消耗品
    mdvec_.push_back(&mdm_ItemPack_);//アイテムパック
    mdvec_.push_back(&mdm_ItemPackSet_);//アイテムパックセット
    mdvec_.push_back(&mdm_Rarity_);//レアリティ設定
    mdvec_.push_back(&mdm_SpSyouhin_);//消耗品ショップ＆なぞのみせ＆しゅーとばばあ
    mdvec_.push_back(&mdm_BBABuy_);//しゅーとばばあシュート購入メニュー
    mdvec_.push_back(&mdm_Maptip_);//マスターマップチップ
    mdvec_.push_back(&mdm_Drop_);//マスタードロップ
    mdvec_.push_back(&mdm_Tokkou_);//マスター特効チーム
    mdvec_.push_back(&mdm_TokkouItem_);//マスター特効アイテム
    mdvec_.push_back(&mdm_TokkouMag_);//マスター特効チーム倍率
    mdvec_.push_back(&mdm_TokkouItemMag_);//マスター特効アイテム倍率
    mdvec_.push_back(&mdm_TokkouSozai_);//マスター特効素材
    mdvec_.push_back(&mdm_Logbo_);//ログインボーナス
    mdvec_.push_back(&mdm_LogboSozai_);//ログインボーナス素材
    mdvec_.push_back(&mdm_VSBonus_);//対戦ボーナス
    mdvec_.push_back(&mdm_VSDrop_);//新通信対戦ボーナス
    mdvec_.push_back(&mdm_Campaign_);//マスターキャンペーン
    mdvec_.push_back(&mdm_Gacha_);//マスターガチャ
    mdvec_.push_back(&mdm_GachaGrp_);//マスターガチャ中身グループ
    mdvec_.push_back(&mdm_GachaCnt_);//マスターガチャ中身詳細
    mdvec_.push_back(&mdm_NaBox_);//マスターなつかしＢＯＸ
    mdvec_.push_back(&mdm_NaBoxCnt_);//マスターなつかしＢＯＸ中身詳細
    mdvec_.push_back(&mdm_Huku_);//マスターふくびき
    mdvec_.push_back(&mdm_HukuCnt_);//マスターふくびき中身詳細
    mdvec_.push_back(&mdm_Event_);//マスターイベント
    mdvec_.push_back(&mdm_Tournament_);//マスター勝ち抜き
    mdvec_.push_back(&mdm_TimeAttack_);//マスタータイムアタック
    mdvec_.push_back(&mdm_Ranking_);//マスターランキング
    mdvec_.push_back(&mdm_RkReward_);//マスターランキング報酬
    mdvec_.push_back(&mdm_Giftbox_);//マスター贈り物ボックス
    mdvec_.push_back(&mdm_Tips_);//Tips表示
	mdvec_.push_back(&mdm_LvUp_);//キャラLvUP経験値テーブル

    models_.clear();
    for (s32 i = 0; i < mdvec_.size(); ++i)
    {
      models_.push_back(&mdvec_[i]->model_);
    }

  }

  TMgCoMaster::~TMgCoMaster(void)
  {

  }

  void TMgCoMaster::LoadDt()
  {
    std::string csvdir = mid::midDatadir() + mid::midMasterdir();
    for (s32 i = 0; i < mdvec_.size(); ++i)
    {
      mdvec_[i]->LoadDt(csvdir.c_str());
    }
  }

  void TMgCoMaster::DoInitSendPack()
  {
    //チェック不要＆再生成
    sendmap_.longmap_["scheck"] = enSessionCheckType_Reset;
    if (mid::midIsIOS())
    {
        sendmap_.longmap_["os"] = 1;
    }
    else if (mid::midIsADR())
    {
        sendmap_.longmap_["os"] = 2;
    }
  }

  //ガチャ内のグループ数（および懐かしボックスの枠数）
  s32 TMgCoMaster::GetGachaGrpNum(s64 mid_gacha)
  {
    s32 num = 0;
    s32 groupno = mdm_Gacha_.GetPKDt(mid_gacha, mdm_gacha_lotGrpNoA);
    for (s32 i = 0; i < mdm_GachaGrp_.GetLineNum(); ++i)
    {
      if (mdm_GachaGrp_.GetLineDt(i, mdm_gachagrp_lotGrpNo) == groupno)
      {
        ++num;
      }
    }

    return num;
  }

  //ガチャ種類
  BOOL TMgCoMaster::IsStarGacha(s64 mid_gacha)
  {
    s64 mid_supply_ken = mdm_Gacha_.GetPKDt(mid_gacha, mdm_gacha_id_m_supply_ken);
    s32 pricetype = mdm_Supply_.GetPKDt(mid_supply_ken, mdm_supply_priceType);

    return  (pricetype == enPriceType_Star);
  }



  //商品のアイコン
  s32 TMgCoMaster::GetSpSyouhinIconNo(s64 mid_syouhin)
  {
    s32 res = 0;

    s32 itemtype = mdm_SpSyouhin_.GetPKDt(mid_syouhin, mdm_spsyouhin_itemType);
    s32 itemid = mdm_SpSyouhin_.GetPKDt(mid_syouhin, mdm_spsyouhin_itemID);
    s32 itemnum = mdm_SpSyouhin_.GetPKDt(mid_syouhin, mdm_spsyouhin_itemNum);
    s32 prise = 0;
    switch (itemtype)
    {
    case enItemType_Supply: prise = mdm_Supply_.GetPKDt(itemid, mdm_supply_iconNo); break;
    case enItemType_Pack: prise = mdm_ItemPack_.GetPKDt(itemid, mdm_itempack_iconNo); break;
    }

    return res;
  }

  //シュートの売却価格
  s32 TMgCoMaster::GetShotSellPrice(INTVECTOR sozairarityvec)
  {
    s32 res = 0;
    for (s32 i = 0; i < sozairarityvec.size(); ++i)
    {
      for (s32 i2 = 0; i2 < mdm_Rarity_.GetLineNum(); ++i2)
      {
        if (sozairarityvec[i] == i2)
        {
          res += mdm_Rarity_.GetLineDt(i2, mdm_rarity_priceSell);
          break;
        }
      }
    }

    return res;
  }

  //位置からＰＫを求める
  s64 TMgCoMaster::GetMapTipPK(s32 mapno, s32 posx, s32 posy)
  {
    s64 res = 0;
    for (s32 i = 0; i < mdm_Maptip_.GetLineNum(); ++i)
    {
      if ((mdm_Maptip_.GetLineDt(i, mdm_maptip_mapNo) == mapno)
        && (mdm_Maptip_.GetLineDt(i, mdm_maptip_posX) == posx)
        && (mdm_Maptip_.GetLineDt(i, mdm_maptip_posY) == posy))
      {
        res = mdm_Maptip_.GetLineDt(i, mdm_maptip_pk);
        break;
      }
    }
    return res;
  }

  //位置からＰＫを求める
  s64 TMgCoMaster::GetMapTipPosNo(s32 mapno, s32 posx, s32 posy)
  {
    s64 res = 0;
    for (s32 i = 0; i < mdm_Maptip_.GetLineNum(); ++i)
    {
      if ((mdm_Maptip_.GetLineDt(i, mdm_maptip_mapNo) == mapno)
        && (mdm_Maptip_.GetLineDt(i, mdm_maptip_posX) == posx)
        && (mdm_Maptip_.GetLineDt(i, mdm_maptip_posY) == posy))
      {
        res = mdm_Maptip_.GetLineDt(i, mdm_maptip_posNo);
        break;
      }
    }
    return res;
  }

  s64 TMgCoMaster::GetCharID(s64 mid_team, s32 memNo)
  {
    return mdm_Team_.GetLineDt(mid_team, (enmdm_Team)(mdm_team_id_m_char0 + memNo));
  }

  //現在開催中のランキング
  LONGVECTOR TMgCoMaster::GetNowRanking(LONGVECTOR nowevent)
  {
    LONGVECTOR res;

    //現在開催中のイベント
    for (s32 i = 0; i < nowevent.size(); ++i)
    {
      //ランキングをさらう
      for (s32 i2 = 0; i2 < mdm_Ranking_.GetLineNum(); ++i2)
      {
        if (mdm_Ranking_.GetLineDt(i2, mdm_ranking_id_m_event) == nowevent[i])
        {
          s64 mid_ranking = mdm_Ranking_.GetLineDt(i2, mdm_ranking_pk);
          res.push_back(mid_ranking);
        }
      }
    }
    return res;
  }

  //現在開催中のランキング
  s64 TMgCoMaster::GetEventRanking(LONGVECTOR nowevent, s32 mid_event)
  {
    s64 res = NGNUM;
    //ランキングをさらう
    for (s32 i2 = 0; i2 < mdm_Ranking_.GetLineNum(); ++i2)
    {
      if (mdm_Ranking_.GetLineDt(i2, mdm_ranking_id_m_event) == mid_event)
      {
        res = mdm_Ranking_.GetLineDt(i2, mdm_ranking_pk);
      }
    }
    return res;
  }

  //--------------------------------------------------------------------------------------------
  // 任意のイベントの対象ＢＧＭ所持数取得
  //--------------------------------------------------------------------------------------------
  LONGVECTOR2 TMgCoMaster::GetTokkouBgm(s64 mid_event)
  {
	  LONGVECTOR2 bgmVec;
	  bgmVec.clear();

	  // 無効なイベントＩＤ
	  if (mid_event == NGNUM) { return bgmVec; }

	  // 最初に現在のイベントに設定されている特効ＢＧＭのものだけを抽出
	  INTVECTOR targetVec;
	  targetVec.clear();
	  s64 tokkouNo = pmgEO_->mgCoM_.mdm_Event_.GetPKDt(mid_event, enmdm_Event::mdm_event_tokkouNo);

	  // 特効の設定なし
	  if (tokkouNo == 0) { return bgmVec; }

	  LONGVECTOR v;
	  v.clear();

	  BOOL isSet = FALSE;

	  // 特効の設定テーブルから検索
	  for (s32 i = 0; i < pmgEO_->mgCoM_.mdm_TokkouItem_.GetLineNum(); i++)
	  {
		  // 対象のイベント用でなければ無視
		  if (pmgEO_->mgCoM_.mdm_TokkouItem_.GetLineDt(i, enmdm_TokkouItem::mdm_tokkouitem_tokkouNo) != tokkouNo) continue;

		  // ＢＧＭでなければ無視
		  if (pmgEO_->mgCoM_.mdm_TokkouItem_.GetLineDt(i, enmdm_TokkouItem::mdm_tokkouitem_itemType) != enItemType::enItemType_BGM) continue;

		  // 対象のＢＧＭをとりあえず保存
		  v.push_back(pmgEO_->mgCoM_.mdm_TokkouItem_.GetLineDt(i, enmdm_TokkouItem::mdm_tokkouitem_itemID));

		  isSet = TRUE;
	  }

	  // 特効の設定テーブルにデータがなかった
	  if (!isSet)
	  {
		  return bgmVec;
	  }

	  // その次にタイトル別に分ける
	  for (s32 i = 0; i < pmgEO_->mgDt_.dtTitle_.GetLineNum(); i++)
	  {
		  LONGVECTOR tmp;
		  tmp.clear();

		  for (s32 j = 0; j < v.size(); j++)
		  {
			  s32 titleid = pmgEO_->mgDt_.dtBGM_.GetDtLine(v[j], enDtBGM_titleid);
			  if (pmgEO_->mgDt_.dtTitle_.GetDtLine(i, enDtTitle::enDtTitle_pk) == titleid)
			  {
				  tmp.push_back(v[j]);
			  }
		  }
		  bgmVec.push_back(tmp);
	  }
	  return bgmVec;
  }

  //--------------------------------------------------------------------------------------------
  // 任意のイベントの対象ＢＧＭ所持数取得(タイトル縛り)
  //--------------------------------------------------------------------------------------------
  LONGVECTOR TMgCoMaster::GetTokkouBgmByTitle(s64 mid_event, s64 titleid)
  {
	  LONGVECTOR bgmVec;
	  bgmVec.clear();

	  LONGVECTOR2 v = GetTokkouBgm(mid_event);
	  
	  // 任意のタイトルだけ取得
	  for (s32 i = 0; i < v.size(); i++)
	  {
		  s64 pk = pmgEO_->mgDt_.dtTitle_.GetDtLine(i, enDtTitle::enDtTitle_pk);
		  if (pk == titleid)
		  {
			  bgmVec = v[i];
			  break;
		  }
	  }
	  return bgmVec;
  }

  //--------------------------------------------------------------------------------------------
  // 任意のイベントの対象ＢＧＭ所持数取得
  //--------------------------------------------------------------------------------------------
  LONGVECTOR2 TMgCoMaster::GetHaveTokkouBgm(s64 mid_event)
  {
	  LONGVECTOR2 bgmVec;
	  bgmVec.clear();

	  LONGVECTOR2 v = GetTokkouBgm(mid_event);
	  // 持ってるものだけを抽出する
	  for (s32 i = 0; i < v.size(); i++)
	  {
		  LONGVECTOR tmp;
		  tmp.clear();
		  for (s32 j = 0; j < v[i].size(); j++)
		  {
			 s64 bgmID = v[i][j];
			 if (pmgEO_->mgCoU_.IsBGM(bgmID))
			 {
				 tmp.push_back(bgmID);
			 }
		  }
		  bgmVec.push_back(tmp);
	  }

	  return bgmVec;
  }

  //--------------------------------------------------------------------------------------------
  // 任意のイベントの対象ＢＧＭ所持数取得(タイトル縛り)
  //--------------------------------------------------------------------------------------------
  LONGVECTOR TMgCoMaster::GetHaveTokkouBgmByTitle(s64 mid_event, s64 titleid)
  {
	  LONGVECTOR bgmVec;
	  bgmVec.clear();

	  LONGVECTOR2 v = GetHaveTokkouBgm(mid_event);

	  // 任意のタイトルだけ取得
	  for (s32 i = 0; i < v.size(); i++)
	  {
		  s64 pk = pmgEO_->mgDt_.dtTitle_.GetDtLine(i, enDtTitle::enDtTitle_pk);
		  if (pk == titleid)
		  {
			  bgmVec = v[i];
			  break;
		  }
	  }
	  return bgmVec;
  }

  //--------------------------------------------------------------------------------------------
  // 任意のイベントの対象ＢＧＭ数取得
  //--------------------------------------------------------------------------------------------
  s64 TMgCoMaster::GetTokkouBgmNum(s64 mid_event)
  {
	 LONGVECTOR2 v = GetTokkouBgm(mid_event);
	 s64 num = 0;
	 for (s32 i = 0; i < v.size(); i++)
	 {
		 num += v[i].size();
	 }
	 return num;
  }
  //--------------------------------------------------------------------------------------------
  // 任意のイベントの対象ＢＧＭ数取得(タイトル縛り)
  //--------------------------------------------------------------------------------------------
  s64 TMgCoMaster::GetTokkouBgmNumByTitle(s64 mid_event, s64 titleid)
  {
	  LONGVECTOR v = GetTokkouBgmByTitle(mid_event, titleid);
	  return v.size();
  }
  //--------------------------------------------------------------------------------------------
  // 任意のイベントの対象ＢＧＭ所持数取得
  //--------------------------------------------------------------------------------------------
  s64 TMgCoMaster::GetHaveTokkouBgmNum(s64 mid_event)
  {
	  LONGVECTOR2 v = GetHaveTokkouBgm(mid_event);
	  s64 num = 0;
	  for (s32 i = 0; i < v.size(); i++)
	  {
		  num += v[i].size();
	  }
	  return num;
  }
  //--------------------------------------------------------------------------------------------
  // 任意のイベントの対象ＢＧＭ所持数取得(タイトル縛り) 
  //--------------------------------------------------------------------------------------------
  s64 TMgCoMaster::GetHaveTokkouBgmNumByTitle(s64 mid_event, s64 titleid)
  {
	  LONGVECTOR v = GetHaveTokkouBgmByTitle(mid_event, titleid);
	  return v.size();
  }

  //--------------------------------------------------------------------------------------------
  // 任意のイベントの特効背景取得
  //--------------------------------------------------------------------------------------------
  LONGVECTOR2 TMgCoMaster::GetTokkouHaikei(s64 mid_event)
  {
	  LONGVECTOR2 haikeiVec;
	  haikeiVec.clear();

	  // 無効なイベントＩＤ
	  if (mid_event == NGNUM) { return haikeiVec; }

	  // 最初に現在のイベントに設定されている特効ＢＧＭのものだけを抽出
	  INTVECTOR targetVec;
	  targetVec.clear();
	  s64 tokkouNo = pmgEO_->mgCoM_.mdm_Event_.GetPKDt(mid_event, enmdm_Event::mdm_event_tokkouNo);

	  // 特効の設定なし
	  if (tokkouNo == 0) { return haikeiVec; }

	  LONGVECTOR v;
	  v.clear();

	  BOOL isSet = FALSE;

	  // 特効の設定テーブルから検索
	  for (s32 i = 0; i < pmgEO_->mgCoM_.mdm_TokkouItem_.GetLineNum(); i++)
	  {
		  // 対象のイベント用でなければ無視
		  if (pmgEO_->mgCoM_.mdm_TokkouItem_.GetLineDt(i, enmdm_TokkouItem::mdm_tokkouitem_tokkouNo) != tokkouNo) continue;

		  // 背景でなければ無視
		  if (pmgEO_->mgCoM_.mdm_TokkouItem_.GetLineDt(i, enmdm_TokkouItem::mdm_tokkouitem_itemType) != enItemType::enItemType_Haikei) continue;

		  // 対象の背景をとりあえず保存
		  v.push_back(pmgEO_->mgCoM_.mdm_TokkouItem_.GetLineDt(i, enmdm_TokkouItem::mdm_tokkouitem_itemID));

		  isSet = TRUE;
	  }

	  // 特効の設定テーブルにデータがなかった
	  if (!isSet)
	  {
		  return haikeiVec;
	  }

	  // その次にタイトル別に分ける
	  for (s32 i = 0; i < pmgEO_->mgDt_.dtTitle_.GetLineNum(); i++)
	  {
		  LONGVECTOR tmp;
		  tmp.clear();

		  for (s32 j = 0; j < v.size(); j++)
		  {
			  s32 titleid = pmgEO_->mgDt_.dtCourt_.GetDtLine(v[j], enDtCourt::enDtCourt_titleid);
			  if (pmgEO_->mgDt_.dtTitle_.GetDtLine(i, enDtTitle::enDtTitle_pk) == titleid)
			  {
				  tmp.push_back(v[j]);
			  }
		  }
		  haikeiVec.push_back(tmp);
	  }
	  return haikeiVec;
  }

  //--------------------------------------------------------------------------------------------
  // 任意のイベントの特効背景取得(タイトル縛り)
  //--------------------------------------------------------------------------------------------
  LONGVECTOR TMgCoMaster::GetTokkouHaikeiByTitle(s64 mid_event, s64 titleid)
  {
	  LONGVECTOR haikeiVec;
	  haikeiVec.clear();

	  LONGVECTOR2 v = GetTokkouHaikei(mid_event);

	  // 任意のタイトルだけ取得
	  for (s32 i = 0; i < v.size(); i++)
	  {
		  s64 pk = pmgEO_->mgDt_.dtTitle_.GetDtLine(i, enDtTitle::enDtTitle_pk);
		  if (pk == titleid)
		  {
			  haikeiVec = v[i];
			  break;
		  }
	  }
	  return haikeiVec;
  }

  //--------------------------------------------------------------------------------------------
  // 任意のイベントの所持している特効背景取得
  //--------------------------------------------------------------------------------------------
  LONGVECTOR2 TMgCoMaster::GetHaveTokkouHaikei(s64 mid_event)
  {
	  LONGVECTOR2 haikeiVec;
	  haikeiVec.clear();

	  LONGVECTOR2 v = GetTokkouHaikei(mid_event);
	  // 持ってるものだけを抽出する
	  for (s32 i = 0; i < v.size(); i++)
	  {
		  LONGVECTOR tmp;
		  tmp.clear();
		  for (s32 j = 0; j < v[i].size(); j++)
		  {
			  s64 haikeiID = v[i][j];
			  if (pmgEO_->mgCoU_.IsHaikei(haikeiID))
			  {
				  tmp.push_back(haikeiID);
			  }
		  }
		  haikeiVec.push_back(tmp);
	  }

	  return haikeiVec;
  }

  //--------------------------------------------------------------------------------------------
  // 任意のイベントの所持している特効背景取得(タイトル縛り)
  //--------------------------------------------------------------------------------------------
  LONGVECTOR TMgCoMaster::GetHaveTokkouHaikeiByTitle(s64 mid_event, s64 titleid)
  {
	  LONGVECTOR haikeiVec;
	  haikeiVec.clear();

	  LONGVECTOR2 v = GetHaveTokkouHaikei(mid_event);

	  // 任意のタイトルだけ取得
	  for (s32 i = 0; i < v.size(); i++)
	  {
		  s64 pk = pmgEO_->mgDt_.dtTitle_.GetDtLine(i, enDtTitle::enDtTitle_pk);
		  if (pk == titleid)
		  {
			  haikeiVec = v[i];
			  break;
		  }
	  }
	  return haikeiVec;
  }

  //--------------------------------------------------------------------------------------------
  // 任意のイベントの対象背景数取得
  //--------------------------------------------------------------------------------------------
  s64 TMgCoMaster::GetTokkouHaikeiNum(s64 mid_event)
  {
	  LONGVECTOR2 v = GetTokkouHaikei(mid_event);
	  s64 num = 0;
	  for (s32 i = 0; i < v.size(); i++)
	  {
		  num += v[i].size();
	  }
	  return num;
  }
  //--------------------------------------------------------------------------------------------
  // 任意のイベントの対象背景数取得(タイトル縛り)
  //--------------------------------------------------------------------------------------------
  s64 TMgCoMaster::GetTokkouHaikeiNumByTitle(s64 mid_event, s64 titleid)
  {
	  LONGVECTOR v = GetTokkouHaikeiByTitle(mid_event, titleid);
	  return v.size();
  }
  //--------------------------------------------------------------------------------------------
  // 任意のイベントの対象背景所持数取得
  //--------------------------------------------------------------------------------------------
  s64 TMgCoMaster::GetHaveTokkouHaikeiNum(s64 mid_event)
  {
	  LONGVECTOR2 v = GetHaveTokkouHaikei(mid_event);
	  s64 num = 0;
	  for (s32 i = 0; i < v.size(); i++)
	  {
		  num += v[i].size();
	  }
	  return num;
  }
  //--------------------------------------------------------------------------------------------
  // 任意のイベントの対象背景所持数取得(タイトル縛り) 
  //--------------------------------------------------------------------------------------------
  s64 TMgCoMaster::GetHaveTokkouHaikeiNumByTitle(s64 mid_event, s64 titleid)
  {
	  LONGVECTOR v = GetHaveTokkouHaikeiByTitle(mid_event, titleid);
	  return v.size();
  }



  //素材の基礎値
  void TMgCoMaster::GetBasePowSpin(TSozaiData sozaidt, s32* basepow, s32* basespin, BOOL rank_f)
  {
	  if (sozaidt.mid_sozai_ < 0)
	  {
		  *basepow = -1;
		  *basespin = -1;
		  return;
	  }
    s32 powtype = pmgEO_->mgDt_.dtShElm_.GetDt(sozaidt.mid_sozai_, sz_para_type_pow);
    s32 spintype = pmgEO_->mgDt_.dtShElm_.GetDt(sozaidt.mid_sozai_, sz_para_type_spn);
    if (rank_f)
    {
      *basepow = pmgEO_->mgDt_.dtShType_.GetDtBaseRank(powtype);
      *basespin = pmgEO_->mgDt_.dtShType_.GetDtBaseRank(spintype);
    }
    else
    {
      *basepow = pmgEO_->mgDt_.dtShType_.GetDtBasePoint(powtype);
      *basespin = pmgEO_->mgDt_.dtShType_.GetDtBasePoint(spintype);
    }
  }
  //素材の個体値
  void TMgCoMaster::GetRevPowSpin(TSozaiData sozaidt, s32* revpow, s32* revspin)
  {
    const s32 INDVRANGE = 10;//10段階なのでそれで割ることで値が出る

    s32 powtype = pmgEO_->mgDt_.dtShElm_.GetDt(sozaidt.mid_sozai_, sz_para_type_pow);
    s32 powrev = pmgEO_->mgDt_.dtShType_.GetDtRev(powtype, sozaidt.rarity_);
    s32 powrange = pmgEO_->mgDt_.dtShType_.GetDtRange(powtype, sozaidt.rarity_);
    *revpow = (powrev + ((powrange * sozaidt.indvpow_) / INDVRANGE));

    s32 spintype = pmgEO_->mgDt_.dtShElm_.GetDt(sozaidt.mid_sozai_, sz_para_type_spn);
    s32 spinrev = pmgEO_->mgDt_.dtShType_.GetDtRev(spintype, sozaidt.rarity_);
    s32 spinrange = pmgEO_->mgDt_.dtShType_.GetDtRange(spintype, sozaidt.rarity_);
    *revspin = (spinrev + ((spinrange * sozaidt.indvspin_) / INDVRANGE));
  }
  s32 TMgCoMaster::GetLightFlag(s32 mid_sozai)
  {
    return pmgEO_->mgDt_.dtShElm_.GetLightFlag(mid_sozai);
  }

  //チーム特効倍率
  s32 TMgCoMaster::GetTokkouMag(s32 id_team, s32 mid_event)
  {
    s32 tokkouNo = mdm_Event_.GetPKDt(mid_event, mdm_event_tokkouNo);
    s32 mid_team = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_id_m_team);
    s32 overlap = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_overlap);

    return GetTokkouMag(mid_team, tokkouNo, overlap);
  }

  //チーム特効倍率
  s32 TMgCoMaster::GetTokkouMag(s32 mid_team, s32 tokkouNo, s32 overlap)
  {
    BOOL tokkou_f = FALSE;
    s32 res = 100;
    s32 magNo = 0;

    //サーチ
    for (s32 i = 0; i < mdm_Tokkou_.GetLineNum(); ++i)
    {
      if (tokkouNo == mdm_Tokkou_.GetLineDt(i, mdm_tokkou_tokkouNo))
      {
        if (mid_team == mdm_Tokkou_.GetLineDt(i, mdm_tokkou_id_m_team))
        {
          tokkou_f = TRUE;
          magNo = mdm_Tokkou_.GetLineDt(i, mdm_tokkou_magNo);
          break;
        }
      }
    }

    //特効チームではない
    if (tokkou_f == FALSE) return res;

    //枚数による倍率
    for (s32 i = 0; i < mdm_TokkouMag_.GetLineNum(); ++i)
    {
      if (magNo == mdm_TokkouMag_.GetLineDt(i, mdm_tokkoumag_magNo))
      {
        if (overlap >= mdm_TokkouMag_.GetLineDt(i, mdm_tokkoumag_overlap))
        {
          res = mdm_TokkouMag_.GetLineDt(i, mdm_tokkoumag_mag);
        }
        else
        {
          break;
        }
      }
    }

    return res;
  }


  //特効素材倍率
  s32 TMgCoMaster::GetTokkouSozaiMag(s32 mid_event, const SOZAIDTVECTOR& sozaivec)
  {
    s32 res = 0;//追加倍率なので何もないときはゼロ
    if (sozaivec.size() == 0) return res;

    s32 tokkouNo = mdm_Event_.GetPKDt(mid_event, mdm_event_tokkouNo);

    //サーチ
    for (s32 i = 0; i < mdm_TokkouSozai_.GetLineNum(); ++i)
    {
      if (tokkouNo != mdm_TokkouSozai_.GetLineDt(i, mdm_tokkousozai_tokkouNo)) continue;

      for (s32 i2 = 0; i2 < sozaivec.size(); ++i2)
      {
        if (sozaivec[i2].mid_sozai_ == mdm_TokkouSozai_.GetLineDt(i, mdm_tokkousozai_id_m_sozai))
        {
          enmdm_TokkouSozai rarity = (enmdm_TokkouSozai)(mdm_tokkousozai_magD + sozaivec[i2].rarity_);
          res += mdm_TokkouSozai_.GetLineDt(i, rarity);
          break;
        }
      }
    }

    return res;
  }

  BOOL TMgCoMaster::IsTimer(s32 mid_timer)
  {

    BOOL res = FALSE;

    switch (mdm_Timer_.GetPKDt(mid_timer, mdm_timer_weekday))
    {
    case 0://時間指定
    {
      s64 nowtime = mid::midNowTime();
      s64 sttime = mdm_Timer_.GetPKDt(mid_timer, mdm_timer_eventSt_date);
      s64 edtime = mdm_Timer_.GetPKDt(mid_timer, mdm_timer_eventEd_date);

      res = ((sttime < nowtime) && (nowtime < edtime));
    }
    break;
    case 8://毎日
      res = TRUE;
      break;
    default://各曜日
    {
      res = TRUE;
      //s64 nowtime = mid::midNowTime();
      //struct tm when;
      //time_t when_t;
      //int input_year, input_month, input_day;
      //printf("time_tの長さ%02d\n", sizeof(time_t));//time_tのサイズを確認

      //memset(&when, 0, sizeof(struct tm));
      //printf("年月日を８桁で入力してください:-->");
      //scanf("%04d%02d%02d", &input_year, &input_month, &input_day);
      //when.tm_mday = input_day;
      //when.tm_mon = input_month - 1;
      //when.tm_year = input_year - 1900;

      //if ((when_t = mktime(&when)) != (time_t)-1)
      //{
      //  localtime_s(&when_t, &when);
      //  switch (when.tm_wday)
      //  {
      //  case 0:
      //    printf("日曜日です。\n");
      //  }
      //}
    }
    break;
    }
    //saday = ((self.weekday + 7 - today.isoweekday()) % 7)#今日からの差分
    //  resday = today + datetime.timedelta(days = saday)
    //  res = datetime.date(resday.year, resday.month, resday.day)

    return res;

    //曜日判定できてないのでなおす
  }
}
