// SplitView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CGRealtime_2010_00.h"
#include "SplitView.h"


// CSplitView

IMPLEMENT_DYNCREATE(CSplitView, CView)

CSplitView::CSplitView()
{

}

CSplitView::~CSplitView()
{
}

BEGIN_MESSAGE_MAP(CSplitView, CView)
END_MESSAGE_MAP()


// CSplitView ��ͼ

void CSplitView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: �ڴ���ӻ��ƴ���
}


// CSplitView ���

#ifdef _DEBUG
void CSplitView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CSplitView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSplitView ��Ϣ�������
