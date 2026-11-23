#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <chrono>
#include <atomic>
#include <windows.h> // SetConsoleOutputCP, чтобы русский вывод не превращался в кашу

// Самодельный мьютекс на атомарной переменной и compare_exchange
class my_mutex {
    std::atomic<bool> locked{ false }; // false - мьютекс свободен
public:
    void lock() {
        bool expected = false;
        // Атомарно: если мьютекс свободен (locked == false), занимаем его
        while (!locked.compare_exchange_weak(expected, true)) {
            expected = false; // после неудачи в expected лежит текущее значение, сбрасываем
            std::this_thread::yield();
        }
    }
    void unlock() {
        locked.store(false);
    }
};

int dish1 = 3000;
int dish2 = 3000;
int dish3 = 3000;
int eaten1 = 0;
int eaten2 = 0;
int eaten3 = 0;
int burst_count = 0;
std::string result;
std::atomic<bool> running{ true };
my_mutex m; // вместо std::mutex из ЛР4

void fat_man(int& dish, int& eaten, int gluttony, int eat_pause_ms) {
    while (running) {
        m.lock();

        // Наггетсы закончились - повара придется уволить
        if (dish <= 0) {
            result = "Кука уволили";
            running = false;
            m.unlock();
            break;
        }

        // Толстяк съел больше 10 000 - он лопнул
        if (eaten > 10000) {
            burst_count++;
            if (burst_count == 3) {
                result = "Кук не получил зарплату.";
                running = false;
            }
            m.unlock();
            break;
        }

        // Жрем только полной порцией, крошки не подбираем
        if (dish >= gluttony) {
            dish -= gluttony;
            eaten += gluttony;
        }

        m.unlock();

        // eat_pause_ms моделирует скорость еды: 0 - жрет без передышки
        if (eat_pause_ms > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(eat_pause_ms));
        }
        else {
            std::this_thread::yield(); // Даем остальным шанс подойти к столу
        }
    }
}

void cook(int efficiency_factor, std::chrono::steady_clock::time_point start) {
    int per_dish = efficiency_factor / 3; // раскладываем по 3 тарелкам сразу

    while (running) {
        // 5 дней по 1 секунде прошло - силы кончились
        if (std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count() >= 5.0) {
            m.lock();
            if (running) {
                result = "Кук уволился сам!!!";
                running = false;
            }
            m.unlock();
            break;
        }

        m.lock();
        dish1 += per_dish;
        dish2 += per_dish;
        dish3 += per_dish;
        m.unlock();

        std::this_thread::yield();
    }
}

void run_simulation(int gluttony, int efficiency_factor, int eat_pause_ms) {
    dish1 = 3000;
    dish2 = 3000;
    dish3 = 3000;
    eaten1 = 0;
    eaten2 = 0;
    eaten3 = 0;
    burst_count = 0;
    running = true;

    std::cout << "\ngluttony=" << gluttony << ", efficiency_factor=" << efficiency_factor << "\n";

    auto start = std::chrono::steady_clock::now();

    std::thread t1(fat_man, std::ref(dish1), std::ref(eaten1), gluttony, eat_pause_ms);
    std::thread t2(fat_man, std::ref(dish2), std::ref(eaten2), gluttony, eat_pause_ms);
    std::thread t3(fat_man, std::ref(dish3), std::ref(eaten3), gluttony, eat_pause_ms);
    std::thread tc(cook, efficiency_factor, start);

    t1.join();
    t2.join();
    t3.join();
    tc.join();

    std::cout << "Итог: " << result << "\n";
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, ".UTF-8");

    std::atomic<bool> check;
    std::cout << "atomic<bool> is_lock_free: " << (check.is_lock_free() ? "true" : "false") << "\n";

    run_simulation(3, 3, 0);   // жрут быстрее, чем Кук успевает готовить
    run_simulation(100, 300, 1); // Кук кормит вовремя, но толстяки все равно лопаются
    run_simulation(1, 3, 2);   // Кук справляется, толстяки едят медленно - 5 дней выдержал

    system("pause");
    return 0;
}
