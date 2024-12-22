#include "dbMgCoInfo.h"
#include "dbMgEngineObj.h"
namespace db
{
  TMgCoInfo::TMgCoInfo(TMgEngineObj* pmgEO) :TMgCoGen(pmgEO, "co_info")
  {
    mdvec_.clear();   
    mdvec_.push_back(&mdm_Info_);//マスターお知らせ
    mdvec_.push_back(&mdm_banner_);//マスターバナー

    models_.clear();
    for (s32 i = 0; i < mdvec_.size(); ++i)
    {
      models_.push_back(&mdvec_[i]->model_);
    }
  }

  TMgCoInfo::~TMgCoInfo(void)
  {

  }

  void TMgCoInfo::DoInitSendPack()
  {
    sendmap_.longmap_["scheck"] = enSessionCheckType_Reset;
    if (mid::midIsIOS())
    {
        sendmap_.longmap_["os"] = 1;
    }
    else if (mid::midIsADR())
    {
        sendmap_.longmap_["os"] = 2;
    }
  }
  
  bool cmpdate(const INTVECTOR& left, const INTVECTOR& right)
  {
    return (left[1] > right[1]);//大きい方が先頭
  }
  
  INTVECTOR TMgCoInfo::GetSortInfo()
  {
    //modalPanelにお知らせに関する全てのデータを渡してしまう
    s32 infonum = mdm_Info_.model_.intvecmap_.at("pk").size();
    
    INTVECTOR2 infovec2;
    
    for (s32 i = 0; i < infonum; ++i)
    {
      //お知らせＯＳ判定
//      s32 ostype = mdm_Info_.GetLineDt(i, mdm_info_osType);
//      if ((ostype == 1) && (mid::midIsIOS() == FALSE)) continue;
//      if ((ostype == 2) && (mid::midIsADR() == FALSE)) continue;

      INTVECTOR newvec;
      newvec.push_back(i);
      newvec.push_back(mdm_Info_.model_.intvecmap_.at("infoSt_date")[i]);
      infovec2.push_back(newvec);
    }
    
    std::sort(infovec2.begin(), infovec2.end(), cmpdate);
    
    infonum = infovec2.size();

    INTVECTOR res;
    for (s32 i = 0; i < infonum; ++i)
    {
      res.push_back(infovec2[i][0]);
    }
    
    return res;
  }

  s64 TMgCoInfo::GetNewestDate()
  {
    //modalPanelにお知らせに関する全てのデータを渡してしまう
    s32 infonum = mdm_Info_.model_.intvecmap_.at("pk").size();
    s64 res = 0;
    for (s32 i = 0; i < infonum; ++i)
    {
      if (res < mdm_Info_.model_.intvecmap_.at("infoSt_date")[i])
      {
        res = mdm_Info_.model_.intvecmap_.at("infoSt_date")[i];
      }
    }

    return res;
  }

  BOOL TMgCoInfo::IsNewestInfo()
  {
    return (GetNewestDate() < pmgEO_->mgCoU_.mdre_Refresh_.GetLineDt(0, mdre_refresh_newestinfo));
  }

  void TMgCoInfo::LoadDt()
  {
    std::string csvdir = mid::midDatadir() + mid::midMasterdir();
    for (s32 i = 0; i < mdvec_.size(); ++i)
    {
      mdvec_[i]->LoadDt(csvdir.c_str());
    }
  }

}
