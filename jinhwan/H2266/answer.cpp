#include <cstring>
#include <unordered_map>
#include <map>
#include <string>
#include <set>
 
#define MAXN            (500)
#define MAXL            (11)
#define MAX     501*2
 
using namespace std;
 
using namespace std;
 
int N;
int Root[MAX];
map <string, int> ComId;
 
//map < int, map <int, int >> Prj;
 
int Prj[MAX][MAX];
 
//map < int, set< int >> Ally;
int Ally[MAX*2];
 
int getRoot(int idx)
{
    if (idx == Root[idx]) return idx;
 
    Root[idx] = getRoot(Root[idx]);
    return Root[idx];
}
 
 
void init(int mNumA, char mCompanyListA[MAXN][MAXL], int mNumB, char mCompanyListB[MAXN][MAXL])
{
    N = mNumA + mNumB;
    ComId.clear();
     
    memset(Ally, 0, sizeof(Ally));
 
    for (int i = 0; i < MAX; i++)
    {
        memset(Prj[i], 0, sizeof(Prj[i]));
        Root[i] = i;
//      Ally[i].clear();
    }
 
    for (int i = 0; i < mNumA; i++)
    {
        ComId.insert({ mCompanyListA[i], i });
        Ally[i] = 1;
    }
    for (int i = 0; i < mNumB; i++)
    {
        ComId.insert({ mCompanyListB[i], i + mNumA });
        Ally[i + mNumA] = 1;
    }
}
 
void startProject(char mCompanyA[MAXL], char mCompanyB[MAXL])
{
    int allyId1 = getRoot(ComId[mCompanyA]);
    int allyId2 = getRoot(ComId[mCompanyB]);
 
    Prj[allyId1][allyId2] += 1;
    Prj[allyId2][allyId1] += 1;
 
}
 
void finishProject(char mCompanyA[MAXL], char mCompanyB[MAXL])
{
    int allyId1 = getRoot(ComId[mCompanyA]);
    int allyId2 = getRoot(ComId[mCompanyB]);
 
    Prj[allyId1][allyId2] -= 1;
    Prj[allyId2][allyId1] -= 1;
}
 
void ally(char mCompany1[MAXL], char mCompany2[MAXL])
{
    int allyId1 = getRoot(ComId[mCompany1]);
    int allyId2 = getRoot(ComId[mCompany2]);
 
    if (allyId1 == allyId2) return;
    //trimAllyId(allyId1, allyId2);
    int from = allyId1, to = allyId2;
 
 
    if (Ally[from] > Ally[to]) // 작은넘이 큰넘으로 합쳐진다.
    {
        int tmp = from; from = to; to = tmp;
    }
 
     
    // [from][??] --> [to][??]
 
    for (int i = 0; i < N; i++)
    {
        // Prj [from][???]  && Prj [???][from] 
        // ==> Prj [to][???]  && Prj [???][ to ] 
 
        if (Prj[from][i] == 0 )  continue;
 
        Prj[to][i] += Prj[from][i];
        Prj[i][to] += Prj[i][from];
 
        Prj[from][i] = 0; Prj[i][from] = 0;
    }
 
     
    Ally[to] += Ally[from];
    Root[from] = to;
    Ally[from] =0;
}
 
int conflict(char mCompany1[MAXL], char mCompany2[MAXL])
{
    //string cmp1= mCompany1, cmp2= mCompany2;
    //if (cmp1 == "lxt" && cmp2 == "qwlwwdsnt")
        //int aaa = 9;
 
    int allyId1 = getRoot(ComId[mCompany1]);
    int allyId2 = getRoot(ComId[mCompany2]);
 
    int cnt = 0;
    for (int i = 0; i < N; i++)
    {
        if (Prj[allyId1][i] > 0 && Prj[allyId2][i] > 0)
        {
            cnt += Ally[i];
        }
    }
 
    return cnt;
}