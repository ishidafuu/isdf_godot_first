#include "dbUGLiTmFilter.h"
#include "dbUILayer.h"
#include "dbUGRcNormal.h"

namespace db
{
  //コンストラクタ
  TUGLiTmFilter::TUGLiTmFilter(TUIGenLayer* player) :TUGLiGen(player)
  {
    stLiTmFilter_.Init();
    teamvec_.clear();
  }

  TUGLiTmFilter::~TUGLiTmFilter()
  {
    //解放時必ず呼ぶ
    ClearRecord();
  }
  void TUGLiTmFilter::DoMakeGroupSub()
  {
    //ugbtn_subSortMID_ = MakeSubBtn("ばんごう", ugi_SortTeamMID);
    ugbtn_subSortRarity_ = MakeSubBtn("レアリティ", ugi_RarityAll);
    ugbtn_subSortWait_ = MakeSubBtn("まちじかん", ugi_MGK);
    ugbtn_subSortLv_ = MakeSubBtn("レベル", ugi_SortLV);
    ugbtn_subSortNew_ = MakeSubBtn("かくとくにちじ", ugi_SortTeamNew);
  }
  //リストクリア（変数部分的に初期化、レコードとレコードエンド解放）
  void TUGLiTmFilter::DoClearRecord()
  {
    //stLiTmFilter_.Init();
    for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);
    records_.clear();
  }

  TUGRcNormal* TUGLiTmFilter::MakeObj_Record()
  {
    TUGRcNormal* newrecord = new TUGRcNormal(player_);
    s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
    records_.push_back(newrecord);
    GenMakeObj_Record(newrecord, rh);

    return newrecord;
  }

  void TUGLiTmFilter::Ready(s32 mid_event)
  {
    
    ClearRecord();//レコードクリアstLiSoubi_もクリアされてる
    //チームベクタ初期化
    id_teamvec_ = pmgEO_->mgCoU_.GetTeamList_Filter(&teamvec_, stLiTmFilter_.sorttype_, stLiTmFilter_.filtertype_, FALSE, mid_event);//とりあえずイベントＩＤは入れない
    stLiTmFilter_.mid_event_ = mid_event;

    std::string shsetname = "";
    for (s32 i = 0; i < enTeamFilTypeEND; ++i)
    {
      TUGRcNormal* newrecord = (TUGRcNormal*)MakeObj_Record();
      newrecord->SetCommonID(i);
      switch (i)
      {
      case db::enTeamFilType_All:newrecord->ugname_.SetNameIcon("すべて", ugi_RarityAll); break;
      case db::enTeamFilType_Tokkou:newrecord->ugname_.SetNameIcon("とっこう", ugi_FilTokkou); break;
      case db::enTeamFilType_RarityD:newrecord->ugname_.SetNameIcon("ランクＤ", ugi_RarityD); break;
      case db::enTeamFilType_RarityC:newrecord->ugname_.SetNameIcon("ランクＣ", ugi_RarityC); break;
      case db::enTeamFilType_RarityB:newrecord->ugname_.SetNameIcon("ランクＢ", ugi_RarityB); break;
      case db::enTeamFilType_RarityA:newrecord->ugname_.SetNameIcon("ランクＡ", ugi_RarityA); break;
      case db::enTeamFilType_RarityS:newrecord->ugname_.SetNameIcon("ランクＳ", ugi_RarityS); break;
      case db::enTeamFilType_RaritySS:newrecord->ugname_.SetNameIcon("ランクＳＳ", ugi_RaritySS); break;
      //case db::enTeamFilType_RaritySSS:newrecord->ugname_.SetNameIcon("ランクＳＳＳ", ugi_RaritySSS); break;
      //case db::enTeamFilType_ShSetA:newrecord->ugname_.SetNameIcon("おまかせセット", ugi_ShSetAuto); break;
      //case db::enTeamFilType_ShSetB:newrecord->ugname_.SetNameIcon(pmgEO_->mgCoU_.GetShSetName(1).c_str(), ugi_ShSet1);break;
      //case db::enTeamFilType_ShSetC:newrecord->ugname_.SetNameIcon(pmgEO_->mgCoU_.GetShSetName(2).c_str(), ugi_ShSet2); break;
      //case db::enTeamFilType_ShSetD:newrecord->ugname_.SetNameIcon(pmgEO_->mgCoU_.GetShSetName(3).c_str(), ugi_ShSet3); break;
      //case db::enTeamFilType_ShSetE:newrecord->ugname_.SetNameIcon("カスタムセット", ugi_ShSetCstm); break;
      }
    }
    Refresh();
  }

  BOOL TUGLiTmFilter::Refresh()
  {
    BOOL reset_f = FALSE;
    SORTTEAMVECTOR checkteamvec = pmgEO_->mgCoU_.GetTeamVec();
    for (s32 i = 0; i < enTeamFilTypeEND; ++i)
    {
      s32 num = pmgEO_->mgCoU_.GetNumTeamList(&checkteamvec, (enTeamFilType)i, stLiTmFilter_.mid_event_);
      records_[i]->SetValue(lib_str::IntToStr(num).c_str());
      BOOL disable_f = (num == 0);
      records_[i]->SetDisable(disable_f);
      if (disable_f && (i == stLiTmFilter_.filtertype_)) reset_f = TRUE;
    }

    //空になってたときは全カードにリセット
    if (reset_f) GetTeamList_Filter(enTeamFilType_All);

    //フィルタ種類を店頭差し得
    SetSelect(stLiTmFilter_.filtertype_);
    ugradio_sub_.SetSelect(stLiTmFilter_.sorttype_);
    SetZeroPos();

    return reset_f;
  }
  //void TUGLiTmFilter::SetMIDEvent(s32 mid_event)
  //{
  //  stLiTmFilter_.mid_event_ = mid_event;
  //}

  void TUGLiTmFilter::GetTeamList_Filter(enTeamFilType filtertype)
  {
    stLiTmFilter_.filtertype_ = filtertype;
    id_teamvec_ = pmgEO_->mgCoU_.GetTeamList_Filter(&teamvec_, stLiTmFilter_.sorttype_, stLiTmFilter_.filtertype_, stLiTmFilter_.rev_f_, stLiTmFilter_.mid_event_);
  }
  void TUGLiTmFilter::GetTeamList_Sort(enTeamSrtType sorttype)
  {
    if (stLiTmFilter_.sorttype_ == sorttype)
    {
      stLiTmFilter_.rev_f_ = !(stLiTmFilter_.rev_f_);
    }
    else
    {
      stLiTmFilter_.rev_f_ = FALSE;
    }
    stLiTmFilter_.sorttype_ = sorttype;
    id_teamvec_ = pmgEO_->mgCoU_.GetTeamList_Sort(&teamvec_, stLiTmFilter_.sorttype_, stLiTmFilter_.rev_f_);
  }
 
  void TUGLiTmFilter::DoUpdateSub(BOOL draw_f)
  {

  }
  void TUGLiTmFilter::DoActionSub()
  {
    if (IsActRes(enUGLiAct_SubA))
    {
      //GetTeamList_Sort(enTeamSrtType_MID);
      GetTeamList_Sort(enTeamSrtType_Rarity);
    }
    else if (IsActRes(enUGLiAct_SubB))
    {
      GetTeamList_Sort(enTeamSrtType_Wait);
    }
    else if (IsActRes(enUGLiAct_SubC))
    {
      GetTeamList_Sort(enTeamSrtType_Lv);
    }
    else if (IsActRes(enUGLiAct_SubD))
    {
      GetTeamList_Sort(enTeamSrtType_New);
    }
    //else if (IsActRes(enUGLiAct_SubE))
    //{
    //}
    else if (IsActRes(enUGLiAct_Record))
    {
      GetTeamList_Filter((enTeamFilType)GetActIndex());
      //選ばれたレコード色づけ
      SetSelect();
    }
  }
    BOOL TUGLiTmFilter::IsActRes2(enUGLiTmFilterAct res)//返値ゲット
    {
      switch (GetActRes())
      {
      case db::enUGLiAct_SubA:
      case db::enUGLiAct_SubB:
      case db::enUGLiAct_SubC:
      case db::enUGLiAct_SubD:
      case db::enUGLiAct_SubE:
        return (res == enUGLiTmFilter_Sort);
        break;
      case db::enUGLiAct_Record:
        return(res == enUGLiTmFilter_Filter);
        break;
      }
      return FALSE;
    }
}
