#pragma once
#include "dbLyHmGen.h"
#include "dbUGBgmList.h"

namespace db
{
    class TLyHmTmBGM :public TLyHmGen
    {
    protected:

        s64 mid_bgm_;
        TUGBgmList ugBGM_;

        void MvPage_BGM();

        void Post_SetBGM();
        void Connect_SetBGM();

        void DoLoad();
        void DoReady();
        void DoDown();
        void DoConnect(BOOL rqok_f);
        void DoUpdate(BOOL draw_f);

    public:
        //コンストラクタ
        explicit TLyHmTmBGM(TSnHome* pScene);
        virtual ~TLyHmTmBGM(void);
    };

}
