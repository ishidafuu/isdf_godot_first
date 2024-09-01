using System;
using Microsoft.VisualBasic;

namespace db;

public class SettingData
{
    private static int GravityOffset => 0;
    private static int DamageOffset => GravityOffset + Enum.GetValues<endtSet_Grv>().Length;
    private static int JumpOffset => DamageOffset + Enum.GetValues<endtSet_Damage>().Length;
    private static int CourtOffset => JumpOffset + Enum.GetValues<endtSet_Jump>().Length;
    private static int BallOffset => CourtOffset + Enum.GetValues<endtSet_Court>().Length;
    private static int WallOffset => BallOffset + Enum.GetValues<endtSet_Ball>().Length;
    private static int InfieldOffset => WallOffset + Enum.GetValues<endtSet_Wall>().Length;
    private static int CatchOffset => InfieldOffset + Enum.GetValues<endtSet_Infield>().Length;
    private static int ShootOffset => CatchOffset + Enum.GetValues<endtSet_Catch>().Length;
    private static int PassOffset => ShootOffset + Enum.GetValues<endtSet_Shot>().Length;
    private static int FormationOffset => PassOffset + Enum.GetValues<endtSet_Pass>().Length;
    private static int ComOffset => FormationOffset + Enum.GetValues<endtSet_Formation>().Length;
    private static int InputOffset => ComOffset + Enum.GetValues<endtSet_COM>().Length;
    private static int AllSettingLength => InputOffset + Enum.GetValues<endtSet_Input>().Length;

    public int[] Sheet { get; private set; } = new int[AllSettingLength];

    public SettingData()
    {
    }

    public void Load()
    {
    }

    /// <summary>
    /// 重力
    /// </summary>
    public int GetGravity(endtSet_Grv value)
    {
        var index = GravityOffset + (int)value;
        return Sheet[index];
    }

    /// <summary>
    /// ダメージ
    /// </summary>
    public int GetDamage(endtSet_Damage value)
    {
        var index = DamageOffset + (int)value;
        return Sheet[index];
    }

    /// <summary>
    /// ジャンプ
    /// </summary>
    public int GetJump(endtSet_Jump value)
    {
        var index = JumpOffset + (int)value;
        return Sheet[index];
    }

    /// <summary>
    /// コート
    /// </summary>
    public int GetCourt(endtSet_Court value)
    {
        var index = CourtOffset + (int)value;
        return Sheet[index];
    }

    /// <summary>
    /// ボール
    /// </summary>
    public int GetBall(endtSet_Ball value)
    {
        var index = BallOffset + (int)value;
        return Sheet[index];
    }

    /// <summary>
    /// 壁
    /// </summary>
    public int GetWall(endtSet_Wall value)
    {
        var index = WallOffset + (int)value;
        return Sheet[index];
    }

    /// <summary>
    /// 内野
    /// </summary>
    public int GetInfield(endtSet_Infield value)
    {
        var index = InfieldOffset + (int)value;
        return Sheet[index];
    }

    /// <summary>
    /// キャッチ
    /// </summary>
    public int GetCatch(endtSet_Catch value)
    {
        var index = CatchOffset + (int)value;
        return Sheet[index];
    }

    /// <summary>
    /// シュート
    /// </summary>
    public int GetShoot(endtSet_Shot value)
    {
        var index = ShootOffset + (int)value;
        return Sheet[index];
    }

    /// <summary>
    /// パス
    /// </summary>
    public int GetPass(endtSet_Pass value)
    {
        var index = PassOffset + (int)value;
        return Sheet[index];
    }

    /// <summary>
    /// フォーメーション
    /// </summary>
    public int GetFormation(endtSet_Formation value)
    {
        var index = FormationOffset + (int)value;
        return Sheet[index];
    }

    /// <summary>
    /// COM思考
    /// </summary>
    public int GetCom(endtSet_COM value)
    {
        var index = ComOffset + (int)value;
        return Sheet[index];
    }

    /// <summary>
    /// 入力
    /// </summary>
    public int GetInput(endtSet_Input value)
    {
        var index = InputOffset + (int)value;
        return Sheet[index];
    }
}