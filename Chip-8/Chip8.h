#pragma once

class Chip8
{
public:
	Chip8();
	~Chip8();

	void load(const char *rom);
	void cycle();

	void getDisplay(char gfx[64 * 32]) const
	{
		for (int i = 0; i < 64 * 32; i++)
			gfx[i] = this->gfx[i];
	}
	
	void setKeyPressed(short key, bool pressed) { this->key[key] = pressed; }
private:
	unsigned char memory[4096] = { 0 };
	unsigned short pc = 0x200;

	unsigned char V[16] = { 0 };
	unsigned short int I = 0;

	unsigned char delay_timer = 0;
	unsigned char sound_timer = 0;

	unsigned char gfx[64 * 32] = { 0 };

	unsigned short stack[16] = { 0 };
	unsigned short sp = 0;

	bool key[16] = { 0 };
};

