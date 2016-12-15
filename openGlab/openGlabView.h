
// openGlabView.h : interface of the CopenGlabView class
//

#pragma once
#include "Renderer.h"

class CopenGlabView : public CView
{
protected:
	Renderer renderer;

protected: // create from serialization only
	CopenGlabView();
	DECLARE_DYNCREATE(CopenGlabView)

// Attributes
public:
	CopenGlabDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	void OnInitialUpdate();

// Implementation
public:
	virtual ~CopenGlabView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // debug version in openGlabView.cpp
inline CopenGlabDoc* CopenGlabView::GetDocument() const
   { return reinterpret_cast<CopenGlabDoc*>(m_pDocument); }
#endif

