namespace db;

public partial class CharaBehavior
{
    public void UpdateProgress()
    {
        MyState.Motion.IncrementMotionCount();
        MyState.Damage.DecrementFumbleCount();
        

    }
}