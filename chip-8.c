#include "chip-8.h"
#include "graphics.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

Chip8 chip;
struct tm *timeinfo;
#define MEM_START 0x200
#define FONT_SIZE 80
#define FONT_START 0x50
#define FONT_END 0x0A0
#define MEM_END 0xFFF

uint8_t randomB() {
	return rand() % 255;
}



uint8_t fontset[] =
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


void ADDXB() {
	chip.regs[getVX()] += chip.regs[getByte()];
}

void LDXY() {
	chip.regs[getVX()] = chip.regs[getVY()];
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


void DRWXYN() {
	uint8_t Vx = (chip.opcode & 0x0F00u) >> 8;
	uint8_t Vy = (chip.opcode & 0x00F0u) >> 4;
	uint8_t height = chip.opcode & 0x000F;
	// Wrap if going beyond screen boundaries
	uint8_t xPos = chip.regs[Vx] % VIDEO_WIDTH;
	uint8_t yPos = chip.regs[Vy] % VIDEO_HEIGHT;

	chip.regs[0xF] = 0;

	for (unsigned int row = 0; row < height; ++row)
	{
		uint8_t spriteByte = chip.memory[chip.index + row];

		for (unsigned int col = 0; col < 8; ++col)
		{
			uint8_t spritePixel = spriteByte & (0x80u >> col);
			uint32_t* screenPixel = &chip.video[(yPos + row) * VIDEO_WIDTH + (xPos + col)];

			// Sprite pixel is on
			if (spritePixel)
			{
				// Screen pixel also on - collision
				if (*screenPixel == 0xFFFFFFFF)
				{
					chip.regs[0xF] = 1;
				}

				// Effectively XOR with the sprite pixel
				*screenPixel ^= 0xFFFFFFFF;
			}
		}
	}


}


void SKPX() {
	if(chip.keys[chip.regs[getVX()]]) {
		chip.pc+=2;
	}
}


void SKNPX() {
	if(!chip.keys[chip.regs[getVX()]]) {
		chip.pc+=2;
	}
}


void LDXT() {
	chip.regs[getVX()] = chip.timer;
}

void LDXK() {
	int isDone = 0;
	for(int i=0; i<16; i++) {
		if(chip.regs[i]) {
			chip.regs[getVX()] = i;
			isDone = 1;	
		}
	}
	if(!isDone) {		
		chip.pc -= 2;
	}
}

void LDTX() {
	chip.timer = chip.regs[getVX()];
}


void LDSX() {
	chip.soundTimer = chip.regs[getVX()];
}

void ADDIX() {
	chip.index = chip.index + chip.regs[getVX()];
}

void LDFX() {
	uint16_t Vx = getVX();
	uint8_t digit = chip.regs[Vx];

	chip.index = FONT_START + (5 * digit);
}


void LDBX() {
	uint16_t val = chip.regs[getVX()];

	chip.memory[chip.index + 2] = val % 10;
	val /= 10;
	chip.memory[chip.index + 1] = val % 10;
	val /=10;
	chip.memory[chip.index] = val % 10;
}

void LDIX() {
	uint16_t n = getVX();
	for(int i=0; i<=n; i++) {
		chip.memory[chip.index + i] = chip.regs[i];
	}
}


void LDXI() {
	uint8_t Vx = (chip.opcode & 0x0F00u) >> 8u;
	for (uint8_t i = 0; i <= Vx; ++i)
	{
		chip.regs[i] = chip.memory[chip.index + i];
	}
}



void decodeAndExe(uint16_t code) {
	switch((code & 0xF000u) >> 12u) {
		
		case 1:
			JP();
			break;
		case 2:
			CALL();
			break;
		case 3:
			SEXB();
			break;
		case 4:
			SNEXB();
			break;
		case 5:
			SEXY();
			break;
		case 6:
			LDXB();
			break;
		case 7:
			ADDXB();
			break;
		case 0xA:
			LDIA();
			break;
		case 0xB:
			JPVA();
			break;
		case 0xC:
			RNDXB();
			break;
		case 0xD:
			DRWXYN();
			break;
		

		case 0: {
				if((code & 0x000F) == 0) {
						CLS(); break;
				} else {
						RET(); break;					
				}
		}
		break;	


		case 8: {
			switch(code & 0x000F) {
				case 0:
					LDXY();
					break;
				case 1:
					ORXY();
				case 2:
					ANDXY();
				case 3:
					XORXY();
					break;
				case 4:
					ADDXY();
					break;
				case 5:
					SUBXY();
					break;
				case 6:
					SHRX();
					break;
				case 7:
					SUBNXY();
					break;
				case 0xE:
					SHLX();
					break;
			}
		}
		break;
		case 0xE: {
			printf("lasdf");
			switch(code & 0x000F) {
				case 0xE:
					SKPX();
					break;
				case 1:
					SKNPX();
					break;
			}
		}
		break;
		case 0xF: {
			switch(code & 0x00FF) {
				case 0x07:
					LDXT();
					break;
				case 0x0A:
					LDXK();
					break;
				case 0x15:
					LDTX();
					break;
				case 0x18:
					LDSX();
					break;
				case 0x1E:
					ADDIX();
					break;
				case 0x29:
					LDFX();
					break;
				case 0x33:
					LDBX();
					break;
				case 0x55:
					LDIX();
					break;
				case 0x65:
					LDXI();
					break;
			}
			break;
		}
		default:
			printf("Unknown Instruction");
			break;
	}
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


uint16_t fetch() {
	return (chip.memory[chip.pc] << 8) | (chip.memory[chip.pc+1]);
}



void initChip(char *file, int scale, int delay) {
	chip.pc = MEM_START;	
	int pitch = sizeof(chip.video[0]) * VIDEO_WIDTH;
	
	if(!initGraphics("Chip-8 Emu", VIDEO_WIDTH * scale, VIDEO_HEIGHT * scale, VIDEO_WIDTH, VIDEO_HEIGHT)) {
		exit(0);	
	}
	
	int EXIT = 0;
	loadRomToMemory(file);
	while(!EXIT) {
		EXIT = input(chip.keys);	
		chip.opcode = fetch();
		chip.pc += 2;	
		sleep(1);
		decodeAndExe(chip.opcode);
				
		if(chip.timer > 0) {
			chip.timer--;
		} 
		
		if(chip.soundTimer > 0) {
			chip.soundTimer--;	
		}

		SDL_UPDATE(chip.video, pitch); 
	}
	return;
}
