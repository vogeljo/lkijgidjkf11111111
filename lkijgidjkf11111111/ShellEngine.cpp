#include "ShellEngine.h"

ShellEngine::ShellEngine()
{
}


ShellEngine::~ShellEngine()
{
}

void ShellEngine::SetHandler(std::wstring command, handler_type handler, bool overwrite /*= true*/)
{
	command = Util::ToLower(command);
	auto it = mHandlers.find(command);
	if (it == mHandlers.end() || overwrite)
		mHandlers[command] = handler;
}

void ShellEngine::RemoveHandler(std::wstring command)
{
	command = Util::ToLower(command);
	mHandlers.erase(command);
}

ShellEngine::handler_type ShellEngine::GetHandler(std::wstring command)
{
	command = Util::ToLower(command);
	auto it = mHandlers.find(command);
	if (it != mHandlers.end())
		return it->second;
	else
		return handler_type();
}

bool ShellEngine::Execute(std::wstring command, std::vector<std::wstring>& args)
{
	command = Util::ToLower(command);
	auto it = mHandlers.find(command);
	if (it != mHandlers.end()) {
		it->second(*this, args);
		return true;
	}
	return false;
}

bool ShellEngine::Execute(std::wstring input_line)
{
	size_t space_index = 0;
	std::vector<std::wstring> words;
	do {
		space_index = input_line.find(' ');

		if (space_index != 0) {
			auto word = input_line.substr(0, space_index);
			words.push_back(word);
		}

		if (space_index != std::wstring::npos)
			input_line.assign(input_line.begin() + space_index + 1, input_line.end());
	} while (space_index != std::wstring::npos);

	if (words.size() != 0) {
		auto cmd = words.front();
		words.erase(words.begin());
		return this->Execute(cmd, words);
	}
	return false;
}
