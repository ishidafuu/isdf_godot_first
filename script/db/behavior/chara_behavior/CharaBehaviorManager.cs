namespace db;

/// <summary>
/// キャラクターの行動を管理するマネージャークラス
/// 全キャラクターのインスタンスを保持し、一括更新処理を提供します
/// </summary>
public class CharaBehaviorManager
{
    private static CharaBehaviorManager _instance;
    
    /// <summary>
    /// シングルトンインスタンスを取得します
    /// </summary>
    public static CharaBehaviorManager Instance => _instance ??= new CharaBehaviorManager();

    /// <summary>
    /// 全キャラクター（サイド別、Index順）を保持する配列
    /// </summary>
    private readonly CharaBehavior[][] _allCharas = new CharaBehavior[Defines.SideCount][];

    /// <summary>
    /// 指定されたサイドのキャラクター配列を取得します
    /// </summary>
    /// <param name="sideIndex">サイドインデックス</param>
    /// <returns>指定サイドのキャラクター配列</returns>
    public CharaBehavior[] GetSideCharas(int sideIndex)
    {
        return _allCharas[sideIndex];
    }

    /// <summary>
    /// 指定されたサイドとオーダーインデックスのキャラクターを取得します
    /// </summary>
    /// <param name="sideIndex">サイドインデックス</param>
    /// <param name="orderIndex">オーダーインデックス</param>
    /// <returns>指定されたキャラクター、存在しない場合はnull</returns>
    public CharaBehavior GetOrderChara(int sideIndex, int orderIndex)
    {
        return GetOrderChara(sideIndex, (OrderIndexType)orderIndex);
    }

    /// <summary>
    /// 指定されたサイドとオーダータイプのキャラクターを取得します
    /// </summary>
    /// <param name="sideIndex">サイドインデックス</param>
    /// <param name="orderIndex">オーダータイプ</param>
    /// <returns>指定されたキャラクター、存在しない場合はnull</returns>
    public CharaBehavior GetOrderChara(int sideIndex, OrderIndexType orderIndex)
    {
        foreach (var chara in _allCharas[sideIndex])
        {
            if (chara.MyOrderIndex == orderIndex)
            {
                return chara;
            }
        }

        return null;
    }

    /// <summary>
    /// CharaBehaviorManagerのコンストラクタ
    /// 全キャラクターの初期化を行います
    /// </summary>
    private CharaBehaviorManager()
    {
        SetupAll();
    }

    /// <summary>
    /// 全キャラクターのインスタンスを生成し初期化します
    /// </summary>
    private void SetupAll()
    {
        for (var side = 0; side < Defines.SideCount; side++)
        {
            for (var member = 0; member < Defines.MemberCount; member++)
            {
                _allCharas[side][member] = new CharaBehavior();
            }
        }
    }

    /// <summary>
    /// 全キャラクターの状態を更新します
    /// 各キャラクターのUpdateProgress処理を実行します
    /// </summary>
    public void UpdateProgress()
    {
        for (var side = 0; side < Defines.SideCount; side++)
        {
            for (var member = 0; member < Defines.MemberCount; member++)
            {
                _allCharas[side][member].UpdateProgress();
            }
        }
    }

    /// <summary>
    /// 全キャラクターの行動入力を更新します
    /// 各キャラクターのUpdateActionInput処理を実行します
    /// </summary>
    public void UpdateActionInput()
    {
        for (var side = 0; side < Defines.SideCount; side++)
        {
            for (var member = 0; member < Defines.MemberCount; member++)
            {
                _allCharas[side][member].UpdateActionInput();
            }
        }
    }

    /// <summary>
    /// 全キャラクターの移動処理を更新します
    /// 各キャラクターのUpdateMoving処理を実行します
    /// </summary>
    public void UpdateMoving()
    {
        for (var side = 0; side < Defines.SideCount; side++)
        {
            for (var member = 0; member < Defines.MemberCount; member++)
            {
                _allCharas[side][member].UpdateMoving();
            }
        }
    }
}
