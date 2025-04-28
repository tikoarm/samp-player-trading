#define LAVKA_OBJECT        3862
#define LAVKA_MAX           3
#define LAVKA_DISTANCE      (3.5)
#define LAVKA_MIN_SELLPRICE 500
#define LAVKA_MAX_SELLPRICE 100000000

#define LAV_TYPE_NAME_NONE  "{ccccff}-"
#define LAV_TYPE_NAME_HOUSE "Недвижимость"
#define LAV_TYPE_NAME_BIZZ  "Средство заработка"
#define LAV_TYPE_NAME_AZS   "Средство заработка"
#define LAV_TYPE_NAME_MBIZZ "Средство заработка"
#define LAV_TYPE_NAME_AUTO  "Движемое имущество"
#define LAV_TYPE_NAME_ACS   "Другое"
#define LAV_TYPE_NAME_INV   "Другое"

#define LAV_ITEM_NAME_NONE  "{ccccff}-"
#define LAV_ITEM_NAME_HOUSE "Дом"
#define LAV_ITEM_NAME_BIZZ  "Бизнес"
#define LAV_ITEM_NAME_AZS   "АЗС"
#define LAV_ITEM_NAME_MBIZZ "МНП"
#define LAV_ITEM_NAME_AUTO  "Автомобиль"
#define LAV_ITEM_NAME_ACS   "Аксессуары"
#define LAV_ITEM_NAME_INV   "Предмет"

enum LavkaInfo
{
    li_OwnerID,
    li_Price,

    li_Object,
    Text3D:li_3DText,
    Float:li_ObjectPos[6],
    li_Active,
    li_Area,
    li_ArenaSec,

    li_WithObject
};
new Lavka[LAVKA_MAX][LavkaInfo];

enum L_Items
{
    L_None,
    L_House,
    L_Bizz,
    L_Azs,
    L_Mbizz,
    L_Auto,
    L_Acs,
    L_Inv
};

#define MAX_LAVKA_ITEMS     15
enum LavkaItemsInfo
{
    L_Items:lType,
    lModel,
    lValue,
    lPrice,
    lAmount
}
new LavkaItems[LAVKA_MAX][MAX_LAVKA_ITEMS][LavkaItemsInfo];


static lavka_total_value;
stock get_lavka_total() return lavka_total_value;
stock set_lavka_total(val) { lavka_total_value = val; return 1; }

static current_player_lavka[MAX_PLAYERS];
stock get_player_lavka(playerid) return current_player_lavka[playerid];
stock set_player_lavka(playerid, val) { current_player_lavka[playerid] = val; return 1; }

static lavka_timer_end[MAX_PLAYERS];
stock get_player_timer_end(playerid) return lavka_timer_end[playerid];
stock set_player_timer_end(playerid, val) { lavka_timer_end[playerid] = val; return 1; }

static lavka_workitem[MAX_PLAYERS];
stock get_lavka_workitem(playerid) return lavka_workitem[playerid];
stock set_lavka_workitem(playerid, val) { lavka_workitem[playerid] = val; return 1; }

//
static L_Items:edit_lavka_type[MAX_PLAYERS];
stock L_Items:get_lavka_edit_type(playerid) return edit_lavka_type[playerid];
stock set_lavka_edit_type(playerid, L_Items:val) { edit_lavka_type[playerid] = val; return 1; }

static edit_lavka_model[MAX_PLAYERS];
stock get_lavka_edit_model(playerid) return edit_lavka_model[playerid];
stock set_lavka_edit_model(playerid, val) { edit_lavka_model[playerid] = val; return 1; }

static edit_lavka_value[MAX_PLAYERS];
stock get_lavka_edit_value(playerid) return edit_lavka_value[playerid];
stock set_lavka_edit_value(playerid, val) { edit_lavka_value[playerid] = val; return 1; }

//
static lavka_chooseitem_inv[MAX_PLAYERS];
stock is_player_choosing_lavkainv(playerid) return lavka_chooseitem_inv[playerid];
stock set_player_choosing_lavkainv(playerid, val) { lavka_chooseitem_inv[playerid] = val; return 1; }

//
static customer_lavka_id[MAX_PLAYERS];
stock get_lavka_customer_id(playerid) return customer_lavka_id[playerid];
stock set_lavka_customer_id(playerid, val) { customer_lavka_id[playerid] = val; return 1; }

static customer_lavka_model[MAX_PLAYERS];
stock get_lavka_customer_model(playerid) return customer_lavka_model[playerid];
stock set_lavka_customer_model(playerid, val) { customer_lavka_model[playerid] = val; return 1; }

static customer_lavka_value[MAX_PLAYERS];
stock get_lavka_customer_value(playerid) return customer_lavka_value[playerid];
stock set_lavka_customer_value(playerid, val) { customer_lavka_value[playerid] = val; return 1; }

static L_Items:customer_lavka_type[MAX_PLAYERS];
stock L_Items:get_lavka_customer_type(playerid) return customer_lavka_type[playerid];
stock set_lavka_customer_type(playerid, L_Items:val) { customer_lavka_type[playerid] = val; return 1; }

static customer_lavka_amount[MAX_PLAYERS];
stock get_lavka_customer_amount(playerid) return customer_lavka_amount[playerid];
stock set_lavka_customer_amount(playerid, val) { customer_lavka_amount[playerid] = val; return 1; }