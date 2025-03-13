#pragma once
#include <iostream>
#include <vector>
#include <string>
using namespace std;

#ifndef GAMEPROJECT_H
#define GAMEPROJECT_H

// 복권 소유 여부
static bool m_isLotto = false;

// 마지막 데미지 원인
// 0: 독 1: 함정 2: 지뢰
static int m_lastDamage = -1;

extern int MyMoney;

// 직업
extern string Myjob;
extern string MyName;

#endif