
import subprocess
import os
import sys
import glob


def main():
	args = sys.argv
	if(len(args) < 2):
		print("input required!")
		return
	ls_v = glob.glob(args[1] + "*.json")
	print(len(ls_v))
	for c in ls_v:
		name = c.split('.')[len(c.split('.')) - 2].split('/')[len(c.split('.')[len(c.split('.')) - 2].split('/')) - 1]
		print(name)
		if(os.path.exists("./meas/" + args[1].split('/')[len(args[1].split('/')) - 2] + "/" + name + "_unode") == 0):
			os.makedirs("./meas/" + args[1].split('/')[len(args[1].split('/')) - 2] + "/" + name + "_unode")
		os.chdir("./meas/" + args[1].split('/')[len(args[1].split('/')) - 2] + "/" + name + "_unode")
		subprocess.run(['starpu_fxt_tool', '-i', '/tmp/' + name + '_unode_0'])
		os.chdir("../../../")
#		subprocess.run(['pwd'])
		print(os.path.exists("./meas/" + args[1].split('/')[len(args[1].split('/')) - 2] + "/" + name + "_mpi"))
		if(os.path.exists("./meas/" + args[1].split('/')[len(args[1].split('/')) - 2] + "/" + name + "_mpi") == 0):
			os.makedirs("./meas/" + args[1].split('/')[len(args[1].split('/')) - 2] + "/" + name + "_mpi")
		os.chdir("./meas/" + args[1].split('/')[len(args[1].split('/')) - 2] + "/" + name + "_mpi")
		subprocess.run(['starpu_fxt_tool', '-i', '/tmp/' + name + '_mpi_0'])
		os.chdir("../../../")
		subprocess.run(['pwd'])
#
main()
