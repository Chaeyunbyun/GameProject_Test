// GameProject_Test.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <string.h>
#include<string>
#include <random>
#include <conio.h> // Windows에서 방향키 입력을 위해 사용

#include <windows.h>
#include <thread>
#include <chrono>
#include <fstream>
#include <sstream>
#include <shlwapi.h>  // PathRemoveFileSpec 함수 사용을 위해 추가
#pragma comment(lib, "shlwapi.lib")  // shlwapi.lib 라이브러리 링크 추가

#include "GameProject.h"
#include "Npc_Game.h"

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
    // 훔치기스탯
    int intSnatch;
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

// 엔딩번호, 설명, 엔딩조건, 수집여부
struct EndingInfo
{
    int EndingNo;
    string EndingDesc;
    string EndingPrecon;
    bool isCollected;
};

MyCharacter MyCh;
Iteminfo iteminfo[10];
Inventorty MyInven[7];
EndingInfo Endinfo[10];

int MyMoney = 100;
string Myjob ="";
string MyName = "";
int MySnatch = 0;

bool isContinue = true;
bool isItemUseEnd = false;

void ItemMaster()
{
    // itemcode, itemname, hp , mp, level
    iteminfo[0] = { 0,"빨간물약",10,0,0 };
    iteminfo[1] = { 1,"파란물약",0,10,0 };
    iteminfo[2] = { 2,"이상한사탕",0,0,1 };
    iteminfo[3] = { 3,"물",0,0,0 };
    iteminfo[4] = { 4,"독약",-100,0,0 };
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

void SetEndingInfo()
{
    Endinfo[0] = { 0,"중독", "독약을 마시고 사망", false};
    Endinfo[1] = { 1,"추락", "함정에 빠진 후 사망", false};
    Endinfo[2] = { 2,"폭발", "지뢰를 밟고 사망",  false};
    Endinfo[3] = { 3,"죽음의 노트", "데스노트에 자신의 이름을 적는다", false};
    Endinfo[4] = { 4,"원피스", "보물을 발견", false};
    Endinfo[5] = { 5,"운수좋은 날", "복권방에 로또를 가져가 당첨된다",false};
    Endinfo[6] = { 6,"키라", "데스노트에 지명수배자의 이름을 적는다",false};
    Endinfo[7] = { 7,"도적", "도적 두목의 공격에 의한 사망" ,false};
    Endinfo[8] = { 8,"선택받지 못한 쪽", "타노스를 만난 후 50%의 확률로 사망",false};
    Endinfo[9] = { 9,"손버릇의 최후", "상점에서 훔치기를 실패 한 후 상인의 공격으로 사망", false};

    // 파일에서 획득여부 읽어와야됨

    string line;
    ifstream file("Endlist.txt"); 

    if (file.is_open()) 
    {
        while (getline(file, line)) 
        {
            std::stringstream ss(line); // stringstream으로 줄을 처리

            std::string number;
            std::string isEnable;

            ss >> number;  // 번호 읽기
            getline(ss >> std::ws, isEnable);  // 공백 무시하고 나머지 읽기
            if (isEnable == "Y")
            {
                Endinfo[stoi(number)].isCollected = true;
            }
        }
        file.close(); // 열었떤 파일을 닫는다. 
    }
    else 
    {
        cout << "Unable to open file";
    }
}

void SetWorkingDirectoryToTargetDir()
{
    // EXE 파일의 경로를 가져오고 작업 디렉토리로 설정
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
    PathRemoveFileSpecA(path);  // 파일 경로에서 디렉터리 경로만 남기기
    SetCurrentDirectoryA(path); // 디렉터리 설정
}

void SaveEndingList(int endNo)
{
    std::ifstream fileIn("Endlist.txt"); // 기존 파일 읽기
    std::vector<std::string> lines; // 파일 내용을 저장할 벡터
    std::string line;

    if (!fileIn.is_open()) {
        std::cout << "파일을 열 수 없습니다!" << std::endl;
        return;
    }
    // 파일 내용을 읽어서 벡터에 저장
    while (getline(fileIn, line)) 
    {
        std::stringstream ss(line);
        std::string number, isEnable;
        ss >> number >> isEnable;  // 번호, Y/N 읽기

        // 특정 번호를 찾으면 변경
        if (std::stoi(number) == endNo) 
        {
            if (isEnable == "N")
            {
                cout << "신규 엔딩이 목록에 추가되었습니다." << "\n";
            }
            isEnable = "Y"; // 특정 번호의 값을 Y로 변경
        }

        lines.push_back(number + " " + isEnable);
    }
    fileIn.close(); // 파일 닫기

    // 수정된 내용을 다시 파일에 저장
    std::ofstream fileOut("Endlist.txt"); // 덮어쓰기 모드

    if (!fileOut.is_open()) 
    {
        std::cout << "파일을 열 수 없습니다!" << std::endl;
        return;
    }

    for (const auto& l : lines) 
    {
        fileOut << l << "\n"; // 줄 단위로 다시 쓰기
    }
    fileOut.close();

    std::cout << "파일 저장 완료!" << std::endl;
}

// 이미지 출력
void ShowBitmap(const char* filename)
{
    HBITMAP hBitmap;
    BITMAP bitmap;
    HDC hdc, hdcMem;
    HWND hwnd = GetConsoleWindow(); // 콘솔 창 핸들 가져오기

    // 콘솔 DC 얻기
    hdc = GetDC(hwnd);
    hBitmap = (HBITMAP)LoadImageA(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (!hBitmap) 
    {
        return;
    }

    hdcMem = CreateCompatibleDC(hdc);
    SelectObject(hdcMem, hBitmap);
    GetObject(hBitmap, sizeof(BITMAP), &bitmap);

    // 20, 50 위치에 200 x 200 사이즈로 출력
    StretchBlt(hdc, 20, 100, 250, 250, hdcMem, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);

    DeleteDC(hdcMem);
    ReleaseDC(hwnd, hdc);
    DeleteObject(hBitmap);
}

void SetCursorPosition(int x, int y) 
{
    COORD coord;
    coord.X = x;  // x 좌표
    coord.Y = y;  // y 좌표
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);  // 콘솔 커서 위치 설정
}

bool GameEndingMenu()
{
    vector<string> menuItems = { "게임 종료", "다시 하기" };
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

        if (key == 13)
        { // Enter 키
            if (pointerPosition == 0)
            {
                cout << "게임을 종료합니다." << endl;
                return false;
                //exit(0); // 프로그램 종료
            }
            if (pointerPosition == 1)
            {
                //PlayGame();
                return true;
            }
        }
    }
}

// 게임 종료 조건 - 엔딩/ 클리어 여부
void GameEnding(int n, bool isGameClear)
{
    system("cls");

    // 엔딩이미지
    vector<string> Ending_imgName(10);
    Ending_imgName[0] = "Ending_img\\poison.bmp";
    Ending_imgName[1] = "Ending_img\\trap.bmp";
    Ending_imgName[2] = "";
    Ending_imgName[3] = "";
    Ending_imgName[4] = "Ending_img\\treasure.bmp";
    Ending_imgName[5] = "";
    Ending_imgName[6] = "Ending_img\\kira.bmp";
    Ending_imgName[7] = "";
    Ending_imgName[8] = "";
    Ending_imgName[9] = "";

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
    // 도적단 두목에게 사망
    else if (n == 7)
    {
        strEnd = "Bad Ending 05: 도적단 두목에게 공격받아 사망했습니다.";
    }
    // 타노스에게 사망
    else if (n==8)
    {
        cout << MyCh.strName + ": 으윽... 속이 이상해... 기분이 안 좋아..." << "\n";
        cout << "-- 몸이 점차 먼지가 되어 사라지기 시작했다. --" << "\n";
        strEnd = "Bad Ending 06 (Hidden): 이젠 가망이 없어.";
    }
    // 상점 주인에게 사망
    else if (n == 9)
    {
        strEnd = "Bad Ending 07: 상점 주인에게 공격받아 사망했습니다.";
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

    if (Ending_imgName[n]!="")
    {
        // 이미지 출력하기
        ShowBitmap(Ending_imgName[n].c_str());
    }

    // 클리어 / 게임오버 표시
    cout << GameEndState << endl;
    // 엔딩종류
    cout << strEnd << endl;
    SaveEndingList(n);

    cout << "아무 키나 눌러주세요..." << endl;
    _getch();  // 키 입력을 기다림

    isContinue = GameEndingMenu();
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
        vector<string> menuItems = { "불태운다", "내 이름을 적어본다" , "지명수배서에 있는 범죄자 이름을 적어본다", "돌아가기"};
        int pointerPosition = 0;
        while (true)
        {
            system("cls");
            cout << "-- 설명: 이 노트에 이름이 적힌 인간은 죽는다. --" << endl;

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

                    MyInven[InventNo].intItemCount--;
                    return;
                }
                // 내 이름 적기
                if (pointerPosition == 1)
                {
                    GameEnding(3, false);
                    isItemUseEnd = true;
                    return;
                }
                // 범죄자 이름 적기
                if (pointerPosition == 2)
                {
                    GameEnding(6, true);
                    isItemUseEnd = true;
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
                    m_lastDamage = 0;
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
                GameEnding(m_lastDamage, false);
                isItemUseEnd = true;
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
        m_isLotto = true;
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

// 공백 길이 맞추기
// 전체 총 길이, 문자열
string SetSpace(int fulllen, string str)
{
    string ret = "";

    int len = str.size();
    for (int i = 0; i < fulllen - len; i++)
    {
        ret += " ";
    }

    return ret;
}

void PrintInfo()
{
    string strName = "| 이름: " + MyCh.strName;
    string strLv = "| 레벨: " + to_string(MyCh.intLevel);
    string strHp = "| Hp: " + to_string(MyCh.intHP);
    string strMp = "| Mp: " + to_string(MyCh.intMp);
    string strAtt = "| 공격력 :" + to_string(MyCh.intAttack);
    string strMgk = "| 마법력 :" + to_string(MyCh.intMagic);
    string strMoney = "|         소지금: " + to_string(MyMoney);

    cout << "_________________________" << "\n";
    cout << "|***** 캐릭터 정보 ******" << "|\n";
    cout << "|________________________" << "|\n";
    cout << strMoney + SetSpace(25, strMoney) << "|\n";
    cout << "|________________________" << "|\n";
    cout << strName + SetSpace(25, strName) << "|\n";
    cout << strLv + SetSpace(25, strLv) << "|\n";
    cout << strHp + SetSpace(25, strHp) << "|\n";
    cout << strMp + SetSpace(25, strMp) << "|\n";
    cout << strAtt + SetSpace(25, strAtt) << "|\n";
    cout << strMgk + SetSpace(25, strMgk) << "|\n";
    cout << "|________________________" << "|\n";
}

void StartGame() {
    cout << "게임이 시작됩니다!" << endl;
}

void Settings() {
    cout << "설정 메뉴입니다!" << endl;
}

void ShowEndingList()
{
    cout << "============ 엔딩 목록 ============ " << "\n";
    for (int i = 0; i < 10; i++)
    {
        int endno = Endinfo[i].EndingNo;
        string strend;
        if (endno<10)
        {
            strend = "0" + to_string(endno);
        }
        else
        {
            strend = to_string(endno);
        }
        if (Endinfo[i].isCollected)
        {
            //cout << strend + ": " << Endinfo[i].EndingDesc << "  " << Endinfo[i].EndingPrecon << "\n";
            string strEndList = strend + ": " + Endinfo[i].EndingDesc;
            cout << strEndList + SetSpace(25, strEndList) + Endinfo[i].EndingPrecon << "\n";
        }
        else
        {
            cout << strend + ": " << "???" << "\n";
        }
    }
    int key = _getch();
}

void PlayGame()
{
    vector<string> StartItem = { "게임시작", "엔딩 목록", "종료"};
    int pntPos = 0;

    SetEndingInfo();

    while (true)
    {
        // 메뉴 출력
        system("cls"); // Windows에서는 "cls"로 변경

        for (int i = 0; i < StartItem.size(); i++) {
            if (i == pntPos) {
                cout << ">> " << StartItem[i] << endl;
            }
            else {
                cout << "   " << StartItem[i] << endl;
            }
        }

        // 키 입력 처리
        int key = _getch(); // 키 입력을 받음
        if (key == 72) { // 위쪽 방향키
            pntPos = (pntPos - 1 + StartItem.size()) % StartItem.size();
        }
        else if (key == 80) { // 아래쪽 방향키
            pntPos = (pntPos + 1) % StartItem.size();
        }
        else if (key == 13)
        { // Enter 키
         // 선택된 메뉴에 따라 동작 실행
            if (pntPos == 0)
            {
                break;
            }
            if (pntPos == 1)
            {
                ShowEndingList();
            }
            if (pntPos == 2)
            {
                isContinue = false;
                return;
            }
        }
    }
    system("cls"); // Windows에서는 "cls"로 변경

    std::cout << "----Game Start!----\n";

    cout << "???: 낯선 천장이다... 여기는 대체 어디지." << endl;

    cout << "신비한 목소리: 당신은 모험을 통해 보물을 찾아내야 합니다." << endl;
    cout << "신비한 목소리: 함정도 존재하니 아이템을 적절하게 활용하여 게임 오버를 피하세요." << endl;

    cout << "\n";
    cout << "당신의 캐릭터 이름을 정하세요." << endl;
    string strYourName;
    cin >> strYourName;

    MyCh.strName = strYourName;

    // 전역변수에 전달
    MyName = strYourName;

    vector<string> RuleItem = { "전사", "마법사", "도적","개발자" };
    int pntPosition = 0;

    vector <Npc_Game> Npclist;

    // npc 지정
    // 상인 npc
    Npc_Game merchant;
    merchant.NpcNo = 0;
    merchant.NpcName = "상인";
    merchant.NpcAttkPnt = 10;

    Npclist.push_back(merchant);

    // 도적두목
    Npc_Game burglar;
    burglar.NpcNo = 1;
    burglar.NpcName = "도적두목";
    burglar.NpcAttkPnt = 70;

    Npclist.push_back(burglar);

    // 타노스
    Npc_Game thanos;
    thanos.NpcNo = 2;
    thanos.NpcName = "타노스";
    thanos.NpcAttkPnt = 1000;

    Npclist.push_back(thanos);

    // 복권방 사장
    Npc_Game lottoNpc;
    lottoNpc.NpcNo = 3;
    lottoNpc.NpcName = "복권방 사장";
    lottoNpc.NpcAttkPnt = 20;

    merchant.LoadShopItemInfo();

    while (true)
    {
        // 메뉴 출력
        system("cls"); // Windows에서는 "cls"로 변경


        cout << "100 원이 지급되었습니다." << "\n";

        cout << "직업을 선택해 주세요." << endl;

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
                MyCh.intSnatch = 10;

                Myjob = "warrior";
            }
            // 마법사
            if (pntPosition == 1)
            {
                MyCh.intLevel = 1;
                MyCh.intHP = 100;
                MyCh.intMp = 150;
                MyCh.intAttack = 0;
                MyCh.intMagic = 20;
                MyCh.intSnatch = 10;

                Myjob = "socerer";
            }
            // 도적
            if (pntPosition == 2)
            {
                MyCh.intLevel = 1;
                MyCh.intHP = 80;
                MyCh.intMp = 60;
                MyCh.intAttack = 30;
                MyCh.intMagic = 30;
                MyCh.intSnatch = 30;

                Myjob = "rogue";
            }
            // 개발자
            if (pntPosition == 3)
            {
                MyCh.intLevel = 1;
                MyCh.intHP = 10;
                MyCh.intMp = 10;
                MyCh.intAttack = 20;
                MyCh.intMagic = 0;
                MyCh.intSnatch = 10;

                Myjob = "programmer";
            }
            MySnatch = MyCh.intSnatch;

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
                        // 해당 아이템이 존재 하지 않으면
                        if (MyInven[intItemno].intItemCount <= 0)
                        {
                            continue;
                        }

                        UseItem(intItemno);

                        if (isItemUseEnd)
                        {
                            return;
                        }
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

                // 함정(확률: 15%)
                if (randValue < 15)
                {
                    cout << "--함정에 걸렸습니다. 체력이 20 하락합니다.  / Hp: " + to_string(MyCh.intHP - 20) << endl;
                    MyCh.intHP = MyCh.intHP - 20;
                    m_lastDamage = 1;
                }
                // NPC 발견(확률: 15%) 
                if (15 <= randValue && randValue < 30)
                {
                    // 시드값을 얻기 위한 random_device 생성.
                    std::random_device rdItem;

                    // random_device 를 통해 난수 생성 엔진을 초기화 한다.
                    std::mt19937 genItem(rdItem());

                    // 0 부터 99 까지 균등하게 나타나는 난수열을 생성하기 위해 균등 분포 정의.
                    std::uniform_int_distribution<int> disItem(0, 99);

                    // 난수를 한 번 생성해서 변수에 저장
                    int randItemValue = disItem(genItem);

                    // npc 별 등장확률
                    if (randItemValue < 5)
                    {
                        int res = thanos.NpcEvent(thanos.NpcNo, m_isLotto);
                        if (res == 100)
                        {
                            cout << "-- 50%의 확률을 뚫고 생존하였다. --";
                            _getch();
                            continue;
                        }
                        if (res == -100)
                        {
                            m_lastDamage = 8;
                            MyCh.intHP = 0;
                        }
                    }
                    if (5 <= randItemValue && randItemValue < 30)
                    {
                        int res = burglar.NpcEvent(burglar.NpcNo, m_isLotto);
                        // 도적두목에게 공격당함
                        if (res == -100)
                        {
                            cout << "-- 도적 두목에게 공격받았습니다. / Hp " << burglar.NpcAttkPnt << " 감소" << endl;

                            m_lastDamage = 7;
                            MyCh.intHP -= burglar.NpcAttkPnt;

                            _getch();
                        }
                        else
                        {
                            continue;
                        }
                    }
                    if (30 <= randItemValue && randItemValue < 90)
                    {
                        int res = merchant.NpcEvent(merchant.NpcNo, m_isLotto);
                        // 구매한 경우 인벤토리에 추가 처리
                        if (res >= 0 && res < 100)
                        {
                            AddInvent(res, 1);
                        }
                        if (res == -50)
                        {
                            cout << merchant.NpcName + ": 어딜 훔치려고." << "\n";
                            cout << "-- 상인에게 공격받았습니다. / Hp " << merchant.NpcAttkPnt << " 감소" << endl;


                            m_lastDamage = 9;
                            MyCh.intHP -= merchant.NpcAttkPnt;

                            _getch();
                        }
                    }
                    if (90 <= randItemValue && randItemValue < 100)
                    {
                        int res = lottoNpc.NpcEvent(lottoNpc.NpcNo, m_isLotto);
                        if (res == -100)
                        {
                            cout << MyCh.strName + ": 복권을 가지고 다시 오자." << "\n";
                            _getch();
                        }
                        if (res == 100)
                        {
                            _getch();
                        }
                        if (res == 101)
                        {
                            GameEnding(5, true);
                            cout << "프로그램을 종료합니다." << endl;
                            return;
                        }
                    }
                }

                // 아이템 발견(확률: 60%)
                if (30 <= randValue && randValue < 95)
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
                    // 물
                    else if (65 <= randItemValue && randItemValue < 70)
                    {
                        AddInvent(3, 1);
                    }
                    // 돈 획득
                    else if (70 <= randItemValue && randItemValue < 90)
                    {
                        int findMoney = 10;
                        MyMoney += findMoney;
                        cout << findMoney << " 원을 획득하였습니다. (현재 소지금 : " << MyMoney << ")\n";
                    }
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
                if (95 <= randValue && randValue < 98)
                {
                    cout << "--지뢰에 걸렸습니다. 체력이 50 하락합니다.  / Hp: " + to_string(MyCh.intHP - 50) << endl;
                    MyCh.intHP = MyCh.intHP - 50;
                    m_lastDamage = 2;
                }


                // 보물(2.5%)
                if (98 <= randValue && randValue < 100)
                {
                    GameEnding(4, true);
                    cout << "프로그램을 종료합니다." << endl;
                    return;
                }

                if (MyCh.intHP <= 0)
                {
                    GameEnding(m_lastDamage, false);
                    cout << "프로그램을 종료합니다." << endl;
                    return;
                }
            }

            // 게임 종료
            else if (pointerPosition == 3)
            {
                cout << "-- 종료하시겠습니까? --" << endl;
                vector<string> menuItems = { "종료하기", "돌아가기" };
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
                        // 종료하기
                        if (pointerPosition == 0)
                        {
                            cout << "프로그램을 종료합니다." << endl;
                            isContinue = false;
                            return;
                        }
                        // 돌아가기
                        if (pointerPosition == 1)
                        {
                            break;
                        }
                    }
                }
            }
            // Enter 입력 후 키 대기
            system("pause");
        }
    }
}

int main()
{
    while (true)
    {
        PlayGame();
        if (isContinue == false)
        {
            break;
        }
    }
    return 0;
}
