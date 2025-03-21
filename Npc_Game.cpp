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
// ����: �����۹�ȣ: �������� �κ��� �߰�
// ����: 100 - ó�� x
// ����: -100 - ���
// ����: -50 - ü�°���
int Npc_Game::NpcEvent(int NpcNo, bool isLotto)
{
    // ����
	if (NpcNo == 0)
	{
        cout << " -- [����]�� �������ϴ�. -- " << "\n";
		int ret = ShopEvent();
        _getch();
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
    if (NpcNo == 3)
    {
        cout << "���ǹ� ����: �ȳ��ϼ���" << "\n";
        cout << "���ǹ� ����: ������ �����;� ���� �����մϴ�." << "\n";

        if (isLotto == false)
        {
            return -100;
        }
        else
        {
            _getch();

            // �õ尪�� ��� ���� random_device ����.
            std::random_device rdItem;

            // random_device �� ���� ���� ���� ������ �ʱ�ȭ �Ѵ�.
            std::mt19937 genItem(rdItem());

            // 0 ���� 99 ���� �յ��ϰ� ��Ÿ���� �������� �����ϱ� ���� �յ� ���� ����.
            std::uniform_int_distribution<int> disItem(0, 99);

            // ������ �� �� �����ؼ� ������ ����
            int randItemValue = disItem(genItem);

            // ��÷
            if (randItemValue < 20)
            {
                return 101;
            }
            else
            {
                cout << "���ǹ� ����: �ƽ��׿�, ���� ��ȸ��..." << "\n";
                return 100;
            }
        }
    }
}

Npc_Game::Itemcost iteminfo[6];
Npc_Game::ItemDisc itemdisc[6];

// gameProject_test �� itemcode �� �������
//0,"��������"
//1,"�Ķ�����"
//2,"�̻��ѻ���"
//3,"��"
//4,"����"
//5,"����"
//6,"������Ʈ"

void ItemCostInfo()
{
    // 0: �ʵ忡���� �����
    // itemcode, itemname, itemcost
    iteminfo[0] = { 0,"��������",50};
    iteminfo[1] = { 1,"�Ķ�����",50};
    iteminfo[2] = { 2,"�̻��ѻ���",0};
    iteminfo[3] = { 3,"��",10 };
    iteminfo[4] = { 4,"����",20};
    iteminfo[5] = { 5,"����",150 };
}

void ItemDiscInfo()
{
    itemdisc[0] = { 0,"Hp�� 10 ȸ����Ų��." };
    itemdisc[1] = { 1,"Mp�� 10 ȸ����Ų��." };
    itemdisc[2] = { 2,"������ 1 ����" };
    itemdisc[3] = { 3,"����� ��. ȿ���� ����δ�..." };
    itemdisc[4] = { 4,"�ұ��� ������ ���� �����̴�" };
    itemdisc[5] = { 5,"[���ǹ�]���� �������� �������� ��������" };

}

void Npc_Game::LoadShopItemInfo()
{
    ItemCostInfo();
    ItemDiscInfo();

    // ��ü ������ ���� ��ŭ �߰�
    for (int i = 0; i < 6; i++)
    {
        if (iteminfo[i].intCost != 0)
        {
            AllItem.push_back(iteminfo[i].strItemName);
            MapItem.insert(make_pair(iteminfo[i].strItemName, iteminfo[i].intItemNo));
        }
    }
}

// ������ ���� ���� / ���� ó��
// ���� �� ��ġ�� ����: 1 ���� ����: 0 ��ħ ����: 2
// ������ ��� ��ǰ��Ͽ��� ����
int ShowItemDisc(int itemCode, vector<string> &RuleItem)
{
    cout << "����: " << itemdisc[itemCode].strItemDisc << "\n";
    cout << "����: " << iteminfo[itemCode].intCost << "\n";
    cout << "0: �����ϱ�\n1: ��ġ�� (���� Ȯ��:"<<MySnatch<<"%)" << endl;
    cout << " ( ���ư���: 0~1�� ������ �ƹ� Ű�� ������ ���ư��ϴ�.)" << endl;

    char key = _getch(); // Ű �Է� �ޱ�

    if (key == '0')
    {
        if (MyMoney < iteminfo[itemCode].intCost)
        {
            cout << "�ܾ��� �����մϴ�." << "\n";
            _getch();
            return 0;
        }
        cout << iteminfo[itemCode].strItemName <<" �����Ͽ����ϴ�.\n";
        MyMoney -= iteminfo[itemCode].intCost;
        //RuleItem.erase(RuleItem.begin() + itemCode);

        _getch();
        return 1;
    }
    // ��ġ��
    else if (key == '1')
    {
        // �õ尪�� ��� ���� random_device ����.
        std::random_device rd;

        // random_device �� ���� ���� ���� ������ �ʱ�ȭ �Ѵ�.
        std::mt19937 gen(rd());

        // 0 ���� 99 ���� �յ��ϰ� ��Ÿ���� �������� �����ϱ� ���� �յ� ���� ����.
        std::uniform_int_distribution<int> dis(0, 99);

        // ������ �� �� �����ؼ� ������ ����
        int randValue = dis(gen);
        if (randValue < MySnatch)
        {
            // ����
            cout << iteminfo[itemCode].strItemName << " ��ġ�⸦ �����߽��ϴ�.\n";
            
            _getch();
            return 1;
        }
        else
        {
            // ����
            cout << iteminfo[itemCode].strItemName << " ��ġ�⸦ �����߽��ϴ�.\n";
            _getch();
            return 2;
        }
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

                int result = ShowItemDisc(itemNo, RuleItem);

                // ����, ��ħ ���� - �κ��丮 ���� ������ ���� ����
                if (result == 1)
                {
                    return itemNo;
                }
                // ��ħ ����
                if (result == 2)
                {
                    return -50;
                }
                if (result == 0)
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