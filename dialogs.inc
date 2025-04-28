#define dLavkaEdit_1 5135
#define dLavkaEdit_2 5136
#define dLavkaEdit_Price 5137
#define dLavkaEdit_3 5138
#define dLavkaEdit_Cancel 5139
#define dLavkaEdit_Vehicle1 5140
#define dLavkaEdit_InvInfo 5141
#define dLavkaEdit_InvInput 5142
#define dLavkaEdit_InvConfirm 5143

#define dLavkaEdit_AcsInvInfo 5144
#define dLavkaEdit_AcsInvInput 5145
#define dLavkaEdit_AcsInvConfirm 5146

#define dLavkaCustomer_Main 5147
#define dLavkaCustomer_Buy 5148
#define dLavkaCustomer_Buy2 5149
#define dLavkaCustomer_Buy3 5150

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid)
    {
        case dLavkaCustomer_Buy3:
        {
            if(!response) return 1;
            if(PlayerInfo[playerid][pCarKey][listitem] != -1) return error(playerid, "В данном слоте у Вас уже есть автомобиль!"), BuyAutoFromLavka_SlotChoose(playerid);
            new lavkaid = get_lavka_customer_id(playerid);
            new item = get_lavka_workitem(playerid);
            new price = LavkaItems[lavkaid][item][lPrice];
            if(PlayerInfo[playerid][pMoney] < price) return error(playerid, "{CC183F}Покупка предмета невозможна:{ffffff} У Вас недостаточно денег");
            if(LavkaItems[lavkaid][item][lType] == L_Items:L_None) return error(playerid, "Товар был снят с продажи продацом или его уже купили"), ClearLavkaCustomerVars(playerid);

            if(LavkaItems[lavkaid][item][lModel] != get_lavka_customer_model(playerid)) return error(playerid, "Товар был снят с продажи продацом или его уже купили"), ClearLavkaCustomerVars(playerid);
            if(LavkaItems[lavkaid][item][lValue] != get_lavka_customer_value(playerid)) return error(playerid, "Товар был снят с продажи продацом или его уже купили"), ClearLavkaCustomerVars(playerid);
            if(LavkaItems[lavkaid][item][lType] != get_lavka_customer_type(playerid)) return error(playerid, "Товар был снят с продажи продацом или его уже купили"), ClearLavkaCustomerVars(playerid);

            new buyer_slot = listitem;
            new owner_slot = LavkaItems[lavkaid][item][lValue];
            new ownerid = Lavka[lavkaid][li_OwnerID];
            if(buyer_slot == 2 && PlayerInfo[playerid][pCarSlots] == 0)return error(playerid, "Слот не куплен. Приобретите его в /donate"),RemovePlayerFromVehicle(playerid);
			if(buyer_slot == 3 && PlayerInfo[playerid][pCarSlots] <= 1)return error(playerid, "Слот не куплен. Приобретите его в /donate"),RemovePlayerFromVehicle(playerid);
			if(buyer_slot == 4 && PlayerInfo[playerid][pCarSlots] <= 2)return error(playerid, "Слот не куплен. Приобретите его в /donate"),RemovePlayerFromVehicle(playerid);
            new carkey = PlayerInfo[ownerid][pCarKey][owner_slot];

            new typetext[64], itemtext[64];
            get_litems_type_name(LavkaItems[lavkaid][item][lType], typetext);
            get_litems_item_name(LavkaItems[lavkaid][item][lType], LavkaItems[lavkaid][item][lModel], 1, itemtext);

            GiveMoney(playerid, -price);
            GiveMoney(ownerid, price);

            PlayerInfo[ownerid][pCarKey][owner_slot] = -1;
            PlayerInfo[playerid][pCarKey][buyer_slot] = carkey;
            format(VehicleInfo[carkey][OwnerName], 24 + 1, "%s", Name(playerid));

            PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
            SaveParkingSystem();
			updateVehicleInfo();
			OnPlayerSave(playerid);
			OnPlayerSave(ownerid);

            Tipp(playerid, "Вы успешно приобрели {ffd700}%s{ffffff}: {c6e2ff}%s {ffffff}за {a0db8e}$ %i", typetext, itemtext, price);
            Tipp(ownerid, "Вы успешно продали {ffd700}%s{ffffff}: {c6e2ff}%s {ffffff}за {a0db8e}$ %i", typetext, itemtext, price);
            ClearLavkaCustomerVars(playerid);
            ClearLavkaItem(lavkaid, item);

            new string[1024*3];
            LavkaGenerate_3DText(lavkaid, string);
            UpdateDynamic3DTextLabelText(Lavka[lavkaid][li_3DText], -1, string);
        }
        case dLavkaCustomer_Main:
        {
            if(!response) return 1;

            set_lavka_workitem(playerid, listitem);
            new lavkaid = get_lavka_customer_id(playerid);
            new item = get_lavka_workitem(playerid);

            if(LavkaItems[lavkaid][item][lType] != L_Items:L_None)
            {
                set_lavka_customer_model(playerid, LavkaItems[lavkaid][item][lModel]);
                set_lavka_customer_value(playerid, LavkaItems[lavkaid][item][lValue]);
                set_lavka_customer_type(playerid, LavkaItems[lavkaid][item][lType]);

                new amount = LavkaItems[lavkaid][item][lAmount];
                if(amount == 1)
                {
                    set_lavka_customer_amount(playerid, 1);
                    LavkaSaleItem_MainDLG(playerid);
                }
                else InputLavkaItemAmount_ToBuy(playerid);
            }
            else ShowPlayerLavkaItems(playerid, lavkaid);
        }
        case dLavkaCustomer_Buy:
        {
            if(!response) return ShowPlayerLavkaItems(playerid, get_lavka_customer_id(playerid));

            new lavkaid = get_lavka_customer_id(playerid);
            new item = get_lavka_workitem(playerid);
            new price = LavkaItems[lavkaid][item][lPrice];
            if(PlayerInfo[playerid][pMoney] < price) return error(playerid, "{CC183F}Покупка предмета невозможна:{ffffff} У Вас недостаточно денег");
            if(LavkaItems[lavkaid][item][lType] == L_Items:L_None) return error(playerid, "Товар был снят с продажи продацом или его уже купили"), ClearLavkaCustomerVars(playerid);

            if(LavkaItems[lavkaid][item][lModel] != get_lavka_customer_model(playerid)) return error(playerid, "Товар был снят с продажи продацом или его уже купили"), ClearLavkaCustomerVars(playerid);
            if(LavkaItems[lavkaid][item][lValue] != get_lavka_customer_value(playerid)) return error(playerid, "Товар был снят с продажи продацом или его уже купили"), ClearLavkaCustomerVars(playerid);
            if(LavkaItems[lavkaid][item][lType] != get_lavka_customer_type(playerid)) return error(playerid, "Товар был снят с продажи продацом или его уже купили"), ClearLavkaCustomerVars(playerid);
            
            BuyLavkaItem(playerid, lavkaid, LavkaItems[lavkaid][item][lType], item, get_lavka_customer_amount(playerid));
        }
        case dLavkaCustomer_Buy2:
        {
            if(!response) return ShowPlayerLavkaItems(playerid, get_lavka_customer_id(playerid));

            new lavkaid = get_lavka_customer_id(playerid);
            new item = get_lavka_workitem(playerid);
            new total = LavkaItems[lavkaid][item][lAmount];
            new input = strval(inputtext);

            if(input > total) {
                Error(playerid, "Вы превысили максимальное количество %i единиц", total);
                InputLavkaItemAmount_ToBuy(playerid);
                return 1;
            }
            if(PlayerInfo[playerid][pMoney] < LavkaItems[lavkaid][item][lPrice]) return error(playerid, "{CC183F}Покупка предмета невозможна:{ffffff} У Вас недостаточно денег");

            set_lavka_customer_amount(playerid, input);
            LavkaSaleItem_MainDLG(playerid);
        }
        //
        case dLavkaEdit_AcsInvInfo:
        {
            if(!response) return LavkaEdit_ChooseType(playerid);
            ChooseSaleItem_AcsInventory(playerid);
        }
        case dLavkaEdit_AcsInvInput:
        {
            if(!response) return LavkaEdit_ChooseType(playerid);
            new ident = get_lavka_edit_model(playerid);
            new total = CountAcsItem(playerid, ident);
            new input = strval(inputtext);

            if(input > total)
            {
                Error(playerid, "Неверное количество! Максимум: %i", total);
                LavkaSaleAcsItem_Set(playerid, ident);
                return 1;
            }
            if(input <= 0)
            {
                error(playerid, "Неверное количество! Минимум: 1");
                LavkaSaleAcsItem_Set(playerid, ident);
                return 1;
            }
            set_lavka_edit_value(playerid, input);

            new string[1024];
            format(string, sizeof(string), "\n\
            {ffffff}Выбранный предмет: {a0db8e}%s\n\
            {ffffff}Кол-во в инвентаре аксессуаров: {a0db8e}%i шт.\n\
            {ffffff}Из них выбрано для продажи: {a0db8e}%i шт.\n\
            {ffffff}Останется у Вас: {a0db8e}%i шт.\n\
            \n\
            {ffffff}Вы действительно желаете выставить предмет на продажу?", 
            AcsInvType[ident][acsinvText], total, input, total - input);

            ShowPlayerDialog(playerid, dLavkaEdit_InvConfirm, DIALOG_STYLE_MSGBOX, "{ff4040}Рынок", string, "Да", "Нет");
        }
        case dLavkaEdit_AcsInvConfirm:
        {
            if(!response) return LavkaEdit_ChooseType(playerid);
            ShowLavkaInputPrice(playerid);
        }
        //
        case dLavkaEdit_InvConfirm:
        {
            if(!response) return LavkaEdit_ChooseType(playerid);
            ShowLavkaInputPrice(playerid);
        }
        case dLavkaEdit_InvInput:
        {
            if(!response) return LavkaEdit_ChooseType(playerid);
            new ident = get_lavka_edit_model(playerid);
            new total = CountItem(playerid, ident);
            new input = strval(inputtext);

            if(input > total)
            {
                Error(playerid, "Неверное количество! Максимум: %i", total);
                LavkaSaleItem_Set(playerid, ident);
                return 1;
            }
            if(input <= 0)
            {
                error(playerid, "Неверное количество! Минимум: 1");
                LavkaSaleItem_Set(playerid, ident);
                return 1;
            }
            set_lavka_edit_value(playerid, input);

            new string[1024];
            format(string, sizeof(string), "\n\
            {ffffff}Выбранный предмет: {a0db8e}%s\n\
            {ffffff}Кол-во в инвентаре: {a0db8e}%i шт.\n\
            {ffffff}Из них выбрано для продажи: {a0db8e}%i шт.\n\
            {ffffff}Останется у Вас: {a0db8e}%i шт.\n\
            \n\
            {ffffff}Вы действительно желаете выставить предмет на продажу?", 
            InvType[ident][invText], total, input, total - input);

            ShowPlayerDialog(playerid, dLavkaEdit_InvConfirm, DIALOG_STYLE_MSGBOX, "{ff4040}Рынок", string, "Да", "Нет");
        }
        case dLavkaEdit_InvInfo:
        {
            if(!response) return LavkaEdit_ChooseType(playerid);
            ChooseSaleItem_Inventory(playerid);
        }
        case dLavkaEdit_Vehicle1:
        {
            if(!response) return LavkaEdit_ChooseType(playerid);
            new lavkaid = get_player_lavka(playerid);
            
            if(PlayerInfo[playerid][pCarKey][listitem] == -1) return error(playerid, "В данном слоте у Вас нет автомобиля!"), LavkaChooseMyVehicle(playerid);
            new v = PlayerInfo[playerid][pCarKey][listitem];

            if(Lavka_IsItemOnSale(lavkaid, L_Items:L_Auto, VehicleInfo[v][vRealID], listitem) != -1) return LavkaChooseMyVehicle(playerid);
	        if(GosVehiclePrice(GetVehicleModel(VehicleInfo[v][vRealID])) == 0) return error(playerid, "Вы не можете продавать этот автомобиль!"), LavkaChooseMyVehicle(playerid);
            
            set_lavka_edit_type(playerid, L_Items:L_Auto);
            set_lavka_edit_model(playerid, VehicleInfo[v][vRealID]);
            set_lavka_edit_value(playerid, listitem); // slot
            ShowLavkaInputPrice(playerid);
        }
        case dLavkaEdit_1:
        {
            if(!response) return 1;
            if(!listitem)
            {
                ShowPlayerDialog(playerid, dLavkaEdit_Cancel, DIALOG_STYLE_MSGBOX, "{ff4040}Отказ от аренды ларька", "\n{ffffff}Вы действительно желаете отменить контракт аренды на ларёк?", "Да", "Нет");
                return 1;
            }

            listitem--;
            set_lavka_workitem(playerid, listitem);

            new lavkaid = get_player_lavka(playerid);
            new item = get_lavka_workitem(playerid);
            if(LavkaItems[lavkaid][item][lType] != L_Items:L_None)
            {
                new string[512], typetext[64], itemtext[64];
                get_litems_type_name(LavkaItems[lavkaid][item][lType], typetext);
                get_litems_item_name(LavkaItems[lavkaid][item][lType], LavkaItems[lavkaid][item][lModel], LavkaItems[lavkaid][item][lAmount], itemtext, 0);

                format(string, sizeof(string), "\n{ffffff}Пожалуйста, подтвердите действие:\n\n\n{ffffff}Снятие предмета\n{ffd700}%s{ffffff}: {c6e2ff}%s\n{ffffff}с продажи", typetext, itemtext);
                ShowPlayerDialog(playerid, dLavkaEdit_3, DIALOG_STYLE_MSGBOX, "{ff4040}Снять предмет с продажи", string, "Да", "Нет");
            }
            else LavkaEdit_ChooseType(playerid);
        }
        case dLavkaEdit_2:
        {
            if(!response) return LavkaEditPanel(playerid);
            new lavkaid = get_player_lavka(playerid);
            if(lavkaid == -1) return 1;

            switch(listitem)
            {
                case 0:
                {
                    if(PlayerInfo[playerid][pPhousekey] == 9999) return error(playerid, "У тебя нет дома."), LavkaEdit_ChooseType(playerid);
                    if(Lavka_IsItemOnSale(lavkaid, L_Items:L_House, PlayerInfo[playerid][pPhousekey], -1) != -1) return 0;
                    set_lavka_edit_type(playerid, L_Items:L_House);
                    set_lavka_edit_model(playerid, PlayerInfo[playerid][pPhousekey]);
                    ShowLavkaInputPrice(playerid);
                }
                case 1:
                {
                    if(PlayerInfo[playerid][pPbiskey] == 9999) return error(playerid, "У тебя нет бизнеса."), LavkaEdit_ChooseType(playerid);
                    if(Lavka_IsItemOnSale(lavkaid, L_Items:L_Bizz, PlayerInfo[playerid][pPbiskey], -1) != -1) return 0;
                    set_lavka_edit_type(playerid, L_Items:L_Bizz);
                    set_lavka_edit_model(playerid, PlayerInfo[playerid][pPbiskey]);
                    ShowLavkaInputPrice(playerid);
                }
                case 2:
                {
                    if(PlayerInfo[playerid][pPAZSkey] == 9999) return error(playerid, "У тебя нет заправочной станции."), LavkaEdit_ChooseType(playerid);
                    if(Lavka_IsItemOnSale(lavkaid, L_Items:L_Azs, PlayerInfo[playerid][pPAZSkey], -1) != -1) return 0;
                    set_lavka_edit_type(playerid, L_Items:L_Azs);
                    set_lavka_edit_model(playerid, PlayerInfo[playerid][pPAZSkey]);
                    ShowLavkaInputPrice(playerid);
                }
                case 3:
                {
                    if(PlayerInfo[playerid][pPMNPkey] == 255) return error(playerid, "У тебя нет магазинa незаконной продукции."), LavkaEdit_ChooseType(playerid);
                    if(Lavka_IsItemOnSale(lavkaid, L_Items:L_Mbizz, PlayerInfo[playerid][pPMNPkey], -1) != -1) return 0;
                    set_lavka_edit_type(playerid, L_Items:L_Mbizz);
                    set_lavka_edit_model(playerid, PlayerInfo[playerid][pPMNPkey]);
                    ShowLavkaInputPrice(playerid);
                }
                case 4: LavkaChooseMyVehicle(playerid);
                case 5: ChooseSaleItem_DLGAcsInventory(playerid);
                case 6: ChooseSaleItem_DLGInventory(playerid);
            }
        }
        case dLavkaEdit_Price:
        {
            if(!response) return LavkaEditPanel(playerid);
            new price = strval(inputtext);
            if(!strlen(inputtext)) return ShowLavkaInputPrice(playerid);
            if(price < LAVKA_MIN_SELLPRICE) return Tipp(playerid, "Минимальная стоимость для продажи товара: {a0db8e}$ %i", LAVKA_MIN_SELLPRICE), ShowLavkaInputPrice(playerid);
            if(price > LAVKA_MAX_SELLPRICE) return Tipp(playerid, "Минимальная стоимость для продажи товара: {a0db8e}$ %i", LAVKA_MAX_SELLPRICE), ShowLavkaInputPrice(playerid);

            new item = get_lavka_workitem(playerid);
            new lavkaid = get_player_lavka(playerid);
            new L_Items:selltype = get_lavka_edit_type(playerid);
            LavkaItems[lavkaid][item][lAmount] = 1;

            switch(L_Items:selltype)
            {
                case (L_Items:L_House): LavkaItems[lavkaid][item][lModel] = get_lavka_edit_model(playerid);
                case (L_Items:L_Bizz): LavkaItems[lavkaid][item][lModel] = get_lavka_edit_model(playerid);
                case (L_Items:L_Azs): LavkaItems[lavkaid][item][lModel] = get_lavka_edit_model(playerid);
                case (L_Items:L_Mbizz): LavkaItems[lavkaid][item][lModel] = get_lavka_edit_model(playerid);
                case (L_Items:L_Auto):
                {
                    LavkaItems[lavkaid][item][lModel] = get_lavka_edit_model(playerid);
                    LavkaItems[lavkaid][item][lValue] = get_lavka_edit_value(playerid);
                }
                case (L_Items:L_Inv):
                {
                    LavkaItems[lavkaid][item][lModel] = get_lavka_edit_model(playerid);
                    LavkaItems[lavkaid][item][lAmount] = get_lavka_edit_value(playerid);
                    TakeItem(playerid, LavkaItems[lavkaid][item][lModel], get_lavka_edit_value(playerid));
                }
                case (L_Items:L_Acs):
                {
                    LavkaItems[lavkaid][item][lModel] = get_lavka_edit_model(playerid);
                    LavkaItems[lavkaid][item][lAmount] = get_lavka_edit_value(playerid);
                    TakeAcsItem(playerid, LavkaItems[lavkaid][item][lModel], get_lavka_edit_value(playerid));
                }
            }

            LavkaItems[lavkaid][item][lType] = selltype;
            LavkaItems[lavkaid][item][lPrice] = price;

            new typetext[64], itemtext[64];
            get_litems_type_name(LavkaItems[lavkaid][item][lType], typetext);
            get_litems_item_name(LavkaItems[lavkaid][item][lType], LavkaItems[lavkaid][item][lModel], LavkaItems[lavkaid][item][lAmount], itemtext, 0);

            if(get_lavka_edit_type(playerid) == L_Items:L_Inv || get_lavka_edit_type(playerid) == L_Items:L_Acs)
                Tipp(playerid, "{ffd700}%s{ffffff}: {c6e2ff}%s{ffffff} успешно выставлен на продажу за {a0db8e}$ %i {ffffff}за одну единицу продукта", typetext, itemtext, LavkaItems[lavkaid][item][lPrice]);
            else
                Tipp(playerid, "{ffd700}%s{ffffff}: {c6e2ff}%s{ffffff} успешно выставлен на продажу за {a0db8e}$ %i", typetext, itemtext, LavkaItems[lavkaid][item][lPrice]);

            LavkaEditPanel(playerid);
        }
        case dLavkaEdit_3:
        {
            if(!response) return LavkaEditPanel(playerid);
            new item = get_lavka_workitem(playerid);
            new lavkaid = get_player_lavka(playerid);
            new typetext[64], itemtext[64];
            get_litems_type_name(LavkaItems[lavkaid][item][lType], typetext);
            get_litems_item_name(LavkaItems[lavkaid][item][lType], LavkaItems[lavkaid][item][lModel], LavkaItems[lavkaid][item][lAmount], itemtext, 0);

            Tipp(playerid, "{ffd700}%s{ffffff}: {c6e2ff}%s {ffffff}был успешно снят с продажи!", typetext, itemtext);
            ClearLavkaItem(lavkaid, item);
        }
        case dLavkaEdit_Cancel:
        {
            if(!response) return LavkaEditPanel(playerid);
            LavkaRentStop(get_player_lavka(playerid), 4);
        }
    }

    #if defined b_OnDialogResponse
		b_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
	#endif

    return 1;
}