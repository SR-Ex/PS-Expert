#include <set>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

const int LM = 30000;
using building = pair<int, int>;
set<building> building_set;
unordered_map <int, building> building_hash;
int Arr[LM];

void init(int N, int mId[], int mLocation[]){
    building_hash.clear();
    building_set.clear();
    for(int i = 0; i < N ; i++){
        building_hash[mId[i]] = pair<int,int>(mLocation[i], mId[i]);
        building_set.insert(pair<int,int>(mLocation[i], mId[i]));
    }
}

int add(int mId, int mLocation){
    auto it = building_hash.find(mId);
    if(it != building_hash.end()){
        building_set.erase(it->second);
    }
    building_hash[mId] = pair<int,int>(mLocation, mId);
    building_set.insert(pair<int,int>(mLocation, mId));
    return (int) building_hash.size();
}

int remove(int mStart, int mEnd){
    auto it = building_set.lower_bound(pair<int,int>(mStart, 0));
    while(it != building_set.end()){
        if(it->first > mEnd) break;
        building_hash.erase(it->second);
        it = building_set.erase(it);
    }
    return (int)building_hash.size();
}

int install(int M){
    int low = (int)1e9, mid, high;
    int i, ans, len =0;
    for(auto &p : building_set){
        Arr[++len] = p.first;
    }
    for(i = 2; i <= len; ++i){
        low = min(low, Arr[i] - Arr[i -1]);
    }
    ans = high = Arr[len] - Arr[1];
    while (low <= high){                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
        mid = (low + high) / 2;
        int cnt = 1, latest = Arr[1];
        for( i = 2; i <= len; ++i){
            if(Arr[i] - latest >= mid){
                cnt++, latest = Arr[i];
            }
        }
        if (cnt >= M){
            ans = mid;
            low = mid + 1;
        }else{
            high = mid - 1;
        }
    }
    return ans;
}