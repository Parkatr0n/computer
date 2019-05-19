#!/usr/bin/env python3

# Q Compiler
# v0.1.0

import sys

insts = {}
insts["load"] = 1
insts["load_ram"] = 2
insts["store"] = 3
insts["store_reg"] = 4

def lex(text):
	cap = ""
	tokens = []

	in_comment = False

	for char in text:
		if char == ";": in_comment = True

		if char.isspace() or char in ",":
			if not cap.isspace() and not in_comment and cap != "":
				tokens.append(cap)
			cap = ""
		else:
			cap += char

		if char == "\n": in_comment = False
	
	return tokens

def get_type(loc):
	if loc in "abcd":
		# Register.
		return "reg"
	elif loc.isdigit():
		return "value"
	elif loc[0] in "$:":
		return "addr"
	
	return "label"

def get(loc):
	t = get_type(loc)

	if t == "reg":
		return "abcd".find(loc)
	elif t == "addr":
		if loc.replace("$", "") in "abcd":
			return "abcd".find(loc.replace("$", ""))
		else:
			return int(loc.replace("$", "").replace(":", ""))

	return int(loc)

def parse(tokens):
	instructions = []

	jumps = {}

	jump_list = [
		"jmp", "je",  "jne",
		"jg",  "jge", "jl",
		"jle"
	]

	# We do a double-passthrough to find jump markers and log them.
	for i in range(len(tokens)):
		t = tokens[i]

		if t[-1] == ":":
			# It's a label. Store it.
			jumps[t[:-1]] = i

	for i in range(len(tokens)):
		t = tokens[i]

		if t == "mov":
			loc1 = tokens[i + 1]
			loc2 = tokens[i + 2]

			loc1_type = get_type(loc1)
			loc2_type = get_type(loc2)

			if loc1_type == "value":
				if loc2_type == "reg":
					instructions.extend([1, get(loc2), get(loc1)])
				if loc2_type == "addr":
					instructions.extend([3, get(loc1), get(loc2)])
			if loc1_type == "reg":
				if loc2_type == "addr":
					if loc2.replace("$", "") in "abcd":
						instructions.extend([5, get(loc1), get(loc2)])
					else:
						instructions.extend([4, get(loc1), get(loc2)])

			if loc1_type == "addr":
				if loc2_type == "reg":
					instructions.extend([2, get(loc2), get(loc1)])
		elif t == "add":
			reg1 = tokens[i + 1]
			reg2 = tokens[i + 2]
			instructions.extend([8, get(reg1), get(reg2)])
		elif t == "sub":
			reg1 = tokens[i + 1]
			reg2 = tokens[i + 2]
			instructions.extend([9, get(reg1), get(reg2)])
		elif t == "mul":
			reg1 = tokens[i + 1]
			reg2 = tokens[i + 2]
			instructions.extend([10, get(reg1), get(reg2)])
		elif t == "div":
			reg1 = tokens[i + 1]
			reg2 = tokens[i + 2]
			instructions.extend([11, get(reg1), get(reg2)])
		elif t == "cmp":
			reg1 = tokens[i + 1]
			reg2 = tokens[i + 2]
			instructions.extend([16, get(reg1), get(reg2)])
		elif t in jump_list:
			loc = tokens[i + 1]
			_type = get_type(loc)

			addr = jumps[loc]

			instructions.extend([17 + jump_list.index(t), addr - i])
		elif t == "dbg":
			instructions.extend([26])

	return instructions

if __name__ == "__main__":
	if len(sys.argv) <= 1:
		print("Please pass a file to compile.")
		sys.exit(1)
	
	# Load the file into a string.
	text = open(sys.argv[1]).read()

	tokens = lex(text)

	print(tokens)

	instructions = parse(tokens)

	print(instructions)

	inst = []
	for i in instructions:
		inst.append(str(i))

	open(sys.argv[2], "w").write(" ".join(inst))

