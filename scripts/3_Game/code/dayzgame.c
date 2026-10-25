#ifdef CodeLock
modded class DayZGame
{
    protected ref VVN_Greenhouse_Codelock_Config VVN_Greenhouse_Codelock_config;
    
    VVN_Greenhouse_Codelock_Config Get_VVN_Greenhouse_Codelock_Config()
    {
        if (!VVN_Greenhouse_Codelock_config && !GetGame().IsClient())
        {
            ServerSet_VVN_Greenhouse_Codelock_Config(new VVN_Greenhouse_Codelock_Config);
        }
        else if (!VVN_Greenhouse_Codelock_config && GetGame().IsClient())
        {
            return null;
        }
        return VVN_Greenhouse_Codelock_config;
    }

    void ServerSet_VVN_Greenhouse_Codelock_Config(VVN_Greenhouse_Codelock_Config config)
    {
        if (GetGame().IsClient())
        {
            Error("ERROR: ServerSet_VVN_Greenhouse_Codelock_Config can only be called on server");
            return;
        }
        VVN_Greenhouse_Codelock_config = config;
    }

    void ClientSet_VVN_Greenhouse_Codelock_Config(VVN_Greenhouse_Codelock_Config config)
    {
        if (!GetGame().IsClient())
        {
            Error("ERROR: ClientSet_VVN_Greenhouse_Codelock_Config can only be called on client");
            return;
        }

        VVN_Greenhouse_Codelock_config = config;
    }

    override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
    {
        super.OnRPC(sender, target, rpc_type, ctx);

        switch (rpc_type)
        {
            case VVN_Greenhouse_Codelock_RPC.RPC_CLIENT_SETCONFIG: 
            {
                if (!GetGame().IsClient())
                    return;
                Param1<VVN_Greenhouse_Codelock_Config> dataConfig;
                if (!ctx.Read(dataConfig))
                    return;
                ClientSet_VVN_Greenhouse_Codelock_Config(dataConfig.param1);
                PrintFormat("[VVN_Greenhouse_Codelock_Config] Received instance of config %1", dataConfig.param1);
                break;
            }
            case VVN_Greenhouse_Codelock_RPC.RPC_SERVER_SYNCHCONFIG:
            {
                if (!GetGame().IsServer())
                    return;
                auto configParams = new Param1<VVN_Greenhouse_Codelock_Config>(Get_VVN_Greenhouse_Codelock_Config());
                GetGame().RPCSingleParam(null, VVN_Greenhouse_Codelock_RPC.RPC_CLIENT_SETCONFIG, configParams, true, sender);
                break;
            }
        }
    }
};
#endif