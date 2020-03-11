#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <Windows.h>
#include <simple2d.h>


#define TIMER_GUARD(a, t)                    \
static DWORD lastTick##a = ::GetTickCount(); \
if (GetTickCount() - lastTick##a < t)        \
  return;                                    \
lastTick##a = ::GetTickCount();

using namespace std;

template<typename T>
vector<T> rangeint(T from, T to)
{
  // #STL homework implement in a more efficient way
  vector<T> ret(to - from + 1);
  iota(begin(ret), end(ret), from);
  return ret;
}
