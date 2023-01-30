import os
import sys
import glob
import json
import subprocess

numthreads = 2;
hosts = '' # define hostnames to run on
rep = 1;

def main():
	args = sys.argv
	if(os.path.exists("./build/json") == 0):
		os.mkdir("./build/json")
	if(len(args) < 2):
		print("input required!")
	else:
		print(args.pop(0))
		print(args)
		for c in args:
			mod = c.split('.')[len(c.split('.')) - 2].split('/')[len(c.split('.')[len(c.split('.')) - 2].split('/')) - 1]
			print(mod)
			if(os.path.exists(c) == 0):
				print("err: file " + c + " not found!")
				continue
			with open("build.ys", "w") as fp:
				fp.write("read_verilog " + c + "\n")
				fp.write("hierarchy -check -top " + mod +  "\n")
				fp.write("proc; opt; fsm; opt; memory; opt\n")
				fp.write("techmap; opt\n")
				fp.write("flatten;\n")
				fp.write("abc -g gates,MUX\n")
				fp.write("clean -purge\n")
				fp.write("write_json ./build/json/" + mod + ".json\n")
			subprocess.run(['yosys', 'build.ys'])
			subprocess.run(['./build/src/client_manual', '--circuit', './build/json/' + mod + '.json', '--repetition', str(rep)])
			print('unode')
			subprocess.run(['./build/src/runtime_ms', '--circuit', './build/json/' + mod + '.json', '--repetition', str(rep)])
			#print('mpi')
			#subprocess.run(['mpirun','-n', str(numthreads), '-host', hosts, './build/src/runtime_ms', '--circuit', './build/json/' + mod + '.json', '--repetition', str(rep)])
			
			subprocess.run(['./build/src/verify', '--circuit', './build/json/' + mod + '.json', '--repetition', str(rep)])
			
main()
