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
}