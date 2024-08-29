#pragma once
#include "dbUGChBasePerson.h"
#include "dbUGButton.h"
//=============================================================================================
//
//	Tipsキャラベースクラス
//
//=============================================================================================
namespace db
{
	//リスト
	class TUGChTipsPerson : public TUGChBasePerson
	{
		typedef TUGChBasePerson Base;

		public:
			//コンストラクタ
			explicit TUGChTipsPerson(TUIGenLayer* player, s32 no);
			// デストラクタ
			virtual ~TUGChTipsPerson(void);

			void MakeGroup(TUIObj* parent);
			void SetName(std::string name);
			void SetDrawBtn(BOOL isDraw){ btn_.SetDraw(isDraw); }

			BOOL IsSelect(void);

		protected:
			void DoUpdate(BOOL draw_f);
			void UpdateFollowParts(void);

			TUGButton btn_;

			std::string name_;
			s64 faceNo_;
			s64 HairNo_;

			// 色
			s32 colEye_;	// 眼
			s32 colSkin_;	// 肌
			s32 colHair_;	// 髪
			s32 colAcc_;	// アクセサリー

			s32 colWear_;	// ウェア
			s32 colPants_;	// パンツ
			s32 colSide_;	// サイドライン
			s32 colUnder_;	// アンダーライン
			s32 colShoe_;	// シューズ

			s32 speedType_;	// スピードタイプ

			STRVECTOR2 msg_;	// Tipsメッセージ

			// 歩きタイプ
			s32 moveType_;
	};
}
