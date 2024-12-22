#pragma once
#include "dbInclude.h"
#include "dbUGLiGen.h"
#include "dbUGRcNormal.h"
namespace db
{
  struct TUGLiAlbumSt
  {
    void Init()
    {
      Zeromem(this);
    }
  };

  //リスト
  class TUGLiAlbum :public TUGLiGen
  {
  protected:
    void DoUpdateSub(BOOL draw_f);
    void DoActionSub();
    void DoClearRecord();
  public:
    std::vector<TUGRcNormal*> records_;
    INTVECTOR allnumvec_;
    TUGLiAlbumSt stLiBGM_;
    TUGRcNormal* MakeObj_Record();

    //コンストラクタ
    explicit TUGLiAlbum(TUIGenLayer* player);
    virtual ~TUGLiAlbum(void);

    void Refresh();
	void Refresh(s64 mid_event);
  };
 
}
