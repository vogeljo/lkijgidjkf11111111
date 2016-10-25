#pragma once

class DragController
{
	struct Coords {
		int x, y;

		Coords();
		Coords(int x, int y);
		Coords Limited(int min_x, int min_y, int max_x, int max_y);
		Coords LimitedDown(int min_x, int min_y);
		Coords LimitedUp(int max_x, int max_y);
		bool NullValue();

		Coords operator-(Coords& c);
		Coords operator+(Coords& c);
		Coords operator*(float factor);
	};
	struct Viewport {
		int width, height;
	};
private:
	Coords mOffset;
	Coords mDragStart;
	Coords mDragMove; // last mouse move in drag
	Viewport mViewport;
	Coords mDimensions;
	bool mDragging;
	float mAcceleration;
public:
	DragController(int viewWidth, int viewHeight);
	~DragController();

	void SetDragAreaSize(int width, int height);
	void SetAcceleration(float acc);

	void StartDrag(int x, int y);
	void MouseMove(int x, int y);
	void EndDrag();
	void Reset();

	int GetOffsetX();
	int GetOffsetY();
	void SetOffset(int x, int y);
	void SetOffset(Coords& offset);
	int GetRealX(int x);
	int GetRealY(int y);
	int GetViewX(int x);
	int GetViewY(int y);
	float GetViewX(float x);
	float GetViewY(float y);
};

