#pragma once
#include "dbInclude.h"

namespace db
{
  class TMgGrp;
  class TMgEngineObj;
  class TMgEtcGrp;
  class TDtCell;
  
  struct TCharLooks//0~255
  {
    u16 ffileNo_;//顔番号
    u16 hfileNo_;//顔番号
    u16 faceNo_;//顔番号
    u16 hairNo_;//髪番号
    u16 skinHue_;
    u16 hairHue_; u16 hairLum_;
    u16 acceHue_; u16 acceLum_;
    u16 eyesHue_; u16 eyesLum_;
    u16 shirHue_; u16 shirLum_;
    u16 pantHue_; u16 pantLum_;
    u16 lineHue_; u16 lineLum_;
    u16 shoeHue_; u16 shoeLum_;
    BOOL rand_f_;
    BOOL mono_f_;
  };
  
  
  struct TCharDraw
  {
    s32 depth_;
    BOOL mirror_f_;
    mid::enRoll roll_;
    BOOL cap_f_;
    
    BOOL ang_f_;
    s32 cadam_c_;
    //s32 shTg_c_;
    s32 cellNo_;
    BOOL ov_f_;

    mid::enDrawMode drawmode_;
    mid::TARGB argb_;//カラー
    mid::TARGB argb2_;//カラー2
    s32 limL_;
    s32 limR_;
    s32 limU_;
    s32 limD_;

    void Init()
    {
      Zeromem(this);
      argb_.SetDEFCOL();
      argb2_.SetDEFCOL();
    }
  };
  
  
  
  //キャラ画像管理クラスステータス
  class TStGrChar
  {
  public:
    TMgEngineObj* pmgEO_;
    s64 mid_char_face_;
    s64 mid_char_hair_;
    s64 mid_team_;
    lib_rgb::RGBPAL pal_;//dxパレット
    s32 skincol_;
   
    //コンストラクタ
    explicit TStGrChar(TMgEngineObj* pmgEO);
    virtual ~TStGrChar(){};

    void SetCharDt(s64 mid_char, s64 mid_team, BOOL gb_f);
    void SetCharDt(s32 hair, s32 face, s32 haircol, s32 skincol, s32 eyecol, s32 acccol, s32 clothecol, BOOL gb_f);
	void SetCharDt(s32 hair, s32 face, s32 haircol, s32 skincol, s32 eyecol, s32 acccol, s32 wearcol, s32 pantscol, s32 sidecol, s32 undercol, s32 shoecol, BOOL gb_f);
//    s32 GetLooks(enCharLooks dtNo);
    s32 GetLooks_ZuraFile();
    s32 GetLooks_ZuraNo();
    s32 GetLooks_FaceFile();
    s32 GetLooks_FaceNo();
  };
  
  
  //キャラ画像管理クラス
  class TMgCharGrp
  {
  protected:
    
    //エンジンオブジェ
    TMgEngineObj* pmgEO_;
    TMgEtcGrp* etc_;
    
    //角度計算
    mid::enRoll AddRoll(mid::enRoll r, s32 addr);
    //画像全体を回転させたときの描画座標
    void RotetPos(s32* posX, s32* posY, BOOL mirror_f, mid::enRoll r);
    //一部を中心に画像を回転させる（ずらを中心にかおとあごを回転）
    void RotetPart(s32* posX, s32* posY, s32 cposX, s32 cposY, mid::enRoll r);
  public:
    base::TGrTile* body_;
    base::TGrTile2* face_;
    base::TGrTile2* hair_;
    TDtCell* ccdt_;

    //キャラ画像管理クラスステータス
    TStGrChar stGC_;
    
    //コンストラクタ
    explicit TMgCharGrp(TMgEngineObj* pmgEO);
    
    virtual ~TMgCharGrp();
    
    //キャラ番号設定
    void SetCharDt(s64 mid_char, s64 mid_team, BOOL gb_f);
    void SetCharDt(s32 hair, s32 face, s32 haircol, s32 skincol, s32 eyecol, s32 acccol, s32 clothecol, BOOL gb_f);
	void SetCharDt(s32 hair, s32 face, s32 haircol, s32 skincol, s32 eyecol, s32 acccol, s32 wearcol, s32 pantscol, s32 sidecol, s32 undercol, s32 shoecol, BOOL gb_f);
    void SetMyKantokuDt();
    void SetRankerDt(s64 id_ranker);
	void SetRankerDtByData(s64 id_ranker);
    void SetEnemyDt(s32 no);
    //キャラ番号設定
    void SetGrTile(base::TGrTile* body);
    void SetGrTile(s32 sideno, s32 charno);
    void SetGrTile(base::TGrTile* body, base::TGrTile2* face, base::TGrTile2* hair, TDtCell* ccdt);
    //void FreeFace();

//    s32 GetLooks(enCharLooks dtNo);
    s32 GetLooks_ZuraFile();
    s32 GetLooks_ZuraNo();
    s32 GetLooks_FaceFile();
    s32 GetLooks_FaceNo();
    //描画オーダー
    void OrderDraw(u16 bodyNo, s32 dx, s32 dy, TCharDraw* pcdst);
    //影描画
    void OrderShadow(s32 posX, s32 posY, BOOL nazo_f, BOOL prev_f);
    
  };
  
  
}
