#include "dbMgEngineObj.h"

namespace db
{
  TMgEngineObj::TMgEngineObj(void)
    : mgGrp_(this)
    //, mgStSn_(&mgDt_)
    , mgPo_(this)
    , mgCoU_(this)
    , mgCoM_(this)
    , mgCoInfo_(this)
    , mgNSF_(this)
    , mgCoClmaster_("clmaster", "clmver")
    , mgCoSvmaster_("svmaster", "svmver")
    , mgCoBannerBmp_("bannerbmp", "bnver")
    , mgCoEventBmp_("eventbmp", "evver")
  {
    mid::midLog("TMgEngineObj::TMgEngineObj\n");
      id_team_top_ = NGNUM;
      mid_team_top_ = NGNUM;
  }
  
  TMgEngineObj::~TMgEngineObj(void)
  {
#ifdef __K_DEBUG_USER_DATA__
    kdebug::DebugUDManager::GetInstance()->Destroy();
#endif	// #ifdef __K_DEBUG_USER_DATA__

	ClearStData();
  }
  
  //読み込み
  void TMgEngineObj::DoLoadData()
  {
    //ＣＳＶデータ読み込み
    mid::midLog("mgDt_.LoadDt()\n");
    mgDt_.LoadDt();
  }

  //読み込み
  void TMgEngineObj::DoLoadGrp()
  {
    //画像読み込み
    mid::midLog("mgGrp_.LoadBmpCntn()\n");
    mgGrp_.LoadBmpCntn();

#ifdef __K_DEBUG_USER_DATA__
    kdebug::DebugUDManager::Create(this);
#endif	// #ifdef __K_DEBUG_USER_DATA__
  }

  
  //準備
  void TMgEngineObj::DoReady()
  {
    //キャラアイコン作成
    mgGrp_.Ready();
  }

  //更新
  void TMgEngineObj::DoUpdate()
  {
  }  

  //更新
  void TMgEngineObj::RefreshStateTbl(void)
  {
	  s64 totalStTop = 0;
	  id_team_top_ = NGNUM;
	  mid_team_top_ = NGNUM;

	  for (s32 i = 0; i < mgCoU_.mdTeam_.GetLineNum(); i++)
	  {
		  // 売ったチームは対象外
		  if (mgCoU_.mdTeam_.GetLineDt(i, enmdTeam::mdteam_overlap) == 0) { continue; }

		 s64 id_team  = mgCoU_.mdTeam_.GetLineDt(i, enmdTeam::mdteam_pk);
		 s64 mid_team = mgCoU_.mdTeam_.GetLineDt(i, enmdTeam::mdteam_id_m_team);

		 LONGVECTOR2 v;
		 v.clear();

		 s64 tmpTotalSt = 0;

		 for (s32 j = 0; j < DBMEMBER_ALL; j++)
		 {
			 LONGVECTOR charaVec;
			 charaVec.clear();

			 s64 charaid = mgCoM_.GetCharID(mid_team, j);
			 s64 Lv = mgCoU_.GetMyTmCharaLv(mid_team, j);
			 
			 s64 stPow = CalcSt(mid_team, id_team, charaid, clv_Pow, Lv);
			 charaVec.push_back(stPow);

			 s64 stGuts = CalcSt(mid_team, id_team, charaid, clv_Guts, Lv);
			 charaVec.push_back(stGuts);

			 s64 stKire = CalcSt(mid_team, id_team, charaid, clv_Kire, Lv);
			 charaVec.push_back(stKire);

			 s64 stTech = CalcSt(mid_team, id_team, charaid, clv_Tech, Lv);
			 charaVec.push_back(stTech);


			 tmpTotalSt += stPow + stGuts + stKire + stTech;
			 
			 v.push_back(charaVec);
		 }
		 stData_[id_team] = v;

		 // トップチーム更新
		 if(totalStTop <= tmpTotalSt)
		 {
			 id_team_top_ = id_team;
			 mid_team_top_ = mid_team;
			 totalStTop = tmpTotalSt;
		 }
	  }
  }


  //更新(チーム単体)
  void TMgEngineObj::RefreshStateTbl(s64 mid_team)
  {
	  s64 id_team = NGNUM;
	  s64 tmLv = NGNUM;
	  for (s32 i = 0; i < mgCoU_.mdTeam_.GetLineNum(); ++i)
	  {
		  if (mgCoU_.mdTeam_.GetLineDt(i, mdteam_id_m_team) == mid_team)
		  {
			  id_team = mgCoU_.mdTeam_.GetLineDt(i, mdteam_pk);
			  tmLv = mgCoU_.mdTeam_.GetLineDt(i, mdteam_overlap);
		  }
	  }

	  // キーとなるチームのユーザーデータIDが分からないのでデータテーブル更新失敗
	  if(id_team == NGNUM){ return; }

	s64 totalStTop = 0;

	// 最強チーム
	// なぜかテーブルが作られていない可能性があるのでその時は再生成
	if (id_team_top_ == NGNUM || mid_team_top_ == NGNUM)
	{
		RefreshStateTbl();
	}

	for (s32 i = 0; i < DBMEMBER_ALL; i++)
	{
		// 実際に計算する
		s64 stPow  = GetSt(id_team_top_, i, clv_Pow);
		s64 stGuts = GetSt(id_team_top_, i, clv_Guts);
		s64 stKire = GetSt(id_team_top_, i, clv_Kire);
		s64 stTech = GetSt(id_team_top_, i, clv_Tech);
		totalStTop += (stPow + stGuts + stKire + stTech);
	}

	LONGVECTOR2 v;
	v.clear();

	s64 tmpTotalSt = 0;

	for (s32 j = 0; j < DBMEMBER_ALL; j++)
	{
		LONGVECTOR charaVec;
		charaVec.clear();

		s64 charaid = mgCoM_.GetCharID(mid_team, j);
		s64 Lv = mgCoU_.GetMyTmCharaLv(mid_team, j);

		s64 stPow = CalcSt(mid_team, id_team, charaid, clv_Pow, Lv, tmLv);
		charaVec.push_back(stPow);

		s64 stGuts = CalcSt(mid_team, id_team, charaid, clv_Guts, Lv, tmLv);
		charaVec.push_back(stGuts);

		s64 stKire = CalcSt(mid_team, id_team, charaid, clv_Kire, Lv, tmLv);
		charaVec.push_back(stKire);

		s64 stTech = CalcSt(mid_team, id_team, charaid, clv_Tech, Lv, tmLv);
		charaVec.push_back(stTech);


		tmpTotalSt += stPow + stGuts + stKire + stTech;

		v.push_back(charaVec);
	}
	stData_[id_team] = v;

	// トップチーム更新
	if (totalStTop <= tmpTotalSt)
	{
		id_team_top_ = id_team;
		mid_team_top_ = mid_team;
	}
  }

  // 最強チーム取得
  s64 TMgEngineObj::GetTopTeamByMasterID(void)
  {
	  // 設定されていない時は再計算
	  if (mid_team_top_ == NGNUM)
	  {
		  RefreshStateTbl();
	  }
	  return mid_team_top_;
  }
  // 最強チーム取得
  s64 TMgEngineObj::GetTopTeamByUserID(void)
  {
	  // 設定されていない時は再計算
	  if (id_team_top_ == NGNUM)
	  {
		  RefreshStateTbl();
	  }
	  return id_team_top_;
  }	




  // 事前に計算したところから取得
  // 2015/07/02現在ホームの最強チーム表示(dbUGTmBoard.cpp)でしか使っていない
  // 最終的には全部ここからステータス関係は取得する(予定)
  s64 TMgEngineObj::GetSt(s64 id_team, s64 charaPos, s32 type)
  {
	  s32 no = type - clv_Pow;
	  s64 ret = stData_[id_team][charaPos][no];
	  return ret;
  }

  s64 TMgEngineObj::CalcSt(s64 mid_team, s64 id_team, s64 charaid, s32 type, s32 lv, s32 tmLv)
  {
	  // チームレベルの指定がある場合(外で決めたい時)は今のチームレベルを取ってこない
	  s32 overlap = tmLv;
	  if (overlap == NGNUM)
	  {
		  overlap = mgCoU_.mdTeam_.GetPKDt(id_team, enmdTeam::mdteam_overlap);
	  }
	  // チームカード初取得時で1が入っているので-1する
	  overlap -= 1;

	  s32 rarity = mgCoM_.mdm_Team_.GetPKDt(mid_team, mdm_team_rarity);
	  s32 charaLv = lv; 
	  // キャラクター初期レベルは1が入っているので-1する
	  charaLv -= 1;

	  f64 base = GetNowBaseSt(charaid, charaLv, overlap, rarity);

	  //ぱわー　＝　round(総合値　×　体力)　×　攻撃　＋　乱数値								※小数点切り上げ
	  //きあい　＝　round(総合値　×　体力)　×　（１　－　攻撃）　－（乱数値）					※小数点切り下げ
	  //すぴん　＝　round（総合値　－　round（総合値×体力）　）　×　攻撃　＋　乱数値				※小数点切り上げ
	  //まもり　＝　round（総合値　－　round（総合値×体力）　）　×　（１　－　攻撃）　－（乱数値）※小数点切り下げ
		

	  //ぱわー・きあい用＿乱数値　＝　round(総合値　×　体力)　×　乱数　÷　200
	  //すぴん・まもり用＿乱数値　＝　round（総合値　－　round（総合値×体力）　）　×　乱数　÷　200

	  // 乱数値
	  f64 rnd = 0.0f;
	  // キャラクター固有のランダム値
	  f64 myRnd = (f64)(mgDt_.dtChar_.GetLv(clv_RandomVal, charaid));
	  f64 hp = 0.0f;
	  f64 skill = 0.0f;
	  f64 atk = 0.0f;
	  f64 def = 0.0f;

	  // キャラクター固有の体技数値
	  s64 bodyType = mgDt_.dtChar_.GetLv(clv_BodyType, charaid);
	  endtSet_BodyType bt = (endtSet_BodyType)(endtSet_BodyType::setCharaBtNormal - bodyType);
	  hp = (f32)(mgDt_.dtCharaSet_.GetBodytypeDt(bt)) / 10000.0f;
	  skill = 1 - hp;

	  // キャラクター固有の心数値
	  s64 characterType = mgDt_.dtChar_.GetLv(clv_CharacterType, charaid);
	  endtSet_CharacterType ct = (endtSet_CharacterType)(endtSet_CharacterType::setCharaCtNormal - characterType);
	  atk = (f32)(mgDt_.dtCharaSet_.GetCharacterTypeDt(ct)) / 10000.0f;
	  def = 1 - atk;

	  switch (type)
	  {
	  case clv_Pow:
	  case clv_Guts:
		  rnd = round(base * hp) * myRnd / 200.0f;
		  break;
	  case clv_Kire:
	  case clv_Tech:
		  rnd = round(base - round(base*hp)) * myRnd / 200.0f;
		  //rnd = base * skill * myRnd / 200.0f;
		  break;
	  }

	  f64 ret = 0;
	  switch (type)
	  {
	  case clv_Pow:
		  // 小数点切り上げ
		  ret = ceil(round(base * hp) * atk + rnd);
		  //ret = round(round(base * hp) * atk + rnd);
		  break;
	  case clv_Guts:
		  // 小数点切り下げ
		  ret = floor(round(base * hp) * (1 - atk) - rnd);
		  //ret = round(base * hp * def - rnd);
		  break;
	  case clv_Kire:
		  // 小数点切り上げ
		  ret = ceil(round(base - round(base*hp)) * atk + rnd);
		  //ret = round(base * skill * atk + rnd);
		  break;
	  case clv_Tech:
		  // 小数点切り下げ
		  ret = floor(round(base - round(base*hp)) * (1 - atk) - rnd);
		  //ret = round(base * skill * def - rnd);
		  break;
	  }
	  return (s64)(ret);
  }

  void TMgEngineObj::ClearStData(void)
  {
	  for (std::map<s64, LONGVECTOR2>::iterator itr = stData_.begin(); itr != stData_.end(); itr++)
	  {
		  LONGVECTOR2 v = itr->second;
		  for (s32 i = 0; i < v.size(); i++)
		  {
			  v[i].clear();
		  }
		  v.clear();
	  }
	  stData_.clear();
  }


  //-----------------------------------------------
  // 以下の３つを足した値
  //-----------------------------------------------
  f64 TMgEngineObj::GetNowBaseSt(s64 charaid, s64 charaLv, s64 teamLv, s64 teamRarity)
  {
	  f64 base = GetBaseSt(charaid);
	  f64 teamSt = GetTeamLvSt(charaid, teamRarity, teamLv);
	  f64 charaSt = GetCharaLvSt(charaLv);
	  f64 ret = 0;
	  ret = base + teamSt + charaSt;
	  return ret;
  }

  //-----------------------------------------------
  // 基礎総合値
  //-----------------------------------------------
  f64 TMgEngineObj::GetBaseSt(s64 charaid)
  {
	  const f32 SISU_INTERVAL	= (f32)(mgDt_.dtCharaSet_.GetDt(endtSet_Param::setIndexInterval) / 10000.0f);
	  const f32 BOTTOM_VAL		= (f32)(mgDt_.dtCharaSet_.GetDt(endtSet_Param::setBottomValue));
	  const f32 ANY_SCALE_VAL	= (f32)(mgDt_.dtCharaSet_.GetDt(endtSet_Param::setScaleFactor));

	  /*
	  const s32 RNK_TBL[7] =
	  {
		  mgDt_.dtCharaSet_.GetDt(endtSet_Param::setCharaBaseLvRankD),
		  mgDt_.dtCharaSet_.GetDt(endtSet_Param::setCharaBaseLvRankC),
		  mgDt_.dtCharaSet_.GetDt(endtSet_Param::setCharaBaseLvRankB),
		  mgDt_.dtCharaSet_.GetDt(endtSet_Param::setCharaBaseLvRankA),
		  mgDt_.dtCharaSet_.GetDt(endtSet_Param::setCharaBaseLvRankS),
		  mgDt_.dtCharaSet_.GetDt(endtSet_Param::setCharaBaseLvRankSS),
		  mgDt_.dtCharaSet_.GetDt(endtSet_Param::setCharaBaseLvRankSSS),
	  };
	  */

	  f32 rarityLv = (f32)(mgDt_.dtChar_.GetLv(clv_RarityLv, charaid));

	  //基礎総合値　　＝　　底　＾　（　（Lv　－　１　　）　×　指数間隔　）　＊　適当倍
	  f64 ret = 0;
	  //ret = (RNK_TBL[teamRarity] - 1) * SISU_INTERVAL;
	  ret = (rarityLv - 1.0f) * SISU_INTERVAL;
	  ret = pow(BOTTOM_VAL, ret);
	  ret *= ANY_SCALE_VAL;
	  return ret;
  }

  //-----------------------------------------------
  // チームレベル値
  //-----------------------------------------------
  f64 TMgEngineObj::GetTeamLvSt(s64 charaid, s64 teamRarity, s64 teamLv)
  {
	  const f32 SISU_INTERVAL	= (f32)(mgDt_.dtCharaSet_.GetDt(endtSet_Param::setIndexInterval) / 10000.0f);
	  const f32 BOTTOM_VAL		= (f32)(mgDt_.dtCharaSet_.GetDt(endtSet_Param::setBottomValue));
	  const f32 ANY_SCALE_VAL	= (f32)(mgDt_.dtCharaSet_.GetDt(endtSet_Param::setScaleFactor));

	  /*
	  const s32 RNK_TBL[7] =
	  {
		  mgDt_.dtCharaSet_.GetDt(endtSet_Param::setCharaBaseLvRankD),
		  mgDt_.dtCharaSet_.GetDt(endtSet_Param::setCharaBaseLvRankC),
		  mgDt_.dtCharaSet_.GetDt(endtSet_Param::setCharaBaseLvRankB),
		  mgDt_.dtCharaSet_.GetDt(endtSet_Param::setCharaBaseLvRankA),
		  mgDt_.dtCharaSet_.GetDt(endtSet_Param::setCharaBaseLvRankS),
		  mgDt_.dtCharaSet_.GetDt(endtSet_Param::setCharaBaseLvRankSS),
		  mgDt_.dtCharaSet_.GetDt(endtSet_Param::setCharaBaseLvRankSSS),
	  };
	  */

	  const f32 GROWTH_UP_LV = (f32)(mgDt_.dtCharaSet_.GetDt(endtSet_Param::setGrowthUpLv));

	  const f32 RNK_MAX_TBL[7] =
	  {
		  (f32)(mgDt_.dtCharaSet_.GetDt(endtSet_Param::setCharaMaxLvRankD)),
		  (f32)(mgDt_.dtCharaSet_.GetDt(endtSet_Param::setCharaMaxLvRankC)),
		  (f32)(mgDt_.dtCharaSet_.GetDt(endtSet_Param::setCharaMaxLvRankB)),
		  (f32)(mgDt_.dtCharaSet_.GetDt(endtSet_Param::setCharaMaxLvRankA)),
		  (f32)(mgDt_.dtCharaSet_.GetDt(endtSet_Param::setCharaMaxLvRankS)),
		  (f32)(mgDt_.dtCharaSet_.GetDt(endtSet_Param::setCharaMaxLvRankSS)),
		  (f32)(mgDt_.dtCharaSet_.GetDt(endtSet_Param::setCharaMaxLvRankSSS)),
	  };

	  f32 rarityLv = (f32)(mgDt_.dtChar_.GetLv(clv_RarityLv, charaid));

	  //チームLv値　＝（　底　＾　（　（Lv　＋　育成上昇Lv　）　×　指数間隔　）　＊　適当倍　－　基礎総合値　　）　×　log11 （　限界突破数　÷　レアリティ毎最大Lv　×　10　＋　1　）
	  f64 ret = 0;
	  //ret = (RNK_TBL[teamRarity] + GROWTH_UP_LV) * SISU_INTERVAL;
	  ret = (rarityLv + GROWTH_UP_LV) * SISU_INTERVAL;
	  ret = pow(BOTTOM_VAL, ret);
	  ret = ret * ANY_SCALE_VAL - GetBaseSt(charaid);

	  f64 tmp = (f64)(teamLv) / RNK_MAX_TBL[teamRarity] * 10.0f + 1.0f;
	  tmp = log(tmp) / log(11.0f); // ---> log11(x)は log(x) / log(11)と同じらしい
	  ret *= tmp;
	  return ret;
  }

  //-----------------------------------------------
  // キャラレベル値
  //-----------------------------------------------
  f64 TMgEngineObj::GetCharaLvSt(s64 lv)
  {
	  const f32 CHARA_LV_VALUE = (f32)(mgDt_.dtCharaSet_.GetDt(endtSet_Param::setCharaLvValue));

	  //個別Lv値　＝　個人Lv　×　個人Lv増量
	  f64 ret = 0;
	  ret = lv * CHARA_LV_VALUE;
	  return ret;
  }
}
