#include "dbUGPlateCredit.h"
namespace db
{

  const s32 TEXTNUM = 2;
  //コンストラクタ
  TUGPlateCredit::TUGPlateCredit(TUIGenLayer* player) :TUGGen(player)
    , ugrTechno_(player)
    , ugrStar_(player)
    , ugrTodd_(player)
    //, ugrGacha_(player)
    , ugrHuku_(player)
    , ugrText_(player)
  {

  }

  TUGPlateCredit::~TUGPlateCredit()
  {

  }

  void TUGPlateCredit::MakeGroup(TUIObj* parent)
  {
    SetParent(parent);
    field_->z_SetPos(-156, -56);

    field_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_list_panelavatar));
    field_->g_SetDepth(UIDP_06BUTTONHI);

    static const s32 POSX = 16;
    static const s32 POSY = 8;
    static const s32 DY = 16;

    ugrTechno_.MakeGroup_Techno(field_);
    ugrStar_.MakeGroup_Star(field_);
    ugrTodd_.MakeGroup_Todd(field_);
    //ugrGacha_.MakeGroup_Gacha(field_);
    ugrHuku_.MakeGroup_Huku(field_);
    ugrText_.MakeGroup(field_, 2);

    ugrTechno_.SetPos(POSX, POSY);
    ugrStar_.SetPos(POSX, POSY+ (DY * 1));
    ugrTodd_.SetPos(POSX, POSY + (DY * 2));
    //ugrGacha_.SetPos(POSX, POSY + (DY * 3));
    ugrHuku_.SetPos(POSX, POSY + (DY * 3));
    ugrText_.SetPos(8, POSY + (DY * 4) + 4);
    Refresh();

    SetDraw(TRUE);
  }

  void TUGPlateCredit::DoUpdate(BOOL draw_f)
  {
    field_->Update(draw_f);
    ugrTechno_.Update(draw_f);
    ugrStar_.Update(draw_f);
    ugrTodd_.Update(draw_f);
    //ugrGacha_.Update(draw_f);
    ugrHuku_.Update(draw_f);
    ugrText_.Update(draw_f);
  }

  void TUGPlateCredit::DoAction()
  {

  }

  void TUGPlateCredit::Refresh()
  {
    ugrTechno_.Refresh();
    ugrStar_.Refresh();
    ugrTodd_.Refresh();
    //ugrGacha_.Refresh();
    ugrHuku_.Refresh();
    //ugrText_.Refresh();

    STRVECTOR textvec;
    s32 hukumade = (XYMAG - (pmgEO_->mgCoU_.mdKantoku_.GetMyDt(mdkantoku_totalStar) % XYMAG));
    textvec.push_back("あと" + lib_str::IntToStr(hukumade) + "スター＿つかうと");
    textvec.push_back("ふくびきけん＿１まいプレゼント！");
    ugrText_.SetData(textvec);
  }

}
