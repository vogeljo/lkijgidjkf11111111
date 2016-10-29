#include "CashDisplayLayer.h"

CashDisplayLayer::CashDisplayLayer(Game& game, Unit& unit, int width, int height)
	: Layer(game, width, height), mUnit(unit), mDrawUnitName(true), mMode(CashDisplayLayerMode::Total)
{
	this->SetPadding(3.0f);
}

CashDisplayLayer::~CashDisplayLayer()
{
}

void CashDisplayLayer::SetUnit(Unit unit)
{
	mUnit = unit;
}

void CashDisplayLayer::SetDrawUnitName(bool value)
{
	mDrawUnitName = value;
	this->Invalidate(INVALIDATION_ALL);
}

void CashDisplayLayer::ToggleDrawUnitName()
{
	SetDrawUnitName(!mDrawUnitName);
}

void CashDisplayLayer::OnUpdate()
{
	//throw std::logic_error("The method or operation is not implemented.");
}

bool CashDisplayLayer::OnDraw(ID2D1RenderTarget* target)
{
	target->Clear(D2D1::ColorF(D2D1::ColorF::Black, 0.5f));

	std::wstring modeStr = L"";
	const float fontSize = 22.0f;

	switch (mMode)
	{
	case CashDisplayLayerMode::Total:
		modeStr = L"Total";
		D2Pool::PrintText(D2Pool::IntToMoney(mUnit.GetStats().Get(Stat::Money)), target, D2Pool::GetFormat(D2PoolFont::MONOSPACE), this->GetContentRectangle(), D2Pool::GetSolidColorBrush(D2D1::ColorF::White), fontSize, DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		break;
	case CashDisplayLayerMode::Change:
	{
		modeStr = L"Change";
		int chg = mUnit.GetMoneyChange();
		D2D1_COLOR_F color = D2D1::ColorF(D2D1::ColorF::Yellow);
		if (chg < 0)
			color = D2D1::ColorF(D2D1::ColorF::Red);
		else if (chg > 0)
			color = D2D1::ColorF(0.3f, 1.0f, 0.3f);

		D2Pool::PrintText(D2Pool::IntToMoneyChange(chg, true), target, D2Pool::GetFormat(D2PoolFont::MONOSPACE), this->GetContentRectangle(), D2Pool::GetSolidColorBrush(color), fontSize, DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		
	}
		break;
	default:
		break;
	}

	if (mDrawUnitName)
		D2Pool::PrintText(D2Pool::FormatString(L"%s", mUnit.GetName().c_str()), target, D2Pool::GetFormat(D2PoolFont::NORMAL), this->GetContentRectangle(), D2Pool::GetSolidColorBrush(D2D1::ColorF::White), 12.0f, DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	D2Pool::PrintText(modeStr, target, D2Pool::GetFormat(D2PoolFont::NORMAL), this->GetContentRectangle(), D2Pool::GetSolidColorBrush(D2D1::ColorF::White), 12.0f, DWRITE_TEXT_ALIGNMENT_TRAILING, DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	return true;
}

bool MUST_CALL CashDisplayLayer::OnLMouseDown(int x, int y)
{
	mMode = (mMode == CashDisplayLayerMode::Total) ? CashDisplayLayerMode::Change : CashDisplayLayerMode::Total;
	this->Invalidate(INVALIDATION_ALL);
	//this->ToggleDrawUnitName();
	return Layer::OnLMouseDown(x, y);
}