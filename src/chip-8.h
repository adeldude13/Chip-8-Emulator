#ifndef CHIP8
#define CHIP8 

#include <inttypes.h>

#define VIDEO_SIZE 64*32
#define VIDEO_WIDTH 64
#define VIDEO_HEIGHT 32

typedef struct {
	uint8_t regs[16];
	uint8_t memory[4096];
	uint16_t index;
	uint16_t pc;
	uint16_t stack[16];
	uint8_t sp;
	uint8_t timer;
	uint8_t soundTimer;
	uint8_t keys[16];
	uint32_t video[VIDEO_HEIGHT*VIDEO_WIDTH];
	uint16_t opcode;
} Chip8;

void loadRomToMemory(char *file);
void initChip(char *file, int scale);


#endif
