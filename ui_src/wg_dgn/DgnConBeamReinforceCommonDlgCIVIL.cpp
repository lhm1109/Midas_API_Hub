// CDgnConBeamReinforceCommonDlgCIVIL.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConBeamReinforceCommonDlgCIVIL.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_MsgDll.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\DBCodeDef.h"

#include "..\wg_main\wg_mainRes2.h"

#include "DgnDataCtrl.h"
#include "DgnBeamGridWndCIVIL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConBeamReinforceCommonDlgCIVIL dialog


CDgnConBeamReinforceCommonDlgCIVIL::CDgnConBeamReinforceCommonDlgCIVIL(int nIDD, CWnd* pParent /*=NULL*/)
	: CDialogMove(nIDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConBeamReinforceCommonDlgCIVIL)
	m_bCrack = -1;
	m_nIMJ = 0;
	m_RchkData.Initialize();
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();

	m_bTorsionalBar = FALSE;
	m_bBundledBar = FALSE;
	m_bBentUpBar = FALSE;
}


void CDgnConBeamReinforceCommonDlgCIVIL::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConBeamReinforceCommonDlgCIVIL)
	DDX_Control(pDX, IDC_DGN_DELETE, m_btnDel);
	DDX_Control(pDX, IDOK, m_btnAdd);
	DDX_Control(pDX, IDC_REDRAW_BTN, m_btnRedraw);
	DDX_Control(pDX, IDC_BEAM_SECT_LIST, m_wndList);
	DDX_Control(pDX, IDC_DGN_BEAM_TAB, m_ctrlTab);
	DDX_Control(pDX, IDC_DGN_RC_BHFUNIT, m_HfUnit);
	DDX_Control(pDX, IDC_DGN_RC_BHF, m_Hf);
	DDX_Control(pDX, IDC_DGN_RC_BHCUNIT, m_HcUnit);
	DDX_Control(pDX, IDC_DGN_RC_BHC, m_Hc);
	DDX_Control(pDX, IDC_DGN_RC_BBFUNIT, m_BfUnit);
	DDX_Control(pDX, IDC_DGN_RC_BBF, m_Bf);
	DDX_Control(pDX, IDC_DGN_RC_BBCUNIT, m_BcUnit);
	DDX_Control(pDX, IDC_DGN_RC_BBC, m_Bc);
	DDX_Radio(pDX, IDC_DGN_CON_BEAM_CRACK1, m_bCrack);
	DDX_Control(pDX, IDC_DGN_RC_As_Top_UNIT, m_AsTopUT);
	DDX_Control(pDX, IDC_DGN_RC_As_Top, m_AsTop);
	DDX_Control(pDX, IDC_DGN_RC_As_Bot_Unit, m_AsBotUT);
	DDX_Control(pDX, IDC_DGN_RC_As_Bot, m_AsBot);
	DDX_Control(pDX, IDC_DGN_RC_BSTIRRUP_SPACE_UNIT, m_cStirrupSpaceUT);
	DDX_Control(pDX, IDC_DGN_RC_BSTIRRUP_SPACE, m_cStirrupSpace);
	DDX_Control(pDX, IDC_DGN_RC_BSTIRRUP_SIZE, m_cStirrupSize);
	DDX_Control(pDX, IDC_DGN_RC_TOP_LAYER_CB, m_cTopLayerCB);
	DDX_Control(pDX, IDC_DGN_RC_BOT_LAYER_CB, m_cBotLayerCB);
	DDX_Control(pDX, IDC_DGN_RC_BSTIRRUP_NUMBER, m_cStirrupNum);
	DDX_Check  (pDX, IDC_RCHK_TOR_CHK,                 m_bTorsionalBar);
	DDX_Control(pDX, IDC_RCHK_TOR_STIRRUP_SIZE_CMB,    m_cmbTorBar);
	DDX_Control(pDX, IDC_RCHK_TOR_STIRRUP_SPACING_EDT, m_edtTorSpacing);
	DDX_Control(pDX, IDC_RCHK_TOR_STIRRUP_SPACING_UNT, m_untTorSpacing);
	DDX_Check  (pDX, IDC_RCHK_TOR_BUNDLED_CHK,         m_bBundledBar);
	DDX_Control(pDX, IDC_RCHK_TOR_BUNDLED_NUM_EDT,     m_edtBundledBarNum);
	DDX_Control(pDX, IDC_RCHK_TOR_LONGI_SIZE_CMB,      m_cmbLongiBar);
	DDX_Control(pDX, IDC_RCHK_TOR_LONGI_NUM_EDT,       m_edtLongiBarNum);
	DDX_Control(pDX, IDC_DGN_RC_BSTIRRUP_ANGLE,        m_edtStirrupAngle);
	DDX_Control(pDX, IDC_DGN_RC_BSTIRRUP_ANGLE_UNIT,   m_untStirrupAngle);
	DDX_Check  (pDX, IDC_DGN_RC_BENTUP_CHK,            m_bBentUpBar);
	DDX_Control(pDX, IDC_DGN_RC_BENTUP_SIZE,           m_cmbBentUpBatSize);
	DDX_Control(pDX, IDC_DGN_RC_BENTUP_NUMBER,         m_edtBentUpBarNum);
	DDX_Control(pDX, IDC_DGN_RC_BENTUP_ANGLE,          m_edtBentUpBarAngle);
	DDX_Control(pDX, IDC_DGN_RC_BENTUP_ANGLE_UNIT,     m_untBentUpBarAngle);
	DDX_Control(pDX, IDC_DGN_BEAMRECT_BIT, m_wndPictureRect);
	DDX_Control(pDX, IDC_DGN_BEAMTEE_BIT, m_wndPictureTee);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnConBeamReinforceCommonDlgCIVIL, CDialogMove)
	//{{AFX_MSG_MAP(CDgnConBeamReinforceCommonDlgCIVIL)
	ON_BN_CLICKED(IDC_DGN_DESIGN_SECTION, OnDgnDesignSection)
	ON_BN_CLICKED(IDC_RCHK_TOR_CHK,         OnDgnTorsionalBar)
	ON_BN_CLICKED(IDC_DGN_RC_BENTUP_CHK,    OnDgnBentUpBarChk)
	ON_BN_CLICKED(IDC_RCHK_TOR_BUNDLED_CHK, OnDgnBundledStirrups)
	ON_BN_CLICKED(IDC_DGN_DELETE, OnDgnDelete)
	ON_BN_CLICKED(IDC_DGN_CON_BEAM_CRACK1, OnClickOptCrack)
	ON_BN_CLICKED(IDC_REDRAW_BTN, OnRedrawBtn)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_BEAM_SECT_LIST, OnItemchangedDgnSectList)	
	ON_NOTIFY(NM_CLICK, IDC_BEAM_SECT_LIST, OnClickDgnSectList)
	ON_NOTIFY(TCN_SELCHANGE, IDC_DGN_BEAM_TAB, OnSelchangeTab)
	ON_NOTIFY(TCN_SELCHANGING, IDC_DGN_BEAM_TAB, OnSelchangingTab)
	ON_CBN_SELCHANGE(IDC_DGN_RC_BOT_LAYER_CB, OnSelchangeDgnRcBotLayerCb)
	ON_CBN_SELCHANGE(IDC_DGN_RC_TOP_LAYER_CB, OnSelchangeDgnRcTopLayerCb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConBeamReinforceCommonDlgCIVIL message handlers

//---------------------------------------------------------------------------
// Initialize Unit
//---------------------------------------------------------------------------
void CDgnConBeamReinforceCommonDlgCIVIL::SetInitUnit()
{
	m_Hf.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Hc.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Bf.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Bc.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_HfUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);	
	m_HcUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_BfUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_BcUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_edtTorSpacing.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untTorSpacing.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_edtBundledBarNum.SetUnitType(D_UNITSYS_NONE);
	m_edtLongiBarNum.SetUnitType(D_UNITSYS_NONE);

	m_edtStirrupAngle.SetUnitType(D_UNITSYS_BASE_DEGREE);
	m_untStirrupAngle.SetUnitType(D_UNITSYS_BASE_DEGREE);
	m_edtBentUpBarNum.SetUnitType(D_UNITSYS_NONE);
	m_edtBentUpBarAngle.SetUnitType(D_UNITSYS_BASE_DEGREE);
	m_untBentUpBarAngle.SetUnitType(D_UNITSYS_BASE_DEGREE);
}

void CDgnConBeamReinforceCommonDlgCIVIL::ShowRECTBitmap()
{
	GetDlgItem(IDC_DGN_BEAMRECT_BIT)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_BEAMTEE_BIT)->ShowWindow(SW_HIDE);
}

void CDgnConBeamReinforceCommonDlgCIVIL::ShowTEEBitmap()
{
	GetDlgItem(IDC_DGN_BEAMTEE_BIT)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_BEAMRECT_BIT)->ShowWindow(SW_HIDE);
}

//---------------------------------------------------------------------------
// SetData2Dlg 
//---------------------------------------------------------------------------
void CDgnConBeamReinforceCommonDlgCIVIL::SetData2Dlg()
{
	// UnSelect All Elem.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL, FALSE);

	m_bCrack  = 0;
		
	m_Hf.SetEditUnit(0.);
	m_Hc.SetEditUnit(0.);
	m_Bf.SetEditUnit(0.);
	m_Bc.SetEditUnit(0.);

	m_Hf.EnableWindow(FALSE);
	m_Hc.EnableWindow(FALSE);
	m_Bf.EnableWindow(FALSE);
	m_Bc.EnableWindow(FALSE);
	GetDlgItem(IDC_DGN_RC_BSHAPE)->EnableWindow(FALSE);

	m_bTorsionalBar = FALSE;
	m_cmbTorBar.SetCurSel(-1);
	m_edtTorSpacing.SetEditUnit(0.0);
	m_bBundledBar = FALSE;
	m_edtBundledBarNum.SetEditUnit(0.0);
	m_cmbLongiBar.SetCurSel(-1);	
	m_edtLongiBarNum.SetEditUnit(0.0);
	m_edtStirrupAngle.SetEditUnit(0.0);
	m_bBentUpBar = FALSE;
	m_cmbBentUpBatSize.SetCurSel(-1);	
	m_edtBentUpBarNum.SetEditUnit(0.0);
	m_edtBentUpBarAngle.SetEditUnit(0.0);
		
	ShowRECTBitmap();
	ShowHideSectCtrl(FALSE);
 
	UpdateData(FALSE);
}

void CDgnConBeamReinforceCommonDlgCIVIL::OnClickOptCrack()
{
	//UpdateData(TRUE);
}

BOOL CDgnConBeamReinforceCommonDlgCIVIL::OnInitDialog() 
{
	/*
	CList<UINT, UINT> DisableVManagerControls;
	DisableVManagerControls.AddTail(IDC_DGN_SP_ID_PREVIEW);
	EnableVisualManagerStyle(FALSE, TRUE, &DisableVManagerControls);
	*/
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here

#if defined(_ORG)
	SetWindowText(_LS(IDS_DGN_CON_REIN_BEAM_DLG_org));
#else
	SetWindowText(_LS(IDS_DGN_CON_REIN_BEAM_DLG));
#endif


	m_bModify = FALSE;
	for(int i=0; i<3; i++) bExistData[i]=TRUE;

	SetHeaderTitle();
	MakeItemEx();
	InitTabCtrl();  
	
	// 좌측 Section Data Control.
	SetInitUnit();
	SetData2Dlg();

	ChangeDlgItemInfo();

// Tab Control.
	// Init LayerCB
	InitLayerCB();

	// Init StirrupCB
	InitStirrupCB();
	
	// Init Grid
	InitGridData();
	
	// Init Tab Data  
	m_TabBeamData.Initialize();
	
	//  Init View Window
	CWnd* PlaceHolder = GetDlgItem(IDC_DGN_SP_ID_PREVIEW); ASSERT(PlaceHolder);
	m_SectWnd.Initial(PlaceHolder);
	
	m_btnAdd.EnableWindow(FALSE);
	m_btnDel.EnableWindow(FALSE);
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
			m_btnAdd.EnableWindow(TRUE);
			m_btnDel.EnableWindow(TRUE);
			m_btnRedraw.EnableWindow(TRUE);
		}
	}

	ControlsEnableDisable();

	m_wndPictureRect.SetImage(_T("SVG\\Illustration\\Dialog\\dgn_beam.svg"));
	m_wndPictureTee.SetImage(_T("SVG\\Illustration\\Dialog\\dgn_beam_tee_civil.svg"));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnConBeamReinforceCommonDlgCIVIL::OnDgnDesignSection() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND,MAKEWPARAM(ID_STRUCT_PROP_SECT4DGN,0));	
}

void CDgnConBeamReinforceCommonDlgCIVIL::OnDgnBentUpBarChk()
{
	ControlsEnableDisable();
}

void CDgnConBeamReinforceCommonDlgCIVIL::OnDgnTorsionalBar()
{
	ControlsEnableDisable();
}

void CDgnConBeamReinforceCommonDlgCIVIL::OnDgnBundledStirrups()
{
	ControlsEnableDisable();
}

// add/replace
void CDgnConBeamReinforceCommonDlgCIVIL::OnOK() 
{
	// TODO: Add extra validation here
	
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
	for(UINT i=0; i< nSelCount; i++)
	{
		nItem = m_wndList.GetNextItem(nItem, LVNI_SELECTED);
		ASSERT(nItem != -1);
		arRchKey[i]   = m_wndList.GetItemData(nItem);
		m_arRchkKey.SetAt(i,m_wndList.GetItemData(nItem));
	}

	if(!SelectSection(arRchKey))
	{
		AfxMessageBox(_LS(IDS_DGN_CON_SELECT_DiffShpe_Err));
		return;
	}

	T_DCON_D ConcData;   ConcData.Initialize();
	CDgnDataCtrl DataCtrl;
	DataCtrl.Get_DgnConDcon(ConcData);
	
	SetDlg2Data_Rebar_TabCtrl(m_nIMJ);
	
	if(!CheckInputData_Rebar_New()) return;  
 
	SetDlg2Data_Rebar();
	
	if(!CheckInputData_New()) return;
	
	if(SetDlg2Data()) SetData2Dlg();

	for (int i=0; i < nSelCount; i++)
	{
		arRchkData[i] = m_RchkData;   
	}

	if (!m_pDoc->m_pDataCtrl->AddRchk(arRchKey, arRchkData))
		return;  
	
	MakeItemEx();

	int nIndex = m_wndList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	ChangeItem(nIndex);
	
	//CDialogMove::OnOK();
}

// close
void CDgnConBeamReinforceCommonDlgCIVIL::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}

void CDgnConBeamReinforceCommonDlgCIVIL::OnDgnDelete() 
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

void CDgnConBeamReinforceCommonDlgCIVIL::PostNcDestroy() 
{	
	CDialogMove::PostNcDestroy();
}

void CDgnConBeamReinforceCommonDlgCIVIL::InitTabCtrl()
{
	for(int i=0; i<3; i++)
		m_nStiCBox[i] = -1;

	for(int i=0; i<3; i++)
		m_nBentUpBar[i] = -1;

	for(int i=0; i<3; i++)
		m_nTorBar[i] = -1;

	for(int i=0; i<3; i++)
		m_nTorLongiBar[i] = -1;

	TC_ITEM item;
	
	CStringArray strTabName;
	CString strTab;
	strTab = _LS(IDS_DGN_GRID_I_NODE); strTabName.Add(strTab);
	strTab = _LS(IDS_DGN_GRID_MIDDLE); strTabName.Add(strTab);
	strTab = _LS(IDS_DGN_GRID_J_NODE); strTabName.Add(strTab);
		
	for(int i=0; i<3; i++)
	{
		strTab = strTabName.GetAt(i);
		item.mask = TCIF_TEXT|TCIF_IMAGE;
		item.pszText=strTab.GetBuffer(0);
		item.iImage=i;
		m_ctrlTab.InsertItem(i,&item);

		strTab.ReleaseBuffer();
	}

}

void CDgnConBeamReinforceCommonDlgCIVIL::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int iTab = m_ctrlTab.GetCurSel();
	m_nIMJ = iTab;
		
	SetData_TabSelChange();  

	m_SectWnd.Draw_Shape(0, m_RchkKey, FALSE, TRUE);
	m_SectWnd.Draw_ChkRebar(m_RchkKey, iTab, m_RchkData, FALSE, TRUE);
	m_SectWnd.Invalidate();

	*pResult = 0;  
}

void CDgnConBeamReinforceCommonDlgCIVIL::OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int iTab = m_ctrlTab.GetCurSel();
		
	SetDlg2Data_Rebar_TabCtrl(iTab);  

	*pResult = 0;  
}


void CDgnConBeamReinforceCommonDlgCIVIL::OnClickDgnSectList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_wndList.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	m_btnAdd.EnableWindow(nIndex != -1);
	m_btnDel.EnableWindow(nIndex != -1);
	m_btnRedraw.EnableWindow(nIndex !=-1);
	*pResult = 0;
}

void CDgnConBeamReinforceCommonDlgCIVIL::OnItemchangedDgnSectList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	int nIndex = pNMListView->iItem; 
	ChangeItem(nIndex);
	*pResult = 0;
}


void CDgnConBeamReinforceCommonDlgCIVIL::ChangeItem(int nIndex)
{
	// enable/disable
	m_btnAdd.EnableWindow(nIndex != -1);
	m_btnDel.EnableWindow(nIndex != -1);
	m_btnRedraw.EnableWindow(nIndex != -1);

	for(int i=0; i<3; i++)
		m_nStiCBox[i] = -1;
	m_cStirrupSize.SetCurSel(m_nStiCBox[m_nIMJ]);

	for(int i=0; i<3; i++)
		m_nBentUpBar[i] = -1;
	m_cmbBentUpBatSize.SetCurSel(m_nBentUpBar[m_nIMJ]);

	for(int i=0; i<3; i++)
		m_nTorBar[i] = -1;
	m_cmbTorBar.SetCurSel(m_nTorBar[m_nIMJ]);

	for(int i=0; i<3; i++)
		m_nTorLongiBar[i] = -1;
	m_cmbLongiBar.SetCurSel(m_nTorLongiBar[m_nIMJ]);

	// get data
	m_RchkKey = 0;
	m_RchkData.Initialize();
	m_SectD.Initialize();  
	if (nIndex != -1)
	{
	  m_RchkKey = m_wndList.GetItemData(nIndex);
		m_pDoc->m_pAttrCtrl->GetSect(m_RchkKey, m_SectD);
	}

	// Left Section Data.
	ChangeSectionInfo();

	// Tab Section & Rebar Data.
	int iTab = m_ctrlTab.GetCurSel();
	SetRchkData(iTab, m_RchkKey, &(m_RchkData.BEAM));
	
	for(int i=0; i<3; i++)
		m_nStiCBox[i] = m_cStirrupSize.SelectString(-1,m_RchkData.BEAM.strSubBarNa[i]);

	for(int i=0; i<3; i++)
		m_nBentUpBar[i] = m_cmbBentUpBatSize.SelectString(-1,m_RchkData.BEAM.strBentUpBar[i]);

	for(int i=0; i<3; i++)
		m_nTorBar[i] = m_cmbTorBar.SelectString(-1,m_RchkData.BEAM.strTorBarNa[i]);

	for(int i=0; i<3; i++)
		m_nTorLongiBar[i] = m_cmbLongiBar.SelectString(-1,m_RchkData.BEAM.strLongiBarNa[i]);

	SetLayer_Change();

	SetData2Dlg_Rebar();
	m_TabBeamData = m_RchkData.BEAM;
	
	m_SectWnd.Draw_Shape(0, m_RchkKey, FALSE, TRUE);
	m_SectWnd.Draw_ChkRebar(m_RchkKey, iTab, m_RchkData, FALSE, TRUE);
	m_SectWnd.Invalidate();

	ControlsEnableDisable();
}

void CDgnConBeamReinforceCommonDlgCIVIL::SetHeaderTitle()
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

void CDgnConBeamReinforceCommonDlgCIVIL::MakeItemEx()
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

BOOL CDgnConBeamReinforceCommonDlgCIVIL::InsertItem(T_SECT_K Key, T_SECT_D &Data)
{
	
	T_RCHK_D RchkD;
	if (!m_pDoc->m_pAttrCtrl->GetRchk(Key, RchkD))
	{
		RchkD.Initialize();
		// 임시 조치 Torsional Reinforcement Check Btn 달면 빼자 - 구조체 Initilize 하는 부분도 수정하자 
		m_RchkData.BEAM.dTorBarSpacing[m_nIMJ] = m_pDoc->m_pUnitCtrl->ConvertUnitLengthOut(m_RchkData.BEAM.dTorBarSpacing[m_nIMJ]);
	}

	LVITEM lvitem;
	CString str;
	int nItem = m_wndList.GetItemCount();

	lvitem.iItem = FindInsertionPos(Key);
	for(int i=0; i < 5; i++)
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

int CDgnConBeamReinforceCommonDlgCIVIL::FindInsertionPos(T_SECT_K nID)
{
	int nCount = m_wndList.GetItemCount();
	for (int i=0; i < nCount; i++)
	{
        T_SECT_K key = m_wndList.GetItemData(i);
        if ( nID < key ) { return i; }
	}
    return nCount;
}

CString CDgnConBeamReinforceCommonDlgCIVIL::DataToStr(int i, T_SECT_K Key, T_SECT_D &Data, T_RCHK_D &RData)
{  
	T_RCHK_BEAM RchkBeam;
	RchkBeam = RData.BEAM;
	CString str;
	if(i == 0) str.Format(_T("%d"), Key);
	else if(i == 1) str = Data.SName;
	else if(i==2 || i==3 || i==4)
	{
		if(RchkBeam.iBotBarNum[i-2][0]!=0 ||
			 RchkBeam.iTopBarNum[i-2][0]!=0 ) str = _T("O");
		else str = _T("X");
	}
	else ASSERT(0);

	return str;
}

BOOL CDgnConBeamReinforceCommonDlgCIVIL::SelectSection(CArray<T_RCHK_K, T_RCHK_K>&  arRchKey)
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

void CDgnConBeamReinforceCommonDlgCIVIL::CheckDiffSections(T_SECT_D OrginSectData, T_SECT_D SectData, int& iResult)
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

//---------------------------------------------------------------------------
// CheckInputData 
//---------------------------------------------------------------------------
void CDgnConBeamReinforceCommonDlgCIVIL::CopyRebarData(int FromNode, int ToNode)
{
	for(int i=0; i<T_RCHK_BEAM_LAYER; i++)
	{
		// Top
		m_RchkData.BEAM.iTopBarNum[ToNode][i]  = m_RchkData.BEAM.iTopBarNum[FromNode][i];
		m_RchkData.BEAM.strTopBarNa1[ToNode][i] = m_RchkData.BEAM.strTopBarNa1[FromNode][i];
		m_RchkData.BEAM.strTopBarNa2[ToNode][i] = m_RchkData.BEAM.strTopBarNa2[FromNode][i];
		m_RchkData.BEAM.dDt[ToNode][i]         = m_RchkData.BEAM.dDt[FromNode][i];

		// Bottom
		m_RchkData.BEAM.iBotBarNum[ToNode][i]  = m_RchkData.BEAM.iBotBarNum[FromNode][i];
		m_RchkData.BEAM.strBotBarNa1[ToNode][i] = m_RchkData.BEAM.strBotBarNa1[FromNode][i];
		m_RchkData.BEAM.strBotBarNa2[ToNode][i] = m_RchkData.BEAM.strBotBarNa2[FromNode][i];
		m_RchkData.BEAM.dDb[ToNode][i]         = m_RchkData.BEAM.dDb[FromNode][i];

		// Group
		m_RchkData.BEAM.bTopGroup[ToNode][i]   = m_RchkData.BEAM.bTopGroup[FromNode][i];
		m_RchkData.BEAM.nTopGroupNum[ToNode][i]   = m_RchkData.BEAM.nTopGroupNum[FromNode][i];
		m_RchkData.BEAM.dTopGroupGap[ToNode][i]   = m_RchkData.BEAM.dTopGroupGap[FromNode][i];
		m_RchkData.BEAM.bBotGroup[ToNode][i]   = m_RchkData.BEAM.bBotGroup[FromNode][i];
		m_RchkData.BEAM.nBotGroupNum[ToNode][i]   = m_RchkData.BEAM.nBotGroupNum[FromNode][i];
		m_RchkData.BEAM.dBotGroupGap[ToNode][i]   = m_RchkData.BEAM.dBotGroupGap[FromNode][i];
	}

	// Stirrup
	m_RchkData.BEAM.strSubBarNa[ToNode] = m_RchkData.BEAM.strSubBarNa[FromNode];
	m_RchkData.BEAM.dSubBarDist[ToNode] = m_RchkData.BEAM.dSubBarDist[FromNode];
	m_RchkData.BEAM.dSubBarNum[ToNode]  = m_RchkData.BEAM.dSubBarNum[FromNode];
	m_RchkData.BEAM.dSubBarAngle[ToNode]  = m_RchkData.BEAM.dSubBarAngle[FromNode];
	
	// Bent Up Bar
	m_RchkData.BEAM.bBentUpBar[ToNode] = m_RchkData.BEAM.bBentUpBar[FromNode];
	m_RchkData.BEAM.strBentUpBar[ToNode] = m_RchkData.BEAM.strBentUpBar[FromNode];
	m_RchkData.BEAM.dBentUpBarNum[ToNode] = m_RchkData.BEAM.dBentUpBarNum[FromNode];
	m_RchkData.BEAM.dBentUpBarAngle[ToNode] = m_RchkData.BEAM.dBentUpBarAngle[FromNode];
	
	// Torsional Reinforcement
	m_RchkData.BEAM.bTorsionalBar[ToNode]  = m_RchkData.BEAM.bTorsionalBar[FromNode];
	m_RchkData.BEAM.strTorBarNa[ToNode]    = m_RchkData.BEAM.strTorBarNa[FromNode];
	m_RchkData.BEAM.dTorBarSpacing[ToNode] = m_RchkData.BEAM.dTorBarSpacing[FromNode];
	m_RchkData.BEAM.bBundledBar[ToNode]    = m_RchkData.BEAM.bBundledBar[FromNode];
	m_RchkData.BEAM.dBundledBarNum[ToNode] = m_RchkData.BEAM.dBundledBarNum[FromNode];
	m_RchkData.BEAM.strLongiBarNa[ToNode]  = m_RchkData.BEAM.strLongiBarNa[FromNode];
	m_RchkData.BEAM.dLongiBarNum[ToNode]   = m_RchkData.BEAM.dLongiBarNum[FromNode];
}

BOOL CDgnConBeamReinforceCommonDlgCIVIL::CheckInputData()
{

	// PropNo Check
	T_SECT_D sData;
	sData.Initialize();
	BOOL bGetOK = m_pDoc->m_pAttrCtrl->GetSectDesign(m_RchkKey, sData);
	
	if(!bGetOK){
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_Property_Num_Err));
//    AfxMessageBox(_T("[오류] Property Number 선택이 적당하지 않습니다."));
		return FALSE;
	}

	// Rebar Check
	int  nInput = 0;
	BOOL bInput[3];
	for(int i=0; i<3; i++) bInput[i] = TRUE;
	if(m_RchkData.BEAM.iTopBarNum[0][0] == 0) bInput[0] = FALSE; // I_Node
	if(m_RchkData.BEAM.iTopBarNum[1][0] == 0) bInput[1] = FALSE; // Middle
	if(m_RchkData.BEAM.iTopBarNum[2][0] == 0) bInput[2] = FALSE; // J_Node

	for(int i=0; i<3; i++) if(bInput[i] == TRUE) nInput = nInput + 1;

	if(nInput == 0)          // 입력이 하나도 안됨
	{
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_Rebar_Info_Err));
//    AfxMessageBox(_T("[오류] Rebar 정보가 입력되지 않았습니다."));
		return FALSE;
	}
	else if(nInput == 3)     // 셋다 입력    
	{
		return TRUE;
	}
	else if(nInput == 1)    // 하나만 입력
	{
		if(bInput[0] == TRUE){        // I_Node 
			CopyRebarData(0, 1);
			CopyRebarData(0, 2);
			//WriteNodeInfoOnGrid(1);
			//WriteNodeInfoOnGrid(2);
		}
		else if(bInput[1] == TRUE){    // Middle
			CopyRebarData(1, 0);
			CopyRebarData(1, 2);
			//WriteNodeInfoOnGrid(0);
			//WriteNodeInfoOnGrid(2);
		}
		else if(bInput[2] == TRUE){    // J_Node
			CopyRebarData(2, 0);
			CopyRebarData(2, 1);
			//WriteNodeInfoOnGrid(0);
			//WriteNodeInfoOnGrid(1);
		}
		return TRUE;
	}
	else if(nInput == 2)    // 두개 입력
	{
		if(bInput[0] == FALSE)        // I_Node X
		{
			CopyRebarData(2, 0);
			//WriteNodeInfoOnGrid(0);
		}
		else if(bInput[1] == FALSE)   // Middle X
		{
			CopyRebarData(0, 1);
			//WriteNodeInfoOnGrid(1);
		}
		else if(bInput[2] == FALSE)   // J_Node X
		{
			CopyRebarData(0, 2);
			//WriteNodeInfoOnGrid(2);
		}

		return TRUE;
	}

	return TRUE;
}

BOOL CDgnConBeamReinforceCommonDlgCIVIL::CheckInputData_New()
{
	
	// PropNo Check
	T_SECT_D sData;
	sData.Initialize();
	BOOL bGetOK = m_pDoc->m_pAttrCtrl->GetSectDesign(m_RchkKey, sData);
	
	if(!bGetOK){
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_Property_Num_Err));
		//    AfxMessageBox(_T("[오류] Property Number 선택이 적당하지 않습니다."));
		return FALSE;
	}
	
	// Rebar Check
	int  nInput = 0;
	BOOL bInput[3];
	for(int i=0; i<3; i++) bInput[i] = TRUE;
	if(m_RchkData.BEAM.iTopBarNum[0][0] == 0 && m_RchkData.BEAM.iBotBarNum[0][0] == 0) bInput[0] = FALSE; // I_Node
	if(m_RchkData.BEAM.iTopBarNum[1][0] == 0 && m_RchkData.BEAM.iBotBarNum[1][0] == 0) bInput[1] = FALSE; // Middle
	if(m_RchkData.BEAM.iTopBarNum[2][0] == 0 && m_RchkData.BEAM.iBotBarNum[2][0] == 0) bInput[2] = FALSE; // J_Node
	
	for(int i=0; i<3; i++) if(bInput[i] == TRUE) nInput = nInput + 1;
	 
	if(nInput == 0)          // 입력이 하나도 안됨
	{
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_Rebar_Info_Err));
		//    AfxMessageBox(_T("[오류] Rebar 정보가 입력되지 않았습니다."));
		return FALSE;
	}
	else if(nInput == 3)     // 셋다 입력    
	{
		return TRUE;
	}
	else if(nInput == 1)    // 하나만 입력
	{
		if(bInput[0] == TRUE){        // I_Node 
			CopyRebarData(0, 1);
			CopyRebarData(0, 2);
			//WriteNodeInfoOnGrid(1);
			//WriteNodeInfoOnGrid(2);
		}
		else if(bInput[1] == TRUE){    // Middle
			CopyRebarData(1, 0);
			CopyRebarData(1, 2);
			//WriteNodeInfoOnGrid(0);
			//WriteNodeInfoOnGrid(2);
		}
		else if(bInput[2] == TRUE){    // J_Node
			CopyRebarData(2, 0);
			CopyRebarData(2, 1);
			//WriteNodeInfoOnGrid(0);
			//WriteNodeInfoOnGrid(1);
		}
		return TRUE;
	}
	else if(nInput == 2)    // 두개 입력
	{
		if(bInput[0] == FALSE)        // I_Node X
		{
			CopyRebarData(2, 0);
			//WriteNodeInfoOnGrid(0);
		}
		else if(bInput[1] == FALSE)   // Middle X
		{
			CopyRebarData(0, 1);
			//WriteNodeInfoOnGrid(1);
		}
		else if(bInput[2] == FALSE)   // J_Node X
		{
			CopyRebarData(0, 2);
			//WriteNodeInfoOnGrid(2);
		}
		
		return TRUE;
	}
	
	return TRUE;
}

BOOL CDgnConBeamReinforceCommonDlgCIVIL::SetDlg2Data()
{
	BOOL bCheck = TRUE;
	UpdateData(TRUE);

	m_RchkData.nEnvironType = m_bCrack;
	m_RchkData.nType = 0;

	return bCheck;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// Change Prop. CB 관련 
//

//---------------------------------------------------------------------------
// SelectElemOnView 
//---------------------------------------------------------------------------
void CDgnConBeamReinforceCommonDlgCIVIL::SelectElemOnView()
{  
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);

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
					if(DataCtrl.Get_DgnGenMbtp(ElemKey) == D_MBTP_BEAM)  arElemKeyList.Add(ElemKey);
		}
	}
	
	m_pDoc->m_pViewCtrl->SelectElem(NULL, arElemKeyList, FALSE);
}

//---------------------------------------------------------------------------
// ShowHideSectCtrl 
//---------------------------------------------------------------------------
void CDgnConBeamReinforceCommonDlgCIVIL::ShowHideSectCtrl(BOOL bShow)
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
// ChangeSectionInfo 
//---------------------------------------------------------------------------
void CDgnConBeamReinforceCommonDlgCIVIL::ChangeSectionInfo()
{

	// Select Elem. On View
	SelectElemOnView();

	T_SECT_D SectData; SectData.Initialize();

	if(!m_pDoc->m_pAttrCtrl->GetSect(m_RchkKey, SectData))	return;

	// Set TEE Section Data
	if(SectData.SectBefore.Shape==D_SECT_SHAPE_REG_T)
	{
		ShowTEEBitmap();
		ShowHideSectCtrl(TRUE);

		GetDlgItem(IDC_DGN_RC_BSHAPE)->SetWindowText(_LS(IDS_DGN_GRID_TEXT_SECT_TEE));  
		m_Hc.SetEditUnit(SectData.SectBefore.SectI.Size[0]);   //  H
		m_Bf.SetEditUnit(SectData.SectBefore.SectI.Size[1]);   //  B
		m_Bc.SetEditUnit(SectData.SectBefore.SectI.Size[2]);   //  Tw
		m_Hf.SetEditUnit(SectData.SectBefore.SectI.Size[3]);   //  Tf1
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

	// Set Rebar Data
	//WriteInfoGridTitle();

	m_RchkData.Initialize();
	m_pDoc->m_pAttrCtrl->GetRchk(m_RchkKey, m_RchkData);

	//for(int i=0; i<3; i++) WriteNodeInfoOnGrid(i);

	m_bCrack  = m_RchkData.nEnvironType;

	if (m_pDoc->m_pAttrCtrl->IsDcon_CrackForAASHTO())
	{
		if (m_bCrack > 1) m_bCrack = 0;
	}

	UpdateData(FALSE);  
}

void CDgnConBeamReinforceCommonDlgCIVIL::ChangeDlgItemInfo()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_DGN_CON_BEAM_CRACK1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CON_BEAM_CRACK2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CON_BEAM_CRACK3)->ShowWindow(SW_HIDE);  
	GetDlgItem(IDC_DGN_CON_BEAM_CRACK4)->ShowWindow(SW_HIDE); 
	GetDlgItem(IDC_DGN_CONC_CrackFrm)->ShowWindow(SW_HIDE);

	CStringArray aCrackEnv;
	aCrackEnv.RemoveAll();

	int iCrackEnvNum = CDBLib::GetCrackEnvList(aCrackEnv);

//  AlignControls(iCrackEnvNum);

	if(iCrackEnvNum ==1)
	{
		m_bCrack = 0;
		GetDlgItem(IDC_DGN_CONC_CrackFrm)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CON_BEAM_CRACK1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CON_BEAM_CRACK1)->SetWindowText(aCrackEnv[0]);
	}
	else if(iCrackEnvNum ==2)
	{
		if(m_bCrack > 1) m_bCrack = 0;
		GetDlgItem(IDC_DGN_CONC_CrackFrm)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CON_BEAM_CRACK1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CON_BEAM_CRACK1)->SetWindowText(aCrackEnv[0]);
		GetDlgItem(IDC_DGN_CON_BEAM_CRACK2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CON_BEAM_CRACK2)->SetWindowText(aCrackEnv[1]);
	}
	else if(iCrackEnvNum == 3)
	{
		if(m_bCrack > 2) m_bCrack = 0;
		GetDlgItem(IDC_DGN_CONC_CrackFrm)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CON_BEAM_CRACK1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CON_BEAM_CRACK1)->SetWindowText(aCrackEnv[0]);
		GetDlgItem(IDC_DGN_CON_BEAM_CRACK2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CON_BEAM_CRACK2)->SetWindowText(aCrackEnv[1]);
		GetDlgItem(IDC_DGN_CON_BEAM_CRACK3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CON_BEAM_CRACK3)->SetWindowText(aCrackEnv[2]);
	}
	else if(iCrackEnvNum == 4)
	{
		GetDlgItem(IDC_DGN_CONC_CrackFrm)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CON_BEAM_CRACK1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CON_BEAM_CRACK1)->SetWindowText(aCrackEnv[0]);
		GetDlgItem(IDC_DGN_CON_BEAM_CRACK2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CON_BEAM_CRACK2)->SetWindowText(aCrackEnv[1]);
		GetDlgItem(IDC_DGN_CON_BEAM_CRACK3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CON_BEAM_CRACK3)->SetWindowText(aCrackEnv[2]);
		GetDlgItem(IDC_DGN_CON_BEAM_CRACK4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CON_BEAM_CRACK4)->SetWindowText(aCrackEnv[3]);
	}

	m_bTorsionalBar = m_RchkData.BEAM.bTorsionalBar[0];
	if(m_cmbTorBar.SelectString(-1, m_RchkData.BEAM.strTorBarNa[0])==LB_ERR) 
		m_cmbTorBar.SetCurSel(-1);
	m_edtTorSpacing.SetEditUnit(m_RchkData.BEAM.dTorBarSpacing[0]);
	m_bBundledBar = m_RchkData.BEAM.bBundledBar[0];
	m_edtBundledBarNum.SetEditUnit(m_RchkData.BEAM.dBundledBarNum[0]);
	if(m_cmbLongiBar.SelectString(-1, m_RchkData.BEAM.strLongiBarNa[0])==LB_ERR) 
		m_cmbLongiBar.SetCurSel(-1);
	m_edtLongiBarNum.SetEditUnit(m_RchkData.BEAM.dLongiBarNum[0]);

	m_edtStirrupAngle.SetEditUnit(m_RchkData.BEAM.dSubBarAngle[0]);
	m_bBentUpBar = m_RchkData.BEAM.bBentUpBar[0];
	if(m_cmbBentUpBatSize.SelectString(-1, m_RchkData.BEAM.strBentUpBar[0])==LB_ERR) 
		m_cmbBentUpBatSize.SetCurSel(-1);
	m_edtBentUpBarNum.SetEditUnit(m_RchkData.BEAM.dBentUpBarNum[0]);
	m_edtBentUpBarAngle.SetEditUnit(m_RchkData.BEAM.dBentUpBarAngle[0]);

	UpdateData(FALSE);
	Invalidate();
}

//-------------------------------------------------
//Tab에 관련된 함수들..
//---------------------------------------------------
void CDgnConBeamReinforceCommonDlgCIVIL::SetRchkData(int nIMJ, T_RCHK_K RchkKey, T_RCHK_BEAM* pBeamData)
{
	m_nIMJ = nIMJ;
	m_RchkKey = RchkKey;
	m_pBeamData = NULL;
	m_pBeamData = pBeamData;
}

void CDgnConBeamReinforceCommonDlgCIVIL::SetLayer_Change()
{
	// Rebar Layer
	for(int i=0; i<3; i++)
	{
		int TopLayer = 0;
		int BotLayer = 0;
		for(int j=0; j<T_RCHK_BEAM_LAYER; j++)
		{
			if(m_pBeamData->iTopBarNum[i][j]>0) TopLayer++;
			if(m_pBeamData->iBotBarNum[i][j]>0) BotLayer++;
		}
		
//     m_nTPosLayer[i] = TopLayer;
//     m_nBPosLayer[i] = BotLayer;      
		if(TopLayer == 0) m_nTPosLayer[i] = 1;
		else              m_nTPosLayer[i] = TopLayer;
		
		if(BotLayer == 0) m_nBPosLayer[i] = 1;
		else              m_nBPosLayer[i] = BotLayer;      
	}
}

void CDgnConBeamReinforceCommonDlgCIVIL::SetData2Dlg_Rebar()
{
	m_cStirrupSpace.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_cStirrupSpaceUT.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_AsTop.SetUnitType(D_UNITSYS_BASE_AREA);
	m_AsTopUT.SetUnitType(D_UNITSYS_BASE_AREA);
	m_AsBot.SetUnitType(D_UNITSYS_BASE_AREA);
	m_AsBotUT.SetUnitType(D_UNITSYS_BASE_AREA);
	m_cStirrupNum.SetUnitType(D_UNITSYS_NONE);

	m_edtTorSpacing.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untTorSpacing.SetUnitType(D_UNITSYS_BASE_LENGTH);
	
	m_edtBundledBarNum.SetUnitType(D_UNITSYS_NONE);
	m_edtLongiBarNum.SetUnitType(D_UNITSYS_NONE);

	// Stirrup Bar
//	m_cStirrupNum.SelectString(-1, m_pBeamData->dSubBarNum[m_nIMJ]);
	m_cStirrupNum.SetEditUnit(m_pBeamData->dSubBarNum[m_nIMJ]);
	m_cStirrupSpace.SetEditUnit(m_pBeamData->dSubBarDist[m_nIMJ]);
	m_cStirrupSize.SelectString(-1, m_pBeamData->strSubBarNa[m_nIMJ]);

	m_bTorsionalBar = m_pBeamData->bTorsionalBar[m_nIMJ];
	m_cmbTorBar.SelectString(-1, m_pBeamData->strTorBarNa[m_nIMJ]);
	m_edtTorSpacing.SetEditUnit(m_pBeamData->dTorBarSpacing[m_nIMJ]);
	m_bBundledBar = m_pBeamData->bBundledBar[m_nIMJ];

	m_edtBundledBarNum.SetEditUnit(m_pBeamData->dBundledBarNum[m_nIMJ]);
	m_cmbLongiBar.SelectString(-1, m_pBeamData->strLongiBarNa[m_nIMJ]);
	m_edtLongiBarNum.SetEditUnit(m_pBeamData->dLongiBarNum[m_nIMJ]);

	m_edtStirrupAngle.SetEditUnit(m_pBeamData->dSubBarAngle[m_nIMJ]);
	m_bBentUpBar = m_pBeamData->bBentUpBar[m_nIMJ];
	m_cmbBentUpBatSize.SelectString(-1, m_pBeamData->strBentUpBar[m_nIMJ]);
	m_edtBentUpBarNum.SetEditUnit(m_pBeamData->dBentUpBarNum[m_nIMJ]);
	m_edtBentUpBarAngle.SetEditUnit(m_pBeamData->dBentUpBarAngle[m_nIMJ]);

	int nIndex;

	// Rebar Layer
	CString str;
	m_nTopLayer = 0;
	m_nBotLayer = 0;
	for(int i=0; i<T_RCHK_BEAM_LAYER; i++){
		if(m_pBeamData->iTopBarNum[m_nIMJ][i]>0) m_nTopLayer++;
		if(m_pBeamData->iBotBarNum[m_nIMJ][i]>0) m_nBotLayer++;
	}

	if(m_nTopLayer == 0) m_nTopLayer = 1;        
	str.Format(_T("%d"), m_nTopLayer);
	nIndex = m_cTopLayerCB.SelectString(-1, str);    
	ASSERT(nIndex != LB_ERR);

	if(m_nBotLayer == 0) m_nBotLayer = 1;
	str.Format(_T("%d"), m_nBotLayer);
	nIndex = m_cBotLayerCB.SelectString(-1, str);    
	ASSERT(nIndex != LB_ERR);

	// Initialize는 그리드 내부에서..
	SetDataToGrid();
	SetLayerNumToGrid(0, m_nTPosLayer[m_nIMJ]);
	SetLayerNumToGrid(1, m_nBPosLayer[m_nIMJ]);
	
	double dAsTop=0.0, dAsBot=0.0;
	CalcAs(*m_pBeamData, dAsTop, dAsBot);

	UpdateAsInfo(dAsTop, dAsBot);
	UpdateData(FALSE);
}

BOOL CDgnConBeamReinforceCommonDlgCIVIL::CheckPosiInputData_New(int iIMJ)
{
	CString strSubBarName=_T("");
	int nIndex = m_nStiCBox[iIMJ];
	
	double dZero = 1.0E-7;
	if(nIndex >= 0) 
		m_cStirrupSize.GetLBText(nIndex, strSubBarName);
	
	if(fabs(m_TabBeamData.dSubBarNum[iIMJ]-2.)>dZero) return FALSE;
	if(m_TabBeamData.dSubBarDist[iIMJ]!=0.0) return FALSE;
	//if(m_TabBeamData.strSubBarNa[iIMJ]!=_T("")) return FALSE;
	
	// 	if(m_TabBeamData.dTorBarSpacing[iIMJ]!=0.0) return FALSE;
	// 	if(m_TabBeamData.dBundledBarNum[iIMJ]!=0.0) return FALSE;
	// 	if(m_TabBeamData.dLongiBarNum[iIMJ] >= 4) return FALSE;	
	
	//Tob
	BOOL bCheck = TRUE;
	for(int j=0; j<m_nTPosLayer[iIMJ]; j++)
	{
		if(m_TabBeamData.dDt[iIMJ][j]!=0.0)        return FALSE; 
		if(m_TabBeamData.iTopBarNum[iIMJ][j]!=0.0) return FALSE; 
		if(m_TabBeamData.strTopBarNa1[iIMJ][j]!=_T("")) return FALSE; 
		if(m_TabBeamData.strTopBarNa2[iIMJ][j]!=_T("")) return FALSE; 
		if(m_TabBeamData.bTopGroup[iIMJ][j])
		{
			if(m_TabBeamData.nTopGroupNum[iIMJ][j] < 1) return FALSE;
			if(m_TabBeamData.dTopGroupGap[iIMJ][j] < 0.0) return FALSE;
		}
	}
	//Bot  
	for(int j=0; j<m_nTPosLayer[iIMJ]; j++)
	{
		if(m_TabBeamData.dDb[iIMJ][j]!=0.0) return FALSE; 
		if(m_TabBeamData.iBotBarNum[iIMJ][j]!=0.0) return FALSE; 
		if(m_TabBeamData.strBotBarNa1[iIMJ][j]!=_T("")) return FALSE; 
		if(m_TabBeamData.strBotBarNa2[iIMJ][j]!=_T("")) return FALSE; 
		if(m_TabBeamData.bBotGroup[iIMJ][j])
		{
			if(m_TabBeamData.nBotGroupNum[iIMJ][j] < 1) return FALSE;
			if(m_TabBeamData.dBotGroupGap[iIMJ][j] < 0.0) return FALSE;
		}
	}
	
	return TRUE;
}

BOOL CDgnConBeamReinforceCommonDlgCIVIL::CheckInputData_Rebar_New()
{
	int iChkResult = 0;
	_DGNC_RBAR RebarData; RebarData.Initialize();
		
	int nError=0;
	CString str, errMsg;
	CDgnDataCtrl DataCtrl;
	T_DCON_D DconD; DconD.Initialize();
	DataCtrl.Get_DgnConDcon(DconD);

	int Index=0;
	T_RCHK_K RchkKey; 
	for(int iPosi=0; iPosi<3; iPosi++)
	{
		int nIndex = m_nStiCBox[iPosi];
		if(!CheckPosiInputData_New(iPosi)) 
		{
			POSITION Pos = m_arRchkKey.GetStartPosition();
	    while(Pos)
	    {
				RebarData.Initialize();
		    RchkKey=0; 
				str=_T(""); errMsg=_T("");
		    m_arRchkKey.GetNextAssoc(Pos, Index, RchkKey);
		
				if(nIndex >= 0) 
					m_cStirrupSize.GetLBText(nIndex, RebarData.strSubBarNa);

				RebarData.iSectNo  = RchkKey;
				RebarData.iBarLayt = m_nTPosLayer[iPosi];
				RebarData.iBarLayb = m_nBPosLayer[iPosi];

				// Top
				for(int i=0; i<m_nTPosLayer[iPosi]; i++)
				{     
					RebarData.iBarNum[0][i] = m_TabBeamData.iTopBarNum[iPosi][i];
					if(RebarData.iBarNum[0][i] < 0)
					 iChkResult |= 2;
			
					RebarData.dDt[i] = m_TabBeamData.dDt[iPosi][i];
					double CL = 0;
					if(i == 0) CL = 0;
					else       CL = RebarData.dDt[i-1];

					if(i == 0)
					{
						if(RebarData.dDt[i] < CL)
							iChkResult |= 4;
					}
					else
					{
						if(RebarData.dDt[i] <= CL)
							iChkResult |= 4;
					}
					RebarData.strBarNa1[0][i] = m_TabBeamData.strTopBarNa1[iPosi][i];
					RebarData.strBarNa2[0][i] = m_TabBeamData.strTopBarNa2[iPosi][i];
				}

				// Bottom
				for(int i=0; i<m_nBPosLayer[iPosi] ; i++)
				{      
					RebarData.iBarNum[1][i] = m_TabBeamData.iBotBarNum[iPosi][i];
					if(RebarData.iBarNum[1][i] < 0)
						iChkResult |= 8;      
					RebarData.dDb[i] = m_TabBeamData.dDb[iPosi][i];
					double CL = 0;
					if(i == 0) CL = 0;
					else       CL = RebarData.dDb[i-1];

					if(i == 0) 
					{
						if(RebarData.dDb[i] < CL)
							iChkResult |= 16;
					}
					else
					{
						if(RebarData.dDb[i] <= CL)
							iChkResult |= 16;
					}
			
					RebarData.strBarNa1[1][i] = m_TabBeamData.strBotBarNa1[iPosi][i];
					RebarData.strBarNa2[1][i] = m_TabBeamData.strBotBarNa2[iPosi][i];
				}

				// Check Rebar Data
				nError = DataCtrl.Check_RbarCivil(1, RebarData);

				if(nError == 0)  
				{
					// Top
					for(int i=0; i<m_nTPosLayer[iPosi]; i++)
					{
						if(RebarData.bCheckRebar[0][i] == FALSE)
						{ iChkResult |= 32; }

						if(RebarData.bCheckSpace[0][i] == FALSE)
						{ iChkResult |= 64; }

						if(RebarData.bCheckCovt[i] == FALSE)
						{ iChkResult |= 128; }
					}

					// Bottom
					for(int i=0; i<m_nBPosLayer[iPosi]; i++)
					{
						if(RebarData.bCheckRebar[1][i] == FALSE)
						{ iChkResult |= 256; }

						if(RebarData.bCheckSpace[1][i] == FALSE)
						{ iChkResult |= 512; }

						if(RebarData.bCheckCovb[i] == FALSE)
						{ iChkResult |= 1024; }
					}
				}
				else
				{
					if(nError == 1)       iChkResult |= 2048;
					else if(nError == 2)  iChkResult |= 4096;
					else if(nError == 3)  iChkResult |= 8192;
				}

				// Check Stirrup Bar
				if(m_TabBeamData.dSubBarNum[iPosi]<= 0.)    iChkResult |= 16384; // m_cStirrupNum.GetEditValue()
				
				if(m_TabBeamData.dSubBarDist[iPosi] <= 0.)  iChkResult |= 32768; // m_cStirrupSpace.GetEditValue()

				CString strSubBarNa;
				nIndex = m_nStiCBox[iPosi]; // m_cStirrupSize.GetCurSel();
				if(nIndex < 0)  iChkResult |= 65536;
			}
		 

		} //if(CheckPosiInputData(iPosi))
		else { bExistData[iPosi]=FALSE; } // FALSE==No Input Data.
	}
 
	if(iChkResult & 2)
	{
		errMsg.Format(_LS(IDS_DGN_CON_BEAM_Top_Rebar_Num_Err));
		//      errMsg.Format(_T("[오류] Top Rebar 정보 중 Layer의 철근 개수가 적당하지 않습니다."));
		AfxMessageBox(errMsg);
		return FALSE;
	}
	else if(iChkResult & 4)
	{
		errMsg.Format(_LS(IDS_DGN_CON_BEAM_Top_Rebar_Dc_Err));
		//      errMsg.Format(_T("[오류] Top Rebar 정보 중 Layer의 Dc 값이 적당하지 않습니다."));
		AfxMessageBox(errMsg);
		return FALSE;
	}
	else if(iChkResult & 8)
	{
		errMsg.Format(_LS(IDS_DGN_CON_BEAM_Bot_Rebar_Num_Err));
		//      errMsg.Format(_T("[오류] Bottom Rebar 정보 중 Layer의 철근 개수가 적당하지 않습니다."));
		AfxMessageBox(errMsg);
		return FALSE;
	}
	else if(iChkResult & 16)
	{
		errMsg.Format(_LS(IDS_DGN_CON_BEAM_Bot_Rebar_Dc_Err));
		//      errMsg.Format(_T("[오류] Bottom Rebar 정보 중 Layer의 Dc 값이 적당하지 않습니다."));
		AfxMessageBox(errMsg);
		return FALSE;
	}
	else if(iChkResult & 32)
	{
		errMsg.Format(_LS(IDS_DGN_CON_BEAM_Top_Rebar_Info_Err));
		//errMsg.Format(_T("[오류] Top Rebar 정보 중 Layer의 Rebar 정보가 올바르지 않습니다."));
		AfxMessageBox(errMsg);
		return FALSE;        
	}
	else if(iChkResult & 256)
	{
		errMsg.Format(_LS(IDS_DGN_CON_BEAM_Bot_Rebar_Info_Warning));
//        errMsg.Format(_T("[오류] Bottom Rebar 정보 중 Layer의 Rebar 정보가 올바르지 않습니다."));
		AfxMessageBox(errMsg);
		return FALSE;
	}
	else if(iChkResult & 2048)
	{
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_No_good_Code));  /*AfxMessageBox(_T("No good Code"));*/       
		return FALSE;  
	}
	else if(iChkResult & 4096)
	{
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_Not_exist_Section)); /*AfxMessageBox(_T("Not exist Section."));*/ 
		return FALSE;  
	}
	else if(iChkResult & 8192)
	{
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_No_good_Section));   /*AfxMessageBox(_T("No good Section."));*/   
		return FALSE; 
	}
	else if(iChkResult & 16384)
	{
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_Stirrup_Bar_Num_Err));
//    AfxMessageBox(_T("[오류] Stirrup(Spacing) 입력이 적당하지 않습니다."));
		return FALSE;
	}
	else if(iChkResult & 32768)
	{
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_Stirrup_Bar_Spacing_Err));
//    AfxMessageBox(_T("[오류] Stirrup(Spacing) 입력이 적당하지 않습니다."));
		return FALSE;
	}
	else if(iChkResult & 65536)
	{
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_Stirrup_Bar_Size_Err));
//    AfxMessageBox(_T("[오류] Stirrup(Size) 입력이 적당하지 않습니다."));
		return FALSE;
	}
	else if(iChkResult & 64)
	{
		errMsg.Format(_LS(IDS_DGN_CON_BEAM_Top_Rebar_Space_Warning));
//        errMsg.Format(_T("[경고] Top Rebar 정보 중 Layer의 철근 간격이 적당하지 않습니다.\n계속 하시겠습니까?"));
		if(AfxMessageBox(errMsg, MB_YESNO) == IDNO) return FALSE;
	}
	else if(iChkResult & 128)
	{
		if( DconD.DesignCode!= CONCODE_SNIP_2_05_03_84 && DconD.DesignCode!= CONCODE_SP_35_13330_2011 && 
			DconD.DesignCode!= CONCODE_SNIP_2_05_03_84_MKS && DconD.DesignCode!= CONCODE_SP_35_13330_2011_MKS &&
			DconD.DesignCode != CONCODE_AS5100_5_17)
		{
			errMsg.Format(_LS(IDS_DGN_CON_BEAM_Top_Rebar_Dc_Warning));
//        errMsg.Format(_T("[경고] Top Rebar 정보 중 Layer의 Dc 값이 적당하지 않습니다.\n계속 하시겠습니까?"));
			if(AfxMessageBox(errMsg, MB_YESNO) == IDNO) return FALSE;
		}
	 }
	else if(iChkResult & 512)
	{
		if( DconD.DesignCode!= CONCODE_SNIP_2_05_03_84 && DconD.DesignCode!= CONCODE_SP_35_13330_2011 && 
			DconD.DesignCode!= CONCODE_SNIP_2_05_03_84_MKS && DconD.DesignCode!= CONCODE_SP_35_13330_2011_MKS &&
			DconD.DesignCode != CONCODE_AS5100_5_17)
		{
			errMsg.Format(_LS(IDS_DGN_CON_BEAM_Bot_Rebar_Space_Warning));
//        errMsg.Format(_T("[경고] Bottom Rebar 정보 중 Layer의 철근 간격이 적당하지 않습니다.\n계속 하시겠습니까?"));
		if(AfxMessageBox(errMsg, MB_YESNO) == IDNO) return FALSE;
		}
	}
	else if(iChkResult & 1024)
	{
		errMsg.Format(_LS(IDS_DGN_CON_BEAM_Bot_Rebar_Dc_Warning));
//        errMsg.Format(_T("[경고] Bottom Rebar 정보 중 Layer의 Dc 값이 적당하지 않습니다.\n계속 하시겠습니까?"));
		if(AfxMessageBox(errMsg, MB_YESNO) == IDNO) return FALSE;
	}

	return TRUE;
}

BOOL CDgnConBeamReinforceCommonDlgCIVIL::CheckReDraw_Rebar()
{
	int iChkResult = 0;
	_DGNC_RBAR RebarData;
	
	int nIndex = m_cStirrupSize.GetCurSel();
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
		
		if(nIndex >= 0) 
			m_cStirrupSize.GetLBText(nIndex, RebarData.strSubBarNa);

		RebarData.iSectNo  = RchkKey;
		RebarData.iBarLayt = m_nTopLayer;
		RebarData.iBarLayb = m_nBotLayer;

		if(!CheckReDraw_RebarGrid(RebarData, iChkResult))	return FALSE; // return FALSE인경우에 메세지 처리는 CheckReDraw_RebarGrid()안에서 하시오..

		// Check Rebar Data
		nError = DataCtrl.Check_RbarCivil(1, RebarData);

		if(nError == 0)  
		{
			// Top
			for(int i=0; i<m_nTopLayer; i++)
			{
				if(RebarData.bCheckRebar[0][i] == FALSE)
					iChkResult |= 32;

				if(RebarData.bCheckSpace[0][i] == FALSE)
					iChkResult |= 64;

				if(RebarData.bCheckCovt[i] == FALSE)
					iChkResult |= 128;
			}

			// Bottom
			for(int i=0; i<m_nBotLayer; i++)
			{
				if(RebarData.bCheckRebar[1][i] == FALSE)
					iChkResult |= 256;

				if(RebarData.bCheckSpace[1][i] == FALSE)
					iChkResult |= 512;

				if(RebarData.bCheckCovb[i] == FALSE)
					iChkResult |= 1024;
			}
		}
		else
		{
			if(nError == 1)       iChkResult |= 2048;
			else if(nError == 2)  iChkResult |= 4096;
			else if(nError == 3)  iChkResult |= 8192;
		}

		// Check Stirrup Bar
		if(m_cStirrupNum.GetEditValue() <= 1.)    iChkResult |= 16384;

		if(m_cStirrupSpace.GetEditValue() <= 0.)  iChkResult |= 32768;

		CString strSubBarNa;
		nIndex = m_cStirrupSize.GetCurSel();
		if(nIndex < 0)  iChkResult |= 65536;
	}
 
	if(iChkResult & 2)
	{
		errMsg.Format(_LS(IDS_DGN_CON_BEAM_Top_Rebar_Num_Err));
//      errMsg.Format(_T("[오류] Top Rebar 정보 중 Layer의 철근 개수가 적당하지 않습니다."));
		AfxMessageBox(errMsg);
		return FALSE;

	}
	else if(iChkResult & 4)
	{
		errMsg.Format(_LS(IDS_DGN_CON_BEAM_Top_Rebar_Dc_Err));
//      errMsg.Format(_T("[오류] Top Rebar 정보 중 Layer의 Dc 값이 적당하지 않습니다."));
		AfxMessageBox(errMsg);
		return FALSE;
	}
	else if(iChkResult & 8)
	{
		errMsg.Format(_LS(IDS_DGN_CON_BEAM_Bot_Rebar_Num_Err));
//      errMsg.Format(_T("[오류] Bottom Rebar 정보 중 Layer의 철근 개수가 적당하지 않습니다."));
		AfxMessageBox(errMsg);
		return FALSE;
	}
	else if(iChkResult & 16)
	{
		errMsg.Format(_LS(IDS_DGN_CON_BEAM_Bot_Rebar_Dc_Err));
//      errMsg.Format(_T("[오류] Bottom Rebar 정보 중 Layer의 Dc 값이 적당하지 않습니다."));
		AfxMessageBox(errMsg);
		return FALSE;
	}
	else if(iChkResult & 32)
	{
		errMsg.Format(_LS(IDS_DGN_CON_BEAM_Top_Rebar_Info_Err));
		//errMsg.Format(_T("[오류] Top Rebar 정보 중 Layer의 Rebar 정보가 올바르지 않습니다."));
		AfxMessageBox(errMsg);
		return FALSE;        
	}
	else if(iChkResult & 256)
	{
		errMsg.Format(_LS(IDS_DGN_CON_BEAM_Bot_Rebar_Info_Warning));
//        errMsg.Format(_T("[오류] Bottom Rebar 정보 중 Layer의 Rebar 정보가 올바르지 않습니다."));
		AfxMessageBox(errMsg);
		return FALSE;
	}
	else if(iChkResult & 2048)
	{
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_No_good_Code));  /*AfxMessageBox(_T("No good Code"));*/       
		return FALSE;  
	}
	else if(iChkResult & 4096)
	{
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_Not_exist_Section)); /*AfxMessageBox(_T("Not exist Section."));*/ 
		return FALSE;  
	}
	else if(iChkResult & 8192)
	{
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_No_good_Section));   /*AfxMessageBox(_T("No good Section."));*/   
		return FALSE; 
	}
	else if(iChkResult & 16384)
	{
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_Stirrup_Bar_Num_Err));
//    AfxMessageBox(_T("[오류] Stirrup(Spacing) 입력이 적당하지 않습니다."));
		return FALSE;
	}
	else if(iChkResult & 32768)
	{
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_Stirrup_Bar_Spacing_Err));
//    AfxMessageBox(_T("[오류] Stirrup(Spacing) 입력이 적당하지 않습니다."));
		return FALSE;
	}
	else if(iChkResult & 65536)
	{
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_Stirrup_Bar_Size_Err));
//    AfxMessageBox(_T("[오류] Stirrup(Size) 입력이 적당하지 않습니다."));
		return FALSE;
	}
	else if(iChkResult & 64)
	{
		errMsg.Format(_LS(IDS_DGN_CON_BEAM_Top_Rebar_Space_Warning));
//        errMsg.Format(_T("[경고] Top Rebar 정보 중 Layer의 철근 간격이 적당하지 않습니다.\n계속 하시겠습니까?"));
		if(AfxMessageBox(errMsg, MB_YESNO) == IDNO) return FALSE;
	}
	else if(iChkResult & 128)
	{
		errMsg.Format(_LS(IDS_DGN_CON_BEAM_Top_Rebar_Dc_Warning));
//        errMsg.Format(_T("[경고] Top Rebar 정보 중 Layer의 Dc 값이 적당하지 않습니다.\n계속 하시겠습니까?"));
		if(AfxMessageBox(errMsg, MB_YESNO) == IDNO) return FALSE;
	}
	else if(iChkResult & 512)
	{
		errMsg.Format(_LS(IDS_DGN_CON_BEAM_Bot_Rebar_Space_Warning));
//        errMsg.Format(_T("[경고] Bottom Rebar 정보 중 Layer의 철근 간격이 적당하지 않습니다.\n계속 하시겠습니까?"));
		if(AfxMessageBox(errMsg, MB_YESNO) == IDNO) return FALSE;
	}
	else if(iChkResult & 1024)
	{
		errMsg.Format(_LS(IDS_DGN_CON_BEAM_Bot_Rebar_Dc_Warning));
//        errMsg.Format(_T("[경고] Bottom Rebar 정보 중 Layer의 Dc 값이 적당하지 않습니다.\n계속 하시겠습니까?"));
		if(AfxMessageBox(errMsg, MB_YESNO) == IDNO) return FALSE;
	}

	return TRUE;
}

void CDgnConBeamReinforceCommonDlgCIVIL::SetDlg2Data_Rebar()
{
	// Stirrup Bar
	for(int iPosi=0; iPosi<3; iPosi++) //0=I, 1=M, 2=J
	{
		int nIndex = m_nStiCBox[iPosi];
		m_pBeamData->dSubBarDist[iPosi] = m_TabBeamData.dSubBarDist[iPosi];
		m_pBeamData->dSubBarNum[iPosi]  = m_TabBeamData.dSubBarNum[iPosi];    
		if(nIndex >= 0) m_cStirrupSize.GetLBText(nIndex, m_pBeamData->strSubBarNa[iPosi]);
		m_pBeamData->dSubBarAngle[iPosi] = m_TabBeamData.dSubBarAngle[iPosi];

		m_pBeamData->bTorsionalBar[iPosi] = m_TabBeamData.bTorsionalBar[iPosi];
		nIndex = m_nTorBar[iPosi];
		if(nIndex >= 0) m_cmbTorBar.GetLBText(nIndex, m_pBeamData->strTorBarNa[iPosi]);
		m_pBeamData->dTorBarSpacing[iPosi]  = m_TabBeamData.dTorBarSpacing[iPosi];    
		m_pBeamData->bBundledBar[iPosi]  = m_TabBeamData.bBundledBar[iPosi];    
		m_pBeamData->dBundledBarNum[iPosi]  = m_TabBeamData.dBundledBarNum[iPosi];    

		nIndex = m_nTorLongiBar[iPosi];
		if(nIndex >= 0) m_cmbTorBar.GetLBText(nIndex, m_pBeamData->strLongiBarNa[iPosi]);
		m_pBeamData->dLongiBarNum[iPosi]  = m_TabBeamData.dLongiBarNum[iPosi];    

		m_pBeamData->bBentUpBar[iPosi] = m_TabBeamData.bBentUpBar[iPosi];
		nIndex = m_nBentUpBar[iPosi];
		if(nIndex >= 0) m_cmbBentUpBatSize.GetLBText(nIndex, m_pBeamData->strBentUpBar[iPosi]);
		m_pBeamData->dBentUpBarNum[iPosi]  = m_TabBeamData.dBentUpBarNum[iPosi];    
		m_pBeamData->dBentUpBarAngle[iPosi]  = m_TabBeamData.dBentUpBarAngle[iPosi];    
		
		// Rebar
		GetDataFromGrid(iPosi);
	}  
}

void CDgnConBeamReinforceCommonDlgCIVIL::GetDataFromGrid(int iIMJ)
{
	// Top Rebar
	for(int i=0; i<T_RCHK_BEAM_LAYER; i++)
	{
		m_pBeamData->iTopBarNum[iIMJ][i]   = m_TabBeamData.iTopBarNum[iIMJ][i];
		m_pBeamData->strTopBarNa1[iIMJ][i] = m_TabBeamData.strTopBarNa1[iIMJ][i];
		m_pBeamData->strTopBarNa2[iIMJ][i] = m_TabBeamData.strTopBarNa2[iIMJ][i];
		m_pBeamData->dDt[iIMJ][i]          = m_TabBeamData.dDt[iIMJ][i];	
		m_pBeamData->bTopGroup[iIMJ][i]    = m_TabBeamData.bTopGroup[iIMJ][i];
		m_pBeamData->nTopGroupNum[iIMJ][i] = m_TabBeamData.nTopGroupNum[iIMJ][i];
		m_pBeamData->dTopGroupGap[iIMJ][i] = m_TabBeamData.dTopGroupGap[iIMJ][i];
	}

	// Bot Rebar
	for(int i=0; i<T_RCHK_BEAM_LAYER; i++)
	{
		m_pBeamData->iBotBarNum[iIMJ][i]   = m_TabBeamData.iBotBarNum[iIMJ][i];
		m_pBeamData->strBotBarNa1[iIMJ][i] = m_TabBeamData.strBotBarNa1[iIMJ][i];
		m_pBeamData->strBotBarNa2[iIMJ][i] = m_TabBeamData.strBotBarNa2[iIMJ][i];
		m_pBeamData->dDb[iIMJ][i]          = m_TabBeamData.dDb[iIMJ][i];
		m_pBeamData->bBotGroup[iIMJ][i]    = m_TabBeamData.bBotGroup[iIMJ][i];
		m_pBeamData->nBotGroupNum[iIMJ][i] = m_TabBeamData.nBotGroupNum[iIMJ][i];
		m_pBeamData->dBotGroupGap[iIMJ][i] = m_TabBeamData.dBotGroupGap[iIMJ][i];
	}  
}

//----------------------------------------------------------------------------
// Initialize Layer CB
//----------------------------------------------------------------------------
void CDgnConBeamReinforceCommonDlgCIVIL::InitLayerCB()
{
	CString str;
	m_cTopLayerCB.ResetContent();
	m_cBotLayerCB.ResetContent();

	for(int i=0; i<T_RCHK_BEAM_LAYER+1; i++)
	{
		str.Format(_T("%d"), i);
		m_cTopLayerCB.AddString(str);
		m_cBotLayerCB.AddString(str);
	}

	
	m_cTopLayerCB.SetCurSel(1);
	m_cBotLayerCB.SetCurSel(1);
	UpdateData(FALSE);
}

//----------------------------------------------------------------------------
// Initialize Stirrup CB
//----------------------------------------------------------------------------
void CDgnConBeamReinforceCommonDlgCIVIL::InitStirrupCB()
{
	if(m_cStirrupSize.GetCount()>0) m_cStirrupSize.ResetContent();
	CDBLib::GetRebarNameAtComboBox(&m_cStirrupSize);
/*
	if(m_cStirrupNum.GetCount()>0) m_cStirrupNum.ResetContent();
	CStringArray aRebarNum;
	if(CDBLib::GetSubRebarNumList_Civil(aRebarNum))
	{
		for(int i=0; i<aRebarNum.GetSize(); i++) m_cStirrupNum.AddString(aRebarNum[i]);
	}
*/

	m_cmbBentUpBatSize.ResetContent();
	CDBLib::GetRebarNameAtComboBox(&m_cmbBentUpBatSize);

	m_cmbTorBar.ResetContent();
	CDBLib::GetRebarNameAtComboBox(&m_cmbTorBar);
	
	m_cmbLongiBar.ResetContent();
	CDBLib::GetRebarNameAtComboBox(&m_cmbLongiBar);
}

void CDgnConBeamReinforceCommonDlgCIVIL::UpdateViewWnd()
{
	m_SectWnd.Draw_Shape(0, m_RchkKey, FALSE, TRUE);
	m_RchkData.BEAM = m_TabBeamData;
	m_SectWnd.Draw_ChkRebar(m_RchkKey, m_nIMJ, m_RchkData, FALSE, TRUE);
	m_SectWnd.Invalidate();
}


void CDgnConBeamReinforceCommonDlgCIVIL::UpdateAsInfo(double dAst, double Asb)
{
	m_AsTop.SetEditUnit(dAst);
	m_AsBot.SetEditUnit(Asb);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// Change Layer CB 관련 
// 

void CDgnConBeamReinforceCommonDlgCIVIL::OnSelchangeDgnRcBotLayerCb() 
{
	CString strSel = _T("");
	int nIndex, nLayer;

	nIndex = m_cBotLayerCB.GetCurSel();
	if(nIndex>=0) m_cBotLayerCB.GetLBText(nIndex, strSel);
	nLayer = _ttoi(strSel);

	if(nLayer<0 || nLayer>T_RCHK_BEAM_LAYER) return;

	m_nBotLayer = nLayer;
	SetLayerNumToGrid(1, m_nBotLayer);
}

void CDgnConBeamReinforceCommonDlgCIVIL::OnSelchangeDgnRcTopLayerCb() 
{
	CString strSel = _T("");
	int nIndex, nLayer;

	nIndex = m_cTopLayerCB.GetCurSel();
	m_cTopLayerCB.GetLBText(nIndex, strSel);
	nLayer = _ttoi(strSel);

	if(nLayer<0 || nLayer>T_RCHK_BEAM_LAYER) return;

	m_nTopLayer = nLayer;
	SetLayerNumToGrid(0, m_nTopLayer);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// Tab Click할때 

void CDgnConBeamReinforceCommonDlgCIVIL::SetData_TabSelChange()
{
	UpdateData(TRUE);
	m_cStirrupSpace.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_cStirrupSpaceUT.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_AsTop.SetUnitType(D_UNITSYS_BASE_AREA);
	m_AsTopUT.SetUnitType(D_UNITSYS_BASE_AREA);
	m_AsBot.SetUnitType(D_UNITSYS_BASE_AREA);
	m_AsBotUT.SetUnitType(D_UNITSYS_BASE_AREA);
	m_cStirrupNum.SetUnitType(D_UNITSYS_NONE);

	m_edtTorSpacing.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untTorSpacing.SetUnitType(D_UNITSYS_BASE_LENGTH);
	
	m_edtBundledBarNum.SetUnitType(D_UNITSYS_NONE);
	m_edtLongiBarNum.SetUnitType(D_UNITSYS_NONE);

	m_edtStirrupAngle.SetUnitType(D_UNITSYS_BASE_DEGREE);
	m_untStirrupAngle.SetUnitType(D_UNITSYS_BASE_DEGREE);
	m_edtBentUpBarNum.SetUnitType(D_UNITSYS_NONE);
	m_edtBentUpBarAngle.SetUnitType(D_UNITSYS_BASE_DEGREE);
	m_untBentUpBarAngle.SetUnitType(D_UNITSYS_BASE_DEGREE);

	// Stirrup Bar
//	m_cStirrupNum.SelectString(-1, m_TabBeamData.dSubBarNum[m_nIMJ]);
	m_cStirrupNum.SetEditUnit(m_TabBeamData.dSubBarNum[m_nIMJ]);
	m_cStirrupSpace.SetEditUnit(m_TabBeamData.dSubBarDist[m_nIMJ]);
	m_cStirrupSize.SetCurSel(m_nStiCBox[m_nIMJ]);
	m_edtStirrupAngle.SetEditUnit(m_TabBeamData.dSubBarAngle[m_nIMJ]);

	m_bTorsionalBar = m_TabBeamData.bTorsionalBar[m_nIMJ];
	m_cmbTorBar.SetCurSel(m_nTorBar[m_nIMJ]);
	m_cmbLongiBar.SetCurSel(m_nTorLongiBar[m_nIMJ]);

	m_edtTorSpacing.SetEditUnit(m_TabBeamData.dTorBarSpacing[m_nIMJ]);
	m_bBundledBar = m_TabBeamData.bBundledBar[m_nIMJ];
	m_edtBundledBarNum.SetEditUnit(m_TabBeamData.dBundledBarNum[m_nIMJ]);
	m_edtLongiBarNum.SetEditUnit(m_TabBeamData.dLongiBarNum[m_nIMJ]);

	m_bBentUpBar = m_TabBeamData.bBentUpBar[m_nIMJ];
	m_cmbBentUpBatSize.SetCurSel(m_nBentUpBar[m_nIMJ]);
	m_edtBentUpBarNum.SetEditUnit(m_TabBeamData.dBentUpBarNum[m_nIMJ]);
	m_edtBentUpBarAngle.SetEditUnit(m_TabBeamData.dBentUpBarAngle[m_nIMJ]);

//  m_cStirrupSize.SelectString(-1, m_TabBeamData.strSubBarNa[m_nIMJ]);
	int nIndex;

	// Rebar Layer
	/*
	CString str;
	m_nTopLayer = 0;
	m_nBotLayer = 0;
	for(int i=0; i<T_RCHK_BEAM_LAYER; i++){
		if(m_TabBeamData.iTopBarNum[m_nIMJ][i]>0) m_nTopLayer++;
		if(m_TabBeamData.iBotBarNum[m_nIMJ][i]>0) m_nBotLayer++;
	}
	*/
	CString str;
	str.Format(_T("%d"), m_nTPosLayer[m_nIMJ]);
	nIndex = m_cTopLayerCB.SelectString(-1, str);    
	ASSERT(nIndex != LB_ERR);

	str.Format(_T("%d"), m_nBPosLayer[m_nIMJ]);  
	nIndex = m_cBotLayerCB.SelectString(-1, str);    
	ASSERT(nIndex != LB_ERR);


	// Initialize는 그리드 내부에서..
	SetDataToGrid_TabCtrl();
	SetLayerNumToGrid(0, m_nTPosLayer[m_nIMJ]);
	SetLayerNumToGrid(1, m_nBPosLayer[m_nIMJ]);

	m_nTopLayer = m_nTPosLayer[m_nIMJ];
	m_nBotLayer = m_nBPosLayer[m_nIMJ];
	
	double dAsTop=0.0, dAsBot=0.0;
	CalcAs(m_TabBeamData, dAsTop, dAsBot);	
	UpdateAsInfo(dAsTop, dAsBot);
}

void CDgnConBeamReinforceCommonDlgCIVIL::CalcAs(T_RCHK_BEAM& RchkBeamD, double& dAsTop, double& dAsBot)
{
	T_UNIT_INDEX CurIndex;
	CDBDoc::GetDocPoint()->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	CDBDoc::GetDocPoint()->m_pMatlDB->Create_RebarData(CurIndex.nBase_Length, CurIndex.nBase_Force, TRUE);
	
	for(int i=0; i<T_RCHK_BEAM_LAYER; i++)
	{
		// Top.
		if(RchkBeamD.iTopBarNum[m_nIMJ][i] > 0)
		{
			double dAs1=0.0, dAs2=0.0;
			if(RchkBeamD.strTopBarNa1[m_nIMJ][i]!=_T(""))  dAs1 = CDBDoc::GetDocPoint()->m_pMatlDB->Get_RebarArea(RchkBeamD.strTopBarNa1[m_nIMJ][i]);
			if(RchkBeamD.strTopBarNa2[m_nIMJ][i]!=_T(""))  dAs2 = CDBDoc::GetDocPoint()->m_pMatlDB->Get_RebarArea(RchkBeamD.strTopBarNa2[m_nIMJ][i]);
			if(dAs2==0.0) dAs2 = dAs1;
			// Change by ZINU.('06.02.27). ex) 5(3EA + 2EA) -> 5(2.5EA + 2.5EA).
			double dBarNum1 = RchkBeamD.iTopBarNum[m_nIMJ][i]/2.;
			double dBarNum2 = dBarNum1;
			
			int nGapNum = 1;
			if(RchkBeamD.bTopGroup[m_nIMJ][i])
			{
				nGapNum = RchkBeamD.nTopGroupNum[m_nIMJ][i];
			}
			dAsTop += (dAs1*dBarNum1 + dAs2*dBarNum2) * nGapNum;
		}
		// Bottom.
		if(RchkBeamD.iBotBarNum[m_nIMJ][i] > 0)
		{
			double dAs1=0.0, dAs2=0.0;
			if(RchkBeamD.strBotBarNa1[m_nIMJ][i]!=_T(""))  dAs1 = CDBDoc::GetDocPoint()->m_pMatlDB->Get_RebarArea(RchkBeamD.strBotBarNa1[m_nIMJ][i]);
			if(RchkBeamD.strBotBarNa2[m_nIMJ][i]!=_T(""))  dAs2 = CDBDoc::GetDocPoint()->m_pMatlDB->Get_RebarArea(RchkBeamD.strBotBarNa2[m_nIMJ][i]);
			if(dAs2==0.0) dAs2 = dAs1;
			// Change by ZINU.('06.02.27). ex) 5(3EA + 2EA) -> 5(2.5EA + 2.5EA).
			double dBarNum1 = RchkBeamD.iBotBarNum[m_nIMJ][i]/2.;
			double dBarNum2 = dBarNum1;
			
			int nGapNum = 1;
			if(RchkBeamD.bBotGroup[m_nIMJ][i])
			{
				nGapNum = RchkBeamD.nBotGroupNum[m_nIMJ][i];
			}
			dAsBot += (dAs1*dBarNum1 + dAs2*dBarNum2) * nGapNum;
		}
	}
}

void CDgnConBeamReinforceCommonDlgCIVIL::SetDlg2Data_Rebar_TabCtrl(int iIMJ)
{
	// Stirrup Bar
	m_TabBeamData.dSubBarDist[iIMJ] = m_cStirrupSpace.GetEditValue();
	m_TabBeamData.dSubBarNum[iIMJ] = m_cStirrupNum.GetEditValue();
	m_nStiCBox[iIMJ] = m_cStirrupSize.GetCurSel();
	m_TabBeamData.dSubBarAngle[iIMJ] = m_edtStirrupAngle.GetEditValue();
		
	int nIndex = m_cStirrupSize.GetCurSel();
	if(nIndex >= 0) m_cStirrupSize.GetLBText(nIndex, m_TabBeamData.strSubBarNa[iIMJ]);

	m_TabBeamData.bTorsionalBar[iIMJ] = m_bTorsionalBar;
	m_nTorBar[iIMJ] = m_cmbTorBar.GetCurSel();
	nIndex = m_cmbTorBar.GetCurSel();
	if(nIndex >= 0) m_cmbTorBar.GetLBText(nIndex, m_TabBeamData.strTorBarNa[iIMJ]);
	m_TabBeamData.dTorBarSpacing[iIMJ] = m_edtTorSpacing.GetEditValue();
	m_TabBeamData.bBundledBar[iIMJ] = m_bBundledBar;
	m_TabBeamData.dBundledBarNum[iIMJ] = m_edtBundledBarNum.GetEditValue();

	m_nTorLongiBar[iIMJ] = m_cmbLongiBar.GetCurSel();
	nIndex = m_cmbLongiBar.GetCurSel();
	if(nIndex >= 0) m_cmbLongiBar.GetLBText(nIndex, m_TabBeamData.strLongiBarNa[iIMJ]);
	m_TabBeamData.dLongiBarNum[iIMJ] = m_edtLongiBarNum.GetEditValue();

	m_TabBeamData.bBentUpBar[iIMJ] = m_bBentUpBar;
	m_nBentUpBar[iIMJ] = m_cmbBentUpBatSize.GetCurSel();
	nIndex = m_cmbBentUpBatSize.GetCurSel();
	if(nIndex >= 0) m_cmbBentUpBatSize.GetLBText(nIndex, m_TabBeamData.strBentUpBar[iIMJ]);
	m_TabBeamData.dBentUpBarNum[iIMJ] = m_edtBentUpBarNum.GetEditValue();
	m_TabBeamData.dBentUpBarAngle[iIMJ] = m_edtBentUpBarAngle.GetEditValue();
	
	double dAsTop=0.0, dAsBot=0.0;
	CalcAs(m_TabBeamData, dAsTop, dAsBot);	
	UpdateAsInfo(dAsTop, dAsBot);
	UpdateData(FALSE);

	// Rebar
	GetDataFromGrid_TabCtrl(iIMJ);
}

void CDgnConBeamReinforceCommonDlgCIVIL::OnRedrawBtn() 
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

	GetDataFromGrid_TabCtrl(m_nIMJ);
	SetDlg2Data_Rebar_TabCtrl(m_nIMJ);
	if(!CheckReDraw_Rebar()) return;  
	//SetDlg2Data_Rebar();
	
	UpdateViewWnd();
}

void CDgnConBeamReinforceCommonDlgCIVIL::ControlsEnableDisable()
{
	UpdateData(TRUE);

	CDgnDataCtrl DataCtrl;
	T_DCON_D DconD; DconD.Initialize();
	DataCtrl.Get_DgnConDcon(DconD);
	
	BOOL bEnable = FALSE;
	
	if( DconD.DesignCode== CONCODE_SNIP_2_05_03_84 || DconD.DesignCode== CONCODE_SP_35_13330_2011 ||
		DconD.DesignCode== CONCODE_SNIP_2_05_03_84_MKS || DconD.DesignCode== CONCODE_SP_35_13330_2011_MKS ||
		DconD.DesignCode == CONCODE_AS5100_5_17) bEnable = TRUE;

	GetDlgItem(IDC_DGN_RC_BENTUP_CHK           )->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_RC_BENTUP_SIZE_STC      )->EnableWindow(bEnable && m_bBentUpBar);
	GetDlgItem(IDC_DGN_RC_BENTUP_SIZE          )->EnableWindow(bEnable && m_bBentUpBar);
	GetDlgItem(IDC_DGN_RC_BENTUP_NUMBER_STC    )->EnableWindow(bEnable && m_bBentUpBar);
	GetDlgItem(IDC_DGN_RC_BENTUP_NUMBER        )->EnableWindow(bEnable && m_bBentUpBar);
	GetDlgItem(IDC_DGN_RC_BENTUP_ANGLE_STC     )->EnableWindow(bEnable && m_bBentUpBar);
	GetDlgItem(IDC_DGN_RC_BENTUP_ANGLE         )->EnableWindow(bEnable && m_bBentUpBar);
	GetDlgItem(IDC_DGN_RC_BENTUP_ANGLE_UNIT    )->EnableWindow(bEnable && m_bBentUpBar);
	GetDlgItem(IDC_RCHK_TOR_CHK                )->EnableWindow(bEnable);

	SetWindowTheme(GetDlgItem(IDC_RCHK_TOR_STIRRUP_GRP)->GetSafeHwnd(), _T(""), _T(""));	
	GetDlgItem(IDC_RCHK_TOR_STIRRUP_GRP        )->EnableWindow(bEnable && m_bTorsionalBar);
	GetDlgItem(IDC_RCHK_TOR_STIRRUP_SIZE_STC   )->EnableWindow(bEnable && m_bTorsionalBar);
	GetDlgItem(IDC_RCHK_TOR_STIRRUP_SIZE_CMB   )->EnableWindow(bEnable && m_bTorsionalBar);
	GetDlgItem(IDC_RCHK_TOR_STIRRUP_SPACING_STC)->EnableWindow(bEnable && m_bTorsionalBar);
	GetDlgItem(IDC_RCHK_TOR_STIRRUP_SPACING_EDT)->EnableWindow(bEnable && m_bTorsionalBar);
	GetDlgItem(IDC_RCHK_TOR_STIRRUP_SPACING_UNT)->EnableWindow(bEnable && m_bTorsionalBar);
	GetDlgItem(IDC_RCHK_TOR_BUNDLED_CHK        )->EnableWindow(bEnable && m_bTorsionalBar);
	GetDlgItem(IDC_RCHK_TOR_BUNDLED_NUM_STC    )->EnableWindow(bEnable && m_bTorsionalBar && m_bBundledBar);
	GetDlgItem(IDC_RCHK_TOR_BUNDLED_NUM_EDT    )->EnableWindow(bEnable && m_bTorsionalBar && m_bBundledBar);

	SetWindowTheme(GetDlgItem(IDC_RCHK_TOR_LONGI_GRP)->GetSafeHwnd(), _T(""), _T(""));	
	GetDlgItem(IDC_RCHK_TOR_LONGI_GRP          )->EnableWindow(bEnable && m_bTorsionalBar);
	GetDlgItem(IDC_RCHK_TOR_LONGI_SIZE_STC     )->EnableWindow(bEnable && m_bTorsionalBar);
	GetDlgItem(IDC_RCHK_TOR_LONGI_SIZE_CMB     )->EnableWindow(bEnable && m_bTorsionalBar);
	GetDlgItem(IDC_RCHK_TOR_LONGI_NUM_STC      )->EnableWindow(bEnable && m_bTorsionalBar);
	GetDlgItem(IDC_RCHK_TOR_LONGI_NUM_EDT      )->EnableWindow(bEnable && m_bTorsionalBar);


	if (DconD.DesignCode == _T("AREMA-2023")) {

		//SetWindowTheme(GetDlgItem(IDC_RDGN_TOR_LONGI_GRP)->GetSafeHwnd(), _T(""), _T(""));
		//IDC_RDGN_TOR_GRP
		GetDlgItem(IDC_RCHK_TOR_GRP)->ShowWindow(bEnable);
		GetDlgItem(IDC_RCHK_TOR_STIRRUP_GRP)->ShowWindow(bEnable);
		GetDlgItem(IDC_RCHK_TOR_STIRRUP_SIZE_STC)->ShowWindow(bEnable);
		GetDlgItem(IDC_RCHK_TOR_STIRRUP_SIZE_CMB)->ShowWindow(bEnable);
		GetDlgItem(IDC_RCHK_TOR_BUNDLED_CHK)->ShowWindow(bEnable);
		GetDlgItem(IDC_RCHK_TOR_BUNDLED_NUM_STC)->ShowWindow(bEnable);
		GetDlgItem(IDC_RCHK_TOR_BUNDLED_NUM_EDT)->ShowWindow(bEnable);
		GetDlgItem(IDC_RCHK_TOR_CHK)->ShowWindow(bEnable);
		GetDlgItem(IDC_RCHK_TOR_LONGI_GRP)->ShowWindow(bEnable);
		GetDlgItem(IDC_RCHK_TOR_LONGI_SIZE_STC)->ShowWindow(bEnable);
		GetDlgItem(IDC_RCHK_TOR_LONGI_SIZE_CMB)->ShowWindow(bEnable);
		GetDlgItem(IDC_RCHK_TOR_LONGI_NUM_STC)->ShowWindow(bEnable);
		GetDlgItem(IDC_RCHK_TOR_LONGI_NUM_EDT)->ShowWindow(bEnable);
		GetDlgItem(IDC_RCHK_TOR_STIRRUP_SPACING_EDT)->ShowWindow(bEnable);
		GetDlgItem(IDC_RCHK_TOR_STIRRUP_SPACING_STC)->ShowWindow(bEnable);
		GetDlgItem(IDC_RCHK_TOR_STIRRUP_SPACING_UNT)->ShowWindow(bEnable);
		//IDC_DGN_RC_BSTIRRUP_ANGLE
		GetDlgItem(IDC_DGN_RC_BSTIRRUP_ANGLE)->EnableWindow(bEnable);
	}
}
