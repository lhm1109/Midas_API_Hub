// CCmdSetOrthoLC_C3_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdSetOrthoLC_C3_Dlg.h"
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
// CCmdSetOrthoLC_C3_Dlg dialog

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

CCmdSetOrthoLC_C3_Dlg::CCmdSetOrthoLC_C3_Dlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCmdSetOrthoLC_C3_Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCmdSetOrthoLC_C3_Dlg)
	//}}AFX_DATA_INIT
	m_nOrthoCombType = COMBTYPE_BOTH;
    m_bConsiderST = false;
}

CCmdSetOrthoLC_C3_Dlg::CCmdSetOrthoLC_C3_Dlg(bool bConsiderST, CWnd* pParent /*=NULL*/)
    : CDialogMove(CCmdSetOrthoLC_C3_Dlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CCmdSetOrthoLC_C3_Dlg)
    //}}AFX_DATA_INIT
    m_nOrthoCombType = COMBTYPE_BOTH;
    m_bConsiderST = bConsiderST;
}

void CCmdSetOrthoLC_C3_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCmdSetOrthoLC_C3_Dlg)
	DDX_Radio  (pDX, IDC_ETC_OLGROUP_RDO_BOTH, m_nOrthoCombType);
	DDX_Control(pDX, IDC_ETC_LOADCASEX,        m_LoadCaseX);
	DDX_Control(pDX, IDC_ETC_LOADCASEY,        m_LoadCaseY);
	DDX_Control(pDX, IDC_ETC_LOADCASEZ,        m_LoadCaseZ);
	DDX_Control(pDX, IDC_ETC_OLGROUP_LIST,     m_ListCtrl);
	//}}AFX_DATA_MAP
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CCmdSetOrthoLC_C3_Dlg, CDialog)
#undef CDialog

	//{{AFX_MSG_MAP(CCmdSetOrthoLC_C3_Dlg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_ETC_OLGROUP_LIST, OnChangedCurrentItem) //OLG = Orthogonal Loads Group
	ON_BN_CLICKED(IDC_ETC_OLGROUP_RDO_BOTH, OnCmdOLGCombBoth)
	ON_BN_CLICKED(IDC_ETC_OLGROUP_RDO_PLUS, OnCmdOLGCombPlus)
	ON_BN_CLICKED(IDC_ETC_LOADCASE_BTN, OnClickLoadCaseBtn)
	ON_BN_CLICKED(IDC_ETC_BTN_ADD, OnCmdOLGAdd)
	ON_BN_CLICKED(IDC_ETC_BTN_MODIFY, OnCmdOLGModify)
	ON_BN_CLICKED(IDC_ETC_BTN_DELETE, OnCmdOLGDelete)
	
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCmdSetOrthoLC_C3_Dlg message handlers

void CCmdSetOrthoLC_C3_Dlg::SetListCtrlHeader()
{
		CString aTitle[] = {  _LS(IDS_CMD_Orthgonal_Effect_Num), 
													_LS(IDS_CMD_Orthgonal_Effect_LONGITUDINAL), 
													_LS(IDS_CMD_Orthgonal_Effect_TRANSVERSE),
													_LS(IDS_CMD_Orthgonal_Effect_VERTICAL)};
	int nColWidth[] = {40, 60, 60, 60};
	int nColNum = 4;
	CString title;
	LV_COLUMN lvcolumn;

	// set full row select mode
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_ListCtrl.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_ListCtrl.GetSafeHwnd(), dwStyle);

	// Set Title
	for(int i = 0; i < nColNum; i++)
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

void CCmdSetOrthoLC_C3_Dlg::SetData(int& nOrthoCombType, CArray<_LCOM_100_30_LC3, _LCOM_100_30_LC3&>& ar100_30_Load)
{
	m_nOrthoCombType = nOrthoCombType;
	m_a100_30_Load.Copy(ar100_30_Load);
}

void CCmdSetOrthoLC_C3_Dlg::GetData(int& nOrthoCombType, CArray<_LCOM_100_30_LC3, _LCOM_100_30_LC3&>& ar100_30_Load)
{
	nOrthoCombType = m_nOrthoCombType;
	ar100_30_Load.Copy(m_a100_30_Load);
}

BOOL CCmdSetOrthoLC_C3_Dlg::GetSelectedItem(int &nIndex)
{
	int iItem = m_ListCtrl.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;

	nIndex = iItem;
	return TRUE;
}

void CCmdSetOrthoLC_C3_Dlg::MakeItemEx()
{
	m_ListCtrl.DeleteAllItems();

	int nItemCount = m_a100_30_Load.GetSize(); 
	if(nItemCount == 0) return;

	for (int nCount = 0; nCount < nItemCount; nCount++)
	{
		m_ListCtrl.InsertItem(nCount, _T(""));
		SetItem(nCount, m_a100_30_Load.GetAt(nCount));
	}
	m_ListCtrl.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);  
}

CString CCmdSetOrthoLC_C3_Dlg::DataToStr(int iLCase, _LCOM_100_30_LC3 &data)
{
	CString value = _T("");
	if (iLCase==LC_3 && data.bLCase3==FALSE)
	{
		value = _T("None");
		return value;
	}

	if(data.nLoadCaseType==RS) // 1=Response Spectrum
	{
		T_SPLC_K key;
		if(iLCase==LC_1)      key = data.iLCKey1;
		else if(iLCase==LC_2) key = data.iLCKey2;
		else if(iLCase==LC_3) key = data.iLCKey3;

		T_SPLC_D rData;
		m_pDoc->m_pAttrCtrl->GetSplc(key, rData);
		value = rData.LoadCaseName+_T("(RS)");
	}
	else ASSERT(0);

	return value;
}

void CCmdSetOrthoLC_C3_Dlg::SetItem(int nIndex, _LCOM_100_30_LC3 &data)
{
	LVITEM lvitem;
	CString str;
	CString strIndex;
	for(int i = 0; i < 4; i++)
	{
		lvitem.iItem=nIndex;
		lvitem.iSubItem=i;
		if(i==0) str.Format(_T("%d"),nIndex+1);
	  else	   str = DataToStr(i-1, data);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT ;
		m_ListCtrl.SetItem(&lvitem);
	  str.ReleaseBuffer();
	}
}

BOOL CCmdSetOrthoLC_C3_Dlg::ValidItem(_LCOM_100_30_LC3& data)
{
	if(data.nLoadCaseType)  return TRUE;  // Response Spectrum is always TRUE.

	return TRUE;
}

// Coded by Seungjun MNet:No.2516 ('20061027)
BOOL CCmdSetOrthoLC_C3_Dlg::ValidItem(_LCOM_100_30_LC3& data, CString& strErrorMsg)
{  
	// Coded by Seungjun MNet:No.2593 ('20061128) Bug Fix.
	if(data.nLoadCaseType)  return TRUE;  // Response Spectrum is always TRUE.
	return TRUE;
}

BOOL CCmdSetOrthoLC_C3_Dlg::Dlg2Item(_LCOM_100_30_LC3 &data)
{
	int iLoadCaseTypeX, iLoadCaseTypeY, iLoadCaseTypeZ;
	int IndexX = m_LoadCaseX.GetCurSel();
	int IndexY = m_LoadCaseY.GetCurSel();
	int IndexZ = m_LoadCaseZ.GetCurSel();

	auto lambda_RSTrimRight = [] (CString& sLCName)
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

	if(IndexX != -1 || IndexY != -1 || IndexZ != -1 )
	{
		CString strName=_T("");
		
		m_LoadCaseX.GetLBText(IndexX,strName);
		CString strLCNameX = strName;
		m_strLCNameX  = strLCNameX;
		m_LoadCaseY.GetLBText(IndexY,strName);
		CString strLCNameY = strName;
		m_strLCNameY  = strLCNameY;
		m_LoadCaseZ.GetLBText(IndexZ,strName);
		CString strLCNameZ = strName;
		m_strLCNameZ  = strLCNameZ;

		iLoadCaseTypeX = GetLoadCaseType(LC_1, IndexX);
		iLoadCaseTypeY = GetLoadCaseType(LC_2, IndexY);
		iLoadCaseTypeZ = GetLoadCaseType(LC_3, IndexZ);

		if(iLoadCaseTypeX == NONE || iLoadCaseTypeY == NONE)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Types_of_two_loadcases_are_different_));
			return FALSE;  
		}
		if(iLoadCaseTypeX != iLoadCaseTypeY) 
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Types_of_two_loadcases_are_different_));
			return FALSE;  
		}
        
        if (iLoadCaseTypeX==ST && (IndexX == IndexY))
        {
            AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Two_loadcases_are_identical_));
            return FALSE;
        }

        if (iLoadCaseTypeZ != NONE)
        {
            if (iLoadCaseTypeX != iLoadCaseTypeZ)
            {
                AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Types_of_two_loadcases_are_different_));
                return FALSE;
            }
        }

		int nKeyX = 0, nKeyY = 0, nKeyZ = 0;
        if (iLoadCaseTypeX == RS)
        {
            lambda_RSTrimRight(strLCNameX);
            lambda_RSTrimRight(strLCNameY);
            nKeyX = m_pDoc->m_pAttrCtrl->GetSplcKey(strLCNameX);
            nKeyY = m_pDoc->m_pAttrCtrl->GetSplcKey(strLCNameY);
        }
        else
        {
            lambda_STTrimRight(strLCNameX);
            lambda_STTrimRight(strLCNameY);
            nKeyX = m_pDoc->m_pAttrCtrl->GetStldKey(strLCNameX);
            nKeyY = m_pDoc->m_pAttrCtrl->GetStldKey(strLCNameY);
        }

		if (iLoadCaseTypeZ == RS)
		{
			lambda_RSTrimRight(strLCNameZ);    
			nKeyZ = m_pDoc->m_pAttrCtrl->GetSplcKey(strLCNameZ);
			data.bLCase3 = TRUE;
		}
        else if (iLoadCaseTypeZ == ST)
        {
            lambda_STTrimRight(strLCNameZ);
            nKeyZ = m_pDoc->m_pAttrCtrl->GetStldKey(strLCNameZ);
            data.bLCase3 = TRUE;
        }
		else if (iLoadCaseTypeZ == NONE)
		{
			nKeyZ = 0;
			data.bLCase3 = FALSE;
		}

		data.iLCKey1 = nKeyX;
		data.iLCKey2 = nKeyY;
		data.iLCKey3 = nKeyZ;
		data.nLoadCaseType = iLoadCaseTypeX;
	}
	else
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Loadcase_was_not_selected_));
		return FALSE;
	}

	return TRUE;
}

BOOL CCmdSetOrthoLC_C3_Dlg::Item2Dlg(int nIndex, _LCOM_100_30_LC3 &data)
{
	SetLoadCaseCombo(data);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CCmdSetOrthoLC_C3_Dlg message handlers

void CCmdSetOrthoLC_C3_Dlg::Initial_LCaseComboBox()
{
	m_LoadCaseX.ResetContent();
	m_LoadCaseY.ResetContent();
	m_LoadCaseZ.ResetContent();
	m_arLCtype[0].RemoveAll();
	m_arLCtype[1].RemoveAll();
	m_arLCtype[2].RemoveAll();

    // None 추가
    m_LoadCaseZ.AddString(_T("None"));
    T_SEIS_LCTP LCTypeZ;
    LCTypeZ.index = m_LoadCaseZ.GetCount()-1;
    LCTypeZ.iLoadCaseType = NONE; // 0: Static, 1: Response Spectrum 2: None
    m_arLCtype[2].Add(LCTypeZ); // 2=Z

    // Static
    CArray<T_STLD_K, T_STLD_K> arKeyList;
    m_pDoc->m_pAttrCtrl->GetStldKeyList(arKeyList);
    int iCount = m_pDoc->m_pAttrCtrl->GetCountStld();

    if (iCount>0)
    {
        for (int i = 0; i < iCount; i++)
        {
            int iKey = arKeyList.GetAt(i);
            T_STLD_D rData;
            rData.Initialize();
            BOOL bCheck = m_pDoc->m_pAttrCtrl->GetStld(iKey, rData);
            ASSERT(bCheck);

            CString strLoadType = rData.LoadCaseType;
            CString strLoadCase = rData.LoadCaseName;
            if (CDBLib::IsStldEL(strLoadType))
            {
                if (strLoadType==_T("EVT"))
                {
                    m_LoadCaseZ.AddString(strLoadCase+_T("(ST)"));
                    T_SEIS_LCTP LCTypeZ;
                    LCTypeZ.index				 = m_LoadCaseX.GetCount()-1;
                    LCTypeZ.iLoadCaseType = ST; // 0: Static,		1: Response Spectrum
                    m_arLCtype[2].Add(LCTypeZ); // 2=Z
                }
                else
                {
                    m_LoadCaseX.AddString(strLoadCase+_T("(ST)"));
                    m_LoadCaseY.AddString(strLoadCase+_T("(ST)"));

                    T_SEIS_LCTP LCTypeX;
                    LCTypeX.index				 = m_LoadCaseX.GetCount()-1;
                    LCTypeX.iLoadCaseType = ST; // 0: Static,		1: Response Spectrum
                    m_arLCtype[0].Add(LCTypeX); // 0=X

                    T_SEIS_LCTP LCTypeY;
                    LCTypeY.index				 = m_LoadCaseY.GetCount()-1;
                    LCTypeY.iLoadCaseType = ST; // 0: Static,		1: Response Spectrum
                    m_arLCtype[1].Add(LCTypeY); // 1=Y
                }
            }

        }
    }

	// SplcK 분류
	CArray<T_SPLC_K, T_SPLC_K> aSplcK, aSplcXK, aSplcYK, aSplcZK;
	m_pDoc->m_pAttrCtrl->GetSplcKeyList(aSplcK);
	int nSplc = aSplcK.GetSize();
	for(int i=0; i<nSplc; ++i)
	{
		T_SPLC_K SplcK = aSplcK[i];

		T_SPLC_D SplcD;
		SplcD.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetSplc(SplcK,SplcD)) { ASSERT(0); continue; }
		if (SplcD.nAngleType == 1) continue;

		int nDir=0;
		BOOL bXYDir = m_pDoc->m_pAttrCtrl->GetBridgeDir(SplcD, nDir);

		if(!bXYDir)      aSplcZK.Add(SplcK);
		else if(nDir==0) aSplcXK.Add(SplcK);
		else if(nDir==1) aSplcYK.Add(SplcK);
		else ASSERT(0);
	}

	int nDirX = aSplcXK.GetSize();
	int nDirY = aSplcYK.GetSize();
	int nDirZ = aSplcZK.GetSize();

	if(nDirX==0)
	{
        if (m_arLCtype[0].GetSize()>0 )
        {
            // None 추가
            m_LoadCaseX.AddString(_T("None"));
            T_SEIS_LCTP LCType;
            LCType.index = 0;
            LCType.iLoadCaseType = NONE; // 0: Static, 1: Response Spectrum 2: None
            m_arLCtype[0].Add(LCType);
        }
	}
	else
	{
		//  Spectrum Load Case Data가 있음.
		for(int i=0; i<nDirX; ++i)
		{
			T_SPLC_K SplcK = aSplcXK[i];

			T_SPLC_D SplcD;
			SplcD.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetSplc(SplcK,SplcD)) { ASSERT(0); continue; }

			CString str = SplcD.LoadCaseName;
			m_LoadCaseX.AddString(str+_T("(RS)"));

			T_SEIS_LCTP LCTypeX;
			LCTypeX.index		      = m_LoadCaseX.GetCount()-1;
			LCTypeX.iLoadCaseType = RS; // 0: Static,		1: Response Spectrum
			m_arLCtype[0].Add(LCTypeX); // 0=X
		}
	}

	if(nDirY==0)
	{
        if (m_arLCtype[1].GetSize()>0 )
        {
            // None 추가
            m_LoadCaseY.AddString(_T("None"));
            T_SEIS_LCTP LCType;
            LCType.index = 0;
            LCType.iLoadCaseType = NONE; // 0: Static, 1: Response Spectrum 2: None
            m_arLCtype[1].Add(LCType);
        }
	}
	else
	{
		//  Spectrum Load Case Data가 있음.
		for(int i=0; i<nDirY; ++i)
		{
			T_SPLC_K SplcK = aSplcYK[i];

			T_SPLC_D SplcD;
			SplcD.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetSplc(SplcK,SplcD)) { ASSERT(0); continue; }

			CString str = SplcD.LoadCaseName;
			m_LoadCaseY.AddString(str+_T("(RS)"));

			T_SEIS_LCTP LCTypeY;
			LCTypeY.index			    = m_LoadCaseY.GetCount()-1;
			LCTypeY.iLoadCaseType = RS; // 0: Static,		1: Response Spectrum
			m_arLCtype[1].Add(LCTypeY); // 1=Y
		}
	}
    
	//  Spectrum Load Case Data가 있음.
	for(int i=0; i<nDirZ; ++i)
	{
		T_SPLC_K SplcK = aSplcZK[i];

		T_SPLC_D SplcD;
		SplcD.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetSplc(SplcK,SplcD)) { ASSERT(0); continue; }

		CString str = SplcD.LoadCaseName;
		m_LoadCaseZ.AddString(str+_T("(RS)"));

		T_SEIS_LCTP LCTypeZ;
		LCTypeZ.index				 = m_LoadCaseZ.GetCount()-1;
		LCTypeZ.iLoadCaseType = RS; // 0: Static,		1: Response Spectrum
		m_arLCtype[2].Add(LCTypeZ); // 2=Z
	}

	CDlgUtil::CobxAdjustListBoxWidth(m_LoadCaseX);  
	CDlgUtil::CobxAdjustListBoxWidth(m_LoadCaseY);  
	CDlgUtil::CobxAdjustListBoxWidth(m_LoadCaseZ);  

	m_LoadCaseX.SetCurSel(0);
	m_LoadCaseY.SetCurSel(0);
	m_LoadCaseZ.SetCurSel(0);
}

BOOL CCmdSetOrthoLC_C3_Dlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	// LoadCase ComboBox.
	Initial_LCaseComboBox();

	SetListCtrlHeader();
	MakeItemEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCmdSetOrthoLC_C3_Dlg::OnCmdOLGCombBoth() 
{
	m_nOrthoCombType = COMBTYPE_BOTH;
}

void CCmdSetOrthoLC_C3_Dlg::OnCmdOLGCombPlus() 
{
	m_nOrthoCombType = COMBTYPE_PLUS;
}

void CCmdSetOrthoLC_C3_Dlg::OnOK() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	// Coded by Seungjun MNet:No.2516 ('20061031)
	int iCount = m_a100_30_Load.GetSize();
	CString strError = _T("");
	BOOL    IsAllValid = TRUE;

	for (int i = 0; i < iCount ; ++i)
	{
		_LCOM_100_30_LC3 LcoeBase = m_a100_30_Load.GetAt(i);
		m_strLCNameX = DataToStr(LC_1, LcoeBase);
		m_strLCNameY = DataToStr(LC_2, LcoeBase);
		m_strLCNameZ = DataToStr(LC_3, LcoeBase);
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
	CDialogMove::OnOK();
}

void CCmdSetOrthoLC_C3_Dlg::OnCancel() 
{
	// TODO: Add your control notification handler code here
	//CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);	
	CDialogMove::OnCancel();
}

void CCmdSetOrthoLC_C3_Dlg::OnClickLoadCaseBtn() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE, 0));	
}

void CCmdSetOrthoLC_C3_Dlg::OnCmdOLGAdd() 
{
	// TODO: Add your control notification handler code here
	_LCOM_100_30_LC3 data;

	if (!Dlg2Item(data)) return;
	if (!ValidItem(data)) return;
	for (int i = 0; i < m_a100_30_Load.GetSize(); i++)
	{
		_LCOM_100_30_LC3 Base = m_a100_30_Load.GetAt(i);

		if(Base.iLCKey1 == data.iLCKey1 && Base.iLCKey2 == data.iLCKey2 && Base.iLCKey3 == data.iLCKey3)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__An_identical_load_case_is_already_assigned_));
			return;
		}
		else if(Base.iLCKey1 == data.iLCKey2 && Base.iLCKey2 == data.iLCKey1)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__An_identical_load_case_is_already_assigned_));
			return;
		}
	}
	
	m_a100_30_Load.Add(data);
	int nNumLC = m_a100_30_Load.GetSize();
	m_ListCtrl.InsertItem(nNumLC-1, _T(""));
	SetItem(nNumLC-1, data);
	m_ListCtrl.SetItemState(nNumLC-1, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CCmdSetOrthoLC_C3_Dlg::OnCmdOLGModify() 
{
	// TODO: Add your control notification handler code here
	int nIndex;
	if (!GetSelectedItem(nIndex))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_item_in_the_list__nCh));
		return;
	}

	_LCOM_100_30_LC3 data;
	if (!Dlg2Item(data)) return;
	if (!ValidItem(data)) return;
	for (int i = 0; i < m_a100_30_Load.GetSize(); i++)
	{
		_LCOM_100_30_LC3 Base = m_a100_30_Load.GetAt(i);
		if (i == nIndex) continue;  // 변경 대상은 비교 제외
		//
		if (Base.iLCKey1 == data.iLCKey1 && Base.iLCKey2 == data.iLCKey2 && Base.iLCKey3 == data.iLCKey3)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__An_identical_load_group_already_exists_));
			return;
			//return FALSE;
		}
		else if(Base.iLCKey1 == data.iLCKey2 && Base.iLCKey2 == data.iLCKey1 && Base.iLCKey3 == data.iLCKey3)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__An_identical_load_group_already_exists_));
			return;
		}
	}
	
	m_a100_30_Load.SetAt(nIndex, data);
	SetItem(nIndex, data);
	m_ListCtrl.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCmdSetOrthoLC_C3_Dlg::OnCmdOLGDelete() 
{
	// TODO: Add your control notification handler code here

	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
	if(nItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Not_Selected_Data____));
		return;
	}
	m_ListCtrl.DeleteItem(nItem);
	m_a100_30_Load.RemoveAt(nItem);

	for(int i=0; i<m_a100_30_Load.GetSize(); i++)
	{
	  _LCOM_100_30_LC3 data;
	  data = m_a100_30_Load.GetAt(i);
	  SetItem(i, data);
	  m_ListCtrl.SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
	}
	// Set Focus.
	if(m_a100_30_Load.GetSize() > nItem)
		m_ListCtrl.SetItemState(nItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCmdSetOrthoLC_C3_Dlg::OnChangedCurrentItem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	int nIndex;
	_LCOM_100_30_LC3 data;
	if (!GetSelectedItem(nIndex))
		data.Initialize();
	else data = m_a100_30_Load.GetAt(nIndex);

	// 값을 설정한다.
	Item2Dlg(nIndex, data);

	*pResult = 0;
}

void CCmdSetOrthoLC_C3_Dlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CCmdSetOrthoLC_C3_Dlg::UpdateBuffer()
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
				for (i = m_a100_30_Load.GetSize()-1; i >= 0; i--)
				{					
					if (m_a100_30_Load[i].iLCKey1== Key ||
						  m_a100_30_Load[i].iLCKey2== Key &&
							m_a100_30_Load[i].nLoadCaseType ==0)
					{
						m_ListCtrl.DeleteItem(i);
						m_a100_30_Load.RemoveAt(i);
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
				for (i = 0; i < m_a100_30_Load.GetSize(); i++)
				{

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
				for (i = m_a100_30_Load.GetSize()-1; i >= 0; i--)
				{					
					if (m_a100_30_Load[i].iLCKey1 == spKey ||
						  m_a100_30_Load[i].iLCKey2 == spKey &&
							m_a100_30_Load[i].nLoadCaseType ==1)
					{
						m_ListCtrl.DeleteItem(i);
						m_a100_30_Load.RemoveAt(i);
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
				for (i = 0; i < m_a100_30_Load.GetSize(); i++)
				{					
					if (m_a100_30_Load[i].iLCKey1 == spKey ||
						  m_a100_30_Load[i].iLCKey2 == spKey &&
							m_a100_30_Load[i].nLoadCaseType ==1)
					{
						//m_Data.aLoadCombination[i].LoadCaseKey = spKeyBak;
						SetItem(i, m_a100_30_Load[i]);
					}				
				}
				// 플래그 수정
				bMFD = FALSE;
				break;
		default:
			break;
		}
	} // end of while

}

BOOL CCmdSetOrthoLC_C3_Dlg::CheckLoadCaseName(CString str, int& Index)
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

int CCmdSetOrthoLC_C3_Dlg::GetLoadCaseType(int iXY, int iIndex)
{
	int iType;
	int iArrSize = m_arLCtype[iXY].GetSize();
	for(int i=0; i<iArrSize; i++)
	{
		T_SEIS_LCTP LCtype = m_arLCtype[iXY].GetAt(i);
		if(iIndex==LCtype.index)
			iType = LCtype.iLoadCaseType;		
	}
	return iType;
}

void CCmdSetOrthoLC_C3_Dlg::SetLoadCaseCombo(_LCOM_100_30_LC3 &data)
{
	if(m_a100_30_Load.GetSize()!=0)
	{
        if (data.nLoadCaseType == 0)  // 0=static
        {
            T_STLD_K KeyX = data.iLCKey1;
            T_STLD_D rData;
            rData.Initialize();
            BOOL bCheck = m_pDoc->m_pAttrCtrl->GetStld(KeyX, rData);
            CString strLoadCaseName = rData.LoadCaseName + _T("(ST)");
            int iCbIndex = m_LoadCaseX.SelectString(-1, strLoadCaseName);
            m_LoadCaseX.SetCurSel(iCbIndex);

            T_STLD_K KeyY = data.iLCKey2;
            rData.Initialize();
            bCheck = m_pDoc->m_pAttrCtrl->GetStld(KeyY, rData);
            strLoadCaseName = rData.LoadCaseName + _T("(ST)");
            iCbIndex = m_LoadCaseY.SelectString(-1, strLoadCaseName);
            m_LoadCaseY.SetCurSel(iCbIndex);

            if (data.bLCase3==TRUE)
            {
                T_STLD_K KeyZ = data.iLCKey3;
                rData.Initialize();
                bCheck = m_pDoc->m_pAttrCtrl->GetStld(KeyZ, rData);
                strLoadCaseName = rData.LoadCaseName + _T("(ST)");
                iCbIndex = m_LoadCaseZ.SelectString(-1, strLoadCaseName);
                m_LoadCaseZ.SetCurSel(iCbIndex);
            }
        }
		else if(data.nLoadCaseType == RS)  // 1=Response Spectrum
		{
			T_SPLC_K KeyX = data.iLCKey1;
			T_SPLC_D rData;
			rData.Initialize();
			BOOL bCheck = m_pDoc->m_pAttrCtrl->GetSplc(KeyX, rData);
			CString strLoadCaseName = rData.LoadCaseName + _T("(RS)");
			int iCbIndex = m_LoadCaseX.SelectString(-1,strLoadCaseName);
			m_LoadCaseX.SetCurSel(iCbIndex);

			T_SPLC_K KeyY = data.iLCKey2;
			rData.Initialize();
			bCheck = m_pDoc->m_pAttrCtrl->GetSplc(KeyY, rData);
			strLoadCaseName = rData.LoadCaseName + _T("(RS)");
			iCbIndex = m_LoadCaseY.SelectString(-1,strLoadCaseName);
			m_LoadCaseY.SetCurSel(iCbIndex);

			if (data.bLCase3==TRUE)
			{
				T_SPLC_K KeyZ = data.iLCKey3;
				rData.Initialize();
				bCheck = m_pDoc->m_pAttrCtrl->GetSplc(KeyZ, rData);
				strLoadCaseName = rData.LoadCaseName + _T("(RS)");
				iCbIndex = m_LoadCaseZ.SelectString(-1,strLoadCaseName);
				m_LoadCaseZ.SetCurSel(iCbIndex);
			}
		}
	}
}
