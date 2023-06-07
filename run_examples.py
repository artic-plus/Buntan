import os
import sys
import glob
import json
import csv
import datetime
import subprocess
from tqdm import tqdm

numthreads = ;
hosts = [''] # define hostnames to run on except itself
rep = 1;

def main():
	args = sys.argv
	if(len(args) < 3):
		print("input required!")
		return
	clk = args[2]
	ls_v = glob.glob(args[1] + "*.json")
	print(len(ls_v))
	datestr = datetime.datetime.now().strftime('%Y:%m:%d_%H:%M')
	if(os.path.exists("./meas/" + args[1].split('/')[len(args[1].split('/')) - 2]) == 0):
		os.mkdir("./meas" + args[1].split('/')[len(args[1].split('/')) - 2])
	with open("meas/" + args[1].split('/')[len(args[1].split('/')) - 2] + "_" + str(numthreads) + "thread_" + datestr + ".csv", "w") as f:
		writer = csv.writer(f)
		writer.writerow(["circuit",  "init", "insert","exec","total"])
		with open("meas/" + args[1].split('/')[len(args[1].split('/')) - 2] + "_" + str(numthreads) + "thread_ave_" + datestr  +  ".csv", "w") as f_ave:
			writer_ave = csv.writer(f_ave)
			writer_ave.writerow(["circuit", "init","insert", "exec","total"])
			for c in tqdm(ls_v):
				name = c.split('.')[len(c.split('.')) - 2].split('/')[len(c.split('.')[len(c.split('.')) - 2].split('/')) - 1]
				print(name)
				subprocess.run(['./build/src/client', '--circuit', c, '--repetition', str(clk)])
				for host in hosts:
					subprocess.run(['scp', './cloud.data', 'ubuntu@' + host + ':' + os.getcwd()])
					subprocess.run(['scp', './cloud.key', 'ubuntu@' + host + ':' + os.getcwd()])

				time = [0,0,0,0]
				for i in range(rep):
#					print('unode')
					os.environ['STARPU_FXT_SUFFIX'] = name + '_' + str(numthreads) + 'thread'
					ret = subprocess.run(['mpirun','-n', str(numthreads), '-host', ','.join(hosts) + '.127.0.0.0', './build/src/runtime_ms', '--circuit', c, '--repetition', str(clk)], capture_output=True, text=True).stdout
#					os.chdir("./meas" + args[1])
#					subprocess.run(['starpu_fxt_tool' '-i' '/tmp/' + c + '_unode_0'])
#					os.chdir("../")
#					print(unode.split())
					print(ret)
					time[0] += float(ret.split()[7])
					time[1] += float(ret.split()[12])
					time[2] += float(ret.split()[17])
					time[3] += float(ret.split()[22])
					writer.writerow([name, ret.split()[7], ret.split()[12], ret.split()[17], ret.split()[22]])
#				subprocess.run(['./build/src/verify', '--circuit', c, '--repetition', str(clk)])
				writer_ave.writerow([name, time[0] / rep, time[1] / rep, time[2] / rep, time[3] / rep])

main()
