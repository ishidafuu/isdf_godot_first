#pragma once
#include "dbInclude.h"

namespace db
{
	//param,//

	enum endtSet_ClientSetting
	{
		setAccessGooglePlayByIOS = 0,	//IOSのwifi接続をグーグルプレイにする
        setAccessGooglePlayByMobage,
        setAccessGooglePlayByGoogle,
		setClientSettingEND,
	};

    enum endtSet_ClientSetting_Str
    {
        setGooglePlayClientIDByIOS = 0,	//IOSのgpid
        //setGooglePlayClientIDByADR,	//Androidのgpid
        setClientSetting_StrEND,
    };

	//各種設定データ
	class TDtClientSetting
	{
	public:
		explicit TDtClientSetting(void);
		virtual ~TDtClientSetting(void);

		//読み込み
		void LoadDt(const char *CSVDir, const char *CSVFilename);
		//取得
		BOOL IsAccessGooglePlayByIOS(void);
        BOOL IsAccessGooglePlayMobage(void);
        BOOL IsAccessGooglePlayGoogle(void);
        
        std::string GooglePlayID(void);

		s32 GetDt(endtSet_ClientSetting dtNo);
        std::string GetStrDt(endtSet_ClientSetting_Str strdtNo);
	protected:
		s32 GenGetDt(s32 dtNo);
        std::string GenGetStrDt(s32 strdtNo);

		s32 dt_[setClientSettingEND];
        std::string strdt_[setClientSetting_StrEND];
	};


}
