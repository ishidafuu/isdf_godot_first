#pragma once
#include "dbInclude.h"
#include "dbUGLiGen.h"
#include "dbUGRcNormal.h"
#include "dbUGListEnd.h"
namespace db
{
  struct TUGLiSozaiSt
  {
    void Init()
    {
      Zeromem(this);
    }
  };

  //リスト
  class TUGLiSozai :public TUGLiGen
  {
  public:
	  std::vector<TUGRcNormal*> records_;
	  TUGLiSozaiSt stLiItemGet_;
	  
	  void SetData(SOZAIDTVECTOR sozai);

	  //コンストラクタ
	  explicit TUGLiSozai(TUIGenLayer* player);
	  virtual ~TUGLiSozai(void);
	  s64 GetCommonID();
	  s64 GetEtcNum();

  protected:
    void DoUpdateSub(BOOL draw_f);
    void DoActionSub();
    void DoClearRecord();
    void DoMakeGroupSub();
  };
 
}
