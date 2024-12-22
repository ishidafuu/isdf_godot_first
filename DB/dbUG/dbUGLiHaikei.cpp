#include "dbUGLiHaikei.h"
#include "dbUILayer.h"
#include "dbUGRcNormal.h"

namespace db
{
  //コンストラクタ
  TUGLiHaikei::TUGLiHaikei(TUIGenLayer* player) :TUGLiGen(player)
  {
    stLiHaikei_.Init();
    
    allnumvec_.clear();
    
    for (s32 i = 0; i < pmgEO_->mgDt_.dtTitle_.GetLineNum(); ++i) allnumvec_.push_back(0);
    
    for (s32 i2 = 0; i2 < pmgEO_->mgDt_.dtCourt_.GetLineNum(); ++i2)
    {
      s32 titleid = pmgEO_->mgDt_.dtCourt_.GetDtLine(i2, enDtCourt_titleid);
      for (s32 i = 0; i < pmgEO_->mgDt_.dtTitle_.GetLineNum(); ++i)
      {
        if (pmgEO_->mgDt_.dtTitle_.GetDtLine(i, enDtTitle_pk) == titleid)
        {
          allnumvec_[i] += 1;
          break;
        }
      }
    }
  }

  TUGLiHaikei::~TUGLiHaikei()
  {
    //解放時必ず呼ぶ
    ClearRecord();
  }

  //リストクリア（変数部分的に初期化、レコードとレコードエンド解放）
  void TUGLiHaikei::DoClearRecord()
  {
    stLiHaikei_.Init();
    for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);
    records_.clear();
  }

  TUGRcNormal* TUGLiHaikei::MakeObj_Record()
  {
    TUGRcNormal* newrecord = new TUGRcNormal(player_);
    s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
    records_.push_back(newrecord);
    GenMakeObj_Record(newrecord, rh);

    return newrecord;
  }
  void TUGLiHaikei::Refresh()
  {
    ClearRecord();
    SetDraw(TRUE);

    //画像を読み込む
    INTVECTOR havenumvec;
    for (s32 i = 0; i < pmgEO_->mgDt_.dtTitle_.GetLineNum(); ++i)
    {
      havenumvec.push_back(0);
    }
    
    //所持haikei検索
    for (s32 i3 = 0; i3 < pmgEO_->mgDt_.dtCourt_.GetLineNum(); ++i3)
    {
      if (pmgEO_->mgCoU_.IsHaikei(i3) == FALSE) continue;

      s32 titleid = pmgEO_->mgDt_.dtCourt_.GetDtLine(i3, enDtCourt_titleid);
      for (s32 i = 0; i < pmgEO_->mgDt_.dtTitle_.GetLineNum(); ++i)
      {
        if (pmgEO_->mgDt_.dtTitle_.GetDtLine(i, enDtTitle_pk) == titleid)
        {
          havenumvec[i] += 1;
          break;
        }
      }
    }
    
    for (s32 i = 0; i < pmgEO_->mgDt_.dtTitle_.GetLineNum(); ++i)
    {
      if (allnumvec_[i] == 0) continue;
      
      TUGRcNormal* newrecord = MakeObj_Record();
      //if (havenumvec[i] == 0) newrecord->SetDisable(TRUE);
      s32 titleid = pmgEO_->mgDt_.dtTitle_.GetDtLine(i, enDtTitle_pk);
      newrecord->SetCommonID(titleid);//タイトルＩＤを保存
      newrecord->ugname_.SetNameHaikeiTitle(titleid);
      std::string strnum = lib_str::IntToStr(havenumvec[i]) + "／" + lib_str::IntToStr(allnumvec_[i]);
      newrecord->SetSubValue(strnum.c_str());
    }
    
    //レコードが出来てから戻す
    SetZeroPos();

    //ゼロの時は表示無し
//    if (titleidvec.size() == 0) SetDraw(FALSE);
  }

  void TUGLiHaikei::Refresh(s64 mid_event)
  {
	  if (mid_event == NGNUM)
	  {
		  Refresh();
		  return;
	  }

	  ClearRecord();

	  SetDraw(TRUE);

	  LONGVECTOR2 allHaikeiVec = pmgEO_->mgCoM_.GetTokkouHaikei(mid_event);
	  LONGVECTOR2 haveHaikeiVec = pmgEO_->mgCoM_.GetHaveTokkouHaikei(mid_event);

	  if (allHaikeiVec.size() <= 0)
	  {
		  Refresh();
		  return;
	  }

	  BOOL isSet = FALSE;
	  for (s32 i = 0; i < allHaikeiVec.size(); i++)
	  {
		  if (allHaikeiVec[i].size() <= 0){ continue; }

		  s64 titleid = pmgEO_->mgDt_.dtTitle_.GetDtLine(i, enDtTitle::enDtTitle_pk);

		  TUGRcNormal* newrecord = MakeObj_Record();
		  //if (havenumvec[i] == 0) newrecord->SetDisable(TRUE);
		  newrecord->SetCommonID(titleid);//タイトルＩＤを保存
		  newrecord->ugname_.SetNameHaikeiTitle(titleid);
		  std::string strnum = lib_str::IntToStr(haveHaikeiVec[i].size()) + "／" + lib_str::IntToStr(allHaikeiVec[i].size());
		  newrecord->SetSubValue(strnum.c_str());
		  isSet = TRUE;
	  }

	  //レコードが出来てから戻す
	  SetZeroPos();
  }


  void TUGLiHaikei::DoUpdateSub(BOOL draw_f)
  {

  }

  void TUGLiHaikei::DoActionSub()
  {
    //選ばれたレコード色づけ
    SetSelect();
  }

}
