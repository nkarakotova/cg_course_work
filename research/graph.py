import matplotlib.pyplot as plot

file = open("research/result/result.txt", "r") 

depth = []
time = []

line = file.readline()

while line:

    depth.append([float(x) for x in line.split()][0])
    time.append([float(x) for x in line.split()][1])

    line = file.readline()


plot.xlabel("Глубина трассировки")
plot.ylabel("Время (мс)")
plot.grid(True)

plot.plot(depth, time, color = "black")
plot.savefig("research/result/result.png")
