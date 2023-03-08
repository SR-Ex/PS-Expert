#include <stdio.h>
#include <queue>
#include <vector>
#include <cmath>
 
#define MAX_CITY 201
 
using namespace std;
 
 
struct Act {
    int type, from, to, amount, time;
};
 
struct City {
    int ID;
    int amount;
}Cities[MAX_CITY];
 
struct cmp{
    bool operator()(Act a1, Act a2) {
        if (a1.time == a2.time) {
            return a1.type > a2.type;
        }
        else {
            return a1.time > a2.time;
        }
    }
};
 
int N, M;
int expected[MAX_CITY];
priority_queue<Act, vector<Act>, cmp> actQueue;
bool sending[MAX_CITY];
 
void sendOfficer(int tStamp) {
 
    if (M < 1) {
        return;
    }
    else {
        while (M > 0) {
            int _max = 0;
            int _maxIdx = -1;
            for (int i = 1; i < N; ++i) {
                if (expected[i] > _max && !sending[i]) {
                    _maxIdx = i;
                    _max = expected[i];
                }
            }
            if (_maxIdx == -1) {
                return;
            }
            else {
                Act new_act{};
                new_act.type = 1;
                new_act.to = _maxIdx;
                new_act.time = tStamp + _maxIdx;
                actQueue.push(new_act);
                M -= 1;
                sending[_maxIdx] = true;
            }
        }
    }
 
}
 
void simulation(int tStamp) {
 
    if (actQueue.empty()) {
        return;
    }
    Act cur = actQueue.top();
    while (cur.time <= tStamp) {
        actQueue.pop();
        if (cur.type == 0) {
            int idx = cur.to;
            Cities[idx].amount += cur.amount;
        }
        else if (cur.type == 1) {
            int destination = cur.to;
            int remain = Cities[destination].amount;
            expected[cur.to] -= Cities[destination].amount;
            Cities[destination].amount = 0;
            Act newAct{};
            newAct.amount = remain;
            newAct.to = 0;
            newAct.from = cur.to;
            newAct.type = 2;
            newAct.time = cur.time + cur.to;
            actQueue.push(newAct);
        }
        else if (cur.type == 3) {
            expected[cur.to] += cur.amount;
            sendOfficer(cur.time);
        }
        else {
            Cities[0].amount += cur.amount;
            sending[cur.from] = false;
            M += 1;
            sendOfficer(cur.time);
        }
        if (actQueue.empty()) {
            break;
        }
        else {
            cur = actQueue.top();
        }
         
    }
 
}
 
void init(int _N, int _M)
{
    N = _N;
    M = _M;
    for (int i = 0; i < N; i++) {
        Cities[i].amount = 0;
        sending[i] = false;
        expected[i] = 0;
    }
}
 
void destroy()
{
    while (!actQueue.empty()) {
        actQueue.pop();
    }
}
 
int order(int tStamp, int mCityA, int mCityB, int mTax)
{
    simulation(tStamp-1);
    Act newAct;
    int distance = abs(mCityA - mCityB);
    newAct.type = 0;
    newAct.amount = mTax;
    newAct.from = mCityA;
    newAct.to = mCityB;
    newAct.time = tStamp + distance;
    actQueue.push(newAct);
    if (distance <= mCityB) {
        expected[mCityB] += mTax;
        sendOfficer(tStamp);
    }
    else {
        newAct.type = 3;
        newAct.time = tStamp + distance - mCityB;
        newAct.to = mCityB;
        newAct.amount = mTax;
        actQueue.push(newAct);
    }
    simulation(tStamp);
    return Cities[0].amount;
}
 
int check(int tStamp)
{
    simulation(tStamp);
    return Cities[0].amount;
}