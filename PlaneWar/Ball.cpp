#include "StdAfx.h"
#include "Ball.h"

//静态的图像链表引用
CImageList CBall::imagesDown;
CImageList CBall::imagesUp;
//构造函数，设置起始位置和运动方向，运动速度
CBall::CBall(int x, int y, int direction)
{
	mPoint.x = x;
	mPoint.y = y;
	this->direction = direction;
	this->ballSpeed = BALL_SPEED;
}

CBall::~CBall(void)
{
}
//绘制炮弹图标
BOOL CBall::Draw(CDC *pDC, BOOL bPause)
{
	if (bPause == 0) {
		//炮弹纵向飞行，只需要更改纵坐标，先只实现炮弹由上到下飞行
		mPoint.y += ballSpeed * this->direction;
		//Boss炮弹随机变化
		int index = rand() % 5;
		if (direction == 1) {
			imagesDown.Draw(pDC, index, mPoint, ILD_TRANSPARENT);
		}
		else {
			imagesUp.Draw(pDC, index, mPoint, ILD_TRANSPARENT);
		}
		return TRUE;
	}
	else return FALSE;
}
BOOL CBall::Draw(CDC *pDC, int passNum,BOOL bPause)
{
	//敌机炮弹随关卡变化
	int index = (passNum - 1) / 2;
	if (bPause == 0) {
		//炮弹纵向飞行，只需要更改纵坐标，先只实现炮弹由上到下飞行
		mPoint.y += ballSpeed * this->direction;
		if (direction == 1) {
			imagesDown.Draw(pDC, index, mPoint, ILD_TRANSPARENT);
		}
		else {
			imagesUp.Draw(pDC, index, mPoint, ILD_TRANSPARENT);
		}
		return TRUE;
	}
	else return FALSE;
}
//加载炮弹图像
BOOL CBall::LoadImageUp()
{
	return  CGameObject::LoadImageW(imagesUp,IDB_BMP_BALLUP, RGB(0, 0, 0), BALL_WIDTH, BALL_HEIGHT, 5);
}
//加载炮弹图像
BOOL CBall::LoadImageDown()
{
	return  CGameObject::LoadImageW(imagesDown, IDB_BMP_BALLDOWN,RGB(0,0,0), BALL_WIDTH, BALL_HEIGHT, 5);
}

