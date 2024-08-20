using Godot;
using System;

public partial class MyNode2D : Node2D
{
    [Signal]
    public delegate void HealthDepletedEventHandler();


    private int _health = 10;

    public void TakeDamage(int amount)
    {
        _health -= amount;

        if (_health <= 0)
        {
            EmitSignal(SignalName.HealthDepleted);
        }
    }
}