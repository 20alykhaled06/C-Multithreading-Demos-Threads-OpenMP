#include <iostream>
#include <chrono>
#include <cstdlib>
#include <vector>

#include "TSQueue.h"
#include "integerServer.h"
#include "Client.h"

constexpr std::size_t size = 20;

template <std::size_t s>
auto createClientAndWork(TSQueue<int, s>& q) {
    Client<int, s> client(q, time(0), milliseconds(125), milliseconds(10000));
    client.work();

    return;
}

template <std::size_t s>
auto createServerAndServe(TSQueue<int, s>& q) {
    integerServer<s> server(q, milliseconds(500), milliseconds(10000), 4);
    server.serve();
}

int main()
{
    TSQueue<int, size> q;

    std::thread clientManagerThread(createClientAndWork<size>, std::ref(q));
    std::this_thread::sleep_for(milliseconds(750));
    std::thread serverManagerThread(createServerAndServe<size>, std::ref(q));

    clientManagerThread.join();
    serverManagerThread.join();
}