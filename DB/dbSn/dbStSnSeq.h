#pragma once
#include "dbStSnGen.h"

namespace db
{


  //ShiaiTypeステート
  class TStSnSeq : TStSnGen
  {
  public:
    enum enTutoType
    {
      tt_None,
      tt_Retro,
      tt_Smart,
      tt_RetroFirst,
	  tt_Auto,
    };

    //コンストラクタ
    explicit TStSnSeq();
    virtual ~TStSnSeq();
    void Init(){ Zeromem(this); };

    void SetNoSv(){ nosv_f_ = TRUE; };
    BOOL IsNoSv(){ return nosv_f_; }
    void SetReview(){ stg_f_ = TRUE; };
    BOOL IsStg(){ return stg_f_; };

    void SetIDEvent(s32 mid_event, s32 wid_event){ mid_event_ = mid_event; wid_event_ = wid_event; };
    s32 GetMidEvent(){ return mid_event_; };
    s32 GetWidEvent(){ return wid_event_; };
    BOOL IsFirstTuto(){ return (tutoType_ == tt_RetroFirst); };
    void SetFirstTuto(){ tutoType_ = tt_RetroFirst; };
    enTutoType GetTutoType(){ return tutoType_; }
    BOOL IsTutoRetro(){ return ((tutoType_ == tt_Retro) || (tutoType_ == tt_RetroFirst)); };
    BOOL IsTutoSmart(){ return (tutoType_ == tt_Smart); };
	BOOL IsTutoAuto(){ return (tutoType_ == tt_Auto); };
    void ReadyEvent();
    void ReadyMapTip(s32 mid_maptip, s32 lno_maptip, BOOL clear_f, enTutoType tutotype);
    BOOL IsHomeCourt(){ return homecourt_f_; };
    void SetHomeCourt(BOOL homecourt_f){ homecourt_f_ = homecourt_f; };

    s32 GetMidMapTip(){ return mid_maptip_; };
    s32 GetLNoMapTip(){ return lno_maptip_; };
    void PushNeedOmakase(){ needomakase_f_ = TRUE; };
    BOOL PopNeedOmakase();
    void PushNeedEvent(){ needevent_f_ = TRUE; };
    BOOL PopNeedEvent();
    void PushNeedMami(){ needmami_f_ = TRUE; };
    BOOL PopNeedMami();
    void PushNeedMaruka(){ needmaruka_f_ = TRUE; };
    BOOL PopNeedMaruka();
    BOOL IsNeedCheck(){ return (needmami_f_ || needmaruka_f_); };
    void PushNewInfo(){ newinfo_f_ = TRUE; };
    BOOL IsNewInfo(){ return newinfo_f_; };
    BOOL PopNewInfo();
    void SetDrawFlag(s32 mami, s32 maruka);

    BOOL IsDrawMaruka(){ return drawmaruka_f_; };
    BOOL IsDrawMami(){ return drawmami_f_; };

    BOOL IsClearTip(){ return cleartip_f_; };

	void SetEventTimeAttack(BOOL isTimeAttack){ isEventTimeAttack_ = isTimeAttack; }
	BOOL IsEventTimeAttack(void){ return isEventTimeAttack_; }

	void SetEventTour(BOOL isTour){ isEventTour_ = isTour; }
	BOOL IsEventTour(void){ return isEventTour_; }


	void SetShiaiRanking(BOOL isRank){ isShiaiRanking_ = isRank; }
	BOOL GetShiaiRanking(void){ return isShiaiRanking_; }

#ifndef __K_MAKE_HIDE_EVENT_SOUND__
	void SetMapMidBGM(s64 mid_bgm_map){ mid_bgm_map_ = mid_bgm_map; }
	s64 GetMapMidBGM(void){ return mid_bgm_map_; }
#endif


  protected:
    BOOL PopFlag(BOOL& flag);
    BOOL nosv_f_;//サーバー不使用（ＷＩＮデバッグ）
    BOOL stg_f_;//ステージング（デバッグ）
    BOOL cleartip_f_;//クリア済みフラグ
    BOOL needomakase_f_;//お任せセットが必要
    BOOL needevent_f_;//イベント更新が必要
    BOOL needmami_f_;//おくりものＢＯＸ（まみ）更新が必要
    BOOL needmaruka_f_;//まるか更新が必要
    BOOL newinfo_f_;//あたらしいお知らせあり
    s32 mid_maptip_;//マップチップ行番号
    s32 lno_maptip_;//マップチップ行番号
    s32 shiaitype_;//試合の種類
    s64 id_shot_prev_;//プレビュー用シュートＩＤ
    s32 mid_event_;//イベントマスターＩＤ
    s32 wid_event_;//イベントワールドＩＤ
    enTutoType tutoType_;
    BOOL drawmami_f_;//まみ描画
    BOOL drawmaruka_f_;//まるか描画
    BOOL homecourt_f_;//ホームコート

	BOOL isEventTimeAttack_;	// タイムアタックイベント
	BOOL isEventTour_;			// 勝ち抜きイベント
	BOOL isShiaiRanking_;	// 試合ランキング

#ifndef __K_MAKE_HIDE_EVENT_SOUND__
	s32 mid_bgm_map_;
#endif
  };

}
