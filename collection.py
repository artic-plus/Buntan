
import subprocess
import os
import sys
import glob


def main():
	args = sys.argv
	if(len(args) < 3):
		print("input required!")
		return
	ls_v = glob.glob(args[1] + "*.json")
	print(len(ls_v))
	for c in ls_v:
		name = c.split('.')[len(c.split('.')) - 2].split('/')[len(c.split('.')[len(c.split('.')) - 2].split('/')) - 1]
		print(name)
		if(os.path.exists("./meas/" + args[1].split('/')[len(args[1].split('/')) - 2] + "/" + name + '_' + args[2]) == 0):
			os.makedirs("./meas/" + args[1].split('/')[len(args[1].split('/')) - 2] + "/" + name + '_' + args[2])
		os.chdir("./meas/" + args[1].split('/')[len(args[1].split('/')) - 2] + "/" + name + args[2])
		subprocess.run(['starpu_fxt_tool', '-i', '/tmp/' + name + '_' + args[0] + '_0'])
		os.chdir("../../../")
#		subprocess.run(['pwd'])
main()
