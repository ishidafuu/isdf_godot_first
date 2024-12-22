#include "dbLyTuProfile.h"
#include "dbSnTutorial.h"
#include "dbLyBarGen.h"
#include "dbUGFinger.h"

namespace db
{
  enum enProfMenuList
  {
    pml_Name,
    pml_Prof0,
    pml_Prof1,
    pml_Prof2,
    pml_Hair,
    pml_Face,
    pml_HairCol,
    pml_SkinCol,
    pml_EyeCol,
    pml_AccCol,
    pml_ClotheCol,
    pmlEND,
  };


  enum enPage
  {
	  pg_ProfMenu,
	  pg_Menu,
    pg_Name,
    pg_Prof0,
    pg_Prof1,
    pg_Prof2,
    pg_Hair,
    pg_Face,
    pg_HairCol,
    pg_HairCol2,
    pg_SkinCol,
    pg_SkinCol2,
    pg_EyeCol,
    pg_EyeCol2,
    pg_AccCol,
    pg_AccCol2,
    pg_ClotheCol,
    pg_ClotheCol2,
    pgEND,
  };

  enum enConnect
  {
    cn_Name,
    cn_Prof,
    cn_Avatar,
  };

  //コンストラクタ
  TLyTuProfile::TLyTuProfile(TScene* pScene, TUIFrame* frame, TLyBarGen* lybar) :TUILayer(pScene, frame)
    , ugprof_(this)
	, uglistMenu_(this)
    , uglistProfMenu_(this)
    , uglistFace_(this)
    , uglistCol_(this)
	, isNameChange_(FALSE)
	, isHairChange_(FALSE)
  , ugbtn_back_prof_(this)
  , ugbtn_mid_prof_(this)
  , faceCharaNo_(-1)
  , hairCharaNo_(-1)
  , myName_("かんとくくん")
  {
    lybar_ = lybar;
    stProfile_.Init();
    stLy_.hedderlayer_f_ = TRUE;//ヘッダ用レイヤ
  }

  TLyTuProfile::~TLyTuProfile()
  {

  }
  void TLyTuProfile::DoLoad()
  {
    //ヘッダフッタ間フィルタ
    field_->g_SetCenter(TRUE);//フィルタはフィルタサイズが小さいためセンタリングが正常に効かない
    field_->o_SetObjFilterGrp(512, 512, 0xFF, 0, 0, 0);
    field_->g_SetAlp(FALSE, 0x80);
    field_->g_SetDepth(UIDP_06BUTTONHI);
    field_->g_SetDraw(FALSE);

    ugprof_.MakeGroup(field_, TRUE);
    ugprof_.SetProfMe();

	uglistMenu_.MakeGroup(field_, FALSE);
	uglistMenu_.SetDepth(UIDP_07BUTTONTOP);

    uglistProfMenu_.MakeGroup(field_, FALSE);
    uglistProfMenu_.SetDepth(UIDP_07BUTTONTOP);

    uglistCol_.MakeGroup(field_, FALSE);
    uglistCol_.SetDepth(UIDP_07BUTTONTOP);

    uglistFace_.MakeGroup(field_, FALSE);
    uglistFace_.SetDepth(UIDP_07BUTTONTOP);

    listvec_.clear();
	listvec_.push_back(&uglistMenu_);
    listvec_.push_back(&uglistProfMenu_);
    listvec_.push_back(&uglistCol_);
    listvec_.push_back(&uglistFace_);

	TUGRcNormal* tmprecord = (TUGRcNormal*)uglistMenu_.MakeObj_Record();
	tmprecord->ugname_.SetNameIcon("プロフィール", ugi_Prof);


    for (s32 i = 0; i < pmlEND; ++i)
    {
      TUGRcNormal* newrecord = (TUGRcNormal*)uglistProfMenu_.MakeObj_Record();
      switch (i)
      {
      case db::pml_Name:newrecord->ugname_.SetNameIcon("なまえ＿へんこう", ugi_EditProf);  break;
      case db::pml_Prof0:newrecord->ugname_.SetNameIcon("コメント１＿へんこう", ugi_EditProf);  break;
      case db::pml_Prof1:newrecord->ugname_.SetNameIcon("コメント２＿へんこう", ugi_EditProf);  break;
      case db::pml_Prof2:newrecord->ugname_.SetNameIcon("コメント３＿へんこう", ugi_EditProf);  break;
      case db::pml_Hair:newrecord->ugname_.SetNameIcon("かみがた＿へんこう", ugi_Zura);  break;
      case db::pml_Face:newrecord->ugname_.SetNameIcon("つらがまえ＿へんこう", ugi_Kao);  break;
      case db::pml_HairCol:newrecord->ugname_.SetNameIcon("かみいろ＿へんこう", ugi_Col);  break;
      case db::pml_SkinCol:newrecord->ugname_.SetNameIcon("はだいろ＿へんこう", ugi_Col);  break;
      case db::pml_EyeCol:newrecord->ugname_.SetNameIcon("めのいろ＿へんこう", ugi_Col);  break;
      case db::pml_AccCol:newrecord->ugname_.SetNameIcon("アクセいろ＿へんこう", ugi_Col);  break;
      case db::pml_ClotheCol:newrecord->ugname_.SetNameIcon("せいふく＿へんこう", ugi_Col);  break;
      }
    }

    ugbtn_back_prof_.MakeGroup_FtPos(field_, 0, "もどる");
    ugbtn_mid_prof_.MakeGroup_FtPos(field_, 2, "こうしん");
    ugbtn_back_prof_.SetDepth(UIDP_09MSGBOX);
    ugbtn_mid_prof_.SetDepth(UIDP_09MSGBOX);
    ugbtn_back_prof_.SetDraw(TRUE);
  }

  void TLyTuProfile::SetMyAvtVar()
  {
    name_ = pmgEO_->mgCoU_.mdKantokuSub_.GetMyDtStr(mdkantokusub_name_c32);
    stProfile_.hair_ = pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_avtHair);
    stProfile_.face_ = pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_avtFace);
    stProfile_.haircol_ = pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_avtHairCol);
    stProfile_.skincol_ = pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_avtSkinCol);
    stProfile_.eyecol_ = pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_avtEyeCol);
    stProfile_.acccol_ = pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_avtAccCol);
    stProfile_.clothecol_ = pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_avtClotheCol);

    //監督とキャプテン読み込み
    pmgEO_->mgGrp_.SetProf_Me();

    //監督アイコンと体
    pmgEO_->mgGrp_.SetKantoku();
  }

  //void TLyTuProfile::SetAvtVar(s64 id_ranker)
  //{
  //  pmgEO_->mgGrp_.SetKantoku_Ranker(id_ranker);
  //}

  //void TLyTuProfile::RefreshTex()
  //{
  //  pmgEO_->mgGrp_.MakeKantokuTexture();
  //  pmgEO_->mgGrp_.pmgcg_kantoku_->SetGrTile(
  //    pmgEO_->mgGrp_.pgr_Kantoku_,
  //    pmgEO_->mgGrp_.pgr_KantokuFace_,
  //    pmgEO_->mgGrp_.pgr_KantokuHair_,
  //    &pmgEO_->mgDt_.dtCell_kantoku_);
  //}
  //入店時の処理
  void TLyTuProfile::DoReady()
  {
    stProfile_.Init();
    field_->g_SetDraw(TRUE);
    SetMyAvtVar();

    //ugbtn_back_prof_.SetFont("おーけー！");
    //ugbtn_back_prof_.SetDepth(UIDP_07BUTTONTOP);
    ////ugbtn_ok_->SetFont("かう");
    ////ugbtn_ok_->SetDepth(UIDP_07BUTTONTOP);
  }
  void TLyTuProfile::DoDown()
  {
    field_->g_SetDraw(FALSE);
    lybar_->CloseHeader();
	((TSnTutorial*)(pGenScene_))->lyTuHome_->ComebackProfile();
  }
  //顔アイコン作成
  void TLyTuProfile::MakeChest(s32 pageno, INTVECTOR dtvec)
  {
    switch (pageno)
    {
    case pg_Hair:
    case pg_Face:
      uglistFace_.RefreshFace(dtvec);
      break;
    case pg_SkinCol:
      uglistFace_.RefreshCol(dtvec, TRUE);
      break;
    case pg_HairCol:
    case pg_EyeCol:
    case pg_AccCol:
    case pg_ClotheCol:
      uglistFace_.RefreshCol(dtvec, FALSE);
      break;
    case pg_SkinCol2:
      uglistFace_.RefreshCol2(dtvec, TRUE);
      break;
    case pg_HairCol2:
    case pg_EyeCol2:
    case pg_AccCol2:
    case pg_ClotheCol2:
      uglistFace_.RefreshCol2(dtvec, FALSE);
      break;
    }

    s32 hair = stProfile_.hair_;
    s32 face = stProfile_.face_;
    s32 haircol = stProfile_.haircol_;
    s32 skincol = stProfile_.skincol_;
    s32 eyecol = stProfile_.eyecol_;
    s32 acccol = stProfile_.acccol_;
    s32 clothecol = stProfile_.clothecol_;
    s32 gb_f = stProfile_.gb_f_;

    for (s32 i = 0; i < DBSIDE; ++i)
    {
      for (s32 i2 = 0; i2 < DBMEMBER_ALL; ++i2)
      {
        s32 i3 = (i * DBMEMBER_ALL) + i2;

        if (i3 >= dtvec.size()) break;

        switch (pageno)
        {
        case pg_Hair: hair = dtvec[i3]; break;
        case pg_Face: face = dtvec[i3]; break;
        case pg_HairCol: haircol = dtvec[i3]; break;
        case pg_HairCol2: haircol = dtvec[i3]; break;
        case pg_SkinCol: skincol = dtvec[i3]; break;
        case pg_SkinCol2: skincol = dtvec[i3]; break;
        case pg_EyeCol: eyecol = dtvec[i3]; break;
        case pg_EyeCol2: eyecol = dtvec[i3]; break;
        case pg_AccCol: acccol = dtvec[i3]; break;
        case pg_AccCol2: acccol = dtvec[i3]; break;
        case pg_ClotheCol: clothecol = dtvec[i3]; break;
        case pg_ClotheCol2: clothecol = dtvec[i3]; break;
        }

        pmgEO_->mgGrp_.pmgChGrp_[i][i2]->SetCharDt(hair, face, haircol, skincol, eyecol, acccol, clothecol, gb_f);
      }
      pmgEO_->mgGrp_.MakeIconTexture_Chest(i, TRUE, FALSE);
    }

  }


  void TLyTuProfile::DrawList(TUGLiGen* drawlist)
  {
    for (s32 i = 0; i < listvec_.size(); ++i)
    {
      listvec_[i]->SetDraw(FALSE);
    }
    if (drawlist != NULL)
    {
      drawlist->SetDraw(TRUE);
      //drawlist->SetZeroPos();
    }
    ResetObj();
    ugbtn_back_prof_.SetDraw(TRUE);
  }
  void TLyTuProfile::ResetObj()
  {
    //ugbtn_ok_->SetDraw(FALSE);
  }

  //表示書き換え
  void TLyTuProfile::RefreshProf()
  {
    //表示書き換え
    ugprof_.SetProfMe();
    //変数更新（テクスチャ読み込み）
    SetMyAvtVar();
  }

  void TLyTuProfile::DoChangePage(s32 pageno)
  {
    const s32 NAMELEN = 6;
    const s32 PROFLEN = 13;
    const s32 HUENUM = 13;
    const s32 LUMNUM = 4;
    const s32 SKINHUENUM = 3 + 7;
    const s32 SKINLUMNUM = 3;

	dtvec_.clear();

	// 追加した顔ヅラ更新用ボタン
	// まずは消す
	ugbtn_mid_prof_.SetDraw(FALSE);

	f32 posX = 0;
	f32 posY = 0;
	f32 limY = 0;
	switch ((enPage)pageno)
    {
	case db::pg_Menu:
		DrawList(&uglistMenu_);
		uglistMenu_.records_[0]->SetupFinger(TRUE, 100.0f, 12.0f);
		break;
    case db::pg_ProfMenu:
      DrawList(&uglistProfMenu_);
	  uglistProfMenu_.records_[pml_Name]->SetupFinger(!isNameChange_, 100.0f, 12.0f);
	  uglistProfMenu_.records_[pml_Hair]->SetupFinger(!isHairChange_, 100.0f, 12.0f);
      break;
    case db::pg_Name:
      DrawList(NULL);
      frame_->SetHudInput(NAMELEN, FALSE, pmgEO_->mgFont_.CnvStrToMCVec(name_.c_str()));
      ugbtn_back_prof_.SetDraw(FALSE);
      break;
    case db::pg_Prof0:
      DrawList(NULL);
      frame_->SetHudInput(PROFLEN, FALSE, pmgEO_->mgFont_.CnvStrToMCVec(pmgEO_->mgCoU_.mdKantokuSub_.GetMyDtStr(mdkantokusub_prof0_c128).c_str()));
      ugbtn_back_prof_.SetDraw(FALSE);
      break;
    case db::pg_Prof1:
      DrawList(NULL);
      frame_->SetHudInput(PROFLEN, FALSE, pmgEO_->mgFont_.CnvStrToMCVec(pmgEO_->mgCoU_.mdKantokuSub_.GetMyDtStr(mdkantokusub_prof1_c128).c_str()));
      ugbtn_back_prof_.SetDraw(FALSE);
      break;
    case db::pg_Prof2:
      DrawList(NULL);
      frame_->SetHudInput(PROFLEN, FALSE, pmgEO_->mgFont_.CnvStrToMCVec(pmgEO_->mgCoU_.mdKantokuSub_.GetMyDtStr(mdkantokusub_prof2_c128).c_str()));
      ugbtn_back_prof_.SetDraw(FALSE);
      break;
    case db::pg_Hair:
      DrawList(&uglistFace_);
        for (s32 i = 0; i < DBSIDE; ++i)
        {
          for (s32 i2 = 0; i2 < DBMEMBER_ALL; ++i2)
          {
			while(1)
			{
				// ランダムでチョイス
				s32 charno = mid::midGetRandMn(pmgEO_->mgDt_.dtChar_.GetLineNum());
				// 出して良いキャラか判定
				//s32 charNo = dtvec_[uglistFace_.GetActIndex()];
				s32 ret = pmgEO_->mgDt_.dtChar_.GetLooks(clk_SearchFlag, charno);
				if (ret & sf_Hair )
				{
					dtvec_.push_back(charno);
					break;
				}
			}
          }
        }
		// 更新ボタン追加
		ugbtn_mid_prof_.SetFont("こうしん");
		ugbtn_mid_prof_.SetDraw(TRUE);
      break;
    case db::pg_Face:
      DrawList(&uglistFace_);
		for (s32 i = 0; i < DBSIDE; ++i)
		{
		  for (s32 i2 = 0; i2 < DBMEMBER_ALL; ++i2)
		  {
			while (1)
			{
				// ランダムでチョイス
				s32 charno = mid::midGetRandMn(pmgEO_->mgDt_.dtChar_.GetLineNum());
				// 出して良いキャラか判定
				//s32 charNo = dtvec_[uglistFace_.GetActIndex()];
				s32 ret = pmgEO_->mgDt_.dtChar_.GetLooks(clk_SearchFlag, charno);
				if (ret & sf_Face)
				{
					dtvec_.push_back(charno);
					break;
				}
			}
		  }
		}
		// 更新ボタン追加
		ugbtn_mid_prof_.SetFont("こうしん");
		ugbtn_mid_prof_.SetDraw(TRUE);
      break;
    case db::pg_HairCol:
      DrawList(&uglistFace_);
      for (s32 i = 0; i < HUENUM; ++i) dtvec_.push_back((i * 10) + 1);

      break;
    case db::pg_HairCol2:
      DrawList(&uglistFace_);
      for (s32 i = 0; i < LUMNUM; ++i)
      {
        dtvec_.push_back(stProfile_.hueno_ + i);
      }
      break;
    case db::pg_SkinCol:
      DrawList(&uglistFace_);
      for (s32 i = 0; i < SKINHUENUM; ++i) dtvec_.push_back((i * 10) + 1);
      break;
    case db::pg_SkinCol2:
      DrawList(&uglistFace_);
      for (s32 i = 0; i < SKINLUMNUM; ++i)
      {
        //人外カラーは二段階メイド
        if ((stProfile_.hueno_ > 20) && (i >= (SKINLUMNUM - 1))) break;
        dtvec_.push_back(stProfile_.hueno_ + i);
      }
      break;
    case db::pg_EyeCol:
      DrawList(&uglistFace_);
      for (s32 i = 0; i < HUENUM; ++i) dtvec_.push_back((i * 10) + 1);
      break;
    case db::pg_EyeCol2:
      DrawList(&uglistFace_);
      for (s32 i = 0; i < LUMNUM; ++i)
      {
        dtvec_.push_back(stProfile_.hueno_ + i);
      }
      break;
    case db::pg_AccCol:
      DrawList(&uglistFace_);
      for (s32 i = 0; i < HUENUM; ++i) dtvec_.push_back((i * 10) + 1);
      break;
    case db::pg_AccCol2:
      DrawList(&uglistFace_);
      for (s32 i = 0; i < LUMNUM; ++i)
      {
        dtvec_.push_back(stProfile_.hueno_ + i);
      }
      break;
    case db::pg_ClotheCol:
      DrawList(&uglistFace_);
      for (s32 i = 0; i < HUENUM; ++i) dtvec_.push_back((i * 10) + 1);
      break;
    case db::pg_ClotheCol2:
      DrawList(&uglistFace_);
      for (s32 i = 0; i < LUMNUM; ++i)
      {
        dtvec_.push_back(stProfile_.hueno_ + i);
      }
      break;
    }

    //キャラアイコン作成
    if (dtvec_.size() > 0) MakeChest(pageno, dtvec_);
  }

  //総合メニュー
  void TLyTuProfile::MvPage_Menu()
  {

	  if (ugbtn_back_prof_.IsAct())
	  {
		  Down();//もどる
	  }
	  else if (uglistMenu_.IsAct())
	  {
		  ChangePage(pg_ProfMenu);
	  }
  }
  void TLyTuProfile::MvPage_ProfMenu()
  {
	  if (ugbtn_back_prof_.IsAct())
	  {
		 // ChangePage(pg_Menu);//もどる
		  Down();//もどる
	  }
    else if (uglistProfMenu_.IsAct())
    {
      switch ((enProfMenuList)uglistProfMenu_.GetActIndex())
      {
      case db::pml_Name:
        ChangePage(pg_Name);
        break;
      case db::pml_Prof0:
        ChangePage(pg_Prof0);
        break;
      case db::pml_Prof1:
        ChangePage(pg_Prof1);
        break;
      case db::pml_Prof2:
        ChangePage(pg_Prof2);
        break;
      case db::pml_Hair:
        ChangePage(pg_Hair);
        break;
      case db::pml_Face:
        ChangePage(pg_Face);
        break;
      case db::pml_HairCol:
        ChangePage(pg_HairCol);
        break;
      case db::pml_SkinCol:
        ChangePage(pg_SkinCol);
        break;
      case db::pml_EyeCol:
        ChangePage(pg_EyeCol);
        break;
      case db::pml_AccCol:
        ChangePage(pg_AccCol);
        break;
      case db::pml_ClotheCol:
        ChangePage(pg_ClotheCol);
        break;
      }
    }
  }
  void TLyTuProfile::MvPage_Name()
  {
    if (frame_->IsHudCancel())
    {
      ChangePage(pg_ProfMenu);//もどる
    }
    else if (frame_->IsHudOK())//確定ボタンにかえる
    {
	  std::string newName = frame_->HudOutputString();
	  // 名前が変更された
	  if( newName.compare(name_) )
	  {
		  isNameChange_ = TRUE;
	  }
      name_ = newName;
      Post_Name();
    }
  }
  void TLyTuProfile::MvPage_Prof0()
  {
    if (frame_->IsHudCancel())
    {
      ChangePage(pg_ProfMenu);//もどる
    }
    else if (frame_->IsHudOK())
    {
      stProfile_.profline_ = 0;
      prof_ = frame_->HudOutputString();
      //POSTフレンドコード
      Post_Prof();
    }
  }
  void TLyTuProfile::MvPage_Prof1()
  {
    if (frame_->IsHudCancel())
    {
      ChangePage(pg_ProfMenu);//もどる
    }
    else if (frame_->IsHudOK())
    {
      stProfile_.profline_ = 1;
      prof_ = frame_->HudOutputString();
      Post_Prof();
    }
  }
  void TLyTuProfile::MvPage_Prof2()
  {
    if (frame_->IsHudCancel())
    {
      ChangePage(pg_ProfMenu);//もどる
    }
    else if (frame_->IsHudOK())
    {
      stProfile_.profline_ = 2;
      prof_ = frame_->HudOutputString();
      Post_Prof();
    }
  }
  void TLyTuProfile::MvPage_Hair()
  {
    if (ugbtn_back_prof_.IsAct())
    {
      ChangePage(pg_ProfMenu);//もどる
    }
    else if (uglistFace_.IsAct())
    {
	 s32 newHair = uglistFace_.GetCommonID();
	 if (newHair != stProfile_.hair_)
	 {
		 hairCharaNo_ = dtvec_[uglistFace_.GetActIndex()];
		 // ヅラが変わったのでフラグを立てる
		 isHairChange_ = TRUE;
	 }
      stProfile_.hair_ = newHair;
      Post_Avatar();
    }
	// 更新ボタンが押された
	else if (ugbtn_mid_prof_.IsAct())
	{
		// リスト更新
		ChangePage(db::pg_Hair);
	}
  }
  void TLyTuProfile::MvPage_Face()
  {
    if (ugbtn_back_prof_.IsAct())
    {
      ChangePage(pg_ProfMenu);//もどる
    }
    else if (uglistFace_.IsAct())
    {
      stProfile_.face_ = uglistFace_.GetCommonID();
	  faceCharaNo_ = dtvec_[uglistFace_.GetActIndex()];
      Post_Avatar();
    }
	// 更新ボタンが押された
	else if (ugbtn_mid_prof_.IsAct())
	{
		// リスト更新
		ChangePage(db::pg_Face);
	}
  }
  void TLyTuProfile::MvPage_Haircol()
  {
    if (ugbtn_back_prof_.IsAct())
    {
      ChangePage(pg_ProfMenu);//もどる
    }
    else if (uglistFace_.IsAct())
    {
      stProfile_.SetHueNo(uglistFace_.GetCommonID());
      ChangePage(pg_HairCol2);
    }
  }
  void TLyTuProfile::MvPage_Haircol2()
  {
    if (ugbtn_back_prof_.IsAct())
    {
      ChangePage(pg_HairCol);//もどる
    }
    else if (uglistFace_.IsAct())
    {
      stProfile_.haircol_ = uglistFace_.GetCommonID();
      Post_Avatar();
    }
  }
  void TLyTuProfile::MvPage_SkinCol()
  {
    if (ugbtn_back_prof_.IsAct())
    {
      ChangePage(pg_ProfMenu);//もどる
    }
    else if (uglistFace_.IsAct())
    {
      stProfile_.SetHueNo(uglistFace_.GetCommonID());
      ChangePage(pg_SkinCol2);
    }
  }
  void TLyTuProfile::MvPage_SkinCol2()
  {
    if (ugbtn_back_prof_.IsAct())
    {
      ChangePage(pg_SkinCol);//もどる
    }
    else if (uglistFace_.IsAct())
    {
      stProfile_.skincol_ = uglistFace_.GetCommonID();
      Post_Avatar();
    }
  }
  void TLyTuProfile::MvPage_EyeCol()
  {
    if (ugbtn_back_prof_.IsAct())
    {
      ChangePage(pg_ProfMenu);//もどる
    }
    else if (uglistFace_.IsAct())
    {
      stProfile_.SetHueNo(uglistFace_.GetCommonID());
      ChangePage(pg_EyeCol2);
    }
  }
  void TLyTuProfile::MvPage_EyeCol2()
  {
    if (ugbtn_back_prof_.IsAct())
    {
      ChangePage(pg_EyeCol);//もどる
    }
    else if (uglistFace_.IsAct())
    {
      stProfile_.eyecol_ = uglistFace_.GetCommonID();
      Post_Avatar();
    }
  }
  void TLyTuProfile::MvPage_AccCol()
  {
    if (ugbtn_back_prof_.IsAct())
    {
      ChangePage(pg_ProfMenu);//もどる
    }
    else if (uglistFace_.IsAct())
    {
      stProfile_.SetHueNo(uglistFace_.GetCommonID());
      ChangePage(pg_AccCol2);
    }
  }
  void TLyTuProfile::MvPage_AccCol2()
  {
    if (ugbtn_back_prof_.IsAct())
    {
      ChangePage(pg_AccCol);//もどる
    }
    else if (uglistFace_.IsAct())
    {
      stProfile_.acccol_ = uglistFace_.GetCommonID();
      Post_Avatar();
    }
  }
  void TLyTuProfile::MvPage_ClotheCol()
  {
    if (ugbtn_back_prof_.IsAct())
    {
      ChangePage(pg_ProfMenu);//もどる
    }
    else if (uglistFace_.IsAct())
    {
      stProfile_.SetHueNo(uglistFace_.GetCommonID());
      ChangePage(pg_ClotheCol2);
    }
  }
  void TLyTuProfile::MvPage_ClotheCol2()
  {
    if (ugbtn_back_prof_.IsAct())
    {
      ChangePage(pg_ClotheCol);//もどる
    }
    else if (uglistFace_.IsAct())
    {
      stProfile_.clothecol_ = uglistFace_.GetCommonID();
      Post_Avatar();
    }
  }
  void TLyTuProfile::MvPage_Out()
  {
  
  }

  ///////////////////////////////////////
  void TLyTuProfile::Post_Name()
  {
    if (pmgEO_->stSeq_.IsNoSv())
    {
      Connect_Name();
    }
    else
    {
      StartConnect(cn_Name);
      pmgEO_->mgPo_.poHome_.Ready_name(name_);
    }
  }
  void TLyTuProfile::Post_Prof()
  {
    if (pmgEO_->stSeq_.IsNoSv())
    {
      Connect_Name();
    }
    else
    {
      StartConnect(cn_Prof);
      pmgEO_->mgPo_.poHome_.Ready_prof(stProfile_.profline_, prof_);
    }
  }
  void TLyTuProfile::Post_Avatar()
  {
    if (pmgEO_->stSeq_.IsNoSv())
    {
      Connect_Name();
    }
    else
    {
		// もしヅラを変えていたら
		// ここのチュートリアルはクリアなので送信する
		if (isHairChange_)
		{
			// いしださんおねがいします。
		}

      StartConnect(cn_Avatar);
      pmgEO_->mgPo_.poHome_.Ready_avatar(stProfile_.hair_,
        stProfile_.face_,
        stProfile_.haircol_,
        stProfile_.skincol_,
        stProfile_.eyecol_,
        stProfile_.acccol_,
        stProfile_.clothecol_);
    }
  }

  //接続
  void TLyTuProfile::Connect_Name()
  {
    RefreshProf();
    ChangePage(pg_ProfMenu);
  }
  void TLyTuProfile::Connect_Prof()
  {
    RefreshProf();
    ChangePage(pg_ProfMenu);
  }
  void TLyTuProfile::Connect_Avatar()
  {
    RefreshProf();
    ChangePage(pg_ProfMenu);
  }

  //接続完了
  void TLyTuProfile::DoConnect(BOOL rqok_f)
  {
    if (rqok_f)//ＯＫのみ
    {
      if (pmgEO_->mgPo_.IsCodeZero())
      {
        switch (stLy_.connect_)
        {
        case cn_Name: Connect_Name(); break;
        case cn_Prof: Connect_Prof(); break;
        case cn_Avatar: Connect_Avatar(); break;
        }
      }
      else
      {
        ChangePage(pg_ProfMenu);
        //エラーメッセージ
        EndConnectNG(pmgEO_->mgPo_.GetCode(), pmgEO_->mgPo_.GetMsg().c_str());
      }
    }
    else
    {
      //通信エラー
		ChangePage(pg_ProfMenu);
      //エラーメッセージ
      EndConnectNG(pmgEO_->mgPo_.GetCode(), pmgEO_->mgPo_.GetMsg().c_str());
    }
  }

  void TLyTuProfile::DoUpdate(BOOL draw_f)
  {
    ugprof_.Update(draw_f);
#ifdef __K_DEBUG_PROFILE__
//    ugbtn_mid_prof_.Update(draw_f);
//    ugbtn_next_->Update(draw_f);
//    ugbtn_prev_->Update(draw_f);
#endif	// #ifdef __K_DEBUG_PROFILE__
    uglistCol_.Update(draw_f);
    uglistFace_.Update(draw_f);
    uglistMenu_.Update(draw_f);
    uglistProfMenu_.Update(draw_f);

    ugbtn_back_prof_.Update(draw_f);
    ugbtn_mid_prof_.Update(draw_f);

    if (draw_f == FALSE)
    {
      switch ((enPage)stLy_.pageNo_)
      {
	  case db::pg_Menu:MvPage_Menu(); break;
      case db::pg_ProfMenu:MvPage_ProfMenu(); break;
      case db::pg_Name:MvPage_Name(); break;
      case db::pg_Prof0:MvPage_Prof0(); break;
      case db::pg_Prof1:MvPage_Prof1(); break;
      case db::pg_Prof2:MvPage_Prof2(); break;
      case db::pg_Hair:MvPage_Hair(); break;
      case db::pg_Face:MvPage_Face(); break;
      case db::pg_HairCol:MvPage_Haircol(); break;
      case db::pg_HairCol2:MvPage_Haircol2(); break;
      case db::pg_SkinCol:MvPage_SkinCol(); break;
      case db::pg_SkinCol2:MvPage_SkinCol2(); break;
      case db::pg_EyeCol:MvPage_EyeCol(); break;
      case db::pg_EyeCol2:MvPage_EyeCol2(); break;
      case db::pg_AccCol:MvPage_AccCol(); break;
      case db::pg_AccCol2:MvPage_AccCol2(); break;
      case db::pg_ClotheCol:MvPage_ClotheCol(); break;
      case db::pg_ClotheCol2:MvPage_ClotheCol2(); break;
      }

    }
  }

}