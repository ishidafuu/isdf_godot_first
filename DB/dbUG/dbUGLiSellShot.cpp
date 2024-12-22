#include "dbUGLiSellShot.h"
#include "dbUILayer.h"

namespace db
{
  const s32 RECNUM = 10;

  //コンストラクタ
  TUGLiSellShot::TUGLiSellShot(TUIGenLayer* player) :TUGLiGen(player)
  {
    stLiSellShot_.Init();
  }

  TUGLiSellShot::~TUGLiSellShot()
  {
    //解放時必ず呼ぶ
    ClearRecord();
  }
  void TUGLiSellShot::DoMakeGroupSub()
  {
    field_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_list_frameshop));//ショップの時は変える

    ugbtn_subA_ = MakeSubBtn("すべてせんたく", ugi_AllCheck);
    ugbtn_subAC_ = MakeSubBtn("すべてかいじょ", ugi_AllClear);
  }
  //リストクリア（変数部分的に初期化、レコードとレコードエンド解放）
  void TUGLiSellShot::DoClearRecord()
  {
    stLiSellShot_.Init();
    for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);
    records_.clear();
  }

  TUGRcSellShot* TUGLiSellShot::MakeObj_Record()
  {
    TUGRcSellShot* newrecord = new TUGRcSellShot(player_);
    s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
    newrecord->SetShopRecord();
    records_.push_back(newrecord);
    GenMakeObj_Record(newrecord, rh);

    return newrecord;
  }

  BOOL TUGLiSellShot::Refresh(s32 pageno, LONGVECTOR* id_shotvec)
  {
    //各レコードクリアして入れる
    ClearRecord();

    s32 stno = (pageno * RECNUM);
    s32 edno = (stno + RECNUM);

    if (id_shotvec->size() == 0)
    {
      return FALSE;
    }

    if (edno >= id_shotvec->size())
    {
      edno = id_shotvec->size();
    }

    s32 viewno = 0;
    for (s32 i = 0; i < id_shotvec->size(); ++i)
    {
      if ((viewno >= stno) && (viewno < edno))
      {
        TUGRcSellShot* newrecord = MakeObj_Record();
        s64 id_shot = (pmgEO_->mgCoU_.mdShot_.GetPKDt(id_shotvec->at(i), mdshot_pk));
        newrecord->SetShotDt(id_shot);
      }

      if (viewno >= edno) break;

      ++viewno;
    }


    if (mid::midIsDXL())
    {
      for (s32 i = 0; i < 10; ++i)
      {
        TUGRcSellShot* newrecord = MakeObj_Record();
        newrecord->SetShotDt(i + (RECNUM * pageno));
      }
    }
    stLiSellShot_.maxpage_ = ((id_shotvec->size() - 1) / RECNUM);


    SetZeroPos();
    SetDraw(TRUE);


    return TRUE;
  }

  s32 TUGLiSellShot::GetMaxPage()
  {
    if (mid::midIsDXL())
    {
      return 1;
    }
    else
    {
      return stLiSellShot_.maxpage_;
    }

  }
  LONGVECTOR TUGLiSellShot::GetIDSellShot()
  {
    LONGVECTOR res;
    res.clear();
    INTVECTOR slvec = GetSelectedRec();
    for (s32 i = 0; i < slvec.size(); ++i)
    {
      res.push_back(records_[slvec[i]]->GetCommonID());
    }

    return res;
  }


  void TUGLiSellShot::DoUpdateSub(BOOL draw_f)
  {

  }

  void TUGLiSellShot::DoActionSub()
  {
    //選ばれたレコード色づけ
    //SetSelect();

    if (IsActRes(enUGLiAct_SubA))
    {
      for (s32 i = 0; i < records_.size(); ++i) records_[i]->SetSelect(TRUE);
    }
    else if (IsActRes(enUGLiAct_SubB))
    {
      for (s32 i = 0; i < records_.size(); ++i) records_[i]->SetSelect(FALSE);
      stLiSellShot_.id_shot_last_ = 0;
    }
    else if (IsActRes(enUGLiAct_Record))
    {
      //最後に選択されたＩＤ
      stLiSellShot_.id_shot_last_ = records_[GetActIndex()]->GetCommonID();
    }
  }

  s64 TUGLiSellShot::GetIDShotLast()
  {
    return stLiSellShot_.id_shot_last_;
  }

}
