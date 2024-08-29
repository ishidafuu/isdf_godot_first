#include "dbUGDrHaikei.h"

namespace db
{

  //コンストラクタ
  TUGDrHaikei::TUGDrHaikei(TUIGenLayer* player) : TUGDrGen(player)
    //ura_(player),
    //drumend_(player)
  {
    stDrHaikei_.Init();
  }
  TUGDrHaikei::~TUGDrHaikei()
  {
    for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);
  }

  //背景カード
  void TUGDrHaikei::MakeGroup(TUIObj* parent, BOOL leftpos_f, base::TGrPart* grfilm)
  {
    stDrGen_.USEOK_F_ = FALSE;
    stDrGen_.USEEYE_F_ = FALSE;

    stDrGen_.TOPFRONT_F_ = TRUE;
    stDrGen_.REVY_ = -64;
    stDrGen_.DISTY_ = 32;
    stDrGen_.LOOKPOS_ = 4;
    stDrGen_.SLIDEY_ = -88;
    stDrGen_.CARDH_ = 120;

    stDrGen_.EYEX_ = 38 - 24;
    stDrGen_.EYEY_ = (stDrGen_.REVY_ + (stDrGen_.DISTY_ * stDrGen_.LOOKPOS_) + 126);//継承先のデータが必要
    stDrGen_.OKX_ = 136 - 24;
    stDrGen_.OKY_ = (stDrGen_.REVY_ + (stDrGen_.DISTY_ * stDrGen_.LOOKPOS_) + 126);

    //サイズを決めてから継承元のMakeGroupを呼び出す
    GenMakeGroup(parent, leftpos_f);

    for (s32 i = 0; i < DRRECNUM; ++i)
    {
      TUGHaikei* newrecord = new TUGHaikei(player_);
      newrecord->MakeGroup(field_);//fieldで大丈夫か？
      records_.push_back(newrecord);
      GenAddRecord(newrecord);
    }

  }
  //ドラム準備
  void TUGDrHaikei::DoReady()
  {
//    //共通な物はGenに写すか
//    SelectDrumEnd(FALSE);//末端消す
//    SelectUra(FALSE);//表表示
//    //StopSwipe(TRUE);//スワイプきっとく
//    SetDrawBtn(FALSE);//ボタン消す
  }

  void TUGDrHaikei::DoStop()
  {
  
  }
  void TUGDrHaikei::DoRoll()
  {
    //for (s32 i = 0; i < DRUMNUM; ++i)
    //{
    //  //同じ画像ならとばす
    //  if ((!first_f) && (records_[i]->IsSameGrp(pgrvec_[stDrGen_.alldtno_[i]])))
    //  {
    //    continue;
    //  }
    //  records_[i]->SetHaikeiDt(pgrvec_[stDrGen_.alldtno_[i]],
    //    stHaikei_.iconno_,
    //    "００　０１　０１",
    //    courtname_[ugdrum_.stDrGen_.alldtno_[i]].c_str());
    //}
  }
  void TUGDrHaikei::DoUpdateSub(BOOL draw_f)//更新
  {

  }
  void TUGDrHaikei::DoActionSub()//更新
  {
    ////押されてるボタンのアクションを返す
    //if (ugbtnsub_.IsAct()) SetAct(enUGDrHaikeiAct_Sub);
    //else  if (ugbtnsub2_.IsAct()) SetAct(enUGDrHaikeiAct_Sub2);
    //else if (ugbtnsub3_.IsAct()) SetAct(enUGDrHaikeiAct_Sub3);

    //if (ugbtnsub_.GetField()->b_IsEnter(TRUE)
    //  || ugbtnsub2_.GetField()->b_IsEnter(TRUE)
    //  || ugbtnsub3_.GetField()->b_IsEnter(TRUE)) SetDontRoll();

  }

  BOOL TUGDrHaikei::IsActRes(enUGDrHaikeiAct res)//返値ゲット
  {
    return (GetActRes() == res);
  }

}
