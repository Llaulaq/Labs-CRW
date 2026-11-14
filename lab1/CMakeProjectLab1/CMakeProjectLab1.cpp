#include <iostream>
#include <string>
#include <ctime>
#include <thread>
#include <mutex>
#include <chrono>
#include <iomanip>

std::mutex print_mutex;

unsigned long long factorial(int n) {
    unsigned long long result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= static_cast<unsigned long long>(i);
    }
    return result;
}

void thread_func(const std::string& name) {
    volatile int n = 10;
    unsigned long long local_sink = 0;
    const int iterations = 10'000'000;

    for (int i = 0; i < iterations; ++i) {
        local_sink += factorial(n);
    }

    std::lock_guard<std::mutex> lock(print_mutex);
    std::cout << "thread " << name << " done, sink = " << local_sink << "\n";
}

void task1_2() {
    std::cout << "\n1.2\n";
    const int iterations = 10'000'000;
    volatile int n = 10;
    volatile unsigned long long sink = 0;

    clock_t start = clock();
    for (int i = 0; i < iterations; ++i) {
        sink = sink + factorial(n);
    }
    clock_t end = clock();
    double seconds = static_cast<double>(end - start) / CLOCKS_PER_SEC;

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "10! = " << factorial(10) << "\n";
    std::cout << "sink = " << sink << "\n";
    std::cout << "time = " << seconds << " s\n";
}

void task1_3() {
    std::cout << "\n1.3\n";

    std::cout << "sequential...\n";
    auto s1 = std::chrono::steady_clock::now();
    thread_func("t1");
    thread_func("t2");
    auto e1 = std::chrono::steady_clock::now();
    double t_seq = std::chrono::duration<double>(e1 - s1).count();

    std::cout << "parallel...\n";
    auto s2 = std::chrono::steady_clock::now();
    std::thread thread1(thread_func, "t1");
    std::thread thread2(thread_func, "t2");
    thread1.join();
    thread2.join();
    auto e2 = std::chrono::steady_clock::now();
    double t_par = std::chrono::duration<double>(e2 - s2).count();

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "sequential time = " << t_seq << " s\n";
    std::cout << "parallel time   = " << t_par << " s\n";
    std::cout << "speedup         = " << (t_seq / t_par) << "x\n";
}

void task1_1() {
    std::cout << "\n1.1\n";
    std::string s = "01234";

    for (unsigned int i = s.size() - 1; i >= 0; i--) {
        std::cout << s[i] << std::endl;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    task1_2();
    task1_3();
    task1_1();

    return 0;
}