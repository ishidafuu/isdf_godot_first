using System;

namespace db;

public struct UpCounter
{
    public int Value { get; private set; }

    public UpCounter(int value)
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

    public void Clear()
    {
        Value = 0;
    }
}