#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemStlgView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCMSectItemStlgView::CCMSectItemStlgView(CWnd* pParent /*=NULL*/)
	: CCMDlgBase(CCMSectItemStlgView::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectItemStlgView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pSect = NULL;
	m_pBitmap = 0;
	m_nBitmapID = 0;
}

CCMSectItemStlgView::~CCMSectItemStlgView()
{
	m_pSect = NULL;

	if (m_pBitmap) 
	{
		m_pBitmap->DeleteObject();
		delete m_pBitmap;
	}  
}

void CCMSectItemStlgView::DoDataExchange(CDataExchange* pDX)
{
	CCMDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectItemStlgView)
	DDX_Control(pDX, IDC_CMD_BITMAP1_FRM, m_ImgSectItem);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMSectItemStlgView, CCMDlgBase)
	//{{AFX_MSG_MAP(CCMSectItemStlgView)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//	ON_BN_CLICKED(IDC_CMD_SECT_CENTROID_BTN, OnCmdSectCentroidBtn)

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemStlgView message handlers


/////////////////////////////////////////////////////////////////////////////
// Change Bitmap
//
BOOL CCMSectItemStlgView::ChangeBitmap()
{
	// 이 함수를 부르기 전에 꼭 Dlg2Data(TRUE); 를 불러주세요.
	BOOL bResult = FALSE;
// 	CBCGPStatic* pImage = (CBCGPStatic *)GetDlgItem(IDC_CMD_BITMAP1_FRM);
// 	if(!(::IsWindow(pImage->GetSafeHwnd()))) return FALSE;

	CString strImgPath_Sect;
	int nBitmapResID = GetResIDBySect(strImgPath_Sect);
	if(nBitmapResID == -1) return FALSE;

	if(m_nBitmapID == nBitmapResID) return TRUE;

// 	if (m_pBitmap) { m_pBitmap->DeleteObject(); delete m_pBitmap; }
// 	m_pBitmap = new CBitmap;
// 	m_pBitmap->LoadBitmap((UINT)nBitmapResID);
// 	pImage->SetBitmap(HBITMAP(*m_pBitmap));
	m_ImgSectItem.SetImage(strImgPath_Sect);

	m_nBitmapID = nBitmapResID;  // 같은 bitmap이면 Load 안하기 위해 현재 Bitmap ID 저장
 
	return bResult;
}

int CCMSectItemStlgView::GetResIDBySect(CString& strImgPath_Sect)
{
	if(!m_pSect)
	{
		return -1;
	}

	int nType;
	nType = m_pSect->nStype;
	if(m_pSect->nStype == D_SECT_TYPE_TAPERED)
	{
		nType = m_pSect->SectBefore.nStype;
	}

	switch (nType)
	{
	case D_SECT_TYPE_STLG_MCELL:
		return GetResIDBMCell(strImgPath_Sect);

	default:
		ASSERT(0);
		return -1;
		break;
	}  
}

int CCMSectItemStlgView::GetResIDBMCell(CString& strImgPath_Sect)
{
	int nType;
	nType = m_pSect->nStype;
	if(m_pSect->nStype == D_SECT_TYPE_TAPERED)
	{
		nType = m_pSect->SectBefore.nStype;
	}

	ASSERT(nType == D_SECT_TYPE_STLG_MCELL);

	switch (m_pSect->SectBefore.nFlangeShape)
	{
	case 0: // General
		{
			if(m_pSect->SectBefore.nCellNum == 0) // Half
			{
				if(m_pSect->SectBefore.nCellType == 0) // Left
				{
					strImgPath_Sect = _T("SVG\\illustration\\Dialog\\MCellGenHalfLeft.svg");
					return IDB_CMD_STLG_MCELL_GEN_HALFL;
				}
				else if(m_pSect->SectBefore.nCellType == 1) // Right
				{
					strImgPath_Sect = _T("SVG\\illustration\\Dialog\\MCellGenHalfRight.svg");
					return IDB_CMD_STLG_MCELL_GEN_HALFR;
				}
				else {ASSERT(0); return -1;}
			}
			else if(m_pSect->SectBefore.nCellNum == 1)
			{
				strImgPath_Sect = _T("SVG\\illustration\\Dialog\\MCellGen1Cell.svg");
				return IDB_CMD_STLG_MCELL_GEN_1CELL;
			}
			else if(m_pSect->SectBefore.nCellNum == 2)
			{
				strImgPath_Sect = _T("SVG\\illustration\\Dialog\\MCellGenEven.svg");
				return IDB_CMD_STLG_MCELL_GEN_EVEN;
			}
			else
			{
				strImgPath_Sect = _T("SVG\\illustration\\Dialog\\MCellGenOdd.svg");
				return IDB_CMD_STLG_MCELL_GEN_ODD;
			}
		}
		break;
	case 1: // Rebar-1
		{
			if(m_pSect->SectBefore.nCellNum == 0) // Half
			{
				if(m_pSect->SectBefore.nCellType == 0) // Left
				{
					strImgPath_Sect = _T("SVG\\illustration\\Dialog\\MCellRebar1HalfLeft.svg");
					return IDB_CMD_STLG_MCELL_REBAR1_HALFL;
				}
				else if(m_pSect->SectBefore.nCellType == 1) // Right
				{
					strImgPath_Sect = _T("SVG\\illustration\\Dialog\\MCellRebar1HalfRight.svg");
					return IDB_CMD_STLG_MCELL_REBAR1_HALFR;
				}
				else {ASSERT(0); return -1;}
			}
			else if(m_pSect->SectBefore.nCellNum == 1)
			{
				strImgPath_Sect = _T("SVG\\illustration\\Dialog\\MCellRebar11Cell.svg");
				return IDB_CMD_STLG_MCELL_REBAR1_1CELL;
			}
			else if(m_pSect->SectBefore.nCellNum == 2)
			{
				strImgPath_Sect = _T("SVG\\illustration\\Dialog\\MCellRebar1Even.svg");
				return IDB_CMD_STLG_MCELL_REBAR1_EVEN;
			}
			else
			{
				strImgPath_Sect = _T("SVG\\illustration\\Dialog\\MCellRebar1Odd.svg");
				return IDB_CMD_STLG_MCELL_REBAR1_ODD;
			}
		}
		break;
	case 2: // Rebar-2
		{
			if(m_pSect->SectBefore.nCellNum == 0) // Half
			{
				if(m_pSect->SectBefore.nCellType == 0) // Left
				{
					strImgPath_Sect = _T("SVG\\illustration\\Dialog\\MCellRebar2HalfLeft.svg");
					return IDB_CMD_STLG_MCELL_REBAR2_HALFL;
				}
				else if(m_pSect->SectBefore.nCellType == 1) // Right
				{
					strImgPath_Sect = _T("SVG\\illustration\\Dialog\\MCellRebar2HalfRight.svg");
					return IDB_CMD_STLG_MCELL_REBAR2_HALFR;
				}
				else {ASSERT(0); return -1;}
			}
			else if(m_pSect->SectBefore.nCellNum == 1)
			{
				strImgPath_Sect = _T("SVG\\illustration\\Dialog\\MCellRebar21Cell.svg");
				return IDB_CMD_STLG_MCELL_REBAR2_1CELL;
			}
			else if(m_pSect->SectBefore.nCellNum == 2) // 짝수
			{
				strImgPath_Sect = _T("SVG\\illustration\\Dialog\\MCellRebar2Even.svg");
				return IDB_CMD_STLG_MCELL_REBAR2_EVEN;
			}
			else
			{
				strImgPath_Sect = _T("SVG\\illustration\\Dialog\\MCellRebar2Odd.svg");
				return IDB_CMD_STLG_MCELL_REBAR2_ODD;
			}
		}
		break;
	case 3: // Closed
		{
			if(m_pSect->SectBefore.nCellNum == 0) // Half
			{
				if(m_pSect->SectBefore.nCellType == 0) // Left
				{
					strImgPath_Sect = _T("SVG\\illustration\\Dialog\\MCellClosedHalfLeft.svg");
					return IDB_CMD_STLG_MCELL_CLOSED_HALFL;
				}
				else if(m_pSect->SectBefore.nCellType == 1) // Right
				{
					strImgPath_Sect = _T("SVG\\illustration\\Dialog\\MCellClosedHalfRight.svg");
					return IDB_CMD_STLG_MCELL_CLOSED_HALFR;
				}
				else {ASSERT(0); return -1;}
			}
			else if(m_pSect->SectBefore.nCellNum == 1)
			{
				strImgPath_Sect = _T("SVG\\illustration\\Dialog\\MCellClosed1Cell.svg");
				return IDB_CMD_STLG_MCELL_CLOSED_1CELL;
			}
			else if(m_pSect->SectBefore.nCellNum == 2)
			{
				strImgPath_Sect = _T("SVG\\illustration\\Dialog\\MCellClosedEven.svg");
				return IDB_CMD_STLG_MCELL_CLOSED_EVEN;
			}
			else
			{
				strImgPath_Sect = _T("SVG\\illustration\\Dialog\\MCellClosedOdd.svg");
				return IDB_CMD_STLG_MCELL_CLOSED_ODD;
			}
		}
		break;
	default:
		break;
	}
	return -1;
}

void CCMSectItemStlgView::SetDataSource(T_SECT_D* pDataSrc)
{  
	ASSERT(pDataSrc);
	m_wndSecView.SetDataSource(pDataSrc, FALSE);  
	m_pSect = pDataSrc;
	m_wndSecView.SetShearCheckFlag(TRUE);
}

void CCMSectItemStlgView::RedrawSection(BOOL bCentroid/*=FALSE*/)
{
	if(!m_pSect)
	{
		AfxMessageBox(_T("Error"));
		return;
	}

	HWND hWnd;
	if(hWnd = m_wndSecView.GetSafeHwnd())
	{
		if(::IsWindow(hWnd)) 
		{
			m_wndSecView.SetCentroidFlag(bCentroid);
			m_wndSecView.Invalidate();
		}
	}  
}

void CCMSectItemStlgView::ShowSectView(BOOL bShow)
{
	HWND hWnd = m_wndSecView.GetSafeHwnd();
	if (hWnd && (::IsWindow(hWnd)))
	{
		// View를 숨기고 다이얼로그 크기 조절 
		CWnd* pWnd;
		pWnd = GetDlgItem(IDC_CMD_PSC_VIEW_STATIC); ASSERT(pWnd);
		pWnd->ShowWindow(bShow ? SW_SHOW : SW_HIDE);
		pWnd = GetDlgItem(IDC_CMD_VIEW_FRM); ASSERT(pWnd);
		pWnd->ShowWindow(bShow ? SW_SHOW : SW_HIDE);

		CRect rectLast, r;
		if (bShow)  
			pWnd = GetDlgItem(IDC_CMD_PSC_VIEW_STATIC);
		else
			pWnd = GetDlgItem(IDC_CMD_BITMAP1_FRM);
		if (pWnd == 0) { ASSERT(0); return; }
		
		pWnd->GetWindowRect(rectLast);
		this->GetWindowRect(r);
		r.bottom = rectLast.bottom+10;
		this->MoveWindow(r);
	}
}

/////////////////////////////////////////////////////////////////////////////
//
// Redraw - Update Viewer
//
BOOL CCMSectItemStlgView::OnInitDialog() 
{
	CCMDlgBase::OnInitDialog();
	SetIgnoreBottomBtnArea(TRUE);
	// TODO: Add extra initialization here
	CWnd* pWnd = GetDlgItem(IDC_CMD_VIEW_FRM);
	ASSERT(pWnd);
	m_wndSecView.Init(pWnd);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMSectItemStlgView::OnSysCommand(UINT nID, LPARAM lParam) 
{
	// TODO: Add your message handler code here and/or call default
	// Close를 막아준다.
	if(nID == SC_CLOSE)
	{
		return;
	}

	CCMDlgBase::OnSysCommand(nID, lParam);
}

void CCMSectItemStlgView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
//	ChangeBitmap(m_nCurIndex);

	// Do not call CCMDlgBase::OnPaint() for painting messages
}

BOOL CCMSectItemStlgView::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CCMDlgBase::DestroyWindow();
}
