#include "dbUGLiSoubi.h"
#include "dbUILayer.h"
#include "dbUGRcNormal.h"

namespace db
{

  ////装備品ショット
  //const s32 STEPTYPENUM = 6;//素早さＡ～Ｆの６段階
  //const s32 SLOTLV[STEPNUM * STEPTYPENUM] =
  //{
  //  0, 1, 2, 3, 4, 5, 6,
  //  2, 0, 1, 3, 4, 5, 6,
  //  5, 2, 0, 3, 1, 4, 6,
  //  6, 5, 2, 3, 0, 1, 4,
  //  6, 5, 4, 3, 2, 0, 1,
  //  6, 5, 4, 3, 2, 1, 0,
  //};

  //const s32 DEFSTEP = 3;
  //const s32 VJOPENSTEP = 3;
  //const s32 VJICONSTEP = 7;

  //コンストラクタ
  TUGLiSoubi::TUGLiSoubi(TUIGenLayer* player) :TUGLiGen(player)
  {
    stLiSoubi_.Init();
  }

  TUGLiSoubi::~TUGLiSoubi()
  {
    //解放時必ず呼ぶ
    ClearRecord();
  }

  //リストクリア（変数部分的に初期化、レコードとレコードエンド解放）
  void TUGLiSoubi::DoClearRecord()
  {
    stLiSoubi_.Init();
    for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);
    records_.clear();
  }

  TUGRcSoubi* TUGLiSoubi::MakeObj_Record()
  {
    TUGRcSoubi* newrecord = new TUGRcSoubi(player_);
    s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
    records_.push_back(newrecord);
    GenMakeObj_Record(newrecord, rh);

    return newrecord;
  }
  void TUGLiSoubi::Refresh_Custom_mc(s64 id_team, s64 mid_team, s64 id_shset, s32 posno, BOOL init_f, LONGVECTOR* id_shotvec)
  {
    s32 charid = pmgEO_->mgCoM_.GetCharID(mid_team, posno);
    s32 rarity = pmgEO_->mgCoM_.mdm_Team_.GetLineDt(mid_team, mdm_team_rarity);
    s32 steptype = pmgEO_->mgDt_.dtChar_.GetLv(clv_Speed, charid);
    //s32 openno = (VJOPENSTEP * DBMEMBER_ALL) + posno;//その歩数が開く番号
    s32 openno = (SLOTLV[(steptype * STEPNUM) + VJOPENSTEP] * DBMEMBER_ALL) + posno;//その歩数が開く番号
    s32 needover = pmgEO_->mgCoM_.mdm_Slot_.GetLineDt(rarity, (enmdm_Slot)(mdm_slot_slot00 + openno));//必要なオーバーラップ

    if (needover > pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_overlap))//レベル足りてない
    {
      TUGRcSoubi* newrecord = (TUGRcSoubi*)MakeObj_Record();
      newrecord->SetBaseDt(VJICONSTEP, openno);
      newrecord->SetOpenLvDt(needover);//素材ＩＤ
    }
    else
    {
      //マニュアル装備
      //s64 id_shot = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, (enmdTeam)(mdteam_id_shot00 + openno));
      TUGRcSoubi* newrecord = (TUGRcSoubi*)MakeObj_Record();
      newrecord->SetBaseDt(VJICONSTEP, openno);
      newrecord->SetShotDt(id_shotvec->at(openno), FALSE);//素材ＩＤ
    }

    //歩数
    s32 recno = 1;
    for (s32 step = 0; step < STEPNUM; ++step)
    {
      //デフォルト以外
      if (step != DEFSTEP)
      {
        s32 openno = (SLOTLV[(steptype * STEPNUM) + step] * DBMEMBER_ALL) + posno;//その歩数が開く番号
        s32 needover = pmgEO_->mgCoM_.mdm_Slot_.GetLineDt(rarity, (enmdm_Slot)(mdm_slot_slot00 + openno));//必要なオーバーラップ

        if (needover > pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_overlap))//レベル足りてない
        {
          TUGRcSoubi* newrecord = (TUGRcSoubi*)MakeObj_Record();
          newrecord->SetBaseDt(step, openno);
          newrecord->SetOpenLvDt(needover);//素材ＩＤ
        }
        else
        {
          //s64 id_shot = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, (enmdTeam)(mdteam_id_shot00 + openno));
          TUGRcSoubi* newrecord = (TUGRcSoubi*)MakeObj_Record();
          newrecord->SetBaseDt(step, openno);
          newrecord->SetShotDt(id_shotvec->at(openno), FALSE);//素材ＩＤ
        }
      }
      else//デフォルトシュート
      {
        TUGRcSoubi* newrecord = (TUGRcSoubi*)MakeObj_Record();
        newrecord->SetBaseDt(step, NGNUM);
        s32 defshot = pmgEO_->mgDt_.dtChar_.GetLv(clv_DefShotID, charid);
        newrecord->SetShotDt(defshot, TRUE);//デフォシュートＩＤ
      }
      ++recno;
    }
  }
  void TUGLiSoubi::Refresh_Custom(s64 id_team, s64 mid_team, s64 id_shset, s32 posno, BOOL init_f, LONGVECTOR* id_shotvec)
  {
    s32 charid = pmgEO_->mgCoM_.GetCharID(mid_team, posno);
    s32 rarity = pmgEO_->mgCoM_.mdm_Team_.GetLineDt(mid_team, mdm_team_rarity);
    s32 steptype = pmgEO_->mgDt_.dtChar_.GetLv(clv_Speed, charid);

    //s32 openno = (VJOPENSTEP * DBMEMBER_ALL) + posno;//その歩数が開く番号
    s32 openno = (SLOTLV[(steptype * STEPNUM) + VJOPENSTEP] * DBMEMBER_ALL) + posno;//その歩数が開く番号
    s32 needover = pmgEO_->mgCoM_.mdm_Slot_.GetLineDt(rarity, (enmdm_Slot)(mdm_slot_slot00 + openno));//必要なオーバーラップ

    if (needover > pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_overlap))//レベル足りてない
    {
      records_[0]->SetBaseDt(VJICONSTEP, openno);
      records_[0]->SetOpenLvDt(needover);//素材ＩＤ
    }
    else
    {
      //s64 id_shot = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, (enmdTeam)(mdteam_id_shot00 + openno));
      records_[0]->SetBaseDt(VJICONSTEP, openno);
      records_[0]->SetShotDt(id_shotvec->at(openno), FALSE);//素材ＩＤ
    }


    //歩数
    s32 recno = 1;
    for (s32 step = 0; step < STEPNUM; ++step)
    {
      //デフォルト以外
      if (step != DEFSTEP)
      {
        s32 openno = (SLOTLV[(steptype * STEPNUM) + step] * DBMEMBER_ALL) + posno;//その歩数が開く番号
        s32 needover = pmgEO_->mgCoM_.mdm_Slot_.GetLineDt(rarity, (enmdm_Slot)(mdm_slot_slot00 + openno));//必要なオーバーラップ

        if (needover > pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_overlap))//レベル足りてない
        {
          records_[recno]->SetBaseDt(step, openno);
          records_[recno]->SetOpenLvDt(needover);//素材ＩＤ
        }
        else
        {
          //s64 id_shot = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, (enmdTeam)(mdteam_id_shot00 + openno));
          records_[recno]->SetBaseDt(step, openno);
          records_[recno]->SetShotDt(id_shotvec->at(openno), FALSE);//素材ＩＤ
        }
      }
      ++recno;
    }
  }
  void TUGLiSoubi::Refresh_ShSet_mc(s64 id_team, s64 mid_team, s64 id_shset, s32 posno, BOOL init_f, LONGVECTOR* id_shotvec)
  {
    s32 charid = pmgEO_->mgCoM_.GetCharID(mid_team, posno);
    s32 rarity = pmgEO_->mgCoM_.mdm_Team_.GetLineDt(mid_team, mdm_team_rarity);
    s32 steptype = pmgEO_->mgDt_.dtChar_.GetLv(clv_Speed, charid);
    //s32 openno = (VJOPENSTEP * DBMEMBER_ALL) + posno;//その歩数が開く番号
    s32 openno = (SLOTLV[(steptype * STEPNUM) + VJOPENSTEP] * DBMEMBER_ALL) + posno;//その歩数が開く番号
    s32 needover = pmgEO_->mgCoM_.mdm_Slot_.GetLineDt(rarity, (enmdm_Slot)(mdm_slot_slot00 + openno));//必要なオーバーラップ

    if (needover > pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_overlap))//レベル足りてない
    {
      TUGRcSoubi* newrecord = (TUGRcSoubi*)MakeObj_Record();
      newrecord->SetBaseDt_ShSet(VJICONSTEP, openno);
      newrecord->SetOpenLvDt(needover);//素材ＩＤ
    }
    else
    {
      //s64 id_shot = pmgEO_->mgCoU_.mdShSet_.GetPKDt(stLiSoubi_.id_shset_, (enmdShSet)(mdshset_id_shot00 + openno));
      TUGRcSoubi* newrecord = (TUGRcSoubi*)MakeObj_Record();
      newrecord->SetBaseDt_ShSet(VJICONSTEP, openno);
      newrecord->SetShotDt_ShSet(id_shotvec->at(openno), FALSE);//素材ＩＤ
    }

    //歩数
    s32 recno = 1;
    for (s32 step = 0; step < STEPNUM; ++step)
    {
      //デフォルト以外
      if (step != DEFSTEP)
      {
        s32 openno = (SLOTLV[(steptype * STEPNUM) + step] * DBMEMBER_ALL) + posno;//その歩数が開く番号
        s32 needover = pmgEO_->mgCoM_.mdm_Slot_.GetLineDt(rarity, (enmdm_Slot)(mdm_slot_slot00 + openno));//必要なオーバーラップ

        if (needover > pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_overlap))//レベル足りてない
        {
          TUGRcSoubi* newrecord = (TUGRcSoubi*)MakeObj_Record();
          newrecord->SetBaseDt_ShSet(step, openno);
          newrecord->SetOpenLvDt(needover);//素材ＩＤ
        }
        else
        {
          //s64 id_shot = pmgEO_->mgCoU_.mdShSet_.GetPKDt(stLiSoubi_.id_shset_, (enmdShSet)(mdshset_id_shot00 + openno));
          TUGRcSoubi* newrecord = (TUGRcSoubi*)MakeObj_Record();
          newrecord->SetBaseDt_ShSet(step, openno);
          newrecord->SetShotDt_ShSet(id_shotvec->at(openno), FALSE);//素材ＩＤ
        }
      }
      else//デフォルトシュート
      {
        TUGRcSoubi* newrecord = (TUGRcSoubi*)MakeObj_Record();
        newrecord->SetBaseDt_ShSet(step, NGNUM);
        s32 defshot = pmgEO_->mgDt_.dtChar_.GetLv(clv_DefShotID, charid);
        newrecord->SetShotDt_ShSet(defshot, TRUE);//デフォシュートＩＤ
      }
      ++recno;
    }
  }
  void TUGLiSoubi::Refresh_ShSet(s64 id_team, s64 mid_team, s64 id_shset, s32 posno, BOOL init_f, LONGVECTOR* id_shotvec)
  {
    s32 charid = pmgEO_->mgCoM_.GetCharID(mid_team, posno);
    s32 rarity = pmgEO_->mgCoM_.mdm_Team_.GetLineDt(mid_team, mdm_team_rarity);
    s32 steptype = pmgEO_->mgDt_.dtChar_.GetLv(clv_Speed, charid);
    //s32 openno = (VJOPENSTEP * DBMEMBER_ALL) + posno;//その歩数が開く番号
    s32 openno = (SLOTLV[(steptype * STEPNUM) + VJOPENSTEP] * DBMEMBER_ALL) + posno;//その歩数が開く番号
    s32 needover = pmgEO_->mgCoM_.mdm_Slot_.GetLineDt(rarity, (enmdm_Slot)(mdm_slot_slot00 + openno));//必要なオーバーラップ

    if (needover > pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_overlap))//レベル足りてない
    {
      records_[0]->SetBaseDt_ShSet(VJICONSTEP, openno);
      records_[0]->SetOpenLvDt(needover);//素材ＩＤ
    }
    else
    {
      //s64 id_shot = pmgEO_->mgCoU_.mdShSet_.GetPKDt(stLiSoubi_.id_shset_, (enmdShSet)(mdshset_id_shot00 + openno));
      records_[0]->SetBaseDt_ShSet(VJICONSTEP, openno);
      records_[0]->SetShotDt_ShSet(id_shotvec->at(openno), FALSE);//素材ＩＤ
    }

    //歩数
    s32 recno = 1;
    for (s32 step = 0; step < STEPNUM; ++step)
    {
      //デフォルト以外
      if (step != DEFSTEP)
      {
        s32 openno = (SLOTLV[(steptype * STEPNUM) + step] * DBMEMBER_ALL) + posno;//その歩数が開く番号
        s32 needover = pmgEO_->mgCoM_.mdm_Slot_.GetLineDt(rarity, (enmdm_Slot)(mdm_slot_slot00 + openno));//必要なオーバーラップ

        if (needover > pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_overlap))//レベル足りてない
        {
          records_[recno]->SetBaseDt_ShSet(step, openno);
          records_[recno]->SetOpenLvDt(needover);//素材ＩＤ
        }
        else
        {
          //s64 id_shot = pmgEO_->mgCoU_.mdShSet_.GetPKDt(stLiSoubi_.id_shset_, (enmdShSet)(mdshset_id_shot00 + openno));
          records_[recno]->SetBaseDt_ShSet(step, openno);
          records_[recno]->SetShotDt_ShSet(id_shotvec->at(openno), FALSE);//素材ＩＤ
        }
      }
      ++recno;
    }
  }

  void TUGLiSoubi::SwitchEdit(BOOL edit_f)
  {
    for (auto& item : records_)
    {
      item->SwitchEdit(edit_f);
    }
    SetSelect(NGNUM);
  }
  void TUGLiSoubi::Refresh(s64 id_team, s64 mid_team, s64 id_shset, s32 posno, LONGVECTOR* id_shotvec, BOOL init_f)
  {

    SetDraw(TRUE);
    const s32 SLOTNUM = (mdteam_create_date - mdteam_id_shot00);

    //SetDraw(TRUE);
    //選択したメンバーのデフォルトシュート
    //選択したメンバーの現在空いているスロット
    //選択したメンバーに現在装備されているシュート
    //シュートインベントリ全部をさらってメンバー
    BOOL memchange_f = FALSE;
    if ((stLiSoubi_.id_team_ != id_team) || (stLiSoubi_.posno_ != posno) || init_f || mid::midIsDXL())
    {
      memchange_f = TRUE;
      s32 tmpidx = GetSelect();
      ClearRecord();//レコードクリアstLiSoubi_もクリアされてる
      SetSelect(tmpidx);

      stLiSoubi_.id_team_ = id_team;
      stLiSoubi_.posno_ = posno;
      stLiSoubi_.mid_team_ = mid_team;
      stLiSoubi_.id_shset_ = id_shset;
      SetZeroPos();
    }


    if (stLiSoubi_.id_shset_ == 0)
    {
      if (memchange_f)
      {
        Refresh_Custom_mc(id_team, mid_team, id_shset, posno, init_f, id_shotvec);
      }
      else
      {
        Refresh_Custom(id_team, mid_team, id_shset, posno, init_f, id_shotvec);
      }
    }
    else
    {
      if (memchange_f)
      {
        Refresh_ShSet_mc(id_team, mid_team, id_shset, posno, init_f, id_shotvec);
      }
      else
      {
        Refresh_ShSet(id_team, mid_team, id_shset, posno, init_f, id_shotvec);
      }
    }


  }


  void TUGLiSoubi::DoUpdateSub(BOOL draw_f)
  {

  }

  void TUGLiSoubi::DoActionSub()
  {
    //選ばれたレコード色づけ
    SetSelect();
  }
}
