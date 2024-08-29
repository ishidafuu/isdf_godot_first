#pragma once
#include "dbInclude.h"
#include "dbUGGen.h"
#include "dbUIObj.h"
#include "dbUGButton.h"
//=====================================================================================================
//
//	Tipsキャラ用パラメータクラス
//
//=====================================================================================================
namespace db
{

	class TUGTipsParam
	{
	public:
		TUGTipsParam(void);
		virtual ~TUGTipsParam(void);

#ifdef __K_DEBUG_ROOT__
		db::TUIGenLayer* pPlayer_;
		void SetupDebugParam(void);	//共通処理
		void SetupDebugParam1(void);
		void SetupDebugParam2(void);
		void SetupDebugParam3(void);
#endif
		// 内部で抽選
		void SetupParam(s32 id);

		//------------------------------------------------------------
		// セッター
		void SetID(s32 id){ id_ = id; }
		void SetName(std::string n){ name_ = n; }
		void SetPersonType(s32 type){ personType_ = type; }
		void SetMoveType(s32 type){ moveType_ = type; }
		void SetItem(s32 no){ item_ = no; }
		void SetChDt(s32 hair, s32 face, s32 haircol, s32 skincol, s32 eyecol, s32 acccol, s32 wearcol, s32 pantscol, s32 sidecol, s32 undercol, s32 shoecol, BOOL gb_f)
		{
			hairNo_ = hair;
			faceNo_ = face;
			colHair_ = haircol;
			colSkin_ = skincol;
			colEye_ = eyecol;
			colAcc_ = acccol;
			colWear_ = wearcol;
			colPants_ = pantscol;
			colSide_ = sidecol;
			colUnder_ = undercol;
			colShoe_ = shoecol;
			isGB_ = gb_f;
		}
		void SetSpeedType(s32 type){ speedType_ = type; }
		void SetPercent(s32 percent){ selectPercent_ = percent; }
		void SetTerminalType(s32 type){ terminalType_ = type; }
		void SetText(STRVECTOR2 v) { msg_ = v; }
		void SetText(s32 pageNo, STRVECTOR v) { msg_[pageNo] = v; }
		void SetDate(s64 st_date, s64 ed_date) { startDate_ = st_date; endDate_ = ed_date; }
		void SetEndTips(BOOL isEnd){ isEndTips_ = isEnd; }
		//------------------------------------------------------------
		// ゲッター
		s32 GetID(void){ return id_; }
		std::string GetName(void){ return name_; }
		s32 GetPersonType(void){ return personType_; }
		s32 GetMoveType(void){ return moveType_; }
		s32 GetItem(void){ return item_; }
		s32 GetFaceNo(void){ return faceNo_; }
		s32 GetHairNo(void){ return hairNo_; }
		s32 GetColorEye(void){ return colEye_; }
		s32 GetColorSkin(void){ return colSkin_; }
		s32 GetColorHair(void){ return colHair_; }
		s32 GetColorAccessories(void){ return colAcc_; }
		s32 GetColorWear(void){ return colWear_; }
		s32 GetColorPants(void){ return colPants_; }
		s32 GetColorSide(void){ return colSide_; }
		s32 GetColorUnder(void){ return colUnder_; }
		s32 GetColorShoe(void){ return colShoe_; }
		BOOL IsColorGB(void){ return isGB_; }
		s32 GetSpeedType(void){ return speedType_; }
		s32 GetSelectPercent(void){ return selectPercent_; }
		STRVECTOR2 GetText(void){ return msg_; }
		STRVECTOR GetText(s32 pageNo){ return msg_[pageNo]; }
		std::string GetText(s32 pageNo, s32 no)
		{
			if (pageNo >= msg_.size()){ return ""; }
			if (no >= msg_[pageNo].size()){ return ""; }
			return msg_[pageNo][no];
		}

		BOOL IsEndTips(void){ return isEndTips_; }

		// この端末で出すか
		BOOL IsSelectTerminal(void);

		// 今Tipsキャラを表示する時間かどうか
		BOOL IsDrawDate(void);

	protected:

		s32 id_;
		std::string name_;
		s32 personType_;
		s32 moveType_;
		s32 item_;			// 未実装
		s32 faceNo_;
		s32 hairNo_;
		s32 colEye_;
		s32 colSkin_;
		s32 colHair_;
		s32 colAcc_;
		s32 colWear_;
		s32 colPants_;
		s32 colSide_;
		s32 colUnder_;
		s32 colShoe_;
		s32 isGB_;
		s32 speedType_;
		s32 selectPercent_;
		s32 terminalType_;
		STRVECTOR2 msg_;

		s64 startDate_;		// 表示開始時間
		s64 endDate_;		// 表示終了時間

		BOOL isEndTips_;
	};
}
