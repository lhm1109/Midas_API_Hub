// WindGustKBC2009Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldWindPressureAreaDlg_CH2012_CrossTorsion.h"

// #include "..\wg_base\wg_base_StrParser.h"
// #include "..\wg_base\wg_base_MsgDll.h"  
// #include "..\wg_db\DB_ST_DT.h"
// #include "..\wg_db\wg_db_UnitCtrl.h"
// #include "..\wg_db\wg_db_DBDoc.h"
// #include "..\wg_db\AttrCtrl2.h"
// #include "..\wg_db\EditData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMWindPressureArea_KBC2009_GustF dialog
CStldWindPressureAreaDlg_CH2012_CrossTorsion::CStldWindPressureAreaDlg_CH2012_CrossTorsion(CWnd* pParent /*=NULL*/)
	: CDialogMove(CStldWindPressureAreaDlg_CH2012_CrossTorsion::IDD, pParent)
{
	m_bCross = TRUE;
	m_nType = 1;
	m_dCrossPeriod = 0.0;
	m_bTorsion = TRUE;
	m_dTorsionPeriod = 0.0;
}


void CStldWindPressureAreaDlg_CH2012_CrossTorsion::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TM_PERIOD_X_EDIT, m_edtCrossPeriod);
	DDX_Control(pDX, IDC_TM_PERIOD_EDIT,   m_edtTorsionPeriod);
}

BEGIN_MESSAGE_MAP(CStldWindPressureAreaDlg_CH2012_CrossTorsion, CDialogMove)
	ON_BN_CLICKED(IDC_TM_WIND_CROSSWIND_CHECK,   OnChkCross)  
	ON_BN_CLICKED(IDC_TM_WIND_TORSIONWIND_CHECK, OnChkTorsion)  
	ON_BN_CLICKED(IDC_TM_WIND_RECT_RADIO,        OnRdoRect)  
	ON_BN_CLICKED(IDC_TM_WIND_ROUND_RADIO,       OnRdoReound)  
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMWindPressureArea_KBC2009_GustF message handlers

BOOL CStldWindPressureAreaDlg_CH2012_CrossTorsion::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	Data2Dlg();
	OnChkTorsion();
	OnChkCross();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStldWindPressureAreaDlg_CH2012_CrossTorsion::OnOK() 
{
	Dlg2Data();

	CDialogMove::OnOK();
}

void CStldWindPressureAreaDlg_CH2012_CrossTorsion::OnCancel() 
{
	CDialogMove::OnCancel();		
}

void CStldWindPressureAreaDlg_CH2012_CrossTorsion::OnChkTorsion()
{
	CButton* pChkTorsion = (CButton*)GetDlgItem(IDC_TM_WIND_TORSIONWIND_CHECK);

	if(pChkTorsion->GetCheck()) GetDlgItem(IDC_TM_PERIOD_EDIT)->EnableWindow(TRUE);
	else                        GetDlgItem(IDC_TM_PERIOD_EDIT)->EnableWindow(FALSE);  
}

void CStldWindPressureAreaDlg_CH2012_CrossTorsion::OnChkCross()
{
	CButton* pChkCross = (CButton*)GetDlgItem(IDC_TM_WIND_CROSSWIND_CHECK);
	
	if(pChkCross->GetCheck())
	{
		GetDlgItem(IDC_TM_WIND_RECT_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_TM_WIND_ROUND_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_TM_PERIOD_X_EDIT)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_TM_WIND_RECT_RADIO)->EnableWindow(FALSE); 
		GetDlgItem(IDC_TM_WIND_ROUND_RADIO)->EnableWindow(FALSE); 
		GetDlgItem(IDC_TM_PERIOD_X_EDIT)->EnableWindow(FALSE);  
	}
}

void CStldWindPressureAreaDlg_CH2012_CrossTorsion::Dlg2Data()
{
	CButton* pChkCross = (CButton*)GetDlgItem(IDC_TM_WIND_CROSSWIND_CHECK);
	CButton* pChkTorsion = (CButton*)GetDlgItem(IDC_TM_WIND_TORSIONWIND_CHECK);
	
	if(pChkCross->GetCheck())   m_bCross = TRUE;
	else                        m_bCross = FALSE;
	if(m_bCross)
	{
		CButton* pRdoRect  = (CButton*)GetDlgItem(IDC_TM_WIND_RECT_RADIO);
		CButton* pRdoRound = (CButton*)GetDlgItem(IDC_TM_WIND_ROUND_RADIO);
		
		if(pRdoRound->GetCheck())     m_nType = 2;
		else if(pRdoRect->GetCheck()) m_nType = 1;
		else
		{
			m_nType = 1;
			pRdoRect->SetCheck(TRUE);
		}
		m_dCrossPeriod = m_edtCrossPeriod.GetEditValue();
	}
	
	if(pChkTorsion->GetCheck()) m_bTorsion = TRUE;
	else                        m_bTorsion = FALSE; 
	if(m_bTorsion)
	{
		m_dTorsionPeriod = m_edtTorsionPeriod.GetEditValue();
	}  
}

void CStldWindPressureAreaDlg_CH2012_CrossTorsion::Data2Dlg()
{
	CButton* pChkCross = (CButton*)GetDlgItem(IDC_TM_WIND_CROSSWIND_CHECK);
	CButton* pChkTorsion = (CButton*)GetDlgItem(IDC_TM_WIND_TORSIONWIND_CHECK);
	
	CButton* pRdoRect  = (CButton*)GetDlgItem(IDC_TM_WIND_RECT_RADIO);
	CButton* pRdoRound = (CButton*)GetDlgItem(IDC_TM_WIND_ROUND_RADIO);

	if(m_bCross) pChkCross->SetCheck(TRUE);
	else         pChkCross->SetCheck(FALSE);

	if(m_nType == 1)      { pRdoRect->SetCheck(TRUE);  pRdoRound->SetCheck(FALSE); }
	else if(m_nType == 2) { pRdoRect->SetCheck(FALSE); pRdoRound->SetCheck(TRUE);  }
	else                  { pRdoRect->SetCheck(TRUE);  pRdoRound->SetCheck(FALSE); m_nType = 1; }

	m_edtCrossPeriod.SetEditUnit(m_dCrossPeriod);

	if(m_bTorsion) pChkTorsion->SetCheck(TRUE);
	else           pChkTorsion->SetCheck(FALSE);

	m_edtTorsionPeriod.SetEditUnit(m_dTorsionPeriod);
}

void CStldWindPressureAreaDlg_CH2012_CrossTorsion::GetData( T_WDPR_CH2012& rWdprCH2012 )
{
	rWdprCH2012.bCross         = m_bCross;
	rWdprCH2012.nSectType      = m_nType;
	rWdprCH2012.dCrossPeriod   = m_dCrossPeriod;
	rWdprCH2012.bTorsion       = m_bTorsion;
	rWdprCH2012.dTorsionPeriod = m_dTorsionPeriod;
}

void CStldWindPressureAreaDlg_CH2012_CrossTorsion::SetData( const T_WDPR_CH2012& rWdprCH2012 )
{
	m_bCross         = rWdprCH2012.bCross;
	m_nType          = rWdprCH2012.nSectType;
	m_dCrossPeriod   = rWdprCH2012.dCrossPeriod;
	m_bTorsion       = rWdprCH2012.bTorsion;
	m_dTorsionPeriod = rWdprCH2012.dTorsionPeriod;
}

void CStldWindPressureAreaDlg_CH2012_CrossTorsion::OnRdoReound()
{
	m_edtCrossPeriod.SetEditUnit(0.5);  
}

void CStldWindPressureAreaDlg_CH2012_CrossTorsion::OnRdoRect()
{
	m_edtCrossPeriod.SetEditUnit(0.0);
}
