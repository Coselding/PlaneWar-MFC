#pragma once
#include "GameObject.h"
#include "Resource.h"

//血包的速度
#define BLOOD_SPEED 3
//血包图标宽高
#define BLOOD_HEIGHT 60


//血包类
class CBlood :public CGameObject
{
public:
	CBlood(int WINDOWS_WIDTH,int WINDOWS_HEIGHT);
	~CBlood();
	BOOL Draw(CDC* pDC, BOOL bPause);

	static BOOL LoadImage();

	//获得血包所在的矩形框
	CRect GetRect()
	{
		return CRect(mPoint, CPoint(mPoint.x + BLOOD_HEIGHT, mPoint.y + BLOOD_HEIGHT));
	}
	void SetBallSpeed(int bloodSpeed)
	{
		this->bloodSpeed = bloodSpeed;
	}

private:
	static CImageList images;
	int bloodSpeed;//血包的速度
	int directionX;//血包飞行方向，1为向右，-1为向左
	int directionY;//血包飞行方向，1为向下，-1为向上
	int WINDOWS_WIDTH;//窗口宽度
	int WINDOWS_HEIGHT;//窗口高度
};


