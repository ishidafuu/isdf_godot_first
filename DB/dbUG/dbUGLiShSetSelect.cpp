#include "dbUGLiShSetSelect.h"
#include "dbUILayer.h"
#include "dbUGRcNormal.h"

namespace db
{

  //コンストラクタ
  TUGLiShSetSelect::TUGLiShSetSelect(TUIGenLayer* player) :TUGLiGen(player)
  {
    stLiShSetSelect_.Init();
  }

  TUGLiShSetSelect::~TUGLiShSetSelect()
  {
    //解放時必ず呼ぶ
    ClearRecord();
  }

  void TUGLiShSetSelect::DoMakeGroupSub()
  {
    ugbtn_subAuto_ = MakeSubBtn("おまかせセット", ugi_ShSetAuto);
    ugbtn_subShSet1_ = MakeSubBtn(pmgEO_->mgCoU_.GetShSetName(1, TRUE).c_str(), ugi_ShSet1);
    ugbtn_subShSet2_ = MakeSubBtn(pmgEO_->mgCoU_.GetShSetName(2, TRUE).c_str(), ugi_ShSet2);
    ugbtn_subShSet3_ = MakeSubBtn(pmgEO_->mgCoU_.GetShSetName(3, TRUE).c_str(), ugi_ShSet3);
    ugbtn_subCustom_ = MakeSubBtn("カスタムそうび", ugi_ShSetCstm);

    ugradio_sub_.SetRadio(FALSE);
  }
  //リストクリア（変数部分的に初期化、レコードとレコードエンド解放）
  void TUGLiShSetSelect::DoClearRecord()
  {
    stLiShSetSelect_.Init();
    for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);
    records_.clear();
  }
  void TUGLiShSetSelect::DoUpdateSub(BOOL draw_f)
  {

  }

  void TUGLiShSetSelect::DoActionSub()
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

  TUGRcShSetSelect* TUGLiShSetSelect::MakeObj_Record()
  {
    TUGRcShSetSelect* newrecord = new TUGRcShSetSelect(player_);
    s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
    records_.push_back(newrecord);
    GenMakeObj_Record(newrecord, rh);

    return newrecord;
  }


  void TUGLiShSetSelect::Refresh(s64 id_team, s64 id_shset, s32 posno, LONGVECTOR& id_shotvec)
  {

    SetDraw(TRUE);
    stLiShSetSelect_.Init();

    ClearRecord();//レコードクリアstLiSoubi_もクリアされてる

    //s32 id_shset = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_id_shset);
    s32 mid_team = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_id_m_team);
    s32 rarity = pmgEO_->mgCoM_.mdm_Team_.GetLineDt(mid_team, mdm_team_rarity);
    s32 overlap = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_overlap);

    stLiShSetSelect_.id_team_ = id_team;
    stLiShSetSelect_.mid_team_ = mid_team;
    stLiShSetSelect_.rarity_ = rarity;
    stLiShSetSelect_.overlap_ = overlap;
    stLiShSetSelect_.id_shset_ = id_shset;
    stLiShSetSelect_.posNo_ = posno;
    id_shotvec_ = &id_shotvec;


    if (stLiShSetSelect_.id_shset_ == 0)
    {
      ugradio_sub_.SetSelect(SHSETNUM);
    }
    else
    {
      for (s32 i = 0; i < SHSETNUM; ++i)
      {
        if (stLiShSetSelect_.id_shset_ == pmgEO_->mgCoU_.mdShSet_.GetLineDt(i, mdshset_pk))
        {
          ugradio_sub_.SetSelect(i);
          break;
        }
      }
    }


    SetShotVec();

    for (s32 i = 0; i < STEPNUM + 1; ++i)
    {
      TUGRcShSetSelect* newrecord = (TUGRcShSetSelect*)MakeObj_Record();
      newrecord->SetBaseDt_ShSet(i, stLiShSetSelect_.mid_team_);
    }

    GenRefresh();
  }


  void TUGLiShSetSelect::Refresh2(s32 shsetNo)
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

    SetShotVec();
    GenRefresh();
  }


  void TUGLiShSetSelect::GenRefresh()
  {


    s32 charid = pmgEO_->mgCoM_.GetCharID(stLiShSetSelect_.mid_team_, stLiShSetSelect_.posNo_);
    //s32 stLiShSetSelect_.rarity_ = pmgEO_->mgCoM_.mdm_Team_.GetLineDt(stLiShSetSelect_.mstLiShSetSelect_.id_team__, mdm_team_stLiShSetSelect_.rarity_);
    s32 steptype = pmgEO_->mgDt_.dtChar_.GetLv(clv_Speed, charid);
    //s32 openno = (VJOPENSTEP * DBMEMBER_ALL) + stLiShSetSelect_.posNo_;//その歩数が開く番号
    s32 openno = (SLOTLV[(steptype * STEPNUM) + VJOPENSTEP] * DBMEMBER_ALL) + stLiShSetSelect_.posNo_;//その歩数が開く番号
    s32 needover = pmgEO_->mgCoM_.mdm_Slot_.GetLineDt(stLiShSetSelect_.rarity_, (enmdm_Slot)(mdm_slot_slot00 + openno));//必要なオーバーラップ

    if (needover > pmgEO_->mgCoU_.mdTeam_.GetPKDt(stLiShSetSelect_.id_team_, mdteam_overlap))//レベル足りてない
    {
      records_[0]->SetBaseDt_ShSet(VJICONSTEP, openno);
      records_[0]->SetOpenLvDt(needover);//素材ＩＤ
    }
    else
    {
      //s64 id_shot = pmgEO_->mgCoU_.mdShSet_.GetPKDt(stLiShSetSelect_.id_shset_, (enmdShSet)(mdshset_id_shot00 + openno));
      records_[0]->SetBaseDt_ShSet(VJICONSTEP, openno);
      records_[0]->SetShotDt_ShSet(id_shotvec_->at(openno), FALSE);//素材ＩＤ
    }

    //歩数
    s32 recno = 1;
    for (s32 step = 0; step < STEPNUM; ++step)
    {
      //デフォルト以外
      if (step != DEFSTEP)
      {
        s32 openno = (SLOTLV[(steptype * STEPNUM) + step] * DBMEMBER_ALL) + stLiShSetSelect_.posNo_;//その歩数が開く番号
        s32 needover = pmgEO_->mgCoM_.mdm_Slot_.GetLineDt(stLiShSetSelect_.rarity_, (enmdm_Slot)(mdm_slot_slot00 + openno));//必要なオーバーラップ

        if (needover > pmgEO_->mgCoU_.mdTeam_.GetPKDt(stLiShSetSelect_.id_team_, mdteam_overlap))//レベル足りてない
        {
          records_[recno]->SetBaseDt_ShSet(step, openno);
          records_[recno]->SetOpenLvDt(needover);//素材ＩＤ
        }
        else
        {
          //s64 id_shot = pmgEO_->mgCoU_.mdShSet_.GetPKDt(stLiShSetSelect_.id_shset_, (enmdShSet)(mdshset_id_shot00 + openno));
          records_[recno]->SetBaseDt_ShSet(step, openno);
          records_[recno]->SetShotDt_ShSet(id_shotvec_->at(openno), FALSE);//素材ＩＤ
        }
      }
      else
      {
        s32 defshot = pmgEO_->mgDt_.dtChar_.GetLv(clv_DefShotID, charid);
        records_[recno]->SetBaseDt_ShSet(step, NGNUM);
        records_[recno]->SetShotDt_ShSet(defshot, TRUE);//素材ＩＤ
      }
      ++recno;
    }
  }


  void TUGLiShSetSelect::SetShotVec()
  {

    id_shotvec_->clear();
    for (s32 i = 0; i < SLOTALL; ++i)
    {
      s64 id_shot = (stLiShSetSelect_.id_shset_ == 0)
        ? pmgEO_->mgCoU_.mdTeam_.GetPKDt(stLiShSetSelect_.id_team_, (enmdTeam)(mdteam_id_shot00 + i))
        : pmgEO_->mgCoU_.mdShSet_.GetPKDt(stLiShSetSelect_.id_shset_, (enmdShSet)(mdshset_id_shot00 + i));

      id_shotvec_->push_back(id_shot);
    }
  }
  //変わったシュートセット
  s64 TUGLiShSetSelect::GetShSet()
  {
    return stLiShSetSelect_.id_shset_;
  }


}
