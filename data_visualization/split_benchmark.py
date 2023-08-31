if __name__ == "__main__":
    for i in range(101,110):
        info = []
        with open(f'../benchmark/C1/C{i}.txt') as f:
            for line in f.readlines():
                info.append(line)

        l = [26, 51, 101]
        for j in l:
            with open(f'../benchmark/C1/C{i}_{j-1}.txt', 'w') as f:
                for k in range(0, j):
                    f.write(info[k])

    print('Done!')