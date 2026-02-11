// WindGustKBC2009Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldWindPressureAreaDlg_CH_Period.h"

#include <math.h>

#include "..\wg_db\wg_db_UnitCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMWindPressureArea_KBC2009_GustF dialog
CStldWindPressureAreaDlg_CH_Period::CStldWindPressureAreaDlg_CH_Period(CWnd* pParent /*=NULL*/)
	: CDialogMove(CStldWindPressureAreaDlg_CH_Period::IDD, pParent)
{

}


void CStldWindPressureAreaDlg_CH_Period::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TM_CH_PERIOD_H, m_edtH);
	DDX_Control(pDX, IDC_TM_CH_PERIOD_B, m_edtB);
	DDX_Control(pDX, IDC_TM_CH_PERIOD_N, m_edtN);
}

BEGIN_MESSAGE_MAP(CStldWindPressureAreaDlg_CH_Period, CDialogMove)
	ON_BN_CLICKED(IDC_RDO_XMETHOD1,        OnCmdRdoTypeChanged) 
	ON_BN_CLICKED(IDC_RDO_XMETHOD2,        OnCmdRdoTypeChanged) 
	ON_BN_CLICKED(IDC_RDO_XMETHOD3,        OnCmdRdoTypeChanged) 
	ON_BN_CLICKED(IDC_RDO_XMETHOD4,        OnCmdRdoTypeChanged) 
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMWindPressureArea_KBC2009_GustF message handlers

BOOL CStldWindPressureAreaDlg_CH_Period::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	InitControl();
	
	OnCmdRdoTypeChanged();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStldWindPressureAreaDlg_CH_Period::OnOK() 
{
	m_nType = GetType();

	Dlg2Data();

	if(!CalcPeriod(m_dH, m_dB, m_dN, m_nType, m_dPeriod))
	{
		m_dPeriod = 0.0;
		return;
	}

	CDialogMove::OnOK();
}

void CStldWindPressureAreaDlg_CH_Period::OnCancel() 
{
	CDialogMove::OnCancel();		
}

void CStldWindPressureAreaDlg_CH_Period::InitControl()
{
	BOOL bSetType[4];
	if(m_nType == 2)
	{
		bSetType[0] = FALSE;    bSetType[1] = TRUE;
		bSetType[2] = FALSE;    bSetType[3] = FALSE;
	}
	else if(m_nType == 3)
	{
		bSetType[0] = FALSE;    bSetType[1] = FALSE;
		bSetType[2] = TRUE;     bSetType[3] = FALSE;
	}
	else if(m_nType == 4)
	{
		bSetType[0] = FALSE;    bSetType[1] = FALSE;
		bSetType[2] = FALSE;    bSetType[3] = TRUE;
	}
	else
	{
		m_nType = 1;
		bSetType[0] = TRUE;     bSetType[1] = FALSE;
		bSetType[2] = FALSE;    bSetType[3] = FALSE;
	}
	((CButton*)GetDlgItem(IDC_RDO_XMETHOD1))->SetCheck(bSetType[0]);
	((CButton*)GetDlgItem(IDC_RDO_XMETHOD2))->SetCheck(bSetType[1]);
	((CButton*)GetDlgItem(IDC_RDO_XMETHOD3))->SetCheck(bSetType[2]);
	((CButton*)GetDlgItem(IDC_RDO_XMETHOD4))->SetCheck(bSetType[3]);

	m_edtH.SetUnitType(D_UNITSYS_NONE);
	m_edtB.SetUnitType(D_UNITSYS_NONE);
	m_edtN.SetUnitType(D_UNITSYS_NONE);

	
	m_edtH.SetEditUnit(m_dH);
	m_edtN.SetEditUnit(m_dN);
	m_edtB.SetEditUnit(m_dB);
}


void CStldWindPressureAreaDlg_CH_Period::Data2Dlg()
{
	
}

void CStldWindPressureAreaDlg_CH_Period::Dlg2Data()
{
	m_dH = m_edtH.GetEditValue();
	m_dN = m_edtN.GetEditValue();
	m_dB = m_edtB.GetEditValue();  
}

int CStldWindPressureAreaDlg_CH_Period::GetType()
{
	int nType = 1;
	if(((CButton*)GetDlgItem(IDC_RDO_XMETHOD1))->GetCheck())
		nType = 1;
	else if(((CButton*)GetDlgItem(IDC_RDO_XMETHOD2))->GetCheck())
		nType = 2;
	else if(((CButton*)GetDlgItem(IDC_RDO_XMETHOD3))->GetCheck())
		nType = 3;
	else if(((CButton*)GetDlgItem(IDC_RDO_XMETHOD4))->GetCheck())
		nType = 4;
	else 
	{
		nType = 1;
		((CButton*)GetDlgItem(IDC_RDO_XMETHOD1))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RDO_XMETHOD2))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RDO_XMETHOD3))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RDO_XMETHOD4))->SetCheck(FALSE);
	}
	return nType;
}

void CStldWindPressureAreaDlg_CH_Period::OnCmdRdoTypeChanged()
{
	m_nType = GetType();

	if(m_nType == 1 || m_nType == 2 || m_nType == 3)
	{
		m_edtH.EnableWindow(TRUE);
		m_edtB.EnableWindow(TRUE);
		m_edtN.EnableWindow(FALSE);
	}
	else if(m_nType == 4)
	{
		m_edtH.EnableWindow(FALSE);
		m_edtB.EnableWindow(FALSE);
		m_edtN.EnableWindow(TRUE);
	}
	else
		ASSERT(0);  
}


BOOL CStldWindPressureAreaDlg_CH_Period::CalcPeriod(double dH, double dB, int nN, int nMethod, double& dPeriod)
{
	if(nMethod == 1 || nMethod == 2 || nMethod == 3) 
	{
		if(dB <= 0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__the_value_B_is_invalid_));
			return FALSE;
		}
	}
	
	switch(nMethod)
	{
		case 1: dPeriod = 0.22 + 0.035 * dH / pow(dB, (1.0/3.0));	break;
		case 2: dPeriod = 0.25 + 0.00053 * dH * dH / pow(dB, (1.0/3.0));	break;
		case 3: dPeriod = 0.03 + 0.03 * dH / pow(dB, (1.0/3.0));	break;
		case 4: dPeriod = 0.1 * nN;	break;		
	}
	return TRUE;
}

void CStldWindPressureAreaDlg_CH_Period::SetInit( double dB, double dH, int dN )
{
	m_dB = dB;

	m_dH = dH;
	m_dN = dN;
}
