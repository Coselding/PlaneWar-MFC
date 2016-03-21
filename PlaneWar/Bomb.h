#pragma once
#include "GameObject.h"
#include "Resource.h"

//战机子弹高都为20
#define BOMB_HEIGHT 60

//战机子弹宽
#define BOMB_WIDTH 30

//子弹飞行速度
#define BOMB_SPEED 30

//战机子弹类
class CBomb :public CGameObject
{
public:
	CBomb(int x, int y, int direction,int isUpdate);
	~CBomb(void);

	//绘制
	BOOL Draw(CDC* pDC, BOOL bPause);

	//加载方向向上的炮弹图标
	static BOOL LoadImage();

	CRect GetRect()
	{
		return CRect(mPoint, CPoint(mPoint.x + BOMB_WIDTH, mPoint.y + BOMB_HEIGHT));
	}
	//获取炸弹的高度
	int GetBombHeight()
	{
		return BOMB_HEIGHT;
	}
	//获取炸弹的宽度
	int GetBombWidth()
	{
		return BOMB_WIDTH;
	}
	void SetDirection(int direction)
	{
		this->direction = direction;
	}
	int GetDirection()
	{
		return direction;
	}
	void SetBombSpeed(int bombSpeed)
	{
		this->bombSpeed = bombSpeed;
	}
	int GetBombSpeed()
	{
		return this->bombSpeed;
	}
private:
	//战机子弹图像链表
	static CImageList imagesUpdate;
	static CImageList images;
	int direction;//取值1向上，-1向下
	int bombSpeed;//设置子弹的飞行速度
	BOOL isUpdate;//标记是否为升级炮弹
};
