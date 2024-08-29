#include "dbUGLiNaBoxNum.h"
#include "dbUILayer.h"

namespace db
{
  //コンストラクタ
  TUGLiNaBoxNum::TUGLiNaBoxNum(TUIGenLayer* player) :TUGLiGen(player), uglistend_(player)
  {
    stLiNaBoxNum_.Init();
  }

  TUGLiNaBoxNum::~TUGLiNaBoxNum()
  {
    //解放時必ず呼ぶ
    ClearRecord();
  }

  //リストクリア（変数部分的に初期化、レコードとレコードエンド解放）
  void TUGLiNaBoxNum::DoClearRecord()
  {
    stLiNaBoxNum_.Init();
    for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);
    records_.clear();
  }

  void TUGLiNaBoxNum::DoMakeGroupSub()
  {
    field_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_list_frameshop));//ショップの時は変える
    uglistend_.MakeGroup_ListEnd_Shop(slider_, sliderx_, slidery_);
    uglistend_.SetDraw(FALSE);
  }

  //マスターガチャIDを入れてレコードをすべて作成
  void TUGLiNaBoxNum::Refresh(s64 mid_nabox)
  {
    //個数の更新が必要
//    if (stLiNaBoxNum_.mid_nabox_ == mid_nabox) return;
    
    ClearRecord();
    
    std::string name = pmgEO_->mgCoM_.mdm_NaBox_.GetPKDtStr(mid_nabox, mdm_nabox_name_c64);
    s64 mid_ken = pmgEO_->mgCoM_.mdm_NaBox_.GetPKDt(mid_nabox, mdm_nabox_id_m_supply_ken);
    s32 kennum = pmgEO_->mgCoM_.mdm_NaBox_.GetPKDt(mid_nabox, mdm_nabox_kenNum);
    s32 price = (pmgEO_->mgCoM_.mdm_Supply_.GetPKDt(mid_ken, mdm_supply_price) * kennum);
    s32 iconno = pmgEO_->mgCoM_.mdm_NaBox_.GetPKDt(mid_nabox, mdm_nabox_iconNo);

    s32 id_nabox = pmgEO_->mgCoU_.GetNaBox_id(mid_nabox);
    s32 opennum = pmgEO_->mgCoU_.mdNaBox_.GetPKDt(id_nabox, mdnabox_openNum);
    s32 boxsize = pmgEO_->mgCoM_.mdm_NaBox_.GetPKDt(mid_nabox, mdm_nabox_boxSize);
    
    stLiNaBoxNum_.mid_nabox_ = mid_nabox;

    s32 no = 0;


	//ＢＯＸなかみ
	{
		TUGRcNormal* newrecord = new TUGRcNormal(player_);
		s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
		newrecord->SetShopRecord();
		newrecord->SetName("ＢＯＸなかみ＿かくにん");
		//s32 iconno = pmgEO_->mgCoM_.mdm_NaBox_.GetPKDt(mid_nabox, mdm_nabox_iconNo);
		newrecord->ugname_.ugicon_.SetIconNaBox(mid_nabox);
		//newrecord->GetField()->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_icon_nabox));
		//newrecord->GetField()->g_SetPartNo(iconno);
		records_.push_back(newrecord);
		GenMakeObj_Record(newrecord, rh);

#ifdef __K_DEBUG_ROOT__
		newrecord->SetDisable(FALSE);
#else
		//チュートクリアしてないと一個買いできない
		if (pmgEO_->mgCoU_.IsTutoClear(enTutoBitType_NaBox) == FALSE)
		{
			newrecord->SetDisable(TRUE);
		}
#endif
	}
    //１個買い
    {
      TUGRcNormal* newrecord = new TUGRcNormal(player_);
      s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
      newrecord->SetShopRecord();
      newrecord->SetNameIcon("ＢＯＸ１かいひく", ugi_NaBox);
      newrecord->SetValue_Star(price);
      records_.push_back(newrecord);
      GenMakeObj_Record(newrecord, rh);

#ifdef __K_DEBUG_ROOT__
	  newrecord->SetDisable(FALSE);
#else
      //チュートクリアしてないと一個買いできない
      if (pmgEO_->mgCoU_.IsTutoClear(enTutoBitType_NaBox) == FALSE)
      {
        newrecord->SetDisable(TRUE);
      }
#endif
    }
    //大人買い
    {
      TUGRcNormal* newrecord = new TUGRcNormal(player_);
      s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
      newrecord->SetShopRecord();
      newrecord->SetNameIcon("ＢＯＸおとながい", ugi_NaBoxOtona);
      s32 otonaprice = (price * (boxsize - opennum));
      newrecord->SetValue_Star(otonaprice);
      records_.push_back(newrecord);
      GenMakeObj_Record(newrecord, rh);
    }

  }
  void TUGLiNaBoxNum::ChangeListEnd(BOOL listend_f)
  {
    uglistend_.SetDraw(listend_f);
    for (s32 i = 0; i < records_.size(); ++i) records_[i]->SetDraw(!listend_f);
  }
  void TUGLiNaBoxNum::SetListEnd(BOOL otona_f)
  {
    uglistend_.SetDataNaBox(stLiNaBoxNum_.mid_nabox_, otona_f);
  }

  void TUGLiNaBoxNum::DoUpdateSub(BOOL draw_f)
  {
    uglistend_.Update(draw_f);
  }

  void TUGLiNaBoxNum::DoActionSub()
  {
  }

}
