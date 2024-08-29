#include "dbUGLiBuySozaiType.h"
#include "dbUILayer.h"

namespace db
{
  const s32 RECNUM = 10;

  //コンストラクタ
  TUGLiBuySozaiType::TUGLiBuySozaiType(TUIGenLayer* player) :TUGLiGen(player)
  {
    stLiBuySozaiType_.Init();
  }

  TUGLiBuySozaiType::~TUGLiBuySozaiType()
  {
    //解放時必ず呼ぶ
    ClearRecord();
  }
  void TUGLiBuySozaiType::DoMakeGroupSub()
  {
    field_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_list_frameshop));//ショップの時は変える
  }
  //リストクリア（変数部分的に初期化、レコードとレコードエンド解放）
  void TUGLiBuySozaiType::DoClearRecord()
  {
    stLiBuySozaiType_.Init();
    for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);
    records_.clear();
  }

  void TUGLiBuySozaiType::Refresh()
  {
    //各レコードクリアして入れる
    ClearRecord();

    for (s32 i = 0; i < pmgEO_->mgDt_.dtShElm_.GetGenreNum(); ++i)
    {
      TUGRcNormal* newrecord = new TUGRcNormal(player_);
      s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
      records_.push_back(newrecord);
      newrecord->SetShopRecord();
      newrecord->SetCommonID(pmgEO_->mgDt_.dtShElm_.GetGenreID(i));
      newrecord->ugname_.SetName(pmgEO_->mgDt_.dtShElm_.GetGenreNameLine(i));
      newrecord->ugname_.ugicon_.SetIcon((enUGPIcon)(ugi_FilGenre0 + i));
      GenMakeObj_Record(newrecord, rh);

    }
  }

  void TUGLiBuySozaiType::Refresh(s32 mid_bbabuy)
  {
	  //各レコードクリアして入れる
	  ClearRecord();

	  s32 max = pmgEO_->mgDt_.dtShElm_.GetGenreNum();
	  for (s32 i = 0; i < max; ++i)
	  {
		  s32 genreType = pmgEO_->mgDt_.dtShElm_.GetGenreID(i);


		  // データがあるかチェック
		  BOOL isEmpty = TRUE;

		  s32 type_st = genreType;
		  s32 type_ed = ((i + 1) >= max) ? 0 : pmgEO_->mgDt_.dtShElm_.GetGenreID(i + 1);

		  s32 shoptype = pmgEO_->mgCoM_.mdm_BBABuy_.GetPKDt(mid_bbabuy, mdm_bbabuy_shopType);
		  for (s32 i2 = 0; i2 < pmgEO_->mgCoM_.mdm_SpSyouhin_.GetLineNum(); ++i2)
		  {
			  //このショップの種類
			  if (pmgEO_->mgCoM_.mdm_SpSyouhin_.GetLineDt(i2, mdm_spsyouhin_shopType) == shoptype)
			  {
				  s32 mid_sozai = pmgEO_->mgCoM_.mdm_SpSyouhin_.GetLineDt(i2, mdm_spsyouhin_itemID) / 10;//タイプはレア度を省いたものでチェック

				  if (mid_sozai < type_st) continue;
				  if ((type_ed != 0) && (mid_sozai >= type_ed)) continue;

				  isEmpty = FALSE;
				  break;
			  }
		  }

		  if (isEmpty){ continue; }


		  TUGRcNormal* newrecord = new TUGRcNormal(player_);
		  s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
		  records_.push_back(newrecord);
		  newrecord->SetShopRecord();
		  newrecord->SetCommonID(genreType);
		  newrecord->ugname_.SetName(pmgEO_->mgDt_.dtShElm_.GetGenreNameLine(i));
		  newrecord->ugname_.ugicon_.SetIcon((enUGPIcon)(ugi_FilGenre0 + i));
		  GenMakeObj_Record(newrecord, rh);
	  }
  }


  void TUGLiBuySozaiType::DoUpdateSub(BOOL draw_f)
  {

  }

  void TUGLiBuySozaiType::DoActionSub()
  {

  }
  s32 TUGLiBuySozaiType::GetType()
  {
    //レコードから商品MIDを取得
    return records_[GetActIndex()]->GetCommonID();
  }
  s32 TUGLiBuySozaiType::GetNextType()
  {
    //レコードから商品MIDを取得
    s32 res = 0;
    if ((GetActIndex() + 1) < records_.size())
    {
      res = records_[(GetActIndex() + 1)]->GetCommonID();
    }

    return res;
  }


}
