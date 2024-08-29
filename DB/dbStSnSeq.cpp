#include "dbStSnSeq.h"

namespace db
{
  enum enLastScene
  {
    ls_Title,
    ls_Map,
    ls_VS,
    ls_Tuto,
  };


  //ShiaiTypeステート******************************************************
  TStSnSeq::TStSnSeq()
  {
    Init();
  }

  TStSnSeq::~TStSnSeq()
  {
  }
  BOOL TStSnSeq::PopFlag(BOOL& flag)
  {
    BOOL res = flag;
    flag = FALSE;
    return res;
  }

  BOOL TStSnSeq::PopNeedOmakase()
  {
    return PopFlag(needomakase_f_);
  }
  BOOL TStSnSeq::PopNeedEvent()
  {
    return PopFlag(needevent_f_);
  }
  BOOL TStSnSeq::PopNeedMami()
  {
    return PopFlag(needmami_f_);
  }
  BOOL TStSnSeq::PopNeedMaruka()
  {
    return PopFlag(needmaruka_f_);
  }
  BOOL TStSnSeq::PopNewInfo()
  {
    return PopFlag(newinfo_f_);
  }
  void TStSnSeq::SetDrawFlag(s32 mami, s32 maruka)
  {
    //-1のときは変動させない
    if (mami == 1)
    {
      drawmami_f_ = TRUE;
    }
    else if (mami == 2)
    {
      drawmami_f_ = FALSE;
    }

    //-1のときは変動させない
    if (maruka == 1)
    {
      drawmaruka_f_ = TRUE;
    }
    else if (maruka == 2)
    {
      drawmaruka_f_ = FALSE;
    }

  };

  void TStSnSeq::ReadyEvent()
  {
    mid_maptip_ = NGID;
    lno_maptip_ = NGID;
    cleartip_f_ = FALSE;
    tutoType_ = tt_None;
  }
  void TStSnSeq::ReadyMapTip(s32 mid_maptip, s32 lno_maptip, BOOL clear_f, enTutoType tutotype)
  {
    mid_maptip_ = mid_maptip;
    lno_maptip_ = lno_maptip;
    cleartip_f_ = clear_f;
    tutoType_ = tutotype;
  }
}
