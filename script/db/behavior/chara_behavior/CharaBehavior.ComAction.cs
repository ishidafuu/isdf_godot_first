namespace db;

public partial class CharaBehavior
{
    /// <summary>
    /// //ＣＯＭ思考行動（パス、シュート）★★
    /// </summary>
    private void ComAction()
    {
        //アクション中は来ない
        //ボールマンだけ
        if (Motion.HasFlag(CharaMotionFlag.Act)
            || Composite.IsBallHolder == false)
        {
            return;
        }

        if (MyTeam.SemiAuto.SemiShotF
            || MyTeam.Main.ComOverTimeF)
        {
            COMShootAct();
            // MyTeam.SemiAuto.SemiShotF = false;
        }
        else if (MyTeam.AiMain.Isdm)
        {
            COMAction_DM();
        }
        else
        {
            COMAction_Std();
        }
    }


    //ＣＯＭのシュートやパス
    private void COMShoot()
    {
        if (MyTeam.AiAttack.PaF)
        {
            COMPass(false); //シュート切り替えパスのみここにくる
        }
        else
        {
            COMShootAct(); //シュート発動
        }
    }


    private void COMShootAct()
    {
        //ノーガードは狙わない場合
        bool paok_f = MyTeam.Main.ComOverTimeF == false;

        bool ngshng_f = false;

        bool shtgnone_f = Ball.Main.ShotTargetOrder == OrderIndexType.Disabled;

        if (shtgnone_f == false)
        {
            ngshng_f = MyTeam.AiPattern.GetActionPattern(MyOrderIndex, AiActionType.cmaChanceSh) == ActionGroupType.Type3PosiPassive
                       && CharaBehaviorManager.Instance.GetOrderChara(EnemySideIndex, Ball.Main.ShotTargetOrder).Composite.IsNoGuard(true);
        }

        //セミオートでオーダーでシュート命令が出てないとき
        bool semiPass_f = MyTeam.SemiAuto.SemiF
                          && MyTeam.SemiAuto.SemiOrderF
                          && MyTeam.SemiAuto.SemiShotF == false;

        bool pass = semiPass_f || (paok_f && (ngshng_f || shtgnone_f));

        if (pass)
        {
            // MyComState.AttackState.PaTag = MyOrder.IsInfield
            //     ? GetNaiyaPassTag()
            //     : GetGaiyaPassTag();

            COMPass(false); //シュート切り替えパス
        }
        else
        {
            //タゲ方向向く
            //居ないときはオートで探す
            LookTg(Ball.Main.ShotTargetOrder, false, true);

            switch (Motion.MotionType)
            {
                case CharaMotionType.St or CharaMotionType.Wk or CharaMotionType.Ds:
                    SetMotionType(CharaMotionType.Sh);
                    break;
                case CharaMotionType.JUp or CharaMotionType.JDn:
                    SetMotionType(CharaMotionType.JSh);
                    break;
            }
        }
    }


    //ＣＯＭパス
    private void COMPass(bool dmpass_f)
    {
        var paTag = Order.IsInfield
            ? GetNaiyaPassTag()
            : GetGaiyaPassTag();

        //パスタゲがパス出せないとき
        SetPassTarget(paTag);

        //パスカットキャラセット
        PaCtTagSet(paTag);
        //タゲの方を向く
        LookTg(Ball.Main.ShotTargetOrder, true, false);

        switch (Motion.MotionType)
        {
            case CharaMotionType.St or CharaMotionType.Wk or CharaMotionType.Ds:
                SetMotionType(CharaMotionType.Pa);
                break;
            case CharaMotionType.JUp or CharaMotionType.JDn:
                SetMotionType(CharaMotionType.JPa);
                break;
        }
    }


    private void COMAction_DM()
    {
        //ダッシュマン作戦に完全に入ってる必要がある（態勢ウエイトはなにもしない）
        if (MyTeam.AiMain.DashmanF == false)
        {
            return;
        }

        //シュート指示が出てる
        if (MyTeam.AiMain.DashmanShOkF == false)
        {
            //飛ばないダッシュマン
            if (MyTeam.IsNoJpDashman())
            {
                //歩数過ぎたら投げてイイ
                if (Move.StepCountValue >= MyTeamAiAction.DShStep
                    || Composite.LeftCourtX > MyTeam.GetAtcLineX(false, true)) //ジャンプしないダッシュマンはこのラインが最大限界
                {
                    COMAction_DM_Sh();
                }
            }
            else
            {
                COMAction_DM_Sh();
            }
        }
        else //パス回し
        {
            COMAction_DM_Pa(); //パス回し
        }
    }


    private void COMAction_DM_Sh() //シュート指示が出てる
    {
        //空中攻撃
        if (MyTeam.AiAttack.JpF
            || Motion.HasFlag(CharaMotionFlag.Ar))
        {
            if (Motion.HasFlag(CharaMotionFlag.Ar) //ジャンプ状態
                && Air.AirCountValue >= MyTeamAiAction.JShTime) //予定時間こえてる
            {
                //シュート
                COMShootAct();
            }
        }
        else //地上攻撃
        {
            if (Order.IsInfield == false //外野
                || Move.StepCountValue >= MyTeamAiAction.DShStep //予定歩数超えた
                || Composite.LeftCourtX > MyTeam.GetAtcLineX(false, true)) //センターライン超えそう
            {
                //シュート
                COMShootAct();
            }
        }
    }


    private void COMAction_DM_Pa() //パス回し
    {
        //既にメンバー走り出しに入ってる
        if (MyTeam.AiMain.CallOkF)
        {
            //ダッシュマンパス（タイミングなどは関数内）
            COMDMPassAct();
        }
        else if (MyTeam.AiMain.SetterOkF == false) //セッターにボールが渡ってない
        {
            //空中攻撃
            if (MyTeam.AiAttack.JpF
                || Motion.HasFlag(CharaMotionFlag.Ar))
            {
                if (Motion.HasFlag(CharaMotionFlag.Ar) //ジャンプ状態
                    && Air.AirCountValue >= MyTeamAiAction.JShTime) //予定時間こえてる
                {
                    COMPass(false); //セッターパス
                }
            }
            else //地上攻撃
            {
                COMPass(false); //セッターパス
            }
        }
    }


    /// <summary>
    /// ＣＯＭ思考行動通常
    /// </summary>
    private void COMAction_Std()
    {
        //シュート
        if (MyTeam.AiAttack.ShF)
        {
            //空中攻撃
            if (MyTeam.AiAttack.JpF
                || Motion.HasFlag(CharaMotionFlag.Ar))
            {
                int shtiming = Motion.HasFlag(CharaMotionFlag.Ds)
                    ? MyTeamAiAction.JShTime
                    : MyTeamAiAction.JShTime2;

                if (Motion.HasFlag(CharaMotionFlag.Ar) //ジャンプ状態
                    && Air.AirCountValue >= shtiming) //予定時間こえてる
                {
                    //内野ダッシュジャンプ攻撃のときは
                    if (Order.IsInfield && MyTeam.AiAttack.DsF)
                    {
                        if (Court.IsLandEnemyCourt) //相手コートに落ちてしまう場合はやむを得ずシュート
                        {
                            COMShoot();
                        }
                        else if (Motion.HasFlag(CharaMotionFlag.Ds)) //ダッシュフラグがたっていたら
                        {
                            COMShoot();
                        }
                    }
                    else
                    {
                        COMShoot(); //それ以外はシュート
                    }
                }
            }
            else if (MyTeam.IsWaitOver()) //地上攻撃
            {
                //内野ダッシュ攻撃
                if (Order.IsInfield && MyTeam.AiAttack.DsF)
                {
                    if (Court.IsLandEnemyCourt) //相手コートに落ちてしまう場合はやむを得ずシュート
                    {
                        COMShoot();
                    }
                    else if //((MyTeam.AiAttack.quick_f == false)//クイック一時停止
                        (MyTeam.AiAttack.WaitmoveC == 1)
                    {
                        COMShoot();
                    }
                    //ダッシュ開始位置まで行ったフラグ
                    else if (MyTeam.AiAttack.DsposF)
                    {
                        int needstep = MyTeam.AiAttack.OnestepF //一歩ＤＳ
                            ? 1
                            : MyTeamAiAction.DShStep;

                        if (Move.StepCountValue >= needstep //予定歩数超えた
                            || Composite.LeftCourtX > MyTeam.GetAtcLineX(false, false)) //攻撃ライン超えた
                        {
                            if (ComAuto.IsComCounter == false
                                || MyTeam.AiAttack.Getstep < Move.StepCountValue)
                            {
                                COMShoot();
                            }
                        }
                    }
                }
                else
                {
                    COMShoot(); //それ以外はシュート
                }
            }
        }
        else if (MyTeam.AiAttack.PaF) //パス
        {
            //空中攻撃
            if (MyTeam.AiAttack.JpF
                || Motion.HasFlag(CharaMotionFlag.Ar))
            {
                if (Motion.HasFlag(CharaMotionFlag.Ar) //ジャンプ状態
                    && Air.AirCountValue >= MyTeamAiAction.JShTime) //予定時間こえてる
                {
                    COMPass(false); //パス
                }
            }
            else if (MyTeam.IsWaitOver()) //地上攻撃
            {
                COMPass(false); //パス
            }
        }
    }


    //ダッシュマンパス
    private void COMDMPassAct()
    {
        //ジャンプしないダッシュマン
        if (MyTeam.IsNoJpDashman())
        {
            switch (MyTeam.AiPattern.GetPlanPattern(AiPlanType.comDMPaNum))
            {
                case ActionGroupType.Type0NegaActive: //一人一回パスの場合
                    //全員呼んだ＆触ってない人がいない
                    if (MyTeam.AiMain.DmcallNum == MyTeam.AiMain.DmcalledNum
                        && MyTeam.Position.DashmanNum_Run_NT == 0)
                    {
                        //歩数きたら投げる
                        // MyTeam.AiMain.DashmanShOkF = true;
                    }
                    break;
                //case 1://無限パス
                //  break;
                case ActionGroupType.Type2NegaPassive: //パスしない場合
                    //歩数きたら投げる
                    // MyTeam.AiMain.DashmanShOkF = true;
                    break;
            }
        }

        //パスしない場合
        if (MyTeam.AiPattern.GetPlanPattern(AiPlanType.comDMPaNum) == ActionGroupType.Type2NegaPassive)
        {
            return;
        }

        //シュートタイミング
        bool shtiming_f = false;

        //セッター以外の内野ダッシュマンもしくはボールマンダッシュセッター
        bool setter_f = MyTeam.AiMain.SetterNo == MyOrderIndex;

        if (Order.IsInfield
            && Motion.HasFlag(CharaMotionFlag.Ds) //ダッシュしてる
            && (setter_f == false || MyTeam.AiMain.SetterBmRsvGoF))
        {
            //現状パスのjpが入っているので
            var pwsh = MyTeam.AiPattern.GetActionPattern(MyOrderIndex, AiActionType.cmaPwShType);

            switch (pwsh)
            {
                case ActionGroupType.Type0NegaActive: //ＤＪＳ
                    if (Composite.LeftCourtX > MyTeam.GetAtcLineX(true, true) //攻撃ライン超えた
                        && Motion.HasFlag(CharaMotionFlag.Ar) //ジャンプ状態
                        && Air.AirCountValue >= MyTeam.AiAction(MyOrderIndex).JShTime) //予定時間こえてる
                    {
                        shtiming_f = true;
                    }
                    break;
                case ActionGroupType.Type1PosiActive: //ＤＳ
                    if (Motion.HasFlag(CharaMotionFlag.Ar) == false
                        && (Move.StepCountValue >= MyTeam.AiAction(MyOrderIndex).DShStep //予定歩数超えた
                            || Composite.LeftCourtX > MyTeam.GetAtcLineX(false, true))) //攻撃ライン超えた
                    {
                        shtiming_f = true;
                    }
                    break;
                case ActionGroupType.Type3PosiPassive: //ＪＳ
                    if (Motion.HasFlag(CharaMotionFlag.Ar) //ジャンプ状態
                        && Air.AirCountValue >= MyTeam.AiAction(MyOrderIndex).JShTime) //予定時間こえてる
                    {
                        shtiming_f = true;
                    }
                    break;
                default: //一歩ダッシュＳ
                    if (Motion.HasFlag(CharaMotionFlag.Ar) == false
                        && (Move.StepCountValue >= 1 //予定歩数超えた
                            || Composite.LeftCourtX > MyTeam.GetAtcLineX(false, true))) //攻撃ライン超えた
                    {
                        shtiming_f = true;
                    }
                    break;
            }
        }
        else
        {
            shtiming_f = true; //外野およびセッターなら無条件でシュートタイミングでいいか
        }

        //パスタゲ(完全ＯＫのみ)
        int oktag = GetCOMDMPassTag(true);

        bool act_f = false;

        //行動するかどうか//最低数フレ待つ（全員が走り出す前に投げてしまう）
        if (oktag != NGNUM && MyTeam.AiAttack.dmpawait_c > 1)
        {
            const int WAITSTEP = 2;

            bool setterBMPaOK_f = MyTeam.AiMain.setterBMRsvGo_f
                                  && Motion.HasFlag(CharaMotionFlag.Ds)
                                  && Move.StepCountValue > WAITSTEP;

            //セッターでなくて、ダッシュしていないキャラ
            bool scndSetter_f = MyTeam.AiMain.setterNo != MyOrderIndex
                                && Motion.HasFlag(CharaMotionFlag.Ds) == false;

            //セッターダッシュしない、もしくは、セッターダッシュ開始後
            bool setterOK_f = Order.IsInfield == false
                              || MyTeam.AiMain.setterBMRsv_f == false || setterBMPaOK_f;

            //セカンドセッター
            if (scndSetter_f)
            {
                act_f = true; //無条件ＯＫ
            }
            else if (setter_f == false || setterOK_f)
            {
                //外野がパス出せずにジャンプしてしまうのをこれで避けられるか？
                if (Order.IsInfield)
                {
                    //内野セッター（もしくはセッターでないのにボールを渡された人）
                    bool infsetter_f = MyTeam.AiMain.setterNo == MyOrderIndex
                                       || Motion.HasFlag(CharaMotionFlag.Ds) == false;

                    switch (MyTeam.AiMain.comPtn[comDMPaTime])
                    {
                        case 0: //パス先が前衛ライン超えたら
                            if (st_.pmgMyTm_->st_.pmgMyCh_[oktag]->Composite.LeftCourtX > DBCRT_CLXL)
                            {
                                act_f = true;
                            }
                            break;
                        case 1: //パスインターバル後
                            if (st_.pmgMyTm_->IsDMPaItvTime(infsetter_f))
                            {
                                act_f = true;
                            }
                            break;
                        case 2: //自分のシュートタイミング★
                            act_f = shtiming_f;
                            break;
                        default: //パス先がダッシュ開始したら
                            act_f = true; //タゲOKになってる時点で走ってるはず
                            break;
                    }
                }
                else
                {
                    act_f = true; //外野は無条件ＯＫ
                }
            }

            //行動する
            if (act_f)
            {
                //タゲセット
                MyTeam.AiAttack.paTag = oktag;
                //パスタイプ

                int patype = Order.IsInfield
                    ? MyTeam.AiAction(MyOrderIndex).comactPtn[cmaPaTypeInf]
                    : MyTeam.AiAction(MyOrderIndex).comactPtn[cmaPaType];

                st_.pmgMyTm_->SetPassType(patype);

                COMPass(true); //パス

                //１回だけパスの場合
                if (MyTeam.AiMain.comPtn[comDMPaNum] == 3)
                {
                    //シュートOK
                    MyTeam.AiMain.dashmanShOK_f = true;
                }
            }
        }

        //パスを出さなかった
        if (act_f == false)
        {
            //これ以上は待てないラインを超えた
            if (Motion.HasFlag(CharaMotionFlag.Ar)) //ジャンプ状態
            {
                //const int LIMTIME = pmgEO_->mgDt_.dtFomation_.GetComDt(cmvJShTime, 0);
                //位置も最低限指定しないとポストマンジャンプの次点で判定されてしまう//パスカットもあるのでダッシュフラグに
                //if ((Air.AirCountValue >= LIMTIME)
                //  && Composite.LeftCourtX > MyTeam.GetAtcLineX(true, true)
                //  && Motion.HasFlag(CharaMotionFlag.Ds))
                if (Composite.LeftCourtX > MyTeam.GetAtcLineX(true, true) //攻撃ライン超えた
                    && Air.AirCountValue >= MyTeam.AiAction(MyOrderIndex).JShTime) //予定時間こえてる
                {
                    //シュートOK
                    MyTeam.AiMain.dashmanShOK_f = true;
                }
            }
            else
            {
                //ジャンプするダッシュマン用
                const int LIMLINE = MyTeam.GetAtcLineX(false, true);

                if (Composite.LeftCourtX > LIMLINE)
                {
                    //シュートOK
                    MyTeam.AiMain.dashmanShOK_f = true;
                }
            }
        }
    }
}
