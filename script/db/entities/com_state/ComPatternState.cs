using System;

namespace db;

/// <summary>
/// COM思考パターンに関するステータス
/// </summary>
public class ComPatternState
{
    public int[] PlanPattern { get; set; } = new int[ComPlanData.PlanTypeLength];
    public int[][] ActionPattern { get; set; } = new int[ComActionData.ActionTypeLength][];

    public ComPatternState()
    {
        for (var i = 0; i < ComActionData.ActionTypeLength; i++)
        {
            ActionPattern[i] = new int[Defines.MemberCount];
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