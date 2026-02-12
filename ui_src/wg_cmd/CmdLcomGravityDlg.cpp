// CmdLcomGravityDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdLcomGravityDlg.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum LIST_IDX { IDX_NAME=0, IDX_FACTOR };
/////////////////////////////////////////////////////////////////////////////
// CCmdLcomGravityDlg dialog

CCmdLcomGravityDlg::CCmdLcomGravityDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCmdLcomGravityDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCmdLcomGravityDlg)
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
	m_dEffLoadFactor=1.0;
}

CCmdLcomGravityDlg::~CCmdLcomGravityDlg()
{
}

void CCmdLcomGravityDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCmdLcomGravityDlg)
	DDX_Control(pDX, IDC_GRV_LOAD_SEL_LST   , m_lstGrvSel       );		
	DDX_Control(pDX, IDC_GRV_LOAD_UNSEL_LST , m_lstGrvUnsel     );		
	DDX_Control(pDX, IDC_EFF_LOAD_LST       , m_lstEffLoad      );		
	DDX_Control(pDX, IDC_EFF_LOAD_LC_CMB    , m_cmbEffLoadCase  );		
	DDX_Text   (pDX, IDC_EFF_LOAD_LF_EDT    , m_dEffLoadFactor);		
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCmdLcomGravityDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCmdLcomGravityDlg)
	ON_BN_CLICKED(IDC_GRV_LOAD_LEFT_BTN , OnGrvLeftBtn   )
	ON_BN_CLICKED(IDC_GRV_LOAD_RIGHT_BTN, OnGrvRightBtn  )
	ON_BN_CLICKED(IDC_EFF_LOAD_ADD_BTN  , OnEffLoadAddBtn)
	ON_BN_CLICKED(IDC_EFF_LOAD_MOD_BTN  , OnEffLoadModBtn)
	ON_BN_CLICKED(IDC_EFF_LOAD_DEL_BTN  , OnEffLoadDelBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCmdLcomGravityDlg message handlers

BOOL CCmdLcomGravityDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	InitCtrl();
	Data2Dlg();

	return TRUE;
}

void CCmdLcomGravityDlg::OnOK() 
{
	if(!Dlg2Data()) return;
	CDialogMove::OnOK();
}

void CCmdLcomGravityDlg::OnCancel() 
{
	CDialogMove::OnCancel();
}

void CCmdLcomGravityDlg::InitCtrl()
{
	CString aTitle_grv[] = { _LS(IDS_WG_CMD__ADDD__Load_Case_Name) };
	int nColWidth_grv[] = {115};
	SetListCtrlHeader(m_lstGrvSel, 1, aTitle_grv, nColWidth_grv);
	SetListCtrlHeader(m_lstGrvUnsel, 1, aTitle_grv, nColWidth_grv);

	CString aTitle_eff[] = { _LS(IDS_WG_CMD__ADDD__Load_Case), _LS(IDS_WG_CMD__ADDD__Factor) };
	int nColWidth_eff[] = {120, 80};
	SetListCtrlHeader(m_lstEffLoad, 2, aTitle_eff, nColWidth_eff);

	CArray<T_STLD_K, T_STLD_K> aStld_EffLoad;
	m_pDoc->m_pAttrCtrl->GetStldKeyListVariableParam(aStld_EffLoad, 3, _T("L"), _T("LR"), _T("S"));
	for(int i=0; i<aStld_EffLoad.GetSize(); i++)
	{
		T_STLD_D StldD;
		if(!m_pDoc->m_pAttrCtrl->GetStld(aStld_EffLoad[i], StldD)) { ASSERT(0); continue; }
		CDlgUtil::CobxAddItem(m_cmbEffLoadCase, StldD.LoadCaseName, aStld_EffLoad[i]);
	}
	m_cmbEffLoadCase.SetCurSel(0);
}

void CCmdLcomGravityDlg::SetListCtrlHeader(CListCtrl& list, const int nColNum, const CString* aTitle, const int* nColWidth)
{
	// set full row select mode
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(list.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(list.GetSafeHwnd(), dwStyle);

	// Set Title
	for(int i = 0; i<nColNum; i++)
	{
		CString title = aTitle[i];
		LV_COLUMN lvcolumn;
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = i==IDX_NAME ? LVCFMT_LEFT : LVCFMT_CENTER;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		list.InsertColumn(i,&lvcolumn);
		list.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCmdLcomGravityDlg::Data2Dlg()
{
	LV_ITEM lvitem;

	// Selected
	lvitem.iSubItem = IDX_NAME;
	for(int i=0; i<m_aSelGravityLoad.GetSize(); i++)
	{
		T_STLD_D StldD;
		if(!m_pDoc->m_pAttrCtrl->GetStld(m_aSelGravityLoad[i], StldD)) { ASSERT(0); continue; }
		lvitem.iItem  	= i;
		lvitem.mask	= LVIF_TEXT | LVFI_STRING;
		lvitem.pszText	= (LPTSTR)(LPCTSTR)StldD.LoadCaseName;
		m_lstGrvSel.InsertItem(&lvitem);
	}

	// Unselected
	for(int i=0; i<m_aUnselGravityLoad.GetSize(); i++)
	{
		T_STLD_D StldD;
		if(!m_pDoc->m_pAttrCtrl->GetStld(m_aUnselGravityLoad[i], StldD)) { ASSERT(0); continue; }
		lvitem.iItem  	= i;
		lvitem.pszText	= (LPTSTR)(LPCTSTR)StldD.LoadCaseName;
		m_lstGrvUnsel.InsertItem(&lvitem);
	}
	
	// Effective Load
	for(int i=0; i<m_aEffectiveLoad.GetSize(); i++)
	{
		T_STLD_D StldD;
		if(!m_pDoc->m_pAttrCtrl->GetStld(m_aEffectiveLoad[i].StldK, StldD)) { ASSERT(0); continue; }
		lvitem.iItem  	= i;
		lvitem.iSubItem = IDX_NAME;
		lvitem.mask=LVIF_TEXT | LVFI_STRING;
		lvitem.pszText	= (LPTSTR)(LPCTSTR)StldD.LoadCaseName;
		m_lstEffLoad.InsertItem(&lvitem);

		lvitem.iSubItem = IDX_FACTOR;
		lvitem.mask=LVIF_TEXT;
		CString strFactor;
		strFactor.Format(_T("%g"), m_aEffectiveLoad[i].dFactor);
		lvitem.pszText	= (LPTSTR)(LPCTSTR)strFactor;
		m_lstEffLoad.SetItem(&lvitem);
	}

	UpdateData(FALSE);
}

BOOL CCmdLcomGravityDlg::Dlg2Data()
{
	m_aSelGravityLoad.RemoveAll();
	for(int i=0; i<m_lstGrvSel.GetItemCount(); i++)
	{
		CString strStldName = m_lstGrvSel.GetItemText(i,IDX_NAME);
		T_STLD_K StldK = m_pDoc->m_pAttrCtrl->GetStldKey(strStldName);
		m_aSelGravityLoad.Add(StldK);
	}

	m_aUnselGravityLoad.RemoveAll();
	for(int i=0; i<m_lstGrvUnsel.GetItemCount(); i++)
	{
		CString strStldName = m_lstGrvUnsel.GetItemText(i,IDX_NAME);
		T_STLD_K StldK = m_pDoc->m_pAttrCtrl->GetStldKey(strStldName);
		m_aUnselGravityLoad.Add(StldK);
	}
	
	m_aEffectiveLoad.RemoveAll();
	for(int i=0; i<m_lstEffLoad.GetItemCount(); i++)
	{
		_LCOM_GRAV_EFF gEff;
		CString strStldName = m_lstEffLoad.GetItemText(i,IDX_NAME);
		gEff.StldK = m_pDoc->m_pAttrCtrl->GetStldKey(strStldName);
		gEff.dFactor = _ttof(m_lstEffLoad.GetItemText(i,IDX_FACTOR));
		m_aEffectiveLoad.Add(gEff);
	}

	return TRUE;
}

void CCmdLcomGravityDlg::SetData(CArray<T_STLD_K, T_STLD_K>& aSelGravityLoad, CArray<T_STLD_K, T_STLD_K>& aUnselGravityLoad, CArray<_LCOM_GRAV_EFF, _LCOM_GRAV_EFF&>& aEffectiveLoad)
{
	m_aSelGravityLoad.Copy(aSelGravityLoad);
	m_aUnselGravityLoad.Copy(aUnselGravityLoad);
	m_aEffectiveLoad.Copy(aEffectiveLoad);
}

void CCmdLcomGravityDlg::GetData(CArray<T_STLD_K, T_STLD_K>& aSelGravityLoad, CArray<T_STLD_K, T_STLD_K>& aUnselGravityLoad, CArray<_LCOM_GRAV_EFF, _LCOM_GRAV_EFF&>& aEffectiveLoad)
{
	aSelGravityLoad.Copy(m_aSelGravityLoad);
	aUnselGravityLoad.Copy(m_aUnselGravityLoad);
	aEffectiveLoad.Copy(m_aEffectiveLoad);
}

void CCmdLcomGravityDlg::OnEffLoadAddBtn()
{
	UpdateData();

	T_STLD_K StldK = CDlgUtil::CobxGetCurSelItemData(m_cmbEffLoadCase, m_cmbEffLoadCase.GetCurSel());
	if(StldK<1) 
		return;

	T_STLD_D StldD;
	m_pDoc->m_pAttrCtrl->GetStld(StldK,StldD);

	// 리스트에 있나?
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_STRING;
	FindInfo.psz = StldD.LoadCaseName;
	int nItem = m_lstEffLoad.FindItem(&FindInfo);
	if (nItem != -1)
		return;

	LV_ITEM lvitem;
	lvitem.mask = LVIF_TEXT | LVFI_STRING;
	lvitem.iItem  	= m_lstEffLoad.GetItemCount();
	lvitem.iSubItem = IDX_NAME;
	lvitem.pszText	= (LPTSTR)(LPCTSTR)StldD.LoadCaseName;
	m_lstEffLoad.InsertItem(&lvitem);

	lvitem.iSubItem = IDX_FACTOR;
	lvitem.mask	= LVIF_TEXT;
	CString strFactor;
	strFactor.Format(_T("%g"), m_dEffLoadFactor);
	lvitem.pszText	= (LPTSTR)(LPCTSTR)strFactor;
	m_lstEffLoad.SetItem(&lvitem);
}

void CCmdLcomGravityDlg::OnEffLoadModBtn()
{
	UpdateData();

	T_STLD_K StldK = CDlgUtil::CobxGetCurSelItemData(m_cmbEffLoadCase, m_cmbEffLoadCase.GetCurSel());
	if(StldK<1) 
		return;

	T_STLD_D StldD;
	m_pDoc->m_pAttrCtrl->GetStld(StldK,StldD);

	// 리스트에 있나?
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_STRING;
	FindInfo.psz = StldD.LoadCaseName;
	int iItem = m_lstEffLoad.FindItem(&FindInfo);
	if (iItem == -1)
		return;

	LV_ITEM lvitem;
	lvitem.mask = LVIF_TEXT | LVFI_STRING;
	lvitem.iItem  	= iItem;
	lvitem.iSubItem = IDX_NAME;
	lvitem.pszText	= (LPTSTR)(LPCTSTR)StldD.LoadCaseName;
	m_lstEffLoad.SetItem(&lvitem);

	lvitem.iSubItem = IDX_FACTOR;
	lvitem.mask	= LVIF_TEXT;
	CString strFactor;
	strFactor.Format(_T("%g"), m_dEffLoadFactor);
	lvitem.pszText	= (LPTSTR)(LPCTSTR)strFactor;
	m_lstEffLoad.SetItem(&lvitem);
}

void CCmdLcomGravityDlg::OnEffLoadDelBtn()
{
	int iItem = m_lstEffLoad.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_delet));
		return;
	}

	m_lstEffLoad.DeleteItem(iItem);
}

void CCmdLcomGravityDlg::OnGrvLeftBtn()
{
	int iItem = m_lstGrvUnsel.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_CMD0417__Not_exist_selected_item));
		return;
	}

	CString strStldName = m_lstGrvUnsel.GetItemText(iItem,0);
	LV_ITEM lvitem;
	lvitem.iItem  	= m_lstGrvSel.GetItemCount();
	lvitem.iSubItem = IDX_NAME;
	lvitem.mask	= LVIF_TEXT | LVFI_STRING;
	lvitem.pszText	= (LPTSTR)(LPCTSTR)strStldName;
	m_lstGrvSel.InsertItem(&lvitem);
	m_lstGrvUnsel.DeleteItem(iItem);
}

void CCmdLcomGravityDlg::OnGrvRightBtn()
{
	int iItem = m_lstGrvSel.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_CMD0417__Not_exist_selected_item));
		return;
	}

	CString strStldName = m_lstGrvSel.GetItemText(iItem,0);
	LV_ITEM lvitem;
	lvitem.iItem  	= m_lstGrvUnsel.GetItemCount();
	lvitem.iSubItem = IDX_NAME;
	lvitem.mask	= LVIF_TEXT | LVFI_STRING;
	lvitem.pszText	= (LPTSTR)(LPCTSTR)strStldName;
	m_lstGrvUnsel.InsertItem(&lvitem);
	m_lstGrvSel.DeleteItem(iItem);
}