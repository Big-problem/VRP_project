import matplotlib.pyplot as plt

def get_info(): # 讀address跟position
    with open('../benchmark/C1/C101_100.txt') as f:
        d = {}
        for info in f.readlines():
            tmp = info.split(';')[:3]
            d[int(tmp[0])] = (float(tmp[1]), float(tmp[2]))
    return d

def draw(d):
    plt.figure(figsize=(5, 3), dpi=150)

    with open('../result/C1/C101_100_result.txt') as f:
        for count, route in enumerate(f.readlines()):
            if route == "\n":
                break
            tmp = route.split('->')            
            x, y = [], []
            for i in map(lambda x: int(x.strip()), tmp):
                x.append(d[i][0])
                y.append(d[i][1])
            x.append(x[0])
            y.append(y[0])
            plt.plot(x, y, label=f'route {count+1}', marker='.', markersize=5)

    plt.title('Vehicle Routes', fontdict={'fontsize': 20})
    plt.xlabel('X Axis')
    plt.ylabel('Y Axis')

    plt.legend(fontsize=5)
    plt.show()


if __name__ == '__main__':
    d = get_info()

    draw(d)