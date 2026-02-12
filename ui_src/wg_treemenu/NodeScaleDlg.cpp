// NodeScaleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "NodeScaleDlg.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNodeScaleDlg dialog


CNodeScaleDlg::CNodeScaleDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CNodeScaleDlg::IDD, pParent)
{
	m_pDoc = 0;
	//{{AFX_DATA_INIT(CNodeScaleDlg)
	m_ScaleAboutCbo = 0;
	m_SelectTypeCbo = 0;
	//}}AFX_DATA_INIT

	m_arCtrlUser.Add(IDC_TM_NODE_SCALEEDIT);
	m_arCtrlUser.Add(IDC_TM_NODE_SCALEUNIT);
}


void CNodeScaleDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNodeScaleDlg)
	DDX_Control(pDX, IDC_TM_NODE_SCALEUNIT, m_ScaleUserUnit);
	DDX_Control(pDX, IDC_TM_NODE_SCALEEDIT, m_ScaleUserEdit);
	DDX_Control(pDX, IDC_TM_NODE_SCALEZ_EDIT, m_ScaleZEdit);
	DDX_Control(pDX, IDC_TM_NODE_SCALEY_EDIT, m_ScaleYEdit);
	DDX_Control(pDX, IDC_TM_NODE_SCALEX_EDIT, m_ScaleXEdit);
	DDX_Radio(pDX, IDC_TM_NODE_UCS_CBO, m_ScaleAboutCbo);
	DDX_Radio(pDX, IDC_TM_NODE_SELALL_CBO, m_SelectTypeCbo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNodeScaleDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CNodeScaleDlg)
	ON_BN_CLICKED(IDC_TM_NODE_UCS_CBO, OnScaleAbout)
	ON_BN_CLICKED(IDC_TM_NODE_GCS_CBO, OnScaleAbout)
	ON_BN_CLICKED(IDC_TM_NODE_AVG_CBO, OnScaleAbout)
	ON_BN_CLICKED(IDC_TM_NODE_CENTER_CBO, OnScaleAbout)
	ON_BN_CLICKED(IDC_TM_NODE_USER_CBO, OnScaleAbout)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_TM_NODE_SCALEX_SPIN, OnDeltaposTmNodeScalexSpin)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_TM_NODE_SCALEY_SPIN, OnDeltaposTmNodeScaleySpin)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_TM_NODE_SCALEZ_SPIN, OnDeltaposTmNodeScalezSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNodeScaleDlg message handlers

BOOL CNodeScaleDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
	CMenuBarChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	CtrlEnableDisable(m_arCtrlUser, FALSE);

	m_ScaleXEdit.SetRange(0, SHRT_MAX);
	m_ScaleXEdit.SetWindowText(_LS(IDS_WG_TREEMENU_1_0));
	m_ScaleYEdit.SetRange(0, SHRT_MAX);
	m_ScaleYEdit.SetWindowText(_LS(IDS_WG_TREEMENU_1_0));
	m_ScaleZEdit.SetRange(0, SHRT_MAX);
	m_ScaleZEdit.SetWindowText(_LS(IDS_WG_TREEMENU_1_0));

	m_ScaleAboutCbo = 0;
	m_SelectTypeCbo = 0;

	UpdateData(FALSE);
	
	m_ScaleUserEdit.SetAttUcsPos();
	m_ScaleUserEdit.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_ScaleUserEdit.SetWindowText(_LS(IDS_WG_TREEMENU_0__0__0));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNodeScaleDlg::OnScaleAbout() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(m_ScaleAboutCbo == 4)
	{
		CtrlEnableDisable(m_arCtrlUser, TRUE);
		GotoDlgCtrl(&m_ScaleUserEdit);
	}
	else
		CtrlEnableDisable(m_arCtrlUser, FALSE);
}

void CNodeScaleDlg::Execute()
{
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	if(!UpdateData(TRUE)) return;

	double dfx;
	double dfy;
	double dfz;
	double uxyz[3][3];
	CArray<T_NODE_K, T_NODE_K> rKey;
	int nScaleAbout;
	double oux, tempoux;
	double ouy, tempouy;
	double ouz, tempouz;

	CString strScaleFactor;
	CString strScaleUser;
	CString strUcsVector[] = { _LS(IDS_WG_TREEMENU_1_0_0) ,_LS(IDS_WG_TREEMENU_0_1_0), _LS(IDS_WG_TREEMENU_0_0_1)};

	if(!m_SelectTypeCbo)  // All Select
	{
		m_pDoc->m_pAttrCtrl->GetNodeKeyList(rKey);
	}
	else  // By Select
		if(!pIGM->GetSelectedNodeKeyList(rKey))
		{
			GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___No_selected_node_));
			return;
		}

	m_ScaleXEdit.GetWindowText(strScaleFactor);
	if(!GetFloatNumber(strScaleFactor, dfx)) return;
	m_ScaleYEdit.GetWindowText(strScaleFactor);
	if(!GetFloatNumber(strScaleFactor, dfy)) return;
	m_ScaleZEdit.GetWindowText(strScaleFactor);
	if(!GetFloatNumber(strScaleFactor, dfz)) return;

	for(int i = 0; i<3; i++)  // for UCS vector
	{
		GetPosition(strUcsVector[i],	tempoux, tempouy,	tempouz);
		pIGM->GetWcsVector(tempoux, tempouy, tempouz, uxyz[i][0], uxyz[i][1], uxyz[i][2]);
	}
	
	nScaleAbout = m_ScaleAboutCbo+1;
	if(nScaleAbout == 1)  // UCS Origin
	{
		pIGM->GetWcsPos(0,0,0, oux, ouy, ouz);
	}
	else if(nScaleAbout == 5) // User Input 
	{
		m_ScaleUserEdit.GetWindowText(strScaleUser);
		GetPosition(strScaleUser,	tempoux, tempouy,	tempouz);
		pIGM->GetWcsPos(tempoux, tempouy, tempouz, oux, ouy, ouz);
	}
	else  // default
	{
		oux = 0;
		ouy = 0;
		ouz = 0;
	}

	if(m_pDoc->m_pDataCtrl->ScaleNode(dfx, dfy, dfz, uxyz, rKey, nScaleAbout, oux, ouy, ouz))
		m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CNodeScaleDlg::OnDeltaposTmNodeScalexSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString strTemp;
	m_ScaleXEdit.GetWindowText(strTemp);
	
	if(pNMUpDown->iDelta < 0)
	{
		strTemp.Format(_LS(IDS_WG_TREEMENU__g),_tstof(strTemp) + 1.0);
		m_ScaleXEdit.SetWindowText(strTemp);		
	}
	else
		if(_tstof(strTemp) > 1)
		{
			strTemp.Format(_LS(IDS_WG_TREEMENU__g),_tstof(strTemp) - 1.0);
			m_ScaleXEdit.SetWindowText(strTemp);		
		}
	
	*pResult = 0;
}

void CNodeScaleDlg::OnDeltaposTmNodeScaleySpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString strTemp;
	m_ScaleYEdit.GetWindowText(strTemp);
	
	if(pNMUpDown->iDelta < 0)
	{
		strTemp.Format(_LS(IDS_WG_TREEMENU__g),_tstof(strTemp) + 1.0);
		m_ScaleYEdit.SetWindowText(strTemp);		
	}
	else
		if(_tstof(strTemp) > 1)
		{
			strTemp.Format(_LS(IDS_WG_TREEMENU__g),_tstof(strTemp) - 1.0);
			m_ScaleYEdit.SetWindowText(strTemp);		
		}

	*pResult = 0;
}

void CNodeScaleDlg::OnDeltaposTmNodeScalezSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString strTemp;
	m_ScaleZEdit.GetWindowText(strTemp);
	
	if(pNMUpDown->iDelta < 0)
	{
		strTemp.Format(_LS(IDS_WG_TREEMENU__g),_tstof(strTemp) + 1.0);
		m_ScaleZEdit.SetWindowText(strTemp);		
	}
	else
		if(_tstof(strTemp) > 1)
		{
			strTemp.Format(_LS(IDS_WG_TREEMENU__g),_tstof(strTemp) - 1.0);
			m_ScaleZEdit.SetWindowText(strTemp);		
		}
	
	*pResult = 0;
}
