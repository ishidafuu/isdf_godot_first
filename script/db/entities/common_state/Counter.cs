using System;

namespace db;

public struct Counter
{
    public int Value { get; private set; }

    public Counter(int value)
    {
        Value = value;
    }

    public void Set(int value)
    {
        Value = value;
    }

    public void Add(int step = 1)
    {
        Value += step;
    }

    public bool AddUntil(int limit, int step = 1)
    {
        if (Value >= limit)
        {
            Value = limit;
            return false;
        }

        Value = Math.Min(limit, Value + step);
        return Value == limit;
    }

    public bool Sub(int step = 1)
    {
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