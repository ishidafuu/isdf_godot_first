#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGTutoMsgGen.h"

namespace db
{
	class TUGTutoMsgAuto : public TUGTutoMsgGen
	{
	public:
		//コンストラクタ
		explicit TUGTutoMsgAuto(TUIGenLayer* player);
		virtual ~TUGTutoMsgAuto(void);

	protected:
		BOOL DoNextMsg();
	};
}
