#pragma once
#include "afx.h"
//游戏对象的父类，所有游戏对象都继承自它
class CGameObject : public CObject
{
public:
	CGameObject(int x = 0, int y = 0);
	virtual ~CGameObject();

	//绘制对象
	virtual BOOL Draw(CDC* pDC, BOOL bPause) = 0;

	//获得矩形区域
	virtual CRect GetRect() = 0;

	//获得左上角坐标
	CPoint GetPoint()
	{
		return mPoint;
	}

	//设置该游戏对象位置
	void SetPoint(int x, int y)
	{
		mPoint.x = x;
		mPoint.y = y;
	}
protected:
	//加载该游戏对象对应的图像,图像对象是共有数据，只需要加载一份，使用静态方法
	static BOOL LoadImage(CImageList& imageList, UINT bmpID, COLORREF crMask, int cx, int cy, int nInitial);
protected:
	//游戏对象的位置
	CPoint  mPoint;
};

