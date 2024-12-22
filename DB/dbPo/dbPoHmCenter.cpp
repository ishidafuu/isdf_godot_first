#include "dbPoHmCenter.h"

namespace db
{
  TPostHmCenter::TPostHmCenter(TMgEngineObj* pmgEO):TPoGen(pmgEO)
  {
  }

  TPostHmCenter::~TPostHmCenter(void)
  {
  }
  //送信パック初期化
  void TPostHmCenter::Ready_gousei(s64 id_dock, LONGVECTOR* id_shot_mixvec)
  {
    ClearMap();
    sendmap_.longmap_["id_dock"] = id_dock;
    s32 i = 0;
    for (auto& id_shot : *id_shot_mixvec)
    {
      std::string outid = "id_shot" + lib_str::IntToStr1b(i, 0);
      sendmap_.longmap_[outid.c_str()] = id_shot;
      ++i;
    }
    //sendmap_.longmap_["id_shot_base"] = id_shot_base;
    //sendmap_.longmap_["id_shot_add"] = id_shot_add;
    StartConnect("hmcenter_gousei");
  }
  void TPostHmCenter::Ready_okgousei(s64 id_dock)
  {
    ClearMap();
    sendmap_.longmap_["id_dock"] = id_dock;
    StartConnect("hmcenter_okgousei");
  }
  void TPostHmCenter::Ready_nggousei(s64 id_dock)
  {
    ClearMap();
    sendmap_.longmap_["id_dock"] = id_dock;
    StartConnect("hmcenter_nggousei");
  }
  void TPostHmCenter::Ready_quickgousei(s64 id_dock)
  {
    ClearMap();
    sendmap_.longmap_["id_dock"] = id_dock;
    StartConnect("hmcenter_quickgousei");
  }
  void TPostHmCenter::Ready_adddock()
  {
    ClearMap();
    sendmap_.longmap_["dummy"] = 0;
    StartConnect_NoneRetry("hmcenter_adddock");
  }
  void TPostHmCenter::Ready_checkdock()
  {
    ClearMap();
    sendmap_.longmap_["dummy"] = 0;
    StartConnect("hmcenter_checkdock");
  }

  void TPostHmCenter::Ready_maruka(s32 page)
  {
    ClearMap();
    sendmap_.longmap_["page"] = page;
    StartConnect("hmcenter_maruka");
  }

  void TPostHmCenter::Ready_outmaruka(LONGVECTOR* outidvec)
  {
    ClearMap();
    for (s32 i = 0; i < outidvec->size(); ++i)
    {
      std::string outid = "id_log_maruka" + lib_str::IntToStr1b(i, 0);
      sendmap_.longmap_[outid.c_str()] = outidvec->at(i);
    }
    StartConnect("hmcenter_outmaruka");
  }


  void TPostHmCenter::Ready_sellshot(LONGVECTOR* sellidvec)
  {
    ClearMap();
    for (s32 i = 0; i < sellidvec->size(); ++i)
    {
      std::string outid = "id_shot" + lib_str::IntToStr1b(i, 0);
      sendmap_.longmap_[outid.c_str()] = sellidvec->at(i);
    }
    StartConnect_NoneRetry("hmcenter_sellshot");
  }
  void TPostHmCenter::Ready_buysozai(s64 mid_spsyouhin)
  {
    ClearMap();
    sendmap_.longmap_["mid_spsyouhin"] = mid_spsyouhin;
    StartConnect_NoneRetry("hmcenter_buysozai");
  }
  void TPostHmCenter::Ready_addshotinv(s64 mid_spsyouhin)
  {
    ClearMap();
    sendmap_.longmap_["mid_spsyouhin"] = mid_spsyouhin;
    StartConnect_NoneRetry("hmcenter_addshotinv");
  }
  void TPostHmCenter::Ready_bara(s64 id_shot, BOOL todd_f)
  {
    ClearMap();
    sendmap_.longmap_["id_shot"] = id_shot;
    sendmap_.longmap_["last_f"] = 0;
    sendmap_.longmap_["todd_f"] = (todd_f) ? 1 : 0;

    StartConnect_NoneRetry("hmcenter_bara");
  }


}
