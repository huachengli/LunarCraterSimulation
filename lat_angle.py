# -*- coding: utf-8 -*-
# 计算陨石在月球上的落点撞击纬度
import codecs
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
import math
import seaborn as sns
from sklearn.neighbors import KernelDensity

MoonEarth = 3.844e8
MoonMass  = 7.349e22
MoonRadiu = 1.738e6
EarthRadiu= 6.371e6
EarthMass = 5.965e24
G = 6.67e-11
LIM = 5 * MoonEarth

off  = 3
offv = -3
offm = 12
s    = 100

outM = codecs.open("./outMoon.txt","r",encoding="utf-8")

pX,pY,pZ = [],[],[]
Mx,My,Mz = [],[],[]
Vx,Vy,Vz = [],[],[]

for x in np.linspace(0,2*np.pi,30):
    Mx.append(4*MoonEarth*np.sin(x))
    My.append(4*MoonEarth*np.cos(x))
    Mz.append(0)

for line in outM:
    tmp = line.split(",")
    pX.append(float(tmp[4+off])-float(tmp[4+offm]))
    pY.append(float(tmp[5+off])-float(tmp[5+offm]))
    pZ.append(float(tmp[6+off])-float(tmp[6+offm]))

    Vx.append(float(tmp[4+offv]))
    Vy.append(float(tmp[5+offv]))
    Vz.append(float(tmp[6+offv]))

outM.close()


pX = np.array(pX)
pY = np.array(pY)
pZ = np.array(pZ)

vX = np.array(Vx)
vY = np.array(Vy)
vZ = np.array(Vz)

R = np.sqrt(pX*pX+pY*pY+pZ*pZ)
theta = np.arcsin(pZ/R)*180.0/np.pi

beta  = np.arctan(pX/pY)*180.0/np.pi

fig = plt.figure(2)
ax1 = fig.add_subplot(111)
ax1.set_xlim(-90,90)
# ax1.set_title("V=%d[m/s]"%s)
ax1.set_title("pos")
sns.distplot(theta,bins=50,hist=True,kde=True,norm_hist=True,rug=True,ax = ax1)

tX = []
tY = []

fp = codecs.open("lat_angle.csv","r","utf-8")
for line in fp:
    tmp = line.split(",")
    tX.append(float(tmp[0]))
    tY.append(float(tmp[1]))

fp.close()

tX = np.array(tX)
tY = np.array(tY)

ax1.plot(tX,tY,"r-.")

# ax2 = fig.add_subplot(212)
# ax2.set_xlim(-90,90)
# # ax2.set_ylim(0,2)
# theta2d = theta[:,np.newaxis]
# tX2d = tX[:,np.newaxis]
# sig = np.std(theta,ddof=1)
# bd  = np.power(len(theta),-0.2)*1.06*sig
    
# kde = KernelDensity(kernel='gaussian', bandwidth=bd).fit(theta2d)
# log_den = kde.score_samples(tX2d)
# den = np.exp(log_den)
# # ax2.plot(tX,den/tY,"k-")
# # ax2.plot(tX,den-tY)

# cX = []
# cY = []
# for k in range(len(tY)-1):
#     cX.append(0.5*tX[k]+0.5*tX[k+1])
#     tmp = (den[k]+den[k+1])/(tY[k]+tY[k+1])
#     cY.append(tmp)

# ax2.plot(cX,cY)
# ax2.plot(tX,[1]*len(tX),"-.")
# ax2.set_xlabel("latitude")
# ax2.set_ylabel("prob./area")
# ax2.set_ylim(0,5)

plt.show()





