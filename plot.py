import os
import sys
import csv
import matplotlib.pyplot as plt


def main():
        args = sys.argv
        if(len(args) < 3):
                print("path input required!")
                return
        cmap = plt.get_cmap("tab10")
        label = []
        print(len(args))
        args.pop(0)
        bar_width = 1 / (len(args) + 2)
        print(len(args))
        with open(args.pop(0)) as c:
                reader_c = csv.reader(c)
                header_c = next(reader_c)
                csvs = []
                for i, arg in enumerate(args):
                        with open(arg) as f:
                                reader = csv.reader(f)
                                header = next(reader)
                                csv_ = []
                                for r in reader:
                                        csv_.append(r)
                        csvs.append(csv_)
                for n, row in enumerate(reader_c):
                                        print(row[0])
                                        print("aaa")
                                        label.append(row[0])
                                        print(len(csvs))
                                        for i, csv_ in enumerate(csvs):
                                                print(args[i])
                                                for r in csv_:
                                                        print(r[0])
                                                        if(r[0] == row[0]):
                                                                print(r[3])
                                                                bar1 = plt.bar(n - (1/2 + i) /(len(args) + 2), float(r[3]), width=bar_width, label=row[0], align="center", color = cmap(i))
                                                                plt.text(n - (1/2 + i) / (len(args) + 2), float(r[3]), '{:.0f}'.format(float(r[3])), va="bottom", ha="center", rotation = 90, size=12)
        plt.tick_params(labelsize=12)
        plt.xticks(range(0, n+1), label)
        plt.ylabel("time [ms]", size=12)
#       plt.title("", y=-0.4)

        plt.show()
main()

