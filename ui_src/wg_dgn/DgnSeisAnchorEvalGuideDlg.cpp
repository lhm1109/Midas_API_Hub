#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisAnchorEvalGuideDlg.h"
#include "../wg_db/DBDoc.h"
#include "../wg_db/AttrCtrl2.h"
#include "DgnSeisAnchorEvalItemNewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorEvalGuideDlg dialog
CDgnSeisAnchorEvalGuideDlg::CDgnSeisAnchorEvalGuideDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnSeisAnchorEvalGuideDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSeisAnchorEvalGuideDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bInitDone = FALSE;
	m_pParent = pParent;
}

CDgnSeisAnchorEvalGuideDlg::~CDgnSeisAnchorEvalGuideDlg()
{
	
}

void CDgnSeisAnchorEvalGuideDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSeisAnchorEvalGuideDlg)
	DDX_Control(pDX, IDC_DGN_ANCHOR_GUIDE_VIEW, m_View);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnSeisAnchorEvalGuideDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnSeisAnchorEvalGuideDlg)
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//	ON_BN_CLICKED(IDC_CMD_SECT_CENTROID_BTN, OnCmdSectCentroidBtn)

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorEvalGuideDlg message handlers


void CDgnSeisAnchorEvalGuideDlg::SetDataSource(T_ANEV_D* pDataSrc)
{  
	ASSERT(pDataSrc);
	m_pData = pDataSrc;
}

BOOL CDgnSeisAnchorEvalGuideDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	m_View.SetParent(this);
	m_View.Init();
	m_View.SetMode(EN_DRAW_WINMODE_SELECT);
	int nRatType[4] = { 0 };
	m_View.MakeDrawUnit(nRatType);

	m_View.SetDrawType(3, 0, FALSE);

	m_bInitDone = TRUE;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnSeisAnchorEvalGuideDlg::SetDataToView()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_BRPR_D BrprD;
	std::vector<std::vector<T_BRPR_D>> aArBrprD;
	BOOL bFail = FALSE;
	for (int i = 0; i < m_pData->BearingProp.aBrprKs.GetSize(); i++)
	{
		aArBrprD.push_back(std::vector<T_BRPR_D>());
		for (int j = 0; j < m_pData->BearingProp.aBrprKs[i].aBrprK.GetSize(); j++)
		{
			if (!pDoc->m_pAttrCtrl2->GetBrpr(m_pData->BearingProp.aBrprKs[i].aBrprK[j], BrprD))
			{
				bFail = TRUE;
				break;
			}
			aArBrprD[i].push_back(BrprD);
		}
	}
	if (bFail)
	{
		aArBrprD.clear();
	}

	m_View.SetData(aArBrprD, *m_pData, FALSE);
	m_View.SetDrawType(3, 0, FALSE);
}

void CDgnSeisAnchorEvalGuideDlg::ResetDlgPos()
{
	if (!m_bInitDone)
		return;

	MoveDlgToInitPos();
}

void CDgnSeisAnchorEvalGuideDlg::OnSysCommand(UINT nID, LPARAM lParam) 
{
	// TODO: Add your message handler code here and/or call default
	if(nID == SC_CLOSE)
	{
		if (m_pParent)
		{
			// close 는 Parent단에서 hide 처리로 수정함
			((CDgnSeisAnchorEvalItemNewDlg*)m_pParent)->OnCloseGuideDlg();
		}
		
		return;
	}

	CDialogMove::OnSysCommand(nID, lParam);
}

BOOL CDgnSeisAnchorEvalGuideDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialogMove::DestroyWindow();
}
