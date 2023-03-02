/*
@민성아빠 님의 코드를 바탕으로 작성된 코드입니다
*/
#include <iostream>
#include <string>
#include <unordered_map>
#include <algorithm>

using namespace std;

#define MAXN			(500)
#define MAXL			(11)
#define MAX     501*2

int allyCnt = 0;

unordered_map<string, int> indexingComp;

int Root[MAX];

int Proj[MAX][MAX];
 
int Ally[MAX*2];

int N;

int getRoot(int idx)
{
    if(idx == Root[idx]) return idx;
    Root[idx] = getRoot(Root[idx]);
    return Root[idx];
}

void init(int mNumA, char mCompanyListA[MAXN][MAXL], int mNumB, char mCompanyListB[MAXN][MAXL])
{
    N = mNumA + mNumB;
    indexingComp.clear();
    memset(Ally, 0, sizeof(Ally));
    for (int i = 0; i < MAX; i++)
    {
        memset(Proj[i], 0, sizeof(Proj[i]));
        Root[i] = i;
    }
    for (int i = 0; i < mNumA; i++)
    {
        indexingComp.insert({ mCompanyListA[i], i });
        Ally[i] = 1;
    }
    for (int i = 0; i < mNumB; i++)
    {
        indexingComp.insert({ mCompanyListB[i], i + mNumA });
        Ally[i + mNumA] = 1;
    }
    
}

void startProject(char mCompanyA[MAXL], char mCompanyB[MAXL])
{
    string nameA = mCompanyA;
    string nameB = mCompanyB;
    int idxA = getRoot(indexingComp[nameA]);
    int idxB = getRoot(indexingComp[nameB]);
    Proj[idxA][idxB] += 1;
    Proj[idxB][idxA] += 1;
}

void finishProject(char mCompanyA[MAXL], char mCompanyB[MAXL])
{
    int idxA = getRoot(indexingComp[mCompanyA]);
    int idxB = getRoot(indexingComp[mCompanyB]);
    Proj[idxA][idxB] -= 1;
    Proj[idxB][idxA] -= 1;
}

void ally(char mCompany1[MAXL], char mCompany2[MAXL])
{
    int origin = getRoot(indexingComp[mCompany1]);
    int target = getRoot(indexingComp[mCompany2]);
    
    if(origin == target) return;

    if(Ally[target] > Ally[origin]){
        int tmp = target; target = origin; origin = tmp;
    }

    for(int i = 0 ; i < N; i++)
    {
        if (Proj[target][i] == 0) continue;
        Proj[origin][i] += Proj[target][i];
        Proj[i][origin] += Proj[i][target];

        Proj[target][i] = 0; Proj[i][target] = 0; 
    }
    
    Ally[origin] += Ally[target];
    Root[target] = origin;
    Ally[target] = 0;

}

int conflict(char mCompany1[MAXL], char mCompany2[MAXL])
{
    int ally1 = getRoot(indexingComp[mCompany1]);
    int ally2 = getRoot(indexingComp[mCompany2]);

    int cnt = 0;
    for(int i = 0; i < N; i++){
        if(Proj[ally1][i] > 0 && Proj[ally2][i] > 0)
        {
            cnt += Ally[i];
        }
    }
    return cnt;
}