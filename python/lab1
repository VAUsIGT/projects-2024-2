import os
import matplotlib.pyplot as plt
temp=[]
depth=[]
i=1

def derix(t,d):
    n=len(t)
    der=[]
    for i in range(n-1):
        der.append((t[i+1]-t[i])/(d[i+1]-d[i]))
    return der

with open("Alldata 07_26_2018.csv", "r") as file:
    file.readline()
    data=file.readlines()
    for line in data:
        spl_str=line.split(",")
        if int(spl_str[1]) == 18001:
            temp.append(float(spl_str[8]))
            depth.append(float(spl_str[6]))
            der1 = derix(temp, depth)
        elif 18001 + i:
            plt.plot(der1, depth[:-1])
            plt.plot(temp, depth)
            break

plt.gca().invert_yaxis()
plt.show()
print(temp,depth)
print(der1)
