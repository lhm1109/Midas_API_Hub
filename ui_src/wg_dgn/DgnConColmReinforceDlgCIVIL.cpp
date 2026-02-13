// DgnConColmReinforceDlgCIVIL.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConColmReinforceDlgCIVIL.h"
#include "DgnConColmReinforceRebarDlgCIVIL.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#include "..\wg_main\wg_mainRes2.h"
#include "DgnBeamGridWndCIVIL.h"
#include "DgnDataCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConColmReinforceDlgCIVIL dialog


CDgnConColmReinforceDlgCIVIL::CDgnConColmReinforceDlgCIVIL(CWnd* pParent /*=NULL*/)
	: CChildDialog(CDgnConColmReinforceDlgCIVIL::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConColmReinforceDlgCIVIL)
	m_nType = 0;
	//}}AFX_DATA_INIT

	//m_pParent = (CDgnConColmReinforceDlgRebarCIVIL*)pParent;

	if(m_arColmData.GetCount()>0) m_arColmData.RemoveAll();

	m_RchkKey = -1;
	m_RchkData.Initialize();

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

	m_Tie1stType.Add(IDC_DGN_SIZE_CB);
  //m_Tie1stType.Add(IDC_DGN_NUM_CB);
	
	m_Tie2ndType.Add(IDC_DGN_TIE_Y_SIZE_STC);
	m_Tie2ndType.Add(IDC_DGN_TIE_Z_SIZE_STC);
	m_Tie2ndType.Add(IDC_DGN_TIE_Y_SIZE_CB );
	m_Tie2ndType.Add(IDC_DGN_TIE_Z_SIZE_CB );

	m_TieTypeNum.Add(IDC_DGN_TIE_Y_NUM_STC );
	m_TieTypeNum.Add(IDC_DGN_TIE_Z_NUM_STC );
	m_TieTypeNum.Add(IDC_DGN_TIE_Y_NUM_EDT );
	m_TieTypeNum.Add(IDC_DGN_TIE_Z_NUM_EDT );

	m_pDoc = CDBDoc::GetDocPoint();
	m_bTieSecondType = FALSE;

	m_ColGrid = NULL;
}


void CDgnConColmReinforceDlgCIVIL::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConColmReinforceDlgCIVIL)
	//DDX_Control(pDX, IDC_DGN_DELETE, m_btnDel);
	//DDX_Control(pDX, IDOK, m_btnAdd);
	DDX_Control(pDX, IDC_REDRAW_BTN, m_btnRedraw);
	DDX_Control(pDX, IDC_COLM_SECT_LIST, m_wndList);	

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

	DDX_Control(pDX, IDC_DGN_RC_As_UT, m_AsUT);
	DDX_Control(pDX, IDC_DGN_RC_As, m_As);
	DDX_Control(pDX, IDC_DGN_NUM_CB, m_Num);	
	DDX_Control(pDX, IDC_DGN_TIE_Y_NUM_EDT, m_YNum_2nd);
	DDX_Control(pDX, IDC_DGN_TIE_Z_NUM_EDT, m_ZNum_2nd);
	DDX_Control(pDX, IDC_DGN_SPACE_UNIT, m_SpaceUT);
	DDX_Control(pDX, IDC_DGN_SPACE, m_Space);
	DDX_Control(pDX, IDC_DGN_SIZE_CB, m_SizeCB);
	DDX_Control(pDX, IDC_DGN_TIE_Y_SIZE_CB, m_SizeYCB_2nd);
	DDX_Control(pDX, IDC_DGN_TIE_Z_SIZE_CB, m_SizeZCB_2nd);
	DDX_Control(pDX, IDC_DGN_LAYER_CB, m_LayerCB);
	DDX_Radio(pDX, IDC_DGN_HOOP_TYPE_RD1, m_nType);

	DDX_Control(pDX, IDC_DGN_GRID, *m_ColGrid);
	
	DDX_Control(pDX, IDC_DGN_RECT_FRAME, m_wndPicRect);
	DDX_Control(pDX, IDC_DGN_SRECT_FRAME, m_wndPicRectSolid);
	DDX_Control(pDX, IDC_DGN_CIRC_FRAME, m_wndPicCirc);
	DDX_Control(pDX, IDC_DGN_SCIRC_FRAME, m_wndPicCircSolid);
	DDX_Control(pDX, IDC_DGN_OCTA_FRAME, m_wndPicOcta);
	DDX_Control(pDX, IDC_DGN_SOCTA_FRAME, m_wndPicOctaSolid);
	DDX_Control(pDX, IDC_DGN_HALF_FRAME, m_wndPicTracHalf);
	DDX_Control(pDX, IDC_DGN_TRACK_FRAME, m_wndPicTrac);
	DDX_Control(pDX, IDC_DGN_STRACK_FRAME, m_wndPicTracSolid);
	DDX_Control(pDX, IDC_DGN_H_FRAME, m_wndPicH);

	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnConColmReinforceDlgCIVIL, CChildDialog)
	//{{AFX_MSG_MAP(CDgnConColmReinforceDlgCIVIL)
	ON_BN_CLICKED(IDC_DGN_DESIGN_SECTION, OnDgnDesignSection)
 	ON_BN_CLICKED(IDC_DGN_DELETE, OnDgnDelete)  
	ON_BN_CLICKED(IDC_REDRAW_BTN, OnRedrawBtn)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_COLM_SECT_LIST, OnItemchangedDgnSectList)	
	ON_NOTIFY(NM_CLICK, IDC_COLM_SECT_LIST, OnClickDgnSectList)  
	ON_CBN_SELCHANGE(IDC_DGN_LAYER_CB, OnSelchangeDgnLayerCb)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConColmReinforceDlgCIVIL message handlers


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//
//  Initialize 관련 
//

void CDgnConColmReinforceDlgCIVIL::SetInitCtrl_TieRbar()
{
	CRect rectOrg, rectCrr;
	m_SizeCB.GetWindowRect(rectOrg);
	GetDlgItem(IDC_DGN_TIE_Y_SIZE_CB)->GetWindowRect(rectCrr);

	CDlgUtil::CtrlMoveDistX(this, m_Tie2ndType, (rectOrg.left - rectCrr.left), TRUE);
	CDlgUtil::CtrlMoveDistX(this, m_TieTypeNum, (rectOrg.left - rectCrr.left), TRUE);
}
//------------------------------------------------------------------
// Init Unit
//------------------------------------------------------------------
void CDgnConColmReinforceDlgCIVIL::SetInitUnit()
{
	m_t.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_H.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_b.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_B.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_a.SetUnitType(D_UNITSYS_BASE_LENGTH);
	
	m_tunit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Hunit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_bunit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Bunit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_aunit.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

//------------------------------------------------------------------
// Init Data
//------------------------------------------------------------------
void CDgnConColmReinforceDlgCIVIL::SetData2Dlg()
{
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);

	m_Shape = _LS(IDS_DGN_COLM_TEXT_SECT_SREC);

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
	
	// Bitmap
	CString str;
	str.Format(_T("%s"), D_SECT_SHAPE_REG_SB);
	ShowBitmap(str);
	ShowHideControl(str);

	UpdateData(FALSE);
/*
	CString title=_T("");
	CGXStyle style;
	style.SetValue(title);
	style.SetInterior(GetSysColor(COLOR_3DFACE));
	style.SetControl(GX_IDS_CTRL_STATIC);
	m_InfoGrid.SetStyleRange(CGXRange().SetTable(), CGXStyle().SetReadOnly(FALSE));
	for(int i=1; i<6; i++)for(int j=1; j<4; j++) m_InfoGrid.SetStyleRange(CGXRange(i,j),style);
	m_InfoGrid.SetStyleRange(CGXRange().SetTable(), CGXStyle().SetReadOnly(TRUE));
	*/
}

void CDgnConColmReinforceDlgCIVIL::AddDgn()
{
	// get data

	UpdateData(TRUE);
	// 저장하기 전 Data의 유효성 여부 Check - Data 저장
	// Grid in Tab.
	CArray<T_RCHK_K, T_RCHK_K>  arRchKey;
	CArray<T_RCHK_D, T_RCHK_D&> arRchkData;
	int nItem = -1;
	int nSelCount = m_wndList.GetSelectedCount();
	arRchKey.SetSize(nSelCount);
	arRchkData.SetSize(nSelCount);
	for (UINT i = 0; i < nSelCount; i++)
	{
		nItem = m_wndList.GetNextItem(nItem, LVNI_SELECTED);
		ASSERT(nItem != -1);
		arRchKey[i] = m_wndList.GetItemData(nItem);
		m_arRchkKey.SetAt(i, m_wndList.GetItemData(nItem));
	}

	if (!SelectSection(arRchKey))
	{
		AfxMessageBox(_LS(IDS_DGN_CON_SELECT_DiffShpe_Err));
		return;
	}

	if (!CheckInputData_Rebar()) return;
	SetDlg2Data_Rebar();

	if (!CheckInputData()) return;
	if (SetDlg2Data()) SetData2Dlg();

	for (int i = 0; i < nSelCount; i++)
	{
		arRchkData[i] = m_RchkData;
	}

	if (!m_pDoc->m_pDataCtrl->AddRchk(arRchKey, arRchkData))
		return;

	MakeItemEx();

	int nIndex = m_wndList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	ChangeItem(nIndex);
}

void CDgnConColmReinforceDlgCIVIL::DeleteDgn()
{
	// TODO: Add your control notification handler code here
	T_RCHK_K key;
	CArray<T_RCHK_K, T_RCHK_K> aKey;
	int nItem = -1;

	int nSelCount = m_wndList.GetSelectedCount();
	for (UINT i=0; i < nSelCount; i++)
	{
		nItem = m_wndList.GetNextItem(nItem, LVNI_SELECTED);
		ASSERT(nItem != -1);
		key = m_wndList.GetItemData(nItem);
		if (m_pDoc->m_pAttrCtrl->ExistRchk(key)) aKey.Add(key);
	}

	if (!m_pDoc->m_pDataCtrl->DelRchk(aKey)) return;
	 
	MakeItemEx();
	int nIndex = m_wndList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	ChangeItem(nIndex);
}

void CDgnConColmReinforceDlgCIVIL::InitGridData()
{
	m_ColGrid->Initialize();  
	 
	m_ColGrid->HideRows(5,6);
}
//------------------------------------------------------------------
// OnInitDialog
//------------------------------------------------------------------
BOOL CDgnConColmReinforceDlgCIVIL::OnInitDialog() 
{
	if (m_ColGrid == nullptr)
		m_ColGrid = new CDgnColumnGridWndCIVIL();

	CChildDialog::OnInitDialog();

#if defined(_ORG)
	SetWindowText(_LS(IDS_DGN_CON_REIN_COLUMN_DLG_org));
#else
	SetWindowText(_LS(IDS_DGN_CON_REIN_COLUMN_DLG));
#endif

	SetHeaderTitle();
	MakeItemEx();
	
	// Tie Rebar
	SetInitCtrl_TieRbar();
	// 좌측 Section Data Control.
	SetInitUnit();
	SetData2Dlg();

// Right Data & Grid Control.

	// Init RebarCB
	SetInitRebarCB();

	// Init LayerCB
	SetInitLayerCB();

	// Init Grid
	InitGridData();

	// Init Data
	//SetData2Dlg_Rebar();

	m_wndPicRect.SetImage(_T("SVG\\Illustration\\Dialog\\dgn_colm_rect_civil.svg"));
	m_wndPicRectSolid.SetImage(_T("SVG\\Illustration\\Dialog\\dgn_colm_srect_civil.svg"));
	m_wndPicCirc.SetImage(_T("SVG\\Illustration\\Dialog\\dgn_colm_circ_civil.svg"));
	m_wndPicCircSolid.SetImage(_T("SVG\\Illustration\\Dialog\\dgn_colm_scirc_civil.svg"));
	m_wndPicOcta.SetImage(_T("SVG\\Illustration\\Dialog\\dgn_colm_octa_civil.svg"));
	m_wndPicOctaSolid.SetImage(_T("SVG\\Illustration\\Dialog\\dgn_colm_socta_civil.svg"));
	m_wndPicTracHalf.SetImage(_T("SVG\\Illustration\\Dialog\\dgn_colm_half_civil.svg"));
	m_wndPicTrac.SetImage(_T("SVG\\Illustration\\Dialog\\dgn_colm_track_civil.svg"));
	m_wndPicTracSolid.SetImage(_T("SVG\\Illustration\\Dialog\\dgn_colm_strack_civil.svg"));
	m_wndPicH.SetImage(_T("SVG\\Illustration\\Dialog\\dgn_colm_h_civil.svg"));

	// Init View
	CWnd* PlaceHolder = GetDlgItem(IDC_DGN_SP_ID_PREVIEW);
	ASSERT(PlaceHolder);
	m_SectWnd.Initial(PlaceHolder);

	((CDgnConColmReinforceRebarDlgCIVIL*)m_pParent)->SetStatusAddBtn(CDgnConColmReinforceRebarDlgCIVIL::DGNCON_BTN_DISABLE);
	((CDgnConColmReinforceRebarDlgCIVIL*)m_pParent)->SetStatusDeleteBtn(CDgnConColmReinforceRebarDlgCIVIL::DGNCON_BTN_DISABLE);
	//m_btnAdd.EnableWindow(FALSE);
	//m_btnDel.EnableWindow(FALSE);
	m_btnRedraw.EnableWindow(FALSE);

	if (m_bModify)
	{
		int nItemCount = m_wndList.GetItemCount();
        int ix = 0;
		for (ix = 0; ix < nItemCount; ix++)
			if (m_RchkKey == m_wndList.GetItemData(ix)) break;
		if (ix != nItemCount)
		{
			m_wndList.SetItemState(ix, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
			((CDgnConColmReinforceRebarDlgCIVIL*)m_pParent)->SetStatusAddBtn(CDgnConColmReinforceRebarDlgCIVIL::DGNCON_BTN_ENABLE);
			((CDgnConColmReinforceRebarDlgCIVIL*)m_pParent)->SetStatusDeleteBtn(CDgnConColmReinforceRebarDlgCIVIL::DGNCON_BTN_ENABLE);
			//m_btnAdd.EnableWindow(TRUE);
			//m_btnDel.EnableWindow(TRUE);
			m_btnRedraw.EnableWindow(TRUE);
		}
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnConColmReinforceDlgCIVIL::OnDgnDesignSection() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND,MAKEWPARAM(ID_STRUCT_PROP_SECT4DGN,0));	
	
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//
//  OnOK 관련 
//

//------------------------------------------------------------------
// OnOK
//------------------------------------------------------------------

void CDgnConColmReinforceDlgCIVIL::OnOK() 
{
	AddDgn();	
	
	//CChildDialog::OnOK();
}

void CDgnConColmReinforceDlgCIVIL::PostNcDestroy() 
{
	if(m_ColGrid != NULL){ delete m_ColGrid; m_ColGrid = NULL; }	

	CChildDialog::PostNcDestroy();
}

BOOL CDgnConColmReinforceDlgCIVIL::CheckInputData()
{
	// PropNo Check
	T_SECT_D sData;
	sData.Initialize();
	BOOL bGetOK = m_pDoc->m_pAttrCtrl->GetSectDesign(m_RchkKey, sData);
	
	if(!bGetOK){
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_Property_Num_Err));
//    AfxMessageBox(_T("Property Number 선택이 적당하지 않습니다."));
		return FALSE;
	}

	return TRUE;
}

BOOL CDgnConColmReinforceDlgCIVIL::SetDlg2Data()
{
	BOOL bCheck = TRUE;
	UpdateData(TRUE);

	m_RchkData.nType = 1;

	return bCheck;
}

// close
void CDgnConColmReinforceDlgCIVIL::OnCancel() 
{
	// TODO: Add extra cleanup here
	((CDialog*)m_pParent)->EndDialog(IDCANCEL);
	//CChildDialog::OnCancel();
}

void CDgnConColmReinforceDlgCIVIL::OnDgnDelete() 
{
	DeleteDgn();
}

void CDgnConColmReinforceDlgCIVIL::OnClickDgnSectList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_wndList.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	((CDgnConColmReinforceRebarDlgCIVIL*)m_pParent)->SetStatusAddBtn(nIndex != -1?CDgnConColmReinforceRebarDlgCIVIL::DGNCON_BTN_ENABLE
		: CDgnConColmReinforceRebarDlgCIVIL::DGNCON_BTN_DISABLE);
	((CDgnConColmReinforceRebarDlgCIVIL*)m_pParent)->SetStatusDeleteBtn(nIndex != -1 ? CDgnConColmReinforceRebarDlgCIVIL::DGNCON_BTN_ENABLE
		: CDgnConColmReinforceRebarDlgCIVIL::DGNCON_BTN_DISABLE);
	//m_btnAdd.EnableWindow(nIndex != -1);
	//m_btnDel.EnableWindow(nIndex != -1);
	m_btnRedraw.EnableWindow(nIndex != -1);
		
	*pResult = 0;
}

void CDgnConColmReinforceDlgCIVIL::OnItemchangedDgnSectList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
 
	int nIndex = pNMListView->iItem; 
	ChangeItem(nIndex);

	*pResult = 0;
}


void CDgnConColmReinforceDlgCIVIL::ChangeItem(int nIndex)
{
	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	// 일단 0 으로 고정해두었다~	

	// enable/disable
	((CDgnConColmReinforceRebarDlgCIVIL*)m_pParent)->SetStatusAddBtn(nIndex != -1 ? CDgnConColmReinforceRebarDlgCIVIL::DGNCON_BTN_ENABLE
		: CDgnConColmReinforceRebarDlgCIVIL::DGNCON_BTN_DISABLE);
	((CDgnConColmReinforceRebarDlgCIVIL*)m_pParent)->SetStatusDeleteBtn(nIndex != -1 ? CDgnConColmReinforceRebarDlgCIVIL::DGNCON_BTN_ENABLE
		: CDgnConColmReinforceRebarDlgCIVIL::DGNCON_BTN_DISABLE);
	//m_btnAdd.EnableWindow(nIndex != -1);
	//m_btnDel.EnableWindow(nIndex != -1);
	m_btnRedraw.EnableWindow(nIndex != -1);

	// get data
	m_RchkKey = 0;
	m_RchkData.Initialize();
	m_SectD.Initialize();  
	if (nIndex != -1)
	{
	  m_RchkKey = m_wndList.GetItemData(nIndex);
		if (!m_pDoc->m_pAttrCtrl->GetRchk(m_RchkKey, m_RchkData))
			m_RchkData.Initialize();
		m_pDoc->m_pAttrCtrl->GetSect(m_RchkKey, m_SectD);
	}

	// Left Section Data.
	ChangeSectionInfo();

	// Right Data & Rebar Data.
	SetRchkData(m_RchkKey, &(m_RchkData.COLM));

	if(m_SectD.SectBefore.Shape == D_SECT_SHAPE_REG_SB  ||
		 m_SectD.SectBefore.Shape == D_SECT_SHAPE_REG_B   ||
		 m_SectD.SectBefore.Shape == D_SECT_SHAPE_REG_TRK ||
		 m_SectD.SectBefore.Shape == D_SECT_SHAPE_REG_STRK||
		 m_SectD.SectBefore.Shape == D_SECT_SHAPE_REG_H)
	{
		m_ColGrid->HideCols(4,6,FALSE);
	}
	else if(m_SectD.SectBefore.Shape == D_SECT_SHAPE_REG_OCT  ||
					m_SectD.SectBefore.Shape == D_SECT_SHAPE_REG_SOCT ||
					m_SectD.SectBefore.Shape == D_SECT_SHAPE_REG_HTRK)
	{
		m_ColGrid->HideCols(4,9,FALSE);
	}
	
	SetData2Dlg_Rebar();
	
	int iInx = m_SizeCB.SelectString(-1, m_pColmData->strSubBarNa[0]);
	m_SizeCB.SetCurSel(iInx); 
	iInx = m_SizeYCB_2nd.SelectString(-1, m_pColmData->strSubBarNa_y[0]);
	m_SizeYCB_2nd.SetCurSel(iInx); 
	iInx = m_SizeZCB_2nd.SelectString(-1, m_pColmData->strSubBarNa_z[0]);
	m_SizeZCB_2nd.SetCurSel(iInx); 

	m_SectWnd.Draw_Shape(1, m_RchkKey, FALSE, TRUE);
	m_SectWnd.Draw_ChkRebar(m_RchkKey, 0, m_RchkData, FALSE, TRUE);
	m_SectWnd.Invalidate();
}

void CDgnConColmReinforceDlgCIVIL::UpdateViewWnd()
{
	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	// 일단 0 으로 고정해두었다~	
	
	T_RCHK_D RchkData; RchkData.Initialize();
	RchkData.nType = 1;

	CString str = _T("");

	for(int i=0; i<m_nLayerNum ; i++)
	{
		str = m_ColGrid->GetValueRowCol(i+2, 1);
		RchkData.COLM.iBarNum[0][0][i] = _ttoi(str);
		str = m_ColGrid->GetValueRowCol(i+2, 2);
		RchkData.COLM.strBarNa1[0][0][i] = str;
		str = m_ColGrid->GetValueRowCol(i+2, 3);
		RchkData.COLM.strBarNa2[0][0][i] = str;

		str = m_ColGrid->GetValueRowCol(i+2, 4);
		RchkData.COLM.iBarNum[0][1][i] = _ttoi(str);
		str = m_ColGrid->GetValueRowCol(i+2, 5);
		RchkData.COLM.strBarNa1[0][1][i] = str;
		str = m_ColGrid->GetValueRowCol(i+2, 6);
		RchkData.COLM.strBarNa2[0][1][i] = str;

		str = m_ColGrid->GetValueRowCol(i+2, 7);
		RchkData.COLM.iBarNum[0][2][i] = _ttoi(str);
		str = m_ColGrid->GetValueRowCol(i+2, 8);
		RchkData.COLM.strBarNa1[0][2][i] = str;
		str = m_ColGrid->GetValueRowCol(i+2, 9);
		RchkData.COLM.strBarNa2[0][2][i] = str;

		str = m_ColGrid->GetValueRowCol(i+2, 10);
		RchkData.COLM.dDc[0][i] = _tstof(str);
	}

	m_SectWnd.Draw_Shape(1, m_RchkKey, FALSE, TRUE);
	m_SectWnd.Draw_ChkRebar(m_RchkKey, 0, RchkData, FALSE, TRUE);
	m_SectWnd.Invalidate();
}

void CDgnConColmReinforceDlgCIVIL::SetHeaderTitle()
{
	CStringArray aTitle;
	CArray<int, int> aWidth;

#define M_ADD_LIST_INFO(TITLE, WIDTH) aTitle.Add(TITLE); aWidth.Add(WIDTH)
	M_ADD_LIST_INFO(_LS(IDS_DGN_LIST_ID)  ,   35);  //_T("ID")   
	M_ADD_LIST_INFO(_LS(IDS_DGN_LIST_NAME),   120); //_T("Name")
	M_ADD_LIST_INFO(_LS(IDS_DGN_LIST_BAR)  ,   70);  //_T("Bar") 
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

void CDgnConColmReinforceDlgCIVIL::MakeItemEx()
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

			if (SectData.nStype != D_SECT_TYPE_REGULAR && SectData.nStype != D_SECT_TYPE_USER) continue;

			T_DCON_D dconData; dconData.Initialize();
			m_pDoc->m_pAttrCtrl->GetDcon(dconData);
			bool bCH_Code = false;
			bCH_Code = (dconData.DesignCode == _T("JTJ023-85")		|| 
									dconData.DesignCode == _T("GB50010-02")		||
									dconData.DesignCode == _T("JTG D62-04")		||
									dconData.DesignCode == _T("CJJ11-2011")		||
									dconData.DesignCode == _T("TB 10002.3-05")	);
			bCH_Code = true;
			
			if(bCheck == TRUE) 
			{
				if(nMembType == D_MBTP_BEAM && (SectData.SectBefore.Shape == _T("SB") || SectData.SectBefore.Shape == _T("T")) &&
					 SectData.nStype==D_SECT_TYPE_REGULAR) 
					 BeamSecMap.SetAt(ElemData.elpro,ElemData.elpro);

				if(bCH_Code)
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
						 SectData.SectBefore.Shape==D_SECT_SHAPE_REG_HTRK ||
						 SectData.SectBefore.Shape==D_SECT_SHAPE_REG_H)) 

						 m_arColmData.SetAt(ElemData.elpro, ElemData.elpro);
				}
				else
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
						 SectData.SectBefore.Shape==D_SECT_SHAPE_REG_HTRK )) 

						 m_arColmData.SetAt(ElemData.elpro, ElemData.elpro);
				}
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

BOOL CDgnConColmReinforceDlgCIVIL::InsertItem(T_SECT_K Key, T_SECT_D &Data)
{
	
	T_RCHK_D RchkD;
	if (!m_pDoc->m_pAttrCtrl->GetRchk(Key, RchkD))
		RchkD.Initialize();

	LVITEM lvitem;
	CString str;
	int nItem = m_wndList.GetItemCount();

	lvitem.iItem = FindInsertionPos(Key);
	for(int i=0; i < 3; i++)
	{
		lvitem.iSubItem = i;
		str = DataToStr(i, Key, Data, RchkD);
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

int CDgnConColmReinforceDlgCIVIL::FindInsertionPos(T_SECT_K nID)
{
	int nCount = m_wndList.GetItemCount();
	for (int i=0; i < nCount; i++)
	{
        T_SECT_K key = m_wndList.GetItemData(i);
		if (nID < key) { return i; }
	}
	return nCount;
}

CString CDgnConColmReinforceDlgCIVIL::DataToStr(int i, T_SECT_K Key, T_SECT_D &Data, T_RCHK_D &RData)
{  
	T_RCHK_COLM RchkColm;
	RchkColm = RData.COLM;
	CString str;
	if(i == 0) str.Format(_T("%d"), Key);
	else if(i == 1) str = Data.SName;
	else if(i == 2)
	{
		if(RchkColm.iBarNum[0][i-2][0]!=0 ) str = _T("O");
		else                                str = _T("X");
	}
	else ASSERT(0);

	return str;
}

BOOL CDgnConColmReinforceDlgCIVIL::SelectSection(CArray<T_RCHK_K, T_RCHK_K>&  arRchKey)
{
	BOOL bCanbeChked = TRUE;

	int iCheckResult = 0;
	T_SECT_D OrginSectData; 
	OrginSectData.Initialize();
	T_SECT_D SectData; 

	int Index = 0;

	for(int i=0; i < arRchKey.GetSize(); i++)
	{
		iCheckResult = 0;
		SectData.Initialize();
		T_RCHK_K RchK = arRchKey.GetAt(i);
		if(!m_pDoc->m_pAttrCtrl->GetSectDesign(RchK, SectData)) continue;
		
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

void CDgnConColmReinforceDlgCIVIL::CheckDiffSections(T_SECT_D OrginSectData, T_SECT_D SectData, int& iResult)
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

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//
//  SelChange PropNo CB 관련 
//

//------------------------------------------------------------------
// ShowBitmap
//------------------------------------------------------------------

void CDgnConColmReinforceDlgCIVIL::ShowBitmap(CString& colType)
{
	CWnd* pWnd[10];
	pWnd[0] = GetDlgItem(IDC_DGN_RECT_FRAME);   ASSERT(pWnd[0]);
	pWnd[1] = GetDlgItem(IDC_DGN_SRECT_FRAME);  ASSERT(pWnd[1]);
	pWnd[2] = GetDlgItem(IDC_DGN_CIRC_FRAME);   ASSERT(pWnd[2]);
	pWnd[3] = GetDlgItem(IDC_DGN_SCIRC_FRAME);  ASSERT(pWnd[3]);
	pWnd[4] = GetDlgItem(IDC_DGN_OCTA_FRAME);   ASSERT(pWnd[4]);
	pWnd[5] = GetDlgItem(IDC_DGN_SOCTA_FRAME);  ASSERT(pWnd[5]);
	pWnd[6] = GetDlgItem(IDC_DGN_HALF_FRAME);   ASSERT(pWnd[6]);
	pWnd[7] = GetDlgItem(IDC_DGN_TRACK_FRAME);  ASSERT(pWnd[7]);
	pWnd[8] = GetDlgItem(IDC_DGN_STRACK_FRAME); ASSERT(pWnd[8]);
	pWnd[9] = GetDlgItem(IDC_DGN_H_FRAME);			ASSERT(pWnd[9]);

	int iSelection;
	if(colType == D_SECT_SHAPE_REG_B)         { pWnd[0]->ShowWindow(SW_SHOW); iSelection=0; }
	else if(colType == D_SECT_SHAPE_REG_SB)   { pWnd[1]->ShowWindow(SW_SHOW); iSelection=1; }
	else if(colType == D_SECT_SHAPE_REG_P)    { pWnd[2]->ShowWindow(SW_SHOW); iSelection=2; }
	else if(colType == D_SECT_SHAPE_REG_SR)   { pWnd[3]->ShowWindow(SW_SHOW); iSelection=3; }
	else if(colType == D_SECT_SHAPE_REG_OCT)  { pWnd[4]->ShowWindow(SW_SHOW); iSelection=4; }
	else if(colType == D_SECT_SHAPE_REG_SOCT) { pWnd[5]->ShowWindow(SW_SHOW); iSelection=5; }
	else if(colType == D_SECT_SHAPE_REG_HTRK) { pWnd[6]->ShowWindow(SW_SHOW); iSelection=6; }
	else if(colType == D_SECT_SHAPE_REG_TRK)  { pWnd[7]->ShowWindow(SW_SHOW); iSelection=7; }
	else if(colType == D_SECT_SHAPE_REG_STRK) { pWnd[8]->ShowWindow(SW_SHOW); iSelection=8; }
	else if(colType == D_SECT_SHAPE_REG_H)		{ pWnd[9]->ShowWindow(SW_SHOW); iSelection=9; }
	for(int i=0; i<10; i++) 
	{
		if(i!=iSelection) pWnd[i]->ShowWindow(SW_HIDE);
	}  
}

void CDgnConColmReinforceDlgCIVIL::SetControlText(BOOL bBox)
{
	GetDlgItem(IDC_DGN_COLB_STATIC)->SetWindowText(_T("B"));
	GetDlgItem(IDC_DGN_COLH_STATIC)->SetWindowText(_T("H"));

	if(bBox == TRUE)
	{		
		GetDlgItem(IDC_DGN_COLa_STATIC)->SetWindowText(_T("tf1"));
		GetDlgItem(IDC_DGN_COLb2_STATIC)->SetWindowText(_T("tf2"));
		GetDlgItem(IDC_DGN_COLt_STATIC)->SetWindowText(_T("tw"));;
	}
	else
	{		
		GetDlgItem(IDC_DGN_COLa_STATIC)->SetWindowText(_T("a"));
		GetDlgItem(IDC_DGN_COLb2_STATIC)->SetWindowText(_T("b"));
		GetDlgItem(IDC_DGN_COLt_STATIC)->SetWindowText(_T("t"));
	}
}

//------------------------------------------------------------------
// ShowHideControl
//------------------------------------------------------------------
void CDgnConColmReinforceDlgCIVIL::ShowHideControl(CString& colType)
{
	BOOL bRound = FALSE;
	if(colType == D_SECT_SHAPE_REG_SR)
	{
		bRound = TRUE;
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
		bRound = TRUE;
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
	else if(colType == D_SECT_SHAPE_REG_H)
	{
		CDlgUtil::CtrlEnableDisable(this, m_BGroup, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_HGroup, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_abGroup, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_tGroup, TRUE);

		GetDlgItem(IDC_DGN_COLB_STATIC)->SetWindowText(_T("B1"));
		GetDlgItem(IDC_DGN_COLH_STATIC)->SetWindowText(_T("H"));
		GetDlgItem(IDC_DGN_COLa_STATIC)->SetWindowText(_T("B2"));
		GetDlgItem(IDC_DGN_COLb2_STATIC)->SetWindowText(_T("tf1"));
		GetDlgItem(IDC_DGN_COLt_STATIC)->SetWindowText(_T("tf2"));
	}

	CDlgUtil::CtrlShowHide(this, m_Tie1stType, (!m_bTieSecondType));
	CDlgUtil::CtrlShowHide(this, m_Tie2ndType, (m_bTieSecondType));

	if(m_bTieSecondType)
	{
		GetDlgItem(IDC_DGN_NUM_CB)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_DGN_NUM_CB)->ShowWindow(bRound);
		CDlgUtil::CtrlShowHide(this, m_TieTypeNum, !bRound);
	}
}

//---------------------------------------------------------------------------
// SelectElemOnView 
//---------------------------------------------------------------------------
void CDgnConColmReinforceDlgCIVIL::SelectElemOnView()
{
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL, FALSE);

	CArray<T_ELEM_K, T_ELEM_K> arElemKeyList;
	arElemKeyList.RemoveAll();

	CArray<T_RCHK_K, T_RCHK_K>  arRchKey;
	int nItem = -1;

	int nSelCount = m_wndList.GetSelectedCount();
	arRchKey.SetSize(nSelCount);
	for (UINT i=0; i < nSelCount; i++)
	{
		nItem = m_wndList.GetNextItem(nItem, LVNI_SELECTED);
		ASSERT(nItem != -1);
		arRchKey[i]   = m_wndList.GetItemData(nItem);  
	}

	T_ELEM_K ElemKey;
	T_ELEM_D ElemData;
	CDgnDataCtrl DataCtrl;

	T_RCHK_K RchKey;
	int Index = 0;
	for(int i=0; i<arRchKey.GetSize(); i++)
	{
		RchKey = arRchKey.GetAt(i);
		POSITION PosElem = m_pDoc->m_pAttrCtrl->GetStartElem();
		while(PosElem)
		{
			ElemData.Initialize();
			m_pDoc->m_pAttrCtrl->GetNextElem(PosElem, ElemKey, ElemData);
		
			if(ElemData.elpro == RchKey) 
				if(ElemData.eltyp == TRUSS_EL || ElemData.eltyp == BEAM_EL)
					if(DataCtrl.Get_DgnGenMbtp(ElemKey) == D_MBTP_COLUMN)  arElemKeyList.Add(ElemKey);
		}
	}

	m_pDoc->m_pViewCtrl->SelectElem(NULL, arElemKeyList, FALSE);
}

//---------------------------------------------------------------------------
// ChangeSectionInfo 
//---------------------------------------------------------------------------
void CDgnConColmReinforceDlgCIVIL::ChangeSectionInfo()
{
	// Select Elem. On View
	SelectElemOnView();

	T_SECT_D SectData; SectData.Initialize();

	if(!m_pDoc->m_pAttrCtrl->GetSect(m_RchkKey, SectData))	return;

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
		if(SectData.SectBefore.SectI.Size[5] == 0)  tf2 = SectData.SectBefore.SectI.Size[3]; 
		else                                        tf2 = SectData.SectBefore.SectI.Size[5];
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
	else if(SectData.SectBefore.Shape == D_SECT_SHAPE_REG_H)
	{
		GetDlgItem(IDC_DGN_SHAPE)->SetWindowText(_T("H-Section"));//(_T("H")); 
		str.Format(_T("%s"), D_SECT_SHAPE_REG_H);
		m_H.SetEditUnit(SectData.SectBefore.SectI.Size[0]);
		m_B.SetEditUnit(SectData.SectBefore.SectI.Size[1]);
		m_a.SetEditUnit(SectData.SectBefore.SectI.Size[4]);
		m_b.SetEditUnit(SectData.SectBefore.SectI.Size[3]);
		m_t.SetEditUnit(SectData.SectBefore.SectI.Size[5]);
	}
		
	ShowBitmap(str);
	ShowHideControl(str);

	m_RchkData.Initialize();
	m_pDoc->m_pAttrCtrl->GetRchk(m_RchkKey, m_RchkData);    

}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//
//  Button Click 관련 
//

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// DB 관련 
//
/*
void CDgnConColmReinforceDlgCIVIL::UpdateUnit()
{
	m_pDoc->m_pUnitCtrl->ConvertUnitRchkPrevious(m_RchkData);    
}

*/

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// Right Data & Grid Control.

void CDgnConColmReinforceDlgCIVIL::SetRchkData(T_RCHK_K RchkKey, T_RCHK_COLM* pColmData)
{
	m_RchkKey = RchkKey;

	m_pColmData = NULL;
	m_pColmData = pColmData;
}

BOOL CDgnConColmReinforceDlgCIVIL::CheckInputData_Rebar()
{

	int iChkResult = 0;
	_DGNC_RBAR RebarData;
	
	int nError;
	CString str, errMsg;
	CDgnDataCtrl DataCtrl;

	int Index=0;
	T_RCHK_K RchkKey; 
	POSITION Pos = m_arRchkKey.GetStartPosition();
	while(Pos)
	{
		RebarData.Initialize();
		RchkKey=0; 
		str=_T(""); errMsg=_T("");
		m_arRchkKey.GetNextAssoc(Pos, Index, RchkKey);

		RebarData.iSectNo = RchkKey;
		RebarData.iBarLayc = m_nLayerNum;

		for(int i=0; i<m_nLayerNum; i++)
		{
			// pos1
			str = m_ColGrid->GetValueRowCol(i+2, 1);
			RebarData.iBarNum[0][i] = _ttoi(str);
			str = m_ColGrid->GetValueRowCol(i+2, 2);
			RebarData.strBarNa1[0][i] = str;
			str = m_ColGrid->GetValueRowCol(i+2, 3);
			RebarData.strBarNa2[0][i] = str;
			// pos2
			str = m_ColGrid->GetValueRowCol(i+2, 4);
			RebarData.iBarNum[1][i] = _ttoi(str);
			str = m_ColGrid->GetValueRowCol(i+2, 5);
			RebarData.strBarNa1[1][i] = str;
			str = m_ColGrid->GetValueRowCol(i+2, 6);
			RebarData.strBarNa2[1][i] = str;

			// pos3
			str = m_ColGrid->GetValueRowCol(i+2, 7);
			RebarData.iBarNum[2][i] = _ttoi(str);
			str = m_ColGrid->GetValueRowCol(i+2, 8);
			RebarData.strBarNa1[2][i] = str;
			str = m_ColGrid->GetValueRowCol(i+2, 9);
			RebarData.strBarNa2[2][i] = str;

			str = m_ColGrid->GetValueRowCol(i+2, 10);
			RebarData.dDc[i] = _tstof(str);
			double CL = 0;
			if(i == 0) CL = 0;
			else       CL = RebarData.dDc[i-1]; 
		}

		// Check Rebar Data
		nError = DataCtrl.Check_RbarCivil(2, RebarData);

		if(nError == 0)
		{
			for(int pos=0; pos<3; pos++)
			{
				for(int i=0; i<m_nLayerNum; i++)
				{
					if(RebarData.bCheckRebar[pos][i] == FALSE)  iChkResult |= 2;

					if(RebarData.bCheckSpace[pos][i] == FALSE)  iChkResult |= 32;

					if(RebarData.bCheckCovc[i] == FALSE)        iChkResult |= 64;
				}
			}
		}
		else 
		{
			if(nError == 1)       iChkResult |= 4;
			else if(nError == 2)  iChkResult |= 8;
			else if(nError == 3)  iChkResult |= 16;
		}
	}

	if(iChkResult & 2)
	{
		//errMsg.Format(_T("[오류] %d Layer(pos%d)의 Rebar 정보가 올바르지 않습니다."), i+1, pos+1);
		AfxMessageBox(_LS(IDS_DGN_CON_COLM_Pos_Rebar_Info_Err));
		return FALSE;
	}
	else if(iChkResult & 4)
	{
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_No_good_Code));      
		return FALSE;
	}
	else if(iChkResult & 8)
	{
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_Not_exist_Section)); 
		return FALSE;
	}
	else if(iChkResult & 16)
	{
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_No_good_Section));   
		return FALSE;
	}
	else if(iChkResult & 32)
	{
		//errMsg.Format(_T("[경고] %d Layer(pos%d)의 철근 간격이 적당하지 않습니다.\n계속 하시겠습니까?"), i+1, pos+1);
		if(AfxMessageBox(_LS(IDS_DGN_CON_COLM_Pos_Rebar_Space_Warning), MB_YESNO) == IDNO) return FALSE;
	}
	else if(iChkResult & 64)
	{
		//errMsg.Format(_T("[경고] %d Layer의 Dc 값이 적당하지 않습니다.\n계속 하시겠습니까?"), i+1);
		if(AfxMessageBox(_LS(IDS_DGN_CON_COLM_Pos_Rebar_Dc_Warning), MB_YESNO) == IDNO) return FALSE;
	}

	UpdateAsInfo(RebarData.dAsc);

	return TRUE;
}

void CDgnConColmReinforceDlgCIVIL::SetDlg2Data_Rebar()
{
	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	// 일단 0 으로 고정해두었다~	

	UpdateData(TRUE);

	// Stirrup Bar
/*
	int nIndex = m_Num.GetCurSel();
	if(nIndex >= 0) m_Num.GetLBText(nIndex, m_pColmData->dSubBarNum);
	//m_pColmData->iSubBarNum = m_Num.GetEditValue();
*/
	m_pColmData->dSubBarNum[0] = m_Num.GetEditValue();
	m_pColmData->dSubBarNum_y[0] = m_YNum_2nd.GetEditValue();
	m_pColmData->dSubBarNum_z[0] = m_ZNum_2nd.GetEditValue();
	m_pColmData->dSubBarDist[0] = m_Space.GetEditValue();

	int nIndex = m_SizeCB.GetCurSel();
	if(nIndex >= 0) m_SizeCB.GetLBText(nIndex, m_pColmData->strSubBarNa[0]);
	nIndex = m_SizeYCB_2nd.GetCurSel();
	if(nIndex >= 0) m_SizeYCB_2nd.GetLBText(nIndex, m_pColmData->strSubBarNa_y[0]);
	nIndex = m_SizeZCB_2nd.GetCurSel();
	if(nIndex >= 0) m_SizeZCB_2nd.GetLBText(nIndex, m_pColmData->strSubBarNa_z[0]);

	if(!m_bTieSecondType)
	{// 기존방식
		T_SECT_D SectData; SectData.Initialize();
		if (!m_pDoc->m_pAttrCtrl->GetSect(m_RchkKey, SectData))	return;

		CString str = _T("");
		if (SectData.SectBefore.Shape == D_SECT_SHAPE_REG_SR || SectData.SectBefore.Shape == D_SECT_SHAPE_REG_P)
		{
			m_pColmData->dSubBarNum_y[0] = m_pColmData->dSubBarNum[0]; // dSubBarNum_y, dSubBarNum_z 사용하기로함(v940)
			m_pColmData->dSubBarNum_z[0] = m_pColmData->dSubBarNum[0];
		}

		m_pColmData->strSubBarNa_y[0] = m_pColmData->strSubBarNa_z[0] = m_pColmData->strSubBarNa[0];
	}
	else 
	{// 두번째 방식:Tie Rebar 개별 입력(중국 내진기준에서만 사용됨 09.05.20)
		double dArea_y = m_pDoc->m_pMatlDB->Get_RebarArea(m_pColmData->strSubBarNa_y[0]) * m_pColmData->dSubBarNum_y[0];
		double dArea_z = m_pDoc->m_pMatlDB->Get_RebarArea(m_pColmData->strSubBarNa_z[0]) * m_pColmData->dSubBarNum_z[0];
		if(dArea_y > dArea_z)
		{
			m_pColmData->dSubBarNum[0]  = m_pColmData->dSubBarNum_z[0];
			m_pColmData->strSubBarNa[0] = m_pColmData->strSubBarNa_z[0];
		}
		else 
		{
			m_pColmData->dSubBarNum[0]  = m_pColmData->dSubBarNum_y[0];
			m_pColmData->strSubBarNa[0] = m_pColmData->strSubBarNa_y[0];
		}
	}
	
	// Rebar
	GetDataFromGrid();
}

void CDgnConColmReinforceDlgCIVIL::SetInitRebarCB()
{
	if(m_SizeCB.GetCount()>0) m_SizeCB.ResetContent();
	CDBLib::GetRebarNameAtComboBox(&m_SizeCB);

	
	if(m_SizeYCB_2nd.GetCount()>0) m_SizeYCB_2nd.ResetContent();
	CDBLib::GetRebarNameAtComboBox(&m_SizeYCB_2nd);

	
	if(m_SizeZCB_2nd.GetCount()>0) m_SizeZCB_2nd.ResetContent();
	CDBLib::GetRebarNameAtComboBox(&m_SizeZCB_2nd);
/*
	if(m_Num.GetCount()>0) m_Num.ResetContent();
	CStringArray aRebarNum;
	if(CDBLib::GetSubRebarNumList_Civil(aRebarNum))
	{
		for(int i=0; i<aRebarNum.GetSize(); i++)	m_Num.AddString(aRebarNum[i]);
	}
*/
}

void CDgnConColmReinforceDlgCIVIL::SetInitLayerCB()
{
	CString str;
	m_LayerCB.ResetContent();

	for(int i=0; i<5; i++)
	{
		str.Format(_T("%d"), i+1);
		m_LayerCB.AddString(str);  
	}

	UINT uIndex = 0;
	m_LayerCB.SetCurSel(uIndex);
	UpdateData(FALSE);
}

void CDgnConColmReinforceDlgCIVIL::OnSelchangeDgnLayerCb() 
{
	CString strSel = _T("");
	int nIndex, nLayer;

	nIndex = m_LayerCB.GetCurSel();
	if(nIndex>=0) m_LayerCB.GetLBText(nIndex, strSel);
	nLayer = _ttoi(strSel);

	if(nLayer<1 || nLayer>5) return;

	m_nLayerNum = nLayer;
	m_ColGrid->SetLayerNum(m_RchkKey, m_nLayerNum);

}

void CDgnConColmReinforceDlgCIVIL::SetData2Dlg_Rebar()
{
	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	// 일단 0 으로 고정해두었다~	

	m_Space.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_SpaceUT.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_As.SetUnitType(D_UNITSYS_BASE_AREA);
	m_AsUT.SetUnitType(D_UNITSYS_BASE_AREA);
	m_Num.SetUnitType(D_UNITSYS_NONE);
	m_YNum_2nd.SetUnitType(D_UNITSYS_NONE);
	m_ZNum_2nd.SetUnitType(D_UNITSYS_NONE);

	// StirrupBar
	m_Num.SetEditUnit(m_pColmData->dSubBarNum[0]);
	m_YNum_2nd.SetEditUnit(m_pColmData->dSubBarNum_y[0]);
	m_ZNum_2nd.SetEditUnit(m_pColmData->dSubBarNum_z[0]);
//  m_Num.SelectString(-1, m_pColmData->dSubBarNum);
	m_Space.SetEditUnit(m_pColmData->dSubBarDist[0]);
	int nIndex = m_SizeCB.SelectString(-1, m_pColmData->strSubBarNa[0]);
	nIndex = m_SizeYCB_2nd.SelectString(-1, m_pColmData->strSubBarNa_y[0]);
	nIndex = m_SizeZCB_2nd.SelectString(-1, m_pColmData->strSubBarNa_z[0]);

	// Rebar Layer
	CString str;
	m_nLayerNum = 0;
	for(int i=0; i<5; i++)
	{
		if(m_pColmData->iBarNum[0][0][i]>0 || 
			 m_pColmData->iBarNum[0][1][i]>0 || 
			 m_pColmData->iBarNum[0][2][i]>0) m_nLayerNum++;
	}

	if(m_nLayerNum == 0) m_nLayerNum = 1;
	str.Format(_T("%d"), m_nLayerNum);
	nIndex = m_LayerCB.SelectString(-1, str);
	ASSERT(nIndex != LB_ERR);

	// Grid
	SetDataToGrid();
	m_ColGrid->SetLayerNum(m_RchkKey, m_nLayerNum); 

	// As
	CDgnDataCtrl DataCtrl;
	T_UNIT_INDEX CurIndex;
	CDBDoc::GetDocPoint()->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	CDBDoc::GetDocPoint()->m_pMatlDB->Create_RebarData(CurIndex.nBase_Length, CurIndex.nBase_Force, TRUE);
	double dAsc=0.0;
	for(int i=0; i<5; i++)
	{
		for(int j=0; j<3; j++)
		{
			if(m_pColmData->iBarNum[0][j][i] > 0)
			{
				double dAs1=0.0, dAs2=0.0;
				if(m_pColmData->strBarNa1[0][j][i]!=_T(""))  dAs1 = CDBDoc::GetDocPoint()->m_pMatlDB->Get_RebarArea(m_pColmData->strBarNa1[0][j][i]);
				if(m_pColmData->strBarNa2[0][j][i]!=_T(""))  dAs2 = CDBDoc::GetDocPoint()->m_pMatlDB->Get_RebarArea(m_pColmData->strBarNa2[0][j][i]);
				if(dAs2==0.0) dAs2 = dAs1;
				// Change by ZINU.('06.02.27). ex) 5(2.5EA + 2.5EA) -> 5(3EA + 2EA).
				int iBarNum = m_pColmData->iBarNum[0][j][i];
				double dBarNum1 = ceil(iBarNum/2.);
				double dBarNum2 = iBarNum - dBarNum1;
				dAsc += DataCtrl.Get_UsedAs(m_RchkKey, j+1, dAs1*dBarNum1 + dAs2*dBarNum2);
			}
		}
	}
	UpdateAsInfo(dAsc);

	UpdateData(FALSE);
}

void CDgnConColmReinforceDlgCIVIL::SetDataToGrid()
{
	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	// 일단 0 으로 고정해두었다~	

	for(int i=0; i<5; i++)
	{
		m_ColGrid->SetValueRange(CGXRange(i+2,1), double(m_pColmData->iBarNum[0][0][i]));
		m_ColGrid->SetValueRange(CGXRange(i+2,2), m_pColmData->strBarNa1[0][0][i]);
		m_ColGrid->SetValueRange(CGXRange(i+2,3), m_pColmData->strBarNa2[0][0][i]);

		m_ColGrid->SetValueRange(CGXRange(i+2,4), double(m_pColmData->iBarNum[0][1][i]));
		m_ColGrid->SetValueRange(CGXRange(i+2,5), m_pColmData->strBarNa1[0][1][i]);
		m_ColGrid->SetValueRange(CGXRange(i+2,6), m_pColmData->strBarNa2[0][1][i]);

		m_ColGrid->SetValueRange(CGXRange(i+2,7), double(m_pColmData->iBarNum[0][2][i]));
		m_ColGrid->SetValueRange(CGXRange(i+2,8), m_pColmData->strBarNa1[0][2][i]);
		m_ColGrid->SetValueRange(CGXRange(i+2,9), m_pColmData->strBarNa2[0][2][i]);

		m_ColGrid->SetValueRange(CGXRange(i+2,10), m_pColmData->dDc[0][i]);
	}
}

void CDgnConColmReinforceDlgCIVIL::GetDataFromGrid()
{
	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	// 일단 0 으로 고정해두었다~	

	CString str = _T("");

	for(int i=0; i<5 ; i++)
	{
		str = m_ColGrid->GetValueRowCol(i+2, 1);
		m_pColmData->iBarNum[0][0][i] = _ttoi(str);
		str = m_ColGrid->GetValueRowCol(i+2, 2);
		m_pColmData->strBarNa1[0][0][i] = str;
		str = m_ColGrid->GetValueRowCol(i+2, 3);
		m_pColmData->strBarNa2[0][0][i] = str;

		str = m_ColGrid->GetValueRowCol(i+2, 4);
		m_pColmData->iBarNum[0][1][i] = _ttoi(str);
		str = m_ColGrid->GetValueRowCol(i+2, 5);
		m_pColmData->strBarNa1[0][1][i] = str;
		str = m_ColGrid->GetValueRowCol(i+2, 6);
		m_pColmData->strBarNa2[0][1][i] = str;

		str = m_ColGrid->GetValueRowCol(i+2, 7);
		m_pColmData->iBarNum[0][2][i] = _ttoi(str);
		str = m_ColGrid->GetValueRowCol(i+2, 8);
		m_pColmData->strBarNa1[0][2][i] = str;
		str = m_ColGrid->GetValueRowCol(i+2, 9);
		m_pColmData->strBarNa2[0][2][i] = str;

		str = m_ColGrid->GetValueRowCol(i+2, 10);
		m_pColmData->dDc[0][i] = _tstof(str);
	}
}

void CDgnConColmReinforceDlgCIVIL::UpdateAsInfo(double dAsc)
{
	m_As.SetEditUnit(dAsc);
}

void CDgnConColmReinforceDlgCIVIL::OnRedrawBtn() 
{
	CArray<T_RCHK_K, T_RCHK_K>  arRchKey;
	CArray<T_RCHK_D, T_RCHK_D&> arRchkData;
	int nItem = -1;
	int nSelCount = m_wndList.GetSelectedCount();
	arRchKey.SetSize(nSelCount);
	arRchkData.SetSize(nSelCount);
	for(UINT i=0; i< nSelCount; i++)
	{
		nItem = m_wndList.GetNextItem(nItem, LVNI_SELECTED);
		ASSERT(nItem != -1);
		arRchKey[i]   = m_wndList.GetItemData(nItem);
		m_arRchkKey.SetAt(i,m_wndList.GetItemData(nItem));
	}

	if(!CheckInputData_Rebar()) return;
	SetDlg2Data_Rebar();
		
	UpdateViewWnd();
}