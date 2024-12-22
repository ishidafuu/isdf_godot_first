#include "dbUGRHdCredit.h"
#include "dbUILayer.h"

namespace db
{
  const s32 POSY = 27;

  enum enCreditType
  {
    enCreditType_Todd,
    enCreditType_Star,
    enCreditType_Huku,
    enCreditType_Gacha,
  };

  //コンストラクタ
  TUGRHdCredit::TUGRHdCredit(TUIGenLayer* player) : TUGGen(player)//アクションなし
    , ugicon_(player)
  {
    stHdCredit_.Init();

  }

  TUGRHdCredit::~TUGRHdCredit()
  {

  }
  void TUGRHdCredit::GenMakeGroup(TUIObj* parent)
  {
    SetParent(parent);
    //SetPos(352 - 64, POSY);
    ugicon_.MakeGroup(field_);
    ugicon_.SetPos(-14, 7);
    SetDraw(FALSE);//デフォは消えてる
  }
  void TUGRHdCredit::MakeGroup_Todd(TUIObj* parent)
  {
    GenMakeGroup(parent);
    stHdCredit_.credittype_ = enCreditType_Todd;
    ugicon_.SetIcon(ugi_Todd);
  }
  void TUGRHdCredit::MakeGroup_Star(TUIObj* parent)
  {
    GenMakeGroup(parent);
    stHdCredit_.credittype_ = enCreditType_Star;
    ugicon_.SetIcon(ugi_Star);
  }
  void TUGRHdCredit::MakeGroup_Huku(TUIObj* parent)
  {
    GenMakeGroup(parent);
    stHdCredit_.credittype_ = enCreditType_Huku;
    ugicon_.SetIcon(ugi_Huku);
  }
  void TUGRHdCredit::MakeGroup_Gacha(TUIObj* parent)
  {
    GenMakeGroup(parent);
    stHdCredit_.credittype_ = enCreditType_Gacha;
    ugicon_.SetIcon(ugi_GachaKen);
  }

  s32 TUGRHdCredit::GenRefresh()
  {
    std::string name = "";
    s32 num = 0;
    switch (stHdCredit_.credittype_)
    {
    case enCreditType_Todd:
      name = "しょじトッド＿";
      num = (player_->pmgEO_->mgCoU_.mdKantoku_.GetMyDt(mdkantoku_haveTodd));
      break;
    case enCreditType_Star:
      name = "しょじスター＿";
      num = player_->pmgEO_->mgCoU_.mdKantoku_.GetMyDt(mdkantoku_haveFStar)
        + player_->pmgEO_->mgCoU_.mdKantoku_.GetMyDt(mdkantoku_haveBStar);
      break;
    case enCreditType_Huku:
      name = "しょじふくびきけん＿";
      num = (player_->pmgEO_->mgCoU_.mdKantoku_.GetMyDt(mdkantoku_haveHuku));
      break;
    }
    name += lib_str::IntToStr(num);
    field_->f_MakeFont(name.c_str(), FC2_normal);
    s32 posx = (name.size() * GRIDSIZE / 3);
    
    SetDraw(TRUE);

    return posx;
  }


  void TUGRHdCredit::Refresh()
  {
    s32 posx = GenRefresh();
    SetPos(372 - posx, POSY);
  }

  void TUGRHdCredit::RefreshMid()
  {
    s32 posx = GenRefresh();
    SetPos(372 - 160 - posx, POSY);
  }

  void TUGRHdCredit::RefreshGacha(s32 mid_gacha)
  {
    std::string name = "";
    s32 num = 0;
    switch (stHdCredit_.credittype_)
    {
    case enCreditType_Gacha:
      name = "しょじガチャチケット＿";
      num = pmgEO_->mgCoU_.GetGachaKenNum(mid_gacha);
      break;
    }
    name += lib_str::IntToStr(num);
    field_->f_MakeFont(name.c_str(), FC2_normal);
    s32 posx = (name.size() * GRIDSIZE / 3);
    SetPos(372 - posx, POSY);
    SetDraw(TRUE);
  }

  void TUGRHdCredit::DoUpdate(BOOL draw_f)
  {
    field_->Update(draw_f);
    ugicon_.Update(draw_f);
  }

}
