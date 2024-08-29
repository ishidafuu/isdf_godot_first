#include "dbDtCharaSetting.h"

namespace db
{
	TDtCharaSetting::TDtCharaSetting(void)
	{
	}

	TDtCharaSetting::~TDtCharaSetting(void)
	{
	}

	//読み込み
	void TDtCharaSetting::LoadDt(const char *CSVDir, const char *CSVFilename)
	{
		//dt_->clear();
		mid::TCSV csv;
		csv.midLoadCSV(CSVDir, CSVFilename, FALSE, TRUE);

		//s32の0番だけ
		//for (s32 i = 0; i < endbSetDtEND; ++i)
		//{
		//  if (i < csv.longvec2_.size())

		for (s32 i = 0; i < csv.GetLine(); ++i)
		{
			//for (s32 i2 = 0; i2 < csv.GetIntSize(0); ++i2)
			{
				dt_[i] = csv.longvec2_[i][0];
			}
		}
	}

	BOOL TDtCharaSetting::IsOpenTraingMenu(void)
	{
		s32 ret = GenGetDt(setOpenTrainingMenu);
		return (ret == 1) ? TRUE : FALSE;
	}

	s32 TDtCharaSetting::GetDt(endtSet_Param dtNo)
	{
		return GenGetDt(dtNo);
	}
	s32 TDtCharaSetting::GetBodytypeDt(endtSet_BodyType dtNo)
	{
		return GenGetDt(dtNo);
	}
	s32 TDtCharaSetting::GetCharacterTypeDt(endtSet_CharacterType dtNo)
	{
		return GenGetDt(dtNo);
	}

	s32 TDtCharaSetting::GenGetDt(s32 dtNo)
	{
		return dt_[dtNo];
	}
}
