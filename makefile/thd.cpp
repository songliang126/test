#include "thd.h"

#include <thread>
#include <iostream>
#include <vector>
#include <algorithm>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <string>

using namespace std;

void testthd()
{
    vector<thread> workers;
    for (int i = 0; i < 5; ++i) {
        workers.push_back(thread([=]() {
            cout << "thread[" << i << "] function" << endl;
        }));
    }

    for_each(workers.begin(), workers.end(), [](thread &t) {
        t.join();
    });

    mutex mtx;
    condition_variable cond;
    queue<string> q;

    thread t([&] {
        unique_lock<mutex> lock(mtx);
        cond.wait(lock, [&] {
            return !q.empty();
        });
        cout << "queue data : " << q.front() << endl;
        lock.unlock();
    });

    q.push("this is my data!");
    cond.notify_one();

    if (t.joinable()) {
        t.join();
    }
}
