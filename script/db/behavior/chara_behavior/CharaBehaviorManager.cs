namespace db;

public class CharaBehaviorManager
{
    private static CharaBehaviorManager _instance;
    public static CharaBehaviorManager Instance => _instance ??= new CharaBehaviorManager();

    /// <summary>
    /// 全キャラ（Index順）
    /// </summary>
    private readonly CharaBehavior[] _allCharas = new CharaBehavior[Defines.AllCharaCount];

    public CharaBehavior[] GetAllCharas() => _allCharas;

    public CharaBehavior GetChara(int sideIndex, int orderIndex)
    {
        return GetChara(sideIndex, (OrderIndexType)orderIndex);
    }

    public CharaBehavior GetChara(int sideIndex, OrderIndexType orderIndex)
    {
        foreach (var chara in _allCharas)
        {
            if (chara.MySideIndex == sideIndex
                && chara.MyOrderIndex == orderIndex)
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
        for (var i = 0; i < _allCharas.Length; i++)
        {
            _allCharas[i] = new CharaBehavior();
        }
    }

    public void UpdateProgress()
    {
        foreach (var chara in _allCharas)
        {
            chara.UpdateProgress();
        }
    }

    public void UpdateActionInput()
    {
        foreach (var chara in _allCharas)
        {
            chara.UpdateActionInput();
        }
    }
}