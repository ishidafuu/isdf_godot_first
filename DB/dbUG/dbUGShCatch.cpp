#include "dbUGShCatch.h"
#include "dbUIGenLayer.h"
#include "dbGame.h"
#include "dbUGShKantoku.h"

namespace db
{
  const s32 MARUNUM = 3;
  //コンストラクタ
  TUGShCatch::TUGShCatch(TUIGenLayer* player) :TUGGen(player)
  {
    stShCatch_.Init();
  }

  TUGShCatch::~TUGShCatch()
  {
    //for (s32 i = 0; i < maru_.size(); ++i) Freemem(maru_[i]);
  }

  void TUGShCatch::MakeGroup(TUIObj* parent, TUGShKantoku* ugshkantoku)
  {
    ugshkantoku_ = ugshkantoku;

    SetParent(parent);

    field_->g_SetDepth(UIDP_05BUTTON);
    SetPos(-160, -80);

    //キャッチ土台
    catch_ = MakeObj();
    catch_->o_SetParent(field_);
    catch_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_game_cacth));
    //catch_->g_SetFrontDepth();//最前面
    //catch_->z_SetPos_SafeL(0, 0);
    catch_->g_SetAlp(TRUE, 200);




    barCa_ = MakeObj();
    barCa_->o_SetParent(catch_);
    barCa_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_game_bar));
    barCa_->g_SetPartNo(1);
    barCa_->z_SetPos(8, 13);

    barNG_ = MakeObj();
    barNG_->o_SetParent(barCa_);
    barNG_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_game_bar));
    barNG_->g_SetPartNo(0);
    //barNG_->z_SetPos(8, 13);

    barNCa_ = MakeObj();
    barNCa_->o_SetParent(barCa_);
    barNCa_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_game_bar));
    barNCa_->g_SetPartNo(2);
    //barNCa_->z_SetPos(8, 13);


    //○
    for (s32 i = 0; i < MARUNUM; ++i)
    {
      TUIObj* newmaru = MakeObj();
      maru_.push_back(newmaru);
      newmaru->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_game_maru));
      newmaru->o_SetParent(catch_);
      newmaru->z_SetPos(51 + (32 * i), 3);
      newmaru->g_SetPartNo(i);
      newmaru->g_SetDraw(FALSE);//ひつよう
    }
  }

  void TUGShCatch::Refresh()
  {
    stShCatch_.shotDraw_f_ = (player_->pGame_->mgSG_.stTm_[mid::midGetMyIdx()].semi_Shot_c_ > 0);
    stShCatch_.catchBar_c_ = player_->pGame_->mgSG_.stTm_[mid::midGetMyIdx()].semi_Catch_c_;
    stShCatch_.catchMaru_c_ = player_->pGame_->mgSG_.stTm_[mid::midGetMyIdx()].semi_Catching_c_;
    stShCatch_.offence_f_ = player_->pGame_->mgSG_.stTm_[mid::midGetMyIdx()].ofside_f;
    stShCatch_.catchPos_ = player_->pGame_->mgSG_.stTm_[mid::midGetMyIdx()].semi_CaPos_;
    stShCatch_.ncaLen_ = player_->pGame_->mgSG_.stTm_[mid::midGetMyIdx()].semi_NCaLen_;
    //キャッチゲージ
    //bar_->g_SetSrcW(stShCatch_.catchBar_c_ * 2);

    //if (stShCatch_.catchBar_c_ == 0) bar_->g_SetSrcW(64 * 2);

    if (stShCatch_.catchMaru_c_ > 0)
    {
      ugshkantoku_->SerifuSelect(ks_catch);
      //s32 maru = ((stShCatch_.catchBar_c_ + 1) / 16) - 1;
      //if (maru >= MARUNUM) maru = (MARUNUM - 1);
      //if (maru < 0) maru = 0;

      //BOOL marudraw_f = ((stShCatch_.catchMaru_c_ / 4) % 2 == 1);
      //maru_[maru]->g_SetDraw(marudraw_f);
    }
    else if (stShCatch_.offence_f_)//オフェンス
    {
      //シュート命令
      if (stShCatch_.shotDraw_f_)
      {
        ugshkantoku_->SerifuSelect(ks_shot);
      }
      else
      {
        ugshkantoku_->SerifuSelect(ks_shotwait);
      }
    }
    else//ディフェンス、ルーズボール
    {
      if (stShCatch_.catchBar_c_ == 0)
      {
        ugshkantoku_->SerifuSelect(ks_catchwait);
      }
      else
      {
        ugshkantoku_->SerifuSelect(ks_catch);
      }

      barCa_->g_SetDraw(TRUE);
      //barCa2_->g_SetDraw(TRUE);
      barNCa_->g_SetDraw(TRUE);
      barNG_->g_SetDraw(TRUE);
    }


    const s32 LEN = pmgEO_->mgDt_.dtSet_.GetDtInput(setSemiCaLen);
    s32 BASE = (LEN - pmgEO_->mgDt_.dtSet_.GetDtInput(setSemiCaBase)) - stShCatch_.catchPos_;
    if (BASE < 0)
    {
        BASE = 0;
    }
    const s32 NICE = (LEN - pmgEO_->mgDt_.dtSet_.GetDtInput(setSemiCaBase)) + pmgEO_->mgDt_.dtSet_.GetDtInput(setSemiCaNice);

    s32 catchBar_c = (stShCatch_.catchBar_c_ == 0)
        ? LEN
        : stShCatch_.catchBar_c_;

    barCa_->g_SetDraw(TRUE);
    barCa_->g_SetSrcW(catchBar_c * 2);

    BOOL ng_f = (BASE > 0);
    s32 nglen = lib_num::Min(catchBar_c, BASE);
    barNG_->g_SetDraw(ng_f);
    barNG_->g_SetSrcW(nglen * 2);


    BOOL nca_f = (catchBar_c > NICE) && (stShCatch_.ncaLen_ > 0);
    barNCa_->g_SetDraw(nca_f);

    s32 ncapos = (NICE)* 2;
    barNCa_->z_SetPos(ncapos, 0);

    s32 ncalen = (nca_f)
        ? lib_num::Min((catchBar_c - NICE), stShCatch_.ncaLen_)
        : 0;
    barNCa_->g_SetSrcW(ncalen * 2);
    
    if (stShCatch_.catchMaru_c_ > 0)
    {

        BOOL ca_f = (stShCatch_.catchBar_c_ > BASE);
        BOOL nca_f = (stShCatch_.catchBar_c_ > NICE) && (stShCatch_.catchBar_c_ <= (NICE + ncalen));
        s32 maru = 0;
        if (nca_f)
        {
            maru = 2;
        }
        else if (ca_f)
        {
            maru = 1;
        }

        //s32 maru = ((stShCatch_.catchBar_c_ + 1) / 16) - 1;
        //if (maru >= MARUNUM) maru = (MARUNUM - 1);
        //if (maru < 0) maru = 0;

        BOOL marudraw_f = ((stShCatch_.catchMaru_c_ / 4) % 2 == 1);
        maru_[maru]->g_SetDraw(marudraw_f);
    }



  }

  void TUGShCatch::DoUpdate(BOOL draw_f)
  {
    Refresh();
    field_->Update(draw_f);
    catch_->Update(draw_f);

    barCa_->Update(draw_f);
    barNCa_->Update(draw_f);
    barNG_->Update(draw_f);

    //bar_->Update(draw_f);
    for (s32 i = 0; i < maru_.size(); ++i) maru_[i]->Update(draw_f);
    
  }

  void TUGShCatch::DoAction()
  {
  }
}
