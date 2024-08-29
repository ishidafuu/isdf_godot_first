#pragma once
#include "dbInclude.h"
#include "dbUGRc2Gen.h"
namespace db
{
  enum enUGListEndAct
  {
    enUGListEndAct_ok,
    enUGListEndAct_sub,
  };

  //リスト
  class TUGListEnd :public TUGRc2Gen
  {
  protected:
    void GenSetDataSupply(s64 mid_supply);
    void GenSetDataShotInv(s64 mid_supply);
    void GenSetDataItemPack(s64 mid_ItemPack);
    void SetInfoMsg(s32 gettype, s64 getinfo, s64 getinfo2);
  public:

    //コンストラクタ
    explicit TUGListEnd(TUIGenLayer* player);
    virtual ~TUGListEnd(void);
    s32 MakeGroup_ListEnd(TUIObj* parent, s32 limx, s32 limy);
    s32 MakeGroup_ListEnd_Shop(TUIObj* parent, s32 limx, s32 limy);
    void SetDataSpSyouhin(s64 mid_spsyouhin);
    void SetDataSpTechno(s64 mid_spsyouhin);
    void SetDataGacha(s64 mid_gacha);
    void SetDataGachaCnt(s64 mid_gachacnt);
    void SetDataNaBox(s64 mid_nabox, BOOL otona_f);
    void SetDataNaBoxCnt(s64 mid_naboxcnt);
    void SetDataHuku(s64 mid_huku);
    void SetDataHukuCnt(s64 mid_hukucnt);
    void SetDataGetLog(s64 id_itemget);
    void SetDataLogBuyStar(s64 id_bstar);
    void SetDataProf(s64 id_kantoku);
    void SetDataInfoStar();

    BOOL IsActRes(enUGListEndAct res);
  };
 
}
