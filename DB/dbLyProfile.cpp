#include "dbLyProfile.h"
#include "dbLyBarGen.h"

namespace db
{
  enum enRoomList
  {
    ml_Prof,
    ml_Rank,
    ml_RankResult,
    mlEND,
  };

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

  enum enRankMenuList
  {
    rml_rank0,
    rml_rank1,
    rmlEND,
  };

  enum enPage
  {
    pg_Menu,
    pg_ProfMenu,
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
    pg_RankMenu,
    pg_Ranker,
    pg_MyRank,
    pgEND,
  };

  enum enConnect
  {
    cn_Name,
    cn_Prof,
    cn_Avatar,
    cn_Ranker,
    cn_MyRank,
  };



#ifdef __K_DEBUG_PROFILE__
  //---------------------------------------------------
  // ローカル環境なのをいいことにグローバル変数定義しまくる
  static s32 hairPageNo_ = 0;	// ヅラページ数
  static s32 facePageNo_ = 0;	// 顔ページ数

  // わかりづらいので定数用意
  enum
  {
    SORT_TYPE_MIN = 0,

    SORT_TYPE_DEFAULT = SORT_TYPE_MIN,	// ランダム
    SORT_TYPE_UNIQUE_ID,					// ユニークＩＤ
    SORT_TYPE_FACEHAIR,					// 顔ヅラＩＤ

    SORT_TYPE_MAX,
  };
  static s32 sortType_ = SORT_TYPE_DEFAULT;	// デフォはランダム
  //---------------------------------------------------
#endif	// #ifdef __K_DEBUG_PROFILE__



  //コンストラクタ
  TLyProfile::TLyProfile(TScene* pScene, TUIFrame* frame, TLyBarGen* lybar) :TUILayer(pScene, frame)
    , ugprof_(this)
    , uglistMenu_(this)
    , uglistProfMenu_(this)
    , uglistRankMenu_(this)
    , uglistRanker_(this)
    , uglistMyRank_(this)
    , uglistFace_(this)
    , uglistCol_(this)
    ,ugbtn_back_prof_(this)
    //, ugbtn_ok_prof_(this)
    , ugbtn_mid_prof_(this)
    , ugbtn_next_prof_(this)
    , ugbtn_prev_prof_(this)
  {
    lybar_ = lybar;
    stProfile_.Init();
    stLy_.hedderlayer_f_ = TRUE;//ヘッダ用レイヤ
  }

  TLyProfile::~TLyProfile()
  {

  }
  void TLyProfile::DoLoad()
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

    uglistRankMenu_.MakeGroup(field_, FALSE);
    uglistRankMenu_.SetDepth(UIDP_07BUTTONTOP);

    uglistRanker_.MakeGroup(field_, FALSE);
    uglistRanker_.SetDepth(UIDP_07BUTTONTOP);

    uglistMyRank_.MakeGroup(field_, FALSE);
    uglistMyRank_.SetDepth(UIDP_07BUTTONTOP);

    uglistCol_.MakeGroup(field_, FALSE);
    uglistCol_.SetDepth(UIDP_07BUTTONTOP);

    uglistFace_.MakeGroup(field_, FALSE);
    uglistFace_.SetDepth(UIDP_07BUTTONTOP);

    listvec_.clear();
    listvec_.push_back(&uglistMenu_);
    listvec_.push_back(&uglistProfMenu_);
    listvec_.push_back(&uglistRankMenu_);
    listvec_.push_back(&uglistRanker_);
    listvec_.push_back(&uglistMyRank_);
    listvec_.push_back(&uglistCol_);
    listvec_.push_back(&uglistFace_);


    ugbtn_back_prof_.MakeGroup_FtPos(field_, 0, "もどる");
    //ugbtn_ok_prof_.MakeGroup_FtPos(field_, 1, "もどる");
    ugbtn_mid_prof_.MakeGroup_FtPos(field_, 2, "こうしん");
    ugbtn_next_prof_.MakeGroup_FtPos(field_, 3, "つぎのページ");
    ugbtn_prev_prof_.MakeGroup_FtPos(field_, 4, "まえのページ");
    ugbtn_back_prof_.SetDepth(UIDP_09MSGBOX);
    //ugbtn_ok_prof_.SetDepth(UIDP_09MSGBOX);
    //ugbtn_mid_prof_.SetDepth(UIDP_09MSGBOX);
    //ugbtn_next_prof_.SetDepth(UIDP_09MSGBOX);
    //ugbtn_prev_prof_.SetDepth(UIDP_09MSGBOX);

    //メニューレコード
    for (s32 i = 0; i < mlEND; ++i)
    {
      TUGRcNormal* newrecord = (TUGRcNormal*)uglistMenu_.MakeObj_Record();
      switch (i)
      {
      case db::ml_Prof:newrecord->ugname_.SetNameIcon("プロフィール", ugi_Prof);  break;
      case db::ml_Rank:newrecord->ugname_.SetNameIcon("ランキング", ugi_Ranking);  break;
      case db::ml_RankResult:newrecord->ugname_.SetNameIcon("ランキングけっか", ugi_Rank);  break;
      }
    }


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
  }

  void TLyProfile::SetMyAvtVar()
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


  //入店時の処理
  void TLyProfile::DoReady()
  {
    stProfile_.Init();
    field_->g_SetDraw(TRUE);
    SetMyAvtVar();

  }
  void TLyProfile::DoDown()
  {
    field_->g_SetDraw(FALSE);
    lybar_->CloseHeader();
  }
  //顔アイコン作成
  void TLyProfile::MakeChest(s32 pageno, INTVECTOR dtvec)
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


  void TLyProfile::DrawList(TUGLiGen* drawlist)
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
    ugbtn_prev_prof_.SetDraw(FALSE);
    ugbtn_next_prof_.SetDraw(FALSE);
  }
  void TLyProfile::ResetObj()
  {
    //ugbtn_ok_->SetDraw(FALSE);
  }

  //表示書き換え
  void TLyProfile::RefreshProf()
  {
    //表示書き換え
    ugprof_.SetProfMe();
    //変数更新（テクスチャ読み込み）
    SetMyAvtVar();
  }

  void TLyProfile::DoChangePage(s32 pageno)
  {
    const s32 NAMELEN = 6;
    const s32 PROFLEN = 13;
    const s32 HUENUM = 13;
    const s32 LUMNUM = 4;
    const s32 SKINHUENUM = 3 + 7;
    const s32 SKINLUMNUM = 3;

	dtvec_.clear();

    //---------------------------------------------------
#ifdef __K_DEBUG_PROFILE__
    // 顔ヅラでなければ更新ボタン等表示しない
    if ((enPage)pageno == db::pg_Hair ||
      (enPage)pageno == db::pg_Face)
    {
		ugbtn_prev_prof_.SetFont("まえへ");
		ugbtn_prev_prof_.SetDraw(TRUE);
		ugbtn_next_prof_.SetFont("つぎへ");
		ugbtn_next_prof_.SetDraw(TRUE);
		ugbtn_mid_prof_.SetFont("もーど");
		ugbtn_mid_prof_.SetDraw(TRUE);
    }
    else
    {
		ugbtn_prev_prof_.SetDraw(FALSE);
		ugbtn_next_prof_.SetDraw(FALSE);
		ugbtn_mid_prof_.SetDraw(FALSE);
	}
#endif	// #ifdef __K_DEBUG_PROFILE__
    //---------------------------------------------------

	// 追加した顔ヅラ更新用ボタン
	// まずは消す
	ugbtn_mid_prof_.SetDraw(FALSE);

    switch ((enPage)pageno)
    {
    case db::pg_Menu:
      DrawList(&uglistMenu_);
      
      break;
    case db::pg_ProfMenu:
      DrawList(&uglistProfMenu_);
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
      //--------------------------------------------------------------------------------------------------------]
#ifdef __K_DEBUG_PROFILE__
        // ランダムモード時はランダムシード処理はやる
        if (sortType_ == SORT_TYPE_DEFAULT)
        {
          //mid::midGetRandMnSeedReset(pmgEO_->mgCoU_.mdKantoku_.GetMyDt(mdkantoku_logbo_date));
          //mid::midGetRandMnSeedReset();
        }
        for (s32 i = 0; i < DBSIDE; ++i)
        {
          for (s32 i2 = 0; i2 < DBMEMBER_ALL; ++i2)
          {
            switch (sortType_)
            {
                //-----------------------------------------------------------------------------
                // ランダム方式
              case SORT_TYPE_DEFAULT:
              {
                // ランダムでチョイス
                s32 charno = mid::midGetRandMn(pmgEO_->mgDt_.dtChar_.GetLineNum());
                dtvec_.push_back(charno);
              }
                break;
                
                //-----------------------------------------------------------------------------
                // ユニークID方式
              case SORT_TYPE_UNIQUE_ID:
              {
                // チーム・キャラ・シュート.xlsm
                // キャラシートの論理名列のＩＤ順にする
                s32 no = hairPageNo_ * (DBSIDE * DBMEMBER_ALL) + (DBMEMBER_ALL*i) + i2;
                s32 max = pmgEO_->mgDt_.dtChar_.GetLineNum();
                if (no >= max) break;
                dtvec_.push_back(no);
              }
                break;
                
                //-----------------------------------------------------------------------------
                // 顔ヅラID方式
              case SORT_TYPE_FACEHAIR:
              {
                // 顔番号.xls
                // 各キャラの顔ヅラＩＤ順にする
                s32 no = hairPageNo_ * (DBSIDE * DBMEMBER_ALL) + (DBMEMBER_ALL*i) + i2;
                s32 max = pmgEO_->mgDt_.dtChar_.GetLineNum();
                if (no >= max) break;
                
                // ユニーク方式のテーブル（1765個くらい？）を頭からなめていって
                // 自分の探している顔ヅラ番号を使ってるキャラを探す
                // この処理であってるかわからないけど、絵的にちゃんとしてるので大丈夫そう・・・？
                for (s32 j = 0; j < max; j++)
                {
                  if (no != pmgEO_->mgDt_.dtChar_.GetLooks(db::clk_ZuraNo, j)) continue;
                  
                  dtvec_.push_back(j);
                  break;
                }
              }
                break;
            }
          }
        }
#else
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
				  if (ret & sf_Hair)
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
#endif	// #ifdef __K_DEBUG_PROFILE__
      break;
    case db::pg_Face:
      DrawList(&uglistFace_);

#ifdef __K_DEBUG_PROFILE__
        // ランダムモード時はランダムシード処理はやる
        if (sortType_ == SORT_TYPE_DEFAULT)
        {
          //mid::midGetRandMnSeedReset(pmgEO_->mgCoU_.mdKantoku_.GetMyDt(mdkantoku_logbo_date));
          //mid::midGetRandMnSeedReset();
        }
        for (s32 i = 0; i < DBSIDE; ++i)
        {
          for (s32 i2 = 0; i2 < DBMEMBER_ALL; ++i2)
          {
            switch (sortType_)
            {
                //-----------------------------------------------------------------------------
                // ランダム方式
              case SORT_TYPE_DEFAULT:
              {
                // ランダムでチョイス
                s32 charno = mid::midGetRandMn(pmgEO_->mgDt_.dtChar_.GetLineNum());
                dtvec_.push_back(charno);
              }
                break;
                
                //-----------------------------------------------------------------------------
                // ユニークID方式
              case SORT_TYPE_UNIQUE_ID:
              {
                // チーム・キャラ・シュート.xlsm
                // キャラシートの論理名列のＩＤ順にする
                s32 no = facePageNo_ * (DBSIDE * DBMEMBER_ALL) + (DBMEMBER_ALL*i) + i2;
                s32 max = pmgEO_->mgDt_.dtChar_.GetLineNum();
                if (no >= max) break;
                dtvec_.push_back(no);
              }
                break;
                
                //-----------------------------------------------------------------------------
                // 顔ヅラID方式
              case SORT_TYPE_FACEHAIR:
              {
                // 顔番号.xls
                // 各キャラの顔ヅラＩＤ順にする
                s32 no = facePageNo_ * (DBSIDE * DBMEMBER_ALL) + (DBMEMBER_ALL*i) + i2;
                s32 max = pmgEO_->mgDt_.dtChar_.GetLineNum();
                if (no >= max) break;
                
                // ユニーク方式のテーブル（1765個くらい？）を頭からなめていって
                // 自分の探している顔ヅラ番号を使ってるキャラを探す
                // この処理であってるかわからないけど、絵的にちゃんとしてるので大丈夫そう・・・？
                for (s32 j = 0; j < max; j++)
                {
                  if (no != pmgEO_->mgDt_.dtChar_.GetLooks(db::clk_FaceNo, j)) continue;
                  
                  dtvec_.push_back(j);
                  break;
                }
              }
                break;
            }
          }
        }
        
        //--------------------------------------------------------------------------------------------------------
#else
        //mid::midGetRandMnSeedReset(pmgEO_->mgCoU_.mdKantoku_.GetMyDt(mdkantoku_logbo_date));
        //mid::midGetRandMnSeedReset();
    
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

#endif	// #ifdef __K_DEBUG_PROFILE__
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

    case db::pg_RankMenu:
      DrawList(&uglistRankMenu_);
      uglistRankMenu_.RefreshAllEvent(stProfile_.rankresult_f_);
      break;

    case db::pg_Ranker:
      DrawList(&uglistRanker_);
      pmgEO_->mgCoU_.MakeChestRanker();//ランカーアイコン作成
      if (stProfile_.rankingpage_ == 0)
      {
		  uglistRanker_.Refresh(TRUE, stProfile_.mid_ranking_);
        ugprof_.SetProfMe();
      }
      else
      {
		  uglistRanker_.Refresh(FALSE, stProfile_.mid_ranking_);
      }
      SwitchRankerPageBtn();
      break;
    case db::pg_MyRank:
      DrawList(&uglistMyRank_);
      pmgEO_->mgCoU_.MakeChestRanker();//ランカーアイコン作成
	  uglistMyRank_.Refresh(FALSE, stProfile_.mid_ranking_);
      break;
    }

    //キャラアイコン作成
    if (dtvec_.size() > 0) MakeChest(pageno, dtvec_);
  }

  //総合メニュー
  void TLyProfile::MvPage_Menu()
  {
	  if (frame_->ugfooter_->IsHideBtn() == FALSE) frame_->ugfooter_->HideBtn(TRUE);

    if (ugbtn_back_prof_.IsAct())
    {
      Down();//もどる
    }
    else if (uglistMenu_.IsAct())
    {
      switch ((enRoomList)uglistMenu_.GetActIndex())
      {
      case db::ml_Prof:
        ChangePage(pg_ProfMenu);
        break;
      case db::ml_Rank:
        stProfile_.rankresult_f_ = FALSE;
        ChangePage(pg_RankMenu);
        break;
      case db::ml_RankResult:
        stProfile_.rankresult_f_ = TRUE;
        ChangePage(pg_RankMenu);
        break;
      }
    }
  }
  void TLyProfile::MvPage_ProfMenu()
  {
    if (ugbtn_back_prof_.IsAct())
    {
      ChangePage(pg_Menu);//もどる
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
  void TLyProfile::MvPage_Name()
  {
    if (frame_->IsHudCancel())
    {
      ChangePage(pg_ProfMenu);//もどる
    }
    else if (frame_->IsHudOK())//確定ボタンにかえる
    {
      name_ = frame_->HudOutputString();
      Post_Name();
    }
  }
  void TLyProfile::MvPage_Prof0()
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
  void TLyProfile::MvPage_Prof1()
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
  void TLyProfile::MvPage_Prof2()
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
  void TLyProfile::MvPage_Hair()
  {
    if (ugbtn_back_prof_.IsAct())
    {
      ChangePage(pg_ProfMenu);//もどる
    }
#ifdef __K_DEBUG_PROFILE__
    // きりかえボタン押された
    else if (ugbtn_mid_prof_.IsAct())
    {
      // リスト表示形式変更
      sortType_++;
      if (sortType_ >= SORT_TYPE_MAX) sortType_ = SORT_TYPE_MIN;

      // なんかあったらめんどうなのでページ数リセット
      hairPageNo_ = 0;
      facePageNo_ = 0;
      ChangePage(db::pg_Hair);
    }
	//-----------------------------------------------------------------------------------------
	// まえのぺーじぼたん押された
	else if (ugbtn_prev_prof_.IsAct())
	{
		--hairPageNo_;
		if (sortType_ == SORT_TYPE_UNIQUE_ID)
		{
			// ページ数の計算
			s32 maxPageNo = pmgEO_->mgDt_.dtChar_.GetLineNum() / (DBSIDE * DBMEMBER_ALL);
			if (hairPageNo_ < 0) hairPageNo_ = maxPageNo;
		}
		else if (sortType_ == SORT_TYPE_FACEHAIR)
		{
			s32 max = -1;
			// ものすごくばからしいけど全キャラの中で一番大きいＩＤを探して
			// それを基準にページ数を決める
			// ※※※ あとでいしださんに顔ヅラ数の最大値取れないか聞いておくこと ※※※
			for (s32 i = 0; i < pmgEO_->mgDt_.dtChar_.GetLineNum(); i++)
			{
				s32 tmpNo = pmgEO_->mgDt_.dtChar_.GetLooks(db::clk_ZuraNo, i);
				if (tmpNo > max)
				{
					max = tmpNo;
				}
			}
			s32 maxPageNo = max / (DBSIDE * DBMEMBER_ALL);
			if (hairPageNo_ < 0) hairPageNo_ = maxPageNo;
		}

		// リスト更新
		ChangePage(db::pg_Hair);
	}
	//-----------------------------------------------------------------------------------------
	// つぎのぺーじぼたんが押された
	else if (ugbtn_next_prof_.IsAct())
	{
		++hairPageNo_;
		if (sortType_ == SORT_TYPE_UNIQUE_ID)
		{
			// ページ数の計算
			s32 maxPageNo = pmgEO_->mgDt_.dtChar_.GetLineNum() / (DBSIDE * DBMEMBER_ALL);
			if (hairPageNo_ > maxPageNo) hairPageNo_ = 0;
		}
		else if (sortType_ == SORT_TYPE_FACEHAIR)
		{
			s32 max = -1;
			// ものすごくばからしいけど全キャラの中で一番大きいＩＤを探して
			// それを基準にページ数を決める
			// ※※※ あとでいしださんに顔ヅラ数の最大値取れないか聞いておくこと ※※※
			for (s32 i = 0; i < pmgEO_->mgDt_.dtChar_.GetLineNum(); i++)
			{
				s32 tmpNo = pmgEO_->mgDt_.dtChar_.GetLooks(db::clk_ZuraNo, i);
				if (tmpNo > max)
				{
					max = tmpNo;
				}
			}
			s32 maxPageNo = max / (DBSIDE * DBMEMBER_ALL);
			if (hairPageNo_ > maxPageNo) hairPageNo_ = 0;
		}

		// リスト更新
		ChangePage(db::pg_Hair);
	}
#endif	// #ifdef __K_DEBUG_PROFILE__
    else if (uglistFace_.IsAct())
    {
      stProfile_.hair_ = uglistFace_.GetCommonID();
      Post_Avatar();
    }
	// 更新ボタンが押された
	else if(ugbtn_mid_prof_.IsAct())
	{
		// リスト更新
		ChangePage(db::pg_Hair);
	}
  }
  void TLyProfile::MvPage_Face()
  {
    if (ugbtn_back_prof_.IsAct())
    {
      ChangePage(pg_ProfMenu);//もどる
    }
#ifdef __K_DEBUG_PROFILE__
    // きりかえボタン押された
    else if (ugbtn_mid_prof_.IsAct())
    {
      // リスト表示形式変更
      sortType_++;
      if (sortType_ >= SORT_TYPE_MAX) sortType_ = SORT_TYPE_MIN;

      // なんかあったらめんどうなのでページ数リセット
      hairPageNo_ = 0;
      facePageNo_ = 0;
      ChangePage(db::pg_Face);
    }
	//-----------------------------------------------------------------------------------------
	// まえのぺーじぼたん押された
	else if (ugbtn_prev_prof_.IsAct())
	{
		--facePageNo_;
		if (sortType_ == SORT_TYPE_UNIQUE_ID)
		{
			// ページ数の計算
			s32 maxPageNo = pmgEO_->mgDt_.dtChar_.GetLineNum() / (DBSIDE * DBMEMBER_ALL);
			if (facePageNo_ < 0) facePageNo_ = maxPageNo;
		}
		else if (sortType_ == SORT_TYPE_FACEHAIR)
		{
			s32 max = -1;
			// ものすごくばからしいけど全キャラの中で一番大きいＩＤを探して
			// それを基準にページ数を決める
			for (s32 i = 0; i < pmgEO_->mgDt_.dtChar_.GetLineNum(); i++)
			{
				s32 tmpNo = pmgEO_->mgDt_.dtChar_.GetLooks(db::clk_FaceNo, i);
				if (tmpNo > max)
				{
					max = tmpNo;
				}
			}
			s32 maxPageNo = max / (DBSIDE * DBMEMBER_ALL);
			if (facePageNo_ < 0) facePageNo_ = maxPageNo;
		}
		// リスト更新
		ChangePage(db::pg_Face);
	}
	//-----------------------------------------------------------------------------------------
	// つぎのぺーじぼたんが押された
	else if (ugbtn_next_prof_.IsAct())
	{
		++facePageNo_;
		if (sortType_ == SORT_TYPE_UNIQUE_ID)
		{
			// ページ数の計算
			s32 maxPageNo = pmgEO_->mgDt_.dtChar_.GetLineNum() / (DBSIDE * DBMEMBER_ALL);
			if (facePageNo_ > maxPageNo) facePageNo_ = 0;
		}
		else if (sortType_ == SORT_TYPE_FACEHAIR)
		{
			s32 max = -1;
			// ものすごくばからしいけど全キャラの中で一番大きいＩＤを探して
			// それを基準にページ数を決める
			for (s32 i = 0; i < pmgEO_->mgDt_.dtChar_.GetLineNum(); i++)
			{
				s32 tmpNo = pmgEO_->mgDt_.dtChar_.GetLooks(db::clk_FaceNo, i);
				if (tmpNo > max)
				{
					max = tmpNo;
				}
			}
			s32 maxPageNo = max / (DBSIDE * DBMEMBER_ALL);
			if (facePageNo_ > maxPageNo) facePageNo_ = 0;
		}
		// リスト更新
		ChangePage(db::pg_Face);
	}
#endif	// #ifdef __K_DEBUG_PROFILE__
    else if (uglistFace_.IsAct())
    {
      stProfile_.face_ = uglistFace_.GetCommonID();
      Post_Avatar();
    }
	// 更新ボタンが押された
	else if (ugbtn_mid_prof_.IsAct())
	{
		// リスト更新
		ChangePage(db::pg_Face);
	}
  }
  void TLyProfile::MvPage_Haircol()
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
  void TLyProfile::MvPage_Haircol2()
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
  void TLyProfile::MvPage_SkinCol()
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
  void TLyProfile::MvPage_SkinCol2()
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
  void TLyProfile::MvPage_EyeCol()
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
  void TLyProfile::MvPage_EyeCol2()
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
  void TLyProfile::MvPage_AccCol()
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
  void TLyProfile::MvPage_AccCol2()
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
  void TLyProfile::MvPage_ClotheCol()
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
  void TLyProfile::MvPage_ClotheCol2()
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

  //ランキング用ページ  
  void TLyProfile::MvPage_RankMenu()
  {
    if (ugbtn_back_prof_.IsAct())
    {
      //メニューに戻る
      ChangePage(pg_Menu);//もどる
    }
    else if (uglistRankMenu_.IsAct())
    {
      stProfile_.mid_ranking_ = uglistRankMenu_.GetCommonID();
      Post_Ranker();
    }
  }
  void TLyProfile::MvPage_Ranker()
  {
    if (ugbtn_back_prof_.IsAct())
    {
      //自分に表示を戻す
      RefreshProf();
      //ランクメニューに戻る
      ChangePage(pg_RankMenu);
    }
    else if (uglistRanker_.IsAct())
    {
      //一行目が押されたら自分付近ランク表示、それ以外は上位ランク者表示
      if ((stProfile_.rankingpage_ == 0) && (uglistRanker_.GetActIndex() == 0))
      {
        Post_MyRank();
      }
      else if (uglistRanker_.GetCommonID() != NGNUM)
      {
        //表示書き換え//変数更新（テクスチャ読み込み）
        ugprof_.SetProfRanker(uglistRanker_.GetCommonID());
      }
    }
    else if (ugbtn_prev_prof_.IsAct() && (stProfile_.rankingpage_ > 0))
    {
      --stProfile_.rankingpage_;
      if (stProfile_.rankingpage_ == 0)
      {
        Post_Ranker();
      }
      else
      {
        Post_RankerPage();
      }
    }
    else if ((ugbtn_next_prof_.IsAct() && (stProfile_.rankingpage_ < 9)))
    {
      ++stProfile_.rankingpage_;
      Post_RankerPage();
    }

  }
  void TLyProfile::SwitchRankerPageBtn()
  {
    ugbtn_prev_prof_.SetFont("まえのページ");
    ugbtn_next_prof_.SetFont("つぎのページ");
    ugbtn_prev_prof_.SetDraw((stProfile_.rankingpage_ > 0));
    ugbtn_next_prof_.SetDraw((stProfile_.rankingpage_ < 9));
  }
  void TLyProfile::MvPage_MyRank()
  {
    if (ugbtn_back_prof_.IsAct())
    {
      //ランカー表示
      Post_Ranker();
    }
    else if (uglistMyRank_.IsAct())
    {
      //表示書き換え//変数更新（テクスチャ読み込み）
      ugprof_.SetProfRanker(uglistMyRank_.GetCommonID());
    }
  }
  void TLyProfile::MvPage_Out()
  {
  
  }

  ///////////////////////////////////////
  void TLyProfile::Post_Name()
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
  void TLyProfile::Post_Prof()
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
  void TLyProfile::Post_Avatar()
  {
#ifdef __K_DEBUG_PROFILE__
	  // ネットワークが関係しているので
	  // ローカル環境だとアバターが反映されない
	  // だからここに勝手に書いてしまう！
	  s32 hair = stProfile_.hair_;
	  s32 face = stProfile_.face_;
	  s32 haircol = stProfile_.haircol_;
	  s32 skincol = stProfile_.skincol_;
	  s32 eyecol = stProfile_.eyecol_;
	  s32 acccol = stProfile_.acccol_;
	  s32 clothecol = stProfile_.clothecol_;
	  pmgEO_->mgGrp_.pmgcg_kantoku_->SetCharDt(hair, face, haircol, skincol, eyecol, acccol, clothecol, FALSE);

	  pmgEO_->mgGrp_.MakeKantokuTexture();
	  pmgEO_->mgGrp_.pmgcg_kantoku_->SetGrTile(
		  pmgEO_->mgGrp_.pgr_Kantoku_,
		  pmgEO_->mgGrp_.pgr_KantokuFace_,
		  pmgEO_->mgGrp_.pgr_KantokuHair_,
		  &pmgEO_->mgDt_.dtCell_kantoku_);
#else
    if (pmgEO_->stSeq_.IsNoSv())
    {
      Connect_Name();
    }
    else
    {
      StartConnect(cn_Avatar);
      pmgEO_->mgPo_.poHome_.Ready_avatar(stProfile_.hair_,
        stProfile_.face_,
        stProfile_.haircol_,
        stProfile_.skincol_,
        stProfile_.eyecol_,
        stProfile_.acccol_,
        stProfile_.clothecol_);
    }
#endif	//#ifdef __K_DEBUG_PROFILE__
  }
  void TLyProfile::Post_Ranker()
  {
    StartConnect(cn_Ranker);
    //通信はページの更新と変わらない
    pmgEO_->mgPo_.poHome_.Ready_ranker(stProfile_.mid_ranking_, stProfile_.rankresult_f_);

    if (pmgEO_->stSeq_.IsNoSv()) Connect_Ranker();
  }
  //POSTランカーpage
  void TLyProfile::Post_RankerPage()
  {
    StartConnect(cn_Ranker);
    //通信はページの更新と変わらない
    pmgEO_->mgPo_.poHome_.Ready_rankerpage(stProfile_.mid_ranking_, stProfile_.rankresult_f_, stProfile_.rankingpage_);
    if (pmgEO_->stSeq_.IsNoSv()) Connect_Ranker();
  }
  void TLyProfile::Post_MyRank()
  {
    StartConnect(cn_MyRank);
    //通信はページの更新と変わらない
    pmgEO_->mgPo_.poHome_.Ready_myrank(stProfile_.mid_ranking_, stProfile_.rankresult_f_);

    if (pmgEO_->stSeq_.IsNoSv()) Connect_MyRank();
  }

  //接続
  void TLyProfile::Connect_Name()
  {
    RefreshProf();
    ChangePage(pg_ProfMenu);
  }
  void TLyProfile::Connect_Prof()
  {
    RefreshProf();
    ChangePage(pg_ProfMenu);
  }
  void TLyProfile::Connect_Avatar()
  {
    RefreshProf();
    ChangePage(pg_ProfMenu);
  }
  void TLyProfile::Connect_Ranker()
  {
    ChangePage(pg_Ranker);
  }
  void TLyProfile::Connect_RankerPage()
  {
    ChangePage(pg_Ranker);
  }
  void TLyProfile::Connect_MyRank()
  {
    ChangePage(pg_MyRank);
  }

  //接続完了
  void TLyProfile::DoConnect(BOOL rqok_f)
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
        case cn_Ranker: Connect_Ranker(); break;
        case cn_MyRank: Connect_MyRank(); break;
        }
      }
      else
      {
        ChangePage(pg_Menu);
        //エラーメッセージ
        EndConnectNG(pmgEO_->mgPo_.GetCode(), pmgEO_->mgPo_.GetMsg().c_str());
      }
    }
    else
    {
      //通信エラー
      ChangePage(pg_Menu);
      //エラーメッセージ
      EndConnectNG(pmgEO_->mgPo_.GetCode(), pmgEO_->mgPo_.GetMsg().c_str());
    }
  }

  void TLyProfile::DoUpdate(BOOL draw_f)
  {
    ugprof_.Update(draw_f);
#ifdef __K_DEBUG_PROFILE__
//    ugbtn_mid_prof_.Update(draw_f);
//    ugbtn_next_prof_.Update(draw_f);
//    ugbtn_prev_prof_.Update(draw_f);
#endif	// #ifdef __K_DEBUG_PROFILE__
    uglistCol_.Update(draw_f);
    uglistFace_.Update(draw_f);
    uglistMenu_.Update(draw_f);
    uglistProfMenu_.Update(draw_f);
    uglistRankMenu_.Update(draw_f);
    uglistRanker_.Update(draw_f);
    uglistMyRank_.Update(draw_f);

    //ugbtn_tsh_tec_.Update(draw_f);
    ugbtn_back_prof_.Update(draw_f);
    //ugbtn_ok_tec_.Update(draw_f);
    ugbtn_mid_prof_.Update(draw_f);
    ugbtn_next_prof_.Update(draw_f);
    ugbtn_prev_prof_.Update(draw_f);


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
      case db::pg_RankMenu:MvPage_RankMenu(); break;
      case db::pg_Ranker:MvPage_Ranker(); break;
      case db::pg_MyRank:MvPage_MyRank(); break;
      }

    }
  }

}