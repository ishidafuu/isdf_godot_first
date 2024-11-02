namespace db;

public struct Rect : IRectGetter, IRectSetter
{
    public int X { get; set; }
    public int Y { get; set; }
    public int Width { get; set; }
    public int Height { get; set; }

    public Rect(int x, int y, int width, int height)
    {
        X = x;
        Y = y;
        Width = width;
        Height = height;
    }

    public void Initialize()
    {
        Set(0, 0, 0, 0);
    }

    public void Set(int x, int y, int width, int height)
    {
        X = x;
        Y = y;
        Width = width;
        Height = height;
    }

    public bool IsPile(Rect targetRect)
    {
        return X <= targetRect.X + targetRect.Width
               && targetRect.X <= X + Width
               && Y <= targetRect.Y + targetRect.Height
               && targetRect.Y <= Y + Height;
    }

    public bool IsPile(int x, int y)
    {
        return X <= x
               && x <= X + Width
               && Y <= y
               && y <= Y + Height;
    }
}

public interface IRectGetter
{
    int X { get; }
    int Y { get; }
    int Width { get; }
    int Height { get; }
    bool IsPile(Rect targetRect);
    bool IsPile(int x, int y);
}

public interface IRectSetter
{
    void Set(int x, int y, int width, int height);
    void Initialize();
}