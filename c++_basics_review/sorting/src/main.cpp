#include <cmath>
#include <iostream>
#include <string>
#include <vector>

void fullTest(std::vector<float> testNums);
void bubblesort(std::vector<float>& nums);
void mergesort(std::vector<float>& nums);
void quicksort(std::vector<float>& nums);
void quicksort(std::vector<float>& nums, int start, int end);
bool isSorted(const std::vector<float>& nums);
void printVec(const std::vector<float>& nums);

int main(int argc, char** argv) {
    fullTest({1.0f, 2.0f, 3.0f, 4.0f});
    fullTest({100.0f, 100.5f, 99.5f, 2.0f, -2.0f, 5.0f, -200.0f, 100.0f, 100.0f, 2.0f});
    return EXIT_SUCCESS;
}

void fullTest(std::vector<float> testNums) {
    std::cout << "=============";
    std::cout << "Sorting";
    printVec(testNums);

    std::vector<float> bubbleSorted = testNums;
    bubblesort(bubbleSorted);
    std::cout << "Bubble ";
    printVec(bubbleSorted);
    std::cout << "Bubble sort success " << std::boolalpha << isSorted(bubbleSorted) << "\n";

    std::vector<float> mergeSorted = testNums;
    mergesort(mergeSorted);
    std::cout << "Merge ";
    printVec(mergeSorted);
    std::cout << "Merge sort success " << std::boolalpha << isSorted(mergeSorted) << "\n";

    std::vector<float> quickSorted = testNums;
    quicksort(quickSorted);
    std::cout << "Quick ";
    printVec(quickSorted);
    std::cout << "Quick sort success " << std::boolalpha << isSorted(quickSorted) << "\n\n";
}

void bubblesort(std::vector<float>& nums) {
    bool done = false;
    while (!done) {
        done = true;
        for (int index = 1; index < nums.size(); index++) {
            if (nums[index] < nums[index - 1]) {
                std::swap(nums[index], nums[index - 1]);
                done = false;
            }
        }
    }
}

void mergesort(std::vector<float>& nums) {
    if (nums.size() == 1) {
        return;
    }
    std::vector<float> left(nums.size() / 2);
    std::vector<float> right((size_t)std::ceil(nums.size() / 2.0f));

    std::copy(nums.begin(), nums.begin() + (nums.size() / 2), left.begin());
    std::copy(nums.begin() + (nums.size() / 2), nums.begin() + nums.size(), right.begin());

    mergesort(left);
    mergesort(right);

    int leftIndex = 0;
    int rightIndex = 0;

    for (int outIndex = 0; outIndex < nums.size(); outIndex++) {
        if (rightIndex < right.size() && (leftIndex >= left.size() || left[leftIndex] > right[rightIndex])) {
            nums[outIndex] = right[rightIndex];
            rightIndex++;
        } else {
            nums[outIndex] = left[leftIndex];
            leftIndex++;
        }
    }
}

void quicksort(std::vector<float>& nums) {
    quicksort(nums, 0, nums.size() - 1);
}
void quicksort(std::vector<float>& nums, int start, int end) {
    if (start >= end) {
        return;
    }
    auto pivot = nums[end];
    auto pivotIndex = end;
    for (int index = start; index < pivotIndex; index++) {
        if (nums[index] < pivot) {
            continue;
        }

        std::swap(nums[pivotIndex], nums[index]);
        std::swap(nums[index], nums[pivotIndex - 1]);
        index--;
        pivotIndex--;
    }

    quicksort(nums, start, pivotIndex - 1);
    quicksort(nums, pivotIndex + 1, end);
}

bool isSorted(const std::vector<float>& nums) {
    for (int index = 1; index < nums.size(); index++) {
        if (nums[index] < nums[index - 1]) {
            return false;
        }
    }
    return true;
}

void printVec(const std::vector<float>& nums) {
    std::cout << " { ";
    for (auto num : nums) {
        std::cout << num << " ";
    }
    std::cout << "}\n";
}