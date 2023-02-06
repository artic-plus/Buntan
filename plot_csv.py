import os
import sys
import csv
import matplotlib.pyplot as plt

bar_width = 0.4

def main():
	args = sys.argv
	if(len(args) < 2):
		print("path input required!")
		return
	fig = plt.figure()
	init = fig.add_subplot(3,1,1)
	total = fig.add_subplot(3,1,2)
	process = fig.add_subplot(3,1,3)
	label_init = []
	label_total = []
	label_process = []
	with open(args[1]) as f:
		reader = csv.reader(f)
		header = next(reader)
		for i, row in enumerate(reader):
			u_process = float(row[2]) - float(row[1])
			m_process = float(row[4]) - float(row[3])
			init.bar(i - bar_width/2, float(row[1]), width=bar_width, label='unode', align="center", color = 'g')
			init.text(i - bar_width/2, float(row[1]), '{:.0f}'.format(float(row[1])), va="bottom", ha="center")
			init.bar(i + bar_width/2, float(row[3]), width=bar_width, label='mpi', align="center", color = 'b')
			init.text(i + bar_width/2, float(row[3]), '{:.0f}'.format(float(row[3])), va="bottom", ha="center")
#			init.text(i, -3, '{:.2f}x'.format(float(row[1])/float(row[3])), va="top", ha="center")
			label_init.append(row[0] + '\n' + '{:.2f}x'.format(float(row[1])/float(row[3])))

			total.bar(i - bar_width/2, float(row[2]), width=bar_width, label='unode', align="center", color = 'g')
			total.text(i - bar_width/2, float(row[2]), '{:.0f}'.format(float(row[2])), va="bottom", ha="center")
			total.bar(i + bar_width/2, float(row[4]), width=bar_width, label='mpi', align="center", color = 'b')
			total.text(i + bar_width/2, float(row[4]), '{:.0f}'.format(float(row[4])), va="bottom", ha="center")
#			total.text(i, -3, '{:.2f}x'.format(float(row[2])/float(row[4])), va="top", ha="center")
			label_total.append(row[0] + '\n' + '{:.2f}x'.format(float(row[2])/float(row[4])))

			process.bar(i - bar_width/2, u_process, width=bar_width, label='unode', align="center", color = 'g')
			process.text(i - bar_width/2, u_process, '{:.0f}'.format(u_process), va="bottom", ha="center")
			process.bar(i + bar_width/2, m_process, width=bar_width, label='mpi', align="center", color = 'b')
			process.text(i + bar_width/2, m_process, '{:.0f}'.format(m_process), va="bottom", ha="center")
#			process.text(i, -3, '{:.2f}x'.format(u_process/m_process), va="top", ha="center")
			label_process.append(row[0] + '\n' + '{:.2f}x'.format(u_process/m_process))
	init.set_xticks(range(0, i + 1))
	init.set_xticklabels(label_init)
	init.set_title("init", y=-0.15)
#	init.legend()
	total.set_xticks(range(0, i + 1)) 
	total.set_xticklabels(label_total)
	total.set_title("total", y=-0.15)
#	total.legend()
	process.set_xticks(range(0, i + 1)) 
	process.set_xticklabels(label_process)
	process.set_title("process", y=-0.15)
#	process.legend()

	plt.show()
main()
