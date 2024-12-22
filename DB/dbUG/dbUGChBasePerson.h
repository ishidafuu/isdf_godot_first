#pragma once
#include "dbUGChGen.h"
//=============================================================================================
//
//	Tipsキャラベースクラス
//
//=============================================================================================
namespace db
{
	//モーション
	enum enPersonMotionType
	{
		pmt_Sei_St = 0,//制服立ち
		pmt_Sei_Ds,//制服歩き
		pmt_Org_St,//オリジナル立ち
		pmt_Org_Ds,//オリジナル走り
		pmt_Soc_St,//サッカー立ち
		pmt_Soc_Ds,//サッカー走り
		pmt_Uni_St,//ユニフォーム立ち
		pmt_Uni_Ds,//ユニフォーム走り
		pmt_Jid_St,//時代劇立ち
		pmt_Jid_Ds,//時代劇走り
		pmt_Bsk_St,//バスケ立ち
		pmt_Bsk_Ds,//バスケ走り
		pmt_Mas_St,//マーシャル立ち
		pmt_Mas_Ds,//マーシャル走り
		pmt_Kan_St,//カンフー立ち
		pmt_Kan_Ds,//カンフー走り
		pmt_Jud_St,//柔道家立ち
		pmt_Jud_Ds,//柔道家走り
		pmt_Hok_St,//ホッケー立ち
		pmt_Hok_Ds,//ホッケー走り
	};

	// キャラタイプ
	enum enPersonType
	{
		pt_Seifuku = 0,			// 制服
		pt_OriginalUniform,		// ゲーム内ユニフォーム
		pt_Soccer,				// サッカー
		pt_Uniform,				// ユニフォーム
		pt_Jidaigeki,			// 時代劇
		pt_BasketBall,			// バスケ
		pt_Martial,				// マーシャル
		pt_KangFu,				// カンフー
		pt_Judo,				// 柔道
		pt_Hockey,				// ホッケー
		
		pt_Max,
	};
	// モーションタイプ
	enum enPersonActType
	{
		pat_Stand = 0,
		pat_Walk,
		pat_Run,

		pat_Max,
	};


	//リスト
	class TUGChBasePerson : public TUGChGen
	{
		public:
			//コンストラクタ
			explicit TUGChBasePerson(TUIGenLayer* player, s32 no);
			// デストラクタ
			virtual ~TUGChBasePerson(void);

			void SetChDt(s32 teamid, s32 charano);
			void SetChDt(s32 hair, s32 face, s32 haircol, s32 skincol, s32 eyecol, s32 acccol, s32 wearcol, s32 pantscol, s32 sidecol, s32 undercol, s32 shoecol, BOOL gb_f);
			void SetChDtRnd(void);	// デバッグ機能

			void MakeGroup(TUIObj* parent);

			void SetPersonType(enPersonType type);
			void SetPersonActType(enPersonActType type);

			void SetMType(enPersonMotionType mtype);
			void z_MoveWork(s32 moveno, f32 tagx, f32 tagy, f32 spd);
			void z_MoveWork(s32 moveno, f32 tagx, f32 tagy, f32 spd, enUGChGenMuki muki);
			void z_MoveWork_Full(s32 moveno, f32 tagx, f32 tagy, f32 spd);
			void z_MoveWork_Full(s32 moveno, f32 tagx, f32 tagy, f32 spd, f32 lookx);
			void z_MoveWork_FullMuki(s32 moveno, f32 tagx, f32 tagy, f32 spd, enUGChGenMuki muki);
			void z_SetStop(void);

		protected:
			void DoActionSub();

			s32 no_;
			enPersonType personType_;
			enPersonActType personActType_;
	};
}
