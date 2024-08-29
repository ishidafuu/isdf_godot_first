#include "dbMgNSF.h"
#include "dbMgEngineObj.h"
namespace db
{

  TMgNSF::TMgNSF(TMgEngineObj* pmgEO):
    pmgEO_(pmgEO)
  {
    mid_bgm_now_ = NGNUM;
    mid_bgm_last_ = NGNUM;
  }

  TMgNSF::~TMgNSF(void)
  {

  }
  
  void TMgNSF::GenPlayID(s32 mid_bgm)
  {
    std::string filename = "s_";
    filename = filename + pmgEO_->mgDt_.dtBGM_.GetStrLine(mid_bgm, enDtBGMStr_filename);
    mid::enNSF nsftype = (mid::enNSF)(pmgEO_->mgDt_.dtBGM_.GetDtLine(mid_bgm, enDtBGM_nsftype));
    s32 trackno = pmgEO_->mgDt_.dtBGM_.GetDtLine(mid_bgm, enDtBGM_nsftrack);
    pmgEO_->mgSound_.PlayNSF(filename.c_str(), nsftype, trackno, 1.0);
  }
  void TMgNSF::PlayID(s32 mid_bgm)
  {
    mid_bgm_last_ = NGNUM;
    mid_bgm_now_ = mid_bgm;
    
    GenPlayID(mid_bgm);
  }
  void TMgNSF::Play(enNSF nsfno)
  {
    PlayID(nsfno);
  }
  
  void TMgNSF::PlayID_GetBGM(s32 mid_bgm)
  {
    mid_bgm_last_ = mid_bgm_now_;
    mid_bgm_now_ = mid_bgm;
    
    GenPlayID(mid_bgm);
  }
  void TMgNSF::Pause()
  {
    pmgEO_->mgSound_.PauseBGM();
  }
  void TMgNSF::Restart()
  {
    pmgEO_->mgSound_.RestartBGM();
  }
  void TMgNSF::PlayLast()
  {
    if (mid_bgm_last_ == NGNUM) return;
    GenPlayID(mid_bgm_last_);
    mid_bgm_now_ = mid_bgm_last_;
    mid_bgm_last_ = NGNUM;
  }
  
  void TMgNSF::StopBGM()
  {
    pmgEO_->mgSound_.StopBGM();
  }
}
