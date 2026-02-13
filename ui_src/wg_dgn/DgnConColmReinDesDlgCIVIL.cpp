// DgnConColmDgnDlgCIVIL.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConColmReinDesDlgCIVIL.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"

#include "..\wg_main\wg_mainRes2.h"
#include "DgnDataCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConColmReinDesDlgCIVIL dialog


CDgnConColmReinDesDlgCIVIL::CDgnConColmReinDesDlgCIVIL(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnConColmReinDesDlgCIVIL::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConColmReinDesDlgCIVIL)
	//}}AFX_DATA_INIT

	m_RdgnData.Initialize();

	m_BGroup.Add(IDC_DGN_COLB_STATIC);
	m_BGroup.Add(IDC_DGN_COLB);
	m_BGroup.Add(IDC_DGN_COLB_UNIT);

	m_HGroup.Add(IDC_DGN_COLH_STATIC);
	m_HGroup.Add(IDC_DGN_COLH);
	m_HGroup.Add(IDC_DGN_COLH_UNIT);

	m_tGroup.Add(IDC_DGN_COLt_STATIC);
	m_tGroup.Add(IDC_DGN_COLt);
	m_tGroup.Add(IDC_DGN_COLt_UNIT);

	m_abGroup.Add(IDC_DGN_COLa_STATIC);
	m_abGroup.Add(IDC_DGN_COLa);
	m_abGroup.Add(IDC_DGN_COLa_UNIT);
	m_abGroup.Add(IDC_DGN_COLb2_STATIC);
	m_abGroup.Add(IDC_DGN_COLb2);
	m_abGroup.Add(IDC_DGN_COLb2_UNIT);

	if(m_arColmData.GetCount()>0) m_arColmData.RemoveAll();

	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnConColmReinDesDlgCIVIL::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConColmReinDesDlgCIVIL)
	DDX_Control(pDX, IDC_DGN_DELETE, m_btnDel);
	DDX_Control(pDX, IDOK, m_btnAdd);
	DDX_Control(pDX, IDC_COLM_SECT_LIST, m_wndList);	
	
	DDX_Control(pDX, IDC_STIRRUP_NUM_CB, m_barnum);
	DDX_Control(pDX, IDC_STIRRUP_CB, m_StirrupCB);
	DDX_Control(pDX, IDC_DGN_COLt_UNIT, m_tunit);
	DDX_Control(pDX, IDC_DGN_COLt, m_t);
	DDX_Control(pDX, IDC_DGN_COLH_UNIT, m_Hunit);
	DDX_Control(pDX, IDC_DGN_COLH, m_H);
	DDX_Control(pDX, IDC_DGN_COLb2_UNIT, m_bunit);
	DDX_Control(pDX, IDC_DGN_COLb2, m_b);
	DDX_Control(pDX, IDC_DGN_COLB_UNIT, m_Bunit);
	DDX_Control(pDX, IDC_DGN_COLB, m_B);
	DDX_Control(pDX, IDC_DGN_COLa_UNIT, m_aunit);
	DDX_Control(pDX, IDC_DGN_COLa, m_a);
	DDX_Control(pDX, IDC_Dc_UNIT, m_Dcunit);
	DDX_Control(pDX, IDC_Dc_EDIT, m_Dc);
	
	DDX_Control(pDX, IDC_DGN_RECT_FRAME, m_wndPictureRect);
	DDX_Control(pDX, IDC_DGN_SRECT_FRAME, m_wndPictureRectSolid);
	DDX_Control(pDX, IDC_DGN_CIRC_FRAME, m_wndPictureCirc);
	DDX_Control(pDX, IDC_DGN_SCIRC_FRAME, m_wndPictureCircSolid);
	DDX_Control(pDX, IDC_DGN_OCTA_FRAME, m_wndPictureOcta);
	DDX_Control(pDX, IDC_DGN_SOCTA_FRAME, m_wndPictureOctaSolid);
	DDX_Control(pDX, IDC_DGN_TRACK_FRAME, m_wndPictureTrac);
	DDX_Control(pDX, IDC_DGN_STRACK_FRAME, m_wndPictureTracSolid);
	DDX_Control(pDX, IDC_DGN_HALF_FRAME, m_wndPictureTracHalf);

	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnConColmReinDesDlgCIVIL, CDialogMove)
	//{{AFX_MSG_MAP(CDgnConColmReinDesDlgCIVIL)
	ON_BN_CLICKED(IDC_DGN_DESIGN_SECTION, OnDgnDesignSection)
 	ON_BN_CLICKED(IDC_DGN_DELETE, OnDgnDelete)  
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_COLM_SECT_LIST, OnItemchangedDgnSectList)	
	ON_NOTIFY(NM_CLICK, IDC_COLM_SECT_LIST, OnClickDgnSectList)    
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConColmReinDesDlgCIVIL message handlers

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// Initialize 관련 
//

void CDgnConColmReinDesDlgCIVIL::SetInitStirCB()
{
	if(m_StirrupCB.GetCount()>0) m_StirrupCB.ResetContent();
	CDBLib::GetRebarNameAtComboBox(&m_StirrupCB);
/*
	if(m_barnum.GetCount()>0) m_barnum.ResetContent(); 
	CStringArray aRebarNum;
	if(CDBLib::GetSubRebarNumList_Civil(aRebarNum))
	{
		for(int i=0; i<aRebarNum.GetSize(); i++)	m_barnum.AddString(aRebarNum[i]);
	}
*/
}

void CDgnConColmReinDesDlgCIVIL::SetInitUnit()
{
	m_H.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_B.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_a.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_b.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_t.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Dc.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_Hunit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Bunit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_aunit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_bunit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_tunit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Dcunit.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_barnum.SetUnitType(D_UNITSYS_NONE);
}

void CDgnConColmReinDesDlgCIVIL::SetData2Dlg()
{
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);

	m_H.SetEditUnit(0);
	m_B.SetEditUnit(0);
	m_a.SetEditUnit(0);
	m_b.SetEditUnit(0);
	m_t.SetEditUnit(0);

	m_H.EnableWindow(FALSE);
	m_B.EnableWindow(FALSE);
	m_a.EnableWindow(FALSE);
	m_b.EnableWindow(FALSE);
	m_t.EnableWindow(FALSE);
	GetDlgItem(IDC_DGN_SHAPE)->EnableWindow(FALSE);

	m_StirrupCB.SetCurSel(-1);
	m_Dc.SetEditUnit(0);
	m_barnum.SetEditUnit(0);

	// Bitmap
	CString str;
	str.Format(_T("%s"), D_SECT_SHAPE_REG_SB);
	ShowBitmap(str);
	ShowHideControl(str);

	UpdateData(FALSE);
}

BOOL CDgnConColmReinDesDlgCIVIL::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	SetHeaderTitle();
	MakeItemEx();
	
	SetInitStirCB();
	SetInitUnit();
	SetData2Dlg();

	m_wndPictureRect.SetImage(_T("SVG\\Illustration\\Dialog\\dgn_colm_rect_dgn_civil.svg"));
	m_wndPictureRectSolid.SetImage(_T("SVG\\Illustration\\Dialog\\dgn_colm_srect_dgn_civil.svg"));
	m_wndPictureCirc.SetImage(_T("SVG\\Illustration\\Dialog\\dgn_colm_circ_dgn_civil.svg"));
	m_wndPictureCircSolid.SetImage(_T("SVG\\Illustration\\Dialog\\dgn_colm_scir_dgn_civil.svg"));
	m_wndPictureOcta.SetImage(_T("SVG\\Illustration\\Dialog\\dgn_colm_octa_dgn_civil.svg"));
	m_wndPictureOctaSolid.SetImage(_T("SVG\\Illustration\\Dialog\\dgn_colm_socta_dgn_civil.svg"));
	m_wndPictureTrac.SetImage(_T("SVG\\Illustration\\Dialog\\dgn_colm_track_dgn_civil.svg"));
	m_wndPictureTracSolid.SetImage(_T("SVG\\Illustration\\Dialog\\dgn_colm_strack_dgn_civil.svg"));
	m_wndPictureTracHalf.SetImage(_T("SVG\\Illustration\\Dialog\\dgn_colm_half_dgn_civil.svg"));

	m_wndPictureRect.SetIgnoreLargeView(TRUE);
	m_wndPictureRectSolid.SetIgnoreLargeView(TRUE);
	m_wndPictureCirc.SetIgnoreLargeView(TRUE);
	m_wndPictureCircSolid.SetIgnoreLargeView(TRUE);
	m_wndPictureOctaSolid.SetIgnoreLargeView(TRUE);
	m_wndPictureRect.SetIgnoreLargeView(TRUE);
	m_wndPictureTrac.SetIgnoreLargeView(TRUE);
	m_wndPictureTracSolid.SetIgnoreLargeView(TRUE);
	m_wndPictureTracHalf.SetIgnoreLargeView(TRUE);
	
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

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnConColmReinDesDlgCIVIL::OnDgnDesignSection() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND,MAKEWPARAM(ID_STRUCT_PROP_SECT4DGN,0));	
	
}

void CDgnConColmReinDesDlgCIVIL::OnOK() 
{
	// TODO: Add extra validation here

	// get data
	UpdateData(TRUE);

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
void CDgnConColmReinDesDlgCIVIL::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}

void CDgnConColmReinDesDlgCIVIL::OnDgnDelete() 
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

void CDgnConColmReinDesDlgCIVIL::OnClickDgnSectList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_wndList.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	m_btnAdd.EnableWindow(nIndex != -1);
	m_btnDel.EnableWindow(nIndex != -1);

	*pResult = 0;
}

void CDgnConColmReinDesDlgCIVIL::OnItemchangedDgnSectList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	int nIndex = pNMListView->iItem; 
	ChangeItem(nIndex);
	*pResult = 0;
}


void CDgnConColmReinDesDlgCIVIL::ChangeItem(int nIndex)
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

	int iInx = m_StirrupCB.SelectString(-1, m_RdgnData.COLM.strSubBarNa[0]);
	m_StirrupCB.SetCurSel(iInx); 

	m_SectWnd.Draw_Shape(1, m_RdgnKey, FALSE, TRUE);
	m_SectWnd.Invalidate();
}

void CDgnConColmReinDesDlgCIVIL::SetHeaderTitle()
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

void CDgnConColmReinDesDlgCIVIL::MakeItemEx()
{
	CWaitCursor Cursor;

	m_wndList.DeleteAllItems();

	// Data Sort & Store
	if(m_arColmData.GetCount()>0) m_arColmData.RemoveAll();

	CArray<T_ELEM_K, T_ELEM_K> arElemKeyList;
	CMap<int,int,int,int> BeamSecMap; BeamSecMap.RemoveAll();

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
				if(nMembType == D_MBTP_BEAM && (SectData.SectBefore.Shape == _T("SB") || SectData.SectBefore.Shape == _T("T")) &&
					 SectData.nStype==D_SECT_TYPE_REGULAR) 
					 BeamSecMap.SetAt(ElemData.elpro,ElemData.elpro);

				if(nMembType == D_MBTP_COLUMN && 
					(SectData.SectBefore.Shape==D_SECT_SHAPE_REG_B    ||
					 SectData.SectBefore.Shape==D_SECT_SHAPE_REG_SB   || 
					 SectData.SectBefore.Shape==D_SECT_SHAPE_REG_P    ||
					 SectData.SectBefore.Shape==D_SECT_SHAPE_REG_SR   ||
					 SectData.SectBefore.Shape==D_SECT_SHAPE_REG_OCT  ||
					 SectData.SectBefore.Shape==D_SECT_SHAPE_REG_SOCT ||
					 SectData.SectBefore.Shape==D_SECT_SHAPE_REG_TRK  ||
					 SectData.SectBefore.Shape==D_SECT_SHAPE_REG_STRK ||
					 SectData.SectBefore.Shape==D_SECT_SHAPE_REG_HTRK )) m_arColmData.SetAt(ElemData.elpro, ElemData.elpro);
			}
		}
	}

	POSITION Pos = BeamSecMap.GetStartPosition();
	while(Pos)
	{
		int SectK=0;
		int rSectK=0;
		BeamSecMap.GetNextAssoc(Pos, SectK, rSectK);
		if(m_arColmData.Lookup(SectK, rSectK))
		{
			m_arColmData.RemoveKey(SectK);
		}
	}  

	T_SECT_D rData;
	Pos = m_arColmData.GetStartPosition();
	while(Pos)
	{
		int SectK=0;
		int rSectK=0;
		m_arColmData.GetNextAssoc(Pos, SectK, rSectK);
		if(m_arColmData.Lookup(SectK, rSectK))
		{
			m_pDoc->m_pAttrCtrl->GetSectDesign(rSectK,rData);
			InsertItem(rSectK,rData);
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// OnOK 관련 
//

BOOL CDgnConColmReinDesDlgCIVIL::CheckInputData()
{
	double dH;
	if(m_t.GetEditValue() != 0) dH = m_t.GetEditValue();
	else                        dH = m_B.GetEditValue()/2.0;  
	if(m_Dc.GetEditValue()<=0 || m_Dc.GetEditValue()>=dH)
	{
		AfxMessageBox(_LS(IDS_DGN_CON_COLM_Dc_Input_Err));
//    AfxMessageBox(_T("[오류] Dc 입력이 잘못되었습니다."));
		return FALSE;
	}
	if(m_barnum.GetEditValue()<=0)
	{
		AfxMessageBox(_LS(IDS_DGN_CON_COLM_Tied_Spiral_Num_Input_Err));
//    AfxMessageBox(_T("[오류] Tie/Spiral(Number) 입력이 적당하지 않습니다."));
		return FALSE;
	}

	return TRUE;
}

void CDgnConColmReinDesDlgCIVIL::SetDlg2Data()
{
	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	// 일단 0 으로 고정해두었다~	

	CString str = _T("");
	m_RdgnData.nType = 1;

	m_RdgnData.COLM.dDc[0] = m_Dc.GetEditValue();
	m_RdgnData.COLM.dSubBarNum[0] = m_barnum.GetEditValue();

	int nIndex = m_StirrupCB.GetCurSel();
	if(nIndex>=0) m_StirrupCB.GetLBText(nIndex, str);
	m_RdgnData.COLM.strSubBarNa[0] = str;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// Change PropNo ComboBox 관련 
//

//------------------------------------------------------------------
// ShowBitmap
//------------------------------------------------------------------

void CDgnConColmReinDesDlgCIVIL::ShowBitmap(CString& colType)
{
	CWnd* pWnd[9];
	pWnd[0] = GetDlgItem(IDC_DGN_RECT_FRAME);   ASSERT(pWnd[0]);
	pWnd[1] = GetDlgItem(IDC_DGN_SRECT_FRAME);  ASSERT(pWnd[1]);
	pWnd[2] = GetDlgItem(IDC_DGN_CIRC_FRAME);   ASSERT(pWnd[2]);
	pWnd[3] = GetDlgItem(IDC_DGN_SCIRC_FRAME);  ASSERT(pWnd[3]);
	pWnd[4] = GetDlgItem(IDC_DGN_OCTA_FRAME);   ASSERT(pWnd[4]);
	pWnd[5] = GetDlgItem(IDC_DGN_SOCTA_FRAME);  ASSERT(pWnd[5]);
	pWnd[6] = GetDlgItem(IDC_DGN_HALF_FRAME);   ASSERT(pWnd[6]);
	pWnd[7] = GetDlgItem(IDC_DGN_TRACK_FRAME);  ASSERT(pWnd[7]);
	pWnd[8] = GetDlgItem(IDC_DGN_STRACK_FRAME); ASSERT(pWnd[8]);

	for(int i=0; i<9; i++) pWnd[i]->ShowWindow(SW_HIDE);

	if(colType == D_SECT_SHAPE_REG_B)         pWnd[0]->ShowWindow(SW_SHOW);
	else if(colType == D_SECT_SHAPE_REG_SB)   pWnd[1]->ShowWindow(SW_SHOW);
	else if(colType == D_SECT_SHAPE_REG_P)    pWnd[2]->ShowWindow(SW_SHOW);
	else if(colType == D_SECT_SHAPE_REG_SR)   pWnd[3]->ShowWindow(SW_SHOW);
	else if(colType == D_SECT_SHAPE_REG_OCT)  pWnd[4]->ShowWindow(SW_SHOW);
	else if(colType == D_SECT_SHAPE_REG_SOCT) pWnd[5]->ShowWindow(SW_SHOW);
	else if(colType == D_SECT_SHAPE_REG_HTRK) pWnd[6]->ShowWindow(SW_SHOW);
	else if(colType == D_SECT_SHAPE_REG_TRK)  pWnd[7]->ShowWindow(SW_SHOW);
	else if(colType == D_SECT_SHAPE_REG_STRK) pWnd[8]->ShowWindow(SW_SHOW);
}


//------------------------------------------------------------------
// ShowHideControl
//------------------------------------------------------------------
void CDgnConColmReinDesDlgCIVIL::ShowHideControl(CString& colType)
{
	if(colType == D_SECT_SHAPE_REG_SR)
	{
		CDlgUtil::CtrlEnableDisable(this, m_BGroup, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_HGroup, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_abGroup, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_tGroup, FALSE);
		
		m_H.SetWindowText(_T("0"));
		m_a.SetWindowText(_T("0"));
		m_b.SetWindowText(_T("0"));
		m_t.SetWindowText(_T("0"));
		SetControlText(FALSE);
	}
	else if(colType == D_SECT_SHAPE_REG_P)
	{
		CDlgUtil::CtrlEnableDisable(this, m_BGroup, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_HGroup, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_abGroup, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_tGroup, TRUE);

		m_H.SetWindowText(_T("0"));
		m_a.SetWindowText(_T("0"));
		m_b.SetWindowText(_T("0"));
		SetControlText(FALSE);
	}
	else if(colType == D_SECT_SHAPE_REG_SB || colType == D_SECT_SHAPE_REG_HTRK || colType == D_SECT_SHAPE_REG_STRK)
	{
		CDlgUtil::CtrlEnableDisable(this, m_BGroup, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_HGroup, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_abGroup, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_tGroup, FALSE);

		m_a.SetWindowText(_T("0"));
		m_b.SetWindowText(_T("0"));
		m_t.SetWindowText(_T("0"));
		SetControlText(FALSE);
	}
	else if(colType == D_SECT_SHAPE_REG_TRK)
	{
		CDlgUtil::CtrlEnableDisable(this, m_BGroup, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_HGroup, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_abGroup, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_tGroup, TRUE);

		m_a.SetWindowText(_T("0"));
		m_b.SetWindowText(_T("0"));
		SetControlText(FALSE);
	}
	else if(colType == D_SECT_SHAPE_REG_SOCT)
	{
		CDlgUtil::CtrlEnableDisable(this, m_BGroup, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_HGroup, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_abGroup, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_tGroup, FALSE);

		m_t.SetWindowText(_T("0"));
		SetControlText(FALSE);
	}
	else if(colType == D_SECT_SHAPE_REG_OCT)
	{
		CDlgUtil::CtrlEnableDisable(this, m_BGroup, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_HGroup, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_abGroup, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_tGroup, TRUE);
		SetControlText(FALSE);
	}
	else if(colType == D_SECT_SHAPE_REG_B)
	{
		CDlgUtil::CtrlEnableDisable(this, m_BGroup, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_HGroup, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_abGroup, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_tGroup, TRUE);
		SetControlText(TRUE);
	}
}

void CDgnConColmReinDesDlgCIVIL::SetControlText(BOOL bBox)
{
	if(bBox == TRUE)
	{
		GetDlgItem(IDC_DGN_COLa_STATIC)->SetWindowText(_T("tf1"));
		GetDlgItem(IDC_DGN_COLb2_STATIC)->SetWindowText(_T("tf2"));;
		GetDlgItem(IDC_DGN_COLt_STATIC)->SetWindowText(_T("tw"));;
	}
	else
	{
		GetDlgItem(IDC_DGN_COLa_STATIC)->SetWindowText(_T("a"));
		GetDlgItem(IDC_DGN_COLb2_STATIC)->SetWindowText(_T("b"));
		GetDlgItem(IDC_DGN_COLt_STATIC)->SetWindowText(_T("t"));
	}
}

//---------------------------------------------------------------------------
// SelectElemOnView 
//---------------------------------------------------------------------------
void CDgnConColmReinDesDlgCIVIL::SelectElemOnView()
{
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL, FALSE);

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
					if(DataCtrl.Get_DgnGenMbtp(ElemKey) == D_MBTP_COLUMN)  arElemKeyList.Add(ElemKey);
		}
	}

	m_pDoc->m_pViewCtrl->SelectElem(NULL, arElemKeyList, FALSE);
}

//---------------------------------------------------------------------------
// ChangeSectionInfo 
//---------------------------------------------------------------------------
void CDgnConColmReinDesDlgCIVIL::ChangeSectionInfo()
{
	// Select Elem. On View
	SelectElemOnView();

	T_SECT_D SectData; SectData.Initialize();

	if(!m_pDoc->m_pAttrCtrl->GetSectDesign(m_RdgnKey, SectData))	return;

	CString str = _T("");
	if(SectData.SectBefore.Shape == D_SECT_SHAPE_REG_SR)
	{
		GetDlgItem(IDC_DGN_SHAPE)->SetWindowText(_LS(IDS_DGN_COLM_TEXT_SECT_SRND));//(_T("Solid Round")); 
		str.Format(_T("%s"), D_SECT_SHAPE_REG_SR);
		m_B.SetEditUnit(SectData.SectBefore.SectI.Size[0]);
	}
	else if(SectData.SectBefore.Shape == D_SECT_SHAPE_REG_P)
	{
		GetDlgItem(IDC_DGN_SHAPE)->SetWindowText(_LS(IDS_DGN_COLM_TEXT_SECT_PIPE));//(_T("Pipe")); 
		str.Format(_T("%s"), D_SECT_SHAPE_REG_P);
		m_B.SetEditUnit(SectData.SectBefore.SectI.Size[0]); 
		m_t.SetEditUnit(SectData.SectBefore.SectI.Size[1]);
	}
	else if(SectData.SectBefore.Shape == D_SECT_SHAPE_REG_SB)
	{
		GetDlgItem(IDC_DGN_SHAPE)->SetWindowText(_LS(IDS_DGN_COLM_TEXT_SECT_SREC));//(_T("Solid Rectangle")); 
		str.Format(_T("%s"), D_SECT_SHAPE_REG_SB);
		m_H.SetEditUnit(SectData.SectBefore.SectI.Size[0]);
		m_B.SetEditUnit(SectData.SectBefore.SectI.Size[1]);
	}
	else if(SectData.SectBefore.Shape == D_SECT_SHAPE_REG_B)
	{
		GetDlgItem(IDC_DGN_SHAPE)->SetWindowText(_LS(IDS_DGN_COLM_TEXT_SECT_BOX));//(_T("Box")); 
		str.Format(_T("%s"), D_SECT_SHAPE_REG_B);
		double tf2;
		if(SectData.SectBefore.SectI.Size[5] == 0) tf2 = SectData.SectBefore.SectI.Size[3];
		m_H.SetEditUnit(SectData.SectBefore.SectI.Size[0]); 
		m_B.SetEditUnit(SectData.SectBefore.SectI.Size[1]);
		m_a.SetEditUnit(SectData.SectBefore.SectI.Size[3]);
		m_b.SetEditUnit(tf2);
		m_t.SetEditUnit(SectData.SectBefore.SectI.Size[2]);
	}
	else if(SectData.SectBefore.Shape == D_SECT_SHAPE_REG_OCT)
	{
		GetDlgItem(IDC_DGN_SHAPE)->SetWindowText(_LS(IDS_DGN_COLM_TEXT_SECT_OCTA));//(_T("Octagon")); 
		str.Format(_T("%s"), D_SECT_SHAPE_REG_OCT);
		m_H.SetEditUnit(SectData.SectBefore.SectI.Size[0]);
		m_B.SetEditUnit(SectData.SectBefore.SectI.Size[1]);
		m_a.SetEditUnit(SectData.SectBefore.SectI.Size[2]);
		m_b.SetEditUnit(SectData.SectBefore.SectI.Size[3]);
		m_t.SetEditUnit(SectData.SectBefore.SectI.Size[4]);
	}
	else if(SectData.SectBefore.Shape == D_SECT_SHAPE_REG_SOCT)
	{
		GetDlgItem(IDC_DGN_SHAPE)->SetWindowText(_LS(IDS_DGN_COLM_TEXT_SECT_SOCT));//(_T("Solid Octagon")); 
		str.Format(_T("%s"), D_SECT_SHAPE_REG_SOCT);
		m_H.SetEditUnit(SectData.SectBefore.SectI.Size[0]);
		m_B.SetEditUnit(SectData.SectBefore.SectI.Size[1]);
		m_a.SetEditUnit(SectData.SectBefore.SectI.Size[2]);
		m_b.SetEditUnit(SectData.SectBefore.SectI.Size[3]);
	}
	else if(SectData.SectBefore.Shape == D_SECT_SHAPE_REG_TRK)
	{
		GetDlgItem(IDC_DGN_SHAPE)->SetWindowText(_LS(IDS_DGN_COLM_TEXT_SECT_TRCK));//(_T("Track")); 
		str.Format(_T("%s"), D_SECT_SHAPE_REG_TRK);
		m_H.SetEditUnit(SectData.SectBefore.SectI.Size[0]);
		m_B.SetEditUnit(SectData.SectBefore.SectI.Size[1]);
		m_t.SetEditUnit(SectData.SectBefore.SectI.Size[2]);
	}
	else if(SectData.SectBefore.Shape == D_SECT_SHAPE_REG_STRK)
	{
		GetDlgItem(IDC_DGN_SHAPE)->SetWindowText(_LS(IDS_DGN_COLM_TEXT_SECT_STRK));//(_T("Solid Track")); 
		str.Format(_T("%s"), D_SECT_SHAPE_REG_STRK);
		m_H.SetEditUnit(SectData.SectBefore.SectI.Size[0]);
		m_B.SetEditUnit(SectData.SectBefore.SectI.Size[1]);
	}
	else if(SectData.SectBefore.Shape == D_SECT_SHAPE_REG_HTRK)
	{
		GetDlgItem(IDC_DGN_SHAPE)->SetWindowText(_LS(IDS_DGN_COLM_TEXT_SECT_HTRK));//(_T("Half Track")); 
		str.Format(_T("%s"), D_SECT_SHAPE_REG_HTRK);
		m_H.SetEditUnit(SectData.SectBefore.SectI.Size[0]);
		m_B.SetEditUnit(SectData.SectBefore.SectI.Size[1]);
	}
	ShowBitmap(str);
	ShowHideControl(str);
	
	m_RdgnData.Initialize();
	m_pDoc->m_pAttrCtrl->GetRdgn(m_RdgnKey, m_RdgnData);

	// Init Data
	m_Dc.SetEditUnit(m_RdgnData.COLM.dDc[0]);
	m_barnum.SetEditUnit(m_RdgnData.COLM.dSubBarNum[0]);
//	m_barnum.SelectString(-1, m_RdgnData.COLM.dSubBarNum);
	m_StirrupCB.SelectString(-1, m_RdgnData.COLM.strSubBarNa[0]); 
}

BOOL CDgnConColmReinDesDlgCIVIL::InsertItem(T_SECT_K Key, T_SECT_D &Data)
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

int CDgnConColmReinDesDlgCIVIL::FindInsertionPos(T_SECT_K nID)
{
	int nCount = m_wndList.GetItemCount();
	for (int i=0; i < nCount; i++)
	{
        T_SECT_K key = m_wndList.GetItemData(i);
        if ( nID < key ) { return i; }
	}
	return nCount;
}

CString CDgnConColmReinDesDlgCIVIL::DataToStr(int i, T_SECT_K Key, T_SECT_D &Data, T_RDGN_D &RData)
{  
	T_RDGN_COLM rDgnColm;
	rDgnColm = RData.COLM;

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

BOOL CDgnConColmReinDesDlgCIVIL::SelectSection(CArray<T_RDGN_K, T_RDGN_K>&  arRdgnKey)
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

void CDgnConColmReinDesDlgCIVIL::CheckDiffSections(T_SECT_D OrginSectData, T_SECT_D SectData, int& iResult)
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
// DB 관련 
//
/*
void CDgnConColmReinDesDlgCIVIL::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
		case D_UPDATE_DEFAULT:
		// do something...
			break;
		case D_UPDATE_BUFFER_BEFORE:
		// do something...
			break;
		case D_UPDATE_UNIT:
		// do something...
			break;
		case D_UPDATE_SEL_ADD:
		// do something...
			break;
		case D_UPDATE_SEL_DEL:
		// do something...
			break;
		case D_UPDATE_BUFFER_AFTER:
			UpdateBuffer();
			break;
		case D_UPDATE_PREF_CHANGED:
			// Add by ZINU.('02.10.16). Update changed rebar kind.
			if(m_StirrupCB.GetCount()>0) m_StirrupCB.ResetContent();
			CDBLib::GetRebarNameAtComboBox(&m_StirrupCB);
			break;
		default:
			//ASSERT(FALSE);
			break;
	  }
}

void CDgnConColmReinDesDlgCIVIL::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if(nCount == 0) return; 
	ASSERT(nCount == 1);

	BOOL bSECD = FALSE;
	BOOL bDEFT = FALSE;

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
			// 다른 case도 추가해주어야 한다.
			case(UR_SECD_ADD): 
			case(UR_SECD_DEL): 
			case(UR_SECD_MFS): 
			case(UR_SECD_MFD):
			case(UR_RDGN_ADD): 
			case(UR_RDGN_DEL):  { bSECD = TRUE; break; }
			default:						{ bDEFT = TRUE;	break; }
		}
	}

	if(bSECD) ResetSectGridData(TRUE);
}
*/
