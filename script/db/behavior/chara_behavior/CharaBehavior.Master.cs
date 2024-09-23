namespace db;

public partial class CharaBehavior
{
    /// <summary>
    /// SettingMaster重力
    /// </summary>
    private int GetSettingGravity(SettingGravityType value)
    {
        return MasterManager.Instance.SettingMaster.GetGravity(value);
    }

    /// <summary>
    /// SettingMasterダメージ
    /// </summary>
    private int GetSettingDamage(SettingDamageType value)
    {
        return MasterManager.Instance.SettingMaster.GetDamage(value);
    }

    /// <summary>
    /// SettingMasterジャンプ
    /// </summary>
    private int GetSettingJump(SettingJumpType value)
    {
        return MasterManager.Instance.SettingMaster.GetJump(value);
    }

    /// <summary>
    /// SettingMasterコート
    /// </summary>
    private int GetSettingCourt(SetingCourtType value)
    {
        return MasterManager.Instance.SettingMaster.GetCourt(value);
    }

    /// <summary>
    /// SettingMasterボール
    /// </summary>
    private int GetSettingBall(SettingBallType value)
    {
        return MasterManager.Instance.SettingMaster.GetBall(value);
    }

    /// <summary>
    /// SettingMaster壁
    /// </summary>
    private int GetSettingWall(SettingWallType value)
    {
        return MasterManager.Instance.SettingMaster.GetWall(value);
    }

    /// <summary>
    /// SettingMaster内野
    /// </summary>
    private int GetSettingInfield(SettingInfieldType value)
    {
        return MasterManager.Instance.SettingMaster.GetInfield(value);
    }

    /// <summary>
    /// SettingMasterキャッチ
    /// </summary>
    private int GetSettingCatch(SettingCatchType value)
    {
        return MasterManager.Instance.SettingMaster.GetCatch(value);
    }

    /// <summary>
    /// SettingMasterシュート
    /// </summary>
    private int GetSettingShoot(SettingShotType value)
    {
        return MasterManager.Instance.SettingMaster.GetShoot(value);
    }

    /// <summary>
    /// SettingMasterパス
    /// </summary>
    private int GetSettingPass(SettingPassType value)
    {
        return MasterManager.Instance.SettingMaster.GetPass(value);
    }

    /// <summary>
    /// SettingMasterフォーメーション
    /// </summary>
    private int GetSettingFormation(SettingFormationType value)
    {
        return MasterManager.Instance.SettingMaster.GetFormation(value);
    }

    /// <summary>
    /// SettingMasterCOM思考
    /// </summary>
    private int GetSettingComPlan(SettingComType value)
    {
        return MasterManager.Instance.SettingMaster.GetCom(value);
    }

    /// <summary>
    /// SettingMaster入力
    /// </summary>
    private int GetSettingInput(SettingInputType value)
    {
        return MasterManager.Instance.SettingMaster.GetInput(value);
    }
}