// CMSectionPage.cpp : implementation file
//
#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlModifyEffSectPropDlg.h"
#include "DgnStlModifyEffSectPropGrid.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"

#include "..\wg_db\wg_db_SectUtil.h"

#include "..\wg_base\wg_base_CompFunc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COLCOUNT 4


/////////////////////////////////////////////////////////////////////////////
// CDgnStlModifyEffSectPropDlg dialog

CDgnStlModifyEffSectPropDlg::CDgnStlModifyEffSectPropDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnStlModifyEffSectPropDlg::IDD, pParent)
{
	m_Key = NULL;
	m_Data.Initialize();

	m_pSectULSGrid = NULL;
	//m_pSectSLSGrid = NULL;
}

void CDgnStlModifyEffSectPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnStlModifyEffSectPropDlg)
	DDX_Control(pDX, IDC_DGN_STL_MODIFY_EFFSECTPROP_LIST, m_List);
	DDX_Control(pDX, IDC_DGN_STL_MODIFY_EFFSECTPROP_DESIGN_ULT_GRID, *m_pSectULSGrid);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CDgnStlModifyEffSectPropDlg implementation functions
#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnStlModifyEffSectPropDlg, CInternationalDlg)
#undef CDialog
//{{AFX_MSG_MAP(CDgnStlCodeDlg)
ON_NOTIFY(NM_CLICK,        IDC_DGN_STL_MODIFY_EFFSECTPROP_LIST, OnClickList)
ON_NOTIFY(LVN_ITEMCHANGED, IDC_DGN_STL_MODIFY_EFFSECTPROP_LIST, OnItemChanged)

ON_BN_CLICKED(IDC_DGN_STL_MODIFY_EFFSECTPROP_DESIGN_MODIFY_BTN, OnModify)
ON_BN_CLICKED(IDC_DGN_STL_MODIFY_EFFSECTPROP_DESIGN_CLOSE_BTN,  OnCancel)

//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDgnStlModifyEffSectPropDlg message handlers

BOOL CDgnStlModifyEffSectPropDlg::OnInitDialog() 
{
	if (m_pSectULSGrid == nullptr)
		m_pSectULSGrid = new CDgnStlModifyEffSectPropULSGrid(this);

	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();
	
	// Grid
	InitGridData();
	SetData();

	// List
	Initial_ListCtrl();
	Initial_Data();
	
	return TRUE;
}

void CDgnStlModifyEffSectPropDlg::PostNcDestroy()
{
	if(m_pSectULSGrid != NULL)     { delete m_pSectULSGrid;  m_pSectULSGrid = NULL; }
	//if(m_pSectSLSGrid != NULL)     { delete m_pSectSLSGrid;  m_pSectSLSGrid = NULL; }

	CDialogMove::PostNcDestroy();
}



void CDgnStlModifyEffSectPropDlg::InitGridData()
{
	if (m_pSectULSGrid != NULL)
	{
		m_pSectULSGrid->InitGrid();   
	}

//   if (m_pSectSLSGrid == NULL)
//   {
//     m_pSectSLSGrid = new CDgnStlModifyEffSectPropSLSGrid(this); 
//     m_pSectSLSGrid->SubclassDlgItem(IDC_DGN_STL_MODIFY_EFFSECTPROP_DESIGN_SERV_GRID, this);
//     m_pSectSLSGrid->InitGrid();   
//   }
}



void CDgnStlModifyEffSectPropDlg::Initial_ListCtrl()
{
//  CString strCoating = _T("");
//   CString strUnit;
//   m_pDoc->m_pUnitCtrl->GetUnitSystem(D_UNITSYS_BASE_LENGTH, strUnit);
//   strCoating.Format(_T("%s(%s)"), _LS(IDS_CMD_SECTION_COATING), strUnit);

	CString aTitle[] = { _LS(IDS_CMD_SECTION_id), _LS(IDS_CMD_SECTION_name),_LS(IDS_CMD_SECTION_type), _LS(IDS_CMD_SECTION_shape)};

	int nColWidth[COLCOUNT];
	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	// Calculate width for each column
	nColWidth[0] = 40; nColWidth[1] = 130; nColWidth[2] = 60; nColWidth[3] = 50;

	// Set Title
	for(i = 0; i < COLCOUNT; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		if (i == 0) lvcolumn.fmt = LVCFMT_RIGHT;
		else lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i,&lvcolumn);
		m_List.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}


void CDgnStlModifyEffSectPropDlg::Initial_Data()
{
	CWaitCursor Cursor;

	m_List.DeleteAllItems();
	if(m_pDoc->m_pAttrCtrl->GetCountSect() == 0) return;

	LV_ITEM lvitem;
	POSITION pos;
	CString str;
	int nItemCount=0;

	CString strCfsCodeName = CDBLib::GetCfsCodeName();

	CMapEx<CString,LPCTSTR,int,int> mEnableSect; mEnableSect.RemoveAll();
	mEnableSect.InitHashTable(2);
	if(strCfsCodeName==_T("Eurocode3-1-3:06"))
	{
//     mEnableSect.SetAt(D_SECT_SHAPE_REG_CC, 0);
		mEnableSect.SetAt(D_SECT_SHAPE_REG_UP, 0);
	}

	T_SECT_K Key;
	T_SECT_D Data;

	std::vector<T_SECT_K> aSectK; aSectK.clear();
	pos = m_pDoc->m_pAttrCtrl->GetStartSectDesign();
	while(pos !=NULL)
	{
		m_pDoc->m_pAttrCtrl->GetNextSectDesign(pos, Key, Data);
		if(!mEnableSect.Exist(Data.SectBefore.Shape)) continue;
		aSectK.push_back(Key);
		nItemCount++;
	}
	qsort(aSectK.data(), nItemCount, sizeof(T_SECT_K), CCompFunc::UINTAsc);

	int ix = 0;
	for(std::vector<T_SECT_K>::iterator IterPos=aSectK.begin(); IterPos!=aSectK.end(); ++IterPos)
	{
		Key = *IterPos;
		if(!m_pDoc->m_pAttrCtrl->GetSectDesign(Key, Data)) { ASSERT(0); continue; }

		for(int i = 0; i < COLCOUNT; i++)
		{
			lvitem.iItem=ix;
			lvitem.iSubItem=i;
			str = DataToStr(i, Key, Data);
			lvitem.pszText=str.GetBuffer(0);
			lvitem.mask=LVIF_TEXT ;
			if( i == 0 )
				m_List.InsertItem(&lvitem);
			else
				m_List.SetItem(&lvitem);
			str.ReleaseBuffer();
		}
		ix++;
	}

	if(ix>0) m_List.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

CString CDgnStlModifyEffSectPropDlg::DataToStr(int i, T_SECT_K Key, T_SECT_D &Data)
{
	CString str;

	if(i==0) str.Format(_T("%6d"),Key);
	else if(i==1) str = Data.SName;
	else if(i==2) 
	{
		switch(Data.nStype)
		{
		case D_SECT_TYPE_REGULAR: 
			{
				if (Data.SectBefore.SectI.DBName == _T("")) str = _LS(IDS_WG_CMD__ADD2__User);
				else str =  _LS(IDS_WG_CMD__ADD2__DB);
			}
			break;
		case D_SECT_TYPE_USER:           str = _LS(IDS_WG_CMD__ADDD__Value);       break;
		case D_SECT_TYPE_SRC:            str = _LS(IDS_WG_CMD__ADDD_SECT_SRC);     break;
		case D_SECT_TYPE_COMBINED:       str = _LS(IDS_WG_CMD__ADDD__Combined);    break;
		case D_SECT_TYPE_TAPERED:        str = _LS(IDS_WG_CMD__ADDD__Tapered);     break;
		case D_SECT_TYPE_CONSTRUCTION:   str = _LS(IDS_WG_CMD__ADDD__Composite);   break;
		case D_SECT_TYPE_HIBEAM:         str = _LS(IDS_WG_CMD__ADDD__Hi_Beam);     break;
		case D_SECT_TYPE_COLDFORMED:     str = _LS(IDS_WG_CMD__ADDD__Cold_Formed); break;
		case D_SECT_TYPE_COMPO_B:        str = _LS(IDS_WG_CMD__ADDD__Composite);   break;
		case D_SECT_TYPE_COMPO_I:        str = _LS(IDS_WG_CMD__ADDD__Composite);   break;
		case D_SECT_TYPE_COMPO_TUB:      str = _LS(IDS_WG_CMD__ADDD__Composite);   break;
		case D_SECT_TYPE_COMPO_STLG_B:   str = _LS(IDS_WG_CMD__ADDD__Composite);   break;
		case D_SECT_TYPE_COMPO_STLG_I:   str = _LS(IDS_WG_CMD__ADDD__Composite);   break;
		case D_SECT_TYPE_COMPO_STLG_TUB: str = _LS(IDS_WG_CMD__ADDD__Composite);   break;
		case D_SECT_TYPE_COMPO_CI:       str = _LS(IDS_WG_CMD__ADDD__Composite);   break;
		case D_SECT_TYPE_COMPO_CT:       str = _LS(IDS_WG_CMD__ADDD__Composite);   break;
		case D_SECT_TYPE_COMPO_PC:       str = _LS(IDS_WG_CMD__ADDD__Composite);   break;
		case D_SECT_TYPE_COMPO_G:        str = _LS(IDS_WG_CMD__ADDD__Composite);   break;
		case D_SECT_TYPE_PSC:            str = _LS(IDS_WG_CMD__ADDD__PSC);         break;
		case D_SECT_TYPE_STLG_B:         str = _LS(IDS_WG_CMD__ADDD__STLGIRDER);   break;
		case D_SECT_TYPE_STLG_I:         str = _LS(IDS_WG_CMD__ADDD__STLGIRDER);   break;
		case D_SECT_TYPE_STLG_MCELL:     str = _LS(IDS_WG_CMD__ADDD__STLGIRDER);   break;
		}
	}
	else if(i==3) 
	{
		str = Data.SectBefore.Shape;
		CSectUtil::CustomizeShapeNameByLanguage(Data.nStype, str, FALSE);
	}
	else str = _LS(IDS_WG_CMD__ADDD__Error);

	return str;
}


void CDgnStlModifyEffSectPropDlg::SetData()
{
	// 그리드에 데이터 세팅하기
	T_SECT_STIFFNESS_CF StiffnessCF = m_Data.SectBefore.SectI.StiffnessCF;

	//ULS
	T_SECT_STIFFNESS_CF_BASE  CFStiffULS; CFStiffULS.Initialize();
	CFStiffULS = StiffnessCF.CFStiffULS;
	m_pSectULSGrid->SetData(&CFStiffULS);
	
	//SLS
//   T_SECT_STIFFNESS_CF_BASE  CFStiffSLS; CFStiffSLS.Initialize();
//   CFStiffSLS = StiffnessCF.CFStiffSLS;
//   m_pSectSLSGrid->SetData(&CFStiffSLS);

}

BOOL CDgnStlModifyEffSectPropDlg::Dlg2Data()
{
	T_SECT_STIFFNESS_CF_BASE CFStiffULS; CFStiffULS.Initialize();
	T_SECT_STIFFNESS_CF_BASE CFStiffSLS; CFStiffSLS.Initialize();
	m_pSectULSGrid->GetData(&CFStiffULS);
	//m_pSectSLSGrid->GetData(&CFStiffSLS);

	// data Check
	if(!DataCheck(CFStiffULS))  return FALSE;

	m_Data.SectBefore.SectI.StiffnessCF.CFStiffULS =  CFStiffULS;
	//m_Data.SectBefore.SectI.StiffnessCF.CFStiffSLS =  CFStiffSLS;

	return TRUE;
}

BOOL CDgnStlModifyEffSectPropDlg::DataCheck(T_SECT_STIFFNESS_CF_BASE& CFStiffULS)
{
	if(CFStiffULS.dA_eff  <=0.0) {AfxMessageBox(_T("A_eff must be greater than zero")); return FALSE;}
	if(CFStiffULS.dA_net <=0.0) {AfxMessageBox(_T("A_net must be greater than zero")); return FALSE;}
	if(CFStiffULS.dIy_eff <=0.0) {AfxMessageBox(_T("Iy_eff must be greater than zero")); return FALSE;}
	if(CFStiffULS.dIz_eff <=0.0) {AfxMessageBox(_T("Iz_eff must be greater than zero")); return FALSE;}
	if(CFStiffULS.dWy_eff <=0.0) {AfxMessageBox(_T("Wy_eff must be greater than zero")); return FALSE;}
	if(CFStiffULS.dWz_eff <=0.0) {AfxMessageBox(_T("Wz_eff must be greater than zero")); return FALSE;}
	if(CFStiffULS.dIt_eff <=0.0) {AfxMessageBox(_T("It_eff must be greater than zero")); return FALSE;}
	if(CFStiffULS.dIw_eff <=0.0) {AfxMessageBox(_T("Iw_eff must be greater than zero")); return FALSE;}

	return TRUE;
}

void CDgnStlModifyEffSectPropDlg::OnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnItemChanged(pNMHDR,pResult);
	*pResult = 0;
}

void CDgnStlModifyEffSectPropDlg::OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nItem = pNMListView->iItem;
	CString strID = m_List.GetItemText(nItem,0);
	m_Key = _ttoi(strID);

	// TODO: Add your control notification handler code here
	if (m_Key == NULL) 
		m_Data.Initialize();
	else
	{
		if (!m_pDoc->m_pAttrCtrl->GetSectDesign(m_Key, m_Data)) return;
		//if (!m_pDoc->m_pAttrCtrl->GetSect(m_Key, m_Data)) return;
	}

	if (m_pSectULSGrid != NULL/* && m_pSectSLSGrid != NULL*/)
	{
		InitGridData();
	}
	SetData(); // 그리드에 데이터 보여주기

	*pResult = 0;
}



void CDgnStlModifyEffSectPropDlg::OnModify() 
{
	int nItem = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nItem == -1) 
	{
		AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_SECT),MB_OK);
		return;
	}

	CString strName=_T(""), strMatName=_T("");
	CString strID = m_List.GetItemText(nItem, 0);
	m_Key = _ttoi(strID);
	T_SECT_D rData;
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(m_Key, m_Data)) return;
	//if (!m_pDoc->m_pAttrCtrl->GetSect(m_Key, rData)) return;

	UpdateData(TRUE);

	if(!Dlg2Data()) return; // m_Data 채움.

	//if(bCheck)
	{
		BOOL bDiffType = TRUE;
		// if(m_Data.nFormingType != nSelType) bDiffType = TRUE;  // 달라진것 검토?

		if( bDiffType)
		{
			CArray<T_SECT_K, T_SECT_K> aKey;
			CArray<T_SECT_D, T_SECT_D&> aData;
			aKey.Add(m_Key);
			aData.Add(m_Data);
			//BOOL bCheck = m_pDoc->m_pDataCtrl->AddSect(aKey, aData, TRUE);
			BOOL bCheck = m_pDoc->m_pDataCtrl->AddSecd(aKey, aData, CMDTYPE_REMOVE_CFSTEEL);

		}
	}
}

void CDgnStlModifyEffSectPropDlg::OnCancel() 
{
	CDialogMove::OnCancel();
}

