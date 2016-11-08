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
	target->Clear(D2D1::ColorF(D2D1::ColorF::Black, 0.0f));

	target->FillRectangle(this->GetBoundingRectangle(), mBackground);

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

void CashDisplayLayer::Initialize()
{
	Layer::Initialize();

	D2D1_GRADIENT_STOP g_stops[] = {
		D2D1::GradientStop(0.0f, D2D1::ColorF(D2D1::ColorF::Black, 0.3f)),
		D2D1::GradientStop(0.9f, D2D1::ColorF(D2D1::ColorF::Black, 0.2f)),
		D2D1::GradientStop(1.0f, D2D1::ColorF(D2D1::ColorF::Black, 0.5f))
	};
	ID2D1GradientStopCollection *stops = nullptr;
	D2Pool::GetSourceRenderTarget()->CreateGradientStopCollection(g_stops, 3, D2D1_GAMMA_1_0, D2D1_EXTEND_MODE_CLAMP, &stops);

	D2Pool::GetSourceRenderTarget()->CreateLinearGradientBrush(D2D1::LinearGradientBrushProperties(D2D1::Point2F(0, 0), D2D1::Point2F(this->GetWidth(), 0)), stops, &mBackground);

	stops->Release();
}
