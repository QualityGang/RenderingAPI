#include "Chip8.h"

#include <Windows.h>
#include <thread>
#include <fstream>
#include <random>

static unsigned char fontset[] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

static inline bool IsWindowsVersionOrGreater(DWORD majorVersion, DWORD minorVersion, WORD servicePackMajor)
{
	OSVERSIONINFOEX osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	ULONGLONG dwlConditionMask = VerSetConditionMask(
		VerSetConditionMask(
			VerSetConditionMask(
				0, VER_MAJORVERSION, VER_GREATER_EQUAL
			),
			VER_MINORVERSION, VER_GREATER_EQUAL
		),
		VER_SERVICEPACKMAJOR, VER_GREATER_EQUAL
	);

	osvi.dwMajorVersion    = majorVersion;
	osvi.dwMinorVersion    = minorVersion;
	osvi.wServicePackMajor = servicePackMajor;

	return VerifyVersionInfo(&osvi, VER_MAJORVERSION | VER_MINORVERSION | VER_SERVICEPACKMAJOR, dwlConditionMask) != FALSE;
}

static inline bool IsWindows7OrGreater()
{
	return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WIN7), LOBYTE(_WIN32_WINNT_WIN7), 0);
}

Chip8::Chip8()
{
	for (int i = 0; i < 80; i++)
		memory[i] = fontset[i];
}

Chip8::~Chip8()
{
}

void Chip8::load(const char *rom)
{
	std::ifstream is(rom, std::ios::binary);

	if (!is)
		return; //throw std::exception();

	is.seekg(0, is.end);
	std::streampos length = is.tellg();
	is.seekg(0, is.beg);

	if (length.seekpos() == -1)
		return;

	char *buffer = new char[(unsigned int)length.seekpos()] { 0 };
	is.read(buffer, length);

	//if (!is)
	//	return; //throw std::exception();

	//is.close();

	if (length.seekpos() + 512 > 4096)
		return;

	for (int i = 0; i < length; i++)
		memory[i + 512] = buffer[i];
}

void Chip8::cycle()
{
	// fetch opcode
	unsigned short opcode = memory[pc] << 8 | memory[pc + 1];

	switch (opcode & 0xF000)
	{
		case 0x0000:
			switch (opcode & 0x000F)
			{
				// 00E0 -> clear the screen
				case 0x0000:
					for (int i = 0; i < 64 * 32; i++)
						gfx[i] = 0;

					pc += 2;
					break;

				// 00EE -> return from subroutine
				case 0x000E:
					pc = stack[sp];
					sp--;
					pc += 2;
					break;
			}
			break;

		// 1NNN -> jump to location NNN
		case 0x1000:
			pc = opcode & 0x0FFF;
			break;

		// 2NNN -> call subroutine at NNN
		case 0x2000:
			sp++;
			stack[sp] = pc;
			pc = opcode & 0x0FFF;
			break;

		// 3XKK -> skip next instruction if Vx == KK
		case 0x3000:
			if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
				pc += 4;
			else
				pc += 2;
			break;

		// 4XKK -> skip next instruction if Vx != KK
		case 0x4000:
			if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
				pc += 4;
			else
				pc += 2;
			break;

		// 5XY0 -> skip next instruction if Vx == Vy
		case 0x5000:
			if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
				pc += 4;
			else
				pc += 2;
			break;

		// 6XKK -> set Vx = KK
		case 0x6000:
			V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
			pc += 2;
			break;

		// 7XKK -> set Vx = Vx + kk
		case 0x7000:
			V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
			pc += 2;
			break;

		case 0x8000:
			switch (opcode & 0x000F)
			{
				// 8XY0 -> set Vx = Vy
				case 0x0000:
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
					pc += 2;
					break;

				// 8XY1 -> set Vx = Vx OR Vy
				case 0x0001:
					V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
					pc += 2;
					break;

				// 8XY2 -> set Vx = Vx AND Vy
				case 0x0002:
					V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
					pc += 2;
					break;

				// 8XY3 -> set Vx = Vx XOR Vy
				case 0x0003:
					V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4];
					pc += 2;
					break;

				// 8XY4 -> set Vx = Vx + Vy; set VF = carry
				// if the result is greater than 8 bits (i.e., > 255,) VF is set to 1, otherwise 0
				case 0x0004:
					if (V[(opcode & 0x00F0) >> 4] + V[(opcode & 0x0F00) >> 8] > 0xFF)
						V[0xF] = 1; // carry
					else
						V[0xF] = 0;
					V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
					pc += 2;
					break;

				// 8XY5 -> set Vx = Vx - Vy; set VF = NOT borrow
				// if Vx > Vy, then VF is set to 1, otherwise 0
				case 0x0005:
					if (V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4])
						V[0xF] = 1;
					else
						V[0xF] = 0;
					V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
					pc += 2;
					break;

				// 8XY6 -> set Vx = Vx SHR 1
				// if the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0
				case 0x0006:
					V[0xF] = V[(opcode & 0x0F00) >> 8] & 0x1;
					V[(opcode & 0x0F00) >> 8] >>= 1;
					pc += 2;
					break;

				// 8XY7 -> set Vx = Vy - Vx; set VF = NOT borrow
				// If Vy > Vx, then VF is set to 1, otherwise 0
				case 0x0007:
					if (V[(opcode & 0x00F0) >> 4] > V[(opcode & 0x0F00) >> 8])
						V[0xF] = 1;
					else
						V[0xF] = 0;
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
					pc += 2;
					break;

				// 8XYE -> set Vx = Vx SHL 1
				// if the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0
				case 0x000E:
					V[0xF] = V[(opcode & 0x0F00) >> 8] >> 7;
					V[(opcode & 0x0F00) >> 8] <<= 1;
					pc += 2;
					break;
			}
			break;

		// 9XY0 -> skip next instruction if Vx != Vy
		case 0x9000:
			if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4])
				pc += 4;
			else
				pc += 2;
			break;

		// ANNN -> set I = NNN
		case 0xA000:
			I = opcode & 0x0FFF;
			pc += 2;
			break;

		// BNNN -> jump to location NNN + V0
		case 0xB000:
			pc = opcode & 0x0FFF + V[0];
			break;

		// CXKK -> set Vx = random byte AND kk
		case 0xC000:
		{
			std::random_device rd;
			std::mt19937 eng(rd());
			std::uniform_int_distribution<> dist(0, 255);

			int random = dist(eng);

			V[(opcode & 0x0F00) >> 8] = random & (opcode & 0x00FF);
			pc += 2;
		} break;

		// DXYN -> display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision
		// sprites are XORed onto the existing screen
		// if this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0
		// if the sprite is positioned so part of it is outside the coordinates of the display, it wraps around to the opposite side of the screen
		case 0xD000:
		{
			unsigned short x = V[(opcode & 0x0F00) >> 8];
			unsigned short y = V[(opcode & 0x00F0) >> 4];
			unsigned short height = opcode & 0x000F;

			V[0xF] = 0;

			for (int iy = 0; iy < height; iy++)
			{
				unsigned short pixel = memory[I + iy];

				for (int ix = 0; ix < 8; ix++)
				{
					if ((pixel & (0x80 >> ix)) != 0)
					{
						if (gfx[(x + ix + ((y + iy) * 64))] == 1)
							V[0xF] = 1;

						gfx[x + ix + ((y + iy) * 64)] ^= 1;
					}
				}
			}

			pc += 2;
		} break;

		case 0xE000:
			switch (opcode & 0x00FF)
			{
				// EX9E -> skip next instruction if key with the value of Vx is pressed
				case 0x009E:
					if (key[V[(opcode & 0x0F00) >> 8]])
						pc += 4;
					else
						pc += 2;
					break;


				// EXA1 -> skip next instruction if key with the value of Vx is not pressed
				case 0x00A1:
					if (!key[V[(opcode & 0x0F00) >> 8]])
						pc += 4;
					else
						pc += 2;
					break;
			}
			break;

		case 0xF000:
			switch (opcode & 0x00FF)
			{
				// FX07 -> set Vx = delay timer value
				case 0x0007:
					V[(opcode & 0x0F00) >> 8] = delay_timer;
					pc += 2;
					break;

				// FX0A -> wait for a key press, store the value of the key in Vx
				case 0x000A:
				{
					bool keyPressed = false;

					for (int i = 0; i < 16; i++)
					{
						if (key[i] != 0)
						{
							V[(opcode & 0x0F00) >> 8] = i;
							keyPressed = true;
						}
					}

					// run the same instruction on the next cycle
					if (!keyPressed)
						return;

					pc += 2;
				} break;

				// FX15 -> set delay timer = Vx
				case 0x0015:
					delay_timer = V[(opcode & 0x0F00) >> 8];
					pc += 2;
					break;

				// FX18 -> set sound timer = Vx
				case 0x0018:
					sound_timer = V[(opcode & 0x0F00) >> 8];
					pc += 2;
					break;

				// FX1E -> set I = I + Vx
				// F is set to 1 when range overflow (I + VX > 0xFFF), and 0 when there isn't
				case 0x001E:
					if (I + V[(opcode & 0x0F00) >> 8] > 0xFFF)
						V[0xF] = 1;
					else
						V[0xF] = 0;

					I += V[(opcode & 0x0F00) >> 8];
					pc += 2;
					break;

				// FX29 -> set I = location of sprite for digit Vx
				// represented by a 4x5 font
				case 0x0029:
					I = V[(opcode & 0x0F00) >> 8] * 0x5;
					pc += 2;
					break;

				// FX33 -> store Binary-coded decimal representation of Vx in memory locations I, I + 1 and I + 2
				case 0x0033:
					memory[I + 0] =  V[(opcode & 0x0F00) >> 8] / 100;
					memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10)  % 10;
					memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;
					pc += 2;
					break;

				// FX55 -> store registers V0 through Vx in memory starting at location I
				case 0x0055:
					for (int i = 0; i <= ((opcode & 0x0F00) >> 8); i++)
						memory[I + i] = V[i];

					I += ((opcode & 0x0F00) >> 8) + 1;
					pc += 2;
					break;

				// FX65 -> read registers V0 through Vx from memory starting at location I
				case 0x0065:
					for (int i = 0; i <= ((opcode & 0x0F00) >> 8); ++i)
						V[i] = memory[I + i];
					
					I += ((opcode & 0x0F00) >> 8) + 1;
					pc += 2;
					break;
			}
			break;
	}

	if (delay_timer > 0)
		delay_timer--;

	if (sound_timer > 0)
	{
		sound_timer--;

		if (sound_timer == 0)
		{
			if (IsWindows7OrGreater())
				Beep(3000, 250);
			else
				MessageBeep(0xFFFFFFFF);
		}
	}
}
