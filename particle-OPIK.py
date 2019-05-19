import codecs
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
import math
import seaborn as sns
from sklearn.neighbors import KernelDensity
from numpy.linalg import cholesky


A, e, i = [], [], []
tga = []
fp = codecs.open("particle-OPIK.data", "r", "utf-8")
for line in fp:
    tmp = line.split()
    try:
        tmp = [float(x) for x in tmp]
        L1 = 1.0 - 1.0 / (tmp[0] * (1 + tmp[1]))
        L2 = 1.0 / (tmp[0] * (1 - tmp[1])) - 1
        
        if (L1 * L2 < 0):
            raise ValueError
        
        A.append(tmp[0])
        e.append(tmp[1])
        i.append(tmp[2])
        tga.append(np.sqrt(L1*L2))
    except:
        # print(line)
        pass
    finally:
        pass
fp.close()

A = np.array(A)
e = np.array(e)
i = np.array(i)/180.0 * np.pi
tga = np.array(tga)

print(tga.shape, A.shape)

# i = i + 5.0 / 180.0 * np.pi

m = np.sqrt(A * (1 - e*e))
Ux = m * tga
Uy = m * np.cos(i) - 1.0
Uz = m * np.sin(i)
U  = np.sqrt(3.0 -1.0/A - 2.0*m*np.cos(i))
# P = U / np.sin(i) / Ux
P = U / np.sqrt(Ux * Ux + Uz * Uz)
Q = np.sort(-P)
fig = plt.figure(1)
ax = fig.add_subplot(111)

ax.plot(-Q)

# sns.distplot(P, bins=50,ax=ax)
# ax.set_xlim(0,100)
# Hist = ax.hist(P, bins=100)
# ax.set_xlim(0, 1000)
# print(Hist)
plt.show()

for x in range(20):
    print(10*x,Q[10*x],Q[-10*x-1])