namespace db;

public partial class BaseBehavior
{
    /// <summary>
    /// SettingMaster重力
    /// </summary>
    protected int GetSettingGravity(SettingGravityType value)
    {
        return MasterManager.Instance.SettingMaster.GetGravity(value);
    }

    /// <summary>
    /// SettingMasterダメージ
    /// </summary>
    protected int GetSettingDamage(SettingDamageType value)
    {
        return MasterManager.Instance.SettingMaster.GetDamage(value);
    }

    /// <summary>
    /// SettingMasterジャンプ
    /// </summary>
    protected int GetSettingJump(SettingJumpType value)
    {
        return MasterManager.Instance.SettingMaster.GetJump(value);
    }

    /// <summary>
    /// SettingMasterコート
    /// </summary>
    protected int GetSettingCourt(SetingCourtType value)
    {
        return MasterManager.Instance.SettingMaster.GetCourt(value);
    }

    /// <summary>
    /// SettingMasterボール
    /// </summary>
    protected int GetSettingBall(SettingBallType value)
    {
        return MasterManager.Instance.SettingMaster.GetBall(value);
    }

    /// <summary>
    /// SettingMaster壁
    /// </summary>
    protected int GetSettingWall(SettingWallType value)
    {
        return MasterManager.Instance.SettingMaster.GetWall(value);
    }

    /// <summary>
    /// SettingMaster内野
    /// </summary>
    protected int GetSettingInfield(SettingInfieldType value)
    {
        return MasterManager.Instance.SettingMaster.GetInfield(value);
    }

    /// <summary>
    /// SettingMasterキャッチ
    /// </summary>
    protected int GetSettingCatch(SettingCatchType value)
    {
        return MasterManager.Instance.SettingMaster.GetCatch(value);
    }

    /// <summary>
    /// SettingMasterシュート
    /// </summary>
    protected int GetSettingShoot(SettingShotType value)
    {
        return MasterManager.Instance.SettingMaster.GetShoot(value);
    }

    /// <summary>
    /// SettingMasterパス
    /// </summary>
    protected int GetSettingPass(SettingPassType value)
    {
        return MasterManager.Instance.SettingMaster.GetPass(value);
    }

    /// <summary>
    /// SettingMasterフォーメーション
    /// </summary>
    protected int GetSettingFormation(SettingFormationType value)
    {
        return MasterManager.Instance.SettingMaster.GetFormation(value);
    }

    /// <summary>
    /// SettingMasterCOM思考
    /// </summary>
    protected int GetSettingComPlan(SettingComType value)
    {
        return MasterManager.Instance.SettingMaster.GetCom(value);
    }

    /// <summary>
    /// SettingMaster入力
    /// </summary>
    protected int GetSettingInput(SettingInputType value)
    {
        return MasterManager.Instance.SettingMaster.GetInput(value);
    }
}