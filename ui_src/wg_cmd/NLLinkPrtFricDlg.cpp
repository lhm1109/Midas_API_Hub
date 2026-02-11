// NLLinkPrtFricDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "NLLinkPrtFricDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNLLinkPrtFricDlg dialog


CNLLinkPrtFricDlg::CNLLinkPrtFricDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CNLLinkPrtFricDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNLLinkPrtFricDlg)
	//}}AFX_DATA_INIT
}


void CNLLinkPrtFricDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNLLinkPrtFricDlg)
	DDX_Control(pDX, IDC_CMD_NLLK_STIF_UNIT, m_wndStifnessUnit);
	DDX_Control(pDX, IDC_CMD_NLLK_RADI_UNIT, m_wndRadiSurfUnit);
	DDX_Control(pDX, IDC_CMD_NLLK_RATE_UNIT, m_wndRateParaUnit);	//1unit
	DDX_Control(pDX, IDC_CMD_NLLK_ALPA_EDIT, m_wndHystAlph);
	DDX_Control(pDX, IDC_CMD_NLLK_BETA_EDIT, m_wndHystBeta);
	DDX_Control(pDX, IDC_CMD_NLLK_RADI_EDIT, m_wndRadiSurf);
	DDX_Control(pDX, IDC_CMD_NLLK_RATE_EDIT, m_wndRatePara);
	DDX_Control(pDX, IDC_CMD_NLLK_FRCF_EDIT, m_wndFricFast);
	DDX_Control(pDX, IDC_CMD_NLLK_FRCS_EDIT, m_wndFricSlow);
	DDX_Control(pDX, IDC_CMD_NLLK_STIF_EDIT, m_wndStifness);	
	DDX_Control(pDX, IDC_WG_CMD_STATIC17, m_wndPicture);
	DDX_Control(pDX, IDC_WG_CMD_STATIC15, m_wndPicture2);
	DDX_Control(pDX, IDC_WG_CMD_STATIC16, m_wndPicture3);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNLLinkPrtFricDlg, CDialogMove)
	//{{AFX_MSG_MAP(CNLLinkPrtFricDlg)
//	ON_EN_CHANGE(IDC_CMD_NLLK_BETA_EDIT, OnChangeCmdNllkBetaEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNLLinkPrtFricDlg message handlers

BOOL CNLLinkPrtFricDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
//  m_wndHystAlph.EnableWindow(FALSE);
	
	switch(m_nDofType)
	{    
		case 1: ShowData(1); break;
		case 2: ShowData(2); break;
	}
	SetWindowText(_LS(IDS_WG_CMD__ADD2_Nonlinear_Link_Prop11));  
	m_wndStifnessUnit.SetUnitType(D_UNITSYS_BASE_SPRING);
	m_wndRadiSurfUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndRateParaUnit.SetUnitType(D_UNITSYS_BASE_SEC_L);   //1unit

	m_wndPicture.SetImage(_T("SVG\\Illustration\\Dialog\\cmd_nllp_fp01.svg"));
	m_wndPicture2.SetImage(_T("SVG\\Illustration\\Dialog\\cmd_nllp_fp02.svg"));
	m_wndPicture3.SetImage(_T("SVG\\Illustration\\Dialog\\cmd_nllp_fp03.svg"));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNLLinkPrtFricDlg::ShowData(int nFlag)
{
	m_wndStifness.SetEditUnit(m_Data.NDP_Prop[nFlag].FPSI.dStiffness       );
	m_wndFricSlow.SetEditUnit(m_Data.NDP_Prop[nFlag].FPSI.dFricCoeffSlow   );
	m_wndFricFast.SetEditUnit(m_Data.NDP_Prop[nFlag].FPSI.dFricCoeffFast   );
	m_wndRatePara.SetEditUnit(m_Data.NDP_Prop[nFlag].FPSI.dRateParam       );      
	m_wndRadiSurf.SetEditUnit(m_Data.NDP_Prop[nFlag].FPSI.dRadius          );
	m_wndHystBeta.SetEditUnit(m_Data.NDP_Prop[nFlag].FPSI.dLoopParamB      );
	m_wndHystAlph.SetEditUnit(m_Data.NDP_Prop[nFlag].FPSI.dLoopParamA      );      
}

/*
void CNLLinkPrtFricDlg::OnChangeCmdNllkBetaEdit() 
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
void CNLLinkPrtFricDlg::OnOK() 
{
	// TODO: Add extra validation here
	if(m_wndStifness.GetEditValue() <= 0)
	{
	  AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_Stiffness)); 	return;
	}
	if(m_wndFricSlow.GetEditValue() <= 0 || m_wndFricFast.GetEditValue() <= 0)
	{
	  AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_FrictionCoefficient)); 	return;
	}
	if(m_wndRatePara.GetEditValue() < 0)
	{
	  AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_RateParameter)); 	return;
	}
	if(m_wndRadiSurf.GetEditValue() < 0)
	{
	  AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_Radius)); 	return;
	}
	
	m_Data.NDP_Prop[m_nDofType].FPSI.dStiffness        = m_wndStifness.GetEditValue();
	m_Data.NDP_Prop[m_nDofType].FPSI.dFricCoeffSlow    = m_wndFricSlow.GetEditValue();
	m_Data.NDP_Prop[m_nDofType].FPSI.dFricCoeffFast    = m_wndFricFast.GetEditValue();
	m_Data.NDP_Prop[m_nDofType].FPSI.dRateParam        = m_wndRatePara.GetEditValue();
	m_Data.NDP_Prop[m_nDofType].FPSI.dRadius           = m_wndRadiSurf.GetEditValue();
	m_Data.NDP_Prop[m_nDofType].FPSI.dLoopParamB       = m_wndHystBeta.GetEditValue();
	m_Data.NDP_Prop[m_nDofType].FPSI.dLoopParamA       = m_wndHystAlph.GetEditValue();

	CDialogMove::OnOK();
}

void CNLLinkPrtFricDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialogMove::OnCancel();
}
