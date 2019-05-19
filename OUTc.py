import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.ticker import FuncFormatter
import codecs

p = []
Scale = 5 * 3.844e8
fp = codecs.open("OUTc", "r", "utf-8")
for line in fp:
    tmp = line.split(",")
    try:
        tmp = [float(x) for x in tmp[:-1]]

        p.append(tmp)
    except:
        print(tmp)
    finally:
        pass

fp.close()
p = np.array(p)

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d', aspect='equal')
ax.set_zlim(-Scale, Scale)
ax.set_ylim(-Scale, Scale)
ax.set_xlim(-Scale, Scale)

ax.plot(p[:, 0], p[:, 1], p[:, 2])
ax.plot(p[:, 3], p[:, 4], p[:, 5])
plt.show()
