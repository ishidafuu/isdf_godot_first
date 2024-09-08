using System;

namespace db;

public class SettingMaster
{
    private static int GravityOffset => 0;
    private static int DamageOffset => GravityOffset + Enum.GetValues<SettingGravityType>().Length;
    private static int JumpOffset => DamageOffset + Enum.GetValues<SettingDamageType>().Length;
    private static int CourtOffset => JumpOffset + Enum.GetValues<SettingJumpType>().Length;
    private static int BallOffset => CourtOffset + Enum.GetValues<SetingCourtType>().Length;
    private static int WallOffset => BallOffset + Enum.GetValues<SettingBallType>().Length;
    private static int InfieldOffset => WallOffset + Enum.GetValues<SettingWallType>().Length;
    private static int CatchOffset => InfieldOffset + Enum.GetValues<SettingInfieldType>().Length;
    private static int ShootOffset => CatchOffset + Enum.GetValues<SettingCatchType>().Length;
    private static int PassOffset => ShootOffset + Enum.GetValues<SettingShotType>().Length;
    private static int FormationOffset => PassOffset + Enum.GetValues<SettingPassType>().Length;
    private static int ComOffset => FormationOffset + Enum.GetValues<SettingFormationType>().Length;
    private static int InputOffset => ComOffset + Enum.GetValues<SettingComType>().Length;
    private static int AllSettingLength => InputOffset + Enum.GetValues<SettingInputType>().Length;

    private int[] Sheet { get; set; } = new int[AllSettingLength];

    public SettingMaster()
    {
    }

    public void Load()
    {
    }

    /// <summary>
    /// 重力
    /// </summary>
    public int GetGravity(SettingGravityType value)
    {
        var index = GravityOffset + (int)value;
        return Sheet[index];
    }

    /// <summary>
    /// ダメージ
    /// </summary>
    public int GetDamage(SettingDamageType value)
    {
        var index = DamageOffset + (int)value;
        return Sheet[index];
    }

    /// <summary>
    /// ジャンプ
    /// </summary>
    public int GetJump(SettingJumpType value)
    {
        var index = JumpOffset + (int)value;
        return Sheet[index];
    }

    /// <summary>
    /// コート
    /// </summary>
    public int GetCourt(SetingCourtType value)
    {
        var index = CourtOffset + (int)value;
        return Sheet[index];
    }

    /// <summary>
    /// ボール
    /// </summary>
    public int GetBall(SettingBallType value)
    {
        var index = BallOffset + (int)value;
        return Sheet[index];
    }

    /// <summary>
    /// 壁
    /// </summary>
    public int GetWall(SettingWallType value)
    {
        var index = WallOffset + (int)value;
        return Sheet[index];
    }

    /// <summary>
    /// 内野
    /// </summary>
    public int GetInfield(SettingInfieldType value)
    {
        var index = InfieldOffset + (int)value;
        return Sheet[index];
    }

    /// <summary>
    /// キャッチ
    /// </summary>
    public int GetCatch(SettingCatchType value)
    {
        var index = CatchOffset + (int)value;
        return Sheet[index];
    }

    /// <summary>
    /// シュート
    /// </summary>
    public int GetShoot(SettingShotType value)
    {
        var index = ShootOffset + (int)value;
        return Sheet[index];
    }

    /// <summary>
    /// パス
    /// </summary>
    public int GetPass(SettingPassType value)
    {
        var index = PassOffset + (int)value;
        return Sheet[index];
    }

    /// <summary>
    /// フォーメーション
    /// </summary>
    public int GetFormation(SettingFormationType value)
    {
        var index = FormationOffset + (int)value;
        return Sheet[index];
    }

    /// <summary>
    /// COM思考
    /// </summary>
    public int GetCom(SettingComType value)
    {
        var index = ComOffset + (int)value;
        return Sheet[index];
    }

    /// <summary>
    /// 入力
    /// </summary>
    public int GetInput(SettingInputType value)
    {
        var index = InputOffset + (int)value;
        return Sheet[index];
    }
}