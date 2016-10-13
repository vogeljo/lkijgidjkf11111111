#include "CashDisplayLayer.h"

#include "D2Pool.h"

CashDisplayLayer::CashDisplayLayer(Unit& unit, int width, int height)
	: Layer(width, height), mUnit(unit), mDrawUnitName(true), mMode(CashDisplayLayerMode::Total)
{

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
	target->Clear(D2D1::ColorF(D2D1::ColorF::Black));

	std::wstring modeStr = L"";

	switch (mMode)
	{
	case CashDisplayLayerMode::Total:
		modeStr = L"Total";
		D2Pool::PrintText(D2Pool::IntToMoney(mUnit.GetStats().Get(Stat::Money)), target, D2Pool::GetFormat(D2PoolFont::NORMAL), this->GetRectangle(), D2Pool::GetSolidColorBrush(D2D1::ColorF::White), 24.0f, DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		break;
	case CashDisplayLayerMode::Change:
	{
		modeStr = L"+/-";
		int chg = mUnit.GetMoneyChange();
		D2D1_COLOR_F color = D2D1::ColorF(D2D1::ColorF::Yellow);
		if (chg < 0)
			color = D2D1::ColorF(D2D1::ColorF::Red);
		else if (chg > 0)
			color = D2D1::ColorF(0.3f, 1.0f, 0.3f);

		D2Pool::PrintText(D2Pool::IntToMoneyChange(chg, true), target, D2Pool::GetFormat(D2PoolFont::NORMAL), this->GetRectangle(), D2Pool::GetSolidColorBrush(color), 24.0f, DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		
	}
		break;
	default:
		break;
	}

	if (mDrawUnitName)
		D2Pool::PrintText(D2Pool::FormatString(L"%s", mUnit.GetName().c_str()), target, D2Pool::GetFormat(D2PoolFont::NORMAL), this->GetRectangle(), D2Pool::GetSolidColorBrush(D2D1::ColorF::White), 12.0f, DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	D2Pool::PrintText(modeStr, target, D2Pool::GetFormat(D2PoolFont::NORMAL), this->GetRectangle(), D2Pool::GetSolidColorBrush(D2D1::ColorF::White), DWRITE_TEXT_ALIGNMENT_TRAILING, DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	return true;
}

bool MUST_CALL CashDisplayLayer::OnLMouseDown(int x, int y)
{
	mMode = (mMode == CashDisplayLayerMode::Total) ? CashDisplayLayerMode::Change : CashDisplayLayerMode::Total;
	this->Invalidate(INVALIDATION_ALL);
	//this->ToggleDrawUnitName();
	return Layer::OnLMouseDown(x, y);
}
