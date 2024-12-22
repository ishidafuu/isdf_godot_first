#pragma once
#include "../DB/dbUGChGen.h"
#include "dbUGSerifu.h"
#include "dbUGTipsParam.h"
namespace db
{
	class TUGChTipsPerson;

	// Tipsキャラの速度
	static const f32 PERSON_SPEED_A = 1.4f;
	static const f32 PERSON_SPEED_B = 1.2f;
	static const f32 PERSON_SPEED_C = 1.05f;
	static const f32 PERSON_SPEED_D = 0.9f;
	static const f32 PERSON_SPEED_E = 0.8f;
	static const f32 PERSON_SPEED_F = 0.7f;

	// 端末
	enum enSelectTerminalType
	{
		terminal_ALL = 0,
		terminal_IOS,
		terminal_ANDROID,
	};

	// Tipsキャラのルート
	enum enPersonLine
	{
		PERSON_LINE_A = 0,
		PERSON_LINE_B,
		PERSON_LINE_C,

		PERSON_LINE_MAX,
	};

	class TUGTipsManager
	{

	public:
		// インスタンス取得
		static TUGTipsManager* GetInstance(void);
		static TUGTipsManager* Create(db::TUIGenLayer* player, db::TUIObj* parent);
		void Destroy(void);

		// ロビーキャラ生成
		void Setup(void);
		BOOL SetupParam(s32 no);
		void ClearParam(s32 no);

		BOOL IsSelect(void);
		s32 GetSelectNo(void);
		f32 GetPersonPosX(s32 no);
		f32 GetPersonPosY(s32 no);
		void SetStopPerson(s32 no);
		void SetPersonFrameOutR(s32 no);
		void SetPersonFrameOutL(s32 no);

		void ShowTips(s32 no, BOOL isMirror);
		BOOL IsEndTips(s32 no);
		void CloseTips(void);

		void SetDrawTipsBtnALL(BOOL isDraw);
		void SetDrawTipsBtn(s32 no, BOOL isDraw);

		void Update(BOOL draw_f);

	private:

		db::TUIGenLayer* pPlayer_;
		db::TUIObj* pParent_;

		TUGSerifu* ugSerifu_;	//セリフ
		STRVECTOR serifuVec_;
		s32 msgNo_;
		s32 tipsPersonNo_;

		BOOL isUnDrawBtn_;

		s32 timecheck_c_;

		// 各位置の出現までのタイマー
		s32 intervalTimer_[PERSON_LINE_MAX];
		
		std::vector<TUGChTipsPerson*> ugVecTipsPerson_;
		std::vector<TUGTipsParam> tipsParam_;

		BOOL ChooseParam(s32 no);

		void SetMsg(s32 no, STRVECTOR strVec);
		void SetMsg(s32 no, const char* text0, const char* text1, const char* text2);
		void ConvPersonName(s32 no, std::string* s);

		// コンストラクタ
		TUGTipsManager(db::TUIGenLayer* player, db::TUIObj* parent);
		// コピーコンストラクタ
		TUGTipsManager(const TUGTipsManager& r){}
		// 代入演算子
		TUGTipsManager& operator=(const TUGTipsManager& r){ return *this; }
		// デストラクタ
		virtual ~TUGTipsManager(void);

		static TUGTipsManager* pInstance_;
	};
}
