// IehpMphiLocDlg.cpp: implementation of the CIehpMphiLocDlg class.
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "wg_cmd.h"

#include "IehpMphiLocDlg.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DBDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CIehpMphiLocDlg::CIehpMphiLocDlg(int nAutoUserLocMemb, int nLocMemb, CWnd* pParent /*=NULL*/ )
	: CDialogMove(CIehpMphiLocDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIehpMphiLocDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nAutoUserLocMemb = nAutoUserLocMemb;
	m_nLocMemb         = nLocMemb;
	m_rectPosition.SetRectEmpty();
}

CIehpMphiLocDlg::~CIehpMphiLocDlg()
{  
}

void CIehpMphiLocDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIehpMphiLocDlg)
	DDX_Radio(pDX, IDC_ETC_IEHP_MPHI_LOC_IEHC_RDO, m_nAutoUserLocMemb);
	DDX_Radio(pDX, IDC_ETC_IEHP_MPHI_LOC_I_RDO, m_nLocMemb);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIehpMphiLocDlg, CDialogMove)
	//{{AFX_MSG_MAP(CIehpMphiLocDlg)
	ON_BN_CLICKED(IDC_ETC_IEHP_MPHI_LOC_IEHC_RDO, CtrlManager)
	ON_BN_CLICKED(IDC_ETC_IEHP_MPHI_LOC_USER_RDO, CtrlManager)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIehpMphiLocDlg message handlers
BOOL CIehpMphiLocDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	MoveDlg();
	CtrlManager();

	return TRUE;
}

void CIehpMphiLocDlg::OnOK() 
{
	UpdateData(TRUE);
	CDialogMove::OnOK();  
}

void CIehpMphiLocDlg::MoveDlg()
{
	CRect rectDlg; rectDlg.SetRectEmpty();
	GetWindowRect(&rectDlg);

	SetWindowPos(NULL, m_rectPosition.left, m_rectPosition.top, rectDlg.Width(), rectDlg.Height(), SWP_NOZORDER);
}

void CIehpMphiLocDlg::CtrlManager()
{
	UpdateData(TRUE);

	CArray<UINT,UINT> aLocCtrlID;
	aLocCtrlID.Add(IDC_ETC_IEHP_MPHI_LOC_I_RDO);
	aLocCtrlID.Add(IDC_ETC_IEHP_MPHI_LOC_C_RDO);
	aLocCtrlID.Add(IDC_ETC_IEHP_MPHI_LOC_J_RDO);

	const BOOL bAuto = m_nAutoUserLocMemb==0;
	if(bAuto)
	{
		T_IEHC_D IehcD;
		if(!CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetIehc(IehcD)) IehcD.Initialize();
		m_nLocMemb = IehcD.nBeamLoc;
		UpdateData(FALSE);

		for(int i=0; i<3; i++)
		{
			GetDlgItem(aLocCtrlID[i])->EnableWindow(i==m_nLocMemb);
		}
	}
	else
	{
		CDlgUtil::CtrlEnableDisable(this,aLocCtrlID,TRUE);
	}
}