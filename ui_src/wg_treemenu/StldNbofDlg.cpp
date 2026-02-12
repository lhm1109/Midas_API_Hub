// StldNbofDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldNbofDlg.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_main\wg_mainRes2.h"

#include "..\wg_base\wg_base_StrParser.h"

#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStldNbofDlg dialog


CStldNbofDlg::CStldNbofDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CStldNbofDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStldNbofDlg)
	m_bUseChk  = FALSE;	
	m_bNmasChk = TRUE;
	m_bLtmsChk = TRUE;
	m_bStmsChk = TRUE;
	m_bGmasChk = TRUE;
	//}}AFX_DATA_INIT
}


void CStldNbofDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStldNbofDlg)
	DDX_Control(pDX, IDC_TM_NBOF_LIST, m_List);
	DDX_Control(pDX, IDC_TM_NBOF_Z_EDIT, m_wndFactorZ);
	DDX_Control(pDX, IDC_TM_NBOF_Y_EDIT, m_wndFactorY);
	DDX_Control(pDX, IDC_TM_NBOF_X_EDIT, m_wndFactorX);
	DDX_Control(pDX, IDC_TM_NBOF_LOAD_CBO, m_LoadCase);
	DDX_Control(pDX, IDC_TM_NBOF_LDGR_CBO, m_wndLdgrCobx);
	DDX_Control(pDX, IDC_TM_NBOF_STGR_CBO, m_wndGrupCobx);  
	DDX_Control(pDX, IDC_TM_EDT_NODE, m_edtSelectedNode);
	DDX_Check(pDX, IDC_TM_NBOF_USE_CHK,  m_bUseChk);
	DDX_Check(pDX, IDC_TM_NBOF_NMAS_CHK, m_bNmasChk);
	DDX_Check(pDX, IDC_TM_NBOF_LTMS_CHK, m_bLtmsChk);
	DDX_Check(pDX, IDC_TM_NBOF_STMS_CHK, m_bStmsChk);   
	DDX_Check(pDX, IDC_TM_NBOF_GMAS_CHK, m_bGmasChk); 
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStldNbofDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CStldNbofDlg)  
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TM_NBOF_LIST, OnChangedItemSelection)	
	ON_BN_CLICKED(IDC_TM_NBOF_LOAD_BTN, OnTmNbofLoadBtn)
	ON_BN_CLICKED(IDC_TM_NBOF_LDGR_BTN, OnTmNbofLdgrBtn)
	ON_BN_CLICKED(IDC_TM_NBOF_STGR_BTN, OnTmNbofStgrBtn)
	ON_BN_CLICKED(IDC_TM_NBOF_ADD, OnTmNbofAdd)
	ON_BN_CLICKED(IDC_TM_NBOF_MODIFY, OnTmNbofModify)
	ON_BN_CLICKED(IDC_TM_NBOF_DELETE, OnTmNbofDelete)
	ON_BN_CLICKED(IDC_TM_NBOF_USE_CHK, OnTmNbofUseChk)
	ON_CBN_SELCHANGE(IDC_TM_NBOF_STGR_CBO, OnSelchangeTmNbofStgrCbo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldNbofDlg message handlers

void CStldNbofDlg::OnTmClose() 
{
	// TODO: Add your control notification handler code here
	CloseDlg();
}

void CStldNbofDlg::Execute()
{

}

void CStldNbofDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_POST_STAGE_CHANGED:
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		MakeItemEx();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		MakeItemEx();
		break;
	case D_UPDATE_UNIT:
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

BOOL CStldNbofDlg::ExternalInit(UINT key)
{
	if(key == 0) return FALSE;

	T_NBOF_D data;

	m_pDoc->m_pAttrCtrl->GetNbof(key, data);
	m_wndFactorX.SetEditUnit(data.dFactorX);
	m_wndFactorY.SetEditUnit(data.dFactorY);
	m_wndFactorZ.SetEditUnit(data.dFactorZ);
	
	UpdateData(FALSE);

	return TRUE;
}

BOOL CStldNbofDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	m_pDoc = CDBDoc::GetDocPoint();

	m_wndFactorX.SetUnitType(D_UNITSYS_NONE);
	m_wndFactorY.SetUnitType(D_UNITSYS_NONE);
	m_wndFactorZ.SetUnitType(D_UNITSYS_NONE);

	m_wndFactorX.SetEditUnit(0);
	m_wndFactorY.SetEditUnit(0);
	m_wndFactorZ.SetEditUnit(0);
	
	SetListCtrlHeader();

	// DB에 저장된 Body Force 값을 가져와서 List에 보여준다.
	MakeItemEx();

	/////////// Grid Mass는 일본 Civil에서만 사용  /////////
	////////////////////////////////////////////////////////  
	GetDlgItem(IDC_TM_NBOF_GMAS_CHK)->ShowWindow(FALSE);
	if(CProduct::IsMovingType(D_PRODUCT_MOVING_JP))
	{
		#if defined(_CIVIL)
			GetDlgItem(IDC_TM_NBOF_GMAS_CHK)->ShowWindow(TRUE);
		#endif
	}   
	////////////////////////////////////////////////////////

	UpdateData(FALSE);
	OnTmNbofUseChk(); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStldNbofDlg::SetListCtrlHeader()
{
	CStringArray aTitles;
	CArray<int,int> aWidths;
	CArray<float,float> aWRatio;
	aTitles.Add(_LS(IDS_WG_TREEMENU_Load_Case));
	aTitles.Add(_T("X"));
	aTitles.Add(_T("Y"));
	aTitles.Add(_T("Z"));
	aTitles.Add(_LS(IDS_WG_TREEMENU_ADD3_Group));
	CSize size;
#pragma warning(disable : 4305)
	aWRatio.Add((float)4/10.);
	aWRatio.Add((float)2/10.);
	aWRatio.Add((float)2/10.);
	aWRatio.Add((float)2/10.);
	aWRatio.Add((float)0/10.);
#pragma warning(default : 4305)

	CDlgUtil::_SetListCtrlHeader(&m_List,aTitles,&aWRatio,NULL);
}

void CStldNbofDlg::OnChangedItemSelection(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	T_NBOF_K key;
	T_NBOF_D data;
	if(!GetSelectedNodalBodyForce(key))
	{
		data.Initialize();
	}
	else
	{
		if(!m_pDoc->m_pAttrCtrl->GetNbof(key, data)) return;
		// Load case 선택을 변경한다.
		int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
		CString sLoadCaseName = m_List.GetItemText(iItem, 0);
		//m_LoadCase.SelectString(-1, sLoadCaseName);
		T_NBOF_K key = m_pDoc->m_pAttrCtrl->GetStldKey(sLoadCaseName);    
		Data2Dlg(key, data);  
		OnTmNbofUseChk();
		OnSelchangeTmNbofStgrCbo();    
	}  
	*pResult = 0;
}

void CStldNbofDlg::GetFactor(double &x, double &y, double &z)
{  
	x = m_wndFactorX.GetEditValue();
	y = m_wndFactorY.GetEditValue();
	z = m_wndFactorZ.GetEditValue();	
}

BOOL CStldNbofDlg::GetSelectedNodalBodyForce(T_BODF_K &key)
{
	
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;

	key = (T_NBOF_K)m_List.GetItemData(iItem);
	/*
	CString sLoadCaseName = m_List.GetItemText(iItem, 0);
	key = (T_BODF_K)m_pDoc->m_pAttrCtrl->GetStldKey(sLoadCaseName);
	*/

	return TRUE;
}

void CStldNbofDlg::MakeItemEx()
{
	m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountNbof();
	if(nItemCount == 0) return;

	T_NBOF_K Key;
	T_NBOF_D Data;
	CArray<UINT, UINT> KeyBuffer;
	POSITION Pos;
	Pos = m_pDoc->m_pAttrCtrl->GetStartNbof();
	while(Pos != NULL)
	{
		m_pDoc->m_pAttrCtrl->GetNextNbof(Pos,Key,Data);
		KeyBuffer.Add(Key);
	}
	
	int nSize = KeyBuffer.GetSize();
	CStringArray Contents;
	CString      TStr;
	T_STLD_D     LcData  ; // Load Case   
	T_LDGR_D     LdgrData; // Load Group Data
	if(nSize)
	{
		qsort(KeyBuffer.GetData(),nItemCount,sizeof(T_NBOF_K), CCompFunc::UINTAsc);
		int nIndex = 0;
		for(int i = 0 ;i < nSize; i++)
		{
			if(m_pDoc->m_pAttrCtrl->GetNbof(KeyBuffer[i],Data))
			{
				//Data.LoadCaseKey;
				if(m_pDoc->m_pAttrCtrl->GetStld(Data.LoadCaseKey,LcData))
				{
					if(!IsAdd2ListCase(LcData))
						 continue;
					Contents.RemoveAll();
					Contents.Add(LcData.LoadCaseName);
					TStr.Format(_T("%g"), Data.dFactorX);
					Contents.Add(TStr);
					TStr.Format(_T("%g"), Data.dFactorY);
					Contents.Add(TStr);
					TStr.Format(_T("%g"), Data.dFactorZ);
					Contents.Add(TStr);
					
					if(m_pDoc->m_pAttrCtrl->GetLdgr(Data.LdgrKey,LdgrData))
					{
						Contents.Add(LdgrData.GroupName);
					}
					else
					{
						Contents.Add(_LS(IDS_WG_TREEMENU_ADD3_Default)); 
					}
					CDlgUtil::SetListItem(&m_List, nIndex, Contents,(DWORD)KeyBuffer[i]);
					nIndex += 1;
				}
			}
		}
	}
}
BOOL CStldNbofDlg::IsAdd2ListCase(const T_STLD_D &Stld)
{
		return TRUE;
}
CString CStldNbofDlg::DataToStr(int i, T_NBOF_K key, T_NBOF_D &data)
{
	CString value;
	T_STLD_D LoadData;
	if (i == 0) 
	{
		VERIFY(m_pDoc->m_pAttrCtrl->GetStld((T_STLD_K)key, LoadData));
		value = LoadData.LoadCaseName;
	}
	else if (i == 1) value.Format(_T("%g"), data.dFactorX);
	else if (i == 2) value.Format(_T("%g"), data.dFactorY);
	else if (i == 3) value.Format(_T("%g"), data.dFactorZ);
	return value;
}


void CStldNbofDlg::OnTmNbofLoadBtn() 
{
	  AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE,0));
}

void CStldNbofDlg::OnTmNbofLdgrBtn() 
{
	CreateOrActivateDlg(m_pDoc, CCMLoadGroupDlg::IDD);		
}

void CStldNbofDlg::OnTmNbofStgrBtn() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_GROUP,0));	
}

void CStldNbofDlg::OnTmNbofAdd() 
{  
	T_NBOF_D data;
	data.Initialize();

	if(!Dlg2Data(data)) 
	{
		return;
	}
	m_pDoc->m_pDataCtrl->AddNbof(data);	
}

void CStldNbofDlg::OnTmNbofModify() 
{
	T_NBOF_K KeyOld;
	T_NBOF_D data;
	data.Initialize();
	
	if (!GetSelectedNodalBodyForce(KeyOld))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error_NBOF_NOT_SELECT));
		return;
	}
	
	if(!Dlg2Data(data)) return;
			
	m_pDoc->m_pDataCtrl->ModifyNbof(KeyOld, data);
}

void CStldNbofDlg::OnTmNbofDelete() 
{
	T_NBOF_K key;

	if (!GetSelectedNodalBodyForce(key))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error_NBOF_NOT_SELECT));
		return;
	}
	m_pDoc->m_pDataCtrl->DelNbof(key);
}

void CStldNbofDlg::OnTmNbofUseChk() 
{
	UpdateData(TRUE);
	GetDlgItem(IDC_TM_NBOF_STGR_CBO)->EnableWindow(m_bUseChk);
	GetDlgItem(IDC_TM_NBOF_STGR_BTN)->EnableWindow(m_bUseChk);

	m_edtSelectedNode.SetReadOnly(m_bUseChk);
	
	OnSelchangeTmNbofStgrCbo();
	
	if(m_bUseChk)
		m_edtSelectedNode.Disconnect();
	else
		m_edtSelectedNode.Connect(SC_ID_NODE, &m_edtSelectedNode);	
}

void CStldNbofDlg::OnSelchangeTmNbofStgrCbo() 
{
	T_GRUP_K rKey; //T_GRUP_D rData;
	CArray<T_NODE_K, T_NODE_K> arKeyNode;

	CString str;
	CNumericOptimizer optimizer;
	UpdateData(TRUE); 

	if(m_bUseChk)
	{    
		m_wndGrupCobx.GetSelectedGrup(rKey); // 그룹 박스에서 선택 된 것의 키 정보를 얻어 온다.
	
		m_pDoc->m_pEditData->GetNbofNodeList(rKey, arKeyNode); //Node 키 정보를 얻어온다.

		str = optimizer.OptimizeUnsortedArray(arKeyNode); // optimise data
	
		m_edtSelectedNode.SetWindowText(str); // set editbox
	}	
}

BOOL CStldNbofDlg::Data2Dlg(T_NBOF_K key, T_NBOF_D &data)
{
	CString strNode;
	CNumericOptimizer optimizer;

	m_LoadCase.ChangeSelect(D_LOADCASE_STATIC, key);
	m_wndLdgrCobx.ChangeSelect(data.LdgrKey);
	m_bUseChk = data.bUseGrup;
		
	if (m_bUseChk)
	{
		m_wndGrupCobx.ChangeSelect(data.GrupKey);
	}
	else
	{
		strNode = optimizer.OptimizeUnsortedArray(data.arKeyNode);
		m_edtSelectedNode.SetWindowText(strNode);
		m_edtSelectedNode.SelectByStr(strNode);
	}

	m_wndFactorX.SetEditUnit(data.dFactorX);
	m_wndFactorY.SetEditUnit(data.dFactorY);
	m_wndFactorZ.SetEditUnit(data.dFactorZ);
		
	m_bNmasChk = data.bNodalMass;
	m_bLtmsChk = data.bLoadToMass;
	m_bStmsChk = data.bStructMass;
	m_bGmasChk = data.bGridMass;
	
	UpdateData(FALSE);
	return TRUE;
}

BOOL CStldNbofDlg::Dlg2Data(T_NBOF_D &data) 
{
	UpdateData(TRUE);
	data.Initialize();
	
	unsigned int nLoadCaseKey ;
	unsigned int nLoadCaseType;
	if(!m_LoadCase.GetSelectedLoad(nLoadCaseType, nLoadCaseKey)) 
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Load_case_was_not_selected_));
		return FALSE;
	}
	
	data.LoadCaseKey = nLoadCaseKey;    
	m_wndLdgrCobx.GetSelectedLdgr(data.LdgrKey);  
	data.bUseGrup = m_bUseChk;
	data.bNodalMass  = m_bNmasChk;
	data.bLoadToMass = m_bLtmsChk;
	data.bStructMass = m_bStmsChk;
	data.bGridMass   = m_bGmasChk; 
	data.dFactorX = m_wndFactorX.GetEditValue();
	data.dFactorY = m_wndFactorY.GetEditValue();
	data.dFactorZ = m_wndFactorZ.GetEditValue();	
	
	if(m_bUseChk)
		m_wndGrupCobx.GetSelectedGrup(data.GrupKey);
	else
		m_pDoc->m_pViewCtrl->GetAllSelectedNode(data.arKeyNode);
	
	return TRUE;
}

/*
struct T_NBOF_D
{
	T_STLD_K LoadCaseKey;
	T_LDGR_K LdgrKey;  
	BOOL    bUseGrup;
	T_GRUP_K GrupKey;
	CArray<T_NODE_K, T_NODE_K> arKeyNode;    
	BOOL    bNodalMass;
	BOOL    bLoadToMass;
	BOOL    bStructMass;
	BOOL    bGridMass;
	double  dFactorX;
	double  dFactorY;
	double  dFactorZ;

*/