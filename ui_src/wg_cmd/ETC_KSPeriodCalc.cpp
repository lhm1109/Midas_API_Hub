// ETC_KSPeriodCalc.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_KSPeriodCalc.h"


#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include <math.h>
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\DB_ST_DT.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CETC_KSPeriodCalc dialog

CETC_KSPeriodCalc::CETC_KSPeriodCalc(CWnd* pParent /*=NULL*/)
	: CDialogMove(CETC_KSPeriodCalc::IDD, pParent)
{
	//{{AFX_DATA_INIT(CETC_KSPeriodCalc)
	m_nXMethod2000=0;
	m_nYMethod2000=0;
	//}}AFX_DATA_INIT

	m_arOldType.Add(IDC_RDO_XMETHOD1);
	m_arOldType.Add(IDC_RDO_XMETHOD2);
	m_arOldType.Add(IDC_RDO_XMETHOD3);
	m_arOldType.Add(IDC_RDO_YMETHOD1);
	m_arOldType.Add(IDC_RDO_YMETHOD2);
	m_arOldType.Add(IDC_RDO_YMETHOD3);

	m_arNewType.Add(IDC_CMD_XMETHOD1_KS2000);
	m_arNewType.Add(IDC_CMD_XMETHOD2_KS2000);
	m_arNewType.Add(IDC_CMD_XMETHOD3_KS2000);
	m_arNewType.Add(IDC_CMD_XMETHOD4_KS2000);
	m_arNewType.Add(IDC_CMD_YMETHOD1_KS2000);
	m_arNewType.Add(IDC_CMD_YMETHOD2_KS2000);
	m_arNewType.Add(IDC_CMD_YMETHOD3_KS2000);
	m_arNewType.Add(IDC_CMD_YMETHOD4_KS2000);

	m_arAC.Add(IDC_CMD_ACX_TITLE);
	m_arAC.Add(IDC_CMD_SEIS_XAc);
	m_arAC.Add(IDC_CMD_ACX_UNIT);
	m_arAC.Add(IDC_CMD_ACY_TITLE);
	m_arAC.Add(IDC_CMD_SEIS_YAc);
	m_arAC.Add(IDC_CMD_ACY_UNIT);
	
	m_arB.Add(IDC_CMD_BX_TITLE);
	m_arB.Add(IDC_ETC_SEIS_XB);
	m_arB.Add(IDC_CMD_BX_UNIT);
	m_arB.Add(IDC_CMD_BY_TITLE);
	m_arB.Add(IDC_ETC_SEIS_YB);
	m_arB.Add(IDC_CMD_BY_UNIT);
}


void CETC_KSPeriodCalc::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CETC_KSPeriodCalc)
	DDX_Control(pDX, IDC_ETC_SEIS_YHn, m_edtHn_y);
	DDX_Control(pDX, IDC_ETC_SEIS_XHn, m_edtHn_x);
	DDX_Control(pDX, IDC_CMD_SEIS_XAc, m_edtXAc);
	DDX_Control(pDX, IDC_CMD_SEIS_YAc, m_edtYAc);
	DDX_Control(pDX, IDC_ETC_SEIS_YB, m_edtBy);
	DDX_Control(pDX, IDC_ETC_SEIS_XB, m_edtBx);
	DDX_Radio(pDX, IDC_RDO_XMETHOD1, m_nXMethod);
	DDX_Radio(pDX, IDC_RDO_YMETHOD1, m_nYMethod);
	DDX_Radio(pDX, IDC_CMD_XMETHOD1_KS2000, m_nXMethod2000);
	DDX_Radio(pDX, IDC_CMD_YMETHOD1_KS2000, m_nYMethod2000);
	//}}AFX_DATA_MAP
	/*
	DDX_Text(pDX, IDC_CMD_SEIS_YAc, m_dAc_Y);
	DDX_Text(pDX, IDC_CMD_SEIS_XAc, m_dAc_X);
	DDX_Text(pDX, IDC_ETC_SEIS_XHn, m_dHn_x);
	DDX_Text(pDX, IDC_ETC_SEIS_XB, m_dBx);
	DDX_Text(pDX, IDC_ETC_SEIS_YB, m_dBy);
	DDX_Text(pDX, IDC_ETC_SEIS_YHn, m_dHn_y);
	*/
	if (pDX->m_bSaveAndValidate)
	{
		m_dAc_X = m_edtXAc .GetEditValue();
		m_dAc_Y = m_edtYAc .GetEditValue();
		m_dHn_x = m_edtHn_x.GetEditValue();
		m_dHn_y = m_edtHn_y.GetEditValue();
		m_dBx   = m_edtBx  .GetEditValue();
		m_dBy   = m_edtBy  .GetEditValue();
	}
	else
	{
		m_edtXAc .SetEditUnit(m_dAc_X);
		m_edtYAc .SetEditUnit(m_dAc_Y);
		m_edtHn_x.SetEditUnit(m_dHn_x);
		m_edtHn_y.SetEditUnit(m_dHn_y);
		m_edtBx  .SetEditUnit(m_dBx  );
		m_edtBy  .SetEditUnit(m_dBy  );
	}
}


BEGIN_MESSAGE_MAP(CETC_KSPeriodCalc, CDialogMove)
	//{{AFX_MSG_MAP(CETC_KSPeriodCalc)
	ON_BN_CLICKED(IDC_RDO_XMETHOD1, OnRdoXmethodChange)
	ON_BN_CLICKED(IDC_RDO_YMETHOD1, OnRdoYmethodChange)
	ON_BN_CLICKED(IDC_RDO_XMETHOD2, OnRdoXmethodChange)
	ON_BN_CLICKED(IDC_RDO_XMETHOD3, OnRdoXmethodChange)
	ON_BN_CLICKED(IDC_RDO_YMETHOD2, OnRdoYmethodChange)
	ON_BN_CLICKED(IDC_RDO_YMETHOD3, OnRdoYmethodChange)
	ON_BN_CLICKED(IDC_CMD_XMETHOD1_KS2000, OnCmdXmethodKs2000)
	ON_BN_CLICKED(IDC_CMD_XMETHOD2_KS2000, OnCmdXmethodKs2000)
	ON_BN_CLICKED(IDC_CMD_XMETHOD3_KS2000, OnCmdXmethodKs2000)
	ON_BN_CLICKED(IDC_CMD_XMETHOD4_KS2000, OnCmdXmethodKs2000)
	ON_BN_CLICKED(IDC_CMD_YMETHOD1_KS2000, OnCmdYmethodKs2000)
	ON_BN_CLICKED(IDC_CMD_YMETHOD2_KS2000, OnCmdYmethodKs2000)
	ON_BN_CLICKED(IDC_CMD_YMETHOD3_KS2000, OnCmdYmethodKs2000)
	ON_BN_CLICKED(IDC_CMD_YMETHOD4_KS2000, OnCmdYmethodKs2000)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CETC_KSPeriodCalc message handlers

void CETC_KSPeriodCalc::AlignControl()
{
	CRect rRef;
	CRect rToMove;
	int nDistY;

	GetDlgItem(m_arOldType.GetAt(0))->GetWindowRect(rRef);
	GetDlgItem(m_arNewType.GetAt(0))->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_arNewType, nDistY);

	GetDlgItem(m_arB.GetAt(0))->GetWindowRect(rRef);
	GetDlgItem(m_arAC.GetAt(0))->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_arAC, nDistY);

	CWnd* pWndLast = GetDlgItem(IDOK);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+globalUtils.ScaleByDPI(10);
	MoveWindow(r);	
}

BOOL CETC_KSPeriodCalc::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	AlignControl();

	if(m_nCodeType == 5)
	{
		CDlgUtil::CtrlShowHide(this, m_arNewType, TRUE);
		CDlgUtil::CtrlShowHide(this, m_arAC, TRUE);
		CDlgUtil::CtrlShowHide(this, m_arOldType, FALSE);
		CDlgUtil::CtrlShowHide(this, m_arB, FALSE);
		m_edtXAc.SetEditUnit(m_dAc_X);
		m_edtYAc.SetEditUnit(m_dAc_Y);
		OnCmdXmethodKs2000();
		OnCmdYmethodKs2000();
	}
	else
	{
		CDlgUtil::CtrlShowHide(this, m_arNewType, FALSE);
		CDlgUtil::CtrlShowHide(this, m_arAC, FALSE);
		CDlgUtil::CtrlShowHide(this, m_arOldType, TRUE);
		CDlgUtil::CtrlShowHide(this, m_arB, TRUE);
//		m_edtBx.EnableWindow(m_nXMethod == 2);
//		m_edtBy.EnableWindow(m_nYMethod == 2);
		m_edtBx.SetEditUnit(m_dBx);
		m_edtBy.SetEditUnit(m_dBy);
		OnRdoXmethodChange();
		OnRdoYmethodChange();
	}
	
	m_edtHn_x.SetEditUnit(m_dHn_x);
	m_edtHn_y.SetEditUnit(m_dHn_y);
	
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CETC_KSPeriodCalc::CalcPeriod(double dB, double dHn, double dAc, double &dPeriod,int nMethod)
{
	if(m_nCodeType == 5)
	{
		switch(nMethod)
		{
			case 0:
				dPeriod = 0.0853*pow(dHn,(3.0/4.0));
				break;
			case 1:
				dPeriod = 0.0731*pow(dHn,(3.0/4.0));
				break;
			case 2:
				dPeriod = 0.0488*pow(dHn,(3.0/4.0));
				break;
			case 3:
				if (dAc <= 0)
				{
					AfxMessageBox(_LS(IDS_WG_CMD__ADDD__the_value_Ac_is_invalid_));
					return FALSE;
				}
				dPeriod = 0.0743*pow(dHn,(3.0/4.0)) / sqrt(dAc);
				break;
		}		
	}
	else
	{
		switch(nMethod)
		{
			case 0:
				dPeriod = 0.085*pow(dHn,(3.0/4.0));
				break;
			case 1:
				dPeriod = 0.06*pow(dHn,(3.0/4.0));
				break;
			case 2:
				if(dB<=0)
				{
					AfxMessageBox(_LS(IDS_WG_CMD__ADDD__the_value_B_is_invalid_));
					return FALSE;
				}
				dPeriod = 0.09*dHn/sqrt(dB);
		}
	}
	return TRUE;
}

void CETC_KSPeriodCalc::OnOK() 
{
	UpdateData(TRUE);
	if(m_nCodeType == 5)
	{
		if(!CalcPeriod(m_dBx,m_dHn_x,m_dAc_X,m_dPeriod_x,m_nXMethod2000)) return;
		if(!CalcPeriod(m_dBy,m_dHn_y,m_dAc_Y,m_dPeriod_y,m_nYMethod2000)) return;
		m_dAc_X = m_edtXAc.GetEditValue();
		m_dAc_Y = m_edtYAc.GetEditValue();
	}
	else
	{
		if(!CalcPeriod(m_dBx,m_dHn_x,m_dAc_X,m_dPeriod_x,m_nXMethod)) return;
		if(!CalcPeriod(m_dBy,m_dHn_y,m_dAc_Y,m_dPeriod_y,m_nYMethod)) return;
		m_dBx = m_edtBx.GetEditValue();
		m_dBy = m_edtBy.GetEditValue();
	}	
	m_dHn_x = m_edtHn_x.GetEditValue();
	m_dHn_y = m_edtHn_y.GetEditValue();

	CDialogMove::OnOK();
}

void CETC_KSPeriodCalc::OnRdoXmethodChange() 
{
	UpdateData(TRUE);
	m_edtBx.EnableWindow(m_nXMethod==2);
		
}

void CETC_KSPeriodCalc::OnRdoYmethodChange() 
{
	UpdateData(TRUE);
	m_edtBy.EnableWindow(m_nYMethod==2);	
}


void CETC_KSPeriodCalc::OnCmdXmethodKs2000() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
	m_edtXAc.EnableWindow(m_nXMethod2000 == 3);
}

void CETC_KSPeriodCalc::OnCmdYmethodKs2000() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
	m_edtYAc.EnableWindow(m_nYMethod2000 == 3);	
}
