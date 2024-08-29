#include "dbUGRCredit.h"
#include "dbUILayer.h"

namespace db
{
    const s32 POSY = 27;

    enum enCreditType
    {
        enCreditType_Techno,
        enCreditType_Todd,
        enCreditType_Star,
        enCreditType_Huku,
        enCreditType_Gacha,
    };

    //コンストラクタ
    TUGRCredit::TUGRCredit(TUIGenLayer* player) : TUGGen(player)//アクションなし
        , ugicon_(player)
    {
        stCredit_.Init();

    }

    TUGRCredit::~TUGRCredit()
    {

    }
    void TUGRCredit::GenMakeGroup(TUIObj* parent)
    {
        SetParent(parent);
        //SetPos(352 - 64, POSY);
        ugicon_.MakeGroup(field_);
        ugicon_.SetPos(0, 7);
        SetDraw(TRUE);
    }
    void TUGRCredit::MakeGroup_Techno(TUIObj* parent)
    {
        GenMakeGroup(parent);
        stCredit_.credittype_ = enCreditType_Techno;
        if (mid::midIsADR())
        {
            ugicon_.SetIcon(ugi_BuyMbCoin);
        }
        else
        {
            ugicon_.SetIcon(ugi_BuyTechno);
        }
    }
    void TUGRCredit::MakeGroup_Todd(TUIObj* parent)
    {
        GenMakeGroup(parent);
        stCredit_.credittype_ = enCreditType_Todd;
        ugicon_.SetIcon(ugi_Todd);
    }
    void TUGRCredit::MakeGroup_Star(TUIObj* parent)
    {
        GenMakeGroup(parent);
        stCredit_.credittype_ = enCreditType_Star;
        ugicon_.SetIcon(ugi_Star);
    }
    void TUGRCredit::MakeGroup_Huku(TUIObj* parent)
    {
        GenMakeGroup(parent);
        stCredit_.credittype_ = enCreditType_Huku;
        ugicon_.SetIcon(ugi_Huku);
    }
    void TUGRCredit::MakeGroup_Gacha(TUIObj* parent)
    {
        GenMakeGroup(parent);
        stCredit_.credittype_ = enCreditType_Gacha;
        ugicon_.SetIcon(ugi_GachaKen);
    }


    void TUGRCredit::Refresh()
    {
        std::string name = "";
        s32 num = 0;
        switch (stCredit_.credittype_)
        {
        case enCreditType_Techno:
            name = (mid::midIsADR())
                ? "モバコイン＿＿"
                : "しょじテクノ＿";

            num = mid::midResBalance();
            break;
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
            name = "ふくびきけん＿";
            num = (player_->pmgEO_->mgCoU_.mdKantoku_.GetMyDt(mdkantoku_haveHuku));
            break;
        case enCreditType_Gacha:
            name = "ガチャけん＿＿";
            num = (player_->pmgEO_->mgCoU_.mdKantoku_.GetMyDt(mdkantoku_haveHuku));
            break;
        }
        name += lib_str::IntToStr(num);
        field_->f_MakeFont((name).c_str(), FC2_normal);
        field_->f_FontPos(16, 0);
        //s32 posx = (name.size() * GRIDSIZE / 3);
    }


    void TUGRCredit::DoUpdate(BOOL draw_f)
    {
        field_->Update(draw_f);
        ugicon_.Update(draw_f);
    }

}
