#include "MapTile.h"

MapTile::MapTile()
	: type(MapTileType::None)
{
}


MapTile::~MapTile()
{
}

D2D1_COLOR_F GetMapTileColor(MapTileType type)
{
	switch (type)
	{
	case MapTileType::Water:
		return D2D1::ColorF(D2D1::ColorF::CornflowerBlue);
	case MapTileType::Dirt:
		return D2D1::ColorF(0xA0522D);
	case MapTileType::Stone:
		return D2D1::ColorF(0xA9A9A9);
	case MapTileType::Sand:
		return D2D1::ColorF(0xF0E68C);
	case MapTileType::House:
		return D2D1::ColorF(0xA7A7A7);
		break;
	case MapTileType::Unknown:
		return D2D1::ColorF(D2D1::ColorF::Black, 0.2f);
	case MapTileType::None:
	default:
		return D2D1::ColorF(D2D1::ColorF::Gray);
	}
}

void GetMapTileInfo(MapTileType type, std::wstring& dTitle, std::wstring& dText)
{
	std::wstring rtitle, rtext;
	switch (type) {
	case MapTileType::Water:
		rtext = L"Wasser (H2O) ist eine chemische Verbindung aus den Elementen Sauerstoff (O) und Wasserstoff (H). Wasser ist als Flüssigkeit durchsichtig, weitgehend farb-, geruch- und geschmacklos.";
		rtitle = L"Wasser";
		break;
	case MapTileType::Dirt:
		rtext = L"Schmutz bezeichnet etwas, das Unsauberkeit oder Verunreinigung verursacht. Schmutz und Verschmutzung ist also dort, wo Sauberkeit abwesend ist.";
		rtitle = L"Schmutz";
		break;
	case MapTileType::Stone:
		rtext = L"Als Gestein bezeichnet man eine feste, natürlich auftretende Vereinigung von Mineralen, Gesteinsbruchstücken, Gläsern oder Rückständen von Organismen.";
		rtitle = L"Gestein";
		break;
	case MapTileType::Sand:
		rtext = L"Sand ist ein natürlich vorkommendes, unverfestigtes Sediment, das sich überwiegend aus Mineralkörnern mit einer Korngröße von 0,063 bis 2 Millimeter zusammensetzt.";
		rtitle = L"Sand";
		break;
	case MapTileType::House:
		rtext = L"Häuser bieten verschiedene Aktionen an.";
		rtitle = L"Haus";
		break;
	}

	if (!rtitle.empty())
		dTitle.replace(dTitle.begin(), dTitle.end(), rtitle);
	if (!rtext.empty())
		dText.replace(dText.begin(), dText.end(), rtext);

}
