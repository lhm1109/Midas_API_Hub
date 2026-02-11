// MassNmasDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "MassNmasDlg.h"
#include "StructMassDlg.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMassNmasDlg dialog


CMassNmasDlg::CMassNmasDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CMassNmasDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMassNmasDlg)
	m_nOption = -1;
	//}}AFX_DATA_INIT

	m_aCtrlOkApply.Add(IDC_TM_EXECUTE);
	m_aCtrlOkApply.Add(IDC_TM_CLOSE);
}

void CMassNmasDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMassNmasDlg)
	DDX_Radio(pDX, IDC_TM_NMAS_OPT_ADD, m_nOption);
	//}}AFX_DATA_MAP
	UINT nID[6][2] = { 
		{IDC_TM_NMAS_RMSX, IDC_TM_NMAS_RMSX_UNIT},
		{IDC_TM_NMAS_RMSY, IDC_TM_NMAS_RMSY_UNIT},
		{IDC_TM_NMAS_RMSZ, IDC_TM_NMAS_RMSZ_UNIT},
		{IDC_TM_NMAS_RIMX, IDC_TM_NMAS_RIMX_UNIT},
		{IDC_TM_NMAS_RIMY, IDC_TM_NMAS_RIMY_UNIT},
		{IDC_TM_NMAS_RIMZ, IDC_TM_NMAS_RIMZ_UNIT},
	};
	for (int i = 0; i < 6; i++)
	{
	  DDX_Control(pDX, nID[i][0], m_Value[i]);
	  DDX_Control(pDX, nID[i][1], m_Unit[i]);
	}
	UINT aAngleID[] = {IDC_TM_ANGLE_X_CMB, IDC_TM_ANGLE_Y_CMB, IDC_TM_ANGLE_Z_CMB};
	for(int i=0; i < 3; i++)
	{
		DDX_Control(pDX, aAngleID[i], m_AngleCobx[i]);    
	}  

	DDX_Control(pDX, IDC_WG_TREEMENU_STATIC7, m_wndPicture);
	DDX_Control(pDX, IDC_TM_PICTURE, m_wndPicture2);
}

BEGIN_MESSAGE_MAP(CMassNmasDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CMassNmasDlg)
	//ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	//ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_TM_NMAS_OPT_ADD, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_NMAS_OPT_DELETE, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_NMAS_OPT_REPLACE, OnChangeOption)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMassNmasDlg message handlers

BOOL CMassNmasDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CMenuBarChildDlg::OnInitDialog();	
		
	for (int i = 0; i < 6; i++)
	{
		m_Unit[i].SetUnitType(CUnitCtrl::m_NMAS_UNIT.Mass[i]);
		m_Value[i].SetUnitType(CUnitCtrl::m_NMAS_UNIT.Mass[i]);
	}
	m_nOption = 0;

	for(int i=0; i < 3; i++)
	{
		m_AngleCobx[i].ResetContent();
		m_AngleCobx[i].AddString(_T("0"));
		m_AngleCobx[i].AddString(_LS(IDS_WG_TREEMENU_45));
		m_AngleCobx[i].AddString(_LS(IDS_WG_TREEMENU_90));
		m_AngleCobx[i].AddString(_LS(IDS_WG_TREEMENU_180));
		m_AngleCobx[i].SetCurSel(0);    
	}
	
	CDlgUtil::GetCtrlIDByIncRect(this, m_aCtrlRotAngle, IDC_TM_NLLD_STATIC, TRUE);

	AlignCtrl();
	UpdateData(FALSE);		
	
	m_wndPicture.SetImage(_T("SVG\\illustration\\Dialog\\tm_nmas 1.svg"));
	m_wndPicture2.SetImage(_T("SVG\\illustration\\Dialog\\tm_skew1.svg"));
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMassNmasDlg::AlignCtrl()
{
	BOOL bShowRotAngle = FALSE;
// Version 660, 670에서는 Solver의 반영 미비로 회전질량 기능을 막음(고영현 차장님 지시): 2004.10.22 by KYM
// EditData::CheckNmas도 풀 것 
//#if defined(_CIVIL)
//  bShowRotAngle = TRUE;  
//#endif
	CDlgUtil::CtrlShowHide(this, m_aCtrlRotAngle, bShowRotAngle);
	if(!bShowRotAngle)
	{
		CRect rRef;
		CRect rToMove;
		int nDistY;	
		GetDlgItem(IDC_TM_NMAS_FRM)->GetWindowRect(rRef);

		GetDlgItem(m_aCtrlOkApply[0])->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlOkApply, nDistY);
	}
}

void CMassNmasDlg::Execute() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CArray<T_NMAS_K, T_NMAS_K> aSelKey;
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedNodeKeyList(aSelKey);
	if (aSelKey.GetSize() == 0) 
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_node_));
		return;
	}
	T_NMAS_D data;
	data.Initialize();

	for (int i = 0; i < 6; i++)
	{
		data.Mass[i] = m_Value[i].GetEditValue();
	}

	for(int i=0; i < 3; i++)
	{
		if (!CFormulaEdit::GetEditValue(&m_AngleCobx[i], data.dAngle[i]))
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_angle_value_));
			return;
		}
	}

	BOOL bSuccess=FALSE;
	if (m_nOption == 0 || m_nOption == 1) // Replace
		bSuccess = m_pDoc->m_pDataCtrl->AddNmas(aSelKey, data, (m_nOption==0)?FALSE:TRUE);	
	else if (m_nOption == 2)  // delete
		bSuccess = m_pDoc->m_pDataCtrl->DelNmas(aSelKey);	

	if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CMassNmasDlg::OnChangeOption() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_nOption == 2) // delete
		EnableValueEdit(FALSE); // disable all value edit
	else EnableValueEdit(TRUE);	
	
}

void CMassNmasDlg::EnableValueEdit(BOOL bEnable)
{
	for (int i = 0; i < 6; i++)
	{
		m_Value[i].EnableWindow(bEnable);
	}
}

BOOL CMassNmasDlg::ExternalInit(UINT key)
{
	if(key == 0) return FALSE;

	T_NMAS_D data;
	m_pDoc->m_pAttrCtrl->GetNmas(key, data);

	for (int i = 0; i < 6; i++)
	{
		m_Value[i].SetEditUnit(data.Mass[i]);
	}
	CString str;
	for(int i=0; i < 3; i++)
	{
		str.Format(_T("%g"), data.dAngle[i]);
		m_AngleCobx[i].SelectString(-1, str);
	}      

	m_nOption = 0;
	UpdateData(FALSE);
	return TRUE;
}
