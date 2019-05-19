
#ifndef COMPUTER_H
#define COMPUTER_H

#include <fstream>
#include <sstream>
#include <iostream>

#include <SFML/Graphics.hpp>

class Computer {
public:
	int* ram;
	int registers[4] = { 0, 0, 0, 0 };
	int flag;

	int ram_size;

	int i_head = 0;
	int instruction;

	Computer(int ram_size);

	void loadFromDrive();
	bool step(sf::RenderWindow& window);

	void keyboard(int code);
};

Computer::Computer(int ram_size) {
	this->ram_size = ram_size;
	
	// Resize the ram.
	// calloc() guarantees that the RAM is zeroed out.
	ram = (int*) calloc(ram_size, sizeof(int));
};

void Computer::keyboard(int code) {
	ram[0x201] = code;
}

int sign(int x) {
	if (x < 0) return -1;
	if (x > 0) return 1;
	return 0;
}

void Computer::loadFromDrive() {
	std::ifstream file("program", std::ios::in);

	int x;
	int i = 0;
	
	// Load instructions from the bootloader straight into RAM.
	// Bootloader starts at 0x0 and ends at 0x200.
	while (file >> x) {
		if (i >= 0x200) break;

		ram[i] = x;

		i++;
	}
}

bool Computer::step(sf::RenderWindow& window) {
	if (i_head > ram_size) return 1;

	instruction = ram[i_head];

	// Update RAM positions with information.
	sf::Vector2i mpos = sf::Mouse::getPosition(window);
	ram[515] = mpos.x;
	ram[516] = mpos.y;

	// Load Video Memory into a pixel array.
	int width = 400;
	int height = 400;

	sf::Uint8* pixels = new sf::Uint8[width * height * 4];

	sf::Texture texture;
	texture.create(width, height);
	
	sf::Sprite sprite(texture);

	for (register int i = 0x300; i < width * height * 4; i += 4) {
		pixels[i] = ram[i];
		pixels[i + 1] = ram[i + 1];
		pixels[i + 2] = ram[i + 2];
		pixels[i + 3] = ram[i + 3];
	}

	// Declare some storage variables.
	int REG;
	int REG1;
	int REG2;
	int VALUE;
	int ADDR;

	// Run the instruction.
	// We use a switch-case here, because AFAIK it's
	//  just a little bit faster, and looks really
	//  nice.
	switch (instruction) {
	case 0x0: // NONE
	case 0x1: // LOAD REG VALUE
		REG = ram[++i_head];
		VALUE = ram[++i_head];
		registers[REG] = VALUE;
		break;
	case 0x2: // LOAD_RAM REG ADDR
		REG = ram[++i_head];
		ADDR = ram[++i_head];
		registers[REG] = ram[ADDR];
		break;
	case 0x3: // STORE VALUE ADDR
		VALUE = ram[++i_head];
		ADDR = ram[++i_head];
		ram[ADDR] = VALUE;
		break;
	case 0x4: // STORE_REG REG ADDR
		REG = ram[++i_head];
		ADDR = ram[++i_head];
		ram[ADDR] = registers[REG];
		break;
	
	case 0x8: // ADD REG REG
		REG1 = ram[++i_head];
		REG2 = ram[++i_head];
		registers[REG1] += registers[REG2];
		break;
	case 0x9: // SUB REG REG
		REG1 = ram[++i_head];
		REG2 = ram[++i_head];
		registers[REG1] -= registers[REG2];
		break;
	case 0xA: // MUL REG REG
		REG1 = ram[++i_head];
		REG2 = ram[++i_head];
		registers[REG1] *= registers[REG2];
		break;
	case 0xB: // DIV REG REG
		REG1 = ram[++i_head];
		REG2 = ram[++i_head];
		registers[REG1] /= registers[REG2];
		break;
	
	case 0x10: // CMP REG REG
		REG1 = ram[++i_head];
		REG2 = ram[++i_head];
		flag = sign(registers[REG1] - registers[REG2]);
		break;
	case 0x11: // JMP ADDR
		ADDR = ram[++i_head];
		i_head += ADDR - 1;
		break;
	case 0x12: // JE  ADDR
		ADDR = ram[++i_head];
		if (flag == 0) i_head += ADDR - 1;
		break;
	case 0x13: // JNE ADDR
		ADDR = ram[++i_head];
		if (flag != 0) i_head += ADDR - 1;
		break;
	case 0x14: // JG  ADDR
		ADDR = ram[++i_head];
		if (flag == 1) i_head += ADDR - 1;
		break;
	case 0x15: // JGE ADDR
		ADDR = ram[++i_head];
		if (flag != -1) i_head += ADDR - 1;
		break;
	case 0x16: // JL  ADDR
		ADDR = ram[++i_head];
		if (flag == -1) i_head += ADDR - 1;
		break;
	case 0x17: // JLE ADDR
		ADDR = ram[++i_head];
		if (flag != 1) i_head += ADDR - 1;
		break;
	case 26: // DEBUG
		std::cout << registers[0] << " " << registers[1] << "\n";
		break;
	}

	i_head++;

	return 0;
}

#endif

