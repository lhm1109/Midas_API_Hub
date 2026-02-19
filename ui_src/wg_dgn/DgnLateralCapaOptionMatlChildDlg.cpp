// DgnLateralCapaOptionMatlChildDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnLateralCapaOptionMatlChildDlg.h"

#include "DgnLateralCapaOptionMatlDlg.h"


#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\EditData.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_base\wg_base_ControlEx.h"

#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\wg_base_ArrayUtil.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_main\wg_mainRes2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define D_RCPSC_TYPE	0
#define D_STEEL_TYPE	1
#define D_SRC_TYPE		2

/////////////////////////////////////////////////////////////////////////////
// CDgnLateralCapaOptionMatlChildDlg dialog


CDgnLateralCapaOptionMatlChildDlg::CDgnLateralCapaOptionMatlChildDlg(LateralCapaDataAll* pDataAll, CWnd* pParent /*=NULL*/, int nType /*=0*/)
	: CDlgChild(CDgnLateralCapaOptionMatlChildDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	//m_pDoc = CDBDoc::GetDocPoint();
	m_pParent = (CDgnLateralCapaOptionMatlDlg*) pParent;
	m_nType = nType;
	m_pDataAll = pDataAll;

	m_nCurSelectedKey = 0;
	m_bModify = FALSE;

	//{{AFX_DATA_INIT(CDgnLateralCapaOptionMatlChildDlg)
	m_nConsider = 0;
	//}}AFX_DATA_INIT

	// set flag childHasBtn Flag....
	SetChildHasBtnFlag(TRUE);
}


void CDgnLateralCapaOptionMatlChildDlg::DoDataExchange(CDataExchange* pDX)
{
	CDlgChild::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnLateralCapaOptionMatlChildDlg)	
	DDX_Control(pDX, IDC_DGN_PROPERTY_LIST,		m_cPropertyListCtrl);
	DDX_Radio(pDX,   IDC_EQ_RESISTANT_A_RDO,	m_nLimitState);	
	DDX_Radio(pDX,   IDC_CONSIDER_RDO,				m_nConsider);	
	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnLateralCapaOptionMatlChildDlg, CDlgChild)
	//{{AFX_MSG_MAP(CDgnLateralCapaOptionMatlChildDlg)
	ON_BN_CLICKED(IDC_ADD_REPLACE_BTN, OnAddReplaceBtn)
	ON_BN_CLICKED(IDC_DEL_BTN, OnDelBtn)
	ON_BN_CLICKED(IDC_RCPSC_BTN, OnRcpscBtn)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DGN_PROPERTY_LIST, OnItemchangedMatlList)
	ON_NOTIFY(NM_CLICK, IDC_DGN_PROPERTY_LIST, OnClickMatlList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnLateralCapaOptionMatlChildDlg message handlers
BOOL CDgnLateralCapaOptionMatlChildDlg::OnInitDialog() 
{
	CDlgChild::OnInitDialog();
		
	InitCtrls();
	InitListCtrl();
	InitData();
			
	if(!Data2Dlg()) return FALSE;

	EnableCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnLateralCapaOptionMatlChildDlg::SetModifyData(T_MATL_K Key)
{
	m_nCurSelectedKey = Key; 
	m_bModify = TRUE;
}

void CDgnLateralCapaOptionMatlChildDlg::InitCtrls()
{
	
}

BOOL CDgnLateralCapaOptionMatlChildDlg::ExistMphg(T_MPHG_K MphgK)
{
	for (int i = 0; i < m_pDataAll->aMphgK.GetSize(); i++)
	{
		if(m_pDataAll->aMphgK[i] == MphgK)
		{
			return TRUE;
		}
	}
	return FALSE;
}


void CDgnLateralCapaOptionMatlChildDlg::InitListCtrl()
{
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_cPropertyListCtrl.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_cPropertyListCtrl.GetSafeHwnd(),dwStyle);

	LV_COLUMN lvcolumn;

	
	if(m_nType==D_RCPSC_TYPE || m_nType==D_SRC_TYPE)
	{		
		TCHAR *list[9] = {_T("ID"),
						(TCHAR*)(LPCTSTR)_LS(IDS_CMD_MPHI_NAME),
						(TCHAR*)(LPCTSTR)_LS(IDS_CMD_MPHI_C_GRADE),
						(TCHAR*)(LPCTSTR)_LS(IDS_CMD_MPHI_EC),
						(TCHAR*)(LPCTSTR)_LS(IDS_CMD_MPHI_SIG_CK),
						(TCHAR*)(LPCTSTR)_LS(IDS_CMD_MPHI_S_GRADE),
						(TCHAR*)(LPCTSTR)_LS(IDS_CMD_MPHI_ES),
						(TCHAR*)(LPCTSTR)_LS(IDS_CMD_MPHI_SIG_SY),
						(TCHAR*)(LPCTSTR)_LS(IDS_CMD_MPHI_USE)};

		int width[9]	= {34,80,63,65,65,63,65,65,34};		
		for(int i=0; i<9; i++)
		{
			lvcolumn.mask     = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
			lvcolumn.fmt      = LVCFMT_CENTER;
			lvcolumn.pszText  = list[i];
			lvcolumn.iSubItem = i;
			lvcolumn.cx       = globalUtils.ScaleByDPI(width[i]);
			m_cPropertyListCtrl.InsertColumn(i,&lvcolumn);
		}
	}
	else
	{
		TCHAR *list[6] = {_T("ID"),
						(TCHAR*)(LPCTSTR)_LS(IDS_CMD_MPHI_NAME),
						(TCHAR*)(LPCTSTR)_LS(IDS_CMD_MPHI_GRADE),
						(TCHAR*)(LPCTSTR)_LS(IDS_CMD_MPHI_ES),
						(TCHAR*)(LPCTSTR)_LS(IDS_CMD_MPHI_SIG_SY),
						(TCHAR*)(LPCTSTR)_LS(IDS_CMD_MPHI_USE)};

		int width[6]	= {40,154,110,95,95,40};
		for(int i=0; i<6; i++)
		{
			lvcolumn.mask     = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
			lvcolumn.fmt      = LVCFMT_CENTER;
			lvcolumn.pszText  = list[i];
			lvcolumn.iSubItem = i;
			lvcolumn.cx       = globalUtils.ScaleByDPI(width[i]);
			m_cPropertyListCtrl.InsertColumn(i,&lvcolumn);
		}
	}
}


void CDgnLateralCapaOptionMatlChildDlg::InitData()
{
	m_cPropertyListCtrl.DeleteAllItems();
	CArray<T_MATL_K,T_MATL_K> KeyList;
	m_pDoc->m_pAttrCtrl->GetMatlKeyList(KeyList);
	int iCount = KeyList.GetSize();

	int ListLineNo = 0;
	for(int i=0; i<iCount; i++)
	{
		T_MATD_D mData;
		mData.Initialize();
		int nID = KeyList.GetAt(i);
		BOOL bCheck = m_pDoc->m_pAttrCtrl->GetMatlDesign(nID,mData);
		if(bCheck)
		{
			if(m_nType==D_RCPSC_TYPE && mData.Type == _T("C"))
			{
				DgnListData data; data.initialize();
				data.nId = nID;
				data.cName = mData.Name;
				data.cGradeName = mData.Data1.CodeMatlName;
				data.dElast = mData.Data1.Analysis.Elast;
				data.dSig = mData.Data1.Design.C_fc;
				data.cSGradeName = mData.MainRebar_RebarName;
				data.dSElast = mData.MainRebarData.B_Elast;
				// 20080514 mylee - MPhi 이고 Rebar Code를 None 선택시 Preference 코드를 사용하여 계산한다.
				// 내부 계산은 다른 곳에서 되고 있으며, 여기서는 리스트에 Es 값을 보여주기 위해서 처리. (이진우)
				if(mData.Rebar_CodeName==_T("None"))
					GetEsFromPreference(data.dSElast);
				data.dSigSy = mData.MainRebarData.B_fy;								
				data.bUse = ExistMphg(nID);
				SetListCtrl(ListLineNo, data);				        
				ListLineNo++;
			}
			else if(m_nType==D_STEEL_TYPE && mData.Type == _T("S"))
			{
				DgnListData data; data.initialize();
				data.nId = nID;
				data.cName = mData.Name;
				data.cGradeName = mData.Data1.CodeMatlName;
				data.dElast = mData.Data1.Analysis.Elast;
				data.dSig = mData.Data1.Design.S_Fy1;			
				data.bUse = ExistMphg(nID);
				SetListCtrl(ListLineNo, data);				        
				ListLineNo++;
			}
			else if(m_nType==D_SRC_TYPE && mData.Type == _T("SRC"))
			{
				DgnListData data; data.initialize();
				data.nId = nID;
				data.cName = mData.Name;
				data.cGradeName = mData.Data2.CodeMatlName;
				data.dElast = mData.Data2.Analysis.Elast;
				data.dSig = mData.Data2.Design.C_fc;
				data.cSGradeName = mData.Data1.CodeMatlName;
				data.dSElast = mData.Data1.Analysis.Elast;
				data.dSigSy = mData.Data1.Design.S_Fy1;								
				data.bUse = ExistMphg(nID);
				SetListCtrl(ListLineNo, data);				        
				ListLineNo++;
			}
		}
	}
}

// Main Rebar Code 가 None 일때만 불리는 함수
void CDgnLateralCapaOptionMatlChildDlg::GetEsFromPreference(double& dEs)
{
	// 1. Preference에 지정된 Default 철근기준과 재질기준을 받아옵니다.
	CString strRebarCode=_T(""), strRebarName=_T("");
	CDBLib::GetDefaultConRbar(strRebarCode, strRebarName);

	// 2. 철근기준과 재질기준을 이용(1에서 받아온)하여 철근정보를 받아옵니다.
	T_MATL_REBAR RbarD;
	if(!m_pDoc->m_pMatlDB->GetRebarData(strRebarCode,strRebarName,RbarD))          RbarD.Initialize();
	
	// 3. 설계용 재질정보와 철근정보를 이용하여 주철근강도를 반환합니다.
	dEs = RbarD.B_Elast;
}

void CDgnLateralCapaOptionMatlChildDlg::SetListCtrl(int ListLineNo, DgnListData data)
{
	LV_ITEM lvitem;
	if(m_nType==D_RCPSC_TYPE || m_nType==D_SRC_TYPE)
	{
		LPTSTR szText[9];
		CString tempS;
		tempS.Format(_T("%5d"), data.nId);
		szText[0] = (LPTSTR)(LPCTSTR)tempS;
		szText[1] = (LPTSTR)(LPCTSTR)data.cName;
		szText[2] = (LPTSTR)(LPCTSTR)data.cGradeName;
		CString tempS2;
		tempS2.Format(_T("%.2e"), data.dElast);
		szText[3] = (LPTSTR)(LPCTSTR)tempS2;
		CString tempS3;
		tempS3.Format(_T("%.2e"), data.dSig);
		szText[4] = (LPTSTR)(LPCTSTR)tempS3;
		szText[5] = (LPTSTR)(LPCTSTR)data.cSGradeName;
		CString tempS4;
		tempS4.Format(_T("%.2e"), data.dSElast);
		szText[6] = (LPTSTR)(LPCTSTR)tempS4;
		CString tempS5;
		tempS5.Format(_T("%.2e"), data.dSigSy);
		szText[7] = (LPTSTR)(LPCTSTR)tempS5;
		CString tempS6;
		tempS6 = data.bUse ? _T("O") : _T("X");
		szText[8] = (LPTSTR)(LPCTSTR)tempS6;
		// Input ListCtrl Box.		
		for(int i=0; i<9; i++)
		{				
			lvitem.iItem  	= ListLineNo;
			lvitem.iSubItem = i;
			lvitem.pszText	= szText[i];			
			lvitem.mask		  = LVIF_TEXT;
			if( i == 0 ) 
			{
				lvitem.mask |= LVIF_PARAM;
				lvitem.lParam = (LPARAM)data.nId;
				m_cPropertyListCtrl.InsertItem(&lvitem);
			}
			else m_cPropertyListCtrl.SetItem(&lvitem);
		}
	}
	else if(m_nType==D_STEEL_TYPE)
	{
		LPTSTR szText[6];
		CString tempS;
		tempS.Format(_T("%5d"), data.nId);
		szText[0] = (LPTSTR)(LPCTSTR)tempS;
		szText[1] = (LPTSTR)(LPCTSTR)data.cName;
		szText[2] = (LPTSTR)(LPCTSTR)data.cGradeName;
		CString tempS2;
		tempS2.Format(_T("%.2e"), data.dElast);
		szText[3] = (LPTSTR)(LPCTSTR)tempS2;
		CString tempS3;
		tempS3.Format(_T("%.2e"), data.dSig);
		szText[4] = (LPTSTR)(LPCTSTR)tempS3;		
		CString tempS4;
		tempS4 = data.bUse ? _T("O") : _T("X");
		szText[5] = (LPTSTR)(LPCTSTR)tempS4;
		// Input ListCtrl Box.		
		for(int i=0; i<6; i++)
		{			
			lvitem.iItem  	= ListLineNo;
			lvitem.iSubItem = i;
			lvitem.pszText	= szText[i];
			lvitem.mask		  = LVIF_TEXT;			
			if( i == 0 ) 
			{
				lvitem.mask |= LVIF_PARAM;
				lvitem.lParam = (LPARAM)data.nId;
				m_cPropertyListCtrl.InsertItem(&lvitem);
			}
			else m_cPropertyListCtrl.SetItem(&lvitem);
		}
	}
}

void CDgnLateralCapaOptionMatlChildDlg::OnClickMatlList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnItemchangedMatlList(pNMHDR,pResult);
	*pResult = 0;
}

void CDgnLateralCapaOptionMatlChildDlg::OnItemchangedMatlList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	int nItem = pNMListView->iItem;
	CString strID = m_cPropertyListCtrl.GetItemText(nItem,0);	
	if(strID != _T(""))
		m_nCurSelectedKey = _ttoi(strID);		
		
	if(!Data2Dlg()) return;		
	
	EnableCtrl();	

	*pResult = 0;
}

void CDgnLateralCapaOptionMatlChildDlg::OnAddReplaceBtn() 
{
	T_MATL_D MatlData;
	if(!m_pDoc->m_pAttrCtrl->GetMatl(m_nCurSelectedKey, MatlData))
	{
		AfxMessageBox(_LS(IDS_CMD_IMPORT_DXF_No_Selcted_Item));
		return;
	} 

	if(!Apply()) return;
	InitData();
}

void CDgnLateralCapaOptionMatlChildDlg::OnDelBtn() 
{
	T_MATL_D MatlData;
	if(!m_pDoc->m_pAttrCtrl->GetMatl(m_nCurSelectedKey, MatlData))
	{
		AfxMessageBox(_LS(IDS_CMD_IMPORT_DXF_No_Selcted_Item));
		return;
	} 

	ASSERT(m_pDataAll->aMphgK.GetSize() == m_pDataAll->aMphgD.GetSize());
	int nFindIdx = -1;
	for (int i = 0; i < m_pDataAll->aMphgK.GetSize(); i++)
	{
		if(m_pDataAll->aMphgK[i] == m_nCurSelectedKey)
		{
			nFindIdx = i;
			break;
		}
	}
	if(nFindIdx == -1)
	{
		ASSERT(0); // 뭔가 꼬였음..
		return ; 
	}
	m_pDataAll->aMphgK.RemoveAt(nFindIdx);
	//if(!m_pDoc->m_pDataCtrl->DelMphg(m_nCurSelectedKey)) return ;
	InitData();
}


void CDgnLateralCapaOptionMatlChildDlg::OnRcpscBtn() 
{
	// TODO: Add your control notification handler code here
	 // RC menu쪽 호출(PSC쪽은 PSC Bridge Option을 구매하지 않으면 Disable됨)
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DGN_CON_MATD, 0)); 
}

void CDgnLateralCapaOptionMatlChildDlg::OnOK()
{
	return;
}

void CDgnLateralCapaOptionMatlChildDlg::OnCancel() 
{
	return;
}

BOOL CDgnLateralCapaOptionMatlChildDlg::GetSelectedItem(int &nIndex)
{
	int iItem = m_cPropertyListCtrl.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;

	nIndex = iItem;
	return TRUE;
}

void CDgnLateralCapaOptionMatlChildDlg::EnableCtrl()
{
	UpdateData();

	int nCode = m_pDataAll->MpgbD.nCode;  // 0:_T("콘크리트표준시방서"), 1:_T("도시V (평성14년)"), 2:_T("도시III (평성14년, σc,σck)"), 3:_T("도시III (평성14년, σc,0.85σck)"), 4:_T("도시V (평성24년)")

	GetDlgItem(IDC_CONSIDER_RDO)->EnableWindow(m_nType==D_RCPSC_TYPE && (nCode == 0 || nCode == 1 || nCode == 4));
	GetDlgItem(IDC_DONT_CONSIDER_RDO)->EnableWindow(m_nType==D_RCPSC_TYPE && (nCode == 0 || nCode == 1 || nCode == 4));
	GetDlgItem(IDC_RCPSC_BTN)->EnableWindow(m_nType==D_RCPSC_TYPE);
}

/////////////////////////////////////////////////////////////////////
// Data Control.....
BOOL CDgnLateralCapaOptionMatlChildDlg::Apply()
{
	if(!Dlg2Data()) return FALSE;
	if(!m_pDoc->m_pEditData->CheckMphg(m_nCurSelectedKey, m_Data)) return FALSE;

	// 일단 m_pDataAll에 쌓아 놓음
	ASSERT(m_pDataAll->aMphgK.GetSize() == m_pDataAll->aMphgD.GetSize());
	int nFindIdx = -1;
	for (int i = 0; i < m_pDataAll->aMphgK.GetSize(); i++)
	{
		if(m_pDataAll->aMphgK[i] == m_nCurSelectedKey)
		{
			nFindIdx = i;
			break;
		}
	}
	if(nFindIdx == -1)
	{
		m_pDataAll->aMphgK.Add(m_nCurSelectedKey);
		m_pDataAll->aMphgD.Add(m_Data);
	}
	else
	{
		ASSERT(m_pDataAll->aMphgK[nFindIdx] == m_nCurSelectedKey);    
		m_pDataAll->aMphgD[nFindIdx] = m_Data;
	}

	T_MATD_D MatdD;
	BOOL bCheck = m_pDoc->m_pAttrCtrl->GetMatlDesign(m_nCurSelectedKey,MatdD);
	ModifyItem(m_nCurSelectedKey, m_nCurSelectedKey, MatdD);
	//
	//if(!m_pDoc->m_pDataCtrl->AddMphg(m_nCurSelectedKey, m_Data)) return FALSE;

	return TRUE;
}


BOOL CDgnLateralCapaOptionMatlChildDlg::Data2Dlg()
{		
	int nFindIdx = -1;
	for (int i = 0; i < m_pDataAll->aMphgK.GetSize(); i++)
	{
		if(m_pDataAll->aMphgK[i] == m_nCurSelectedKey)
		{
			nFindIdx = i;
			break;
		}
	}

	if(nFindIdx == -1)
	{
		m_Data.Initialize();
		if(m_nType!=D_RCPSC_TYPE)
		{
			m_Data.nBrokenLine1 = 1; // Steel, SRC일 때는 기본값이 다름
		}
	}
	else
	{
		m_Data = m_pDataAll->aMphgD[nFindIdx];
	}
	
	m_nLimitState = m_Data.nLimitState;
	m_nConsider = m_Data.nConsiderSigbt;
	
	UpdateData(FALSE);

	if(m_bModify) 
	{
		CDlgUtil::SelectListItemData(&m_cPropertyListCtrl, m_nCurSelectedKey);
		m_bModify = FALSE;
	}

	return TRUE;
}

BOOL CDgnLateralCapaOptionMatlChildDlg::Dlg2Data()
{
	UpdateData();
	if(!m_pDoc->m_pAttrCtrl->GetMphg(m_nCurSelectedKey, m_Data)) m_Data.Initialize();
	
	m_Data.nConsiderSigbt = m_nConsider;
	m_Data.nLimitState = m_nLimitState;

	return TRUE;
}

/////////////////////////////////////////
// modify list item data.....
BOOL CDgnLateralCapaOptionMatlChildDlg::ModifyItem(T_MATL_K KeyOld, T_MATL_K Key, T_MATD_D &mData)
{
	DgnListData data; data.initialize();

	if(m_nType==D_RCPSC_TYPE && mData.Type == _T("C"))
	{		
		data.nId = (int)Key;
		data.cName = mData.Name;
		data.cGradeName = mData.Data1.CodeMatlName;
		data.dElast = mData.Data1.Analysis.Elast;
		data.dSig = mData.Data1.Design.C_fc;
		data.cSGradeName = mData.MainRebar_RebarName;
		data.dSElast = mData.MainRebarData.B_Elast;
		// 20080514 mylee - MPhi 이고 Rebar Code를 None 선택시 Preference 코드를 사용하여 계산한다.
		// 내부 계산은 다른 곳에서 되고 있으며, 여기서는 리스트에 Es 값을 보여주기 위해서 처리. (이진우)
		if(mData.Rebar_CodeName==_T("None"))
			GetEsFromPreference(data.dSElast);
		data.dSigSy = mData.MainRebarData.B_fy;								
		data.bUse = ExistMphg(Key);
	}
	else if(m_nType==D_STEEL_TYPE && mData.Type == _T("S"))
	{
		data.nId = (int)Key;
		data.cName = mData.Name;
		data.cGradeName = mData.Data1.CodeMatlName;
		data.dElast = mData.Data1.Analysis.Elast;
		data.dSig = mData.Data1.Design.S_Fy1;			
		data.bUse = ExistMphg(Key);
	}
	else if(m_nType==D_SRC_TYPE && mData.Type == _T("SRC"))
	{
		data.nId = (int)Key;
		data.cName = mData.Name;
		data.cGradeName = mData.Data2.CodeMatlName;
		data.dElast = mData.Data2.Analysis.Elast;
		data.dSig = mData.Data2.Design.C_fc;
		data.cSGradeName = mData.Data1.CodeMatlName;
		data.dSElast = mData.Data1.Analysis.Elast;
		data.dSigSy = mData.Data1.Design.S_Fy1;								
		data.bUse = ExistMphg(Key);
	}
	
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_cPropertyListCtrl.FindItem(&FindInfo);

	if (nItem != -1)
	{
		CString str;
		if(m_nType==D_RCPSC_TYPE || m_nType==D_SRC_TYPE)
		{
			for(int i = 0; i < 9; i++)
			{
				str = DataToStr(i, (int)Key, data);
				m_cPropertyListCtrl.SetItemText(nItem, i, str);
			}
		}
		else if(m_nType==D_STEEL_TYPE)
		{
			int index = 0;
			for(int i = 0; i < 9; i++)
			{
				if(i<5 || i>7)
				{
					str = DataToStr(i, (int)Key, data);
					m_cPropertyListCtrl.SetItemText(nItem, index, str);
					index++;
				}
			}
		}
		// key 재 설정 
		LVITEM lvitem;
		lvitem.iItem = nItem;
		lvitem.iSubItem=0;
		lvitem.mask = LVIF_PARAM;
		lvitem.lParam = (LPARAM)Key;
		m_cPropertyListCtrl.SetItem(&lvitem);
	}
	return TRUE;
}

CString CDgnLateralCapaOptionMatlChildDlg::DataToStr(int i, int Key, DgnListData &Data)
{
	CString str;

	if (i==0) str.Format(_T("%5d"), Data.nId);
	else if (i==1) str = Data.cName;
	else if (i==2) str = Data.cGradeName;
	else if (i==3) str.Format(_T("%.2e"), Data.dElast);
	else if (i==4) str.Format(_T("%.2e"), Data.dSig);	
	else if (i==5) str = Data.cSGradeName;
	else if (i==6) str.Format(_T("%.2e"), Data.dSElast);
	else if (i==7) str.Format(_T("%.2e"), Data.dSigSy);
	else if (i==8) str = Data.bUse ? _T("O") : _T("X");	
	else str = _LS(IDS_WG_CMD__ADDD__Error);

	return str;
}

void CDgnLateralCapaOptionMatlChildDlg::UpdateList(LPARAM lHint, CObject* pHint)
{
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
	case D_UPDATE_EXE_START:
	case D_UPDATE_EXE_END:
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}


void CDgnLateralCapaOptionMatlChildDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_MATL_K Key, KeyBak;
	T_MATD_D Data, DataBak;

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_MATD_ADD):
				// 삽입될 위치(nRow)를 찾아 넣는다.
				pViewBuff->GetMatd(nKey, Key, Data);
				//InsertItem(Key, Data);
				break;
		case(UR_MATD_DEL):
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetMatd(nKey, Key, Data);
				//DeleteItem(Key, Data);
				break;
		case(UR_MATD_MFD):
				// 여기서는 저장만 하고 UR_MATD_MFS에서 처리한다.
				pViewBuff->GetMatd(nKey, Key, Data);
				KeyBak = Key;
				DataBak = Data;
				bMFD = TRUE;
				break;
		case(UR_MATD_MFS):
				// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
				ASSERT(bMFD);
				pViewBuff->GetMatd(nKey, Key, Data);
				ModifyItem(Key, KeyBak, DataBak);
				// 플래그 수정
				bMFD = FALSE;
				break;
		default:
			break;
		}
	} // end of while
}
