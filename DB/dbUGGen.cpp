#include "dbUGGen.h"
#include "dbUILayer.h"

namespace db
{
  //コンストラクタ
  TUGGen::TUGGen(TUIGenLayer* player)
  {
    pmgEO_ = player->pmgEO_;
    pmgUI_ = player->pmgUI_;
    player_ = player;
    field_ = MakeObj();
    parent_ = NULL;

    actRes_ = NGNUM;
    act_f_ = FALSE;
    actIndex_ = 0;

  }
  TUGGen::TUGGen(TUIGenLayer* player, TMgEngineObj* pmgEO, TMgUI* pmgUI)
  {
    pmgEO_ = pmgEO;
    pmgUI_ = pmgUI;
    player_ = player;
    actRes_ = 0;
    field_ = MakeObj();
    parent_ = NULL;
  }

  TUGGen::~TUGGen()
  {
    //オブジェ解放
    for (s32 i = 0; i < puiovec_.size(); ++i) Freemem(puiovec_[i]);
    puiovec_.clear();
  }
  //オブジェ作成（オブジェ番号を返す）
  TUIObj* TUGGen::MakeObj()
  {
    TUIObj* pso = new TUIObj(pmgEO_, pmgUI_);
    puiovec_.push_back(pso);
    return pso;
  }
  TUIObj* TUGGen::GetField()
  {
    return field_;
  }
  void TUGGen::SetDraw(BOOL draw_f)//描画オンオフ
  {
    field_->g_SetDraw(draw_f);
  }
  void TUGGen::SetPos(s32 posx, s32 posy)//位置
  {
    field_->z_SetPos(posx, posy);
  }
  void TUGGen::SetPos_Full(s32 posx, s32 posy)//位置
  {
    field_->z_SetPos_Full(posx, posy);
  }
  void TUGGen::SetParent(TUIObj* parent)//親セット
  {
    field_->o_SetParent(parent);
    parent_ = parent;
  }
  void TUGGen::SetModal(BOOL modal_f)//モーダルボタン
  {
    field_->b_SetModal(modal_f);
  }
  void TUGGen::SetDepth(enUIDepth depth)
  {
    field_->g_SetDepth(depth);
  }

  BOOL TUGGen::IsDraw()//描画
  {
    return field_->IsDraw();
  }
  
  //UI追加（Updateだけ追加で行う）
  void TUGGen::AddUI(TUIObj* pui)
  {
    childUI_.push_back(pui);
    pui->o_SetParent(field_);
  }

  //アクション発生（Update内で自身のアクション発生、CheckAct内で子のアクション発生検知し自身のアクションとする）
  void TUGGen::SetAct(s32 res)
  {
    SetAct(res, NGNUM);
  }
  void TUGGen::SetAct(s32 res, s32 index)
  {
    act_f_ = TRUE;
    actRes_ = res;
    actIndex_ = index;
  }

  BOOL TUGGen::IsAct()//アクション発生確認
  {
    return act_f_;
  }
  s32 TUGGen::GetActRes()//返値ゲット
  {
    return (IsAct())
      ? actRes_
      : NGNUM;
  }

  s32 TUGGen::GetActIndex()//返値が発生したインデックス（通常０、リストなどでレコード番号）
  {
    return (IsAct())
      ? actIndex_
      : NGNUM;
  }
  void TUGGen::ClearAct()
  {
    actRes_ = NGNUM;
    act_f_ = FALSE;
    actIndex_ = 0;
  }
  //更新
  void TUGGen::Update(BOOL draw_f)
  {
    //まず自身のアクション発生を切る
    ClearAct();//drawの時も切ることで、postなどでdrawがこない時に連続アクションしてしまうのを防ぐ
    //if (draw_f == FALSE) ClearAct();
    
    //子ＵＩの更新
    for (s32 i = 0; i < childUI_.size(); ++i) childUI_[i]->Update(draw_f);
    //自身の更新
    DoUpdate(draw_f);

    if (draw_f == FALSE) DoAction();
  }
  void TUGGen::DoAction()//更新（継承先を呼び出す）
  {
    //デフォは何も無し
  }
}
