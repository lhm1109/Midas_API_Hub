#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisAnchorEvalGuide2Dlg.h"
#include "../wg_db/DBDoc.h"
#include "../wg_db/AttrCtrl2.h"
#include "DgnSeisAnchorEvalItem2NewDlg.h"
#include "DgnSeisAnchorEvalItem2NewSubArrangeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorEvalGuide2Dlg dialog
CDgnSeisAnchorEvalGuide2Dlg::CDgnSeisAnchorEvalGuide2Dlg(CWnd* pParent /*=NULL*/, int nType/* = 0*/)
	: CDialogMove(CDgnSeisAnchorEvalGuide2Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSeisAnchorEvalGuide2Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bInitDone = FALSE;
	m_pParent = pParent;

	m_nGuideType = nType;
	m_nViewType = 0;
}

CDgnSeisAnchorEvalGuide2Dlg::~CDgnSeisAnchorEvalGuide2Dlg()
{
}

void CDgnSeisAnchorEvalGuide2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_VIEW_GUIDE_NONSKEW_RDO,	 m_nViewType);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_BMP, m_wndPicture);
	//{{AFX_DATA_MAP(CDgnSeisAnchorEvalGuide2Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnSeisAnchorEvalGuide2Dlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnSeisAnchorEvalGuide2Dlg)
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_EVAL_VIEW_GUIDE_NONSKEW_RDO, OnSeisAnchorEvalGuideRdo)
	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_EVAL_VIEW_GUIDE_SKEW_RDO,    OnSeisAnchorEvalGuideRdo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//	ON_BN_CLICKED(IDC_CMD_SECT_CENTROID_BTN, OnCmdSectCentroidBtn)

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorEvalGuide2Dlg message handlers


void CDgnSeisAnchorEvalGuide2Dlg::SetDataSource(T_ANEV_D* pDataSrc)
{  
	ASSERT(pDataSrc);
	m_pData = pDataSrc;
}

BOOL CDgnSeisAnchorEvalGuide2Dlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	if(m_nGuideType == 1)
	{
		GetDlgItem(IDC_DGN_SEIS_ANCHOR_EVAL_VIEW_GUIDE_NONSKEW_RDO)->ShowWindow(FALSE);
		GetDlgItem(IDC_DGN_SEIS_ANCHOR_EVAL_VIEW_GUIDE_SKEW_RDO   )->ShowWindow(FALSE);
	}

	m_wndPicture.SetIgnoreLargeView(TRUE);
	ChangeBitmap();
	SetIgnoreBottomBtnArea();

	m_bInitDone = TRUE;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnSeisAnchorEvalGuide2Dlg::OnSeisAnchorEvalGuideRdo()
{
	UpdateData(TRUE);
	ChangeBitmap();
}

void CDgnSeisAnchorEvalGuide2Dlg::ChangeBitmap()
{
	int nType = (m_nGuideType == 1) ? 2 : m_nViewType;
	CString aBitmapPath[] = { 
		_T("SVG\\illustration\\Dialog\\Dgn_Seis_Anchor_Eval_Non_Skew.svg"),
		_T("SVG\\illustration\\Dialog\\Dgn_Seis_Anchor_Eval_Skew.svg"),
		_T("SVG\\illustration\\Dialog\\Dgn_Seis_Anchor_Eval_CopingAngle.svg")
		};
	m_wndPicture.SetImage(aBitmapPath[nType]);
}

void CDgnSeisAnchorEvalGuide2Dlg::SetDataToView()
{

}

void CDgnSeisAnchorEvalGuide2Dlg::ResetDlgPos()
{
	if (!m_bInitDone)
		return;

	MoveDlgToInitPos();
}

void CDgnSeisAnchorEvalGuide2Dlg::OnSysCommand(UINT nID, LPARAM lParam) 
{
	// TODO: Add your message handler code here and/or call default
	if(nID == SC_CLOSE)
	{
		if (m_pParent)
		{
			if (m_nGuideType == 1)
			{
				((CDgnSeisAnchorEvalItem2NewSubArrangeDlg*)m_pParent)->OnCloseGuideDlg();
			}
			else
			{
				// close 는 Parent단에서 hide 처리로 수정함
				((CDgnSeisAnchorEvalItem2NewDlg*)m_pParent)->OnCloseGuideDlg();
			}		
		}
		
		return;
	}

	CDialogMove::OnSysCommand(nID, lParam);
}

BOOL CDgnSeisAnchorEvalGuide2Dlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialogMove::DestroyWindow();
}
