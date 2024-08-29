#include "dbUGRc2Gen.h"
namespace db
{
  const s32 TEXTNUM = 5;
  TUGRc2Gen::TUGRc2Gen(TUIGenLayer* player) : TUGRcGen(player)
    , ugbtn_ok_(player)
    , ugname_(player)
    , ugvalue_(player)
    , ugrprof_(player)
  {
    SetPlate(TRUE);
    prof_f_ = FALSE;
  }
  TUGRc2Gen::~TUGRc2Gen(void)
  {
    //for (s32 i = 0; i < text_.size(); ++i) Freemem(text_[i]);
  }
  s32 TUGRc2Gen::DoMakeGroupSub()
  {
    //TUGRcGen::MakeGroupから呼ばれる
    basegrp_ = pmgUI_->GetUGGrp(ug_list_panel);
    basegrp2_ = pmgUI_->GetUGGrp(ug_list_panel);
    field_->o_SetObjGrp(basegrp_);
    field_->b_SetBtn(FALSE);//リストエンド自体はボタンにならないように

    static const s32 NAMEX = 28;
    static const s32 NAMEY = 8;
    static const s32 VALUEX = 138;

    //ugname_.MakeGroup(field_);
    //ugname_.SetPos(NAMEX, NAMEY);
    ugname_.MakeGroup(field_);
    ugname_.SetPos(POSXL, POSY);

    ugvalue_.MakeGroup(field_);
    ugvalue_.SetPos(POSXR, POSY);
    ugvalue_.GetField()->f_SetRightShift();

    //value_ = MakeObj();
    //ugvalue_.o_SetParent(field_);
    //ugvalue_.f_SetRightShift();
    //ugvalue_.z_SetPos(VALUEX, NAMEY);

    const s32 TEXTY = 24;
    const s32 DISTY = 12;
    const s32 TEXTX = 12;

    for (s32 i = 0; i < TEXTNUM; ++i)
    {
      TUIObj* newtext = MakeObj();
      newtext->o_SetParent(field_);
      newtext->z_SetPos(TEXTX, TEXTY + (DISTY * i));
      text_.push_back(newtext);
    }


    const s32 DATAX = 20;
    const s32 DATAY = TEXTY + (DISTY * 3) + 4;
    const s32 DATAY2 = DATAY + DISTY;
    dttitle_ = MakeObj();
    dttitle_->o_SetParent(field_);
    dttitle_->z_SetPos(DATAX, DATAY);

    dtvalue_ = MakeObj();
    dtvalue_->o_SetParent(field_);
    dtvalue_->f_SetRightShift();
    dtvalue_->z_SetPos(VALUEX, DATAY);

    dttitle2_ = MakeObj();
    dttitle2_->o_SetParent(field_);
    dttitle2_->z_SetPos(DATAX, DATAY2);

    dtvalue2_ = MakeObj();
    dtvalue2_->o_SetParent(field_);
    dtvalue2_->f_SetRightShift();
    dtvalue2_->z_SetPos(VALUEX, DATAY2);

    ugbtn_ok_.MakeGroup(field_, ug_btn_ok);
    ugbtn_ok_.SetPos(72, 96);
    ugbtn_ok_.SetDraw(FALSE);

    ugrprof_.MakeGroup(field_);

    //継承先呼ぶ
    DoMakeGroupSub2();

    return RECORDH;//レコード高さを返す(いみなし)
  }
  void TUGRc2Gen::DoSetLimY(s32 limu, s32 hhh)
  {
    ugname_.SetLimY(limu, hhh);
    ugvalue_.SetLimY(limu, hhh);
    dttitle_->g_SetLimY(limu, hhh);
    dtvalue_->g_SetLimY(limu, hhh);
    dttitle2_->g_SetLimY(limu, hhh);
    dtvalue2_->g_SetLimY(limu, hhh);
    for (s32 i = 0; i < text_.size(); ++i) text_[i]->g_SetLimY(limu, hhh);
    
    DoSetLimY2(limu, hhh);
  }

  void TUGRc2Gen::DoUpdateSub(BOOL draw_f)
  {
    if (prof_f_)
    {
      ugrprof_.Update(draw_f);
    }
    else
    {
      ugname_.Update(draw_f);
      ugvalue_.Update(draw_f);
      dttitle_->Update(draw_f);
      dtvalue_->Update(draw_f);
      dttitle2_->Update(draw_f);
      dtvalue2_->Update(draw_f);
      for (s32 i = 0; i < text_.size(); ++i) text_[i]->Update(draw_f);
      ugbtn_ok_.Update(draw_f);
    }
    DoUpdateSub2(draw_f);
  }

  void TUGRc2Gen::DoActionSub()
  {
    if (ugbtn_ok_.IsAct())
    {
      SetAct(0);
    }
  }
  void TUGRc2Gen::UndrawAll()
  {
    ugname_.SetDraw(FALSE);
    ugvalue_.SetDraw(FALSE);
    dttitle_->g_SetDraw(FALSE);
    dtvalue_->g_SetDraw(FALSE);
    dttitle2_->g_SetDraw(FALSE);
    dtvalue2_->g_SetDraw(FALSE);
    for (s32 i = 0; i < text_.size(); ++i) text_[i]->g_SetDraw(FALSE);
    ugbtn_ok_.SetDraw(FALSE);
  }
  
  //void TUGRc2Gen::GenSetValue(const char* value)
  //{
  //  ugvalue_.f_MakeFont(value, FC2_normal);
  //  ugvalue_.g_SetDraw(TRUE);
  //}

  void TUGRc2Gen::SetValue(const char* value)
  {
    ugvalue_.SetName(value);
    ugvalue_.SetDraw(TRUE);
  }
  void TUGRc2Gen::SetValue_Star(s32 value)
  {
    ugvalue_.SetValueStar(value);
    ugvalue_.SetDraw(TRUE);
  }
  void TUGRc2Gen::SetValue_Todd(s32 value)
  {
    ugvalue_.SetValueTodd(value);
    ugvalue_.SetDraw(TRUE);
  }
  void TUGRc2Gen::SetValue_Huku(s32 value)
  {
    ugvalue_.SetValueHukuKen(value);
    ugvalue_.SetDraw(TRUE);
  }
  void TUGRc2Gen::SetValue_Gacha(s32 value)
  {
    ugvalue_.SetValueGachaKen(value);
    ugvalue_.SetDraw(TRUE);
  }
  void TUGRc2Gen::GenSetData(s32 dtno, const char* dttitle, const char* dtvalue)
  {
    if (dtno == 0)
    {
      dttitle_->f_MakeFont(dttitle, FC2_normal);
      dtvalue_->f_MakeFont(dtvalue, FC2_normal);
      dttitle_->g_SetDraw(TRUE);
      dtvalue_->g_SetDraw(TRUE);
    }
    else
    {
      dttitle2_->f_MakeFont(dttitle, FC2_normal);
      dtvalue2_->f_MakeFont(dtvalue, FC2_normal);
      dttitle2_->g_SetDraw(TRUE);
      dtvalue2_->g_SetDraw(TRUE);
    }

  }

  void TUGRc2Gen::GenSetText(STRVECTOR textvec)
  {
    for (s32 i = 0; i < text_.size(); ++i) text_[i]->g_SetDraw(FALSE);
    for (s32 i = 0; i < textvec.size(); ++i)
    {
      text_[i]->f_MakeFont(textvec[i].c_str(), FC2_normal);
      text_[i]->g_SetDraw(TRUE);
    }
  }
  void TUGRc2Gen::GenSetButton(const char* btntitle)
  {
    ugbtn_ok_.SetFont(btntitle, FC2_normal);
    ugbtn_ok_.SetDraw(TRUE);
  }

}
