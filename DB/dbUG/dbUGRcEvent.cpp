#include "dbUGRcEvent.h"
#include "dbUILayer.h"

namespace db
{
  const s32 RECORDH = 48;

  TUGRcEvent::TUGRcEvent(TUIGenLayer* player) : TUGRcGen(player)
  {
    
  }
  TUGRcEvent::~TUGRcEvent(void)
  {
  
  }
  s32 TUGRcEvent::DoMakeGroupSub()
  {
    //TUGRcGen::MakeGroupから呼ばれる
    basegrp_ = pmgUI_->GetUGGrp(ug_list_recordmem);
    basegrp2_ = pmgUI_->GetUGGrp(ug_list_recordmem);
    field_->o_SetObjGrp(basegrp_);
    name_ = MakeObj();
    banner_ = MakeObj();
    //nokori_ = MakeObj();

    items_.push_back(name_);
    items_.push_back(banner_);
    //items_.push_back(nokori_);

    for (s32 i = 0; i < items_.size(); ++i) items_[i]->o_SetParent(field_);

    //レコード高さを返す
    return RECORDH;
  }

  //チームマスターから情報を引いてくる
  void TUGRcEvent::SetDt(s64 mid_event)
  {
    mid_event_ = mid_event;
    //レイヤの画像をクリアしてるので、使い所によってはバッティングする
    //player_->ClearGrp();//pgrvec_をクリア
    s32 bmpno = pmgEO_->mgCoM_.mdm_Event_.GetPKDt(mid_event, mdm_event_bmpNo);
    std::string bannername = "ev_" + lib_str::IntToStr1b(bmpno, 5);//数値→1byte文字列
    //std::string bannername = "bn_" + lib_str::IntToStr1b(mid_event, 5);//数値→1byte文字列
    base::TGrp* evbanner = player_->LoadGrp("event", bannername.c_str());//pgrvec_の中に入る
    banner_->o_SetObjGrp(evbanner);
    std::string evname = pmgEO_->mgCoM_.mdm_Event_.GetPKDtStr(mid_event, mdm_event_name_c64);
    const s32 MAXLEN = 17;
    name_->f_MakeFont_Len(evname.c_str(), FC2_normal, MAXLEN);
    name_->z_SetPos(8, 33);
    //nokori_->z_SetPos(8,32);
  }

  s64 TUGRcEvent::GetMIDEvent()
  {
    return mid_event_;
  }


}
