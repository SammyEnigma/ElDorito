#include "GameConsole.h"
#include "../Utils/VersionInfo.h"
#include "../CommandMap.h"
#include <sstream>

// TODO: why does pressing shift or caps lock break keyboard input?
// TODO: why is all input in capital letters?

void writeToMemory(uint8_t* const destination, const uint8_t bytesToPatch[], const size_t numOfBytes) { // TODO: use eldewrito method instead of this
	DWORD origProtect;
	VirtualProtect(destination, numOfBytes, PAGE_EXECUTE_READWRITE, &origProtect);
	memcpy(destination, bytesToPatch, numOfBytes);
	VirtualProtect(destination, numOfBytes, origProtect, 0);
}

void GameConsole::startIRCBackend()
{
	GameConsole::getInstance().ircBackend = std::make_unique<IRCBackend>();
}

GameConsole::GameConsole()
{
	for (int i = 0; i < numOfLines; i++) {
		queue.push_back("");
	}

	initPlayerName();
	DirectXHook::hookDirectX();
	KeyboardHook::setHook();
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&startIRCBackend, 0, 0, 0);

	pushLineFromGameToUI("ElDewrito Version: " + Utils::Version::GetVersionString() + " Build Date: " + __DATE__ + " " + __TIME__);
	pushLineFromGameToUI("Enter /help or /help <command> to get started!");
}

bool GameConsole::isConsoleShown() {
	return boolShowConsole;
}

int GameConsole::getMsSinceLastConsoleOpen()
{
	return GetTickCount() - lastTimeConsoleShown;
}

void GameConsole::peekConsole()
{
	lastTimeConsoleShown = GetTickCount();
}

void GameConsole::hideConsole()
{
	lastTimeConsoleShown = GetTickCount();
	boolShowConsole = false;
	inputLine.clear();
	enableGameKeyboardInput();
}

void GameConsole::showConsole()
{
	boolShowConsole = true;
	disableGameKeyboardInput();
}

void GameConsole::enableGameKeyboardInput()
{
	uint8_t byteArrayBuffer[] = { 0x55, 0x8B, 0xEC, 0x83, 0xEC, 0x14 }; // restore to original code
	writeToMemory((uint8_t*)0x512690, byteArrayBuffer, 6);
}

void GameConsole::disableGameKeyboardInput()
{
	// TODO: this only works 99.99% of the time
	uint8_t byteArrayBuffer[] = { 0xE9, 0x0B, 0x03, 0x00, 0x00, 0x90 }; // jmp 5129A0
	writeToMemory((uint8_t*)0x512690, byteArrayBuffer, 6);
}

void GameConsole::virtualKeyCallBack(USHORT vKey)
{
	if (!isConsoleShown())
	{
		if (vKey == VK_RETURN)
		{
			showConsole();
		}
		return;
	}

	switch (vKey)
	{
	case VK_RETURN:
		if (!inputLine.empty())
		{
			pushLineFromKeyboardToGame(inputLine);
		}
		hideConsole();
		break;

	case VK_ESCAPE:
		hideConsole();
		break;

	case VK_BACK:
		if (!inputLine.empty())
		{
			inputLine.pop_back();
		}
		break;

	default:
		WORD buf;
		BYTE keysDown[256] = {};

		if (GetAsyncKeyState(VK_SHIFT))
		{
			keysDown[16] = 0x80; // SHIFT down
		}

		int retVal = ToAscii(vKey, 0, keysDown, &buf, 0);

		if (retVal == 1)
		{
			inputLine += buf & 0x00ff;
		}
		else if (retVal == 2)
		{
			inputLine += buf >> 8;
			inputLine += buf & 0x00ff;
		}
		break;
	}
}

std::string GameConsole::getInputLine()
{
	return inputLine;
}

void GameConsole::pushLineFromKeyboardToGame(std::string line)
{
	if (line.find("/") == 0)
	{
		pushLineFromGameToUI(line);
		line.erase(0, 1);
		pushLineFromGameToUIMultipleLines(Modules::CommandMap::Instance().ExecuteCommand(line));
	}
	else
	{
		std::string preparedLineForIRC = playerName;
		preparedLineForIRC += ": ";
		preparedLineForIRC += line;
		sendThisLineToIRCServer = preparedLineForIRC;
		pushLineFromGameToUI(preparedLineForIRC);
	}
}

void GameConsole::initPlayerName()
{
	wchar_t* inGameName = (wchar_t*)0x19A03E8; // unicode
	std::wstring toWstr(inGameName);
	std::string toStr(toWstr.begin(), toWstr.end());
	playerName = toStr;
}

void GameConsole::pushLineFromGameToUI(std::string line)
{
	for (int i = 0; i < numOfLines - 1; i++)
	{
		queue.at(i) = queue.at(i + 1);
	}
	queue.at(numOfLines - 1) = line;

	peekConsole();
}

void GameConsole::pushLineFromGameToUIMultipleLines(std::string multipleLines)
{
	std::vector<std::string> linesVector;
	split(multipleLines, '\n', linesVector);

	for (std::string line : linesVector)
	{
		pushLineFromGameToUI(line);
	}
}

std::string GameConsole::getPlayerName()
{
	return playerName;
}

int GameConsole::getNumOfLines()
{
	return numOfLines;
}

std::string GameConsole::at(int i)
{
	return queue.at(i);
}

std::vector<std::string>& GameConsole::split(const std::string &s, char delim, std::vector<std::string> &elems)
{
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}