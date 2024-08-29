#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGGen.h"
#include "dbUGTeam.h"
#include "dbUGButton.h"
namespace db
{
  enum enUGTeamUraAct
  {
    enUGTeamUraAct_Eye,
    enUGTeamUraAct_Data,
    enUGTeamUraAct_Soubi,
  };

  struct TTeamUraSt
  {
    s32 sideNo_;
    s32 mid_team_;
    BOOL tipsdraw_f_;
    void Init()
    {
      Zeromem(this);
      mid_team_ = NGNUM;
    }
  };

  //リスト
  class TUGTeamUra : public TUGGen
  {
  protected:
    static const s32 TEXTLEN = 4;
    TUIObj* cap_;
    TUGRName ugname_;
    TUIObj* overlapnum_;
    TUIObj* shotnum_;
    TUIObj* text_[TEXTLEN];

    void DoUpdate(BOOL draw_f);
  public:
    TTeamUraSt stTeamUra_;

    //コンストラクタ
    explicit TUGTeamUra(TUIGenLayer* player);
    virtual ~TUGTeamUra(void);
    void MakeGroup(TUIObj* parent, s32 sideNo);
    void SetTeamUraDt(s32 mid_team);

    BOOL IsActRes(enUGTeamUraAct res);//返値ゲット
  };
 
}
