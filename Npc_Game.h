#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

class Npc_Game
{
public:

    struct Itemcost
    {
        int intItemNo;
        string strItemName;
        int intCost;
    };

    struct ItemDisc
    {
        int intItemNo;
        string strItemDisc;
    };

	int NpcNo;
	string NpcName;
    int NpcAttkPnt;

    vector<string> AllItem;

    // item 이름으로 itemno 찾기
    unordered_map<string, int> MapItem;

	int NpcEvent(int NpcNo, bool isLotto);
	int ShopEvent();
    void LoadShopItemInfo();
};

