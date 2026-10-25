#ifdef CodeLock
class ActionManageLock : ActionInteractBase
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

    override typename GetInputType() 
    {
        return ContinuousInteractActionInput;
    }

    override bool HasProgress() 
    {
        return false;
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item) 
    {
        if (!target || !player)
            return false;
        
        VVN_greenhouse_Base greenhouse = VVN_greenhouse_Base.Cast(target.GetObject());
        CodeLock codelock;

        if (greenhouse && !greenhouse.IsDoorOpen())
        {
            string selection = greenhouse.GetActionComponentName(target.GetComponentIndex());
            
            if (selection != "door1") 
            {
                return false;
            }
            
            codelock = greenhouse.GetCodeLockRS();

            if (codelock && codelock.GetLockState()) 
            {
                if (player && !GetDayZGame().CLIsActionRateLimited()) 
                {
                    auto params = new Param1<CodeLock>(codelock);
                    g_Game.RPCSingleParam(player, CLRPC.RPC_SERVER_SYNCHLOCK, params, true);
                    GetDayZGame().SetCLActionRateLimit();
                }
                if (codelock.IsOwner(player.GetIdentity().GetId())) 
                {
                    lockState = "Manage Lock";
                    return true;
                } 
                else if (codelock.HasNoOwner()) 
                {
                    lockState = "Claim Ownership";
                    return true;
                }
            }
        }
        return false;
    }

    override bool CanContinue(ActionData action_data) 
    {
        return true;
    }

    override void OnStartClient(ActionData action_data) 
    {
        PlayerBase player = action_data.m_Player;
        if (!player) return;
        
        VVN_greenhouse_Base greenhouse = VVN_greenhouse_Base.Cast(action_data.m_Target.GetObject());
        CodeLock codelock;

        if (greenhouse) 
        {
            codelock = greenhouse.GetCodeLockRS();

            if (codelock && codelock.GetLockState()) 
            {
                if (codelock.IsOwner(player.GetIdentity().GetId())) 
                {
                    GetCLUIManager().ShowMenu(CLMENU.ENTER_CODE_MENU, greenhouse);
                } 
                else if (codelock.HasNoOwner()) 
                {
                    GetCLUIManager().ShowMenu(CLMENU.CLAIM_CODE_MENU, greenhouse);
                }
            }
        }
    }

    override void CreateAndSetupActionCallback(ActionData action_data) 
    {
        super.CreateAndSetupActionCallback(action_data);
    }
};
#endif