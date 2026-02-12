// WindChinaTransFlutter2011Dlg.cpp : implementation file
//

#include "stdafx.h"
#include <math.h>
#include "wg_cmd.h"
#include "CMWindItemChina2012CrossDlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"  
#include "..\wg_base\wg_base_DlgUtil.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMWindItemChina2012CrossDlg dialog
CMWindItemChina2012CrossDlg::CMWindItemChina2012CrossDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CMWindItemChina2012CrossDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMWindItemChina2012CrossDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nRoundOrRectRidio = 0;
	m_nCrossCheck = TRUE;
	m_nTorsionCheck = TRUE;
	
	m_dCrossPeriodX = 0;
	m_dCrossPeriodY = 0;
	m_dPeriodTorsion = 0.5;
}


void CMWindItemChina2012CrossDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMWindItemChina2012CrossDlg)
	DDX_Radio(pDX, IDC_CMD_WIND_RECT_RADIO, m_nRoundOrRectRidio);
	DDX_Check(pDX, IDC_CMD_WIND_CROSSWIND_CHECK, m_nCrossCheck);
	DDX_Check(pDX, IDC_CMD_WIND_TORSIONWIND_CHECK, m_nTorsionCheck);

	DDX_Control(pDX, IDC_CMD_PERIOD_X_EDIT, m_editCrossPeriodX);
	DDX_Control(pDX, IDC_CMD_PERIOD_Y_EDIT, m_editCrossPeriodY);
	DDX_Control(pDX, IDC_CMD_PERIOD_EDIT, m_editPeriodTorsion);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMWindItemChina2012CrossDlg, CDialogMove)
	//{{AFX_MSG_MAP(CMWindItemChina2012CrossDlg)
	ON_BN_CLICKED(IDC_CMD_WIND_CROSSWIND_CHECK, OnCheckCross)	
	ON_BN_CLICKED(IDC_CMD_WIND_TORSIONWIND_CHECK, OnCheckTorsion)
	ON_BN_CLICKED(IDC_CMD_WIND_RECT_RADIO, OnCheckRadio)	
	ON_BN_CLICKED(IDC_CMD_WIND_ROUND_RADIO, OnCheckRadio)		
	ON_BN_CLICKED(IDCANCEL, OnCancel)
	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMWindItemChina2012CrossDlg message handlers

void CMWindItemChina2012CrossDlg::SetData(T_WIND_CH2012& data)
{
	m_nRoundOrRectRidio = data.nSectType;
	m_nCrossCheck = data.bCross;
	m_nTorsionCheck = data.bTorsion;
	
	m_dCrossPeriodX = data.dCrossPeriod_X;
	m_dCrossPeriodY = data.dCrossPeriod_Y;
	m_dPeriodTorsion = data.dTorsionPeriod;
}


BOOL CMWindItemChina2012CrossDlg::OnInitDialog()//(T_CHINA_WIND_TRANS TransWind) 
{
	CDialogMove::OnInitDialog();
	// TODO: Add extra initialization here
	UpdateData(TRUE);

	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_WIND_RECT_RADIO, m_nCrossCheck == 1);
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_WIND_ROUND_RADIO, m_nCrossCheck == 1);
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_PERIOD_X_EDIT, m_nCrossCheck == 1);
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_PERIOD_Y_EDIT, m_nCrossCheck == 1);
	
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_PERIOD_EDIT, m_nTorsionCheck == 1);

	m_editCrossPeriodX.SetEditUnit(m_dCrossPeriodX);
	m_editCrossPeriodY.SetEditUnit(m_dCrossPeriodY);
	m_editPeriodTorsion.SetEditUnit(m_dPeriodTorsion);

	OnCheckCross();
	OnCheckTorsion();
	
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMWindItemChina2012CrossDlg::OnCheckCross() 
{
	UpdateData(TRUE);
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_WIND_RECT_RADIO, m_nCrossCheck == 1);
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_WIND_ROUND_RADIO, m_nCrossCheck == 1);
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_PERIOD_X_EDIT, m_nCrossCheck == 1);
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_PERIOD_Y_EDIT, m_nCrossCheck == 1);
	UpdateData(FALSE);
}

void CMWindItemChina2012CrossDlg::OnCheckTorsion() 
{
	UpdateData(TRUE);
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_PERIOD_EDIT, m_nTorsionCheck == 1);
	UpdateData(FALSE);
}
void CMWindItemChina2012CrossDlg::OnCheckRadio() 
{
	UpdateData(TRUE);
	if (m_nRoundOrRectRidio==0)
	{
		m_dCrossPeriodX = m_dCrossPeriodY = .0;
	}
	else 
	{
		m_dCrossPeriodX = m_dCrossPeriodY = .5;
	}
	m_editCrossPeriodX.SetEditUnit(m_dCrossPeriodX);
	m_editCrossPeriodY.SetEditUnit(m_dCrossPeriodY);
	UpdateData(FALSE);
}
void CMWindItemChina2012CrossDlg::OnCancel() 
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnCancel();	
}
void CMWindItemChina2012CrossDlg::OnOk() 
{
	// TODO: Add your control notification handler code here
	UpdateData();	

	m_dCrossPeriodX = m_editCrossPeriodX.GetEditValue();
	m_dCrossPeriodY = m_editCrossPeriodY.GetEditValue();
	m_dPeriodTorsion = m_editPeriodTorsion.GetEditValue();
	CDialogMove::OnOK();
}