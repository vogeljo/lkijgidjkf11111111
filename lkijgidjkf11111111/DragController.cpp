#include "DragController.h"

DragController::DragController(int viewWidth, int viewHeight)
{
	mViewport.width = viewWidth;
	mViewport.height = viewHeight;

	mOffset.x = mOffset.y = 0;

	mDragging = false;
}

DragController::~DragController()
{
}

void DragController::StartDrag(int x, int y)
{
	if (!mDragging) {
		mDragging = true;
		mDragStart = Coords(x, y);
	}
}

void DragController::MouseMove(int x, int y)
{
	Coords delta = Coords(x, y) - mDragStart;
}

void DragController::EndDrag()
{
	if (mDragging) {
		mDragging = false;
	}
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

DragController::Coords DragController::Coords::operator-(Coords& c)
{
	return Coords(x - c.x, x - c.x);
}

DragController::Coords DragController::Coords::operator+(Coords& c)
{
	return Coords(x + c.x, x + c.x);
}
