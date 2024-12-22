#pragma once
#include "dbLyBarGen.h"

namespace db
{
  enum enShLayer
  {
    shl_Play,
    shl_Retry,
    shl_Pause,
    shl_Result,
    shl_VSError,
    shl_Map,
    shl_Home,
	shl_Rank,
	shl_EventHome,
  };

  struct TTLyShBarSt
  {
    enShLayer nowLayer_;
    void Init()
    {
      Zeromem(this);
    }
  };

  class TSnShiai;

  //ヘッダフッタ////////////////////////////////
  class TLyShBar :public TLyBarGen
  {
  protected:
    TSnShiai* pScene_;

    //オーバーロード
    void DoOpenHeader();
    void DoCloseHeader();
    void DoJumpHeader();
    void DoOpenTechno();
    void DoOpenProfile();
    void DoMoveFilter();
    void DoMoveFilterAll();
    void DoBackBtn();

    void IdleShiai();
    void RestartShiai();
  public:
    TTLyShBarSt stShBar_;

    //コンストラクタ
    explicit TLyShBar(TSnShiai* pScene);
    virtual ~TLyShBar(void);
    void MoveLayer(enShLayer mvlayer);

    void ReadyShiai();
	void ReadyRanking();
  };

}
