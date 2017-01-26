#pragma once
#include <functional>
#include <map>
#include <vector>
#include "Util.h"

class ShellEngine
{
public:
	typedef std::function<void(ShellEngine&, std::vector<std::wstring>&)> handler_type;
private:
	std::map<std::wstring, handler_type> mHandlers;
public:
	ShellEngine();
	virtual ~ShellEngine();

	void SetHandler(std::wstring command, handler_type handler, bool overwrite = true);
	void RemoveHandler(std::wstring command);
	handler_type GetHandler(std::wstring command);

	bool Execute(std::wstring command, std::vector<std::wstring>& args);
	bool Execute(std::wstring input_line);
};

