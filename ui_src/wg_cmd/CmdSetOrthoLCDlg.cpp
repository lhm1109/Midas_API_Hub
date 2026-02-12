// CCmdSetOrthoLCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdSetOrthoLCDlg.h"
#include "LoadCombCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_base\wg_base_LocaleLib.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_main\wg_mainres2.h"	

#include "..\wg_base\wg_base_CompFunc.h"

#include "..\wg_db\wg_db_LoadDgnCodeCheck.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCmdSetOrthoLCDlg dialog
enum LoadCaseType
{
	ST = 0,  // 0: Static
	RS,      // 1: Response Spectrum
	NONE     // 2: None
};

enum LoadCaseDirection
{
	LC_1 = 0,  // 0: LC1 (X)
	LC_2,      // 1: LC2 (Y)
	LC_3       // 2: LC3 (Z)
};

CCmdSetOrthoLCDlg::CCmdSetOrthoLCDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCmdSetOrthoLCDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCmdSetOrthoLCDlg)
	//}}AFX_DATA_INIT
	m_bSeismicEvaluation = FALSE;
	m_bUser = FALSE;
	m_bUseLoadCaseZCombo = FALSE;
	m_bShowLoadCaseZ = FALSE;
}

void CCmdSetOrthoLCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCmdSetOrthoLCDlg)
	DDX_Control(pDX, IDC_ETC_LOADCASEX, m_LoadCaseX);
	DDX_Control(pDX, IDC_ETC_LOADCASEY, m_LoadCaseY);
	DDX_Control(pDX, IDC_ETC_LOADCASEZ, m_LoadCaseZ);
	DDX_Control(pDX, IDC_ETC_OLGROUP_LIST, m_ListCtrl);
	//}}AFX_DATA_MAP
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CCmdSetOrthoLCDlg, CDialog)
#undef CDialog

	//{{AFX_MSG_MAP(CCmdSetOrthoLCDlg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_ETC_OLGROUP_LIST, OnChangedCurrentItem) //OLG = Orthogonal Loads Group
	ON_BN_CLICKED(IDC_ETC_LOADCASE_BTN, OnClickLoadCaseBtn)
	ON_BN_CLICKED(IDC_ETC_BTN_ADD, OnCmdOLGAdd)
	ON_BN_CLICKED(IDC_ETC_BTN_MODIFY, OnCmdOLGModify)
	ON_BN_CLICKED(IDC_ETC_BTN_DELETE, OnCmdOLGDelete)
	ON_BN_CLICKED(IDC_ETC_AUTOGROUP_BTN, OnCmdOLGAuto)
	
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCmdSetOrthoLCDlg message handlers

void CCmdSetOrthoLCDlg::SetListCtrlHeader()
{
	CString aTitle[] = {  _LS(IDS_CMD_Orthgonal_Effect_Group_No), 
						  _LS(IDS_CMD_Orthgonal_Effect_Load_Case_1), 
						  _LS(IDS_CMD_Orthgonal_Effect_Load_Case_2),
						  _LS(IDS_CMD_Orthgonal_Effect_Load_Case_3) };

	int nColWidth[] = {70, 60, 60, 60};
	int nColNum = m_bShowLoadCaseZ ? 4 : 3;
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

BOOL CCmdSetOrthoLCDlg::GetSelectedItem(int &nIndex)
{
	int iItem = m_ListCtrl.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;

	nIndex = iItem;
	return TRUE;
}

void CCmdSetOrthoLCDlg::MakeItemEx()
{
	m_ListCtrl.DeleteAllItems();

	int nItemCount = m_Data.aLoadCombination.GetSize(); 
	if(nItemCount == 0) return;

	for (int nCount = 0; nCount < nItemCount; nCount++)
	{
		m_ListCtrl.InsertItem(nCount, _T(""));
		/*// Coded by Seungjun MNet:No.2516 ('20061027)
		m_strLCNameX = DataToStr(1, m_Data.aLoadCombination.GetAt(nCount));
		m_strLCNameY = DataToStr(2, m_Data.aLoadCombination.GetAt(nCount));
		
z(m_Data.aLoadCombination.GetAt(nCount)); */
		SetItem(nCount, m_Data.aLoadCombination.GetAt(nCount));
	}
	m_ListCtrl.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);  
}

CString CCmdSetOrthoLCDlg::DataToStr(int i, T_LCOE_BASE &data)
{
	CString value = _T("");
	if (i == 3 && (data.bStldKdZ == FALSE || m_bUseLoadCaseZCombo == FALSE))
	{
		value = _T("None");
		return value;
	}
	if(data.nLoadCaseType==0)	// 0=Static
	{
		T_STLD_K key;
		if     (i==1) key = data.StldKdX;
		else if(i==2) key = data.StldKdY;
		else if(i==3) key = data.StldKdZ;
		
		T_STLD_D rData;
		m_pDoc->m_pAttrCtrl->GetStld(key, rData);
		value = rData.LoadCaseName+_T("(ST)");
	}
	else if(data.nLoadCaseType==1) // 1=Response Spectrum
	{
		T_SPLC_K key;
		if     (i==1) key = data.StldKdX;
		else if(i==2) key = data.StldKdY;		
		else if(i==3) key = data.StldKdZ;
		
		T_SPLC_D rData;
		m_pDoc->m_pAttrCtrl->GetSplc(key, rData);
		value = rData.LoadCaseName+_T("(RS)");
	}

	return value;
}

void CCmdSetOrthoLCDlg::SetItem(int nIndex, T_LCOE_BASE &data)
{
	LVITEM lvitem;
	CString str;
	CString strIndex;
	int nColNum = m_bShowLoadCaseZ ? 4 : 3;
	for(int i = 0; i < nColNum; i++)
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

BOOL CCmdSetOrthoLCDlg::ValidItem(T_LCOE_BASE& data)
{
	// Coded by Seungjun MNet:No.2593 ('20061128) Bug Fix.
	if ( m_bUseLoadCaseZCombo && data.bStldKdZ )
	{
		if ( data.StldKdZ <= 0 )
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Invalid_loadcase_key_));
			return FALSE;
		}

		if ( data.nLoadCaseType == 0 )
		{
			T_STLD_D StldD;
			if ( !m_pDoc->m_pAttrCtrl->GetStld(data.StldKdX, StldD) )
			{
				ASSERT(0); return FALSE;
			}
			if ( StldD.LoadCaseType == _T("EVT") ) // Z
			{
				AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Invalid_loadcase_key_));
				return FALSE;
			}
			StldD.Initialize();
			if ( !m_pDoc->m_pAttrCtrl->GetStld(data.StldKdY, StldD) )
			{
				ASSERT(0); return FALSE;
			}
			if ( StldD.LoadCaseType == _T("EVT") ) // Z
			{
				AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Invalid_loadcase_key_));
				return FALSE;
			}
		}
		else
		{
			T_SPLC_D SplcD;
			if ( !m_pDoc->m_pAttrCtrl->GetSplc(data.StldKdX, SplcD) )
			{
				ASSERT(0); return FALSE;
			}
			if ( SplcD.nDirection == 2 ) // Z
			{
				AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Invalid_loadcase_key_));
				return FALSE;
			}
			SplcD.Initialize();
			if ( !m_pDoc->m_pAttrCtrl->GetSplc(data.StldKdY, SplcD) )
			{
				ASSERT(0); return FALSE;
			}
			if ( SplcD.nDirection == 2 ) // Z
			{
				AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Invalid_loadcase_key_));
				return FALSE;
			}
		}
	}

	if(data.nLoadCaseType)  return TRUE;  // Response Spectrum is always TRUE.

	if (data.StldKdX <= 0 || data.StldKdY <= 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Invalid_loadcase_key_));
		return FALSE;
	}

	// Coded by Seungjun MNet:No.2516 ('20061027)
	BOOL bStldKdX = FALSE;
	BOOL bStldKdY = FALSE;
	for(int i=0; i<m_arEQKeyList.GetSize(); i++)
	{
		if(data.StldKdX == m_arEQKeyList.GetAt(i))   bStldKdX = TRUE;
		if(data.StldKdY == m_arEQKeyList.GetAt(i))   bStldKdY = TRUE;
	}
	
	if(!bStldKdX && !bStldKdY)
	{
		CString strErrorMsg=_T("");
		CString strErrorCase=_T("");
		strErrorCase.Format(_T("%s and %s"),m_strLCNameX,m_strLCNameY);
		strErrorMsg.Format(_LS(IDS_WG_CMD__ADDD__Invalid_loadcase_key_EQ),strErrorCase);
		AfxMessageBox(strErrorMsg, MB_OK);
		return FALSE;
	}
	else if(!bStldKdX && bStldKdY)
	{
		CString strErrorMsg=_T("");
		strErrorMsg.Format(_LS(IDS_WG_CMD__ADDD__Invalid_loadcase_key_EQ),m_strLCNameX);
		AfxMessageBox(strErrorMsg, MB_OK);
		return FALSE;
	}
	else if(bStldKdX && !bStldKdY)
	{
		CString strErrorMsg=_T("");
		strErrorMsg.Format(_LS(IDS_WG_CMD__ADDD__Invalid_loadcase_key_EQ),m_strLCNameY);
		AfxMessageBox(strErrorMsg, MB_OK);
		return FALSE;
	}

	return TRUE;
}

// Coded by Seungjun MNet:No.2516 ('20061027)
BOOL CCmdSetOrthoLCDlg::ValidItem(T_LCOE_BASE& data, CString& strErrorMsg)
{  
	// Coded by Seungjun MNet:No.2593 ('20061128) Bug Fix.
	if(data.nLoadCaseType)  return TRUE;  // Response Spectrum is always TRUE.

	BOOL bStldKdX = FALSE;
	BOOL bStldKdY = FALSE;
	for(int i=0; i<m_arEQKeyList.GetSize(); i++)
	{
		if(data.StldKdX == m_arEQKeyList.GetAt(i))   bStldKdX = TRUE;
		if(data.StldKdY == m_arEQKeyList.GetAt(i))   bStldKdY = TRUE;
	}
	if(!bStldKdX && !bStldKdY)
	{
		CString strErrorCase=_T("");
		strErrorCase.Format(_T("%s and %s of case (%s,%s)"),m_strLCNameX,m_strLCNameY,m_strLCNameX,m_strLCNameY);
		strErrorMsg.Format(_LS(IDS_WG_CMD__ADDD__Invalid_loadcase_key_EQ),strErrorCase);
		return FALSE;
	}
	else if(!bStldKdX && bStldKdY)
	{
		CString strErrorCase=_T("");
		strErrorCase.Format(_T("%s of case (%s,%s)"),m_strLCNameX,m_strLCNameX,m_strLCNameY);
		strErrorMsg.Format(_LS(IDS_WG_CMD__ADDD__Invalid_loadcase_key_EQ),strErrorCase);
		return FALSE;
	}
	else if(bStldKdX && !bStldKdY)
	{
		CString strErrorCase=_T("");
		strErrorCase.Format(_T("%s of case (%s,%s)"),m_strLCNameY,m_strLCNameX,m_strLCNameY);
		strErrorMsg.Format(_LS(IDS_WG_CMD__ADDD__Invalid_loadcase_key_EQ),strErrorCase);
		return FALSE;
	}

	return TRUE;
}

BOOL CCmdSetOrthoLCDlg::Dlg2Item(T_LCOE_BASE &data)
{
	 
	int iLoadCaseTypeX, iLoadCaseTypeY, iLoadCaseTypeZ;
	int IndexX = m_LoadCaseX.GetCurSel();
	int IndexY = m_LoadCaseY.GetCurSel();
	int IndexZ = m_LoadCaseZ.GetCurSel();

	auto lambda_RSTrimRight = [](CString& sLCName)
	{
		sLCName.TrimRight(_T(")"));
		sLCName.TrimRight(_T("S"));
		sLCName.TrimRight(_T("R"));
		sLCName.TrimRight(_T("("));
	};

	auto lambda_STTrimRight = [](CString& sLCName)
	{
		sLCName.TrimRight(_T(")"));
		sLCName.TrimRight(_T("T"));
		sLCName.TrimRight(_T("S"));
		sLCName.TrimRight(_T("("));
	};

	bool isIndexXValid = IndexX != -1;
	bool isIndexYValid = IndexY != -1;
	bool isIndexZValid = IndexZ != -1 && m_bUseLoadCaseZCombo;
	bool isLoadCaseZComboNotUsed = !m_bUseLoadCaseZCombo;

	if(isIndexXValid && isIndexYValid && (isIndexZValid || isLoadCaseZComboNotUsed))
	{
		CString strName=_T("");
		CString strLCNameX =_T(""), strLCNameY = _T(""), strLCNameZ = _T("");
		auto L_getLoadCaseText = [](auto& m_LoadCase, int index, auto& m_strLCName, auto& strLCName) {
			CString strName = _T("");
			if(index != -1) m_LoadCase.GetLBText(index, strName);
			m_strLCName = strName;
			strLCName = strName;
		};

		L_getLoadCaseText(m_LoadCaseX, IndexX, m_strLCNameX, strLCNameX);
		L_getLoadCaseText(m_LoadCaseY, IndexY, m_strLCNameY, strLCNameY);
		L_getLoadCaseText(m_LoadCaseZ, IndexZ, m_strLCNameZ, strLCNameZ);

		int iX=0, iY=1, iZ = 2;
		iLoadCaseTypeX = GetLoadCaseType(iX, IndexX);
		iLoadCaseTypeY = GetLoadCaseType(iY, IndexY);
		iLoadCaseTypeZ = GetLoadCaseType(iZ, IndexZ);

		if(iLoadCaseTypeX != iLoadCaseTypeY || (iLoadCaseTypeY != iLoadCaseTypeZ && m_bUseLoadCaseZCombo && iLoadCaseTypeZ!=NONE) )
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Types_of_three_loadcases_are_different_));
			return FALSE;  
		}
		if (iLoadCaseTypeZ != NONE)
		{
			if (iLoadCaseTypeX != iLoadCaseTypeZ)
			{
				AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Types_of_three_loadcases_are_different_));
				return FALSE;
			}
		}
		int iLoadCaseType = iLoadCaseTypeX;

		int nKeyX = 0, nKeyY = 0, nKeyZ = 0;
		if(iLoadCaseType == ST)
		{
			lambda_STTrimRight(strLCNameX);
			lambda_STTrimRight(strLCNameY);

			nKeyX = m_pDoc->m_pAttrCtrl->GetStldKey(strLCNameX);
			nKeyY = m_pDoc->m_pAttrCtrl->GetStldKey(strLCNameY);
		}
		else if(iLoadCaseType == RS)
		{
			lambda_RSTrimRight(strLCNameX);
			lambda_RSTrimRight(strLCNameY);

			nKeyX = m_pDoc->m_pAttrCtrl->GetSplcKey(strLCNameX);
			nKeyY = m_pDoc->m_pAttrCtrl->GetSplcKey(strLCNameY);
		}

		if (iLoadCaseTypeZ == RS)
		{
			lambda_RSTrimRight(strLCNameZ);
			nKeyZ = m_pDoc->m_pAttrCtrl->GetSplcKey(strLCNameZ);
			data.bStldKdZ = TRUE;
		}
		else if (iLoadCaseTypeZ == ST)
		{
			lambda_STTrimRight(strLCNameZ);
			nKeyZ = m_pDoc->m_pAttrCtrl->GetStldKey(strLCNameZ);
			data.bStldKdZ = TRUE;
		}
		else if (iLoadCaseTypeZ == NONE)
		{
			nKeyZ = 0;
			data.bStldKdZ = FALSE;
		}

		if (nKeyX == nKeyY || nKeyY == nKeyZ || nKeyZ == nKeyX)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__three_loadcases_are_identical_));
			return FALSE;
		}

		data.StldKdX = nKeyX;
		data.StldKdY = nKeyY;
		data.StldKdZ = nKeyZ;
		data.nLoadCaseType = iLoadCaseType;
	}
	else
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Loadcase_was_not_selected_));
		return FALSE;
	}

	return TRUE;
}

BOOL CCmdSetOrthoLCDlg::Item2Dlg(int nIndex, T_LCOE_BASE &data)
{
	SetLoadCaseCombo(data);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CCmdSetOrthoLCDlg message handlers

void CCmdSetOrthoLCDlg::Initial_LCaseComboBox()
{
	m_LoadCaseX.ResetContent();
	m_LoadCaseY.ResetContent();
	m_LoadCaseZ.ResetContent();
	m_arLCtype[0].RemoveAll();
	m_arLCtype[1].RemoveAll();
	m_arLCtype[2].RemoveAll();

	auto L_addLoadCase = [](T_SEIS_LCTP& LCType, auto& m_LoadCase, auto& m_arLCtype, int index, int nLCType) {
		LCType.index = m_LoadCase.GetCount() - 1;
		LCType.iLoadCaseType = nLCType; // 0: Static, 1: Response Spectrum, 2: None
		m_arLCtype[index].Add(LCType);
	};

	// None 추가
	m_LoadCaseZ.AddString(_T("None"));
	T_SEIS_LCTP LCTypeZ_none;
	L_addLoadCase(LCTypeZ_none, m_LoadCaseZ, m_arLCtype, 2, NONE); // 1=Y

	// Static
	CArray<T_STLD_K, T_STLD_K> arKeyList;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(arKeyList);
	int iCount = m_pDoc->m_pAttrCtrl->GetCountStld();
		
	if(iCount>0)
	{
		for(int i = 0; i < iCount; i++)
		{
			int iKey = arKeyList.GetAt(i);
			T_STLD_D rData;
			rData.Initialize();
			BOOL bCheck = m_pDoc->m_pAttrCtrl->GetStld(iKey, rData);
			ASSERT(bCheck);            

			CString strLoadCase = rData.LoadCaseName;
			CString strLoadType = rData.LoadCaseType;

			if(CDBLib::IsStldEL(strLoadType)) 
			{
				if (CProduct::GetMovingType() != D_PRODUCT_MOVING_CH || rData.LoadCaseType != _T("EVT"))
				{
					m_LoadCaseX.AddString(strLoadCase + _T("(ST)"));
					m_LoadCaseY.AddString(strLoadCase + _T("(ST)"));

					T_SEIS_LCTP LCTypeX;
					L_addLoadCase(LCTypeX, m_LoadCaseX, m_arLCtype, 0, ST); // 0=X

					T_SEIS_LCTP LCTypeY;
					L_addLoadCase(LCTypeY, m_LoadCaseY, m_arLCtype, 1, ST); // 1=Y
				}

				if ( m_bUseLoadCaseZCombo && rData.LoadCaseType == _T("EVT") )
				{
					m_LoadCaseZ.AddString(strLoadCase + _T("(ST)"));
					T_SEIS_LCTP LCTypeZ;
					L_addLoadCase(LCTypeZ, m_LoadCaseZ, m_arLCtype, 2, ST); // 2=Z
				}
			}
		}    
	}

	// Spectrum.
	CArray<T_SPLC_K, T_SPLC_K> KeyList;
	m_pDoc->m_pAttrCtrl->GetSplcKeyList(KeyList);
	iCount = KeyList.GetSize();  
	if(iCount > 0)
	{
		//  Spectrum Load Case Data가 있음.
		for(int i=0; i<iCount; i++)
		{
			T_SPLC_D rData;
			rData.Initialize();
			int key = KeyList.GetAt(i);

			BOOL bCheck = m_pDoc->m_pAttrCtrl->GetSplc(key,rData);
			ASSERT(bCheck);

			CString str = rData.LoadCaseName;
			if (CProduct::GetMovingType() != D_PRODUCT_MOVING_CH || rData.nDirection != 2)
			{
				m_LoadCaseX.AddString(str + _T("(RS)"));
				m_LoadCaseY.AddString(str + _T("(RS)"));
				
				T_SEIS_LCTP LCTypeX;
				L_addLoadCase(LCTypeX, m_LoadCaseX, m_arLCtype, 0, RS); // 0=X

				T_SEIS_LCTP LCTypeY;
				L_addLoadCase(LCTypeY, m_LoadCaseY, m_arLCtype, 1, RS); // 1=Y
			}

			if ( m_bUseLoadCaseZCombo && rData.nDirection == 2 ) // Z
			{
				m_LoadCaseZ.AddString(str + _T("(RS)"));
				T_SEIS_LCTP LCTypeZ;
				L_addLoadCase(LCTypeZ, m_LoadCaseZ, m_arLCtype, 2, RS); // 2=Z
			}
		}
//    Show_EditComboBox(TRUE);
	}
//  else Show_EditComboBox(FALSE);
	m_LoadCaseZ.SetCurSel(0);
	CDlgUtil::CobxAdjustListBoxWidth(m_LoadCaseX);  
	CDlgUtil::CobxAdjustListBoxWidth(m_LoadCaseY);  
	CDlgUtil::CobxAdjustListBoxWidth(m_LoadCaseZ);
}

BOOL CCmdSetOrthoLCDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();
	
	// TODO: Add your specialized code here and/or call the base class

	// Auto Grouping button 기능 다시 살림
	//GetDlgItem(IDC_ETC_AUTOGROUP_BTN)->ShowWindow(SW_SHOW);

	if (!m_bUser)
	{
		if (!m_pDoc->m_pAttrCtrl->GetLcoe(m_Data)) 
		{
			//GetDlgItem(IDC_CMD_REMOVE_DATA)->EnableWindow(FALSE);
			m_Data.Initialize();
		}
	}

	// Coded by Seungjun MNet:No.2516 ('20061027)
	GetUsedEQKeyList();

	// LoadCase ComboBox.
	Initial_LCaseComboBox();

	SetListCtrlHeader();
	MakeItemEx();

	m_LoadCaseZ.EnableWindow(m_bUseLoadCaseZCombo);

	int nShowLoadCaseZ = m_bShowLoadCaseZ ? SW_SHOW : SW_HIDE;
	GetDlgItem(IDC_WG_CMD_STATIC23)->ShowWindow(nShowLoadCaseZ);
	GetDlgItem(IDC_ETC_LOADCASEZ)->ShowWindow(nShowLoadCaseZ);
	GetDlgItem(IDC_ETC_AUTOGROUP_BTN)->ShowWindow(nShowLoadCaseZ);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCmdSetOrthoLCDlg::OnOK() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	// Coded by Seungjun MNet:No.2516 ('20061031)
	int iCount = m_Data.aLoadCombination.GetSize();
	CString strError = _T("");
	BOOL    IsAllValid = TRUE;
		
	for (int i = 0; i < iCount ; i++)
	{
		T_LCOE_BASE LcoeBase = m_Data.aLoadCombination.GetAt(i);
		m_strLCNameX = DataToStr(1, LcoeBase);
		m_strLCNameY = DataToStr(2, LcoeBase);
		m_strLCNameZ = DataToStr(3, LcoeBase);
		CString strErrorMsg=_T("");
		if (!ValidItem(LcoeBase,strErrorMsg))
		{
			IsAllValid = FALSE;
			strErrorMsg += _T("\n");
			strError    += strErrorMsg;
		}
	}
	if(!IsAllValid)
	{
		AfxMessageBox(strError, MB_OK);
		return;
	}
	//////////////////////////////////////////////////////////////
	if (!m_bUser)
	{
		if (m_pDoc->m_pDataCtrl->AddLcoe(m_Data))
		{
			CDialogMove::OnOK();
		}
	}
	else
	{
		LOAD_CASE_K CaseKeyX, CaseKeyY;
		for (int i=0; i<m_Data.aLoadCombination.GetCount(); i++)
		{
			CaseKeyX.Key = m_Data.aLoadCombination[i].StldKdX;
			CaseKeyX.nLoadCaseType = m_Data.aLoadCombination[i].nLoadCaseType== ST ? D_LCOM_STATIC : D_LCOM_SPECTRUM;
			CaseKeyY.Key = m_Data.aLoadCombination[i].StldKdY;
			CaseKeyY.nLoadCaseType = m_Data.aLoadCombination[i].nLoadCaseType== ST ? D_LCOM_STATIC : D_LCOM_SPECTRUM;
			m_mapBiEarthquake[CaseKeyX] = CaseKeyY;
		}
		CDialogMove::OnOK();
	}

}

void CCmdSetOrthoLCDlg::OnCancel() 
{
	// TODO: Add your control notification handler code here
	//CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);	
	if (m_bUser)
	{
		LOAD_CASE_K CaseKeyX, CaseKeyY;
		for (int i = 0; i < m_Data.aLoadCombination.GetCount(); i++)
		{
			CaseKeyX.Key = m_Data.aLoadCombination[i].StldKdX;
			CaseKeyX.nLoadCaseType = m_Data.aLoadCombination[i].nLoadCaseType == ST ? D_LCOM_STATIC : D_LCOM_SPECTRUM;
			CaseKeyY.Key = m_Data.aLoadCombination[i].StldKdY;
			CaseKeyY.nLoadCaseType = m_Data.aLoadCombination[i].nLoadCaseType == ST ? D_LCOM_STATIC : D_LCOM_SPECTRUM;
			m_mapBiEarthquake[CaseKeyX] = CaseKeyY;
		}
	}
	CDialogMove::OnCancel();
}

void CCmdSetOrthoLCDlg::OnClickLoadCaseBtn() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE, 0));	
}

void CCmdSetOrthoLCDlg::OnCmdOLGAdd() 
{
	// TODO: Add your control notification handler code here
	T_LCOE_BASE data;

	if (!Dlg2Item(data)) return;
	if (!ValidItem(data)) return;

	std::vector<unsigned int> aStldK = { data.StldKdX, data.StldKdY, data.StldKdZ };
	std::sort(aStldK.begin(), aStldK.end());

	for (int i = 0; i < m_Data.aLoadCombination.GetSize(); i++)
	{
		T_LCOE_BASE LcoeBase = m_Data.aLoadCombination.GetAt(i);
		/*
		if (LcoeBase.StldKdX == data.StldKdX && LcoeBase.StldKdY == data.StldKdY && data.nLoadCaseType==0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__An_identical_load_group_already_exists_));
			return;
		}
		else if (LcoeBase.StldKdX == data.StldKdX && LcoeBase.StldKdY == data.StldKdY && data.nLoadCaseType==1)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__An_identical_load_group_already_exists_));
			return;
		}
		else if (LcoeBase.StldKdX == data.StldKdY && LcoeBase.StldKdY == data.StldKdX && data.nLoadCaseType==0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__An_identical_load_group_already_exists_));
			return;
		}
		else if (LcoeBase.StldKdX == data.StldKdY && LcoeBase.StldKdY == data.StldKdX && data.nLoadCaseType==1)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__An_identical_load_group_already_exists_));
			return;
		}
		*/
		/*
		if (LcoeBase.StldKdX == data.StldKdX || LcoeBase.StldKdX == data.StldKdY ||
				LcoeBase.StldKdY == data.StldKdY || LcoeBase.StldKdY == data.StldKdX)// && data.nLoadCaseType==0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__An_identical_load_case_is_already_assigned_));
			return;
		}
		*/
		std::vector<unsigned int> aStldK_tgt = { LcoeBase.StldKdX, LcoeBase.StldKdY, LcoeBase.StldKdZ };
		std::sort(aStldK_tgt.begin(), aStldK_tgt.end());
		if(aStldK == aStldK_tgt)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__An_identical_load_case_is_already_assigned_));
			return;
		}
// 		else if(LcoeBase.StldKdX == data.StldKdY && LcoeBase.StldKdY == data.StldKdX)
// 		{
// 			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__An_identical_load_case_is_already_assigned_));
// 			return;
// 		}
	}
	
	m_Data.aLoadCombination.Add(data);
	int nNumLC = m_Data.aLoadCombination.GetSize();
	m_ListCtrl.InsertItem(nNumLC-1, _T(""));
	SetItem(nNumLC-1, data);
	m_ListCtrl.SetItemState(nNumLC-1, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CCmdSetOrthoLCDlg::OnCmdOLGModify() 
{
	// TODO: Add your control notification handler code here
	int nIndex;
	if (!GetSelectedItem(nIndex))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_item_in_the_list__nCh));
		return;
	}

	T_LCOE_BASE data;
	if (!Dlg2Item(data)) return;
	if (!ValidItem(data)) return;
	for (int i = 0; i < m_Data.aLoadCombination.GetSize(); i++)
	{
		T_LCOE_BASE LcoeBase = m_Data.aLoadCombination.GetAt(i);
		if (i == nIndex) continue;  // 변경 대상은 비교 제외
		//
		if (LcoeBase.StldKdX == data.StldKdX && LcoeBase.StldKdY == data.StldKdY && data.nLoadCaseType==0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__An_identical_load_group_already_exists_));
			return;
			//return FALSE;
		}
		else if(LcoeBase.StldKdX == data.StldKdY && LcoeBase.StldKdY == data.StldKdX  && data.nLoadCaseType==0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__An_identical_load_group_already_exists_));
			return;
		}
		//
		/*
		if (LcoeBase.StldKdX == data.StldKdX && LcoeBase.StldKdY == data.StldKdY && data.nLoadCaseType==0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__An_identical_load_group_already_exists_));
			return;
			//return FALSE;
		}
		else if (LcoeBase.StldKdX == data.StldKdX || LcoeBase.StldKdX == data.StldKdY ||
						 LcoeBase.StldKdY == data.StldKdY || LcoeBase.StldKdY == data.StldKdX)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__An_identical_load_case_is_already_assigned_));
			return;
		}
		*/
	}
	
	m_Data.aLoadCombination.SetAt(nIndex, data);
	SetItem(nIndex, data);
	m_ListCtrl.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCmdSetOrthoLCDlg::OnCmdOLGAuto()
{
	// Develop auto-generation feature for LC1, LC2, LC3 when LC3 is present
	// 1. Generate for the same LoadCaseType
	// 2. Only Vertical (Z) is possible for LC3
	// 3. Use existing algorithm when Vertical is not present
	if ( m_bUseLoadCaseZCombo && m_LoadCaseZ.GetCount() > 1 ) // If there is a Z load case other than None
	{
		CreateAutoGenerate_3Dir();
		CreateAutoGenerate_2Dir();
	}
	else
	{
		CreateAutoGenerate_2Dir();
	}
	//Show error box for invalid or unused load cases (added by Maharshi)
	BOOL tmp;
	T_STLD_K currKey;
	POSITION pos = m_mInvalidEQKeyList.GetStartPosition();
	CString strErrMsgAll;
	while (pos)
	{
		m_mInvalidEQKeyList.GetNextAssoc(pos, currKey, tmp);
		T_STLD_D rData;
		rData.Initialize();
		BOOL bCheck = m_pDoc->m_pAttrCtrl->GetStld(currKey, rData);
		CString strLoadCaseName = rData.LoadCaseName;
		CString strErrMsg;
		strErrMsg.Format(_LS(IDS_WG_CMD__ADDD__Invalid_loadcase_key_EQ), strLoadCaseName);
		strErrMsg += _T("\n");
		strErrMsgAll += strErrMsg;
	}

	/// [SWD-3611] 경고 메시지가 있는 경우에만 message box 띄우기.
	if (!strErrMsgAll.IsEmpty())
	{
		AfxMessageBox(strErrMsgAll, MB_OK);
	}	
}

void CCmdSetOrthoLCDlg::OnCmdOLGDelete() 
{
	// TODO: Add your control notification handler code here

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
	  T_LCOE_BASE data;
	  data = m_Data.aLoadCombination.GetAt(i);
	  SetItem(i, data);
	  m_ListCtrl.SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
	}
	// Set Focus.
	if(m_Data.aLoadCombination.GetSize() > nItem)
		m_ListCtrl.SetItemState(nItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCmdSetOrthoLCDlg::OnChangedCurrentItem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	int nIndex;
	T_LCOE_BASE data;
	if (!GetSelectedItem(nIndex))
		data.Initialize();
	else data = m_Data.aLoadCombination.GetAt(nIndex);

	// 값을 설정한다.
	Item2Dlg(nIndex, data);

	*pResult = 0;
}

void CCmdSetOrthoLCDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

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
	case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CCmdSetOrthoLCDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_STLD_K Key, KeyBak;
	T_STLD_D Data, DataBak;

	int i;

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_STLD_ADD):
				// do nothing
				break;
		case(UR_STLD_DEL):
				// 현재 loadcase list에서 지워진 것 삭제한다.
				pViewBuff->GetStld(nKey, Key, Data);
				for (i = m_Data.aLoadCombination.GetSize()-1; i >= 0; i--)
				{					
					if (m_Data.aLoadCombination[i].StldKdX== Key ||
						  m_Data.aLoadCombination[i].StldKdY== Key &&
							m_Data.aLoadCombination[i].nLoadCaseType ==0)
					{
						m_ListCtrl.DeleteItem(i);
						m_Data.aLoadCombination.RemoveAt(i);
					}
					
				}
				break;
		case(UR_STLD_MFD):
				// 여기서는 저장만 하고 UR_STLD_MFS에서 처리한다.
				pViewBuff->GetStld(nKey, Key, Data);
				KeyBak = Key;
				DataBak = Data;
				bMFD = TRUE;
				break;
		case(UR_STLD_MFS):
				// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
				ASSERT(bMFD);
				pViewBuff->GetStld(nKey, Key, Data);
				for (i = 0; i < m_Data.aLoadCombination.GetSize(); i++)
				{
					/*
					if (m_Data.aLoadCombination[i].StldKdX== Key ||
							m_Data.aLoadCombination[i].StldKdY== Key &&
							m_Data.aLoadCombination[i].nLoadCaseType ==0)
					{						
						//m_Data.aLoadCombination[i].LoadCaseKey = KeyBak;
						SetItem(i, m_Data.aLoadCombination[i]);
					}
					*/
				}
				// 플래그 수정
				bMFD = FALSE;
				break;
		default:
			break;
		}
	} // end of while

	// 
	T_SPLC_K spKey, spKeyBak;
	T_SPLC_D spData, spDataBak;

	pos = pViewBuff->GetStartBuffer();
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_SPLC_ADD):
				// do nothing
				break;
		case(UR_SPLC_DEL):
				// 현재 loadcase list에서 지워진 것 삭제한다.
				pViewBuff->GetSplc(nKey, spKey, spData);
				for (i = m_Data.aLoadCombination.GetSize()-1; i >= 0; i--)
				{					
					if (m_Data.aLoadCombination[i].StldKdX == spKey ||
						  m_Data.aLoadCombination[i].StldKdY == spKey &&
							m_Data.aLoadCombination[i].nLoadCaseType ==1)
					{
						m_ListCtrl.DeleteItem(i);
						m_Data.aLoadCombination.RemoveAt(i);
					}				
				}
				break;
		case(UR_SPLC_MFD):
				// 여기서는 저장만 하고 UR_STLD_MFS에서 처리한다.
				pViewBuff->GetSplc(nKey, spKey, spData);
				spKeyBak = spKey;
				spDataBak = spData;
				bMFD = TRUE;
				break;
		case(UR_SPLC_MFS):
				// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
				ASSERT(bMFD);
				pViewBuff->GetSplc(nKey, spKey, spData);
				for (i = 0; i < m_Data.aLoadCombination.GetSize(); i++)
				{					
					if (m_Data.aLoadCombination[i].StldKdX == spKey ||
						  m_Data.aLoadCombination[i].StldKdY == spKey &&
							m_Data.aLoadCombination[i].nLoadCaseType ==1)
					{
						//m_Data.aLoadCombination[i].LoadCaseKey = spKeyBak;
						SetItem(i, m_Data.aLoadCombination[i]);
					}				
				}
				// 플래그 수정
				bMFD = FALSE;
				break;
		default:
			break;
		}
	} // end of while


	/*/ LCOE 의 변화를 반영한다.
	BOOL bUpdate = FALSE;
	pos = pViewBuff->GetStartBuffer();
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
			
		case(UR_LCOE_ADD):
			//pViewBuff->GetLcoe(nKey, m_Data);
			bUpdate = TRUE;
			break;
		case(UR_LCOE_DEL):
			break;  // do nothing
		
		}
	}
	if (bUpdate)
	{
		MakeItemEx();
	}*/
}

BOOL CCmdSetOrthoLCDlg::CheckLoadCaseName(CString str, int& Index)
{
	BOOL bCheck=TRUE;
	int Count = m_ListCtrl.GetItemCount();
	for(int i=0; i<Count; i++)
	{
		CString strLcName = m_ListCtrl.GetItemText(i,0);
		if(str == strLcName)  
		{
			Index = i;
			bCheck = FALSE;
		}
	}
	return bCheck;
}

int CCmdSetOrthoLCDlg::GetLoadCaseType(int iXY, int iIndex)
{
	int iType = iXY == 2? NONE : ST;
	int iArrSize = m_arLCtype[iXY].GetSize();
	for(int i=0; i<iArrSize; i++)
	{
		T_SEIS_LCTP LCtype = m_arLCtype[iXY].GetAt(i);
		if(iIndex==LCtype.index)
			iType = LCtype.iLoadCaseType;		
	}
	return iType;
}

void CCmdSetOrthoLCDlg::SetLoadCaseCombo(T_LCOE_BASE &data)
{
	if(m_Data.aLoadCombination.GetSize()!=0)
	{
		if(data.nLoadCaseType == 0)  // 0=static
		{
			T_STLD_K KeyX = data.StldKdX;
			T_STLD_D rData;
			rData.Initialize();
			BOOL bCheck = m_pDoc->m_pAttrCtrl->GetStld(KeyX, rData);
			//ASSERT(bCheck);            
			CString strLoadCaseName = rData.LoadCaseName + _T("(ST)");
			int iCbIndex = m_LoadCaseX.SelectString(-1,strLoadCaseName);
			m_LoadCaseX.SetCurSel(iCbIndex);

			T_STLD_K KeyY = data.StldKdY;
			rData.Initialize();
			bCheck = m_pDoc->m_pAttrCtrl->GetStld(KeyY, rData);
			//ASSERT(bCheck);
			strLoadCaseName = rData.LoadCaseName + _T("(ST)");
			iCbIndex = m_LoadCaseY.SelectString(-1,strLoadCaseName);
			m_LoadCaseY.SetCurSel(iCbIndex);

			if (data.bStldKdZ == TRUE && m_bUseLoadCaseZCombo)
			{
				T_STLD_K KeyZ = data.StldKdZ;
				rData.Initialize();
				bCheck = m_pDoc->m_pAttrCtrl->GetStld(KeyZ, rData);
				strLoadCaseName = rData.LoadCaseName + _T("(ST)");
				iCbIndex = m_LoadCaseZ.SelectString(-1, strLoadCaseName);
				m_LoadCaseZ.SetCurSel(iCbIndex);
			}
			else m_LoadCaseZ.SetCurSel(0);
		}
		else if(data.nLoadCaseType == 1)  // 1=Response Spectrum
		{
			T_SPLC_K KeyX = data.StldKdX;
			T_SPLC_D rData;
			rData.Initialize();
			BOOL bCheck = m_pDoc->m_pAttrCtrl->GetSplc(KeyX, rData);
			//ASSERT(bCheck);            
			CString strLoadCaseName = rData.LoadCaseName + _T("(RS)");
			int iCbIndex = m_LoadCaseX.SelectString(-1,strLoadCaseName);
			m_LoadCaseX.SetCurSel(iCbIndex);

			T_SPLC_K KeyY = data.StldKdY;
			rData.Initialize();
			bCheck = m_pDoc->m_pAttrCtrl->GetSplc(KeyY, rData);
			//ASSERT(bCheck);
			strLoadCaseName = rData.LoadCaseName + _T("(RS)");
			iCbIndex = m_LoadCaseY.SelectString(-1,strLoadCaseName);
			m_LoadCaseY.SetCurSel(iCbIndex);

			if (data.bStldKdZ == TRUE && m_bUseLoadCaseZCombo)
			{
				T_SPLC_K KeyZ = data.StldKdZ;
				rData.Initialize();
				bCheck = m_pDoc->m_pAttrCtrl->GetSplc(KeyZ, rData);
				strLoadCaseName = rData.LoadCaseName + _T("(RS)");
				iCbIndex = m_LoadCaseZ.SelectString(-1, strLoadCaseName);
				m_LoadCaseZ.SetCurSel(iCbIndex);
			}
			else m_LoadCaseZ.SetCurSel(0);
		}
	}
}


BOOL CCmdSetOrthoLCDlg::GetUsedEQKeyList()
{
	POSITION pos;
	T_STLD_K StldK;

	CMap<T_STLD_K, T_STLD_K, BOOL, BOOL> mStldK;
	// Self Weight
	pos = m_pDoc->m_pAttrCtrl->GetStartBodf();
	while(pos)
	{
		T_BODF_D BodfD;
		m_pDoc->m_pAttrCtrl->GetNextBodf(pos, StldK, BodfD);
		if(m_pDoc->m_pAttrCtrl->ExistStld(BodfD.LoadCaseKey)) mStldK.SetAt(BodfD.LoadCaseKey, TRUE);
	}
	// Nodal Load
	pos = m_pDoc->m_pAttrCtrl->GetStartCnld();
	while(pos)
	{
		T_CNLD_K CnldK; T_CNLD_D CnldD;
		m_pDoc->m_pAttrCtrl->GetNextCnld(pos, CnldK, CnldD);
		if(m_pDoc->m_pAttrCtrl->ExistStld(CnldD.LoadCaseKey)) mStldK.SetAt(CnldD.LoadCaseKey, TRUE);
	}
	// Add by GAY.('06.04.11). Nodal Body Force
	pos = m_pDoc->m_pAttrCtrl->GetStartNbof();
	while(pos)
	{
		T_NBOF_K NbofK; T_NBOF_D NbofD;
		m_pDoc->m_pAttrCtrl->GetNextNbof(pos, NbofK, NbofD);
		if(m_pDoc->m_pAttrCtrl->ExistStld(NbofD.LoadCaseKey)) mStldK.SetAt(NbofD.LoadCaseKey, TRUE);
	}
	// Specified Displacement
	pos = m_pDoc->m_pAttrCtrl->GetStartSdsp();
	while(pos)
	{
		T_SDSP_K SdspK; T_SDSP_D SdspD;
		m_pDoc->m_pAttrCtrl->GetNextSdsp(pos, SdspK, SdspD);
		if(m_pDoc->m_pAttrCtrl->ExistStld(SdspD.LoadCaseKey)) mStldK.SetAt(SdspD.LoadCaseKey, TRUE);
	}
	// Beam Load
	pos = m_pDoc->m_pAttrCtrl->GetStartBmld();
	while(pos)
	{
		T_BMLD_K BmldK; T_BMLD_D BmldD;
		m_pDoc->m_pAttrCtrl->GetNextBmld(pos, BmldK, BmldD);
		if(m_pDoc->m_pAttrCtrl->ExistStld(BmldD.LoadCaseKey)) mStldK.SetAt(BmldD.LoadCaseKey, TRUE);
	}
	// Typical Beam Load 
	// Floor Load
	pos = m_pDoc->m_pAttrCtrl->GetStartFbla();
	while(pos)
	{
		T_FBLA_K TblaK; T_FBLA_D FblaD; T_FBLD_D FbldD;
		m_pDoc->m_pAttrCtrl->GetNextFbla(pos, TblaK, FblaD);
		m_pDoc->m_pAttrCtrl->GetFbld(FblaD.LoadTypeKey, FbldD);
		if(m_pDoc->m_pAttrCtrl->ExistStld(FbldD.LoadCaseKey[0])) mStldK.SetAt(FbldD.LoadCaseKey[0], TRUE);
		if(m_pDoc->m_pAttrCtrl->ExistStld(FbldD.LoadCaseKey[1])) mStldK.SetAt(FbldD.LoadCaseKey[1], TRUE);
		if(m_pDoc->m_pAttrCtrl->ExistStld(FbldD.LoadCaseKey[2])) mStldK.SetAt(FbldD.LoadCaseKey[2], TRUE);
		if(m_pDoc->m_pAttrCtrl->ExistStld(FbldD.LoadCaseKey[3])) mStldK.SetAt(FbldD.LoadCaseKey[3], TRUE);
		if(m_pDoc->m_pAttrCtrl->ExistStld(FbldD.LoadCaseKey[4])) mStldK.SetAt(FbldD.LoadCaseKey[4], TRUE);
		if(m_pDoc->m_pAttrCtrl->ExistStld(FbldD.LoadCaseKey[5])) mStldK.SetAt(FbldD.LoadCaseKey[5], TRUE);
		if(m_pDoc->m_pAttrCtrl->ExistStld(FbldD.LoadCaseKey[6])) mStldK.SetAt(FbldD.LoadCaseKey[6], TRUE);
		if(m_pDoc->m_pAttrCtrl->ExistStld(FbldD.LoadCaseKey[7])) mStldK.SetAt(FbldD.LoadCaseKey[7], TRUE);
	}
	// Plane Load
	pos = m_pDoc->m_pAttrCtrl->GetStartPnla();
	while(pos)
	{
		T_PNLA_K PlnaK; T_PNLA_D PnlaD;
		m_pDoc->m_pAttrCtrl->GetNextPnla(pos, PlnaK, PnlaD);
		if(m_pDoc->m_pAttrCtrl->ExistStld(PnlaD.LoadCaseKey)) mStldK.SetAt(PnlaD.LoadCaseKey, TRUE);
	}
	// Finishing Material Load
	pos = m_pDoc->m_pAttrCtrl->GetStartFmld();
	while(pos)
	{
		T_FMLD_K FmldK; T_FMLD_D FmldD;
		m_pDoc->m_pAttrCtrl->GetNextFmld(pos, FmldK, FmldD);
		if(m_pDoc->m_pAttrCtrl->ExistStld(FmldD.LoadCaseKey)) mStldK.SetAt(FmldD.LoadCaseKey, TRUE);
	}
	// Prestress Beam Load
	pos = m_pDoc->m_pAttrCtrl->GetStartPrst();
	while(pos)
	{
		 T_PRST_K PrstK; T_PRST_D PrstD;
			m_pDoc->m_pAttrCtrl->GetNextPrst(pos, PrstK, PrstD);
		if(m_pDoc->m_pAttrCtrl->ExistStld(PrstD.LoadCaseKey)) mStldK.SetAt(PrstD.LoadCaseKey, TRUE);
	}
	// Pretension Load
	pos = m_pDoc->m_pAttrCtrl->GetStartPtns();
	while(pos)
	{
		T_PTNS_K PtnsK; T_PTNS_D PtnsD;
		m_pDoc->m_pAttrCtrl->GetNextPtns(pos, PtnsK, PtnsD);
		if(m_pDoc->m_pAttrCtrl->ExistStld(PtnsD.LoadCaseKey)) mStldK.SetAt(PtnsD.LoadCaseKey, TRUE);
	}
	// Tendon Prestress Load
	pos = m_pDoc->m_pAttrCtrl->GetStartTdpl();
	while(pos)
	{
		T_TDPL_K TdplK; T_TDPL_D TdplD;
		m_pDoc->m_pAttrCtrl->GetNextTdpl(pos, TdplK, TdplD);
		if(m_pDoc->m_pAttrCtrl->ExistStld(TdplD.LoadCaseKey)) mStldK.SetAt(TdplD.LoadCaseKey, TRUE);
	}
	// Pressure Load
	pos = m_pDoc->m_pAttrCtrl->GetStartPres();
	while(pos)
	{
		T_PRES_K PresK; T_PRES_D PresD;
		m_pDoc->m_pAttrCtrl->GetNextPres(pos, PresK, PresD);
		if(m_pDoc->m_pAttrCtrl->ExistStld(PresD.LoadCaseKey)) mStldK.SetAt(PresD.LoadCaseKey, TRUE);
	}
	// Hydrostatic Pressure Load
	// Initial Force
	if(m_pDoc->m_pAttrCtrl->ExistIfct())
	{
		T_IFCT_D IfctD;
		m_pDoc->m_pAttrCtrl->GetIfct(IfctD);
		if(m_pDoc->m_pAttrCtrl->ExistStld(IfctD.LoadCaseKey)) mStldK.SetAt(IfctD.LoadCaseKey, TRUE);
	}
	// System Temperature
	T_STMP_K StmpK; T_STMP_D StmpD;
	pos = m_pDoc->m_pAttrCtrl->GetStartStmp();
	while(pos)
	{
		m_pDoc->m_pAttrCtrl->GetNextStmp(pos, StmpK, StmpD);
		if(m_pDoc->m_pAttrCtrl->ExistStld(StmpD.LoadCaseKey)) mStldK.SetAt(StmpD.LoadCaseKey, TRUE);
	}
	// Nodal Temperature
	pos = m_pDoc->m_pAttrCtrl->GetStartNtmp();
	while(pos)
	{
		T_NTMP_K NtmpK; T_NTMP_D NtmpD;
		m_pDoc->m_pAttrCtrl->GetNextNtmp(pos, NtmpK, NtmpD);
		if(m_pDoc->m_pAttrCtrl->ExistStld(NtmpD.LoadCaseKey)) mStldK.SetAt(NtmpD.LoadCaseKey, TRUE);
	}
	// Element Temperature
	pos = m_pDoc->m_pAttrCtrl->GetStartEtmp();
	while(pos)
	{
		T_ETMP_K EtmpK; T_ETMP_D EtmpD;
		m_pDoc->m_pAttrCtrl->GetNextEtmp(pos, EtmpK, EtmpD);
		if(m_pDoc->m_pAttrCtrl->ExistStld(EtmpD.LoadCaseKey)) mStldK.SetAt(EtmpD.LoadCaseKey, TRUE);
	}
	// Temperature Gradient
	T_GTMP_K GtmpK; T_GTMP_D GtmpD;
	pos = m_pDoc->m_pAttrCtrl->GetStartGtmp();
	while(pos)
	{
		m_pDoc->m_pAttrCtrl->GetNextGtmp(pos, GtmpK, GtmpD);
		if(m_pDoc->m_pAttrCtrl->ExistStld(GtmpD.LoadCaseKey)) mStldK.SetAt(GtmpD.LoadCaseKey, TRUE);
	}
	// Beam Section Temperature
	pos = m_pDoc->m_pAttrCtrl->GetStartBtmp();
	while(pos)
	{
		T_BTMP_K BtmpK; T_BTMP_D BtmpD;
		m_pDoc->m_pAttrCtrl->GetNextBtmp(pos, BtmpK, BtmpD);
		if(m_pDoc->m_pAttrCtrl->ExistStld(BtmpD.LoadCaseKey)) mStldK.SetAt(BtmpD.LoadCaseKey, TRUE);
	}
	// Wind Load
	pos = m_pDoc->m_pAttrCtrl->GetStartWind();
	while(pos)
	{
		T_WIND_K WindK; T_WIND_D WindD;
		m_pDoc->m_pAttrCtrl->GetNextWind(pos, WindK, WindD);
		if(m_pDoc->m_pAttrCtrl->ExistStld((T_STLD_K)WindK)) mStldK.SetAt((T_STLD_K)WindK, TRUE);
	}

	if(m_bSeismicEvaluation==FALSE)
	{
		// Static Seismic Load
		pos = m_pDoc->m_pAttrCtrl->GetStartSeis();
		while(pos)
		{
			T_SEIS_K SeisK; T_SEIS_D SeisD;
			m_pDoc->m_pAttrCtrl->GetNextSeis(pos, SeisK, SeisD);
			if(m_pDoc->m_pAttrCtrl->ExistStld((T_STLD_K)SeisK)) mStldK.SetAt((T_STLD_K)SeisK, TRUE);
		}
		// Superstructure Seismic Load
		pos = m_pDoc->m_pAttrCtrl->GetStartSsei();
		while(pos)
		{
			T_SSEI_K SseiK; T_SSEI_D SseiD;
			m_pDoc->m_pAttrCtrl->GetNextSsei(pos, SseiK, SseiD);
			//if(m_pDoc->m_pAttrCtrl->ExistStld(SseiD.StldKey)) mStldK.SetAt(SseiD.StldKey, TRUE);
			if(m_pDoc->m_pAttrCtrl->ExistStld((T_STLD_K)SseiK)) mStldK.SetAt((T_STLD_K)SseiK, TRUE);
		}
	}
	else
	{
		// Pseudo Seismic Force
		pos = m_pDoc->m_pAttrCtrl->GetStartPsll();
		while(pos)
		{
			T_PSLL_K PsllK; T_PSLL_D PsllD;
			m_pDoc->m_pAttrCtrl->GetNextPsll(pos, PsllK, PsllD);
			if(m_pDoc->m_pAttrCtrl->ExistStld((T_STLD_K)PsllK)) mStldK.SetAt((T_STLD_K)PsllK, TRUE);
		}
	}
 
	BOOL tmp;
	pos = mStldK.GetStartPosition();
	while(pos)
	{
		mStldK.GetNextAssoc(pos, StldK, tmp);
		m_arEQKeyList.Add(StldK);
	}
	m_arEQKeyList.FreeExtra();
	qsort(m_arEQKeyList.GetData(), m_arEQKeyList.GetSize(), sizeof(UINT), CCompFunc::UINTAsc);

	return TRUE;
}

void CCmdSetOrthoLCDlg::SetUserDefDataList(const std::map<LOAD_CASE_K, LOAD_CASE_K>  &mapBiEarthquake, BOOL bUserDef)
{
	m_bUser = bUserDef;
	m_Data.aLoadCombination.RemoveAll();
	for (auto itr=mapBiEarthquake.begin(); itr!=mapBiEarthquake.end(); itr++)
	{
		T_LCOE_BASE LcoeBase;
		LcoeBase.StldKdX = itr->first.Key;
		LcoeBase.StldKdY = itr->second.Key;
		LcoeBase.nLoadCaseType = itr->first.nLoadCaseType== D_LCOM_STATIC ? 0 : 1;
		m_Data.aLoadCombination.Add(LcoeBase);
	}
}

void CCmdSetOrthoLCDlg::GetUserDefDataList(std::map<LOAD_CASE_K, LOAD_CASE_K>  &mapBiEarthquake)
{
	mapBiEarthquake = m_mapBiEarthquake;
}

void CCmdSetOrthoLCDlg::CreateAutoGenerate_2Dir()
{
	int iCount = m_LoadCaseX.GetCount();

	for ( int i = 0; i < iCount - 1; i++ )
	{
		for ( int j = i + 1; j < iCount; j++ )
		{
			int iLoadCaseTypeX, iLoadCaseTypeY;
			T_LCOE_BASE data;
			data.Initialize();

			CString strName = _T("");

			m_LoadCaseX.GetLBText(i, strName);
			CString strLCNameX = strName;
			m_LoadCaseY.GetLBText(j, strName);
			CString strLCNameY = strName;

			int iX = 0, iY = 1;
			iLoadCaseTypeX = GetLoadCaseType(iX, i);
			iLoadCaseTypeY = GetLoadCaseType(iY, j);

			if ( iLoadCaseTypeX != iLoadCaseTypeY ) continue;
			else if ( i == j )                      continue;

			int iLoadCaseType = iLoadCaseTypeX;

			int nKeyX;
			int nKeyY;
			if ( iLoadCaseType == 0 )
			{
				strLCNameX.TrimRight(_T(")"));
				strLCNameX.TrimRight(_T("T"));
				strLCNameX.TrimRight(_T("S"));
				strLCNameX.TrimRight(_T("("));

				strLCNameY.TrimRight(_T(")"));
				strLCNameY.TrimRight(_T("T"));
				strLCNameY.TrimRight(_T("S"));
				strLCNameY.TrimRight(_T("("));

				nKeyX = m_pDoc->m_pAttrCtrl->GetStldKey(strLCNameX);
				nKeyY = m_pDoc->m_pAttrCtrl->GetStldKey(strLCNameY);

				data.StldKdX = nKeyX;
				data.StldKdY = nKeyY;
				data.nLoadCaseType = 0;
			}
			else if ( iLoadCaseType == 1 )
			{
				strLCNameX.TrimRight(_T(")"));
				strLCNameX.TrimRight(_T("S"));
				strLCNameX.TrimRight(_T("R"));
				strLCNameX.TrimRight(_T("("));

				strLCNameY.TrimRight(_T(")"));
				strLCNameY.TrimRight(_T("S"));
				strLCNameY.TrimRight(_T("R"));
				strLCNameY.TrimRight(_T("("));

				nKeyX = m_pDoc->m_pAttrCtrl->GetSplcKey(strLCNameX);
				nKeyY = m_pDoc->m_pAttrCtrl->GetSplcKey(strLCNameY);

				data.StldKdX = nKeyX;
				data.StldKdY = nKeyY;
				data.nLoadCaseType = 1;
			}
			if ( data.StldKdX <= 0 || data.StldKdY <= 0 ) continue;
			if (!CheckAndSetInvalidItemList(data)) { continue; }; //Added by Maharshi

			BOOL bAssigned = FALSE;
			for ( int k = 0; k < m_Data.aLoadCombination.GetSize(); k++ )
			{
				T_LCOE_BASE LcoeBase = m_Data.aLoadCombination.GetAt(k);

				if ( LcoeBase.bStldKdZ )
				{
					if ( LcoeBase.nLoadCaseType != data.nLoadCaseType )
					{
						continue;
					}
					if ( LcoeBase.StldKdZ == data.StldKdX || LcoeBase.StldKdZ == data.StldKdY )
					{
						bAssigned = TRUE;
						break;
					}
					if ( LcoeBase.StldKdX == data.StldKdX && LcoeBase.StldKdY == data.StldKdY )
					{
						bAssigned = TRUE;
						break;
					}
					if ( LcoeBase.StldKdX == data.StldKdY && LcoeBase.StldKdY == data.StldKdX )
					{
						bAssigned = TRUE;
						break;
					}
				}

				if ( LcoeBase.StldKdX == data.StldKdX && LcoeBase.StldKdY == data.StldKdY && data.nLoadCaseType == 0 )
				{
					bAssigned = TRUE;
					break;
				}
				else if ( LcoeBase.StldKdX == data.StldKdY && LcoeBase.StldKdY == data.StldKdX && data.nLoadCaseType == 0 )
				{
					bAssigned = TRUE;
					break;
				}
			}		
			if ( bAssigned ) continue;

			m_Data.aLoadCombination.Add(data);
			int nNumLC = m_Data.aLoadCombination.GetSize();
			m_ListCtrl.InsertItem(nNumLC - 1, _T(""));
			SetItem(nNumLC - 1, data);
			m_ListCtrl.SetItemState(nNumLC - 1, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
		}
	}
}

//Added by Maharshi
bool CCmdSetOrthoLCDlg::CheckAndSetInvalidItemList(T_LCOE_BASE& data)
{
	if (data.nLoadCaseType) { return true; }
	bool bXOk = false, bYOk = false, bZOk = false;
	UINT nSize = m_arEQKeyList.GetSize();
	if (data.bStldKdZ)
	{
		for (int i = 0; i < nSize; i++)
		{
			T_STLD_K currCase = m_arEQKeyList.GetAt(i);
			if (currCase == data.StldKdZ) { bZOk = true; }
		}
		if (!bZOk) { m_mInvalidEQKeyList.SetAt(data.StldKdZ, 0); }
	}

	for (int i = 0; i < nSize; i++)
	{
		T_STLD_K currCase = m_arEQKeyList.GetAt(i);
		if (currCase == data.StldKdX) { bXOk = true; }
		if (currCase == data.StldKdY) { bYOk = true; }
	}
	if (!bXOk) { m_mInvalidEQKeyList.SetAt(data.StldKdX, 0); }
	if (!bYOk) { m_mInvalidEQKeyList.SetAt(data.StldKdY, 0); }
	
	if (data.bStldKdZ && !bZOk) { return false; }
	if (!bXOk || !bYOk) { return false; }
	return true;
}

void CCmdSetOrthoLCDlg::CreateAutoGenerate_3Dir()
{
	// 3Dir 인 경우는 다 지우고 시작하자
	m_Data.aLoadCombination.RemoveAll();
	m_ListCtrl.DeleteAllItems();

	const int iCountX = m_LoadCaseX.GetCount();
	const int iCountY = m_LoadCaseY.GetCount();
	const int iCountZ = m_LoadCaseZ.GetCount();

	auto _l_removeLastFourChars = [] (CString& str) {
		if ( str.GetLength() > 4 ) {
			str = str.Left(str.GetLength() - 4);
		}
	};

	auto _l_IsLoadCaseType_XY = [this, &_l_removeLastFourChars] (int LCaseType, CString& str) {
		_l_removeLastFourChars(str);

		if ( LCaseType == ST )
		{
			T_STLD_D StldD;
			if ( !m_pDoc->m_pAttrCtrl->GetStld(str, StldD) )
			{
				ASSERT(0); return false;
			}
			if ( StldD.LoadCaseType == _T("EVT") ) // Z
			{
				return false;
			}
			return true;
		}
		else if ( LCaseType == RS )
		{
			T_SPLC_D SplcD;
			if ( !m_pDoc->m_pAttrCtrl->GetSplc(str, SplcD) )
			{
				ASSERT(0); return false;
			}
			if ( SplcD.nDirection == 2 ) // Z
			{
				return false;
			}
			return true;
		}
		else ASSERT(0); return false;
	};
	for ( int z = 0; z < iCountZ; z++ )
	{
		CString strLCNameZ = _T("");
		m_LoadCaseZ.GetLBText(z, strLCNameZ);
		_l_removeLastFourChars(strLCNameZ);
		const int iLoadCaseTypeZ = GetLoadCaseType(LC_3, z);

		for ( int x = 0; x < iCountX; x++ )
		{
			CString strLCNameX = _T("");
			m_LoadCaseX.GetLBText(x, strLCNameX);
			const int iLoadCaseTypeX = GetLoadCaseType(LC_1, x);
			if ( iLoadCaseTypeX != iLoadCaseTypeZ ) // Static or RS 같은 조합끼리만 생성
			{
				continue;
			}
			if ( !_l_IsLoadCaseType_XY(iLoadCaseTypeX, strLCNameX) ) // 같은 조합이라면 Z 는 제외
			{
				continue;
			}
			for ( int y = 0; y < iCountY; y++ )
			{
				CString strLCNameY = _T("");
				m_LoadCaseY.GetLBText(y, strLCNameY);
				const int iLoadCaseTypeY = GetLoadCaseType(LC_2, y);
				if ( iLoadCaseTypeY != iLoadCaseTypeZ ) // Static or RS 같은 조합끼리만 생성
				{
					continue;
				}
				if ( !_l_IsLoadCaseType_XY(iLoadCaseTypeY, strLCNameY) )
				{
					continue;
				}
				if ( strLCNameX.Compare(strLCNameY) == 0 )
				{
					continue;
				}

				T_LCOE_BASE data;
				data.bStldKdZ = TRUE;
				data.nLoadCaseType = iLoadCaseTypeZ;
				if ( iLoadCaseTypeZ == ST )
				{
					data.StldKdX = m_pDoc->m_pAttrCtrl->GetStldKey(strLCNameX);
					data.StldKdY = m_pDoc->m_pAttrCtrl->GetStldKey(strLCNameY);
					data.StldKdZ = m_pDoc->m_pAttrCtrl->GetStldKey(strLCNameZ);
				}
				else if ( iLoadCaseTypeZ == RS )
				{
					data.StldKdX = m_pDoc->m_pAttrCtrl->GetSplcKey(strLCNameX);
					data.StldKdY = m_pDoc->m_pAttrCtrl->GetSplcKey(strLCNameY);
					data.StldKdZ = m_pDoc->m_pAttrCtrl->GetSplcKey(strLCNameZ);
				}
				else ASSERT(0);
				m_strLCNameX = DataToStr(1, data);
				m_strLCNameY = DataToStr(2, data);
				m_strLCNameZ = DataToStr(3, data);
				
				if (!CheckAndSetInvalidItemList(data)) { continue; };

				BOOL bAssigned = FALSE;
				for ( int k = 0; k < m_Data.aLoadCombination.GetSize(); k++ )
				{
					T_LCOE_BASE LcoeBase = m_Data.aLoadCombination.GetAt(k);

					if ( LcoeBase.StldKdX == data.StldKdX && LcoeBase.StldKdY == data.StldKdY && LcoeBase.StldKdZ == data.StldKdZ )
					{
						bAssigned = TRUE;
						break;
					}
					else if ( LcoeBase.StldKdX == data.StldKdY && LcoeBase.StldKdY == data.StldKdX && LcoeBase.StldKdZ == data.StldKdZ )
					{
						bAssigned = TRUE;
						break;
					}
				}
				if ( bAssigned ) continue;

				m_Data.aLoadCombination.Add(data);
				int nNumLC = m_Data.aLoadCombination.GetSize();
				m_ListCtrl.InsertItem(nNumLC - 1, _T(""));
				SetItem(nNumLC - 1, data);
				m_ListCtrl.SetItemState(nNumLC - 1, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
			}
		}
	}
}

