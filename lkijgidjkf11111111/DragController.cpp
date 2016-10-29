#include "DragController.h"

DragController::DragController(int viewWidth, int viewHeight)
	: mAcceleration(1.0f)
{
	mViewport.width = viewWidth;
	mViewport.height = viewHeight;

	mDragging = false;
}

DragController::~DragController()
{
}

void DragController::SetDragAreaSize(int width, int height)
{
	mDimensions.x = width;
	mDimensions.y = height;
}

void DragController::SetAcceleration(float acc)
{
	mAcceleration = acc;
}

void DragController::StartDrag(int x, int y)
{
	if (!mDragging) {
		mDragging = true;
		mDragStart = Coords(x, y);
		mDragMove = mDragStart;
	}
}

void DragController::MouseMove(int x, int y)
{
	if (mDragging) {
		Coords delta = (Coords(x, y) - mDragMove) * mAcceleration;
		Coords new_offset = mOffset - delta; // substract because dragging works the other way around

		this->SetOffset(new_offset);
		mDragMove = Coords(x, y);

		SetCursor(LoadCursor(NULL, IDC_SIZEALL));
	}
}

void DragController::EndDrag()
{
	if (mDragging) {
		mDragging = false;
	}
}

void DragController::Reset()
{
	mDragging = false;
	mOffset = Coords();
}

int DragController::GetOffsetX()
{
	return mOffset.x;
}

int DragController::GetOffsetY()
{
	return mOffset.y;
}

void DragController::SetOffset(int x, int y)
{
	this->SetOffset(Coords(x, y));
}

void DragController::SetOffset(Coords& offset)
{
	auto new_offset = offset.LimitedDown(0, 0);

	if (!mDimensions.NullValue()) {
		new_offset = new_offset.LimitedUp(mDimensions.x - mViewport.width, mDimensions.y - mViewport.height).LimitedDown(0, 0);
	}
	mOffset = new_offset;
}

int DragController::GetRealX(int x)
{
	return x + this->GetOffsetX();
}

int DragController::GetRealY(int y)
{
	return y + this->GetOffsetY();
}

int DragController::GetViewX(int x)
{
	return x - this->GetOffsetX();
}

int DragController::GetViewY(int y)
{
	return y - this->GetOffsetY();
}

float DragController::GetViewX(float x)
{
	return x - this->GetOffsetX();
}

float DragController::GetViewY(float y)
{
	return y - this->GetOffsetY();
}

// ************************************************************************************
// COORDS #############################################################################
// ************************************************************************************

DragController::Coords::Coords(int x, int y)
	: x(x), y(y)
{

}

DragController::Coords::Coords()
	: Coords(0, 0)
{

}

DragController::Coords DragController::Coords::Limited(int min_x, int min_y, int max_x, int max_y)
{
	return this->LimitedDown(min_x, min_y).LimitedUp(max_x, max_y);
}

DragController::Coords DragController::Coords::LimitedDown(int min_x, int min_y)
{
	return Coords(
		std::max(min_x, x),
		std::max(min_y, y)
	);
}

DragController::Coords DragController::Coords::LimitedUp(int max_x, int max_y)
{
	return Coords(
		std::min(max_x, x),
		std::min(max_y, y)
	);
}

bool DragController::Coords::NullValue()
{
	return x == 0 && y == 0;
}

DragController::Coords DragController::Coords::operator-(Coords& c)
{
	return Coords(x - c.x, y - c.y);
}

DragController::Coords DragController::Coords::operator+(Coords& c)
{
	return Coords(x + c.x, y + c.y);
}

DragController::Coords DragController::Coords::operator*(float factor)
{
	return Coords(std::round(x * factor), std::round(y * factor));
}
