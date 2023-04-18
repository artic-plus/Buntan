import os
import sys
import glob
import json
import csv
import datetime
import subprocess

numthreads = 2;
hosts = 'futago,kazura' # define hostnames to run on
rep = 10;

def main():
	args = sys.argv
	if(len(args) < 3):
		print("input required!")
		return
	clk = args[2]
	ls_v = glob.glob(args[1] + "*.json")
	print(len(ls_v))
	datestr = datetime.datetime.now().strftime('%Y:%m:%d_%H:%M')
	if(os.path.exists("./meas") == 0):
		os.mkdir("./meas")
	with open("meas/meas_mpi_" + datestr + ".csv", "w") as f:
		writer = csv.writer(f)
		writer.writerow(["circuit",  "unode_init", "unode_insert","unode_exec","unode_total", "mpi_init","mpi_insert","mpi_exec", "mpi_total"])
		with open("meas/meas_mpi_ave_" + datestr + ".csv", "w") as f_ave:
			writer_ave = csv.writer(f_ave)
			writer_ave.writerow(["circuit",  "unode_init","unode_insert", "unode_exec","unode_total", "mpi_init","mpi_insert","mpi_exec",  "mpi_total"])
			for c in ls_v:
				name = c.split('.')[len(c.split('.')) - 2].split('/')[len(c.split('.')[len(c.split('.')) - 2].split('/')) - 1]
				print(name)
				subprocess.run(['./build/src/client', '--circuit', c, '--repetition', str(clk)])
				time = [0,0,0,0,0,0,0,0]
				for i in range(rep):
#					print('unode')
					unode = subprocess.run(['./build/src/runtime_ms', '--circuit', c, '--repetition', str(clk)], capture_output=True, text=True).stdout
#					print('mpi')
					mpi = subprocess.run(['mpirun','-n', str(numthreads), '-host', hosts, './build/src/runtime_ms', '--circuit', c, '--repetition', str(clk)], capture_output=True, text=True).stdout
#					print(unode.split())
					print(mpi.split())
					time[0] += float(unode.split()[7])
					time[1] += float(unode.split()[12])
					time[2] += float(unode.split()[17])
					time[3] += float(unode.split()[22])
					time[4] += float(mpi.split()[7])
					time[5] += float(mpi.split()[12])
					time[6] += float(mpi.split()[17])
					time[7] += float(mpi.split()[22])
					writer.writerow([name, unode.split()[7], unode.split()[12], unode.split()[17], unode.split()[22],  mpi.split()[7], mpi.split()[12],  mpi.split()[17], mpi.split()[22]])
#				subprocess.run(['./build/src/verify', '--circuit', c, '--repetition', str(clk)])
				writer_ave.writerow([name, time[0] / rep, time[1] / rep, time[2] / rep, time[3] / rep , time[4] / rep, time[5] / rep, time[6] / rep, time[7] / rep])

main()
