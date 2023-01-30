import os
import glob
import json
import subprocess

def main():
	print("hoge")
	if(os.path.exists("./build/examples") == 0):
		os.mkdir("./build/examples")
	ls_v = glob.glob("./examples/*.v")
	for c in ls_v:
		mod = c.split('.')[1].split('/')[2]
		if(("88" not in mod)):
			print(mod)
			with open("build.ys", "w") as fp:
				fp.write("read_verilog ./examples/" + mod + ".v\n")
				fp.write("hierarchy -check -top " + mod +  "\n")
				fp.write("proc; opt; fsm; opt; memory; opt\n")
				fp.write("techmap; opt\n")
				fp.write("flatten;\n")
				fp.write("abc -g gates,MUX\n")
				fp.write("clean -purge\n")
				fp.write("write_json ./build/examples/" + mod + ".json\n")
			subprocess.run(['yosys', 'build.ys'])
			
main()
