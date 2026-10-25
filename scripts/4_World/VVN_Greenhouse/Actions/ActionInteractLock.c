#ifdef CodeLock
class ActionInteractLock : ActionInteractBase
{
    protected string lockState;

    override void CreateConditionComponents()
    {
        m_ConditionItem = new CCINone;
        m_ConditionTarget = new CCTNone;
    }

    override string GetText()
    {
        return lockState;
    }

    override bool IsInstant()
    {
        return true;
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (!target || !player) return false;
        
        VVN_greenhouse_Base greenhouse = VVN_greenhouse_Base.Cast(target.GetObject());
        if (!greenhouse) return false;
        
        // Only show when door is closed
        if (greenhouse.IsDoorOpen()) return false;
        
        CodeLock codelock;
        string selection = greenhouse.GetActionComponentName(target.GetComponentIndex());

        if (selection != "door1") return false;
            
        codelock = greenhouse.GetCodeLockRS();

        if (codelock)
        {
            PlayerIdentity identity = player.GetIdentity();
            if (!identity) return false;
            
            // Synchronize lock state
            if (player && !GetDayZGame().CLIsActionRateLimited()) 
            {
                auto params = new Param1<CodeLock>(codelock);
                GetGame().RPCSingleParam(player, CLRPC.RPC_SERVER_SYNCHLOCK, params, true);
                GetDayZGame().SetCLActionRateLimit();
            }
            
            if (codelock.GetLockState()) 
            {
                if (codelock.IsOwner(identity.GetId()) || codelock.IsGuest(identity.GetId())) 
                {
                    lockState = "Open";
                } 
                else if (!codelock.HasNoOwner()) 
                {
                    lockState = "Enter Passcode";
                } 
                else 
                {
                    return false;
                }
            } 
            else 
            {
                lockState = "Set Passcode";
            }
            return true;
        }
        return false;
    }

    override void OnStartClient(ActionData action_data)
    {
        PlayerBase player = action_data.m_Player;
        if (!player) return;
        
        PlayerIdentity identity = player.GetIdentity();
        if (!identity) return;
        
        VVN_greenhouse_Base greenhouse = VVN_greenhouse_Base.Cast(action_data.m_Target.GetObject());
        CodeLock codelock;

        if (greenhouse) 
        {
            codelock = greenhouse.GetCodeLockRS();

            if (codelock) 
            {
                if (codelock.GetLockState()) 
                {
                    if (!codelock.IsOwner(identity.GetId()) && !codelock.IsGuest(identity.GetId())) 
                    {
                        GetCLUIManager().ShowMenu(CLMENU.GUEST_CODE_MENU, greenhouse);
                    }
                } 
                else 
                {
                    GetCLUIManager().ShowMenu(CLMENU.SET_CODE_MENU, greenhouse);
                }
            }
        }
    }

    override void OnStartServer(ActionData action_data)
    {
        PlayerBase player = action_data.m_Player;
        if (!player) return;
        
        PlayerIdentity identity = player.GetIdentity();
        if (!identity) return;
        
        VVN_greenhouse_Base greenhouse = VVN_greenhouse_Base.Cast(action_data.m_Target.GetObject());
        CodeLock codelock;

        if (greenhouse) 
        {
            codelock = greenhouse.GetCodeLockRS();

            if (codelock) 
            {
                if (codelock.GetLockState()) 
                {
                    if (codelock.IsOwner(identity.GetId()) || codelock.IsGuest(identity.GetId())) 
                    {
                        GetCodeLockLogger().WriteLog("acceSS", identity, codelock.GetPosition());
                        greenhouse.OpenDoor();
                    }
                }
            }
        }
    }
};
#endif