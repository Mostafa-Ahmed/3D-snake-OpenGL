#include "Snake.h"



Snake::Snake()
{
	in_y = 0, in_x = 0;
	X_new = X_old = 0;
	Y_new = Y_old = 0;
	angle = 0;
	R = rotate(0.0f, 0.0f, 0.0f, 1.0f);
}

mat4 Snake::turn_Right(int y_Dir)
{
	if (y_Dir == 1)
	{
		angle -= 90;
		return rotate(angle, 0.0f, 0.0f, 1.0f);
	}
	else if (y_Dir == -1)
	{
		angle += 90;
		return rotate(angle, 0.0f, 0.0f, 1.0f);
	}
	else
		return rotate(angle, 0.0f, 0.0f, 1.0f);
}

mat4 Snake::turn_Left(int y_Dir)
{
	if (y_Dir == 1)
	{
		angle += 90;
		return rotate(angle, 0.0f, 0.0f, 1.0f);
	}
	else if (y_Dir == -1)
	{
		angle -= 90;
		return rotate(angle, 0.0f, 0.0f, 1.0f);
	}
	else
		return rotate(angle, 0.0f, 0.0f, 1.0f);
}

mat4 Snake::turn_Up(int x_Dir)
{
	if (x_Dir == 1)
	{
		angle += 90;
		return rotate(angle, 0.0f, 0.0f, 1.0f);
	}
	else if (x_Dir == -1)
	{
		angle -= 90;
		return rotate(angle, 0.0f, 0.0f, 1.0f);
	}
	else
		return rotate(angle, 0.0f, 0.0f, 1.0f);
}

mat4 Snake::turn_Down(int x_Dir)
{
	if (x_Dir == 1)
	{
		angle -= 90;
		return rotate(angle, 0.0f, 0.0f, 1.0f);
	}
	else if (x_Dir == -1)
	{
		angle += 90;
		return rotate(angle, 0.0f, 0.0f, 1.0f);
	}
	else
		return rotate(angle, 0.0f, 0.0f, 1.0f);
}

Snake::~Snake()
{
}
