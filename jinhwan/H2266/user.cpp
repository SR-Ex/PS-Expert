#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <set>

using namespace std;

#define MAXN			(500)
#define MAXL			(11)

int allyCnt = 0;

unordered_map<string, int> indexingComp;
unordered_map<int, set<int>> allyHash;
unordered_map<int, int> compAllyHash; //key는 회사 인덱스
unordered_map<int, set<int>> ProjectHash; //key는 회사 인덱스, vector는 같이 프로젝트 진행하는 회사들의 index로 이루어진 vector


void init(int mNumA, char mCompanyListA[MAXN][MAXL], int mNumB, char mCompanyListB[MAXN][MAXL])
{
    indexingComp.clear();
    allyHash.clear();
    compAllyHash.clear();
    ProjectHash.clear();
    allyCnt = 0;
    int cnt = 0;
	for (int i = 0; i < mNumA; i++) {
		string name = mCompanyListA[i];
		indexingComp[name] = cnt;
        allyHash[allyCnt].insert(cnt);
        compAllyHash[cnt] = allyCnt;
        cnt += 1;
        allyCnt += 1;
	}
	for (int j = 0; j < mNumB; j++) {
		string name = mCompanyListB[j];
		indexingComp[name] = cnt;
        allyHash[allyCnt].insert(cnt);
        compAllyHash[cnt] = allyCnt;
        cnt += 1;
        allyCnt += 1;
    }
}

void startProject(char mCompanyA[MAXL], char mCompanyB[MAXL])
{
	string nameA = mCompanyA;
	string nameB = mCompanyB;
    int aIdx = indexingComp[nameA];
    int bIdx = indexingComp[nameB];
	ProjectHash[aIdx].insert(bIdx);
	ProjectHash[bIdx].insert(aIdx);
}

void finishProject(char mCompanyA[MAXL], char mCompanyB[MAXL])
{
	string nameA = mCompanyA;
    string nameB = mCompanyB;
    int aIdx = indexingComp[nameA];
    int bIdx = indexingComp[nameB];
    ProjectHash[aIdx].erase(bIdx);
    ProjectHash[bIdx].erase(aIdx);
}

void ally(char mCompany1[MAXL], char mCompany2[MAXL])
{

    string com1 = mCompany1;
    string com2 = mCompany2;
    int com1Idx = indexingComp[com1];
    int com2Idx = indexingComp[com2];
    set<int> ally1 = allyHash[compAllyHash[com1Idx]];
    set<int> ally2 = allyHash[compAllyHash[com2Idx]];
    set_union(ally1.begin(), ally1.end(), ally2.begin(), ally2.end(), inserter(ally1, ally1.begin()));
    allyCnt += 1;
    compAllyHash[com1Idx] = allyCnt;
    compAllyHash[com2Idx] = allyCnt;
    for(auto s : ally1){
        compAllyHash[s] = allyCnt;
    }
    allyHash[allyCnt] = ally1;

}

int conflict(char mCompany1[MAXL], char mCompany2[MAXL])
{
    
    string com1 = mCompany1;
    string com2 = mCompany2;
    int com1Idx = indexingComp[com1];
    int com2Idx = indexingComp[com2];

    set<int> com1AllyProject;
    set<int> com2AllyProject;
    set<int> com1Ally = allyHash[compAllyHash[com1Idx]];
    set<int> com2Ally = allyHash[compAllyHash[com2Idx]];

    //자신을 포함한 동맹들의 프로젝트 기업들
    for(auto s : com1Ally){
        set<int> projectCompSet = ProjectHash[s];
        set_union(com1AllyProject.begin(), com1AllyProject.end(), projectCompSet.begin(), projectCompSet.end(), inserter(com1AllyProject, com1AllyProject.begin()));
        for(auto s2: projectCompSet){
            set<int> allySet = allyHash[compAllyHash[s2]];
            set_union(com1AllyProject.begin(), com1AllyProject.end(), allySet.begin(), allySet.end(), inserter(com1AllyProject, com1AllyProject.begin()));
        }
    }
    

    for(auto s : com2Ally){
        set<int> projectCompSet = ProjectHash[s];
        set_union(com2AllyProject.begin(), com2AllyProject.end(), projectCompSet.begin(), projectCompSet.end(), inserter(com2AllyProject, com2AllyProject.begin()));
        for(auto s2: projectCompSet){
            set<int> allySet = allyHash[compAllyHash[s2]];
            set_union(com2AllyProject.begin(), com2AllyProject.end(), allySet.begin(), allySet.end(), inserter(com2AllyProject, com2AllyProject.begin()));
        }
    }
    set<int> intersectComp;
    set_intersection(com1AllyProject.begin(), com1AllyProject.end(), com2AllyProject.begin(), com2AllyProject.end(), inserter(intersectComp, intersectComp.begin()));
    
    int answer = intersectComp.size();
    return answer;
}