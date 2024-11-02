using System.Collections.Generic;
using isdf;

namespace db;

public class PadBehaviorManager : BaseBehavior
{
    private static PadBehaviorManager _instance;
    public static PadBehaviorManager Instance => _instance ??= new PadBehaviorManager();

    /// <summary>
    /// 全チーム（Index順）
    /// </summary>
    private readonly List<PadBehavior> _allPads = new();

    public IReadOnlyCollection<PadBehavior> GetAllPads() => _allPads;

    public PadBehavior Get(int padIndex)
    {
        return padIndex < _allPads.Count ? _allPads[padIndex] : null;
    }

    private PadBehaviorManager()
    {
        for (var i = 0; i < Defines.SideCount; i++)
        {
            _allPads.Add(new PadBehavior(_allPads.Count));
        }
    }

    public void AttachPad(IPad pad)
    {
        foreach (var padBehavior in _allPads)
        {
            if (padBehavior.IsDummy == false)
            {
                continue;
            }

            padBehavior.SetPad(pad);
            return;
        }

        _allPads.Add(new PadBehavior(_allPads.Count, pad));
    }
}