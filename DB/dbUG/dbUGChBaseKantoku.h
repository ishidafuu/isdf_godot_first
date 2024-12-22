#pragma once
#include "dbUGChGen.h"
//=============================================================================================
//
//	かんとくベースクラス
//
//=============================================================================================
namespace db
{
	//モーション
	enum enKantokuMotionType
	{
		kmt_St,//立ち
		kmt_Wk,//歩き
		kmt_Ds,//走り
		kmt_Jp,//ジャンプ
		kmt_Cr,//着地
		kmt_Sit,//ソファー座り
		kmt_Shop,//店
		kmt_ShopOrd,//注文
		kmt_ShopOK,//うなずく
		kmt_ShopNG,//首振り
		kmt_Take,//受け取る
		kmt_Punch,//パンチ
		kmt_Upper,//アッパー
		kmt_TameUpper,//ためありアッパー
		kmt_Icon,//アイコン
	};
	//リスト
	class TUGChBaseKantoku : public TUGChGen
	{
		public:
			//コンストラクタ
			explicit TUGChBaseKantoku(TUIGenLayer* player);
			// デストラクタ
			virtual ~TUGChBaseKantoku(void);

			void MakeGroup(TUIObj* parent, TMgCharGrp* pmgcg);
			void SetMType(enKantokuMotionType mtype);
			void z_MoveWork(s32 moveno, f32 tagx, f32 tagy, f32 spd);
			void z_MoveWork(s32 moveno, f32 tagx, f32 tagy, f32 spd, enUGChGenMuki muki);
			void z_MoveWork_Full(s32 moveno, f32 tagx, f32 tagy, f32 spd);
			void z_MoveWork_Full(s32 moveno, f32 tagx, f32 tagy, f32 spd, f32 lookx);
			void z_MoveWork_FullMuki(s32 moveno, f32 tagx, f32 tagy, f32 spd, enUGChGenMuki muki);

		protected:
			void DoActionSub();
	};
}
