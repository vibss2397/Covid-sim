import numpy as np
from matplotlib import pyplot as plt
from scipy.stats import poisson

m = np.arange(5, 20)
for ele in m:
    temp_x = []
    temp_y = []
    for i in range(0, 30):
        y = poisson.pmf(i, mu=ele)
        temp_x.append(i)
        temp_y.append(y)
    plt.plot(temp_x, temp_y)
plt.legend(np.arange(5, 20))
plt.show()
