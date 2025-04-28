public OnGameModeInit()
{
    set_lavka_total(0);

    for(new i = 0; i < LAVKA_MAX; i++)
    {
        Lavka[i][li_OwnerID]    = INVALID_PLAYER_ID;
        Lavka[i][li_Price]      = 5000;
        Lavka[i][li_Object]     = 0;
        Lavka[i][li_Active]     = 0;
        Lavka[i][li_ArenaSec]   = 0;

        for(new g = 0; g < 6; g++) Lavka[i][li_ObjectPos][g] = 0.0;

        for(new item = 0; item < MAX_LAVKA_ITEMS; item++)
        {
            ClearLavkaItem(i, item);
        }
    }
    
    create_lavka(15000, 1020.0837, -1920.4396, 12.6238, 0.0, 0.0, 0.0, 0);
    create_lavka(15000, 1020.0939, -1905.8657, 12.6491, 0.0, 0.0, 0.0, 0);
    create_lavka(15000, 1020.0930, -1891.0249, 12.6741, 0.0, 0.0, 0.0, 0);

    #if defined bazar_OnGameModeInit
		bazar_OnGameModeInit();
	#endif
    return 1;
}

public OnPlayerConnect(playerid)
{
    set_player_lavka(playerid, -1);
    set_player_timer_end(playerid, 0);
    ClearLavkaEditVars(playerid);
    ClearLavkaCustomerVars(playerid);

	#if defined bazar_OnPlayerConnect
		bazar_OnPlayerConnect(playerid);
	#endif
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
    if(get_player_lavka(playerid) != -1)
    {
        LavkaRentStop(get_player_lavka(playerid), 3);
    }
    
	#if defined bazar_OnPlayerDisconnect
		bazar_OnPlayerDisconnect(playerid, reason);
	#endif
	return 1;
}

public OnPlayerLeaveDynamicArea(playerid, areaid)
{
    if(get_player_lavka(playerid) != -1)
    {
        new lavkaid = get_player_lavka(playerid);
        if(areaid == Lavka[lavkaid][li_Area])
        {
            SendTipp(playerid, "��������� � ���� ����� �����, ����� ��������� ���� ������� �� ������!");
        }
    }

    #if defined b_OnPlayerLeaveDynamicArea
		b_OnPlayerLeaveDynamicArea(playerid, areaid);
	#endif
    return 1;
}

public OnPlayerEnterDynamicArea(playerid, areaid)
{
    if(get_player_lavka(playerid) != -1)
    {
        new lavkaid = get_player_lavka(playerid);
        if(areaid == Lavka[lavkaid][li_Area])
        {
            SendTipp(playerid, "�� ������� ��������� � ���� ����� �������� �����, ������� �� ������ ��������.");
            set_player_timer_end(playerid, 0);
        }
    }

    #if defined b_OnPlayerEnterDynamicArea
		b_OnPlayerEnterDynamicArea(playerid, areaid);
	#endif
    return 1;
}

#include ../modules/systems/bazar/dialogs
#include ../modules/systems/bazar/hooks_define