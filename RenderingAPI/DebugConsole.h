#pragma once

#include "DebugConsoleCommand.h"
#include "SpriteBatch.h"

#define ConsoleCommand(var, name) static DebugConsoleCommand name##_CONSOLE_COMMAND(var, #name)

class DebugConsole
{
public:
	DLL_REN_API static void Draw(GraphicsContext *context, const Window &window, SpriteBatch &batch, hTexture2D background, const FontAtlas &atlas);
	 
	DLL_REN_API static DebugConsoleCommand* GetCommand(const char *name);
	DLL_REN_API static void AddCommand(DebugConsoleCommand &command);
	DLL_REN_API static void RemoveCommand(const DebugConsoleCommand &command);
	DLL_REN_API static void Save();
	DLL_REN_API static void Load();
	 
	DLL_REN_API static void AddLine(const char *line = "", const Color &color = NormalColor);
	DLL_REN_API static void ProcessInputLine(const char *inputLine);

	DLL_REN_API static void OnKey(uint32_t key);

	DLL_REN_API static bool IsOpen() { return Open; }
private:
	DebugConsole() = delete;
	~DebugConsole() = delete;

	struct ConsoleLine
	{
		ConsoleLine(const std::string &text, const Color &color)
			: text(text), color(color) {}

		std::string text;
		Color color;
	};

	static void DebugConsoleStatic();

	static std::string CommandToString(DebugConsoleCommand *command);

	static std::vector<DebugConsoleCommand*> CommandList;
	static std::vector<ConsoleLine> LineList;

	static std::string InputLine;

	static bool Open;

	static const Color InputColor;
	static const Color NormalColor;
	static const Color ErrorColor;
	static const Color SuccessColor;
	static const Color SpecialColor;
	static const Color BackColor;
	static const Color BorderColor;
};

