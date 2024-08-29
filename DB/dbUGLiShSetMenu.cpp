#include "dbUGLiShSetMenu.h"
#include "dbUILayer.h"
#include "dbUGRcNormal.h"


namespace db
{
  //コンストラクタ
  TUGLiShSetMenu::TUGLiShSetMenu(TUIGenLayer* player) :TUGLiGen(player)
  {
  }

  TUGLiShSetMenu::~TUGLiShSetMenu()
  {
  }

  //リストクリア（変数部分的に初期化、レコードとレコードエンド解放）
  void TUGLiShSetMenu::DoClearRecord()
  {
    for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);
    records_.clear();
  }

  TUGRcNormal* TUGLiShSetMenu::MakeObj_Record()
  {
    TUGRcNormal* newrecord = new TUGRcNormal(player_);
    s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
    records_.push_back(newrecord);
    GenMakeObj_Record(newrecord, rh);
    return newrecord;
  }
  void TUGLiShSetMenu::Ready(BOOL omakase_f)
  {

    ClearRecord();//レコードクリアstLiSoubi_もクリアされてる


    {
      TUGRcNormal* newrecord = MakeObj_Record();
      std::string shsetname = "しゅーとセット＿きりかえ";
      newrecord->ugname_.SetNameIcon(shsetname.c_str(), ugi_Ball);
      newrecord->SetCommonID(0);
    }

    {
      TUGRcNormal* newrecord = MakeObj_Record();
      std::string shsetname = "カスタムそうび＿こべつ";
      newrecord->ugname_.SetNameIcon(shsetname.c_str(), ugi_ShSetCstm);
      newrecord->SetCommonID(1);
    }

    {
      TUGRcNormal* newrecord = MakeObj_Record();
      std::string shsetname = "カスタムそうび＿ぜんたい";
      newrecord->ugname_.SetNameIcon(shsetname.c_str(), ugi_ShSetCstm);
      newrecord->SetCommonID(2);
    }

        {
          TUGRcNormal* newrecord = MakeObj_Record();
          std::string shsetname = "まるひレシピ";
          newrecord->ugname_.SetNameIcon(shsetname.c_str(), ugi_FilFullMixed);
          newrecord->SetCommonID(3);
        }

    ////シュートセット
    //for (s32 i = 0; i < (pmgEO_->mgCoU_.mdShSet_.GetLineNum() + 1); ++i)
    //{
    //  if ((i == 0) && (omakase_f == FALSE)) continue;
    //  TUGRcNormal* newrecord = MakeObj_Record();

    //  //std::string shsetname = (i == 0)
    //  //  ? "おまかせタイプ"
    //  //  : "しゅーとセット" + lib_str::IntToStr(i);
    //  std::string shsetname = "";

    //  if (i == 0)
    //  {
    //    shsetname = "おまかせセット";
    //    newrecord->ugname_.SetNameIcon(shsetname.c_str(), ugi_ShSetAuto);
    //  }
    //  else if (i == pmgEO_->mgCoU_.mdShSet_.GetLineNum())
    //  {
    //    shsetname = "カスタムそうび";
    //    newrecord->ugname_.SetNameIcon(shsetname.c_str(), ugi_ShSetCstm);
    //  }
    //  else
    //  {
    //    newrecord->ugname_.SetNameIcon(pmgEO_->mgCoU_.GetShSetName(i, TRUE).c_str(), (enUGPIcon)(ugi_ShSet1 + i - 1));
    //  }


    //  s32 id_shset = (i == pmgEO_->mgCoU_.mdShSet_.GetLineNum())
    //    ? 0
    //    : pmgEO_->mgCoU_.mdShSet_.GetLineDt(i, mdshset_pk);

    //  newrecord->SetCommonID(id_shset);
    //}
  }
  s64 TUGLiShSetMenu::GetCommonID()
  {
    return records_[GetActIndex()]->GetCommonID();
  }
}
