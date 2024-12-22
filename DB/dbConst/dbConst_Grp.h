#pragma once
#include "dbInclude.h"

//グローバル扱いな変数
namespace db
{
  //フォントカラー
  enum enFontCol2
  {
    FC2_normal = base::FC2_normal,
    FC2_shiai,
    FC2_mini,
    FC2_support,
    FC2_rareD,
    FC2_rareC,
    FC2_rareB,
    FC2_rareA,
    FC2_rareS,
    FC2_rareSS,
    FC2_rareSSS,
    FC2_list_yellow,
    FC2_list_green,
    FC2_list_gray,
    FC2_list_cyan,
    FC2_list_gold,
    FC2_list_blue,
    FC2_record_yellow,
    FC2_record_gray,
    FC2_record_gold,
    FC2_record_blue,
    FC2_hl_normal,
    FC2_hl_mark,
    FC2_hl_mini,
    FC2_btn_normal,
    FC2_btn_mark,
    FC2_btn_mini,
    FC2_btn_normal_gray,
    FC2_btn_mark_gray,
    FC2_btn_mini_gray,
    FC2_hd_normal,
    FC2_hd_mark,
    FC2_hd_mini,
    FC2_num_red,
    FC2_num_yellow,
    FC2_num_blue,
    FC2_dock_add,
    FC2_hmshiai_event,
    FC2_hmshiai_com,
    FC2_hmshiai_vs,
    FC2_map_boss,
    FC2_input_word,
    FC2_input_tab,
    FC2_input_hilight,
    FC2_avt_film,
    FC2_avt_no,
	FC2_red,
    FC2END,
  };

  enum enObjRectType
  {
    obmt_CharShadow,
    obmt_BallShadow,
    obmt_MiniShadow,
    obmt_CursorD_ALL,
    obmt_CursorD_COM,
    obmt_CursorD_1P,
    obmt_CursorD_2P,
    obmt_CursorD_EN,
    obmt_CursorD_4P,
    obmt_Todd,
    obmt_TeamD,
    obmt_TeamC,
    obmt_TeamB,
    obmt_TeamA,
    obmt_TeamS,
    obmt_TeamSS,
    obmt_TeamSSS,
    obmt_SozaiD,
    obmt_SozaiC,
    obmt_SozaiB,
    obmt_SozaiA,
    obmt_SozaiS,
    obmt_SozaiSS,
    obmt_SozaiSSS,
    obmt_HPBarBase,
    obmt_HPBar2,
    obmt_StepBase,
    obmt_StepVJ,
    obmt_StepLightD,
    obmt_StepLightC,
    obmt_StepLightB,
    obmt_StepLightA,
    obmt_StepLightS,
    obmt_StepLightSS,
    obmt_StepLightSSS,
    obmt_StepVJD,
    obmt_StepVJC,
    obmt_StepVJB,
    obmt_StepVJA,
    obmt_StepVJS,
    obmt_StepVJSS,
    obmt_StepVJSSS,
    obmt_Event0,
    obmt_Event1,
    obmt_Event2,
    obmt_Event3,
    obmt_END,
  };

  //描画
  const s32 ZMAG = 1000;//描画深度倍率
  const s32 PRVLAYRE = 20;
  const s32 PRVLAYRE_BODY = 10;
  const s32 PRVLAYRE_BALL = 15;
  const s32 PRVLAYRE_OV = 18;
  const s32 HAIRW = 24;
  const s32 HAIRH = 16;
  const s32 FACEW = 16;
  const s32 FACEH = 8;
  const s32 BODYW = 32;
  const s32 BODYH = 40;
  const s32 CHESTH = 20;
  const s32 CHESTW = 24;
  const s32 MOUSEW = 16;
  const s32 MOUSEH = 8;
  const s32 BALLSIZE = 32;
  const s32 SDWALP = 200;
  const s32 SHADOWCOL_NAZO = 160;
  const s32 SHADOWCOL = 48;

  //コートライン
  const s32 LINECOL[][3] =
  {
    { 255, 255, 255 },//白
    { 200, 200, 200 },//グレー
    { 255, 255, 232 },//セピア
    { 160, 144, 128 },//海岸
    { 255, 240, 192 },//銭湯
    { 176, 160, 112 },//公園
    { 240, 232, 224 },//神社
    { 32, 32, 32 }//なぞ
  };

}
