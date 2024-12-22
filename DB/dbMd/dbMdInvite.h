#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdInvite
  {
    mdinvite_pk,//招待ＩＤ
    mdinvite_id_kantoku,//招待監督ＩＤ
    mdinvite_id_kantoku_to,//被招待監督ＩＤ
    mdinvite_invited_f,//招待成功フラグ
    mdinvite_comp_f,//招待完了フラグ
    mdinvite_create_date,//作成日時
    mdinvite_updated_date,//更新日時
    mdinvite_del_f,//削除フラグ
  };
  
  const std::string INVITECLM[] = 
  {
    "pk",//招待ＩＤ
    "id_kantoku",//招待監督ＩＤ
    "id_kantoku_to",//被招待監督ＩＤ
    "invited_f",//招待成功フラグ
    "comp_f",//招待完了フラグ
    "create_date",//作成日時
    "updated_date",//更新日時
    "del_f",//削除フラグ
  };
  
  class TMdInvite: public TModelGen
  {
  protected:

  public:
    explicit TMdInvite();
    virtual ~TMdInvite();
    s64 GetLineDt(s64 line, enmdInvite dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdInvite dtNo);
    s64 GetPKDt(s64 pk, enmdInvite dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdInvite dtNo);
  };
}
