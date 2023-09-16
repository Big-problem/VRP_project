if __name__ == "__main__":
    for i in range(101,109):
        info = []
        with open(f'../benchmark/RC1/RC{i}.txt') as f:
            for line in f.readlines():
                info.append(line)

        l = [26, 51, 101]
        for j in l:
            with open(f'../benchmark/RC1/RC{i}_{j-1}.txt', 'w') as f:
                for k in range(0, j):
                    f.write(info[k])

    print('Done!')