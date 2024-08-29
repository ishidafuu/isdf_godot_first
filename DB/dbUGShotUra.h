#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGGen.h"
#include "dbUGShot.h"
#include "dbUGButton.h"
#include "dbUGRQuality.h"
namespace db
{
  struct TShotUraSt
  {
    s32 urano_;
    s32 rarity_;
    BOOL mixed_f_;
    void Init()
    {
      Zeromem(this);
      //tid_ = NGNUM;
    }
  };

  //リスト
  class TUGShotUra : public TUGGen
  {
    static const s32 TEXTNUM = 4;
    static const s32 GENRENUM = 8;
  public:
    //コンストラクタ
    explicit TUGShotUra(TUIGenLayer* player);
    virtual ~TUGShotUra(void);
    void MakeGroup(TUIObj* parent);
    void SetShotUraDt(s32 id_shot);
    void SetShotUraDt(LONGVECTOR* id_shotvec);
	void SetShotUraDt(SOZAIDTVECTOR id_shotvec);
    void SetShotUraDtBuy(s32 mid_shot);
    void SetUraNo(s32 urano);
  protected:
    void DoUpdate(BOOL draw_f);
    void DoAction();
    void GenUraDt(TSozaiData& sozaidt);
    void SetSlot(s32 slotno, const TSozaiData& sozai, s32 baserarity);
    void ClearShotName();
    TUIObj* slotname_[HSELEMMAX_ADD];
    std::vector<TUGRQuality*> ugpow_;
    std::vector<TUGRQuality*> ugspin_;
    TUIObj* text_[TEXTNUM];
    TUIObj* basename_;
    std::vector<TUGRIcon*> uggenre_;

    TShotUraSt stShotUra_;
  };
 
}
