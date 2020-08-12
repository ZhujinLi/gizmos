#pragma once
#include <vector>
#include <cassert>


// Merge arr[left, mid] and arr[mid+1, right] into arr[left, right].
template<typename T>
using MergeCallback = void (*)(std::vector<T>& arr, size_t start, size_t mid, size_t right);

template <typename T>
inline void _recursiveImple(std::vector<T>& arr, size_t left, size_t right, MergeCallback<T> mergeCallback);

template <typename T>
void TwoWayMergeAlgorithm_recursive(std::vector<T>& arr, MergeCallback<T> mergeCallback)
{
    if (!arr.empty())
        _recursiveImple(arr, 0, arr.size() - 1, mergeCallback);
}

template <typename T>
void TwoWayMergeAlgorithm_iterative(std::vector<T>& arr, MergeCallback<T> mergeCallback)
{
    struct StackFrame
    {
        size_t left, right;
        int state;
    };

    std::vector<StackFrame> stack;

    if (!arr.empty())
        stack.push_back(StackFrame{ 0, arr.size() - 1, 0 });

    while (!stack.empty())
    {
        size_t left = stack.back().left;
        size_t right = stack.back().right;
        size_t mid = (left + right) / 2;

        if (left == right)                  // Leaf
        {
            stack.pop_back();
        }
        else if (stack.back().state == 0)   // First time
        {
            stack.back().state++;
            stack.push_back(StackFrame{ left, mid, 0 });
        }
        else if (stack.back().state == 1)   // Left sub-tree has been visited
        {
            stack.back().state++;
            stack.push_back(StackFrame{ mid + 1, right, 0 });
        }
        else if (stack.back().state == 2)   // Right sub-tree has been visited
        {
            mergeCallback(arr, left, mid, right);
            stack.pop_back();
        }
        else
        {
            assert(false);
        }
    }
}

template <typename T>
inline void _recursiveImple(std::vector<T>& arr, size_t left, size_t right, MergeCallback<T> mergeCallback)
{
    if (left < right)
    {
        size_t mid = (left + right) / 2;
        _recursiveImple(arr, left, mid, mergeCallback);
        _recursiveImple(arr, mid + 1, right, mergeCallback);
        mergeCallback(arr, left, mid, right);
    }
}

