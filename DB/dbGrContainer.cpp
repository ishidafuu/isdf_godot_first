#include "dbGrContainer.h"


namespace db
{
  //コンストラクタ
  TGrContainer::TGrContainer()
  {
    cntnBO_.SetVec();
    cntnBO_.Init();
    cntnPng_.Init();
  }
  //開放
  TGrContainer::~TGrContainer()
  {
    //管理されたpngデータの解放
    mid::midDeletePngDataList(&midPngList_);
    
    //コートの解放
    mid::midDeletePngData(&cntnPng_.png_Court_);

    //小物の解放
    mid::midDeletePngData(&cntnPng_.png_Etc_);
    
    //テクスチャ開放
    cntnBO_.DeleteTex();

	for (std::map<s32, mid::TBmpObj*>::iterator itr = cntnBO_.bmo_Person_.begin(); itr != cntnBO_.bmo_Person_.end(); itr++){ Freemem(itr->second); }
	for (std::map<s32, mid::TBmpObj*>::iterator itr = cntnBO_.bmo_PersonFace_.begin(); itr != cntnBO_.bmo_PersonFace_.end(); itr++){ Freemem(itr->second); }
	for (std::map<s32, mid::TBmpObj*>::iterator itr = cntnBO_.bmo_PersonHair_.begin(); itr != cntnBO_.bmo_PersonHair_.end(); itr++){ Freemem(itr->second); }
	cntnBO_.bmo_Person_.clear();
	cntnBO_.bmo_PersonFace_.clear();
	cntnBO_.bmo_PersonHair_.clear();
  }
  
  //全てのファイルを読み込む
  void TGrContainer::LoadBmpAll()
  {

    //pngファイルを読み込む（後にパレットを書き替える用）

    //ここでの読み込みはmidPngList_に入れて管理する
    mid::midLoadPngData(&cntnPng_.png_Body_, DIR_BODY, BMP_BODY, &midPngList_, FALSE);
    mid::midLoadPngData(&cntnPng_.png_Mouse_, DIR_BODY, BMP_MOUSE, &midPngList_, FALSE);
    mid::midLoadPngData(&cntnPng_.png_Chest_, DIR_BODY, BMP_CHEST, &midPngList_, FALSE);
    mid::midLoadPngData(&cntnPng_.png_Cap_, DIR_BODY, BMP_CAP, &midPngList_, FALSE);
    mid::midLoadPngData(&cntnPng_.png_Shot_, DIR_BODY, BMP_SHOT, &midPngList_, FALSE);
	mid::midLoadPngData(&cntnPng_.png_Person_, DIR_BODY, BMP_PERSON, &midPngList_, FALSE);
    for (s32 i = 0; i < FACENUM_ALL; ++i)
    {
      char s[16];
      sprintf(s, "%02ld", i);
      std::string kao = BMP_KAO;
      kao += s;
      mid::midLoadPngData(&cntnPng_.png_Face_[i], DIR_BODY, kao.c_str(), &midPngList_, FALSE);
    }

    for (s32 i = 0; i < FACENUM_ALL; ++i)
    {
      char s[16];
      sprintf(s, "%02ld", i);
      std::string zura = BMP_ZURA;
      zura += s;
      mid::midLoadPngData(&cntnPng_.png_Hair_[i], DIR_BODY, zura.c_str(), &midPngList_, FALSE);
    }

    mid::midLoadPngData(&cntnPng_.png_Line_, DIR_GAME, BMP_LINE, &midPngList_, FALSE);
    mid::midLoadPngData(&cntnPng_.png_Referee_, DIR_GAME, BMP_REFEREE, &midPngList_, FALSE);
    mid::midLoadPngData(&cntnPng_.png_Ball_, DIR_GAME, BMP_BALL, &midPngList_, FALSE);
    //mid::midLoadPngData(&cntnPng_.png_Etc_, DIR_GAME, BMP_ETC, &midPngList_, FALSE);

    mid::midLoadPngData(&cntnPng_.png_Sensyu_, DIR_BODY, BMP_UIBODY, &midPngList_, FALSE);
    mid::midLoadPngData(&cntnPng_.png_Kantoku_, DIR_BODY, BMP_KANTOKU, &midPngList_, FALSE);
	mid::midLoadPngData(&cntnPng_.png_Captain_, DIR_BODY, BMP_UIBODY, &midPngList_, FALSE);

   
    //おみせやさん
    mid::midLoadCI(&cntnBO_.bmo_Clerk_, DIR_BODY, BMP_CLERK);
    //女子高生
    mid::midLoadCI(&cntnBO_.bmo_Girl_, DIR_BODY, BMP_GIRL);

    mid::midLoadPngData(&cntnPng_.png_Etc_, DIR_GAME, BMP_ETC, FALSE);//TPngDataList* ppngdataListに登録しない
  }

  //コートのロード
  void TGrContainer::LoadCourt(const char* courtname)
  {
    //まず解放
    mid::midDeletePngData(&cntnPng_.png_Etc_);
    mid::midLoadPngData(&cntnPng_.png_Etc_, DIR_GAME, BMP_ETC,  FALSE);//TPngDataList* ppngdataListに登録しない

    //まず解放
    mid::midDeletePngData(&cntnPng_.png_Court_);
    //ここではＰＮＧ読み込みのみ　後に統合テクスチャに読み込む
    mid::midLoadPngData(&cntnPng_.png_Court_, "court", courtname, TRUE);//TPngDataList* ppngdataListに登録しない
  }

  void TGrContainer::ReloadTex()
  {
    //おみせやさん
    mid::midLoadCI(&cntnBO_.bmo_Clerk_, DIR_BODY, BMP_CLERK);
    //女子高生
    mid::midLoadCI(&cntnBO_.bmo_Girl_, DIR_BODY, BMP_GIRL);

//    //おみせやさん
//    cntnBO_.bmo_Clerk_.ReloadTex();
//    //女子高生
//    cntnBO_.bmo_Girl_.ReloadTex();
  }
  
}
