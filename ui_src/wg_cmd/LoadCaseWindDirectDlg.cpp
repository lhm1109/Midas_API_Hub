// CMLoadMLLCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "LoadCaseWindDirectDlg.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "CMLoadCombDlg.h"
#include "CMLoadCaseDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoadCaseWindDirectDlg dialog

CLoadCaseWindDirectDlg::CLoadCaseWindDirectDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CLoadCaseWindDirectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoadCaseWindDirectDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
	m_nType = 0;
	m_nBuildingType = 1;

	m_aCtrlHigh.RemoveAll();
	m_aCtrlHigh.Add(IDC_CMD_LOAD_WIND_TORSION_COMB_STATIC);
	m_aCtrlHigh.Add(IDC_CMD_LOAD_WIND_TORSION_CASE_CBO);
	m_aCtrlHigh.Add(IDC_CMD_LOAD_WIND_FRAME3);
	m_aCtrlHigh.Add(IDC_CMD_LOAD_WIND_FACT_GD_EDIT_STATIC);
	m_aCtrlHigh.Add(IDC_CMD_LOAD_WIND_FACT_K_EDIT_STATIC);
	m_aCtrlHigh.Add(IDC_CMD_LOAD_WIND_FACT_GD_EDT);
	m_aCtrlHigh.Add(IDC_CMD_LOAD_WIND_FACT_K_EDT);
}


void CLoadCaseWindDirectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoadCaseWindDirectDlg)
	DDX_Radio(pDX, IDC_CMD_LOAD_WIND_MIDD_RDO           , m_nBuildingType);  
	DDX_Control(pDX, IDC_CMD_LOAD_WIND_ALONG_CASE_CBO   , m_LoadCaseAlong);
	DDX_Control(pDX, IDC_CMD_LOAD_WIND_ACROSS_CASE_CBO  , m_LoadCaseAcross);
	DDX_Control(pDX, IDC_CMD_LOAD_WIND_TORSION_CASE_CBO , m_LoadCaseTorsion);
	DDX_Control(pDX, IDC_CMD_LOAD_WIND_FACT_GD_EDT      , m_edtFactorGD);
	DDX_Control(pDX, IDC_CMD_LOAD_WIND_FACT_K_EDT       , m_edtFactorK );
	DDX_Control(pDX, IDC_CMD_LOAD_WIND_TORSION_WIND_CBO , m_cmbWindDirection);
	DDX_Control(pDX, IDC_CMD_LOAD_WIND_LIST, m_ListCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoadCaseWindDirectDlg, CDialogMove)
	//{{AFX_MSG_MAP(CLoadCaseWindDirectDlg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_LOAD_WIND_LIST, OnChangedCurrentItem) 
	ON_NOTIFY(LVN_ITEMCHANGING, IDC_CMD_LOAD_WIND_LIST, OnChangedCurrentItem) 
 	ON_BN_CLICKED(IDC_CMD_LOAD_WIND_CASE_BTN, OnClickLoadCaseBtn)
	ON_BN_CLICKED(IDC_CMD_LOAD_WIND_MIDD_RDO, OnClickBuildingTypeRdo)
	ON_BN_CLICKED(IDC_CMD_LOAD_WIND_HIGH_RDO, OnClickBuildingTypeRdo)
	ON_BN_CLICKED(IDC_CMD_LOAD_WIND_ADD_BTN, OnCmdWindAddBtn)
	ON_BN_CLICKED(IDC_CMD_LOAD_WIND_MOD_BTN, OnCmdWindModBtn)
	ON_BN_CLICKED(IDC_CMD_LOAD_WIND_DEL_BTN, OnCmdWindDelBtn)
	ON_BN_CLICKED(IDC_CMD_LOAD_WIND_CANCEL_BTN, OnCmdCancelBtn)
	ON_BN_CLICKED(IDC_CMD_LOAD_WIND_OK_BTN, OnCmdOkBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoadCaseWindDirectDlg message handlers

BOOL CLoadCaseWindDirectDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	if (!m_pDoc->m_pAttrCtrl->GetLcwd(m_Data)) 
	{
		m_Data.Initialize();
	}

	InitCtrl();
	Data2Dlg();
	
	//OnCmdTypeRadio();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CLoadCaseWindDirectDlg::~CLoadCaseWindDirectDlg()
{
}

void CLoadCaseWindDirectDlg::InitCtrl()
{
	InitCtrl_LoadCaseCmb();

	m_cmbWindDirection.ResetContent();
	m_cmbWindDirection.AddString(_LS(IDS_CMD_WIND_DIRECTION_PLUS_MINUS));
	m_cmbWindDirection.AddString(_LS(IDS_CMD_WIND_DIRECTION_PLUS));
	m_cmbWindDirection.AddString(_LS(IDS_CMD_WIND_DIRECTION_MINUS));

	m_edtFactorGD.SetUnitType(D_UNITSYS_NONE);
	m_edtFactorK.SetUnitType(D_UNITSYS_NONE);

	SetListCtrlHeader();
}

void CLoadCaseWindDirectDlg::InitCtrl_LoadCaseCmb()
{
	m_LoadCaseAlong.ResetContent();
	m_LoadCaseAcross.ResetContent();
	m_LoadCaseTorsion.ResetContent();

	//m_LoadCaseAlong.AddString(_LS(IDS_WG_CMD__ADDD__None));
	m_LoadCaseAcross.AddString(_LS(IDS_WG_CMD__ADDD__None));
	m_LoadCaseTorsion.AddString(_LS(IDS_WG_CMD__ADDD__None));

	// Static
	CArray<T_STLD_K, T_STLD_K> arKeyList;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(arKeyList);
	int iCount = m_pDoc->m_pAttrCtrl->GetCountStld();
	if(iCount < 1) 
	{
		//m_LoadCaseAlong.SetCurSel(0);
		m_LoadCaseAcross.SetCurSel(0);
		m_LoadCaseTorsion.SetCurSel(0);
		return;
	}

	for(int i = 0; i < iCount; i++)
	{
		int iKey = arKeyList.GetAt(i);
		T_STLD_D rData;
		rData.Initialize();
		BOOL bCheck = m_pDoc->m_pAttrCtrl->GetStld(iKey, rData);
		ASSERT(bCheck);            

		CString strLoadType = rData.LoadCaseType;
		CString strLoadCase = rData.LoadCaseName;
		if     (strLoadType==_T("W"))  m_LoadCaseAlong.AddString(strLoadCase);   
		else if(strLoadType==_T("WA")) m_LoadCaseAcross.AddString(strLoadCase);   
		else if(strLoadType==_T("WT")) m_LoadCaseTorsion.AddString(strLoadCase);   
		else continue;
	}

	if(m_LoadCaseAlong.GetCount() > 1)m_LoadCaseAlong.SetCurSel(1);
	else m_LoadCaseAlong.SetCurSel(0);
	if(m_LoadCaseAcross.GetCount() > 1)m_LoadCaseAcross.SetCurSel(1);
	else m_LoadCaseAcross.SetCurSel(0);
	if(m_LoadCaseTorsion.GetCount() > 1) m_LoadCaseTorsion.SetCurSel(1);
	else m_LoadCaseTorsion.SetCurSel(0);
}

void CLoadCaseWindDirectDlg::Data2Dlg()
{
	//m_nType = m_Data.nType;
	UpdateData(FALSE);

	m_edtFactorGD.SetEditUnit(2.2);
	m_edtFactorK.SetEditUnit(0.8);

	MakeItemEx();

	m_cmbWindDirection.SetCurSel(m_Data.nTorsionWindDirect);
	CtrlEnable();
}

void CLoadCaseWindDirectDlg::CtrlEnable()
{
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlHigh, m_nBuildingType==1);
}

void CLoadCaseWindDirectDlg::SetListCtrlHeader()
{
	CString aTitle[] = {  _LS(IDS_CMD_Orthgonal_Effect_Group_No), _LS(IDS_CMD_WIND_DIRECTION_ALONG), _LS(IDS_CMD_WIND_DIRECTION_ACROSS), _LS(IDS_CMD_WIND_DIRECTION_TORSION),  _LS(IDS_CMD_WIND_DIRECTION_FACTOR_GD), _LS(IDS_CMD_WIND_DIRECTION_FACTOR_K), _LS(IDS_CMD_WIND_DIRECTION_FACTOR_K)/*, _T("Auto") */};
	int nColWidth[] = {70, 60, 60, 60, 40, 40/*, 40*/};
	int nColNum = 6;
	CString title;
	int i;
	LV_COLUMN lvcolumn;

	// set full row select mode
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_ListCtrl.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_ListCtrl.GetSafeHwnd(), dwStyle);

	// Set Title
	for(i = 0; i < nColNum; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_CENTER;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_ListCtrl.InsertColumn(i,&lvcolumn);
		m_ListCtrl.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

BOOL CLoadCaseWindDirectDlg::GetSelectedItem(int &nIndex)
{
	int iItem = m_ListCtrl.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;

	nIndex = iItem;
	return TRUE;
}

void CLoadCaseWindDirectDlg::MakeItemEx()
{
	m_ListCtrl.DeleteAllItems();

	int nItemCount = m_Data.aLoadCombination.GetSize(); 
	if(nItemCount == 0) return;

	for (int nCount = 0; nCount < nItemCount; nCount++)
	{
		m_ListCtrl.InsertItem(nCount, _T(""));
		SetItem(nCount, m_Data.aLoadCombination.GetAt(nCount));
	}
	m_ListCtrl.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);  

	m_edtFactorGD.SetEditUnit(m_Data.aLoadCombination[0].dFactorGD);
	m_edtFactorK.SetEditUnit(m_Data.aLoadCombination[0].dFactorK);

	m_nBuildingType = m_Data.aLoadCombination[0].nBuildingType;
	UpdateData(FALSE);
}


void CLoadCaseWindDirectDlg::SetItem(int nIndex, T_LCWD_BASE &data)
{
	LVITEM lvitem;
	CString str;
	CString strIndex;
	for(int i = 0; i < 6; i++)
	{
		lvitem.iItem=nIndex;
		lvitem.iSubItem=i;
		if(i==0) str.Format(_T("%d"),nIndex+1);
		else	 str = DataToStr(i, data);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT ;

		m_ListCtrl.SetItem(&lvitem);
		str.ReleaseBuffer();
	}
}

CString CLoadCaseWindDirectDlg::DataToStr(int i, T_LCWD_BASE &data)
{
	CString value = _T("");
	
	T_STLD_K key;
	if     (i==1) key = data.StldKAlong;
	else if(i==2) key = data.StldKAcross;
	else if(i==3) key = data.StldKTorsion;
	else if(i==4) value.Format(_T("%g"), data.dFactorGD);
	else if(i==5) value.Format(_T("%g"), data.dFactorK);
	else if(i==6) value.Format(_T("%d"), data.bAuto);
	if(i<4)
	{
		T_STLD_D rData;
		if(m_pDoc->m_pAttrCtrl->GetStld(key, rData)) value = rData.LoadCaseName;
		else value = _LS(IDS_WG_CMD__ADDD__None);
	}
	else if(i<6)
	{
		if(data.nBuildingType == 0) value = _T("-");
	}
	return value;
}

BOOL CLoadCaseWindDirectDlg::ValidItem(T_LCWD_BASE& data)
{
	if (data.StldKAlong <= 0 && data.StldKAcross <= 0 && data.StldKTorsion <= 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Invalid_loadcase_key_));
		return FALSE;
	}
	if(data.dFactorGD <= 0.0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD_ERROR_CHK_WIND_LOAD_DIRECTION_GD));
		return FALSE;
	}
	if(data.dFactorK  > 0.8 || data.dFactorK  < 0.55)
	{
		AfxMessageBox(_LS(IDS_WG_CMD_ERROR_CHK_WIND_LOAD_DIRECTION_K));
		return FALSE;
	}
	return TRUE;
}

void CLoadCaseWindDirectDlg::OnChangedCurrentItem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	int nIndex;
	T_LCWD_BASE data;
	if (GetSelectedItem(nIndex))
	{
		data = m_Data.aLoadCombination.GetAt(nIndex);
		// 값을 설정한다.
		Item2Dlg(nIndex, data);
	}

	*pResult = 0;
}

BOOL CLoadCaseWindDirectDlg::Dlg2Item(T_LCWD_BASE &data)
{
	int IndexAlong   = m_LoadCaseAlong.GetCurSel();
	int IndexAcross  = m_LoadCaseAcross.GetCurSel();
	int IndexTorsion = m_LoadCaseTorsion.GetCurSel();
	if(IndexAlong==-1)
	{
		AfxMessageBox(_LS(IDS_WG_TREEMENU_Error___Load_case_was_not_selected_));
		return FALSE;
	}

	if(IndexAlong != -1 || IndexAcross != -1 || IndexTorsion != -1)
	{
		CString strNameAlong=_T(""), strNameAcross=_T(""), strNameTorsion=_T("");

		if(IndexAlong != -1) m_LoadCaseAlong.GetLBText(IndexAlong,strNameAlong);
		m_LoadCaseAcross.GetLBText(IndexAcross,strNameAcross);
		m_LoadCaseTorsion.GetLBText(IndexTorsion,strNameTorsion);

		data.StldKAlong = m_pDoc->m_pAttrCtrl->GetStldKey(strNameAlong);
		data.StldKAcross = m_pDoc->m_pAttrCtrl->GetStldKey(strNameAcross);
		data.StldKTorsion = m_pDoc->m_pAttrCtrl->GetStldKey(strNameTorsion);
	}

	data.dFactorGD = m_edtFactorGD.GetEditValue();
	data.dFactorK  = m_edtFactorK.GetEditValue();
	
	UpdateData(TRUE);
	data.nBuildingType = m_nBuildingType;

	// Middle Low Rise Building 이면 Torsion Stld key는 0이 되어야 함
	if(data.nBuildingType==0) data.StldKTorsion = 0;
	return TRUE;
}

BOOL CLoadCaseWindDirectDlg::Item2Dlg(int nIndex, T_LCWD_BASE &data)
{
	T_STLD_K KeyAlong = data.StldKAlong;
	T_STLD_D rData;
	CString strLoadCaseName = _T("");
	rData.Initialize();
	BOOL bCheck = m_pDoc->m_pAttrCtrl->GetStld(KeyAlong, rData);
	//ASSERT(bCheck);            
	if(bCheck) strLoadCaseName = rData.LoadCaseName;
	else strLoadCaseName = _LS(IDS_WG_CMD__ADDD__None);
	int iCbIndex = m_LoadCaseAlong.SelectString(-1,strLoadCaseName);
	m_LoadCaseAlong.SetCurSel(iCbIndex);

	T_STLD_K KeyAcross = data.StldKAcross;
	rData.Initialize();
	bCheck = m_pDoc->m_pAttrCtrl->GetStld(KeyAcross, rData);
	//ASSERT(bCheck);
	if(bCheck) strLoadCaseName = rData.LoadCaseName;
	else strLoadCaseName = _LS(IDS_WG_CMD__ADDD__None);
	iCbIndex = m_LoadCaseAcross.SelectString(-1,strLoadCaseName);
	m_LoadCaseAcross.SetCurSel(iCbIndex);

	T_STLD_K KeyTorsion = data.StldKTorsion;
	rData.Initialize();
	bCheck = m_pDoc->m_pAttrCtrl->GetStld(KeyTorsion, rData);
	//ASSERT(bCheck);
	if(bCheck) strLoadCaseName = rData.LoadCaseName;
	else strLoadCaseName = _LS(IDS_WG_CMD__ADDD__None);
	iCbIndex = m_LoadCaseTorsion.SelectString(-1,strLoadCaseName);
	m_LoadCaseTorsion.SetCurSel(iCbIndex);

	m_edtFactorGD.SetEditUnit(data.dFactorGD);
	m_edtFactorK.SetEditUnit(data.dFactorK);

	m_nBuildingType = data.nBuildingType;
	UpdateData(FALSE);
	CtrlEnable();

	return TRUE;
}


void CLoadCaseWindDirectDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		UpdateBuffer();
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
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CLoadCaseWindDirectDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_STLD_K Key;
	T_STLD_D Data;

	int i;

	BOOL bMFDStld = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_STLD_ADD):
				bMFDStld=TRUE;
				break;
		case(UR_STLD_DEL):
		case(UR_STLD_MFD):
		case(UR_STLD_MFS):
				// 현재 loadcase list에서 지워진 것 삭제한다.
				pViewBuff->GetStld(nKey, Key, Data);
				for (i = m_Data.aLoadCombination.GetSize()-1; i >= 0; i--)
				{					
					if (m_Data.aLoadCombination[i].StldKAcross == Key ||
						  m_Data.aLoadCombination[i].StldKAlong  == Key ||
							m_Data.aLoadCombination[i].StldKTorsion== Key)
					{
						m_ListCtrl.DeleteItem(i);
						m_Data.aLoadCombination.RemoveAt(i);
					}
				}
				bMFDStld=TRUE;
				break;
		default:
			break;
		}
	} // end of while

	if(bMFDStld) InitCtrl_LoadCaseCmb();
}
// void CLoadCaseWindDirectDlg::OnCmdTypeRadio()
// {
//   UpdateData(TRUE);
//   CDlgUtil::CtrlEnableDisableByRect(this, IDC_CMD_LOAD_WIND_FRAME2, m_nType==0 ? FALSE : TRUE, TRUE);
//}

void CLoadCaseWindDirectDlg::OnClickLoadCaseBtn()
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE, 0));	
}

void CLoadCaseWindDirectDlg::OnClickBuildingTypeRdo()
{
	UpdateData(TRUE);
	CtrlEnable();
}
void CLoadCaseWindDirectDlg::OnCmdWindAddBtn()
{
	// TODO: Add your control notification handler code here
	T_LCWD_BASE data;
	data.bAuto=FALSE;
	if (!Dlg2Item(data)) return;
	if (!ValidItem(data)) return;
	for (int i = 0; i < m_Data.aLoadCombination.GetSize(); i++)
	{
		T_LCWD_BASE LcwdBase = m_Data.aLoadCombination.GetAt(i);
		
		if (LcwdBase == data)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__An_identical_load_group_already_exists_));
			return;
			//return FALSE;
		}
	}
	
	m_Data.aLoadCombination.Add(data);
	int nNumLC = m_Data.aLoadCombination.GetSize();
	m_ListCtrl.InsertItem(nNumLC-1, _T(""));
	SetItem(nNumLC-1, data);
	m_ListCtrl.SetItemState(nNumLC-1, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CLoadCaseWindDirectDlg::OnCmdWindModBtn()
{
	int nIndex;
	if (!GetSelectedItem(nIndex))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_item_in_the_list__nCh));
		return;
	}

	T_LCWD_BASE data; data.bAuto=FALSE;
	if (!Dlg2Item(data)) return;
	if (!ValidItem(data)) return;
	for (int i = 0; i < m_Data.aLoadCombination.GetSize(); i++)
	{
		T_LCWD_BASE LcwdBase = m_Data.aLoadCombination.GetAt(i);
		if (i == nIndex) continue;  // 변경 대상은 비교 제외
		//
		if (LcwdBase.StldKAlong == data.StldKAlong && LcwdBase.StldKAcross == data.StldKAcross && LcwdBase.StldKTorsion == data.StldKTorsion && 
				fabs(LcwdBase.dFactorGD-data.dFactorGD) < 1.0E-9 && fabs(LcwdBase.dFactorK-data.dFactorK) < 1.0E-9)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__An_identical_load_group_already_exists_));
			return;
			//return FALSE;
		}
	}
	
	m_Data.aLoadCombination.SetAt(nIndex, data);
	SetItem(nIndex, data);
	m_ListCtrl.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CLoadCaseWindDirectDlg::OnCmdWindDelBtn()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
	if(nItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Not_Selected_Data____));
		return;
	}
	m_ListCtrl.DeleteItem(nItem);
	m_Data.aLoadCombination.RemoveAt(nItem);

	for(int i=0; i<m_Data.aLoadCombination.GetSize(); i++)
	{
		T_LCWD_BASE data;
		data = m_Data.aLoadCombination.GetAt(i);
		SetItem(i, data);
		m_ListCtrl.SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
	}
	// Set Focus.
	if(m_Data.aLoadCombination.GetSize() > nItem)
		m_ListCtrl.SetItemState(nItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CLoadCaseWindDirectDlg::OnCmdCancelBtn() 
{
	OnCancel();	
}

void CLoadCaseWindDirectDlg::OnCmdOkBtn() 
{
	UpdateData(TRUE);

	m_Data.nType = m_nType;
	m_Data.nTorsionWindDirect = m_cmbWindDirection.GetCurSel();
	//////////////////////////////////////////////////////////////
	if (m_pDoc->m_pDataCtrl->AddLcwd(m_Data))
	{
		CDialogMove::OnOK();
	}
}



