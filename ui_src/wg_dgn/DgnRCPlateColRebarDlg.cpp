// DgnRCPlateColRebarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnRCPlateColRebarDlg.h"

#include "DgnDataCtrl.h"
#include "DgnBeamGridWndCIVIL.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "DgnRCPlateColRebarMBarDlg.h"
#include "DgnRCPlateColRebarSBarDlg.h"
#include "..\wg_db\DataCtrl.h"
#include "..\wg_db\AttrCtrl2.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TOTALLAYER 3

/////////////////////////////////////////////////////////////////////////////
// CDgnRCPlateColRebarDlg dialog


CDgnRCPlateColRebarDlg::CDgnRCPlateColRebarDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnRCPlateColRebarDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_Data.Initialize();
	m_KeyForModify = NULL;
}

CDgnRCPlateColRebarDlg::~CDgnRCPlateColRebarDlg()
{
}

void CDgnRCPlateColRebarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnRCPlateColRebarDlg)  	
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

BEGIN_MESSAGE_MAP(CDgnRCPlateColRebarDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnRCPlateColRebarDlg)  
	ON_BN_CLICKED(IDC_DGN_Plate_STIRRUP_CHK,       OnClickedShearChk)
	ON_BN_CLICKED(IDC_DGN_CLOSE_BTN,               OnBtnClose)  
	ON_NOTIFY(NM_CLICK, IDC_DGN_RBAR_LST, OnClkListItem)
	ON_BN_CLICKED(IDC_DGN_ADD_BTN,       OnClickedAdd)
	ON_BN_CLICKED(IDC_DGN_MOD_BTN,       OnClickedMod)
	ON_BN_CLICKED(IDC_DGN_DEL_BTN,       OnClickedDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnRCPlateColRebarDlg message handlers

BOOL CDgnRCPlateColRebarDlg::OnInitDialog() 
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

void CDgnRCPlateColRebarDlg::SetData(T_RIPC_K RipcK)
{
	m_KeyForModify = RipcK; // Tree에서 들어올 경우 선택된채로 띄우기 위한 처리
}

void CDgnRCPlateColRebarDlg::InitLst()
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

void CDgnRCPlateColRebarDlg::RefreshList(BOOL bKeepSelection)
{  
	// ListCtrl에 내용 채우기
	T_RIPC_D RipcD;

	CArray<T_RIPC_K, T_RIPC_K> aRipcK;
	m_pDoc->m_pAttrCtrl2->GetRipcKeyList(aRipcK);

	CArray<DWORD, DWORD> aSelKey;
	CDlgUtil::GetSelectedListItemData(&m_LstRebar, aSelKey);  
	T_RIPC_K RipcK = NULL;
	if(aSelKey.GetSize() > 0)
	{
		RipcK = aSelKey[0];
	}

	m_LstRebar.DeleteAllItems();
	CString szTemp;
	CStringArray aContents;
	for(int i = 0; i < (int)aRipcK.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl2->GetRipc(aRipcK[i], RipcD)) { ASSERT(0); RipcD.Initialize(); }

		aContents.RemoveAll();
		// No
		szTemp.Format(_T("%d"), aRipcK[i]);
		aContents.Add(szTemp);

		// Name
		aContents.Add(RipcD.strName);

		CDlgUtil::SetListItem(&m_LstRebar, i, aContents,(DWORD)(aRipcK[i]));
	}	

	if(RipcK && bKeepSelection)
	{
		CDlgUtil::SelectListItemData(&m_LstRebar, RipcK); // 선택 살림
	}
}

void CDgnRCPlateColRebarDlg::SetDataByCurListItem()
{
	CArray<DWORD, DWORD> aSelKey;
	CDlgUtil::GetSelectedListItemData(&m_LstRebar, aSelKey);
	if(aSelKey.GetSize() == 0) return ;

	T_RIPC_K RipcK = aSelKey[0];
	T_RIPC_D RipcD;
	if(!m_pDoc->m_pAttrCtrl2->GetRipc(RipcK, RipcD)) { ASSERT(0); RipcD.Initialize(); }

	m_Data = RipcD;
}

void CDgnRCPlateColRebarDlg::InitCombo()
{
	CString str;
	
	m_cmbSSize.ResetContent();
	if(m_cmbSSize.GetCount()>0) m_cmbSSize.ResetContent();
	CDBLib::GetRebarNameAtComboBox(&m_cmbSSize);
	m_cmbSSize.SetCurSel(0);
}

void CDgnRCPlateColRebarDlg::InitUnit()
{
	m_edtSSpace.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untSSpace.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtSNum.SetEditUnit(D_UNITSYS_NONE); 
}

void CDgnRCPlateColRebarDlg::Data2Dlg()
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

BOOL CDgnRCPlateColRebarDlg::Dlg2Data()
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

BOOL CDgnRCPlateColRebarDlg::ChildDlg2Data()
{
	int nSel;
	CChildDialog* pDlg;

	nSel	= m_wndTab.GetCurSel();
	pDlg	= (CChildDialog*) m_wndTab.GetTab(nSel); ASSERT(pDlg);

	switch(nSel)
	{
	case 0:
		return	((CDgnRCPlateColRebarMBarDlg  *)	pDlg)->Dlg2Data();
	case 1:
		return	((CDgnRCPlateColRebarSBarDlg  *)	pDlg)->Dlg2Data();  
	}
	return TRUE;
}

void CDgnRCPlateColRebarDlg::ChildData2Dlg()
{
	int nSel;
	CChildDialog* pDlg;

	nSel	= m_wndTab.GetCurSel();
	pDlg	= (CChildDialog*) m_wndTab.GetTab(nSel); ASSERT(pDlg);

	switch(nSel)
	{
	case 0:	((CDgnRCPlateColRebarMBarDlg  *)	pDlg)->Data2Dlg(); break;	
	case 1:	((CDgnRCPlateColRebarSBarDlg  *)	pDlg)->Data2Dlg(); break;	
	}
}

void CDgnRCPlateColRebarDlg::ControlsEnableDisable()
{ 
	CArray<UINT, UINT> aControls;
	CDlgUtil::GetCtrlIDByIncRect(this, aControls, IDC_DGN_Plate_STIRRUP_GRP, FALSE);  
	CDlgUtil::CtrlEnableDisable(this, aControls, m_bSRebar);
	GetDlgItem(IDC_DGN_Plate_STIRRUP_CHK)->EnableWindow(TRUE);
}

void CDgnRCPlateColRebarDlg::PostNcDestroy() 
{	

	CDialogMove::PostNcDestroy();
}

void CDgnRCPlateColRebarDlg::LayoutChildDlgs()
{
	CWnd* pPlaceHolder = GetDlgItem(IDC_DGN_PLACEHOLDER);

	m_wndTab.SetPlaceHolder(pPlaceHolder);
	m_wndTab.DeleteAllItems();

	CChildDialog* pBaseDlg=NULL;

	pBaseDlg = new CDgnRCPlateColRebarMBarDlg(&m_Data.MainRebar, this);  
	m_wndTab.AddTab(pBaseDlg, _LS(IDS_DGN_RC_PLATE_Main_REBAR), CDgnRCPlateColRebarMBarDlg::IDD, TRUE);

	pBaseDlg = new CDgnRCPlateColRebarSBarDlg(&m_Data.SubRebar, this);  
	m_wndTab.AddTab(pBaseDlg, _LS(IDS_DGN_RC_PLATE_DISTRIBUTION_BAR), CDgnRCPlateColRebarSBarDlg::IDD, TRUE);

	m_wndTab.ShowTab(0);
}


void CDgnRCPlateColRebarDlg::OnClickedShearChk()
{
	UpdateData(TRUE);
	ControlsEnableDisable();
}

void CDgnRCPlateColRebarDlg::OnBtnClose()
{
	CDialogMove::OnOK();
}

void CDgnRCPlateColRebarDlg::OnClickedAdd()
{
	if(!Dlg2Data()) return;
	m_pDoc->m_pDataCtrl->AddRipc(m_Data);
	RefreshList(FALSE);
}
void CDgnRCPlateColRebarDlg::OnClickedMod()
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
	T_RIPC_K RipcK = aSelKey[0];

	m_pDoc->m_pDataCtrl->ModifyRipc(RipcK, RipcK, m_Data);

	RefreshList(TRUE);
}
void CDgnRCPlateColRebarDlg::OnClickedDel()
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
	T_RIPC_K RipcK = aSelKey[0];

	m_pDoc->m_pDataCtrl->DelRipc(RipcK);
	RefreshList(FALSE);
}

void CDgnRCPlateColRebarDlg::OnClkListItem( NMHDR* pNMHDR, LRESULT* pResult )
{
	SetDataByCurListItem();
	Data2Dlg();
}
	
	
