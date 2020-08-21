import math
import matplotlib.pyplot as plt

def t0(n):
    return n

def t1(n):
    return n * math.log(n)

def t2(n):
    logn = math.log(n)
    sum = 0
    for i in range(int(logn) + 1):
        sum += math.pow(2, i) * (logn - i)
    return sum

def t3(n):
    sum = 0
    while n >= 1:
        sum += math.log(n)
        n -= 1
    return sum

N = 1000

plt.plot(range(N-1), [t0(n) for n in range(1, N)], 'r--', label='t0 = n')
plt.plot(range(N-1), [t1(n) for n in range(1, N)], 'g--', label='t1 = n*log(n)')
plt.plot(range(N-1), [t2(n) for n in range(1, N)], 'r-', label='t2 = O(n) is time of making heap')
plt.plot(range(N-1), [t3(n) for n in range(1, N)], 'g-', label='t3 = O(n*log(n)) is time of heap sort')
plt.legend()
plt.show()
