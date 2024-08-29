#include "dbUGLiShSet.h"
#include "dbUILayer.h"
#include "dbUGRcNormal.h"

namespace db
{
  //コンストラクタ
  TUGLiShSet::TUGLiShSet(TUIGenLayer* player) :TUGLiGen(player)
  {
    stLiShSet_.Init();
  }

  TUGLiShSet::~TUGLiShSet()
  {
    //解放時必ず呼ぶ
    ClearRecord();
  }
  void TUGLiShSet::DoMakeGroupSub()
  {
    ugbtn_subRename_ = MakeSubBtn("なまえへんこう", ugi_EditProf);
    ugbtn_subAuto_ = MakeSubBtn("オートせんたく", ugi_ShSetAuto);
    ugbtn_subClear_ = MakeSubBtn("すべてはずす", ugi_ShSetClear);
    ugradio_sub_.SetRadio(FALSE);
  }
  //リストクリア（変数部分的に初期化、レコードとレコードエンド解放）
  void TUGLiShSet::DoClearRecord()
  {
    stLiShSet_.Init();
    for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);
    records_.clear();
  }

  TUGRcShSet* TUGLiShSet::MakeObj_Record()
  {
    TUGRcShSet* newrecord = new TUGRcShSet(player_);
    s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
    records_.push_back(newrecord);
    GenMakeObj_Record(newrecord, rh);

    return newrecord;
  }
  void TUGLiShSet::Refresh(s64 id_shset, LONGVECTOR* id_shotvec, BOOL init_f)
  {
    SetDraw(TRUE);
  
    if (init_f)
    {
      id_shotvec->clear();
      ClearRecord();//レコードクリアstLiSoubi_もクリアされてる
      stLiShSet_.id_shset_ = id_shset;
      for (s32 i = 0; i < SLOTALL; ++i)
      {
        s64 id_shot = pmgEO_->mgCoU_.mdShSet_.GetPKDt(id_shset, (enmdShSet)(mdshset_id_shot00 + i));
        TUGRcShSet* newrecord = (TUGRcShSet*)MakeObj_Record();
        newrecord->SetBaseDt(i);
        newrecord->SetShotDt(id_shot);//素材ＩＤ
        id_shotvec->push_back(id_shot);
      }
      SetZeroPos();
    }
    else
    {
      s32 slot = 0;
      for (auto& id_shot : *id_shotvec)
      {
        records_[slot]->SetShotDt(id_shot);//素材ＩＤ
        ++slot;
      }
    }
  }
  void TUGLiShSet::Rename(s32 slot, s64 id_shot)
  {
    records_[slot]->SetShotDt(id_shot);//素材ＩＤ
  }


  void TUGLiShSet::DoUpdateSub(BOOL draw_f)
  {

  }

  void TUGLiShSet::DoActionSub()
  {
    //選ばれたレコード色づけ
    //SetSelect();
  }
}
