#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGLiGen.h"
#include "dbUGRcNormal.h"
namespace db
{

  struct TUGListWSt
  {
    f32 overh_;
    s32 bottomy_;
    s32 marker_c_;
    BOOL listend_f_;
    void Init()
    {
      Zeromem(this);
    }
  };


  //リスト
  class TUGLiGenW :public TUGGen
  {
  private:
    std::vector<TUGRcGen*> genrecords_;
  protected:
    TUIObj* slider_;//実際に移動するフィールド
    TUIObj* marker_;//マーカー
    f32 sliderx_;
    f32 slidery_;

    void GenMakeObj_Record(TUGRcGen* newrecord, s32 rh);
    void SetRecordPos();
    void SetSelect();

    virtual void DoActionSub();
    virtual void DoUpdateSub(BOOL draw_f);
    virtual void DoMakeGroupSub();
    virtual void DoClearRecord() = 0;//必須

    void DoUpdate(BOOL draw_f);
    void DoAction();
  public:
    //コンストラクタ
    explicit TUGLiGenW(TUIGenLayer* player);
    virtual ~TUGLiGenW(void);
    TUGListSt stListW_;
    void MakeGroup(TUIObj* parent);
    void ClearRecord();
    void SetZeroPos();

    BOOL IsActRes(enUGLiAct res);
  };

}
