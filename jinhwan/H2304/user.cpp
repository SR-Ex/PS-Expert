#include <unordered_map>
#include <map>

int total_building_num;

std::unordered_map<int, int> mIdKeyHash;
std::map<int, int> mLocationKeyHash;

void init(int N, int mId[], int mLocation[]) {
    mIdKeyHash.clear();
    mLocationKeyHash.clear();
	total_building_num = N;
    for(int i = 0; i< N; i++){
        mIdKeyHash.insert(std::pair<int,int>(mId[i], mLocation[i]));
        mLocationKeyHash.insert(std::pair<int,int>(mLocation[i], mId[i]));
    }
    return;
}

int add(int mId, int mLocation) {
    auto mIdKeyHash_ptr = mIdKeyHash.find(mId);
    if(mIdKeyHash_ptr != mIdKeyHash.end()){
        int last_mLocation = mIdKeyHash_ptr->second;
        mIdKeyHash_ptr->second = mLocation;
        mLocationKeyHash.erase(last_mLocation);
        mLocationKeyHash.insert(std::pair<int,int>(mLocation, mId));
    }else{
        mIdKeyHash.insert(std::pair<int,int>(mId, mLocation));
        mLocationKeyHash.insert(std::pair<int,int>(mLocation, mId));
        total_building_num += 1;
    }
	return total_building_num;
}

int remove(int mStart, int mEnd) {
    auto startBuilding = mLocationKeyHash.lower_bound(mStart);
    auto endBuilding = mLocationKeyHash.upper_bound(mEnd);
    int mId;
    for(auto building = startBuilding; building != endBuilding; ++building){
        mId = building->second;
        mIdKeyHash.erase(mId);
        total_building_num -= 1;
    }
    mLocationKeyHash.erase(startBuilding, endBuilding);
	return total_building_num;
}

int install(int M) {
    int beginLocation = mLocationKeyHash.begin()->first;
    int endLocation = mLocationKeyHash.rbegin()->first;
    if(M == 2){
        return endLocation - beginLocation;
    }

    int station_placed_cnt, mid, high, low, ans;
    low = 1e9;
    int last_location = - 1;
    for(auto location = mLocationKeyHash.begin(); location != mLocationKeyHash.end(); ++location){
        if(last_location == -1){
            last_location = location->first;
        }else{
            if(low > location->first - last_location){
                low = location->first - last_location;
            }
            last_location = location->first;
        }
    }
    high = endLocation - beginLocation;
    while(low <= high){
        mid = (high + low) / 2;
        station_placed_cnt = 1;
        int lastest_station_pos = mLocationKeyHash.begin()->first;
        for(auto location = mLocationKeyHash.begin(); location != mLocationKeyHash.end(); ++location){
            if(location->first - lastest_station_pos >= mid){
                lastest_station_pos = location->first;
                station_placed_cnt += 1;
            }
        }
        if(station_placed_cnt >= M){
            ans = mid;
            low = mid + 1;

        }else{
            high = mid - 1;
        }
    }
	return ans;
}