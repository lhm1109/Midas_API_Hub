// DgnRCPlateBeamRebarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnRCPlateBeamRebarDlg.h"

#include "DgnDataCtrl.h"
#include "DgnBeamGridWndCIVIL.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "DgnRCPlateBeamRebarMBarDlg.h"
#include "DgnRCPlateBeamRebarSBarDlg.h"
#include "..\wg_db\DataCtrl.h"
#include "..\wg_db\AttrCtrl2.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TOTALLAYER 3

/////////////////////////////////////////////////////////////////////////////
// CDgnRCPlateBeamRebarDlg dialog


CDgnRCPlateBeamRebarDlg::CDgnRCPlateBeamRebarDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnRCPlateBeamRebarDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_Data.Initialize();
	m_KeyForModify = NULL;
}

CDgnRCPlateBeamRebarDlg::~CDgnRCPlateBeamRebarDlg()
{
}

void CDgnRCPlateBeamRebarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnRCPlateBeamRebarDlg)  	
	DDX_Check  (pDX, IDC_DGN_Plate_STIRRUP_CHK,          m_bSRebar);
	DDX_Control(pDX, IDC_DGN_Plate_STIRRUP_SIZE_CMB,     m_cmbSSize);
	DDX_Control(pDX, IDC_DGN_Plate_STIRRUP_SPACE_EDT,    m_edtSSpace);
	DDX_Control(pDX, IDC_DGN_Plate_STIRRUP_SPACE_UNIT,   m_untSSpace); 
	DDX_Control(pDX, IDC_DGN_Plate_STIRRUP_NUMBER_EDT,   m_edtSNum);
	DDX_Control(pDX, IDC_DGN_NAME_EDT,                   m_edtName);  
	DDX_Control(pDX, IDC_DGN_TAB_CONTROL, m_wndTab);
	DDX_Control(pDX, IDC_DGN_RBAR_LST, m_LstRebar);	
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnRCPlateBeamRebarDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnRCPlateBeamRebarDlg)  
	ON_BN_CLICKED(IDC_DGN_Plate_STIRRUP_CHK,       OnClickedShearChk)
	ON_BN_CLICKED(IDC_DGN_CLOSE_BTN,               OnBtnClose)  
	ON_NOTIFY(NM_CLICK, IDC_DGN_RBAR_LST, OnClkListItem)
	ON_BN_CLICKED(IDC_DGN_ADD_BTN,       OnClickedAdd)
	ON_BN_CLICKED(IDC_DGN_MOD_BTN,       OnClickedMod)
	ON_BN_CLICKED(IDC_DGN_DEL_BTN,       OnClickedDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnRCPlateBeamRebarDlg message handlers

BOOL CDgnRCPlateBeamRebarDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	LayoutChildDlgs();
	InitLst();
	InitCombo();
	InitUnit();

	RefreshList(FALSE);  
	if(m_KeyForModify)
	{
		CDlgUtil::SelectListItemData(&m_LstRebar, m_KeyForModify);
		SetDataByCurListItem();
		Data2Dlg();
	}
	else
	{
		Data2Dlg();
	}

	ControlsEnableDisable();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnRCPlateBeamRebarDlg::SetData(T_RIPB_K RipbK)
{
	m_KeyForModify = RipbK; // Tree에서 들어올 경우 선택된채로 띄우기 위한 처리
}

void CDgnRCPlateBeamRebarDlg::InitLst()
{
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_LstRebar.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_LstRebar.GetSafeHwnd(), dwStyle);
	CString aTitleRdot[2] = {_LS(IDS_DGN_RPSC_ID), _LS(IDS_DGN_RPSC_NAME)};
	int		  aColWidthRdot[2] = {40, 200};
	LV_COLUMN lvcolumn;
	CString title;
	for(int i = 0; i < 2; i++)
	{
		title = aTitleRdot[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(aColWidthRdot[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_LstRebar.InsertColumn(i,&lvcolumn);
		m_LstRebar.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

void CDgnRCPlateBeamRebarDlg::RefreshList(BOOL bKeepSelection)
{  
	// ListCtrl에 내용 채우기
	T_RIPB_D RipbD;

	CArray<T_RIPB_K, T_RIPB_K> aRipbK;
	m_pDoc->m_pAttrCtrl2->GetRipbKeyList(aRipbK);

	CArray<DWORD, DWORD> aSelKey;
	CDlgUtil::GetSelectedListItemData(&m_LstRebar, aSelKey);  
	T_RIPB_K RipbK = NULL;
	if(aSelKey.GetSize() > 0)
	{
		RipbK = aSelKey[0];
	}

	m_LstRebar.DeleteAllItems();
	CString szTemp;
	CStringArray aContents;
	for(int i = 0; i < (int)aRipbK.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl2->GetRipb(aRipbK[i], RipbD)) { ASSERT(0); RipbD.Initialize(); }

		aContents.RemoveAll();
		// No
		szTemp.Format(_T("%d"), aRipbK[i]);
		aContents.Add(szTemp);

		// Name
		aContents.Add(RipbD.strName);

		CDlgUtil::SetListItem(&m_LstRebar, i, aContents,(DWORD)(aRipbK[i]));
	}	

	if(RipbK && bKeepSelection)
	{
		CDlgUtil::SelectListItemData(&m_LstRebar, RipbK); // 선택 살림
	}
}

void CDgnRCPlateBeamRebarDlg::SetDataByCurListItem()
{
	CArray<DWORD, DWORD> aSelKey;
	CDlgUtil::GetSelectedListItemData(&m_LstRebar, aSelKey);
	if(aSelKey.GetSize() == 0) return ;

	T_RIPB_K RipbK = aSelKey[0];
	T_RIPB_D RipbD;
	if(!m_pDoc->m_pAttrCtrl2->GetRipb(RipbK, RipbD)) { ASSERT(0); RipbD.Initialize(); }

	m_Data = RipbD;
}

void CDgnRCPlateBeamRebarDlg::InitCombo()
{
	CString str;
	
	m_cmbSSize.ResetContent();
	if(m_cmbSSize.GetCount()>0) m_cmbSSize.ResetContent();
	CDBLib::GetRebarNameAtComboBox(&m_cmbSSize);
	m_cmbSSize.SetCurSel(0);
}

void CDgnRCPlateBeamRebarDlg::InitUnit()
{
	m_edtSSpace.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untSSpace.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtSNum.SetEditUnit(D_UNITSYS_NONE); 
}

void CDgnRCPlateBeamRebarDlg::Data2Dlg()
{  
	ChildData2Dlg();
	m_edtName.SetWindowText(m_Data.strName);
	m_bSRebar = m_Data.bChkSubBar;
	m_cmbSSize.SelectString(-1, m_Data.strSubBarNa);
	m_edtSSpace.SetEditUnit(m_Data.dSubBarDist);
	m_edtSNum.SetEditUnit(m_Data.nSubBarNum);

	UpdateData(FALSE);	

	m_wndTab.ShowTab(0);
	ControlsEnableDisable();
}

BOOL CDgnRCPlateBeamRebarDlg::Dlg2Data()
{ 
	UpdateData(TRUE);

	if(!ChildDlg2Data())  return FALSE;

	m_edtName.GetWindowText(m_Data.strName);
	m_Data.bChkSubBar = m_bSRebar;
	if(m_cmbSSize.GetCurSel() >= 0)
	{
		m_cmbSSize.GetLBText(m_cmbSSize.GetCurSel(), m_Data.strSubBarNa); 
	}	
	m_Data.dSubBarDist = m_edtSSpace.GetEditValue();
	m_Data.nSubBarNum  = m_edtSNum.GetEditValue();  
	/*
	if(!CheckRebarData(m_Data)) return FALSE;
	*/
	return TRUE;
}

BOOL CDgnRCPlateBeamRebarDlg::ChildDlg2Data()
{
	int nSel;
	CChildDialog* pDlg;

	nSel	= m_wndTab.GetCurSel();
	pDlg	= (CChildDialog*) m_wndTab.GetTab(nSel); ASSERT(pDlg);

	switch(nSel)
	{
	case 0:
		return	((CDgnRCPlateBeamRebarMBarDlg  *)	pDlg)->Dlg2Data();
	case 1:
		return	((CDgnRCPlateBeamRebarSBarDlg  *)	pDlg)->Dlg2Data();  
	}
	return TRUE;
}

void CDgnRCPlateBeamRebarDlg::ChildData2Dlg()
{
	int nSel;
	CChildDialog* pDlg;

	nSel	= m_wndTab.GetCurSel();
	pDlg	= (CChildDialog*) m_wndTab.GetTab(nSel); ASSERT(pDlg);
	
	switch(nSel)
	{
	case 0:	((CDgnRCPlateBeamRebarMBarDlg  *)	pDlg)->Data2Dlg(); break;	
	case 1:	((CDgnRCPlateBeamRebarSBarDlg  *)	pDlg)->Data2Dlg(); break;	
	}
}

void CDgnRCPlateBeamRebarDlg::ControlsEnableDisable()
{ 
	CArray<UINT, UINT> aControls;
	CDlgUtil::GetCtrlIDByIncRect(this, aControls, IDC_DGN_Plate_STIRRUP_GRP, FALSE);  
	CDlgUtil::CtrlEnableDisable(this, aControls, m_bSRebar);
	GetDlgItem(IDC_DGN_Plate_STIRRUP_CHK)->EnableWindow(TRUE);
}

void CDgnRCPlateBeamRebarDlg::PostNcDestroy() 
{	

	CDialogMove::PostNcDestroy();
}

void CDgnRCPlateBeamRebarDlg::LayoutChildDlgs()
{
	CWnd* pPlaceHolder = GetDlgItem(IDC_DGN_PLACEHOLDER);

	m_wndTab.SetPlaceHolder(pPlaceHolder);
	m_wndTab.DeleteAllItems();

	CChildDialog* pBaseDlg=NULL;

	pBaseDlg = new CDgnRCPlateBeamRebarMBarDlg(&m_Data.MainRebar, this);  
	m_wndTab.AddTab(pBaseDlg, _LS(IDS_DGN_RC_PLATE_Main_REBAR), CDgnRCPlateBeamRebarMBarDlg::IDD, TRUE);

	pBaseDlg = new CDgnRCPlateBeamRebarSBarDlg(&m_Data.SubRebar, this);  
	m_wndTab.AddTab(pBaseDlg, _LS(IDS_DGN_RC_PLATE_DISTRIBUTION_BAR), CDgnRCPlateBeamRebarSBarDlg::IDD, TRUE);

	m_wndTab.ShowTab(0);
}


void CDgnRCPlateBeamRebarDlg::OnClickedShearChk()
{
	UpdateData(TRUE);
	ControlsEnableDisable();
}

void CDgnRCPlateBeamRebarDlg::OnBtnClose()
{
	CDialogMove::OnOK();
}

void CDgnRCPlateBeamRebarDlg::OnClickedAdd()
{
	if(!Dlg2Data()) return;
	m_pDoc->m_pDataCtrl->AddRipb(m_Data);
	RefreshList(FALSE);
}
void CDgnRCPlateBeamRebarDlg::OnClickedMod()
{
	if(!Dlg2Data()) return;

	CArray<DWORD, DWORD> aSelKey;
	CDlgUtil::GetSelectedListItemData(&m_LstRebar, aSelKey);  
	
	if(aSelKey.GetSize() == 0)
	{
		AfxMessageBox(_LS(IDS_DGN_RC_PLATE_NO_SEL_ITEM));
		return ;
	}
	if(aSelKey.GetSize() != 1)
	{
		AfxMessageBox(_LS(IDS_DGN_RC_PLATE_SEL_ITEM_ONE));
		return ;
	}
	T_RIPB_K RipbK = aSelKey[0];

	m_pDoc->m_pDataCtrl->ModifyRipb(RipbK, RipbK, m_Data);

	RefreshList(TRUE);
}
void CDgnRCPlateBeamRebarDlg::OnClickedDel()
{
	if(!Dlg2Data()) return;

	CArray<DWORD, DWORD> aSelKey;
	CDlgUtil::GetSelectedListItemData(&m_LstRebar, aSelKey);  

	if(aSelKey.GetSize() == 0)
	{
		AfxMessageBox(_LS(IDS_DGN_RC_PLATE_NO_SEL_ITEM));
		return ;
	}
	ASSERT(aSelKey.GetSize() == 1);
	T_RIPB_K RipbK = aSelKey[0];

	m_pDoc->m_pDataCtrl->DelRipb(RipbK);
	RefreshList(FALSE);
}

void CDgnRCPlateBeamRebarDlg::OnClkListItem( NMHDR* pNMHDR, LRESULT* pResult )
{
	SetDataByCurListItem();
	Data2Dlg();
}
	
	
