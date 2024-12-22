#pragma once
#include "dbInclude.h"
#include "dbUGGen.h"
#include "dbUGBtnLabel.h"
#include "dbUGRadio.h"
#include "dbUGRName.h"
#include "dbUGBossSerifu.h"
namespace db
{
  class TUILayer;

  struct TUGScEnemySt
  {
    BOOL inputtip_f_;
    s32 posx_;
    s32 posy_;
    s32 www_;
    s32 enemyNo_;
    s64 mid_team_;
    void Init()
    {
      Zeromem(this);
    }
  };

  class TUGScEnemy : public TUGGen
  {
  public:
    //コンストラクタ
    explicit TUGScEnemy(TUIGenLayer* player);
    virtual ~TUGScEnemy(void);
    void MakeGroup(TUIObj* parent, s32 enemyNo);//敵ＡＢＣ
    void Ready(s64 mid_maptip, s32 lno_maptip);
    TUGScEnemySt stScEnemy_;
  protected:
    void DoUpdate(BOOL draw_f);
    TUIObj* enemy_;//敵
	TUGBossSerifu ugBossSerifu_;
    TUGRName ugname_;//チーム名
    std::vector<TUGBtnLabel*> ugbtn_drop_;
    TUGRadio ugradio_drop_;
  };
 
}
