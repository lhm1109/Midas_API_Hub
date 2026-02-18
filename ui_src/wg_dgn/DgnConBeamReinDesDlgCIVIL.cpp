// DgnConBeamDgnDlgCIVIL.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConBeamReinDesDlgCIVIL.h"
#include "DgnDataCtrl.h"

#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"

#include "..\wg_main\wg_mainRes2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConBeamReinDesDlgCIVIL dialog


CDgnConBeamReinDesDlgCIVIL::CDgnConBeamReinDesDlgCIVIL(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnConBeamReinDesDlgCIVIL::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConBeamReinDesDlgCIVIL)	
	//}}AFX_DATA_INIT

	if(m_arBeamData.GetCount()>0) m_arBeamData.RemoveAll();

	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);

	m_bTorsionalBar = FALSE;
	m_bBundledBar = FALSE;
}


void CDgnConBeamReinDesDlgCIVIL::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConBeamReinDesDlgCIVIL)
	DDX_Control(pDX, IDC_DGN_DELETE, m_btnDel);
	DDX_Control(pDX, IDOK, m_btnAdd);
	DDX_Control(pDX, IDC_BEAM_SECT_LIST, m_wndList);
	DDX_Control(pDX, IDC_STIRRUP_NUM_CB, m_SubNum);
	DDX_Control(pDX, IDC_STIRRUP_CB, m_SubCB);
	DDX_Control(pDX, IDC_dT_UNIT, m_dTunit);
	DDX_Control(pDX, IDC_dT_EDIT, m_dT);
	DDX_Control(pDX, IDC_DGN_RC_BHFUNIT, m_hfunit);
	DDX_Control(pDX, IDC_DGN_RC_BHF, m_hf);
	DDX_Control(pDX, IDC_DGN_RC_BHCUNIT, m_Hcunit);
	DDX_Control(pDX, IDC_DGN_RC_BHC, m_Hc);
	DDX_Control(pDX, IDC_DGN_RC_BBFUNIT, m_bfunit);
	DDX_Control(pDX, IDC_DGN_RC_BBF, m_bf);
	DDX_Control(pDX, IDC_DGN_RC_BBCUNIT, m_Bcunit);
	DDX_Control(pDX, IDC_DGN_RC_BBC, m_Bc);
	DDX_Control(pDX, IDC_dB_UNIT, m_dBunit);
	DDX_Control(pDX, IDC_dB_EDIT, m_dB);
	DDX_Check  (pDX, IDC_RDGN_TOR_CHK,              m_bTorsionalBar);
	DDX_Control(pDX, IDC_RDGN_TOR_STIRRUP_SIZE_CMB, m_cmbTorBar);
	DDX_Check  (pDX, IDC_RDGN_TOR_BUNDLED_CHK,      m_bBundledBar);
	DDX_Control(pDX, IDC_RDGN_TOR_BUNDLED_NUM_EDT,  m_edtBundledBarNum);
	DDX_Control(pDX, IDC_RDGN_TOR_LONGI_SIZE_CMB,   m_cmbLongiBar);
	DDX_Control(pDX, IDC_RDGN_TOR_LONGI_NUM_EDT,    m_edtLongiBarNum);
	DDX_Control(pDX, IDC_DGN_BEAMRECT_BIT, m_wndPictureRect);
	DDX_Control(pDX, IDC_DGN_BEAMTEE_BIT, m_wndPictureTee);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnConBeamReinDesDlgCIVIL, CDialogMove)
	//{{AFX_MSG_MAP(CDgnConBeamReinDesDlgCIVIL)
	ON_BN_CLICKED(IDC_DGN_DESIGN_SECTION,   OnDgnDesignSection)
	ON_BN_CLICKED(IDC_RDGN_TOR_CHK,         OnDgnTorsionalBar)
	ON_BN_CLICKED(IDC_RDGN_TOR_BUNDLED_CHK, OnDgnBundledStirrups)
	ON_BN_CLICKED(IDC_DGN_DELETE, OnDgnDelete)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_BEAM_SECT_LIST, OnItemchangedDgnSectList)	
	ON_NOTIFY(NM_CLICK, IDC_BEAM_SECT_LIST, OnClickDgnSectList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConBeamReinDesDlgCIVIL message handlers

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// Initialize 관련 
//

void CDgnConBeamReinDesDlgCIVIL::SetInitStirCB()
{
	if(m_SubCB.GetCount()>0) m_SubCB.ResetContent();
	CDBLib::GetRebarNameAtComboBox(&m_SubCB);

	m_cmbTorBar.ResetContent();
	CDBLib::GetRebarNameAtComboBox(&m_cmbTorBar);

	m_cmbLongiBar.ResetContent();
	CDBLib::GetRebarNameAtComboBox(&m_cmbLongiBar);
}

void CDgnConBeamReinDesDlgCIVIL::SetInitUnit()
{
	m_dT.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_hf.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Hc.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_bf.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Bc.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_dB.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_dTunit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_hfunit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Hcunit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_bfunit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Bcunit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_dBunit.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_SubNum.SetUnitType(D_UNITSYS_NONE);

	m_edtBundledBarNum.SetUnitType(D_UNITSYS_NONE);
	m_edtLongiBarNum.SetUnitType(D_UNITSYS_NONE);
}

void CDgnConBeamReinDesDlgCIVIL::SetData2Dlg()
{
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);

	//ResetSectGridData();

	//m_AddorDel = 0;
		
	m_hf.SetEditUnit(0.);
	m_Hc.SetEditUnit(0.);
	m_bf.SetEditUnit(0.);
	m_Bc.SetEditUnit(0.);

	m_hf.EnableWindow(FALSE);
	m_Hc.EnableWindow(FALSE);
	m_bf.EnableWindow(FALSE);
	m_Bc.EnableWindow(FALSE);
	GetDlgItem(IDC_DGN_RC_BSHAPE)->EnableWindow(FALSE);
		
	ShowRECTBitmap();
	ShowHideSectCtrl(FALSE);

	m_SubCB.SetCurSel(-1);
	m_SubNum.SetEditUnit(0.);
	m_dB.SetEditUnit(0);
	m_dT.SetEditUnit(0);

	m_bTorsionalBar = FALSE;
	m_cmbTorBar.SetCurSel(-1);
	m_bBundledBar = FALSE;
	m_edtBundledBarNum.SetEditUnit(0.0);
	m_cmbLongiBar.SetCurSel(-1);	
	m_edtLongiBarNum.SetEditUnit(0.0);

	UpdateData(FALSE);
}

BOOL CDgnConBeamReinDesDlgCIVIL::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	SetHeaderTitle();
	MakeItemEx();
	
	SetInitStirCB();
	SetInitUnit();
	SetData2Dlg();

	CWnd* PlaceHolder = GetDlgItem(IDC_DGN_SP_ID_PREVIEW); ASSERT(PlaceHolder);
	m_SectWnd.Initial(PlaceHolder);
	
	m_btnAdd.EnableWindow(FALSE);
	m_btnDel.EnableWindow(FALSE);

	if (m_bModify)
	{
		int nItemCount = m_wndList.GetItemCount();
        int ix = 0;
		for (ix = 0; ix < nItemCount; ix++)
			if (m_RdgnKey == m_wndList.GetItemData(ix)) break;
		if (ix != nItemCount)
		{
			m_wndList.SetItemState(ix, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
			m_btnAdd.EnableWindow(TRUE);
			m_btnDel.EnableWindow(TRUE);
		}
	}

	ControlsEnableDisable();

	m_wndPictureRect.SetImage(_T("SVG\\Illustration\\Dialog\\dgn_beam_rect_dgn_civil.svg"));
	m_wndPictureTee.SetImage(_T("SVG\\Illustration\\Dialog\\dgn_beam_tee_dgn_civil.svg"));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnConBeamReinDesDlgCIVIL::OnDgnDesignSection() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND,MAKEWPARAM(ID_STRUCT_PROP_SECT4DGN,0));	
}

void CDgnConBeamReinDesDlgCIVIL::OnDgnTorsionalBar()
{
	ControlsEnableDisable();
}

void CDgnConBeamReinDesDlgCIVIL::OnDgnBundledStirrups()
{
	ControlsEnableDisable();
}

// add/replace
void CDgnConBeamReinDesDlgCIVIL::OnOK() 
{
	// TODO: Add extra validation here

	// get data
	UpdateData(TRUE);
	// 저장하기 전 Data의 유효성 여부 Check - Data 저장
	// Grid in Tab.
	CArray<T_RDGN_K, T_RDGN_K>  arRdgnKey;
	CArray<T_RDGN_D, T_RDGN_D&> arRdgnData;
	int nItem = -1;
	int nSelCount = m_wndList.GetSelectedCount();
	arRdgnKey.SetSize(nSelCount);
	arRdgnData.SetSize(nSelCount);
	for(UINT i=0; i< nSelCount; i++)
	{
		nItem = m_wndList.GetNextItem(nItem, LVNI_SELECTED);
		ASSERT(nItem != -1);
		arRdgnKey[i]   = m_wndList.GetItemData(nItem);
		m_arRdgnKey.SetAt(i,m_wndList.GetItemData(nItem));
	}

	if(!SelectSection(arRdgnKey))
	{
		AfxMessageBox(_LS(IDS_DGN_CON_SELECT_DiffShpe_Err));
		return;
	}
	
	if(!CheckInputData()) return;
	SetDlg2Data();
	SetData2Dlg();

	for (int i=0; i < nSelCount; i++)
	{
		arRdgnData[i] = m_RdgnData;   
	}

	if (!m_pDoc->m_pDataCtrl->AddRdgn(arRdgnKey, arRdgnData))
		return;  

	MakeItemEx();

	int nIndex = m_wndList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	ChangeItem(nIndex);
	
	//CDialogMove::OnOK();
}

// close
void CDgnConBeamReinDesDlgCIVIL::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}

void CDgnConBeamReinDesDlgCIVIL::OnDgnDelete() 
{
	// TODO: Add your control notification handler code here
	T_RDGN_K key;
	CArray<T_RDGN_K, T_RDGN_K> aKey;
	int nItem = -1;

	int nSelCount = m_wndList.GetSelectedCount();
	for (UINT i=0; i < nSelCount; i++)
	{
		nItem = m_wndList.GetNextItem(nItem, LVNI_SELECTED);
		ASSERT(nItem != -1);
		key = m_wndList.GetItemData(nItem);
		if (m_pDoc->m_pAttrCtrl->ExistRdgn(key)) aKey.Add(key);
	}

	if (!m_pDoc->m_pDataCtrl->DelRdgn(aKey)) return;
	 
	MakeItemEx();
	int nIndex = m_wndList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	ChangeItem(nIndex);
}

void CDgnConBeamReinDesDlgCIVIL::OnClickDgnSectList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_wndList.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	m_btnAdd.EnableWindow(nIndex != -1);
	m_btnDel.EnableWindow(nIndex != -1);

	*pResult = 0;
}

void CDgnConBeamReinDesDlgCIVIL::OnItemchangedDgnSectList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	int nIndex = pNMListView->iItem; 
	ChangeItem(nIndex);
	*pResult = 0;
}


void CDgnConBeamReinDesDlgCIVIL::ChangeItem(int nIndex)
{
	// enable/disable
	m_btnAdd.EnableWindow(nIndex != -1);
	m_btnDel.EnableWindow(nIndex != -1);

	// get data
	m_RdgnKey = 0;
	m_RdgnData.Initialize();
	m_SectD.Initialize();  
	if (nIndex != -1)
	{
	  m_RdgnKey = m_wndList.GetItemData(nIndex);
		if (!m_pDoc->m_pAttrCtrl->GetRdgn(m_RdgnKey, m_RdgnData))
			m_RdgnData.Initialize();
		m_pDoc->m_pAttrCtrl->GetSectDesign(m_RdgnKey, m_SectD);
	}

	// Left Section Data.
	ChangeSectionInfo();

	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	// 일단 0 으로 고정해두었다~	

	int iInx = m_SubCB.SelectString(-1, m_RdgnData.BEAM.strSubBarNa[0]);
	m_SubCB.SetCurSel(iInx); 

	m_SectWnd.Draw_Shape(0, m_RdgnKey, FALSE, TRUE);
	m_SectWnd.Invalidate();

	ControlsEnableDisable();
}

void CDgnConBeamReinDesDlgCIVIL::SetHeaderTitle()
{
	CStringArray aTitle;
	CArray<int, int> aWidth;

#define M_ADD_LIST_INFO(TITLE, WIDTH) aTitle.Add(TITLE); aWidth.Add(WIDTH)
	M_ADD_LIST_INFO(_LS(IDS_DGN_LIST_ID)  ,   35);  //_T("ID")   
	M_ADD_LIST_INFO(_LS(IDS_DGN_LIST_NAME),   120); //_T("Name")
	M_ADD_LIST_INFO(_LS(IDS_DGN_LIST_BAR)  ,   40);  //_T("Bar") 
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

void CDgnConBeamReinDesDlgCIVIL::MakeItemEx()
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

		if(MatdData.Type == _T("C"))
		{
			// 1=column, 2=beam, 3=brace, 0=기타
	    int nMembType = m_pDoc->m_pAttrCtrl->GetMemberType(arElemKeyList[i]);

			T_SECT_D SectData; SectData.Initialize();
			BOOL bCheck = m_pDoc->m_pAttrCtrl->GetSectDesign(ElemData.elpro,SectData);

			if(bCheck == TRUE) 
			{
				if(nMembType == D_MBTP_COLUMN && 
					(SectData.SectBefore.Shape==D_SECT_SHAPE_REG_B    ||
					 SectData.SectBefore.Shape==D_SECT_SHAPE_REG_SB   || 
					 SectData.SectBefore.Shape==D_SECT_SHAPE_REG_P    ||
					 SectData.SectBefore.Shape==D_SECT_SHAPE_REG_SR   ||
					 SectData.SectBefore.Shape==D_SECT_SHAPE_REG_OCT  ||
					 SectData.SectBefore.Shape==D_SECT_SHAPE_REG_SOCT ||
					 SectData.SectBefore.Shape==D_SECT_SHAPE_REG_TRK  ||
					 SectData.SectBefore.Shape==D_SECT_SHAPE_REG_STRK ||
					 SectData.SectBefore.Shape==D_SECT_SHAPE_REG_HTRK )) ColmSecMap.SetAt(ElemData.elpro, ElemData.elpro);


				if(nMembType == D_MBTP_BEAM && (SectData.SectBefore.Shape == _T("SB") || SectData.SectBefore.Shape == _T("T")) &&
					 SectData.nStype==D_SECT_TYPE_REGULAR) 
					 m_arBeamData.SetAt(ElemData.elpro,ElemData.elpro);
			}
		}
	}

	POSITION Pos = ColmSecMap.GetStartPosition();
	while(Pos)
	{
		int SectK=0;
		int rSectK=0;
		ColmSecMap.GetNextAssoc(Pos, SectK, rSectK);
		if(m_arBeamData.Lookup(SectK, rSectK))
		{
			m_arBeamData.RemoveKey(SectK);
		}
	}  

	T_SECT_D rData;
	Pos = m_arBeamData.GetStartPosition();
	while(Pos)
	{
		int SectK=0;
		int rSectK=0;
		m_arBeamData.GetNextAssoc(Pos, SectK, rSectK);
		if(m_arBeamData.Lookup(SectK, rSectK))
		{
			m_pDoc->m_pAttrCtrl->GetSectDesign(rSectK,rData);
			InsertItem(rSectK,rData);
		}
	}
}

BOOL CDgnConBeamReinDesDlgCIVIL::InsertItem(T_SECT_K Key, T_SECT_D &Data)
{
	
	T_RDGN_D RdgnD;
	if (!m_pDoc->m_pAttrCtrl->GetRdgn(Key, RdgnD))
		RdgnD.Initialize();

	LVITEM lvitem;
	CString str;
	int nItem = m_wndList.GetItemCount();

	lvitem.iItem = FindInsertionPos(Key);
	for(int i=0; i < 3; i++)
	{
		lvitem.iSubItem = i;
		str = DataToStr(i, Key, Data, RdgnD);
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

int CDgnConBeamReinDesDlgCIVIL::FindInsertionPos(T_SECT_K nID)
{
	int nCount = m_wndList.GetItemCount();
	for (int i=0; i < nCount; i++)
	{
        T_SECT_K key = m_wndList.GetItemData(i);
        if ( nID < key ) { return i; }
	}
	return nCount;
}

CString CDgnConBeamReinDesDlgCIVIL::DataToStr(int i, T_SECT_K Key, T_SECT_D &Data, T_RDGN_D &rData)
{  
	T_RDGN_BEAM rDgnBeam;
	rDgnBeam = rData.BEAM;

	CString str;
	if(i == 0) str.Format(_T("%d"), Key);
	else if(i == 1) str = Data.SName;
	else if(i == 2)
	{
		if(m_pDoc->m_pAttrCtrl->ExistRdgn(Key)) str = _T("O");
		else                                    str = _T("X");
	}
	else ASSERT(0);

	return str;
}

BOOL CDgnConBeamReinDesDlgCIVIL::SelectSection(CArray<T_RDGN_K, T_RDGN_K>&  arRdgnKey)
{
	BOOL bCanbeChked = TRUE;

	int iCheckResult = 0;
	T_SECT_D OrginSectData; 
	OrginSectData.Initialize();
	T_SECT_D SectData; 

	int Index = 0;

	for(int i=0; i < arRdgnKey.GetSize(); i++)
	{
		iCheckResult = 0;
		SectData.Initialize();
		T_RDGN_K Rdgn = arRdgnKey.GetAt(i);
		if(!m_pDoc->m_pAttrCtrl->GetSectDesign(Rdgn, SectData)) continue;
		
		if(Index == 0)
			OrginSectData = SectData;
		CheckDiffSections(OrginSectData, SectData, iCheckResult);
		if(iCheckResult > 0)
		{
			bCanbeChked = FALSE;
			return bCanbeChked;
		}
		Index++;
	}
	 
	return bCanbeChked;
}

void CDgnConBeamReinDesDlgCIVIL::CheckDiffSections(T_SECT_D OrginSectData, T_SECT_D SectData, int& iResult)
{
	//iResult = 2: Different Section Shape
	//iResult = 4: Different Section Size
	if(SectData.SectBefore.Shape != OrginSectData.SectBefore.Shape)
		iResult |= 2;
/*
	else 
	{
		if(SectData.SectBefore.Shape == _T("SB"))
		{
			if((SectData.SectBefore.SectI.Size[0] != OrginSectData.SectBefore.SectI.Size[0]) ||
				 (SectData.SectBefore.SectI.Size[1] != OrginSectData.SectBefore.SectI.Size[1]))
				 iResult |= 4;
		}
		else if(SectData.SectBefore.Shape == _T("T"))
		{
			if((SectData.SectBefore.SectI.Size[0] != OrginSectData.SectBefore.SectI.Size[0]) ||
				 (SectData.SectBefore.SectI.Size[1] != OrginSectData.SectBefore.SectI.Size[1]) ||
				 (SectData.SectBefore.SectI.Size[2] != OrginSectData.SectBefore.SectI.Size[2]) ||
				 (SectData.SectBefore.SectI.Size[3] != OrginSectData.SectBefore.SectI.Size[3]))
				 iResult |= 4;
		}
		else ASSERT(0);
	}
*/
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// OnOK 관련 
//

BOOL CDgnConBeamReinDesDlgCIVIL::CheckInputData()
{
	int nIndex;

	T_SECT_D sData;
	sData.Initialize();
	BOOL bGetOK = m_pDoc->m_pAttrCtrl->GetSectDesign(m_RdgnKey, sData);
	
	if(!bGetOK){
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_Property_Num_Err));
//    AfxMessageBox(_T("[오류] Property Number 선택이 적당하지 않습니다."));
		return FALSE;
	}

	nIndex = m_SubCB.GetCurSel();
	if(nIndex < 0)
	{
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_Stirrup_Bar_Size_Err));
		return FALSE;
	}

	double dH = m_Hc.GetEditValue()/2.0;
	if(m_dT.GetEditValue()<=0 || m_dT.GetEditValue()>=dH)
	{
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_Dt_Input_Err));
		//AfxMessageBox(_T("[오류] dT 입력이 잘못되었습니다."));
		return FALSE;
	}

	if(m_dB.GetEditValue()<=0 || m_dB.GetEditValue()>=dH)
	{
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_Db_Input_Err));
		//AfxMessageBox(_T("[오류] dB 입력이 잘못되었습니다."));
		return FALSE;
	}

	if(m_SubNum.GetEditValue()<=0)
	{
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_Stirrup_Bar_Num_Err));
//    AfxMessageBox(_T("[오류] Stirrup(Spacing) 입력이 적당하지 않습니다."));
		return FALSE;
	}

	if(m_bTorsionalBar)
	{
		nIndex = m_cmbTorBar.GetCurSel();
		if(nIndex < 0)
		{
			AfxMessageBox(_LS(IDS_DGN_CON_BEAM_TOR_STIRRUP_BAR_SIZE_ERR));
			//AfxMessageBox(_T("[오류] Torsional Reinforcement - Stirrup (Size) 입력이 적당하지 않습니다."));
			return FALSE;
		}
		
		if(m_bBundledBar)
		{
			if(m_edtBundledBarNum.GetEditValue()<=0)
			{
				AfxMessageBox(_LS(IDS_DGN_CON_BEAM_TOR_STIRRUP_BAR_NUM_ERR));
				//AfxMessageBox(_T("[오류] Torsional Reinforcement - Stirrup (Number) 입력이 적당하지 않습니다."));
				return FALSE;
			}
		}
		
		nIndex = m_cmbLongiBar.GetCurSel();
		if(nIndex < 0)
		{
			AfxMessageBox(_LS(IDS_DGN_CON_BEAM_LONG_STIRRUP_BAR_SIZE_ERR));
			//AfxMessageBox(_T("[오류] Torsional Reinforcement - Longitudinal Reinforcement (Size) 입력이 적당하지 않습니다."));
			return FALSE;
		}
		
		if(m_edtLongiBarNum.GetEditValue()<4)
		{
			AfxMessageBox(_LS(IDS_DGN_CON_BEAM_TOR_STIRRUP_BAR_SIZE_ERR));
			//AfxMessageBox(_T("[오류] Torsional Reinforcement - Stirrup (Number) 입력이 적당하지 않습니다."));
			return FALSE;
		}
	}

	return TRUE;
}

void CDgnConBeamReinDesDlgCIVIL::SetDlg2Data()
{
	UpdateData(TRUE);
	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	// 일단 0 으로 고정해두었다~	

	CString str;
	m_RdgnData.nType = 0;

	m_RdgnData.BEAM.dDt[0] = m_dT.GetEditValue();
	m_RdgnData.BEAM.dDb[0] = m_dB.GetEditValue();  
	
/*
	int nIndex = m_SubNum.GetCurSel();
	if(nIndex>=0) m_SubNum.GetLBText(nIndex, m_RdgnData.BEAM.strSubBarNum);
*/
	m_RdgnData.BEAM.dSubBarNum[0] = m_SubNum.GetEditValue();

	int nIndex = m_SubCB.GetCurSel();
	if(nIndex>=0) m_SubCB.GetLBText(nIndex, m_RdgnData.BEAM.strSubBarNa[0]);

	// Torsional Reinforcement
	m_RdgnData.BEAM.bTorsionalBar[0] = m_bTorsionalBar;

	nIndex = m_cmbTorBar.GetCurSel();
	if(nIndex>=0) m_cmbTorBar.GetLBText(nIndex, m_RdgnData.BEAM.strTorBarNa[0]);

	m_RdgnData.BEAM.bBundledBar[0] = m_bBundledBar;
	m_RdgnData.BEAM.dBundledBarNum[0] = m_edtBundledBarNum.GetEditValue();

	nIndex = m_cmbLongiBar.GetCurSel();
	if(nIndex>=0) m_cmbLongiBar.GetLBText(nIndex, m_RdgnData.BEAM.strLongiBarNa[0]);

	m_RdgnData.BEAM.dLongiBarNum[0] = m_edtLongiBarNum.GetEditValue();	
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// Change PropNo 관련 
//

void CDgnConBeamReinDesDlgCIVIL::ShowRECTBitmap()
{
	GetDlgItem(IDC_DGN_BEAMRECT_BIT)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_BEAMTEE_BIT)->ShowWindow(SW_HIDE);
}

void CDgnConBeamReinDesDlgCIVIL::ShowTEEBitmap()
{
	GetDlgItem(IDC_DGN_BEAMTEE_BIT)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_BEAMRECT_BIT)->ShowWindow(SW_HIDE);  
}

//---------------------------------------------------------------------------
// ShowHideSectCtrl 
//---------------------------------------------------------------------------
void CDgnConBeamReinDesDlgCIVIL::ShowHideSectCtrl(BOOL bShow)
{
	int i;
	CWnd* pWnd[6];
	pWnd[0] = GetDlgItem(IDC_DGN_BF_STATIC);  ASSERT(pWnd[0]);
	pWnd[1] = GetDlgItem(IDC_DGN_RC_BBF);     ASSERT(pWnd[1]);
	pWnd[2] = GetDlgItem(IDC_DGN_RC_BBFUNIT); ASSERT(pWnd[2]);
	pWnd[3] = GetDlgItem(IDC_DGN_HF_STATIC);  ASSERT(pWnd[3]);
	pWnd[4] = GetDlgItem(IDC_DGN_RC_BHF);     ASSERT(pWnd[4]);
	pWnd[5] = GetDlgItem(IDC_DGN_RC_BHFUNIT); ASSERT(pWnd[5]);

	if(bShow) for(i=0; i<6; i++) pWnd[i]->EnableWindow(TRUE);
	else      for(i=0; i<6; i++) pWnd[i]->EnableWindow(FALSE);
}

//---------------------------------------------------------------------------
// SelectElemOnView 
//---------------------------------------------------------------------------
void CDgnConBeamReinDesDlgCIVIL::SelectElemOnView()
{
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);

	CArray<T_ELEM_K, T_ELEM_K> arElemKeyList;
	arElemKeyList.RemoveAll();

	CArray<T_RDGN_K, T_RDGN_K>  arRdgnKey;
	int nItem = -1;

	int nSelCount = m_wndList.GetSelectedCount();
	arRdgnKey.SetSize(nSelCount);
	for (UINT i=0; i < nSelCount; i++)
	{
		nItem = m_wndList.GetNextItem(nItem, LVNI_SELECTED);
		ASSERT(nItem != -1);
		arRdgnKey[i]   = m_wndList.GetItemData(nItem);  
	}

	T_ELEM_K ElemKey;
	T_ELEM_D ElemData;
	CDgnDataCtrl DataCtrl;

	T_RDGN_K RdgnKey;
	int Index = 0;
	for(int i=0; i<arRdgnKey.GetSize(); i++)
	{
		RdgnKey = arRdgnKey.GetAt(i);
		POSITION PosElem = m_pDoc->m_pAttrCtrl->GetStartElem();
		while(PosElem)
		{
			ElemData.Initialize();
			m_pDoc->m_pAttrCtrl->GetNextElem(PosElem, ElemKey, ElemData);
		
			if(ElemData.elpro == RdgnKey) 
				if(ElemData.eltyp == TRUSS_EL || ElemData.eltyp == BEAM_EL)
					if(DataCtrl.Get_DgnGenMbtp(ElemKey) == D_MBTP_BEAM)  arElemKeyList.Add(ElemKey);
		}
	}

	m_pDoc->m_pViewCtrl->SelectElem(NULL, arElemKeyList, FALSE);
}

void CDgnConBeamReinDesDlgCIVIL::ChangeSectionInfo()
{
	// Select Elem. On View
	SelectElemOnView();

	T_SECT_D SectData; SectData.Initialize();

	if(!m_pDoc->m_pAttrCtrl->GetSectDesign(m_RdgnKey, SectData))	return;

	// Set TEE Section Data
	if(SectData.SectBefore.Shape==D_SECT_SHAPE_REG_T)
	{
		ShowTEEBitmap();
		ShowHideSectCtrl(TRUE);

		GetDlgItem(IDC_DGN_RC_BSHAPE)->SetWindowText(_LS(IDS_DGN_GRID_TEXT_SECT_TEE));  
		m_Hc.SetEditUnit(SectData.SectBefore.SectI.Size[0]);   //  H
		m_bf.SetEditUnit(SectData.SectBefore.SectI.Size[1]);   //  B
		m_Bc.SetEditUnit(SectData.SectBefore.SectI.Size[2]);   //  Tw
		m_hf.SetEditUnit(SectData.SectBefore.SectI.Size[3]);   //  Tf1
	}
	// SEt RECT Section Data
	else if(SectData.SectBefore.Shape==D_SECT_SHAPE_REG_SB)
	{
		ShowRECTBitmap();
		ShowHideSectCtrl(FALSE);

		GetDlgItem(IDC_DGN_RC_BSHAPE)->SetWindowText(_LS(IDS_DGN_GRID_TEXT_SECT_RECT));
		m_Hc.SetEditUnit(SectData.SectBefore.SectI.Size[0]);   //  H
		m_Bc.SetEditUnit(SectData.SectBefore.SectI.Size[1]);   //  B
	}

	m_RdgnData.Initialize();
	m_pDoc->m_pAttrCtrl->GetRdgn(m_RdgnKey, m_RdgnData);

	// Init Data
	m_dT.SetEditUnit(m_RdgnData.BEAM.dDt[0]);
	m_dB.SetEditUnit(m_RdgnData.BEAM.dDb[0]);
//	m_SubNum.SelectString(-1, m_RdgnData.BEAM.strSubBarNum);
	m_SubNum.SetEditUnit(m_RdgnData.BEAM.dSubBarNum[0]);
//  m_SubNum.SetEditUnit(m_RdgnData.BEAM.iSubBarNum);

	if(m_SubCB.SelectString(-1, m_RdgnData.BEAM.strSubBarNa[0])==LB_ERR) 
		m_SubCB.SetCurSel(-1);

	// Torsional Reinforcement
	m_bTorsionalBar = m_RdgnData.BEAM.bTorsionalBar[0];

	if(m_cmbTorBar.SelectString(-1, m_RdgnData.BEAM.strTorBarNa[0])==LB_ERR) 
		m_cmbTorBar.SetCurSel(-1);

	m_bBundledBar = m_RdgnData.BEAM.bBundledBar[0];
	m_edtBundledBarNum.SetEditUnit(m_RdgnData.BEAM.dBundledBarNum[0]);
	if(m_cmbLongiBar.SelectString(-1, m_RdgnData.BEAM.strLongiBarNa[0])==LB_ERR) 
		m_cmbLongiBar.SetCurSel(-1);
	m_edtLongiBarNum.SetEditUnit(m_RdgnData.BEAM.dLongiBarNum[0]);

	UpdateData(FALSE);
}

void CDgnConBeamReinDesDlgCIVIL::ControlsEnableDisable()
{
	UpdateData(TRUE);

	CDgnDataCtrl DataCtrl;
	T_DCON_D DconD; DconD.Initialize();
	DataCtrl.Get_DgnConDcon(DconD);

	BOOL bEnable = FALSE;

	if(DconD.DesignCode== CONCODE_SNIP_2_05_03_84 || DconD.DesignCode== CONCODE_SP_35_13330_2011 ||
		 DconD.DesignCode == CONCODE_SNIP_2_05_03_84_MKS || DconD.DesignCode == CONCODE_SP_35_13330_2011_MKS) bEnable = TRUE;

	GetDlgItem(IDC_RDGN_TOR_CHK)->EnableWindow(bEnable);

	SetWindowTheme(GetDlgItem(IDC_RDGN_TOR_STIRRUP_GRP)->GetSafeHwnd(), _T(""), _T(""));	
	GetDlgItem(IDC_RDGN_TOR_STIRRUP_GRP     )->EnableWindow(bEnable && m_bTorsionalBar);
	GetDlgItem(IDC_RDGN_TOR_STIRRUP_SIZE_STC)->EnableWindow(bEnable && m_bTorsionalBar);
	GetDlgItem(IDC_RDGN_TOR_STIRRUP_SIZE_CMB)->EnableWindow(bEnable && m_bTorsionalBar);
	GetDlgItem(IDC_RDGN_TOR_BUNDLED_CHK     )->EnableWindow(bEnable && m_bTorsionalBar);
	GetDlgItem(IDC_RDGN_TOR_BUNDLED_NUM_STC )->EnableWindow(bEnable && m_bTorsionalBar && m_bBundledBar);
	GetDlgItem(IDC_RDGN_TOR_BUNDLED_NUM_EDT )->EnableWindow(bEnable && m_bTorsionalBar && m_bBundledBar);

	SetWindowTheme(GetDlgItem(IDC_RDGN_TOR_LONGI_GRP)->GetSafeHwnd(), _T(""), _T(""));	
	GetDlgItem(IDC_RDGN_TOR_LONGI_GRP       )->EnableWindow(bEnable && m_bTorsionalBar);
	GetDlgItem(IDC_RDGN_TOR_LONGI_SIZE_STC  )->EnableWindow(bEnable && m_bTorsionalBar);
	GetDlgItem(IDC_RDGN_TOR_LONGI_SIZE_CMB  )->EnableWindow(bEnable && m_bTorsionalBar);
	GetDlgItem(IDC_RDGN_TOR_LONGI_NUM_STC   )->EnableWindow(bEnable && m_bTorsionalBar);
	GetDlgItem(IDC_RDGN_TOR_LONGI_NUM_EDT   )->EnableWindow(bEnable && m_bTorsionalBar);

	if (DconD.DesignCode == _T("AREMA-2023")) {

		//SetWindowTheme(GetDlgItem(IDC_RDGN_TOR_LONGI_GRP)->GetSafeHwnd(), _T(""), _T(""));
		//IDC_RDGN_TOR_GRP
		GetDlgItem(IDC_RDGN_TOR_GRP)->ShowWindow(bEnable);
		GetDlgItem(IDC_RDGN_TOR_STIRRUP_GRP)->ShowWindow(bEnable);
		GetDlgItem(IDC_RDGN_TOR_STIRRUP_SIZE_STC)->ShowWindow(bEnable);
		GetDlgItem(IDC_RDGN_TOR_STIRRUP_SIZE_CMB)->ShowWindow(bEnable);
		GetDlgItem(IDC_RDGN_TOR_BUNDLED_CHK)->ShowWindow(bEnable);
		GetDlgItem(IDC_RDGN_TOR_BUNDLED_NUM_STC)->ShowWindow(bEnable);
		GetDlgItem(IDC_RDGN_TOR_BUNDLED_NUM_EDT)->ShowWindow(bEnable);
		GetDlgItem(IDC_RDGN_TOR_CHK)->ShowWindow(bEnable);
		GetDlgItem(IDC_RDGN_TOR_LONGI_GRP)->ShowWindow(bEnable);
		GetDlgItem(IDC_RDGN_TOR_LONGI_SIZE_STC)->ShowWindow(bEnable);
		GetDlgItem(IDC_RDGN_TOR_LONGI_SIZE_CMB)->ShowWindow(bEnable);
		GetDlgItem(IDC_RDGN_TOR_LONGI_NUM_STC)->ShowWindow(bEnable);
		GetDlgItem(IDC_RDGN_TOR_LONGI_NUM_EDT)->ShowWindow(bEnable);
	}
}



