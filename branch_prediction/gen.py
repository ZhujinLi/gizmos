import random

N = 100000000
f = open("data.cpp", "w")
f.write(f"int N = {N};")
f.write("int arr[] = {")

last = 0
for i in range(N):
    if (i % 10000 == 0):
        print(i)
    if (random.randint(0, 1) == 1):
        last = random.randint(0, 1000000000)
    f.write(str(last) + ",")
f.write("0};")
