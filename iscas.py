import os
import subprocess


def main():
	if(os.path.exists("./examples/iscas85") == 0):
		os.makedirs("./examples/iscas85")
	os.chdir("./examples/iscas85")
	subprocess.run(["wget", "https://pld.ttu.ee/~maksim/benchmarks/iscas85/verilog/"])
	with open("index.html") as f:
		for line in f:
			if(line.split(" ")[0] == "<img"):
				subprocess.run(["wget", "https://pld.ttu.ee/~maksim/benchmarks/iscas85/verilog/" + line.split('"')[5]])
	if(os.path.exists("../iscas89") == 0):
		os.makedirs("../iscas89")
	os.chdir("../iscas89")
	subprocess.run(["wget", "https://pld.ttu.ee/~maksim/benchmarks/iscas89/verilog/"])
	with open("index.html") as f:
		for line in f:
			if(line.split(" ")[0] == "<img"):
				subprocess.run(["wget", "https://pld.ttu.ee/~maksim/benchmarks/iscas89/verilog/" + line.split('"')[5]])

main()



