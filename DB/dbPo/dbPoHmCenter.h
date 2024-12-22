#pragma once
#include "dbPoGen.h"


namespace db
{
  enum enPostHmCenter
  {
    enPostHmCenter_gousei,
    enPostHmCenter_getok,
    enPostHmCenter_getng,
    enPostHmCenter_quick,
    enPostHmCenter_adddock,
    enPostHmCenter_outbox,
    enPostHmCenter_sellshot,
    enPostHmCenter_buysozai,
    enPostHmCenter_addshotinv,
    enPostHmCenter_baralast,
    enPostHmCenter_baraall,
    enPostHmCenterEND,
  };

  class TPostHmCenter : public TPoGen
  {
  protected:
    enPostHmCenter posttype_;
  public:
    explicit TPostHmCenter(TMgEngineObj* pmgEO);
    virtual ~TPostHmCenter(void);
    //ドック
    void Ready_gousei(s64 id_dock, LONGVECTOR* id_shot_mixvec);
    void Ready_okgousei(s64 id_dock);
    void Ready_nggousei(s64 id_dock);
    void Ready_quickgousei(s64 id_dock);
    void Ready_adddock();
    void Ready_checkdock();
    //まるか
    void Ready_maruka(s32 page);
    void Ready_outmaruka(LONGVECTOR* outidvec);
    //ＢＢＡ
    void Ready_sellshot(LONGVECTOR* sellidvec);
    void Ready_buysozai(s64 mid_spsyouhin);
    void Ready_addshotinv(s64 mid_spsyouhin);
    void Ready_bara(s64 id_shot, BOOL todd_f);

  };
}
