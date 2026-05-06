#include <bits/stdc++.h>
using namespace std;

void demo_deque() {
    deque<int> dq;
    dq.push_back(10);  dq.push_front(20);
    dq.pop_back();     dq.pop_front();
    dq.front(); dq.back(); dq[1];
    dq.clear();
}

// Sliding Window Maximum — O(N)
void sliding_window_max(const vector<int>& arr, int k) {
    deque<int> dq; // guarda índices
    for (int i = 0; i < (int)arr.size(); i++) {
        if (!dq.empty() && dq.front() == i - k) dq.pop_front();
        while (!dq.empty() && arr[dq.back()] <= arr[i]) dq.pop_back();
        dq.push_back(i);
        if (i >= k - 1) cout << arr[dq.front()] << " ";
    }
}