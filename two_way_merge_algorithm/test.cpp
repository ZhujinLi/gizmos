#include <vector>
#include <iostream>
#include "two_way_merge_algorithm.h"


static void mergeSortCallback(std::vector<int>& arr, size_t left, size_t mid, size_t right)
{
    std::vector<int> arrCopy = arr;
    size_t i = left, j = mid + 1, k = left;
    while (i <= mid && j <= right)
    {
        if (arrCopy[i] < arrCopy[j])
            arr[k++] = arrCopy[i++];
        else
            arr[k++] = arrCopy[j++];
    }
    while (i <= mid)
        arr[k++] = arrCopy[i++];
    while (j <= right)
        arr[k++] = arrCopy[j++];
}

int main()
{
    {
        std::vector<int> arr = { 4,2,1,8,10,3,9 };
        TwoWayMergeAlgorithm_iterative(arr, mergeSortCallback);
        for (int x : arr)
            std::cout << x << std::endl;
    }

    std::cout << "--------" << std::endl;

    {
        std::vector<int> arr = { 4,2,1,8,10,3,9 };
        TwoWayMergeAlgorithm_recursive(arr, mergeSortCallback);
        for (int x : arr)
            std::cout << x << std::endl;
    }

    return 0;
}