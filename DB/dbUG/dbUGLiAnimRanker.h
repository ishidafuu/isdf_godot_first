#pragma once
#include "dbInclude.h"
#include "dbUGLiGen.h"
#include "dbUGRcAnimRanker.h"
#include "dbUGListEnd.h"
namespace db
{

  struct TUGLiAnimRankerSt
  {
    BOOL myscore_f_;
    void Init()
    {
      Zeromem(this);
    }
  };


  //リスト
  class TUGLiAnimRanker :public TUGLiGen
  {
  public:
	  std::vector<TUGRcAnimRanker*> records_;
	  std::vector<TUGRcNormal*> records2_;
	  //TUGListEnd uglistend_;
	  TUGLiAnimRankerSt stLiRanker_;

	  //コンストラクタ
	  explicit TUGLiAnimRanker(TUIGenLayer* player);
	  virtual ~TUGLiAnimRanker(void);

	  void Refresh(BOOL myscore_f);
	  void ChangeListEnd(BOOL listend_f);
	  //void SetListEnd(s32 recordidx);
	  s64 GetCommonID();


		// 描画制限を取得
	  s32 GetRecordLimU(void);
	  s32 GetRecordLimD(void);

	  // 登録されているレコードの番号を取得
	  s32 GetRecordNoByRank(s64 rank);

	  // レコードのX座標取得
	  s32 GetRecordPosX(void);

	  // スライドを計算に入れない位置取得
	  s32 GetPosYByListNo(s32 no);

	  // 指定したランクがリストの0番位置になるよう座標セット
	  void SetScrollPosByRank(s64 rank);

	  // 指定したランクのレコードの描画オンオフ
	  void SetDrawByRank(s64 rank, BOOL draw_f);

	  BOOL IsMove(void);
	  void SetMoveEnd(void);

	  // 指定したランクがリストの0番位置になるよう指定したフレーム数でスクロールアニメ
	  void SetScrollMoveByRank(s32 rank, s32 frame);

	  void AutoMove(s32 frame, s32 dist);


  protected:
    void DoUpdateSub(BOOL draw_f);
    void DoMakeGroupSub();
    void DoClearRecord();
    TUGRcNormal* MakeScoreRecord();
	
	std::vector<f32> moveList_;
	s32 initY_;
	BOOL isMove_;
	s32 moveNo_;
  };
 
}
