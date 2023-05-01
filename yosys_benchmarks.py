import os
import sys
import glob
import json
import csv
import subprocess

def main():
	args = sys.argv
	if(len(args) < 2):
		print("dir path required")
		return
	if(os.path.exists("./build/" + args[1]) == 0):
		os.makedirs("./build/" + args[1])
	ls_v = glob.glob(args[1] + "*.v")
	print(len(ls_v))
	with open("./build/" + args[1] + 'circuits.csv', "w") as f:
		writer = csv.writer(f)
		writer.writerow(["circuit", "numgates", "numinputs", "numoutputs"])

		for c in ls_v:
			dff_mod = -1
			name = c.split('.')[len(c.split('.')) - 2].split('/')[len(c.split('.')[len(c.split('.')) - 2].split('/')) - 1]
			print(name)
			with open(c, "r") as fv:
				lines = fv.readlines()
			for i, line in enumerate(lines):
				if(len(line.split()) == 0):
					continue
				print(line.split()[0])
				if line.split()[0] == "module":
					if line.split()[1] == "dff":
						dff_mod = i
						print(line)
					else:
						mod = line.split()[1].split('(')[0]
						break
				elif line.split()[0] == "endmodule":
					end_mod = i
				
			if dff_mod >= 0:
				for i in range(dff_mod, end_mod + 1):
					lines.pop(dff_mod)
				c = c + ".modified"
				with open(c, "w") as fv:
					fv.writelines(lines)

			try:
				mod
			except NameError:
				print('verilog file "' + name + '" do not have module!')
				break

			with open("./build/build.ys", "w") as fp:
				fp.write("read_verilog " + c + " ./examples/basic_gates.v\n")
				fp.write("hierarchy -check -top " + mod +  "\n")
				fp.write("proc; opt; fsm; opt; memory; opt\n")
				fp.write("techmap; opt\n")
				fp.write("flatten;\n")
				fp.write("abc -g gates,MUX\n")
				fp.write("clean -purge\n")
				fp.write("write_json ./build/" + args[1] + name + ".json\n")
			out = subprocess.run(['yosys', './build/build.ys'],  capture_output=True, text=True).stdout
			ngates = 0
			ninputs = 0
			noutputs = 0
			lines = out.splitlines()
			for line in lines:
			        if(len(line.split()) > 2):
			                if((line.split()[0] == "ABC") & (line.split()[1] == "RESULTS:")):
			                        print(int(line.split()[4]))
			                        if(line.split()[3] == "cells:"):
			                                ngates = ngates + int(line.split()[4])
			                                print(ngates)
			                        if(line.split()[2] == "input"):
			                                ninputs = int(line.split()[4])
			                        if(line.split()[2] == "input"):
			                                noutputs = int(line.split()[4])
			writer.writerow([mod, ngates, ninputs, noutputs])

main()
