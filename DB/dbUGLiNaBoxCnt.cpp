#include "dbUGLiNaBoxCnt.h"
#include "dbUILayer.h"
#include "dbUGRcNormal.h"


namespace db
{
  //コンストラクタ
  TUGLiNaBoxCnt::TUGLiNaBoxCnt(TUIGenLayer* player) :TUGLiGen(player)
    //, uglistend_(player)
  {
    stLiBoxCnt_.Init();
  }

  TUGLiNaBoxCnt::~TUGLiNaBoxCnt()
  {
    //解放時必ず呼ぶ
    ClearRecord();
  }

  //リストクリア（変数部分的に初期化、レコードとレコードエンド解放）
  void TUGLiNaBoxCnt::DoClearRecord()
  {
    stLiBoxCnt_.Init();
    for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);
    records_.clear();
  }

  void TUGLiNaBoxCnt::DoMakeGroupSub()
  {
    field_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_list_frameshop));//ショップの時は変える
    //uglistend_.MakeGroup_ListEnd(field_, sliderx_, slidery_);
    //uglistend_.SetDraw(FALSE);
  }

  //マスターガチャIDを入れてレコードをすべて作成
  void TUGLiNaBoxCnt::Refresh_NaBox(s64 mid_nabox)
  {
    ClearRecord();
    s64 id_nabox = pmgEO_->mgCoU_.GetNaBox_id(mid_nabox);
    s32 boxsize = pmgEO_->mgCoM_.mdm_NaBox_.GetPKDt(mid_nabox, mdm_nabox_boxSize);
    for (s32 i = 0; i < pmgEO_->mgCoM_.mdm_NaBoxCnt_.GetLineNum(); ++i)
    {
      //このなつかしボックスか
      if (pmgEO_->mgCoM_.mdm_NaBoxCnt_.GetLineDt(i, mdm_naboxcnt_id_m_nabox) == mid_nabox)
      {
        s64 mid_naboxcnt = pmgEO_->mgCoM_.mdm_NaBoxCnt_.GetLineDt(i, mdm_naboxcnt_pk);
        s32 no = pmgEO_->mgCoM_.mdm_NaBoxCnt_.GetLineDt(i, mdm_naboxcnt_boxNo);
        TUGRcNaBoxCnt* newrecord = new TUGRcNaBoxCnt(player_);
        s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
        newrecord->SetNaBoxCntDt(mid_naboxcnt, id_nabox);//mid_naboxcntをCommonに保存
        records_.push_back(newrecord);
        GenMakeObj_Record(newrecord, rh);
        ++no;
        if (no >= boxsize) break;
      }
    }
    
  }

  s64 TUGLiNaBoxCnt::GetMIDNaBoxCnt(s32 recordidx)
  {
    //レコードから懐かし中身MIDを取得
    return records_[GetActIndex()]->GetCommonID();
  }


  void TUGLiNaBoxCnt::DoUpdateSub(BOOL draw_f)
  {
    //uglistend_.Update(draw_f);
  }

  void TUGLiNaBoxCnt::DoActionSub()
  {
  }

}
