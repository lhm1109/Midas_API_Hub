// IsolatorDmpRbrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "IsolatorLrbH30Dlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIsolatorLrbH30Dlg dialog


CIsolatorLrbH30Dlg::CIsolatorLrbH30Dlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CIsolatorLrbH30Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIsolatorLrbH30Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CIsolatorLrbH30Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIsolatorLrbH30Dlg)
	DDX_Control(pDX, IDC_CMD_ISOLATOR_SW_CMB, m_wndSWCmb);
	DDX_Control(pDX, IDC_CMD_ISOLATOR_H_UNIT, m_wndHUnit);
	DDX_Control(pDX, IDC_CMD_ISOLATOR_AREA_UNIT, m_wndEffAreaUnit);
	DDX_Control(pDX, IDC_CMD_ISOLATOR_AREA_UNIT3, m_wndPlugAreaUnit);
	DDX_Control(pDX, IDC_CMD_ISOLATOR_H_EDIT, m_wndHEdit);
	DDX_Control(pDX, IDC_CMD_ISOLATOR_AREA_EDIT, m_wndEffAreaEdit);
	DDX_Control(pDX, IDC_CMD_ISOLATOR_AREA_EDIT3, m_wndPlugAreaEdit);
	DDX_Control(pDX, IDC_CMD_ISOLATOR_GS_EDIT, m_wndCbEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIsolatorLrbH30Dlg, CDialogMove)
	//{{AFX_MSG_MAP(CIsolatorLrbH30Dlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIsolatorLrbH30Dlg message handlers

BOOL CIsolatorLrbH30Dlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();
	
	m_wndHUnit.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dH);
	m_wndEffAreaUnit.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dArea);
	m_wndPlugAreaUnit.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dAreap);
	
	int nIndex;
	m_wndSWCmb.ResetContent();
	nIndex = m_wndSWCmb.AddString(_LSX(G=8));
	m_wndSWCmb.SetItemData(nIndex, 0);
	nIndex = m_wndSWCmb.AddString(_LSX(G=10));
	m_wndSWCmb.SetItemData(nIndex, 1);
	nIndex = m_wndSWCmb.AddString(_LSX(G=12));
	m_wndSWCmb.SetItemData(nIndex, 2);
	m_wndSWCmb.SetCurSel(0);
	
	Data2Dlg();

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CIsolatorLrbH30Dlg::SetIehpPropData(T_IEHP_D iehpD, T_IEHP_LRBH30 data, int nModel, int nDof)
{
	m_Data = iehpD;    
	m_Data.AllProp.PROP[nDof].LRBH30 = data;
	m_nMdlType = nModel;
	m_nDof = nDof;
}

bool CIsolatorLrbH30Dlg::SyncDialogData()
{
	if (!Dlg2Data()) return false;

	return true;
}

void CIsolatorLrbH30Dlg::Data2Dlg()
{
	int nD = m_nDof;
	m_wndSWCmb.SelectString(0, GetSwName(m_Data.AllProp.PROP[nD].LRBH30.nSW));
	m_wndHEdit.SetEditUnit(m_Data.AllProp.PROP[nD].LRBH30.dH);
	m_wndEffAreaEdit.SetEditUnit(m_Data.AllProp.PROP[nD].LRBH30.dArea);
	m_wndPlugAreaEdit.SetEditUnit(m_Data.AllProp.PROP[nD].LRBH30.dAreap);
	m_wndCbEdit.SetEditUnit(m_Data.AllProp.PROP[nD].LRBH30.dCb);

	UpdateData(FALSE);
}

BOOL CIsolatorLrbH30Dlg::Dlg2Data()
{
	if(!CheckData()) return FALSE;
	UpdateData(TRUE);
	int nD = m_nDof;
	m_Data.AllProp.PROP[nD].LRBH30.nSW   = m_wndSWCmb.GetItemData(m_wndSWCmb.GetCurSel());
	m_Data.AllProp.PROP[nD].LRBH30.dH    = m_wndHEdit.GetEditValue();
	m_Data.AllProp.PROP[nD].LRBH30.dArea = m_wndEffAreaEdit.GetEditValue();
	m_Data.AllProp.PROP[nD].LRBH30.dAreap = m_wndPlugAreaEdit.GetEditValue();
	m_Data.AllProp.PROP[nD].LRBH30.dCb = m_wndCbEdit.GetEditValue();
	return TRUE;
}

CString CIsolatorLrbH30Dlg::GetSwName(int nSw)
{
	CString strSw = _T("");
	switch(nSw)
	{
		case 0: strSw = _LSX(G=8);    break;
		case 1: strSw = _LSX(G=10);   break;
		case 2: strSw = _LSX(G=12);   break;
		default: ASSERT(0); break;
	}
	return strSw;

}
BOOL CIsolatorLrbH30Dlg::CheckErrorNameDataVar(LPCTSTR lpszData, LPCTSTR lpszName, LPCTSTR lpszVar, BOOL bMsg)
{
	if(bMsg) GSaveHistoryFormatNF(_LS(IDS_DB_IREG_DT_NAME_VAR_ERROR), lpszData, lpszName, lpszVar);
	return FALSE;
}

BOOL CIsolatorLrbH30Dlg::CheckData(BOOL bMsg/*=TRUE*/)
{
#define CheckError(x, bMsg) CheckErrorNameDataVar(_LS(IDS_DB_DT_IEHP), m_Data.strName, x, bMsg)
	UpdateData(TRUE);
	if(m_wndHEdit.GetEditValue() <= 0)    return CheckError(_LS(IDS_ERROR_IEHP_H_ZERO), bMsg);
	if(m_wndEffAreaEdit.GetEditValue() <= 0) return CheckError(_LS(IDS_ERROR_IEHP_EFFECTIVE_AREA_ZERO), bMsg);
	if(m_wndPlugAreaEdit.GetEditValue() <= 0) return CheckError(_LS(IDS_ERROR_IEHP_PLUG_AREA_ZERO), bMsg);

	double Ae = m_wndEffAreaEdit.GetEditValue();
	double Ap = m_wndPlugAreaEdit.GetEditValue();
	if (Ap / Ae > 0.1) return CheckError(_LS(IDS_ERROR_IEHP_AREA_RATIO_ZERO), bMsg);

	return TRUE;
#undef CheckError
}

void CIsolatorLrbH30Dlg::OnOK()
{
	// TODO: Add extra validation here
	if(!Dlg2Data()) return;
	CDialogMove::OnOK();
}
