// CDgnStlTransverseStfnDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlTransverseStfnDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\DataCtrl.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_db\ViewBuff.h"
#include "..\wg_db\ViewCtrl.h"
#include "..\wg_db\PostCtrl.h"
#include "..\wg_db\SectUtil.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\DBLib.h"
#include "..\wg_db\MatlDB.h"

#include "..\wg_main\wg_mainRes2.h"

#include "DgnDataCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnStlTransverseStfnDlg dialog


CDgnStlTransverseStfnDlg::CDgnStlTransverseStfnDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnStlTransverseStfnDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnStlTransverseStfnDlg)

	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnStlTransverseStfnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnStlTransverseStfnDlg)
	DDX_Control(pDX, IDC_STL_TVSF_BTN_DEL, m_btnDel);
	DDX_Control(pDX, IDOK, m_btnAdd);
	DDX_Control(pDX, IDC_TRAN_STFN_LIST, m_wndList);

	DDX_Control(pDX, IDC_STL_TVSF_CHECK_SAME, m_chkSameTvsf);
	DDX_Control(pDX, IDC_TVSF_EDIT_ht_i, m_ht_i);
	DDX_Control(pDX, IDC_TVSF_EDIT_t_i, m_t_i);
	DDX_Control(pDX, IDC_TVSF_EDIT_pitch_i, m_Pitch_i);
	DDX_Control(pDX, IDC_STL_TVSF_ht_unit_i, m_ht_i_Unit);
	DDX_Control(pDX, IDC_STL_TVSF_t_unit_i, m_t_i_Unit);
	DDX_Control(pDX, IDC_STL_TVSF_pitch_unit_i, m_Pitch_i_Unit);
	 //
	DDX_Control(pDX, IDC_TVSF_EDIT_ht_m, m_ht_m);
	DDX_Control(pDX, IDC_TVSF_EDIT_t_m, m_t_m);
	DDX_Control(pDX, IDC_TVSF_EDIT_pitch_m, m_Pitch_m);
	DDX_Control(pDX, IDC_STL_TVSF_ht_unit_m, m_ht_m_Unit);
	DDX_Control(pDX, IDC_STL_TVSF_t_unit_m, m_t_m_Unit);
	DDX_Control(pDX, IDC_STL_TVSF_pitch_unit_m, m_Pitch_m_Unit);
	 //
	DDX_Control(pDX, IDC_TVSF_EDIT_ht_j, m_ht_j);
	DDX_Control(pDX, IDC_TVSF_EDIT_t_j, m_t_j);
	DDX_Control(pDX, IDC_TVSF_EDIT_pitch_j, m_Pitch_j);
	DDX_Control(pDX, IDC_STL_TVSF_ht_unit_j, m_ht_j_Unit);
	DDX_Control(pDX, IDC_STL_TVSF_t_unit_j, m_t_j_Unit);
	DDX_Control(pDX, IDC_STL_TVSF_pitch_unit_j, m_Pitch_j_Unit);

	DDX_Control(pDX, IDC_EDIT_SECT_SHAPE_SIZE, m_editTip);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnStlTransverseStfnDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnStlTransverseStfnDlg)
	ON_BN_CLICKED(IDC_DGN_DESIGN_SECTION, OnDgnDesignSection)
	ON_BN_CLICKED(IDC_STL_TVSF_BTN_DEL, OnDgnDelete)
	ON_BN_CLICKED(IDC_STL_TVSF_CHECK_SAME, OnSameStiffener)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TRAN_STFN_LIST, OnItemchangedDgnSectList)	
	ON_NOTIFY(NM_CLICK, IDC_TRAN_STFN_LIST, OnClickDgnSectList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnStlTransverseStfnDlg message handlers

//---------------------------------------------------------------------------
// Initialize Unit
//---------------------------------------------------------------------------
void CDgnStlTransverseStfnDlg::SetInitUnit()
{
	m_ht_i.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_t_i.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Pitch_i.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_ht_m.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_t_m.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Pitch_m.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_ht_j.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_t_j.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Pitch_j.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_ht_i_Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_t_i_Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Pitch_i_Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_ht_m_Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_t_m_Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Pitch_m_Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_ht_j_Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_t_j_Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Pitch_j_Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

BOOL CDgnStlTransverseStfnDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here

//#if defined(_ORG)
//	SetWindowText(_LS(IDS_DGN_CON_REIN_BEAM_DLG_org));
//#else
//	SetWindowText(_LS(IDS_DGN_CON_REIN_BEAM_DLG));
//#endif


	m_bModify = FALSE;
	//for(int i=0; i<3; i++) bExistData[i]=TRUE;

	SetHeaderTitle();
	MakeItemEx();
	//InitTabCtrl();  
	
	// 좌측 Section Data Control.
	SetInitUnit();
	Data2Dlg();

	ChangeDlgItemInfo();
	
	//  Init View Window
	CWnd* PlaceHolder = GetDlgItem(IDC_STL_TRANSTFN_PREVIEW); ASSERT(PlaceHolder);
	//m_SectWnd.Initial(PlaceHolder);
	m_wndSecView.Init(PlaceHolder);
	m_wndSecView.SetCentroidFlag(TRUE);
	
	m_btnAdd.EnableWindow(FALSE);
	m_btnDel.EnableWindow(FALSE);

	if (m_bModify)
	{
		int nItemCount = m_wndList.GetItemCount();
        int ix = 0;
		for (ix = 0; ix < nItemCount; ix++)
			if (m_TvsfKey == m_wndList.GetItemData(ix)) break;
		if (ix != nItemCount)
		{
			m_wndList.SetItemState(ix, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
			m_btnAdd.EnableWindow(TRUE);
			m_btnDel.EnableWindow(TRUE);
			//m_btnRedraw.EnableWindow(TRUE);
		}
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnStlTransverseStfnDlg::OnDgnDesignSection() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND,MAKEWPARAM(ID_STRUCT_PROP_SECT4DGN,0));	
	
}

void CDgnStlTransverseStfnDlg::OnSameStiffener()
{
	m_Data.bSameIMJ = m_chkSameTvsf.GetCheck();
	ChangeDlgItemInfo();
}

// add/replace
void CDgnStlTransverseStfnDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	// get data

	UpdateData(TRUE);
	// 저장하기 전 Data의 유효성 여부 Check - Data 저장
	// Grid in Tab.
	CArray<T_TVSF_K, T_TVSF_K>  arKey;
	CArray<T_TVSF_D, T_TVSF_D&> arTvsfData;
	int nItem = -1;
	int nSelCount = m_wndList.GetSelectedCount();
	arKey.SetSize(nSelCount);
	arTvsfData.SetSize(nSelCount);
	for(int i=0; i< nSelCount; i++)
	{
		nItem = m_wndList.GetNextItem(nItem, LVNI_SELECTED);
		ASSERT(nItem != -1);
		arKey[i]   = m_wndList.GetItemData(nItem);
		m_arTvsfKey.SetAt(i,m_wndList.GetItemData(nItem));
	}

	if(!SelectSection(arKey))
	{
		AfxMessageBox(_LS(IDS_DGN_CON_SELECT_DiffShpe_Err));
		return;
	}

 
	//if(!CheckInputData()) return;
	Dlg2Data();

	for (int i=0; i < nSelCount; i++)
	{
		arTvsfData[i] = m_Data;   
	}

	if (!m_pDoc->m_pDataCtrl->AddTvsf(arKey, arTvsfData))
		return;  
	
	MakeItemEx();

	int nIndex = m_wndList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	ChangeItem(nIndex);
	
	//CDialogMove::OnOK();
}

// close
void CDgnStlTransverseStfnDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}

void CDgnStlTransverseStfnDlg::OnDgnDelete() 
{
	// TODO: Add your control notification handler code here
	T_TVSF_K key;
	CArray<T_TVSF_K, T_TVSF_K> aKey;
	int nItem = -1;

	int nSelCount = m_wndList.GetSelectedCount();
	for (UINT i=0; i < nSelCount; i++)
	{
		nItem = m_wndList.GetNextItem(nItem, LVNI_SELECTED);
		ASSERT(nItem != -1);
		key = m_wndList.GetItemData(nItem);
		if (m_pDoc->m_pAttrCtrl2->ExistTvsf(key)) aKey.Add(key);
	}

	if (!m_pDoc->m_pDataCtrl->DelTvsf(aKey)) return;
	 
	MakeItemEx();
	int nIndex = m_wndList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	ChangeItem(nIndex);
}

void CDgnStlTransverseStfnDlg::OnClickDgnSectList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_wndList.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	m_btnAdd.EnableWindow(nIndex != -1);
	m_btnDel.EnableWindow(nIndex != -1);
	//m_btnRedraw.EnableWindow(nIndex !=-1);
	*pResult = 0;
}

void CDgnStlTransverseStfnDlg::OnItemchangedDgnSectList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	int nIndex = pNMListView->iItem; 
	ChangeItem(nIndex);
	*pResult = 0;
}


void CDgnStlTransverseStfnDlg::ChangeItem(int nIndex)
{
	// enable/disable
	m_btnAdd.EnableWindow(nIndex != -1);
	m_btnDel.EnableWindow(nIndex != -1);
	//m_btnRedraw.EnableWindow(nIndex != -1);

//  for(int i=0; i<3; i++)
//    m_nStiCBox[i] = -1;
//  m_cStirrupSize.SetCurSel(m_nStiCBox[m_nIMJ]);

	// get data
	m_TvsfKey = 0;
	m_Data.Initialize();
	m_SectD.Initialize();  
	if (nIndex != -1)
	{
	  m_TvsfKey = m_wndList.GetItemData(nIndex);
		if (!m_pDoc->m_pAttrCtrl2->GetTvsf(m_TvsfKey, m_Data))
			m_Data.Initialize();
		m_pDoc->m_pAttrCtrl->GetSect(m_TvsfKey, m_SectD);
	}

	ChangeDlgItemInfo();

	Data2Dlg();

	ShowSizeTip(m_SectD);
	
//  for( i=0; i<3; i++)
//    m_nStiCBox[i] = m_cStirrupSize.SelectString(-1,m_Data.BEAM.strSubBarNa[i]);
	
	m_wndSecView.SetDataSource(&m_SectD);
	m_wndSecView.Invalidate();
	
	//m_SectWnd.Draw_Shape(0, m_TvsfKey, FALSE, TRUE);
	//m_SectWnd.Draw_ChkRebar(m_TvsfKey, iTab, m_Data, FALSE, TRUE);
	//m_SectWnd.Invalidate();
}

void CDgnStlTransverseStfnDlg::SetHeaderTitle()
{
	CStringArray aTitle;
	CArray<int, int> aWidth;

#define M_ADD_LIST_INFO(TITLE, WIDTH) aTitle.Add(TITLE); aWidth.Add(WIDTH)
	M_ADD_LIST_INFO(_LS(IDS_DGN_LIST_ID),     35);  //_T("ID")   
	M_ADD_LIST_INFO(_LS(IDS_DGN_LIST_NAME),   90);  //_T("Name")
	M_ADD_LIST_INFO(_LS(IDS_DGN_GRID_I_NODE), 55);  //_T("i_Node")
	M_ADD_LIST_INFO(_LS(IDS_DGN_GRID_MIDDLE), 55);  //_T("middle")
	M_ADD_LIST_INFO(_LS(IDS_DGN_GRID_J_NODE), 55);  //_T("j_Node")
#undef M_ADD_LIST_INFO

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_wndList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_wndList.GetSafeHwnd(), dwStyle);

	// Set Title
	int nNum = aTitle.GetSize();
	for (i=0; i < nNum; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(aWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_wndList.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CDgnStlTransverseStfnDlg::MakeItemEx()
{
	CWaitCursor Cursor;

	m_wndList.DeleteAllItems();

	// Data Sort & Store
	if(m_arBeamData.GetCount()>0) m_arBeamData.RemoveAll();

	CArray<T_ELEM_K, T_ELEM_K> arElemKeyList;
	CMap<int,int,int,int> ColmSecMap; ColmSecMap.RemoveAll();

	m_pDoc->m_pAttrCtrl->GetElemKeyList(arElemKeyList);
	int nCount = arElemKeyList.GetSize();

	for(int i=0; i<nCount; i++)
	{
		T_ELEM_D ElemData; ElemData.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetElem(arElemKeyList[i], ElemData))	ASSERT(0);

		T_MATD_D MatdData; MatdData.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(ElemData.elmat, MatdData))	continue;

		if(MatdData.Type == _T("S"))
		{
			// 1=column, 2=beam, 3=brace, 0=기타
	    int nMembType = m_pDoc->m_pAttrCtrl->GetMemberType(arElemKeyList[i]);

			T_SECT_D SectData; SectData.Initialize();
			BOOL bCheck = m_pDoc->m_pAttrCtrl->GetSectDesign(ElemData.elpro,SectData);
			if(bCheck) 
			{
				CString strSectShape;
				if(SectData.nStype == D_SECT_TYPE_REGULAR)                  strSectShape = SectData.SectBefore.Shape;
				else if(SectData.nStype == D_SECT_TYPE_TAPERED && 
								SectData.SectBefore.nStype == D_SECT_TYPE_REGULAR)  strSectShape = SectData.SectBefore.SectI.Shape;
				else if ( SectData.nStype == D_SECT_TYPE_STLG_B || SectData.nStype == D_SECT_TYPE_STLG_I )
					strSectShape = SectData.SectBefore.Shape;

				BOOL bIsSectShapeOK = CDBLib::IsSectShape4TranserseStfn(strSectShape);
				if(bIsSectShapeOK) m_arBeamData.SetAt(ElemData.elpro,ElemData.elpro);           
			}
		}
	}

	T_SECT_D rData;
	POSITION Pos = m_arBeamData.GetStartPosition();
	while(Pos)
	{
		int SectK=0;
		int rSectK=0;
		m_arBeamData.GetNextAssoc(Pos, SectK, rSectK);
		if(m_arBeamData.Lookup(SectK, rSectK))
		{
			m_pDoc->m_pAttrCtrl->GetSectDesign(rSectK,rData);
			InsertItem(rSectK,rData.SName);
		}
	}
}

BOOL CDgnStlTransverseStfnDlg::InsertItem(T_SECT_K Key, CString strSName)
{
	
	T_TVSF_D TvsfD;
	if (!m_pDoc->m_pAttrCtrl2->GetTvsf(Key, TvsfD))
		TvsfD.Initialize();

	LVITEM lvitem;
	CString str;
	int nItem = m_wndList.GetItemCount();

	lvitem.iItem = FindInsertionPos(Key);
	for(int i=0; i < 5; i++)
	{
		lvitem.iSubItem = i;
		str = DataToStr(i, Key, strSName, TvsfD);
		lvitem.pszText = str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;

		if (i == 0) 
		{
			lvitem.mask |= LVIF_PARAM;
			lvitem.lParam = (LPARAM)Key;
			nItem = m_wndList.InsertItem(&lvitem);
		}
		else m_wndList.SetItem(&lvitem);
		str.ReleaseBuffer();
	}

	m_wndList.SetItemData(nItem, (DWORD)Key);
	return TRUE;
}

int CDgnStlTransverseStfnDlg::FindInsertionPos(T_SECT_K nID)
{
	int nCount = m_wndList.GetItemCount();
	for (int i=0; i < nCount; i++)
	{
        T_SECT_K key = m_wndList.GetItemData(i);
		if (nID < key) { return i; }
	}
	return nCount;
}

CString CDgnStlTransverseStfnDlg::DataToStr(int nId, T_SECT_K Key, CString strSName, T_TVSF_D &rData)
{  
	CString str;
	if(nId == 0) str.Format(_T("%d"), Key);
	else if(nId == 1) str = strSName;
	else if(nId==2 || nId==3 || nId==4)
	{
		if(rData.Tvsf[nId-2].dPitch>0.0) str = _T("O");
		else                             str = _T("X");
	}
	else ASSERT(0);

	return str;
}

void CDgnStlTransverseStfnDlg::ChangeDlgItemInfo()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_STL_TVSF_CHECK_SAME)->EnableWindow(TRUE);

	if(m_Data.bSameIMJ)
	{
		GetDlgItem(IDC_STL_TVSF_FRAME_M)->EnableWindow(FALSE);
		GetDlgItem(IDC_TVSF_STATIC_ht_m)->EnableWindow(FALSE);
		GetDlgItem(IDC_TVSF_STATIC_t_m)->EnableWindow(FALSE);
		GetDlgItem(IDC_TVSF_STATIC_Pitch_m)->EnableWindow(FALSE);
		GetDlgItem(IDC_TVSF_EDIT_ht_m)->EnableWindow(FALSE);
		GetDlgItem(IDC_TVSF_EDIT_t_m)->EnableWindow(FALSE);
		GetDlgItem(IDC_TVSF_EDIT_pitch_m)->EnableWindow(FALSE);
		GetDlgItem(IDC_STL_TVSF_ht_unit_m)->EnableWindow(FALSE);
		GetDlgItem(IDC_STL_TVSF_t_unit_m)->EnableWindow(FALSE);
		GetDlgItem(IDC_STL_TVSF_pitch_unit_m)->EnableWindow(FALSE);
		//
		GetDlgItem(IDC_STL_TVSF_FRAME_J)->EnableWindow(FALSE);
		GetDlgItem(IDC_TVSF_STATIC_ht_j)->EnableWindow(FALSE);
		GetDlgItem(IDC_TVSF_STATIC_t_j)->EnableWindow(FALSE);
		GetDlgItem(IDC_TVSF_STATIC_Pitch_j)->EnableWindow(FALSE);
		GetDlgItem(IDC_TVSF_EDIT_ht_j)->EnableWindow(FALSE);
		GetDlgItem(IDC_TVSF_EDIT_t_j)->EnableWindow(FALSE);
		GetDlgItem(IDC_TVSF_EDIT_pitch_j)->EnableWindow(FALSE);
		GetDlgItem(IDC_STL_TVSF_ht_unit_j)->EnableWindow(FALSE);
		GetDlgItem(IDC_STL_TVSF_t_unit_j)->EnableWindow(FALSE);
		GetDlgItem(IDC_STL_TVSF_pitch_unit_j)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_STL_TVSF_FRAME_M)->EnableWindow(TRUE);
		GetDlgItem(IDC_TVSF_STATIC_ht_m)->EnableWindow(TRUE);
		GetDlgItem(IDC_TVSF_STATIC_t_m)->EnableWindow(TRUE);
		GetDlgItem(IDC_TVSF_STATIC_Pitch_m)->EnableWindow(TRUE);
		GetDlgItem(IDC_TVSF_EDIT_ht_m)->EnableWindow(TRUE);
		GetDlgItem(IDC_TVSF_EDIT_t_m)->EnableWindow(TRUE);
		GetDlgItem(IDC_TVSF_EDIT_pitch_m)->EnableWindow(TRUE);
		GetDlgItem(IDC_STL_TVSF_ht_unit_m)->EnableWindow(TRUE);
		GetDlgItem(IDC_STL_TVSF_t_unit_m)->EnableWindow(TRUE);
		GetDlgItem(IDC_STL_TVSF_pitch_unit_m)->EnableWindow(TRUE);
		//
		GetDlgItem(IDC_STL_TVSF_FRAME_J)->EnableWindow(TRUE);
		GetDlgItem(IDC_TVSF_STATIC_ht_j)->EnableWindow(TRUE);
		GetDlgItem(IDC_TVSF_STATIC_t_j)->EnableWindow(TRUE);
		GetDlgItem(IDC_TVSF_STATIC_Pitch_j)->EnableWindow(TRUE);
		GetDlgItem(IDC_TVSF_EDIT_ht_j)->EnableWindow(TRUE);
		GetDlgItem(IDC_TVSF_EDIT_t_j)->EnableWindow(TRUE);
		GetDlgItem(IDC_TVSF_EDIT_pitch_j)->EnableWindow(TRUE);
		GetDlgItem(IDC_STL_TVSF_ht_unit_j)->EnableWindow(TRUE);
		GetDlgItem(IDC_STL_TVSF_t_unit_j)->EnableWindow(TRUE);
		GetDlgItem(IDC_STL_TVSF_pitch_unit_j)->EnableWindow(TRUE);
	}

	UpdateData(FALSE);
	Invalidate();
}

BOOL CDgnStlTransverseStfnDlg::SelectSection(CArray<T_TVSF_K, T_TVSF_K>&  arKey)
{
	int iCheckResult = 0;
	T_SECT_D OrginSectData; 
	OrginSectData.Initialize();
	T_SECT_D SectData; 

	int nIndex = 0;

	for(int i=0; i < arKey.GetSize(); i++)
	{
		iCheckResult = 0;
		SectData.Initialize();
		T_TVSF_K TvsfK = arKey.GetAt(i);
		if(!m_pDoc->m_pAttrCtrl->GetSectDesign(TvsfK, SectData)) continue;
		
		if(nIndex == 0) OrginSectData = SectData;
		if(!IsDiffSections(OrginSectData, SectData)) return FALSE;
		nIndex++;
	}
	 
	return TRUE;
}

BOOL CDgnStlTransverseStfnDlg::IsDiffSections(T_SECT_D& OrginSectData, T_SECT_D& SectData)
{
	if(OrginSectData.nStype != SectData.nStype) return FALSE;
	
	CString strShape1 = GetShapeOfSection(OrginSectData);
	CString strShape2 = GetShapeOfSection(SectData);

	if(strShape1 != strShape2) return FALSE;
	
	return TRUE;
}

void CDgnStlTransverseStfnDlg::Data2Dlg()
{
	m_chkSameTvsf.SetCheck(m_Data.bSameIMJ);
		m_ht_i   .SetEditUnit(m_Data.Tvsf[0].dht);
		m_t_i    .SetEditUnit(m_Data.Tvsf[0].dt);
		m_Pitch_i.SetEditUnit(m_Data.Tvsf[0].dPitch);
		 //
		m_ht_m   .SetEditUnit(m_Data.Tvsf[1].dht);
		m_t_m    .SetEditUnit(m_Data.Tvsf[1].dt);
		m_Pitch_m.SetEditUnit(m_Data.Tvsf[1].dPitch);
		 //
		m_ht_j   .SetEditUnit(m_Data.Tvsf[2].dht);
		m_t_j    .SetEditUnit(m_Data.Tvsf[2].dt);
		m_Pitch_j.SetEditUnit(m_Data.Tvsf[2].dPitch);
}

BOOL CDgnStlTransverseStfnDlg::Dlg2Data()
{
	m_Data.bSameIMJ = m_chkSameTvsf.GetCheck();
	if(m_Data.bSameIMJ)
	{
		m_Data.Tvsf[0].dht    = m_ht_i.GetEditValue();
		m_Data.Tvsf[0].dt     = m_t_i.GetEditValue();
		m_Data.Tvsf[0].dPitch = m_Pitch_i.GetEditValue();

		m_Data.Tvsf[1] = m_Data.Tvsf[2] = m_Data.Tvsf[0];
	}
	else
	{
		m_Data.Tvsf[0].dht    = m_ht_i.GetEditValue();
		m_Data.Tvsf[0].dt     = m_t_i.GetEditValue();
		m_Data.Tvsf[0].dPitch = m_Pitch_i.GetEditValue();
		 //
		m_Data.Tvsf[1].dht    = m_ht_m.GetEditValue();
		m_Data.Tvsf[1].dt     = m_t_m.GetEditValue();
		m_Data.Tvsf[1].dPitch = m_Pitch_m.GetEditValue();
		 //
		m_Data.Tvsf[2].dht    = m_ht_j.GetEditValue();
		m_Data.Tvsf[2].dt     = m_t_j.GetEditValue();
		m_Data.Tvsf[2].dPitch = m_Pitch_j.GetEditValue();
	}

	return TRUE;
}


CString CDgnStlTransverseStfnDlg::GetShapeOfSection(T_SECT_D& SectD)
{
	CString strSectShape;
	if(SectD.nStype == D_SECT_TYPE_REGULAR)                  strSectShape = SectD.SectBefore.Shape;
	else if(SectD.nStype == D_SECT_TYPE_TAPERED && 
					SectD.SectBefore.nStype == D_SECT_TYPE_REGULAR)  strSectShape = SectD.SectBefore.SectI.Shape;

	return strSectShape;
}

void CDgnStlTransverseStfnDlg::ShowSizeTip(T_SECT_D& rData)
{
	if(rData.nStype != D_SECT_TYPE_REGULAR)
	{
		SetShowTip(CString(_T("")));
		return;
	}
	
	int nShapeIndex = CSectUtil::GetShapeIndexFromNameReg(rData.SectBefore.Shape);
	if(nShapeIndex < 0) return;
	CStringArray aSizeTitle;
	CSectUtil::GetSizeTitleFromShapeIndexReg(nShapeIndex, aSizeTitle);
	int nCount = aSizeTitle.GetSize();
	
	int nSizeNum = 0;
	CString csSizeTip = _T("");
	T_SECT_SECTBASE_D* pSect = &rData.SectBefore.SectI;

	for(int i=0; i<nCount; i++)
	{
		if(aSizeTitle[i] != _T(""))
		{
			CString csTemp;
			csTemp.Format(_T("%s : %g "), aSizeTitle[i], pSect->Size[nSizeNum]);
			csSizeTip += csTemp;
			nSizeNum++;
		}
	}
	SetShowTip(csSizeTip);
}

void CDgnStlTransverseStfnDlg::SetShowTip(CString& csTip)
{
	CWnd* pWnd = GetDlgItem(IDC_EDIT_SECT_SHAPE_SIZE);
	ASSERT(pWnd);

	if(csTip == _T("")) pWnd->ShowWindow(SW_HIDE);
	else
	{
		pWnd->ShowWindow(SW_SHOW);
		m_editTip.SetReadOnly();
	}
	pWnd->SetWindowText(csTip);
//  m_csTip = csTip;
}