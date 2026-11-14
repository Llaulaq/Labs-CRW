#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex m;

void Func(std::string name) {
    long double i = 0;
    auto start = std::chrono::steady_clock::now();

    // Крутимся в цикле, пока не пройдет 1 секунда
    while (true) {
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed = now - start;

        if (elapsed.count() >= 1.0) {
            break;
        }
        i += 1e-9;
    }

    m.lock();
    std::cout << name << ": " << i << std::endl;
    m.unlock();
}

int main() {
    std::thread thread1(Func, "t1");
    std::thread thread2(Func, "t2");
    std::thread thread3(Func, "t3");

    thread1.join();
    thread2.join();
    thread3.join();

    system("pause");
    return 0;
}
