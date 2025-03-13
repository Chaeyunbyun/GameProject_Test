#include "Npc_Game.h"
#include <iostream>
#include <vector>
#include <random>
#include <conio.h> // Windows���� ����Ű �Է��� ���� ���
#include <random>
#include <algorithm> // ���� �� ���

#include "GameProject.h"
/// NPC ����, ���� �̺�Ʈ

// npc �� �̺�Ʈ �б�
int Npc_Game::NpcEvent(int NpcNo)
{
    // ����
	if (NpcNo == 0)
	{
        cout << " -- [����]�� �������ϴ�. -- " << "\n";
		int ret = ShopEvent();
        return ret;
	}
    // ������ �θ�
	if (NpcNo == 1)
	{
        cout << " -- [������ �θ�]�� �������ϴ�.. -- " << "\n";

        if (Myjob == "rogue")
        {
            cout << "������ �θ�: ��," << MyName + " ���ݾ�? �� �׳� �����ٰ�" << "\n";
            _getch();
            return 100;
        }
        cout << "������ �θ�: �� ����. 30�� �ָ� �����ٰ�." << "\n";

        if (MyMoney>= 30)
        {
            cout << " -- 30���� �Ҿ����ϴ�. -- " << "\n";
            MyMoney -= 30;
            _getch();
            return 100;
        }
        else
        {
            cout << "������ �θ�: ���� �� ����? �׷� ����̶� ��������!" << "\n";
            MyMoney = 0;
            _getch();
            return -100;
        }
	}
    // Ÿ�뽺
    if (NpcNo == 2)
    {
        cout << " -- [Ÿ�뽺]�� �������ϴ�.. -- " << "\n";
        cout << "Ÿ�뽺: ���� �ΰ� ������ �ϸ� 50%�� �η��� �Ҹ��Ѵ�" << "\n";
        cout << "Ÿ�뽺: �� ��Ƴ��� �� ������?" << "\n";
        cout << "\n";
        cout << "     -- ��! --" << "\n";
        cout << "------------------" << "\n";
        cout << "\n";

        _getch();

        // �õ尪�� ��� ���� random_device ����.
        std::random_device rdItem;

        // random_device �� ���� ���� ���� ������ �ʱ�ȭ �Ѵ�.
        std::mt19937 genItem(rdItem());

        // 0 ���� 99 ���� �յ��ϰ� ��Ÿ���� �������� �����ϱ� ���� �յ� ���� ����.
        std::uniform_int_distribution<int> disItem(0, 99);

        // ������ �� �� �����ؼ� ������ ����
        int randItemValue = disItem(genItem);

        // ����
        if (randItemValue < 50)
        {
            return 100;
        }
        else
        {
            return -100;
        }
    }
}

Npc_Game::Itemcost iteminfo[5];
Npc_Game::ItemDisc itemdisc[5];

void ItemCostInfo()
{
    // itemcode, itemname, itemcost
    iteminfo[0] = { 0,"��������",50};
    iteminfo[1] = { 1,"�Ķ�����",50};
    iteminfo[2] = { 2,"����",300};
    iteminfo[3] = { 3,"��",10 };
    iteminfo[4] = { 4,"����",20};
}

void ItemDiscInfo()
{
    itemdisc[0] = { 0,"Hp�� 10 ȸ����Ų��." };
    itemdisc[1] = { 1,"Mp�� 10 ȸ����Ų��." };
    itemdisc[2] = { 2,"[���ǹ�]���� �������� �������� ��������" };
    itemdisc[3] = { 3,"����� ��. ȿ���� ����δ�..." };
    itemdisc[4] = { 4,"�ұ��� ������ ���� �����̴�" };

}

void Npc_Game::LoadShopItemInfo()
{
    ItemCostInfo();
    ItemDiscInfo();

    // ��ü ������ ���� ��ŭ �߰�
    for (int i = 0; i < 5; i++)
    {
        AllItem.push_back(iteminfo[i].strItemName);
        MapItem.insert(make_pair(iteminfo[i].strItemName, iteminfo[i].intItemNo));
    }
}

// ������ ���� ���� / ���� ó��
// ������ ��� true �ƴϸ� false
// ������ ��� ��ǰ��Ͽ��� ����
bool ShowItemDisc(int itemCode, vector<string> &RuleItem)
{
    cout << "����: " << itemdisc[itemCode].strItemDisc << "\n";
    cout << "����: " << iteminfo[itemCode].intCost << "\n";
    cout << "0: �����ϱ�\n1: ���ư���" << endl;

    char key = _getch(); // Ű �Է� �ޱ�

    if (key == '0')
    {
        if (MyMoney < iteminfo[itemCode].intCost)
        {
            cout << "�ܾ��� �����մϴ�." << "\n";
            _getch();
            return false;
        }
        cout << iteminfo[itemCode].strItemName <<" �����Ͽ����ϴ�.\n";
        MyMoney -= iteminfo[itemCode].intCost;
        RuleItem.erase(RuleItem.begin() + itemCode);

        _getch();
        return true;
    }
    else if (key == '1')
    {
        return false; // ����
    }
    else
    {
        return false; // ����
    }
}

// ���� �̺�Ʈ
// ������ ��� ���� ó���� ���� itemNo�� ����
// ���� ���� ������ 100 ����
int Npc_Game::ShopEvent()
{
    vector<string> RuleItem;
    int pntPosition = 0;

    // ���� �õ� ����
    random_device rd;
    mt19937 gen(rd());

    // ����
    shuffle(AllItem.begin(), AllItem.end(), gen);

    // �տ��� 3���� ����
    RuleItem.assign(AllItem.begin(), AllItem.begin() + 3);

    RuleItem.push_back("���ư���");

    while (true)
    {
        // �޴� ���
        system("cls"); // Windows������ "cls"�� ����
        cout << "����: �����ϰ� ���� ������ �����ϼ���." << "(���� ������: "<< MyMoney << ")" << endl;

        string pntItemName = "";

        for (int i = 0; i < RuleItem.size(); i++) {
            if (i == pntPosition) {
                cout << ">> " << RuleItem[i] << endl;
                pntItemName = RuleItem[i];
            }
            else {
                cout << "   " << RuleItem[i] << endl;
            }
        }

        // Ű �Է� ó��
        int key = _getch(); // Ű �Է��� ����
        if (key == 72) { // ���� ����Ű
            pntPosition = (pntPosition - 1 + RuleItem.size()) % RuleItem.size();
        }
        else if (key == 80) { // �Ʒ��� ����Ű
            pntPosition = (pntPosition + 1) % RuleItem.size();
        }
        else if (key == 13)
        { // Enter Ű
         // ���õ� �޴��� ���� ���� ����
            // 1��
            if (pntPosition >= 0 && pntPosition <3)
            {
                int itemNo = MapItem[pntItemName];
                if (ShowItemDisc(itemNo, RuleItem))
                {
                    return itemNo;
                }
                else
                {
                    continue;
                }
            }
            if (pntPosition == 3)
            {
                cout << "������ �����ϴ�...\n";
                _getch(); 
                return 100;
            }
        }
    }
}