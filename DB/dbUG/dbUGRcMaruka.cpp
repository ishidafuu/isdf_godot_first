#include "dbUGRcMaruka.h"
namespace db
{
  TUGRcMaruka::TUGRcMaruka(TUIGenLayer* player) : TUGRcNormal(player)
    //ugbtn_eye_(player)
  {
    //自前アクション
    SetPlate(TRUE);
    stMaruka_.Init();
  }
  TUGRcMaruka::~TUGRcMaruka(void)
  {
  
  }
  void TUGRcMaruka::DoMakeGroupSub2()
  {
    //SetShopRecord();
    //ugbtn_eye_.MakeGroup(field_, ug_btn_eye, ug_btn_eye2);
    //ugbtn_eye_.SetPos(134, 12);

  }
  void TUGRcMaruka::DoSetLimY2(s32 limu, s32 hhh)
  {
    //ugbtn_eye_.GetField()->g_SetLimY(limu, hhh);
  }
  void TUGRcMaruka::DoUpdateSub2(BOOL draw_f)
  {
    //ugbtn_eye_.Update(draw_f);
  }
  void TUGRcMaruka::DoActionSub()
  {

    //if (ugbtn_eye_.IsAct())
    //{
    //  SetAct(enUGRcMaruka_Eye);
    //}
    //else 
    if (field_->b_IsDecide())//レコード
    {
      SetSelect(!(IsSelect()));
      SetAct(enUGRcMaruka_Field);//
      field_->g_Flash();//とりあえずここで明るく
    }
    else if (field_->b_IsEnter(TRUE))
    {
      field_->g_Bright();//とりあえずここで明るく
    }
    else
    {
      field_->g_EfReset();//とりあえずここで戻す
    }
    

  }
  void TUGRcMaruka::SetSozaiDt(s64 id_maruka)
  {

    if (mid::midIsDXL())
    {
      ugname_.SetNameSozai(id_maruka, 1, FALSE);
      std::string datestr = base::UnixtimeToDateString(1234567890, TRUE);
      SetSubValue(datestr.c_str());
	  SetSubValue2("ＫＩＧＥＮ");
      //SetValue("６７８９");
    }
    else
    {
      //素材ＩＤ保存（マスターではない）
      SetCommonID(id_maruka);


      s32 mid_maruka = pmgEO_->mgCoU_.mdlog_Maruka_.GetPKDt(id_maruka, mdlog_maruka_id_m_sozai) / SOZAIMID;
      s32 raerity = pmgEO_->mgCoU_.mdlog_Maruka_.GetPKDt(id_maruka, mdlog_maruka_id_m_sozai) % SOZAIMID;
      stMaruka_.id_maruka_ = id_maruka;
      stMaruka_.mid_maruka_ = mid_maruka;

      s64 keepdate = pmgEO_->mgCoU_.mdlog_Maruka_.GetPKDt(id_maruka, mdlog_maruka_keep_date);
      std::string datestr = base::UnixtimeToDateString(keepdate, TRUE);
      //Name
      ugname_.SetNameSozai(mid_maruka, raerity, FALSE);
      //SubValue
      SetSubValue(datestr.c_str());
	  SetSubValue2("ＫＩＧＥＮ");
    }

  }

}
