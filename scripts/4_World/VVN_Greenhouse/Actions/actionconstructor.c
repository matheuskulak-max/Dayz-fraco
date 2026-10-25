modded class ActionConstructor 
{
    override void RegisterActions(TTypenameArray actions)
    {
        super.RegisterActions(actions);

        actions.Insert(ActionOpenVVNDoor);
        actions.Insert(ActionCloseVVNDoor);
        
        #ifdef CodeLock
        actions.Insert(ActionAttachCodeLock);
        actions.Insert(ActionInteractLock);
        actions.Insert(ActionManageLock);
        actions.Insert(ActionLockAdmin);
        #endif
    };
};