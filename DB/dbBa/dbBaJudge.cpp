#include "dbBaJudge.h"
#include "dbGame.h"

namespace db
{
    //ヒット時のデフォルトSE
    const s32 DEF_NORM_NUM = seHitLv1;//上半身
    const s32 DEF_FRNT_NUM = 57;//下半身前
    const s32 DEF_BACK_NUM = 58;//下半身後ろ
    const s32 SOUND_CHIU = 62;//ちうちうサウンド
    //コンストラクタ
    TBaJudge::TBaJudge(TGame* pGame, TBaCommon* pCommon)
        : TBaHaveCommon(pGame, pCommon)
    {

    }

    TBaJudge::~TBaJudge()
    {
    }

    BOOL TBaJudge::IsHitCheck(s32 tNo, s32 pNo, BOOL myteamPass_f)
    {
        TMgChar* tag2 = st_.pmgTm_[tNo]->st_.pmgMyCh_[pNo];

        TRect atariR = st_.pstBa_->Atari;
        s32 atariD = st_.pstBa_->AtariD;

        if (myteamPass_f)
        {
            const s32 MYTEAMPASSMAG = 2;
            const f32 MYTEAMPASSMAGD = 1;
            atariR.x_ -= ((atariR.width_ * (MYTEAMPASSMAG - 1)) / 2);
            atariR.y_ -= ((atariR.height_ * (MYTEAMPASSMAG - 1)) / 2);
            atariR.width_ *= MYTEAMPASSMAG;
            atariR.height_ *= MYTEAMPASSMAG;
            atariD = (s32)(atariD * MYTEAMPASSMAGD);
        }

        //くっつき中処理･･･くっつき中 ＆ 判定ヒット中 ならそのフラグを保持
        //※くっつき中は無敵なためHitReactionを通らない故ココでフラグ管理して、dbChActionで処理をする
        //    if (tag2->st_.pstMyCh_->Sticking_f == TRUE)
        //    {
        //一旦フラグオフにして
        //        tag2->st_.pstMyCh_->Sticking_f = FALSE;

        //生きている＆判定ヒット中なら フラグオン　但し、ちうちう以外なら無敵処理　
        //        if ((tag2->st_.pstMyCh_->Live_f)//死んでない
        //            && (atariR.IsPile(tag2->st_.pstMyCh_->Kurai))//当たり
        //            && (abs(st_.pstBa_->AtariZ - tag2->st_.pstMyCh_->Zahyou.Z) < (atariD * XYMAG))
        //            && (tag2->st_.pstMyCh_->Motion.IsMFlags(dbmfDn) == FALSE)
        //            )
        //        {
        //            tag2->st_.pstMyCh_->Sticking_f = TRUE;
        //        }
        //    }


        //前回のデータを過去データに入れる
        tag2->st_.pstMyCh_->stk1StPst_f = tag2->st_.pstMyCh_->stk1StNow_f;
        //現在のデータをいったんFALSEにする　ヒット後、Pst_fと比較
        tag2->st_.pstMyCh_->stk1StNow_f = FALSE;

        //正規のヒット判定
        if ((tag2->st_.pstMyCh_->Live_f)//死んでない
            && (tag2->st_.pstMyCh_->Motion.IsMFlags(dbmfMTK) == FALSE)//無敵でない
            && (atariR.IsPile(tag2->st_.pstMyCh_->Kurai))//当たり
            && (abs(st_.pstBa_->AtariZ - tag2->st_.pstMyCh_->Zahyou.Z) < (atariD * XYMAG)))
        {
            st_.pstBa_->HitTNo = tNo;
            st_.pstBa_->HitPNo = pNo;
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }

    BOOL TBaJudge::IsHitCheck2(s32 tNo, s32 pNo, BOOL myteamPass_f)
    {
        TMgChar* tag2 = st_.pmgTm_[tNo]->st_.pmgMyCh_[pNo];

        TRect atariR = st_.pstBa_->Atari2;
        s32 atariD = st_.pstBa_->AtariD2;

        if (myteamPass_f)
        {
            const s32 MYTEAMPASSMAG = 2;
            const f32 MYTEAMPASSMAGD = 1;
            atariR.x_ -= ((atariR.width_ * (MYTEAMPASSMAG - 1)) / 2);
            atariR.y_ -= ((atariR.height_ * (MYTEAMPASSMAG - 1)) / 2);
            atariR.width_ *= MYTEAMPASSMAG;
            atariR.height_ *= MYTEAMPASSMAG;
            atariD = (s32)(atariD * MYTEAMPASSMAGD);
        }

        //くっつき中処理･･･くっつき中 ＆ 判定ヒット中 ならそのフラグを保持
        //※くっつき中は無敵なためHitReactionを通らない故ココでフラグ管理して、dbChActionで処理をする
        //    if (tag2->st_.pstMyCh_->Sticking_f == TRUE)
        //    {
        //一旦フラグオフにして
        //        tag2->st_.pstMyCh_->Sticking_f = FALSE;

        //生きている＆判定ヒット中なら フラグオン　但し、ちうちう以外なら無敵処理　
        //        if ((tag2->st_.pstMyCh_->Live_f)//死んでない
        //            && (atariR.IsPile(tag2->st_.pstMyCh_->Kurai))//当たり
        //            && (abs(st_.pstBa_->AtariZ - tag2->st_.pstMyCh_->Zahyou.Z) < (atariD * XYMAG))
        //            && (tag2->st_.pstMyCh_->Motion.IsMFlags(dbmfDn) == FALSE)
        //            )
        //        {
        //            tag2->st_.pstMyCh_->Sticking_f = TRUE;
        //        }
        //    }

        //くっつき中フラグ
        //前回の情報
        //    tag2->st_.pstMyCh_->StickingPr_f = tag2->st_.pstMyCh_->Sticking_f;
        //今回の情報として一旦FALSE　ヒット後、tag2->st_.pstMyCh_->StickingPr_fと比較
        //    tag2->st_.pstMyCh_->Sticking_f = FALSE;

        //前回のデータを過去データに入れる
        tag2->st_.pstMyCh_->stk2StPst_f = tag2->st_.pstMyCh_->stk2StNow_f;
        //現在のデータをいったんFALSEにする　ヒット後、Pst_fと比較
        tag2->st_.pstMyCh_->stk2StNow_f = FALSE;

        BOOL mtkJudge = tag2->st_.pstMyCh_->Motion.IsMFlags(dbmfMTK);
        BOOL pileJudge = atariR.IsPile(tag2->st_.pstMyCh_->Kurai);//当たり
        s32 chHit = abs(st_.pstBa_->AtariZ2 - tag2->st_.pstMyCh_->Zahyou.Z);
        s32 baHit = (atariD * XYMAG);


        if ((tag2->st_.pstMyCh_->Live_f)//死んでない
            //      && (tag2->st_.pstMyCh_->Motion.IsMFlags(dbmfMTK) == FALSE)//無敵でない
            //      && (atariR.IsPile(tag2->st_.pstMyCh_->Kurai))//当たり
            //      && (abs(st_.pstBa_->AtariZ2 - tag2->st_.pstMyCh_->Zahyou.Z) < (atariD * XYMAG))
            && (mtkJudge == FALSE)//無敵でない
            && (pileJudge == TRUE)//当たり
            && (chHit < baHit)
            )
        {
            //      //前回の情報を入れる･･･前回もTRUEの場合、ヒットしなかったことにする
            //      tag2->st_.pstMyCh_->Sticking_f = tag2->st_.pstMyCh_->StickingPr_f;
            st_.pstBa_->HitTNo = tNo;
            st_.pstBa_->HitPNo = pNo;
            ////中点に座標を戻す//ヒットストップ★のためここでやっては駄目
            //st_.pstBa_->Zahyou.X = st_.pstBa_->Atari2X;
            //st_.pstBa_->Zahyou.Y = st_.pstBa_->Atari2Y;
            //st_.pstBa_->Zahyou.Z = st_.pstBa_->Atari2Z;
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }

    BOOL TBaJudge::IsHitCheckFr(s32 tNo, s32 pNo)
    {
        TMgChar* tag2 = st_.pmgTm_[tNo]->st_.pmgMyCh_[pNo];

        TRect atariR = st_.pstBa_->AtariFr;
        s32 atariD = st_.pstBa_->AtariDFr;

        //    //くっつき中処理･･･くっつき中 ＆ 判定ヒット中 ならそのフラグを保持
        //    //※くっつき中は無敵なためHitReactionを通らない故ココでフラグ管理して、dbChActionで処理をする
        //    if (tag2->st_.pstMyCh_->Sticking_f == TRUE)
        //    {
        //        //一旦フラグオフにして
        //        tag2->st_.pstMyCh_->Sticking_f = FALSE;
        //
        //        //生きている＆判定ヒット中なら フラグオン　但し、ちうちう以外なら無敵処理　
        //        if ((tag2->st_.pstMyCh_->Live_f)//死んでない
        //            && (atariR.IsPile(tag2->st_.pstMyCh_->Kurai))//当たり
        //            && (abs(st_.pstBa_->AtariZ - tag2->st_.pstMyCh_->Zahyou.Z) < (atariD * XYMAG)))
        //        {
        //            tag2->st_.pstMyCh_->Sticking_f = TRUE;
        //            if (tag2->st_.pstMyCh_->comboMax <= 0) tag2->st_.pstMyCh_->Motion.AddMFlags(dbmfMTK);
        //        }
        //    }

        //くっつき中＆ちうちうでない場合はヒット処理をしない
        if ((tag2->st_.pstMyCh_->comboMax < 1) && (tag2->st_.pstMyCh_->Sticking_f == TRUE))
        {
            return FALSE;
        }

        if ((tag2->st_.pstMyCh_->Live_f)//死んでない
            && (tag2->st_.pstMyCh_->Motion.IsMFlags(dbmfMTK) == FALSE)//無敵でない
            && (atariR.IsPile(tag2->st_.pstMyCh_->Kurai))//当たり
            && (abs(st_.pstBa_->AtariZFr - tag2->st_.pstMyCh_->Zahyou.Z) < (atariD * XYMAG)))
        {
            st_.pstBa_->HitTNo = tNo;
            st_.pstBa_->HitPNo = pNo;
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }

    //ヒットチェック
    void TBaJudge::OSShoot()
    {
        if ((st_.pstBa_->Motion != bmShoot)
            && (st_.pstBa_->Motion != bmPass)) return;

        ////ヒットストップ★敵も味方も
        //if (st_.pstBa_->HitStop_c > 0)
        //{
        //  for (s32 i = 0; i < DBSIDE; ++i)
        //  {
        //    for (s32 i2 = 0; i2 < DBMEMBER_ALL; ++i2)
        //    {
        //      if (st_.pmgTm_[i]->st_.pmgMyCh_[i2]->st_.pstMyCh_->HitStop_f)
        //      {
        //        st_.pstBa_->HitTNo = i;
        //        st_.pstBa_->HitPNo = i2;
        //        return;
        //      }
        //    }
        //  }
        //}

        //敵からサーチ
        if (st_.pstBa_->NoHit_c == 0)
        {
            //中点からチェック
            for (s32 i = 0; i < DBMEMBER_ALL; ++i)
            {
                if (IsHitCheck2(st_.pstBa_->EnemTNo, i, FALSE)) return;
            }

            //中点で当たっていなかったら現在位置
            for (s32 i = 0; i < DBMEMBER_ALL; ++i)
            {
                if (IsHitCheck(st_.pstBa_->EnemTNo, i, FALSE)) return;
            }
        }

        //味方をサーチ
        for (s32 i = 0; i < DBMEMBER_ALL; ++i)
        {
            TMgChar* tag = st_.pmgTm_[st_.pstBa_->PichTNo]->st_.pmgMyCh_[i];


            //外野からのパスキャッチバグ
            //操作権与えられない＆メンバー操作でもない
            if ((tag->IsInfield())
                && (tag->IsCtrlOK() == FALSE)
                && (tag->IsMAN() == FALSE)
                && (tag->PaCvrMan_f() == FALSE))
            {
                continue;
            }

            //パスの時はタゲもしくは（タゲが操作権があるときの）カバーマン以外に当たらない
            if (st_.pstBa_->Motion == bmPass)
            {
                //タゲが自分ではない
                if (st_.pstBa_->PaTgPNo != i)
                {
                    //投げてすぐは当たらない
                    if (st_.pstBa_->NoHit_c != 0)// && (tag->IsDashman() == FALSE))
                    {
                        continue;
                    }

                    if (tag->IsMAN())//メンバー操作の場合
                    {
                        //自分が投げたボール
                        if (st_.pstBa_->PichPNo == i)
                        {
                            //クイックパスＯＲ自分に当たらないカウンタが残ってる
                            if ((st_.pstBa_->PaJPa_f == FALSE) || (st_.pstBa_->PaSelfHit_c != 0))
                            {
                                //自分には当たらない
                                continue;
                            }
                        }
                    }
                    else
                    {
                        //自分には当たらない
                        if (st_.pstBa_->PichPNo == i) continue;

                        //カバーマンならとれる//到着予定時間を過ぎていればとれる
                        if ((tag->PaCvrMan_f() == FALSE) && (st_.pstBa_->PaETA_c > 0)) continue;
                    }
                }
                else//タゲが自分
                {

                    //マニュアルキャラ以外ではトスパスの場合は投げた瞬間は取れないように
                    if ((tag->IsMAN() == FALSE)
                        && st_.pstBa_->PaJPa_f
                        && (tag->st_.pstMyCh_->Motion.IsMFlags(dbmfAr) == FALSE)
                        //&& (tag->IsDashman() == FALSE) //★ミラー
                        && (st_.pstBa_->NoHit_c != 0)) continue;
                }

                //ダウン中に当たらないはココに
                if ((tag->st_.pstMyCh_->Motion.Mtype == dbmtDnB)
                    || (tag->st_.pstMyCh_->Motion.Mtype == dbmtDnF)) continue;

            }



            if (IsHitCheck(st_.pstBa_->PichTNo, i, (st_.pstBa_->Motion == bmPass)))
            {
                return;
            }
            if (IsHitCheckFr(st_.pstBa_->PichTNo, i))
            {
                return;
            }
        }
    }

    //シュート角度
    s32 TBaJudge::SetShootAgl8(BOOL firstdXZ_f)
    {
        s32 tShAgl = NGNUM;
        s32 dX = st_.pstBa_->Zahyou.dX + st_.pstBa_->CrvX;
        s32 dZ = st_.pstBa_->Zahyou.dZ + st_.pstBa_->CrvZ;

        if ((dX == 0) && (dZ == 0))
        {
            tShAgl = NGNUM;
        }
        else if (dX == 0)
        {
            tShAgl = (dZ < 0) ? 0 : 4;
        }
        else if (dZ == 0)
        {
            tShAgl = (dX < 0) ? 2 : 6;
        }
        else
        {
            BOOL XLong_f = (abs(dZ) < abs(dX));
            BOOL LtoR_f = (dX > 0);
            BOOL UtoD_f = (dZ < 0);

            if (LtoR_f)
            {
                if (UtoD_f)
                {
                    tShAgl = (XLong_f) ? 6 : 7;
                }
                else
                {
                    tShAgl = (XLong_f) ? 5 : 4;
                }
            }
            else
            {
                if (UtoD_f)
                {
                    tShAgl = (XLong_f) ? 1 : 0;
                }
                else
                {
                    tShAgl = (XLong_f) ? 2 : 3;
                }
            }
        }
        return tShAgl;
    }
    //キャッチ向きチェック
    BOOL TBaJudge::IsCheckCaAgl(TMgChar* pHitman, s32 tShAgl)
    {
        BOOL res_f = FALSE;

        if (tShAgl == NGNUM)
        {
            res_f = TRUE;
        }
        else
        {

            //左向き
            if (pHitman->st_.pstMyCh_->Zahyou.Muki == mL)
            {
                switch (pHitman->st_.pstMyCh_->Zahyou.MukiZ)
                {
                case mzB:
                    res_f = (tShAgl == 5)
                        || (tShAgl == 6)
                        || (tShAgl == 7)
                        || (tShAgl == 0)
                        || (tShAgl == 1);
                    break;
                case mzN:
                    res_f = (tShAgl == 3)
                        || (tShAgl == 4)
                        || (tShAgl == 5)
                        || (tShAgl == 6)
                        || (tShAgl == 7)
                        || (tShAgl == 0);
                    break;
                case mzF:
                    res_f = (tShAgl == 2)
                        || (tShAgl == 3)
                        || (tShAgl == 4)
                        || (tShAgl == 5)
                        || (tShAgl == 6);
                    break;
                }
            }
            //右向き
            else if (pHitman->st_.pstMyCh_->Zahyou.Muki == mR)
            {
                switch (pHitman->st_.pstMyCh_->Zahyou.MukiZ)
                {
                case mzB:
                    res_f = (tShAgl == 6)
                        || (tShAgl == 7)
                        || (tShAgl == 0)
                        || (tShAgl == 1)
                        || (tShAgl == 2);
                    break;
                case mzN:
                    res_f = (tShAgl == 7)
                        || (tShAgl == 0)
                        || (tShAgl == 1)
                        || (tShAgl == 2)
                        || (tShAgl == 3)
                        || (tShAgl == 4);
                    break;
                case mzF:
                    res_f = (tShAgl == 1)
                        || (tShAgl == 2)
                        || (tShAgl == 3)
                        || (tShAgl == 4)
                        || (tShAgl == 5);
                    break;
                }
            }

        }

        return res_f;
    }


    //ヒット処理
    enJudgeType TBaJudge::ShootHit(TMgChar* pHitman)
    {
        s32 shAgl = SetShootAgl8(FALSE);
        s32 shAglfst = SetShootAgl8(TRUE);

        s32 shRank = st_.pstSh_->idata.rank;

        //キャッチ判定
        enJudgeType JdType = JdNoHit;

        //味方方向シュート
        BOOL notagmytmsh_f = ((st_.pstBa_->Motion == bmShoot)
            && (st_.pstBa_->PichTNo == pHitman->st_.mysideNo_)
            && (st_.pstBa_->PichPNo < DBMEMBER_INF)
            && (st_.pstBa_->PichPNo != pHitman->st_.posNo_)
            && (st_.pstBa_->ShTgPNo == NGNUM)
            && pHitman->IsInfield());

        //味方は必ずキャッチ
        if ((st_.pstBa_->PichTNo == pHitman->st_.mysideNo_) && (notagmytmsh_f == FALSE))
        {
            if ((st_.pstBa_->Motion == bmPass) || (st_.pstBa_->Motion == bmBound))
            {
                //避けモーションの時はスルー（スルーしたボールのカバーは後々）現状OSShoot;でパスタゲにしか当たらないようになっているので注意
                if ((pHitman->st_.pstMyCh_->Motion.Mtype == dbmtCa)
                    || (pHitman->st_.pstMyCh_->Motion.Mtype == dbmtJCa))
                {
                    JdType = JdCatch;
                }
                else if ((pHitman->st_.pstMyCh_->Motion.Mtype == dbmtDg)
                    || (pHitman->st_.pstMyCh_->Motion.Mtype == dbmtJDg)
                    //|| (pHitman->st_.pstMyCh_->Motion.IsMFlags(dbmfAct))//★ミラー（空中パス後取れない）
                    || (pHitman->st_.pstMyCh_->Motion.IsMFlags(dbmfDam))
                    || (pHitman->st_.pstMyCh_->Motion.IsMFlags(dbmfDn)))//ダウンにも当たらない
                {
                    JdType = JdNoHit;
                }
                else
                {
                    JdType = JdCatch;
                }
            }
            else//シュート
            {
                //とれない
                const s32 NOCATCHTIME = 60;
                if ((st_.pstBa_->PichPNo == pHitman->st_.posNo_)
                    && (st_.pstSh_->cmn.timeTotal_c < NOCATCHTIME))//自分はシュート後１秒以下の球は味方は捕れない
                {
                    JdType = JdNoHit;
                }
                else if ((pHitman->st_.pstMyCh_->Auto.AutoType == dbatReturn)
                    || (pHitman->st_.pstMyCh_->Auto.AutoType == dbatKgReturn))
                {
                    JdType = JdNoHit;
                }
                else if ((abs(st_.pstBa_->Zahyou.dY) > XYMAG)
                    && (abs(st_.pstBa_->Zahyou.dX) < XYMAG)
                    && (pHitman->IsInfield() == FALSE))//ある程度Ｙ速度があってＸ変化量が非常に少ない（もずとか）
                {
                    JdType = JdNoHit;
                }
                else
                {
                    //背中からのシュートはとらない
                    if ((pHitman->st_.posNo_ == (s32)dbpoO4)
                        && (((pHitman->st_.pstMyCh_->Zahyou.Muki == mL) && (st_.pstBa_->Zahyou.dX < 0))
                        || ((pHitman->st_.pstMyCh_->Zahyou.Muki == mR) && (st_.pstBa_->Zahyou.dX > 0))))
                    {
                        JdType = JdNoHit;
                    }
                    else if ((pHitman->st_.posNo_ == (s32)dbpoO2) && (st_.pstBa_->Zahyou.dZ < -XYMAG))
                    {
                        JdType = JdNoHit;
                    }
                    else if ((pHitman->st_.posNo_ == (s32)dbpoO3) && (st_.pstBa_->Zahyou.dZ > +XYMAG))
                    {
                        JdType = JdNoHit;
                    }
                    //自分のシュートに当たらない//内野のシュートを内野はとらない
                    else if ((st_.pstBa_->PichPNo == pHitman->st_.posNo_)
                        || ((st_.pstBa_->PichPNo <= (s32)dbpoI3) && (pHitman->st_.posNo_ <= (s32)dbpoI3)))
                    {
                        JdType = JdNoHit;
                    }
                    //２，３番外野からのシュートを４番外野が取ってしまう
                    else if (((st_.pstBa_->PichPNo == (s32)dbpoO2) || (st_.pstBa_->PichPNo == (s32)dbpoO3))
                        && (st_.pstBa_->ShTgPNo != NGNUM)
                        && (st_.pmgTm_[st_.pstBa_->ShTgTNo]->st_.pmgMyCh_[st_.pstBa_->ShTgPNo]->st_.pstMyCh_->Motion.IsMFlags(dbmfDn))
                        && (pHitman->st_.posNo_ == (s32)dbpoO4))
                    {
                        JdType = JdNoHit;
                    }
                    else
                    {
                        JdType = JdCatch;
                    }
                }

            }
        }
        else if (pHitman->st_.pstMyCh_->HP > 0)//味方以外でＨＰ１以上
        {
            BOOL muki_f = FALSE;
            //全方向ＯＫ
            muki_f = TRUE;

            //後ろからキャッチ
            if (((pHitman->st_.pstMyCh_->Zahyou.Muki == mL) && (st_.pstBa_->BufX[1] < pHitman->st_.pstMyCh_->Zahyou.X))
                || ((pHitman->st_.pstMyCh_->Zahyou.Muki == mR) && (st_.pstBa_->BufX[1] > pHitman->st_.pstMyCh_->Zahyou.X)))
            {
                muki_f = TRUE;
            }

            //フレーム判定


            //パスカット
            if ((st_.pstBa_->Motion == bmPass) && pHitman->IsFreeMotion(FALSE))
            {
                JdType = JdCatch;
                pHitman->chCommon_.CatchSE();
            }
            else if (pHitman->st_.pstMyCh_->Catch_c == NGNUM) //キャッチモーションではない
            {
                if (muki_f == FALSE)
                {
                    JdType = JdHit;
                }
                else if (pHitman->st_.pmgMyTm_->st_.pstMyTm_->semi_f_)//セミオート
                {
                    if (pHitman->st_.pmgMyTm_->st_.pstMyTm_->semi_Catching_c_ == 0)
                    {

                        s32 catchPos = pHitman->st_.pmgMyTm_->st_.pstMyTm_->semi_CaPos_;
                        s32 ncaLen = pHitman->st_.pmgMyTm_->st_.pstMyTm_->semi_NCaLen_;
                        s32 catch_c = pHitman->st_.pmgMyTm_->st_.pstMyTm_->semi_Catch_c_;

                        //セミオート用キャッチ補正
                        const s32 LEN = pmgEO_->mgDt_.dtSet_.GetDtInput(setSemiCaLen);
                        s32 BASE = (LEN - pmgEO_->mgDt_.dtSet_.GetDtInput(setSemiCaBase)) - catchPos;
                        const s32 NICE = (LEN - pmgEO_->mgDt_.dtSet_.GetDtInput(setSemiCaBase)) + pmgEO_->mgDt_.dtSet_.GetDtInput(setSemiCaNice);

                        BOOL ca_f = (catch_c > BASE);
                        BOOL nca_f = (catch_c > NICE) && (catch_c <= (NICE + ncaLen));


                        //if (pHitman->st_.pmgMyTm_->st_.pstMyTm_->semi_Catch_c_ > 48)
                        if (nca_f)
                        {
                            JdType = JdNiceCatch;

                            if (st_.pstBa_->Motion == bmPass)
                            {
                                JdType = JdCatch;
                            }
                        }
                        //else if (pHitman->st_.pmgMyTm_->st_.pstMyTm_->semi_Catch_c_ > 32)
                        else if (ca_f)
                        {
                            pHitman->chCommon_.CatchSE();
                            JdType = JdCatch;
                        }
                        else
                        {
                            JdType = JdHit;
                        }

                        //キャッチ中カウンタ
                        if (pHitman->st_.pmgMyTm_->st_.pstMyTm_->semi_Catch_c_ > 0)
                        {
                            pHitman->st_.pmgMyTm_->st_.pstMyTm_->semi_Catching_c_ = 32;
                        }
                    }
                    else
                    {
                        JdType = JdHit;
                    }
                }
                else
                {
                    ////パスカット
                    //BOOL AutoPaCa_f = ((st_.pstBa_->Motion == bmPass)
                    //    && (pHitman->IsFreeMotion(FALSE))//パスカットのとき
                    //    && ((pHitman->IsCtrl() == FALSE) || pHitman->IsCOM()))
                    //    && (pHitman->IsMAN() == FALSE);

                    BOOL AutoShCa_f = ((pHitman->IsSelfCtrl() == FALSE)
                        && (st_.pstBa_->Motion == bmShoot)
                        && ((pHitman->IsInfield() == FALSE)//外野||assca
                        || pHitman->st_.pstMyCh_->Motion.IsMFlags(dbmfAtCa)));//キャッチできる状態である


                    //if (AutoPaCa_f)//パス
                    //{
                    //  JdType = JdCatch;
                    //  pHitman->chCommon_.CatchSE();
                    //}
                    //else 
                    if (AutoShCa_f)
                    {

#ifdef __K_DEBUG_SHIAI__
                        BOOL catch_f = false;
                        BOOL niceca_f = false;
                        // キャッチさせないモード以外は通常処理
                        if (!kdebug::DebugSystem::GetInstance()->IsEnemyEveryNotCatch())
                        {
                            catch_f = TRUE;
                            //niceca_f = TRUE;
                        }
#else

                        BOOL catch_f = FALSE;
                        BOOL niceca_f = FALSE;
                        if (pmgEO_->stShiai_.IsVSHum() == FALSE)
                        {
                            catch_f = pmgGO_->pmgCommon_->GetRandPer(pHitman->st_.pstMyCh_->comCaPer_);
                            niceca_f = pmgGO_->pmgCommon_->GetRandPer(pHitman->st_.pstMyCh_->comNCaPer_);
                        }

#endif	// #ifdef __K_DEBUG_SHIAI__
                        if (catch_f)//COM確率
                        {
                            pHitman->chCommon_.CatchSE();//キャッチ音
                            //ナイス
                            if (niceca_f)//一番ぎりぎりはCatch_c == 0なので
                            {
                                JdType = JdNiceCatch;//ナイスキャッチ判定
                            }
                            else
                            {
                                JdType = JdCatch;
                            }
                        }
                        else
                        {
                            JdType = JdHit;
                        }
                    }
                    else
                    {
                        JdType = JdHit;
                    }
                }

            }
            else
            {
                BOOL ds_f = (pHitman->st_.pstMyCh_->Motion.IsMFlags(dbmfDs));

                //減少フレーム
                s32 tDecFrm = 0;

                //ダッシュキャッチ
                if (ds_f)
                {
                    tDecFrm = pmgEO_->mgDt_.dtSet_.GetDtCatch(setDsCatchRev);//ダッシュによる減少
                }

                s32 cafrm = GetRankDtTech(rkCatchTime);
                s32 ncafrm = GetRankDtTech(rkNiceCatchTime);
                //s32 tdam = (pmgEO_->mgDt_.dtSet_.GetDtDamage(setHP) * st_.pstSh_->idata.pow) / (pmgEO_->mgDt_.dtSet_.GetDtDamage(setDeadNum) * pHitman->MyStLv(dbst_Guts));
                //最低フレーム
                cafrm = lib_num::Max((cafrm - tDecFrm), 1);

                //向き違い
                if (muki_f == FALSE)
                {
                    JdType = JdHit;
                }
                else if (pHitman->st_.pstMyCh_->Catch_c > cafrm)//有効フレーム外
                {
                    JdType = JdHit;
                }
                else
                {
                    //ナイス
                    BOOL niceca_f = ((pHitman->st_.pstMyCh_->Catch_c < ncafrm)
                        //&& (pHitman->st_.pstMyCh_->HitStop_f == FALSE)
                        );
                    if ((st_.pstBa_->Motion == bmShoot) && niceca_f)//一番ぎりぎりはCatch_c == 0なので
                    {
                        JdType = JdNiceCatch;//ナイスキャッチ判定
                    }
                    else
                    {
                        JdType = JdCatch;

                        //JdType = JdNiceCatch;//ナイスキャッチ判定テスト
                    }
                }
            }

            //カウンターはナイスキャッチ以外ヒット//削りで死ぬときもナイスキャッチ以外ヒット
            if (st_.pstBa_->counter_f || stHit_.kezuridead_f_)
            {
                if ((JdType != JdNoHit) && (JdType != JdNiceCatch))
                {
                    JdType = JdHit;
                }
            }
        }

        return JdType;
    }


    //ボールのヒットおよびファンブルの反射処理
    void TBaJudge::HitReflex(TMgChar* pHitman, BOOL ZShoot_f)
    {
        BOOL damhit_f = pHitman->st_.pstMyCh_->Motion.IsMFlags(dbmfDam);
        const s32 HIBALLY = 22 * XYMAG;
        const s32 LOBALLY = 6 * XYMAG;
        BOOL Hiball_f = (((st_.pstBa_->Zahyou.Y - pHitman->st_.pstMyCh_->Zahyou.Y) > HIBALLY)
            && (st_.pstBa_->Zahyou.dY > 0));
        BOOL Vball_f = (abs(st_.pstBa_->Zahyou.dX * 2) < XYMAG)
            && (abs(st_.pstBa_->Zahyou.dZ * 2) < XYMAG);
        BOOL Loball_f = (st_.pstBa_->Zahyou.Y - pHitman->st_.pstMyCh_->Zahyou.Y) < LOBALLY;

        //角度を威力、速度をきれ反映

        f32 rr = lib_num::DegToRad((f32)GetRankDtTech(rkRefAg));
        s32 sp = GetRankDtTech(rkRefSp);


        const s32 RAD90 = 157;
        const f32 RAD90F = (f32)1.57;
        const f32 RAD45F = (f32)0.79;
        const f32 RAD30F = (f32)0.52;

        f32 randrad;
        BOOL cnt_f = FALSE;
        if (ZShoot_f == FALSE)
        {
            s32 distZ = abs(st_.pstBa_->Zahyou.Z - pHitman->st_.pstMyCh_->Zahyou.Z) / XYMAG;
            const s32 CNTREF = 2;//正面跳ね返り
            const s32 MIDREF = 20;//正面から横方向跳ね返り

            if (damhit_f)
            {
                //        randrad = ((f32)pmgGO_->pmgCommon_->GetRand(RAD90) / 100) + (-RAD45F);
                randrad = (100 / 100) + (-RAD45F);
            }
            else if (distZ < CNTREF)
            {
                //        randrad = ((f32)pmgGO_->pmgCommon_->GetRand(RAD90) / 100) + (RAD90F + RAD45F);
                randrad = (100 / 100) + (RAD90F + RAD45F);
                cnt_f = TRUE;
            }
            else if (distZ < MIDREF)
            {
                //        randrad = ((f32)pmgGO_->pmgCommon_->GetRand(RAD90) / 100) + (RAD30F + RAD30F);
                randrad = (100 / 100) + (RAD30F + RAD30F);
            }
            else
            {
                //        randrad = ((f32)pmgGO_->pmgCommon_->GetRand(RAD90) / 100) + (RAD30F);
                randrad = (100 / 100) + (RAD30F);
            }
        }
        else
        {
            s32 distX = abs(st_.pstBa_->Zahyou.X - pHitman->st_.pstMyCh_->Zahyou.X) / XYMAG;
            const s32 CNTREF = 6;//正面跳ね返り

            if (damhit_f)
            {
                //        randrad = ((f32)pmgGO_->pmgCommon_->GetRand(RAD90) / 100) + (RAD45F);
                randrad = (100 / 100) + (RAD45F);
            }
            else if (distX < CNTREF)
            {
                //        randrad = ((f32)pmgGO_->pmgCommon_->GetRand(RAD90) / 100) + (RAD30F);
                randrad = (100 / 100) + (RAD30F);
                cnt_f = TRUE;
            }
            else
            {
                //          randrad = ((f32)pmgGO_->pmgCommon_->GetRand(RAD90) / 100) - (RAD30F);
                randrad = (100 / 100) - (RAD30F);
            }
        }

        if (cnt_f)
        {
            sp -= 40;
        }

        f32 rxz = (cos(rr) * sp);
        f32 ry = (sin(rr) * sp);

        st_.pstBa_->Zahyou.dX = (s32)(rxz * cos(randrad));
        st_.pstBa_->Zahyou.dZ = (s32)(rxz * sin(randrad));


        if (ZShoot_f == FALSE)
        {
            if (st_.pstBa_->Zahyou.Muki == mL)
            {
                st_.pstBa_->Zahyou.dX = -st_.pstBa_->Zahyou.dX;
            }

            if (st_.pstBa_->Zahyou.Z < pHitman->st_.pstMyCh_->Zahyou.Z)
            {
                st_.pstBa_->Zahyou.dZ = -st_.pstBa_->Zahyou.dZ;
            }
        }
        else
        {
            if (st_.pstBa_->Zahyou.MukiZ == mzB)
            {
                st_.pstBa_->Zahyou.dZ = -st_.pstBa_->Zahyou.dZ;
            }

            if (st_.pstBa_->Zahyou.X < pHitman->st_.pstMyCh_->Zahyou.X)
            {
                st_.pstBa_->Zahyou.dX = -st_.pstBa_->Zahyou.dX;
            }
        }

        //奥行きシュート
        const s32 ZSPPER = 50;
        st_.pstBa_->Zahyou.dZ = lib_num::Percent(st_.pstBa_->Zahyou.dZ, ZSPPER);
        st_.pstBa_->Zahyou.dY = (s32)ry;
        //ボールの処理
        pCommon_->SetMtype(bmBound);
    }

    void TBaJudge::HitReactionAI(TMgChar* pHitman, enJudgeType JdType)
    {
        //敵のシュートボール
        BOOL ensh_f = ((st_.pstBa_->PichTNo != pHitman->st_.mysideNo_) //敵のシュート
            && (st_.pstBa_->Motion == bmShoot));//シュートボール

        BOOL jsh_f = st_.pstSh_->idata.jsh_f;
        BOOL ns_f = st_.pstSh_->idata.nice_f;
        BOOL hs_f = st_.pstSh_->idata.sp_f;

        //AIの対人シュート
        if (ensh_f && (pHitman->IsCOM() == FALSE))
        {
            BOOL shres = (JdType == JdHit);

            s32 nx = 0;
            s32 pr = 0;
            s32 pr2 = 0;
            s32 wt = 0;
            s32 nowrno = 0;
            s32 shtype = aisNml;
            if (ns_f)
            {
                shtype = aisNS;
            }
            else if (hs_f)
            {
                shtype = aisHS;
            }

            if (jsh_f)
            {
                st_.pstBa_->AIJShDt.SetSh(shtype);//今回のシュートパターンを保存
                st_.pstBa_->AIJShDt.SetResult(shres);

                nowrno = st_.pstBa_->AIJShDt.nowRuleNo;
                nx = st_.pstBa_->AIJShDt.Rules[nowrno].nextSh;
                pr = st_.pstBa_->AIJShDt.Rules[nowrno].prvSh;
                pr2 = st_.pstBa_->AIJShDt.Rules[nowrno].prvSh2;
                wt = st_.pstBa_->AIJShDt.Rules[nowrno].weight;
            }
            else
            {
                st_.pstBa_->AIDShDt.SetSh(shtype);//今回のシュートパターンを保存
                st_.pstBa_->AIDShDt.SetResult(shres);

                nowrno = st_.pstBa_->AIDShDt.nowRuleNo;
                nx = st_.pstBa_->AIDShDt.Rules[nowrno].nextSh;
                pr = st_.pstBa_->AIDShDt.Rules[nowrno].prvSh;
                pr2 = st_.pstBa_->AIDShDt.Rules[nowrno].prvSh2;
                wt = st_.pstBa_->AIDShDt.Rules[nowrno].weight;
            }
        }
    }

    void TBaJudge::HitReactionCatch(TMgChar* pHitman, enJudgeType JdType)
    {
        f32 hp = lib_num::Hypot(st_.pstBa_->Zahyou.dX, st_.pstBa_->Zahyou.dZ);
        BOOL ds_f = (pHitman->st_.pstMyCh_->Motion.IsMFlags(dbmfDs));
        BOOL ar_f = (pHitman->st_.pstMyCh_->Motion.IsMFlags(dbmfAr));
        BOOL nice_f = (JdType == JdNiceCatch);
        BOOL comcounter_f = FALSE;
        BOOL tosspaca_f = FALSE;

        //キャッチ向き修正
        enMukiType  CaMuki = pHitman->st_.pstMyCh_->Zahyou.Muki;//キャッチ向き
        enMukiZType CaMukiZ = pHitman->st_.pstMyCh_->Zahyou.MukiZ;//奥行き向き
        const s32 REVZMUKIDZ = (1 * XYMAG);

        if (st_.pstBa_->Zahyou.dX > XYMAG)
        {
            CaMuki = mL;
        }
        else if (st_.pstBa_->Zahyou.dX < -(XYMAG))
        {
            CaMuki = mR;
        }

        if (((pHitman->st_.pstMyCh_->Zahyou.MukiZ == mzB) && (st_.pstBa_->Zahyou.dZ > 0))
            || ((pHitman->st_.pstMyCh_->Zahyou.MukiZ == mzF) && (st_.pstBa_->Zahyou.dZ < 0)))
        {
            CaMukiZ = mzN;
        }

        if (st_.pstBa_->Zahyou.dZ > REVZMUKIDZ)
        {
            CaMukiZ = mzF;
        }
        else if (st_.pstBa_->Zahyou.dZ < -REVZMUKIDZ)
        {
            CaMukiZ = mzB;
        }


        //st_.pstBa_->HitStop_c = 0;
        //pHitman->st_.pstMyCh_->HitStop_f = FALSE;
        pHitman->st_.pstMyCh_->enshCa_f = FALSE;

        //必殺音を消す
        pmgEO_->mgSound_.StopHMNG();

        pHitman->st_.pstMyCh_->FricX = 0;
        pHitman->st_.pstMyCh_->FricZ = 0;

        pHitman->st_.pstMyCh_->Kagami_c = 0;//空中復帰オートキャッチの場合屈みカウンタ入ってるかも知れないので


        //パス
        if ((st_.pstBa_->PichTNo == pHitman->st_.mysideNo_)
            && (st_.pstBa_->Motion == bmPass))
        {
            pHitman->st_.pstMyCh_->AirCatch_f = FALSE;//パスはとびつきキャッチに入らない

            //味方からのパスを空中キャッチ=トスパスキャッチ
            if (pHitman->st_.pstMyCh_->Motion.IsMFlags(dbmfAr)
                //&& (st_.pstBa_->PichPNo < DBMEMBER_INF)//内野からのみを解除
                )
            {
                tosspaca_f = TRUE;
            }
        }


        if (st_.pstBa_->PichTNo != pHitman->st_.mysideNo_)//敵のボール
        {
            //パスカット
            if (st_.pstBa_->Motion == bmPass)
            {
                //★成績//パスカット数合計
                pHitman->chCommon_.AddRec(recPassCut_c);
            }
            else//キャッチ
            {
                if (pHitman->IsInfield())//ファンブルはキャッチにならないように
                {
                    //★成績//キャッチ数合計
                    pHitman->chCommon_.AddRec(recCatchOK_c);
                    //★成績//ナイスキャッチ
                    if (JdType == JdNiceCatch)
                    {
                        pHitman->chCommon_.AddRec(recNiceCatch_c);
                    }
                }

                //敵シュートをキャッチした
                pHitman->st_.pstMyCh_->enshCa_f = TRUE;
            }

        }
        else
        {
            if (st_.pstBa_->Motion == bmPass)
            {
                //★成績//パス成功数合計
                pHitman->chCommon_.AddRec(recPassOK_c);
            }
        }

        //スリップ
        if ((st_.pstBa_->PichTNo == pHitman->st_.mysideNo_)
            && (st_.pstBa_->PichPNo == pHitman->st_.posNo_))
        {
            pCommon_->SESet(seCatch);//キャッチ音
        }
        else
        {
            const s32 FIRCMAG = (3 * XYMAG);


            if (hp > 1)
            {
                pHitman->st_.pstMyCh_->FricX = (s32)(st_.pstBa_->Zahyou.dX * FIRCMAG / hp);
                pHitman->st_.pstMyCh_->FricZ = (s32)(st_.pstBa_->Zahyou.dZ * FIRCMAG / hp);

                //逆滑りならXZの符号をひっくり返す
                if (st_.pstSh_->cmn.efEl.catchSlipR_f == TRUE)
                {
                    pHitman->st_.pstMyCh_->FricX = pHitman->st_.pstMyCh_->FricX * (-1);
                    pHitman->st_.pstMyCh_->FricZ = pHitman->st_.pstMyCh_->FricZ * (-1);
                }
            }

            //      s32 tFric_c = (nice_f || (st_.pstBa_->Motion == bmPass)) //ナイスとパスの時は滑らない
            //        ? 0
            //        : GetRankDtPow(rkSlipSp);

            //スリップ量
            s32 tFric_c;

            //ナイスキャッチ or パスなら滑らない
            if (nice_f || (st_.pstBa_->Motion == bmPass))
            {
                tFric_c = 0;
            }
            else
            {
                //キャッチスリップ

                pHitman->st_.pstMyCh_->rndWorldCh_f = FALSE;  //12:世界一周効果キャッチ状態
                pHitman->st_.pstMyCh_->refHorCh_f = FALSE;  //12:壁はね吹っ飛び効果キャッチ状態
                //特殊吹っ飛び（壁にかかわる吹っ飛び）効果を持つ場合は、その効果のフラグを渡しておく
                if ((st_.pstSh_->cmn.efEl.rndWorld_f == TRUE) && (st_.pstSh_->cmn.efEl.rndWorldLv > 0))
                {
                    pHitman->st_.pstMyCh_->rndWorldCh_f = TRUE;  //12:世界一周効果キャッチ状態
                }

                if ((st_.pstSh_->cmn.efEl.refHor_f == TRUE) && (st_.pstSh_->cmn.efEl.refHorLv > 0))
                {
                    pHitman->st_.pstMyCh_->refHorCh_f = TRUE;  //12:壁はね吹っ飛び効果キャッチ状態
                }

                //キャッチスリップ強化or反転効果
                if ((st_.pstSh_->cmn.efEl.catchSlip_f == TRUE) || (st_.pstSh_->cmn.efEl.catchSlipR_f == TRUE))
                {
                    //デフォの滑り値
                    s32 tFricDef_c = GetRankDtPow(rkSlipSp);
                    //効果による滑り値
                    s32 slipEffLv = st_.pstSh_->cmn.efEl.catchSlipLv + st_.pstSh_->cmn.efEl.catchSlipRLv;
                    s32 tFricEff_c = GetRankDtPow2(rkSlipEff, slipEffLv);

                    tFric_c = tFricDef_c + tFricEff_c;

                }
                else
                {//デフォの滑り値
                    tFric_c = GetRankDtPow(rkSlipSp);
                }
            }

            BOOL ZShoot_f = (abs(st_.pstBa_->Zahyou.dZ) > abs(st_.pstBa_->Zahyou.dX / 3));
            if (ZShoot_f) tFric_c /= 2;//Ｚ方向滑りはとりあえず半分にしてみる

            //ナイスキャッチ
            if (nice_f)
            {
                pmgGO_->pmgCommon_->PopupMsg(pu_Act, "ナイスキャッチ", pHitman->st_.mysideNo_,
                    pHitman->st_.pstMyCh_->Zahyou.X, pHitman->st_.pstMyCh_->Zahyou.Y, pHitman->st_.pstMyCh_->Zahyou.Z, FC2_normal, FALSE);
            }

            //空中
            if (ar_f)
            {
                //踏ん張りが滑りに負けたらＤＪがＪに変わる滑るときは押し戻される
                if (tFric_c > 0)
                {
                    pHitman->st_.pstMyCh_->Zahyou.dX = pHitman->st_.pstMyCh_->Zahyou.dX + (tFric_c * pHitman->st_.pstMyCh_->FricX / XYMAG);
                    pHitman->st_.pstMyCh_->Zahyou.dZ = pHitman->st_.pstMyCh_->Zahyou.dZ + (tFric_c * pHitman->st_.pstMyCh_->FricZ / XYMAG);

                    if (ds_f)//ダッシュフラグを消す
                    {
                        pHitman->st_.pstMyCh_->Motion.SubMFlags(dbmfDs);
                    }

                    //キャッチ方向
                    if (st_.pstBa_->Motion != bmPass)
                    {
                        pHitman->chCommon_.st_.pstMyCh_->Zahyou.Muki = CaMuki;
                        pHitman->chCommon_.st_.pstMyCh_->Zahyou.MukiZ = CaMukiZ;
                    }

                    pHitman->chCommon_.SetMtype(dbmtJCM);//キャッチモーション

                }
                else
                {
                    //キャッチ方向
                    if (st_.pstBa_->Motion != bmPass)
                    {
                        pHitman->chCommon_.st_.pstMyCh_->Zahyou.Muki = CaMuki;
                        pHitman->chCommon_.st_.pstMyCh_->Zahyou.MukiZ = CaMukiZ;
                    }

                }
            }
            else
            {
                //踏ん張りが滑りを超えたらキャッチモーションにならない
                if (tFric_c <= 0)
                {
                    if (ds_f)//必ずダッシュ継続にしてみる
                    {
                        pHitman->chCommon_.SetMtype(dbmtDs);

                        //★ＣＯＭカウンターキャッチ状態
                        if (st_.pstBa_->Motion == bmShoot)
                        {
                            comcounter_f = TRUE;
                        }
                    }
                    else
                    {
                        //キャッチ方向
                        if (st_.pstBa_->Motion != bmPass)
                        {
                            pHitman->chCommon_.st_.pstMyCh_->Zahyou.Muki = CaMuki;
                            pHitman->chCommon_.st_.pstMyCh_->Zahyou.MukiZ = CaMukiZ;
                        }

                        pHitman->chCommon_.SetMtype(dbmtCM);//キャッチモーション
                        pHitman->st_.pstMyCh_->Zahyou.Fric_c = 0;
                    }
                }
                else
                {
                    pHitman->st_.pstMyCh_->Zahyou.dX = 0;
                    pHitman->st_.pstMyCh_->Zahyou.dZ = 0;

                    //キャッチ方向
                    if (st_.pstBa_->Motion != bmPass)
                    {
                        pHitman->chCommon_.st_.pstMyCh_->Zahyou.Muki = CaMuki;
                        pHitman->chCommon_.st_.pstMyCh_->Zahyou.MukiZ = CaMukiZ;
                    }

                    pHitman->chCommon_.SetMtype(dbmtCM);//キャッチモーション

                    if (ar_f == FALSE)
                    {
                        //★すべりコート
                        //銭湯
                        //tFric_c = pmgEO_->mgDt_.dtRank_.GetRankDt(rkopSlpSpIce, st_.pstSh_->idata.rank) - tStop;
                    }
                    pHitman->st_.pstMyCh_->Zahyou.Fric_c = tFric_c;
                }

            }

            const s32 FRICSE1 = 1;
            const s32 FRICSE2 = 40;
            const s32 FRICSE3 = 60;

            //瞬間効果音
            if (nice_f)//ナイスキャッチ
            {
                pCommon_->SESet(seNiceCatch);//キャッチ音
            }
            else
            {
                if (st_.pstSh_->idata.sp_f)
                {
                    pCommon_->SESet(seHitLv0);//けずり音
                }
                else
                {
                    pCommon_->SESet(seCatch);//キャッチ音
                }
            }


            //滑り効果音
            if (ar_f == FALSE)
            {
                if (pHitman->st_.pstMyCh_->Zahyou.Fric_c >= FRICSE3)
                {
                    pCommon_->SESet(seSlipLv3);//強滑り
                }
                else if (pHitman->st_.pstMyCh_->Zahyou.Fric_c >= FRICSE2)
                {
                    pCommon_->SESet(seSlipLv2);//中滑り
                }
                else if (pHitman->st_.pstMyCh_->Zahyou.Fric_c >= FRICSE1)
                {
                    pCommon_->SESet(seSlipLv1);//弱滑り
                }
            }
        }

        pHitman->chCommon_.BallGet(TRUE, FALSE);
        //ナイスカウンター
        st_.pstBa_->counter_f = nice_f;
        if (comcounter_f)pHitman->st_.pstMyCh_->COMCounter_f = TRUE;
        if (tosspaca_f) pHitman->st_.pstMyCh_->COMTossPassGet_f = TRUE;

    }

    BOOL TBaJudge::HitReactionHit(TMgChar* pHitman, enJudgeType JdType)
    {
        BOOL bound_f = FALSE;

        //縦方向シュート
        BOOL ZShoot_f = FALSE;

        if (st_.pstSh_->idata.sp_f)
        {
            //世界一周はしやすく
            ZShoot_f = (st_.pstSh_->idata.rank == 0)
                ? (abs(st_.pstBa_->Zahyou.dZ) > abs(st_.pstBa_->Zahyou.dX))
                : (abs(st_.pstBa_->Zahyou.dZ) > abs(st_.pstBa_->Zahyou.dX / 3));
        }
        else
        {
            ZShoot_f = (abs(st_.pstBa_->Zahyou.dZ) > abs(st_.pstBa_->Zahyou.dX / 3));
        }

        //コンボ中
        BOOL Combo_f = ((pHitman->st_.pstMyCh_->Motion.Mtype == dbmtDnF)
            || (pHitman->st_.pstMyCh_->Motion.Mtype == dbmtDnB)
            || (pHitman->st_.pstMyCh_->Motion.Mtype == dbmtDnHF)
            || (pHitman->st_.pstMyCh_->Motion.Mtype == dbmtDnHB)
            || (pHitman->st_.pstMyCh_->Motion.Mtype == dbmtFlF)
            || (pHitman->st_.pstMyCh_->Motion.Mtype == dbmtFlB)
            || (pHitman->st_.pstMyCh_->AirRev_f)//空中復帰中もコンボ扱い
            );

        f32 hp = lib_num::Hypot(st_.pstBa_->Zahyou.dX, st_.pstBa_->Zahyou.dZ);
        BOOL CheckBackHit_f = (((pHitman->st_.pstMyCh_->Zahyou.Muki == mL) && (st_.pstBa_->Zahyou.dX < 0))
            || ((pHitman->st_.pstMyCh_->Zahyou.Muki == mR) && (st_.pstBa_->Zahyou.dX > 0)));

        const s32 ROLLITV = 8;
        BOOL ds_f = (pHitman->st_.pstMyCh_->Motion.IsMFlags(dbmfDs));
        BOOL ar_f = (pHitman->st_.pstMyCh_->Motion.IsMFlags(dbmfAr));
        BOOL nice_f = (JdType == JdNiceCatch);
        BOOL comcounter_f = FALSE;
        BOOL tosspaca_f = FALSE;

        //st_.pstBa_->HitStop_c = 0;
        //pHitman->st_.pstMyCh_->HitStop_f = FALSE;
        BOOL dam_f = FALSE;

        //人の処理
        BOOL tDnHit_f = FALSE;
        //成績用追い撃ちフラグ
        BOOL tMoreHit_f = FALSE;


        //パス
        if (st_.pstBa_->Motion == bmPass)
        {
            switch (pHitman->st_.pstMyCh_->Motion.Mtype)
            {
            case dbmtDnF:
                pHitman->chCommon_.SetMtype(dbmtDnHF);
                break;
            case dbmtDnB:
                pHitman->chCommon_.SetMtype(dbmtDnHB);
                break;
            default:
                if (ar_f)
                {
                    if (CheckBackHit_f)
                    {
                        pHitman->chCommon_.SetMtype(dbmtFlB);
                    }
                    else
                    {
                        pHitman->chCommon_.SetMtype(dbmtFlF);
                    }
                }
                else
                {
                    if (CheckBackHit_f)
                    {
                        pHitman->chCommon_.SetMtype(dbmtPHB);
                    }
                    else
                    {
                        pHitman->chCommon_.SetMtype(dbmtPHF);
                    }
                }
                break;
            }

            pCommon_->SESet(seHitLv0);
        }
        else
        {
            //ダメージフラグ
            if (st_.pstBa_->PichTNo != pHitman->st_.mysideNo_)//敵のボール
            {
                dam_f = TRUE;
            }

            //成績用追い撃ちフラグ
            tMoreHit_f = (pHitman->st_.pstMyCh_->Motion.IsMFlags(dbmfDam));


            //ダウン以外、もしくは、必殺シュートでふっとぶ
            if ((Combo_f == FALSE) || st_.pstSh_->idata.sp_f)
            {
                if (CheckBackHit_f)
                {
                    pHitman->chCommon_.SetMtype(dbmtFlB);
                }
                else
                {
                    pHitman->chCommon_.SetMtype(dbmtFlF);
                }
            }
            else
            {
                switch (pHitman->st_.pstMyCh_->Motion.Mtype)
                {
                case dbmtDnF:
                case dbmtDnHF:
                    pHitman->chCommon_.SetMtype(dbmtDnHF);
                    tDnHit_f = TRUE;
                    break;
                case dbmtDnB:
                case dbmtDnHB:
                    pHitman->chCommon_.SetMtype(dbmtDnHB);
                    tDnHit_f = TRUE;
                    break;
                default://死なないときがあったのを修正
                    if (CheckBackHit_f)
                    {
                        pHitman->chCommon_.SetMtype(dbmtFlB);
                    }
                    else
                    {
                        pHitman->chCommon_.SetMtype(dbmtFlF);
                    }
                    break;
                }
            }


            //ヒットフラグオン
            st_.pstSh_->cmn.hit_f = TRUE;//段階移行チェック後にオフにする
            st_.pstSh_->cmn.hit_c = st_.pstSh_->cmn.hit_c + 1;

            if (st_.pstSh_->cmn.hit_c == 1)
            {
                for (s32 i = 0; i < 2; ++i)
                {
                    for (s32 i2 = 0; i2 < 7; ++i2)
                    {
                        st_.pstSh_->cmn.comboState[i][i2] = 0;
                    }
                }
            }

            s32 tNum = pHitman->st_.mysideNo_;
            s32 pNum = pHitman->st_.posNo_;
            pHitman->st_.pstMyCh_->combo_c = st_.pstSh_->cmn.comboState[tNum][pNum];

            //効果音の違い
            if ((st_.pstSh_->idata.rank < 4)
                && (st_.pstSh_->idata.nice_f == FALSE)
                && (st_.pstSh_->idata.counter_f == FALSE)
                )
            {//弱ヒット
                s32 seHitId = seHitLv0;
                //チウチウ連続ヒット中
                //        if ((pHitman->st_.pstMyCh_->combo_c >= 1) && (pHitman->st_.pstMyCh_->Stick_f == TRUE))
                //        if (pHitman->st_.pstMyCh_->hitSound != 0) seHitId = pHitman->st_.pstMyCh_->hitSound + 1;
                s32 seKariNo = st_.pstSh_->idata.eff.seHit_NumNrml[st_.pstSh_->cmn.efEl.seHit.times_c];

                if (pHitman->st_.pstMyCh_->hitSound != 0)
                {
                    if (pHitman->st_.pstMyCh_->combo_c >= 1)
                    {
                        //            seHitId = seKariNo + 1;
                        seHitId = seKariNo;
                    }
                }
                enSEID seID = pmgSG_->stSh_.GetSENum(seHitId);

                //ちうちう以外なら指定ＳＥを鳴らす
                //                if (pHitman->st_.pstMyCh_->comboMax <= 0)
                if (st_.pstSh_->cmn.efEl.combo_f == FALSE)
                {
                    pCommon_->SESet(seID);
                }
            }
            else
            {//強ヒット

                //必殺シュートの音をデフォで入れておく
                enSEID seID = seHitLv1;//あとで　exSE_f = TRUE　なら更新する

                //特殊必殺音を使うかのチェックフラグ
                BOOL exSE_f = FALSE;
                if ((pmgSG_->stSh_.cmn.efEl.seHit.phase_f == TRUE)
                    && (pmgSG_->stSh_.cmn.efEl.seHit.incld_f == TRUE))
                    exSE_f = TRUE;

                //ヒット音IDをセットする
                //発動音の配列番号
                s32 arrNum = st_.pstSh_->cmn.efEl.seHit.times_c;

                //上半身ヒット時の音
                s32 seHitId = st_.pstSh_->idata.eff.seHit_NumNrml[arrNum];
                if (seHitId == 0) seHitId = DEF_NORM_NUM;//データがないときは必殺やられ
                //チウチウ連続ヒット中
                //        if ((pHitman->st_.pstMyCh_->combo_c >= 1) && (pHitman->st_.pstMyCh_->Stick_f == TRUE) && (seHitId == 61))
                //            seHitId = SOUND_CHIU;

                //下半身ヒット時の音の設定
                const f32 UNDER_LIMIT = 12.0f;//下半身上限
                BOOL underAngle = FALSE;
                f32 spdX_a = fabs(st_.pstSh_->phs.spdX);
                f32 spdY_a = fabs(st_.pstSh_->phs.spdY);
                if (st_.pstSh_->phs.spdY >= 0.00f) underAngle = TRUE;
                if ((spdX_a != 0.00f) && ((spdY_a / spdX_a) <= 1.0f)) underAngle = TRUE;

                //下半身上限未満にヒットしたら前後ヒットを行う
                if (
                    (st_.pstSh_->phs.posY < ((f32)(pHitman->st_.pstMyCh_->Zahyou.Y / XYMAG) + UNDER_LIMIT))
                    &&
                    (underAngle == TRUE)
                    )
                {
                    //基本は前方ヒット
                    seHitId = st_.pstSh_->idata.eff.seHit_NumFrnt[arrNum];
                    if (seHitId == 0) seHitId = DEF_FRNT_NUM;//データがないときは必殺やられ

                    //チウチウ連続ヒット中
                    //          if ((pHitman->st_.pstMyCh_->combo_c >= 1) && (pHitman->st_.pstMyCh_->Stick_f == TRUE))
                    //              seHitId = SOUND_CHIU;


                    if (
                        //球→方向＆敵→方向
                        ((st_.pstSh_->phs.spdX >= 0.0f) && (pHitman->st_.pstMyCh_->Zahyou.Muki == mR))
                        ||
                        //球←方向＆敵←方向
                        ((st_.pstSh_->phs.spdX < 0.0f) && (pHitman->st_.pstMyCh_->Zahyou.Muki == mL))
                        )
                        //後方ヒット
                    {
                        seHitId = st_.pstSh_->idata.eff.seHit_NumBack[arrNum];
                        if (seHitId == 0) seHitId = DEF_BACK_NUM;//データがないときは必殺やられ
                    }
                }
                //特殊SE使う場合はseIDを更新
                //            if (exSE_f == TRUE) seID = pmgSG_->stSh_.GetSENum(seHitId);
                seID = pmgSG_->stSh_.GetSENum(seHitId);

                //ちうちう以外なら指定ＳＥを鳴らす
                //                if (pHitman->st_.pstMyCh_->comboMax <= 0)
                if (st_.pstSh_->cmn.efEl.combo_f == FALSE)
                {
                    pCommon_->SESet(seID);
                }
            }

            //ちうちうダメージなら　特殊ＳＥ
            //            if (pHitman->st_.pstMyCh_->comboMax > 0)
            if (st_.pstSh_->cmn.efEl.combo_f == TRUE)
            {
                s32 arrNum = st_.pstSh_->cmn.efEl.seHit.times_c;
                s32 seHitId = st_.pstSh_->idata.eff.seHit_NumNrml[arrNum] + 1;
                if (seHitId == 1) seHitId = 19;//データがないときは通常やられ（ちう１）
                pHitman->st_.pstMyCh_->seChiu = pmgSG_->stSh_.GetSENum(seHitId);
                if (pHitman->st_.pstMyCh_->combo_c == 0) pCommon_->SESet(pHitman->st_.pstMyCh_->seChiu);
            }
        }

        //パスヒットダウンヒット
        if ((pHitman->st_.pstMyCh_->Motion.Mtype == dbmtPHF)
            || (pHitman->st_.pstMyCh_->Motion.Mtype == dbmtPHB)
            || (pHitman->st_.pstMyCh_->Motion.Mtype == dbmtDnHF)
            || (pHitman->st_.pstMyCh_->Motion.Mtype == dbmtDnHB))
        {
            pHitman->st_.pstMyCh_->Zahyou.dX = 0;
            pHitman->st_.pstMyCh_->Zahyou.dY = 0;
            pHitman->st_.pstMyCh_->Zahyou.dZ = 0;
        }
        else//シュートヒット
        {

            //ヒットフラグオン
            //      st_.pstSh_->cmn.hit_f = TRUE;//段階移行チェック後にオフにする
            //      st_.pstSh_->cmn.hit_c = st_.pstSh_->cmn.hit_c + 1;

            //      if (st_.pstSh_->cmn.hit_c == 1)
            //      {
            //        for (s32 i = 0; i < 2; ++i)
            //        {
            //          for (s32 i2 = 0; i2 < 7; ++i2)
            //          {
            //            st_.pstSh_->cmn.comboState[i][i2];
            //          }
            //        }
            //      }

            //重力がいじられている特殊吹っ飛びのフラグをOFFに
            pHitman->st_.pstMyCh_->flySpGrav_f = FALSE;
            pHitman->st_.pstMyCh_->flyLoose_f = FALSE;
            pHitman->st_.pstMyCh_->refHor_f = FALSE;
            pHitman->st_.pstMyCh_->rndWorld_f = FALSE;
            pHitman->st_.pstMyCh_->suction_f = FALSE;
            pHitman->st_.pstMyCh_->flying_c = 0;
            pHitman->st_.pstMyCh_->hitSound = st_.pstSh_->idata.eff.seHit_NumNrml[st_.pstSh_->cmn.efEl.seHit.times_c];
            pHitman->st_.pstMyCh_->suctionLockX_f = FALSE;
            pHitman->st_.pstMyCh_->suctionLockDistX = 0;
            pHitman->st_.pstMyCh_->suctionLockZ_f = FALSE;
            pHitman->st_.pstMyCh_->suctionLockDistZ = 0;

            //ゆるふっとびフラグオフフラグ･･･ゆるふっとび中で他のフラグが入っていた場合
            //次回以降ゆるふっとびフラグはオフにする
            BOOL flyLooseOff_f = FALSE;
            if (pHitman->st_.pstMyCh_->flyLoose_f == TRUE)
            {//ゆる吹っ飛び中
                if (
                    (pHitman->st_.pstMyCh_->Stick_f)//くっつき
                    || (pHitman->st_.pstMyCh_->rndWorld_f)//12:世界一周効果
                    || (pHitman->st_.pstMyCh_->refHor_f)//横跳ね吹っ飛びフラグ

                    || (st_.pstSh_->cmn.efEl.flyReverse_f)//7:引っ張り効果b
                    || (st_.pstSh_->cmn.efEl.flyVer_f)  //11:垂直吹っ飛び効果b
                    || (st_.pstSh_->cmn.efEl.flyHor_f)  //11:横吹っ飛び効果b
                    || (st_.pstSh_->cmn.efEl.noMvDwn_f)//その場ダウン
                    )//いずれかの吹っ飛びフラグも立っていたら
                    flyLooseOff_f = TRUE;//ゆる吹っ飛びフラグオフフラグをオンに
            }



            //くっつき効果新シュート後ほど
            //ボールから各フラグを受け取る
            // (pmgEO_->mgDt_.dtShot_.GetDtBOOL((s32)st_.pstBa_->ShSt.HSNo, shdtStick));//くっつき
            pHitman->st_.pstMyCh_->Stick_f = st_.pstSh_->cmn.efEl.peneStick_f;//くっつき
            pHitman->st_.pstMyCh_->rndWorld_f = st_.pstSh_->cmn.efEl.rndWorld_f;  //12:世界一周効果
            pHitman->st_.pstMyCh_->rndWorldLv = st_.pstSh_->cmn.efEl.rndWorldLv;  //12:世界一周効果Lv
            pHitman->st_.pstMyCh_->suction_f = st_.pstSh_->cmn.efEl.suction_f;  //12:吸引効果
            pHitman->st_.pstMyCh_->suctionLv = st_.pstSh_->cmn.efEl.suctionLv;  //12:吸引効果Lv
            pHitman->st_.pstMyCh_->suctionFlySpdRatio = 0;//吸引効果の速度関連初期化
            pHitman->st_.pstMyCh_->combo_f = st_.pstSh_->cmn.efEl.combo_f;//コンボフラグ
            pHitman->st_.pstMyCh_->refVer_f = st_.pstSh_->cmn.efEl.refVer_f;//縦跳ね吹っ飛びフラグ
            pHitman->st_.pstMyCh_->refVerLv = st_.pstSh_->cmn.efEl.refVerLv;//縦跳ね吹っ飛びフラグLv
            pHitman->st_.pstMyCh_->refHor_f = st_.pstSh_->cmn.efEl.refHor_f;//横跳ね吹っ飛びフラグ
            pHitman->st_.pstMyCh_->refHorLv = st_.pstSh_->cmn.efEl.refHorLv;//横跳ね吹っ飛びフラグLv
            pHitman->st_.pstMyCh_->flyLoose_f = st_.pstSh_->cmn.efEl.flyLoose_f;  //11:吹っ飛び緩効果b
            pHitman->st_.pstMyCh_->noDmg_f = st_.pstSh_->cmn.efEl.noDmg_f;  //130:ダメージナシ効果b
            pHitman->st_.pstMyCh_->noMvDwn_f = st_.pstSh_->cmn.efEl.noMvDwn_f;//その場ダウン
            pHitman->st_.pstMyCh_->noMvDwnLv = st_.pstSh_->cmn.efEl.noMvDwnLv;//その場ダウン

            BOOL flyReverse_f = st_.pstSh_->cmn.efEl.flyReverse_f;//7:引っ張り効果b
            BOOL flyVer_f = st_.pstSh_->cmn.efEl.flyVer_f;  //11:垂直吹っ飛び効果b
            BOOL flyHor_f = st_.pstSh_->cmn.efEl.flyHor_f;  //11:横吹っ飛び効果b
            BOOL noMvDwn_f = st_.pstSh_->cmn.efEl.noMvDwn_f;//その場ダウン

            s32 tNum = pHitman->st_.mysideNo_;
            s32 pNum = pHitman->st_.posNo_;

            if (pNum <= 3) st_.pstSh_->cmn.comboGncdHit[pNum] = TRUE;

            //その場ダウンは一旦無敵
            if (noMvDwn_f == TRUE) pHitman->st_.pstMyCh_->Motion.AddMFlags(dbmfMTK);

            //コンボフラグが立たないならヒット数は１のまま
            //      if ((pHitman->st_.pstMyCh_->combo_f == FALSE)) pHitman->st_.pstMyCh_->combo_c = 1, pHitman->st_.pstMyCh_->comboMax = 0;
//            if ((pHitman->st_.pstMyCh_->combo_f == FALSE)) pHitman->st_.pstMyCh_->comboMax = 0;
            //↑くっつき系・ちうちう系のダウン復帰相手に一時的に当たらないので一旦コメントアウト　15/06/14
            if (pHitman->st_.pstMyCh_->hitMTime_cd <= 0)
            {
                pHitman->st_.pstMyCh_->combo_c = st_.pstSh_->cmn.comboState[tNum][pNum] + 1;
                st_.pstSh_->cmn.comboState[tNum][pNum] = pHitman->st_.pstMyCh_->combo_c;
            }
            //ヒットしたときにくっつき属性があれば　くっつき中フラグを立てる
            //くっつき吹っ飛びの方で毎Fr　FALSEにして　
            if ((pHitman->st_.pstMyCh_->Stick_f == TRUE) || (st_.pstSh_->cmn.efEl.penet_c > 0))
            {
                pHitman->st_.pstMyCh_->Sticking_f = TRUE;//くっつき中フラグ
                pHitman->st_.pstMyCh_->Sticked_f = FALSE;//くっつき後フラグ
            }
            //再ヒット効果処理
            if (
                (pHitman->st_.pstMyCh_->combo_f == TRUE)
                &&
                ((st_.pstSh_->cmn.comboState[tNum][pNum] == 0) || (pHitman->st_.pstMyCh_->hitMTime_cd <= 0))
                )
            {
                //          s32 tNum = pHitman->st_.mysideNo_;
                //          s32 pNum = pHitman->st_.posNo_;
                //          pHitman->st_.pstMyCh_->combo_c = st_.pstSh_->cmn.comboState[tNum][pNum] + 1;
                //          st_.pstSh_->cmn.comboState[tNum][pNum] = pHitman->st_.pstMyCh_->combo_c;
                pHitman->st_.pstMyCh_->comboMax = st_.pstSh_->cmn.efEl.combo_c;
                pHitman->st_.pstMyCh_->hitMTime_cd = st_.pstSh_->cmn.efEl.comboNoHitTime_c;//コンボ系の無敵時間カウント
                pHitman->st_.pstMyCh_->hitMltDam = st_.pstSh_->cmn.efEl.comboDam;
                pHitman->st_.pstMyCh_->Motion.AddMFlags(dbmfMTK);
            }

            //吹っ飛び効果別処理
            GetFlyEffData(pHitman, flyLooseOff_f);
            //吹っ飛び角度
            //        f32 fr = lib_num::DegToRad((f32)pmgEO_->mgDt_.dtRank_.GetRankDt(rkopFlyAg, st_.pstSh_->idata.rank));
            //        s32 fs = (pmgEO_->mgDt_.dtRank_.GetRankDt(rkopFlySp, st_.pstSh_->idata.rank));
            f32 fr = stHit_.flyAngle;
            //吹っ飛び速度Y、X(%)
            s32 fs = stHit_.flySpdY;
            s32 fx = stHit_.flySpdX;

            if (tDnHit_f)
            {
                pHitman->st_.pstMyCh_->Zahyou.dX = 0;
                pHitman->st_.pstMyCh_->Zahyou.dY = 0;
                pHitman->st_.pstMyCh_->Zahyou.dZ = 0;
            }
            else
            {
                //引っ張り効果素材用データ
                s32 revX = 1;
                //引っ張り効果があった場合は基本的にXは逆に飛び、Zは移動なし
                if (flyReverse_f == TRUE) revX = -1;

                if (st_.pstBa_->Zahyou.dX == 0)
                {
                    pHitman->st_.pstMyCh_->Zahyou.dX = 0;
                    pHitman->st_.pstMyCh_->Zahyou.dZ = (s32)(lib_num::Sign(st_.pstBa_->Zahyou.dZ) * (cos(fr) * fs)) * fx * revX / 100;
                }
                else
                {
                    if ((ZShoot_f) || (pHitman->st_.pstMyCh_->refHor_f == TRUE))
                    {
                        if (hp != 0)
                        {
                            //球速XYのみで分割したデータ
                            f32 powXZ = (s32)(lib_num::Sign(st_.pstBa_->Zahyou.dX) * (cos(fr) * fs)) * fx * revX / 100;
                            //それをXZ合計値で割る
                            f32 vecXZ = fabs(powXZ / (fabs(st_.pstSh_->phs.spdZ) + fabs(st_.pstSh_->phs.spdX)));

                            //X,Z速度を掛けることでXZ速度分割する
                            pHitman->st_.pstMyCh_->Zahyou.dX = vecXZ * st_.pstSh_->phs.spdX;
                            pHitman->st_.pstMyCh_->Zahyou.dZ = vecXZ * st_.pstSh_->phs.spdZ;

                            //                pHitman->st_.pstMyCh_->Zahyou.dX = (s32)(cos(fr) * fs * (st_.pstBa_->Zahyou.dX / hp)) * fx * revX / 100;
                            //  　　　　　　　pHitman->st_.pstMyCh_->Zahyou.dX = (s32)(lib_num::Sign(st_.pstBa_->Zahyou.dX) * (cos(fr) * fs)) * fx * revX / 100;
                            //ｃｓｖから取得したデータにｚ/ｘをかけてZ方向速度とする
                            //              f32 spdZPow = fabs((f32)pHitman->st_.pstMyCh_->Zahyou.dX * (st_.pstSh_->phs.spdZ / st_.pstSh_->phs.spdX));
                            //              f32 spdZDir = 0;
                            //              if (st_.pstSh_->phs.spdZ != 0.0f) spdZDir = st_.pstSh_->phs.spdZ / fabs(st_.pstSh_->phs.spdZ);
                            //             pHitman->st_.pstMyCh_->Zahyou.dZ = spdZPow * spdZDir;
                        }
                        else
                        {
                            pHitman->st_.pstMyCh_->Zahyou.dX = 0;
                            pHitman->st_.pstMyCh_->Zahyou.dZ = (s32)(cos(fr) * fs * (st_.pstBa_->Zahyou.dZ / hp)) * fx * revX / 100;
                        }
                    }
                    else
                    {
                        pHitman->st_.pstMyCh_->Zahyou.dX = (s32)(lib_num::Sign(st_.pstBa_->Zahyou.dX) * (cos(fr) * fs)) * fx * revX / 100;
                        pHitman->st_.pstMyCh_->Zahyou.dZ = 0;
                    }
                }

                //重力処理が入る吹っ飛びはfs＝Y初速なのでsif(fr)を無関係にするためsif(fr)=1とする
                if (pHitman->st_.pstMyCh_->flySpGrav_f == TRUE) fr = 3.1415f / 2.0f;
                pHitman->st_.pstMyCh_->Zahyou.dY = (s32)(sin(fr) * fs);


                //Ｚ軸に吹っ飛ぶ＆横跳ねじゃないとき　はふっとび量減少 
                const s32 ZSFLYPER = 50;
                if ((ZShoot_f) && (pHitman->st_.pstMyCh_->refHor_f == FALSE))
                {
                    pHitman->st_.pstMyCh_->Zahyou.dX = lib_num::Percent(pHitman->st_.pstMyCh_->Zahyou.dX, ZSFLYPER);
                    pHitman->st_.pstMyCh_->Zahyou.dZ = lib_num::Percent(pHitman->st_.pstMyCh_->Zahyou.dZ, ZSFLYPER);
                }
                else if (st_.pstBa_->Zahyou.dX == 0)
                {
                    pHitman->st_.pstMyCh_->Zahyou.dZ = lib_num::Percent(pHitman->st_.pstMyCh_->Zahyou.dZ, ZSFLYPER);
                }
            }

            //吹っ飛びスキップ量
            pHitman->st_.pstMyCh_->FlySkip = (st_.pstSh_->cmn.efEl.penet_c == 0)//貫通中はスキップしない
                ? GetRankDtPow(rkFrSkip)
                : 0;
            pHitman->st_.pstMyCh_->FlySkip_c = (pHitman->st_.pstMyCh_->FlySkip == 0)
                ? 0
                : 1;

            //パス
            if (st_.pstBa_->Motion == bmPass)
            {
                pHitman->st_.pstMyCh_->Round_f = FALSE;
                pHitman->st_.pstMyCh_->Roll_c = 0;
            }
            else//シュート
            {
                pHitman->st_.pstMyCh_->Round_f = FALSE;
                //世界一周
                if (pHitman->st_.pstMyCh_->rndWorld_f == TRUE)//世界一周効果があればTRUE
                {
                    pHitman->st_.pstMyCh_->Round_f = TRUE;
                }
                //else if→ifに修正…くっつき＋いっしゅうで　一周してしまうため
                if (pHitman->st_.pstMyCh_->Stick_f)
                {
                    pHitman->st_.pstMyCh_->Round_f = FALSE;
                }
                //ごろごろ

                pHitman->st_.pstMyCh_->Roll_c = (((ZShoot_f == FALSE) && (st_.pstSh_->idata.roll_f)))
                    ? GetRankDtHP(rkRollNum) * ROLLITV
                    : 0;
            }

        }


        if ((pHitman->st_.pstMyCh_->Motion.Mtype == dbmtRoF)
            || (pHitman->st_.pstMyCh_->Motion.Mtype == dbmtRoB))
        {
            pmgEO_->mgSound_.StopSE(seRoll);//ごろごろＳＥとめる
        }

        //新シュート貫通
        if ((st_.pstBa_->Motion == bmPass)
            || (st_.pstSh_->idata.sp_f == FALSE)
            || (st_.pstSh_->cmn.efEl.penet_f == FALSE)
            //|| (pmgEO_->mgDt_.dtShot_.GetDtBOOL((s32)st_.pstBa_->ShSt.HSNo, shdtPierce) == FALSE)
            )
        {
            //掛掛掛 シュート終わり 掛掛掛
            bound_f = TRUE;
            pmgEO_->mgSound_.StopHMNG();//必殺音を消す
        }
        else
        {
            //掛掛掛　貫通処理　掛掛掛
            //if (st_.pstSh_->cmn.hit_c == 0)
            //{
            //  st_.pstBa_->ShSt.HitTime = st_.pstSh_->cmn.timeTotal_c;
            //}
            //ヒットカウント増加
            //++st_.pstSh_->cmn.hit_c;
            st_.pstBa_->RbwTNo = pHitman->st_.ensideNo_;

            //貫通力１Down
            if (//くっつきでない場合
                (pHitman->st_.pstMyCh_->Stick_f == FALSE)
                ||//くっつき中でコンボ回数が１のときのみ（＝１回目のヒットだけ処理）
                (
                (pHitman->st_.pstMyCh_->comboMax >= 1)
                && (pHitman->st_.pstMyCh_->hitMTime_cd == st_.pstSh_->cmn.efEl.comboNoHitTime_c)
                && (pHitman->st_.pstMyCh_->combo_c == 1)
                && (pHitman->st_.pstMyCh_->Stick_f == TRUE)
                )
                ||//くっつき中でちうちう以外
                (
                (pHitman->st_.pstMyCh_->comboMax <= 0)
                && (pHitman->st_.pstMyCh_->Stick_f == TRUE)
                )
                ) --st_.pstSh_->cmn.efEl.penet_c;

            //貫通力０になったら次回ヒットでシュートが終わるよう貫通フラグをOFFに
            if (st_.pstSh_->cmn.efEl.penet_c <= 0) st_.pstSh_->cmn.efEl.penet_f = FALSE;

            //威力減少
            lib_num::AprToDest(&st_.pstSh_->idata.pow, 1, pmgEO_->mgDt_.dtSet_.GetDtShot(setPierceDown));
        }

        //バウンドにかわったかどうか
        return bound_f;
    }

    void TBaJudge::HitReactionBound(TMgChar* pHitman, enJudgeType JdType)
    {
        //縦方向シュート
        BOOL ZShoot_f = FALSE;

        if (st_.pstSh_->idata.sp_f)
        {
            //世界一周はしやすく
            ZShoot_f = (st_.pstSh_->idata.rank == 0)
                ? (abs(st_.pstBa_->Zahyou.dZ) > abs(st_.pstBa_->Zahyou.dX))
                : (abs(st_.pstBa_->Zahyou.dZ) > abs(st_.pstBa_->Zahyou.dX / 3));
        }
        else
        {
            ZShoot_f = (abs(st_.pstBa_->Zahyou.dZ) > abs(st_.pstBa_->Zahyou.dX / 3));
        }

        f32 hp = lib_num::Hypot(st_.pstBa_->Zahyou.dX, st_.pstBa_->Zahyou.dZ);

        if (st_.pstBa_->Motion == bmPass)
        {
            const f32 PASSDEG = 120.0;
            f32 rr = lib_num::DegToRad(PASSDEG);
            s32 sp = GetRankDtLevel(rkPaBwSpd);
            f32 rxz = (cos(rr) * sp);
            f32 ry = (sin(rr) * sp);
            if (hp != 0)
            {
                st_.pstBa_->Zahyou.dX = -(s32)(rxz * (st_.pstBa_->Zahyou.dX / hp));
                st_.pstBa_->Zahyou.dZ = -(s32)(rxz * (st_.pstBa_->Zahyou.dZ / hp));
            }

            st_.pstBa_->Zahyou.dY = (s32)ry;

            //ボールの処理
            pCommon_->SetMtype(bmBound);
            st_.pstBa_->ShootFirstBound_f = TRUE;//シュート着地１回目のバウンド
            //return;
        }
        else
        {

            HitReflex(pHitman, ZShoot_f);
            st_.pstBa_->RbwTNo = pHitman->st_.ensideNo_;
        }
    }

    void TBaJudge::HitReactionTuto(TMgChar* pHitman, enJudgeType JdType, BOOL dam_f)
    {
        //チュートリアルのＮＧ行為
        if (JdType == JdHit)
        {
            //チュートＮＧキャッチ失敗
            if (pHitman->st_.mysideNo_ == SIDE0)
            {
                if (st_.pmgRf_->GetTutoActBtn())
                {
                    st_.pmgRf_->SetTutoNG(sta_CatchAct, rta_catch_ngearly);//キャッチはやい
                    st_.pmgRf_->SetTutoNG(sta_DodgeAct, rta_dodge_ngearly);//チュートＮＧよけ失敗
                }
                else
                {
                    st_.pmgRf_->SetTutoNG(sta_CatchAct, rta_catch_nglate);//キャッチ遅い
                    st_.pmgRf_->SetTutoNG(sta_DodgeAct, rta_dodge_nglate);//チュートＮＧよけ失敗
                }
            }
        }
    }

    void TBaJudge::HitReactionDamage(TMgChar* pHitman, enJudgeType JdType)
    {
        //成績用シュートした人
        TMgChar* Shotman = st_.pmgTm_[st_.pstBa_->PichTNo]->st_.pmgMyCh_[st_.pstBa_->PichPNo];
        //ダメージ(ダウン時ヒットの時はココで死亡処理)
        s32 tdam = stHit_.dam_;

        //キャッチダメージ
        if (JdType == JdCatch)
        {
            tdam = stHit_.cadam_;

            ////削り死亡チェックをしてるのでこないはず
            //if ((tdam >= pHitman->st_.pstMyCh_->HP) && (pHitman->st_.pstMyCh_->HP > 1))
            //{
            //  tdam = (pHitman->st_.pstMyCh_->HP - 1);
            //}
        }

        //ＨＰの余韻表示と共有する
        pHitman->st_.pstMyCh_->CaDamAfter_c = 40;
        //pHitman->
        st_.pmgTm_[pHitman->st_.mysideNo_]->st_.pstMyTm_->dam_c_ = 40;
        st_.pmgTm_[pHitman->st_.mysideNo_]->st_.pstMyTm_->dampos_ = pHitman->st_.objNo_;


        //同一キャラヒット時のダメージ
        //同一キャラコンボ効果が無いとき
        if (pHitman->st_.pstMyCh_->comboMax == 0)
        {
            //同一キャラヒット時のダメージ
            if (pHitman->st_.pstMyCh_->combo_c >= 2)
            {
                //逓減率　セッティングデータ／100
                f32 dwnPer = (f32)pmgEO_->mgDt_.dtSet_.GetDtShot(setComboDamDownPer) / 100.0f;
                //シュートのダメージ ×　逓減率の（ヒット数－１）乗
                tdam = tdam * pow(dwnPer, pHitman->st_.pstMyCh_->combo_c - 1);
                if (tdam < 1) tdam = 1;
            }
        }
        else
        {
            //同一キャラコンボ効果があるとき
            //くっつきダメージのダメージ（上書き）
            if (
                (pHitman->st_.pstMyCh_->combo_c >= 2)
                &&
                (pHitman->st_.pstMyCh_->Stick_f == TRUE)
                &&
                (pHitman->st_.pstMyCh_->hitMltDam != 0)
                )
            {
                tdam = pHitman->st_.pstMyCh_->hitMltDam;
                pCommon_->SESet(pHitman->st_.pstMyCh_->seChiu);
            }
            else
                if (
                    (pHitman->st_.pstMyCh_->combo_c >= 2)
                    &&
                    (pHitman->st_.pstMyCh_->Stick_f == FALSE)
                    &&
                    (pHitman->st_.pstMyCh_->hitMltDam == 0)
                    )
                {
                    //逓減率　50／100
                    f32 dwnPer = 50.0f / 100.0f;
                    //シュートのダメージ ×　逓減率の（ヒット数－１）乗
                    tdam = tdam * pow(dwnPer, pHitman->st_.pstMyCh_->combo_c - 1);
                    if (tdam < 1) tdam = 1;
                }
        }

        //ノーダメージ効果中ならダメージ０
        if (pHitman->st_.pstMyCh_->noDmg_f == TRUE) tdam = 0;


        //ダウンカウンタ勧める
        if ((pHitman->st_.pstMyCh_->Motion.Mtype == dbmtDnF)
            || (pHitman->st_.pstMyCh_->Motion.Mtype == dbmtDnB))
        {
            pHitman->st_.pstMyCh_->Down_c += 20;
        }

        //プレビューではダメージ入らない
        if ((tdam > 0) && (pmgEO_->stShiai_.IsPrev() == FALSE))
        {
#ifdef __K_DEBUG_SHIAI__
            if (!kdebug::DebugSystem::GetInstance()->IsDamageZero())
            {
                // ここをコメントアウトするとダメージ計算がなくなるので永遠に死なない
                lib_num::AprTo0(&pHitman->st_.pstMyCh_->HP, tdam);
            }
#else
            lib_num::AprTo0(&pHitman->st_.pstMyCh_->HP, tdam);
#endif	// #ifdef __K_DEBUG_SHIAI__
            pHitman->st_.pstMyCh_->MHP_f = FALSE;
        }


        //★成績
        Shotman->chCommon_.AddRec(recTotaldam, tdam);//トータルダメージ
        Shotman->chCommon_.MaxRec(recMaxdam, tdam);//マックスダメージ

        {
            if ((pHitman->st_.pstMyCh_->HP == 0)
                //&& (pHitman->MyPDt(clv_Cnt) == JPN)
                //&& (pHitman->st_.pmgMyTm_->IsLastOne())
                )
            {
                //st_.pmgMyCh_[i]->MyName()
                //おたのしみ
                pmgGO_->pmgCommon_->PopupMsg(pu_Damage, pHitman->MySerifu().c_str(),
                    pHitman->st_.mysideNo_,
                    pHitman->st_.pstMyCh_->Zahyou.X,
                    pHitman->st_.pstMyCh_->Zahyou.Y, pHitman->st_.pstMyCh_->Zahyou.Z, FC2_shiai, FALSE);
            }
            //else
      {
          //ダメージフォント（デモ中以外）
          pmgGO_->pmgCommon_->PopupMsg(tdam,
              pHitman->st_.pstMyCh_->Zahyou.X,
              pHitman->st_.pstMyCh_->Zahyou.Y - (8 * XYMAG),
              pHitman->st_.pstMyCh_->Zahyou.Z,
              FC2_shiai);
#ifdef __K_DEBUG_SHIAI__
          kdebug::DebugSystem::GetInstance()->AddShootDebugLogDamage(pHitman->st_.mysideNo_, pHitman->st_.posNo_, tdam);
#endif
      }
        }



        //ファンブルでないときはかがみ値の代入もする
        if ((tdam > 0) && (JdType == JdHit))
        {
            pHitman->st_.pstMyCh_->Kagami_c = GetRankDtHP(rkBreath);
            pHitman->st_.pstMyCh_->KagamiRev_c = 0;
            pHitman->st_.pstMyCh_->Auto.ADashStoped_f = FALSE;

            //★成績//シュートを当てた数
            Shotman->chCommon_.AddRec(recHit_c);

            //★成績//ないすシュートを当てた数
            if (st_.pstSh_->idata.nice_f)
            {
                Shotman->chCommon_.AddRec(recNiceHit_c);
            }
            else if (st_.pstSh_->idata.sp_f)//必殺シュートを当てた数
            {
                if (st_.pstSh_->idata.jsh_f)
                {
                    Shotman->chCommon_.AddRec(recJSPHit_c);//ジャンプ必殺
                }
                else
                {
                    Shotman->chCommon_.AddRec(recDSPHit_c);//ダッシュ必殺
                }
            }


        }

        //殺した
        if (pHitman->st_.pstMyCh_->HP <= 0)
        {
            //★成績//アウト数合計

            //++pmgEO_->mgStSn_.EtcSS.tmpdeathnum_[pHitman->st_.mysideNo_];
            pmgEO_->stResult_.IncTmpKillNum(pHitman->st_.ensideNo_);//斬る数はリセットかかるとまずい

            pHitman->chCommon_.AddRec(recDeath_c);//です数はタイムアップとの差別化が居るのでリセット掛かって良い
            ////★成績//奪アウト数合計
            //Shotman->chCommon_.AddRec(recKill_c);
        }

        //タゲ以外ヒット
        if (pHitman->IsInfield())
        {
            if (pHitman->st_.posNo_ != st_.pstBa_->ShTgPNo)
            {
                //★成績//ノンタゲでシュートに当たった数
                pHitman->chCommon_.AddRec(recNoTagDam_c);

                //★成績//タゲ以外シュートを当てた数
                Shotman->chCommon_.AddRec(recNoTagHit_c);
            }
            else
            {
                //★成績//タゲでシュートに当たった数
                pHitman->chCommon_.AddRec(recTagDam_c);
            }
        }


        //★成績//外野からシュート
        if (Shotman->IsInfield() == FALSE)
        {
            Shotman->chCommon_.AddRec(recGaiyaHit_c);
        }


        //死亡処理（成績より後回しにしないといくつかリセット掛かるので）
        if ((pHitman->st_.pstMyCh_->HP <= 0)
            && st_.pmgRf_->IsAngeling()
            && (pHitman->st_.pmgEnTm_->IsAllDead() == FALSE))
        {
            if (((pHitman->st_.pstMyCh_->Motion.Mtype == dbmtDnF)
                || (pHitman->st_.pstMyCh_->Motion.Mtype == dbmtDnB)
                || (pHitman->st_.pstMyCh_->Motion.Mtype == dbmtDnHB)
                || (pHitman->st_.pstMyCh_->Motion.Mtype == dbmtDnHF)
                ))//相手チームが全員死んでるときは死なない
            {
                pHitman->st_.pstMyCh_->ANGEL_f = TRUE;
                st_.pmgTm_[pHitman->st_.mysideNo_]->CheckChangePos();//★★
            }
        }
    }

    //ヒットチェック
    void TBaJudge::HitReaction(TMgChar* pHitman, enJudgeType JdType)
    {

        //敵のシュートボール
        BOOL ensh_f = ((st_.pstBa_->PichTNo != pHitman->st_.mysideNo_) //敵のシュート
            && (st_.pstBa_->Motion == bmShoot));//シュートボール

        //BOOL dam_f = (((JdType == JdHit) || (stHit_.sp_f_ && (JdType == JdCatch))) && stHit_.enshot_f_);
        BOOL dam_f = (((JdType == JdHit) || (JdType == JdCatch)) && stHit_.enshot_f_);
        //守備機会カウンタ
        if (JdType != JdNoHit)
        {
            //敵のシュートボール//内野
            if (ensh_f && (pHitman->IsInfield()))
            {
                pHitman->chCommon_.AddRec(recCatchTry_c);//★成績//守備機会数合計
            }
        }

        BOOL bound_f = FALSE;

        switch (JdType)
        {
            //キャッチ
        case JdCatch:
        case JdNiceCatch:
            HitReactionCatch(pHitman, JdType);
            break;
            //ヒット
        case JdHit:
            bound_f = HitReactionHit(pHitman, JdType);
            break;
        }

        //敵ＣＯＭ用情報セット
        HitReactionAI(pHitman, JdType);


        //ボールの反射処理
        if (bound_f) HitReactionBound(pHitman, JdType);


        //ダメージフラグ
        //BOOL dam_f = ((JdType == JdHit) && stHit_.enshot_f_);

        //BOOL dam_f = ((JdType == JdHit) && (st_.pstBa_->PichTNo != pHitman->st_.mysideNo_));

        //チュートリアルのＮＧ行為
        if (pmgEO_->stShiai_.IsTuto()) HitReactionTuto(pHitman, JdType, dam_f);

        ////プレビュー最低1回当たった
        //if (dam_f) st_.pmgRf_->SetPrevReturn();

        //チュートリアルはノーダメージ
        BOOL tutonodam_f = ((pmgEO_->stShiai_.IsTuto() && (pHitman->st_.mysideNo_ == SIDE0)));

        //ダメージ//内野のみ//試合中のみ
        if (dam_f
            && pHitman->IsInfield()
            && (pmgSG_->stRf_.DBSSt == dbssShiai)
            && (tutonodam_f == FALSE)
            && (st_.pmgRf_->IsNoDamage() == FALSE)
            )
        {

            if (pHitman->st_.pstMyCh_->comboMax > 0)
            {//ちうちう効果中
                //ちうちうダメージタイミングなら　ダメージ
                if (
                    (pHitman->st_.pstMyCh_->hitMTime_cd == st_.pstSh_->cmn.efEl.comboNoHitTime_c)
                    &&
                    (pHitman->st_.pstMyCh_->combo_c < pHitman->st_.pstMyCh_->comboMax)
                    )
                    HitReactionDamage(pHitman, JdType);
            }
            else
            {//ちうちう効果がなければ無条件でダメージ
                HitReactionDamage(pHitman, JdType);
            }

        }

    }


    //吹っ飛び効果→吹っ飛び角度＆速度
    void TBaJudge::GetFlyEffData(TMgChar* pHitman, BOOL flyLooseOff_f)
    {
        //  s32 rankData = 1;//仮のランク
        s32 rankData = GetFlyRankData(pHitman);

        //デフォで通常シュート吹っ飛び
        SetFlyEffData(rkFryNlSpd, rankData);
        pHitman->st_.pstMyCh_->flySpGrav_f = FALSE;
        stHit_.effFly_f = FALSE;//特殊吹っ飛びフラグをひとまずOFFに

        if (st_.pstSh_->idata.iball.spShot_f == TRUE)
        {
            //後に別個になっている吹っ飛びデータを加算する可能性があるためif文で分けておく
            if (CheckBrainHit(pHitman) == TRUE) SetFlyEffDataSp(rkFryBrYSp, GetBrainHitLv(), TRUE, FALSE);//脳天ヒット処理　速度Yを見て吹っ飛びを決定
            if (pHitman->st_.pstMyCh_->rndWorld_f == TRUE) SetFlyEffDataSp(rkFryGrYSp, st_.pstSh_->cmn.efEl.rndWorldLv, TRUE, TRUE);  //12:世界一周効果
            if (pHitman->st_.pstMyCh_->refVer_f == TRUE) SetFlyEffDataSp(rkFryVRYSp, st_.pstSh_->cmn.efEl.refVerLv, FALSE, TRUE);//縦跳ね吹っ飛びフラグ
            if (pHitman->st_.pstMyCh_->refHor_f == TRUE) SetFlyEffDataSp(rkFryHRYSp, st_.pstSh_->cmn.efEl.refHorLv, TRUE, TRUE);//横跳ね吹っ飛びフラグ
            if (st_.pstSh_->cmn.efEl.flyReverse_f == TRUE) SetFlyEffDataSp(rkFryRvYSp, st_.pstSh_->cmn.efEl.flyReverseLv, FALSE, FALSE);//7:引っ張り効果b
            if ((st_.pstSh_->cmn.efEl.flyLoose_f == TRUE) && (flyLooseOff_f == FALSE))  SetFlyEffDataSp(rkFryLsYSp, st_.pstSh_->cmn.efEl.flyLooseLv, TRUE, TRUE); //11:吹っ飛び緩効果b
            if (st_.pstSh_->cmn.efEl.flyVer_f == TRUE)  SetFlyEffDataSp(rkFryVrYSp, st_.pstSh_->cmn.efEl.flyVerLv, FALSE, FALSE); //11:垂直吹っ飛び効果b
            if (st_.pstSh_->cmn.efEl.flyHor_f == TRUE)  SetFlyEffDataSp(rkFryHrYSp, st_.pstSh_->cmn.efEl.flyHorLv, TRUE, FALSE); //11:横吹っ飛び効果b
            if (st_.pstSh_->cmn.efEl.noMvDwn_f == TRUE) SetFlyEffDataSp(rkFryDnYSp, st_.pstSh_->cmn.efEl.noMvDwnLv, FALSE, FALSE);//その場ダウン

            //上記処理のデバッグ用
            //if (CheckBrainHit(pHitman) == TRUE) SetFlyEffDataSp(rkFryBrYSp, st_.pstBa_->debugFlyRankLv, TRUE, FALSE);//脳天ヒット処理　速度Yを見て吹っ飛びを決定
            //if (pHitman->st_.pstMyCh_->rndWorld_f == TRUE) SetFlyEffDataSp(rkFryGrYSp, st_.pstBa_->debugFlyRankLv, TRUE, TRUE);  //12:世界一周効果◎
            //if (pHitman->st_.pstMyCh_->refVer_f == TRUE) SetFlyEffDataSp(rkFryVRYSp, st_.pstBa_->debugFlyRankLv, FALSE, TRUE);//縦跳ね吹っ飛びフラグ
            //if (pHitman->st_.pstMyCh_->refHor_f == TRUE) SetFlyEffDataSp(rkFryHRYSp, st_.pstBa_->debugFlyRankLv, TRUE, TRUE);//横跳ね吹っ飛びフラグ◎
            //if (st_.pstSh_->cmn.efEl.flyReverse_f == TRUE) SetFlyEffDataSp(rkFryRvYSp, st_.pstBa_->debugFlyRankLv, FALSE, FALSE);//7:引っ張り効果◎
            //if (st_.pstSh_->cmn.efEl.flyLoose_f == TRUE)  SetFlyEffDataSp(rkFryLsYSp, st_.pstBa_->debugFlyRankLv, TRUE, TRUE); //11:吹っ飛び緩効果◎
            //if (st_.pstSh_->cmn.efEl.flyVer_f == TRUE)  SetFlyEffDataSp(rkFryVrYSp, st_.pstBa_->debugFlyRankLv, FALSE, FALSE); //11:垂直吹っ飛び効果◎
            //if (st_.pstSh_->cmn.efEl.flyHor_f == TRUE)  SetFlyEffDataSp(rkFryHrYSp, st_.pstBa_->debugFlyRankLv, TRUE, FALSE); //11:横吹っ飛び効果◎
            //if (st_.pstSh_->cmn.efEl.noMvDwn_f == TRUE) SetFlyEffDataSp(rkFryDnYSp, st_.pstBa_->debugFlyRankLv, FALSE, FALSE);//その場ダウン◎

            //重力を司る吹っ飛びがあればそのフラグをONに
            if (stHit_.flySpGrav_f == TRUE) pHitman->st_.pstMyCh_->flySpGrav_f = TRUE;

            //特殊吹っ飛びがない場合は必殺シュート汎用吹っ飛びを採用
            if (stHit_.effFly_f == FALSE) SetFlyEffData(rkFrySpSpd, rankData);//吹っ飛び効果のない必殺シュート
        }

    }

    //普通シュート・必殺シュート汎用吹っ飛び　データ開始番号から吹っ飛びデータをセット
    void TBaJudge::SetFlyEffData(s32 startNo, s32 rankDt)
    {
        enRankPow spd = (enRankPow)startNo;
        enRankPow agl = (enRankPow)(startNo + 1);

        stHit_.flySpdY = GetRankDtPow(spd);// pmgEO_->mgDt_.dtRank_.GetRankDt((enRankFlyPow)startNo, rankDt));//速度Y
        stHit_.flyAngle = lib_num::DegToRad((f32)GetRankDtPow(agl));//角度
        stHit_.flySpdX = 100;//速度X（％）
    }

    //特殊吹っ飛び　ランクデータと吹っ飛びデータ開始番号から吹っ飛びデータをセット
    void TBaJudge::SetFlyEffDataSp(s32 startNo, s32 rankDt, BOOL spdX_f, BOOL grav_f)
    {
        enRankPow no1 = (enRankPow)(startNo);//Dt1のデータ位置
        enRankPow no2 = (enRankPow)(startNo + 1);//Dt2のデータ位置
        enRankPow no3 = (enRankPow)(startNo + 2);//Dt3のデータ位置

        stHit_.flySpdY = GetRankDtPow2(no1, rankDt);//速度Y
        //  stHit_.flySpdY = GetRankDtPow(no1);//速度Y
        stHit_.flyAngle = lib_num::DegToRad((f32)GetRankDtPow2(no2, rankDt));//角度
        //  stHit_.flyAngle = lib_num::DegToRad((f32)GetRankDtPow(no2));//角度
        stHit_.flySpdX = 100;//速度X ひとまず１00
        if (spdX_f == TRUE) stHit_.flySpdX = GetRankDtPow2(no3, rankDt);//速度X
        //    if (spdX_f == TRUE) stHit_.flySpdX = GetRankDtPow(no3);// //速度X
        stHit_.effFly_f = TRUE;//特殊吹っ飛びフラグをTRUEに書き換える
        stHit_.flySpGrav_f = FALSE;
        if (grav_f == TRUE) stHit_.flySpGrav_f = TRUE;
    }

    //吹っ飛びランクを算出
    s32 TBaJudge::GetFlyRankData(TMgChar* pHitman)
    {
        //s32 rankLv;
        s32 rankLv = st_.pstBa_->debugFlyRankLv;//デバッグ用


        if (st_.pstSh_->idata.iball.spShot_f == TRUE)
        {//必殺シュート吹っ飛び（このデータが使われるのは特殊吹っ飛びが無い時のみ）
            //ボール側データ
            //st_.pstSh_->idata.stPow;//術者ステータス：ちから
            //st_.pstSh_->idata.stTeq;//術者ステータス：うまさ
            //st_.pstSh_->idata.stAgl;//術者ステータス：はやさ
            //st_.pstSh_->idata.stGut;//術者ステータス：がっつ
            st_.pstSh_->idata.pow;//シュート自体の攻撃力
            st_.pstSh_->idata.sharp;//シュート自体のキレ
            st_.pstSh_->idata.rank;//シュート自体のランク
        }
        else
        {//通常シュート吹っ飛び

        }
        return rankLv;
    }

    //脳天ヒットチェック
    BOOL TBaJudge::CheckBrainHit(TMgChar* pHitman)
    {
        BOOL res = FALSE;
        const s32 FACE_POS = 22;//脳天ヒット下端高さ（これより上を脳天ヒットとする）
        //…Y速度が強いとめり込むので要微調整
        //下降中
        if (st_.pstSh_->phs.spdY < 0)
        {
            if (//●速度X,Zが１以上のときは、ボールとヒットマンの位置を確認…ヒットマンの顔付近以上にヒットしていたら脳天ヒット
                (//速度Yが速度Xより大きい
                (fabs(st_.pstSh_->phs.spdY) > fabs(st_.pstSh_->phs.spdX))
                &&//速度Yが速度Zより大きい
                (fabs(st_.pstSh_->phs.spdY) > fabs(st_.pstSh_->phs.spdZ))
                &&//ボール位置がヒットマンの顔以上
                ((st_.pstSh_->phs.posY * 100) > (pHitman->st_.pstMyCh_->Zahyou.Y + FACE_POS * XYMAG))
                )
                //        ||
                //        (//●速度XorZが１未満なら無条件で脳天ヒット
                //        (fabs(st_.pstSh_->phs.spdX) < 1) && (fabs(st_.pstSh_->phs.spdZ) < 1)
                //        )
                )
                res = TRUE;//上記条件の時のみTRUEを返す
        }
        return res;
    }

    //脳天ヒットLv
    s32 TBaJudge::GetBrainHitLv()
    {
        s32 res = 0;

        //速度Xと速度Zから大きい方を選出しそいつと速度Yを比較する
        f32 spdQ = fabs(st_.pstSh_->phs.spdX);//仮に比較用変数に速度Xを入れる
        if (spdQ < fabs(st_.pstSh_->phs.spdZ)) spdQ = fabs(st_.pstSh_->phs.spdZ);//速度Xが速度Zに劣るなら比較変数に速度Zを入れる
        f32 spdY = fabs(st_.pstSh_->phs.spdY);//速度Y

        if (spdQ == 0)
        {//直下なら最大Lv
            res = 7;
        }
        else
            if (spdY == 0)
            {//多分ないけど念のため最低Lv
                res = 0;
            }
            else
            {
                s32 ratioQ = spdQ * 100 / spdY;//速度Qの％（＝Yで50％）
                //ratioQからLvを決定
                if (ratioQ > 45) res = 0;
                else if (ratioQ > 40) res = 1;
                else if (ratioQ > 35) res = 2;
                else if (ratioQ > 30) res = 3;
                else if (ratioQ > 25) res = 4;
                else if (ratioQ > 20) res = 5;
                else if (ratioQ > 10) res = 6;
                else res = 7;
            }

        return res;

    }

    //技量比ランクデータ
    s32 TBaJudge::GetRankDtLevel(enRankLevel dtNo)
    {
        return pmgEO_->mgDt_.dtRank_.GetRankDtLevel(dtNo, stHit_.shrank_);
    }
    //技量比ランクデータ
    s32 TBaJudge::GetRankDtTech(enRankTech dtNo)
    {
        return pmgEO_->mgDt_.dtRank_.GetRankDtTech(dtNo, stHit_.tecrank_);
    }
    //威力比ランクデータ
    s32 TBaJudge::GetRankDtPow(enRankPow dtNo)
    {
        return pmgEO_->mgDt_.dtRank_.GetRankDtPow(dtNo, stHit_.powrank_);
    }
    //威力比ランクデータ
    s32 TBaJudge::GetRankDtPow2(enRankPow dtNo, s32 rankDt)
    {
        return pmgEO_->mgDt_.dtRank_.GetRankDtPow(dtNo, rankDt);
    }
    //のこりＨＰデータ
    s32 TBaJudge::GetRankDtHP(enRankHP dtNo)
    {
        return pmgEO_->mgDt_.dtRank_.GetRankDtHP(dtNo, stHit_.hprank_);
    }
    //更新
    void TBaJudge::UpDate()
    {
        st_.pstBa_->HitTNo = NGNUM;
        st_.pstBa_->HitPNo = NGNUM;

        OSShoot();

        if (st_.pstBa_->HitTNo == NGNUM) return;
        if (st_.pstBa_->HitPNo == NGNUM) return;

        TMgChar* pHitman = st_.pmgTm_[st_.pstBa_->HitTNo]->st_.pmgMyCh_[st_.pstBa_->HitPNo];

        if (pHitman->st_.pstMyCh_->symCtrl_f == TRUE)
        {//マリオネット効果を消す
            pHitman->st_.pstMyCh_->symCtrl_f = FALSE;
            pHitman->st_.pstMyCh_->Motion.AddMFlags(dbmfAr); //ジャンプ状態
        }


        //とりあえずヒットしているので　現在のヒットフラグを立てる
        pHitman->st_.pstMyCh_->stk1StNow_f = TRUE;
        pHitman->st_.pstMyCh_->stk2StNow_f = TRUE;

        //1Fr前データを確認
        if (
            (st_.pstBa_->Motion == bmShoot)
            &&
            ((pHitman->st_.pstMyCh_->stk1StPst_f == TRUE) || (pHitman->st_.pstMyCh_->stk2StPst_f == TRUE))
            )
        {//前もヒットしていたらとりあえずフラグは立てる

            //ちうちう以外ならヒットしないとしてココで終了
            if (pHitman->st_.pstMyCh_->comboMax <= 0)
//            if (
//                (pHitman->st_.pstMyCh_->Stick_f == FALSE)
//            )
            {
                pHitman->st_.pstMyCh_->Sticking_f = TRUE;
                return;
            }
            //        else//ちうちうダメージタイム（＝0）以外ならココで終了
            //        if (pHitman->st_.pstMyCh_->hitMTime_cd > 0)
            //        {
            //            return;
            //        }

        }

        stHit_.Init();
        //投げ手のランク
        stHit_.shrank_ = st_.pstSh_->idata.rank;

        s32 hitman_tech = pHitman->MyStLv(dbst_Tech);
        s32 hitman_guts = pHitman->MyStLv(dbst_Guts);

        if (pmgEO_->stShiai_.IsPrev())
        {
            hitman_tech = XYMAG;
            hitman_guts = XYMAG;
        }

        //攻防比
        stHit_.tecrank_ = pmgEO_->mgDt_.dtRank_.GetRankTech(st_.pstSh_->idata.sharp, hitman_tech);
        stHit_.powrank_ = pmgEO_->mgDt_.dtRank_.GetRankPow(st_.pstSh_->idata.pow, hitman_guts);

        s32 maxhp = pmgEO_->mgDt_.dtSet_.GetDtDamage(setHP);
        s32 deadnum = pmgEO_->mgDt_.dtSet_.GetDtDamage(setDeadNum);
        s32 catchdamper = pmgEO_->mgDt_.dtSet_.GetDtDamage(setCatchDamPer);
        stHit_.dam_ = ((maxhp * st_.pstSh_->idata.pow * 10) / (deadnum * hitman_guts));
        stHit_.sp_f_ = st_.pstSh_->idata.sp_f;
        stHit_.enshot_f_ = (st_.pstBa_->PichTNo != pHitman->st_.mysideNo_);
        stHit_.cadam_ = lib_num::Percent(stHit_.dam_, catchdamper);
        stHit_.kezuridead_f_ = ((pHitman->st_.pstMyCh_->HP - stHit_.cadam_) <= 0);//削りで死ぬ状態
        s32 nexthp = (pHitman->st_.pstMyCh_->HP - stHit_.dam_);
        stHit_.hprank_ = pmgEO_->mgDt_.dtRank_.GetRankHP(nexthp, maxhp);


        enJudgeType JdType = ShootHit(pHitman);
        ////真キー待いったんカットする
        //if (JdType != JdNoHit)mid::midSetTrueWait_c();
        //ヒットチェック
        HitReaction(pHitman, JdType);
    }
}
