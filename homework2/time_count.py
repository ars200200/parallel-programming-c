import numpy as np
import matplotlib.pyplot as plt
from my_time import time1_count, time2_count

begin = 2
end = 9
count = int(input())
time1 = np.zeros(end - begin)
time2 = np.zeros(end - begin)

for i in range(count):
    time1 += time1_count()
    time2 += time2_count()

time1 = time1 / count
time2 = time2 / count
plt.plot(np.arange(begin, end), time1, label='MPI_Bcast')
plt.plot(np.arange(begin, end), time2, label='MPI_Send')
plt.legend()
plt.grid()
plt.show()
