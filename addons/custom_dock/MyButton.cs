using Godot;

[Tool]
public partial class MyButton : Button
{
    public override void _EnterTree()
    {
        Pressed += Clicked;
    }

    public void Clicked()
    {
        GD.Print("Yes!");
    }
}