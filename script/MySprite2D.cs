using Godot;
using System;

public partial class MySprite2D : Sprite2D
{
    [Export] private Button _button;

    [Export] private InputNode _inputNode;
    private float _speed = 400;
    private float _angularSpeed = Mathf.Pi;

    public override void _Ready()
    {
        // var timer = GetNode<Timer>("Timer");
        // timer.Timeout += OnTimerTimeout;
    }

    public override void _Process(double delta)
    {
        var direction = 0;
       
        if ( _inputNode.KeyUp.IsPressed)
        {
            direction = -1;
        }

        // if (Input.IsActionPressed("ui_right"))
        // {
        //     direction = 1;
        // }
        //
        // if (Input.IsActionPressed("ButtonA"))
        // {
        //     direction = -1;
        // }
        //
        // if (Input.IsActionPressed("ButtonB"))
        // {
        //     direction = 1;
        // }

        Rotation += _angularSpeed * direction * (float)delta;
        var velocity = Vector2.Up.Rotated(Rotation) * _speed;
        // Position += velocity * (float)delta;
    }

    // We also specified this function name in PascalCase in the editor's connection window.
    private void OnButtonPressed()
    {
        SetProcess(!IsProcessing());
        GD.Print("Processing: " + IsProcessing());
    }

    private void OnButtonDown()
    {
        OnButtonPressed();
    }

    private void OnTimerTimeout()
    {
        Visible = !Visible;
    }
}