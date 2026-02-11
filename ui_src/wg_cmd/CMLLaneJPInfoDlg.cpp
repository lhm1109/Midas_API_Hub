// CMMvctJPInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMLLaneJPInfoDlg.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_StagCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"

#include "CMLLaneJPDlg.h"
#include "..\wg_base\I_GENStateDefine.h"

using namespace mit::lib;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMLLaneJPInfoDlg dialog

#define CDialog CCMDlgBase

CCMLLaneJPInfoDlg::CCMLLaneJPInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCMLLaneJPInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMLLaneJPInfoDlg)
	m_nSelectBy = 0;
	m_bP1Load = FALSE;
	m_bP2Load = FALSE;
	m_bCrowdLoad = FALSE;
	m_bTLoadCheck = FALSE;
	m_bExtraCoeff = FALSE;
	m_bImpactCoeff = FALSE;
	m_strName = _T("");
	m_bP1Load2 = FALSE;
	//}}AFX_DATA_INIT
	m_pLLAND = new T_LLANjp_BASE_OLD;
	m_pLLAND->Initialize();
}

CCMLLaneJPInfoDlg::~CCMLLaneJPInfoDlg()
{
	delete m_pLLAND;
}

void CCMLLaneJPInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMLLaneJPInfoDlg)
	DDX_Control(pDX, IDC_CMD_P1LOAD_UNIT2, m_wndP1Unit2);
	DDX_Control(pDX, IDC_CMD_P1LOAD_EDIT2, m_wndP1Edit2);
	DDX_Control(pDX, IDC_CMD_MODIFY_BUTTON, m_wndModifyBtn);
	DDX_Control(pDX, IDC_CMD_LENGTH_EDIT, m_wndLength);
	DDX_Control(pDX, IDC_CMD_P1LOAD_EDIT, m_wndP1Edit);
	DDX_Control(pDX, IDC_CMD_P2LOAD_EDIT, m_wndP2Edit);
	DDX_Control(pDX, IDC_CMD_CROWDLOAD_EDIT, m_wndCrowdLoad);
	DDX_Control(pDX, IDC_CMD_TLOAD_EDIT, m_wndTLoadEdit);
	DDX_Control(pDX, IDC_CMD_IMPACTCOEFF_EDIT, m_wndImpactCoeff);
	DDX_Control(pDX, IDC_CMD_EXTRACOEFF_EDIT, m_wndExtraCoeff);
	DDX_Control(pDX, IDC_CMD_P1LOAD_UNIT, m_wndP1Unit);
	DDX_Control(pDX, IDC_CMD_P2LOAD_UNIT, m_wndP2Unit);
	DDX_Control(pDX, IDC_CMD_CROWDLOAD_UNIT, m_CrowdUnit);
	DDX_Control(pDX, IDC_CMD_TLOAD_UNIT, m_wndTLoadUnit);
	DDX_Control(pDX, IDC_CMD_LENGTH_UNIT, m_wndLengthUnit);
	DDX_Control(pDX, IDC_CMD_SELBY_EDIT1, m_wndSelBy1);
	DDX_Control(pDX, IDC_CMD_SELBY_EDIT2, m_wndSelBy2);
	DDX_Control(pDX, IDC_CMD_SELBY_UNIT1, m_wndSelByUnit1);
	DDX_Control(pDX, IDC_CMD_SELBY_UNIT2, m_wndSelByUnit2);
	DDX_Radio(pDX, IDC_2POINTS_RADIO, m_nSelectBy);
	DDX_Check(pDX, IDC_CMD_P1LOAD_CHECK, m_bP1Load);
	DDX_Check(pDX, IDC_CMD_P2LOAD_CHECK, m_bP2Load);
	DDX_Check(pDX, IDC_CMD_CROWDLOAD_CHECK, m_bCrowdLoad);
	DDX_Check(pDX, IDC_CMD_TLOAD_CHECK, m_bTLoadCheck);
	DDX_Check(pDX, IDC_CMD_EXTRACOEFF_CHECK, m_bExtraCoeff);
	DDX_Check(pDX, IDC_CMD_IMPACTCOEFF_CHECK, m_bImpactCoeff);
	DDX_Control(pDX, IDC_CMD_ELEM_LIST, m_wndElemList);
	DDX_Text(pDX, IDC_CMD_NAME_EDIT, m_strName);
	DDX_Check(pDX, IDC_CMD_P1LOAD_CHECK2, m_bP1Load2);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMLLaneJPInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CCMLLaneJPInfoDlg)
	ON_BN_CLICKED(IDC_CMD_CROWDLOAD_CHECK, OnCmdCrowdloadCheck)
	ON_BN_CLICKED(IDC_CMD_EXTRACOEFF_CHECK, OnCmdExtracoeffCheck)
	ON_BN_CLICKED(IDC_CMD_IMPACTCOEFF_CHECK, OnCmdImpactcoeffCheck)
	ON_BN_CLICKED(IDC_CMD_P1LOAD_CHECK, OnCmdP1loadCheck)
	ON_BN_CLICKED(IDC_CMD_P2LOAD_CHECK, OnCmdP2loadCheck)
	ON_BN_CLICKED(IDC_CMD_TLOAD_CHECK, OnCmdTloadCheck)
	ON_BN_CLICKED(IDC_2POINTS_RADIO, OnSelectByRadio)
	ON_BN_CLICKED(IDC_CMD_ADD_BUTTON, OnCmdAddButton)
	ON_BN_CLICKED(IDC_CMD_DELETE_BUTTON, OnCmdDeleteButton)
	ON_BN_CLICKED(IDC_CMD_MODIFY_BUTTON, OnCmdModifyButton)
	ON_BN_CLICKED(IDC_CMD_P1LOAD_CHECK2, OnCmdP1loadCheck2)
	ON_BN_CLICKED(IDC_PICKING_RADIO, OnSelectByRadio)
	ON_BN_CLICKED(IDC_NUMBER_RADIO, OnSelectByRadio)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_MOUSEEDITENTER        , OnEnterKeyPressed )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMLLaneJPInfoDlg message handlers

BOOL CCMLLaneJPInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SelectCtrlMan();
	//CtrlMan();
	MakeListHeader();
	ButtonCtrl();

	m_wndLength.SetUnitType(CUnitCtrl::m_LENGTH_UNIT);
	m_wndLengthUnit.SetUnitType(CUnitCtrl::m_LENGTH_UNIT);
 
	m_wndP1Edit     .SetUnitType(CUnitCtrl::m_LLANjp_UNIT_OLD.dP1LoadMomentAdj);
	m_wndP1Unit     .SetUnitType(CUnitCtrl::m_LLANjp_UNIT_OLD.dP1LoadMomentAdj);
	m_wndP1Edit2     .SetUnitType(CUnitCtrl::m_LLANjp_UNIT_OLD.dP1LoadShearAdj);
	m_wndP1Unit2     .SetUnitType(CUnitCtrl::m_LLANjp_UNIT_OLD.dP1LoadShearAdj);
	m_wndP2Edit     .SetUnitType(CUnitCtrl::m_LLANjp_UNIT_OLD.dP2LoadAdj);
	m_wndP2Unit     .SetUnitType(CUnitCtrl::m_LLANjp_UNIT_OLD.dP2LoadAdj);
	m_wndCrowdLoad  .SetUnitType(CUnitCtrl::m_LLANjp_UNIT_OLD.dCrowdLoadAdj);
	m_CrowdUnit     .SetUnitType(CUnitCtrl::m_LLANjp_UNIT_OLD.dCrowdLoadAdj);
	m_wndTLoadEdit  .SetUnitType(CUnitCtrl::m_LLANjp_UNIT_OLD.dTLoadAdj);
	m_wndTLoadUnit  .SetUnitType(CUnitCtrl::m_LLANjp_UNIT_OLD.dTLoadAdj);
	m_wndImpactCoeff.SetUnitType(D_UNITSYS_NONE);
	m_wndExtraCoeff .SetUnitType(D_UNITSYS_NONE);

	m_wndSelByUnit1 .SetUnitType(CUnitCtrl::m_LENGTH_UNIT);
	m_wndSelByUnit2 .SetUnitType(CUnitCtrl::m_LENGTH_UNIT);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMLLaneJPInfoDlg::CtrlMan()
{
	m_wndP1Edit     .EnableWindow(this->m_bP1Load     );
	m_wndP1Unit     .EnableWindow(this->m_bP1Load     );  
	m_wndP1Edit2    .EnableWindow(this->m_bP1Load2    );
	m_wndP1Unit2    .EnableWindow(this->m_bP1Load2    );
	m_wndP2Edit     .EnableWindow(this->m_bP2Load     );
	m_wndP2Unit     .EnableWindow(this->m_bP2Load     );  
	m_wndCrowdLoad  .EnableWindow(this->m_bCrowdLoad  );
	m_CrowdUnit     .EnableWindow(this->m_bCrowdLoad  );
	m_wndTLoadEdit  .EnableWindow(this->m_bTLoadCheck );
	m_wndTLoadUnit  .EnableWindow(this->m_bTLoadCheck );
	m_wndImpactCoeff.EnableWindow(this->m_bImpactCoeff);
	m_wndExtraCoeff .EnableWindow(this->m_bExtraCoeff );
}

void CCMLLaneJPInfoDlg::MakeListHeader()
{
	CStringArray HTitles;
	CArray<float, float> HRatio;
	HTitles.Add (_LS(IDS_CCMLLANJPINFODLG_0829_MSG1)
);         HRatio.Add(0.3f);
	HTitles.Add (_LS(IDS_CCMLLANJPINFODLG_0829_MSG2)
); HRatio.Add(0.7f);
	CDlgUtil::_SetListCtrlHeader(&m_wndElemList,HTitles,&HRatio,NULL);

}

void CCMLLaneJPInfoDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	*m_ppMySelf = NULL;
	*m_pnOP     = 3; // Else
	m_pParentDlg->Invalidate(FALSE);
	CDialog::PostNcDestroy();
}

void CCMLLaneJPInfoDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	DestroyWindow();
	//	CDialog::OnCancel();  호출 하면 안됨.. Modeless Dialog에서는 주의할것..
	//                        PostNCDestroy 호출 안됨 
}

void CCMLLaneJPInfoDlg::OnOK() 
{
	// TODO: Add extra validation here
	if(!Dlg2Data(m_pLLAND)) return;
	if(m_pParentDlg->SetByInfoDlg(m_pLLAND))
	  DestroyWindow();
	//	CDialog::OnOK();      호출 하면 안됨.. Modeless Dialog에서는 주의할것..
	//                        PostNCDestroy 호출 안됨     
}

void CCMLLaneJPInfoDlg::OnCmdApply() 
{
	if(!Dlg2Data(m_pLLAND)) return;
	m_pParentDlg->SetByInfoDlg(m_pLLAND);
	if(*m_pnOP == 0)
	{
		m_wndElemList.DeleteAllItems();
	}
}


void CCMLLaneJPInfoDlg::OnCmdP1loadCheck2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CtrlMan();
}


void CCMLLaneJPInfoDlg::OnCmdCrowdloadCheck() 
{
	UpdateData(TRUE);
	CtrlMan();
}

void CCMLLaneJPInfoDlg::OnCmdExtracoeffCheck() 
{
	UpdateData(TRUE);
	CtrlMan();
}

void CCMLLaneJPInfoDlg::OnCmdImpactcoeffCheck() 
{
	UpdateData(TRUE);
	CtrlMan();
}

void CCMLLaneJPInfoDlg::OnCmdP1loadCheck() 
{
	UpdateData(TRUE);
	CtrlMan();
}

void CCMLLaneJPInfoDlg::OnCmdP2loadCheck() 
{
	UpdateData(TRUE);
	CtrlMan();	
}

void CCMLLaneJPInfoDlg::OnCmdTloadCheck() 
{
	UpdateData(TRUE);
	CtrlMan();	
}

void CCMLLaneJPInfoDlg::SelectCtrlMan()
{
	//I_GENModelBase::SetModelingModeST(IG_STATE_CANCEL);
	switch(this->m_nSelectBy)
	{
	case 0:  //Select By 2Points
		m_wndSelBy1.SetAttUcsPos();
		m_wndSelBy2.SetAttUcsPos();
		m_wndSelBy1.UseDefaultColor(FALSE);
		
		m_wndSelBy1.SetModeToUse(MOUSEEDIT_USE_SETP2_FIRST);
		m_wndSelBy2.SetModeToUse(MOUSEEDIT_USE_SETP2_SECOND);
		
		m_wndSelBy1.ShowWindow(SW_SHOW);
		m_wndSelBy2.ShowWindow(SW_SHOW);

		m_wndSelBy1.SetNextLink(&m_wndSelBy2);
		m_wndSelBy2.SetNextLink(&m_wndSelBy1);

		m_wndSelBy1.SetLButtonDownNotifyWindow(NULL);
		m_wndSelBy2.SetLButtonDownNotifyWindow(this);
		m_wndSelBy1.SetEnterNotifyWindow(NULL);
		m_wndSelBy2.SetEnterNotifyWindow(this);
		
		//m_wndSelBy1.Use
		m_wndSelByUnit1.ShowWindow(SW_SHOW);
		m_wndSelByUnit2.ShowWindow(SW_SHOW);

		//m_wndModifyBtn.EnableWindow(FALSE);
		
		break;
	case 1:  //Select By Picking
		m_wndSelBy1.SetAttElemList();
		m_wndSelBy1.SetModeToUse(MOUSEEDIT_USE_GET_ELEM_LIST);
		
		m_wndSelBy1.SetNextLink(NULL);
		m_wndSelBy1.SetMaxElemKeyNum(1);
		m_wndSelBy2.SetNextLink(NULL);
		m_wndSelBy1.UseDefaultColor(FALSE);
		m_wndSelBy1.SetWindowText(_T(""));
		m_wndSelBy2.SetAttNothing();
	
		m_wndSelBy2.ShowWindow(SW_HIDE);

		m_wndSelBy1.SetLButtonDownNotifyWindow(this);
		m_wndSelBy2.SetLButtonDownNotifyWindow(NULL);
		m_wndSelBy1.SetEnterNotifyWindow(this);
		m_wndSelBy2.SetEnterNotifyWindow(NULL);

		m_wndSelByUnit1.ShowWindow(SW_HIDE);
		m_wndSelByUnit2.ShowWindow(SW_HIDE);

		//m_wndModifyBtn.EnableWindow(TRUE);
		m_wndSelBy1.SetWindowText(_T(""));
		break;
	case 2:  //Select By Number
		
		//m_wndSelBy2.SetWindowText(_T(""));
		m_wndSelBy1.SetAttNothing();
		m_wndSelBy1.SetModeToUse(MOUSEEDIT_USE_NOTHING);
		m_wndSelBy1.UseDefaultColor(TRUE);

		m_wndSelBy1.SetNextLink(NULL);
		m_wndSelBy2.SetAttNothing();
		m_wndSelBy2.ShowWindow(SW_HIDE);

		m_wndSelBy1.SetLButtonDownNotifyWindow(this);
		m_wndSelBy2.SetLButtonDownNotifyWindow(NULL);
		m_wndSelBy1.SetEnterNotifyWindow(this);
		m_wndSelBy2.SetEnterNotifyWindow(NULL);

		m_wndSelByUnit1.ShowWindow(SW_HIDE);
		m_wndSelByUnit2.ShowWindow(SW_HIDE);
		//m_wndModifyBtn.EnableWindow(TRUE);
		m_wndSelBy1.SetWindowText(_T(""));
		break;
	}
	m_wndSelBy1.SetFocus();
}

void CCMLLaneJPInfoDlg::OnSelectByRadio() 
{
	UpdateData(TRUE);
	
	SelectCtrlMan();
}

LRESULT CCMLLaneJPInfoDlg::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
{
	switch(this->m_nSelectBy)
	{
	case 0:  // 2Point
		if((HWND)lParam == this->m_wndSelBy2.GetSafeHwnd())
		{
			this->AddOp(0);
		}
		break;
	case 1:  // Picking
		if((HWND)lParam == this->m_wndSelBy1.GetSafeHwnd())
		{
			this->AddOp(0);
		}
		break; 
	case 2:  // Number   
		break;
	default:
		break;
	}

	return 0L;
}

LRESULT CCMLLaneJPInfoDlg::OnEnterKeyPressed (WPARAM wParam, LPARAM lParam)
{
	switch(this->m_nSelectBy)
	{
	case 0:   // 2Point
		if((HWND)lParam == this->m_wndSelBy2.GetSafeHwnd())
		{
			this->AddOp(0);
		}
		break;
	case 1:   // Picking
	case 2:   // Number   
		if((HWND)lParam == this->m_wndSelBy1.GetSafeHwnd())
		{
			this->AddOp(0);
		}
		break;
	default:
		break;
	}

	return 0L;
}

void CCMLLaneJPInfoDlg::SelectElem()
{
	CArray<T_ELEM_K, T_ELEM_K>arSelElem;
	
	int nCount = m_wndElemList.GetItemCount();
	unsigned int nTemp;
	for(int i = 0; i < nCount; i++)
	{
		nTemp = (unsigned int)m_wndElemList.GetItemData(i);
		arSelElem.Add(nTemp);
	}
	//if(arSelElem.GetSize())
	CDBDoc::GetDocPoint()->m_pViewCtrl->SelectElem(NULL, arSelElem, FALSE, TRUE);
}


BOOL CCMLLaneJPInfoDlg::AddOp(int nAddOrInsert)
{
	switch(this->m_nSelectBy)
	{
	case 0:  // 2Point
		{
			double Wx1,Wy1,Wz1,Ux1,Uy1,Uz1;
			double Wx2,Wy2,Wz2,Ux2,Uy2,Uz2;
			CArray<T_ELEM_K, T_ELEM_K> aElemKey;
			if(!m_wndSelBy1.GetCoordData(&Ux1,&Uy1,&Uz1) || 
				 !m_wndSelBy2.GetCoordData(&Ux2,&Uy2,&Uz2)) 
			{
				return FALSE;
			}

			I_GENModelBase::GetCurMySelfST()->GetWcsPos(Ux1,Uy1,Uz1,Wx1,Wy1,Wz1);
			I_GENModelBase::GetCurMySelfST()->GetWcsPos(Ux2,Uy2,Uz2,Wx2,Wy2,Wz2);
			CDBDoc::GetDocPoint()->m_pDataCtrl->GetFrameElemOnLine(Wx1,Wy1,Wz1,Wx2,Wy2,Wz2,aElemKey, TRUE);
			AddElemToList(aElemKey,nAddOrInsert);
		}
		break;
	case 1:  // Picking
	case 2:  // Number   
		{
			CArray<unsigned int, unsigned int> EList;
			CString strEList;
			m_wndSelBy1.GetWindowText(strEList);
			CStrParser::GetNodeList(strEList,EList);
			AddElemToList(EList,nAddOrInsert);
		}
		break; 
	default:
		break;
	}
	SelectElem();
	return TRUE;
}

BOOL CCMLLaneJPInfoDlg::InsertOp()
{
	return TRUE;
}

BOOL CCMLLaneJPInfoDlg::ModifyOp()
{
	CArray<int,int>      SelIndex;
	CArray<DWORD, DWORD> SelDatas;
	CDlgUtil::GetSelectedListItemData(&m_wndElemList, SelDatas);
	CDlgUtil::GetSelectedListItemIndex(&m_wndElemList, SelIndex);

	int nSel = SelDatas.GetSize();
	if(nSel != 1)
	{
		return FALSE;
	}
	
	switch(this->m_nSelectBy)
	{
	case 0:  // 2Point
		//m_pDoc->m_pDataCtrl->GetFrameElemOnLine(WX1, WY1, WZ1, WX2, WY2, WZ2, aElemKey);
		break;
	case 1:  // Picking
	case 2:  // Number   
		{
			CArray<unsigned int, unsigned int> EList;
			CString strEList;
			m_wndSelBy1.GetWindowText(strEList);
			CStrParser::GetNodeList(strEList,EList);
			if(EList.GetSize() != 1) return FALSE;
			if(!IsValidElem(EList))return FALSE;
			
			CStringArray Contents;
			CString StrTemp;
			StrTemp.Format(_LS(IDS_CCMLLANJPINFODLG_0829_MSG3)
,SelIndex[0]+1); Contents.Add(StrTemp);
			StrTemp.Format(_LS(IDS_CCMLLANJPINFODLG_0829_MSG3)
,EList[0])     ; Contents.Add(StrTemp);

			CDlgUtil::SetListItem(&m_wndElemList,SelIndex[0],Contents,(DWORD)EList[0]);
		}
		break;
	default:
		break;
	}
	SelectElem();
	return TRUE;
}

BOOL CCMLLaneJPInfoDlg::DelOp()
{
	CArray<DWORD, DWORD> SelDatas ;
	CArray<DWORD, DWORD> ItemDatas;
	CMap<DWORD, DWORD, DWORD, DWORD> DataMap;
	//CArray<int,int> arRet;
	//CDlgUtil::GetSelectedListItemIndex(&m_wndElemList,arRet);
	CDlgUtil::GetSelectedListItemData(&m_wndElemList, SelDatas);

	int nD = SelDatas.GetSize();
	for(int i = 0; i < nD; i++)
	{
		DataMap.SetAt(SelDatas[i],SelDatas[i]);
	}

	int nItems = m_wndElemList.GetItemCount();
	for(int i = 0; i < nItems; i++)
	{
		ItemDatas.Add(m_wndElemList.GetItemData(i));
	}

	m_wndElemList.DeleteAllItems();

	DWORD Dummy;
	CStringArray Contents;
	CString StrTemp;
	int Index = 0;

	for(int i = 0; i < nItems; i++)
	{
		if(!DataMap.Lookup(ItemDatas[i],Dummy))
		{
			Contents.RemoveAll();
			
			StrTemp.Format(_LS(IDS_CCMLLANJPINFODLG_0829_MSG3), Index+1); Contents.Add(StrTemp);
			StrTemp.Format(_LS(IDS_CCMLLANJPINFODLG_0829_MSG3), (int)ItemDatas[i]) ; Contents.Add(StrTemp);

			CDlgUtil::SetListItem(&m_wndElemList,Index,Contents,ItemDatas[i]);
			Index++;
		}
	}
	SelectElem();
	return TRUE;
}

/*
else if(nType==1) //Select by Picking
	{
			int nElemKey;
			CFormulaEdit::GetEditValue(&m_wndPickingEdit, nElemKey);
			if (nElemKey <= 0)
			{
				CString msg;
				msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Element_No___d_is_not_exi), nElemKey);
				AfxMessageBox(msg);
				return;
			}
			if (Grid_IsExistKey(nElemKey))
			{
				CString msg;
				msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Element_No__d__is_already), nElemKey);
				AfxMessageBox(msg);
				return;
			}
			T_ELEM_D Data;
			m_pDoc->m_pAttrCtrl->GetElem(nElemKey, Data);
			if (!m_pDoc->m_pAttrCtrl->IsBeam(Data.eltyp))
			{
				CString msg;
				msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Element_No___d_is_not_a_b), nElemKey);
				AfxMessageBox(msg);
				return;
			}
			item.KeyElem = (unsigned int)nElemKey;
			Grid_InsertItem(nPos, item);
	}
*/



BOOL CCMLLaneJPInfoDlg::IsValidElem(CArray<unsigned int, unsigned int> & aElemKey)
{
	int nElem = aElemKey.GetSize();
	//////////////////////////////////////////////////////////////////////
	// 중복 Test 
	CMap<unsigned int, unsigned int, unsigned int, unsigned int> ItemMap;
	int nCount = m_wndElemList.GetItemCount();
	unsigned int nTemp;
	for(int i = 0; i < nCount; i++)
	{
		nTemp = (unsigned int)m_wndElemList.GetItemData(i);
		ItemMap.SetAt(nTemp,nTemp);
	}
	
	T_ELEM_D Data;

	for(int i = 0 ; i < nElem ; i++)
	{
		if(!CDBDoc::GetDocPoint()->m_pAttrCtrl->GetElem(aElemKey[i], Data))
		{
			CString msg;
		  msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Element_No___d_is_not_exi), aElemKey[i]);
			AfxMessageBox(msg);
			return FALSE;
		}
		
		if(!CDBDoc::GetDocPoint()->m_pAttrCtrl->IsBeam(Data.eltyp))
		{
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Element_No___d_is_not_a_b), aElemKey[i]);
			AfxMessageBox(msg);
			return FALSE; 
		}

		if(ItemMap.Lookup(aElemKey[i],nTemp))
		{
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Element_No___d_is_already), aElemKey[i]);
			AfxMessageBox(msg);
			return FALSE;
		}
	}

	return TRUE;
}
// nAddOrInsert (0) Add (1) Insert
BOOL CCMLLaneJPInfoDlg::AddElemToList(CArray<unsigned int, unsigned int> & aElemKey,int nAddOrInsert)
{
	if(!IsValidElem(aElemKey)) return FALSE;

	int nElem  = aElemKey.GetSize();
	int nCount = m_wndElemList.GetItemCount();
	
	CArray<int,int> SelIndex;
	CDlgUtil::GetSelectedListItemIndex(&m_wndElemList, SelIndex);

	if(nAddOrInsert == 0 || SelIndex.GetSize() == 0)
	{
		DWORD ItemData;
		CStringArray Contents;
		CString StrTemp;

		for(int  i  = nCount ; i < nCount + nElem ; i++)
		{
			Contents.RemoveAll();
			StrTemp.Format(_LS(IDS_CCMLLANJPINFODLG_0829_MSG3)
,i+1); Contents.Add(StrTemp);
			StrTemp.Format(_LS(IDS_CCMLLANJPINFODLG_0829_MSG3)
,aElemKey[i-nCount]) ; Contents.Add(StrTemp);
			ItemData = (DWORD)aElemKey[i-nCount];
			CDlgUtil::SetListItem(&m_wndElemList,i,Contents,ItemData);
		}
	}
	else
	{
		DWORD ItemData;
		CStringArray Contents;
		CString StrTemp;
		
		CArray<T_ELEM_K, T_ELEM_K&> EBuffer;
		//CArray<int,int> SelIndex;
		T_ELEM_K ElemK;
		int nCount = m_wndElemList.GetItemCount();
		int nItems = aElemKey.GetSize();
		
		if(SelIndex.GetSize() == 0) return FALSE;
		
		for(int i = SelIndex[0]; i < nCount ; i++)
		{
			ElemK = (unsigned int)m_wndElemList.GetItemData(i);
			EBuffer.Add(ElemK);
		}
		
		int nIStart = SelIndex[0];

		for(int i = nIStart; i < nIStart + nItems ; i++)
		{
			Contents.RemoveAll();
			StrTemp.Format(_LS(IDS_CCMLLANJPINFODLG_0829_MSG3) ,i+1); Contents.Add(StrTemp);
			StrTemp.Format(_LS(IDS_CCMLLANJPINFODLG_0829_MSG3) ,aElemKey[i-nIStart]) ; Contents.Add(StrTemp);
			ItemData = (DWORD)aElemKey[i-nIStart];
			CDlgUtil::SetListItem(&m_wndElemList,i,Contents,ItemData);
		}

		
		nCount = m_wndElemList.GetItemCount();
		int nEBuffer = EBuffer.GetSize();

		nIStart = SelIndex[0] + nItems;

		for(int i  = nIStart ; i < nIStart + nEBuffer ; i++)
		{
			Contents.RemoveAll();
			StrTemp.Format(_LS(IDS_CCMLLANJPINFODLG_0829_MSG3), i+1); Contents.Add(StrTemp);
			StrTemp.Format(_LS(IDS_CCMLLANJPINFODLG_0829_MSG3), EBuffer[i-nIStart]) ; Contents.Add(StrTemp);
			ItemData = (DWORD)EBuffer[i-nIStart];
			CDlgUtil::SetListItem(&m_wndElemList,i,Contents,ItemData);
		}
		
	}
	return TRUE;
}

void CCMLLaneJPInfoDlg::OnCmdAddButton() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	AddOp(0);	
}

void CCMLLaneJPInfoDlg::OnCmdDeleteButton() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	DelOp();
}

void CCMLLaneJPInfoDlg::OnCmdModifyButton() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	AddOp(1);
}

// nOP  (0) Add (1) Modify
void CCMLLaneJPInfoDlg::Data2Dlg(int *pnOP, T_LLANjp_BASE_OLD* pLLAND,CCMLLaneJPDlg* pParentDlg)
{
	// Dialog 생성후에 호출할것 
	m_pnOP         = pnOP       ;
	
	if(pLLAND)
		*m_pLLAND     = *pLLAND   ;
	else
		m_pLLAND->Initialize();

	m_pParentDlg  = pParentDlg;

	m_strName =  m_pLLAND->csName     ;

	m_wndLength.SetEditUnit(m_pLLAND->dLength);         
	m_wndP1Edit.SetEditUnit(m_pLLAND->dP1LoadMomentAdj);
	m_wndP1Edit2.SetEditUnit(m_pLLAND->dP1LoadShearAdj);
	m_wndP2Edit.SetEditUnit(m_pLLAND->dP2LoadAdj);
	m_wndCrowdLoad.SetEditUnit(m_pLLAND->dCrowdLoadAdj);
	m_wndTLoadEdit.SetEditUnit(m_pLLAND->dTLoadAdj);
	m_wndImpactCoeff.SetEditUnit(m_pLLAND->dImpactCoeffAdj);
	m_wndExtraCoeff.SetEditUnit(m_pLLAND->dExtraCoeffAdj);
	
	m_bP1Load      = m_pLLAND->T_LLANjp_BASE_OLD::bP1LoadMomentAdj ;
	m_bP1Load2     = m_pLLAND->T_LLANjp_BASE_OLD::bP1LoadShearAdj  ;
	m_bP2Load      = m_pLLAND->T_LLANjp_BASE_OLD::bP2LoadAdj       ;
	m_bCrowdLoad   = m_pLLAND->T_LLANjp_BASE_OLD::bCrowdLoadAdj    ;
	m_bTLoadCheck  = m_pLLAND->T_LLANjp_BASE_OLD::bTLoadAdj        ;
	m_bImpactCoeff = m_pLLAND->T_LLANjp_BASE_OLD::bImpactCoeffAdj  ;
	m_bExtraCoeff  = m_pLLAND->T_LLANjp_BASE_OLD::bExtraCoeffAdj   ;

	UpdateData(FALSE);

	this->CtrlMan();

	m_wndElemList.DeleteAllItems() ;
	CArray<unsigned int, unsigned int> arElem;
	int nElem = m_pLLAND->arElem.GetSize();
	for(int i = 0; i< nElem; i++)
	{
		arElem.Add(m_pLLAND->arElem[i]);
	}

	AddElemToList(arElem,0);

	SelectElem();
}

BOOL CCMLLaneJPInfoDlg::Dlg2Data(T_LLANjp_BASE_OLD* pLLAND)
{
	UpdateData(TRUE);
	if(m_strName.IsEmpty()) 
	{
		AfxMessageBox(_LS(IDS_CCMLLANJPINFODLG_0829_MSG4)
);
		return FALSE;
	}
	pLLAND->csName = m_strName;
	
	pLLAND->dLength = m_wndLength.GetEditValue();
	
	pLLAND->dP1LoadMomentAdj  =   m_pLLAND->dP1LoadShearAdj   =   m_pLLAND->dP2LoadAdj        =
	pLLAND->dCrowdLoadAdj     =   m_pLLAND->dTLoadAdj         =
	pLLAND->dImpactCoeffAdj   =   m_pLLAND->dExtraCoeffAdj    = 0;

	if(m_bP1Load     )  pLLAND->dP1LoadMomentAdj  = m_wndP1Edit.GetEditValue      ();
	if(m_bP1Load2    )  pLLAND->dP1LoadShearAdj   = m_wndP1Edit2.GetEditValue     ();
	if(m_bP2Load     )  pLLAND->dP2LoadAdj        = m_wndP2Edit.GetEditValue      ();
	if(m_bCrowdLoad  )  pLLAND->dCrowdLoadAdj     = m_wndCrowdLoad.GetEditValue   ();
	if(m_bTLoadCheck )  pLLAND->dTLoadAdj         = m_wndTLoadEdit.GetEditValue   ();
	if(m_bImpactCoeff)  pLLAND->dImpactCoeffAdj   = m_wndImpactCoeff.GetEditValue ();
	if(m_bExtraCoeff )  pLLAND->dExtraCoeffAdj    = m_wndExtraCoeff.GetEditValue  ();

	m_pLLAND->T_LLANjp_BASE_OLD::bP1LoadMomentAdj  = m_bP1Load     ;
	m_pLLAND->T_LLANjp_BASE_OLD::bP1LoadShearAdj   = m_bP1Load2    ;
	m_pLLAND->T_LLANjp_BASE_OLD::bP2LoadAdj        = m_bP2Load     ;
	m_pLLAND->T_LLANjp_BASE_OLD::bCrowdLoadAdj     = m_bCrowdLoad  ;
	m_pLLAND->T_LLANjp_BASE_OLD::bTLoadAdj         = m_bTLoadCheck ;
	m_pLLAND->T_LLANjp_BASE_OLD::bImpactCoeffAdj   = m_bImpactCoeff;
	m_pLLAND->T_LLANjp_BASE_OLD::bExtraCoeffAdj    = m_bExtraCoeff ;

	pLLAND->arElem.RemoveAll();
	int nItem = m_wndElemList.GetItemCount();
	unsigned int ElemK;

	for(int i = 0; i < nItem;  i++)
	{
		ElemK = (unsigned int)m_wndElemList.GetItemData(i);
		pLLAND->arElem.Add(ElemK);
	}
	return TRUE;
}


void CCMLLaneJPInfoDlg::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_UNIT:
		//UpdateUnit();
		break;
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
	case D_UPDATE_POST_STAGE_CHANGED:
	case D_UPDATE_POST_STEP_CHANGED:
		//DestroyWindow();
		ButtonCtrl();
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CCMLLaneJPInfoDlg::ButtonCtrl()
{
	BOOL bEnable = FALSE;
	if (m_pDoc->m_pStagCtrl->IsFinalStag() || m_pDoc->m_pStagCtrl->IsBaseStag())
		bEnable = TRUE;

	GetDlgItem(IDOK)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMD_APPLY)->EnableWindow(bEnable);
}

void CCMLLaneJPInfoDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	BOOL bMvcdChanged = FALSE;
	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		switch(nCmd)
		{
		case(UR_MVCD_ADD):
		case(UR_MVCD_DEL):
			bMvcdChanged = TRUE;
			break;
		default:
			break;
		}
	} // end of while
	// 코드가 바뀌면 무조건 닫는다.
	if (bMvcdChanged)
	{
		DestroyWindow();
	}
}

BOOL CCMLLaneJPInfoDlg::IsRelevantCode(int nCode)
{
	if (nCode == D_MOVE_CODE_JAPAN) return TRUE;
	return FALSE;
}

void CCMLLaneJPInfoDlg::ShowHideByCode()
{
	T_MVCD_D MvcdData;
	CDBDoc* pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
	if (!pDoc->m_pAttrCtrl->GetMvcd(MvcdData)) MvcdData.Initialize();
	if (!IsRelevantCode(MvcdData.nCodeType)) DestroyWindow();
}


/*



////////////////////////// REGISTER ID & STRING //////////////////////////
REG_STR_RES(IDS_CCMLLANJPINFODLG_0829_MSG1      , _T("No."));
REG_STR_RES(IDS_CCMLLANJPINFODLG_0829_MSG2      , _T("Element No."));
REG_STR_RES(IDS_CCMLLANJPINFODLG_0829_MSG3      , _T("%d"));
REG_STR_RES(IDS_CCMLLANJPINFODLG_0829_MSG4      , _T("지간이름이 입력되지 않았습니다."));




////////////////////////// ENUM ID REGISTERING  //////////////////////////
IDS_CCMLLANJPINFODLG_0829_MSG1,
IDS_CCMLLANJPINFODLG_0829_MSG2,
IDS_CCMLLANJPINFODLG_0829_MSG3,
IDS_CCMLLANJPINFODLG_0829_MSG4,

*/