// IsolatorLrbBilDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "IsolatorLrbBilDlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIsolatorLrbBilDlg dialog


CIsolatorLrbBilDlg::CIsolatorLrbBilDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CIsolatorLrbBilDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIsolatorLrbBilDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CIsolatorLrbBilDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIsolatorLrbBilDlg)
	DDX_Control(pDX, IDC_CMD_ISOLATOR_RMIN_EDIT, m_wndRminEdit);
	DDX_Control(pDX, IDC_CMD_ISOLATOR_H_UNIT, m_wndHUnit);
	DDX_Control(pDX, IDC_CMD_ISOLATOR_ALF_EDIT, m_wndAlfEdit);
	DDX_Control(pDX, IDC_CMD_ISOLATOR_H_EDIT, m_wndHEdit);
	DDX_Control(pDX, IDC_CMD_ISOLATOR_QD50_UNIT, m_wndQd50Unit);
	DDX_Control(pDX, IDC_CMD_ISOLATOR_QD50_EDIT, m_wndQd50Edit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIsolatorLrbBilDlg, CDialogMove)
	//{{AFX_MSG_MAP(CIsolatorLrbBilDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIsolatorLrbBilDlg message handlers
BOOL CIsolatorLrbBilDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	m_wndQd50Unit.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dQd50);
	m_wndHUnit.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dH);
	
	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CIsolatorLrbBilDlg::SetIehpPropData(T_IEHP_D iehpD, T_IEHP_LRBBIL data, int nModel, int nDof)
{
	m_Data = iehpD;    
	m_Data.AllProp.PROP[nDof].LRBBIL = data;  
	m_nMdlType = nModel;
	m_nDof = nDof;
}

bool CIsolatorLrbBilDlg::SyncDialogData()
{
	if (!Dlg2Data()) return false;

	return true;
}

void CIsolatorLrbBilDlg::Data2Dlg()
{
	int nD = m_nDof;
	m_wndQd50Edit.SetEditUnit(m_Data.AllProp.PROP[nD].LRBBIL.dQd50);
	m_wndHEdit.SetEditUnit(m_Data.AllProp.PROP[nD].LRBBIL.dH);
	m_wndAlfEdit.SetEditUnit(m_Data.AllProp.PROP[nD].LRBBIL.dAlf);
	m_wndRminEdit.SetEditUnit(m_Data.AllProp.PROP[nD].LRBBIL.dRmin);
}

BOOL CIsolatorLrbBilDlg::Dlg2Data()
{
	if(!CheckData()) return FALSE;
	UpdateData(TRUE);
	int nD = m_nDof;
	m_Data.AllProp.PROP[nD].LRBBIL.dQd50 = m_wndQd50Edit.GetEditValue();
	m_Data.AllProp.PROP[nD].LRBBIL.dH    = m_wndHEdit.GetEditValue();
	m_Data.AllProp.PROP[nD].LRBBIL.dAlf  = m_wndAlfEdit.GetEditValue();
	m_Data.AllProp.PROP[nD].LRBBIL.dRmin = m_wndRminEdit.GetEditValue();  
	return TRUE;
}

BOOL CIsolatorLrbBilDlg::CheckErrorNameDataVar(LPCTSTR lpszData, LPCTSTR lpszName, LPCTSTR lpszVar, BOOL bMsg)
{
	if(bMsg) GSaveHistoryFormatNF(_LS(IDS_DB_IREG_DT_NAME_VAR_ERROR), lpszData, lpszName, lpszVar);
	return FALSE;
}

BOOL CIsolatorLrbBilDlg::CheckData(BOOL bMsg/*=TRUE*/)
{
#define CheckError(x, bMsg) CheckErrorNameDataVar(_LS(IDS_DB_DT_IEHP), m_Data.strName, x, bMsg)
	UpdateData(TRUE);
	if(m_wndQd50Edit.GetEditValue() < 0) return CheckError(_T("Qd50 >= 0"), bMsg);
	if(m_wndHEdit.GetEditValue() < 0)    return CheckError(_T("H >= 0"), bMsg);
	if(m_wndAlfEdit.GetEditValue() < 0)  return CheckError(_T("ALF(a) >= 0"), bMsg);
	if(m_wndRminEdit.GetEditValue() < 0) return CheckError(_T("Rmin >= 0"), bMsg);
	return TRUE;
#undef CheckError
}

void CIsolatorLrbBilDlg::OnOK() 
{
	// TODO: Add extra validation here
	if(!Dlg2Data()) return;
	CDialogMove::OnOK();
}
	
