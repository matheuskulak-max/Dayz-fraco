#ifdef CodeLock
class VVN_Greenhouse_Codelock_Config : Managed
{   
    private float RaidTimeInSeconds;
    private string CanAttach;
    private string CanRaid;
    private string DeleteLockOnRaid;
    private int ToolDamageOnRaid;
    private ref array<string> RaidTools;

    void VVN_Greenhouse_Codelock_Config()
    {
        if (!GetGame().IsServer()) return;

        if (!FileExist(VVN_Greenhouse_Codelock_Const.ModProfileFolder))
        {
            Print("[VVN_Greenhouse_Codelock_Config] '" + VVN_Greenhouse_Codelock_Const.ModProfileFolder + "' does NOT exist, creating directory!");
            MakeDirectory(VVN_Greenhouse_Codelock_Const.ModProfileFolder);
        }

        if (!FileExist(VVN_Greenhouse_Codelock_Const.ConfigPath))
        {
            Print("[VVN_Greenhouse_Codelock_Config] '" + VVN_Greenhouse_Codelock_Const.ConfigPath + "' does NOT exist, creating default config!");
            Default();
            return; 
        }

        Load();
    }

    bool Load()
    {
        if (FileExist(VVN_Greenhouse_Codelock_Const.ConfigPath))
        {
            Print("[VVN_Greenhouse_Codelock_Config] '" + VVN_Greenhouse_Codelock_Const.ConfigPath + "' found, loading existing config!");
            JsonFileLoader<VVN_Greenhouse_Codelock_Config>.JsonLoadFile(VVN_Greenhouse_Codelock_Const.ConfigPath, this);
            return true;
        }
        return false;
    }

    protected void Save()
    {
        JsonFileLoader<VVN_Greenhouse_Codelock_Config>.JsonSaveFile(VVN_Greenhouse_Codelock_Const.ConfigPath, this);
    }

    protected void Default()
    {
        RaidTimeInSeconds = 60.0;
        CanAttach = "true";
        CanRaid = "false";
        DeleteLockOnRaid = "false";
        ToolDamageOnRaid = 100;
        RaidTools = new array<string>;
        RaidTools.Insert("Hacksaw");
        Save();
    }

    float Get_RaidTimeInSeconds()
    {
        return RaidTimeInSeconds;
    }

    bool CanAttach() 
    {
        return this.CanAttach == "true" || this.CanAttach == "TRUE" || this.CanAttach == "True";
    }

    bool CanRaid()
    {
        return this.CanRaid == "true" || this.CanRaid == "TRUE" || this.CanRaid == "True";
    }

    bool ShouldDeleteLockOnRaid()
    {
        return this.DeleteLockOnRaid == "true" || this.DeleteLockOnRaid == "TRUE" || this.DeleteLockOnRaid == "True";
    }

    int Get_ToolDamageOnRaid()
    {
        return ToolDamageOnRaid;
    }

    array<string> Get_RaidTools()
    {
        return RaidTools;
    }
};
#endif