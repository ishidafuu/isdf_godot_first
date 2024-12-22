#include "dbUGLiSoubiAll.h"
#include "dbUILayer.h"
#include "dbUGRcNormal.h"

namespace db
{
  //コンストラクタ
  TUGLiSoubiAll::TUGLiSoubiAll(TUIGenLayer* player) :TUGLiGen(player)
  {
    stLiSoubiAll_.Init();
  }

  TUGLiSoubiAll::~TUGLiSoubiAll()
  {
    //解放時必ず呼ぶ
    ClearRecord();
  }
  void TUGLiSoubiAll::DoMakeGroupSub()
  {
    ugbtn_subAuto_ = MakeSubBtn("おまかせからうつす", ugi_ShSetAuto);
    ugbtn_subShSet1_ = MakeSubBtn("セット１からうつす", ugi_ShSet1);
    ugbtn_subShSet2_ = MakeSubBtn("セット２からうつす", ugi_ShSet2);
    ugbtn_subShSet3_ = MakeSubBtn("セット３からうつす", ugi_ShSet3);
    ugbtn_subClear_ = MakeSubBtn("すべてはずす", ugi_ShSetClear);

    ugradio_sub_.SetRadio(FALSE);
  }
  //リストクリア（変数部分的に初期化、レコードとレコードエンド解放）
  void TUGLiSoubiAll::DoClearRecord()
  {
    stLiSoubiAll_.Init();
    for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);
    records_.clear();
  }

  TUGRcSoubiAll* TUGLiSoubiAll::MakeObj_Record()
  {
    TUGRcSoubiAll* newrecord = new TUGRcSoubiAll(player_);
    s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
    records_.push_back(newrecord);
    GenMakeObj_Record(newrecord, rh);

    return newrecord;
  }
  void TUGLiSoubiAll::Refresh(s64 id_team, LONGVECTOR* id_shotvec, BOOL init_f)
  {

    if (init_f)
    {
      id_shotvec->clear();
      ClearRecord();//レコードクリアstLiSoubi_もクリアされてる
      stLiSoubiAll_.id_team_ = id_team;
      stLiSoubiAll_.mid_team_ = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_id_m_team);
      s32 rarity = pmgEO_->mgCoM_.mdm_Team_.GetLineDt(stLiSoubiAll_.mid_team_, mdm_team_rarity);
      s32 overlap = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_overlap);

      for (s32 i = 0; i < SLOTALL; ++i)
      {
        TUGRcSoubiAll* newrecord = (TUGRcSoubiAll*)MakeObj_Record();
        newrecord->SetBaseDt(i, stLiSoubiAll_.mid_team_);
        s32 needover = pmgEO_->mgCoM_.mdm_Slot_.GetLineDt(rarity, (enmdm_Slot)(mdm_slot_slot00 + i));//必要なオーバーラップ
        s64 id_shot = NGID;
        if (needover > overlap)//レベル足りてない
        {
          newrecord->SetShotDt(id_shot, stLiSoubiAll_.mid_team_);//素材ＩＤ
          newrecord->SetOpenLvDt(needover);//素材ＩＤ
        }
        else
        {
          id_shot = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, (enmdTeam)(mdteam_id_shot00 + i));
          newrecord->SetShotDt(id_shot, stLiSoubiAll_.mid_team_);//素材ＩＤ
        }
        id_shotvec->push_back(id_shot);

      }
      SetZeroPos();
    }
    else
    {
      //stLiSoubiAll_.id_team_ = id_team;
      //stLiSoubiAll_.mid_team_ = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_id_m_team);
      s32 rarity = pmgEO_->mgCoM_.mdm_Team_.GetLineDt(stLiSoubiAll_.mid_team_, mdm_team_rarity);
      s32 overlap = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_overlap);
      s32 slot = 0;
      for (auto& id_shot : *id_shotvec)
      {
        s32 needover = pmgEO_->mgCoM_.mdm_Slot_.GetLineDt(rarity, (enmdm_Slot)(mdm_slot_slot00 + slot));//必要なオーバーラップ
        if (needover > overlap)//レベル足りてない
        {
          records_[slot]->SetShotDt(NGID, stLiSoubiAll_.mid_team_);//素材ＩＤ
          records_[slot]->SetOpenLvDt(needover);//素材ＩＤ
        }
        else
        {
          records_[slot]->SetShotDt(id_shot, stLiSoubiAll_.mid_team_);//素材ＩＤ
        }
        ++slot;
      }
    }
  }
  void TUGLiSoubiAll::Rename(s32 slot, s64 id_shot)
  {
    records_[slot]->SetShotDt(id_shot, stLiSoubiAll_.mid_team_);//素材ＩＤ
  }


  void TUGLiSoubiAll::DoUpdateSub(BOOL draw_f)
  {

  }

  void TUGLiSoubiAll::DoActionSub()
  {
    //選ばれたレコード色づけ
    //SetSelect();
  }
}
