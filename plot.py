import argparse
import matplotlib.pyplot as plt

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-M", dest="file_M", metavar="magnetization_file", required=True, help="Magnetization data file name or file path.")
    parser.add_argument("-S", dest="file_S", metavar="susceptibility_file", required=True, help="Susceptibility data file name or file path.")
    parser.add_argument("--save", dest="figureFile", metavar="figure_output_file", required=False, help="Saves the figure. Provide a file name or file path.")
    parser.add_argument("--no-gui", dest="noGui", action="store_true", required=False, help="Pass the argument if you do not want matplotlib GUI to show up.")
    args = parser.parse_args()

    x_M = []
    y_M = []
    x_S = []
    y_S = []

    with open(args.file_M, "r") as file:
        for line in file:
            pair = line.split()
            x_M.append(float(pair[0]))
            y_M.append(float(pair[1]))

    with open(args.file_S, "r") as file:
        for line in file:
            pair = line.split()
            x_S.append(float(pair[0]))
            y_S.append(float(pair[1]))

    plt.rcParams["figure.figsize"] = [15, 5]

    fig, (ax1, ax2) = plt.subplots(1, 2)
    ax1.scatter(x_M, y_M, s=2)
    ax2.scatter(x_S, y_S, s=2)
    ax1.set(xlabel="Temperature * k / J", ylabel="Average Magnetization per Site")
    ax2.set(xlabel="Temperature * k / J", ylabel="Susceptibility")

    if args.figureFile is not None:
        plt.savefig(args.figureFile)

    if not args.noGui:
        plt.show()


if __name__ == "__main__":
    main()
