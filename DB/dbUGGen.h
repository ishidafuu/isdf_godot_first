#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"

namespace db
{
  class TUILayer;
  class TMgUI;

  class TUGGen
  {
  public:
    //コンストラクタ
    explicit TUGGen(TUIGenLayer* player);
    explicit TUGGen(TUIGenLayer* player, TMgEngineObj* pmgEO, TMgUI* pmgUI);
    virtual ~TUGGen(void);
    TUIObj* GetField();//フィールドゲット
    void SetDraw(BOOL draw_f);//描画オンオフ
    void SetPos(s32 posx, s32 posy);//位置
    void SetPos_Full(s32 posx, s32 posy);//位置
    void SetParent(TUIObj* parent);//親セット
    void SetModal(BOOL modal_f);//モーダルボタン
    void SetDepth(enUIDepth depth);//プライオリティ
    BOOL IsDraw();//描画
    //アクション発生（Update内で自身のアクション発生、CheckAct内で子のアクション発生検知し自身のアクションとする）
    void SetAct(s32 res);
    void SetAct(s32 res, s32 index);
    BOOL IsAct();//返値ゲット
    s32 GetActRes();//返値ゲット
    virtual s32 GetActIndex();//返値が発生したインデックス（通常０、リストなどでレコード番号）
    void AddUI(TUIObj* pui);//UI追加（アップデート管理のみ）
    void Update(BOOL draw_f);//更新
    void ClearAct();
    std::vector<TUIObj*> childUI_;
  protected:
    virtual void DoUpdate(BOOL draw_f) = 0;//更新（継承先を呼び出す）
    virtual void DoAction();//更新（継承先を呼び出す）
    TMgEngineObj* pmgEO_;
    TMgUI* pmgUI_;
    TUIGenLayer* player_;
    TUIObj* field_;
    TUIObj* filter_;
    TUIObj* parent_;
    std::vector<TUIObj*> puiovec_;//TUIObjのポインタを持つ
    s32 actRes_;//関数返値
    s32 actIndex_;//関数返値発生インデックス
    BOOL act_f_;//アクション発生
    s32 lastani_;
    base::TGrp* basegrp_;
    base::TGrp* basegrp2_;
    TUIObj* MakeObj();
  };

}
