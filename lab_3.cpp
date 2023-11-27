#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

struct Stats {
    size_t comparison_count = 0;
    size_t copy_count = 0;
};

template <typename T>
void custom_swap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

template <typename T>
Stats bubbleSort(std::vector<T>& arr) {
    Stats s;
    size_t n = arr.size();
    for (size_t i = 0; i < n - 1; ++i) {
        for (size_t j = 0; j < n - i - 1; ++j) {
            s.comparison_count++;
            if (arr[j] > arr[j + 1]) {
                custom_swap(arr[j], arr[j + 1]);
                s.copy_count += 3; // 2 swaps + 1 temporary variable
            }
        }
    }
    return s;
}

template <typename T>
Stats shakerSort(std::vector<T>& arr) {
    Stats s;
    size_t n = arr.size();
    bool swapped;
    do {
        swapped = false;
        for (size_t i = 0; i < n - 1; ++i) {
            s.comparison_count++;
            if (arr[i] > arr[i + 1]) {
                custom_swap(arr[i], arr[i + 1]);
                s.copy_count += 3;
                swapped = true;
            }
        }
        if (!swapped) break;

        swapped = false;
        for (size_t i = n - 1; i > 0; --i) {
            s.comparison_count++;
            if (arr[i - 1] > arr[i]) {
                custom_swap(arr[i - 1], arr[i]);
                s.copy_count += 3;
                swapped = true;
            }
        }
    } while (swapped);

    return s;
}

template <typename T>
Stats combSort(std::vector<T>& arr) {
    Stats s;
    size_t n = arr.size();
    size_t gap = n;
    bool swapped = true;

    while (gap > 1 || swapped) {
        gap = (gap * 10) / 13;
        if (gap < 1) gap = 1;

        swapped = false;
        for (size_t i = 0; i < n - gap; ++i) {
            s.comparison_count++;
            if (arr[i] > arr[i + gap]) {
                custom_swap(arr[i], arr[i + gap]);
                s.copy_count += 3;
                swapped = true;
            }
        }
    }

    return s;
}

// случайного массива
template <typename T>
std::vector<T> generateRandomArray(size_t size) {
    std::vector<T> arr(size);
    srand(static_cast<unsigned int>(time(0))); // Khởi tạo seed ngẫu nhiên
    for (size_t i = 0; i < size; ++i) {
        arr[i] = rand();
    }
    return arr;
}

//отсортированный массив
template <typename T>
std::vector<T> generateSortedArray(size_t size) {
    std::vector<T> arr(size);
    for (size_t i = 0; i < size; ++i) {
        arr[i] = i;
    }
    return arr;
}


//массив отсортирован в обратном порядке
template <typename T>
std::vector<T> generateReverseSortedArray(size_t size) {
    std::vector<T> arr(size);
    for (size_t i = 0; i < size; ++i) {
        arr[i] = size - i;
    }
    return arr;
}


//в среднем (сгенерировать 100 случайных массивов и посчитать средние значения);
template <typename T>
Stats calculateAverageStats(size_t arraySize, size_t numExperiments, std::vector<T>(*generateArray)(size_t), Stats(*sortFunction)(std::vector<T>&)) {
    Stats totalStats;

    for (size_t i = 0; i < numExperiments; ++i) {
        std::vector<T> arr = generateArray(arraySize);
        Stats result = sortFunction(arr);
        totalStats.comparison_count += result.comparison_count;
        totalStats.copy_count += result.copy_count;
    }

    totalStats.comparison_count /= numExperiments;
    totalStats.copy_count /= numExperiments;

    return totalStats;
}

int main() {
    const size_t numExperiments = 100;

    for (size_t arraySize = 1000; arraySize <= 100000; arraySize += 1000) {

        Stats averageStatsRandom = calculateAverageStats<int>(arraySize, numExperiments, generateRandomArray, bubbleSort);
        Stats averageStatsShaker = calculateAverageStats<int>(arraySize, numExperiments, generateRandomArray, shakerSort);
        Stats averageStatsComb = calculateAverageStats<int>(arraySize, numExperiments, generateRandomArray, combSort);
       
        Stats averageStatsSortedBubble = calculateAverageStats<int>(arraySize, numExperiments, generateSortedArray, bubbleSort);
        Stats averageStatsSortedShaker = calculateAverageStats<int>(arraySize, numExperiments, generateSortedArray, shakerSort);
        Stats averageStatsSortedComb = calculateAverageStats<int>(arraySize, numExperiments, generateSortedArray, combSort);
        
        Stats averageStatsReverse = calculateAverageStats<int>(arraySize, numExperiments, generateReverseSortedArray, bubbleSort);
        Stats averageStatsReverseShaker = calculateAverageStats<int>(arraySize, numExperiments, generateReverseSortedArray, shakerSort);
        Stats averageStatsReverseComb = calculateAverageStats<int>(arraySize, numExperiments, generateReverseSortedArray, combSort);

        std::cout << "Array Size: " << arraySize << "\n";
        std::cout << "Random (Bubble Sort): Comparisons: " << averageStatsRandom.comparison_count << ", Copies: " << averageStatsRandom.copy_count << "\n";
        std::cout << "Random (Shaker Sort): Comparisons: " << averageStatsShaker.comparison_count << ", Copies: " << averageStatsShaker.copy_count << "\n";
        std::cout << "Random (Comb Sort): Comparisons: " << averageStatsComb.comparison_count << ", Copies: " << averageStatsComb.copy_count << "\n\n";
        std::cout << "Sorted (Bubble Sort): Comparisons: " << averageStatsSortedBubble.comparison_count << ", Copies: " << averageStatsSortedBubble.copy_count << "\n";
        std::cout << "Sorted (Shaker Sort): Comparisons: " << averageStatsSortedShaker.comparison_count << ", Copies: " << averageStatsSortedShaker.copy_count << "\n";
        std::cout << "Sorted (Comb Sort): Comparisons: " << averageStatsSortedComb.comparison_count << ", Copies: " << averageStatsSortedComb.copy_count << "\n\n";
        std::cout << "Reverse (Bubble Sort): Comparisons: " << averageStatsReverse.comparison_count << ", Copies: " << averageStatsReverse.copy_count << "\n";
        std::cout << "Reverse (Shaker Sort): Comparisons: " << averageStatsReverseShaker.comparison_count << ", Copies: " << averageStatsReverseShaker.copy_count << "\n";
        std::cout << "Reverse (Comb Sort): Comparisons: " << averageStatsReverseComb.comparison_count << ", Copies: " << averageStatsReverseComb.copy_count << "\n";
    }

    return 0;
}
