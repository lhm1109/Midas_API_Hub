// IsolatorDmpRbrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "IsolatorDmpRbrDlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIsolatorDmpRbrDlg dialog


CIsolatorDmpRbrDlg::CIsolatorDmpRbrDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CIsolatorDmpRbrDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIsolatorDmpRbrDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CIsolatorDmpRbrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIsolatorDmpRbrDlg)
	DDX_Control(pDX, IDC_CMD_ISOLATOR_H_UNIT, m_wndHUnit);
	DDX_Control(pDX, IDC_CMD_ISOLATOR_AREA_UNIT, m_wndAreaUnit);
	DDX_Control(pDX, IDC_CMD_ISOLATOR_SW_CMB, m_wndSWCmb);	
	DDX_Control(pDX, IDC_CMD_ISOLATOR_H_EDIT, m_wndHEdit);
	DDX_Control(pDX, IDC_CMD_ISOLATOR_AREA_EDIT, m_wndAreaEdit);
	DDX_Control(pDX, IDC_CMD_ISOLATOR_GS_EDIT, m_wndGsEdit);
	DDX_Control(pDX, IDC_CMD_ISOLATOR_HS_EDIT, m_wndHsEdit);
	DDX_Control(pDX, IDC_CMD_ISOLATOR_US_EDIT, m_wndUsEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIsolatorDmpRbrDlg, CDialogMove)
	//{{AFX_MSG_MAP(CIsolatorDmpRbrDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIsolatorDmpRbrDlg message handlers

BOOL CIsolatorDmpRbrDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	m_wndHUnit.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dH);
	m_wndAreaUnit.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dArea);
	
	int nIndex;
	m_wndSWCmb.ResetContent();
	nIndex = m_wndSWCmb.AddString(_LSX(KL301));
	m_wndSWCmb.SetItemData(nIndex, 0);
	nIndex = m_wndSWCmb.AddString(_LSX(KL401));
	m_wndSWCmb.SetItemData(nIndex, 1);
	nIndex = m_wndSWCmb.AddString(_LSX(KL302));
	m_wndSWCmb.SetItemData(nIndex, 2);
	nIndex = m_wndSWCmb.AddString(_LSX(KL501));
	m_wndSWCmb.SetItemData(nIndex, 3);
	nIndex = m_wndSWCmb.AddString(_LSX(UHD-G6));
	m_wndSWCmb.SetItemData(nIndex, 4);
	nIndex = m_wndSWCmb.AddString(_LSX(HD-G8));
	m_wndSWCmb.SetItemData(nIndex, 5);
	nIndex = m_wndSWCmb.AddString(_LSX(TOYO));
	m_wndSWCmb.SetItemData(nIndex, 6);
	nIndex = m_wndSWCmb.AddString(_LSX(G=8));
	m_wndSWCmb.SetItemData(nIndex, 7);
	nIndex = m_wndSWCmb.AddString(_LSX(G=10));
	m_wndSWCmb.SetItemData(nIndex, 8);
	nIndex = m_wndSWCmb.AddString(_LSX(G=12));
	m_wndSWCmb.SetItemData(nIndex, 9);
	m_wndSWCmb.SetCurSel(0);
	
	Data2Dlg();

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CIsolatorDmpRbrDlg::SetIehpPropData(T_IEHP_D iehpD, T_IEHP_DMPRBR data, int nModel, int nDof)
{
	m_Data = iehpD;    
	m_Data.AllProp.PROP[nDof].DMPRBR = data;  
	m_nMdlType = nModel;
	m_nDof = nDof;
}

bool CIsolatorDmpRbrDlg::SyncDialogData()
{
	if (!Dlg2Data()) return false;

	return true;
}

void CIsolatorDmpRbrDlg::Data2Dlg()
{
	int nD = m_nDof;
	m_wndSWCmb.SelectString(0, GetSwName(m_Data.AllProp.PROP[nD].DMPRBR.nSW));  
	m_wndHEdit.SetEditUnit(m_Data.AllProp.PROP[nD].DMPRBR.dH);
	m_wndAreaEdit.SetEditUnit(m_Data.AllProp.PROP[nD].DMPRBR.dArea);
	m_wndGsEdit.SetEditUnit(m_Data.AllProp.PROP[nD].DMPRBR.dGs);  
	m_wndHsEdit.SetEditUnit(m_Data.AllProp.PROP[nD].DMPRBR.dHs);  
	m_wndUsEdit.SetEditUnit(m_Data.AllProp.PROP[nD].DMPRBR.dUs);    
	UpdateData(FALSE);
}

BOOL CIsolatorDmpRbrDlg::Dlg2Data()
{
	if(!CheckData()) return FALSE;
	UpdateData(TRUE);
	int nD = m_nDof;
	m_Data.AllProp.PROP[nD].DMPRBR.nSW   = m_wndSWCmb.GetItemData(m_wndSWCmb.GetCurSel());  
	m_Data.AllProp.PROP[nD].DMPRBR.dH    = m_wndHEdit.GetEditValue();
	m_Data.AllProp.PROP[nD].DMPRBR.dArea = m_wndAreaEdit.GetEditValue();
	m_Data.AllProp.PROP[nD].DMPRBR.dGs   = m_wndGsEdit.GetEditValue();    
	m_Data.AllProp.PROP[nD].DMPRBR.dHs   = m_wndHsEdit.GetEditValue();    
	m_Data.AllProp.PROP[nD].DMPRBR.dUs   = m_wndUsEdit.GetEditValue();    
	return TRUE;
}

CString CIsolatorDmpRbrDlg::GetSwName(int nSw)
{
	CString strSw = _T("");
	switch(nSw)
	{
		case 0: strSw = _LSX(KL301);  break;
		case 1: strSw = _LSX(KL401);  break;
		case 2: strSw = _LSX(KL302);  break;
		case 3: strSw = _LSX(KL501);  break;
		case 4: strSw = _LSX(UHD-G6); break;
		case 5: strSw = _LSX(HD-G8);  break;
		case 6: strSw = _LSX(TOYO);   break;
		case 7: strSw = _LSX(G=8);    break;
		case 8: strSw = _LSX(G=10);   break;
		case 9: strSw = _LSX(G=12);   break;    
		default: ASSERT(0); break;
	}
	return strSw;

}
BOOL CIsolatorDmpRbrDlg::CheckErrorNameDataVar(LPCTSTR lpszData, LPCTSTR lpszName, LPCTSTR lpszVar, BOOL bMsg)
{
	if(bMsg) GSaveHistoryFormatNF(_LS(IDS_DB_IREG_DT_NAME_VAR_ERROR), lpszData, lpszName, lpszVar);
	return FALSE;
}

BOOL CIsolatorDmpRbrDlg::CheckData(BOOL bMsg/*=TRUE*/)
{
#define CheckError(x, bMsg) CheckErrorNameDataVar(_LS(IDS_DB_DT_IEHP), m_Data.strName, x, bMsg)
	UpdateData(TRUE);
	if(m_wndHEdit.GetEditValue() < 0)    return CheckError(_T("H >= 0"), bMsg);
	if(m_wndAreaEdit.GetEditValue() < 0) return CheckError(_T("Area >= 0"), bMsg);  
	if(m_wndGsEdit.GetEditValue() < 0)   return CheckError(_T("Gs >= 0"), bMsg);
	if(m_wndHsEdit.GetEditValue() < 0)   return CheckError(_T("Hs >= 0"), bMsg);
	if(m_wndUsEdit.GetEditValue() < 0)   return CheckError(_T("Us >= 0"), bMsg);
	return TRUE;
#undef CheckError
}

void CIsolatorDmpRbrDlg::OnOK() 
{
	// TODO: Add extra validation here
	if(!Dlg2Data()) return;
	CDialogMove::OnOK();
}
