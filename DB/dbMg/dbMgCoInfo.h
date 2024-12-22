#pragma once
#include "dbMgCoGen.h"
#include "dbMdm_Info.h"
#include "dbMdm_Banner.h"

namespace db
{
  class TMgCoInfo : public TMgCoGen
  {
  protected:
    void DoInitSendPack();
  public:
    TMdm_Info mdm_Info_;//マスターお知らせ
    TMdm_Banner mdm_banner_;//マスターバナー
    explicit TMgCoInfo(TMgEngineObj* pmgEO);
    virtual ~TMgCoInfo(void);
   INTVECTOR GetSortInfo();
   s64 GetNewestDate();
   BOOL IsNewestInfo();
   void LoadDt();
  };
}
