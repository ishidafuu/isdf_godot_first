#pragma once
#include "dbInclude.h"

namespace db
{

  //各ファイル名
  const char BMP_ROOT[] = "";

  const char BMP_FONT[] = "font";
  const char BMP_FONTPRO[] = "fontpro";
  //const char BMP_OBJ[]  = "obj";
  const char DIR_BODY[] = "body";
  const char DIR_GAME[] = "game";
  const char DIR_BG[] = "bg";

  const char BMP_ETC[] = "tile_etc";//タイル形式
  const char BMP_Etc[] = "tile_Etc";//タイル形式

  const char BMP_BALL[] = "tile_ball";//タイル形式
  const char BMP_MOUSE[] = "mouse";
  const char BMP_CHEST[] = "chest";
  const char BMP_CAP[] = "cap";
  const char BMP_SHOT[] = "shot";
  //const char BMP_MONOFACE[]  = "monoface";
  //const char BMP_MONOBODY[]  = "monobody";
  //const char BMP_DBCOURT0[] = "dbcourt0";
  //const char BMP_DBCOURT1[] = "dbcourt1";
  //const char BMP_DBCOURT2[] = "dbcourt2";
  //const char BMP_DBCOURT3[] = "dbcourt3";
  //const char BMP_DBCOURT4[] = "dbcourt4";
  //const char BMP_DBCOURT5[] = "dbcourt5";
  //const char BMP_DBCOURT6[] = "dbcourt6";
  //const char BMP_DBCOURT7[] = "dbcourt7";
  const char BMP_LINE[] = "line";
  const char BMP_REFEREE[] = "tile_referee";//タイル形式
  const char BMP_KAO[] = "kao";
  const char BMP_ZURA[] = "zura";
  //const char BMP_KAO0[] = "kao0";
  //const char BMP_KAO1[]  = "kao1";
  //const char BMP_KAO2[]  = "kao2";
  //const char BMP_KAO3[]  = "kao3";
  //const char BMP_ZURA0[]  = "zura0";
  //const char BMP_ZURA1[]  = "zura1";
  //const char BMP_ZURA2[]  = "zura2";
  //const char BMP_ZURA3[]  = "zura3";

  const char BMP_BODY[] = "tile_body";//タイル形式
  const char BMP_UIBODY[] = "tile_sensyu";
  const char BMP_KANTOKU[] = "tile_kantoku";
  const char BMP_CLERK[] = "tile_clerk";
  const char BMP_GIRL[] = "tile_girl";
  const char BMP_PERSON[] = "tile_person";

  const u16 COURTNUM = 6;//6;
  const u16 BALLSPNUM = 3;
  const u16 FACENUM_ALL = 14;//FACENUM_MAIN + FACENUM_DL;
  const u16 FACENUM_Y = 35;//1fileに35

  //アイコン用顔テクスチャの数
  const u16 ICON_FACENUM = 3;
  const u16 ICON_HAIRNUM = 5;
  const u16 ICON_MONO_FACENUM = 2;
  const u16 ICON_MONO_HAIRNUM = 3;

  //肌色の数
  const s32 ICON_MOUSENUM = base::skp2END;


  struct TGrContainerPng
  {
    //PNGデータ（パレット書き替えて使う元画像）
    mid::TPng png_Body_;//体
    mid::TPng png_Mouse_;//口
    mid::TPng png_Chest_;//胸
    mid::TPng png_Cap_;//キャプテン
    mid::TPng png_Shot_;//シュート
    mid::TPng png_Face_[FACENUM_ALL];//顔
    mid::TPng png_Hair_[FACENUM_ALL];//髪
    mid::TPng png_MonoBody_;//モノクロ体
    mid::TPng png_MonoFace_;//モノクロ顔
    mid::TPng png_Court_;//コート
    mid::TPng png_Line_; //コートのライン
    mid::TPng png_Referee_;//レフェリー
    mid::TPng png_Ball_;//ボール
    mid::TPng png_Etc_;//その他
    //mid::TPng png_Etc_;//その他

    mid::TPng png_Sensyu_;//体
    mid::TPng png_Captain_;//キャプテン
    mid::TPng png_Kantoku_;//監督
	mid::TPng png_Person_; // Tipsキャラ
	
    void Init()
    {
      Zeromem(this);
    }
  };

  struct TGrContainerBmpObj
  {
    //全てのBmpObjを保存
    std::vector<mid::TBmpObj*> bovec_;

    //pngデータから画像を生成するやつら

    //試合用統合テクスチャ
    mid::TBmpObj bmo_Game_;

    //顔アイコン用
//    mid::TBmpObj bmo_IconFace_[ICON_FACENUM];
//    mid::TBmpObj bmo_IconHair_[ICON_HAIRNUM];
//    mid::TBmpObj bmo_IconMouse_[ICON_MOUSENUM];
    //UI用アイコン
    mid::TBmpObj bmo_IconChest_[DBSIDE];
    mid::TBmpObj bmo_IconBody_[DBSIDE];
    mid::TBmpObj bmo_IconChestSub_[DBSIDE];
    mid::TBmpObj bmo_IconBodySub_[DBSIDE];
    mid::TBmpObj bmo_IconBodySh_;
	mid::TBmpObj bmo_IconRankerChest_[RANKERNUM];
	mid::TBmpObj bmo_IconRankerChestSub_[RANKERNUM];

    mid::TBmpObj bmo_Boss_;
    mid::TBmpObj bmo_Zako_;

    //直に読み込んで、以降変えない奴ら
    mid::TBmpObj bmo_Ball_[BALLSPNUM];
    mid::TBmpObj bmo_Referee_;
    mid::TBmpObj bmo_Etc_;
    //mid::TBmpObj bmo_Etc_;

    mid::TBmpObj bmo_Clerk_;
    mid::TBmpObj bmo_Girl_;

    mid::TBmpObj bmo_Kantoku_;
    mid::TBmpObj bmo_Captain_;
    mid::TBmpObj bmo_Sensyu_[DBMEMBER_ALL];

    mid::TBmpObj bmo_KantokuFace_;
    mid::TBmpObj bmo_CaptainFace_;
    mid::TBmpObj bmo_SensyuFace_[DBMEMBER_ALL];

    mid::TBmpObj bmo_KantokuHair_;
    mid::TBmpObj bmo_CaptainHair_;
    mid::TBmpObj bmo_SensyuHair_[DBMEMBER_ALL];

    mid::TBmpObj bmo_Enemy_;
    mid::TBmpObj bmo_EnemyFace_;
    mid::TBmpObj bmo_EnemyHair_;

	mid::TBmpObj bmo_Enemies_[PLAYERNUM];
	mid::TBmpObj bmo_EnemiesFace_[PLAYERNUM];
	mid::TBmpObj bmo_EnemiesHair_[PLAYERNUM];

	mid::TBmpObj bmo_Captains_[PLAYERNUM];
	mid::TBmpObj bmo_CaptainsFace_[PLAYERNUM];
	mid::TBmpObj bmo_CaptainsHair_[PLAYERNUM];

	std::map<s32, mid::TBmpObj*> bmo_Person_;
	std::map<s32, mid::TBmpObj*> bmo_PersonFace_;
	std::map<s32, mid::TBmpObj*> bmo_PersonHair_;

    void SetVec()
    {
      bovec_.push_back(&bmo_Game_);
//      for (s32 i = 0; i < ICON_FACENUM; ++i) bovec_.push_back(&bmo_IconFace_[i]);
//      for (s32 i = 0; i < ICON_HAIRNUM; ++i) bovec_.push_back(&bmo_IconHair_[i]);
//      for (s32 i = 0; i < ICON_MOUSENUM; ++i) bovec_.push_back(&bmo_IconMouse_[i]);
      for (s32 i = 0; i < DBSIDE; ++i)
      {

        bovec_.push_back(&bmo_IconBody_[i]);
        bovec_.push_back(&bmo_IconChest_[i]);
        bovec_.push_back(&bmo_IconBodySub_[i]);
        bovec_.push_back(&bmo_IconChestSub_[i]);
      }
	  for (s32 i = 0; i < RANKERNUM; i++)
	  {
		  bovec_.push_back(&bmo_IconRankerChest_[i]);
		  bovec_.push_back(&bmo_IconRankerChestSub_[i]);
	  }
      bovec_.push_back(&bmo_IconBodySh_);

      bovec_.push_back(&bmo_Boss_);

      for (s32 i = 0; i < BALLSPNUM; ++i)bovec_.push_back(&bmo_Ball_[i]);
      bovec_.push_back(&bmo_Referee_);
      bovec_.push_back(&bmo_Etc_);
      //bovec_.push_back(&bmo_Etc_);

      bovec_.push_back(&bmo_Clerk_);
      bovec_.push_back(&bmo_Girl_);

      bovec_.push_back(&bmo_Kantoku_);
      bovec_.push_back(&bmo_KantokuFace_);
      bovec_.push_back(&bmo_KantokuHair_);
      bovec_.push_back(&bmo_Captain_);
      bovec_.push_back(&bmo_CaptainFace_);
      bovec_.push_back(&bmo_CaptainHair_);
      for (s32 i = 0; i < DBMEMBER_ALL; ++i)
      {
        bovec_.push_back(&bmo_Sensyu_[i]);
        bovec_.push_back(&bmo_SensyuFace_[i]);
        bovec_.push_back(&bmo_SensyuHair_[i]);
      }
      bovec_.push_back(&bmo_Enemy_);
      bovec_.push_back(&bmo_EnemyFace_);
      bovec_.push_back(&bmo_EnemyHair_);
	  for (s32 i = 0; i < PLAYERNUM; i++)
	  {
		  bovec_.push_back(&bmo_Enemies_[i]);
		  bovec_.push_back(&bmo_EnemiesFace_[i]);
		  bovec_.push_back(&bmo_EnemiesHair_[i]);
	  }
	  for (s32 i = 0; i < PLAYERNUM; i++)
	  {
		  bovec_.push_back(&bmo_Captains_[i]);
		  bovec_.push_back(&bmo_CaptainsFace_[i]);
		  bovec_.push_back(&bmo_CaptainsHair_[i]);
	  }
    }

    void Init()
    {
      for (s32 i = 0; i < bovec_.size(); ++i) bovec_[i]->Init();
    }

    void DeleteTex()
    {
      for (s32 i = 0; i < bovec_.size(); ++i) bovec_[i]->DeleteTex();
    }

	void AddPerson(s32 key)
	{
		// キーがすでにないかチェック
		if (bmo_Person_.count(key) == 0)
		{
			mid::TBmpObj* body = new mid::TBmpObj();
			mid::TBmpObj* face = new mid::TBmpObj();
			mid::TBmpObj* hair = new mid::TBmpObj();
			body->Init();
			face->Init();
			hair->Init();
			bmo_Person_[key] = body;
			bmo_PersonFace_[key] = face;
			bmo_PersonHair_[key] = hair;
			bovec_.push_back(body);
			bovec_.push_back(face);
			bovec_.push_back(hair);
		}
	}
  };


  //画像ファイルコンテナ（ただのローダーなので変数はpublicで出しときます）
  class TGrContainer
  {
  protected:
    //読み込んだPNGデータリスト
    mid::TPngDataList midPngList_;

  public:
    //変数
    TGrContainerPng cntnPng_;
    TGrContainerBmpObj cntnBO_;

    //コンストラクタ
    explicit TGrContainer();
    virtual ~TGrContainer();

    //関数
    void LoadBmpAll();
    //void LoadCourt(u16 courtNo, mid::enLoadMode loadMode);
    void LoadCourt(const char* courtname);

    void ReloadTex();
  };
}
