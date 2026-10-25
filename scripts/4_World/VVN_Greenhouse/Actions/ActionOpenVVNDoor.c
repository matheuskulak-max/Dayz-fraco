class ActionOpenVVNDoor: ActionInteractBase
{
    void ActionOpenVVNDoor()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
        m_HUDCursorIcon = CursorIcons.OpenDoors;
    }

    override void CreateConditionComponents()
    {
        m_ConditionItem = new CCINone;
        m_ConditionTarget = new CCTNone;
    }

    override string GetText()
    {
        return "#open";
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (!target) return false;
        
        VVN_greenhouse_Base greenhouse = VVN_greenhouse_Base.Cast(target.GetObject());
        
        if (greenhouse)
        {
            string selection = greenhouse.GetActionComponentName(target.GetComponentIndex());
            
            if (greenhouse.CanOpenDoor() && selection == "door1")
                return true;
        }
        return false;
    }

    override void OnStartServer(ActionData action_data)
    {
        VVN_greenhouse_Base greenhouse = VVN_greenhouse_Base.Cast(action_data.m_Target.GetObject());
        
        if (greenhouse)
            greenhouse.OpenDoor();
    }
}