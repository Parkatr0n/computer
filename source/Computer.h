
#ifndef COMPUTER_H
#define COMPUTER_H

class Computer {
public:
	int* ram;
	int registers[4];
	int flag;

	int ram_size;

	int i_head;
	int instruction;

	Computer(int ram_size);

	void step();
};

Computer::Computer(int ram_size) {
	this->ram_size = ram_size;
	
	// Resize the ram.
	// calloc() guarantees that the RAM is zeroed out.
	ram = calloc(ram_size, sizeof(int));

	// We have to zero out the registers to make sure 
	//  that nothing crazy happens.
	registers = { 0, 0, 0, 0 };
}

int sign(int x) {
	if (x < 0) return -1;
	if (x > 0) return 1;
	return 0;
}

void Computer::step() {
	instruction = ram[i_head];

	// Run the instruction.
	// We use a switch-case here, because AFAIK it's
	//  just a little bit faster, and looks really
	//  nice.
	switch (instruction) {
	case 0x0: // NONE
	case 0x1: // LOAD REG VALUE
		int REG = ram[++i_head];
		int VALUE = ram[++i_head];
		registers[REG] = VALUE;
		break;
	case 0x2: // LOAD_RAM REG ADDR
		int REG = ram[++i_head];
		int ADDR = ram[++i_head];
		registers[REG] = ram[ADDR];
		break;
	case 0x3: // STORE VALUE ADDR
		int VALUE = ram[++i_head];
		int ADDR = ram[++i_head];
		ram[ADDR] = VALUE;
		break;
	case 0x4: // STORE_REG REG ADDR
		int REG = ram[++i_head];
		int ADDR = ram[++i_head];
		ram[ADDR] = registers[REG];
		break;
	
	case 0x8: // ADD REG REG
		int REG1 = ram[++i_head];
		int REG2 = ram[++i_head];
		registers[REG1] += registers[REG2];
		break;
	case 0x9: // SUB REG REG
		int REG1 = ram[++i_head];
		int REG2 = ram[++i_head];
		registers[REG1] -= registers[REG2];
		break;
	case 0xA: // MUL REG REG
		int REG1 = ram[++i_head];
		int REG2 = ram[++i_head];
		registers[REG1] *= registers[REG2];
		break;
	case 0xB: // DIV REG REG
		int REG1 = ram[++i_head];
		int REG2 = ram[++i_head];
		registers[REG1] /= registers[REG2];
		break;
	
	case 0x10: // CMP REG REG
		int REG1 = ram[++i_head];
		int REG2 = ram[++i_head];
		flag = sign(registers[REG1] - registers[REG2]);
		break;
	case 0x11: // JMP ADDR
		int ADDR = ram[++i_head];
		i_head = ADDR - 1;
		break;
	case 0x12: // JE  ADDR
	case 0x13: // JN  ADDR
	case 0x14: // JGP ADDR
	case 0x15: // JGE ADDR
	case 0x16: // JL  ADDR
	case 0x17: // JLE ADDR
	}

	i_head++
}

#endif

