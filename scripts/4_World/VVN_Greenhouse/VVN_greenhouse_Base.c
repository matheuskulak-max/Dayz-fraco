class VVN_greenhouse_Base : GardenBase
{   
    protected bool m_IsOpened = false;
    protected bool m_IsOpenedClient = false;
    
    const string ATTACHMENT_SLOT_COMBINATION_LOCK = "Att_CombinationLock";
    const float DOOR_ROTATION_ANGLE_DEG = 1;
    
    const string SOUND_DOOR_OPEN = "vvn_greenhouse_door_open_SoundSet";
    const string SOUND_DOOR_CLOSE = "vvn_greenhouse_door_close_SoundSet";
    
    protected EffectSound m_SoundDoorOpen;
    protected EffectSound m_SoundDoorClose;
    
    void VVN_greenhouse_Base()
    {
        RegisterNetSyncVariableBool("m_IsOpened");
        m_IsOpened = false;
        m_IsOpenedClient = false;
        RefreshSlots();
    }
    
    override int GetGardenSlotsCount()
    {
        return 16;
    }
    
    void RefreshSlots()
    {
        for (int i = 1; i <= 16; i++)
        {
            HideSelection("SeedBase_" + i);
        }
    }
    
    void SetOpenedState(bool state)
    {
        m_IsOpened = state;
    }
    
    override bool IsOpen()
    {
        return m_IsOpened;
    }
    
    bool CanOpenDoor()
    {
        #ifdef CodeLock
        if (IsCodeLocked())
            return false;
        #endif
        
        if (!IsOpen() && !IsLocked())
            return true;
        
        return false;
    }
    
    bool CanCloseDoor()
    {
        return IsOpen();
    }
    
    void OpenDoor()
    {
        if (g_Game.IsServer())
        {
            SetAnimationPhase("door1", DOOR_ROTATION_ANGLE_DEG);
            SetOpenedState(true);
            SetSynchDirty();
        }
        
        if (!g_Game.IsDedicatedServer())
            SoundDoorOpen();
    }
    
    void CloseDoor()
    {
        if (g_Game.IsServer())
        {
            SetAnimationPhase("door1", 0);
            SetOpenedState(false);
            SetSynchDirty();
        }
        
        if (!g_Game.IsDedicatedServer())
            SoundDoorClose();
    }
    
    void UpdateVisualState()
    {
        if (IsOpen())
        {
            SetAnimationPhase("door1", DOOR_ROTATION_ANGLE_DEG);
        }
        else
        {
            SetAnimationPhase("door1", 0);
        }
    }
    
    override void OnVariablesSynchronized()
    {
        super.OnVariablesSynchronized();

        if (m_IsOpened != m_IsOpenedClient)
        {       
            if (IsOpen())
            {
                SoundDoorOpen();
            }
            else
            {
                SoundDoorClose();
            }   
            m_IsOpenedClient = m_IsOpened;
        }
        
        UpdateVisualState();
    }
    
    override void OnStoreSave(ParamsWriteContext ctx)
    {   
        super.OnStoreSave(ctx);
        ctx.Write(m_IsOpened);
    }
    
    override bool OnStoreLoad(ParamsReadContext ctx, int version)
    {
        if (!super.OnStoreLoad(ctx, version))
            return false;
        
        if (!ctx.Read(m_IsOpened))
            return false;
        
        if (m_IsOpened)
            OpenDoor();
        else
            CloseDoor();
        
        return true;
    }
    
    bool IsLocked()
    {
        #ifdef CodeLock
        if (IsCodeLocked())
            return true;
        #endif
        
        return false;
    }
    
    protected void SoundDoorOpen()
    {
        PlaySoundSet(m_SoundDoorOpen, SOUND_DOOR_OPEN, 0.1, 0.1);
    }

    protected void SoundDoorClose()
    {
        PlaySoundSet(m_SoundDoorClose, SOUND_DOOR_CLOSE, 0.1, 0.1);
    }
    
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionOpenVVNDoor);
        AddAction(ActionCloseVVNDoor);
        
        #ifdef CodeLock
        AddAction(ActionAttachCodeLock);
        AddAction(ActionInteractLock);
        AddAction(ActionManageLock);
        AddAction(ActionLockAdmin);
        #endif
    }
    
    // ========== ПРОВЕРКА, ЧТО ИГРОК ВНУТРИ (через inv_min/inv_max) ==========
    override bool IsPlayerInside(PlayerBase player, string selection)
    {
        vector player_pos = player.GetPosition();
        vector object_pos = GetPosition();
        vector ref_dir = GetDirection();
        ref_dir[1] = 0;
        ref_dir.Normalize();
        
        vector min, max;
        min = -GetMemoryPointPos("inv_min");
        max = -GetMemoryPointPos("inv_max");
        
        vector dir_to_object = object_pos - player_pos;
        dir_to_object[1] = 0;
        float len = dir_to_object.Length();
        
        dir_to_object.Normalize();
        
        vector ref_dir_angle = ref_dir.VectorToAngles();
        vector dir_to_object_angle = dir_to_object.VectorToAngles();
        vector test_angles = dir_to_object_angle - ref_dir_angle;
        
        vector test_position = test_angles.AnglesToVector() * len;
        
        if(test_position[0] > max[0] || test_position[0] < min[0] || test_position[2] > max[2] || test_position[2] < min[2])
        {
            return false;
        }
        
        return true;
    }
    
    bool GetPlayerInside()
    {
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (player && IsPlayerInside(player, ""))
        {
            return true;
        }
        return false;
    }
    
    // ========== КОНТРОЛЬ ВИДИМОСТИ СЛОТОВ ==========
    override bool CanDisplayAttachmentSlot(int slot_id)
    {
        string slot_name = InventorySlots.GetSlotName(slot_id);
        
        // Грядки (SeedBase) всегда показываем
        if (slot_name.Contains("SeedBase"))
        {
            return true;
        }
        
        // Остальные слоты (бочки, канистры, кодлок) — только если игрок внутри
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (player)
        {
            float distance = vector.Distance(player.GetPosition(), GetPosition());
            if (distance < 2.0 && GetPlayerInside())
            {
                return true;
            }
            return false;
        }
        
        return super.CanDisplayAttachmentSlot(slot_id);
    }
    
    // Инвентарь всегда видим (управляем через CanDisplayAttachmentSlot)
    override bool IsInventoryVisible()
    {
        return true;
    }
    
     // ========== КОДЛОК ==========
    #ifdef CodeLock
    CodeLock GetCodeLockRS()
    {
        return CodeLock.Cast(FindAttachmentBySlotName(ATTACHMENT_SLOT_COMBINATION_LOCK));
    }
    
    bool IsCodeLocked()
    {
        CodeLock codelock = GetCodeLockRS();
        return codelock && codelock.GetLockState();
    }
    
    bool IsDoorOpen()
    {
        return IsOpen();
    }
    #endif
};