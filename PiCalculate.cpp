// PiCalculate.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cmath>
#include <iomanip>
#include <limits>
#include <chrono>
#include <thread>
#include <mutex>
#include <math.h>
#include <corecrt_math_defines.h>
#include <unordered_map>

#define PI acos(-1)
#define Billion 1000000000

using namespace std;

unordered_map<string, pair<long double, double>> res;
mutex map_mutex;
typedef numeric_limits<long double> ld;

static long double LebnicPi() {
    long double pi = 0;

    for (uint64_t n = 1; n <= Billion; n++) {
        double p = (n % 2 == 0) ? -1.0 : 1.0; // pow(-1, n+1)
        pi += p/(2*n - 1);
    }
    return pi * 4;
}

static long double Wallis() {
    long double pi = 1.0;

    for (uint64_t n = 1; n <= Billion; n++) {
        pi *= (long double)(4 * n * n) / ((4 * n * n) - 1);
    }
    return 2 * pi;
}

static long double Madhava() {
    long double pi = 1.0;

    uint64_t pow3 = 1; // pow(3,i)

    for (uint64_t i = 1; i <= Billion; i++) {
        pow3 *= 3;
        int p = (i % 2 == 0) ? 1 : -1; // pow(-1, i)
        pi += (p / ((2.0 * i + 1) * pow3));
    }
    return sqrt(12)*pi;
}

static long double Nilkanth() {
    long double pi = 3.0;
    int sign = 1;

    for (uint64_t i = 2; i <= Billion; i+=2) {

        pi += (4 * sign) / (i * (i + 1.0) * (i + 2.0));
        sign = -sign;
    }
    return pi;
}


static uint64_t rrand(uint64_t range_max)
{
    return rand() % (range_max + 1);
}
static long double MonteCarlo() {

        srand((unsigned)time(0));

        uint64_t n = Billion; 
        uint64_t  side = RAND_MAX;
        uint64_t qside = side * side;
        uint64_t count = 0;

        for (uint64_t i = 0; i < n; ++i)
        {
            uint64_t x = rrand(side),
                y = rrand(side);

            // Check if it is in the circle
            if (x * x + y * y < qside) ++count;
        }

        return ((4. * count) / n);
    }

static long double Euler() {
    long double pi = 0;
    
    for (uint64_t i = 1; i <= Billion; i++) {
        pi += (1.0 / (i * i));
    }
    return sqrt(6 * pi);
}

int main()
{
    cout << "Default PI:              " << setprecision(ld::digits) << M_PI << endl;
    cout << "With arccos(-1):         " << setprecision(ld::digits) << PI << endl;
    cout << endl;

    auto start = chrono::steady_clock::now();

    thread eulerTh([&]() {
        long double r = Euler();
        auto end = chrono::steady_clock::now();
        lock_guard<mutex> guard(map_mutex);
        res["Euler"] = make_pair(r, chrono::duration_cast<chrono::milliseconds>(end - start).count() / 1000.0);
        cout << "Euler done! ";
        });
    thread leibnizTh([&]() {
        long double r = LebnicPi();
        auto end = chrono::steady_clock::now();
        lock_guard<mutex> guard(map_mutex);
        res["Lebniz"] = make_pair(r, chrono::duration_cast<chrono::milliseconds>(end - start).count() / 1000.0);
        cout << "Lebniz done! ";
        });
    thread wallisTh([&]() {
        long double r = Wallis();
        auto end = chrono::steady_clock::now();
        lock_guard<mutex> guard(map_mutex);
        res["Wallis"] = make_pair(r, chrono::duration_cast<chrono::milliseconds>(end - start).count() / 1000.0);
        cout << "Wallis done! ";
        });
    thread madhavaTh([&]() {
        long double r = Madhava();
        auto end = chrono::steady_clock::now();
        lock_guard<mutex> guard(map_mutex);
        res["Madhava"] = make_pair(r, chrono::duration_cast<chrono::milliseconds>(end - start).count() / 1000.0);
        cout << "Madhava done! ";
        });
    thread nilakanthTh([&]() {
        long double r = Nilkanth();
        auto end = chrono::steady_clock::now();
        lock_guard<mutex> guard(map_mutex);
        res["Nilkanth"] = make_pair(r, chrono::duration_cast<chrono::milliseconds>(end - start).count() / 1000.0);
        cout << "Nilakanth done! ";
        });
    thread monteTh([&]() {
        long double r = MonteCarlo();
        auto end = chrono::steady_clock::now();
        lock_guard<mutex> guard(map_mutex);
        res["MonteCarlo"] = make_pair(r, chrono::duration_cast<chrono::milliseconds>(end - start).count() / 1000.0);
        cout << "Monte done! ";
        });

    eulerTh.join();
    leibnizTh.join();
    wallisTh.join();
    madhavaTh.join();
    nilakanthTh.join();
    monteTh.join();

    cout << endl << endl;

    for (const auto& val : res) {
        cout << setprecision(ld::digits) << "With " << val.first << " method:     " << val.second.first << endl;
        cout << val.first << " time: " << setprecision(4) << val.second.second << " s" << endl << endl;
    }
}




