// CMThkItemCalcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMThkItemCalcDlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMThkItemCalcDlg dialog


CCMThkItemCalcDlg::CCMThkItemCalcDlg(CDBDoc* pDoc, T_THIK_D* thik, CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMThkItemCalcDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMThkItemCalcDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc = pDoc;
	m_pThikData = thik;
}


void CCMThkItemCalcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMThkItemCalcDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	UINT aFirstID[][9] = {
		{ IDC_CMD_TP_CALC_EDIT1, IDC_CMD_TP_CALC_EDIT2, IDC_CMD_TP_CALC_EDIT3,
			IDC_CMD_TP_CALC_EDIT4, IDC_CMD_TP_CALC_EDIT5, IDC_CMD_TP_CALC_EDIT6,
			IDC_CMD_TP_CALC_EDIT7, IDC_CMD_TP_CALC_EDIT8, IDC_CMD_TP_CALC_EDIT9 },
		{ IDC_CMD_TP_CALC_UNIT1,  IDC_CMD_TP_CALC_UNIT2,  IDC_CMD_TP_CALC_UNIT3,
			IDC_CMD_TP_CALC_UNIT4,  IDC_CMD_TP_CALC_UNIT5,  IDC_CMD_TP_CALC_UNIT6,
			IDC_CMD_TP_CALC_UNIT7,  IDC_CMD_TP_CALC_UNIT8,  IDC_CMD_TP_CALC_UNIT9 },
	};
	for (int i = 0; i < 9; i++)
	{
		DDX_Control(pDX, aFirstID[0][i], m_wndEdit[i]);
		DDX_Control(pDX, aFirstID[1][i], m_wndUnit[i]);
	}
}


BEGIN_MESSAGE_MAP(CCMThkItemCalcDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMThkItemCalcDlg)
	ON_BN_CLICKED(IDC_CMD_TP_CALC_BTN_CANCEL, OnCmdTpCalcBtnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMThkItemCalcDlg message handlers

BOOL CCMThkItemCalcDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	MakeUnitSystem();
	Data2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMThkItemCalcDlg::MakeUnitSystem()
{
	m_wndUnit[0].SetUnitType(CUnitCtrl::m_THIK_UNIT.Stiffened.dWeightThik);
	m_wndUnit[1].SetUnitType(CUnitCtrl::m_THIK_UNIT.Stiffened.Value.ThickIn);
	m_wndUnit[2].SetUnitType(CUnitCtrl::m_THIK_UNIT.Stiffened.Value.ThickOut);
	m_wndUnit[3].SetUnitType(CUnitCtrl::m_THIK_UNIT.Stiffened.Value.Hu);
	m_wndUnit[4].SetUnitType(CUnitCtrl::m_THIK_UNIT.Stiffened.Value.Hl);
	m_wndUnit[5].SetUnitType(CUnitCtrl::m_THIK_UNIT.Stiffened.Value.ThickIn);
	m_wndUnit[6].SetUnitType(CUnitCtrl::m_THIK_UNIT.Stiffened.Value.ThickOut);
	m_wndUnit[7].SetUnitType(CUnitCtrl::m_THIK_UNIT.Stiffened.Value.Hu);
	m_wndUnit[8].SetUnitType(CUnitCtrl::m_THIK_UNIT.Stiffened.Value.Hl);
}

void CCMThkItemCalcDlg::Data2Dlg()
{
	m_wndEdit[0].SetEditUnit(m_pThikData->Stiffened.dWeightThik);
	m_wndEdit[1].SetEditUnit(m_pThikData->Stiffened.yzValue.ThickIn);
	m_wndEdit[2].SetEditUnit(m_pThikData->Stiffened.yzValue.ThickOut);
	m_wndEdit[3].SetEditUnit(m_pThikData->Stiffened.yzValue.Hu);
	m_wndEdit[4].SetEditUnit(m_pThikData->Stiffened.yzValue.Hl);
	m_wndEdit[5].SetEditUnit(m_pThikData->Stiffened.xzValue.ThickIn);
	m_wndEdit[6].SetEditUnit(m_pThikData->Stiffened.xzValue.ThickOut);
	m_wndEdit[7].SetEditUnit(m_pThikData->Stiffened.xzValue.Hu);
	m_wndEdit[8].SetEditUnit(m_pThikData->Stiffened.xzValue.Hl);
}

void CCMThkItemCalcDlg::OnCmdTpCalcBtnCancel() 
{
	// TODO: Add your control notification handler code here
	OnOK();
}
