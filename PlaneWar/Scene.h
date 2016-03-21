//场景类
#pragma once
#include <atlimage.h>
#include <Mmsystem.h>
#pragma comment(lib, "Winmm.lib")

class CScene
{
	//成员变量
private:
	CImage	images[8];//滚动背景,0位为开始图片，1-7为七张不同的背景
	int		beginY;//背景的Y坐标

	bool	isStart;//是否开始

//成员函数
public:
	bool InitScene();//初始化场景
	void MoveBg();//移动背景
				  ////绘制场景(注：这里bufferDC是引用参数)
	void StickScene(CDC* pDC, int index, CRect rClient);//传入index-1表示输出开始图片
	void ReleaseScene();//释放内存资源

//构造与析构
public:
	CScene(void);
	~CScene(void);
};


