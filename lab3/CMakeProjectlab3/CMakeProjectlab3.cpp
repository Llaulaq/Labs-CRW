#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <vector>

int coins = 101;
int Bob_coins = 0;
int Tom_coins = 0;
int DeadMan_coins = 0;
std::mutex m;

void coin_sharing(std::string name, int& thief_coins, int& companion_coins) {
    while (true) {
        m.lock();

        // Если монеты кончились, выходим
        if (coins == 0) {
            m.unlock();
            break;
        }

        // Если осталась последняя монета и у воров поровну, она достается покойнику
        if (coins == 1 && thief_coins == companion_coins) {
            DeadMan_coins++;
            coins--;
            m.unlock();
            break;
        }

        // Вор берет монету, если у него меньше или столько же, сколько у напарника
        if (thief_coins <= companion_coins) {
            thief_coins++;
            coins--;
            std::cout << name << " :" << thief_coins << " " << companion_coins << std::endl;
        }

        m.unlock();
        std::this_thread::yield(); // Даем другому потоку шанс захватить мьютекс
    }
}

void run_simulation(int initial_coins) {
    coins = initial_coins;
    Bob_coins = 0;
    Tom_coins = 0;
    DeadMan_coins = 0;

    std::thread thread1(coin_sharing, "Bob", std::ref(Bob_coins), std::ref(Tom_coins));
    std::thread thread2(coin_sharing, "Tom", std::ref(Tom_coins), std::ref(Bob_coins));

    thread1.join();
    thread2.join();

    std::cout << initial_coins << ": Bob " << Bob_coins << " Tom " << Tom_coins << " Dead " << DeadMan_coins << "\n";
}

int main() {
    
     run_simulation(101);
  

    system("pause");
    return 0;
}