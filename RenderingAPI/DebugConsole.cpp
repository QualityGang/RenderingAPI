#include "DebugConsole.h"

#include <algorithm>
#include <sstream>

#include "TemplateUtils.h"
#include "Window.h"
#include "Throw.h"

std::vector<DebugConsoleCommand*> DebugConsole::CommandList;
std::vector<DebugConsole::ConsoleLine> DebugConsole::LineList;

std::string DebugConsole::InputLine;

bool DebugConsole::Open    = false;

const Color DebugConsole::InputColor(127, 127, 255, 255);
const Color DebugConsole::NormalColor(255, 255, 255, 255);
const Color DebugConsole::ErrorColor(255, 127, 127, 255);
const Color DebugConsole::SuccessColor(127, 255, 127, 255);
const Color DebugConsole::SpecialColor(255, 255, 127, 255);
const Color DebugConsole::BackColor(0, 13, 0, 204);
const Color DebugConsole::BorderColor(0, 0, 0, 127);

void DebugConsole::DebugConsoleStatic()
{
	//static_constructor<DebugConsoleStatic>::c;


}

void DebugConsole::Draw(GraphicsContext *context, const Window &window, SpriteBatch &batch, hTexture2D background, const FontAtlas &atlas)
{
	if (!Open)
		return;

	TextureSize bgSize;
	context->getTexture2DSize(background, &bgSize);

	float w = (float)window.getSize().x;
	float h = (float)window.getSize().y;

	float posX = 0;
	float posY = 0;
	float sizeX = w;
	float sizeY = h / 4;

	float bottomPosX = posX + 0;
	float bottomPosY = sizeY;
	float bottomSizeX = sizeX;
	float bottomSizeY = 5;

	Text text;
	text.setString(InputLine.c_str());
	text.setColor(InputColor);

	float textW = 0, textH = 0;
	atlas.getTextSize(text, &textW, &textH);

	text.setPosition(20, sizeY - textH - posY - 5);

	Sprite sprite;
	sprite.setPosition(posX, posY);
	sprite.setSize(sizeX, sizeY);
	sprite.setSrcRect(FloatRect(0, 0, (float)bgSize.width, (float)bgSize.height));
	sprite.setColor(BackColor);
	sprite.setTexture(background);

	batch.begin(window.getRenderTarget());

	batch.draw(sprite);

	sprite.setPosition(bottomPosX, bottomPosY);
	sprite.setSize(bottomSizeX, bottomSizeY);
	sprite.setColor(BorderColor);

	batch.draw(sprite);

	batch.draw(text, atlas);

	//for (const ConsoleLine &line : LineList)
	//{
	//	if (line.text.empty())
	//		continue;

	//	sprite.setColor(line.color);
	//	
	//	text.setPosition(text.getX(), text.getY() - textH - 5);
	//	text.setString(line.text.c_str());
	//	batch.draw(text, atlas);

	//	atlas.getTextSize(text, &textW, &textH);
	//}

	batch.end();
}

DebugConsoleCommand* DebugConsole::GetCommand(const char *name)
{
	auto iter = std::find_if(CommandList.begin(), CommandList.end(),
	[&](DebugConsoleCommand *command) -> bool
	{
		return !strcmp(name, command->getName());
	});

	return *iter;
}

void DebugConsole::AddCommand(DebugConsoleCommand &command)
{
	CommandList.push_back(&command);
}

void DebugConsole::RemoveCommand(const DebugConsoleCommand &command)
{
	CommandList.erase(std::remove_if(CommandList.begin(), CommandList.end(),
	[&](DebugConsoleCommand *cmd)
	{
		return cmd->getName() == command.getName();
	}), CommandList.end());
}

void DebugConsole::Save()
{

}

void DebugConsole::Load()
{

}

void DebugConsole::AddLine(const char *line, const Color &color)
{
	LineList.push_back(ConsoleLine(line, color));
}

void DebugConsole::ProcessInputLine(const char *inputLine)
{
	std::stringstream ss(inputLine);

	std::string name;
	ss >> name;

	DebugConsoleCommand *command = GetCommand(name.c_str());
	DebugConsoleCommand::Type type = command->getType();

	// Remove whitespace at the beginning of the string stream
	while (true)
	{
		int c = ss.peek();

		if (c == ' ' || c == '=' || c == '(' || c == ')' || c == ',' || c == '\t')
			ss.get();
		else
			break;
	}

	void *value = command->getValuePtr();
	bool error = false;

	switch (type)
	{
		case DebugConsoleCommand::Type_Bool:
			ss >> *(bool*)value;
			break;
		case DebugConsoleCommand::Type_Int32:
			ss >> *(int32_t*)value;
			break;
		case DebugConsoleCommand::Type_UInt32:
			ss >> *(uint32_t*)value;
			break;
		case DebugConsoleCommand::Type_Float:
			ss >> *(float*)value;
			break;
		case DebugConsoleCommand::Type_Double:
			ss >> *(double*)value;
			break;
		case DebugConsoleCommand::Type_String:
		{
			std::string buffer;
			std::getline(ss, buffer);

			if (!buffer.empty())
			{
				uint32_t startPos = 0;

				if (buffer[0] == '\"' && buffer[buffer.size() - 1] == '\"')
				{
					buffer[buffer.size() - 1] = '\0';
					startPos++;
				}

				char *string = (char*)value;
				memcpy(string, &buffer[startPos], 255);
			}
		} break;
		case DebugConsoleCommand::Type_Function:
		{
			std::string buffer;
			std::getline(ss, buffer);
			((DebugConsoleCommand::CallbackFunction)(value))(buffer.c_str());
		} break;
		default:
			error = true;
			break;
	}

	std::string output;

	if (type != DebugConsoleCommand::Type_Function)
		output = CommandToString(command);

	if (error)
		AddLine(output.c_str(), ErrorColor);
	else if (type != DebugConsoleCommand::Type_Function)
		AddLine(output.c_str(), SuccessColor);
}

void DebugConsole::OnKey(uint32_t key)
{
	if (VkKeyScan(key) == Key::Oem3)
	{
		Open = !Open;
		return;
	}

	if (Open)
	{
		switch (key)
		{
			case Key::Back:
				if (!InputLine.empty())
					InputLine.erase(InputLine.end() - 1);
				break;

			case 0x0A:
				break;

			case Key::Escape:
					
				break;

			case Key::Tab:

				break;

			case Key::Enter:
				if (!InputLine.empty())
				{
					ProcessInputLine(InputLine.c_str());
					AddLine(InputLine.c_str(), InputColor);
					InputLine.clear();
				}
				break;

			default:
				InputLine += (char)key;
				break;
		}
	}
}

std::string DebugConsole::CommandToString(DebugConsoleCommand *command)
{
	std::string name = command->getName();
	void *value = command->getValuePtr();
	
	name += " = ";

	switch (command->getType())
	{
		case DebugConsoleCommand::Type_Bool:
			return name + std::to_string(*(bool*)value);

		case DebugConsoleCommand::Type_Int32:
			return name + std::to_string(*(int32_t*)value);

		case DebugConsoleCommand::Type_UInt32:
			return name + std::to_string(*(uint32_t*)value);

		case DebugConsoleCommand::Type_Float:
			return name + std::to_string(*(float*)value);

		case DebugConsoleCommand::Type_Double:
			return name + std::to_string(*(double*)value);

		case DebugConsoleCommand::Type_String:
			return name + "\"" + std::string((char*)value) + "\"";

		case DebugConsoleCommand::Type_Function:
			return name;
	}

	THROW("Unknown command");
}
