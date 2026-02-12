// CMSectItmePSCView.cpp : implementation file
// PSC 단면 선택시 같이 뜨는 View Dialog Class
// 이 클래스에서는 Section View와 Bitmap Frame을 가지며 
// PSC 단면타입 변경시 해당 Bitmap을 붙여주고 연결된 DataSrc에 알맞는 단면을 그려준다.

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItmePSCView.h"

//#include "CMSectItemPSC.h"
#include "CCMSectItemPSCDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSectItmePSCView dialog


CCMSectItmePSCView::CCMSectItmePSCView(CWnd* pParent /*=NULL*/)
	: CCMDlgBase(CCMSectItmePSCView::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectItmePSCView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	SetIgnoreBottomBtnArea(); // 구분선 삭제

	m_bSetData = FALSE;
	m_nCurIndex = __SECT_PSC_NML_2_CELL__;
	m_pBitmap = 0;
	m_nBitmapID = 0;
}

CCMSectItmePSCView::~CCMSectItmePSCView()
{
	if (m_pBitmap) 
	{
		m_pBitmap->DeleteObject();
		delete m_pBitmap;
	}
}

void CCMSectItmePSCView::DoDataExchange(CDataExchange* pDX)
{
	CCMDlgBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMD_PICTURE, m_wndPicture);

	//{{AFX_DATA_MAP(CCMSectItmePSCView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMSectItmePSCView, CCMDlgBase)
	//{{AFX_MSG_MAP(CCMSectItmePSCView)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//	ON_BN_CLICKED(IDC_CMD_SECT_CENTROID_BTN, OnCmdSectCentroidBtn)

/////////////////////////////////////////////////////////////////////////////
// CCMSectItmePSCView message handlers


/////////////////////////////////////////////////////////////////////////////
// Change Bitmap
//
// *^^* 타입추가시 여기 추가 
// 단면 ID는 CCMSectItemPSCDefine.h 파일에서 Define
BOOL CCMSectItmePSCView::ChangeBitmapOld(UINT nIndex)
{
	return TRUE;

// 	BOOL bResult = FALSE;
// 	CBCGPStatic* pImage = (CBCGPStatic *)GetDlgItem(IDC_CMD_PICTURE);
// 	if(!(::IsWindow(pImage->GetSafeHwnd()))) return FALSE;
// 
// 	UINT aBitmap[][2] = {
// 		{__SECT_PSC_NML_1_CELL__, IDB_CMD_PSC_HALF_01 },
// 		{__SECT_PSC_NML_2_CELL__, IDB_CMD_PSC_2CELL_01},
// 		{__SECT_PSC_MID_NONE_NONE__, IDB_CMD_PSC_MID_03},
// 		{__SECT_PSC_MID_NONE_CIRC__, IDB_CMD_PSC_MID_09},
// 		{__SECT_PSC_MID_CIRC_NONE__, IDB_CMD_PSC_MID_06},
// 		{__SECT_PSC_MID_CIRC_CIRC__, IDB_CMD_PSC_MID_02},
// 		{__SECT_PSC_MID_POLY_NONE__, IDB_CMD_PSC_MID_07},
// 		{__SECT_PSC_MID_NONE_POLY__, IDB_CMD_PSC_MID_04},
// 		{__SECT_PSC_MID_POLY_POLY__, IDB_CMD_PSC_MID_01},
// 		{__SECT_PSC_MID_CIRC_POLY__, IDB_CMD_PSC_MID_05},
// 		{__SECT_PSC_MID_POLY_CIRC__, IDB_CMD_PSC_MID_08},
// 		{__SECT_PSC_I__            , IDB_CMD_PSC_I_01},
// 		{__SECT_PSC_HLF_LEFT_NONE__, IDB_CMD_PSC_HALF_02},
// 		{__SECT_PSC_HLF_LEFT_CIRC__, IDB_CMD_PSC_HALF_03},
// 		{__SECT_PSC_HLF_LEFT_POLY__, IDB_CMD_PSC_HALF_01},
// 		{__SECT_PSC_HLF_RGHT_NONE__, IDB_CMD_PSC_HALF_06},
// 		{__SECT_PSC_HLF_RGHT_CIRC__, IDB_CMD_PSC_HALF_05},
// 		{__SECT_PSC_HLF_RGHT_POLY__, IDB_CMD_PSC_HALF_04},
// 		{__SECT_PSC_TEE__          , IDB_CMD_PSC_TEE},
// 		{__SECT_PSC_NML_3_CELL__   , IDB_CMD_PSC_3Cell},
// 		{__SECT_PSC_NML_4_CELL__   , IDB_CMD_PSC_4Cell},
// 		{__SECT_PSC_nCELL_POLY1_LEFT__  , IDB_CMD_PSC_NCELL_POLY1_LEFT},
// 		{__SECT_PSC_nCELL_POLY1_RGHT__  , IDB_CMD_PSC_NCELL_POLY1_RIGHT},
// 		{__SECT_PSC_nCELL_POLY2_LEFT__  , IDB_CMD_PSC_NCELL_POLY2_LEFT},
// 		{__SECT_PSC_nCELL_POLY2_RGHT__  , IDB_CMD_PSC_NCELL_POLY2_RIGHT},
// 		{__SECT_PSC_nCELL_CIRC_LEFT__   , IDB_CMD_PSC_NCELL_CIRC_LEFT},
// 		{__SECT_PSC_nCELL_CIRC_RGHT__   , IDB_CMD_PSC_NCELL_CIRC_RIGHT},
// 		{__SECT_PSC_PLAT_HALF_LEFT__, IDB_CMD_PSC_PLAT_02},
// 		{__SECT_PSC_PLAT_HALF_RGHT__, IDB_CMD_PSC_PLAT_03},
// 		{__SECT_PSC_PLAT_1CEL_POLY__, IDB_CMD_PSC_PLAT_04},
// 		{__SECT_PSC_PLAT_1CEL_CIRC__, IDB_CMD_PSC_PLAT_01},
// 		{__SECT_PSC_PLAT_2CEL__     , IDB_CMD_PSC_PLAT_05},
// 		{__SECT_PSC_VALUE__         , IDB_CMD_PSC_VALUE },
// 		{__SECT_PSC_CMPWEB__        , IDB_CMD_PSC_CMP },
// 		{__SECT_PSC_CMPWEB_TAP__    , IDB_CMD_PSC_CMP_TAP },
// 	};
// 
// 	int nCount = sizeof(aBitmap)/sizeof(aBitmap[0]);
// 	int i = 0;
// 	for (i = 0; i < nCount; i++)
// 		if (nIndex == (int)aBitmap[i][0]) break;
// 	if (i == nCount) { ASSERT(0); return FALSE; }
// 
// 	if (m_nBitmapID == aBitmap[i][1]) return TRUE;  // 같은 bitmap이면 Load 안함
// 
// 	if (m_pBitmap) { m_pBitmap->DeleteObject(); delete m_pBitmap; }
// 	m_pBitmap = new CBitmap;
// 	m_pBitmap->LoadBitmap(aBitmap[i][1]);
// 	pImage->SetBitmap(HBITMAP(*m_pBitmap));
// 
// 	m_nCurIndex = nIndex;
// 	m_nBitmapID = aBitmap[i][1];  // 같은 bitmap이면 Load 안하기 위해 현재 Bitmap ID 저장
//  
// 	return bResult;
}

BOOL CCMSectItmePSCView::ChangeBitmap(UINT nIndex)
{
	BOOL bResult = FALSE;
	UINT aSectIdx[] = {
		{__SECT_PSC_NML_1_CELL__   },       // IDB_CMD_PSC_HALF_01 },
		{__SECT_PSC_NML_2_CELL__   },		// IDB_CMD_PSC_2CELL_01},
		{__SECT_PSC_MID_NONE_NONE__},		// IDB_CMD_PSC_MID_03},
		{__SECT_PSC_MID_NONE_CIRC__},		// IDB_CMD_PSC_MID_09},
		{__SECT_PSC_MID_CIRC_NONE__},		// IDB_CMD_PSC_MID_06},
		{__SECT_PSC_MID_CIRC_CIRC__},		// IDB_CMD_PSC_MID_02},
		{__SECT_PSC_MID_POLY_NONE__},		// IDB_CMD_PSC_MID_07},
		{__SECT_PSC_MID_NONE_POLY__},		// IDB_CMD_PSC_MID_04},
		{__SECT_PSC_MID_POLY_POLY__},		// IDB_CMD_PSC_MID_01},
		{__SECT_PSC_MID_CIRC_POLY__},		// IDB_CMD_PSC_MID_05},
		{__SECT_PSC_MID_POLY_CIRC__},		// IDB_CMD_PSC_MID_08},
		{__SECT_PSC_I__            },		// IDB_CMD_PSC_I_01},
		{__SECT_PSC_HLF_LEFT_NONE__},		// IDB_CMD_PSC_HALF_02},
		{__SECT_PSC_HLF_LEFT_CIRC__},		// IDB_CMD_PSC_HALF_03},
		{__SECT_PSC_HLF_LEFT_POLY__},		// IDB_CMD_PSC_HALF_01},
		{__SECT_PSC_HLF_RGHT_NONE__},		// IDB_CMD_PSC_HALF_06},
		{__SECT_PSC_HLF_RGHT_CIRC__},		// IDB_CMD_PSC_HALF_05},
		{__SECT_PSC_HLF_RGHT_POLY__},		// IDB_CMD_PSC_HALF_04},
		{__SECT_PSC_TEE__          },		// IDB_CMD_PSC_TEE},
		{__SECT_PSC_NML_3_CELL__   },		// IDB_CMD_PSC_3Cell},
		{__SECT_PSC_NML_4_CELL__   },		// IDB_CMD_PSC_4Cell},
		{__SECT_PSC_nCELL_POLY1_LEFT__},	// IDB_CMD_PSC_NCELL_POLY1_LEFT},
		{__SECT_PSC_nCELL_POLY1_RGHT__},	// IDB_CMD_PSC_NCELL_POLY1_RIGHT},
		{__SECT_PSC_nCELL_POLY2_LEFT__},	// IDB_CMD_PSC_NCELL_POLY2_LEFT},
		{__SECT_PSC_nCELL_POLY2_RGHT__},	// IDB_CMD_PSC_NCELL_POLY2_RIGHT},
		{__SECT_PSC_nCELL_CIRC_LEFT__ },	// IDB_CMD_PSC_NCELL_CIRC_LEFT},
		{__SECT_PSC_nCELL_CIRC_RGHT__ },	// IDB_CMD_PSC_NCELL_CIRC_RIGHT},
		{__SECT_PSC_PLAT_HALF_LEFT__},		// IDB_CMD_PSC_PLAT_02},
		{__SECT_PSC_PLAT_HALF_RGHT__},		// IDB_CMD_PSC_PLAT_03},
		{__SECT_PSC_PLAT_1CEL_POLY__},		// IDB_CMD_PSC_PLAT_04},
		{__SECT_PSC_PLAT_1CEL_CIRC__},		// IDB_CMD_PSC_PLAT_01},
		{__SECT_PSC_PLAT_2CEL__     },		// IDB_CMD_PSC_PLAT_05},
		{__SECT_PSC_VALUE__         },		// IDB_CMD_PSC_VALUE },
		{__SECT_PSC_CMPWEB__        },		// IDB_CMD_PSC_CMP },
		{__SECT_PSC_CMPWEB_TAP__    },		// IDB_CMD_PSC_CMP_TAP },
	};

	CString aImagePath[] = {
	_T("SVG\\Illustration\\Dialog\\psc05.svg"),           // IDB_CMD_PSC_HALF_01 },
	_T("SVG\\Illustration\\Dialog\\psc_2cell.svg"),       // IDB_CMD_PSC_2CELL_01},
	_T("SVG\\Illustration\\Dialog\\cmd_psc_bth3.svg"),    // IDB_CMD_PSC_MID_03},
	_T("SVG\\Illustration\\Dialog\\psc_rht03.svg"),       // IDB_CMD_PSC_MID_09},
	_T("SVG\\Illustration\\Dialog\\psc_lft03.svg"),       // IDB_CMD_PSC_MID_06},
	_T("SVG\\Illustration\\Dialog\\cmd_psc_bth2.svg"),    // IDB_CMD_PSC_MID_02},
	_T("SVG\\Illustration\\Dialog\\cmd_psc_rht1.svg"),    // IDB_CMD_PSC_MID_07},
	_T("SVG\\Illustration\\Dialog\\cmd_psc_lft1.svg"),    // IDB_CMD_PSC_MID_04},
	_T("SVG\\Illustration\\Dialog\\cmd_psc_.svg"),        // IDB_CMD_PSC_MID_01},
	_T("SVG\\Illustration\\Dialog\\cmd_psc_lft2.svg"),    // IDB_CMD_PSC_MID_05},
	_T("SVG\\Illustration\\Dialog\\cmd_psc_rht2.svg"),    // IDB_CMD_PSC_MID_08},
	_T("SVG\\Illustration\\Dialog\\psc_I_01.svg"),        // IDB_CMD_PSC_I_01},
	_T("SVG\\Illustration\\Dialog\\psc07.svg"),           // IDB_CMD_PSC_HALF_02},
	_T("SVG\\Illustration\\Dialog\\psc06.svg"),           // IDB_CMD_PSC_HALF_03},
	_T("SVG\\Illustration\\Dialog\\psc05.svg"),           // IDB_CMD_PSC_HALF_01},
	_T("SVG\\Illustration\\Dialog\\psc10.svg"),           // IDB_CMD_PSC_HALF_06},
	_T("SVG\\Illustration\\Dialog\\psc09.svg"),           // IDB_CMD_PSC_HALF_05},
	_T("SVG\\Illustration\\Dialog\\psc08.svg"),           // IDB_CMD_PSC_HALF_04},
	_T("SVG\\Illustration\\Dialog\\psc_tee.svg"),         // IDB_CMD_PSC_TEE},
	_T("SVG\\Illustration\\Dialog\\psc_3cell.svg"),       // IDB_CMD_PSC_3Cell},
	_T("SVG\\Illustration\\Dialog\\psc_4cell.svg"),       // IDB_CMD_PSC_4Cell},
	_T("SVG\\Illustration\\Dialog\\psc_n_cell1.svg"),     // IDB_CMD_PSC_NCELL_POLY1_LEFT},
	_T("SVG\\Illustration\\Dialog\\psc_n_cell11.svg"),    // IDB_CMD_PSC_NCELL_POLY1_RIGHT},
	_T("SVG\\Illustration\\Dialog\\psc_n_cell3.svg"),     // IDB_CMD_PSC_NCELL_POLY2_LEFT},
	_T("SVG\\Illustration\\Dialog\\psc_n_cell33.svg"),    // IDB_CMD_PSC_NCELL_POLY2_RIGHT},
	_T("SVG\\Illustration\\Dialog\\psc_n_cell2.svg"),     // IDB_CMD_PSC_NCELL_CIRC_LEFT},
	_T("SVG\\Illustration\\Dialog\\psc_n_cell22.svg"),    // IDB_CMD_PSC_NCELL_CIRC_RIGHT},
	_T("SVG\\Illustration\\Dialog\\psc_plat_02.svg"),     // IDB_CMD_PSC_PLAT_02},
	_T("SVG\\Illustration\\Dialog\\psc_plat_03.svg"),     // IDB_CMD_PSC_PLAT_03},
	_T("SVG\\Illustration\\Dialog\\psc_plat_04.svg"),     // IDB_CMD_PSC_PLAT_04},
	_T("SVG\\Illustration\\Dialog\\psc_plat_01.svg"),     // IDB_CMD_PSC_PLAT_01},
	_T("SVG\\Illustration\\Dialog\\psc_plat_05.svg"),     // IDB_CMD_PSC_PLAT_05},
	_T("SVG\\Illustration\\Dialog\\psc_value.svg"),       // IDB_CMD_PSC_VALUE },
	_T("SVG\\Illustration\\Dialog\\psc_cmp.svg"),         // IDB_CMD_PSC_CMP },
	_T("SVG\\Illustration\\Dialog\\psc_cmp_tap.svg"),     // IDB_CMD_PSC_CMP_TAP },
	};

	int nCount = sizeof(aSectIdx)/sizeof(aSectIdx[0]);
	int i = 0;
	for (i = 0; i < nCount; i++)
		if (nIndex == (int)aSectIdx[i]) break;
	if (i == nCount) { ASSERT(0); return FALSE; }

	if (m_nCurIndex == nIndex && m_wndPicture.IsImageInitialized()) return TRUE;  // 같은 bitmap이면 Load 안함
	m_nCurIndex = nIndex;

	m_wndPicture.SetImage(aImagePath[i]);

	return bResult;
}

void CCMSectItmePSCView::SetDataSource(T_SECT_D* pDataSrc)
{  
	ASSERT(pDataSrc);
	m_wndSecView.SetDataSource(pDataSrc, FALSE);
	m_bSetData = TRUE; 
	m_wndSecView.SetShearCheckFlag(TRUE);
}

void CCMSectItmePSCView::RedrawSection(BOOL bCentroid/*=FALSE*/)
{
	if(!m_bSetData)
	{
		AfxMessageBox(_T("Data가 Setting되지 않았습니다."));
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

void CCMSectItmePSCView::ShowSectView(BOOL bShow)
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
			pWnd = GetDlgItem(IDC_CMD_PICTURE);
		if (pWnd == 0) { ASSERT(0); return; }
		
		pWnd->GetWindowRect(rectLast);
		this->GetWindowRect(r);
		r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(8);
		this->MoveWindow(r);
	}
}

/////////////////////////////////////////////////////////////////////////////
//
// Redraw - Update Viewer
//
BOOL CCMSectItmePSCView::OnInitDialog() 
{
	CCMDlgBase::OnInitDialog();
	
	// TODO: Add extra initialization here
	CWnd* pWnd = GetDlgItem(IDC_CMD_VIEW_FRM);
	ASSERT(pWnd);
	m_wndSecView.Init(pWnd);

	SetIgnoreBottomBtnArea(); // 구분선 삭제

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMSectItmePSCView::OnSysCommand(UINT nID, LPARAM lParam) 
{
	// TODO: Add your message handler code here and/or call default
	// Close를 막아준다.
	if(nID == SC_CLOSE)
	{
		return;
	}

	CCMDlgBase::OnSysCommand(nID, lParam);
}

void CCMSectItmePSCView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	ChangeBitmap(m_nCurIndex);

	// Do not call CCMDlgBase::OnPaint() for painting messages
}

BOOL CCMSectItmePSCView::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CCMDlgBase::DestroyWindow();
}
