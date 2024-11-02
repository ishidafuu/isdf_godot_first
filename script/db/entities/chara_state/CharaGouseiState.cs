namespace db;

/// <summary>
/// 合成シュートに関するステータス
/// </summary>
public class CharaGouseiStateGetter : ICharaGouseiStateGetter, ICharaGouseiStateSetter
{
    public ShotGouseiState[] GouseiShots { get; set; } = new ShotGouseiState[Defines.StepNum];
    public ShotGouseiState VJumpGouseiShots { get; set; }

    public CharaGouseiStateGetter()
    {
        for (var i = 0; i < Defines.StepNum; i++)
        {
            GouseiShots[i] = new ShotGouseiState();
        }

        VJumpGouseiShots = new ShotGouseiState();
    }

    public void Initialize()
    {
        foreach (var shot in GouseiShots)
        {
            shot.Initialize();
        }

        VJumpGouseiShots.Initialize();
    }
}

public interface ICharaGouseiStateGetter
{
    ShotGouseiState[] GouseiShots { get; }
    ShotGouseiState VJumpGouseiShots { get; }
}

public interface ICharaGouseiStateSetter
{
    ShotGouseiState[] GouseiShots { set; }
    ShotGouseiState VJumpGouseiShots { set; }

    void Initialize();
}