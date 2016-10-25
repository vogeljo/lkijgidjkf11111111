#pragma once
#include "Layer.h"
#include "Unit.h"

enum class CashDisplayLayerMode {
	Total, Change
};

class CashDisplayLayer :
	public Layer
{
private:
	Unit& mUnit;
	bool mDrawUnitName;
	CashDisplayLayerMode mMode;
public:
	CashDisplayLayer(Game& game, Unit& unit, int width, int height);
	virtual ~CashDisplayLayer();

	void SetUnit(Unit unit);
	void SetDrawUnitName(bool value);
	void ToggleDrawUnitName();

	virtual void OnUpdate() override;
	virtual bool OnDraw(ID2D1RenderTarget* target) override;


	virtual bool MUST_CALL OnLMouseDown(int x, int y) override;

};

