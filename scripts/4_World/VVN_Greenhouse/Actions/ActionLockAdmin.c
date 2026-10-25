#ifdef CodeLock
class ActionLockAdmin : ActionInteractBase
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
        if (!target || !player)
            return false;
            
        VVN_greenhouse_Base greenhouse = VVN_greenhouse_Base.Cast(target.GetObject());
        if (!greenhouse)
            return false;
            
        // Check that door is closed
        if (greenhouse.IsDoorOpen())
            return false;
            
        CodeLock codelock;
        string selection = greenhouse.GetActionComponentName(target.GetComponentIndex());

        if (selection != "door1")
            return false;
            
        codelock = greenhouse.GetCodeLockRS();

        if (codelock && codelock.GetLockState()) 
        {
            PlayerIdentity identity = player.GetIdentity();
            if (identity && GetCLPermissionManager().UserIsAdmin(identity.GetPlainId())) 
            {
                lockState = "Admin Lock";
                return true;
            }
        }
        return false;
    }

    override void OnStartClient(ActionData action_data) 
    {
        VVN_greenhouse_Base greenhouse = VVN_greenhouse_Base.Cast(action_data.m_Target.GetObject());
        CodeLock codelock;

        if (greenhouse) 
        {
            codelock = greenhouse.GetCodeLockRS();

            if (codelock && codelock.GetLockState()) 
            {
                if (GetCLPermissionManager().UserIsAdmin()) 
                {
                    GetCLUIManager().ShowMenu(CLMENU.ADMIN_CODE_MENU, greenhouse);
                }
            }
        }
    }
};
#endif