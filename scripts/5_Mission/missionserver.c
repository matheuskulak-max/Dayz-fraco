#ifdef CodeLock
modded class MissionServer 
{
    private ref VVN_Greenhouse_Codelock_Config m_VVN_Greenhouse_Codelock_Config;

    override void OnInit()
    {
        super.OnInit();               
        m_VVN_Greenhouse_Codelock_Config = new VVN_Greenhouse_Codelock_Config;
        GetGame().ServerSet_VVN_Greenhouse_Codelock_Config(m_VVN_Greenhouse_Codelock_Config);
    }
};
#endif