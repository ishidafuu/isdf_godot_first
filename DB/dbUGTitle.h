#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGGen.h"

namespace db
{
  class TUGTitle : public TUGGen
  {
  public:
    explicit TUGTitle(TUIGenLayer* player);
    virtual ~TUGTitle(void);
    void MakeGroup(TUIObj* parent, BOOL ovfilter_f);
	void MakeGroup_Mini(TUIObj* parent, BOOL ovfilter_f);
    void SetMessage(const std::string& msg);
    void SetLRPos(BOOL leftpos_f);
		void SetLRPos_BRank(BOOL leftpos_f);
  protected:
    void DoUpdate(BOOL draw_f);
    void DoAction();
  };
 
}