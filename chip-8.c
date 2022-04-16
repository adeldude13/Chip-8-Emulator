#include "chip-8.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


Chip8 chip;

#define MEM_START 0x200
#define FONT_SIZE 80
#define FONT_START 0x50
#define FONT_END 0x0A0


uint8_t randomB() {
	return rand() % 255;
}



uint8_t fontset[FONT_SIZE] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0,
	0x20, 0x60, 0x20, 0x20, 0x70,
	0xF0, 0x10, 0xF0, 0x80, 0xF0,
	0xF0, 0x10, 0xF0, 0x10, 0xF0,
	0x90, 0x90, 0xF0, 0x10, 0x10,
	0xF0, 0x80, 0xF0, 0x10, 0xF0,
	0xF0, 0x80, 0xF0, 0x90, 0xF0,
	0xF0, 0x10, 0x20, 0x40, 0x40,
	0xF0, 0x90, 0xF0, 0x90, 0xF0,
	0xF0, 0x90, 0xF0, 0x10, 0xF0,
	0xF0, 0x90, 0xF0, 0x90, 0x90,
	0xE0, 0x90, 0xE0, 0x90, 0xE0,
	0xF0, 0x80, 0x80, 0x80, 0xF0,
	0xE0, 0x90, 0x90, 0x90, 0xE0,
	0xF0, 0x80, 0xF0, 0x80, 0xF0,
	0xF0, 0x80, 0xF0, 0x80, 0x80
};


uint16_t getAdress() {
	return chip.opcode & 0x0FFFu;
}

uint16_t getByte() {
	return chip.opcode & 0x00FFu;
}

uint16_t getVX() {
	return (chip.opcode & 0x0F00u) >> 8;
}

uint16_t getVY() {
	return (chip.opcode & 0x00F0u) >> 4;
}


void initFont() {
	for(int i=0; i<FONT_SIZE; i++) {
		chip.memory[FONT_START+i] = fontset[i];
	}
}

void CLS() {
	for(int i=0;i<VIDEO_SIZE;i++) {
		chip.video[i] = 0;
	}
}


void RET() {
	chip.sp--;
	chip.pc = chip.stack[chip.sp];
	return;
}


void JP() {
	uint16_t addr = getAdress();
	chip.pc = addr;
}


void CALL() {
	uint16_t addr = getAdress();
	chip.stack[chip.sp++] = chip.pc;
	chip.pc = addr;
}


void SEXB() {
	uint16_t vx = getVX();
	uint16_t byte = getByte();

	if(chip.regs[vx] == byte) {
		chip.pc += 2;
	}
}

void SNEXB() {
	uint16_t vx = getVX();
	uint16_t byte = getByte();

	if(chip.regs[vx] != byte) {
		chip.pc += 2;
	}
}


void SEXY() {
	uint16_t vx = getVX();
	uint16_t vy = getVY();

	if(chip.regs[vx] == chip.regs[vy]) {
		chip.pc += 2;	
	}
}



void LDXB() {
	uint16_t vx = getVX();
	uint16_t vy = getVY();

	chip.regs[vx] = chip.regs[vy];
}

void ORXY() {
	uint16_t vx = getVX();
	uint16_t vy = getVY();

	chip.regs[vx] |= chip.regs[vy];
	
}


void ANDXY() {
	uint16_t vx = getVX();
	uint16_t vy = getVY();

	chip.regs[vx] &= chip.regs[vy];
}


void XORXY() {
	uint16_t vx = getVX();
	uint16_t vy = getVY();

	chip.regs[vx] ^= chip.regs[vy];
}



void ADDXY() {
	uint16_t vx = getVX();
	uint16_t vy = getVY();

	int val = chip.regs[vx] + chip.regs[vy];
	if(val > 255) {
		chip.regs[vx] = 255;
		chip.regs[0xF] = 1;
		return;
	}
	chip.regs[vx] = val;
	chip.regs[0xF] = 0;
}


void SUBXY() {
	uint16_t vx = getVX();
	uint16_t vy = getVY();
	if(chip.regs[vx] > chip.regs[vy]) {
		chip.regs[0xF] = 1;
	} else {
		chip.regs[0xF] = 0;
	}
	chip.regs[vx] -= chip.regs[vy];
}

void SHRX() {
	uint16_t vx = getVX();
	
	chip.regs[0xF] = (chip.regs[vx] & 0x1);

	chip.regs[vx] >>= 1;
}



void SUBNXY() {
	uint16_t vx = getVX();
	uint16_t vy = getVY();

	chip.regs[0xF] = chip.regs[vy] > chip.regs[vx];
	chip.regs[vx] = chip.regs[vy] - chip.regs[vx];
}


void SHLX() {
	uint16_t vx = getVX();
	chip.regs[0xF] = (chip.regs[vx] & 1u);
	chip.regs[vx] <<= 1;
}

void SNEXY() {
	uint16_t vx = getVX();
	uint16_t vy = getVY();

	if(chip.regs[vx] == chip.regs[vy]) {
		chip.pc += 2;
	}
}

void LDIA() {
	uint16_t nnn = getAdress();

	chip.index = nnn;
}

void JPVA() {
	uint16_t nnn = getAdress() + chip.regs[0x0];
	chip.pc = nnn;
}


void RNDXB() {
	chip.regs[getVX()] = randomB() & getByte();
}


void loadRomToMemory(char *file) {
	FILE *fp = fopen(file, "r");
	if(fp == NULL) {
		fprintf(stderr, "Failed to Read File %s", file);
		exit(0);
	}
	// get size of file	
	fseek(fp, 0L, SEEK_END);
	size_t fsize = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	
	// read file content into data varible
	char *data = malloc(sizeof(char)*fsize);
	fread(data, fsize, 1, fp);

	for(int i=0;i<fsize;i++) {
		chip.memory[MEM_START + i] = data[i];
	}
	

	fclose(fp);
	free(data);
	initFont();
}



void initChip(char *file) {
	chip.pc = MEM_START;	
	loadRomToMemory(file);
	return;
}
