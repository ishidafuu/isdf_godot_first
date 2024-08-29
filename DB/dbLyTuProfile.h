#pragma once
#include "dbUILayer.h"
#include "dbUGLiMenu.h"
#include "dbUGLiRanking.h"
#include "dbUGLiRanker.h"
#include "dbUGLiCol.h"
#include "dbUGLiFace.h"
#include "dbUGPlateProf.h"


namespace db
{

  struct TTuProfileSt
  {
    s32 hair_;
    s32 face_;
    s32 haircol_;
    s32 skincol_;
    s32 eyecol_;
    s32 acccol_;
    s32 clothecol_;
    s32 profline_;
    s32 hueno_;
    BOOL gb_f_;

    void Init()
    {
      Zeromem(this);
    }
    void SetHueNo(s32 hueno)
    {
      hueno_ = ((hueno / 10) * 10);
    }
  };
  class TLyBarGen;

  class TLyTuProfile :public TUILayer
  {
  protected:
    TLyBarGen* lybar_;
	TTuProfileSt stProfile_;
    TUGPlateProf ugprof_;
	TUGLiMenu uglistMenu_;
    TUGLiMenu uglistProfMenu_;
    TUGLiCol uglistCol_;
    TUGLiFace uglistFace_;

	INTVECTOR dtvec_;
	std::string myName_;
	s32 faceCharaNo_;	// チュートリアルだけの特別変数(特殊セリフ用)
	s32 hairCharaNo_;	//						〃

    std::vector<TUGLiGen*> listvec_;
    std::string name_;
    std::string prof_;

	BOOL isNameChange_;
	BOOL isHairChange_;

  TUGButton ugbtn_back_prof_;
  TUGButton ugbtn_mid_prof_;
    //void RefreshTex();
    void SetMyAvtVar();
    //void SetAvtVar(s64 id_ranker);

    void MakeChest(s32 pageno, INTVECTOR dtvec);

    void DrawList(TUGLiGen* drawlist);
    void ResetObj();
    void DoChangePage(s32 pageno);
    void RefreshProf();

	void MvPage_Menu();

    void MvPage_ProfMenu();
    void MvPage_Name();
    void MvPage_Prof0();
    void MvPage_Prof1();
    void MvPage_Prof2();
    void MvPage_Hair();
    void MvPage_Face();
    void MvPage_Haircol();
    void MvPage_Haircol2();
    void MvPage_SkinCol();
    void MvPage_SkinCol2();
    void MvPage_EyeCol();
    void MvPage_EyeCol2();
    void MvPage_AccCol();
    void MvPage_AccCol2();
    void MvPage_ClotheCol();
    void MvPage_ClotheCol2();
	void MvPage_Out();

    void Post_Name();
    void Post_Prof();
    void Post_Avatar();

    void Connect_Name();
    void Connect_Prof();
    void Connect_Avatar();

	//オーバーロード
    void DoLoad();
    void DoReady();
    void DoDown();
    void DoConnect(BOOL rqok_f);
    void DoUpdate(BOOL draw_f);
  public:
    //コンストラクタ
    explicit TLyTuProfile(TScene* pScene, TUIFrame* frame, TLyBarGen* lybar);
    virtual ~TLyTuProfile(void);

	inline BOOL IsNameChange(void){ return isNameChange_; }
	inline BOOL IsHairChange(void){ return isHairChange_; }

	inline std::string GetMyName(void) { return myName_; }
	inline s32 GetFaceCharaID(void){ return faceCharaNo_; }	// チュートリアルだけの特別変数(特殊セリフ用)
	inline s32 GetHairCharaID(void){ return hairCharaNo_; }	//	
  };
 
}
