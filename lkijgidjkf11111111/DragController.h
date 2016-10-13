#pragma once



class DragController
{
	struct Coords {
		int x, y;

		Coords();
		Coords(int x, int y);
		Coords operator-(Coords& c);
		Coords operator+(Coords& c);
	};
	struct Viewport {
		int width, height;
	};
private:
	Coords mOffset;
	Coords mDragStart;
	Viewport mViewport;
	bool mDragging;
public:
	DragController(int viewWidth, int viewHeight);
	~DragController();

	void StartDrag(int x, int y);
	void MouseMove(int x, int y);
	void EndDrag();
};

