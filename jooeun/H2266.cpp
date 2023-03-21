#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#define MAXN			(500)
#define MAXL			(11)

#define CMD_INIT			(100)
#define CMD_START_PROJECT	(200)
#define CMD_FINISH_PROJECT	(300)
#define CMD_ALLY			(400)
#define CMD_CONFLICT		(500)

extern void init(int mNumA, char mCompanyListA[MAXN][MAXL], int mNumB, char mCompanyListB[MAXN][MAXL]);
extern void startProject(char mCompanyA[MAXL], char mCompanyB[MAXL]);
extern void finishProject(char mCompanyA[MAXL], char mCompanyB[MAXL]);
extern void ally(char mCompany1[MAXL], char mCompany2[MAXL]);
extern int conflict(char mCompany1[MAXL], char mCompany2[MAXL]);

static bool run()
{
	int Q;	
	int mNumA, mNumB;
	
	static char mCompanyListA[MAXN][MAXL];
	static char mCompanyListB[MAXN][MAXL];

	char mCompanyA[MAXL], mCompanyB[MAXL];
	char mCompany1[MAXL], mCompany2[MAXL];
	
	int ret = -1, ans;

	scanf("%d", &Q);
	
	bool okay = false;
	
	for (int q = 0; q < Q; ++q)
	{
		int cmd;
		scanf("%d", &cmd);
		switch(cmd)
		{
		case CMD_INIT:
			scanf("%d", &mNumA);
			for (int i = 0; i < mNumA; ++i)
				scanf("%s", mCompanyListA[i]);
			scanf("%d", &mNumB);
			for (int i = 0; i < mNumB; ++i)
				scanf("%s", mCompanyListB[i]);
			init(mNumA, mCompanyListA, mNumB, mCompanyListB);
			okay = true;
			break;
		case CMD_START_PROJECT:
			scanf("%s %s", mCompanyA, mCompanyB);
			startProject(mCompanyA, mCompanyB);
			break;
		case CMD_FINISH_PROJECT:
			scanf("%s %s", mCompanyA, mCompanyB);
			finishProject(mCompanyA, mCompanyB);
			break;
		case CMD_ALLY:
			scanf("%s %s", mCompany1, mCompany2);
			ally(mCompany1, mCompany2);
			break;
		case CMD_CONFLICT:
			scanf("%s %s", mCompany1, mCompany2);
			ret = conflict(mCompany1, mCompany2);
			scanf("%d", &ans);
			if (ret != ans)
				okay = false;
			break;
		default:
			okay = false;
			break;
		}
	}

	return okay;
}

int main()
{
	setbuf(stdout, NULL);
	freopen("sample_input.txt", "r", stdin);

	int TC, MARK;

	scanf("%d %d", &TC, &MARK);

	for (int tc = 1; tc <= TC; ++tc)
	{
		int score = run() ? MARK : 0;
		printf("#%d %d\n", tc, score);		
	}

	return 0;
}

#define MAXN			(500)
#define MAXL			(11)
#include<string>
#include<unordered_map>
#include<unordered_set>

std::unordered_map<std::string, int> getIdx;
std::unordered_set<int> S[MAXN * 2+1];
int parent[MAXN * 2+1], cnt[MAXN * 2+1];
int con[MAXN * 2+1][MAXN * 2+1];

int find_root(int x){
    if (x == parent[x]) return x;
    return parent[x] = find_root(parent[x]);
}

void init(int mNumA, char mCompanyListA[MAXN][MAXL], int mNumB, char mCompanyListB[MAXN][MAXL])
{
    getIdx.clear();
    int idx = 0;
    for (int i=0; i<mNumA; i++) {
        std::string name = mCompanyListA[i];
        parent[idx] = idx;
        cnt[idx] = 1;
        S[idx].clear();
        for (int j = 0; j <= mNumA+mNumB; ++j) con[idx][j] = 0;
        getIdx.insert(std::make_pair(name, idx++));
    }
    for (int i=0; i<mNumB; i++) {
        std::string name = mCompanyListB[i];
        parent[idx] = idx;
        cnt[idx] = 1;
        S[idx].clear();
        for (int j = 0; j <= mNumA+mNumB; ++j) con[idx][j] = 0;
        getIdx.insert(std::make_pair(name, idx++));
    }
}
	
void startProject(char mCompanyA[MAXL], char mCompanyB[MAXL])
{
	int a = getIdx[mCompanyA];
    int b = getIdx[mCompanyB];
    int ap = find_root(a), bp = find_root(b);
    // S[ap].insert(b);
    // S[bp].insert(a);
    if (con[ap][b]++ == 0) S[ap].insert(b);
    if (con[bp][a]++ == 0) S[bp].insert(a);
}

void finishProject(char mCompanyA[MAXL], char mCompanyB[MAXL])
{
	int a = getIdx[mCompanyA];
    int b = getIdx[mCompanyB];
    int ap = find_root(a), bp = find_root(b);
    // S[ap].erase(b);
    // S[bp].erase(a);
    if (--con[ap][b] == 0) S[ap].erase(b);
    if (--con[bp][a] == 0) S[bp].erase(a);
}

void ally(char mCompany1[MAXL], char mCompany2[MAXL])
{
	int a = getIdx[mCompany1];
    int b = getIdx[mCompany2];
    int ap = find_root(a), bp = find_root(b);
    if (ap == bp) return;
    if (S[ap].size() > S[bp].size()) {
        parent[bp] = ap;
        cnt[ap] += cnt[bp];
        for (auto it : S[bp]) {
            // S[ap].insert(it);
            if (!con[ap][it]) S[ap].insert(it);
            con[ap][it] += con[bp][it];
        }
    } else {
        parent[ap] = bp;
        cnt[bp] += cnt[ap];
        for (auto it : S[ap]) {
            // S[bp].insert(it);
            if (!con[bp][it]) S[bp].insert(it);
            con[bp][it] += con[ap][it];
        }
    }
}

int conflict(char mCompany1[MAXL], char mCompany2[MAXL])
{
    int a = getIdx[mCompany1];
    int b = getIdx[mCompany2];
    int ap = find_root(a), bp = find_root(b);
    int visit[1001] = {}, res = 0;
    for (auto i : S[ap]) ++visit[find_root(i)];
    for (auto i : S[bp]){
        int p = find_root(i);
        if (visit[p]) {
            visit[p] = 0;
            res += cnt[p];
        }
    }
    return res;
}
