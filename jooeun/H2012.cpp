#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

typedef enum {
	INIT,
	EDIT,
	EDITDONE,
	ADD,
	DELETE,
	SUBSTITUTE
} COMMAND;

extern void Init(int width, int length, char str[]);
extern void Edit(int id, int row, int col);
extern void Add(int id, int length, char str[]);
extern void Delete(int id, int length);
extern void Substitute(int id, int length, char str[]);
extern int EditDone(int id);

static int mSeed;
static int pseudo_rand(void)
{
	mSeed = mSeed * 431345 + 2531999;
	return mSeed & 0x7FFFFFFF;
}

static int initstr(char initbuf[], int N, int Max, int Min, int RatioN)
{
	int line = 0;
	int cnt = 0;
	int idx = 0;
	while (line <= N) {
		if (cnt == Max || (Min < cnt && pseudo_rand() % 100 >= RatioN)) {
			if (line == N) break;
			initbuf[idx++] = '\n';
			++line;
			cnt = 0;
		}
		else {
			initbuf[idx++] = 'a' + pseudo_rand() % 26;
			++cnt;
		}
	}
	return idx;
}

static int run()
{
	int cmd, p1, p2, p3, p4, p5, p6, result, length;
	int sample_test;
	char str[201];
	char initbuf[100000];

	scanf("%d %d %d %d %d %d %d %d", &cmd, &p1, &p2, &p3, &p4, &p5, &p6, &sample_test);
	if (sample_test == 1) {
		length = p2;
		scanf("%s", initbuf);
		for (int i = 0; i < length; ++i)
			if (initbuf[i] == 'N') initbuf[i] = '\n';
	}
	else {
		mSeed = p2;
		length = initstr(initbuf, p3, (p4 / 1000), (p4 % 1000), p5);
	}

	Init(p1, length, initbuf);

	int cmd_length = p6;
	int answer = 100;
	for (int i = 1; i < cmd_length; ++i) {
		scanf("%d", &cmd);

		switch (cmd) {
		case EDIT:
			scanf("%d %d %d", &p1, &p2, &p3);
			Edit(p1, p2, p3);
			break;

		case EDITDONE:
			scanf("%d %d", &p1, &p2);
			result = EditDone(p1);
			if (result != p2)
				answer = 0;
			break;

		case ADD:
			scanf("%d %d %d", &p1, &p2, &p3);
			if (sample_test == 1) {
				scanf("%s", str);
			}
			else {
				mSeed = p3;
				for (int k = 0; k < p2; ++k)
					str[k] = 'a' + pseudo_rand() % 26;
			}
			Add(p1, p2, str);
			break;

		case DELETE:
			scanf("%d %d", &p1, &p2);
			Delete(p1, p2);
			break;

		case SUBSTITUTE:
			scanf("%d %d %d", &p1, &p2, &p3);
			if (sample_test == 1) {
				scanf("%s", str);
			}
			else {
				mSeed = p3;
				for (int k = 0; k < p2; ++k)
					str[k] = 'a' + pseudo_rand() % 26;
			}
			Substitute(p1, p2, str);
			break;

		default:
			printf("cmd error !!!\n");
			break;
		}
	}

	return answer;
}

int main()
{
	setbuf(stdout, NULL);
	freopen("sample_input.txt", "r", stdin);

	int T;
	scanf("%d", &T);

	for (int tc = 1; tc <= T; tc++) {
		printf("#%d %d\n", tc, run());
	}

	return 0;
}

#include<vector>
using namespace std;

int Width;
vector<int> lineLength;

const int WAIT = 0;
const int EDITING = 1;
const int CONFLICT = 2;

struct User {
	int state, row, colStart, colEnd, editLength;	
} user[10010];

vector<int> online[1010];

void Init(int width, int length, char str[])
{
	Width = width;
	lineLength.clear();
	lineLength.push_back(0);
	for (int i = 0; i < length; ++i) {
		if (str[i] == '\n') {
			lineLength.push_back(0);
		} else {
			lineLength.back()++;
		}
	}

	// TODO: initialize online variables
	for (int i = 0; i < lineLength.size(); i++) {
		online[i].clear();
	}
}

void Edit(int id, int row, int col)
{
	int convRow = 0; // 문장의 번호
	int convCol = 0;

	for (int i = 0; i < row; ++i) {
		if (lineLength[convRow] - convCol > Width) {
			convCol += Width; // 문장 길이가 충분히 남아있음 -> 오른쪽으로 이동
		} else {
			convRow++;
			convCol = 0;
		}
	}
	convCol += col;

	User *u = &user[id];
	u->state = EDITING;
	u->row = convRow;
	u->colStart = convCol;
	u->colEnd = convCol + 1;
	u->editLength = 0;

	online[convRow].push_back(id);
}

void Add(int id, int length, char str[])
{
	User *u = &user[id];
	u->editLength = length;
}

void Delete(int id, int length)
{
	User *u = &user[id];
	u->editLength = -length;
	u->colEnd = u->colStart + length;
}

void Substitute(int id, int length, char str[])
{
	User *u = &user[id];
	u->colEnd = u->colStart + length;
}

int EditDone(int id)
{
	// check user state
	User *u = &user[id];
	if (u->state == CONFLICT) {
		u->state = WAIT;
		return 0;
	}

	// find conflicting users
	vector<int> update;
	for (vector<int>::iterator it = online[u->row].begin(); it != online[u->row].end(); it++) {
		int other = *it;
		if (other == id) continue;
		User *otherUser = &user[other];
		if (otherUser->colEnd <= u->colStart) {
			update.push_back(other);
		} else if (u->colEnd <= otherUser->colStart) {
			update.push_back(other);
			otherUser->colStart += u->editLength;
			otherUser->colEnd += u->editLength;
		} else {
			otherUser->state = CONFLICT;
		}
	}
	online[u->row] = update;

	// update edit result
	lineLength[u->row] += u->editLength;
	u->state = WAIT;
	return 1;
}
