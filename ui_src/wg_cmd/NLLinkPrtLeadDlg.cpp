// NLLinkPrtLeadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "NLLinkPrtLeadDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNLLinkPrtLeadDlg dialog


CNLLinkPrtLeadDlg::CNLLinkPrtLeadDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CNLLinkPrtLeadDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNLLinkPrtLeadDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CNLLinkPrtLeadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNLLinkPrtLeadDlg)
	DDX_Control(pDX, IDC_CMD_NLLK_STIF_UNIT, m_wndStifnessUnit);
	DDX_Control(pDX, IDC_CMD_NLLK_YILD_UNIT, m_wndYildStrnUnit);
	DDX_Control(pDX, IDC_CMD_NLLK_YILD_EDIT, m_wndYildStrn);
	DDX_Control(pDX, IDC_CMD_NLLK_STIF_EDIT, m_wndStifness);
	DDX_Control(pDX, IDC_CMD_NLLK_POST_EDIT, m_wndPostYild);
	DDX_Control(pDX, IDC_CMD_NLLK_BETA_EDIT, m_wndHystBeta);
	DDX_Control(pDX, IDC_CMD_NLLK_ALPA_EDIT, m_wndHystAlph);
	DDX_Control(pDX, IDC_WG_CMD_STATIC13, m_wndPicture);
	DDX_Control(pDX, IDC_WG_CMD_STATIC11, m_wndPicture2);
	DDX_Control(pDX, IDC_WG_CMD_STATIC12, m_wndPicture3);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNLLinkPrtLeadDlg, CDialogMove)
	//{{AFX_MSG_MAP(CNLLinkPrtLeadDlg)
//	ON_EN_CHANGE(IDC_CMD_NLLK_BETA_EDIT, OnChangeCmdNllkBetaEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNLLinkPrtLeadDlg message handlers
BOOL CNLLinkPrtLeadDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
//  m_wndHystAlph.EnableWindow(FALSE);

	switch(m_nDofType)
	{
		case 1: ShowData(1); break;
		case 2: ShowData(2); break;    
	}

	SetWindowText(_LS(IDS_WG_CMD__ADD2_Nonlinear_Link_Prop10)); // Shear Spring in Lead Rubber Bearing Isolator
	m_wndStifnessUnit.SetUnitType(D_UNITSYS_BASE_SPRING);
	m_wndYildStrnUnit.SetUnitType(D_UNITSYS_BASE_FORCE);

	m_wndPicture.SetImage(_T("SVG\\Illustration\\Dialog\\cmd_nllp_lr01.svg"));
	m_wndPicture2.SetImage(_T("SVG\\Illustration\\Dialog\\cmd_nllp_hs02.svg"));
	m_wndPicture3.SetImage(_T("SVG\\Illustration\\Dialog\\cmd_nllp_lr03.svg"));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNLLinkPrtLeadDlg::ShowData(int nFlag)
{
	m_wndStifness.SetEditUnit(m_Data.NDP_Prop[nFlag].LRBI.dStiffness          );
	m_wndYildStrn.SetEditUnit(m_Data.NDP_Prop[nFlag].LRBI.dYieldStrength      );
	m_wndPostYild.SetEditUnit(m_Data.NDP_Prop[nFlag].LRBI.dPostYieldStiffRatio);
	m_wndHystBeta.SetEditUnit(m_Data.NDP_Prop[nFlag].LRBI.dLoopParamB         );      
	m_wndHystAlph.SetEditUnit(m_Data.NDP_Prop[nFlag].LRBI.dLoopParamA         );      
}
/*
void CNLLinkPrtLeadDlg::OnChangeCmdNllkBetaEdit() 
{
	CString strAlpha, strBeta;
	double dAlpha, dBeta;
	m_wndHystBeta.GetWindowText(strBeta);
	dBeta = _tstof(strBeta);
	dAlpha = 1.0 - fabs(dBeta);
	strAlpha.Format(_T("%g"), dAlpha);
	m_wndHystAlph.SetWindowText(strAlpha);	
}
*/
void CNLLinkPrtLeadDlg::OnOK() 
{
	// TODO: Add extra validation here
	if(m_wndStifness.GetEditValue() <= 0)
	{
	  AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_Stiffness)); 	return;
	}
	if(m_wndYildStrn.GetEditValue() <= 0)
	{
	  AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_YieldStrength)); 	return;
	}
	if(m_wndPostYild.GetEditValue() < 0 || m_wndPostYild.GetEditValue() > 1.0)
	{
	  AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_PostYieldRatio)); 	return;
	}

	m_Data.NDP_Prop[m_nDofType].LRBI.dStiffness           = m_wndStifness.GetEditValue();
	m_Data.NDP_Prop[m_nDofType].LRBI.dYieldStrength       = m_wndYildStrn.GetEditValue();
	m_Data.NDP_Prop[m_nDofType].LRBI.dPostYieldStiffRatio = m_wndPostYild.GetEditValue();
	m_Data.NDP_Prop[m_nDofType].LRBI.dLoopParamB          = m_wndHystBeta.GetEditValue();
	m_Data.NDP_Prop[m_nDofType].LRBI.dLoopParamA          = m_wndHystAlph.GetEditValue();

	CDialogMove::OnOK();
}

void CNLLinkPrtLeadDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialogMove::OnCancel();
}

