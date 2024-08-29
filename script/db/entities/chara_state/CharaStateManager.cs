using System.Linq;

namespace db;

public class CharaStateManager
{
    private static CharaStateManager _instance;
    public static CharaStateManager Instance => _instance ??= new CharaStateManager();

    private readonly CharaState[] _getAll = new CharaState[Defines.AllCharaCount];
    public CharaState[] GetAll() => _getAll;

    private readonly CharaState[][] _sideMembers = new CharaState[Defines.SideCount][];
    public CharaState[] GetSide(int side) => _sideMembers[side];

    public CharaState Get(int side, int index) => _sideMembers[side][index];

    private CharaStateManager()
    {
        for (var i = 0; i < _getAll.Length; i++)
        {
            _getAll[i] = new CharaState();
        }

        for (var side = 0; side < Defines.SideCount; side++)
        {
            _sideMembers[side] = new CharaState[Defines.MemberCount];
            for (var chara = 0; chara < Defines.MemberCount; chara++)
            {
                _sideMembers[side][chara] = _getAll[side * Defines.MemberCount + chara];
            }
        }
    }

}