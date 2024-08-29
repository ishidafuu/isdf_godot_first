#include "dbUGLiGaCnt.h"
#include "dbUILayer.h"
#include "dbUGRcNormal.h"

namespace db
{
  //コンストラクタ
  TUGLiGaCnt::TUGLiGaCnt(TUIGenLayer* player) :TUGLiGen(player), uglistend_(player)
  {
    stLiGaCnt_.Init();
  }

  TUGLiGaCnt::~TUGLiGaCnt()
  {
    //解放時必ず呼ぶ
    ClearRecord();
  }

  //リストクリア（変数部分的に初期化、レコードとレコードエンド解放）
  void TUGLiGaCnt::DoClearRecord()
  {
    stLiGaCnt_.Init();
    for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);
    records_.clear();
  }

  void TUGLiGaCnt::DoMakeGroupSub()
  {
    field_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_list_frameshop));

    uglistend_.MakeGroup_ListEnd_Shop(slider_, sliderx_, slidery_);
    uglistend_.SetDraw(FALSE);
  }
  
  void TUGLiGaCnt::Refresh(s32 cntgrpNo)
  {
    ClearRecord();
    SetZeroPos();//ゼロ位置に戻す

    s32 num = pmgEO_->mgCoM_.mdm_GachaCnt_.GetLineNum();
    for (s32 i = 0; i < num; ++i)
    {
      //同じグループ
      if (pmgEO_->mgCoM_.mdm_GachaCnt_.GetLineDt(i, mdm_gachacnt_cntGrpNo) != cntgrpNo) continue;

      TUGRcNormal* newrecord = new TUGRcNormal(player_);
      s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
      newrecord->SetShopRecord();
      enItemType itemtype = (enItemType)pmgEO_->mgCoM_.mdm_GachaCnt_.GetLineDt(i, mdm_gachacnt_itemType);
      s64 itemid = pmgEO_->mgCoM_.mdm_GachaCnt_.GetLineDt(i, mdm_gachacnt_itemID);
      s64 itemnum = pmgEO_->mgCoM_.mdm_GachaCnt_.GetLineDt(i, mdm_gachacnt_itemNum);

      //ガチャ中身MIDを保存しとく
      newrecord->SetCommonID(pmgEO_->mgCoM_.mdm_GachaCnt_.GetLineDt(i, mdm_gachacnt_pk));

      switch (itemtype)
      {
      case enItemType_Team: newrecord->ugname_.SetNameTeam_Cnt(itemid);break;
      case enItemType_Sozai:newrecord->ugname_.SetNameSozai2(itemid, FALSE); break;
      case enItemType_Shot: break;
      case enItemType_BGM:newrecord->ugname_.SetNameTape(itemid, FALSE); break;
      case enItemType_Haikei:newrecord->ugname_.SetNameHaikei(itemid);break;
      case enItemType_Supply:newrecord->ugname_.SetNameSupply(itemid, itemnum); break;
      case enItemType_Todd:newrecord->ugname_.SetNameTodd(itemnum); break;
      case enItemType_Star:newrecord->ugname_.SetNameStar(itemnum); break;
      case enItemType_Pack:newrecord->ugname_.SetNameItemPack(itemid, itemnum); break;
      case enItemType_Huku:newrecord->ugname_.SetNameHukuKen(itemnum); break;
      }
      newrecord->ugname_.SetIcon(TRUE);
      GenMakeObj_Record(newrecord, rh);
      
      records_.push_back(newrecord);
    }
  }

  void TUGLiGaCnt::DoUpdateSub(BOOL draw_f)
  {
    uglistend_.Update(draw_f);
  }

  void TUGLiGaCnt::DoActionSub()
  {

  }

  void TUGLiGaCnt::ChangeListEnd(BOOL listend_f)
  {
    uglistend_.SetDraw(listend_f);
    for (s32 i = 0; i < records_.size(); ++i) records_[i]->SetDraw(!listend_f);
  }

  void TUGLiGaCnt::SetListEnd(s32 recordidx)
  {
    //レコードからガチャ中身MIDを取得
    s64 mid_gacnt = records_[recordidx]->GetCommonID();
    uglistend_.SetDataGachaCnt(mid_gacnt);
  }

  s64 TUGLiGaCnt::GetCommonID()
{
    return records_[GetActIndex()]->GetCommonID();
  }

}
