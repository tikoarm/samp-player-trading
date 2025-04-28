stock create_lavka(
    rentprice, 
    Float:objpos_X, Float:objpos_Y, Float:objpos_Z, 
    Float:objpos_RX, Float:objpos_RY, Float:objpos_RZ,
    with_object
)
{
    if(get_lavka_total() >= LAVKA_MAX) return print("НЕВОЗМОЖНО СОЗДАТЬ ЛАВКУ, ПРЕВЫШЕН ЛИМИТ!!!");

    new i = get_lavka_total(), string[1024*3];

    Lavka[i][li_OwnerID]    = INVALID_PLAYER_ID;
    Lavka[i][li_Price]      = rentprice;
    Lavka[i][li_Active]     = 1;

    Lavka[i][li_ObjectPos][0] = objpos_X;
    Lavka[i][li_ObjectPos][1] = objpos_Y;
    Lavka[i][li_ObjectPos][2] = objpos_Z;
    Lavka[i][li_ObjectPos][3] = objpos_RX;
    Lavka[i][li_ObjectPos][4] = objpos_RY;
    Lavka[i][li_ObjectPos][5] = objpos_RZ;

    Lavka[i][li_WithObject] = with_object;

    if(Lavka[i][li_WithObject])
    {
        Lavka[i][li_Object] = CreateDynamicObject(LAVKA_OBJECT, objpos_X, objpos_Y, objpos_Z, objpos_RX, objpos_RY, objpos_RZ);
    }
    
    Lavka[i][li_Area] = CreateDynamicSphere(objpos_X, objpos_Y, objpos_Z, 15.0, 0, 0);

    LavkaGenerate_3DText(i, string);
    Lavka[i][li_3DText] = CreateDynamic3DTextLabel(string, -1, objpos_X,  objpos_Y, objpos_Z, LAVKA_DISTANCE*3); 
    set_lavka_total(i + 1);
    return 1;
}

stock ShowPlayerLavkaItems(playerid, lavkaid)
{
    if(lavkaid == -1) return 0;
    if(Lavka[lavkaid][li_OwnerID] == INVALID_PLAYER_ID) return 0;

    ClearLavkaCustomerVars(playerid);
    set_lavka_customer_id(playerid, lavkaid);

    new typetext[64], itemtext[64];
    new string[1024*2] = "{ccff00}ID\t{ccff00}Тип предмета\t{ccff00}Наименование\t{ccff00}Стоимость\n";
    for(new i = 0; i < MAX_LAVKA_ITEMS; i++)
    {
        get_litems_type_name(LavkaItems[lavkaid][i][lType], typetext);
        get_litems_item_name(LavkaItems[lavkaid][i][lType], LavkaItems[lavkaid][i][lModel], LavkaItems[lavkaid][i][lAmount], itemtext);

        if(LavkaItems[lavkaid][i][lType] == L_Items:L_Inv || LavkaItems[lavkaid][i][lType] == L_Items:L_Acs)
            format(string, sizeof(string), "%s{00ff7f}%i.\t{ffd700}%s\t{c6e2ff}%s\t{a0db8e}$ %i {ffffff}за ед. товара\n", string, i+1, typetext, itemtext, LavkaItems[lavkaid][i][lPrice]);
        else
            format(string, sizeof(string), "%s{00ff7f}%i.\t{ffd700}%s\t{c6e2ff}%s\t{a0db8e}$ %i\n", string, i+1, typetext, itemtext, LavkaItems[lavkaid][i][lPrice]);

        typetext[0] = EOS;
        itemtext[0] = EOS;
    }

    ShowPlayerDialog(playerid, dLavkaCustomer_Main, DIALOG_STYLE_TABLIST_HEADERS, "{ff4040}Рынок", string, "Выбрать", "Закрыть");
    return 1;
}

stock LavkaChooseMyVehicle(playerid)
{
    new string[1024], v, price;
    strcat(string, "{ccff00}Слот\t{ccff00}Наименование\t{ccff00}Статус\n");

    for(new i = 0; i < 5; i++)
    {
        if(PlayerInfo[playerid][pCarKey][i] != -1)
        {
            v = PlayerInfo[playerid][pCarKey][i];
            price = GosVehiclePrice(GetVehicleModel(VehicleInfo[v][vRealID]));
            format(string, sizeof(string), "%s{00ff7f}%i.\t{ffffff}%s {a0db8e}[ID: {ffffff}%i{a0db8e}]\t{ffffff}[ %s {ffffff}]\n", 
                string, i + 1, CarName[GetVehicleModel(VehicleInfo[v][vRealID])-400], GetVehicleModel(VehicleInfo[v][vRealID]), 
                (price == 0) ? ("{ff4040}Запрещен для продажи") : ("{a0db8e}Можно продавать"));
        } 
        else format(string, sizeof(string), "%s{00ff7f}%i.\t{ffffff}Слот пустой\t{ffffff}[ {a0db8e}Слот пустой {ffffff}]\n", string, i + 1);
    }

    ShowPlayerDialog(playerid, dLavkaEdit_Vehicle1, DIALOG_STYLE_TABLIST_HEADERS, "{ff4040}Рынок", string, "Выбрать", "Закрыть");
    return 1;
}

stock LavkaEditPanel(playerid)
{
    ClearLavkaEditVars(playerid);
    new lavkaid = get_player_lavka(playerid);
    if(lavkaid == -1) return 1;
    new typetext[64], itemtext[64];
    
    new string[1024*2] = "{ccff00}ID\t{ccff00}Тип предмета\t{ccff00}Наименование\t{ccff00}Стоимость\n";
    strcat(string, "{ccccff}-\t{ccff00}Отменить аренду\t"LAV_ITEM_NAME_NONE"\t{a0db8e}>>>\n");

    for(new i = 0; i < MAX_LAVKA_ITEMS; i++)
    {
        get_litems_type_name(LavkaItems[lavkaid][i][lType], typetext);
        get_litems_item_name(LavkaItems[lavkaid][i][lType], LavkaItems[lavkaid][i][lModel], LavkaItems[lavkaid][i][lAmount], itemtext);

        if(LavkaItems[lavkaid][i][lType] == L_Items:L_Inv || LavkaItems[lavkaid][i][lType] == L_Items:L_Acs)
            format(string, sizeof(string), "%s{00ff7f}%i.\t{ffd700}%s\t{c6e2ff}%s\t{a0db8e}$ %i {ffffff}за ед. товара\n", string, i+1, typetext, itemtext, LavkaItems[lavkaid][i][lPrice]);
        else
            format(string, sizeof(string), "%s{00ff7f}%i.\t{ffd700}%s\t{c6e2ff}%s\t{a0db8e}$ %i\n", string, i+1, typetext, itemtext, LavkaItems[lavkaid][i][lPrice]);

        typetext[0] = EOS;
        itemtext[0] = EOS;
    }
    
    ShowPlayerDialog(playerid, dLavkaEdit_1, DIALOG_STYLE_TABLIST_HEADERS, "{ff4040}Рынок", string, "Выбрать", "Закрыть");
    return 1;
}
stock LavkaEdit_ChooseType(playerid)
{
    new string[128];
    format(string, sizeof(string), "{ff4040}Линия Продаж №%i", get_lavka_workitem(playerid) + 1);

    ShowPlayerDialog(playerid, dLavkaEdit_2, DIALOG_STYLE_LIST, string, "\
        {ffd700}"LAV_TYPE_NAME_HOUSE": {c6e2ff}"LAV_ITEM_NAME_HOUSE"\n\
        {ffd700}"LAV_TYPE_NAME_BIZZ": {c6e2ff}"LAV_ITEM_NAME_BIZZ"\n\
        {ffd700}"LAV_TYPE_NAME_AZS": {c6e2ff}"LAV_ITEM_NAME_AZS"\n\
        {ffd700}"LAV_TYPE_NAME_MBIZZ": {c6e2ff}"LAV_ITEM_NAME_MBIZZ"\n\
        {ffd700}"LAV_TYPE_NAME_AUTO": {c6e2ff}"LAV_ITEM_NAME_AUTO"\n\
        {ffd700}"LAV_TYPE_NAME_ACS": {c6e2ff}"LAV_ITEM_NAME_ACS"\n\
        {ffd700}"LAV_TYPE_NAME_INV": {c6e2ff}"LAV_ITEM_NAME_INV"\n\
    ", "Выбрать", "Закрыть");
    return 1;
}

stock LavkaToNull(lavkaid)
{
    new playerid = Lavka[lavkaid][li_OwnerID];
    ClearLavkaAllItems(lavkaid);

    Lavka[lavkaid][li_OwnerID] = INVALID_PLAYER_ID;
    Lavka[lavkaid][li_ArenaSec] = 60*60;
    set_player_lavka(playerid, -1);

    new string[1024*3];
    LavkaGenerate_3DText(lavkaid, string);
    UpdateDynamic3DTextLabelText(Lavka[lavkaid][li_3DText], -1, string);
    return 1;
}

stock ClearLavkaItem(lavkaid, item)
{
    if(LavkaItems[lavkaid][item][lType] == L_Items:L_Inv)
    {
        new playerid = Lavka[lavkaid][li_OwnerID];
        if(LavkaItems[lavkaid][item][lAmount] >= 1)
            AddItem(playerid, LavkaItems[lavkaid][item][lModel], LavkaItems[lavkaid][item][lAmount]);
    }
    else if(LavkaItems[lavkaid][item][lType] == L_Items:L_Acs)
    {
        new playerid = Lavka[lavkaid][li_OwnerID];
        if(LavkaItems[lavkaid][item][lAmount] >= 1)
            AddAcsItem(playerid, LavkaItems[lavkaid][item][lModel], LavkaItems[lavkaid][item][lAmount]);
    }

    LavkaItems[lavkaid][item][lType] = L_Items:L_None;
    LavkaItems[lavkaid][item][lModel] = 0;
    LavkaItems[lavkaid][item][lValue] = 0;
    LavkaItems[lavkaid][item][lPrice] = 0;
    LavkaItems[lavkaid][item][lAmount] = 0;
    return 1;
}
stock ClearLavkaAllItems(lavkaid)
{
    for(new i = 0; i < MAX_LAVKA_ITEMS; i++)
    {
        ClearLavkaItem(lavkaid, i);
    }
    return 1;
}

stock Lavka_IsItemOnSale(lavkaid, L_Items:type, product_model, product_value, with_message = 1)
{
    for(new item = 0; item < MAX_LAVKA_ITEMS; item++)
    {
        if(LavkaItems[lavkaid][item][lType] != type) continue;
        if(LavkaItems[lavkaid][item][lModel] != product_model) continue;
        if(product_value != -1)
        {
            if(LavkaItems[lavkaid][item][lValue] != product_value) continue;
        }
        if(with_message) Error(Lavka[lavkaid][li_OwnerID], "Данный продукт уже продаётся в другом ряду Вашего ларька, а именно: №%i", item + 1);
        return item;
    }
    return -1;
}

stock LavkaOneSecondTimer()
{
    new string[1024*3];
    for(new i = 0; i < get_lavka_total(); i++)
    {
        if(Lavka[i][li_OwnerID] == INVALID_PLAYER_ID) continue;
        Lavka[i][li_ArenaSec]--;
        LavkaGenerate_3DText(i, string);
        UpdateDynamic3DTextLabelText(Lavka[i][li_3DText], -1, string);
        if(Lavka[i][li_ArenaSec] <= 0) LavkaRentStop(i, 1);
    }
    return 1;
}
stock LavkaOneSecondTimerPlayer(playerid)
{
    new lavkaid = get_player_lavka(playerid);
    if(lavkaid == -1) return 1;
    if(IsPlayerInDynamicArea(playerid, Lavka[lavkaid][li_Area])) return 1;

    set_player_timer_end(playerid, get_player_timer_end(playerid) + 1);
    new now_sec = get_player_timer_end(playerid);
    if(now_sec > 15) LavkaRentStop(lavkaid, 2);
    else if(now_sec == 5) Tipp(playerid, "Аренда лавки №%i будет завершена через %i сек по причине Вашего длительного отсутствия!", lavkaid + 1, 15-now_sec);
    else if(now_sec == 10) Tipp(playerid, "Аренда лавки №%i будет завершена через %i сек по причине Вашего длительного отсутствия!", lavkaid + 1, 15-now_sec);
    return 1;
}

stock ShowLavkaInputPrice(playerid)
{
    new string[512], typetext[64], itemtext[64];
    get_litems_type_name(get_lavka_edit_type(playerid), typetext);
    get_litems_item_name(get_lavka_edit_type(playerid), get_lavka_edit_model(playerid), get_lavka_edit_value(playerid), itemtext, 0);

    if(get_lavka_edit_type(playerid) == L_Items:L_Inv || get_lavka_edit_type(playerid) == L_Items:L_Acs)
    {
        format(string, sizeof(string), "\n{ffffff}Пожалуйста, введите стоимость товара за одну штуку:\n\n\
            {ffffff}Тип предмета: {ffd700}%s\n\
            {ffffff}Наименование товара: {c6e2ff}%s\n\n\
            {ffffff}Минимальная стоимость: {a0db8e}$ %i\n\
            {ffffff}Максимальная стоимость: {a0db8e}$ %i\
        ", 
        typetext,
        itemtext,
        LAVKA_MIN_SELLPRICE,
        LAVKA_MAX_SELLPRICE);
    }
    else
    {
        format(string, sizeof(string), "\n{ffffff}Пожалуйста, введите стоимость товара для покупки:\n\n\
            {ffffff}Тип предмета: {ffd700}%s\n\
            {ffffff}Наименование товара: {c6e2ff}%s\n\n\
            {ffffff}Минимальная стоимость: {a0db8e}$ %i\n\
            {ffffff}Максимальная стоимость: {a0db8e}$ %i\
        ", 
        typetext,
        itemtext,
        LAVKA_MIN_SELLPRICE,
        LAVKA_MAX_SELLPRICE);
    }

    ShowPlayerDialog(playerid, dLavkaEdit_Price, DIALOG_STYLE_INPUT, "{ff4040}Рынок", string, "Сохранить", "Закрыть");
    return 1;
}

stock KeyWalk_Lavka(playerid)
{
    new lavkaid = get_closest_lavka(playerid);
    if(lavkaid == -1) return 1;
    if(get_player_lavka(playerid) != -1)
    {
        if(lavkaid == get_player_lavka(playerid)) return LavkaEditPanel(playerid);
        Error(playerid, "Вы уже арендуете лавку №%i!", get_player_lavka(playerid) + 1);
        return 0;
    }
    if(Lavka[lavkaid][li_OwnerID] != INVALID_PLAYER_ID)
    {
        ShowPlayerLavkaItems(playerid, lavkaid);
        return 0;
    }
 
    setup_lavka_owner(playerid, lavkaid);
    return 1;
}

stock setup_lavka_owner(playerid, lavkaid = 5555)
{
    if(lavkaid == 5555) lavkaid = get_closest_lavka(playerid);
    if(lavkaid == -1) return 0;

    if(PlayerInfo[playerid][pMoney] < Lavka[lavkaid][li_Price]) return error(playerid, "У Вас недостаточно денег!");
    GiveMoney(playerid, -Lavka[lavkaid][li_Price]);

    Lavka[lavkaid][li_OwnerID] = playerid;
    Lavka[lavkaid][li_ArenaSec] = 60*60;
    new string[1024*3];
    LavkaGenerate_3DText(lavkaid, string);
    UpdateDynamic3DTextLabelText(Lavka[lavkaid][li_3DText], -1, string);
    set_player_lavka(playerid, lavkaid);

    Tipp(playerid, "Вы успешно арендовали лавку для аренды №%i за $%i/час", lavkaid + 1, Lavka[lavkaid][li_Price]);
    SendTipp(playerid, "Если Вы отойдёте от лавки на дистанцию более 15м, договор об аренде будет расторгнут!");
    return 1;
}

stock LavkaGenerate_3DText(lavkaid, text[], len = sizeof(text))
{
    format(text, len, "{ffd700}Прилавок №%i\n", lavkaid + 1);

    if(Lavka[lavkaid][li_OwnerID] == INVALID_PLAYER_ID) {
        format(text, len, "%s\n{008000}Свободен\n{FFFFFF}Стоимость аренды: {008000}$%i\n\n{FFFFFF}Для аренды нажмите {ff6666}L.Alt", text, Lavka[lavkaid][li_Price]);
        return 1;
    }
    format(text, len, "%s{FFFFFF}Занято: {ff7373}%s{FFFFFF}({ff7373}%i{FFFFFF}) [{ff7373}%s{FFFFFF}]\n{FFFFFF}Для просмотра товара нажмите {ff6666}L.Alt\n\n\
    {FFFFFF}Список товаров на прилавке:\n", text, Name(Lavka[lavkaid][li_OwnerID]), Lavka[lavkaid][li_OwnerID], ConvertInGame(Lavka[lavkaid][li_ArenaSec]));
    
    new total_items = 0, typetext[64], itemtext[64];
    for(new item = 0; item < MAX_LAVKA_ITEMS; item++)
    {
        if(LavkaItems[lavkaid][item][lType] != L_Items:L_None)
        {
            get_litems_type_name(LavkaItems[lavkaid][item][lType], typetext);
            get_litems_item_name(LavkaItems[lavkaid][item][lType], LavkaItems[lavkaid][item][lModel], LavkaItems[lavkaid][item][lAmount], itemtext);
            format(text, len, "%s\n{ffd700}%s{ffffff}: {c6e2ff}%s", text, typetext, itemtext);
            total_items++;
            typetext[0] = EOS;
            itemtext[0] = EOS;
        }
    }
    if(!total_items) format(text, len, "%s{a0db8e}Пусто", text);

    return 1;
}

stock get_closest_lavka(playerid)
{
    new lavkaid = -1;
    for(new i = 0; i < get_lavka_total(); i++)
    {   
        if(IsPlayerInRangeOfPoint(playerid, LAVKA_DISTANCE, Lavka[i][li_ObjectPos][0], Lavka[i][li_ObjectPos][1], Lavka[i][li_ObjectPos][2]))
        {
            lavkaid = i;
            break;
        }
    }
    return lavkaid;
}

stock LavkaRentStop(lavkaid, reason)
{
    new playerid = Lavka[lavkaid][li_OwnerID];

    if(reason == 1)
    {
        Tipp(playerid, "Аренда лавки №%i была завершена по причине окончания срока аренды!", lavkaid + 1);
    }
    else if(reason == 2)
    {
        Tipp(playerid, "Аренда лавки №%i была завершена по причине Вашего длительного отсутствия!", lavkaid + 1);
    }
    else if(reason == 3)
    {
        Tipp(playerid, "Аренда лавки №%i была завершена по причине Вашего отключения от сервера!", lavkaid + 1);
    }
    else if(reason == 4)
    {
        Tipp(playerid, "Аренда лавки №%i была завершена по причине Вашей досрочной отмены договора!", lavkaid + 1);
    }

    LavkaToNull(lavkaid);
    return 1;
}

stock get_litems_type_name(L_Items:type, string[], len = sizeof(string))
{
    if(type == (L_Items:L_None)) strcat(string, LAV_TYPE_NAME_NONE, len);
    else if(type == (L_Items:L_House)) strcat(string, LAV_TYPE_NAME_HOUSE, len);
    else if(type == (L_Items:L_Bizz)) strcat(string, LAV_TYPE_NAME_BIZZ, len);
    else if(type == (L_Items:L_Azs)) strcat(string, LAV_TYPE_NAME_AZS, len);
    else if(type == (L_Items:L_Mbizz)) strcat(string, LAV_TYPE_NAME_MBIZZ, len);
    else if(type == (L_Items:L_Auto)) strcat(string, LAV_TYPE_NAME_AUTO, len);
    else if(type == (L_Items:L_Acs)) strcat(string, LAV_TYPE_NAME_ACS, len);
    else if(type == (L_Items:L_Inv)) strcat(string, LAV_TYPE_NAME_INV, len);
    
    return 1;
}

stock get_litems_item_name(L_Items:type, lavka_model, lavka_amount, string[], with_amount = 1, len = sizeof(string))
{
    if(type == (L_Items:L_None)) strcat(string, LAV_ITEM_NAME_NONE, len);
    else if(type == (L_Items:L_House)) strcat(string, LAV_ITEM_NAME_HOUSE, len);
    else if(type == (L_Items:L_Bizz)) strcat(string, LAV_ITEM_NAME_BIZZ, len);
    else if(type == (L_Items:L_Azs)) strcat(string, LAV_ITEM_NAME_AZS, len);
    else if(type == (L_Items:L_Mbizz)) strcat(string, LAV_ITEM_NAME_MBIZZ, len);
    else if(type == (L_Items:L_Auto)) strcat(string, LAV_ITEM_NAME_AUTO, len);
    else if(type == (L_Items:L_Acs)) strcat(string, LAV_ITEM_NAME_ACS, len);
    else if(type == (L_Items:L_Inv)) strcat(string, LAV_ITEM_NAME_INV, len);
    
    if(type == L_Items:L_House) format(string, len, "%s {ff4040}№%i", string, lavka_model);
    else if(type == L_Items:L_Bizz) format(string, len, "%s {ff4040}№%i", string, lavka_model);
    else if(type == L_Items:L_Azs) format(string, len, "%s {ff4040}№%i", string, lavka_model);
    else if(type == L_Items:L_Mbizz) format(string, len, "%s {ff4040}№%i", string, lavka_model);
    else if(type == L_Items:L_Auto) format(string, len, "%s {ff4040}%s [ID: {a0db8e}%i{ff4040}]", string, CarName[GetVehicleModel(lavka_model)-400], lavka_model);
    else if(type == L_Items:L_Acs) 
    {
        if(with_amount) format(string, len, "%s {ff4040}%s {a0db8e}(%i шт.)", string, AcsInvType[lavka_model][acsinvText], lavka_amount);
        else format(string, len, "%s {ff4040}%s", string, AcsInvType[lavka_model][acsinvText]);
    }
    else if(type == L_Items:L_Inv)
    {
        if(with_amount) format(string, len, "%s {ff4040}%s {a0db8e}(%i шт.)", string, InvType[lavka_model][invText], lavka_amount);
        else format(string, len, "%s {ff4040}%s", string, InvType[lavka_model][invText]);
    }

    return 1;
}

stock ClearLavkaEditVars(playerid)
{
    set_lavka_edit_type(playerid, L_Items:L_None);
    set_lavka_edit_model(playerid, 0);
    set_lavka_edit_value(playerid, 0);
    set_lavka_workitem(playerid, 0);
    set_player_choosing_lavkainv(playerid, 0);
    return 1;
}
stock ClearLavkaCustomerVars(playerid)
{
    set_lavka_customer_type(playerid, L_Items:L_None);
    set_lavka_customer_id(playerid, 0);
    set_lavka_workitem(playerid, 0);
    set_lavka_customer_model(playerid, 0);
    set_lavka_customer_value(playerid, 0);
    set_lavka_customer_amount(playerid, 0);
    return 1;
}

stock ChooseSaleItem_DLGInventory(playerid)
{
    ShowPlayerDialog(playerid, dLavkaEdit_InvInfo, DIALOG_STYLE_MSGBOX, "{ff4040}Рынок", "\n\
    {ffffff}Пожалуйста, ознакомьтесь с инструкцией добавления предмета на продажу:\n\n\
    После нажатия на кнопку \"Далее\", будет открыт инвентарь, в котором Вам нужно будет:\n\n\
    {ff4040}1.{ffffff} Выбрать желаемый Вам предмет на продажу.\n\
    {ff4040}2.{ffffff} Нажать на него\n\
    {ff4040}3.{ffffff} Нажать на кнопку \"USE\"\n\
    {ff4040}4.{ffffff} Ввести кол-во предмета\n\n\
    После выполнения этих действий, товар будет выставлен на продажу.", "Далее", "Назад");
    return 1;
}
stock ChooseSaleItem_Inventory(playerid)
{
    set_player_choosing_lavkainv(playerid, 1);
    set_lavka_edit_model(playerid, 0);

    ShowInv(playerid);

    set_lavka_edit_type(playerid, L_Items:L_Inv);
    return 1;
}
stock LavkaSaleItem_Set(playerid, ident)
{
    set_player_choosing_lavkainv(playerid, 0);
    set_lavka_edit_model(playerid, ident);
    new string[1024];
    new total = CountItem(playerid, ident);

    format(string, sizeof(string), "\n\
    {ffffff}Выбранный предмет: {a0db8e}%s\n\
    {ffffff}Кол-во в инвентаре: {a0db8e}%i шт.\n\n\
    {ffffff}Введите количество, которое Вы желаете продать в поле ввода ниже:", InvType[ident][invText], total);

    ShowPlayerDialog(playerid, dLavkaEdit_InvInput, DIALOG_STYLE_INPUT, "{ff4040}Рынок", string, "Далее", "Назад");
    return 1;
}

//
stock ChooseSaleItem_DLGAcsInventory(playerid)
{
    ShowPlayerDialog(playerid, dLavkaEdit_AcsInvInfo, DIALOG_STYLE_MSGBOX, "{ff4040}Рынок", "\n\
    {ffffff}Пожалуйста, ознакомьтесь с инструкцией добавления предмета на продажу:\n\n\
    После нажатия на кнопку \"Далее\", будет открыт инвентарь, в котором Вам нужно будет:\n\n\
    {ff4040}1.{ffffff} Выбрать желаемый Вам предмет на продажу.\n\
    {ff4040}2.{ffffff} Нажать на него\n\
    {ff4040}3.{ffffff} Нажать на кнопку \"DRESS\"\n\
    {ff4040}4.{ffffff} Ввести кол-во предмета\n\n\
    После выполнения этих действий, товар будет выставлен на продажу.", "Далее", "Назад");
    return 1;
}
stock ChooseSaleItem_AcsInventory(playerid)
{
    set_player_choosing_lavkainv(playerid, 1);
    set_lavka_edit_model(playerid, 0);

    ShowAcsInv(playerid);

    set_lavka_edit_type(playerid, L_Items:L_Acs);
    return 1;
}
stock LavkaSaleItem_MainDLG(playerid)
{
    new lavkaid = get_lavka_customer_id(playerid);
    new item = get_lavka_workitem(playerid);

    new string[512], typetext[64], itemtext[64];
    get_litems_type_name(LavkaItems[lavkaid][item][lType], typetext);
    get_litems_item_name(LavkaItems[lavkaid][item][lType], LavkaItems[lavkaid][item][lModel], LavkaItems[lavkaid][item][lAmount], itemtext, 0);
    new totalPrice = LavkaItems[lavkaid][item][lPrice] * get_lavka_customer_amount(playerid);

    format(string, sizeof(string), "\n\
        {ffffff}Пожалуйста, подтвердите покупку:\n\
        {CC183F}Обратите внимание: {ffffff}Оплата возможна {a0db8e}только наличными{ffffff}\n\n\
        \
        {ffffff}Тип предмета: {ffd700}%s\n\
        {ffffff}Наименование: {c6e2ff}%s\n\
        {ffffff}Количество на рынке: {ffd700}%i шт.\n\
        {ffffff}В Вашей корзине: {ffd700}%i шт.\n\n\
        \
        {ffffff}Стоимость покупки за одну единицу: {a0db8e}$ %i\n\
        {ffffff}Общая сумма покупки: {a0db8e}$ %i\n\n\
        {ffffff}Ваш баланс: {a0db8e}$ %i\n\
        {ffffff}Ваш баланс после покупки: {a0db8e}$ %i\
        ", 
    typetext, 
    itemtext, 
    LavkaItems[lavkaid][item][lAmount],
    get_lavka_customer_amount(playerid),
    LavkaItems[lavkaid][item][lPrice],
    totalPrice,
    PlayerInfo[playerid][pMoney],
    PlayerInfo[playerid][pMoney] - totalPrice);

    if(PlayerInfo[playerid][pMoney] < totalPrice) 
        strcat(string, "\n\n{CC183F}Покупка предмета невозможна:{ffffff} У Вас недостаточно денег");

    ShowPlayerDialog(playerid, dLavkaCustomer_Buy, DIALOG_STYLE_MSGBOX, "{ff4040}Рынок", string, "Купить", "Назад");
    return 1;
}
stock LavkaSaleAcsItem_Set(playerid, ident)
{
    set_player_choosing_lavkainv(playerid, 0);
    set_lavka_edit_model(playerid, ident);
    new string[1024];
    new total = CountAcsItem(playerid, ident);

    format(string, sizeof(string), "\n\
    {ffffff}Выбранный предмет: {a0db8e}%s\n\
    {ffffff}Кол-во в инвентаре аксессуаров: {a0db8e}%i шт.\n\n\
    {ffffff}Введите количество, которое Вы желаете продать в поле ввода ниже:", 
    AcsInvType[ident][acsinvText], total);

    ShowPlayerDialog(playerid, dLavkaEdit_AcsInvInput, DIALOG_STYLE_INPUT, "{ff4040}Рынок", string, "Далее", "Назад");
    return 1;
}

stock is_action_blocked(playerid, L_Items:type)
{
    new lavkaid = get_player_lavka(playerid);
    if(lavkaid == -1) return 0;
    if(Lavka[lavkaid][li_OwnerID] == INVALID_PLAYER_ID) return -1;

    for(new item = 0; item < MAX_LAVKA_ITEMS; item++)
    {
        if(LavkaItems[lavkaid][item][lType] != type) continue;
        return 1;
    }
    return 0;
}

stock InputLavkaItemAmount_ToBuy(playerid)
{
    new string[1024];
    new lavkaid = get_lavka_customer_id(playerid);
    new item = get_lavka_workitem(playerid);
    new total = LavkaItems[lavkaid][item][lAmount];
    new model = get_lavka_customer_model(playerid);

    switch(LavkaItems[lavkaid][item][lType])
    {
        case (L_Items:L_Inv):
        {
            format(string, sizeof(string), "\n\
            {ffffff}Выбранный предмет: {a0db8e}%s\n\
            {ffffff}Кол-во на рынке: {a0db8e}%i шт.\n\n\
            {ffffff}Введите количество, которое Вы желаете приобрести в поле ввода ниже:",
            InvType[model][invText], total);
        }
        case (L_Items:L_Acs):
        {
            format(string, sizeof(string), "\n\
            {ffffff}Выбранный предмет: {a0db8e}%s\n\
            {ffffff}Кол-во на рынке: {a0db8e}%i шт.\n\n\
            {ffffff}Введите количество, которое Вы желаете приобрести в поле ввода ниже:",
            AcsInvType[model][acsinvText], total);
        }
    }

    ShowPlayerDialog(playerid, dLavkaCustomer_Buy2, DIALOG_STYLE_INPUT, "{ff4040}Рынок", string, "Далее", "Назад");
    return 1;
}
stock BuyLavkaItem(playerid, lavkaid, L_Items:type, item, amount)
{
    if(lavkaid == -1) return 0;
    if(amount > LavkaItems[lavkaid][item][lAmount]) {
        error(playerid, "На рынке раскупили данный товар в выбранном Вами количестве!");
        ClearLavkaCustomerVars(playerid);
        return 1;
    }

    new typetext[64], itemtext[64];
    get_litems_type_name(LavkaItems[lavkaid][item][lType], typetext);
    get_litems_item_name(LavkaItems[lavkaid][item][lType], LavkaItems[lavkaid][item][lModel], amount, itemtext);
    new ownerid = Lavka[lavkaid][li_OwnerID];
    
    switch(L_Items:type)
    {
        case (L_Items:L_House):
        {
            if(PlayerInfo[playerid][pPhousekey] != 9999) return error(playerid, "У тебя уже есть дом.");
            if(LavkaItems[lavkaid][item][lModel] != PlayerInfo[playerid][pPbiskey]) return error(playerid, "Данный предмет был снят с продажи");

            PlayerInfo[playerid][pPhousekey] = PlayerInfo[ownerid][pPhousekey];
            PlayerInfo[ownerid][pPhousekey] = 9999;
            UpdatePlayerData(playerid, "House", PlayerInfo[playerid][pPhousekey]);
            UpdatePlayerData(ownerid, "House", PlayerInfo[ownerid][pPhousekey]);

            HouseInfo[PlayerInfo[playerid][pPhousekey]][hOwned] = 1;
            strmid(HouseInfo[PlayerInfo[playerid][pPhousekey]][hOwner], PlayerInfo[playerid][pNames], 0, MAX_PLAYER_NAME, 255);

            new year, month, day;
            getdate(year, month, day);
            HouseInfo[PlayerInfo[playerid][pPhousekey]][hOnlineDay] = day;
            HouseInfo[PlayerInfo[playerid][pPhousekey]][hOnlineMonth] = month;
            HouseInfo[PlayerInfo[playerid][pPhousekey]][hOnlineYear] = year;

            UpdateHouse(PlayerInfo[playerid][pPhousekey]);
        }
        case (L_Items:L_Bizz):
        {
            if(PlayerInfo[playerid][pPbiskey] != 9999) return error(playerid, "У тебя уже есть бизнес.");
            if(LavkaItems[lavkaid][item][lModel] != PlayerInfo[playerid][pPbiskey]) return error(playerid, "Данный предмет был снят с продажи");
            new bizid = PlayerInfo[ownerid][pPbiskey];

            PlayerInfo[playerid][pPbiskey] = bizid;
            PlayerInfo[playerid][pBuyDos] = 1;
            PlayerInfo[ownerid][pPbiskey] = 9999;

            if(bizid >= 100)
            {
                SBizzInfo[bizid-100][sbOwned] = 1;
                strmid(SBizzInfo[bizid-100][sbOwner], PlayerInfo[playerid][pNames], 0, MAX_PLAYER_NAME, 255);
                UpdateSBizz(bizid-100);
            }
            else
            {
                BizzInfo[bizid][bOwned] = 1;
                strmid(BizzInfo[bizid][bOwner], PlayerInfo[playerid][pNames], 0, MAX_PLAYER_NAME, 255);

                new year, month, day; 
                getdate(year, month, day);
                BizzInfo[bizid][bOnlineDay] = day;
                BizzInfo[bizid][bOnlineMonth] = month;
                BizzInfo[bizid][bOnlineYear] = year;

                UpdateBizz(bizid);
            }

            UpdatePlayerData(playerid, "Bizz", PlayerInfo[playerid][pPbiskey]);
            UpdatePlayerData(playerid, "BuyDos", PlayerInfo[playerid][pBuyDos]);
            UpdatePlayerData(ownerid, "Bizz", PlayerInfo[ownerid][pPbiskey]);
        }
        case (L_Items:L_Azs):
        {
            if(PlayerInfo[playerid][pPAZSkey] != 9999) return error(playerid, "У тебя уже есть АЗС.");
            if(LavkaItems[lavkaid][item][lModel] != PlayerInfo[playerid][pPAZSkey]) return error(playerid, "Данный предмет был снят с продажи");

            PlayerInfo[playerid][pPAZSkey] = PlayerInfo[ownerid][pPAZSkey];
            UpdatePlayerData(playerid, "AZS", PlayerInfo[playerid][pPAZSkey]);

            SAZSInfo[PlayerInfo[playerid][pPAZSkey]][sAZSOwned] = 1;
            strmid(SAZSInfo[PlayerInfo[playerid][pPAZSkey]][sAZSOwner], PlayerInfo[playerid][pNames], 0, MAX_PLAYER_NAME, 255);

            PlayerInfo[ownerid][pPAZSkey] = 9999;
            UpdatePlayerData(ownerid, "AZS", PlayerInfo[ownerid][pPAZSkey]);

            PlayerInfo[playerid][pBuyDos] = 1;
            UpdatePlayerData(playerid, "BuyDos", PlayerInfo[playerid][pBuyDos]);
            
            UpdateSAZS();
        }
        case (L_Items:L_Mbizz):
        {
            if(PlayerInfo[playerid][pPMNPkey] != 255) return error(playerid, "У тебя уже есть магазин незаконной продукции.");
            if(LavkaItems[lavkaid][item][lModel] != PlayerInfo[playerid][pPMNPkey]) return error(playerid, "Данный предмет был снят с продажи");

            PlayerInfo[playerid][pPMNPkey] = PlayerInfo[ownerid][pPMNPkey];
            UpdatePlayerData(playerid, "MNP", PlayerInfo[playerid][pPMNPkey]);

            SMNPInfo[ PlayerInfo[playerid][pPMNPkey] ][sMNPOwned] = 1;
            strmid(SMNPInfo[ PlayerInfo[playerid][pPMNPkey] ][sMNPOwner], PlayerInfo[playerid][pNames], 0, strlen(PlayerInfo[playerid][pNames]), 255);
            
            PlayerInfo[ownerid][pPMNPkey] = 255; 
            UpdatePlayerData(ownerid, "MNP", PlayerInfo[ownerid][pPMNPkey]);
            
            PlayerInfo[playerid][pBuyDos] = 1; 
            UpdatePlayerData(playerid, "BuyDos", PlayerInfo[playerid][pBuyDos]);

            UpdateSMNP(PlayerInfo[playerid][pPMNPkey]);
        }
        case (L_Items:L_Auto):
        {
            BuyAutoFromLavka_SlotChoose(playerid);
            return 1;
        }
        case (L_Items:L_Inv):
        {
            if(LavkaItems[lavkaid][item][lAmount] < amount) return error(playerid, "Данный предмет был снят с продажи");

            LavkaItems[lavkaid][item][lAmount] -= amount;
            if(LavkaItems[lavkaid][item][lAmount] <= 0) ClearLavkaItem(lavkaid, item);

            AddItem(playerid, LavkaItems[lavkaid][item][lModel], amount);
        }
        case (L_Items:L_Acs):
        {
            if(LavkaItems[lavkaid][item][lAmount] < amount) return error(playerid, "Данный предмет был снят с продажи");

            LavkaItems[lavkaid][item][lAmount] -= amount;
            if(LavkaItems[lavkaid][item][lAmount] <= 0) ClearLavkaItem(lavkaid, item);

            AddAcsItem(playerid, LavkaItems[lavkaid][item][lModel], amount);
        }
    }

    new price = LavkaItems[lavkaid][item][lPrice];
    GiveMoney(playerid, -price);
    GiveMoney(ownerid, price);

    Tipp(playerid, "Вы успешно приобрели {ffd700}%s{ffffff}: {c6e2ff}%s {ffffff}за {a0db8e}$ %i", typetext, itemtext, price);
    Tipp(ownerid, "Вы успешно продали {ffd700}%s{ffffff}: {c6e2ff}%s {ffffff}за {a0db8e}$ %i", typetext, itemtext, price);
    ClearLavkaCustomerVars(playerid);

    new string[1024*3];
    LavkaGenerate_3DText(lavkaid, string);
    UpdateDynamic3DTextLabelText(Lavka[lavkaid][li_3DText], -1, string);
    return 1;
}
stock BuyAutoFromLavka_SlotChoose(playerid)
{
    new string[1024], v;
    strcat(string, "{ccff00}Слот\t{ccff00}Статус\n");

    for(new i = 0; i < 5; i++)
    {
        if(PlayerInfo[playerid][pCarKey][i] != -1)
        {
            v = PlayerInfo[playerid][pCarKey][i];
            format(string, sizeof(string), "%s{00ff7f}%i.\t{ffffff}[ %s {a0db8e}(ID: {ffffff}%i{a0db8e}) {ffffff}]\n", 
                string, i + 1, CarName[GetVehicleModel(VehicleInfo[v][vRealID])-400], GetVehicleModel(VehicleInfo[v][vRealID]));
        } 
        else format(string, sizeof(string), "%s{00ff7f}%i.\t{ffffff}[ {a0db8e}Слот пустой {ffffff}]\n", string, i + 1);
    }

    ShowPlayerDialog(playerid, dLavkaCustomer_Buy3, DIALOG_STYLE_TABLIST_HEADERS, "{ff4040}Рынок", string, "Выбрать", "Закрыть");
    return 1;
}