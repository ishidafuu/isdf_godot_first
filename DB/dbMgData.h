#pragma once
#include "dbDtSetting.h"
#include "dbDtCharaSetting.h"
#include "dbDtRank.h"
#include "dbDtChar.h"
#include "dbDtCell.h"
#include "dbDtBM.h"
#include "dbDtFomation.h"
#include "dbDtCom.h"
#include "dbDtComAct.h"
#include "dbDtElement.h"
#include "dbDtShElm.h"
#include "dbDtShSyn.h"
#include "dbDtShRnd.h"
#include "dbDtShType.h"
#include "dbDtCourt.h"
#include "dbDtTitle.h"
#include "dbDtBGM.h"
#include "dbDtBallAnime.h"
#include "dbDtClientSetting.h"

namespace db
{
  //ファイル名//拡張子はiosの事も考えて、midの方で付ける
  //const char CSV_EXT[] = ".csv";
  const char CSV_SETTINGDT[] = "dbz_settingdt";
  const char CSV_CHARA_SETTINGDT[] = "dbz_charasettingdt";
  const char CSV_CLIENT_SETTINGDT[] = "dbz_clientsettingdt";
  const char CSV_RANKDT[] = "dbz_rankdt";
  const char CSV_FOMATIONDT[] = "dbz_fomation";

  const char CSVDTDIR[] = "csv";
  const char COMDTDIR[] = "com";

  const char CSV_COMDT[] = "dbz_com";
  const char CSV_COMACTYSDT[] = "dbz_comactys";
  const char CSV_COMACTRSDT[] = "dbz_comactrs";

  const char CSV_CCDT[] = "ccdt_body";
  const char CSV_BMDT[] = "bmdt_body";

  const char CSV_CCDT_KANTOKU[] = "ccdt_kantoku";
  const char CSV_BMDT_KANTOKU[] = "bmdt_kantoku";
  const char CSV_CCDT_SENSYU[] = "ccdt_sensyu";
  const char CSV_BMDT_SENSYU[] = "bmdt_sensyu";
  const char CSV_BMDT_CLERK[] = "bmdt_clerk";
  const char CSV_BMDT_GIRL[] = "bmdt_girl";
  const char CSV_CCDT_PERSON[] = "ccdt_person";
  const char CSV_BMDT_PERSON[] = "bmdt_person";

  const char CSV_CHARDT_DL[] = "master_char";
  const char CSV_ELEM_CHANGE[] = "elem_change";
  const char CSV_ELEM_EFFECT[] = "elem_effect";
  const char CSV_ELEM_SHAPE[] = "elem_shape";
  const char CSV_ELEM_ORBIT[] = "elem_orbit";
  const char CSV_ELEM_TARGET[] = "elem_target";

  const char CSV_BANI_PATT[] = "db_BallAnimePatt";
  const char CSV_BANI_DATA[] = "db_BallAnimeData";
  const char CSV_BANI_SET[] = "db_BallAnimeSet";

  const char CSV_SHT_ELM[] = "master_sht_elm";
  const char CSV_SHT_SYN[] = "master_sht_syn";
  const char CSV_SHT_RND[] = "master_rnd_elm";
  const char CSV_SHT_TYPE[] = "master_sht_type";
  const char CSV_TITLE[] = "master_title";
  const char CSV_COURT[] = "master_court";
  const char CSV_BGM[] = "master_bgm";

  class TMgData
  {
  public:

    TDtSetting dtSet_;//設定データ
	TDtCharaSetting dtCharaSet_; // キャラ育成設定データ
	TDtClientSetting dtClient_; // クライアント設定データ

    TDtChar dtChar_;//キャラデータ
    TDtRank dtRank_;//ランクデータ
    TDtFomation dtFomation_;//フォーメーションデータ
    TdtComData dtCom_[COMTYPE];//COM思考データ
    TDtComAct dtComAct_[COMTYPE];//COMACT思考データ


    TDtTitle dtTitle_;//タイトル
    TDtCourt dtCourt_;//コート
    TDtBGM dtBGM_;//BGM

    TBMData dtBM_;//BMデータ
    TDtCell dtCell_;//キャラ描画用データ

    TBMData dtBM_kantoku_;//BMデータ
    TDtCell dtCell_kantoku_;//キャラ描画用データ
    //TBMData dtBM_captain_;//BMデータ
    //TDtCell dtCell_captain_;//キャラ描画用データ
    TBMData dtBM_sensyu_;//BMデータ
    TDtCell dtCell_sensyu_;//キャラ描画用データ
    TBMData dtBM_clerk_;//BMデータ
    TBMData dtBM_girl_;//BMデータ
	TBMData dtBM_person_;//BMデータ
	TDtCell dtCell_person_;//キャラ描画用データ

    TDtElement dtElem_;//素材データ

    TDtShElm dtShElm_;//素材データ
    TDtShSyn dtShSyn_;//素材データ
	TDtShRnd dtShRnd_;//素材データ(ランダム用)
    TDtShType dtShType_;//素材データ

    TDtBallAnime dtBaAni_;//ボールアニメーション



    explicit TMgData(void);
    virtual ~TMgData(void);


    //読み込み
    void LoadDt();
    void Refresh();
    s32 GetState(endbStateType st, s32 id);
  };
}
