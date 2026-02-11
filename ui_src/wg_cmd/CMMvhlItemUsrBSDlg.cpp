// CMMvhlItemUsrBSDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemUsrBSDlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CDialog CInternationalDlg
/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUsrBSDlg dialog


CCMMvhlItemUsrBSDlg::CCMMvhlItemUsrBSDlg(T_MVHL_D* pData, CWnd* pParent /*=NULL*/)
	: CDialog(CCMMvhlItemUsrBSDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvhlItemUsrBSDlg)
// 	m_dBSW1 = 0.0;
//  m_dBSW2 = 0.0;
//  m_dBSW3 = 0.0;
//  m_dBSL1 = 0.0;
//  m_dBSL2 = 0.0;
//  m_dBSPa = 0.0;
//  m_dBSPb = 0.0;
// 	m_dBSD1 = 0.0;
// 	m_dBSD2 = 0.0;
// 	m_dBSD  = 0.0;
//  m_dBSD3 = 0.0;
// 	m_dBSD4 = 0.0;
// 	m_dBSD5 = 0.0;
// 	m_dBSD6 = 0.0;
	//}}AFX_DATA_INIT
	m_pData = NULL;
	m_pData = pData;
}


void CCMMvhlItemUsrBSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlItemUsrBSDlg)
	DDX_Control(pDX, IDC_CMD_MVHL_BS_UNIT_NUM_EDIT, m_edtNumUnit);
//  DDX_Text(pDX, IDC_CMD_MVHL_BS_W1_EDIT, m_dBSW1);
//  DDX_Text(pDX, IDC_CMD_MVHL_BS_W2_EDIT, m_dBSW2);
//  DDX_Text(pDX, IDC_CMD_MVHL_BS_W3_EDIT, m_dBSW3);
//  DDX_Text(pDX, IDC_CMD_MVHL_BS_L1_EDIT, m_dBSL1);
//  DDX_Text(pDX, IDC_CMD_MVHL_BS_L2_EDIT, m_dBSL2);
//  DDX_Text(pDX, IDC_CMD_MVHL_BS_PA_EDIT, m_dBSPa);
//  DDX_Text(pDX, IDC_CMD_MVHL_BS_PB_EDIT, m_dBSPb);
// 	DDX_Text(pDX, IDC_CMD_MVHL_BS_D1_EDIT, m_dBSD1);
// 	DDX_Text(pDX, IDC_CMD_MVHL_BS_D2_EDIT, m_dBSD2);
// 	DDX_Text(pDX, IDC_CMD_MVHL_BS_D_EDIT,  m_dBSD);
//  DDX_Text(pDX, IDC_CMD_MVHL_BS_D3_EDIT, m_dBSD3);
// 	DDX_Text(pDX, IDC_CMD_MVHL_BS_D4_EDIT, m_dBSD4);
// 	DDX_Text(pDX, IDC_CMD_MVHL_BS_D5_EDIT, m_dBSD5);
// 	DDX_Text(pDX, IDC_CMD_MVHL_BS_D6_EDIT, m_dBSD6);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_W1_EDIT, m_edtBSW1);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_W2_EDIT, m_edtBSW2);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_W3_EDIT, m_edtBSW3);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_L1_EDIT, m_edtBSL1);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_L2_EDIT, m_edtBSL2);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_PA_EDIT, m_edtBSPa);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_PB_EDIT, m_edtBSPb);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D1_EDIT, m_edtBSD1);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D2_EDIT, m_edtBSD2);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D_EDIT,  m_edtBSD);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D3_EDIT, m_edtBSD3);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D4_EDIT, m_edtBSD4);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D5_EDIT, m_edtBSD5);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D6_EDIT, m_edtBSD6);
	DDX_Text(pDX, IDC_CMD_MVHL_BS_W2_EXP_STATIC, m_strBSW2_exp);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_W1_UNIT, m_untBSW1);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_W2_UNIT, m_untBSW2);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_W3_UNIT, m_untBSW3);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_L1_UNIT, m_untBSL1);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_L2_UNIT, m_untBSL2);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_PA_UNIT, m_untBSPa);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_PB_UNIT, m_untBSPb);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D1_UNIT, m_untBSD1);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D2_UNIT, m_untBSD2);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D_UNIT,  m_untBSD);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D3_UNIT, m_untBSD3);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D4_UNIT, m_untBSD4);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D5_UNIT, m_untBSD5);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D6_UNIT, m_untBSD6);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMMvhlItemUsrBSDlg, CDialog)
	//{{AFX_MSG_MAP(CCMMvhlItemUsrBSDlg)
	ON_EN_CHANGE(IDC_CMD_MVHL_BS_W1_EDIT, OnChangeUniLoadEdit)
	ON_EN_CHANGE(IDC_CMD_MVHL_BS_W2_EDIT, OnChangeUniLoadEdit)
	ON_EN_CHANGE(IDC_CMD_MVHL_BS_L1_EDIT, OnChangeLoadLenEdit)
	ON_EN_CHANGE(IDC_CMD_MVHL_BS_D2_EDIT, OnChangeLoadDistEdit)
	ON_EN_CHANGE(IDC_CMD_MVHL_BS_D_EDIT,  OnChangeLoadDistEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CCMMvhlItemUsrBSDlg::SetData2Dlg()
{
//  m_dBSW1 = m_pData->dW1;
//  m_dBSW2 = m_pData->dW2;
//  m_dBSW3 = m_pData->dW3;
//  m_dBSL1 = m_pData->dLoadedLength;
//  m_dBSL2 = m_pData->dLoadedLength;
//  m_dBSPa = m_pData->dPa;
//  m_dBSPb = m_pData->dPb;
// 	m_dBSD1 = m_pData->dD1;
// 	m_dBSD2 = m_pData->dD2;
// 	m_dBSD  = m_pData->dDelta;
//  m_dBSD3 = m_dBSD2 + m_dBSD;
// 	m_dBSD4 = m_dBSD2 + m_dBSD * 2.;
// 	m_dBSD5 = m_dBSD2 + m_dBSD * 3.;
// 	m_dBSD6 = m_dBSD2 + m_dBSD * 4.;

	m_edtBSW1.SetEditUnit(m_pData->dW1);
	m_edtBSW2.SetEditUnit(m_pData->dW2);
	m_edtBSW3.SetEditUnit(m_pData->dW3);
	m_edtBSL1.SetEditUnit(m_pData->dLoadedLength);
	m_edtBSL2.SetEditUnit(m_pData->dLoadedLength);
	m_edtBSPa.SetEditUnit(m_pData->dPa);
	m_edtBSPb.SetEditUnit(m_pData->dPb);
	m_edtBSD1.SetEditUnit(m_pData->dD1);
	m_edtBSD2.SetEditUnit(m_pData->dD2);
	m_edtBSD .SetEditUnit(m_pData->dDelta);
	m_edtBSD3.SetEditUnit(m_pData->dD2 + m_pData->dDelta);
	m_edtBSD4.SetEditUnit(m_pData->dD2 + m_pData->dDelta * 2.);
	m_edtBSD5.SetEditUnit(m_pData->dD2 + m_pData->dDelta * 3.);
	m_edtBSD6.SetEditUnit(m_pData->dD2 + m_pData->dDelta * 4.);

	//m_strBSW2_exp.Format(_T("%.3g"),GetExpValue(m_dBSW1, m_dBSW2, m_dBSL1));
	m_strBSW2_exp.Format(_T("%.3g"),GetExpValue(m_pData->dW1, m_pData->dW2, m_pData->dLoadedLength));
	m_edtNumUnit.SetEditUnit(m_pData->dUnitNum);

	UpdateData(FALSE);
}

BOOL CCMMvhlItemUsrBSDlg::SetDlg2Data()
{
	UpdateData(TRUE);

//  m_pData->dW1           = m_dBSW1;
//  m_pData->dW2           = m_dBSW2;
//  m_pData->dW3           = m_dBSW3;
//  m_pData->dLoadedLength = m_dBSL1;
//  m_pData->dPa           = m_dBSPa;
//  m_pData->dPb           = m_dBSPb;
// 	m_pData->dD1           = m_dBSD1;
// 	m_pData->dD2           = m_dBSD2;
// 	m_pData->dDelta        = m_dBSD;

	m_pData->dW1           = m_edtBSW1.GetEditValue();
	m_pData->dW2           = m_edtBSW2.GetEditValue();
	m_pData->dW3           = m_edtBSW3.GetEditValue();
	m_pData->dLoadedLength = m_edtBSL1.GetEditValue();
	m_pData->dPa           = m_edtBSPa.GetEditValue();
	m_pData->dPb           = m_edtBSPb.GetEditValue();
	m_pData->dD1           = m_edtBSD1.GetEditValue();
	m_pData->dD2           = m_edtBSD2.GetEditValue();
	m_pData->dDelta        = m_edtBSD .GetEditValue();

	m_pData->dUnitNum = m_edtNumUnit.GetEditValue();

	m_pData->nLoadType = 0;

	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUsrBSDlg message handlers

BOOL CCMMvhlItemUsrBSDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// 단위 변환
	/*
	m_untBSW1.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dW3);
	m_untBSW2.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dW3);
	m_untBSW3.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dW3);
	m_untBSL1.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dLoadedLength);
	m_untBSL2.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dLoadedLength);
	*/
	m_untBSW1.SetTextFmt(_LSX(kN/m));
	m_untBSW1.SetUnitType(D_UNITSYS_NONE);
	m_untBSW2.SetTextFmt(_LSX(kN/m));
	m_untBSW2.SetUnitType(D_UNITSYS_NONE);
	m_untBSW3.SetTextFmt(_LSX(kN/m));
	m_untBSW3.SetUnitType(D_UNITSYS_NONE);
	m_untBSL1.SetTextFmt(_LSX(m));
	m_untBSL1.SetUnitType(D_UNITSYS_NONE);
	m_untBSL2.SetTextFmt(_LSX(m));
	m_untBSL2.SetUnitType(D_UNITSYS_NONE);

	m_untBSPa.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dPa);
	m_untBSPb.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dPb);
	m_untBSD1.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDelta);
	m_untBSD2.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDelta);
	m_untBSD .SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDelta);
	m_untBSD3.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDelta);
	m_untBSD4.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDelta);
	m_untBSD5.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDelta);
	m_untBSD6.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDelta);
	m_edtNumUnit.SetUnitType(D_UNITSYS_NONE);
	m_edtNumUnit.SetUnitType(D_UNITSYS_NONE);


	SetData2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvhlItemUsrBSDlg::OnChangeUniLoadEdit()
{
	UpdateData(TRUE);

	double dBSW1 = m_edtBSW1.GetEditValue();
	double dBSW2 = m_edtBSW2.GetEditValue();
	double dBSL1 = m_edtBSL1.GetEditValue();

	//m_strBSW2_exp.Format(_T("%.3g"), GetExpValue(m_dBSW1, m_dBSW2, m_dBSL1));
	m_strBSW2_exp.Format(_T("%.3g"), GetExpValue(dBSW1, dBSW2, dBSL1));

	UpdateData(FALSE);
}

void CCMMvhlItemUsrBSDlg::OnChangeLoadLenEdit()
{
	UpdateData(TRUE);

	double dBSW1 = m_edtBSW1.GetEditValue();
	double dBSW2 = m_edtBSW2.GetEditValue();
	double dBSL1 = m_edtBSL1.GetEditValue();

	//m_strBSW2_exp.Format(_T("%.3g"), GetExpValue(m_dBSW1, m_dBSW2, m_dBSL1));
	m_strBSW2_exp.Format(_T("%.3g"), GetExpValue(dBSW1, dBSW2, dBSL1));

	m_edtBSL2.SetEditUnit(dBSL1);

	UpdateData(FALSE);
}

void CCMMvhlItemUsrBSDlg::OnChangeLoadDistEdit()
{
	UpdateData(TRUE);

// 	m_dBSD3 = m_dBSD2 + m_dBSD;
// 	m_dBSD4 = m_dBSD2 + m_dBSD * 2.;
// 	m_dBSD5 = m_dBSD2 + m_dBSD * 3.;
// 	m_dBSD6 = m_dBSD2 + m_dBSD * 4.;

	double dBSD2 = m_edtBSD2.GetEditValue();
	double dBSD  = m_edtBSD .GetEditValue();

	m_edtBSD3.SetEditUnit(dBSD2 + dBSD);
	m_edtBSD4.SetEditUnit(dBSD2 + dBSD * 2.);
	m_edtBSD5.SetEditUnit(dBSD2 + dBSD * 3.);
	m_edtBSD6.SetEditUnit(dBSD2 + dBSD * 4.);

	UpdateData(FALSE);
}

double CCMMvhlItemUsrBSDlg::GetExpValue(double a, double b, double l)
{
	double dExp=0.;

	BOOL bCalc = TRUE;
	if(a <= 0. || b <= 0.) bCalc = FALSE;
	if(l <= 0. || l == 1.) bCalc = FALSE; 
	if(bCalc) dExp = log10(b/a) / log10(l);
	else      dExp = 0.475;

	return dExp;
}

BOOL CCMMvhlItemUsrBSDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==13)
		pMsg->wParam=9;
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==VK_ESCAPE)
		pMsg->wParam=VK_TAB;	
	return CInternationalDlg::PreTranslateMessage(pMsg);
}