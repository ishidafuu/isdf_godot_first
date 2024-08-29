#include "dbUGRcNormal.h"
#include "dbUGFinger.h"
namespace db
{


  TUGRcNormal::TUGRcNormal(TUIGenLayer* player) : TUGRcGen(player)
    , ugname_(player)
    , ugvalue_(player)
    , ugsubname_(player)
	, ugfinger_(NULL)
  {
    //subname_ = NULL;
    subname2_ = NULL;
    subvalue_ = NULL;
	subvalue2_ = NULL;
    id_common_ = 0;
    etcnum_ = 0;
    brink_c_ = 0;
  }
  TUGRcNormal::~TUGRcNormal(void)
  {
	  if(ugfinger_)
	  {
		  Freemem(ugfinger_);
	  }
  }
  s32 TUGRcNormal::DoMakeGroupSub()
  {
    //TUGRcGen::MakeGroupから呼ばれる
    basegrp_ = pmgUI_->GetUGGrp(ug_list_record);
    basegrp2_ = pmgUI_->GetUGGrp(ug_list_record2);
    field_->o_SetObjGrp(basegrp_);

    ugname_.MakeGroup(field_);
    ugname_.SetPos(POSXL, POSY);

    ugvalue_.MakeGroup(field_);
    ugvalue_.SetPos(POSXR, POSY);
    ugvalue_.GetField()->f_SetRightShift();

    ugsubname_.MakeGroup(field_);
    ugsubname_.SetPos(POSXL, SUBY);

    //継承先呼ぶ
    DoMakeGroupSub2();

    return RECORDH;//レコード高さを返す
  }
  void TUGRcNormal::SetShopRecord()
  {
    basegrp_ = pmgUI_->GetUGGrp(ug_list_recordshop);
    basegrp2_ = pmgUI_->GetUGGrp(ug_list_record2);
    field_->o_SetObjGrp(basegrp_);
  }

  void TUGRcNormal::DoSetLimY(s32 limu, s32 hhh)
  {
    ugname_.SetLimY(limu, hhh);
    ugvalue_.SetLimY(limu, hhh);
    ugsubname_.SetLimY(limu, hhh);
    //if (subname_ != NULL) subname_->g_SetLimY(limu, hhh);
    if (subvalue_ != NULL) subvalue_->g_SetLimY(limu, hhh);
	if (subvalue2_ != NULL) subvalue2_->g_SetLimY(limu, hhh);
	if (subname2_ != NULL) subname2_->g_SetLimY(limu, hhh);

    DoSetLimY2(limu, hhh);
  }


  void TUGRcNormal::DoUpdateSub(BOOL draw_f)
  {
    ugname_.Update(draw_f);
    ugvalue_.Update(draw_f);
    ugsubname_.Update(draw_f);
    if (subvalue_ != NULL) subvalue_->Update(draw_f);
	if (subvalue2_ != NULL) subvalue2_->Update(draw_f);
    if (subname2_ != NULL) subname2_->Update(draw_f);
	if (ugfinger_ != NULL) ugfinger_->Update(draw_f);
    DoUpdateSub2(draw_f);
  }


  void TUGRcNormal::SetName(const char* name)
  {
    ugname_.SetName(name);
  }
  void TUGRcNormal::SetNameIcon(const char* name, enUGPIcon iconno)
  {
    ugname_.SetNameIcon(name, iconno);
  }

  void TUGRcNormal::SetValue(const char* value)
  {
    ugvalue_.SetName(value);
  }
  void TUGRcNormal::SetValue_Star(s32 value)
  {
    ugvalue_.SetValueStar(value);
  }
  void TUGRcNormal::SetValue_Todd(s32 value)
  {
    ugvalue_.SetValueTodd(value);
  }
  void TUGRcNormal::SetValue_Huku(s32 value)
  {
    ugvalue_.SetValueHukuKen(value);
  }
  void TUGRcNormal::SetValue_Gacha(s32 value)
  {
    ugvalue_.SetValueGachaKen(value);
  }
  void TUGRcNormal::SetValue_Coach(s64 mid_supply, s32 value)
  {
	  ugvalue_.SetValueCoachKen(mid_supply, value);
  }
  void TUGRcNormal::SetSubName(const char* subname)
  {
    ugsubname_.SetMessage_mini(subname, FC2_mini, FALSE);
  }
  void TUGRcNormal::SetSubName(const char* subname, s32 fontcol, BOOL brink_f)
  {
    ugsubname_.SetMessage_mini(subname, fontcol, brink_f);
  }
  void TUGRcNormal::SetSubName2(const char* subname)
  {
    subname2_ = MakeObj();
    subname2_->o_SetParent(field_);
    subname2_->f_MakeFont_Mini(subname, FC2_mini);
    subname2_->z_SetPos(POSXL, SUBY - 0);
  }
  void TUGRcNormal::SetSubName2(const char* subname, s32 fontcol)
  {
    subname2_ = MakeObj();
    subname2_->o_SetParent(field_);
    subname2_->f_MakeFont_Mini(subname, fontcol);
    subname2_->z_SetPos(POSXL, SUBY - 0);
  }
  void TUGRcNormal::SetSubValue(const char* subvalue)
  {
    subvalue_ = MakeObj();
    subvalue_->o_SetParent(field_);
    subvalue_->f_MakeFont_Mini(subvalue, FC2_mini);
    subvalue_->f_SetRightShift();
    subvalue_->z_SetPos(POSXR, SUBY);
  }
  void TUGRcNormal::SetSubValue2(const char* subvalue)
  {
	  subvalue2_ = MakeObj();
	  subvalue2_->o_SetParent(field_);
	  subvalue2_->f_MakeFont_Mini(subvalue, FC2_mini);
	  //subvalue2_->f_SetRightShift();
	  subvalue2_->z_SetPos(POSXL, SUBY);
  }
  void TUGRcNormal::SetupFinger(BOOL draw_f, f32 offsetX, f32 offsetY)
  {
	  // ないなら作る
	  if(!ugfinger_)
	  {
		  s32 x = offsetX;
		  s32 y = offsetY;
		  ugfinger_ = new TUGFinger(player_);
		  ugfinger_->MakeGroup(field_);
		  ugfinger_->SetPos(x,y);
		  f32 limY = GetField()->stSO_.stGrp_.limU_;
		  f32 hhh = GetField()->stSO_.stGrp_.limD_ - limY;
		  ugfinger_->GetField()->g_SetLimY(limY, hhh);
		  ugfinger_->SetMotionLookPoint(x,y);
	  }
	  ugfinger_->SetDraw(draw_f);
  }
  void TUGRcNormal::SetCommonID(s64 id_common)
  {
    id_common_ = id_common;
  }
  s64 TUGRcNormal::GetCommonID()
  {
    return id_common_;
  }
  void TUGRcNormal::SetEtcNum(s64 etcnum)
  {
    etcnum_ = etcnum;
  }
  s64 TUGRcNormal::GetEtcNum()
  {
    return etcnum_;
  }
  void TUGRcNormal::SetData(LONGVECTOR data)
  {
	  data_.clear();
	  for (s32 i = 0; i < data.size(); i++)
	  {
		  data_.push_back(data[i]);
	  }
  }
  LONGVECTOR TUGRcNormal::GetData(void)
  {
	  return data_;
  }
}
