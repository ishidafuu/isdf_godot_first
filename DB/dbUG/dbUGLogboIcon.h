#pragma once
#include "dbInclude.h"
#include "dbUGGen.h"

namespace db
{
  class TUILayer;

  struct TUGLogboIconSt
  {
    BOOL logbo_f_;
    BOOL logbosp_f_;
    void Init()
    {
      Zeromem(this);
    }
  };

  //リスト
  class TUGLogboIcon : public TUGGen
  {
  protected:
    TUIObj* star_;
    TUIObj* sp_;
    void DoUpdate(BOOL draw_f);
  public:
    TUGLogboIconSt stLogboIcon_;
    void MakeGroup(TUIObj* parent);
    
    //コンストラクタ
    explicit TUGLogboIcon(TUIGenLayer* player);
    virtual ~TUGLogboIcon(void);
    void Refresh();
    BOOL IsLogobo();
    BOOL IsLogoboSp();
  };
 
}
