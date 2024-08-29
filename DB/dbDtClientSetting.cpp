#include "dbDtClientSetting.h"

namespace db
{
    TDtClientSetting::TDtClientSetting(void)
    {

    }

    TDtClientSetting::~TDtClientSetting(void)
    {
    }

    //読み込み
    void TDtClientSetting::LoadDt(const char *CSVDir, const char *CSVFilename)
    {
        //dt_->clear();
        mid::TCSV csv;
        csv.midLoadCSV(CSVDir, CSVFilename, FALSE, FALSE);//文字列も読み込む


        //初期化
        for (s32 i = 0; i < setClientSettingEND; ++i) dt_[i] = 0;

        //数値
        s32 intline = 0;
        for (s32 i = 0; i < csv.GetLine(); ++i)
        {
            if (csv.GetIntSize(i) == 0) continue;

            dt_[intline] = csv.longvec2_[i][0];
            ++intline;
            if (intline >= setClientSettingEND) break;
        }


        //文字列
        s32 strline = 0;
        for (s32 i = 0; i < csv.GetLine(); ++i)
        {
            if (csv.GetStrSize(i) == 0) continue;

            strdt_[strline] = csv.strvec2_[i][0];
            ++strline;
            if (strline >= setClientSetting_StrEND) break;
        }

    }

    BOOL TDtClientSetting::IsAccessGooglePlayByIOS(void)
    {
        s32 ret = GenGetDt(setAccessGooglePlayByIOS);
        return (ret == 1) ? TRUE : FALSE;
    }

    BOOL TDtClientSetting::IsAccessGooglePlayMobage(void)
    {
        s32 ret = GenGetDt(setAccessGooglePlayByMobage);
        return (ret == 1) ? TRUE : FALSE;
    }

    BOOL TDtClientSetting::IsAccessGooglePlayGoogle(void)
    {
        s32 ret = GenGetDt(setAccessGooglePlayByGoogle);
        return (ret == 1) ? TRUE : FALSE;
    }


    std::string TDtClientSetting::GooglePlayID(void)
    {
        //if (mid::midIsADR())
        //{
        //    return GetStrDt(setGooglePlayClientIDByADR);
        //}
        //else
        //{
        return GetStrDt(setGooglePlayClientIDByIOS);
        //}
    }

    s32 TDtClientSetting::GetDt(endtSet_ClientSetting dtNo)
    {
        return GenGetDt(dtNo);
    }
    std::string TDtClientSetting::GetStrDt(endtSet_ClientSetting_Str strdtNo)
    {
        return GenGetStrDt(strdtNo);
    }
    s32 TDtClientSetting::GenGetDt(s32 dtNo)
    {
        return dt_[dtNo];
    }
    std::string TDtClientSetting::GenGetStrDt(s32 strdtNo)
    {
        return strdt_[strdtNo];
    }
}
