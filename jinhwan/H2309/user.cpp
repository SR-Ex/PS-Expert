#include <algorithm>
#include <vector>
#include <unordered_map>
#include <map>

#define MAX_N 100001
#define MAX_M 1001

using namespace std;

int sectionTime[MAX_N] = {0,};
int segmentTree[MAX_N*4];
int sectionType[MAX_N][MAX_M];

unordered_map<int, vector<int>> typeSections;

int N, M;

void build(int node, int start, int end)
{
    if(start == end)
    {
        segmentTree[node] = sectionTime[start];
    }else
    {
        int mid = (start + end) / 2;
        build(2*node, start, mid);
        build(2*node + 1, mid + 1, end);
        segmentTree[node] = segmentTree[2*node] + segmentTree[2*node + 1];
    }
}

void updateTree(int node, int start, int end, int mID, int diff)
{
    if(mID < start || mID > end) return;
    if(start == end)
    {
        if(mID == start) segmentTree[node] += diff;
    }
    else{
        segmentTree[node] += diff;
        int mid = (start + end) / 2;
        updateTree(2*node, start, mid, mID, diff);
        updateTree(2*node + 1, mid + 1, end, mID, diff);
    }
}

int sum(int node, int start, int end, int left, int right)
{
    if( right < start  || end < left) return 0;
    if( left <= start && end <= right) return segmentTree[node];
    int mid = (start + end) / 2;
    int leftSum = sum(2*node, start, mid, left, right);
    int rightSum = sum(2*node + 1, mid + 1, end, left, right);
    return leftSum + rightSum;
}

void init(int _N, int _M, int mType[], int mTime[])
{
    typeSections.clear();
    N = _N;
    M = _M;
    for(int sectionId = 0 ; sectionId < N; sectionId++){
        typeSections[mType[sectionId]].push_back(sectionId);
        sectionTime[sectionId] = mTime[sectionId];
    }
    build(1, 0, N - 2);
}

void destroy()
{  

}

void update(int mID, int mNewTime)
{
    int diff = mNewTime - sectionTime[mID];
    sectionTime[mID] = mNewTime;
    updateTree(1, 0, N - 2, mID, diff);
}

int updateByType(int mTypeID, int mRatio256)
{
    int result = 0;
    for(auto sectionId : typeSections[mTypeID]){
        int time = (sectionTime[sectionId] * mRatio256) / 256;
        int diff = time - sectionTime[sectionId];
        sectionTime[sectionId] = time;
        updateTree(1, 0, N -2, sectionId, diff);
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

	return sum(1, 0, N-2, from, to - 1);
}