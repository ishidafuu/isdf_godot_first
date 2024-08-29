#include "dbUGLiMaruka.h"
#include "dbUILayer.h"

namespace db
{
  const s32 RECNUM = 10;

  //コンストラクタ
  TUGLiMaruka::TUGLiMaruka(TUIGenLayer* player) :TUGLiGen(player)
  {
    stLiMaruka_.Init();
  }

  TUGLiMaruka::~TUGLiMaruka()
  {
    //解放時必ず呼ぶ
    ClearRecord();
  }
  void TUGLiMaruka::DoMakeGroupSub()
  {
    ugbtn_subA_ = MakeSubBtn("すべてせんたく", ugi_AllCheck);
    ugbtn_subAC_ = MakeSubBtn("すべてかいじょ", ugi_AllClear);
  }
  //リストクリア（変数部分的に初期化、レコードとレコードエンド解放）
  void TUGLiMaruka::DoClearRecord()
  {
    stLiMaruka_.Init();
    for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);
    records_.clear();
  }

  TUGRcMaruka* TUGLiMaruka::MakeObj_Record()
  {
    TUGRcMaruka* newrecord = new TUGRcMaruka(player_);
    s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
    records_.push_back(newrecord);
    GenMakeObj_Record(newrecord, rh);

    return newrecord;
  }
  void TUGLiMaruka::Refresh(s32 pageno, s32 endpage)
  {

    ClearRecord();
    SetNamePage(pageno, endpage);//ページ

    for (s32 i = 0; i < pmgEO_->mgCoU_.mdlog_Maruka_.GetLineNum(); ++i)
    {
      TUGRcMaruka* newrecord = new TUGRcMaruka(player_);
      s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
      newrecord->SetSozaiDt(pmgEO_->mgCoU_.mdlog_Maruka_.GetLineDt(i, mdlog_maruka_pk));
      records_.push_back(newrecord);
      GenMakeObj_Record(newrecord, rh);
    }

    SetZeroPos();

  }

  LONGVECTOR TUGLiMaruka::GetIDOutSozai()
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


  void TUGLiMaruka::DoUpdateSub(BOOL draw_f)
  {

  }

  void TUGLiMaruka::DoActionSub()
  {

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
