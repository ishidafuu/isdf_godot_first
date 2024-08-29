#include "dbUGLiItemSet.h"
#include "dbUILayer.h"
#include "dbUGRcNormal.h"
//==============================================================================================================================================
//
//
//
//==============================================================================================================================================
namespace db
{
	//----------------------------------------------------------------------------------
	//コンストラクタ
	//----------------------------------------------------------------------------------
	TUGLiItemSet::TUGLiItemSet(TUIGenLayer* player)
		:TUGLiGen(player)
	{
		stLiItemGet_.Init();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	TUGLiItemSet::~TUGLiItemSet()
	{
		//解放時必ず呼ぶ
		ClearRecord();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	s64 TUGLiItemSet::GetCommonID()
	{
		return records_[GetActIndex()]->GetCommonID();
	}
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	s64 TUGLiItemSet::GetEtcNum()
	{
		return records_[GetActIndex()]->GetEtcNum();
	}
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	LONGVECTOR TUGLiItemSet::GetData(void)
	{
		return records_[GetActIndex()]->GetData();
	}



	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void TUGLiItemSet::Refresh(LONGVECTOR2 data)
	{
		enum paramType
		{
			ptType = 0,
			ptID,
			ptNum,
		};

		//レコードクリア
		ClearRecord();

		//獲得ログからレコード作成
		s32 num = data.size();

		INTVECTOR2 team;
		const s32 MIDTEAM = 0;
		const s32 HAVENUM = 1;
		const s32 DBLNUM = 2;

		// チームカードの処理を先にしておく
		for (s32 i = 0; i < num; ++i)
		{
			enItemType itemtype = (enItemType)data[i][ptType];
			if (itemtype != enItemType_Team) continue;
			s64 itemid = data[i][ptID];
			BOOL find_f = FALSE;
			for (s32 i2 = 0; i2 < team.size(); ++i2)
			{
				//ダブり
				if (team[i2][MIDTEAM] == itemid)
				{
					++team[i2][DBLNUM];
					find_f = TRUE;
					break;
				}
			}

			//初
			if (find_f == FALSE)
			{
				INTVECTOR newrec;
				newrec.push_back(itemid);//MIDTEAMチームＭＩＤ
				for (s32 i3 = 0; i3 < pmgEO_->mgCoU_.mdTeam_.GetLineNum(); ++i3)
				{
					if (itemid == pmgEO_->mgCoU_.mdTeam_.GetLineDt(i3, mdteam_id_m_team))
					{
						newrec.push_back(pmgEO_->mgCoU_.mdTeam_.GetLineDt(i3, mdteam_overlap));//HAVENUM現在の枚数
						break;
					}
				}
				newrec.push_back(0);//DBLNUM獲得枚数
				team.push_back(newrec);
			}
		}

		
		for (s32 i = 0; i < num; ++i)
		{
			TUGRcNormal* newrecord = new TUGRcNormal(player_);
			s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
			newrecord->GetField()->b_SetModal(TRUE);
			newrecord->SetCommonID(i);
			enItemType itemtype = (enItemType)data[i][ptType];
			s64 itemid = data[i][ptID];
			s64 itemnum = data[i][ptNum];

			switch (itemtype)
			{
			case enItemType_Team:
				for (s32 i2 = 0; i2 < team.size(); ++i2)
				{
					if (team[i2][MIDTEAM] == itemid)
					{
						s32 lv = (team[i2][HAVENUM] - team[i2][DBLNUM]);
						--team[i2][DBLNUM];
						if (lv == 1) newrecord->SetSubName("ＮＥＷ！", FC2_num_red, TRUE);
						else newrecord->SetSubName("ＬＥＶＥＬ＿ＵＰ！");
						newrecord->SetPos(24, 2);
						newrecord->ugname_.SetNameTeam_Get(itemid, lv);
						newrecord->SetEtcNum(lv);//そのチームカードの取得した段階の枚数
						break;
					}
				}
				break;
			case enItemType_Sozai:newrecord->ugname_.SetNameSozai2(itemid, FALSE); break;
			case enItemType_Shot: break;
			case enItemType_BGM:newrecord->ugname_.SetNameTape(itemid, FALSE); break;
			case enItemType_Haikei:newrecord->ugname_.SetNameHaikei(itemid); break;
			case enItemType_Supply:newrecord->ugname_.SetNameSupply(itemid, itemnum); break;
			case enItemType_Todd:newrecord->ugname_.SetNameTodd(itemnum); break;
			case enItemType_Star:newrecord->ugname_.SetNameStar(itemnum); break;
			case enItemType_Pack:newrecord->ugname_.SetNameItemPack(itemid, itemnum); break;
			case enItemType_Huku:newrecord->ugname_.SetNameHukuKen(itemnum); break;
			}
			newrecord->ugname_.SetIcon(TRUE);
			
			// データ入れておく
			newrecord->SetData(data[i]);

			GenMakeObj_Record(newrecord, rh);
			records_.push_back(newrecord);
		}

		SetZeroPos();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void TUGLiItemSet::DoUpdateSub(BOOL draw_f)
	{

	}
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void TUGLiItemSet::DoActionSub()
	{

	}
	//----------------------------------------------------------------------------------
	//リストクリア（変数部分的に初期化、レコードとレコードエンド解放）
	//----------------------------------------------------------------------------------
	void TUGLiItemSet::DoClearRecord()
	{
		stLiItemGet_.Init();
		for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);
		records_.clear();
	}

	void TUGLiItemSet::DoMakeGroupSub()
	{
		slider_->b_SetModal(TRUE);
	}



}
