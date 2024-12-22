#pragma once
#include "dbObGameGen.h"

namespace db
{
  class TMgCommon : public TGameObjGen
  {
  public:
    //コンストラクタ
    explicit TMgCommon(TGame* pGame);
    virtual ~TMgCommon();
    
    BOOL IsPer100(s32 border);
    s32 GetRand(s32 len);
    
    void MakeCharNameTex();

    //ポップアップ
    void PopupMsg(enPopUp putype, const char* msg, s32 sideNo, s32 posX, s32 posY, s32 posZ, enFontCol2 col, BOOL dam_f);
    void PopupMsg(s32 msg, s32 posX, s32 posY, s32 posZ, enFontCol2 col);
	void PopupMsgDelay(enPopUp putype, const char* msg, s32 sideNo, s32 posX, s32 posY, s32 posZ, enFontCol2 col, s32 delay, BOOL dam_f);
	void ResetPopupMsg(void);

    //小物オブジェを天使とかにする
    void SetEObj(enEtcObjMotion tMType,TZahyouRec tZahyou, s32 tSideNo, s32 tCharNo);

    //COM思考
    s32 PtnDtRev(s32 ptndt[], s32 lv, s32 adNo);
    s32 PtnDtRev(s32 ptndt[], s32 lv, s32 adNo, s32 adNo2);
    s32 PtnDtRev(s32 ptndt[], s32 lv, s32 adNo, s32 adNo2, s32 adNo3);
    s32 GetComPtn(s32 comType, enCom comDt, s32 sideNo);
    s32 GetComActPtn(s32 comType, enDtComAct cmaDt, BOOL ressei_f, s32 sideNo, s32 id);
    BOOL GetRandPer(s32 perptn);
    
    
    //センタリング用文字列描画位置
    //s32 TitlePosX(s32 msgLength);
  };
}
