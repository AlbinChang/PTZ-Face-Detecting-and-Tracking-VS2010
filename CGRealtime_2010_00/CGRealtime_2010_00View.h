
// CGRealtime_2010_00View.h : CCGRealtime_2010_00View ��Ľӿ�
//

#pragma once

class CCGRealtime_2010_00Doc;
class CCGRealtime_2010_00View : public CView
{
protected: // �������л�����
	CCGRealtime_2010_00View();
	DECLARE_DYNCREATE(CCGRealtime_2010_00View)

// ����
public:
	CCGRealtime_2010_00Doc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CCGRealtime_2010_00View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // CGRealtime_2010_00View.cpp �еĵ��԰汾
inline CCGRealtime_2010_00Doc* CCGRealtime_2010_00View::GetDocument() const
   { return reinterpret_cast<CCGRealtime_2010_00Doc*>(m_pDocument); }
#endif

