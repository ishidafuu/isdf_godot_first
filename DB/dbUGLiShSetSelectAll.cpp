#include "dbUGLiShSetSelectAll.h"
#include "dbUILayer.h"
#include "dbUGRcNormal.h"

namespace db
{
  //コンストラクタ
  TUGLiShSetSelectAll::TUGLiShSetSelectAll(TUIGenLayer* player) :TUGLiGen(player)
  {
    stLiShSetSelect_.Init();
  }

  TUGLiShSetSelectAll::~TUGLiShSetSelectAll()
  {
    //解放時必ず呼ぶ
    ClearRecord();
  }

  void TUGLiShSetSelectAll::Refresh(s64 id_team, s64 id_shset, LONGVECTOR& id_shotvec)
  {
    SetDraw(TRUE);
    stLiShSetSelect_.Init();

    ClearRecord();//レコードクリアstLiSoubi_もクリアされてる

    if (id_shset == NGNUM) id_shset = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_id_shset);
    s32 mid_team = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_id_m_team);
    s32 rarity = pmgEO_->mgCoM_.mdm_Team_.GetLineDt(mid_team, mdm_team_rarity);
    s32 overlap = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_overlap);

    stLiShSetSelect_.id_team_ = id_team;
    stLiShSetSelect_.mid_team_ = mid_team;
    stLiShSetSelect_.rarity_ = rarity;
    stLiShSetSelect_.overlap_ = overlap;
    stLiShSetSelect_.id_shset_ = id_shset;
    id_shotvec_ = &id_shotvec;

    if (id_shset == 0)
    {
      ugradio_sub_.SetSelect(SHSETNUM);
    }
    else
    {
      for (s32 i = 0; i < SHSETNUM; ++i)
      {
        if (id_shset == pmgEO_->mgCoU_.mdShSet_.GetLineDt(i, mdshset_pk))
        {
          ugradio_sub_.SetSelect(i);
          break;
        }
      }
    }


    id_shotvec_->clear();
    for (s32 i = 0; i < SLOTALL; ++i)
    {
      s64 id_shot = (stLiShSetSelect_.id_shset_ == 0)
        ? pmgEO_->mgCoU_.mdTeam_.GetPKDt(stLiShSetSelect_.id_team_, (enmdTeam)(mdteam_id_shot00 + i))
        : pmgEO_->mgCoU_.mdShSet_.GetPKDt(stLiShSetSelect_.id_shset_, (enmdShSet)(mdshset_id_shot00 + i));

      TUGRcShSetSelectAll* newrecord = (TUGRcShSetSelectAll*)MakeObj_Record();
      newrecord->SetBaseDt(i, stLiShSetSelect_.mid_team_);

      s32 needover = pmgEO_->mgCoM_.mdm_Slot_.GetLineDt(stLiShSetSelect_.rarity_, (enmdm_Slot)(mdm_slot_slot00 + i));//必要なオーバーラップ
      if (needover > stLiShSetSelect_.overlap_)//レベル足りてない
      {
        newrecord->SetOpenLvDt(needover);//素材ＩＤ
      }
      else
      {
        newrecord->SetShotDt(id_shot);//素材ＩＤ
      }
      id_shotvec_->push_back(id_shot);
    }


    //GenRefresh();

  }

  void TUGLiShSetSelectAll::Refresh2(s32 shsetNo)
  {
    id_shotvec_->clear();
    s64 id_shset = 0;
    //カスタム以外
    if (shsetNo < SHSETNUM)
    {
      id_shset = pmgEO_->mgCoU_.mdShSet_.GetLineDt(shsetNo, mdshset_pk);
    }

    stLiShSetSelect_.id_shset_ = id_shset;
    ugradio_sub_.SetSelect(shsetNo);

    for (s32 i = 0; i < records_.size(); ++i)
    {
      s64 id_shot = (stLiShSetSelect_.id_shset_ == 0)
        ? pmgEO_->mgCoU_.mdTeam_.GetPKDt(stLiShSetSelect_.id_team_, (enmdTeam)(mdteam_id_shot00 + i))
        : pmgEO_->mgCoU_.mdShSet_.GetPKDt(stLiShSetSelect_.id_shset_, (enmdShSet)(mdshset_id_shot00 + i));
      records_[i]->SetBaseDt(i, stLiShSetSelect_.mid_team_);

      s32 needover = pmgEO_->mgCoM_.mdm_Slot_.GetLineDt(stLiShSetSelect_.rarity_, (enmdm_Slot)(mdm_slot_slot00 + i));//必要なオーバーラップ
      if (needover > stLiShSetSelect_.overlap_)//レベル足りてない
      {
        records_[i]->SetOpenLvDt(needover);//素材ＩＤ
      }
      else
      {
        records_[i]->SetShotDt(id_shot);//素材ＩＤ
      }
      id_shotvec_->push_back(id_shot);
    }

  }

  //変わったシュートセット
  s64 TUGLiShSetSelectAll::GetShSet()
  {
    return stLiShSetSelect_.id_shset_;
  }

  void TUGLiShSetSelectAll::DoMakeGroupSub()
  {
    ugbtn_subAuto_ = MakeSubBtn("おまかせセット", ugi_ShSetAuto);
    ugbtn_subShSet1_ = MakeSubBtn(pmgEO_->mgCoU_.GetShSetName(1, TRUE).c_str(), ugi_ShSet1);
    ugbtn_subShSet2_ = MakeSubBtn(pmgEO_->mgCoU_.GetShSetName(2, TRUE).c_str(), ugi_ShSet2);
    ugbtn_subShSet3_ = MakeSubBtn(pmgEO_->mgCoU_.GetShSetName(3, TRUE).c_str(), ugi_ShSet3);
    ugbtn_subCustom_ = MakeSubBtn("カスタムそうび", ugi_ShSetCstm);

    ugradio_sub_.SetRadio(FALSE);
  }
  //リストクリア（変数部分的に初期化、レコードとレコードエンド解放）
  void TUGLiShSetSelectAll::DoClearRecord()
  {
    //stLiShSetSelect_.Init();
    for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);
    records_.clear();
  }
  void TUGLiShSetSelectAll::DoUpdateSub(BOOL draw_f)
  {

  }

  void TUGLiShSetSelectAll::DoActionSub()
  {
    if (IsActRes(enUGLiAct_SubA))
    {
      Refresh2(0);
    }
    else if (IsActRes(enUGLiAct_SubB))
    {
      Refresh2(1);
    }
    else if (IsActRes(enUGLiAct_SubC))
    {
      Refresh2(2);
    }
    else if (IsActRes(enUGLiAct_SubD))
    {
      Refresh2(3);
    }
    else if (IsActRes(enUGLiAct_SubE))
    {
      Refresh2(4);
    }
  }

  TUGRcShSetSelectAll* TUGLiShSetSelectAll::MakeObj_Record()
  {
    TUGRcShSetSelectAll* newrecord = new TUGRcShSetSelectAll(player_);
    s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
    records_.push_back(newrecord);
    GenMakeObj_Record(newrecord, rh);

    return newrecord;
  }



}
