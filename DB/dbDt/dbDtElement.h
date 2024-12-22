#pragma once
#include "dbInclude.h"
////#include "dbMgStScene.h"

namespace db
{
    class TDtElement
    {
    protected:
#ifdef __K_DEBUG_SHIAI__
		void GenLoadDt(const char *CSVDir, const char *CSVFilename, INTVECTOR2* intvec2, STRVECTOR2* strvec2);
#else
      void GenLoadDt(const char *CSVDir, const char *CSVFilename, INTVECTOR2* intvec2);
#endif // #ifdef __K_DEBUG_SHIAI__
    public:
      INTVECTOR2 change_;
      INTVECTOR2 effect_;
      INTVECTOR2 shape_;
      INTVECTOR2 orbit_;
      INTVECTOR2 target_;

      explicit TDtElement(void);
      virtual ~TDtElement(void);
      void LoadDt(const char *CSVDir, const char *CSV_Change, const char *CSV_Effect, const char *CSV_Shape, const char *CSV_Orbit, const char *CSV_Target);

      s32 GetChangeElmNo(s32 id);
      s32 GetEffectElmNo(s32 id);
      s32 GetShapeElmNo(s32 id);
      s32 GetOrbitElmNo(s32 id);
      s32 GetTargetElmNo(s32 id);

      s32 GetChangeDt(s32 id, s32 dtNo);
      s32 GetEffectDt(s32 id, s32 dtNo);
      s32 GetShapeDt(s32 id, s32 dtNo);
      s32 GetOrbitDt(s32 id, s32 dtNo);
      s32 GetTargetDt(s32 id, s32 dtNo);

      void SetChangeDt(s32 id, s32* dt1);
      void SetChangeDt(s32 id, s32* dt1, s32* dt2);

      void SetTargetDt(s32 id, s32* dt1);
      void SetTargetDt(s32 id, s32* dt1, s32* dt2);

      void SetOrbitDt(s32 id, s32* dt1, s32* dt2, s32* dt3);
      void SetOrbitDt(s32 id, s32* dt1, s32* dt2, s32* dt3, s32* dt4, s32* dt5, s32* dt6, s32* dt7, s32* dt8);

      void SetEffectDt(s32 id, s32* dt1);
      void SetEffectDt(s32 id, s32* dt1, s32* dt2);
      void SetEffectDt(s32 id, s32* dt1, s32* dt2, s32* dt3);
      void SetEffectDt2(s32 id, s32* dt1, s32* dt2, s32* dt3);
      void SetEffectDt3(s32 id, s32* dt1, s32* dt2, s32* dt3);
      void SetEffectDt4(s32 id, s32* dt1, s32* dt2, s32* dt3);



#ifdef __K_DEBUG_SHIAI__
	  STRVECTOR2 changeStr_;
	  STRVECTOR2 effectStr_;
	  STRVECTOR2 shapeStr_;
	  STRVECTOR2 orbitStr_;
	  STRVECTOR2 targetStr_;

	  std::string GetChangeElmName(s32 id);
	  std::string GetEffectElmName(s32 id);
	  std::string GetShapeElmName(s32 id);
	  std::string GetOrbitElmName(s32 id);
	  std::string GetTargetElmName(s32 id);
#endif	// #ifdef __K_DEBUG_SHIAI__
	};
}
