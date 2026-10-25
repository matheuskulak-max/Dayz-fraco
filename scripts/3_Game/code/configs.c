#ifdef CodeLock
enum VVN_Greenhouse_Codelock_RPC 
{
    DIRPC_START = 76532794,  // ← другое число, чтобы не конфликтовать
    RPC_SERVER_SYNCHCONFIG,
    RPC_CLIENT_SETCONFIG
};

class VVN_Greenhouse_Codelock_Const 
{
    const static string ModProfileFolder = "$profile:\\VVN_Greenhouse\\";
    const static string FileName = "VVN_Greenhouse_CodeLock_Config.json";
    const static string ConfigPath = ModProfileFolder + FileName;
};
#endif