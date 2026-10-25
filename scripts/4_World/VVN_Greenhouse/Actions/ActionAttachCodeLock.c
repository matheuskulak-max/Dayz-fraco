#ifdef CodeLock
class ActionAttachCodeLock : ActionSingleUseBase
{
    override void CreateConditionComponents() 
    {
        m_ConditionItem = new CCINonRuined;
        m_ConditionTarget = new CCTNone;
    }

    override string GetText() 
    {
        return "Attach Codelock";
    }
    
    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item) 
    {
        if (!target)
            return false;
        
        VVN_greenhouse_Base greenhouse = VVN_greenhouse_Base.Cast(target.GetObject());
        
        if (greenhouse)
        {
            string selection = greenhouse.GetActionComponentName(target.GetComponentIndex());
            
            int slot = InventorySlots.GetSlotIdFromString("Att_CombinationLock");
            if (slot == InventorySlots.INVALID)
                return false;
            if (!greenhouse.IsCodeLocked() && selection == "door1")
                return true;
        }
        return false;
    }
    
    override void OnExecuteClient(ActionData action_data) 
    {
        VVN_greenhouse_Base greenhouse = VVN_greenhouse_Base.Cast(action_data.m_Target.GetObject());
        PlayerBase player = action_data.m_Player;

        ClearInventoryReservationEx(action_data);

        if (greenhouse) 
        {
            int slotID = InventorySlots.GetSlotIdFromString("Att_CombinationLock");

            action_data.m_Player.PredictiveTakeEntityToTargetAttachmentEx(greenhouse, action_data.m_MainItem, slotID);
            GetCLUIManager().ShowMenu(CLMENU.SET_CODE_MENU, greenhouse);
        }
    }
    
    override void OnExecuteServer(ActionData action_data) 
    {
        VVN_greenhouse_Base greenhouse = VVN_greenhouse_Base.Cast(action_data.m_Target.GetObject());

        // 1.30: release the action juncture on the target so the client's attachment move is accepted
        if (GetGame().IsMultiplayer())
            ClearActionJuncture(action_data);

        if (greenhouse) 
        {
            GetCodeLockLogger().WriteLog("attach", action_data.m_Player.GetIdentity(), greenhouse.GetPosition());
        }
    }
};
#endif