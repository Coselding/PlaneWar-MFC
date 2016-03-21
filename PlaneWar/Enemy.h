#pragma once
#include "GameObject.h"
#include "Resource.h"
#include "stdafx.h"

//敌机的高度
#define ENEMY_HEIGHT 90
//敌机的宽度
#define ENEMY_WIDTH 120
//敌机飞行速度
#define ENEMY_SPEED 3
//敌机血量
#define ENEMY_LIFE 2

//默认等待时间
#define WAIT 0

//敌机类
class CEnemy :public CGameObject
{
public:
	//指定敌机飞行方向
	CEnemy(int direction,int WINDOWS_WIDTH,int WINDOWS_HEIGHT);
	~CEnemy(void);
	//绘制
	BOOL Draw(CDC* pDC, BOOL bPause);
	BOOL Draw(CDC *pDC, int passNum, BOOL bPause);

	//加载敌机图像
	static BOOL LoadImage();

	//获取敌机矩形
	CRect GetRect()
	{
			return CRect(mPoint, CPoint(mPoint.x + ENEMY_HEIGHT, mPoint.y + ENEMY_HEIGHT));
	}

	int GetDirection() const
	{
		return direction;
	}
	void SetDirection(int direction)
	{
		this->direction = direction;
	}
	void SetSpeed(int speed)
	{
	this->speed = speed;
	}

	//是否可以开火发射子弹
	BOOL isFire();

protected:
	static CImageList imagesUp;//向上飞敌机图像
	static CImageList imagesDown;//向下飞敌机图像
	int    direction;//方向 1->向下 0->停止 -1->向上
	 //图像索引
	int imageIndex;//取0代表向下飞的敌机，为1代表向上飞的敌机
	//敌机的速度
	int speed;
	int  wait;//发射延时
	int WINDOWS_WIDTH;//窗口宽
	int WINDOWS_HEIGHT;//窗口高
public :
	int life;//敌机的生命值
};
