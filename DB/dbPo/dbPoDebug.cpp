#include "dbPoDebug.h"

namespace db
{
  TPostDebug::TPostDebug(TMgEngineObj* pmgEO):TPoGen(pmgEO)
  {
  }

  TPostDebug::~TPostDebug(void)
  {
  }
  void TPostDebug::Ready(enPostDebug debug)
  {
    ClearMap();
    posttype_ = debug;
    sendmap_.longmap_["dummy"] = 0;
    switch (debug)
    {
    case db::enPostDebug_AddItem:StartConnect("dbg_additem"); break;
    case db::enPostDebug_ZeroItem:StartConnect("dbg_zeroitem"); break;
    case db::enPostDebug_AddCredit:StartConnect("dbg_addcredit"); break;
    case db::enPostDebug_ZeroCredit:StartConnect("dbg_zerocredit"); break;
    case db::enPostDebug_DecStamina: StartConnect("dbg_decstamina"); break;
    case db::enPostDebug_ZeroShot:StartConnect("dbg_zeroshot"); break;
    case db::enPostDebug_AddShot:StartConnect("dbg_addshot"); break;
    case db::enPostDebug_ResetLogbo: StartConnect("dbg_resetlogbo"); break;
    case db::enPostDebug_AddTeam:StartConnect("dbg_addteam"); break;
	case db::enPostDebug_TeamLvMax:StartConnect("dbg_teamlvmax"); break;
	case db::enPostDebug_TeamLvMin:StartConnect("dbg_teamlvmin"); break;
	case db::enPostDebug_TeamChLvMax:StartConnect("dbg_teamchlvmax"); break;
	case db::enPostDebug_TeamChLvMin:StartConnect("dbg_teamchlvmin"); break;
    case db::enPostDebug_AddGiftBox: StartConnect("dbg_addgiftbox"); break;
    case db::enPostDebug_DelDock: StartConnect("dbg_deldock"); break;
    case db::enPostDebug_ResetNaBox: StartConnect("dbg_resetnabox"); break;
    case db::enPostDebug_AddHaikei:StartConnect("dbg_addhaikei"); break;
    case db::enPostDebug_AddBGM:StartConnect("dbg_addbgm"); break;
    case db::enPostDebug_AddMap:StartConnect("dbg_addmap"); break;
    case db::enPostDebug_DelHaikeiBGM:StartConnect("dbg_delhaikeibgm"); break;
    case db::enPostDebug_ResetTuto:StartConnect("dbg_resettuto"); break;
    case db::enPostDebug_ResetTutoSub:StartConnect("dbg_resettutosub"); break;
    case db::enPostDebug_DelKantoku:StartConnect("dbg_delkantoku"); break;
    default:
      break;
    }
  }
  
}
