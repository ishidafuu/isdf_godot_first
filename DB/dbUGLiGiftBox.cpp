#include "dbUGLiGiftBox.h"
#include "dbUILayer.h"

namespace db
{
  const s32 RECNUM = 10;

  //コンストラクタ
  TUGLiGiftBox::TUGLiGiftBox(TUIGenLayer* player) :TUGLiGen(player)
  {
    stLiGiftBox_.Init();
  }

  TUGLiGiftBox::~TUGLiGiftBox()
  {
    //解放時必ず呼ぶ
    ClearRecord();
  }
  void TUGLiGiftBox::DoMakeGroupSub()
  {
    ugbtn_subA_ = MakeSubBtn("すべてせんたく", ugi_AllCheck);
    ugbtn_subAC_ = MakeSubBtn("すべてかいじょ", ugi_AllClear);
  }
  //リストクリア（変数部分的に初期化、レコードとレコードエンド解放）
  void TUGLiGiftBox::DoClearRecord()
  {
    stLiGiftBox_.Init();
    for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);
    records_.clear();
  }

  TUGRcGiftBox* TUGLiGiftBox::MakeObj_Record()
  {
    TUGRcGiftBox* newrecord = new TUGRcGiftBox(player_);
    s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
    records_.push_back(newrecord);
    GenMakeObj_Record(newrecord, rh);

    return newrecord;
  }

  void TUGLiGiftBox::Refresh(s32 pageno)
  {
    //各レコードクリアして入れる
    ClearRecord();
    SetNamePage(pageno, GetMaxPage());//ページ

    s32 stno = (pageno * RECNUM);
    s32 edno = (stno + RECNUM);

    s32 sozainum = 0;
    LONGVECTOR giftvec = pmgEO_->mgCoU_.GetGiftBox();

    if (giftvec.size() <= edno)
    {
      edno = giftvec.size();
      //stno = (edno - RECNUM);
      //if (stno < 0) stno = 0;
    }

    s32 viewno = 0;
    for (s32 i = 0; i < pmgEO_->mgCoU_.mdGiftbox_.GetLineNum(); ++i)
    {
      if ((viewno >= stno) && (viewno < edno))
      {
        TUGRcGiftBox* newrecord = MakeObj_Record();
        s64 id_giftbox = (pmgEO_->mgCoU_.mdGiftbox_.GetLineDt(i, mdgiftbox_pk));
        newrecord->SetGiftBoxDt(id_giftbox);
      }

      if (viewno >= edno) break;

      ++viewno;
    }


    if (mid::midIsDXL())
    {
      for (s32 i = 0; i < 10; ++i)
      {
        TUGRcGiftBox* newrecord = MakeObj_Record();
        //s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
        newrecord->SetGiftBoxDt(i + (RECNUM * pageno));
      }
    }

    SetZeroPos();
    SetDraw(TRUE);

  }

  s32 TUGLiGiftBox::GetMaxPage()
  {
    if (mid::midIsDXL())
    {
      return 1;
    }
    else if (pmgEO_->mgCoU_.mdGiftbox_.GetLineNum() == 0)
    {
      return 0;
    }
    else
    {
      return ((pmgEO_->mgCoU_.mdGiftbox_.GetLineNum() - 1) / RECNUM);
    }
    
  }
  LONGVECTOR TUGLiGiftBox::GetIDOpen()
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


  void TUGLiGiftBox::DoUpdateSub(BOOL draw_f)
  {

  }

  void TUGLiGiftBox::DoActionSub()
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
    }
  }

}
