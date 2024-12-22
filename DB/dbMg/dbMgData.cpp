#include "dbMgData.h"

namespace db
{
	TMgData::TMgData(void)
	{
		mid::midLog("TMgData::TMgData");
	}

	TMgData::~TMgData(void)
	{
	}

	void TMgData::LoadDt()
	{

		std::string csvdir = mid::midDatadir() + mid::midCSVdir();
		//csvdir += CSVDTDIR;

		//シュート素材
		mid::midLog("dtElem_.LoadDt()\n");
		dtElem_.LoadDt(csvdir.c_str(), CSV_ELEM_CHANGE, CSV_ELEM_EFFECT, CSV_ELEM_SHAPE, CSV_ELEM_ORBIT, CSV_ELEM_TARGET);

		//ＢＭデータ
		mid::midLog("dtBM_.LoadDt()\n");
		dtBM_.LoadDt(csvdir.c_str(), CSV_BMDT);

		//セルデータ
		mid::midLog("dtCell_.LoadDt()\n");
		dtCell_.LoadDt(csvdir.c_str(), CSV_CCDT);

		//監督データ
		dtBM_kantoku_.LoadDt(csvdir.c_str(), CSV_BMDT_KANTOKU);
		dtCell_kantoku_.LoadDt(csvdir.c_str(), CSV_CCDT_KANTOKU);
		////ＵＩ選手データ
		//dtBM_captain_.LoadDt(csvdir.c_str(), CSV_BMDT_SENSYU);
		//dtCell_captain_.LoadDt(csvdir.c_str(), CSV_CCDT_SENSYU);
		//ＵＩ選手データ
		dtBM_sensyu_.LoadDt(csvdir.c_str(), CSV_BMDT_SENSYU);
		dtCell_sensyu_.LoadDt(csvdir.c_str(), CSV_CCDT_SENSYU);
		// Tipsキャラデータ
		dtBM_person_.LoadDt(csvdir.c_str(), CSV_BMDT_PERSON);
		dtCell_person_.LoadDt(csvdir.c_str(), CSV_CCDT_PERSON);
		//店員
		dtBM_clerk_.LoadDt(csvdir.c_str(), CSV_BMDT_CLERK);
		//女子
		dtBM_girl_.LoadDt(csvdir.c_str(), CSV_BMDT_GIRL);


		//ゲーム設定データ
		mid::midLog("dtSet_.LoadDt()\n");
		dtSet_.LoadDt(csvdir.c_str(), CSV_SETTINGDT);

		//キャラ育成関連データ
		mid::midLog("dtCharaSet_.LoadDt()\n");
		dtCharaSet_.LoadDt(csvdir.c_str(), CSV_CHARA_SETTINGDT);

		//クライアント設定データ
		mid::midLog("dtClient_.LoadDt()\n");
		dtClient_.LoadDt(csvdir.c_str(), CSV_CLIENT_SETTINGDT);

		//キャラデータ
		mid::midLog("dtChar_.LoadDt()\n");
		dtChar_.LoadDt(csvdir.c_str(), CSV_CHARDT_DL);



		//タイトルデータ
		mid::midLog("dtTitle_.LoadDt()\n");
		dtTitle_.LoadDt(csvdir.c_str(), CSV_TITLE);

		//コートデータ
		mid::midLog("dtCourt_.LoadDt()\n");
		dtCourt_.LoadDt(csvdir.c_str(), CSV_COURT);

		//BGMデータ
		mid::midLog("dtBGM_.LoadDt()\n");
		dtBGM_.LoadDt(csvdir.c_str(), CSV_BGM);


		dtShElm_.LoadDt(csvdir.c_str(), CSV_SHT_ELM);
		dtShSyn_.LoadDt(csvdir.c_str(), CSV_SHT_SYN);
		dtShRnd_.LoadDt(csvdir.c_str(), CSV_SHT_RND);
		dtShType_.LoadDt(csvdir.c_str(), CSV_SHT_TYPE);

#ifdef __K_DEBUG_SHIAI__
		// シュートの素材がロード終わったということでデバッグ環境にいただいていく
		kdebug::DebugSystem::GetInstance()->pDtShElm_ = &dtShElm_;
		kdebug::DebugSystem::GetInstance()->pDtShSyn_ = &dtShSyn_;
#endif	// #ifdef __K_DEBUG_SHIAI__


		//ランクデータ
		mid::midLog("dtRank_.LoadDt()\n");
		dtRank_.LoadDt(csvdir.c_str(), CSV_RANKDT);

		//フォーメーション
		mid::midLog("dtFomation_.LoadDt()\n");
		dtFomation_.LoadDt(csvdir.c_str(), CSV_FOMATIONDT);

		std::string comdir = csvdir;
		//comdir += mid::middir();
		comdir += COMDTDIR;
		comdir += mid::middir();

		//COM思考
		mid::midLog("dtCom_.LoadDt()\n");
		for (s32 i = 0; i < COMTYPE; ++i)
		{
			char s[16];
			sprintf(s, "%02ld", i);
			std::string str = CSV_COMDT;
			str += s;
			dtCom_[i].LoadDt(comdir.c_str(), str.c_str());
		}

		//COMACT思考
		mid::midLog("dtComAct_.LoadDt()\n");
		for (s32 i = 0; i < COMTYPE; ++i)
		{
			char s[16];
			sprintf(s, "%02ld", i);
			std::string strys = CSV_COMACTYSDT;
			strys += s;
			std::string strrs = CSV_COMACTRSDT;
			strrs += s;
			dtComAct_[i].LoadDt(comdir.c_str(), strys.c_str(), strrs.c_str());
		}

		//アニメ
		mid::midLog("dtani_.LoadDt()\n");
		dtBaAni_.LoadDt(csvdir.c_str(), CSV_BANI_PATT, CSV_BANI_DATA, CSV_BANI_SET);


	}

	void TMgData::Refresh()
	{
		std::string csvdir = mid::midDatadir() + mid::midCSVdir();
		//ゲーム設定データ
		mid::midLog("dtSet_.LoadDt()\n");
		dtSet_.LoadDt(csvdir.c_str(), CSV_SETTINGDT);
		//キャラ育成関連データ
		mid::midLog("dtCharaSet_.LoadDt()\n");
		dtCharaSet_.LoadDt(csvdir.c_str(), CSV_CHARA_SETTINGDT);
		//クライアント設定データ
		mid::midLog("dtClient_.LoadDt()\n");
		dtClient_.LoadDt(csvdir.c_str(), CSV_CLIENT_SETTINGDT);
		//ランクデータ
		mid::midLog("dtRank_.LoadDt()\n");
		dtRank_.LoadDt(csvdir.c_str(), CSV_RANKDT);
	}

	s32 TMgData::GetState(endbStateType st, s32 id)
	{
		s32 res = 0;
		switch (st)
		{
		case dbst_Pow: res = dtChar_.GetLv(clv_Pow, id); break;
		case dbst_Guts: res = dtChar_.GetLv(clv_Guts, id); break;
		case dbst_Kire: res = dtChar_.GetLv(clv_Kire, id); break;
		case dbst_Tech: res = dtChar_.GetLv(clv_Tech, id); break;
		case dbst_Speed: res = dtChar_.GetLv(clv_Speed, id); break;
		}

		return res;
	}


}
