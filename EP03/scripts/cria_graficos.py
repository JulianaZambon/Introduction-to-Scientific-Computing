import matplotlib.pyplot as plt
import numpy as np
import os

def get_file_info(path, metric):
    is_a = True
    with open(path, "r") as file:
        for line in file:
            m, value, _ = line.split(",")[:3]
            if (m != metric):
                continue
            if is_a:
                value_a = float(value)
                is_a = False
            else:
                value_b = float(value)
    return value_a, value_b


def get_info(group, metric):
    path_v1 = "results/v1"
    path_v2 = "results/v2"

    data = {
        "a": {
            ("v1", 10): {},
            ("v1", 1000): {},
            ("v2", 10): {},
            ("v2", 1000): {}
        },
        "b": {
            ("v1", 10): {},
            ("v1", 1000): {},
            ("v2", 10): {},
            ("v2", 1000): {}
        }
    }

    for file in os.listdir(path_v1):
        if file[0] == ".":
            continue
        if "FLOPS" in file:
            g1, g2, k, n = file.split("_")
            g = g1+"_"+g2
        else:
            g, k, n = file.split("_")
        n = int(n[:-4])
        if (g != group):
            continue
        full_path = "results/v1/"+file
        value_a, value_b = get_file_info(full_path, metric)
        data["a"][("v1", n)][int(k)] = value_a
        data["b"][("v1", n)][int(k)] = value_b

    for file in os.listdir(path_v2):
        if file[0] == ".":
            continue
        if "FLOPS" in file:
            g1, g2, k, n = file.split("_")
            g = g1+"_"+g2
        else:
            g, k, n = file.split("_")
        n = int(n[:-4])
        if (g != group):
            continue
        full_path = "results/v2/"+file
        print(full_path, metric)
        value_a, value_b = get_file_info(full_path, metric)
        data["a"][("v2", n)][int(k)] = value_a
        data["b"][("v2", n)][int(k)] = value_b

    return data


# returns in the following format:
# vector k
# vector v1_10
# vector v1_1000
# vector v2_10
# vector v2_1000
def organize_data(data):
    organized_data = {
        "a": [[], [], [], [], []],
        "b": [[], [], [], [], []]
    }
    k = list(data["a"][("v1", 10)].keys())
    k.sort()

    organized_data["a"][0] = k
    organized_data["b"][0] = k

    for i in k:
        data_a = data["a"]
        data_b = data["b"]
        organized_data["a"][1].append(data_a[("v1", 10)].get(i, np.nan))
        organized_data["a"][2].append(data_a[("v1", 1000)].get(i, np.nan))
        organized_data["a"][3].append(data_a[("v2", 10)].get(i, np.nan))
        organized_data["a"][4].append(data_a[("v2", 1000)].get(i, np.nan))
        organized_data["b"][1].append(data_b[("v1", 10)].get(i, np.nan))
        organized_data["b"][2].append(data_b[("v1", 1000)].get(i, np.nan))
        organized_data["b"][3].append(data_b[("v2", 10)].get(i, np.nan))
        organized_data["b"][4].append(data_b[("v2", 1000)].get(i, np.nan))
    return organized_data


def plot_graphic(x, Y, file_path, y_log=False):
    plt.xscale("log")
    if y_log:
        plt.yscale("log")
    labels = ["v1 10", "v1 1000", "v2 10", "v2 1000"]
    styles = ["b--", "g--", "y-", "k-"]
    for i, y in enumerate(Y):
        plt.plot(x, y, styles[i], label=labels[i])
    plt.legend()
    plt.savefig(file_path)
    plt.close()


def plot_from_data(data, dir, file_name, y_log=False):
    file_path_a = os.path.join(dir, "a", file_name)
    x = data["a"][0]
    Y = data["a"][1:]
    plot_graphic(x, Y, file_path_a, y_log=y_log)
    file_path_b = os.path.join(dir, "b", file_name)
    x = data["b"][0]
    Y = data["b"][1:]
    plot_graphic(x, Y, file_path_b, y_log=y_log)


# the time csv is different than the others
# because it was not made with likwid
def plot_time(file_path):
    x = set()
    time = {
        "a": [[], [], [], []],
        "b": [[], [], [], []]
    }

    with open(file_path, "r") as file:
        file.readline()  # ignore first line
        for line in file:
            n, k, version, time_a, time_b = line.split(",")
            x.add(int(k))
            if version == "1":
                if n == "10":
                    time["a"][0].append(float(time_a))
                    time["b"][0].append(float(time_b))
                else:
                    time["a"][1].append(float(time_a))
                    time["b"][1].append(float(time_b))
            else:
                if n == "10":
                    time["a"][2].append(float(time_a))
                    time["b"][2].append(float(time_b))
                else:
                    time["a"][3].append(float(time_a))
                    time["b"][3].append(float(time_b))
    x = list(x)
    x.sort()
    size_10 = len(time["a"][0])
    size_1000 = len(time["a"][1])
    time["a"][1] += [np.nan] * (size_10 - size_1000)
    time["a"][3] += [np.nan] * (size_10 - size_1000)
    time["b"][1] += [np.nan] * (size_10 - size_1000)
    time["b"][3] += [np.nan] * (size_10 - size_1000)
    plot_graphic(x, time["a"], "graphics/a/TIME.png", y_log=True)
    plot_graphic(x, time["b"], "graphics/b/TIME.png", y_log=True)




data = organize_data(get_info("L3CACHE", "L3 miss ratio"))
plot_from_data(data, "graphics", "L3_MISS_RATIO.png")

data = organize_data(get_info("ENERGY", "Energy [J]"))
plot_from_data(data, "graphics", "ENERGY.png")

data = organize_data(get_info("ENERGY", "Energy [J]"))
plot_from_data(data, "graphics", "ENERGY_LOG.png", y_log=True)

data = organize_data(get_info("FLOPS_DP", "DP MFLOP/s"))
plot_from_data(data, "graphics", "FLOPS_DP.png")

data = organize_data(get_info("FLOPS_DP", "AVX DP MFLOP/s"))
plot_from_data(data, "graphics", "FLOPS_AVX_DP.png")

plot_time("results/time.csv")