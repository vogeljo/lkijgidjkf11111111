#include "ConsoleLayer.h"

ConsoleLayer::~ConsoleLayer()
{
}

void ConsoleLayer::Initialize()
{
	//dyn_caret = AlternatingDynamic<int>(0, 1, 1000);

	this->SetPadding(5.0f);
	this->SetOpacity(0.8f);
}

void ConsoleLayer::OnUpdate()
{
	auto caret_value = dyn_caret.Get();
	if (caret_value != last_caret_dyn_value) {
		this->Invalidate();
		last_caret_dyn_value = caret_value;
	}
}

bool ConsoleLayer::OnDraw(ID2D1RenderTarget* target)
{
	target->Clear(D2D1::ColorF(D2D1::ColorF::Black));

	auto text = mLine;
	text = std::wstring(L"> ") + text;
	auto text_rect = this->GetContentRectangle();
	auto text_dims = D2Pool::MeasureString(text, D2Pool::GetFormat(D2PoolFont::MONOSPACE), text_rect, 18.0f, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	D2Pool::PrintText(text, target, D2Pool::GetFormat(D2PoolFont::MONOSPACE), text_rect, D2Pool::GetSolidColorBrush(D2D1::ColorF::White), 18.0f, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	if (this->HasFocus() && dyn_caret.Get() == 0) {
		float cursor_offset = text_dims.width;
		auto cursor_rect = text_rect;
		cursor_rect.left += cursor_offset;

		D2Pool::PrintText(L"_", target, D2Pool::GetFormat(D2PoolFont::MONOSPACE), cursor_rect, D2Pool::GetSolidColorBrush(D2D1::ColorF::White), 18.0f, DWRITE_FONT_WEIGHT_THIN, DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}

	return true;
}

void MUST_CALL ConsoleLayer::OnKeyChar(wchar_t c)
{
	switch (c) {
	case VK_BACK:
		if (!mLine.empty()) {
			mLine.assign(mLine.begin(), mLine.end() - 1);
			this->Invalidate();
		}
		break;
	case VK_RETURN:
		if (!mLine.empty()) {
			this->OnCommand(mLine);
			mLine.clear();
			this->Invalidate();
		}
		break;
	default:
		if (iswprint(c)) {
			this->Insert({ c });
			this->Invalidate();
		}
		break;
	}
	dyn_caret.Reset();
	this->Invalidate();
}

void ConsoleLayer::OnCommand(std::wstring cmd)
{
	size_t index = 0;
	auto next_word = [&index, cmd](std::wstring& target) -> bool {
		if (index == cmd.npos)
			return false;

		while (cmd[index] == L' ')
			++index;

		auto first_space = cmd.find(' ', index);
		std::wstring result;

		if (first_space == cmd.npos) {
			result = cmd.substr(index);
			index = cmd.npos;
		}
		else {
			result = cmd.substr(index, first_space - index);
			index = first_space + 1;
		}

		if (result.empty())
			return false;
		else {
			target = result;
			return true;
		}
	};

	std::vector<std::wstring> commands;
	std::wstring word;
	while (next_word(word)) {
		commands.push_back(word);
	}

	if (!commands.empty()) {
		auto main_command = commands.front();
		std::for_each(main_command.begin(), main_command.end(), [&](wchar_t& chr) { chr = std::tolower(chr); });

		auto assert_ccount = [&](size_t count) {
			if (commands.size() < count)
				MessageBoxW(this->GetGame().GetWindowHandle(), D2Pool::FormatString(L"Not enough params for command '%s'", main_command.c_str()).c_str(), NULL, MB_ICONINFORMATION);
		};

		if (main_command == L"pos") {
			assert_ccount(4);
			MessageBox(this->GetGame().GetWindowHandle(), "Setting position", NULL, MB_ICONINFORMATION);
		}
	}
}

void MUST_CALL ConsoleLayer::OnKeyDown(int key)
{
	switch (key) {
	case 'V':
		if (GetKeyState(VK_CONTROL) & 0x8000) {
			auto hwnd = mGame.GetWindowHandle();
			OpenClipboard(hwnd);

			auto hnd = GetClipboardData(CF_UNICODETEXT);
			auto ptr = GlobalLock(hnd);

			wchar_t* text = (wchar_t*)ptr;

			if (text)
				this->Insert(text);

			GlobalUnlock(hnd);
			CloseClipboard();
		}
	}
}

void ConsoleLayer::Insert(std::wstring str)
{
	std::wstring newstr;
	for (auto it = str.begin(); it != str.end(); ++it) {
		if (iswprint(*it)) {
			newstr.append({ *it });
		}
	}
	mLine.append(newstr);

	this->Invalidate();
}

void ConsoleLayer::OnFocusChange(bool hasFocus)
{
	if (!hasFocus)
		this->FadeOut();
	this->Invalidate();
}