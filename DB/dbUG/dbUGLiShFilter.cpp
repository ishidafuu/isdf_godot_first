#include "dbUGLiShFilter.h"
#include "dbUILayer.h"
#include "dbUGRcNormal.h"

namespace db
{
  //コンストラクタ
  TUGLiShFilter::TUGLiShFilter(TUIGenLayer* player) :TUGLiGen(player)
  {
    stLiShFilter_.Init();
    shotvec_.clear();
  }

  TUGLiShFilter::~TUGLiShFilter()
  {
    //解放時必ず呼ぶ
    ClearRecord();
  }
  void TUGLiShFilter::DoMakeGroupSub()
  {
    ugbtn_subSortRarity_ = MakeSubBtn("レアリティ", ugi_RarityAll);
    ugbtn_subSortPow_ = MakeSubBtn("パワー", ugi_SortShPow);
    ugbtn_subSortSpin_ = MakeSubBtn("スピン", ugi_SortShSpin);
    ugbtn_subSortNew_ = MakeSubBtn("かくとくにちじ", ugi_SortShNew);
  }
  //リストクリア（変数部分的に初期化、レコードとレコードエンド解放）
  void TUGLiShFilter::DoClearRecord()
  {
    //stLiShFilter_.Init();
    for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);
    records_.clear();
  }

  TUGRcNormal* TUGLiShFilter::MakeObj_Record()
  {
    TUGRcNormal* newrecord = new TUGRcNormal(player_);
    s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
    records_.push_back(newrecord);
    GenMakeObj_Record(newrecord, rh);

    return newrecord;
  }
  BOOL TUGLiShFilter::Ready(enShotFilTypeS filtypeS)
  {
    return Ready(filtypeS, NULL);
  }
  BOOL TUGLiShFilter::Ready(enShotFilTypeS filtypeS, LONGVECTOR* ngidvec)
  {

    ClearRecord();//レコードクリアstLiSoubi_もクリアされてる
    //シュートベクタ初期化
    stLiShFilter_.filtertype_ = enShotFilType_All;//readyのときはALLで初期化か
    stLiShFilter_.filtertypeS_ = filtypeS;
    if (ngidvec != NULL)
    {
      ngidvec_ = *ngidvec;
    }
    else
    {
      ngidvec_.clear();
    }
    id_shotvec_ = pmgEO_->mgCoU_.GetShotList_Filter(&shotvec_, stLiShFilter_.sorttype_, stLiShFilter_.filtertype_, filtypeS, stLiShFilter_.rev_f_, ngidvec);

    for (s32 i = 0; i < enShotFilTypeEND; ++i)
    {
      TUGRcNormal* newrecord = (TUGRcNormal*)MakeObj_Record();
      newrecord->SetCommonID(i);

      switch (i)
      {
      case db::enShotFilType_All:newrecord->ugname_.SetNameIcon("すべて", ugi_RarityAll); break;
      case db::enShotFilType_RarityD:newrecord->ugname_.SetNameIcon("ランクＤ", ugi_RarityD); break;
      case db::enShotFilType_RarityC:newrecord->ugname_.SetNameIcon("ランクＣ", ugi_RarityC); break;
      case db::enShotFilType_RarityB:newrecord->ugname_.SetNameIcon("ランクＢ", ugi_RarityB); break;
      case db::enShotFilType_RarityA:newrecord->ugname_.SetNameIcon("ランクＡ", ugi_RarityA); break;
      case db::enShotFilType_RarityS:newrecord->ugname_.SetNameIcon("ランクＳ", ugi_RarityS); break;
      case db::enShotFilType_RaritySS:newrecord->ugname_.SetNameIcon("ランクＳＳ", ugi_RaritySS); break;
      case db::enShotFilType_RaritySSS:newrecord->ugname_.SetNameIcon("ランクＳＳＳ", ugi_RaritySSS); break;
      case db::enShotFilType_Locked:newrecord->ugname_.SetNameIcon("おきにいり", ugi_ShLock); break;
      case db::enShotFilType_Solo:newrecord->ugname_.SetNameIcon("そざいだま", ugi_FilSolo); break;
      case db::enShotFilType_Mixed:newrecord->ugname_.SetNameIcon("ミックス", ugi_FilMixed); break;
      case db::enShotFilType_FullMixed:newrecord->ugname_.SetNameIcon("フルミックス", ugi_FilFullMixed); break;
      case db::enShotFilType_Genre0:newrecord->ugname_.SetNameIcon(pmgEO_->mgDt_.dtShElm_.GetGenreNameLine(0).c_str(), ugi_FilGenre0); break;
      case db::enShotFilType_Genre1:newrecord->ugname_.SetNameIcon(pmgEO_->mgDt_.dtShElm_.GetGenreNameLine(1).c_str(), ugi_FilGenre1); break;
      case db::enShotFilType_Genre2:newrecord->ugname_.SetNameIcon(pmgEO_->mgDt_.dtShElm_.GetGenreNameLine(2).c_str(), ugi_FilGenre2); break;
      case db::enShotFilType_Genre3:newrecord->ugname_.SetNameIcon(pmgEO_->mgDt_.dtShElm_.GetGenreNameLine(3).c_str(), ugi_FilGenre3); break;
      case db::enShotFilType_Genre4:newrecord->ugname_.SetNameIcon(pmgEO_->mgDt_.dtShElm_.GetGenreNameLine(4).c_str(), ugi_FilGenre4); break;
      case db::enShotFilType_Genre5:newrecord->ugname_.SetNameIcon(pmgEO_->mgDt_.dtShElm_.GetGenreNameLine(5).c_str(), ugi_FilGenre5); break;
      case db::enShotFilType_Genre6:newrecord->ugname_.SetNameIcon(pmgEO_->mgDt_.dtShElm_.GetGenreNameLine(6).c_str(), ugi_FilGenre6); break;
      case db::enShotFilType_Genre7:newrecord->ugname_.SetNameIcon(pmgEO_->mgDt_.dtShElm_.GetGenreNameLine(7).c_str(), ugi_FilGenre7); break;
      case db::enShotFilType_Genre8:newrecord->ugname_.SetNameIcon(pmgEO_->mgDt_.dtShElm_.GetGenreNameLine(8).c_str(), ugi_FilGenre8); break;
      case db::enShotFilType_Genre9:newrecord->ugname_.SetNameIcon(pmgEO_->mgDt_.dtShElm_.GetGenreNameLine(9).c_str(), ugi_FilGenre9); break;
      case db::enShotFilType_Genre10:newrecord->ugname_.SetNameIcon(pmgEO_->mgDt_.dtShElm_.GetGenreNameLine(10).c_str(), ugi_FilGenre10); break;
      }
    }
    return Refresh(filtypeS, TRUE);
  }

  BOOL TUGLiShFilter::Refresh(enShotFilTypeS filtypeS, BOOL from_ready)
  {
    BOOL reset_f = FALSE;
    SORTSHOTVECTOR checkShotvec;
    if (from_ready == FALSE)
    {
      checkShotvec = pmgEO_->mgCoU_.GetShotVec(filtypeS, NULL);//再取得
    }

    //SORTSHOTVECTOR* checkShotvec = pmgEO_->mgCoU_.GetSortShotMap();//すでにあるshotvec_を使う
    for (s32 i = 0; i < enShotFilTypeEND; ++i)
    {
      s32 num = 0;
      if (from_ready)
      {
        num = pmgEO_->mgCoU_.GetNumShotList(&shotvec_, (enShotFilType)i);
      }
      else
      {
        num = pmgEO_->mgCoU_.GetNumShotList(&checkShotvec, (enShotFilType)i);
      }
      records_[i]->SetValue(lib_str::IntToStr(num).c_str());
      BOOL disable_f = (num == 0);
      records_[i]->SetDisable(disable_f);
      if (disable_f && (i == stLiShFilter_.filtertype_)) reset_f = TRUE;
    }

    //空になってたときは全カードにリセット
    if (reset_f) GetShotList_Filter(enShotFilType_All, stLiShFilter_.filtertypeS_);

    //フィルタ種類を店頭差し得
    SetSelect(stLiShFilter_.filtertype_);
    ugradio_sub_.SetSelect(stLiShFilter_.sorttype_);
    SetZeroPos();

    return reset_f;
  }

	void TUGLiShFilter::HaveFilter(enShotFilType filtertype, enShotFilTypeS filtertypeS)
	{
		stLiShFilter_.filtertype_ = filtertype;
		stLiShFilter_.filtertypeS_ = filtertypeS;
		id_shotvec_ = pmgEO_->mgCoU_.GetShotList_Filter(&shotvec_, stLiShFilter_.sorttype_, filtertype, filtertypeS, stLiShFilter_.rev_f_, &ngidvec_);
	}

  BOOL TUGLiShFilter::IsHaveMixed()
  {
    for (auto& item : shotvec_)
    {
      if (item.useslot_ > 0) return TRUE;
    }
    return FALSE;
  }

  void TUGLiShFilter::GetShotList_Filter(enShotFilType filtertype, enShotFilTypeS filtertypeS)
  {
    stLiShFilter_.filtertype_ = filtertype;
    stLiShFilter_.filtertypeS_ = filtertypeS;
    id_shotvec_ = pmgEO_->mgCoU_.GetShotList_Filter(&shotvec_, stLiShFilter_.sorttype_, filtertype, filtertypeS, stLiShFilter_.rev_f_, &ngidvec_);
  }
  void TUGLiShFilter::GetShotList_Sort(enShotSrtType sorttype)
  {
    if (stLiShFilter_.sorttype_ == sorttype)
    {
      stLiShFilter_.rev_f_ = !(stLiShFilter_.rev_f_);
    }
    else
    {
      stLiShFilter_.rev_f_ = FALSE;
    }
    stLiShFilter_.sorttype_ = sorttype;
    id_shotvec_ = pmgEO_->mgCoU_.GetShotList_Sort(&shotvec_, stLiShFilter_.sorttype_, stLiShFilter_.filtertypeS_, stLiShFilter_.rev_f_);
  }

  void TUGLiShFilter::DoUpdateSub(BOOL draw_f)
  {

  }
  void TUGLiShFilter::DoActionSub()
  {
    if (IsActRes(enUGLiAct_SubA))
    {
      //GetShotList_Sort(enShotSrtType_MID);
      GetShotList_Sort(enShotSrtType_Rarity);
    }
    else if (IsActRes(enUGLiAct_SubB))
    {
      GetShotList_Sort(enShotSrtType_Pow);
    }
    else if (IsActRes(enUGLiAct_SubC))
    {
      GetShotList_Sort(enShotSrtType_Spin);
    }
    else if (IsActRes(enUGLiAct_SubD))
    {
      GetShotList_Sort(enShotSrtType_New);
    }
    //else if (IsActRes(enUGLiAct_SubE))
    //{
    //}
    else if (IsActRes(enUGLiAct_Record))
    {
      GetShotList_Filter((enShotFilType)GetActIndex(), stLiShFilter_.filtertypeS_);
      //選ばれたレコード色づけ
      SetSelect();
    }
  }
  BOOL TUGLiShFilter::IsActRes2(enUGLiShFilterAct res)//返値ゲット
  {
    switch (GetActRes())
    {
    case db::enUGLiAct_SubA:
    case db::enUGLiAct_SubB:
    case db::enUGLiAct_SubC:
    case db::enUGLiAct_SubD:
    case db::enUGLiAct_SubE:
      return (res == enUGLiShFilter_Sort);
      break;
    case db::enUGLiAct_Record:
      return(res == enUGLiShFilter_Filter);
      break;
    }
    return FALSE;
  }

}
