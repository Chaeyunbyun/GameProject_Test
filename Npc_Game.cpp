#include "Npc_Game.h"
#include <iostream>
#include <vector>
#include <random>
#include <conio.h> // Windows에서 방향키 입력을 위해 사용
#include <random>
#include <algorithm> // 셔플 시 사용

#include "GameProject.h"
/// NPC 정보, 조우 이벤트

// npc 별 이벤트 분기
// 리턴: 아이템번호: 아이템을 인벤에 추가
// 리턴: 100 - 처리 x
// 리턴: -100 - 사망
// 리턴: -50 - 체력감소
int Npc_Game::NpcEvent(int NpcNo, bool isLotto)
{
    // 상인
	if (NpcNo == 0)
	{
        cout << " -- [상인]을 만났습니다. -- " << "\n";
		int ret = ShopEvent();
        _getch();
        return ret;
	}
    // 도적단 두목
	if (NpcNo == 1)
	{
        cout << " -- [도적단 두목]을 만났습니다.. -- " << "\n";

        if (Myjob == "rogue")
        {
            cout << "도적단 두목: 어," << MyName + " 이잖아? 넌 그냥 보내줄게" << "\n";
            _getch();
            return 100;
        }
        cout << "도적단 두목: 돈 내놔. 30원 주면 보내줄게." << "\n";

        if (MyMoney>= 30)
        {
            cout << " -- 30원을 잃었습니다. -- " << "\n";
            MyMoney -= 30;
            _getch();
            return 100;
        }
        else
        {
            cout << "도적단 두목: 뭐야 돈 없어? 그럼 목숨이라도 내놔야지!" << "\n";
            MyMoney = 0;
            _getch();
            return -100;
        }
	}
    // 타노스
    if (NpcNo == 2)
    {
        cout << " -- [타노스]를 만났습니다.. -- " << "\n";
        cout << "타노스: 내가 핑거 스냅을 하면 50%의 인류가 소멸한다" << "\n";
        cout << "타노스: 넌 살아남을 수 있을까?" << "\n";
        cout << "\n";
        cout << "     -- 딱! --" << "\n";
        cout << "------------------" << "\n";
        cout << "\n";

        _getch();

        // 시드값을 얻기 위한 random_device 생성.
        std::random_device rdItem;

        // random_device 를 통해 난수 생성 엔진을 초기화 한다.
        std::mt19937 genItem(rdItem());

        // 0 부터 99 까지 균등하게 나타나는 난수열을 생성하기 위해 균등 분포 정의.
        std::uniform_int_distribution<int> disItem(0, 99);

        // 난수를 한 번 생성해서 변수에 저장
        int randItemValue = disItem(genItem);

        // 생존
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
        cout << "복권방 사장: 안녕하세요" << "\n";
        cout << "복권방 사장: 복권을 가져와야 응모 가능합니다." << "\n";

        if (isLotto == false)
        {
            return -100;
        }
        else
        {
            _getch();

            // 시드값을 얻기 위한 random_device 생성.
            std::random_device rdItem;

            // random_device 를 통해 난수 생성 엔진을 초기화 한다.
            std::mt19937 genItem(rdItem());

            // 0 부터 99 까지 균등하게 나타나는 난수열을 생성하기 위해 균등 분포 정의.
            std::uniform_int_distribution<int> disItem(0, 99);

            // 난수를 한 번 생성해서 변수에 저장
            int randItemValue = disItem(genItem);

            // 당첨
            if (randItemValue < 20)
            {
                return 101;
            }
            else
            {
                cout << "복권방 사장: 아쉽네요, 다음 기회에..." << "\n";
                return 100;
            }
        }
    }
}

Npc_Game::Itemcost iteminfo[6];
Npc_Game::ItemDisc itemdisc[6];

// gameProject_test 의 itemcode 와 맞춰야함
//0,"빨간물약"
//1,"파란물약"
//2,"이상한사탕"
//3,"물"
//4,"독약"
//5,"복권"
//6,"데스노트"

void ItemCostInfo()
{
    // 0: 필드에서만 드랍됨
    // itemcode, itemname, itemcost
    iteminfo[0] = { 0,"빨간물약",50};
    iteminfo[1] = { 1,"파란물약",50};
    iteminfo[2] = { 2,"이상한사탕",0};
    iteminfo[3] = { 3,"물",10 };
    iteminfo[4] = { 4,"독약",20};
    iteminfo[5] = { 5,"복권",150 };
}

void ItemDiscInfo()
{
    itemdisc[0] = { 0,"Hp를 10 회복시킨다." };
    itemdisc[1] = { 1,"Mp를 10 회복시킨다." };
    itemdisc[2] = { 2,"레벨이 1 증가" };
    itemdisc[3] = { 3,"평범한 물. 효과는 없어보인다..." };
    itemdisc[4] = { 4,"불길한 느낌이 나는 물약이다" };
    itemdisc[5] = { 5,"[복권방]으로 가져가면 좋은일이 있을수도" };

}

void Npc_Game::LoadShopItemInfo()
{
    ItemCostInfo();
    ItemDiscInfo();

    // 전체 아이템 개수 만큼 추가
    for (int i = 0; i < 6; i++)
    {
        if (iteminfo[i].intCost != 0)
        {
            AllItem.push_back(iteminfo[i].strItemName);
            MapItem.insert(make_pair(iteminfo[i].strItemName, iteminfo[i].intItemNo));
        }
    }
}

// 아이템 설명 보기 / 구매 처리
// 구매 및 훔치기 성공: 1 구매 안함: 0 훔침 실패: 2
// 구매한 경우 상품목록에서 제외
int ShowItemDisc(int itemCode, vector<string> &RuleItem)
{
    cout << "설명: " << itemdisc[itemCode].strItemDisc << "\n";
    cout << "가격: " << iteminfo[itemCode].intCost << "\n";
    cout << "0: 구매하기\n1: 훔치기 (성공 확률:"<<MySnatch<<"%)" << endl;
    cout << " ( 돌아가기: 0~1를 제외한 아무 키를 누르면 돌아갑니다.)" << endl;

    char key = _getch(); // 키 입력 받기

    if (key == '0')
    {
        if (MyMoney < iteminfo[itemCode].intCost)
        {
            cout << "잔액이 부족합니다." << "\n";
            _getch();
            return 0;
        }
        cout << iteminfo[itemCode].strItemName <<" 구매하였습니다.\n";
        MyMoney -= iteminfo[itemCode].intCost;
        //RuleItem.erase(RuleItem.begin() + itemCode);

        _getch();
        return 1;
    }
    // 훔치기
    else if (key == '1')
    {
        // 시드값을 얻기 위한 random_device 생성.
        std::random_device rd;

        // random_device 를 통해 난수 생성 엔진을 초기화 한다.
        std::mt19937 gen(rd());

        // 0 부터 99 까지 균등하게 나타나는 난수열을 생성하기 위해 균등 분포 정의.
        std::uniform_int_distribution<int> dis(0, 99);

        // 난수를 한 번 생성해서 변수에 저장
        int randValue = dis(gen);
        if (randValue < MySnatch)
        {
            // 성공
            cout << iteminfo[itemCode].strItemName << " 훔치기를 성공했습니다.\n";
            
            _getch();
            return 1;
        }
        else
        {
            // 실패
            cout << iteminfo[itemCode].strItemName << " 훔치기를 실패했습니다.\n";
            _getch();
            return 2;
        }
    }
}

// 상점 이벤트
// 구매한 경우 수량 처리를 위해 itemNo를 리턴
// 구매 하지 않으면 100 리턴
int Npc_Game::ShopEvent()
{
    vector<string> RuleItem;
    int pntPosition = 0;

    // 랜덤 시드 설정
    random_device rd;
    mt19937 gen(rd());

    // 셔플
    shuffle(AllItem.begin(), AllItem.end(), gen);

    // 앞에서 3개만 선택
    RuleItem.assign(AllItem.begin(), AllItem.begin() + 3);

    RuleItem.push_back("돌아가기");

    while (true)
    {
        // 메뉴 출력
        system("cls"); // Windows에서는 "cls"로 변경
        cout << "상인: 구매하고 싶은 물건을 선택하세요." << "(현재 소지금: "<< MyMoney << ")" << endl;

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

        // 키 입력 처리
        int key = _getch(); // 키 입력을 받음
        if (key == 72) { // 위쪽 방향키
            pntPosition = (pntPosition - 1 + RuleItem.size()) % RuleItem.size();
        }
        else if (key == 80) { // 아래쪽 방향키
            pntPosition = (pntPosition + 1) % RuleItem.size();
        }
        else if (key == 13)
        { // Enter 키
         // 선택된 메뉴에 따라 동작 실행
            // 1번
            if (pntPosition >= 0 && pntPosition <3)
            {
                int itemNo = MapItem[pntItemName];

                int result = ShowItemDisc(itemNo, RuleItem);

                // 구매, 훔침 성공 - 인벤토리 수량 조정을 위해 리턴
                if (result == 1)
                {
                    return itemNo;
                }
                // 흠침 실패
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
                cout << "상점을 나갑니다...\n";
                _getch(); 
                return 100;
            }
        }
    }
}