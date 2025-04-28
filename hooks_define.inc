//
#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif
#define OnGameModeInit bazar_OnGameModeInit
#if defined bazar_OnGameModeInit
	forward bazar_OnGameModeInit();
#endif
//
#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect bazar_OnPlayerConnect
#if defined bazar_OnPlayerConnect
	forward bazar_OnPlayerConnect(playerid);
#endif

//
#if defined _ALS_OnPlayerLeaveDynamicArea
    #undef OnPlayerLeaveDynamicArea
#else
    #define _ALS_OnPlayerLeaveDynamicArea
#endif
#define OnPlayerLeaveDynamicArea b_OnPlayerLeaveDynamicArea
#if defined b_OnPlayerLeaveDynamicArea
	forward b_OnPlayerLeaveDynamicArea(playerid, areaid);
#endif

//
#if defined _ALS_OnPlayerEnterDynamicArea
    #undef OnPlayerEnterDynamicArea
#else
    #define _ALS_OnPlayerEnterDynamicArea
#endif
#define OnPlayerEnterDynamicArea b_OnPlayerEnterDynamicArea
#if defined b_OnPlayerEnterDynamicArea
	forward b_OnPlayerEnterDynamicArea(playerid, areaid);
#endif

//
#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect bazar_OnPlayerDisconnect
#if defined bazar_OnPlayerDisconnect
	forward bazar_OnPlayerDisconnect(playerid, reason);
#endif

#if defined _ALS_OnDialogResponse
    #undef OnDialogResponse
#else
    #define _ALS_OnDialogResponse
#endif
#define OnDialogResponse b_OnDialogResponse
#if defined b_OnDialogResponse
	forward b_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]);
#endif