import os
import numpy as np
import matplotlib.pyplot as plt
def time1_count():
    begin = 2
    end = 9
    os.system("mpicc task1.c")
    time1_array= []
    for i in range(begin, end):
        time1 = os.popen(f"mpirun -n {i} ./a.out").read()
        time1_array.append(float(time1))
    return time1_array

def time2_count():
    begin = 2
    end = 9
    os.system("mpicc task2.c")
    time2_array = []
    for i in range(begin, end):
        time2 = os.popen(f"mpirun -n {i} ./a.out").read()
        time2_array.append(float(time2))
    return time2_array

if __name__ =="__main__": 
    time1_array = time1_count()  
    time2_array = time2_count()
    print(time1_array)
    print(time2_array)
    begin = 2
    end = 9
    plt.plot(np.arange(begin, end), time1_array, label='MPI_Bcast')
    plt.plot(np.arange(begin, end), time2_array, label='MPI_Send')

    plt.grid()
    plt.legend()
    plt.show()
