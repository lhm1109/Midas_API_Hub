// IsolatorDmpRbrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "IsolatorDmpH16RbrDlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIsolatorDmpH16RbrDlg dialog


CIsolatorDmpH16RbrDlg::CIsolatorDmpH16RbrDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CIsolatorDmpH16RbrDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIsolatorDmpH16RbrDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CIsolatorDmpH16RbrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIsolatorDmpH16RbrDlg)
	DDX_Radio(pDX, IDC_CMD_ISOLATOR_HDR_RDO, m_wndHdrRdo);
	DDX_Control(pDX, IDC_CMD_ISOLATOR_H_UNIT, m_wndHUnit);
	DDX_Control(pDX, IDC_CMD_ISOLATOR_AREA_UNIT, m_wndAreaUnit);
	DDX_Control(pDX, IDC_CMD_ISOLATOR_SW_CMB, m_wndSWCmb);	
	DDX_Control(pDX, IDC_CMD_ISOLATOR_H_EDIT, m_wndHEdit);
	DDX_Control(pDX, IDC_CMD_ISOLATOR_AREA_EDIT, m_wndAreaEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIsolatorDmpH16RbrDlg, CDialogMove)
	//{{AFX_MSG_MAP(CIsolatorDmpH16RbrDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIsolatorDmpH16RbrDlg message handlers

BOOL CIsolatorDmpH16RbrDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();
	
	m_wndHUnit.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dH);
	m_wndAreaUnit.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dArea);
	
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

void CIsolatorDmpH16RbrDlg::SetIehpPropData(T_IEHP_D iehpD, T_IEHP_DMPRH16 data, int nModel, int nDof)
{
	m_Data = iehpD;    
	m_Data.AllProp.PROP[nDof].DMPRH16 = data;
	m_nMdlType = nModel;
	m_nDof = nDof;
}

bool CIsolatorDmpH16RbrDlg::SyncDialogData()
{
	if (!Dlg2Data()) return false;

	return true;
}

void CIsolatorDmpH16RbrDlg::Data2Dlg()
{
	int nD = m_nDof;
	m_wndSWCmb.SelectString(0, GetSwName(m_Data.AllProp.PROP[nD].DMPRH16.nSW));
	m_wndHEdit.SetEditUnit(m_Data.AllProp.PROP[nD].DMPRH16.dH);
	m_wndAreaEdit.SetEditUnit(m_Data.AllProp.PROP[nD].DMPRH16.dArea);
	m_wndHdrRdo = m_Data.AllProp.PROP[nD].DMPRH16.nHDR;
	UpdateData(FALSE);
}

BOOL CIsolatorDmpH16RbrDlg::Dlg2Data()
{
	if(!CheckData()) return FALSE;
	UpdateData(TRUE);
	int nD = m_nDof;
	m_Data.AllProp.PROP[nD].DMPRH16.nSW   = m_wndSWCmb.GetItemData(m_wndSWCmb.GetCurSel());
	m_Data.AllProp.PROP[nD].DMPRH16.dH    = m_wndHEdit.GetEditValue();
	m_Data.AllProp.PROP[nD].DMPRH16.dArea = m_wndAreaEdit.GetEditValue();
	m_Data.AllProp.PROP[nD].DMPRH16.nHDR  = m_wndHdrRdo;
	return TRUE;
}

CString CIsolatorDmpH16RbrDlg::GetSwName(int nSw)
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
BOOL CIsolatorDmpH16RbrDlg::CheckErrorNameDataVar(LPCTSTR lpszData, LPCTSTR lpszName, LPCTSTR lpszVar, BOOL bMsg)
{
	if(bMsg) GSaveHistoryFormatNF(_LS(IDS_DB_IREG_DT_NAME_VAR_ERROR), lpszData, lpszName, lpszVar);
	return FALSE;
}

BOOL CIsolatorDmpH16RbrDlg::CheckData(BOOL bMsg/*=TRUE*/)
{
#define CheckError(x, bMsg) CheckErrorNameDataVar(_LS(IDS_DB_DT_IEHP), m_Data.strName, x, bMsg)
	UpdateData(TRUE);
	if(m_wndHEdit.GetEditValue() <= 0)    return CheckError(_T("H > 0"), bMsg);
	if(m_wndAreaEdit.GetEditValue() <= 0) return CheckError(_T("Area > 0"), bMsg);
	return TRUE;
#undef CheckError
}

void CIsolatorDmpH16RbrDlg::OnOK()
{
	// TODO: Add extra validation here
	if(!Dlg2Data()) return;
	CDialogMove::OnOK();
}
