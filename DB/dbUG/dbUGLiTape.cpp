#include "dbUGLiTape.h"
#include "dbUILayer.h"
#include "dbUGRcNormal.h"


namespace db
{
  //コンストラクタ
  TUGLiTape::TUGLiTape(TUIGenLayer* player) :TUGLiGen(player)
  {
    stLiTape_.Init();
  }

  TUGLiTape::~TUGLiTape()
  {
    //解放時必ず呼ぶ
    ClearRecord();
  }


  void TUGLiTape::DoMakeGroupSub()
  {
    //ugbtn_ok_->SetFont("ホームＢＧＭ", FC2_normal);
    //ugbtn_ok_->SetDraw(TRUE);

  }

  //リストクリア（変数部分的に初期化、レコードとレコードエンド解放）
  void TUGLiTape::DoClearRecord()
  {
    stLiTape_.Init();
    for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);
    records_.clear();
  }

  TUGRcNormal* TUGLiTape::MakeObj_Record()
  {
    TUGRcNormal* newrecord = new TUGRcNormal(player_);
    s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
    records_.push_back(newrecord);
    GenMakeObj_Record(newrecord, rh);

    return newrecord;
  }

  void TUGLiTape::Refresh(s64 titleid)
  {
    ClearRecord();

    SetDraw(TRUE);

    s32 homebgm = pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_id_m_bgm);

    //ＢＧＭ名を読み込む(i==mid_bgm)
    for (s32 i = 0; i < pmgEO_->mgDt_.dtBGM_.GetLineNum(); ++i)
    {
      //タイトルチェックと持ってるチェック
      if (pmgEO_->mgDt_.dtBGM_.GetDtLine(i, enDtBGM_titleid) == titleid)
      {
        //所持ＢＧＭ検索
        BOOL have_f = pmgEO_->mgCoU_.IsBGM(i);
        TUGRcNormal* newrecord = MakeObj_Record();
        newrecord->SetCommonID(i);//ＢＧＭ行番号を保存
        if (have_f)
        {
          newrecord->ugname_.SetNameTape(i, (homebgm == i));
        }
        else
        {
          s32 mid_nabox = pmgEO_->mgDt_.dtBGM_.GetDtLine(i, enDtBGM_mid_nabox);
          if (mid_nabox == NGID)
          {
            newrecord->ugname_.SetName("はいしん＿みてい");
			newrecord->SetDisable(TRUE);
          }
          else
          {
            //newrecord->SetSubName2("ＢＯＸ＿ＮＡＭＥ", FC2_normal);
            //newrecord->ugname_.SetNameNaBox(mid_nabox);
			  newrecord->ugname_.SetNameIcon("ほげ", enUGPIcon::ugi_Search2);
            //newrecord->ugname_.SetName(pmgEO_->mgCoM_.mdm_NaBox_.GetPKDtStr(mid_nabox, mdm_nabox_name_c64));
			  
			  //newrecord->ugname_.SetName("イントロ＿さいせい");

			  newrecord->ugname_.SetName("もっていない");
			  newrecord->SetSubName2("ＰＬＡＹ＿ＩＮＴＲＯ", FC2_normal);
          }

          //newrecord->SetDisable(TRUE);
        }

      }
    }

    //レコードが出来てから戻す
    SetZeroPos();
  }


  void TUGLiTape::Refresh(s64 titleid, s64 mid_event)
  {
	  if (mid_event == NGNUM)
	  {
		  Refresh(titleid);
		  return;
	  }

	  s32 homebgm = pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_id_m_bgm);

	  ClearRecord();

	  SetDraw(TRUE);

	  LONGVECTOR allBgmVec = pmgEO_->mgCoM_.GetTokkouBgmByTitle(mid_event, titleid);
	  for (s32 i = 0; i < allBgmVec.size(); i++)
	  {
		  s64 bgmid = allBgmVec[i];

		  //所持ＢＧＭ検索
		  BOOL have_f = pmgEO_->mgCoU_.IsBGM(bgmid);
		  TUGRcNormal* newrecord = MakeObj_Record();
		  newrecord->SetCommonID(bgmid);//ＢＧＭ行番号を保存
		  if (have_f)
		  {
			  newrecord->ugname_.SetNameTape(bgmid, (homebgm == bgmid));
		  }
		  else
		  {
			  s32 mid_nabox = pmgEO_->mgDt_.dtBGM_.GetDtLine(bgmid, enDtBGM_mid_nabox);
			  if (mid_nabox == NGID)
			  {
				  newrecord->ugname_.SetName("はいしん＿みてい");
				  newrecord->SetDisable(TRUE);
			  }
			  else
			  {
				  newrecord->ugname_.SetNameIcon("ほげ", enUGPIcon::ugi_Search2);
				  newrecord->ugname_.SetName("もっていない");
				  newrecord->SetSubName2("ＰＬＡＹ＿ＩＮＴＲＯ", FC2_normal);
			  }
		  }
	  }

	  //レコードが出来てから戻す
	  SetZeroPos();
  }

  //ほーむＢＧＭが変わったとき
  void TUGLiTape::RefreshIcon()
  {
    s32 homebgm = pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_id_m_bgm);
#ifdef __K_DEBUG_USER_DATA__
	homebgm = kdebug::DebugUDManager::GetInstance()->GetHomeBGM();
#endif

    for (s32 i = 0; i < records_.size(); ++i)
    {
      s32 mid_bgm = records_[i]->GetCommonID();

      //所持ＢＧＭ検索
      BOOL have_f = pmgEO_->mgCoU_.IsBGM(mid_bgm);
      if (have_f)
      {
        //所持ＢＧＭ検索
        BOOL home_f = (homebgm == mid_bgm);
        records_[i]->ugname_.ugicon_.SetIconTape(mid_bgm, home_f);
      }
    }
  }

  void TUGLiTape::DoUpdateSub(BOOL draw_f)
  {

  }

  void TUGLiTape::DoActionSub()
  {
    //選ばれたレコード色づけ
    SetSelect();
  }

}
