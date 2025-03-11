// GameProject_Test.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <string.h>
#include<string>
#include <random>
#include <conio.h> // Windows에서 방향키 입력을 위해 사용

using namespace std;
using std::cout;
using std::endl;

struct MyCharacter
{
    int intLevel;
    string strName;
    int intHP;
    int intMp;
    int intAttack;
    int intMagic;
};

struct Inventorty
{
    int intItemNo;
    int intItemCount;
};

struct Iteminfo
{
    int intItemNo;
    string strItemName;
    int RestoreHp;
    int RestoreMp;
    int Levelup;
};

MyCharacter MyCh;
Iteminfo iteminfo[10];
Inventorty MyInven[7];

// 복권 소유 여부
bool isLotto = false;

// 마지막 데미지 원인
// 0: 독 1: 함정 2: 지뢰
int lastDamage = -1;

void ItemMaster()
{
    // itemcode, itemname, hp , mp, level
    iteminfo[0] = { 0,"빨간물약",10,0,0 };
    iteminfo[1] = { 1,"파란물약",0,10,0 };
    iteminfo[2] = { 2,"이상한사탕",0,0,1 };
    iteminfo[3] = { 3,"물",0,0,0 };
    iteminfo[4] = { 4,"독약",-100,0,0 };
    // 미구현
    iteminfo[5] = { 5,"복권",0,0,0 };
    iteminfo[6] = { 6,"데스노트",0,0,0 };
}

void InvenFirst()
{
    MyInven[0] = { 0,0 };
    MyInven[1] = { 0,0 };
    MyInven[2] = { 0,0 };
    MyInven[3] = { 0,0 };
    MyInven[4] = { 0,0 };
    MyInven[5] = { 0,0 };
    MyInven[6] = { 0,0 };

}

// 몬스터
struct Mosnterinfo
{
    int intMonNo;
    string strMonName;
    int MonHp;
    int intAttack;
    int Level;
};

Mosnterinfo monsterInfo[5];

// 미구현
// 몬스터 도감
void MonSterMaster()
{
    monsterInfo[0] = { 0,"도적",100,20,1 };
    monsterInfo[1] = { 1,"언데드",20,50,2 };
    monsterInfo[2] = { 2,"박쥐",5,15,1 };
    monsterInfo[3] = { 3,"레드 드래곤",250,35,5};
    monsterInfo[4] = { 4,"웨어울프",50,30,5 };
}

// 게임 종료 조건 - 엔딩/ 클리어 여부
void GameEnding(int n, bool isGameClear)
{
    string strEnd;
    // 독살
    if (n == 0)
    {
        strEnd = "Bad Ending 01: 독약 섭취로 인한 사망했습니다.";
    }
    // 함정으로 인한 추락사
    else if (n == 1)
    {
        strEnd = "Bad Ending 02: 함정에 빠져 사망했습니다.";
    }
    // 지뢰로 인한 폭사
    else if (n == 2)
    {
        strEnd = "Bad Ending 03: 지뢰가 폭발하여 사망했습니다.";
    }
    // 데스 노트에 자기 이름 적기
    else if (n==3)
    {
        cout << MyCh.strName + ": " << "역시 공책엔 내 이름을 적어야지~\n";
        cout << MyCh.strName + ": " << "(40초 뒤) 시야가 흐려진다... 심장 마비인가...\n";

        strEnd = "Bad Ending 04 (Hidden): 데스노트를 사용한 자의 최후.";
    }
    // 보물 찾기 성공
    else if (n == 4)
    {
        cout << MyCh.strName + ": " << "인생은 한방이야~\n";
        strEnd = "Ending: 보물 찾기 성공!!.";
    }
    // 로또를 가진 상태에서 복권점에 도착
    else if (n == 5)
    {
        cout << MyCh.strName + ": 사장님 자동이요" << "\n";
        strEnd = "Hidden Ending 01: 로또에 당첨되었다.";
    }
    // 데스노트로 심판
    else if (n == 6)
    {
        cout << MyCh.strName + ": 범죄자들은 죄값을 치뤄야지 안그래?" << "\n";
        cout << MyCh.strName + ": 내가 신세계의 신이 되겠어." << "\n";

        strEnd = "Hidden Ending 02: 신세계의 신.";
    }
    else
    {
        return;
    }

    string GameEndState;
    if (isGameClear)
    {
        GameEndState == "---- Game Clear ----";
    }
    else
    {
        GameEndState == "---- Game Over ----";
    }

    // 클리어 / 게임오버 표시
    cout << GameEndState << endl;
    // 엔딩종류
    cout << strEnd << endl;

    cout << "아무 키나 눌러주세요..." << endl;
    _getch();  // 키 입력을 기다림

    vector<string> menuItems = { "게임 종료" };
    int pointerPosition = 0;
    while (true) 
    {
        system("cls");

        for (int i = 0; i < menuItems.size(); i++) 
        {
            if (i == pointerPosition) 
            {
                cout << ">> " << menuItems[i] << endl;
            }
            else {
                cout << "   " << menuItems[i] << endl;
            }
        }

        int key = _getch();
        if (key == 13) 
        { // Enter 키
            cout << "게임을 종료합니다." << endl;
            exit(0); // 프로그램 종료
        }
    }
}

void UseItem(int InventNo)
{
    int useItemNo;
    useItemNo = MyInven[InventNo].intItemNo;

    if (useItemNo == 0)
    {
        cout << "-- 설명: Hp를 10 회복합니다 --" << endl;
    }
    if (useItemNo == 1)
    {
        cout << "-- 설명: Mp를 10 회복합니다 --" << endl;
    }
    if (useItemNo == 2)
    {
        cout << "-- 설명: 레벨이 1 증가합니다 --" << endl;
    }
    if (useItemNo == 3)
    {
        cout << "-- 설명: 마시니 개운하고 시원하다. 효과는 없는 것 같다 --" << endl;
    }
    if (useItemNo == 4)
    {
        cout << "-- 설명: 불길한 기운이 드는 녹색 물약. 마셔도 될까? --" << endl;
    }
    if (useItemNo == 5)
    {
        cout << "-- 설명: 지금은 사용할 수 없다. [복권방] 에 가져가면 뭔가 할 수 있을지도... --" << endl;
        return;
    }
    if (useItemNo == 6)
    {
        cout << "-- 설명: 이 노트에 이름이 적힌 인간은 죽는다. --" << endl;
        vector<string> menuItems = { "불태운다", "내 이름을 적어본다" , "지명수배서에 있는 범죄자 이름을 적어본다", "돌아가기"};
        int pointerPosition = 0;
        while (true)
        {
            system("cls");

            for (int i = 0; i < menuItems.size(); i++)
            {
                if (i == pointerPosition)
                {
                    cout << ">> " << menuItems[i] << endl;
                }
                else {
                    cout << "   " << menuItems[i] << endl;
                }
            }

            // 키 입력 대기
            int key = _getch();

            // 위쪽 방향키
            if (key == 72) 
            { 
                pointerPosition = (pointerPosition - 1 + menuItems.size()) % menuItems.size();
            }
            // 아래쪽 방향키
            else if (key == 80) 
            {
                pointerPosition = (pointerPosition + 1) % menuItems.size();
            }
            // Enter 키
            else if (key == 13)
            { 
                // 불태운다
                if (pointerPosition == 0)
                {
                    cout << MyCh.strName + ": " << "이런 불길한 노트는 없애야 해\n";
                    cout << "-- 데스노트는 불타서 없어졌습니다." << endl;
                }
                // 내 이름 적기
                if (pointerPosition == 1)
                {
                    GameEnding(3, false);
                    return;
                }
                // 범죄자 이름 적기
                if (pointerPosition == 2)
                {
                    GameEnding(6, true);
                    return;
                }
                if (pointerPosition == 3)
                {
                    return;
                }
            }
        }
    }

    MyInven[InventNo].intItemCount--;

    for (int i = 0; i < 10; i++)
    {
        if (useItemNo == iteminfo[i].intItemNo)
        {
            MyCh.intHP = MyCh.intHP + iteminfo[i].RestoreHp;
            MyCh.intMp = MyCh.intMp + iteminfo[i].RestoreMp;
            MyCh.intLevel = MyCh.intLevel + iteminfo[i].Levelup;

            if (iteminfo[i].RestoreHp != 0)
            {
                string strStatus;
                if (iteminfo[i].RestoreHp > 0)
                {
                    strStatus = "증가";
                }
                else
                {
                    strStatus = "감소";
                    // 독으로 인한 데미지
                    lastDamage = 0;
                }
                cout << "-- Hp: " << iteminfo[i].RestoreHp << " " << strStatus << " / Hp: " << MyCh.intHP << endl;
            }
            if (iteminfo[i].RestoreMp != 0)
            {
                string strStatus;
                if (iteminfo[i].RestoreMp > 0)
                {
                    strStatus = "증가";
                }
                else
                {
                    strStatus = "감소";
                }

                cout << "-- Mp: " + to_string(iteminfo[i].RestoreMp) + " " + strStatus + "/ Mp: " + to_string(MyCh.intMp) << endl;
            }
            if (iteminfo[i].Levelup != 0)
            {
                string strStatus;
                if (iteminfo[i].Levelup > 0)
                {
                    strStatus = "증가";
                }
                else
                {
                    strStatus = "감소";
                }
                cout << "-- 레벨: " + to_string(iteminfo[i].Levelup) + " " + strStatus + "/ 레벨: " + to_string(MyCh.intLevel) << endl;
            }

            if (MyCh.intHP <= 0)
            {
                GameEnding(lastDamage, false);
            }

            return;
        }
    }
}

void ShowMyInvent()
{
    int intCount = 0;

    cout << "**** 인벤토리 ****" << endl;
    cout << " ( ※ 아이템 번호를 입력하면 사용 가능합니다. (0~ 4))" << endl;
    cout << " ( 돌아가기: 0~4를 제외한 아무 키를 누르면 돌아갑니다.)" << endl;
    for (int i = 0; i < 5; i++)
    {
        if (MyInven[i].intItemCount>0)
        {
            for (int j = 0; j < 10; j++)
            {
                if (MyInven[i].intItemNo == iteminfo[j].intItemNo)
                {
                    cout << to_string(i)+":"+  iteminfo[j].strItemName + "/" + to_string(MyInven[i].intItemCount) + "개" << endl;
                    intCount++;
                    break;
                }
            }
        }
    }

    if (intCount == 0)
    {
        cout << "비어 있음." << endl;
    }
}

void AddInvent(int ItemNo, int ItemCount)
{
    if (ItemNo == 5)
    {
        isLotto = true;
    }

    // 먼저 인벤토리에 같은 아이템이 있는지 확인
    for (int i = 0; i < 5; i++)
    {
        if (MyInven[i].intItemNo == ItemNo) // 이미 존재하는 아이템 번호라면
        {
            MyInven[i].intItemCount += ItemCount; // 수량을 추가
            cout << iteminfo[ItemNo].strItemName + " " + to_string(ItemCount) + "개 추가." << endl;
            return; // 함수 종료
        }
    }

    // 인벤토리에 동일한 아이템이 없으면 빈 슬롯에 추가
    for (int i = 0; i < 5; i++)
    {
        if (MyInven[i].intItemCount == 0) // 빈 슬롯 찾기
        {
            MyInven[i].intItemNo = ItemNo;
            MyInven[i].intItemCount = ItemCount;

            // 아이템 정보 출력
            for (int j = 0; j < 10; j++)
            {
                if (iteminfo[j].intItemNo == ItemNo)
                {
                    cout << iteminfo[j].strItemName + " " + to_string(ItemCount) + "개 획득." << endl;
                    return;
                }
            }
        }
    }
}


void PrintInfo()
{
    cout << "**** 캐릭터 정보 ****" << endl;
    cout << "-- 이름: " + MyCh.strName << endl;
    cout << "-- 레벨: " + to_string(MyCh.intLevel) << endl;
    cout << "-- Hp: " + to_string(MyCh.intHP) << endl;
    cout << "-- Mp: " + to_string(MyCh.intMp) << endl;
    cout << "-- 공격력 :" + to_string(MyCh.intAttack) << endl;
    cout << "-- 마법력 :" + to_string(MyCh.intMagic) << endl;
}

void StartGame() {
    cout << "게임이 시작됩니다!" << endl;
}

void Settings() {
    cout << "설정 메뉴입니다!" << endl;
}


int main()
{
    std::cout << "----Game Start!----\n";

    cout << "???: 낯선 천장이다... 여기는 대체 어디지." << endl;

    cout << "신비한 목소리: 당신은 모험을 통해 보물을 찾아내야 합니다." << endl;
    cout << "신비한 목소리: 함정도 존재하니 아이템을 적절하게 활용하여 게임 오버를 피하세요." << endl;

    cout << "\n";
    cout << "당신의 캐릭터 이름을 정하세요." << endl;
    string strYourName;
    cin >> strYourName;

    MyCh.strName = strYourName;

    cout << "캐릭터 생성." << endl;
    cout << "직업을 선택해 주세요." << endl;

    vector<string> RuleItem = { "전사", "마법사", "도적","개발자" };
    int pntPosition = 0;

    while (true)
    {
        // 메뉴 출력
        system("cls"); // Windows에서는 "cls"로 변경
        for (int i = 0; i < RuleItem.size(); i++) {
            if (i == pntPosition) {
                cout << ">> " << RuleItem[i] << endl;
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
            // 전사
            if (pntPosition == 0)
            {
                MyCh.intLevel = 1;
                MyCh.intHP = 150;
                MyCh.intMp = 0;
                MyCh.intAttack = 20;
                MyCh.intMagic = 0;
            }
            // 마법사
            if (pntPosition == 1)
            {
                MyCh.intLevel = 1;
                MyCh.intHP = 100;
                MyCh.intMp = 150;
                MyCh.intAttack = 0;
                MyCh.intMagic = 20;
            }
            // 도적
            if (pntPosition == 2)
            {
                MyCh.intLevel = 1;
                MyCh.intHP = 80;
                MyCh.intMp = 60;
                MyCh.intAttack = 30;
                MyCh.intMagic = 30;
            }
            // 개발자
            if (pntPosition == 3)
            {
                MyCh.intLevel = 1;
                MyCh.intHP = 10;
                MyCh.intMp = 10;
                MyCh.intAttack = 20;
                MyCh.intMagic = 0;
            }
            break;
        }
    }

        // 아이템 정보 불러오기
        ItemMaster();

        vector<string> menuItems = { "인벤토리", "캐릭터", "탐색하기","게임 종료" };
        int pointerPosition = 0;

        while (true)
        {
            // 메뉴 출력
            system("cls"); // Windows에서는 "cls"로 변경
            for (int i = 0; i < menuItems.size(); i++) {
                if (i == pointerPosition) {
                    cout << ">> " << menuItems[i] << endl;
                }
                else {
                    cout << "   " << menuItems[i] << endl;
                }
            }

            // 키 입력 처리
            int key = _getch(); // 키 입력을 받음
            if (key == 72) { // 위쪽 방향키
                pointerPosition = (pointerPosition - 1 + menuItems.size()) % menuItems.size();
            }
            else if (key == 80) { // 아래쪽 방향키
                pointerPosition = (pointerPosition + 1) % menuItems.size();
            }
            else if (key == 13) { // Enter 키
             // 선택된 메뉴에 따라 동작 실행
                if (pointerPosition == 0)
                {
                    ShowMyInvent();

                    // 아이템 사용
                    char itemno = _getch();

                    string temp = "";
                    if (isdigit(itemno))
                    {
                        temp = temp + itemno;

                        int intItemno = stoi(temp);
                        if (intItemno >= 0 && intItemno <= 4)
                        {
                            UseItem(intItemno);
                        }
                        else
                        {
                            continue;
                        }
                    }
                    else
                    {
                        continue;
                    }
                }
                else if (pointerPosition == 1)
                {
                    PrintInfo();
                }
                else if (pointerPosition == 2)
                {
                    // 랜덤
            // 시드값을 얻기 위한 random_device 생성.
                    std::random_device rd;

                    // random_device 를 통해 난수 생성 엔진을 초기화 한다.
                    std::mt19937 gen(rd());

                    // 0 부터 99 까지 균등하게 나타나는 난수열을 생성하기 위해 균등 분포 정의.
                    std::uniform_int_distribution<int> dis(0, 99);

                    // 난수를 한 번 생성해서 변수에 저장
                    int randValue = dis(gen);

                    // 함정(확률: 25%)
                    if (randValue < 25)
                    {
                        cout << "--함정에 걸렸습니다. 체력이 20 하락합니다.  / Hp: " + to_string(MyCh.intHP - 20) << endl;
                        MyCh.intHP = MyCh.intHP - 20;
                        lastDamage = 1;
                    }
                    // 미발견(확률: 5%)
                    if (25 <= randValue && randValue < 30)
                    {
                        cout << "--아무것도 발견 하지 못하였습니다. --" << endl;
                    }

                    // 아이템 발견(확률: 60%)
                    if (30 <= randValue && randValue < 90)
                    {
                        cout << "--아이템 발견 !! --" << endl;

                        // 랜덤
                        // 시드값을 얻기 위한 random_device 생성.
                        std::random_device rdItem;

                        // random_device 를 통해 난수 생성 엔진을 초기화 한다.
                        std::mt19937 genItem(rdItem());

                        // 0 부터 99 까지 균등하게 나타나는 난수열을 생성하기 위해 균등 분포 정의.
                        std::uniform_int_distribution<int> disItem(0, 99);

                        // 난수를 한 번 생성해서 변수에 저장
                        int randItemValue = disItem(genItem);

                        if (randItemValue < 25)
                        {
                            AddInvent(0, 1);
                        }
                        else if (25 <= randItemValue && randItemValue < 50)
                        {
                            AddInvent(1, 1);
                        }
                        else if (50 <= randItemValue && randItemValue < 65)
                        {
                            AddInvent(2, 1);
                        }
                        else if (65 <= randItemValue && randItemValue < 80)
                        {
                            AddInvent(3, 1);
                        }
                        // 복권
                        else if (80 <= randItemValue && randItemValue < 90)
                        {
                            AddInvent(5, 1);
                        }
                        // 데스노트
                        else if (90 <= randItemValue && randItemValue < 95)
                        {
                            AddInvent(6, 1);
                        }
                        else
                        {
                            AddInvent(4, 1);
                        }
                    }

                    // 지뢰(2.5%)
                    if (90 <= randValue && randValue < 92.5)
                    {
                        cout << "--지뢰에 걸렸습니다. 체력이 100 하락합니다.  / Hp: " + to_string(MyCh.intHP - 100) << endl;
                        MyCh.intHP = MyCh.intHP - 100;
                        lastDamage = 2;
                    }


                    // 보물(2.5%)
                    if (92.5 <= randValue && randValue < 95)
                    {
                        GameEnding(4, true);
                        cout << "프로그램을 종료합니다." << endl;
                        return 0;
                    }
                    // 복권방(5%)
                    if (randValue >= 95)
                    {
                        cout << "복권방에 도착했습니다. 복권을 가져온 경우에만 응모 가능합니다." << "\n";
                        // 로또를 가지고 있으면 히든엔딩
                        if (isLotto)
                        {
                            GameEnding(5, true);
                            cout << "프로그램을 종료합니다." << endl;
                            return 0;
                        }
                    }

                    if (MyCh.intHP <= 0)
                    {
                        GameEnding(lastDamage, false);
                        cout << "프로그램을 종료합니다." << endl;
                        return 0;
                    }
                }
                else if (pointerPosition == 3)
                {
                    cout << "프로그램을 종료합니다." << endl;
                    return 0;
                }
                // Enter 입력 후 키 대기
                system("pause");
            }
        }
    }

