#include "dbUGLiItemGetW.h"
#include "dbUILayer.h"
#include "dbUGRcNormal.h"

namespace db
{
  //コンストラクタ
  TUGLiItemGetW::TUGLiItemGetW(TUIGenLayer* player) :TUGLiGenW(player)
  {
    //stLiItemGet_.Init();
  }

  TUGLiItemGetW::~TUGLiItemGetW()
  {
    //解放時必ず呼ぶ
    ClearRecord();
  }

  //リストクリア（変数部分的に初期化、レコードとレコードエンド解放）
  void TUGLiItemGetW::DoClearRecord()
  {
    //stLiItemGet_.Init();
    for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);
    records_.clear();
  }

  void TUGLiItemGetW::DoMakeGroupSub()
  {
    slider_->b_SetModal(TRUE);
  }

  void TUGLiItemGetW::Refresh()
  {
    //GT_HUKUのときは福引き玉のアイコン

    //レコードクリア
    ClearRecord();

    //獲得ログからレコード作成
    s32 num = pmgEO_->mgCoU_.mdlog_ItemGet_.GetLineNum();

    INTVECTOR2 team;
    const s32 MIDTEAM = 0;
    const s32 HAVENUM = 1;
    const s32 DBLNUM = 2;
    for (s32 i = 0; i < num; ++i)
    {
      enItemType itemtype = (enItemType)pmgEO_->mgCoU_.mdlog_ItemGet_.GetLineDt(i, mdlog_itemget_itemType);
      if (itemtype != enItemType_Team) continue;
      s64 itemid = pmgEO_->mgCoU_.mdlog_ItemGet_.GetLineDt(i, mdlog_itemget_itemID);
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
      //s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
      s32 rh = newrecord->MakeGroup(slider_, stListW_.leftx_, stListW_.bottomy_, sliderx_, slidery_);
      newrecord->GetField()->b_SetModal(TRUE);
      newrecord->SetCommonID(i);
      enItemType itemtype = (enItemType)pmgEO_->mgCoU_.mdlog_ItemGet_.GetLineDt(i, mdlog_itemget_itemType);
      s64 itemid = pmgEO_->mgCoU_.mdlog_ItemGet_.GetLineDt(i, mdlog_itemget_itemID);
      s64 itemnum = pmgEO_->mgCoU_.mdlog_ItemGet_.GetLineDt(i, mdlog_itemget_itemNum);

      switch (itemtype)
      {
      case enItemType_Team:
        for (s32 i2 = 0; i2 < team.size(); ++i2)
        {
          if (team[i2][MIDTEAM] == itemid)
          {
            s32 lv = (team[i2][HAVENUM] - team[i2][DBLNUM]) - 1;
            --team[i2][DBLNUM];
            if (lv == 0) newrecord->SetSubName("ＮＥＷ！", FC2_num_red, TRUE);
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

      //福引きの時はアイコンをふくびき玉に
      if (pmgEO_->mgCoU_.mdlog_ItemGet_.GetLineDt(i, mdlog_itemget_getType) == GT_HUKU)
      {
        s32 grade = pmgEO_->mgCoU_.mdlog_ItemGet_.GetLineDt(i, mdlog_itemget_getInfo3);
        newrecord->ugname_.ugicon_.SetIconCupHuku(grade);
      }

      GenMakeObj_Record(newrecord, rh);
      records_.push_back(newrecord);
    }

    SetZeroPos();
  }

  void TUGLiItemGetW::DoUpdateSub(BOOL draw_f)
  {

  }

  void TUGLiItemGetW::DoActionSub()
  {

  }
  s64 TUGLiItemGetW::GetCommonID()
  {
    return records_[GetActIndex()]->GetCommonID();
  }
  s64 TUGLiItemGetW::GetEtcNum()
  {
    return records_[GetActIndex()]->GetEtcNum();
  }
  //BOOL TUGLiItemGetW::IsActRes(enUGLiItemGetAct res)//返値ゲット
  //{
  //  return (GetActRes() == res);
  //}
}
