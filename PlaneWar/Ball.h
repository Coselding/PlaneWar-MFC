#pragma once
#include "GameObject.h"
#include "Resource.h"

//固定敌机炸弹的速度为20
#define BALL_SPEED 6
//敌机炮弹图标高都为8
#define BALL_HEIGHT 40
#define BALL_WIDTH 15

//敌机炮弹类
class CBall :public CGameObject
{
public:
	//direction表示敌机炮弹方向
	CBall(int x, int y, int direction);
	~CBall(void);

	BOOL Draw(CDC* pDC, BOOL bPause);
	BOOL Draw(CDC *pDC, int passNum, BOOL bPause);

	static BOOL LoadImageUp();
	static BOOL LoadImageDown();

	//获得炮弹所在的矩形框
	CRect GetRect()
	{
		return CRect(mPoint, CPoint(mPoint.x + BALL_WIDTH, mPoint.y + BALL_HEIGHT));
	}
	void SetBallSpeed(int ballSpeed)
	{
		this->ballSpeed = ballSpeed;
	}

private:
	//向上和向下的敌机炮弹
	static CImageList imagesUp;
	static CImageList imagesDown;
	int direction;//控制炸弹的飞行方向
	int ballSpeed;//炸弹的速度
};

