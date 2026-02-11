// TpscGroupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "TpscGroupDlg.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\wg_base_NumericOptimizer.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "TpsgConvertDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTpscGroupDlg dialog


CTpscGroupDlg::CTpscGroupDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CTpscGroupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTpscGroupDlg)
	m_nYAxisSymPln = 0;
	m_nZAxisSymPln = 0;
	m_nYAxisVariType = 0;
	m_nZAxisVariType = 0;
	m_dYAxisDist = 0.0;
	m_dZAxisDist = 0.0;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CTpscGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTpscGroupDlg)
	DDX_Control(pDX, IDC_TM_TPSC_ZAXIS_DIST_UNIT, m_ZAxisUnit);
	DDX_Control(pDX, IDC_TM_TPSC_YAXIS_DIST_UNIT, m_YAxisUnit);
	DDX_Control(pDX, IDC_TM_TPSC_GROUP_NAME_EDIT, m_edtGroupName);
	DDX_Control(pDX, IDC_TM_TPSC_GROUP_LOADLIST_EDIT, m_edtElemList);
	DDX_Control(pDX, IDC_TM_TPSC_GROUP_LIST, m_List);
	DDX_Radio(pDX, IDC_TM_TPSC_YAXIS_SYMPL_I, m_nYAxisSymPln);
	DDX_Radio(pDX, IDC_TM_TPSC_ZAXIS_SYMPL_I, m_nZAxisSymPln);
	DDX_Radio(pDX, IDC_TM_TPSC_YAXIS_LINEAR, m_nYAxisVariType);
	DDX_Radio(pDX, IDC_TM_TPSC_ZAXIS_LINEAR, m_nZAxisVariType);
	DDX_Text(pDX, IDC_TM_TPSC_YAXIS_DIST, m_dYAxisDist);
	DDX_Text(pDX, IDC_TM_TPSC_ZAXIS_DIST, m_dZAxisDist);
	DDX_Control(pDX, IDC_CMD_YQUAD_EDIT, m_edtYQuad);
	DDX_Control(pDX, IDC_CMD_ZQUAD_EDIT, m_edtZQuad);
	DDX_Control(pDX, IDC_TM_PICTURE, m_wndTpscPicture);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTpscGroupDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CTpscGroupDlg)
	ON_BN_CLICKED(IDC_TM_TPSC_GROUP_ADD, OnTmTpscGroupAdd)
	ON_BN_CLICKED(IDC_TM_TPSC_GROUP_MODIFY, OnTmTpscGroupModify)
	ON_BN_CLICKED(IDC_TM_TPSC_GROUP_DELETE, OnTmTpscGroupDelete)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TM_TPSC_GROUP_LIST, OnItemchangedTmTpscGroupList)
	ON_EN_CHANGE(IDC_TM_TPSC_GROUP_LOADLIST_EDIT, OnChangeTmTpscGroupLoadlistEdit)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_TM_TPSC_YAXIS_LINEAR, OnTmTpscYaxisVarType)
	ON_BN_CLICKED(IDC_TM_TPSC_ZAXIS_LINEAR, OnTmTpscZaxisVarType)
	ON_BN_CLICKED(IDC_TM_TPSC_GROUP_CONVERT, OnTmTpscGroupConvert)
	ON_BN_CLICKED(IDC_TM_TPSC_YAXIS_QUAD, OnTmTpscYaxisVarType)
	ON_BN_CLICKED(IDC_TM_TPSC_ZAXIS_QUAD, OnTmTpscZaxisVarType)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_YQUAD_SPIN, OnDeltaposCmdYquadSpin)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_ZQUAD_SPIN, OnDeltaposCmdZquadSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTpscGroupDlg message handlers

BOOL CTpscGroupDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_edtElemList.Connect(SC_ID_ELEM, &m_edtElemList);
	m_YAxisUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_ZAxisUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_edtYQuad.SetUnitType(D_UNITSYS_NONE);
	m_edtYQuad.SetEditUnit(2., 1);
	m_edtZQuad.SetUnitType(D_UNITSYS_NONE);
	m_edtZQuad.SetEditUnit(2., 1);

	OnTmTpscYaxisVarType();
	OnTmTpscZaxisVarType();
	
	InitListTitle();

	//IDB_TM_TPSG             BITMAP                  "res\\tm_tpsg.bmp"
	m_wndTpscPicture.SetImage(_T("SVG\\illustration\\Dialog\\tm_tpsg 1.svg"));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

#define COLCOUNT 2
void CTpscGroupDlg::InitListTitle()
{
	

	CString aTitle[] = {_LS(IDS_WG_TREEMENU_Name),_LS(IDS_WG_TREEMENU_Element_List)};
	int nColWidth[COLCOUNT]={90,160};

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	// Set Title
	for(i = 0; i < COLCOUNT; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}

	if(m_pDoc->m_pAttrCtrl->GetCountTsgr())
	{
		InitTsgrData();
//	  m_List.SetItemState(0,  LVIS_FOCUSED|LVIS_SELECTED , LVIS_FOCUSED|LVIS_SELECTED); 
	}
}

void CTpscGroupDlg::InitTsgrData()
{
	m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountTsgr();
	if(nItemCount == 0) return;

	POSITION pos;
	CString str;
	int nCount;

	if(nItemCount)
	{
		T_TSGR_K Key;
		T_TSGR_D Data;

	  int* KeyBuf=new int[nItemCount];
	  nCount = 0;
		pos = m_pDoc->m_pAttrCtrl->GetStartTsgr();
		while(pos !=NULL)
		{
		  m_pDoc->m_pAttrCtrl->GetNextTsgr(pos, Key, Data);
		  KeyBuf[nCount++] = Key;
		}
		qsort(KeyBuf, nItemCount, sizeof(T_TSGR_K), CCompFunc::UINTAsc);

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
	  {
		  Key=KeyBuf[nCount];
			m_pDoc->m_pAttrCtrl->GetTsgr(Key,Data);

			int nIndex = m_List.InsertItem(nCount, Data.GroupName);
			
//		qsort((void*)m_TsgrData.aElemList.GetData(), m_TsgrData.aElemList.GetSize(), sizeof(T_NODE_K), CCompFunc::UINTAsc);
			CString strElemList;
			CNumericOptimizer optimizer;
			strElemList = optimizer.Optimize((long*)Data.aElemList.GetData(),Data.aElemList.GetSize());
			m_List.SetItemText(nIndex, 1, strElemList);		
		}
		delete []KeyBuf;
	}
}

void CTpscGroupDlg::OnTmTpscGroupAdd() 
{
	UpdateData(TRUE);

	CString strDist;
	CString strGroupName;	
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();

	m_edtGroupName.GetWindowText(strGroupName);

	if(strGroupName == _T(""))
	{
		GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___Group_Name_was_not_defined_));
		return;
	}

	m_TsgrData.Initialize();

	CString strElemList;
	m_edtElemList.GetWindowText(strElemList);
	m_edtElemList.SelectByStr(strElemList);

	pIGM->GetSelectedElemKeyList(m_TsgrData.aElemList);
	if(!m_TsgrData.aElemList.GetSize())
	{
		GSaveHistoryNF(_LS(IDS_WG_TREEMENU_No_selected_element___));
		return;
	}

	//MQC::2785-JHYUN-20090731
	T_ELEM_D ElemData;
	T_SECT_D SectData;

	for(int i=0; i<m_TsgrData.aElemList.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl->GetElem(m_TsgrData.aElemList[i], ElemData)) { ASSERT(0); return; }
		if(!m_pDoc->m_pAttrCtrl->GetSect(ElemData.elpro, SectData)) { ASSERT(0); return; }

		if(SectData.nStype != D_SECT_TYPE_TAPERED)
		{
			GSaveHistoryNF(_LS(IDS_TM_TEPERED_SECTION_GROUP_TYPE_NOT_TAPERED));
			return;
		}
	}
	
	// MQC:334-Seungjun-20100319 중복된 요소가 있는 경우 error처리.
	BOOL bDuplicateElem = FALSE;
	int nCount = m_TsgrData.aElemList.GetSize();
	for(int i=0; i<nCount-1; i++)
	{
		T_ELEM_D ElemOrg, ElemTag;
		if(!m_pDoc->m_pAttrCtrl->GetElem(m_TsgrData.aElemList[i], ElemOrg)) { ASSERT(0); return; }
		for(int j=i+1; j<nCount; j++)
		{
			if(!m_pDoc->m_pAttrCtrl->GetElem(m_TsgrData.aElemList[j], ElemTag)) { ASSERT(0); return; }
			if(m_pDoc->m_pEditData->CheckDuplicate(ElemOrg, ElemTag))
			{
				bDuplicateElem=TRUE;
				GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Duplicated_Elems),m_TsgrData.aElemList[i],m_TsgrData.aElemList[j]);
			}
		}
	}
	if(bDuplicateElem)  return;
		
	m_TsgrData.GroupName = strGroupName;
	m_TsgrData.nVariation_y = m_nYAxisVariType;
	m_TsgrData.nQuadEnd_y   = m_nYAxisSymPln;
	m_TsgrData.nVariation_z = m_nZAxisVariType;
	m_TsgrData.nQuadEnd_z   = m_nZAxisSymPln;
	m_TsgrData.dQuadDistance_y = m_dYAxisDist;
	m_TsgrData.dQuadDistance_z = m_dZAxisDist;
			
	if(m_nYAxisVariType == 1)
	{
		m_TsgrData.dQuadExp_y = m_edtYQuad.GetEditValue();
	}
	
	if(m_nZAxisVariType == 1)
	{
		m_TsgrData.dQuadExp_z = m_edtZQuad.GetEditValue();
	}

	if(m_pDoc->m_pDataCtrl->AddTsgr(m_TsgrData))
	{
		m_pDoc->m_pViewCtrl->UnselectAll(NULL);
		m_edtGroupName.SetWindowText(_T(""));
		m_edtElemList.SetWindowText(_T(""));
		m_edtGroupName.SetFocus();
//		m_List.SetItemState(m_List.GetItemCount()-1,  LVIS_FOCUSED|LVIS_SELECTED , LVIS_FOCUSED|LVIS_SELECTED); 
	}
}

void CTpscGroupDlg::OnTmTpscGroupModify() 
{
	UpdateData(TRUE);
	CArray<int,int> aSel;
	int nSelCount = GetCurrentRows(aSel);

	if(nSelCount < 1)
	{
	  GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Group_Name_was_not_selected));
		return;
	}

	if(nSelCount > 1)
	{
	  //GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Group_Name_was_not_selected));
		return;
	}

	CString strGroupName;
	CString strOldGroupName;
	CString strElemList;
	T_TSGR_D TsgrOldData;

	m_edtGroupName.GetWindowText(strGroupName);
	m_edtElemList.GetWindowText(strElemList);

	m_TsgrData.Initialize();

	m_edtElemList.SelectByStr(strElemList);

	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->GetSelectedElemKeyList(m_TsgrData.aElemList);

	if(!m_TsgrData.aElemList.GetSize())
	{
		GSaveHistoryNF(_LS(IDS_WG_TREEMENU_No_selected_element___));
		return;
	}

	//MQC::2785-JHYUN-20090731
	T_ELEM_D ElemData;
	T_SECT_D SectData;

	for(int i=0; i<m_TsgrData.aElemList.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl->GetElem(m_TsgrData.aElemList[i], ElemData)) { ASSERT(0); return; }
		if(!m_pDoc->m_pAttrCtrl->GetSect(ElemData.elpro, SectData)) { ASSERT(0); return; }

		if(SectData.nStype != D_SECT_TYPE_TAPERED)
		{
			GSaveHistoryNF(_LS(IDS_TM_TEPERED_SECTION_GROUP_TYPE_NOT_TAPERED));
			return;
		}
	}

	m_TsgrData.GroupName = strGroupName;
	m_TsgrData.nVariation_y = m_nYAxisVariType;
	m_TsgrData.nQuadEnd_y   = m_nYAxisSymPln;
	m_TsgrData.nVariation_z = m_nZAxisVariType;
	m_TsgrData.nQuadEnd_z   = m_nZAxisSymPln;
	m_TsgrData.dQuadDistance_y = m_dYAxisDist;
	m_TsgrData.dQuadDistance_z = m_dZAxisDist;

	if(m_nYAxisVariType == 1)
	{
		m_TsgrData.dQuadExp_y = m_edtYQuad.GetEditValue();
	}
	
	if(m_nZAxisVariType == 1)
	{
		m_TsgrData.dQuadExp_z = m_edtZQuad.GetEditValue();
	}

	
	m_pDoc->m_pAttrCtrl->GetTsgr(GetCurrentTsgrKey(), TsgrOldData);

	m_TsgrData.GroupId = TsgrOldData.GroupId;

	strOldGroupName = GetCurrentTsgrKeyName();

	if(m_pDoc->m_pDataCtrl->ModifyTsgr(strOldGroupName, m_TsgrData))
	{
		m_edtGroupName.SetFocus();
		m_List.SetItemState(GetCurrentRow(),  LVIS_FOCUSED|LVIS_SELECTED , LVIS_FOCUSED|LVIS_SELECTED); 
	}
}

void CTpscGroupDlg::OnTmTpscGroupDelete() 
{
	CArray<int,int> aSel;
	CArray<CString,CString&> aSelStrings;
	int nSelCount = GetCurrentRows(aSel);

	if(nSelCount < 1)
	{
	  GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Group_Name_was_not_selected));
		return;
	}


	for(int i=0; i<nSelCount; i++)
	{
		int nSelect = aSel[i];
		aSelStrings.Add(m_List.GetItemText(nSelect, 0));

		if(!m_List.SetItemState(nSelect, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED))
			if(!m_List.SetItemState(nSelect-1, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED))
			{
				m_edtGroupName.Clear();
				m_pDoc->m_pViewCtrl->UnselectAll(NULL);
				m_List.SetItemState(-1, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
			}
	}
	m_pDoc->m_pDataCtrl->DelTsgr(aSelStrings);

}

void CTpscGroupDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
	 	InitTsgrData();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		InitTsgrData();
		break; 
	case D_UPDATE_UNIT:
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}


void CTpscGroupDlg::OnItemchangedTmTpscGroupList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	CString strTemp;

	int nSelect = GetCurrentRow();

	m_TsgrKey = m_pDoc->m_pAttrCtrl->GetTsgrKey(m_List.GetItemText(nSelect, 0));
	if(m_pDoc->m_pAttrCtrl->GetTsgr(m_TsgrKey, m_TsgrData))
	{
		m_edtGroupName.SetWindowText(m_TsgrData.GroupName);
		m_pDoc->m_pViewCtrl->UnselectAll(NULL);

	  CNumericOptimizer optimizer;
	  strTemp = optimizer.Optimize((long*)m_TsgrData.aElemList.GetData(),m_TsgrData.aElemList.GetSize());
	  m_edtElemList.SelectByStr(strTemp);	

		
		m_nYAxisVariType  = m_TsgrData.nVariation_y;
		m_nYAxisSymPln    = m_TsgrData.nQuadEnd_y;
		m_nZAxisVariType  = m_TsgrData.nVariation_z;
		m_nZAxisSymPln    = m_TsgrData.nQuadEnd_z;
		m_dYAxisDist      = m_TsgrData.dQuadDistance_y;
		m_dZAxisDist      = m_TsgrData.dQuadDistance_z;
		m_edtYQuad.SetEditUnit(m_TsgrData.dQuadExp_y, 1);
		m_edtZQuad.SetEditUnit(m_TsgrData.dQuadExp_z, 1);

		UpdateData(FALSE);

		OnTmTpscYaxisVarType();
		OnTmTpscZaxisVarType();

		
	}	

	*pResult = 0;
}

void CTpscGroupDlg::OnChangeTmTpscGroupLoadlistEdit() 
{
/*	CString strTemp;
	m_edtElemList.GetWindowText(strTemp);
	m_edtElemList.SelectByStr(strTemp);  */
}

int CTpscGroupDlg::GetCurrentTsgrKey()
{
	int nSelect;

	CString strGroupName;

	nSelect = GetCurrentRow();

	strGroupName = m_List.GetItemText(nSelect, 0);

	return m_pDoc->m_pAttrCtrl->GetTsgrKey(strGroupName);		
}

CString CTpscGroupDlg::GetCurrentTsgrKeyName()
{
	int nSelect;

	CString strGroupName;

	nSelect = GetCurrentRow();

	strGroupName = m_List.GetItemText(nSelect, 0);

	return strGroupName;		
}

int CTpscGroupDlg::GetCurrentRow()
{
	int nSelect = -1;
	LV_ITEM lvitem;
	int count = m_List.GetItemCount();
	lvitem.mask = LVIF_STATE;

	for(int i=count-1 ; i>=0; i--)
	{
		lvitem.iItem = i;
		if(m_List.GetItemState(i, LVIS_SELECTED) != 0)
		{
			nSelect = i;
			break;
		}
	}
	return nSelect;
}

int CTpscGroupDlg::GetCurrentRows(CArray<int,int>& aSelectedRows)
{// 거꾸로 저장한다. -> 지울때는 그대로 써야한다.
	int count = m_List.GetItemCount();
	aSelectedRows.RemoveAll();
	int nSelCount = 0;
	for(int i=count-1 ; i>=0; i--)
	{
		if(m_List.GetItemState(i, LVIS_SELECTED) != 0)
		{
			aSelectedRows.Add(i);
			nSelCount++;
		}
	}
	return nSelCount;
}


void CTpscGroupDlg::OnDestroy() 
{
	CMenuBarChildDlg::OnDestroy();
	
	// TODO: Add your message handler code here
}

BOOL CTpscGroupDlg::ExternalInit(UINT key)
{
	if(key == 0) return FALSE;

	T_TSGR_D data;
	CString strTemp;

	m_pDoc->m_pAttrCtrl->GetTsgr(key, data);

	m_edtGroupName.SetWindowText(data.GroupName);
	m_pDoc->m_pViewCtrl->UnselectAll(NULL);

	CNumericOptimizer optimizer;
	strTemp = optimizer.Optimize((long*)data.aElemList.GetData(), data.aElemList.GetSize());
	m_edtElemList.SelectByStr(strTemp);

	return TRUE;
}

void CTpscGroupDlg::OnTmTpscYaxisVarType() 
{
	UpdateData(TRUE);
	
	GetDlgItem(IDC_TM_TPSC_YAXIS_SYMPL_I)->EnableWindow(m_nYAxisVariType == 1);
	GetDlgItem(IDC_TM_TPSC_YAXIS_SYMPL_J)->EnableWindow(m_nYAxisVariType == 1);
	GetDlgItem(IDC_TM_TPSC_YAXIS_DIST)->EnableWindow(m_nYAxisVariType == 1);
	YAxisVarCtrlMan();
}

void CTpscGroupDlg::OnTmTpscZaxisVarType() 
{
	UpdateData(TRUE);

	GetDlgItem(IDC_TM_TPSC_ZAXIS_SYMPL_I)->EnableWindow(m_nZAxisVariType == 1);
	GetDlgItem(IDC_TM_TPSC_ZAXIS_SYMPL_J)->EnableWindow(m_nZAxisVariType == 1);
	GetDlgItem(IDC_TM_TPSC_ZAXIS_DIST)->EnableWindow(m_nZAxisVariType == 1);
	ZAxisVarCtrlMan();
}

void CTpscGroupDlg::ZAxisVarCtrlMan()
{
	CArray<UINT,UINT> Ctrls;
	Ctrls.Add(IDC_CMD_ZQUAD_EDIT);
	//Ctrls.Add(IDC_CMD_ZQUAD_SPIN);
	CDlgUtil::CtrlEnableDisable(this,Ctrls,this->m_nZAxisVariType == 1);
}

void CTpscGroupDlg::YAxisVarCtrlMan()
{
	CArray<UINT,UINT> Ctrls;
	Ctrls.Add(IDC_CMD_YQUAD_EDIT);
	//Ctrls.Add(IDC_CMD_YQUAD_SPIN);
	CDlgUtil::CtrlEnableDisable(this,Ctrls,this->m_nYAxisVariType == 1);
}

void CTpscGroupDlg::OnDeltaposCmdYquadSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CDlgUtil::SetFloatSpinValue(this,IDC_CMD_YQUAD_EDIT,_T("1.0"),_T("10.0"),0.1,pNMHDR,pResult);
}

void CTpscGroupDlg::OnDeltaposCmdZquadSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CDlgUtil::SetFloatSpinValue(this,IDC_CMD_ZQUAD_EDIT,_T("-10.0"),_T("10.0"),0.1,pNMHDR,pResult);
}

void CTpscGroupDlg::OnTmTpscGroupConvert() 
{
	CArray<int,int> aSel;
	CArray<CString,CString&> aSelStrings;
	int nSelCount = GetCurrentRows(aSel);
	if(nSelCount < 1)
	{
	  GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Group_Name_was_not_selected));
		return;
	}

	for(int i=0; i<nSelCount; i++)
		aSelStrings.Add(m_List.GetItemText(aSel[i], 0));
	
	CTpsgConvertDlg dlg;
	dlg.SetSelStrings(aSelStrings);
	if(IDOK == dlg.DoModal())
	{
		m_pDoc->m_pViewCtrl->UnselectAll(NULL);
	}
}


