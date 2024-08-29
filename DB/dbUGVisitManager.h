#pragma once
#include "../DB/dbUGChGen.h"
namespace db
{
	class TUGChAnotherKantoku;
	class TUGChAnotherCaptain;

	class TUGVisitManager
	{
		static const s32 POSITION_NUM = 4;

	public:
		// インスタンス取得
		static TUGVisitManager* GetInstance(void);
		static TUGVisitManager* Create(db::TUIGenLayer* player, db::TUIObj* parent);
		void Destroy(void);

		// ロビーキャラ生成
		void SetupVisitChara(void);
#ifdef __K_DEBUG_PAD_MODE__
		void SetupDebug(void);
#endif

		void Update(BOOL draw_f);

	private:

		db::TUIGenLayer* pPlayer_;
		db::TUIObj* pParent_;

		// 使用してる場所チェック
		BOOL isUsePosition_[POSITION_NUM];

		// でばっぐ用位置とチーム紐付け
		s32 dbgUsePositionTeam_[POSITION_NUM];


		std::vector<TUGChAnotherKantoku*> ugVecAnyKantoku_;
		std::vector<TUGChAnotherCaptain*> ugVecAnyCaptain_;

		// コンストラクタ
		TUGVisitManager(db::TUIGenLayer* player, db::TUIObj* parent);
		// コピーコンストラクタ
		TUGVisitManager(const TUGVisitManager& r){}
		// 代入演算子
		TUGVisitManager& operator=(const TUGVisitManager& r){ return *this; }
		// デストラクタ
		virtual ~TUGVisitManager(void);

		static TUGVisitManager* pInstance_;
	};
}
