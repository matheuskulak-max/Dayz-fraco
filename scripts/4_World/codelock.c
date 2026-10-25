#ifdef CodeLock
modded class CodeLock : ItemBase
{
    override void NewUnlockServer(EntityAI player, EntityAI parent)
    {
        if (!GetGame().IsServer() || !GetGame().IsMultiplayer())
            return;
            
        VVN_greenhouse_Base greenhouse = VVN_greenhouse_Base.Cast(parent);
        if (greenhouse)
        {
            greenhouse.OpenDoor();
            
            player.ServerDropEntity(this);
            SetPosition(player.GetPosition());
        }

        super.NewUnlockServer(player, parent);
    }

    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionAttachCodeLock);
    }
};
#endif