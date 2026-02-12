// NLLinkPrtHystDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "NLLinkPrtHystDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNLLinkPrtHystDlg dialog


CNLLinkPrtHystDlg::CNLLinkPrtHystDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CNLLinkPrtHystDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNLLinkPrtHystDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CNLLinkPrtHystDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNLLinkPrtHystDlg)
	DDX_Control(pDX, IDC_CMD_NLLK_STIF_UNIT, m_wndStifnessUnit);
	DDX_Control(pDX, IDC_CMD_NLLK_YILD_UNIT, m_wndYildStrnUnit);
	DDX_Control(pDX, IDC_CMD_NLLK_EXPO_EDIT, m_wndYildExpo);
	DDX_Control(pDX, IDC_CMD_NLLK_STIF_EDIT, m_wndStifness);
	DDX_Control(pDX, IDC_CMD_NLLK_YILD_EDIT, m_wndYildStrn);
	DDX_Control(pDX, IDC_CMD_NLLK_BETA_EDIT, m_wndHystBeta);
	DDX_Control(pDX, IDC_CMD_NLLK_ALPA_EDIT, m_wndHystAlph);
	DDX_Control(pDX, IDC_CMD_NLLK_POST_EDIT, m_wndPostYild);
	DDX_Control(pDX, IDC_WG_CMD_STATIC15, m_wndPicture);
	DDX_Control(pDX, IDC_WG_CMD_STATIC13, m_wndPicture2);
	DDX_Control(pDX, IDC_WG_CMD_STATIC14, m_wndPicture3);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNLLinkPrtHystDlg, CDialogMove)
	//{{AFX_MSG_MAP(CNLLinkPrtHystDlg)
//	ON_EN_CHANGE(IDC_CMD_NLLK_BETA_EDIT, OnChangeCmdNllkBetaEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNLLinkPrtHystDlg message handlers

BOOL CNLLinkPrtHystDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
//  m_wndHystAlph.EnableWindow(FALSE);

	switch(m_nDofType)
	{
		case 0: ShowData(0); break;
		case 1: ShowData(1); break;
		case 2: ShowData(2); break;
		case 3: ShowData(3); break;
		case 4: ShowData(4); break;
		case 5: ShowData(5); break;
	}
	switch(m_nDofType)
	{
		case 0: case 1: case 2:
			m_wndStifnessUnit.SetUnitType(D_UNITSYS_BASE_SPRING);
			m_wndYildStrnUnit.SetUnitType(D_UNITSYS_BASE_FORCE);
			break;
		case 3: case 4: case 5:
			m_wndStifnessUnit.SetUnitType(D_UNITSYS_BASE_ROSPRING);
			m_wndYildStrnUnit.SetUnitType(D_UNITSYS_BASE_MOMENT);
			break;
	}

	m_wndPicture.SetImage(_T("SVG\\Illustration\\Dialog\\cmd_nllp_hs01.svg"));
	m_wndPicture2.SetImage(_T("SVG\\Illustration\\Dialog\\cmd_nllp_hs02.svg"));
	m_wndPicture3.SetImage(_T("SVG\\Illustration\\Dialog\\cmd_nllp_hs03.svg"));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNLLinkPrtHystDlg::ShowData(int nFlag)
{
	m_wndStifness.SetEditUnit(m_Data.NDP_Prop[nFlag].HS.dStiffness          );
	m_wndYildStrn.SetEditUnit(m_Data.NDP_Prop[nFlag].HS.dYieldStrength      );
	m_wndPostYild.SetEditUnit(m_Data.NDP_Prop[nFlag].HS.dPostYieldStiffRatio);
	m_wndYildExpo.SetEditUnit(m_Data.NDP_Prop[nFlag].HS.dYieldingExp        );      
	m_wndHystBeta.SetEditUnit(m_Data.NDP_Prop[nFlag].HS.dLoopParamB         );      
	m_wndHystAlph.SetEditUnit(m_Data.NDP_Prop[nFlag].HS.dLoopParamA         );      
}
/*
void CNLLinkPrtHystDlg::OnChangeCmdNllkBetaEdit() 
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
void CNLLinkPrtHystDlg::OnOK() 
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
	if(m_wndYildExpo.GetEditValue() < 1.0)
	{
	  AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_YieldingExponent)); 	return;
	}
		
	m_Data.NDP_Prop[m_nDofType].HS.dStiffness           = m_wndStifness.GetEditValue();
	m_Data.NDP_Prop[m_nDofType].HS.dYieldStrength       = m_wndYildStrn.GetEditValue();
	m_Data.NDP_Prop[m_nDofType].HS.dPostYieldStiffRatio = m_wndPostYild.GetEditValue();
	m_Data.NDP_Prop[m_nDofType].HS.dYieldingExp         = m_wndYildExpo.GetEditValue();
	m_Data.NDP_Prop[m_nDofType].HS.dLoopParamB          = m_wndHystBeta.GetEditValue();
	m_Data.NDP_Prop[m_nDofType].HS.dLoopParamA          = m_wndHystAlph.GetEditValue();
	//JJB//m_Data.NDP_Prop[m_nDofType].HS.dEffectiveDamping    = m_wndEffeDamp.GetEditValue();

	CDialogMove::OnOK();
}

void CNLLinkPrtHystDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialogMove::OnCancel();
}


