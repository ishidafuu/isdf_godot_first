#pragma once
#include "dbUGLiAlbum.h"
#include "dbUGLiTape.h"
//=====================================================================================================
//	
//
//
//=====================================================================================================
namespace db
{
    struct TUGBgmListMSt
    {
        s32 titleid_;
        s32 iconno_;
        s32 lasthaikei_;
        s64 mid_bgm_;
        s64 mid_bgm_intro_;
        s32 introTimer_;
        s32 wait_;
        void Init()
        {
            Zeromem(this);
            lasthaikei_ = NGNUM;
        }
    };

    enum enUGBgmAct
    {
        enUGBgmAct_Select = 0,
    };


    class TUGBgmList :public TUGGen
    {
    public:
        //コンストラクタ
        explicit TUGBgmList(TUIGenLayer* player);
        virtual ~TUGBgmList(void);

        void MakeGroup(TUIObj* parent);
        void RefreshIcon(void);
        BOOL IsAct(void);

        void Ready(void);
        void Ready(s64 mid_event);
        void Down(void);

        s64 GetBgmID(void){ return stBGM_.mid_bgm_; }
    
    protected:
        void MvPage_BGM();

        //オーバーロード
        void SetTitle(s32 recordno);
        void DoUpdate(BOOL draw_f);
        void DoAction(void);
        //INTVECTOR testdt_;

        TUGBgmListMSt stBGM_;
        TUIObj* filter_;
        TUIObj* nowtitle_;//ヘッダ現在タイトル表示
        TUIObj* nowBGM_;//フッタ現在曲表示
        TUGLiAlbum uglistAlbum_;//タイトルリスト
        TUGLiTape uglistTape_;//BGMリスト
        INTVECTOR bgmline_;//何行目のＢＧＭか

        s64 mid_event_;	// 特効用
    };

}
