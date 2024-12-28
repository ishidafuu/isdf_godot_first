using System;
using isdf;

namespace db;

public partial class CharaBehavior
{
    private DirectionXType GetMoveMuki(bool Input_f)
    {
        if (Composite.IsSelfControl == false)
        {
            return Auto.DirectionX;
        }
        else if (Input_f || (Auto.AutoType == AutoType.Free))
        {
            if (Pad.IsWalk2(DirectionCrossType.Left))//歩き入力
            {
                return DirectionXType.Left;
            }
            else if (Pad.IsWalk2(DirectionCrossType.Right))//歩き入力
            {
                return DirectionXType.Right;
            }
            else
            {
                return DirectionXType.Neutral;
            }
        }
        else
        {
            return Auto.DirectionX;
        }
    }

    private DirectionZType GetMoveMukiZ(bool isInput)
    {
        if (Composite.IsSelfControl == false)
        {
            return Auto.DirectionZ;
        }
        else if (isInput || (Auto.AutoType == AutoType.Free))
        {
            if (Pad.IsWalk2(DirectionCrossType.Up))//歩き入力
            {
                return DirectionZType.Backward;
            }
            else if (Pad.IsWalk2(DirectionCrossType.Down))//歩き入力
            {
                return DirectionZType.Forward;
            }
            else
            {
                return DirectionZType.Neutral;
            }
        }
        else
        {
            return Auto.DirectionZ;
        }
    }

    private DirectionZType GetMoveMukiZMukiLock(bool Input_f)
    {
        if (Composite.IsSelfControl == false)
        {
            return Auto.DirectionZ;
        }
        else if (Input_f || (Auto.AutoType == AutoType.Free))
        {
            if (Pad.IsWalk2(DirectionCrossType.Down))
            {
                return DirectionZType.Backward;
            }
            else if (Pad.IsWalk2(DirectionCrossType.Up))
            {
                return DirectionZType.Forward;
            }
            else
            {
                return DirectionZType.Neutral;
            }
        }
        else
        {
            return Auto.DirectionZ;
        }
    }
}
