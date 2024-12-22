#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGRcGen.h"
#include "dbUGRBlink.h"
namespace db
{
	class TUGFinger;

  class TUGRcNormal :public TUGRcGen
  {
  protected:
    static const s32 POSXL = 24;
    static const s32 POSXR = 142;
    static const s32 POSY = 8;
    static const s32 SUBY = 3;
    static const s32 RECORDH = 24;
    s32 DoMakeGroupSub();
    void DoSetLimY(s32 limu, s32 hhh);
    void DoUpdateSub(BOOL draw_f);

    virtual void DoMakeGroupSub2(){}
    virtual void DoSetLimY2(s32 limu, s32 hhh){}
    virtual void DoUpdateSub2(BOOL draw_f){}
  public:
    TUGRName ugname_;
    TUGRBlink ugsubname_;
    TUIObj* subname2_;	// BGMリストの未所持曲のところに表示する"BOX NAME"用テキストオブジェ
    TUGRName ugvalue_;
    TUIObj* subvalue_;
	TUIObj* subvalue2_; // まるか運送のところに表示する"KIGEN"用テキストオブジェ
	TUGFinger* ugfinger_;	// チュートリアル指さし用
    s64 id_common_;//汎用ＩＤ
    s64 etcnum_;//汎用ＩＤ
	LONGVECTOR data_; // 汎用s64型vector
    s32 brink_c_;
    //コンストラクタ
    explicit TUGRcNormal(TUIGenLayer* player);
    virtual ~TUGRcNormal(void);
    void SetShopRecord();
    void SetName(const char* name);
    void SetNameIcon(const char* name, enUGPIcon iconno);
    void SetValue(const char* value);
    void SetValue_Star(s32 value);
    void SetValue_Todd(s32 value);
    void SetValue_Huku(s32 value);
    void SetValue_Gacha(s32 value);
	void SetValue_Coach(s64 mid_supply, s32 value);
    void SetSubName(const char* subname);
    void SetSubName(const char* subname, s32 fontcol, BOOL brink_f);
    void SetSubName2(const char* subname);
    void SetSubName2(const char* subname, s32 fontcol);
    void SetSubValue(const char* subvalue);
	void SetSubValue2(const char* subvalue);
	void SetupFinger(BOOL draw_f, f32 offsetX = 0, f32 offsetY = 0);
    //void SetNamePosLeft();
    void SetCommonID(s64 id_common);
    s64 GetCommonID();
    void SetEtcNum(s64 etcnum);
    s64 GetEtcNum();
	void SetData(LONGVECTOR data);
	LONGVECTOR GetData(void);
  };
}
