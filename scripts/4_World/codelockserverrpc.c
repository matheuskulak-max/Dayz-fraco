#ifdef CodeLock
modded class CodeLockServerRPC : PluginBase
{
    override void OpenTarget(ItemBase target)
    {
        VVN_greenhouse_Base greenhouse = VVN_greenhouse_Base.Cast(target);
        
        if (greenhouse && !greenhouse.IsOpen())
        {
            greenhouse.OpenDoor();
        }
        
        super.OpenTarget(target);
    }
};
#endif