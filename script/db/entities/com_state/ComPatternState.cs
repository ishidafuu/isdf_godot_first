﻿using System;

namespace db;

/// <summary>
/// COM思考パターンに関するステータス
/// </summary>
public class ComPatternState : IComPatternStateGetter, IComPatternStateSetter
{
    public int[] PlanPattern { get; set; } = new int[ComPlanData.PlanTypeLength];
    public int[][] ActionPattern { get; set; } = new int[Defines.MemberCount][];

    public ActionGroupType GetActionPattern(OrderIndexType orderIndexType, ComActionType comActionType)
    {
        return (ActionGroupType)ActionPattern[(int)orderIndexType][(int)comActionType];
    }

    public ComPatternState()
    {
        for (var i = 0; i < Defines.MemberCount; i++)
        {
            ActionPattern[i] = new int[ComActionData.ActionTypeLength];
        }
    }

    public void Initialize()
    {
        Array.Clear(PlanPattern, 0, PlanPattern.Length);

        for (var i = 0; i < ComActionData.ActionTypeLength; i++)
        {
            Array.Clear(ActionPattern[i], 0, ActionPattern[i].Length);
        }
    }
}

public interface IComPatternStateGetter
{
    int[] PlanPattern { get; }
    int[][] ActionPattern { get; }
    ActionGroupType GetActionPattern(OrderIndexType orderIndexType, ComActionType comActionType);
}

public interface IComPatternStateSetter
{
    int[] PlanPattern { set; }
    int[][] ActionPattern { set; }
    void Initialize();
}