#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

extern void init();
extern int add(int mId, int mGrade, char mGender[7], int mScore);
extern int remove(int mId);
extern int query(int mGradeCnt, int mGrade[], int mGenderCnt, char mGender[][7], int mScore);

/////////////////////////////////////////////////////////////////////////

#define CMD_INIT 100
#define CMD_ADD 200
#define CMD_REMOVE 300
#define CMD_QUERY 400

static bool run() {
	int q;
	scanf("%d", &q);

	int id, grade, score;
	char gender[7];
	int cmd, ans, ret;
	bool okay = false;

	for (int i = 0; i < q; ++i) {
		scanf("%d", &cmd);
		switch (cmd) {
			case CMD_INIT:
				init();
				okay = true;
				break;
			case CMD_ADD:
				scanf("%d %d %s %d %d", &id, &grade, gender, &score, &ans);
				ret = add(id, grade, gender, score);
				if (ans != ret)
					okay = false;
				break;
			case CMD_REMOVE:
				scanf("%d %d", &id, &ans);
				ret = remove(id);
				if (ans != ret)
					okay = false;
				break;
			case CMD_QUERY: {
				int gradeCnt, genderCnt;
				int gradeArr[3];
				char genderArr[2][7];
				scanf("%d", &gradeCnt);
				if (gradeCnt == 1) {
					scanf("%d %d", &gradeArr[0], &genderCnt);
				} else if (gradeCnt == 2) {
					scanf("%d %d %d", &gradeArr[0], &gradeArr[1], &genderCnt);
				} else {
					scanf("%d %d %d %d", &gradeArr[0], &gradeArr[1], &gradeArr[2], &genderCnt);
				}
				if (genderCnt == 1) {
					scanf("%s %d %d", genderArr[0], &score, &ans);
				} else {
					scanf("%s %s %d %d", genderArr[0], genderArr[1], &score, &ans);
				}
				ret = query(gradeCnt, gradeArr, genderCnt, genderArr, score);
				if (ans != ret)
					okay = false;
				break;
			}
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

#include<map>
#include<set>
#include<string>
#include<unordered_map>

using namespace std;

struct record {
    int score, grade, gender;
};

map<int, set<int> > groups[2][3];
unordered_map<int, record> getRecord;

void init() {
    for (int gender = 0; gender < 2; gender++) {
        for (int grade = 0; grade < 3; grade++) {
            groups[gender][grade].clear();
        }
    }
    getRecord.clear();
	return;
}

int add(int mId, int mGrade, char mGender[7], int mScore) {
    string gender = mGender;
    int iGender = 1;
    if (gender == "male") iGender = 0;
    groups[iGender][mGrade-1][mScore].insert(mId);
    int ans = *groups[iGender][mGrade-1].rbegin()->second.rbegin();
    record rec = record();
    rec.gender = iGender; rec.grade = mGrade; rec.score = mScore;
    getRecord[mId] = rec;
	return ans;
}

int remove(int mId) {
    unordered_map<int, record>::iterator iter = getRecord.find(mId);
    int ans = 0;
    if (iter != getRecord.end()) {
        int grade = iter->second.grade;
        int gender = iter->second.gender;
        int score = iter->second.score;
        groups[gender][grade-1][score].erase(mId);
        if (groups[gender][grade-1][score].empty()) {
            groups[gender][grade-1].erase(score);
        }
        if (groups[gender][grade-1].empty()) {
            ans = 0;
        } else {
            ans = *groups[gender][grade-1].begin()->second.begin();
        }
        getRecord.erase(mId);
    }
	return ans;
}

int query(int mGradeCnt, int mGrade[], int mGenderCnt, char mGender[][7], int mScore) {
    int minScore, minId, answer;
    bool missing = true;
    for (int i = 0; i < mGradeCnt; i++) {
        int grade = mGrade[i];
        for (int j = 0; j < mGenderCnt; j++) {
            string gender = mGender[j];
            int iGender = 1;
            if (gender == "male") iGender = 0;
            map<int, set<int> >::iterator iter = groups[iGender][grade-1].upper_bound(mScore-1);
            if (iter != groups[iGender][grade-1].end()) {
                int score = iter->first;
                if (missing) {
                    missing = false;
                    minScore = score;
                    minId = *iter->second.begin();
                } else {
                    if (score < minScore) {
                        minScore = score;
                        minId = *iter->second.begin();
                    } else if (score == minScore) {
                        int id = *iter->second.begin();
                        if (id < minId) minId = id;
                    }
                }
            }
        }
    }
    if (missing) return 0;
    return minId;
}
