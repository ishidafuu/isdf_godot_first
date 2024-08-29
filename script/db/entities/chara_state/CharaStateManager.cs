namespace db;

public class CharaStateManager
{
    private static CharaStateManager _instance;
    public static CharaStateManager Instance => _instance ??= new CharaStateManager();

    /// <summary>
    /// 全キャラ（Index順）
    /// </summary>
    private readonly CharaState[] _allCharas = new CharaState[Defines.AllCharaCount];

    public CharaState[] GetAllCharas() => _allCharas;

    /// <summary>
    /// 各サイドのキャラ（Index順）
    /// </summary>
    private readonly CharaState[][] _sideCharas = new CharaState[Defines.SideCount][];

    /// <summary>
    /// 各サイドのキャラ（オーダー順）
    /// </summary>
    private readonly CharaState[][] _sideOrders = new CharaState[Defines.SideCount][];

    /// <summary>
    /// 各サイドの内野キャラ（オーダー順）
    /// </summary>
    private readonly CharaState[][] _infieldOrders = new CharaState[Defines.SideCount][];

    /// <summary>
    /// オーダー変更あり
    /// </summary>
    private bool _isOrderDirty = false;

    public void SetOrderDirty() => _isOrderDirty = true;

    public CharaState[] GetSideCharas(int side) => _sideCharas[side];

    public CharaState[] GetSideOrders(int side)
    {
        if (_isOrderDirty)
        {
            UpdateOrders();
        }

        return _sideOrders[side];
    }

    public CharaState[] GetSideInfield(int side)
    {
        if (_isOrderDirty)
        {
            UpdateOrders();
        }

        return _infieldOrders[side];
    }

    public CharaState Get(int side, int index) => _sideCharas[side][index];

    private CharaStateManager()
    {
        SetupAll();
        SetupSide();
        UpdateOrders();
    }

    private void SetupAll()
    {
        for (var i = 0; i < _allCharas.Length; i++)
        {
            long sideIndex = i / Defines.MemberCount;
            long memberIndex = i % Defines.MemberCount;
            _allCharas[i] = new CharaState(sideIndex, memberIndex);
        }
    }

    private void SetupSide()
    {
        for (var s = 0; s < Defines.SideCount; s++)
        {
            _sideCharas[s] = new CharaState[Defines.MemberCount];
            for (var m = 0; m < Defines.MemberCount; m++)
            {
                _sideCharas[s][m] = _allCharas[s * Defines.MemberCount + m];
            }
        }
    }

    private void UpdateOrders()
    {
        for (var s = 0; s < Defines.SideCount; s++)
        {
            for (var m = 0; m < Defines.MemberCount; m++)
            {
                var member = _sideCharas[s][m];
                var positionIndex = member.Position.PositionIndex;
                if (positionIndex < Defines.InfieldCount)
                {
                    _infieldOrders[s][positionIndex] = member;
                }

                _sideOrders[s][m] = member;
            }
        }

        _isOrderDirty = false;
    }
}