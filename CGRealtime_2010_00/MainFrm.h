
// MainFrm.h : CMainFrame ��Ľӿ�
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
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CMFCMenuBar       m_wndMenuBar;
	//CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	//CMFCToolBarImages m_UserImages;
	CSplitterWnd	  m_wndSplitter;	//���ھ�̬�зִ��ڵĴ��ڶ���

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

private:

	int count_undetected;						//��¼����δ��⵽�������ۼ���,�����ֵ���ڿ��ƿ��ٱ佹��Զ
	int count_undetected_2;						//��¼����δ��⵽�������ۼ����������ֵ���ڿ�����̨��λ




	HCG	m_hcg;			//ͼ�񿨾��

	BOOL m_bOpen;		//��־ͼ�񿨵Ŀ���
	BOOL m_bStart;		//��־ͼ�񿨵Ĳɼ���ʼ

	BOOL m_bNegative;	//��־�Ƿ�ɫ

	BITMAPINFO *m_pBmpInfo;		//λͼ�ļ���Ϣ
	BYTE *m_pImageBuffer;		//ͼ�񻺳���
	char m_chBmpBuf[2048];		//λͼ�ļ���Ϣ������

	IplImage * m_pIplImg;						//ָ��ǰ�����ͼ���ָ��

	string cascadeName;							//opencvԤ��ѵ���õ���������
	string nestedCascadeName;					//Ƕ�׼���õ�����������������ӡ��۾�

	Mat frame, frameCopy, image;				//֡��֡������ͼ��
	bool tryflip;								//��־�Ƿ�ת���
	CascadeClassifier cascade;					//opencv�еķ�����
	CascadeClassifier nestedCascade;			//����Ƕ�׼���opencv������
	double scale;								//���ʱͼ�����ű���
	bool trynested;								//��־�Ƿ����Ƕ�׼��
	BOOL cascadestatus;							//��־ �Ƿ�ɹ�����opencvԤ��ѵ���õ���������
	BOOL nestedCascadestatus;					//��־ �Ƿ�ɹ�����Ƕ�׼�����������
	bool m_bPortOpen;							//��־ �Ƿ�򿪴���
	BYTE m_pan;									//��̨ˮƽת�����ٶȣ����ֵ0x18
	BYTE m_title;								//��̨��ֱת�����ٶȣ����ֵ0x14

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
	//ͼ�����ݿ�������	
	void CopyData(char *dest, const char *src, int dataByteSize,bool isConvert, int height);
private:
	Point m_CvPointCenter;					//��¼�����������������
	int m_threshold;						//���ڿ�����̨ת������ֵ
	bool m_bZoom;							//��־ �Ƿ�򿪱佹����
	bool m_bZoomStatus;						//��־ �Ƿ����ڱ佹
	CSerialPort m_Port;						//�����࣬����ͨ�ŵ����й����ɸ������
	BOOL m_bMoveStatus;						//��־ ��̨�Ƿ�����ת��
	int m_radius;							//��¼ ��������İ뾶
	BYTE speed1;							//��̨�佹�������ٶ�
	BYTE speed2;							//��̨�佹��Զ���ٶ�
public:
	//���ڴ򿪴�����
	afx_msg void OnEvihd1Open();
	//�˵�״̬������
	afx_msg void OnUpdateEvihd1Open(CCmdUI *pCmdUI);
	//���ڹرմ�����
	afx_msg void OnEvihd1Close();
	//�˵�״̬������
	afx_msg void OnUpdateEvihd1Close(CCmdUI *pCmdUI);
	//�佹�򿪴�����
	afx_msg void OnEvihd1Zoom();
	//�˵�״̬������
	afx_msg void OnUpdateEvihd1Zoom(CCmdUI *pCmdUI);
	//�佹�رմ�����
	afx_msg void OnEvihd1ZoomClose();
	//�˵�״̬������
	afx_msg void OnUpdateEvihd1ZoomClose(CCmdUI *pCmdUI);
	//���ֶ�������̨����
	afx_msg void OnEvihd1Manual();
	//�˵�״̬������
	afx_msg void OnUpdateEvihd1Manual(CCmdUI *pCmdUI);
	//��̬�зִ��ں���
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
};


