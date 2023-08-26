import matplotlib.pyplot as plt

plt.figure(figsize=(5, 3), dpi=150)

x = [35.0,20.0,10.0,35.0]
y = [35.0,50.0,43.0,35.0]

# plt.plot(x, y, label='x', color='red', linewidth=2, marker='.', markersize=10, linestyle='--')

x1 = [35.0,30.0,30.0,35.0]
y1 = [35.0,5.0,25.0,35.0]

# plt.plot(x, y, label='x2', color='blue', linewidth=2, marker='.', markersize=10, linestyle='-')

xx = [x,x1]
yy = [y,y1]
for i in range(2):
    plt.plot(xx[i], yy[i], label=i)

plt.title('Vehicle Routes', fontdict={'fontsize': 20})
plt.xlabel('X Axis')
plt.ylabel('Y Axis')

# plt.xticks([10,15,15,10])
# plt.yticks([10,10,15,10])

plt.legend()

plt.show()