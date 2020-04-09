#include <algorithm>
#include <iostream>
#include <string>
#include <omp.h>
#include <stdlib.h>

namespace ScheduleType {
    enum {
        STATIC,
        STATIC_WITH_CHUNK,
        DYNAMIC,
        DYNAMIC_WITH_CHUNK,
        GUIDED,
        GUIDED_WITH_CHUNK,
        RUNTIME,
        AUTO,
    };
}

class SharedArray {
private:
    char *array;
    int index;
    int size;
    bool mutex;
public:
    SharedArray(int n, bool use) : size(n), index(0), mutex(use) {
        array = new char[size];
        std::fill(array, array + size, '-');
    }

    ~SharedArray() {
        delete[] array;
    }

    void addChar(char c) {
        if (mutex) {
#pragma omp critical
            {
                array[index] = c;
                spendSomeTime();
                index++;
            }
        } else {
            array[index] = c;
            spendSomeTime();
            index++;
        }
    }

    int countOccurrences(char c) {
        return std::count(array, array + size, c);
    }

    std::string toString() {
        return std::string(array, size);
    }

private:
    void spendSomeTime() {
        for (int i = 0; i < 10000; i++) {
            for (int j = 0; j < 100; j++) {
                // These loops shouldn't be removed by the compiler
            }
        }
    }
};


class ArrayFiller {
private:
    static const int nThreads = 3;
    static const int nTimes = 20 * nThreads;
    static const int chunk = 5;
    SharedArray *array;
public:
    ArrayFiller(bool mutex) {
        array = new SharedArray(nTimes, mutex);
    }

    void fillArrayByScheduleType(int scheduleType) {
        switch (scheduleType) {
            case ScheduleType::STATIC:
#pragma omp parallel for schedule(static) num_threads(nThreads)
                for (int i = 0; i < nTimes; i++) {
                    ArrayFiller::run('A' + omp_get_thread_num());
                }
                break;

            case ScheduleType::STATIC_WITH_CHUNK:
#pragma omp parallel for schedule(static, chunk) num_threads(nThreads)
                for (int i = 0; i < nTimes; i++) {
                    ArrayFiller::run('A' + omp_get_thread_num());
                }
                break;

            case ScheduleType::DYNAMIC:
#pragma omp parallel for schedule(dynamic) num_threads(nThreads)
                for (int i = 0; i < nTimes; i++) {
                    ArrayFiller::run('A' + omp_get_thread_num());
                }
                break;

            case ScheduleType::DYNAMIC_WITH_CHUNK:
#pragma omp parallel for schedule(dynamic, chunk) num_threads(nThreads)
                for (int i = 0; i < nTimes; i++) {
                    ArrayFiller::run('A' + omp_get_thread_num());
                }
                break;

            case ScheduleType::GUIDED:
#pragma omp parallel for schedule(guided) num_threads(nThreads)
                for (int i = 0; i < nTimes; i++) {
                    ArrayFiller::run('A' + omp_get_thread_num());
                }
                break;

            case ScheduleType::GUIDED_WITH_CHUNK:
#pragma omp parallel for schedule(guided, chunk) num_threads(nThreads)
                for (int i = 0; i < nTimes; i++) {
                    ArrayFiller::run('A' + omp_get_thread_num());
                }
                break;

            case ScheduleType::RUNTIME:
#pragma omp parallel for schedule(runtime) num_threads(nThreads)
                for (int i = 0; i < nTimes; i++) {
                    ArrayFiller::run('A' + omp_get_thread_num());
                }
                break;

            case ScheduleType::AUTO:
#pragma omp parallel for schedule(auto) num_threads(nThreads)
                for (int i = 0; i < nTimes; i++) {
                    ArrayFiller::run('A' + omp_get_thread_num());
                }
                break;
        }
    }

    void printStats() {
        std::cout << array->toString() << std::endl;
        for (int i = 0; i < nThreads; ++i)
            std::cout << (char) ('A' + i) << "="
                      << array->countOccurrences('A' + i) << " ";
        std::cout << std::endl;
    }

    ~ArrayFiller() {
        delete array;
    }

private:
    void run(char c) {
        array->addChar(c);
    }
};


int main() {
    std::cout << "Each thread should add its char to the array n times (n=20)" << std::endl;
    std::cout << "Correct results should total exactly nThreads*nTimes chars" << std::endl;
    std::cout << "Case 1: Schedule static" << std::endl;
    ArrayFiller m1(true);
    m1.fillArrayByScheduleType(ScheduleType::STATIC);
    m1.printStats();

    std::cout << "Case 2: Schedule static, with chunk specification = 5 = 5" << std::endl;
    ArrayFiller m2(true);
    m2.fillArrayByScheduleType(ScheduleType::STATIC_WITH_CHUNK);
    m2.printStats();

    std::cout << "Case 3: Schedule dynamic" << std::endl;
    ArrayFiller m3(true);
    m3.fillArrayByScheduleType(ScheduleType::DYNAMIC);
    m3.printStats();

    std::cout << "Case 4: Schedule dynamic, with chunk specification = 5" << std::endl;
    ArrayFiller m4(true);
    m4.fillArrayByScheduleType(ScheduleType::DYNAMIC_WITH_CHUNK);
    m4.printStats();

    std::cout << "Case 5: Schedule guided" << std::endl;
    ArrayFiller m5(true);
    m5.fillArrayByScheduleType(ScheduleType::GUIDED);
    m5.printStats();

    std::cout << "Case 6: Schedule guided, with chunk specification = 5" << std::endl;
    ArrayFiller m6(true);
    m6.fillArrayByScheduleType(ScheduleType::GUIDED_WITH_CHUNK);
    m6.printStats();

    std::cout << "Case 7: Schedule runtime" << std::endl;
    ArrayFiller m7(true);
    m7.fillArrayByScheduleType(ScheduleType::RUNTIME);
    m7.printStats();

    std::cout << "Case 8: Schedule auto" << std::endl;
    ArrayFiller m8(true);
    m8.fillArrayByScheduleType(ScheduleType::AUTO);
    m8.printStats();

    std::cout << "Case 9: Schedule runtime without mutex" << std::endl;
    ArrayFiller m9(false);
    m9.fillArrayByScheduleType(ScheduleType::RUNTIME);
    m9.printStats();

    std::cout << "Case 10: Schedule dynamic without mutex" << std::endl;
    ArrayFiller m10(false);
    m10.fillArrayByScheduleType(ScheduleType::DYNAMIC);
    m10.printStats();

    std::cout << "Case 11: Schedule dynamic, with chunk specification = 5 and without mutex" << std::endl;
    ArrayFiller m11(false);
    m11.fillArrayByScheduleType(ScheduleType::DYNAMIC_WITH_CHUNK);
    m11.printStats();

}
