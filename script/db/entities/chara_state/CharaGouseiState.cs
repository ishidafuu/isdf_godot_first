namespace db;

/// <summary>
/// 合成シュートに関するステータス
/// </summary>
public class CharaGouseiState : ICharaGouseiState
{
    public ShotGouseiState[] GouseiShots { get; set; } = new ShotGouseiState[Defines.StepNum];
    public ShotGouseiState VJumpGouseiShots { get; set; }

    public CharaGouseiState()
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

public interface ICharaGouseiState
{
    ShotGouseiState[] GouseiShots { get; }
    ShotGouseiState VJumpGouseiShots { get; }
}