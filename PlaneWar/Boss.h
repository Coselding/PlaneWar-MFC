#pragma once
#include "Enemy.h"

//Boss的高度
#define BOSS_HEIGHT 250
//Boss的宽度
#define BOSS_WIDTH 360
//Boss飞行速度
#define BOSS_SPEED 3
//默认Boss生命
#define BOSS_LIFE 100

//Boss类
class CBoss :
	public CEnemy
{
public:
	CBoss(int direction, int WINDOWS_WIDTH, int WINDOWS_HEIGHT);
	~CBoss();
	//绘制
	BOOL Draw(CDC* pDC, BOOL bPause);
	BOOL Draw(CDC *pDC, int passNum, BOOL bPause);

	//加载敌机图像
	static BOOL LoadImage();
	//获取敌机矩形
	CRect GetRect()
	{
			return CRect(mPoint, CPoint(mPoint.x + BOSS_WIDTH, mPoint.y + BOSS_HEIGHT));
	}

	//是否可以开火发射子弹
	BOOL isFire();
private:
	BOOL bossLoadOK;//Boss是否出场完毕
	//Boss图像
	static CImageList imagesBoss;
};

