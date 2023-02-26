#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

extern void init(int N);
extern int arrive(int mId);
extern int leave(int mId);

/////////////////////////////////////////////////////////////////////////

#define CMD_INIT 1
#define CMD_ARRIVE 2
#define CMD_LEAVE 3

static bool run() {
	int q;
	scanf("%d", &q);

	int n, mid;
	int cmd, ans, ret = 0;
	bool okay = false;

	for (int i = 0; i < q; ++i) {
		scanf("%d", &cmd);
		switch (cmd) {
			case CMD_INIT:
				scanf("%d", &n);
				init(n);
				okay = true;
				break;
			case CMD_ARRIVE:
				scanf("%d %d", &mid, &ans);
				ret = arrive(mid);
				if (ans != ret)
					okay = false;
				break;
			case CMD_LEAVE:
				scanf("%d %d", &mid, &ans);
				ret = leave(mid);
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

#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <vector>
#define MAX_NODE 20001

struct Range;

struct Node {
	int data;
	Range* left;
	Range* right;
};

struct Range {
    int length;
    Node* start;
	Node* end;
    Range(int num, Node* s, Node* e) : length(num), start(s), end(e) {}
};

struct cmp{
    bool operator()(Range* t, Range* u){
		if (t->length == u->length) {
			return t->start->data > u->start->data;
		}
        return t->length < u->length;
    }
};

std::map<int, Node*> mId2loc;
std::set<Node*> nodes;
std::priority_queue<Range*, std::vector<Range*>, cmp> pq;
int count;
int n;

Node node[MAX_NODE];
int nodeCnt;
Node* head;
Node* tail;

Node* getNode(int data) {
	node[nodeCnt].data = data;
	node[nodeCnt].left = nullptr;
	node[nodeCnt].right = nullptr;
	return &node[nodeCnt++];
}

void init(int N) {
	nodeCnt = 0;
    n = N;
    count = N;
    mId2loc.clear();
	head = getNode(0);
	tail = getNode(n+1);
	Range* range = new Range(n+1, head, tail);
	head->right = range;
	tail->left = range;
	pq = std::priority_queue<Range*, std::vector<Range*>, cmp>();
    pq.push(range);
	return;
}

int arrive(int mId) {
	int locker;
	Range* range;
	for (;;) {
		range = pq.top();
		pq.pop();
		if (((range->start->data == 0) || (nodes.count(range->start) == 1)) && ((range->end->data == n+1) || (nodes.count(range->end) == 1))) {
			break;
		}
	}
	Node* start = range->start;
	Node* end = range->end;
	locker = start->data + range->length / 2;
	if (start->data == 0) {
		locker = 1;
	} else if (end->data == n+1) {
		locker = n;
	}
	Node* node = getNode(locker);
	if (locker - start->data >= 1) {
		Range* left = new Range(locker - start->data, start, node);
		pq.push(left);
		node->left = left;
		start->right = left;
	}
	if (end->data - locker >= 1) {
		Range* right = new Range(end->data - locker, node, end);
		pq.push(right);
		node->right = right;
		end->left = right;
	}
    count--;
	mId2loc.insert(std::make_pair(mId, node));
	nodes.insert(node);
	return locker;
}

int leave(int mId) {
	Node* locker = mId2loc[mId];
	Range* left = locker->left;
	Range* right = locker->right;
	Range* newRange = new Range(right->length + left->length, left->start, right->end);
	left->start->right = newRange;
	right->end->left = newRange;
	pq.push(newRange);
    mId2loc.erase(mId);
	nodes.erase(locker);
	return ++count;
}
