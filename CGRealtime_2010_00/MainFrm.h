
// MainFrm.h : CMainFrame 类的接口
//

#pragma once

#include "..\inc\CGVideo.h"
#include "..\inc\CGDef.h"
#include "..\inc\CGVidEx.h"

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <cctype>
#include <iostream>
#include <iterator>
#include <stdio.h>
#include <E:\opencv2.4.4\build\include\opencv2\core\types_c.h>
#include "serialport.h"


using namespace std;
using namespace cv;

#define   WM_SNAP_EX_CHANGE		(WM_USER + 100)

class CMainFrame : public CFrameWndEx
{
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 特性
public:

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CMFCMenuBar       m_wndMenuBar;
	//CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	//CMFCToolBarImages m_UserImages;
	CSplitterWnd	  m_wndSplitter;	//用于静态切分窗口的窗口对象

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

private:

	int count_undetected;						//记录连续未检测到人脸的累计数,这个数值用于控制快速变焦拉远
	int count_undetected_2;						//记录连续未检测到人脸的累计数，这个数值用于控制云台复位




	HCG	m_hcg;			//图像卡句柄

	BOOL m_bOpen;		//标志图像卡的开启
	BOOL m_bStart;		//标志图像卡的采集开始

	BOOL m_bNegative;	//标志是否反色

	BITMAPINFO *m_pBmpInfo;		//位图文件信息
	BYTE *m_pImageBuffer;		//图像缓冲区
	char m_chBmpBuf[2048];		//位图文件信息缓冲区

	IplImage * m_pIplImg;						//指向当前处理的图像的指针

	string cascadeName;							//opencv预先训练好的物体检测器
	string nestedCascadeName;					//嵌套检测用的物体检测器，比如鼻子、眼睛

	Mat frame, frameCopy, image;				//帧、帧拷贝、图像
	bool tryflip;								//标志是否翻转检测
	CascadeClassifier cascade;					//opencv中的分类器
	CascadeClassifier nestedCascade;			//用于嵌套检测的opencv分类器
	double scale;								//检测时图像缩放比例
	bool trynested;								//标志是否进行嵌套检测
	BOOL cascadestatus;							//标志 是否成功载入opencv预先训练好的物体检测器
	BOOL nestedCascadestatus;					//标志 是否成功载入嵌套检测的物体检测器
	bool m_bPortOpen;							//标志 是否打开串口
	BYTE m_pan;									//云台水平转动的速度，最大值0x18
	BYTE m_title;								//云台垂直转动的速度，最大值0x14

	static int CALLBACK SnapThreadCallbackEx(SNAP_INFO *pInfo);

public:
	afx_msg void OnSnapexOpen();
	afx_msg void OnUpdateSnapexOpen(CCmdUI *pCmdUI);
	afx_msg void OnSnapexStart();
	afx_msg void OnUpdateSnapexStart(CCmdUI *pCmdUI);
	afx_msg void OnSnapexStop();
	afx_msg void OnUpdateSnapexStop(CCmdUI *pCmdUI);
	afx_msg void OnSnapexClose();
	afx_msg void OnUpdateSnapexClose(CCmdUI *pCmdUI);

protected:
	afx_msg LRESULT OnSnapExChange(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnClose();
	afx_msg void OnNegative();
	afx_msg void OnUpdateNegative(CCmdUI *pCmdUI);


	bool detectAndDraw(Mat& img, CascadeClassifier& cascade, CascadeClassifier& nestedCascade,double scale, bool tryflip,bool trynested);
	//图像数据拷贝函数	
	void CopyData(char *dest, const char *src, int dataByteSize,bool isConvert, int height);
private:
	Point m_CvPointCenter;					//记录人脸区域的中心坐标
	int m_threshold;						//用于控制云台转动的阈值
	bool m_bZoom;							//标志 是否打开变焦功能
	bool m_bZoomStatus;						//标志 是否正在变焦
	CSerialPort m_Port;						//串口类，串口通信的所有功能由该类完成
	BOOL m_bMoveStatus;						//标志 云台是否正在转动
	int m_radius;							//记录 人脸区域的半径
	BYTE speed1;							//云台变焦拉近的速度
	BYTE speed2;							//云台变焦拉远的速度
public:
	//串口打开处理函数
	afx_msg void OnEvihd1Open();
	//菜单状态处理函数
	afx_msg void OnUpdateEvihd1Open(CCmdUI *pCmdUI);
	//串口关闭处理函数
	afx_msg void OnEvihd1Close();
	//菜单状态处理函数
	afx_msg void OnUpdateEvihd1Close(CCmdUI *pCmdUI);
	//变焦打开处理函数
	afx_msg void OnEvihd1Zoom();
	//菜单状态处理函数
	afx_msg void OnUpdateEvihd1Zoom(CCmdUI *pCmdUI);
	//变焦关闭处理函数
	afx_msg void OnEvihd1ZoomClose();
	//菜单状态处理函数
	afx_msg void OnUpdateEvihd1ZoomClose(CCmdUI *pCmdUI);
	//打开手动控制云台程序
	afx_msg void OnEvihd1Manual();
	//菜单状态处理函数
	afx_msg void OnUpdateEvihd1Manual(CCmdUI *pCmdUI);
	//静态切分窗口函数
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
};


