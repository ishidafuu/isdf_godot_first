using System;

namespace db;

/// <summary>
/// モーションに関するステータス
/// </summary>
public class CharaMotionState : ICharaMotionStateGetter, ICharaMotionStateSetter
{
    public CharaMotionType MotionType { get; private set; }
    public CharaMotionFlag MotionFlag { get; private set; }
    public UpCounter MotionCount { get; private set; }
    public int MotionCountValue => MotionCount.Value;
    public CharaMotionNo MotionNo { get; private set; }
    public UpCounter KomaNo { get; private set; }
    public int KomaNoValue => KomaNo.Value;
    public UpCounter KomaFrameCount { get; set; }
    public int KomaFrameCountValue => KomaFrameCount.Value;
    public DownCounter LoopCount { get; set; }
    public int LoopCountValue => LoopCount.Value;
    public int LoopStartKomaNo { get; set; }
    public bool IsActionPoint { get; set; }

    public void Initialize()
    {
        MotionType = default;
        MotionFlag = default;
        MotionCount.Clear();
        MotionNo = default;
        KomaNo.Clear();
        KomaFrameCount.Clear();
        LoopCount.Clear();
        LoopStartKomaNo = 0;
        IsActionPoint = false;
    }

    public bool HasFlag(CharaMotionFlag flag)
    {
        return MotionFlag.HasFlag(flag);
    }

    public void SubMotionFlag(CharaMotionFlag targetFlag)
    {
        MotionFlag &= ~targetFlag;
    }

    public void AddMotionFlag(CharaMotionFlag flag)
    {
        MotionFlag |= flag;
    }

    public void Progress()
    {
        MotionCount.Add();
    }

    public void SetMotionNo(CharaMotionType motionType, CharaMotionNo motionNo, CharaMotionFlag motionFlag,
        DirectionZType directionZType)
    {
        MotionType = motionType;
        MotionFlag = motionFlag;
        MotionNo = ShiftMotionDirectionZ(motionNo, directionZType);
    }

    public CharaMotionNo ShiftMotionDirectionZ(CharaMotionNo motionNo, DirectionZType directionZType)
    {
        switch (motionNo)
        {
            case CharaMotionNo.DS:
            case CharaMotionNo.FLF:
            case CharaMotionNo.FLB:
            case CharaMotionNo.KG:
            case CharaMotionNo.KG2:
            case CharaMotionNo.DNF:
            case CharaMotionNo.DNB:
            case CharaMotionNo.SL:
            case CharaMotionNo.RTNSH:
            case CharaMotionNo.RTNJSH:
            case CharaMotionNo.PHF:
            case CharaMotionNo.PHB:
            case CharaMotionNo.DNHF:
            case CharaMotionNo.DNHB:
            case CharaMotionNo.ROF:
            case CharaMotionNo.ROB:
            case CharaMotionNo.PWDS:
            case CharaMotionNo.ANG:
            case CharaMotionNo.DRAW:
            case CharaMotionNo.WIN:
            case CharaMotionNo.LOSE:
                return motionNo;
            default:
                return directionZType switch
                {
                    DirectionZType.Neutral => motionNo + 1,
                    DirectionZType.Backward => motionNo + 2,
                    _ => motionNo
                };
        }
    }

    /// <summary>
    /// コマスタート
    /// </summary>
    public void StartKoma(BaseMotionKomaData komaData)
    {
        KomaFrameCount.Clear();
        IsActionPoint = komaData.IsActionPoint;

        switch (komaData.LoopSt)
        {
            case enBMLoopSt.St:
                LoopCount.Set(komaData.LoopNum);
                LoopStartKomaNo = KomaNo.Value;
                break;
            case enBMLoopSt.Ed:
                LoopStartKomaNo = KomaNo.Value;
                break;
            default:
                break;
        }
    }

    public void IncKomaNo()
    {
        KomaNo.Add();
    }

    public void BackToLoopStartKomaNo()
    {
        KomaNo.Set(LoopStartKomaNo);
    }
}

public interface ICharaMotionStateGetter
{
    CharaMotionType MotionType { get; }
    CharaMotionFlag MotionFlag { get; }
    int MotionCountValue { get; }
    CharaMotionNo MotionNo { get; }
    int KomaNoValue { get; }
    int KomaFrameCountValue { get; }
    int LoopCountValue { get; }
    int LoopStartKomaNo { get; }
    bool IsActionPoint { get; }

    bool HasFlag(CharaMotionFlag flag);
}

public interface ICharaMotionStateSetter
{
    UpCounter MotionCount { get; }
    UpCounter KomaFrameCount { get; }
    DownCounter LoopCount { get; }
    int LoopStartKomaNo { set; }
    bool IsActionPoint { set; }

    void Initialize();
    void SubMotionFlag(CharaMotionFlag targetFlag);
    void AddMotionFlag(CharaMotionFlag flag);
    void Progress();
    void SetMotionNo(CharaMotionType motionType, CharaMotionNo motionNo, CharaMotionFlag motionFlag, DirectionZType directionZType);
    void StartKoma(BaseMotionKomaData komaData);
    void IncKomaNo();
    void BackToLoopStartKomaNo();
}