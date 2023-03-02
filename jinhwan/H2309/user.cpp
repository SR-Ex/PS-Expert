#include <algorithm>
#include <vector>
#include <unordered_map>
#include <map>

#define MAX_N 100001
#define MAX_M 1001

using namespace std;

int sectionTime[MAX_N] = {0,};
unordered_map<int, vector<int>> typeSections;

int N, M;

void init(int _N, int _M, int mType[], int mTime[])
{
    typeSections.clear();
    N = _N;
    M = _M;
    for(int sectionId = 0 ; sectionId < N; sectionId++){
        typeSections[mType[sectionId]].push_back(sectionId);
        sectionTime[sectionId] = mTime[sectionId];
    }
}

void destroy()
{  
    
}

void update(int mID, int mNewTime)
{
    sectionTime[mID] = mNewTime;
}

int updateByType(int mTypeID, int mRatio256)
{
    int result = 0;
    sort(typeSections[mTypeID].begin(), typeSections[mTypeID].end());
    for(auto sectionId : typeSections[mTypeID]){
        int time = (sectionTime[sectionId] * mRatio256) / 256;
        sectionTime[sectionId] = time;
        result += time;
    }
	return result;
}

int calculate(int mA, int mB)
{  
    int from = mA;
    int to = mB;
    if(from > to){
        int tmp;
        tmp = to, to = from, from = tmp;
    }

    int result = 0;
    for(int idx = from; idx < to; idx++){
        
        result += sectionTime[idx];
    }
	return result;
}