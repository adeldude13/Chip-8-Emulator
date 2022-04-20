1. Components:
	- 16 8bit regs
	- 4k Bytes Of Memory(RAM)
		* 0x000-0x1FF (0-511) :- Emulator Interpreter.
		 	- 0x050-0x0A0 (80-160) :- Storage for 0-F characters, hard coded.
		* 0x200-0xFFF (512-4095) :- ROMS Instructions.
	- 16Bit Reg: memory Adress for the currently used Operation.
	- 16Bit Program Counter(AKA PC) : keeping track for the next Operation.
	- 16bit 16 Levels Stack: used For calling function.
	- 8Bit Stack Pointer: keeping Track for the Current function in the Top of the Stack.
	- 8Bit Delay Timer(decremented at 60HZ).
	- 8Bit Sound Timer: Beebing while its non-zero
	- 16 Input Keys (0-F): mapped to
	 				Keypad       Keyboard
				+-+-+-+-+    +-+-+-+-+
				|1|2|3|C|    |1|2|3|4|
				+-+-+-+-+    +-+-+-+-+
				|4|5|6|D|    |Q|W|E|R|
				+-+-+-+-+ == +-+-+-+-+
				|7|8|9|E|    |A|S|D|F|
				+-+-+-+-+    +-+-+-+-+
				|A|0|B|F|    |Z|X|C|V|
				+-+-+-+-+    +-+-+-+-+
	- (64x32) Display Pixels: used to disply games roms at certian pixels
