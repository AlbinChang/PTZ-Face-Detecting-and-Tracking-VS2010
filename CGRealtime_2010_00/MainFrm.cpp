
// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "CGRealtime_2010_00.h"

#include "MainFrm.h"

#include "CGRealtime_2010_00View.h"
#include "SplitView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_COMMAND(ID_SNAPEX_OPEN, &CMainFrame::OnSnapexOpen)
	ON_UPDATE_COMMAND_UI(ID_SNAPEX_OPEN, &CMainFrame::OnUpdateSnapexOpen)
	ON_COMMAND(ID_SNAPEX_START, &CMainFrame::OnSnapexStart)
	ON_UPDATE_COMMAND_UI(ID_SNAPEX_START, &CMainFrame::OnUpdateSnapexStart)
	ON_COMMAND(ID_SNAPEX_STOP, &CMainFrame::OnSnapexStop)
	ON_UPDATE_COMMAND_UI(ID_SNAPEX_STOP, &CMainFrame::OnUpdateSnapexStop)
	ON_COMMAND(ID_SNAPEX_CLOSE, &CMainFrame::OnSnapexClose)
	ON_UPDATE_COMMAND_UI(ID_SNAPEX_CLOSE, &CMainFrame::OnUpdateSnapexClose)
	ON_MESSAGE(WM_SNAP_EX_CHANGE, &CMainFrame::OnSnapExChange)
	ON_WM_CLOSE()
	ON_COMMAND(ID_NEGATIVE, &CMainFrame::OnNegative)
	ON_UPDATE_COMMAND_UI(ID_NEGATIVE, &CMainFrame::OnUpdateNegative)
	ON_COMMAND(ID_EVIHD1_OPEN, &CMainFrame::OnEvihd1Open)
	ON_UPDATE_COMMAND_UI(ID_EVIHD1_OPEN, &CMainFrame::OnUpdateEvihd1Open)
	ON_COMMAND(ID_EVIHD1_CLOSE, &CMainFrame::OnEvihd1Close)
	ON_UPDATE_COMMAND_UI(ID_EVIHD1_CLOSE, &CMainFrame::OnUpdateEvihd1Close)
	ON_COMMAND(ID_EVIHD1_ZOOM, &CMainFrame::OnEvihd1Zoom)
	ON_UPDATE_COMMAND_UI(ID_EVIHD1_ZOOM, &CMainFrame::OnUpdateEvihd1Zoom)
	ON_COMMAND(ID_EVIHD1_ZOOM_CLOSE, &CMainFrame::OnEvihd1ZoomClose)
	ON_UPDATE_COMMAND_UI(ID_EVIHD1_ZOOM_CLOSE, &CMainFrame::OnUpdateEvihd1ZoomClose)
	ON_COMMAND(ID_EVIHD1_MANUAL, &CMainFrame::OnEvihd1Manual)
	ON_UPDATE_COMMAND_UI(ID_EVIHD1_MANUAL, &CMainFrame::OnUpdateEvihd1Manual)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);

	/*
	 *	初始化所有成员变量，同时打开图像采集卡
	 */
	CGSTATUS status = CG_OK;

	count_undetected	= 0;
	count_undetected_2	= 0;
	m_threshold		= 40;			//云台 运动的阈值
	m_pan			= 0x09;			//最大值0x18  云台水平旋转的速度
	m_title			= 0x07;			//最大值0x14  云台垂直旋转的速度
	speed1			= 0x02;			//云台变焦标准拉近的速度
	speed2			= 0x03;			//云台变焦标准拉远的速度

	m_CvPointCenter.x=-1;
	m_CvPointCenter.y=-1;
	m_radius		 =-1;

	m_bMoveStatus	= FALSE;
	m_bZoomStatus	= FALSE;
	m_bZoom			= FALSE;
	m_bPortOpen		= FALSE;

	m_bOpen			= FALSE;		
	m_bStart		= FALSE;

	m_bNegative		= FALSE;

	m_pBmpInfo		= NULL;
	m_pImageBuffer	= NULL;




	m_pIplImg		= NULL;


	cascadeName = "haarcascade_frontalface_alt2.xml";//主物体检测器名称和路径
	nestedCascadeName = "haarcascade_eye_tree_eyeglasses.xml";//嵌套（二级）物体检测器名称和路径

	tryflip = false;//标志是否翻转检测，默认关闭
	trynested = false;//标志是否进行二级检测，默认关闭

	scale = 1.3;//缩放比例
	cascadestatus = TRUE;
	nestedCascadestatus = TRUE;

	//载入主物体检测器
	if( !cascade.load( cascadeName ) )
	{
		cout<< "ERROR: Could not load classifier cascade" << endl;
		cascadestatus = FALSE;
	}
	//载入嵌套（二级）物体检测器
	if (!nestedCascade.load(nestedCascadeName))
	{
		cout<< "ERROR: Could not load classifier cascade" << endl;
		nestedCascadestatus = FALSE;
	}

	//打开图像卡1
	status = BeginCGCard(1, &m_hcg);
	//检查函数执行状态，如果失败，则返回错误状态消息框
	CG_VERIFY(status);
}

CMainFrame::~CMainFrame()
{
	CGSTATUS status = CG_OK;

	//关闭图像卡，释放图像卡内部资源
	status = EndCGCard(m_hcg);
	CG_VERIFY(status);

	//回收图像缓冲区
	if (m_pImageBuffer) {
		delete []m_pImageBuffer;
	}

}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 基于持久值设置视觉管理器和样式
	OnApplicationLook(theApp.m_nAppLook);

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("未能创建菜单栏\n");
		return -1;      // 未能创建
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// 防止菜单栏在激活时获得焦点
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));


	/*
	 *	初始化图像卡硬件状态，用户也可以在其他位置初始化图像卡，
	 *	但应保证图像卡已经打开，建议用户在应用程序初始化时，
	 *	同时初始化图像卡硬件
	 */

	//设置视频制式（PAL/NTSC）,由当前视频源制式决定
	CGSetVideoStandard(m_hcg, PAL);

	/*
	 *	视频格式，即采集图像数据描述方式，	
	 *	包括YUV422、RGB888、RGB565、RGB555、RGB8888、ALL8BIT、LIMITED8BIT，
	 *  在采集图像到屏幕时，需要保证视频格式和当前系统屏幕位深度一致，而采集到内存没有此限制。
	 */
	CGSetVideoFormat(m_hcg, RGB888);

	//扫描模式，包括FRAME、FIELD
	CGSetScanMode(m_hcg, FRAME);

	/*
	 *	晶振，包括CRY_OSC_35M、CRY_OSC_28M	
	 *	对于DH-CG300图像卡，一般为CRY_OSC_35M，对于DH-QP300图像卡，一般为CRY_OSC_28M，
	 *	其他类型图像卡没有此硬件设置，但可以调用此接口，并返回CG_NOT_SUPPORT_INTERFACE信息
	 */
	CGSelectCryOSC(m_hcg, CRY_OSC_35M);

	/*
	 *	设置视频源路
	 *  视频源路VIDEO_SOURCE包括视频类型和序号
	 *	各种图像卡支持的视频源路不尽相同，请参看相应硬件说明
	 */
	VIDEO_SOURCE source;
	source.type		= COMPOSITE_VIDEO;
	source.nIndex	= 0;
	CGSetVideoSource(m_hcg, source);

	/*
	 *	视频输入窗口，即视频输入范围，输入窗口取值范围：
	 *	对于视频制式为PAL制，水平方向为0-768，垂直方向为0-576
	 *	对于视频制式为NTSC制，水平方向为0-768，垂直方向为0-576
	 *  视频窗口左上角x坐标和窗口宽度应为4的倍数，左上角y坐标和窗口高度应为2的倍数
	 */
	CGSetInputWindow(m_hcg, 0, 0, 768, 576);

	/*
	 *	视频输出窗口，即视频输出范围，输出窗口取值范围必须在输入窗口范围以内
	 *	视频窗口左上角x坐标和窗口宽度应为4的倍数，左上角y坐标和窗口高度应为2的倍数
	 *	在采集到屏幕时，输出窗口的起始位置为图像屏幕输出位置的屏幕坐标，
	 *	在采集到内存中，输出窗口的起始位置设置为（0,0）即可。
	 */
	CGSetOutputWindow(m_hcg, 0, 0, 768, 576);

	/*
	 *	m_pBmpInfo即指向m_chBmpBuf缓冲区，用户可以自己分配BITMAPINFO缓冲区
	 */
	m_pBmpInfo								= (BITMAPINFO *)m_chBmpBuf;
	
	/*
	 *	初始化BITMAPINFO结构，此结构在保持BMP文件、显示采集图像时使用
	 */
	m_pBmpInfo->bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);

	//	图像宽度，一般为输出窗口宽度
	m_pBmpInfo->bmiHeader.biWidth			= 768;		
	
	/*
	 *	图像高度，根据扫描模式（FRAME/FIELD)的不同
	 *	FRAME制下，一般为输出窗口高度
	 *	FIELD制下，一般为输出窗口高度的一半
	 */
	m_pBmpInfo->bmiHeader.biHeight			= 576;		

	/*
	 *	图像位深度，由视频格式确定，
	 *	采集图像视频格式有RGB565、RGB555、RGB888、ALL8BIT等，
	 *  如果使用CGDateTransfrom函数，则将15,16位数据转换为24位
	 */
	m_pBmpInfo->bmiHeader.biBitCount		= 24;
	
	/*
	 *	以下设置一般相同
	 *	对于低于8位的位图，还应设置相应的位图调色板
	 */
	m_pBmpInfo->bmiHeader.biPlanes			= 1;	
	m_pBmpInfo->bmiHeader.biCompression		= BI_RGB;
	m_pBmpInfo->bmiHeader.biSizeImage		= 0;
	m_pBmpInfo->bmiHeader.biXPelsPerMeter	= 0;
	m_pBmpInfo->bmiHeader.biYPelsPerMeter	= 0;
	m_pBmpInfo->bmiHeader.biClrUsed			= 0;
	m_pBmpInfo->bmiHeader.biClrImportant	= 0;
	
	m_pImageBuffer = new BYTE[768 * 576 * 3];
	if (m_pImageBuffer) {
		FillMemory(m_pImageBuffer, 768 * 576 * 3, 0xff);
	}
	/*
	 *	分配图像缓冲区，一般用来存储采集图像
	 *	用户可以将设备静态内存的图像数据直接通过指针或
	 *	用CGDataTransfrom函数拷贝到图像缓冲区，然后做进一步的处理
	 *	一般图像缓冲区大小由输出窗口大小和视频格式确定。
	 */

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式


	cs.cx = 1400;			//设置整个框架窗口的宽度
	cs.cy = 650;			//设置真个框架窗口的高度

	cs.style=WS_OVERLAPPEDWINDOW;	//改变窗口标题的必要步骤(MSDN查阅window styles)
	cs.lpszName="智能云台控制软件";	//改变窗口标题

	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* 扫描菜单*/);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// 基类将执行真正的工作

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// 为所有用户工具栏启用自定义按钮
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}



void CMainFrame::OnSnapexOpen()
{
	// TODO: 在此添加命令处理程序代码
	CGSTATUS status = CG_OK;

	/*
	 *	初始化图像卡采集图像到内存的控制，
	 *	指定回调函数SnapThreadCallbackEx和用户参数m_hWnd
	 */
	status = CGOpenSnapEx(m_hcg, SnapThreadCallbackEx, m_hWnd);					
	CG_VERIFY(status);
	if (CG_SUCCESS(status)) {
		m_bOpen = TRUE;			//标志已经打开SnapEx环境
	}
}



/*
	函数:
		SnapThreadCallbackEx
	输入参数:
		SNAP_INFO *pInfo		SNAP_INFO结构包括当前图像卡SNAP执行状态
	输出参数:
		int						
	说明:
		图像卡采集到内存回调函数，但用户一般不用调用，由用户提供给SDK使用，
		用户在回调函数内实现对采集数据的处理和显示即可
*/
int CALLBACK CMainFrame::SnapThreadCallbackEx(SNAP_INFO *pInfo)
{
	HWND hwnd = (HWND)(pInfo->pParam);
	
	/*
	 *	发送自定义消息WM_SNAP_EX_CHANGE到主窗口，
	 *	同时传入当前可以处理的图像序号
	 *	注意：用SendMessage发送消息，必须等待消息处理完毕后，才能退出整个SendMessage函数
	 */
	::SendMessage(hwnd, WM_SNAP_EX_CHANGE, pInfo->nNumber, 0);

	return 1;
}

void CMainFrame::OnUpdateSnapexOpen(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable( !m_bOpen);
	pCmdUI->SetCheck(m_bOpen);
}



void CMainFrame::OnSnapexStart()
{
	// TODO: 在此添加命令处理程序代码
	CGSTATUS status = CG_OK;

	/*
	 *	启动图像卡采集图像到内存
	 *	指定图像卡将图像采集到静态内存偏移为0的位置
	 *	采集缓冲区大小为2幅图像大小
	 */
	status = CGStartSnapEx(m_hcg, 0, TRUE, 2);
	CG_VERIFY(status);
	if (CG_SUCCESS(status)) {
		m_bStart = TRUE;
	}
}


void CMainFrame::OnUpdateSnapexStart(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable( (!m_bStart) && (m_bOpen));
	pCmdUI->SetCheck(m_bStart);
}



void CMainFrame::OnSnapexStop()
{
	// TODO: 在此添加命令处理程序代码
	CGSTATUS status =CG_OK;

	//停止采集图像到内存，可以再次调用CGStartSnapEx启动图像卡采集
	status = CGStopSnapEx(m_hcg);
	CG_VERIFY(status);
	if (CG_SUCCESS(status)) {
		m_bStart = FALSE;
	}

	BYTE data1[] = { 0x81, 0x01, 0x04, 0x07, 0x00, 0xff };
	BYTE data2[] = { 0x81, 0x01, 0x06, 0x01, 0x01, 0x01, 0x03, 0x03, 0xff };

	if (m_bZoomStatus && m_bMoveStatus && m_bPortOpen)
	{
		BYTE data[] = {0x81, 0x01, 0x04, 0x07, 0x00, 0xff ,
					   0x81, 0x01, 0x06, 0x01, 0x01, 0x01, 0x03, 0x03, 0xff };
		m_Port.WriteToPort(data,sizeof(data));//当连续发送串口指令时，必须将两串指令合并成一个数组
		m_bZoomStatus = FALSE;
		m_bMoveStatus = FALSE;
	}
	if (m_bPortOpen && m_bZoom && m_bZoomStatus)
	{
		m_bZoomStatus = FALSE;
		m_Port.WriteToPort(data1,sizeof(data1));//停止变焦
	}

	if (m_bPortOpen && m_bMoveStatus)
	{
		m_bMoveStatus = FALSE;
		m_Port.WriteToPort(data2,sizeof(data2));//停止运动
	}
}



void CMainFrame::OnUpdateSnapexStop(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable( m_bStart);
}



void CMainFrame::OnSnapexClose()
{
	// TODO: 在此添加命令处理程序代码
	CGSTATUS status = CG_OK;

	/*
	 *	终止图像卡采集图像到内存，同时释放所有采集环境，
	 *	再次启动图像卡采集，必须重新初始化¯	
	 */
	status = CGCloseSnapEx(m_hcg);
	CG_VERIFY(status);

	if (CG_SUCCESS(status)) {
		m_bOpen		= FALSE;
		m_bStart	= FALSE;
	}

	BYTE data1[] = { 0x81, 0x01, 0x04, 0x07, 0x00, 0xff };
	BYTE data2[] = { 0x81, 0x01, 0x06, 0x01, 0x01, 0x01, 0x03, 0x03, 0xff };

	if (m_bZoomStatus && m_bMoveStatus && m_bPortOpen)
	{
		BYTE data[] = {0x81, 0x01, 0x04, 0x07, 0x00, 0xff ,
			0x81, 0x01, 0x06, 0x01, 0x01, 0x01, 0x03, 0x03, 0xff };
		m_Port.WriteToPort(data,sizeof(data));//当连续发送串口指令时，必须将两串指令合并成一个数组
		m_bZoomStatus = FALSE;
		m_bMoveStatus = FALSE;
	}

	if (m_bPortOpen && m_bZoom && m_bZoomStatus)
	{
		m_bZoomStatus = FALSE;
		m_Port.WriteToPort(data1,sizeof(data1));//停止变焦
	}

	if (m_bPortOpen && m_bMoveStatus)
	{
		m_bMoveStatus = FALSE;
		m_Port.WriteToPort(data2,sizeof(data2));//停止运动
	}
}




void CMainFrame::OnUpdateSnapexClose(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable( m_bOpen);
}




/*
	函数:
		OnSnapExChange
	输入参数:
		WPARAM wParam			字参数，在消息中为当前可以处理的图像序号
		LPARAM lParam			没有使用
	输出参数:
		LRESULT						
	说明:
		实现对此采集数据的处理和显示
 */
afx_msg LRESULT CMainFrame::OnSnapExChange(WPARAM wParam, LPARAM lParam)
{
	CGSTATUS status = CG_OK;

	m_wndSplitter.SetActivePane(0,0);//激活左边的VIEW视图
	
	CView *pView		= GetActiveView();		//获取当前VIEW视图
	CDC *pDC			= pView->GetDC();		//得到VIEW的DC
	
	BYTE *pLinearAddr = NULL;		//静态内存地址指针
	DWORD dwImageSize = 0;			//图像大小
	HANDLE handle = NULL;			//静态内存描述句柄
	
	dwImageSize = 768 * 576 * 3;	//计算图像大小，由视频输出窗口和视频格式确定

	BYTE *p = NULL, * q = NULL;

	/*
	 *	锁定指定位置的静态内存，
	 *	偏移由图像大小和图像序号确定，锁定大小为图像大小
	 *	用户可以在任何时候锁定位置的静态内存，然后通过pLinearAddr指针访问相应的内存。
	 */
	status = CGStaticMemLock(dwImageSize * wParam, dwImageSize, &handle, (PVOID *)&pLinearAddr);
	if (CG_SUCCESS(status)) {
		if (m_pImageBuffer) {
			/*
			 *	将静态内存中的图像传递到用户缓冲区，同时进行格式转换。
			 *	如果静态内存中图像位15、16、32位，则转换为24位。
			 *	由于图像采集卡采集到静态内存的图像数据是正向存放，
			 *	而Windows中处理的位图数据需要倒置，因此一般还要将图像倒置
			 */
			CGDataTransform(m_pImageBuffer,		//图像缓冲区
							pLinearAddr,		//静态内存
							768,				//图像宽度
							576,				//图像高度
							24,					//图像位深度
							TRUE				//是否倒置图像
							);
		}
		CGStaticMemUnlock(handle);				//解除静态内存锁定

		//进行图像反色
		if (m_bNegative) {
			for (int i = 0; i < 768; i++){
				p = m_pImageBuffer + i * 576 * 3;
				for(int j = 0; j < 576; j++){
					q = p + j * 3;
					*(q + 0) = ~(*(q + 0));
					*(q + 1) = ~(*(q + 1));
					*(q + 2) = ~(*(q + 2));
				}	
			}
		}
		
		//在视图客户区显示图像
		StretchDIBits(pDC->GetSafeHdc(),
						0,
						0,
						768,					//显示窗口宽度
						576,					//显示窗口高度
						0,
						0,
						768,					//图像宽度
						576,					//图像高度
						m_pImageBuffer,			//图像缓冲区
						m_pBmpInfo,				//BMP图像描述信息
						DIB_RGB_COLORS,
						SRCCOPY
						);	
		//将图像缓冲区的图像数据拷贝到opencv的图像对象中去
		m_pIplImg = cvCreateImage( cvSize(768, 576), IPL_DEPTH_8U, 3);
		CopyData(m_pIplImg->imageData, (char*)m_pImageBuffer, dwImageSize,true, 576);

		frame = m_pIplImg;//将图像复制给 帧frame
		if( !frame.empty() )
		{
			if( m_pIplImg->origin == IPL_ORIGIN_TL )//如果是左上为图像数据起点，即正排列
				frame.copyTo( frameCopy );//拷贝 图像数据
			else
				flip( frame, frameCopy, 0 );
			if (cascadestatus)
			{
				if (!nestedCascadestatus)
				{
					trynested = false;
				}

				if(		detectAndDraw( frameCopy, cascade, nestedCascade, scale, tryflip, trynested )	)
				{
					count_undetected = 0;
					count_undetected_2 = 0;

					BYTE data_Zoom_Stop[] = {0x81, 0x01, 0x04, 0x07, 0x00, 0xff};//停止变焦
					BYTE data0[] = {0x81, 0x01, 0x04, 0x07, 0x02, 0xff};//标准变焦拉近
					BYTE data1[] = {0x81, 0x01, 0x04, 0x07, 0x03, 0xff};//标准变焦拉远

					if (m_bPortOpen)
					{					
						int dx,dy;
						dx=m_CvPointCenter.x-m_pIplImg->width/2;		//人脸区域中心x坐标到图像中心的距离
						dy=m_CvPointCenter.y-m_pIplImg->height/2;		//人脸区域中心y坐标到图像中心的距离
						int pan_dx = (int)fabs((float)dx)/(26);
						int pan_dy = (int)fabs((float)dy)/(38);
						m_pan = 0x05 + pan_dx;							//最大值0x18
						m_title = 0x05 + pan_dy;						//最大值0x14

						//旋转
						if (dx<-m_threshold && dy<-m_threshold )//左上
						{
							m_bMoveStatus = TRUE;
							BYTE data[] = {0x81, 0x01, 0x06, 0x01, m_pan, m_title, 0x01, 0x01, 0xff };

							if(m_bPortOpen && m_bZoom && m_bZoomStatus)
							{
								m_bZoomStatus = FALSE;

								BYTE data[] = {0x81, 0x01, 0x06, 0x01, m_pan, m_title, 0x01, 0x01, 0xff,
									0x81, 0x01, 0x04, 0x07, 0x00, 0xff};//停止变焦
								//当连续发送串口指令时，必须将两个指令合并成一个数组
								m_Port.WriteToPort(data,sizeof(data));
							}					
							else if (m_bPortOpen)
							{
								m_Port.WriteToPort(data,sizeof(data));		
							}			

						} 
						else if (dx<-m_threshold && dy>m_threshold)//左下
						{
							m_bMoveStatus = TRUE;
							BYTE data[] = {0x81, 0x01, 0x06, 0x01, m_pan, m_title, 0x01, 0x02, 0xff};

							if(m_bPortOpen && m_bZoom && m_bZoomStatus)
							{
								BYTE data[] = {0x81, 0x01, 0x06, 0x01, m_pan, m_title, 0x01, 0x02, 0xff,
									0x81, 0x01, 0x04, 0x07, 0x00, 0xff};//停止变焦
								//当连续发送串口指令时，必须将两个指令合并成一个数组
								m_bZoomStatus = FALSE;
								m_Port.WriteToPort(data,sizeof(data));

							}
							else if(m_bPortOpen)
							{
								m_Port.WriteToPort(data,sizeof(data));	
							}

						}
						else if (dx>m_threshold && dy<-m_threshold)//右上
						{
							m_bMoveStatus = TRUE;
							BYTE data[] = {0x81, 0x01, 0x06, 0x01, m_pan, m_title, 0x02, 0x01, 0xff };

							if(m_bPortOpen && m_bZoom && m_bZoomStatus)
							{
								m_bZoomStatus = FALSE;

								BYTE data[] = {0x81, 0x01, 0x06, 0x01, m_pan, m_title, 0x02, 0x01, 0xff,
									0x81, 0x01, 0x04, 0x07, 0x00, 0xff};//停止变焦
								//当连续发送串口指令时，必须将两个指令合并成一个数组
								m_Port.WriteToPort(data,sizeof(data));
							}
							else if (m_bPortOpen)
							{						
								m_Port.WriteToPort(data,sizeof(data));
							}			
						}
						else if (dx>m_threshold && dy>m_threshold)//右下
						{
							m_bMoveStatus = TRUE;
							BYTE data[] = {0x81, 0x01, 0x06, 0x01, m_pan, m_title, 0x02, 0x02, 0xff };

							if(m_bPortOpen && m_bZoom && m_bZoomStatus)
							{
								m_bZoomStatus = FALSE;

								BYTE data[] = {0x81, 0x01, 0x06, 0x01, m_pan, m_title, 0x02, 0x02, 0xff,
									0x81, 0x01, 0x04, 0x07, 0x00, 0xff};//停止变焦
								//当连续发送串口指令时，必须将两个指令合并成一个数组
								m_Port.WriteToPort(data,sizeof(data));
							}
							else if(m_bPortOpen)
							{						
								m_Port.WriteToPort(data,sizeof(data));
							}
						}
						else if (dy<-m_threshold)//上
						{
							m_bMoveStatus = TRUE;
							BYTE data[] = {0x81, 0x01, 0x06, 0x01, m_pan, m_title, 0x03, 0x01, 0xff };

							if(m_bPortOpen && m_bZoom && m_bZoomStatus)
							{
								m_bZoomStatus = FALSE;

								BYTE data[] = {0x81, 0x01, 0x06, 0x01, m_pan, m_title, 0x03, 0x01, 0xff,
									0x81, 0x01, 0x04, 0x07, 0x00, 0xff};//停止变焦
								//当连续发送串口指令时，必须将两个指令合并成一个数组
								m_Port.WriteToPort(data,sizeof(data));
							}
							else if(m_bPortOpen)
							{
								m_Port.WriteToPort(data,sizeof(data));
							}

						}
						else if (dy>m_threshold)//下
						{
							m_bMoveStatus = TRUE;
							BYTE data[] = {0x81, 0x01, 0x06, 0x01, m_pan, m_title, 0x03, 0x02, 0xff};

							if(m_bPortOpen && m_bZoom && m_bZoomStatus)
							{
								m_bZoomStatus = FALSE;
								BYTE data[] = {0x81, 0x01, 0x06, 0x01, m_pan, m_title, 0x03, 0x02, 0xff,
									0x81, 0x01, 0x04, 0x07, 0x00, 0xff};//停止变焦
								//当连续发送串口指令时，必须将两个指令合并成一个数组
								m_Port.WriteToPort(data,sizeof(data));
							}					
							else if (m_bPortOpen)
							{
								m_Port.WriteToPort(data,sizeof(data));
							}
						}
						else if (dx<-m_threshold)//左
						{
							m_bMoveStatus = TRUE;
							BYTE data[] = {0x81, 0x01, 0x06, 0x01, m_pan, m_title, 0x01, 0x03, 0xff };

							if(m_bPortOpen && m_bZoom && m_bZoomStatus)
							{
								m_bZoomStatus = FALSE;
								BYTE data[] = {0x81, 0x01, 0x06, 0x01, m_pan, m_title, 0x01, 0x03, 0xff,
									0x81, 0x01, 0x04, 0x07, 0x00, 0xff};//停止变焦
								//当连续发送串口指令时，必须将两个指令合并成一个数组
								m_Port.WriteToPort(data,sizeof(data));
							}
							else if(m_bPortOpen)
							{
								m_Port.WriteToPort(data,sizeof(data));
							}
						}
						else if (dx>m_threshold)//右
						{
							m_bMoveStatus = TRUE;
							BYTE data[] = { 0x81, 0x01, 0x06, 0x01, m_pan, m_title, 0x02, 0x03, 0xff };

							if(m_bPortOpen && m_bZoom && m_bZoomStatus)
							{
								m_bZoomStatus = FALSE;
								BYTE data[] = { 0x81, 0x01, 0x06, 0x01, m_pan, m_title, 0x02, 0x03, 0xff,
									0x81, 0x01, 0x04, 0x07, 0x00, 0xff};//停止变焦
								//当连续发送串口指令时，必须将两个指令合并成一个数组
								m_Port.WriteToPort(data,sizeof(data));
							}					
							else if (m_bPortOpen)
							{
								m_Port.WriteToPort(data,sizeof(data));
							}
						}
						else//停止运动
						{
							if (m_bPortOpen && m_radius<=70 && m_bZoom)
							{
								m_bZoomStatus = TRUE;
								if (m_bMoveStatus)
								{
									BYTE data0[] = {0x81, 0x01, 0x04, 0x07, 0x02, 0xff,		//变焦拉近
										0x81, 0x01, 0x06, 0x01, 0x01, 0x01, 0x03, 0x03, 0xff};	//停止运动
									//当连续发送串口指令时，必须将两个指令合并成一个数组
									m_Port.WriteToPort(data0,sizeof(data0));
									m_bMoveStatus = FALSE;
								}
								else
									m_Port.WriteToPort(data0,sizeof(data0));//变焦拉近
							}
							else if (m_bPortOpen && m_radius>=90 && m_bZoom)
							{
								m_bZoomStatus = TRUE;
								if (m_bMoveStatus)
								{
									BYTE data1[] = {0x81, 0x01, 0x04, 0x07, 0x03, 0xff,		//变焦拉远
										0x81, 0x01, 0x06, 0x01, 0x01, 0x01, 0x03, 0x03, 0xff};	//停止运动
									//当连续发送串口指令时，必须将两个指令合并成一个数组
									m_Port.WriteToPort(data1,sizeof(data1));
									m_bMoveStatus = FALSE;
								}
								else
									m_Port.WriteToPort(data1,sizeof(data1));//变焦拉远
							}
							else if(m_bPortOpen && m_bZoomStatus)
							{
								m_bZoomStatus = FALSE;
								if (m_bMoveStatus)
								{
									BYTE data_Zoom_Stop[] = {0x81, 0x01, 0x04, 0x07, 0x00, 0xff,	//停止变焦
										0x81, 0x01, 0x06, 0x01, 0x01, 0x01, 0x03, 0x03, 0xff};			//停止运动
									//当连续发送串口指令时，必须将两个指令合并成一个数组
									m_Port.WriteToPort(data_Zoom_Stop,sizeof(data_Zoom_Stop));
									m_bMoveStatus = FALSE;
								}
								else
									m_Port.WriteToPort(data_Zoom_Stop,sizeof(data_Zoom_Stop));//停止变焦
							}
							else if(m_bPortOpen && m_bMoveStatus)
							{
								BYTE data_move_stop[] = {0x81, 0x01, 0x06, 0x01, 0x01, 0x01, 0x03, 0x03, 0xff};
								m_Port.WriteToPort(data_move_stop,sizeof(data_move_stop));
								m_bMoveStatus = FALSE;
							}
						}

					}
				}
				else
				{
					count_undetected ++;
					count_undetected_2 ++;
					BYTE data1[] = { 0x81, 0x01, 0x04, 0x07, 0x00, 0xff };//停止变焦
					BYTE data2[] = { 0x81, 0x01, 0x06, 0x01, 0x01, 0x01, 0x03, 0x03, 0xff };//停止运动
					BYTE data3[] = { 0x81, 0x01, 0x04, 0x07, 0x37, 0xff};//快速变焦拉远
					BYTE data4[] = { 0x81, 0x01, 0x06, 0x04, 0xff};//云台复位

					if (m_bPortOpen && m_bZoomStatus && m_bMoveStatus)
					{
						BYTE data[] = {0x81, 0x01, 0x04, 0x07, 0x00, 0xff,		//停止变焦
							0x81, 0x01, 0x06, 0x01, 0x01, 0x01, 0x03, 0x03, 0xff};	//停止运动
						//当连续发送串口指令时，必须将两个指令合并成一个数组
						m_Port.WriteToPort(data,sizeof(data));
						m_bMoveStatus = FALSE;
						m_bZoomStatus = FALSE;
					}
					if (m_bPortOpen && m_bZoom)
					{


						if (count_undetected <=100)
						{
							if (m_bZoomStatus)
							{
								m_Port.WriteToPort(data1,sizeof(data1));//停止变焦
								m_bZoomStatus = FALSE;
							}

						}
						else
						{
							count_undetected = 0;
							m_Port.WriteToPort(data3,sizeof(data3));//快速变焦拉远

							m_bZoomStatus = FALSE;					
						}			
					}

					if (m_bPortOpen && m_bMoveStatus)
					{
						m_bMoveStatus = FALSE;
						m_Port.WriteToPort(data2,sizeof(data2));//停止运动				
					}

					if (m_bPortOpen && count_undetected_2 > 160)
					{
						count_undetected_2 = 0;
						m_Port.WriteToPort(data4,sizeof(data4));//云台复位
					}

				}
			}
			
		}
		cvReleaseImage(&m_pIplImg);

	}
	
	pView->ReleaseDC(pDC);
	
	return 1;
}



void CMainFrame::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	/*
	 *	用户在没有通过菜单项正常关闭图像卡采集，
	 *	而直接关闭应用程序时，应保证图像卡采集被关闭
	 */
	if (m_bOpen) {
		CGCloseSnapEx(m_hcg);
	}

	if (m_bPortOpen)
	{
		BYTE data1[] = { 0x81, 0x01, 0x04, 0x07, 0x00, 0xff };
		BYTE data2[] = { 0x81, 0x01, 0x06, 0x01, 0x01, 0x01, 0x03, 0x03, 0xff };

		if (m_bZoomStatus && m_bMoveStatus && m_bPortOpen)
		{
			BYTE data[] = {0x81, 0x01, 0x04, 0x07, 0x00, 0xff ,
				0x81, 0x01, 0x06, 0x01, 0x01, 0x01, 0x03, 0x03, 0xff };
			m_Port.WriteToPort(data,sizeof(data));//当连续发送串口指令时，必须将两串指令合并成一个数组
			m_bZoomStatus = FALSE;
			m_bMoveStatus = FALSE;
		}

		if (m_bZoomStatus)
		{
			m_bZoom = FALSE;
			m_bZoomStatus= FALSE;
			m_Port.WriteToPort(data1,sizeof(data1));//停止变焦
		}
		if (m_bMoveStatus)
		{
			m_bMoveStatus = FALSE;
			m_Port.WriteToPort(data2,sizeof(data2));//停止运动
		}
		m_bPortOpen = FALSE;
		m_Port.ClosePort();
	}
	CFrameWndEx::OnClose();
}




void CMainFrame::OnNegative()
{
	// TODO: 在此添加命令处理程序代码
	m_bNegative = !m_bNegative;			//重置图像反色标志
}



void CMainFrame::OnUpdateNegative(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(m_bNegative);
}



bool CMainFrame::detectAndDraw(Mat& img, CascadeClassifier& cascade, CascadeClassifier& nestedCascade, double scale,bool tryflip,bool trynested)
{
	int i = 0;
	double t = 0;
	vector<Rect> faces, faces2;//用于保存人脸区域的向量
	const static Scalar colors[] =  { CV_RGB(0,0,255),
		CV_RGB(0,128,255),
		CV_RGB(0,255,255),
		CV_RGB(0,255,0),
		CV_RGB(255,128,0),
		CV_RGB(255,255,0),
		CV_RGB(255,0,0),
		CV_RGB(255,0,255)} ;
	Mat gray, smallImg( cvRound (img.rows/scale), cvRound(img.cols/scale), CV_8UC1 );

	cvtColor( img, gray, CV_BGR2GRAY );//图像灰度化
	resize( gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR );//将图像缩小化
	equalizeHist( smallImg, smallImg );//直方图 均衡化

	t = (double)cvGetTickCount();//计时开始

	//opencv中的人脸检测函数
	cascade.detectMultiScale( smallImg, faces,
		1.1, 4, 0
		|CV_HAAR_FIND_BIGGEST_OBJECT					//设置只返回最大人脸区域
		//|CV_HAAR_DO_ROUGH_SEARCH
		//CV_HAAR_SCALE_IMAGE
		,
		Size(30, 30) );

	if( tryflip && faces.empty())//如果尝试 翻转 检测的话
	{
		flip(smallImg, smallImg, 1);//翻转

		cascade.detectMultiScale( smallImg, faces2,
			1.1, 3, 0
			//|CV_HAAR_FIND_BIGGEST_OBJECT
			//|CV_HAAR_DO_ROUGH_SEARCH
			|CV_HAAR_SCALE_IMAGE
			,
			Size(30, 30) );
		for( vector<Rect>::const_iterator r = faces2.begin(); r != faces2.end(); r++ )
		{
			//将翻转检测出来的人脸区域加入face向量去
			faces.push_back(Rect(smallImg.cols - r->x - r->width, r->y, r->width, r->height));
		}
	}

	t = (double)cvGetTickCount() - t;//计时结束
	printf( "detection time = %g ms\n", t/((double)cvGetTickFrequency()*1000.) );

	//如果没有检测到人脸，在右视图显示原始图像
	if (faces.empty())
	{

		m_wndSplitter.SetActivePane(0,1);//激活右VIEW视图
		CView *pView		= GetActiveView();		//获取当前VIEW视图
		CDC *pDC			= pView->GetDC();

		DWORD dwImageSize = 0;			//图像大小
		dwImageSize = 768 * 576 * 3;

		BYTE *m_pImageBuffer = new BYTE[768 * 576 * 3];

		if (m_pImageBuffer) {
			FillMemory(m_pImageBuffer, 768 * 576 * 3, 0xff);
		}
		CopyData((char*)m_pImageBuffer, (char*)img.data, dwImageSize,true, 576);

		//在视图右侧客户区显示图像
		StretchDIBits(pDC->GetSafeHdc(),
			0,
			0,
			768,					//显示窗口宽度
			576,					//显示窗口高度
			0,
			0,
			768,					//图像宽度
			576,					//图像高度
			m_pImageBuffer,			//图像缓冲区
			m_pBmpInfo,				//BMP图像描述信息
			DIB_RGB_COLORS,
			SRCCOPY
			);

		delete []m_pImageBuffer;

		return false;
	}

	for( vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++ )
	{
		if ( (r+1)!=faces.end() )
		{
			continue;
		}

		Mat smallImgROI;						//用于嵌套检测的带有人脸检测区域信息的图像
		vector<Rect> nestedObjects;				//用于保存嵌套(二级)检测区域的向量

		Point center;							//局部变量，记录坐标

		Scalar color = colors[i%8];

		int radius;								//局部变量，记录半径

		m_radius = cvRound((r->width + r->height)*0.25*scale);			//赋值人脸区域的半径给，用于变焦控制
		m_CvPointCenter.x = cvRound((r->x + r->width*0.5)*scale);		//赋值人脸区域的中心位置x坐标，用于云台转动控制
		m_CvPointCenter.y = cvRound((r->y + r->height*0.5)*scale);		//赋值人脸区域的中心位置y坐标，用于云台转动控制

		double aspect_ratio = (double)r->width/r->height;				//计算人脸区域的宽高比

		//如果宽高比在0.75和1.3之间，用圆圈出人脸区域，否则用长方形圈出人脸区域
		if( 0.75 < aspect_ratio && aspect_ratio < 1.3 )
		{
			center.x = cvRound((r->x + r->width*0.5)*scale);
			center.y = cvRound((r->y + r->height*0.5)*scale);
			radius = cvRound((r->width + r->height)*0.25*scale);
			circle( img, center, radius, color, 3, 8, 0 );
		}
		else
			rectangle( img, cvPoint(cvRound(r->x*scale), cvRound(r->y*scale)),
			cvPoint(cvRound((r->x + r->width-1)*scale), cvRound((r->y + r->height-1)*scale)),
			color, 3, 8, 0);

		//如果嵌套物体检测器载入失败或者未设置嵌套检测，则不进行嵌套检测
		if( nestedCascade.empty() || !trynested )
			continue;

		//注入人脸区域信息到图像
		smallImgROI = smallImg(*r);

		//在包含人脸区域信息的图像中进行嵌套检测
		nestedCascade.detectMultiScale( smallImgROI, nestedObjects,
			1.1, 2, 0
			//|CV_HAAR_FIND_BIGGEST_OBJECT
			//|CV_HAAR_DO_ROUGH_SEARCH
			//|CV_HAAR_DO_CANNY_PRUNING
			|CV_HAAR_SCALE_IMAGE
			,
			Size(20, 20) );
		//圈出嵌套检测物体的区域
		for( vector<Rect>::const_iterator nr = nestedObjects.begin(); nr != nestedObjects.end(); nr++ )
		{
			center.x = cvRound((r->x + nr->x + nr->width*0.5)*scale);
			center.y = cvRound((r->y + nr->y + nr->height*0.5)*scale);
			radius = cvRound((nr->width + nr->height)*0.25*scale);
			circle( img, center, radius, color, 3, 8, 0 );
		}
	}

	m_wndSplitter.SetActivePane(0,1);			//激活右边的VIEW视图
	CView *pView		= GetActiveView();		//获取当前VIEW视图
	CDC *pDC			= pView->GetDC();

	DWORD dwImageSize = 0;			//图像大小
	dwImageSize = 768 * 576 * 3;

	BYTE *m_pImageBuffer = new BYTE[768 * 576 * 3];

	if (m_pImageBuffer) {
		FillMemory(m_pImageBuffer, 768 * 576 * 3, 0xff);
	}
	CopyData((char*)m_pImageBuffer, (char*)img.data, dwImageSize,true, 576);

	//在视图右侧客户区显示图像
	StretchDIBits(pDC->GetSafeHdc(),
		0,
		0,
		768,					//显示窗口宽度
		576,					//显示窗口高度
		0,
		0,
		768,					//图像宽度
		576,					//图像高度
		m_pImageBuffer,			//图像缓冲区
		m_pBmpInfo,				//BMP图像描述信息
		DIB_RGB_COLORS,
		SRCCOPY
		);

	delete []m_pImageBuffer;

	return true;
}


//图像数据拷贝函数
void CMainFrame::CopyData(char *dest,  const char *src, int dataByteSize,bool isConvert, int height)
{
	char * p = dest;

	if(!isConvert) {   
		memcpy(dest, src, dataByteSize);
		return;
	}
	if(height<=0) 
		return;

	//int height = dataByteSize/rowByteSize;
	int rowByteSize = dataByteSize / height;
	src = src + dataByteSize - rowByteSize ;

	for(int i=0; i<height; i++) 
	{
		memcpy(dest, src, rowByteSize);
		dest += rowByteSize;
		src  -= rowByteSize;
	}  
}


void CMainFrame::OnEvihd1Open()
{
	// TODO: 在此添加命令处理程序代码
	m_bPortOpen = TRUE;

	//初始化串口，如有必要请更改缺省的参数值
	if(m_Port.InitPort(this))
	{
		m_Port.StartMonitoring();//开始监听
	}
	else
		::MessageBox(NULL,"串口无法正常打开","error",MB_OK);
}



void CMainFrame::OnUpdateEvihd1Open(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(!m_bPortOpen);
}



void CMainFrame::OnEvihd1Close()
{
	// TODO: 在此添加命令处理程序代码
	m_bPortOpen = FALSE;
	m_bZoom		= FALSE;

	BYTE data1[] = { 0x81, 0x01, 0x04, 0x07, 0x00, 0xff };
	BYTE data2[] = { 0x81, 0x01, 0x06, 0x01, 0x01, 0x01, 0x03, 0x03, 0xff };

	if (m_bZoomStatus && m_bMoveStatus && m_bPortOpen)
	{
		BYTE data[] = {0x81, 0x01, 0x04, 0x07, 0x00, 0xff ,
			0x81, 0x01, 0x06, 0x01, 0x01, 0x01, 0x03, 0x03, 0xff };
		m_Port.WriteToPort(data,sizeof(data));//当连续发送串口指令时，必须将两串指令合并成一个数组
		m_bZoomStatus = FALSE;
		m_bMoveStatus = FALSE;
	}

	if (m_bZoomStatus)
	{
		m_bZoomStatus= FALSE;
		m_Port.WriteToPort(data1,sizeof(data1));//停止变焦
	}
	if (m_bMoveStatus)
	{
		m_bMoveStatus = FALSE;
		m_Port.WriteToPort(data2,sizeof(data2));//停止运动
	}



	m_Port.ClosePort();
}


void CMainFrame::OnUpdateEvihd1Close(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(m_bPortOpen);
}


void CMainFrame::OnEvihd1Zoom()
{
	// TODO: 在此添加命令处理程序代码
	m_bZoom = TRUE;
}


void CMainFrame::OnUpdateEvihd1Zoom(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(!m_bZoom && m_bPortOpen);
}


void CMainFrame::OnEvihd1ZoomClose()
{
	// TODO: 在此添加命令处理程序代码
	m_bZoom = FALSE;

	BYTE data_Zoom_Stop[] = {0x81, 0x01, 0x04, 0x07, 0x00, 0xff};
	if (m_bPortOpen && m_bZoomStatus)
	{
		m_bZoomStatus = FALSE;
		m_Port.WriteToPort(data_Zoom_Stop,sizeof(data_Zoom_Stop));//停止变焦
	}
}


void CMainFrame::OnUpdateEvihd1ZoomClose(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(m_bZoom && m_bPortOpen);
}


void CMainFrame::OnEvihd1Manual()
{
	// TODO: 在此添加命令处理程序代码
	if( WinExec("CameraPtz.exe",SW_SHOW) <= 31)
	{
		::MessageBox(NULL,"打开手动云台控制失败","error",MB_OK);
	}
}


void CMainFrame::OnUpdateEvihd1Manual(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(!m_bPortOpen);
}


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 在此添加专用代码和/或调用基类

	//将视图区一分为二
	if (!m_wndSplitter.CreateStatic(this,1,2))
	{
		return FALSE;
	}

	//设置每个视图的视图类
	if (!m_wndSplitter.CreateView(0,0,RUNTIME_CLASS(CCGRealtime_2010_00View),CSize(632,576),pContext) ||
		!m_wndSplitter.CreateView(0,1,RUNTIME_CLASS(CSplitView),CSize(768,576),pContext))
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}
	return TRUE;
}
