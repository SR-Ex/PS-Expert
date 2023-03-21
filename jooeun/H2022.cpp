#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#define CMD_INIT				100
#define CMD_SEND_MESSAGE		200
#define CMD_RETRIEVE_SENTBOX	300
#define CMD_RETRIEVE_INBOX		400
#define CMD_READ_MESSAGE		500
#define CMD_DELETE_MESSAGE		600

#define MAXM					3

extern void init(int N);
extern void sendMessage(int cTimestamp, int uID1, int uID2, int mID, int scheduleTimestamp);
extern int  retrieveSentbox(int cTimestamp, int uID, int mIDList[MAXM], int uIDList[MAXM], int readList[MAXM]);
extern int  retrieveInbox(int cTimestamp, int uID, int mIDList[MAXM], int uIDList[MAXM], int readList[MAXM]);
extern int  readMessage(int cTimestamp, int uID, int mID);
extern int  deleteMessage(int cTimestamp, int uID, int mID);

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

static bool run()
{
    int Q, N;
	int cTimestamp, scheduleTimestamp;
	int uID, uID1, uID2, mID;
	int ret, ans;
	int mIDList[MAXM], uIDList[MAXM], readList[MAXM];
	int mIDList_a[MAXM], uIDList_a[MAXM], readList_a[MAXM];

	bool okay = false;

    scanf("%d", &Q);

	for (int q = 0; q <= Q; ++q)
	{
		int op;
		scanf("%d", &op);

		switch(op)
		{
		case CMD_INIT:
			scanf("%d", &N);
			init(N);
			okay = true;
			break;
		case CMD_SEND_MESSAGE:
			scanf("%d %d %d %d %d", &cTimestamp, &uID1, &uID2, &mID, &scheduleTimestamp);
			if (okay)
				sendMessage(cTimestamp, uID1, uID2, mID, scheduleTimestamp);
			break;
		case CMD_RETRIEVE_SENTBOX:
			scanf("%d %d", &cTimestamp, &uID);
			if (okay)
				ret = retrieveSentbox(cTimestamp, uID, mIDList, uIDList, readList);
			scanf("%d", &ans);
			for (int i = 0; i < ans; ++i)
				scanf("%d %d %d", &mIDList_a[i], &uIDList_a[i], &readList_a[i]);
			if (ret != ans)
			{
				okay = false;
			}
			else
			{
				for (int i = 0; i < ans; ++i)
					if (mIDList[i] != mIDList_a[i]
							|| uIDList[i] != uIDList_a[i] || readList[i] != readList_a[i])
						okay = false;
			}
			break;
		case CMD_RETRIEVE_INBOX:
			scanf("%d %d", &cTimestamp, &uID);
			if (okay)
				ret = retrieveInbox(cTimestamp, uID, mIDList, uIDList, readList);
			scanf("%d", &ans);
			for (int i = 0; i < ans; ++i)
				scanf("%d %d %d", &mIDList_a[i], &uIDList_a[i], &readList_a[i]);
			if (ret != ans)
			{
				okay = false;
			}
			else
			{
				for (int i = 0; i < ans; ++i)
					if (mIDList[i] != mIDList_a[i]
							|| uIDList[i] != uIDList_a[i] || readList[i] != readList_a[i])
						okay = false;
			}
			break;
		case CMD_READ_MESSAGE:
			scanf("%d %d %d", &cTimestamp, &uID, &mID);
			if (okay)
				ret = readMessage(cTimestamp, uID, mID);
			scanf("%d", &ans);
			if (ret != ans)
				okay = false;
			break;
		case CMD_DELETE_MESSAGE:
			scanf("%d %d %d", &cTimestamp, &uID, &mID);
			if (okay)
				ret = deleteMessage(cTimestamp, uID, mID);
			scanf("%d", &ans);
			if (ret != ans)
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

	int T, MARK;
	scanf("%d %d", &T, &MARK);

	for (int tc = 1; tc <= T; tc++)
	{
		int score = run() ? MARK : 0;
		printf("#%d %d\n", tc, score);
	}
	return 0;
}


#define MAXM	3

#include<unordered_map>
#include<vector>
#include<queue>

using namespace std;

struct message {
    int sender, reciever, sTS, rTS;
    bool sdel, rdel, arrive, read;
};

struct mailbox {
    int ts, mid;
};

struct cmp {
    bool operator() (mailbox a, mailbox b){
        if (a.ts == b.ts) return a.mid > b.mid;
        return a.ts > b.ts;
    }
};

unordered_map<int, message> minfo;
unordered_map<int, vector<int> > sentbox;
unordered_map<int, vector<int> > inbox;
priority_queue<mailbox, vector<mailbox>, cmp> pq;

void update(int ts) {
    while (!pq.empty()) {
        mailbox b = pq.top();
        if (b.ts > ts) break;
        pq.pop();
        message m = minfo[b.mid];
        if (!m.sdel) {
            inbox[m.reciever].push_back(b.mid);
            minfo[b.mid].arrive = true;
        }
    }
}

void init(int N)
{
    minfo.clear();
    sentbox.clear();
    inbox.clear();
    pq = priority_queue<mailbox, vector<mailbox>, cmp> ();
}

void sendMessage(int cTimestamp, int uID1, int uID2, int mID, int scheduleTimestamp)
{
    message m = {uID1, uID2, cTimestamp, scheduleTimestamp, false, false, false, false};
    minfo[mID] = m;
    sentbox[uID1].push_back(mID);
    mailbox b = {scheduleTimestamp, mID};
    pq.push(b);
}

int retrieveSentbox(int cTimestamp, int uID, int mIDList[MAXM], int uIDList[MAXM], int readList[MAXM])
{
    update(cTimestamp);
    int idx = 0;
    for (int i=sentbox[uID].size()-1; i >= 0; i--) {
        if (idx == MAXM) break;
        int mid = sentbox[uID][i];
        message m = minfo[mid];
        if (m.sdel) continue;
        mIDList[idx] = mid;
        uIDList[idx] = m.reciever;
        readList[idx] = m.read;
        idx++;
    }
	return idx;
}

int retrieveInbox(int cTimestamp, int uID, int mIDList[MAXM], int uIDList[MAXM], int readList[MAXM])
{
    update(cTimestamp);
    int idx = 0;
    for (int i=inbox[uID].size()-1; i >= 0; i--) {
        if (idx == MAXM) break;
        int mid = inbox[uID][i];
        message m = minfo[mid];
        if (m.rdel) continue;
        mIDList[idx] = mid;
        uIDList[idx] = m.sender;
        readList[idx] = m.read;
        idx++;
    }
	return idx;
}

int readMessage(int cTimestamp, int uID, int mID)
{
    update(cTimestamp);
    message m = minfo[mID];
    if (m.sender == uID) {
        if (m.sdel) return 0;
        return 1;
    } else if (m.reciever == uID) {
        if (m.rdel) return 0;
        if (m.rTS > cTimestamp) return 0;
        if (m.sdel && !m.arrive) return 0;
        minfo[mID].read = true;
        return 1;
    }
	return 0;
}

int deleteMessage(int cTimestamp, int uID, int mID)
{
    update(cTimestamp);
    message m = minfo[mID];
    if (m.sender == uID) {
        if (m.sdel) return 0;
        minfo[mID].sdel = true;
        return 1;
    } else if (m.reciever == uID) {
        if (m.rdel) return 0;
        if (m.rTS > cTimestamp) return 0;
        if (m.sdel && !m.arrive) return 0;
        minfo[mID].rdel = true;
        return 1;
    }
	return 0;
}
