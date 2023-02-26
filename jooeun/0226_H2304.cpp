#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

extern void init(int N, int mId[], int mLocation[]);
extern int add(int mId, int mLocation);
extern int remove(int mStart, int mEnd);
extern int install(int M);

/////////////////////////////////////////////////////////////////////////

#define CMD_INIT 1
#define CMD_ADD 2
#define CMD_REMOVE 3
#define CMD_INSTALL 4

static bool run() {
	int q;
	scanf("%d", &q);

	int n, mid, mloc, mstart, mend, m;
	int midArr[100], mlocArr[100];
	int cmd, ans, ret = 0;
	bool okay = false;

	for (int i = 0; i < q; ++i) {
		scanf("%d", &cmd);
        // printf("CMD %d\n", cmd);
		switch (cmd) {
			case CMD_INIT:
				scanf("%d", &n);
				for (int j = 0; j < n; ++j) {
					scanf("%d %d", &midArr[j], &mlocArr[j]);
				}
				init(n, midArr, mlocArr);
				okay = true;
				break;
			case CMD_ADD:
				scanf("%d %d %d", &mid, &mloc, &ans);
				ret = add(mid, mloc);
                // printf("ans %d, ret %d\n", ans, ret);
				if (ans != ret)
					okay = false;
				break;
			case CMD_REMOVE:
				scanf("%d %d %d", &mstart, &mend, &ans);
				ret = remove(mstart, mend);
                // printf("ans %d, ret %d\n", ans, ret);
				if (ans != ret)
					okay = false;
				break;
			case CMD_INSTALL:
				scanf("%d %d", &m, &ans);
				ret = install(m);
                // printf("ans %d, ret %d\n", ans, ret);
				if (ans != ret)
					okay = false;
				break;
			default:
				okay = false;
				break;
		}
	}
	return okay;
}

int main() {
	setbuf(stdout, NULL);
	freopen("sample_input.txt", "r", stdin);

	int T, MARK;
	scanf("%d %d", &T, &MARK);

	for (int tc = 1; tc <= T; tc++) {
		int score = run() ? MARK : 0;
		printf("#%d %d\n", tc, score);
	}

	return 0;
}


#include <vector>
#include <unordered_map>
#include <algorithm>

std::vector<int> array(30000, 0);
std::unordered_map<int, int> id2loc;
std::unordered_map<int, int> loc2id;
int count;
int answer;

void init(int N, int mId[], int mLocation[]) {
    count = 0;
    array.clear();
    id2loc.clear();
    loc2id.clear();
    for (int i=0; i<N; i++) {
        int id = mId[i];
        int loc = mLocation[i];
        array.push_back(loc);
        id2loc.insert(std::make_pair(id, loc));
        loc2id.insert(std::make_pair(loc, id));
        count++;
    }
	return;
}

int add(int mId, int mLocation) {
    if(id2loc.find(mId)==id2loc.end()) {
        count++;
    } else {
        auto it = std::find(array.begin(), array.end(), id2loc[mId]);
        array.erase(it);
        loc2id.erase(id2loc[mId]);
        id2loc.erase(mId);
    }
    id2loc.insert(std::make_pair(mId, mLocation));
    loc2id.insert(std::make_pair(mLocation, mId));
    array.push_back(mLocation);
	return count;
}

int remove(int mStart, int mEnd) {
    std::sort(array.begin(), array.end());
    int sRange = 0;
    int eRange = 0;
    for (auto& i : array) {
        if (i > mEnd) {
            break;
        } else if (i < mStart) {
            sRange++;
        } else {
            count--;
            id2loc.erase(loc2id[i]);
            loc2id.erase(i);
        }
        eRange++;
    }
    array.erase(array.begin() + sRange, array.begin() + eRange);
	return count;
}

void search(int M, int min_d, int max_d) {
    if (min_d > max_d) {
        return;
    }
    int d = min_d + (max_d - min_d) / 2;
    int tel = 1;
    int start = array[0];
    for (auto& i : array) {
        if (start + d <= i) {
            tel++;
            start = i;
        }
    }
    if (tel < M) {
        search(M, min_d, d-1);
    } else {
        answer = d;
        search(M, d+1, max_d);
    }
    return;
}

int install(int M) {
    std::sort(array.begin(), array.end());
    answer = 1;
    search(M, 1, array[count-1] - array[0]);
	return answer;
}
