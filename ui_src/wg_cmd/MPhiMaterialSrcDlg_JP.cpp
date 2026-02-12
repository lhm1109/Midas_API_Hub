#include "stdafx.h"
#include "wg_cmd.h"
#include "MPhiMaterialSrcDlg_JP.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\wg_db_InitCtrl.h"

#include "MPhiParameterDef.h"

#include "..\wg_db\wg_db_EditData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMPhiMaterialSrcDlg_JP dialog


CMPhiMaterialSrcDlg_JP::CMPhiMaterialSrcDlg_JP(MPhiDataAll* pMPhiDataAll, CWnd* pParent /*=NULL*/, int nType /*=0*/)
	: CDlgChild(CMPhiMaterialSrcDlg_JP::IDD, pParent)
{
	m_cMatName2 = _T("");
	m_sMatName2 = _T("");
	m_MainRebar2 = _T("");
	m_SubRebar2 = _T("");
	m_Es = 0.0;
	m_Fu = 0.0;
	m_Fy1 = 0.0;
	m_Fy2 = 0.0;
	m_Fc = 0.0;
	m_Fyr = 0.0;
	m_Fys = 0.0;
	m_pMPhiDataAll = pMPhiDataAll;
}

void CMPhiMaterialSrcDlg_JP::DoDataExchange(CDataExchange* pDX)
{
	CDlgChild::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMD_SRCMAT_LIST, m_ListCtrl);
	DDX_Control(pDX, IDC_CMD_SRC_ESUNIT, m_EsUnit);
	DDX_Control(pDX, IDC_CMD_SRC_FUUNIT, m_FuUnit);
	DDX_Control(pDX, IDC_CMD_SRC_FY1UNIT, m_Fy1Unit);
	DDX_Control(pDX, IDC_CMD_SRC_FY2UNIT, m_Fy2Unit);
	DDX_Control(pDX, IDC_CMD_SRC_FCUNIT, m_FcUnit);
	DDX_Control(pDX, IDC_CMD_SRC_REBARFYSUNIT, m_reFysUnit);
	DDX_Control(pDX, IDC_CMD_SRC_REBARFYUNIT, m_reFyUnit);
	DDX_Control(pDX, IDC_CMD_SRC_SUBNAME, m_SubRebar);
	DDX_Control(pDX, IDC_CMD_SRC_MAINNAME, m_MainRebar);
	DDX_Control(pDX, IDC_CMD_SRC_REBARCODE, m_RebarCode);
	DDX_Control(pDX, IDC_CMD_SRC_STEELMAT, m_sMatName);
	DDX_Control(pDX, IDC_CMD_SRC_CONCMAT, m_cMatName);
	DDX_Control(pDX, IDC_CMD_SRCMAT_CONCCODE, m_cCodeCombo);
	DDX_Control(pDX, IDC_CMD_SRCMAT_STEELCODE, m_sCodeCombo);
	DDX_Text(pDX, IDC_CMD_SRC_CONCMAT2, m_cMatName2);
	DDX_Text(pDX, IDC_CMD_SRC_STEELMAT2, m_sMatName2);
	DDX_Text(pDX, IDC_CMD_SRC_MAINNAME2, m_MainRebar2);
	DDX_Text(pDX, IDC_CMD_SRC_SUBNAME2, m_SubRebar2);
	DDX_Text(pDX, IDC_CMD_SRC_ES, m_Es);
	DDX_Text(pDX, IDC_CMD_SRC_FU, m_Fu);
	DDX_Text(pDX, IDC_CMD_SRC_FY1, m_Fy1);
	DDX_Text(pDX, IDC_CMD_SRC_FY2, m_Fy2);
	DDX_Text(pDX, IDC_CMD_SRC_FC, m_Fc);
	DDX_Text(pDX, IDC_CMD_SRC_REBARFY, m_Fyr);
	DDX_Text(pDX, IDC_CMD_SRC_REBARFYS, m_Fys);
}

BEGIN_MESSAGE_MAP(CMPhiMaterialSrcDlg_JP, CDlgChild)
	//{{AFX_MSG_MAP(CMPhiMaterialSrcDlg_JP)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_SRCMAT_LIST, OnItemchangedDgnSrcmatList)
	ON_NOTIFY(NM_CLICK, IDC_CMD_SRCMAT_LIST, OnClickDgnSrcmatList)
	ON_CBN_SELCHANGE(IDC_CMD_SRC_CONCMAT, OnSelchangeDgnSrcConcmat)
	ON_CBN_SELCHANGE(IDC_CMD_SRC_STEELMAT, OnSelchangeDgnSrcSteelmat)
	ON_CBN_SELCHANGE(IDC_CMD_SRCMAT_CONCCODE, OnSelchangeDgnSrcmatConccode)
	ON_CBN_SELCHANGE(IDC_CMD_SRCMAT_STEELCODE, OnSelchangeDgnSrcmatSteelcode)
	ON_CBN_SELCHANGE(IDC_CMD_SRC_MAINNAME, OnSelchangeDgnSrcMainname)
	ON_CBN_SELCHANGE(IDC_CMD_SRC_SUBNAME, OnSelchangeDgnSrcSubname)
	ON_CBN_SELCHANGE(IDC_CMD_SRC_REBARCODE, OnSelchangeDgnSrcRebarcode)
	ON_BN_CLICKED(IDC_BTN_MPHI_MATL_SRC_ADD_MOD, OnAddModBtn)
	ON_BN_CLICKED(IDC_BTN_MPHI_MATL_SRC_DELETE, OnDeleteBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CMPhiMaterialSrcDlg_JP::OnInitDialog()
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDlgChild::OnInitDialog();

	// TODO: Add extra initialization here
	Initial_ListCtrl();
	Initial_Data();
	Initial_Unit();
	UpdateData(FALSE);

	return TRUE;
}

void CMPhiMaterialSrcDlg_JP::Initial_ListCtrl()
{
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_ListCtrl.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(m_ListCtrl.GetSafeHwnd(), dwStyle);

	LV_COLUMN lvcolumn;
	TCHAR* list[6] = { _T("ID"),
						(TCHAR*)(LPCTSTR)_LS(IDS_DGN_LIST_NAME),
						(TCHAR*)(LPCTSTR)_LS(IDS_DGN_LIST_STEEL),
						(TCHAR*)(LPCTSTR)_LS(IDS_DGN_LIST_CONCRETE),
						(TCHAR*)(LPCTSTR)_LS(IDS_DGN_LIST_MAIN_BAR),
						(TCHAR*)(LPCTSTR)_LS(IDS_DGN_LIST_SUB_BAR) };
	int width[6] = { 40,140,140,120,140,100 };
	for (int i = 0; i < 6; i++)
	{
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_CENTER;
		lvcolumn.pszText = list[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(width[i]);
		m_ListCtrl.InsertColumn(i, &lvcolumn);
	}
}

void CMPhiMaterialSrcDlg_JP::Initial_Data()
{
	const CArray<T_MATL_K, T_MATL_K>& KeyList = m_pMPhiDataAll->aMatdK;
	int iCount = KeyList.GetSize();

	int ListLineNo = 0;
	for (int i = 0; i < iCount; i++)
	{
		int nID = KeyList.GetAt(i);

		T_MATD_D mData;
		mData.Initialize();
		if (!m_pMPhiDataAll->GetMatd(nID, mData)) continue;
		if (mData.Type != _T("SRC")) continue;
		
		CString strID = _T(""), strName = _T("");
		CString	strSteel = _T(""), strConcrete = _T(""), strMainRebar = _T(""), strSubRebar = _T("");

		strID.Format(_T("%5d"), nID);
		strName = mData.Name;
		strSteel = mData.Data1.CodeMatlName;
		strConcrete = mData.Data2.CodeMatlName;
		//  Rebar Name
		strMainRebar = mData.MainRebar_RebarName;
		strSubRebar = mData.SubRebar_RebarName;

		Write_ListCtrlBox(ListLineNo, strID, strName, strSteel, strConcrete, strMainRebar, strSubRebar);
		ListLineNo++;
	}
	// Initialize ComboBox(Steel, Concrete).
	ShowSteelMatName(-1);
	ShowConcreteMatName(-1);
	ShowRebarMatName(-1);
	
	// Add by ZINU.('01.10.15). Only Active if AIJ-SRC01.
// 	CDgnDataCtrl DataCtrl;
// 	T_DSRC_D rData;
// 	rData.Initialize();
// 	DataCtrl.Get_DgnSrcDsrc(rData);
// 
// 	CString strFyr = _T(""), strFys = _T("");
// 	if (rData.DesignCode == _T("AIJ-SRC01"))
// 	{
// 		strFyr = _T("mFy");
// 		strFys = _T("wFy");
// 	}
// 	else
// 	{
// 		strFyr = _T("Fyr");
// 		strFys = _T("Fys");
// 	}
// 	GetDlgItem(IDC_STATIC_SRCMAT_FYR)->SetWindowText(strFyr);
// 	GetDlgItem(IDC_STATIC_SRCMAT_FYS)->SetWindowText(strFys);
}

void CMPhiMaterialSrcDlg_JP::Write_ListCtrlBox(int Index, CString strID, CString strName, CString strSteel,
	CString strConcrete, CString strMainRebar, CString strSubRebar)
{
	LV_ITEM lvitem;
	LPTSTR szText[6];
	szText[0] = (LPTSTR)(LPCTSTR)strID;
	szText[1] = (LPTSTR)(LPCTSTR)strName;
	szText[2] = (LPTSTR)(LPCTSTR)strSteel;
	szText[3] = (LPTSTR)(LPCTSTR)strConcrete;
	szText[4] = (LPTSTR)(LPCTSTR)strMainRebar;
	szText[5] = (LPTSTR)(LPCTSTR)strSubRebar;
	// Input ListCtrl Box.
	for (int i = 0; i < 6; i++)
	{
		lvitem.mask = LVIF_TEXT;
		lvitem.iItem = Index;
		lvitem.iSubItem = i;
		lvitem.pszText = szText[i];
		if (i == 0)	m_ListCtrl.InsertItem(&lvitem);
		else			m_ListCtrl.SetItem(&lvitem);
	}
}

void CMPhiMaterialSrcDlg_JP::Initial_Unit()
{
	m_EsUnit.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_FuUnit.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_Fy1Unit.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_Fy2Unit.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_FcUnit.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_reFyUnit.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_reFysUnit.SetUnitType(D_UNITSYS_BASE_STRESS);
}

/////////////////////////////////////////////////////////////////////////////
// CMPhiMaterialSrcDlg_JP message handlers LIST

void CMPhiMaterialSrcDlg_JP::OnItemchangedDgnSrcmatList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	T_PREFERENCE rPref;
	rPref.Initialize();
	m_pDoc->m_pInitCtrl->GetPreference(rPref);

	int nItem = pNMListView->iItem;
	CString strID = m_ListCtrl.GetItemText(nItem, 0);
	if (strID != _T(""))
	{
		int key = _ttoi(strID);

		T_MATD_D mData;
		mData.Initialize();
		if (m_pMPhiDataAll->GetMatd(key, mData) == TRUE)
		{
			// Initialize Material Code and Name ComboBox.
			Initial_MaterialCombo(mData.Data1.CodeName, mData.Data1.CodeMatlName,
				mData.Data2.CodeName, mData.Data2.CodeMatlName);

			Initial_RebarCombo(mData);
		}
	}
	*pResult = 0;
}

void CMPhiMaterialSrcDlg_JP::OnClickDgnSrcmatList(NMHDR* pNMHDR, LRESULT* pResult)
{
	OnItemchangedDgnSrcmatList(pNMHDR, pResult);
	*pResult = 0;
}

void CMPhiMaterialSrcDlg_JP::Initial_MaterialCombo(CString sCodeName, CString sCodeMatlName,
	CString cCodeName, CString cCodeMatlName)
{
	Initial_SteelMatCombo(sCodeName, sCodeMatlName);
	Initial_SteelMatName(sCodeName, sCodeMatlName);
	Initial_ConcreteMatCombo(cCodeName, cCodeMatlName);
	Initial_ConcreteMatName(cCodeName, cCodeMatlName);

	UpdateData(FALSE);
}

void CMPhiMaterialSrcDlg_JP::Initial_SteelMatCombo(CString sCodeName, CString sCodeMatlName)
{
	m_sCodeCombo.ResetContent();

	CArray<CString, CString&> DesignCodeList;
	CString strType = _T("S");
	m_pDoc->m_pMatlDB->GetDesignCodeList(strType, DesignCodeList);
	m_sCodeCombo.AddString(_T("None"));
	for (int i = 0; i < DesignCodeList.GetSize(); i++)
	{
		if (DesignCodeList[i] == MATLCODE_STL_JIS || DesignCodeList[i] == MATLCODE_STL_JIS_CIVIL)
		{
#if defined(_RUS)
			m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(DesignCodeList[i]);
#endif
			m_sCodeCombo.AddString(DesignCodeList.GetAt(i));
		}
	}

	if (sCodeName == _T(""))	sCodeName = _T("None");

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(sCodeName);
#endif
	int Index = m_sCodeCombo.FindStringExact(-1, sCodeName);
	m_sCodeCombo.SetCurSel(Index);
	ShowSteelMatName(Index);
}

void CMPhiMaterialSrcDlg_JP::ShowSteelMatName(int Index)
{
	// Set Hide all Control
	{
		GetDlgItem(IDC_CMD_SRC_ES)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CMD_SRC_FU)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CMD_SRC_FY1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CMD_SRC_FY2)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_SRCMAT_FY1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_SRCMAT_FY2)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_CMD_SRC_FY2UNIT)->ShowWindow(SW_HIDE);
	}

	// Index=-1 or 0:User Define, Index!=-1:DB Define
	CString strSCode = _T("");
	if (Index > -1)	m_sCodeCombo.GetLBText(Index, strSCode);
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strSCode);
#endif
	if (strSCode == _T("") || strSCode == _T("None"))
	{
		GetDlgItem(IDC_CMD_SRC_STEELMAT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CMD_SRC_STEELMAT2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_SRCMAT_SNAME)->SetWindowText(_LS(IDS_IDD_CMD_MPHI_ELEMENT_DEFAULT_NAME));

		GetDlgItem(IDC_CMD_SRC_ES)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_SRC_FU)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_SRC_FY1)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_SRC_FY2)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_CMD_SRC_STEELMAT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_SRC_STEELMAT2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_SRCMAT_SNAME)->SetWindowText(_LS(IDS_IDD_CMD_MPHI_ELEMENT_SRC_DLG_IDC_STATIC_SRCMAT_SNAME));

		GetDlgItem(IDC_CMD_SRC_ES)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_SRC_FU)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_SRC_FY1)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_SRC_FY2)->EnableWindow(FALSE);
	}
	// Change by ZINU.('02.9.17).
	//int iChkKind = GetChkKind(strSCode);
	int iChkKind = 2; // only civil nx jp
	if (iChkKind == 1)
	{
		GetDlgItem(IDC_CMD_SRC_ES)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_SRC_FU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_SRC_FY1)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_STATIC_SRCMAT_FY1)->ShowWindow(SW_SHOW);
		//GetDlgItem(IDC_STATIC_SRCMAT_FY1)->SetWindowText(_T("Fy    :"));
	}
	else if (iChkKind == 2)
	{
		GetDlgItem(IDC_CMD_SRC_ES)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_SRC_FU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_SRC_FY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_SRC_FY2)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_STATIC_SRCMAT_FY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_SRCMAT_FY2)->ShowWindow(SW_SHOW);
		//GetDlgItem(IDC_STATIC_SRCMAT_FY1)->SetWindowText(_T("Fy1   :"));

		GetDlgItem(IDC_CMD_SRC_FY2UNIT)->ShowWindow(SW_SHOW);
	}
}

void CMPhiMaterialSrcDlg_JP::Initial_SteelMatName(CString sCodeName, CString sCodeMatlName)
{
	if (m_sMatName.GetCount() > 0)	m_sMatName.ResetContent();

	CArray <CString, CString&> NameList;
	m_pDoc->m_pMatlDB->GetSteelNameList(sCodeName, NameList);
	for (int i = 0; i < NameList.GetSize(); i++)
	{
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(sCodeName, NameList[i]);
#endif
		m_sMatName.AddString(NameList.GetAt(i));
	}

	CString CodeMatlName;
	CodeMatlName = sCodeMatlName;
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(sCodeName, CodeMatlName);
#endif

	int Index = m_sMatName.FindStringExact(-1, CodeMatlName);
	if (Index == -1)	m_sMatName2 = sCodeMatlName;
	else					m_sMatName.SetCurSel(Index);

	SetSteelStrength(sCodeName, sCodeMatlName);
}

void CMPhiMaterialSrcDlg_JP::SetSteelStrength(CString strCode, CString strName)
{
	T_MATL_STEEL sData;
	BOOL bCheck = m_pDoc->m_pMatlDB->GetSteelData(strCode, strName, sData);
	if (bCheck)
	{
		m_Es = sData.Elast;
		m_Fu = sData.S_Fu;
		m_Fy1 = sData.S_Fy1;
		m_Fy2 = sData.S_Fy2;
	}
	else
	{
		int nItem = m_ListCtrl.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
		if (nItem != -1)
		{
			CString strID = m_ListCtrl.GetItemText(nItem, 0);
			int nID = _ttoi(strID);
			
			T_MATD_D mData;	mData.Initialize();
			if (m_pMPhiDataAll->GetMatd(nID, mData) == TRUE)
			{
				m_Es = mData.Data1.Analysis.Elast;
				m_Fu = mData.Data1.Design.S_Fu;
				m_Fy1 = mData.Data1.Design.S_Fy1;
				m_Fy2 = mData.Data1.Design.S_Fy2;
			}
			else
			{
				m_Es = 0.0;
				m_Fu = 0.0;
				m_Fy1 = 0.0;
				m_Fy2 = 0.0;
			}
		}
	}
}

void CMPhiMaterialSrcDlg_JP::Initial_ConcreteMatCombo(CString cCodeName, CString cCodeMatlName)
{
	m_cCodeCombo.ResetContent();

	CArray<CString, CString&> DesignCodeList;
	CString strType = _T("C");
	m_pDoc->m_pMatlDB->GetDesignCodeList(strType, DesignCodeList);
	m_cCodeCombo.AddString(_T("None"));
	for (int i = 0; i < DesignCodeList.GetSize(); i++)
	{
		if (DesignCodeList[i] == MATLCODE_CON_JIS || DesignCodeList[i] == MATLCODE_CON_JIS_CIVIL)
		m_cCodeCombo.AddString(DesignCodeList.GetAt(i));
	}
	if (cCodeName == _T(""))	cCodeName = _T("None");
	int Index = m_cCodeCombo.FindStringExact(-1, cCodeName);
	m_cCodeCombo.SetCurSel(Index);
	ShowConcreteMatName(Index);
}

void CMPhiMaterialSrcDlg_JP::ShowConcreteMatName(int Index)
{
	// Index=-1 or 0:User Define, Index!=-1:DB Define
	CString strCCode = _T("");
	if (Index > -1)	m_cCodeCombo.GetLBText(Index, strCCode);
	if (strCCode == _T("") || strCCode == _T("None"))
	{
		GetDlgItem(IDC_CMD_SRC_CONCMAT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CMD_SRC_CONCMAT2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_SRCMAT_CNAME)->SetWindowText(_LS(IDS_IDD_CMD_MPHI_ELEMENT_DEFAULT_NAME));
		GetDlgItem(IDC_CMD_SRC_FC)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_CMD_SRC_CONCMAT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_SRC_CONCMAT2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_SRCMAT_CNAME)->SetWindowText(_LS(IDS_IDD_CMD_MPHI_ELEMENT_SRC_DLG_IDC_STATIC_SRCMAT_SNAME));
		GetDlgItem(IDC_CMD_SRC_FC)->EnableWindow(FALSE);
	}

}

void CMPhiMaterialSrcDlg_JP::Initial_ConcreteMatName(CString cCodeName, CString cCodeMatlName)
{
	if (m_cMatName.GetCount() > 0)  m_cMatName.ResetContent();

	CArray <CString, CString&> NameList;
	m_pDoc->m_pMatlDB->GetConcreteNameList(cCodeName, NameList);
	for (int i = 0; i < NameList.GetSize(); i++)
	{
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(cCodeName, NameList[i]);
#endif
		m_cMatName.AddString(NameList.GetAt(i));
	}

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(cCodeName, cCodeMatlName);
#endif
	int Index = m_cMatName.FindStringExact(-1, cCodeMatlName);
	if (Index == -1)	m_cMatName2 = cCodeMatlName;
	else					m_cMatName.SetCurSel(Index);

	SetConcreteStrength(cCodeName, cCodeMatlName);
}

void CMPhiMaterialSrcDlg_JP::SetConcreteStrength(CString strCode, CString strName)
{
	T_MATL_CONCRETE cData;
	BOOL bCheck = m_pDoc->m_pMatlDB->GetConcreteData(strCode, strName, cData);
	if (bCheck)	m_Fc = cData.C_fc;
	else
	{
		int nItem = m_ListCtrl.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
		if (nItem != -1)
		{
			CString strID = m_ListCtrl.GetItemText(nItem, 0);
			int nID = _ttoi(strID);

			T_MATD_D mData;
			mData.Initialize();
			BOOL bCheck = m_pMPhiDataAll->GetMatd(nID, mData);
			if (bCheck)	m_Fc = mData.Data2.Design.C_fc;
			else		m_Fc = 0.0;
		}
	}
}

void CMPhiMaterialSrcDlg_JP::Initial_RebarCombo(T_MATD_D mData)
{
	if (m_RebarCode.GetCount() > 0)  m_RebarCode.ResetContent();

	CArray<CString, CString&> DesignCodeList;
	CString strType = _T("C");
	m_pDoc->m_pMatlDB->GetDesignCodeList(strType, DesignCodeList);
	m_RebarCode.AddString(_T("None"));
	for (int i = 0; i < DesignCodeList.GetSize(); i++)	
	{
		if (DesignCodeList[i] == MATLCODE_CON_JIS || DesignCodeList[i] == MATLCODE_CON_JIS_CIVIL)
		{
#if defined(_RUS)
			m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(DesignCodeList[i]);
#endif
			m_RebarCode.AddString(DesignCodeList.GetAt(i));
		}
	}
	SetRebarCode(mData);
}

void CMPhiMaterialSrcDlg_JP::SetRebarCode(T_MATD_D mData)
{
	CString strCode = mData.Rebar_CodeName;
	CString strMain = mData.MainRebar_RebarName;
	CString strSub = mData.SubRebar_RebarName;
	if (strCode == _T(""))	strCode = _T("None");

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(strCode);
#endif
	int Index = m_RebarCode.FindStringExact(-1, strCode);
	m_RebarCode.SetCurSel(Index);

	ShowRebarMatName(Index);

	if (strCode == _T("") || strCode == _T("None"))
	{
		m_MainRebar2 = mData.MainRebar_RebarName;
		m_SubRebar2 = mData.SubRebar_RebarName;
		m_Fyr = mData.MainRebarData.B_fy;
		m_Fys = mData.SubRebarData.B_fy;
	}
	else
	{
		Initial_MainRebarCombo(strCode, strMain);
		Initial_SubRebarCombo(strCode, strSub);
		m_MainRebar2 = _T("");
		m_SubRebar2 = _T("");
	}

	UpdateData(FALSE);
}

void CMPhiMaterialSrcDlg_JP::ShowRebarMatName(int Index)
{
	// Index=-1 or 0:User Define, Index!=-1:DB Define
	CString strCode = _T("");
	if (Index > -1)	m_RebarCode.GetLBText(Index, strCode);
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
#endif
	if (strCode == _T("") || strCode == _T("None"))
	{
		GetDlgItem(IDC_CMD_SRC_MAINNAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CMD_SRC_MAINNAME2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_SRC_SUBNAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CMD_SRC_SUBNAME2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_SRCMAT_MREBAR)->SetWindowText(_LS(IDS_IDD_CMD_MPHI_ELEMENT_DEFAULT_STEEL_MAIN_NAME));
		GetDlgItem(IDC_STATIC_SRCMAT_SREBAR)->SetWindowText(_LS(IDS_IDD_CMD_MPHI_ELEMENT_DEFAULT_STEEL_SUB_NAME));
		GetDlgItem(IDC_CMD_SRC_REBARFY)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_SRC_REBARFYS)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_CMD_SRC_MAINNAME)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_SRC_MAINNAME2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CMD_SRC_SUBNAME)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_SRC_SUBNAME2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_SRCMAT_MREBAR)->SetWindowText(_LS(IDS_IDD_CMD_MPHI_ELEMENT_SRC_DLG_IDC_STATIC_SRCMAT_MREBAR));
		GetDlgItem(IDC_STATIC_SRCMAT_SREBAR)->SetWindowText(_LS(IDS_IDD_CMD_MPHI_ELEMENT_SRC_DLG_IDC_STATIC_SRCMAT_SREBAR));
		GetDlgItem(IDC_CMD_SRC_REBARFY)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_SRC_REBARFYS)->EnableWindow(FALSE);
	}


}

void CMPhiMaterialSrcDlg_JP::Initial_MainRebarCombo(CString strCode, CString strName)
{
	if (m_MainRebar.GetCount() > 0)  m_MainRebar.ResetContent();

	// Initialize Rebar Name ComboBox by Code.
	CArray<CString, CString&> RebarList;
	m_pDoc->m_pMatlDB->GetRebarNameList(strCode, RebarList);
	for (int i = 0; i < RebarList.GetSize(); i++)m_MainRebar.AddString(RebarList.GetAt(i));

	SetMainRebarCombo(strCode, strName);
	UpdateData(FALSE);
}

void CMPhiMaterialSrcDlg_JP::SetMainRebarCombo(CString strCode, CString strName)
{
	if (strName != _T(""))
	{
		int Index = m_MainRebar.FindStringExact(-1, strName);
		m_MainRebar.SetCurSel(Index);
	}
	SetMainRebarStrength(strCode, strName);
}

void CMPhiMaterialSrcDlg_JP::SetMainRebarStrength(CString strCode, CString strName)
{
	T_MATL_REBAR rData;
	rData.Initialize();
	BOOL bCheck = m_pDoc->m_pMatlDB->GetRebarData(strCode, strName, rData);
	if (bCheck)	m_Fyr = rData.B_fy;
	else				m_Fyr = 0.0;
}

void CMPhiMaterialSrcDlg_JP::Initial_SubRebarCombo(CString strCode, CString strName)
{
	if (m_SubRebar.GetCount() > 0)  m_SubRebar.ResetContent();

	// Initialize Rebar Name ComboBox by Code.
	CArray<CString, CString&> RebarList;
	m_pDoc->m_pMatlDB->GetRebarNameList(strCode, RebarList);
	for (int i = 0; i < RebarList.GetSize(); i++)	m_SubRebar.AddString(RebarList.GetAt(i));

	SetSubRebarCombo(strCode, strName);
	UpdateData(FALSE);
}

void CMPhiMaterialSrcDlg_JP::SetSubRebarCombo(CString strCode, CString strName)
{
	if (strName != _T(""))
	{
		int Index = m_SubRebar.FindStringExact(-1, strName);
		m_SubRebar.SetCurSel(Index);
	}
	SetSubRebarStrength(strCode, strName);
}

void CMPhiMaterialSrcDlg_JP::SetSubRebarStrength(CString strCode, CString strName)
{
	T_MATL_REBAR rData;
	rData.Initialize();
	BOOL bCheck = m_pDoc->m_pMatlDB->GetRebarData(strCode, strName, rData);
	if (bCheck)	m_Fys = rData.B_fy;
	else				m_Fys = 0.0;
}

/////////////////////////////////////////////////////////////////////////////
// CMPhiMaterialSrcDlg_JP message handlers  COMBO
void CMPhiMaterialSrcDlg_JP::OnSelchangeDgnSrcConcmat()
{
	UpdateData(TRUE);
	int Index = m_cMatName.GetCurSel();
	CString strName = _T("");
	m_cMatName.GetLBText(Index, strName);

	Index = m_cCodeCombo.GetCurSel();
	CString strCode = _T("");
	m_cCodeCombo.GetLBText(Index, strCode);

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatlDBName(strCode, strName);
#endif

	SetConcreteStrength(strCode, strName);
	UpdateData(FALSE);
}

void CMPhiMaterialSrcDlg_JP::OnSelchangeDgnSrcSteelmat()
{
	UpdateData(TRUE);
	int Index = m_sMatName.GetCurSel();
	CString strName = _T("");
	m_sMatName.GetLBText(Index, strName);

	Index = m_sCodeCombo.GetCurSel();
	CString strCode = _T("");
	m_sCodeCombo.GetLBText(Index, strCode);

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatlDBName(strCode, strName);
#endif

	SetSteelStrength(strCode, strName);
	UpdateData(FALSE);
}

void CMPhiMaterialSrcDlg_JP::OnSelchangeDgnSrcmatConccode()
{
	UpdateData(TRUE);
	if (m_cMatName.GetCount() > 0)  m_cMatName.ResetContent();

	CString CodeName = _T("");
	int Index = m_cCodeCombo.GetCurSel();
	if (Index != -1)	m_cCodeCombo.GetLBText(Index, CodeName);
	else	ASSERT(0);

	ShowConcreteMatName(Index);
	Initial_ConcreteMatName(CodeName, _T(""));
	//Initial_RebarCombo(CodeName,_T(""),_T(""));
	if (CodeName != _T("None"))	m_Fc = 0.0;
	m_cMatName2 = _T("");

	UpdateData(FALSE);
}

void CMPhiMaterialSrcDlg_JP::OnSelchangeDgnSrcmatSteelcode()
{
	UpdateData(TRUE);
	if (m_sMatName.GetCount() > 0)  m_sMatName.ResetContent();

	CString CodeName = _T("");
	int Index = m_sCodeCombo.GetCurSel();
	if (Index != -1) m_sCodeCombo.GetLBText(Index, CodeName);
	else	ASSERT(0);

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(CodeName);
#endif

	ShowSteelMatName(Index);
	Initial_SteelMatName(CodeName, _T(""));
	if (CodeName != _T("None"))
	{
		m_Es = 0.0;
		m_Fu = 0.0;
		m_Fy1 = 0.0;
		m_Fy2 = 0.0;
	}
	m_sMatName2 = _T("");
	UpdateData(FALSE);
}

void CMPhiMaterialSrcDlg_JP::OnSelchangeDgnSrcMainname()
{
	UpdateData(TRUE);
	int Index = m_RebarCode.GetCurSel();
	if (Index != -1)
	{
		CString strCode = _T(""), strName = _T("");
		m_RebarCode.GetLBText(Index, strCode);
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
#endif
		Index = m_MainRebar.GetCurSel();
		if (Index != -1)
		{
			m_MainRebar.GetLBText(Index, strName);
			SetMainRebarCombo(strCode, strName);

			UpdateData(FALSE);
		}
		Index = m_SubRebar.GetCurSel();
		if (Index != -1)
		{
			m_SubRebar.GetLBText(Index, strName);
			SetSubRebarCombo(strCode, strName);

			UpdateData(FALSE);
		}
	}
	else	ASSERT(0);
}

void CMPhiMaterialSrcDlg_JP::OnSelchangeDgnSrcSubname()
{
	UpdateData(TRUE);
	int Index = m_RebarCode.GetCurSel();
	if (Index != -1)
	{
		CString strCode = _T(""), strName = _T("");
		m_RebarCode.GetLBText(Index, strCode);
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
#endif
		Index = m_SubRebar.GetCurSel();
		if (Index != -1)
		{
			m_SubRebar.GetLBText(Index, strName);
			SetSubRebarCombo(strCode, strName);

			UpdateData(FALSE);
		}
	}
	else	ASSERT(0);
}

void  CMPhiMaterialSrcDlg_JP::OnSelchangeDgnSrcRebarcode()
{
	UpdateData(TRUE);

	CString strCode = _T("");
	int Index = m_RebarCode.GetCurSel();
	if (Index != -1)	m_RebarCode.GetLBText(Index, strCode);
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
#endif
	if (strCode == _T("None") || strCode == _T(""))	ShowRebarMatName(Index);
	else
	{
		ShowRebarMatName(Index);
		Initial_MainRebarCombo(strCode, _T(""));
		Initial_SubRebarCombo(strCode, _T(""));
	}

	m_MainRebar2 = _T("");
	m_SubRebar2 = _T("");

	UpdateData(FALSE);
}

BOOL CMPhiMaterialSrcDlg_JP::ErrorCheck()
{
	BOOL bCheck = TRUE;
	CString strMsg = _LS(IDS_DGN_CHK_TITLE_ERROR);
	// Check Steel.
	if (m_sCodeCombo.GetCurSel() == 0)
	{
		if (m_sMatName2 == _T("")) { bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_STL_MATL_NAME_ERROR); }
	}
	if (m_Es <= 0.0) { bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_ES_ERROR); }
	if (m_Fu <= 0.0) { bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FU_ERROR); }
	// Code Error Checking.
	int Index = m_sCodeCombo.GetCurSel();
	CString strSCode = _T("");
	m_sCodeCombo.GetLBText(Index, strSCode);
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strSCode);
#endif
	// Change by ZINU.('02.9.17).
	//int iChkKind = GetChkKind(strSCode);
	int iChkKind = 2; // only civil nx jp
	if (iChkKind == 1)
	{
		if (m_Fy1 <= 0.0) { bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY_ERROR); }
		m_Fy2 = m_Fy1;
	}
	else if (iChkKind == 2)
	{
		if (m_Fy1 <= 0.0) { bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY1_ERROR); }
		if (m_Fy2 <= 0.0) { bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY2_ERROR); }
	}
	else	ASSERT(0);
	// Check Concrete.
	if (m_cCodeCombo.GetCurSel() == 0)
	{
		if (m_cMatName2 == _T("")) { bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_CON_MATL_NAME_ERROR); }
	}

	// Change by ZINU.('03.01.27). Check Window Enabled.
	BOOL bUseSubRebar = (GetDlgItem(IDC_CMD_SRC_REBARFYS)->IsWindowEnabled() ? TRUE : FALSE);

	if (m_RebarCode.GetCurSel() == 0)
	{
		if (bUseSubRebar)
		{
			if (m_MainRebar2 == _T("") || m_SubRebar2 == _T(""))
			{
				bCheck = FALSE;
				strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_STL_MATL_NAME_ERROR);
			}
		}
		else
		{
			if (m_MainRebar2 == _T(""))
			{
				bCheck = FALSE;
				strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_STL_MATL_NAME_ERROR);
			}
		}
	}
	else
	{
		if (m_MainRebar.GetCurSel() == -1)
		{
			bCheck = FALSE;
			strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_MBAR_ERROR);
		}
		if (m_SubRebar.GetCurSel() == -1 && bUseSubRebar)
		{
			bCheck = FALSE;
			strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_SBAR_ERROR);
		}
	}

	if (m_Fc <= 0.0) { bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FC_ERROR); }

	//CDgnDataCtrl DataCtrl;
	//T_DSRC_D rData;
	//rData.Initialize();
	//DataCtrl.Get_DgnSrcDsrc(rData);
	//if (rData.DesignCode == _T("AIJ-SRC01"))
	//{
	//	if (m_Fyr <= 0.0) { bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_MFY_ERROR); }
	//	if (bUseSubRebar && m_Fys <= 0.0)
	//	{
	//		bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_WFY_ERROR);
	//	}
	//}
	//else
	//{
	//	if (m_Fyr <= 0.0) { bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY_ERROR); }
	//	if (bUseSubRebar && m_Fys <= 0.0)
	//	{
	//		bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FYS_ERROR);
	//	}
	//}

	if (!bCheck)	AfxMessageBox(strMsg, MB_OK);

	return bCheck;
}

void CMPhiMaterialSrcDlg_JP::ModifySteel(CString &strSCode, CString &strSName, T_MATD_D &mData)
{
	int Index = m_sCodeCombo.GetCurSel();
	if (Index != 0)	// Code
	{
		m_sCodeCombo.GetLBText(Index, strSCode);
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strSCode);
#endif
		mData.Data1.CodeName = strSCode;
		// Steel Material Name
		Index = m_sMatName.GetCurSel();
		if (Index != -1)
		{
			m_sMatName.GetLBText(Index, strSName);
#if defined(_RUS)
			m_pDoc->m_pEditData->ConvertRusMatlToEngMatlDBName(strSCode, strSName);
#endif
			mData.Data1.CodeMatlName = strSName;
			T_MATL_STEEL sData;
			if (!m_pDoc->m_pMatlDB->GetSteelData(strSCode, strSName, sData))	ASSERT(0);
		}
		else	ASSERT(0);
	}
	else	// Code is None.
	{
		strSName = m_sMatName2;
		mData.Data1.CodeName = _T("None");
		mData.Data1.CodeMatlName = m_sMatName2;
	}
	mData.Data1.Analysis.Elast = m_Es;
	mData.Data1.Design.S_Fu = m_Fu;
	mData.Data1.Design.S_Fy1 = m_Fy1;
	mData.Data1.Design.S_Fy2 = m_Fy2;
}

void CMPhiMaterialSrcDlg_JP::ModifyConcrete(CString& strCCode, CString& strCName, T_MATD_D& mData)
{
	int Index = m_cCodeCombo.GetCurSel();
	if (Index != 0)	// Code
	{
		m_cCodeCombo.GetLBText(Index, strCCode);
		mData.Data2.CodeName = strCCode;
		// Concrete Material Name
		Index = m_cMatName.GetCurSel();
		if (Index != -1)
		{
			m_cMatName.GetLBText(Index, strCName);

#if defined(_RUS)
			m_pDoc->m_pEditData->ConvertRusMatlToEngMatlDBName(strCCode, strCName);
#endif
			mData.Data2.CodeMatlName = strCName;
			T_MATL_CONCRETE cData;
			if (!m_pDoc->m_pMatlDB->GetConcreteData(strCCode, strCName, cData))	ASSERT(0);
		}
		else	ASSERT(0);
	}
	else	// Code
	{
		strCName = m_cMatName2;
		mData.Data2.CodeName = _T("None");
		mData.Data2.CodeMatlName = m_cMatName2;
	}
	mData.Data2.Design.C_fc = m_Fc;
}

void CMPhiMaterialSrcDlg_JP::ModifySRC(CString& strRebarCode, CString& strMainRName, CString& strSubRName, T_MATD_D& mData)
{
	int ReIndex = m_RebarCode.GetCurSel();
	if (ReIndex != -1) m_RebarCode.GetLBText(ReIndex, strRebarCode);
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strRebarCode);
#endif
	mData.Rebar_CodeName = strRebarCode;

	if (ReIndex != 0)	// if Code exists.
	{
		T_MATL_REBAR reData;
		// Set Material of Main Rebar.
		int Index = m_MainRebar.GetCurSel();
		if (Index != -1)
		{
			m_MainRebar.GetLBText(Index, strMainRName);
			mData.MainRebar_RebarName = strMainRName;

			reData.Initialize();
			if (!m_pDoc->m_pMatlDB->GetRebarData(strRebarCode, strMainRName, reData))	ASSERT(0);
			mData.MainRebarData.B_Density = reData.B_Density;
			mData.MainRebarData.B_Elast = reData.B_Elast;
			mData.MainRebarData.B_fy = reData.B_fy;
		}
		else	ASSERT(0);
		// Set Material of Sub Rebar.
		Index = m_SubRebar.GetCurSel();
		if (Index != -1)
		{
			m_SubRebar.GetLBText(Index, strSubRName);
			mData.SubRebar_RebarName = strSubRName;

			reData.Initialize();
			if (!m_pDoc->m_pMatlDB->GetRebarData(strRebarCode, strSubRName, reData))	ASSERT(0);
			mData.SubRebarData.B_Density = reData.B_Density;
			mData.SubRebarData.B_Elast = reData.B_Elast;
			mData.SubRebarData.B_fy = reData.B_fy;
		}
	}
	else
	{
		// Set Material of Main Rebar.
		strMainRName = m_MainRebar2;
		mData.MainRebar_RebarName = strMainRName;
		mData.MainRebarData.B_Density = 0.0;
		mData.MainRebarData.B_Elast = 0.0;
		mData.MainRebarData.B_fy = m_Fyr;
		// Set Material of Sub Rebar.
		strSubRName = m_SubRebar2;
		mData.SubRebar_RebarName = strSubRName;
		mData.SubRebarData.B_Density = 0.0;
		mData.SubRebarData.B_Elast = 0.0;
		mData.SubRebarData.B_fy = m_Fys;
	}
}

void CMPhiMaterialSrcDlg_JP::OnAddModBtn()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nItem == -1) {
		AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_MATL));
		return;
	}
	/////////////////////////////////////////////////////////////////////
	UpdateData(TRUE);
	if (ErrorCheck() == FALSE) return;
	
	CString strID = m_ListCtrl.GetItemText(nItem, 0);
	// Modify Material ID.
	T_MATL_K key = _ttoi(strID);
	T_MATD_D matdD; matdD.Initialize();
	m_pMPhiDataAll->GetMatd(key, matdD);
	
	if (!DlgToData(key, matdD)) return;

	/////////////////////////////////////////////////////////////////////
	// Material Data
	// 일단 CMPhiParameterDlg::m_pMPhiDataAll에 쌓아 놓음
	m_pMPhiDataAll->SetMatd(key, matdD);

	ModifyItem(nItem, key, matdD);

	SetFocus();
	RedrawWindow();
}

void CMPhiMaterialSrcDlg_JP::OnDeleteBtn()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nItem == -1) {
		AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_MATL));
		return;
	}

	CString strID = m_ListCtrl.GetItemText(nItem, 0);

	T_MATL_K key = _ttoi(strID);
	T_MATD_D data; data.Initialize();
	if (m_pMPhiDataAll->GetMatd(key, data) == FALSE) {
		AfxMessageBox(_LS(IDS_CMD_IMPORT_DXF_No_Selcted_Item));
		return;
	}

	data.Rebar_CodeName = _T("");
	data.MainRebar_RebarName = _T("");
	data.MainRebarData.Initialize();
	data.SubRebar_RebarName = _T("");
	data.SubRebarData.Initialize();

	m_pMPhiDataAll->SetMatd(key, data);
	m_pMPhiDataAll->DeleteMatdRein(key);

	ModifyItem(nItem, key, data);
	InitRebarCode();

	SetFocus();
	RedrawWindow();
}

BOOL CMPhiMaterialSrcDlg_JP::DlgToData(const T_MATL_K& key, OUT T_MATD_D& rMatd)
{
	CString strSCode = _T(""), strCCode = _T("");
	CString strSName = _T(""), strCName = _T("");

	CString strMatName = rMatd.Name;
	/////////////////////////////////////////////////////////////////////
	//  Steel Material
	ModifySteel(strSCode, strSName, rMatd);

	/////////////////////////////////////////////////////////////////////
	// Concrete Material
	ModifyConcrete(strCCode, strCName, rMatd);

	/////////////////////////////////////////////////////////////////////
	// SRC Rebar Material
	CString strRebarCode = _T(""), strMainRName = _T(""), strSubRName = _T("");
	ModifySRC(strRebarCode, strMainRName, strSubRName, rMatd);

	return TRUE;
}

void CMPhiMaterialSrcDlg_JP::ModifyItem(const int& nLineNo, const T_MATL_K& Key, const T_MATD_D& matdD)
{
	m_ListCtrl.SetItemText(nLineNo, 1, matdD.Name);				// Name.
	m_ListCtrl.SetItemText(nLineNo, 2, matdD.Data1.CodeMatlName);	// Steel.
	m_ListCtrl.SetItemText(nLineNo, 3, matdD.Data2.CodeMatlName);	// Concrete.
	m_ListCtrl.SetItemText(nLineNo, 4, matdD.MainRebar_RebarName);// M-Rebar.
	m_ListCtrl.SetItemText(nLineNo, 5, matdD.SubRebar_RebarName);	// S-Rebar.
	m_ListCtrl.SetItemState(nLineNo, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
}

void CMPhiMaterialSrcDlg_JP::InitRebarCode()
{
	CString strRebarCode = _T("None");

	int iIndex = m_RebarCode.FindStringExact(-1, strRebarCode);
	m_RebarCode.SetCurSel(iIndex);
	
	ShowRebarMatName(iIndex);

	m_MainRebar2 = _T("");
	m_SubRebar2 = _T("");
	m_Fyr = 0;
	m_Fys = 0;

	UpdateData(FALSE);
}