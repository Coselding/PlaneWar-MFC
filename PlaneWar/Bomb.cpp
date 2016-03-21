#include "StdAfx.h"
#include "Bomb.h"

//CBomb实现

//共有图像链表引用，用静态
CImageList CBomb::imagesUpdate;//升级炮弹图像
CImageList CBomb::images;//未升级炮弹图像

//设置初始子弹位置，飞行速度，飞行方向
CBomb::CBomb(int x, int y, int direction,BOOL isUpdate)
{
	mPoint.x = x;
	mPoint.y = y;
	//设置战机子弹的飞行速度
	bombSpeed = BOMB_SPEED;
	this->direction = direction;
	this->isUpdate = isUpdate;
}

CBomb::~CBomb(void)
{
}

//绘制
BOOL CBomb::Draw(CDC *pDC, BOOL bPause)
{
	if (bPause == 0) {
		//子弹只需要改纵坐标
		mPoint.y -= bombSpeed * direction;//bombSpeed代表子弹的速度，也即是子弹移动的步长
		if (isUpdate == TRUE) {
			int index = rand() % 18;
			return imagesUpdate.Draw(pDC, index, mPoint, ILD_TRANSPARENT);
		}
		else {
			int index = rand() % 15;
			return images.Draw(pDC, index, mPoint, ILD_TRANSPARENT);
		}
	}
	else return FALSE;
}

BOOL CBomb::LoadImage()//加载向上飞的子弹的图片
{
	CGameObject::LoadImage(images, IDB_BMP_BOMB1, RGB(0, 0, 0), BOMB_WIDTH, BOMB_WIDTH, 15);
	CGameObject::LoadImage(imagesUpdate, IDB_BMP_BOMB, RGB(0, 0, 0), BOMB_WIDTH, BOMB_HEIGHT, 18);
	return TRUE;
}