
// CGRealtime_2010_00View.cpp : CCGRealtime_2010_00View 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "CGRealtime_2010_00.h"
#endif

#include "CGRealtime_2010_00Doc.h"
#include "CGRealtime_2010_00View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCGRealtime_2010_00View

IMPLEMENT_DYNCREATE(CCGRealtime_2010_00View, CView)

BEGIN_MESSAGE_MAP(CCGRealtime_2010_00View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CCGRealtime_2010_00View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CCGRealtime_2010_00View 构造/析构

CCGRealtime_2010_00View::CCGRealtime_2010_00View()
{
	// TODO: 在此处添加构造代码

}

CCGRealtime_2010_00View::~CCGRealtime_2010_00View()
{
}

BOOL CCGRealtime_2010_00View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CCGRealtime_2010_00View 绘制

void CCGRealtime_2010_00View::OnDraw(CDC* /*pDC*/)
{
	CCGRealtime_2010_00Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CCGRealtime_2010_00View 打印


void CCGRealtime_2010_00View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CCGRealtime_2010_00View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CCGRealtime_2010_00View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CCGRealtime_2010_00View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CCGRealtime_2010_00View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CCGRealtime_2010_00View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	//theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CCGRealtime_2010_00View 诊断

#ifdef _DEBUG
void CCGRealtime_2010_00View::AssertValid() const
{
	CView::AssertValid();
}

void CCGRealtime_2010_00View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCGRealtime_2010_00Doc* CCGRealtime_2010_00View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCGRealtime_2010_00Doc)));
	return (CCGRealtime_2010_00Doc*)m_pDocument;
}
#endif //_DEBUG


// CCGRealtime_2010_00View 消息处理程序
