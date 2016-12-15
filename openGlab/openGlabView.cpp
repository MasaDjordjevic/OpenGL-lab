
// openGlabView.cpp : implementation of the CopenGlabView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "openGlab.h"
#endif

#include "openGlabDoc.h"
#include "openGlabView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CopenGlabView

IMPLEMENT_DYNCREATE(CopenGlabView, CView)

BEGIN_MESSAGE_MAP(CopenGlabView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CopenGlabView construction/destruction

CopenGlabView::CopenGlabView()
{
	// TODO: add construction code here

}

CopenGlabView::~CopenGlabView()
{
}

BOOL CopenGlabView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CopenGlabView drawing

void CopenGlabView::OnDraw(CDC* pDC)
{
	CopenGlabDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	renderer.DrawScene(pDC);
	
}


// CopenGlabView printing

BOOL CopenGlabView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CopenGlabView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CopenGlabView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CopenGlabView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CDC* pDC = GetDC();
	renderer.PrepareScene(pDC);
	ReleaseDC(pDC);
}


// CopenGlabView diagnostics

#ifdef _DEBUG
void CopenGlabView::AssertValid() const
{
	CView::AssertValid();
}

void CopenGlabView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CopenGlabDoc* CopenGlabView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CopenGlabDoc)));
	return (CopenGlabDoc*)m_pDocument;
}
#endif //_DEBUG


// CopenGlabView message handlers


int CopenGlabView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CDC* pDC = GetDC();
	renderer.CreateGLContext(pDC);
	ReleaseDC(pDC);

	return 0;
}


BOOL CopenGlabView::OnEraseBkgnd(CDC* pDC)
{
	return true;
}


void CopenGlabView::OnDestroy()
{
	CView::OnDestroy();

	CDC* pDC = GetDC();
	renderer.DestroyScene(pDC);
	ReleaseDC(pDC);
}


void CopenGlabView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CDC* pDC = GetDC();
	renderer.Reshape(pDC, cx, cy);
	ReleaseDC(pDC);
}


void CopenGlabView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
	double step = 5;
	switch (nChar)
	{
	case VK_UP:
		renderer.ChangeViewAngleX(step);
		break;
	case VK_DOWN:
		renderer.ChangeViewAngleX(-step);
		break;
	case VK_LEFT:
		renderer.ChangeViewAngleY(step);
		break;
	case VK_RIGHT:
		renderer.ChangeViewAngleY(-step);
		break;
	case VK_ADD:
		renderer.ChangeViewZoom(-step);
		break;
	case VK_SUBTRACT:
		renderer.ChangeViewZoom(step);
		break;
	case 'Q':
		renderer.ChangeLampLowerAngle(-step);
		break;		   
	case 'A':		   
		renderer.ChangeLampLowerAngle(step);
		break;		   
	case 'W':		   
		renderer.ChangeLampUpperAngle(step);
		break;		   
	case 'S':		   
		renderer.ChangeLampUpperAngle(-step);
		break;		   
	case 'E':		   
		renderer.ChangeLampHeadAngle(step);
		break;		   
	case 'D':		   
		renderer.ChangeLampHeadAngle(-step);
		break;
	}

	Invalidate();
}
