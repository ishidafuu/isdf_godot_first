#pragma once
#include "dbStSnGen.h"


namespace db
{

  //システムステート
  class TSystemState
  {
  public:
    s32 resetOK_c;//リセット可能
    BOOL saveOK_f;//セーブ許可
    BOOL loaded_f;//読み込み済み
    BOOL NANDErr_f;//セーブデータ破損
    //BOOL saveFile_f;//ファイルがある
    
    BOOL saveNeed_f;//更新があり、セーブが必要
    BOOL Init_f;
    
    BOOL savemake_f;
    BOOL savemakeOK_f;
    BOOL savemakeNG_f;
    
    BOOL bannermake_f;
    BOOL bannermakeOK_f;
    BOOL bannermakeNG_f;
    
    void Init()
    {
      Zeromem(this);
    }
  };
 
  
}
