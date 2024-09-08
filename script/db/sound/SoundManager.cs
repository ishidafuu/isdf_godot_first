namespace db;

public class SoundManager
{
    private static SoundManager _instance;
    public static SoundManager Instance => _instance ??= new SoundManager();

    public void PlayBgm(string bgmName)
    {
        // BGM再生処理
    }

    public void StopBgm()
    {
        // BGM停止処理
    }

    public void PlaySe(SeType seType)
    {
        // SE再生処理
    }

    public void PlaySysSe(SystemSeType systemSeType)
    {
        // SE再生処理
    }

    public void StopAll()
    {
        // 全ての音声停止処理
    }
}