
// CGRealtime_2010_00View.cpp : CCGRealtime_2010_00View ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CCGRealtime_2010_00View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CCGRealtime_2010_00View ����/����

CCGRealtime_2010_00View::CCGRealtime_2010_00View()
{
	// TODO: �ڴ˴���ӹ������

}

CCGRealtime_2010_00View::~CCGRealtime_2010_00View()
{
}

BOOL CCGRealtime_2010_00View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CCGRealtime_2010_00View ����

void CCGRealtime_2010_00View::OnDraw(CDC* /*pDC*/)
{
	CCGRealtime_2010_00Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CCGRealtime_2010_00View ��ӡ


void CCGRealtime_2010_00View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CCGRealtime_2010_00View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CCGRealtime_2010_00View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CCGRealtime_2010_00View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
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


// CCGRealtime_2010_00View ���

#ifdef _DEBUG
void CCGRealtime_2010_00View::AssertValid() const
{
	CView::AssertValid();
}

void CCGRealtime_2010_00View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCGRealtime_2010_00Doc* CCGRealtime_2010_00View::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCGRealtime_2010_00Doc)));
	return (CCGRealtime_2010_00Doc*)m_pDocument;
}
#endif //_DEBUG


// CCGRealtime_2010_00View ��Ϣ�������
