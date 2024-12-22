#pragma once
#include "dbInclude.h"

//グローバル扱いな変数
namespace db
{

  enum enSendTutoAct
  {
    sta_Intro,
    sta_Dodge,
    sta_DodgeAct,
    sta_Walk,
    sta_WalkAct,
    sta_Jump,
    sta_JumpAct,
    sta_Catch,
    sta_CatchAct,
    sta_Dash,
    sta_DashAct,
    sta_Shot,
    sta_ShotAct,
    sta_Game,
    sta_GameAct,
    sta_Fin,
  };


  enum enRcvTutoAct_Dodge
  {
    rta_dodge_ok,
    rta_dodge_ngpos,
    rta_dodge_ngearly,
    rta_dodge_nglate,
  };

  enum enRcvTutoAct_Catch
  {
    rta_catch_ok,
    rta_catch_ngpos,
    rta_catch_ngearly,
    rta_catch_nglate,
  };

  enum enRcvTutoAct_Walk
  {
    rta_walk_ok,
    rta_walk_ngpos,
  };

  enum enRcvTutoAct_Dash
  {
    rta_dash_ok,
    rta_dash_ngpos,
    rta_dash_ngwalk,
    rta_dash_ngjump,
  };

  enum enRcvTutoAct_Jump
  {
    rta_jump_ok,
    rta_jump_ngpos,
    rta_jump_ngdash,
  };

  enum enRcvTutoAct_Shot
  {
    rta_shot_ok,
    rta_shot_ngpos,
    rta_shot_ngovline,
  };

}
