import os
import sys
import glob
import json
import subprocess

numthreads = 3;
hosts = '' # define hostnames to run on
rep = 1;

def main():
	args = sys.argv
	ls_v = glob.glob("./build/examples/*.json")
	for c in ls_v:
		mod = c.split('.')[1].split('/')[3]
		print(mod)
		subprocess.run(['./build/src/client', '--circuit', './build/examples/' + mod + '.json'])
		print('unode')
		subprocess.run(['./build/src/runtime_ms', '--circuit', './build/examples/' + mod + '.json', '--repetition', str(rep)])
		#print('mpi')
		#subprocess.run(['mpirun','-n', str(numthreads), '-host', hosts, './build/src/runtime_ms', '--circuit', './build/examples/' + mod + '.json', '--repetition', str(rep)])
		
		subprocess.run(['./build/src/verify', '--circuit', './build/examples/' + mod + '.json', '--repetition', str(rep)])
			
main()
