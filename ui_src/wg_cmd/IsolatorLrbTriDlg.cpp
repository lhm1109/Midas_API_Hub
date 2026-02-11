// IsolatorLrbTriDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "IsolatorLrbTriDlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIsolatorLrbTriDlg dialog


CIsolatorLrbTriDlg::CIsolatorLrbTriDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CIsolatorLrbTriDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIsolatorLrbTriDlg)
	m_nRminRdo = 0;
	//}}AFX_DATA_INIT
}


void CIsolatorLrbTriDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIsolatorLrbTriDlg)
	DDX_Control(pDX, IDC_CMD_ISOLATOR_H_UNIT, m_wndHUnit);
	DDX_Control(pDX, IDC_CMD_ISOLATOR_AREA_UNIT, m_wndAreaUnit);
	DDX_Control(pDX, IDC_CMD_ISOLATOR_SW_CMB, m_wndSWCmb);
	DDX_Control(pDX, IDC_CMD_ISOLATOR_RMIN_EDIT, m_wndRminEdit);
	DDX_Control(pDX, IDC_CMD_ISOLATOR_H_EDIT, m_wndHEdit);
	DDX_Control(pDX, IDC_CMD_ISOLATOR_AREA_EDIT, m_wndAreaEdit);
	DDX_Radio(pDX, IDC_CMD_ISOLATOR_RMIN_RDO, m_nRminRdo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIsolatorLrbTriDlg, CDialogMove)
	//{{AFX_MSG_MAP(CIsolatorLrbTriDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIsolatorLrbTriDlg message handlers

BOOL CIsolatorLrbTriDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	m_wndHUnit.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dH);
	m_wndAreaUnit.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dArea);
	
	int nIndex;
	m_wndSWCmb.ResetContent();
	nIndex = m_wndSWCmb.AddString(_LSX(HDR-G12));
	m_wndSWCmb.SetItemData(nIndex, 1);
	nIndex = m_wndSWCmb.AddString(_LSX(HDR-G10));
	m_wndSWCmb.SetItemData(nIndex, 2);
	nIndex = m_wndSWCmb.AddString(_LSX(LRB-G12));
	m_wndSWCmb.SetItemData(nIndex, 3);
	nIndex = m_wndSWCmb.AddString(_LSX(LRB-G10));
	m_wndSWCmb.SetItemData(nIndex, 4);
	nIndex = m_wndSWCmb.AddString(_LSX(RB-G12));
	m_wndSWCmb.SetItemData(nIndex, 5);
	nIndex = m_wndSWCmb.AddString(_LSX(RB-G10));
	m_wndSWCmb.SetItemData(nIndex, 6);
	nIndex = m_wndSWCmb.AddString(_LSX(HDR-G8));
	m_wndSWCmb.SetItemData(nIndex, 7);
	nIndex = m_wndSWCmb.AddString(_LSX(HDR-S-G12));
	m_wndSWCmb.SetItemData(nIndex, 8);
	m_wndSWCmb.SetCurSel(0);
		
	Data2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CIsolatorLrbTriDlg::SetIehpPropData(T_IEHP_D iehpD, T_IEHP_LRBTRI data, int nModel, int nDof)
{
	m_Data = iehpD;    
	m_Data.AllProp.PROP[nDof].LRBTRI = data;  
	m_nMdlType = nModel;
	m_nDof = nDof;
}

bool CIsolatorLrbTriDlg::SyncDialogData()
{
	if (!Dlg2Data()) return false;

	return true;
}

void CIsolatorLrbTriDlg::Data2Dlg()
{
	int nD = m_nDof;
	m_wndSWCmb.SelectString(0, GetSwName(m_Data.AllProp.PROP[nD].LRBTRI.nSW));  
	m_wndHEdit.SetEditUnit(m_Data.AllProp.PROP[nD].LRBTRI.dH);
	m_wndAreaEdit.SetEditUnit(m_Data.AllProp.PROP[nD].LRBTRI.dArea);
	m_wndRminEdit.SetEditUnit(m_Data.AllProp.PROP[nD].LRBTRI.dRmin);  
	m_nRminRdo = m_Data.AllProp.PROP[nD].LRBTRI.nRminUpdate;
	UpdateData(FALSE);
}

BOOL CIsolatorLrbTriDlg::Dlg2Data()
{
	if(!CheckData()) return FALSE;
	UpdateData(TRUE);
	int nD = m_nDof;
	m_Data.AllProp.PROP[nD].LRBTRI.nSW   = m_wndSWCmb.GetItemData(m_wndSWCmb.GetCurSel());  
	m_Data.AllProp.PROP[nD].LRBTRI.dH    = m_wndHEdit.GetEditValue();
	m_Data.AllProp.PROP[nD].LRBTRI.dArea = m_wndAreaEdit.GetEditValue();
	m_Data.AllProp.PROP[nD].LRBTRI.dRmin = m_wndRminEdit.GetEditValue();  
	m_Data.AllProp.PROP[nD].LRBTRI.nRminUpdate = m_nRminRdo;
	return TRUE;
}

CString CIsolatorLrbTriDlg::GetSwName(int nSw)
{
	CString strSw = _T("");
	switch(nSw)
	{
		case 1: strSw = _LSX(HDR-G12);   break;
		case 2: strSw = _LSX(HDR-G10);   break;
		case 3: strSw = _LSX(LRB-G12);   break;
		case 4: strSw = _LSX(LRB-G10);   break;
		case 5: strSw = _LSX( RB-G12);   break;
		case 6: strSw = _LSX( RB-G10);   break;
		default: ASSERT(0); break;
	}
	return strSw;

}
BOOL CIsolatorLrbTriDlg::CheckErrorNameDataVar(LPCTSTR lpszData, LPCTSTR lpszName, LPCTSTR lpszVar, BOOL bMsg)
{
	if(bMsg) GSaveHistoryFormatNF(_LS(IDS_DB_IREG_DT_NAME_VAR_ERROR), lpszData, lpszName, lpszVar);
	return FALSE;
}

BOOL CIsolatorLrbTriDlg::CheckData(BOOL bMsg/*=TRUE*/)
{
#define CheckError(x, bMsg) CheckErrorNameDataVar(_LS(IDS_DB_DT_IEHP), m_Data.strName, x, bMsg)
	UpdateData(TRUE);
	if(m_wndHEdit.GetEditValue() < 0)    return CheckError(_T("H >= 0"), bMsg);
	if(m_wndAreaEdit.GetEditValue() < 0) return CheckError(_T("Area >= 0"), bMsg);  
	if(m_wndRminEdit.GetEditValue() < 0) return CheckError(_T("Rmin >= 0"), bMsg);
	return TRUE;
#undef CheckError
}

void CIsolatorLrbTriDlg::OnOK() 
{
	// TODO: Add extra validation here
	if(!Dlg2Data()) return;
	CDialogMove::OnOK();
}
