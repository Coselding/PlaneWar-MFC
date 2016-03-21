
// PlaneWarView.h : CPlaneWarView 类的接口
//

#pragma once

#include "Ball.h"
#include "Bomb.h"
#include "Enemy.h"
#include "Boss.h"
#include "Explosion.h"
#include "MyPlane.h"
#include "Blood.h"
#include "GameObject.h"
#include "Scene.h"
#include "MyDialog.h"
#include "RestartDialog.h"

//默认战机速度
#define DEFAULT_SPEED 30
//默认战机生命
#define DEFAULT_LIFE 10
//默认战机命条数
#define DEFAULT_LIFE_COUNT 3
//默认关卡
#define DEFAULT_PASS 1
//两个战机子弹间隔
#define BOMB_DISTANCE 35
//方向键步长
#define STEP 30
//过关得分数
#define PASS_SCORE 20
//标记位
#define FLAG_RESTART 2
#define FLAG_STOP 3


//游戏视图窗口类
class CPlaneWarView : public CView
{
protected: // 仅从序列化创建
	CPlaneWarView();
	DECLARE_DYNCREATE(CPlaneWarView)

// 特性
public:
	CPlaneWarDoc* GetDocument() const;

	CScene	scene;//场景

	//创建各游戏对象
	CMyPlane *myplane;
	CEnemy *enemy;
	CBoss *boss;
	CBomb *bomb;
	CBall *ball;
	CExplosion *explosion;
	CBlood* blood;

	//创建存储游戏对象的对象链表
	CObList enemyList;
	CObList meList;
	CObList bombList;
	CObList ballList;
	CObList explosionList;
	CObList bloodList;

	CRect rect;//窗口屏幕矩形

	int speed;//战机的速度，方向键控制
	int myLife;//为战机设置生命值
	int lifeNum;//战机命条数
	int myScore;//战机的得分
	int passScore;//当前关卡得分数
	int lifeCount;//血包产生控制参数
	BOOL bloodExist;//标记屏幕中是否存在血包
	int magicCount;//魔法值，控制能否发大招
	int bossBlood;//Boss血量

	int passNum;//记录当前关卡
	int isPass;//是否通关的标志
	int isPause;//是否暂停
	BOOL isBoss;//标记是否进入Boss
	BOOL bossLoaded;//标记Boss出场完成
	BOOL isProtect;//标记是否开启防护罩
	BOOL isUpdate;//标记战机是否升级
	BOOL test;//无敌模式参数
	BOOL isStop;//标记游戏停止
	BOOL isStarted;//标记欢迎界面是否加载完成
	CImageList startIMG;

// 操作
public:
	//获取战机速度
	int GetSpeed();
	//设置战机速度
	void SetSpeed(int speed);
	//游戏暂停
	void Pause();
	//游戏重新开始
	void Restart();

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CPlaneWarView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	// 生命值归零，游戏结束
	void gameOver(CDC* pDC,  CDC& cdc,  CBitmap* cacheBitmap);
	//设置计时器
	void SetMyTimer();
	//afx_msg void OnClose();
};

#ifndef _DEBUG  // PlaneWarView.cpp 中的调试版本
inline CPlaneWarDoc* CPlaneWarView::GetDocument() const
   { return reinterpret_cast<CPlaneWarDoc*>(m_pDocument); }
#endif

