namespace db;

public class CharaBehaviorManager
{
    private static CharaBehaviorManager _instance;
    public static CharaBehaviorManager Instance => _instance ??= new CharaBehaviorManager();

    /// <summary>
    /// 全キャラ（Index順）
    /// </summary>
    private readonly CharaBehavior[][] _allCharas = new CharaBehavior[Defines.SideCount][];

    public CharaBehavior[] GetSideCharas(int sideIndex)
    {
        return _allCharas[sideIndex];
    }

    public CharaBehavior GetOrderChara(int sideIndex, int orderIndex)
    {
        return GetOrderChara(sideIndex, (OrderIndexType)orderIndex);
    }

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

    private CharaBehaviorManager()
    {
        SetupAll();
    }

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
