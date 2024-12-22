#pragma once
#include "dbInclude.h"

namespace db
{
	//param,//

	enum endtSet_Setting
	{
		setOpenTrainingMenu = 0,	//トレーニングメニューオープンフラグ
		setSettingEND,
	};

	enum endtSet_Param
	{
		setIndexInterval = setSettingEND,	//指数間隔
		setBottomValue,			// 底
		setScaleFactor,			//適当倍
		setCharaBaseLvRankD,	//レアリティＤの基礎レベル
		setCharaBaseLvRankC,	//レアリティＣの基礎レベル
		setCharaBaseLvRankB,	//レアリティＢの基礎レベル
		setCharaBaseLvRankA,	//レアリティＡの基礎レベル
		setCharaBaseLvRankS,	//レアリティＳの基礎レベル
		setCharaBaseLvRankSS,	//レアリティＳＳの基礎レベル
		setCharaBaseLvRankSSS,	//レアリティＳＳＳの基礎レベル
		setCharaMaxLvRankD,		//レアリティＤの最大レベル
		setCharaMaxLvRankC,		//レアリティＣの最大レベル
		setCharaMaxLvRankB,		//レアリティＢの最大レベル
		setCharaMaxLvRankA,		//レアリティＡの最大レベル
		setCharaMaxLvRankS,		//レアリティＳの最大レベル
		setCharaMaxLvRankSS,	//レアリティＳＳの最大レベル
		setCharaMaxLvRankSSS,	//レアリティＳＳＳの最大レベル
		setGrowthUpLv,			//育成上昇Ｌｖ
		setCharaLvValue,		//個人Ｌｖ増量
		setparamEND,
	};

	enum endtSet_BodyType
	{
		setCharaBtHp8 = setparamEND,	//ムキムキ８
		setCharaBtHp7,					//ムキムキ７
		setCharaBtHp6,					//ムキムキ６
		setCharaBtHp5,					//ムキムキ５
		setCharaBtHp4,					//ムキムキ４
		setCharaBtHp3,					//ムキムキ３
		setCharaBtHp2,					//ムキムキ２
		setCharaBtHp1,					//ムキムキ１
		setCharaBtNormal,				//平均
		setCharaBtSkill1,					//キレキレ１
		setCharaBtSkill2,					//キレキレ２
		setCharaBtSkill3,					//キレキレ３
		setCharaBtSkill4,					//キレキレ４
		setCharaBtSkill5,					//キレキレ５
		setCharaBtSkill6,					//キレキレ６
		setCharaBtSkill7,					//キレキレ７
		setCharaBtSkill8,					//キレキレ８
		setBodyTypeEND,
	};

	enum endtSet_CharacterType
	{
		setCharaCtAtk8 = setBodyTypeEND,	//いけいけ８
		setCharaCtAtk7,					//いけいけ７
		setCharaCtAtk6,					//いけいけ６
		setCharaCtAtk5,					//いけいけ５
		setCharaCtAtk4,					//いけいけ４
		setCharaCtAtk3,					//いけいけ３
		setCharaCtAtk2,					//いけいけ２
		setCharaCtAtk1,					//いけいけ１
		setCharaCtNormal,				//平均
		setCharaCtDef1,					//じっくり１
		setCharaCtDef2,					//じっくり２
		setCharaCtDef3,					//じっくり３
		setCharaCtDef4,					//じっくり４
		setCharaCtDef5,					//じっくり５
		setCharaCtDef6,					//じっくり６
		setCharaCtDef7,					//じっくり７
		setCharaCtDef8,					//じっくり８
		setCharacterTypeEND,
	};


	//各種設定データ
	class TDtCharaSetting
	{
	public:
		explicit TDtCharaSetting(void);
		virtual ~TDtCharaSetting(void);

		//読み込み
		void LoadDt(const char *CSVDir, const char *CSVFilename);
		//取得
		BOOL IsOpenTraingMenu(void);
		s32 GetDt(endtSet_Param dtNo);
		s32 GetBodytypeDt(endtSet_BodyType dtNo);
		s32 GetCharacterTypeDt(endtSet_CharacterType dtNo);

	protected:
		s32 GenGetDt(s32 dtNo);
		s32 dt_[setCharacterTypeEND];
	};


}
