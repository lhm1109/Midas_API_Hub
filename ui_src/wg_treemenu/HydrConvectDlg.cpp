// HydrConvectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "HydrConvectDlg.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_cmd\wg_cmdAll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHydrConvectDlg dialog

CHydrConvectDlg::CHydrConvectDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CHydrConvectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHydrConvectDlg)
	m_option = -1;
	m_select = -1;
	//}}AFX_DATA_INIT

	m_group.RemoveAll();
	m_group.Add(IDC_TM_HYDR_FUNC);
	m_group.Add(IDC_TM_HYDR_FUNC_COMBO);
	m_group.Add(IDC_TM_HYDR_FUNC_BTN);
	m_group.Add(IDC_TM_HYDR_FUNC_COMBO1);
	m_group.Add(IDC_TM_HYDR_FUNC_BTN1);
	m_group.Add(IDC_HYDR_CONV_TITLE);
	m_group.Add(IDC_HYDR_AMBI_TITLE);
	m_group.Add(IDC_TM_HYDR_FACE_CHK);
	m_group.Add(IDC_TM_HYDR_FACE_COMBO);
	m_group.Add(IDC_TM_HYDR_SELNODE_CHK);

	m_gFace.Add(IDC_TM_HYDR_FACE_COMBO);

	m_pDoc = CDBDoc::GetDocPoint();
	//m_pDoc->m_pViewCtrl->SetHydratiorInputMode(TRUE);
	//UpDateSelectOption();
}


void CHydrConvectDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHydrConvectDlg)
	DDX_Control(pDX, IDC_TM_GROUP_COMBO, m_wndGroupCombo);
	DDX_Control(pDX, IDC_TM_HYDR_FUNC_COMBO1, m_CobxConv);
	DDX_Control(pDX, IDC_TM_HYDR_FUNC_COMBO, m_CobxFunc );
	DDX_Control(pDX, IDC_TM_HYDR_FACE_COMBO, m_CobxFace );
	DDX_Radio(pDX, IDC_TM_HYDR_ADDREPL, m_option        );
	DDX_Radio(pDX, IDC_TM_HYDR_FACE_CHK, m_select       );
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHydrConvectDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CHydrConvectDlg)
	//ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose                              )
	//ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute                          )
	ON_BN_CLICKED(IDC_TM_HYDR_ADDREPL, OnTmChangeOption                )
	ON_BN_CLICKED(IDC_TM_HYDR_FACE_CHK, OnTmChangeSelect               )
	ON_BN_CLICKED(IDC_TM_HYDR_FUNC_BTN, OnTmHydrFuncBtn                )
	ON_CBN_SELCHANGE(IDC_TM_HYDR_FACE_COMBO, OnSelchangeTmHydrFaceCombo)
	ON_BN_CLICKED(IDC_TM_HYDR_FUNC_BTN1, OnTmHydrFuncBtn1              )
	ON_BN_CLICKED(IDC_TM_DEFINE_GROUP_BUTTON, OnTmDefineGroupButton)
	ON_BN_CLICKED(IDC_TM_HYDR_DEL, OnTmChangeOption                    )
	ON_BN_CLICKED(IDC_TM_HYDR_SELNODE_CHK, OnTmChangeSelect            )
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHydrConvectDlg message handlers

BOOL CHydrConvectDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
//	m_coefficientunit.SetUnitType(CUnitCtrl::m_HECB_UNIT.dConvecCoef);

	// TODO: Add extra initialization here
	m_option = 0;
	m_select = 0;
	m_CobxFace.ResetContent();
	m_CobxFace.AddString(_LS(IDS_WG_TREEMENU_Face__1));   // Face #1
	m_CobxFace.AddString(_LS(IDS_WG_TREEMENU_Face__2));   // Face #1
	m_CobxFace.AddString(_LS(IDS_WG_TREEMENU_Face__3));   // Face #1
	m_CobxFace.AddString(_LS(IDS_WG_TREEMENU_Face__4));   // Face #1
	m_CobxFace.AddString(_LS(IDS_WG_TREEMENU_Face__5));   // Face #1
	m_CobxFace.AddString(_LS(IDS_WG_TREEMENU_Face__6));   // Face #1

	m_CobxFace.SetCurSel(0);
	UpdateData(FALSE);
	
	
	//MakeComboNpln();

	UpDateSelectOption();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
/*
void CHydrConvectDlg::MakeComboNpln()
{
	m_CobxNPlane.ResetContent();
	T_NPLN_K Key;
	T_NPLN_D Data;

	CString csItem;
	int nIndex = 0;
	CArray<T_NPLN_K, T_NPLN_K> aKey;
	m_pDoc->m_pAttrCtrl->GetNplnKeyList(aKey);
	int nSize = aKey.GetSize();
	for (int i=0; i < nSize; i++)
	{
		Key = aKey[i];
	  m_pDoc->m_pAttrCtrl->GetNpln(Key, Data);
		csItem.Format(_T("%s"), Data.PlaneName);
		nIndex = m_CobxNPlane.AddString(csItem);
		m_CobxNPlane.SetItemData(nIndex, (DWORD)Key);
	}

	if (nSize > 0)  m_CobxNPlane.SetCurSel(0);
}
*/

void CHydrConvectDlg::Execute() 
{
	// TODO: Add your control notification handler code here
 	UpdateData();
	CArray<T_ELEM_K, T_ELEM_K> aSelElemList;
	CArray<T_NODE_K, T_NODE_K> aSelNodeList;
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  


	CArray<T_HECB_D, T_HECB_D&> aData;
	T_HECB_D data;
	data.Initialize();

	m_wndGroupCombo.GetSelectedBngr(data.GroupKey);

	if (m_option == 0)  // add/replace
	{
		T_ETFC_K Etfckey;
		m_CobxFunc.GetSelectedEtfc(Etfckey);
		data.nFuncKey = Etfckey;

	  T_CCFC_K Ccfckey;
		m_CobxConv.GetSelectedCcfc(Ccfckey);
	  data.nCcfcKey = Ccfckey;

		if(m_select == 0) // face
		{
	    pIGM->GetSelectedElemKeyList(aSelElemList);
		  if (aSelElemList.GetSize() == 0) 
		  {
			  GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_solid_element_));
			  return;
		  }

			data.nFaceNo = m_CobxFace.GetCurSel()+1;
		  for (int i = 0; i < aSelElemList.GetSize(); i++)
			  aData.Add(data);
		}
		else if(m_select == 1) // selected nodes
		{
		  CArray<T_ELEM_K,T_ELEM_K> aActElemList; // ActiveµÈ element list
		  CArray<int,int>           aFaceNo;

		  pIGM->GetSelectedNodeKeyList(aSelNodeList);
		  pIGM->GetActiveElemKeys(aActElemList);
		  if (aSelNodeList.GetSize() == 0) 
		  {
			  GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_node_));
			  return;
		  }

		  m_pDoc->SelectFaceByNode(aActElemList, aSelNodeList, aSelElemList, aFaceNo);
		  for (int i=0; i < aFaceNo.GetSize(); i++)
		  {
			  data.nFaceNo = aFaceNo[i];
			  aData.Add(data);
		  }
	  }
	}
	else
	{
		pIGM->GetSelectedElemKeyList(aSelElemList);
		if (aSelElemList.GetSize() == 0) 
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_solid_element_));
			return;
		}
	}

	BOOL bSuccess=FALSE;
	if (m_option == 0) // Add or Replace
		bSuccess = m_pDoc->m_pDataCtrl->AddHecb(aSelElemList, aData);
	else if (m_option == 1)  // delete
		bSuccess = m_pDoc->m_pDataCtrl->DelHecb(aSelElemList, data.GroupKey);

	if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CHydrConvectDlg::OnTmChangeOption() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	if(m_option == 1)
	{
		CtrlEnableDisable(m_group, FALSE);
		m_wndGroupCombo.ShowHideAll(TRUE);
	}
	else
	{
		CtrlEnableDisable(m_group, TRUE);
		m_wndGroupCombo.ShowHideAll(FALSE);
		OnTmChangeSelect();
	}
}

void CHydrConvectDlg::OnTmChangeSelect() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	if(m_select == 0)
	{
		CtrlEnableDisable(m_gFace,   TRUE);
	}
	else if(m_select == 1)
	{
		CtrlEnableDisable(m_gFace,   FALSE);
	}
	else if(m_select == 2)
	{
		CtrlEnableDisable(m_gFace,   FALSE);
	}
	else
	{
		CtrlEnableDisable(m_gFace,   FALSE);
	}

	CheckMode();
}

void CHydrConvectDlg::OnTmHydrFuncBtn() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_HYDRATION_ENVTEMP,0));
}

void CHydrConvectDlg::CheckMode()
{
	UpdateData();
	/*
	CString str = _T("");
	
	m_pDoc->m_pViewCtrl->SetHydratiorInputParam(m_select, m_CobxFace.GetCurSel(), str);
	*/
	UpDateSelectOption();
}

void CHydrConvectDlg::OnSelchangeTmHydrFaceCombo() 
{
	// TODO: Add your control notification handler code here
	CheckMode();
}

void CHydrConvectDlg::OnTmHydrFuncBtn1() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_HYDRATION_CCFC,0));
}

BOOL CHydrConvectDlg::ExternalInit(UINT key)
{
	if (key == 0) return FALSE;

	T_HECB_K hecbkey;
	T_HECB_D data;

	hecbkey.keymap = key;
	m_pDoc->m_pAttrCtrl->GetHecb(hecbkey, data);
	
	m_wndGroupCombo.ChangeSelect(data.GroupKey);
	m_CobxFunc.ChangeSelect(data.nFuncKey);
	m_CobxConv.ChangeSelect(data.nCcfcKey);
	m_CobxFace.SetCurSel(data.nFaceNo-1);
	UpdateData(FALSE);

	// change selection
	UpDateSelectOption();

	return TRUE;
}

void CHydrConvectDlg::OnTmDefineGroupButton() 
{
	CreateOrActivateDlg(m_pDoc,CCMBndrGrupDefDlg::IDD);	
}

void CHydrConvectDlg::UpDateSelectOption()
{

	int nElemType;
	if( m_select == 0) //Face
		nElemType = 8 ;
	else
		nElemType = 10;
	
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if(pDoc)
	{
		pDoc->m_pViewCtrl->SetPressureInputMode(TRUE);
		pDoc->m_pViewCtrl->SetPressureInputParam(nElemType, m_CobxFace.GetCurSel());	
	}
}

void CHydrConvectDlg::OnClose() 
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if(pDoc)
	{
		pDoc->m_pViewCtrl->SetPressureInputMode (FALSE);  
		pDoc->m_pViewCtrl->SetPressureInputParam(-1,-1);	
	}
	CMenuBarChildDlg::OnClose();
}
