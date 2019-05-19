
import codecs 
import matplotlib.pyplot as plt
import numpy as np

file = ["outMoon.csv","outEarth.csv"]
V    = range(3100,38000,1000)
stat = {"outMoon.csv":[],"outEarth.csv":[]}


for x in V:
    for y in file:
        cnt = 0
        try:
            if(y==file[0]):
                hdf = codecs.open("./outMoon%d.csv"%x,"r")
            else:
                hdf = codecs.open("./outEarth%d.csv"%x,"r")  
            # hdf = codecs.open(y+str(x),"r")

            for line in hdf:
                cnt = cnt + 1
            hdf.close()
            stat[y].append(1.0*cnt)
        except:
            print("no %d",x)
            # hdf = codecs.open("./V66/"+y+str(x),"r")
        finally:
            pass

        

# print(stat)

Y = np.array(stat["outEarth.csv"])/np.array(stat["outMoon.csv"])
fig = plt.figure(1)
ax = fig.add_subplot(111)
ax.set_xlabel("velocity[m/s]")
ax.set_ylabel("N(Earth)/N(Moon)")
ax.scatter([int(z) for z in V],Y,marker="x")
ax.plot([int(z) for z in V],Y,"k-.")
plt.show()

print V
print Y
