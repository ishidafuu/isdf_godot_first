using System;

namespace db;

public struct DownCounter
{
    public int Value { get; private set; }

    public DownCounter(int value)
    {
        Value = value;
    }

    public void Set(int value)
    {
        Value = value;
    }

    public bool Sub(int step = 1)
    {
        // -1は無限ループ
        if (Value <= 0)
        {
            Value = 0;
            return false;
        }

        Value = Math.Max(0, Value - step);
        return Value == 0;
    }

    public void Clear()
    {
        Value = 0;
    }
}