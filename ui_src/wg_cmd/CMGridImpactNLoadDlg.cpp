// CMGridImpactNLoadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMGridImpactNLoadDlg.h"
#include "CMGridP1LoadFactorTypeDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"


#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#include "ExportFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMGridImpactNLoadDlg dialog


CCMGridImpactNLoadDlg::CCMGridImpactNLoadDlg(CWnd* pParent /*=NULL*/)
	: CCMChildBarBase(CCMGridImpactNLoadDlg::IDD)
{
	//{{AFX_DATA_INIT(CCMGridImpactNLoadDlg)
	m_nFactorType = 0;
	m_nImpactFactor = 0;
	m_nStructType = 0;
	//}}AFX_DATA_INIT
	arImpactFactor.Add(IDC_IMPACT_FRM);
	arImpactFactor.Add(IDC_CMD_GIMP_TYPERDO1);
	arImpactFactor.Add(IDC_CMD_GIMP_TYPERDO2);
	arImpactFactor.Add(IDC_CMD_GIMP_TYPERDO3);
	arImpactFactor.Add(IDC_CMD_GIMP_TYPERDO4);
	arImpactFactor.Add(IDC_CMD_STATIC1);
	arImpactFactor.Add(IDC_CMD_GIMP_LENGTH_EDT);
	arImpactFactor.Add(IDC_CMD_GIMP_FACTOR_EDT);
	arImpactFactor.Add(IDC_CMD_GIMP_UNIT);

	arP1LoadFactor.Add(IDC_P1LOAD_FRM);
	arP1LoadFactor.Add(IDC_CMD_STATIC2);
	arP1LoadFactor.Add(IDC_CMD_STATIC3);
	//arP1LoadFactor.Add(IDC_CMD_STATIC4);
	//arP1LoadFactor.Add(IDC_CMD_STATIC5);
	arP1LoadFactor.Add(IDC_LOADTYPE_COMBO);
	arP1LoadFactor.Add(IDC_CMD_MORE_BUTTON);
	arP1LoadFactor.Add(IDC_CMD_LOAD_ITEM);
	arP1LoadFactor.Add(IDC_LOAD_TYPE_EDT);

	arStructType.Add(IDC_STRUCTTYPE_FRM);
	arStructType.Add(IDC_CMD_STRUCTTYPE_RDO1);
	arStructType.Add(IDC_CMD_STRUCTTYPE_RDO2);
	arStructType.Add(IDC_CMD_STRUCTTYPE_RDO3);
	arStructType.Add(IDC_CMD_STRUCTTYPE_RDO4);
}


void CCMGridImpactNLoadDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMChildBarBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMGridImpactNLoadDlg)
	DDX_Control(pDX, IDC_LOADTYPE_COMBO, m_comboLoadType);
	DDX_Control(pDX, IDC_LOAD_TYPE_EDT, m_wndLoadItem);
	DDX_Control(pDX, IDC_CMD_GIMP_FACTOR_EDT, m_wndImpactFactor);
	DDX_Control(pDX, IDC_CMD_GIMP_LENGTH_EDT, m_wndSpanLength);
	DDX_Control(pDX, IDC_CMD_GRID_LIST, m_GridList);
	DDX_Control(pDX, IDC_CMD_SEL_LINK, m_ctrlSelectLink);
	DDX_Control(pDX, IDC_CMD_LINK, m_ctrlList);
	DDX_Control(pDX, IDC_CMD_GIMP_UNIT, m_dSpanLengthUnit);
	DDX_Radio(pDX, IDC_CMD_CONS_IMPACT, m_nFactorType);
	DDX_Radio(pDX, IDC_CMD_GIMP_TYPERDO1, m_nImpactFactor);
	DDX_Radio(pDX, IDC_CMD_STRUCTTYPE_RDO1, m_nStructType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMGridImpactNLoadDlg, CCMChildBarBase)
	//{{AFX_MSG_MAP(CCMGridImpactNLoadDlg)
	ON_BN_CLICKED(IDC_CMD_CONS_IMPACT, OnTmConsImpact)
	ON_BN_CLICKED(IDC_CMD_CONS_STTYPE, OnTmConsImpact)
	ON_BN_CLICKED(IDC_CMD_GIMP_TYPERDO1, OnTmImpactRdo)
	ON_BN_CLICKED(IDC_CMD_MORE_BUTTON, OnCmdMoreButton)
	ON_CBN_SELCHANGE(IDC_LOADTYPE_COMBO, OnSelchangeLoadtypeCombo)
	ON_BN_CLICKED(IDC_CMD_GRID_LEFT, OnCmdGridLeft)
	ON_BN_CLICKED(IDC_CMD_GRID_RIGHT, OnCmdGridRight)
	ON_BN_CLICKED(IDC_CMD_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_CMD_DELETE, OnTmDelete)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_GRID_LIST, OnItemChangedGridList)
	ON_BN_CLICKED(IDC_CMD_GIMP_TYPERDO2, OnTmImpactRdo)
	ON_BN_CLICKED(IDC_CMD_GIMP_TYPERDO3, OnTmImpactRdo)
	ON_BN_CLICKED(IDC_CMD_GIMP_TYPERDO4, OnTmImpactRdo)
	ON_BN_CLICKED(IDC_CMD_CONS_P1LOAD, OnTmConsImpact)
	ON_LBN_DBLCLK(IDC_CMD_LINK, OnDblclkCmdLink)
	ON_LBN_DBLCLK(IDC_CMD_SEL_LINK, OnDblclkCmdSelLink)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMGridImpactNLoadDlg message handlers

BOOL CCMGridImpactNLoadDlg::OnInitDialog() 
{
	CCMChildBarBase::OnInitDialog();
	
	m_comboLoadType.SetType(D_COBXP1LT_TYPE_MFORCE);
	GetDlgItem(IDC_CMD_CLOSE)->ShowWindow(FALSE);

	m_pDoc = CDBDoc::GetDocPoint();
	m_dSpanLengthUnit.SetUnitType(CUnitCtrl::m_LENGTH_UNIT);
	AlignCtrl();
	SetListCtrlHeader();
	
	MakeItemEx();
	T_LNKF_D data;
	data.Initialize();
	Data2Dlg(data);

	InitLinkList();
	OnTmImpactRdo();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMGridImpactNLoadDlg::AlignCtrl()
{
	CRect rect1, rect2;
	GetDlgItem(IDC_IMPACT_FRM)->GetWindowRect(rect1);
	GetDlgItem(IDC_P1LOAD_FRM)->GetWindowRect(rect2);
	int nWidth = rect1.left - rect2.left;
	int nHeight = rect1.top - rect2.top;
	
	CDlgUtil::CtrlMoveDistXY(this, arP1LoadFactor, nWidth, nHeight);
	CDlgUtil::CtrlShowHide(this, arP1LoadFactor, FALSE);

	//Structure Type
	GetDlgItem(IDC_STRUCTTYPE_FRM)->GetWindowRect(rect2);
	nWidth = rect1.left - rect2.left;
	nHeight = rect1.top - rect2.top;
	
	CDlgUtil::CtrlMoveDistXY(this, arStructType, nWidth, nHeight);
	CDlgUtil::CtrlShowHide(this, arStructType, FALSE);
}

void CCMGridImpactNLoadDlg::InitLinkList()
{
	m_ctrlList.ResetContent();

	CArray<T_NLNK_K, T_NLNK_K>arNonlinearKey;
	CArray<T_ELNK_K, T_ELNK_K>arElasticKey;

	m_pDoc->m_pAttrCtrl->GetNlnkKeyList(arNonlinearKey);
	m_pDoc->m_pAttrCtrl->GetElnkKeyList(arElasticKey);

	CString strName, tmp;
	int nIndex;
	int nSize = arElasticKey.GetSize();
	tmp = _LS(IDS_CMD_IMPACTNP1_ELINK);
	for (int i=0; i<nSize; i++)
	{
		strName.Format(tmp + _T("(%d)"), arElasticKey[i]);
		nIndex = m_ctrlList.AddString (strName);
		m_ctrlList.SetItemData(nIndex, (DWORD)arElasticKey[i]);
	}

	nSize = arNonlinearKey.GetSize();
	tmp = _LS(IDS_CMD_IMPACTNP1_NLINK);
	for (int i=0; i<nSize; i++)
	{
		strName.Format(tmp + _T("(%d)"), arNonlinearKey[i]);
		nIndex = m_ctrlList.AddString (strName);
		m_ctrlList.SetItemData(nIndex, (DWORD)arNonlinearKey[i]);
	}
}

void CCMGridImpactNLoadDlg::OnTmConsImpact() 
{
	UpdateData();

	CDlgUtil::CtrlShowHide(this, arImpactFactor, m_nFactorType == 0);
	CDlgUtil::CtrlShowHide(this, arP1LoadFactor, m_nFactorType == 1);
	CDlgUtil::CtrlShowHide(this, arStructType, m_nFactorType == 2);
}

void CCMGridImpactNLoadDlg::OnTmImpactRdo()
{
	UpdateData();

	m_wndImpactFactor.EnableWindow(FALSE);
	m_wndSpanLength.EnableWindow(FALSE);
	
	if (m_nImpactFactor == 2)
		m_wndSpanLength.EnableWindow(TRUE);
	else if (m_nImpactFactor == 3)
		m_wndImpactFactor.EnableWindow(TRUE);
}

void CCMGridImpactNLoadDlg::Execute()
{
}

BOOL CCMGridImpactNLoadDlg::ExternalInit(UINT key)
{
	if (key == 0) return FALSE;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (pDoc == NULL) { ASSERT(0); return FALSE; }

	int nItemCount = m_GridList.GetItemCount();
	int i = 0;
	for (i=0; i<nItemCount; i++)
		if (m_GridList.GetItemData(i) == key) break;
	if (i == nItemCount) return FALSE;
	
	//CDlgUtil::UnselectAllListItems(&m_GridList);
	//m_GridList.SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);

	T_LNKF_D data;
	if (!pDoc->m_pAttrCtrl->GetLnkf(key, data))
		data.Initialize();

	Data2Dlg(data);

	return TRUE;
}

void CCMGridImpactNLoadDlg::OnCmdMoreButton() 
{
	CCMGridP1LoadFactorTypeDlg dlg;
	dlg.DoModal();

	//CDBDoc* pDoc = CDBDoc::GetDocPoint();
	//CreateOrActivateDlg(pDoc, CCMGridP1LoadFactorTypeDlg::IDD, this);
}

void CCMGridImpactNLoadDlg::OnSelchangeLoadtypeCombo() 
{
	T_P1LT_K P1ltK;
	T_P1LT_D P1ltD;
	m_comboLoadType.GetSelectedP1lt(P1ltK);
	
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc->m_pAttrCtrl->GetP1lt(P1ltK, P1ltD))
		return;

	if (P1ltD.nLoadItem == 0)
		GetDlgItem(IDC_CMD_LOAD_ITEM)->SetWindowText(_LS(IDS_CMD_IMPACTNP1_REACTION));
	else if (P1ltD.nLoadItem == 1)
		GetDlgItem(IDC_CMD_LOAD_ITEM)->SetWindowText(_LS(IDS_CMD_IMPACTNP1_DISP));
	else if (P1ltD.nLoadItem == 2)
		GetDlgItem(IDC_CMD_LOAD_ITEM)->SetWindowText(_LS(IDS_CMD_IMPACTNP1_MEMBFORCE));

	CString strLoadType=_T("");
	for (int i=0; i<6; i++)
	{
		if (P1ltD.nLoadType[i] == 0)
			strLoadType += _T("B");
		else if (P1ltD.nLoadType[i] == 1)
			strLoadType += _T("S");
	}
	m_wndLoadItem.SetWindowText(strLoadType);
}

void CCMGridImpactNLoadDlg::OnCmdGridRight() 
{
	SelectLink();
}

void CCMGridImpactNLoadDlg::OnCmdGridLeft() 
{
	UnselectLink();
}

void CCMGridImpactNLoadDlg::OnTmExecute() 
{
	T_LNKF_D data;
	int nCount = m_ctrlSelectLink.GetCount();
	if (nCount < 1)
		return;
	
	//CArray<int,int> arListBoxSel;
	//arListBoxSel.SetSize(nCount);
	//m_ctrlSelectLink.GetSelItems(nCount, arListBoxSel.GetData()); 
	
	if (!Dlg2Data(data, 0, 0)) return;
	if (!ValidData(data)) return;

	CArray<T_LNKF_D, T_LNKF_D&> raData;
	int nStrIndex;
	CString strName;
	for (int i=0; i<nCount; i++)
	{
		m_ctrlSelectLink.GetText(i, strName);

		nStrIndex = strName.Find('(');
		if (nStrIndex == -1)
			ASSERT(0);

		strName = strName.Left(nStrIndex);
		if (strName == _LS(IDS_CMD_IMPACTNP1_ELINK))
			data.nLinkType = 0;
		else if (strName == _LS(IDS_CMD_IMPACTNP1_NLINK))
			data.nLinkType = 1;

		data.LinkKey = m_ctrlSelectLink.GetItemData(i);
		raData.Add(data);
	}
	m_pDoc->m_pDataCtrl->AddLnkf(raData);
	nCount = m_GridList.GetItemCount();
	
	//CDlgUtil::UnselectAllListItems(&m_GridList);
	//m_GridList.SetItemState(nCount-1, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMGridImpactNLoadDlg::OnTmDelete() 
{
	T_LNKF_K key;
	int nIndex;
	if (!GetSelectedData(nIndex))
	{
		GSaveHistoryFormatNF(_LS(IDS_CMD__ADDD__Error___CROSS_REFERENCE_NO_SELECTED));
		return;
	}
	/**
	int nCount = m_GridList.GetItemCount();
	key = m_GridList.GetItemData(nIndex);
	
	if (m_pDoc->m_pDataCtrl->DelLnkf(key))
	{
		if (nIndex >= nCount-1) nIndex--;
		m_GridList.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
	}
	**/

	POSITION pos = m_GridList.GetFirstSelectedItemPosition();
	while(pos)
	{
		nIndex = m_GridList.GetNextSelectedItem(pos);
		key = m_GridList.GetItemData(nIndex);
		m_pDoc->m_pDataCtrl->DelLnkf(key);
	}
}

void CCMGridImpactNLoadDlg::OnTmClose() 
{
	CloseDlg();
}

BOOL CCMGridImpactNLoadDlg::GetSelectedData(int &nIndex)
{
	int iItem = m_GridList.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;

	nIndex = iItem;
	return TRUE;
}

BOOL CCMGridImpactNLoadDlg::ValidData(T_LNKF_D& data)
{
	BOOL bError = FALSE;
	if (data.nFactorKind == 0)
	{
	}
	else if (data.nFactorKind == 1)
	{
		if (data.TypeKey < 1)
			bError = TRUE;
	}
	
	if (bError)
	{
		GSaveHistoryFormatNF(_LS(IDS_CMD__ADDD__Error___CROSS_REFERENCE));
		return FALSE;
	}
	return TRUE;
}

void CCMGridImpactNLoadDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		MakeItemEx();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
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

void CCMGridImpactNLoadDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;

		switch(nCmd)
		{
		case(UR_LNKF_ADD):
		case(UR_LNKF_DEL):
			bMFD = TRUE;
			break;
		default:
			break;
		}
	}

	if (bMFD)
		MakeItemEx();
}

void CCMGridImpactNLoadDlg::MakeItemEx()
{
	m_GridList.DeleteAllItems();

	T_LNKF_K key;
	T_LNKF_D data;
	CArray<T_LNKF_K, T_LNKF_K> aKey;
	m_pDoc->m_pAttrCtrl->GetLnkfKeyList(aKey);
	
	int nSize = aKey.GetSize();
	if (nSize == 0)
		return;
	
	for (int i=0; i<nSize; i++)
	{
		key = aKey[i];
		m_pDoc->m_pAttrCtrl->GetLnkf(key, data);
		m_GridList.InsertItem(i, _T(""));
		SetItem(i, key, data);
	}
	//CDlgUtil::UnselectAllListItems(&m_GridList);
	//m_GridList.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMGridImpactNLoadDlg::SetItem(int nIndex, T_LNKF_K key, T_LNKF_D &data)
{
	LVITEM lvitem;
	CString str;
	lvitem.iItem=nIndex;
	for(int i = 0; i < 2; i++)
	{
		lvitem.iSubItem=i;
		str = DataToStr(i, key, data);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT ;

		m_GridList.SetItem(&lvitem);
	  str.ReleaseBuffer();
	}
	m_GridList.SetItemData(nIndex, (DWORD)key);
}

CString CCMGridImpactNLoadDlg::DataToStr(int i, T_LNKF_K key, T_LNKF_D &data)
{
	CString value, tmp;
	
	switch(i)
	{
		case 0:
			if (data.nLinkType == 0)
				tmp = _LS(IDS_CMD_IMPACTNP1_ELINK);
			else if (data.nLinkType == 1)
				tmp = _LS(IDS_CMD_IMPACTNP1_NLINK);
			
			value.Format(tmp + _T("(%d)"), data.LinkKey);
			break;
		case 1:
			if (data.nFactorKind == 0)
				value = _LS(IDS_CMD_IMPACTNP1_IMPACTFACTOR);
			else if (data.nFactorKind == 1)
				value = _LS(IDS_CMD_IMPACTNP1_P1);
			else if (data.nFactorKind == 2)
				value = _LS(IDS_CMD_IMPACTNP1_STRUCTTYPE);
			break;
	}
	return value;
}

void CCMGridImpactNLoadDlg::SetListCtrlHeader()
{
	CString aTitle[] = {_LS(IDS_CMD_IMPACTNP1_LINK), _LS(IDS_CMD_IMPACTNP1_FACTORTYPE)};
	int nColWidth[] = {80, 110};
	int nColNum = 2;
	CString title;
	int i;
	LV_COLUMN lvcolumn;

	// set full row select mode
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_GridList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_GridList.GetSafeHwnd(), dwStyle);

	// Set Title
	for(i = 0; i < nColNum; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_GridList.InsertColumn(i,&lvcolumn);
		m_GridList.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

BOOL CCMGridImpactNLoadDlg::Dlg2Data(T_LNKF_D& data, int linkType, UINT key)
{
	UpdateData(TRUE);
	data.nLinkType = linkType;
	data.LinkKey = key;
	data.nFactorKind = m_nFactorType;

	if (m_nFactorType == 0)
	{
		//Impact Factor
		data.nFactorType = m_nImpactFactor;
		if (m_nImpactFactor == 2)
		{
			data.dSpanLength = m_wndSpanLength.GetEditValue();
		}
		else if (m_nImpactFactor == 3)
		{
			data.dImpactFactor = m_wndImpactFactor.GetEditValue();
		}
	}
	else if (m_nFactorType == 1)
	{
		//P1 Load Factor
		m_comboLoadType.GetSelectedP1lt(data.TypeKey);
	}
	else if (m_nFactorType == 2)
	{
		//Structure Type
		data.nElemType = m_nStructType;
	}

	return TRUE;
}

void CCMGridImpactNLoadDlg::Data2Dlg(T_LNKF_D& data)
{
	m_nFactorType = data.nFactorKind;
	BOOL bImpactShow;
	if (m_nFactorType == 0)
	{
		bImpactShow = TRUE;
		m_nImpactFactor = data.nFactorType;
		
		m_wndSpanLength.EnableWindow(FALSE);
		m_wndImpactFactor.EnableWindow(FALSE);

		if (m_nImpactFactor == 2)
		{
			m_wndSpanLength.SetEditUnit(data.dSpanLength);
			m_wndSpanLength.EnableWindow(TRUE);
		}
		else if (m_nImpactFactor == 3)
		{
			m_wndImpactFactor.SetEditUnit(data.dImpactFactor);
			m_wndImpactFactor.EnableWindow(TRUE);
		}
	}
	else if (m_nFactorType == 1)
	{
		bImpactShow = FALSE;
		m_comboLoadType.ChangeSelect(data.TypeKey);
		OnSelchangeLoadtypeCombo();
	}
	else if (m_nFactorType == 2)
	{
		m_nStructType = data.nElemType;
	}
	CDlgUtil::CtrlShowHide(this, arImpactFactor, m_nFactorType == 0);
	CDlgUtil::CtrlShowHide(this, arP1LoadFactor, m_nFactorType == 1);
	CDlgUtil::CtrlShowHide(this, arStructType, m_nFactorType == 2);

	UpdateData(FALSE);

	// List의 내용은 보여주지 않도록 한다.
	/**
	InitLinkList();
	m_ctrlSelectLink.ResetContent();

	CString strItem(_T("")), temp, strCapt;
	if (data.nLinkType == 0)
		strCapt = _LS(IDS_CMD_IMPACTNP1_ELINK);
	else if (data.nLinkType == 1)
		strCapt = _LS(IDS_CMD_IMPACTNP1_NLINK);
	else ASSERT(0);
	
	strItem.Format(strCapt + _T("(%d)"), data.LinkKey);

	int nCount = m_ctrlList.GetCount();
	for (int i=0; i<nCount; i++)
	{
		m_ctrlList.GetText(i, temp);
		if (temp == strItem) break;
	}
	if (i < nCount)
	{
		int nIndex;
		m_ctrlList.DeleteString(i);
		nIndex = m_ctrlSelectLink.AddString(strItem);
		m_ctrlSelectLink.SetItemData(nIndex, data.LinkKey);
	}
	**/
}

void CCMGridImpactNLoadDlg::OnItemChangedGridList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	T_LNKF_K key;
	T_LNKF_D data;
	int nIndex;

	if (!GetSelectedData(nIndex))
	{
		data.Initialize();
	}
	else 
	{
		if (pNMListView->uNewState==0 || pNMListView->uNewState==1)
		{
			*pResult = 0;
			return;
		}
		
		nIndex = pNMListView->iItem;
		key = m_GridList.GetItemData(nIndex);
		m_pDoc->m_pAttrCtrl->GetLnkf(key, data);
	}
	
	Data2Dlg(data);

	*pResult = 0;
}

void CCMGridImpactNLoadDlg::OnDblclkCmdLink() 
{
	SelectLink();
}

void CCMGridImpactNLoadDlg::OnDblclkCmdSelLink() 
{
	UnselectLink();
}

void CCMGridImpactNLoadDlg::SelectLink()
{
	int nCount = m_ctrlList.GetSelCount();
	if (nCount < 1)
		return;
	
	CArray<int,int> arListBoxSel;
	arListBoxSel.SetSize(nCount);
	m_ctrlList.GetSelItems(nCount, arListBoxSel.GetData()); 

	int nIndex;
	CString strName;
	DWORD itemData;
	for (int i=0; i<nCount; i++)
	{
		nIndex = nCount-1-i;
		m_ctrlList.GetText(arListBoxSel[nIndex], strName);
		itemData = m_ctrlList.GetItemData(arListBoxSel[nIndex]);
		m_ctrlList.DeleteString(arListBoxSel[nIndex]);

		nIndex = m_ctrlSelectLink.AddString(strName);
		m_ctrlSelectLink.SetItemData(nIndex, itemData);
	}
}

void CCMGridImpactNLoadDlg::UnselectLink()
{
	int nCount = m_ctrlSelectLink.GetSelCount();
	if (nCount < 1)
		return;
	
	CArray<int,int> arListBoxSel;
	arListBoxSel.SetSize(nCount);
	m_ctrlSelectLink.GetSelItems(nCount, arListBoxSel.GetData()); 

	int nIndex;
	CString strName;
	DWORD itemData;
	for (int i=0; i<nCount; i++)
	{
		nIndex = nCount-1-i;
		m_ctrlSelectLink.GetText(arListBoxSel[nIndex], strName);
		itemData = m_ctrlSelectLink.GetItemData(arListBoxSel[nIndex]);
		m_ctrlSelectLink.DeleteString(arListBoxSel[nIndex]);

		nIndex = m_ctrlList.AddString(strName);
		m_ctrlList.SetItemData(nIndex, itemData);
	}
}
