#pragma once
#include "dbChCommon.h"

namespace db
{
  
  enum enCOMDMPassType//COMDMパス
  {
    cpmUpOrder,//上位オーダー
    cpmNear,//近い
    cpmFar,//遠い
    cpmRear,//後ろ
  };

  //アクション////////////////////////////////
  class TChAction : public TChHaveCommon
  {
  protected:
    void COMPass(BOOL dmpass_f);//ＣＯＭパス
    void COMShootAct();//ＣＯＭのシュート発動
    void COMShoot();//ＣＯＭのシュート
    s32 GetCOMDMPassTag(BOOL nowOKonly_f);
    void COMDMPassAct();//ＣＯＭ思考行動(ダッシュマン)
    void COMAction_DM_Sh();//ＣＯＭ思考行動ダッシュマン
    void COMAction_DM_Pa();//ＣＯＭ思考行動ダッシュマン
    void COMAction_DM();//ＣＯＭ思考行動ダッシュマン
    void COMAction_Std();//ＣＯＭ思考行動通常
    void COMAction();//ＣＯＭ思考行動
    
    //void CheckFreeBallPickUp();//勝手に拾う処理Freemem
    void MCKeyInputAT();//キー入力（アクション）
    void AbsorbXZ(BOOL ZWall_f);//減速
    void SetWallCrash(BOOL ZWall_f);//壁
    void WallAbs(BOOL ZWall_f);//壁ヒット
    void GetLandPos(s32 tGrv);//空中着地点
    void MvDefSl(s32 frc);//スリップ
    void MvDefSl_DSFric();
    void MvWk();//歩き
    void MvDs();//ダッシュ
    void MvJp();//ジャンプ
    void MvCr();//しゃがみ
    void MvFl();//吹っ飛び
    void MvCM();//キャッチモーション
    void MvShPa();//シュートパス
    void MvDgCa();//よけキャッチ
    void MoveZahyou();//座標移動
    void CheckLine(enCrtLine lineType, s32 line);//ライン補正
    void CheckOverLine();//オーバーライン
    void SetLand();//着地位置
    void Moving();//座標移動
    
  public:
    //コンストラクタ
    explicit TChAction(TGame* pGame, TChCommon* pCommon, s32 sideNo, s32 charNo);
    virtual ~TChAction(void);
    
    
    //更新
    void UpDate();
  };
  
}
