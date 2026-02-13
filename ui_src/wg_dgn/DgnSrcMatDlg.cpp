// DgnSrcMatDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSrcMatDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_MatlDB.h"

#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_base\wg_base_LocaleLib.h"
#include "..\wg_db\wg_db_EditData.h"

#include "DgnDataCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSrcMatDlg dialog


CDgnSrcMatDlg::CDgnSrcMatDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnSrcMatDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSrcMatDlg)
	m_cMatName2 = _T("");
	m_sMatName2 = _T("");
	m_MainRebar2 = _T("");
	m_SubRebar2 = _T("");
	m_Fu = 0.0;
	m_Es = 0.0;
	m_Fy1 = 0.0;
	m_Fy2 = 0.0;
	m_Fy3 = 0.0;
	m_Fy4 = 0.0;
	m_Fy5 = 0.0;  // add by Seungjun (`06.05.30).
	m_Fy6 = 0.0;  // add by Seungjun (`06.05.30).
	m_Fc = 0.0;
	m_Fyr = 0.0;
	m_Fys = 0.0;	
	//}}AFX_DATA_INIT
}


void CDgnSrcMatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSrcMatDlg)
	DDX_Control(pDX, IDC_DGN_SRC_FUUNIT, m_FuUnit);
	DDX_Control(pDX, IDC_DGN_SRC_FY1UNIT, m_Fy1Unit);
	DDX_Control(pDX, IDC_DGN_SRC_FY2UNIT, m_Fy2Unit);
	DDX_Control(pDX, IDC_DGN_SRC_FY3UNIT, m_Fy3Unit);
	DDX_Control(pDX, IDC_DGN_SRC_FY4UNIT, m_Fy4Unit);
	DDX_Control(pDX, IDC_DGN_SRC_FY5UNIT, m_Fy5Unit);  // add by Seungjun (`06.05.30).
	DDX_Control(pDX, IDC_DGN_SRC_FY6UNIT, m_Fy6Unit);  // add by Seungjun (`06.05.30).
	DDX_Control(pDX, IDC_DGN_SRC_FY7UNIT, m_Fy7Unit);
	DDX_Control(pDX, IDC_DGN_SRC_FY8UNIT, m_Fy8Unit);
	DDX_Control(pDX, IDC_DGN_SRC_FY9UNIT, m_Fy9Unit);
	DDX_Control(pDX, IDC_DGN_SRC_FY10UNIT, m_Fy10Unit);
	DDX_Control(pDX, IDC_DGN_SRC_ESUNIT, m_EsUnit);
	DDX_Control(pDX, IDC_DGN_SRC_FCUNIT, m_FcUnit);
	DDX_Control(pDX, IDC_DGN_SRC_SUBNAME, m_SubRebar);
	DDX_Control(pDX, IDC_DGN_SRC_MAINNAME, m_MainRebar);
	DDX_Control(pDX, IDC_DGN_SRC_REBARCODE, m_RebarCode);
	DDX_Control(pDX, IDC_DGN_SRC_STEELMAT, m_sMatName);
	DDX_Control(pDX, IDC_DGN_SRC_CONCMAT, m_cMatName);
	DDX_Control(pDX, IDC_DGN_SRCMAT_CONCCODE, m_cCodeCombo);
	DDX_Control(pDX, IDC_DGN_SRCMAT_STEELCODE, m_sCodeCombo);
	DDX_Control(pDX, IDC_DGN_SRC_REBARFYSUNIT, m_reFysUnit);
	DDX_Control(pDX, IDC_DGN_SRC_REBARFYUNIT, m_reFyUnit);
	DDX_Control(pDX, IDC_DGN_SRCMAT_LIST, m_ListCtrl);
	DDX_Text(pDX, IDC_DGN_SRC_CONCMAT2, m_cMatName2);
	DDX_Text(pDX, IDC_DGN_SRC_STEELMAT2, m_sMatName2);
	DDX_Text(pDX, IDC_DGN_SRC_MAINNAME2, m_MainRebar2);
	DDX_Text(pDX, IDC_DGN_SRC_SUBNAME2, m_SubRebar2);
	DDX_Text(pDX, IDC_DGN_SRC_FU, m_Fu);
	DDX_Text(pDX, IDC_DGN_SRC_ES, m_Es);
	DDX_Text(pDX, IDC_DGN_SRC_FY1, m_Fy1);
	DDX_Text(pDX, IDC_DGN_SRC_FY2, m_Fy2);
	DDX_Text(pDX, IDC_DGN_SRC_FY3, m_Fy3);
	DDX_Text(pDX, IDC_DGN_SRC_FY4, m_Fy4);
	DDX_Text(pDX, IDC_DGN_SRC_FY5, m_Fy5);  // add by Seungjun (`06.05.30).
	DDX_Text(pDX, IDC_DGN_SRC_FY6, m_Fy6);  // add by Seungjun (`06.05.30).
	DDX_Text(pDX, IDC_DGN_SRC_FY7, m_Fy7);
	DDX_Text(pDX, IDC_DGN_SRC_FY8, m_Fy8);
	DDX_Text(pDX, IDC_DGN_SRC_FY9, m_Fy9);
	DDX_Text(pDX, IDC_DGN_SRC_FY10, m_Fy10);
	DDX_Text(pDX, IDC_DGN_SRC_FC, m_Fc);
	DDX_Text(pDX, IDC_DGN_SRC_REBARFY, m_Fyr);
	DDX_Text(pDX, IDC_DGN_SRC_REBARFYS, m_Fys);
	//}}AFX_DATA_MAP
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnSrcMatDlg, CInternationalDlg)
#undef CDialog

	//{{AFX_MSG_MAP(CDgnSrcMatDlg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DGN_SRCMAT_LIST, OnItemchangedDgnSrcmatList)
	ON_NOTIFY(NM_CLICK, IDC_DGN_SRCMAT_LIST, OnClickDgnSrcmatList)
	ON_CBN_SELCHANGE(IDC_DGN_SRC_CONCMAT, OnSelchangeDgnSrcConcmat)
	ON_CBN_SELCHANGE(IDC_DGN_SRC_STEELMAT, OnSelchangeDgnSrcSteelmat)
	ON_CBN_SELCHANGE(IDC_DGN_SRCMAT_CONCCODE, OnSelchangeDgnSrcmatConccode)
	ON_CBN_SELCHANGE(IDC_DGN_SRCMAT_STEELCODE, OnSelchangeDgnSrcmatSteelcode)
	ON_CBN_SELCHANGE(IDC_DGN_SRC_MAINNAME, OnSelchangeDgnSrcMainname)
	ON_CBN_SELCHANGE(IDC_DGN_SRC_SUBNAME, OnSelchangeDgnSrcSubname)
	ON_CBN_SELCHANGE(IDC_DGN_SRC_REBARCODE, OnSelchangeDgnSrcRebarcode)
	ON_BN_CLICKED(IDC_DGN_SRCMAT_MODIFY, OnDgnSrcmatModify)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnSrcMatDlg message handlers

BOOL CDgnSrcMatDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	Initial_ListCtrl();
	Initial_Data();
	Initial_Unit();
	UpdateData(FALSE);

	return TRUE;
}

void CDgnSrcMatDlg::Initial_Unit()
{
	m_EsUnit.SetUnitType(D_UNITSYS_BASE_ELAST);

	m_FuUnit.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_Fy1Unit.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_Fy2Unit.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_Fy3Unit.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_Fy4Unit.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_Fy5Unit.SetUnitType(D_UNITSYS_BASE_STRESS);  // add by Seungjun (`06.05.30).
	m_Fy6Unit.SetUnitType(D_UNITSYS_BASE_STRESS);  // add by Seungjun (`06.05.30).
	m_Fy7Unit.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_Fy8Unit.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_Fy9Unit.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_Fy10Unit.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_FcUnit.SetUnitType(D_UNITSYS_BASE_STRESS);

	m_reFyUnit.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_reFysUnit.SetUnitType(D_UNITSYS_BASE_STRESS);
}

void CDgnSrcMatDlg::Initial_ListCtrl()
{
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_ListCtrl.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_ListCtrl.GetSafeHwnd(),dwStyle);

	LV_COLUMN lvcolumn;
	TCHAR *list[6]	= {_T("ID"),
						(TCHAR*)(LPCTSTR)_LS(IDS_DGN_LIST_NAME),
						(TCHAR*)(LPCTSTR)_LS(IDS_DGN_LIST_STEEL),
						(TCHAR*)(LPCTSTR)_LS(IDS_DGN_LIST_CONCRETE),
						(TCHAR*)(LPCTSTR)_LS(IDS_DGN_LIST_MAIN_BAR),
						(TCHAR*)(LPCTSTR)_LS(IDS_DGN_LIST_SUB_BAR)};
	int width[6]	= {40,75,64,82,72,72};
	for(int i=0; i<6; i++)
	{
		lvcolumn.mask     = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt      = LVCFMT_CENTER;
		lvcolumn.pszText  = list[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx       = globalUtils.ScaleByDPI(width[i]);
		m_ListCtrl.InsertColumn(i,&lvcolumn);
	}
}

void CDgnSrcMatDlg::Initial_Data()
{
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
			if(mData.Type == _T("SRC"))
			{
				CString strID=_T(""), strName=_T("");
				CString	strSteel=_T(""), strConcrete=_T(""), strMainRebar=_T(""), strSubRebar=_T("");

				strID.Format(_T("%5d"),nID);
				strName     = mData.Name;
				strSteel    = mData.Data1.CodeMatlName;
				strConcrete = mData.Data2.CodeMatlName;
				//  Rebar Name�� ����ݴϴ�.
				strMainRebar = mData.MainRebar_RebarName;
				strSubRebar  = mData.SubRebar_RebarName;

				Write_ListCtrlBox(ListLineNo, strID, strName, strSteel, strConcrete, strMainRebar, strSubRebar);
				ListLineNo++;
			}
		}
	}
	// Initialize ComboBox(Steel, Concrete).
	ShowSteelMatName(-1);
	ShowConcreteMatName(-1);
	ShowRebarMatName(-1);

	// Add by ZINU.('01.10.15). Only Active if AIJ-SRC01.
	CDgnDataCtrl DataCtrl;
	T_DSRC_D rData;
	rData.Initialize();
	DataCtrl.Get_DgnSrcDsrc(rData);

	CString strFyr=_T(""), strFys=_T("");
	if(rData.DesignCode == _T("AIJ-SRC01"))
	{
		strFyr = _T("mFy");
		strFys = _T("wFy");
	}
	else
	{
		strFyr = _T("Fyr");
		strFys = _T("Fys");
	}
	GetDlgItem(IDC_STATIC_SRCMAT_FYR)->SetWindowText(strFyr);
	GetDlgItem(IDC_STATIC_SRCMAT_FYS)->SetWindowText(strFys);
}

void CDgnSrcMatDlg::Write_ListCtrlBox(int Index, CString strID, CString strName, CString strSteel,
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
	for(int i=0; i<6; i++)
	{
		lvitem.mask		  = LVIF_TEXT;
		lvitem.iItem  	= Index;
		lvitem.iSubItem = i;
		lvitem.pszText	= szText[i];
		if(i==0)	m_ListCtrl.InsertItem(&lvitem);
		else			m_ListCtrl.SetItem(&lvitem);
	}
}

void CDgnSrcMatDlg::OnClickDgnSrcmatList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnItemchangedDgnSrcmatList(pNMHDR,pResult);
	*pResult = 0;
}

void CDgnSrcMatDlg::OnItemchangedDgnSrcmatList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	T_PREFERENCE rPref;
	rPref.Initialize();
	m_pDoc->m_pInitCtrl->GetPreference(rPref);

	int nItem = pNMListView->iItem;
	CString strID = m_ListCtrl.GetItemText(nItem,0);
	if(strID != _T(""))
	{
		T_MATD_D mData;
		mData.Initialize();
		int key = _ttoi(strID);
		BOOL bCheck = m_pDoc->m_pAttrCtrl->GetMatlDesign(key,mData);
		if(bCheck)
		{
			// Initialize Material Code and Name ComboBox.
			Initial_MaterialCombo(mData.Data1.CodeName,mData.Data1.CodeMatlName,
														mData.Data2.CodeName,mData.Data2.CodeMatlName);

			Initial_RebarCombo(mData);
		}
	}
	*pResult = 0;
}

void CDgnSrcMatDlg::Initial_RebarCombo(T_MATD_D mData)
{
	if(m_RebarCode.GetCount() > 0)  m_RebarCode.ResetContent();

	CArray<CString,CString&> DesignCodeList;
	CString strType = _T("C");
	m_pDoc->m_pMatlDB->GetDesignCodeList(strType,DesignCodeList);
	m_RebarCode.AddString(_T("None"));
	for (int i = 0; i < DesignCodeList.GetSize(); i++)
	{
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(DesignCodeList[i]);
#endif
		m_RebarCode.AddString(DesignCodeList.GetAt(i));
	}
	SetRebarCode(mData);
}

void CDgnSrcMatDlg::SetRebarCode(T_MATD_D mData)
{
	CString strCode = mData.Rebar_CodeName;
	CString strMain = mData.MainRebar_RebarName;
	CString strSub = mData.SubRebar_RebarName;
 	if(strCode==_T(""))	strCode = _T("None");

	CString CodeName;
	CodeName = strCode;
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(CodeName);
#endif
	int Index = m_RebarCode.FindStringExact(-1, CodeName);
	m_RebarCode.SetCurSel(Index);

	ShowRebarMatName(Index);

	if(strCode == _T("") || strCode == _T("None"))
	{
		m_MainRebar2 = mData.MainRebar_RebarName;
		m_SubRebar2 = mData.SubRebar_RebarName;
		m_Fyr = mData.MainRebarData.B_fy;
		m_Fys = mData.SubRebarData.B_fy;
	}
	else
	{
		Initial_MainRebarCombo(strCode,strMain);
	  Initial_SubRebarCombo(strCode,strSub);
		m_MainRebar2=_T("");
		m_SubRebar2=_T("");
	}

	UpdateData(FALSE);
}

void CDgnSrcMatDlg::Initial_MainRebarCombo(CString strCode,CString strName)
{
	if(m_MainRebar.GetCount() > 0)  m_MainRebar.ResetContent();

	// Initialize Rebar Name ComboBox by Code.
	CArray<CString,CString&> RebarList;
	m_pDoc->m_pMatlDB->GetRebarNameList(strCode,RebarList);
	for(int i=0; i<RebarList.GetSize(); i++)	m_MainRebar.AddString(RebarList.GetAt(i));

	SetMainRebarCombo(strCode,strName);
	UpdateData(FALSE);
}

void CDgnSrcMatDlg::Initial_SubRebarCombo(CString strCode,CString strName)
{
	if(m_SubRebar.GetCount() > 0)  m_SubRebar.ResetContent();
	
	// Initialize Rebar Name ComboBox by Code.
	CArray<CString,CString&> RebarList;
	m_pDoc->m_pMatlDB->GetRebarNameList(strCode, RebarList);
	for(int i=0; i<RebarList.GetSize(); i++)	m_SubRebar.AddString(RebarList.GetAt(i));

	SetSubRebarCombo(strCode,strName);
	UpdateData(FALSE);
}

void CDgnSrcMatDlg::SetMainRebarStrength(CString strCode,CString strName)
{
	T_MATL_REBAR rData;
	rData.Initialize();
	BOOL bCheck = m_pDoc->m_pMatlDB->GetRebarData(strCode,strName,rData);
	if(bCheck)	m_Fyr = rData.B_fy;
	else				m_Fyr = 0.0;
}

void CDgnSrcMatDlg::SetSubRebarStrength(CString strCode,CString strName)
{
	T_MATL_REBAR rData;
	rData.Initialize();
	BOOL bCheck = m_pDoc->m_pMatlDB->GetRebarData(strCode,strName,rData);
	if(bCheck)	m_Fys = rData.B_fy;
	else				m_Fys = 0.0;
}

void CDgnSrcMatDlg::SetMainRebarCombo(CString strCode,CString strName)
{
	if(strName!=_T(""))
	{
		int Index = m_MainRebar.FindStringExact(-1,strName);
		m_MainRebar.SetCurSel(Index);
	}
	SetMainRebarStrength(strCode,strName);
}

void CDgnSrcMatDlg::SetSubRebarCombo(CString strCode,CString strName)
{
	if(strName!=_T(""))
	{
		int Index = m_SubRebar.FindStringExact(-1,strName);
		m_SubRebar.SetCurSel(Index);
	}
	SetSubRebarStrength(strCode,strName);
}

void CDgnSrcMatDlg::Initial_MaterialCombo(CString sCodeName,CString sCodeMatlName,
																					CString cCodeName,CString cCodeMatlName)
{
	Initial_SteelMatCombo(sCodeName, sCodeMatlName);
	Initial_SteelMatName(sCodeName, sCodeMatlName);
	Initial_ConcreteMatCombo(cCodeName, cCodeMatlName);
	Initial_ConcreteMatName(cCodeName, cCodeMatlName);

	UpdateData(FALSE);
}

void CDgnSrcMatDlg::SetSteelStrength(CString strCode,CString strName)
{
	T_MATL_STEEL sData;
	BOOL bCheck = m_pDoc->m_pMatlDB->GetSteelData(strCode,strName,sData);
	if(bCheck)
	{
		m_Es  = sData.Elast;
		m_Fu  = sData.S_Fu;
		m_Fy1 = sData.S_Fy1;
		m_Fy2 = sData.S_Fy2;
		m_Fy3 = sData.S_Fy3;
		m_Fy4 = sData.S_Fy4;
		m_Fy5 = sData.S_Fy5;  // add by Seungjun (`06.05.30).
		m_Fy6 = sData.S_Fy6;  // add by Seungjun (`06.05.30).
		m_Fy7 = sData.S_Fy7;
		m_Fy8 = sData.S_Fy8;
		m_Fy9 = sData.S_Fy9;
		m_Fy10 = sData.S_Fy10;
	}
	else
	{
		int nItem = m_ListCtrl.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
		if(nItem != -1)
		{
			CString strID = m_ListCtrl.GetItemText(nItem,0);
			int nID = _ttoi(strID);
			T_MATD_D mData;
			mData.Initialize();
			BOOL bCheck = m_pDoc->m_pAttrCtrl->GetMatlDesign(nID,mData);
			if(bCheck)
			{
				m_Es  = mData.Data1.Analysis.Elast;
				m_Fu  = mData.Data1.Design.S_Fu;
				m_Fy1 = mData.Data1.Design.S_Fy1;
				m_Fy2 = mData.Data1.Design.S_Fy2;
				m_Fy3 = mData.Data1.Design.S_Fy3;
				m_Fy4 = mData.Data1.Design.S_Fy4;
				m_Fy5 = mData.Data1.Design.S_Fy5;  // add by Seungjun (`06.05.30).
				m_Fy6 = mData.Data1.Design.S_Fy6;  // add by Seungjun (`06.05.30).
				m_Fy7 = mData.Data1.Design.S_Fy7;
				m_Fy8 = mData.Data1.Design.S_Fy8;
				m_Fy9 = mData.Data1.Design.S_Fy9;
				m_Fy10 = mData.Data1.Design.S_Fy10;
			}
			else
			{
				m_Es  = 0.0;
				m_Fu  = 0.0;
				m_Fy1 = 0.0;
				m_Fy2 = 0.0;
				m_Fy3 = 0.0;
				m_Fy4 = 0.0;
				m_Fy5 = 0.0;  // add by Seungjun (`06.05.30).
				m_Fy6 = 0.0;  // add by Seungjun (`06.05.30).
				m_Fy7 = 0.0;
				m_Fy8 = 0.0;
				m_Fy9 = 0.0;
				m_Fy10 = 0.0;
			}
		}
	}
}

void CDgnSrcMatDlg::SetConcreteStrength(CString strCode,CString strName)
{
	T_MATL_CONCRETE cData;
	BOOL bCheck = m_pDoc->m_pMatlDB->GetConcreteData(strCode,strName,cData);
	if(bCheck)	m_Fc = cData.C_fc;
	else
	{
		int nItem = m_ListCtrl.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
		if(nItem != -1)
		{
			CString strID = m_ListCtrl.GetItemText(nItem,0);
			int nID = _ttoi(strID);

			T_MATD_D mData;
			mData.Initialize();
			BOOL bCheck = m_pDoc->m_pAttrCtrl->GetMatlDesign(nID,mData);
			if(bCheck)	m_Fc = mData.Data2.Design.C_fc;
			else				m_Fc = 0.0;
		}
	}
}

void CDgnSrcMatDlg::Initial_SteelMatName(CString sCodeName,CString sCodeMatlName)
{
	if(m_sMatName.GetCount() > 0)	m_sMatName.ResetContent();

	CArray <CString, CString&> NameList;
	m_pDoc->m_pMatlDB->GetSteelNameList(sCodeName, NameList);
	for (int i = 0; i < NameList.GetSize(); i++)
	{
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(sCodeName, NameList[i]);
#endif
		m_sMatName.AddString(NameList.GetAt(i));
	}

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(sCodeName, sCodeMatlName);
#endif

	int Index = m_sMatName.FindStringExact(-1, sCodeMatlName);
	if(Index==-1)	m_sMatName2 = sCodeMatlName;
	else					m_sMatName.SetCurSel(Index);

	SetSteelStrength(sCodeName,sCodeMatlName);
}

void CDgnSrcMatDlg::Initial_ConcreteMatName(CString cCodeName,CString cCodeMatlName)
{
	if(m_cMatName.GetCount() > 0)  m_cMatName.ResetContent();

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

	int Index = m_cMatName.FindStringExact(-1,cCodeMatlName);
	if(Index==-1)	m_cMatName2 = cCodeMatlName;
	else					m_cMatName.SetCurSel(Index);

	SetConcreteStrength(cCodeName,cCodeMatlName);
}

void CDgnSrcMatDlg::Initial_SteelMatCombo(CString sCodeName,CString sCodeMatlName)
{
	m_sCodeCombo.ResetContent();

	CArray<CString, CString&> DesignCodeList;
	CString strType=_T("S");
	m_pDoc->m_pMatlDB->GetDesignCodeList(strType,DesignCodeList);
	m_sCodeCombo.AddString(_T("None"));
	for (int i = 0; i < DesignCodeList.GetSize(); i++)
	{
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(DesignCodeList[i]);
#endif
		m_sCodeCombo.AddString(DesignCodeList.GetAt(i));
	}

	if(sCodeName==_T(""))	sCodeName = _T("None");
  CString CodeName;
	CodeName = sCodeName;
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(CodeName);
#endif

	int Index = m_sCodeCombo.FindStringExact(-1, CodeName);
	m_sCodeCombo.SetCurSel(Index);
	ShowSteelMatName(Index);
}

void CDgnSrcMatDlg::Initial_ConcreteMatCombo(CString cCodeName,CString cCodeMatlName)
{
	m_cCodeCombo.ResetContent();

	CArray<CString, CString&> DesignCodeList;
	CString strType=_T("C");
	m_pDoc->m_pMatlDB->GetDesignCodeList(strType,DesignCodeList);
	m_cCodeCombo.AddString(_T("None"));
	for (int i = 0; i < DesignCodeList.GetSize(); i++)
	{
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(DesignCodeList[i]);
#endif
		m_cCodeCombo.AddString(DesignCodeList.GetAt(i));
	}

	if(cCodeName==_T(""))	cCodeName = _T("None");
	CString CodeName;
	CodeName = cCodeName;
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(CodeName);
#endif
	int Index = m_cCodeCombo.FindStringExact(-1, CodeName);
	m_cCodeCombo.SetCurSel(Index);
	ShowConcreteMatName(Index);
}

void CDgnSrcMatDlg::OnSelchangeDgnSrcmatConccode() 
{
	UpdateData(TRUE);
	if(m_cMatName.GetCount() > 0)  m_cMatName.ResetContent();

	CString CodeName=_T("");
	int Index = m_cCodeCombo.GetCurSel();
	if(Index != -1)	m_cCodeCombo.GetLBText(Index,CodeName);
	else	ASSERT(0);

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(CodeName);
#endif

	ShowConcreteMatName(Index);
	Initial_ConcreteMatName(CodeName,_T(""));
//Initial_RebarCombo(CodeName,_T(""),_T(""));
	if(CodeName!=_T("None"))	m_Fc = 0.0;
	m_cMatName2 = _T("");

	UpdateData(FALSE);
}

void CDgnSrcMatDlg::OnSelchangeDgnSrcmatSteelcode() 
{
	UpdateData(TRUE);
	if(m_sMatName.GetCount() > 0)  m_sMatName.ResetContent();

	CString CodeName=_T("");
	int Index = m_sCodeCombo.GetCurSel();
	if(Index!=-1) m_sCodeCombo.GetLBText(Index,CodeName);
	else	ASSERT(0);

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(CodeName);
#endif

	ShowSteelMatName(Index);
	Initial_SteelMatName(CodeName,_T(""));
	if(CodeName!=_T("None"))
	{
		m_Es	= 0.0;
		m_Fu	= 0.0;
		m_Fy1 = 0.0;
		m_Fy2 = 0.0;
		m_Fy3 = 0.0;
		m_Fy4 = 0.0;
		m_Fy5 = 0.0;  // add by Seungjun (`06.05.30).
		m_Fy6 = 0.0;  // add by Seungjun (`06.05.30).
		m_Fy7 = 0.0;
		m_Fy8 = 0.0;
		m_Fy9 = 0.0;
		m_Fy10 = 0.0;
	}
	m_sMatName2 = _T("");
	UpdateData(FALSE);
}

void CDgnSrcMatDlg::OnSelchangeDgnSrcConcmat() 
{
	UpdateData(TRUE);
	int Index = m_cMatName.GetCurSel();
	CString strName=_T("");
	m_cMatName.GetLBText(Index, strName);

	Index = m_cCodeCombo.GetCurSel();
	CString strCode=_T("");
	m_cCodeCombo.GetLBText(Index,strCode);

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatlDBName(strName, strCode);
#endif

	SetConcreteStrength(strCode, strName);
	UpdateData(FALSE);
}

void CDgnSrcMatDlg::OnSelchangeDgnSrcSteelmat() 
{
	UpdateData(TRUE);
	int Index = m_sMatName.GetCurSel();
	CString strName=_T("");
	m_sMatName.GetLBText(Index,strName);

	Index = m_sCodeCombo.GetCurSel();
	CString strCode=_T("");
	m_sCodeCombo.GetLBText(Index,strCode);

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatlDBName(strCode, strName);
#endif

	SetSteelStrength(strCode,strName);
	UpdateData(FALSE);
}

void CDgnSrcMatDlg::ShowSteelMatName(int Index)
{
	// Set Hide all Control
	{
		GetDlgItem(IDC_DGN_SRC_ES)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_SRC_FU)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_SRC_FY1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_SRC_FY2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_SRC_FY3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_SRC_FY4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_SRC_FY5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_SRC_FY6)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_SRC_FY7)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_SRC_FY8)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_SRC_FY9)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_SRC_FY10)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_SRCMAT_FY1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_SRCMAT_FY2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_SRCMAT_FY3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_SRCMAT_FY4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_SRCMAT_FY5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_SRCMAT_FY6)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_SRCMAT_FY7)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_SRCMAT_FY8)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_SRCMAT_FY9)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_SRCMAT_FY10)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_SRC_FY2UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_SRC_FY3UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_SRC_FY4UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_SRC_FY5UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_SRC_FY6UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_SRC_FY7UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_SRC_FY8UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_SRC_FY9UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_SRC_FY10UNIT)->ShowWindow(SW_HIDE);
	}

	// Index=-1 or 0:User Define, Index!=-1:DB Define
	CString strSCode = _T("");
	if(Index > -1)	m_sCodeCombo.GetLBText(Index,strSCode);
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strSCode);
#endif
	if(strSCode==_T("") || strSCode==_T("None"))
	{
		GetDlgItem(IDC_DGN_SRC_STEELMAT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_SRC_STEELMAT2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_SRCMAT_SNAME)->SetWindowText(_LS(IDS_DGN_TEXT_NAME));

		GetDlgItem(IDC_DGN_SRC_ES)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_SRC_FU)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_SRC_FY1)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_SRC_FY2)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_SRC_FY3)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_SRC_FY4)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_SRC_FY5)->EnableWindow(TRUE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_DGN_SRC_FY6)->EnableWindow(TRUE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_DGN_SRC_FY7)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_SRC_FY8)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_SRC_FY9)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_SRC_FY10)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_SRC_STEELMAT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_STEELMAT2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_SRCMAT_SNAME)->SetWindowText(_LS(IDS_DGN_TEXT_DB_NAME));

		GetDlgItem(IDC_DGN_SRC_ES)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_SRC_FU)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_SRC_FY1)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_SRC_FY2)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_SRC_FY3)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_SRC_FY4)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_SRC_FY5)->EnableWindow(FALSE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_DGN_SRC_FY6)->EnableWindow(FALSE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_DGN_SRC_FY7)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_SRC_FY8)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_SRC_FY9)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_SRC_FY10)->EnableWindow(FALSE);
	}
	// Change by ZINU.('02.9.17).
	int iChkKind = GetChkKind(strSCode);
	if(iChkKind==1)
	{
		GetDlgItem(IDC_DGN_SRC_ES)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FY1)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_STATIC_SRCMAT_FY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_SRCMAT_FY1)->SetWindowText(_T("Fy"));
	}
	else if(iChkKind==2) 
	{
		GetDlgItem(IDC_DGN_SRC_ES)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FY2)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_STATIC_SRCMAT_FY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_SRCMAT_FY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_SRCMAT_FY1)->SetWindowText(_T("Fy1"));
		GetDlgItem(IDC_DGN_SRC_FY2UNIT)->ShowWindow(SW_SHOW);
	}
	else if(iChkKind==3)
	{
		GetDlgItem(IDC_DGN_SRC_ES)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FY3)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_STATIC_SRCMAT_FY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_SRCMAT_FY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_SRCMAT_FY3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_SRCMAT_FY1)->SetWindowText(_T("Fy1"));
		GetDlgItem(IDC_DGN_SRC_FY2UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FY3UNIT)->ShowWindow(SW_SHOW);
	}
	else if(iChkKind==4)
	{
		GetDlgItem(IDC_DGN_SRC_ES)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FY3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FY4)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_STATIC_SRCMAT_FY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_SRCMAT_FY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_SRCMAT_FY3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_SRCMAT_FY4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_SRCMAT_FY1)->SetWindowText(_T("Fy1"));
		GetDlgItem(IDC_DGN_SRC_FY2UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FY3UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FY4UNIT)->ShowWindow(SW_SHOW);
	}
	else if(iChkKind==5) // by GAY. PMS:4817. ('14.03.11).
	{
			GetDlgItem(IDC_DGN_SRC_ES)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_SRC_FU)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_SRC_FY1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_SRC_FY2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_SRC_FY3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_SRC_FY4)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_SRC_FY5)->ShowWindow(SW_SHOW);
			
			GetDlgItem(IDC_STATIC_SRCMAT_FY1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_SRCMAT_FY2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_SRCMAT_FY3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_SRCMAT_FY4)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_SRCMAT_FY5)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_SRCMAT_FY1)->SetWindowText(_T("Fy1"));
			GetDlgItem(IDC_DGN_SRC_FY2UNIT)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_SRC_FY3UNIT)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_SRC_FY4UNIT)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_SRC_FY5UNIT)->ShowWindow(SW_SHOW);
	}
	else if(iChkKind==6)  // add by Seungjun (`06.05.30).
	{
		GetDlgItem(IDC_DGN_SRC_ES)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FY3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FY4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FY5)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FY6)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_STATIC_SRCMAT_FY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_SRCMAT_FY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_SRCMAT_FY3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_SRCMAT_FY4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_SRCMAT_FY5)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_SRCMAT_FY6)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_SRCMAT_FY1)->SetWindowText(_T("Fy1"));
		GetDlgItem(IDC_DGN_SRC_FY2UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FY3UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FY4UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FY5UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FY6UNIT)->ShowWindow(SW_SHOW);
	}
	else if (iChkKind == 10) // SP16.2017t.B3(S).
	{
		GetDlgItem(IDC_DGN_SRC_ES)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FY3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FY4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FY5)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FY6)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FY7)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FY8)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FY9)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FY10)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_STATIC_SRCMAT_FY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_SRCMAT_FY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_SRCMAT_FY3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_SRCMAT_FY4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_SRCMAT_FY5)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_SRCMAT_FY6)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_SRCMAT_FY7)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_SRCMAT_FY8)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_SRCMAT_FY9)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_SRCMAT_FY10)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_SRCMAT_FY1)->SetWindowText(_T("Fy1"));
		GetDlgItem(IDC_DGN_SRC_FY2UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FY3UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FY4UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FY5UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FY6UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FY7UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FY8UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FY9UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_FY10UNIT)->ShowWindow(SW_SHOW);
	}
}

void CDgnSrcMatDlg::ShowConcreteMatName(int Index)
{
	// Index=-1 or 0:User Define, Index!=-1:DB Define
	CString strCCode = _T("");
	if(Index > -1)	m_cCodeCombo.GetLBText(Index,strCCode);
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCCode);
#endif
	if(strCCode==_T("") || strCCode==_T("None"))
	{
		GetDlgItem(IDC_DGN_SRC_CONCMAT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_SRC_CONCMAT2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_SRCMAT_CNAME)->SetWindowText(_LS(IDS_DGN_TEXT_NAME));
		GetDlgItem(IDC_DGN_SRC_FC)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_SRC_CONCMAT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_CONCMAT2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_SRCMAT_CNAME)->SetWindowText(_LS(IDS_DGN_TEXT_DB_NAME));
		GetDlgItem(IDC_DGN_SRC_FC)->EnableWindow(FALSE);
	}

}

void CDgnSrcMatDlg::ShowRebarMatName(int Index)
{
	// Index=-1 or 0:User Define, Index!=-1:DB Define
	CString strCode = _T("");
	if(Index > -1)	m_RebarCode.GetLBText(Index,strCode);
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
#endif
	if(strCode==_T("") || strCode==_T("None"))
	{
		GetDlgItem(IDC_DGN_SRC_MAINNAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_SRC_MAINNAME2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_SUBNAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_SRC_SUBNAME2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_SRCMAT_MREBAR)->SetWindowText(_LS(IDS_DGN_TEXT_FY_NAME));
		GetDlgItem(IDC_STATIC_SRCMAT_SREBAR)->SetWindowText(_LS(IDS_DGN_TEXT_FYS_NAME));
		GetDlgItem(IDC_DGN_SRC_REBARFY)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_SRC_REBARFYS)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_SRC_MAINNAME)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_MAINNAME2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_SRC_SUBNAME)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_SUBNAME2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_SRCMAT_MREBAR)->SetWindowText(_LS(IDS_DGN_TEXT_FY_DB_NAME));
		GetDlgItem(IDC_STATIC_SRCMAT_SREBAR)->SetWindowText(_LS(IDS_DGN_TEXT_FYS_DB_NAME));
		GetDlgItem(IDC_DGN_SRC_REBARFY)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_SRC_REBARFYS)->EnableWindow(FALSE);
	}


}

void CDgnSrcMatDlg::OnSelchangeDgnSrcMainname() 
{
	UpdateData(TRUE);
	int Index = m_RebarCode.GetCurSel();
	if(Index!=-1)
	{
		CString strCode=_T(""), strName=_T("");
		m_RebarCode.GetLBText(Index, strCode);
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
#endif
		Index = m_MainRebar.GetCurSel();
		if(Index!=-1)
		{
			m_MainRebar.GetLBText(Index, strName);
			SetMainRebarCombo(strCode, strName);

			UpdateData(FALSE);
		}
		Index = m_SubRebar.GetCurSel();
		if(Index!=-1)
		{
			m_SubRebar.GetLBText(Index, strName);
			SetSubRebarCombo(strCode, strName);

			UpdateData(FALSE);
		}
	}
	else	ASSERT(0);
}

void CDgnSrcMatDlg::OnSelchangeDgnSrcSubname() 
{
	UpdateData(TRUE);
	int Index = m_RebarCode.GetCurSel();
	if(Index != -1)
	{
		CString strCode=_T(""), strName=_T("");
		m_RebarCode.GetLBText(Index, strCode);
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
#endif
		Index = m_SubRebar.GetCurSel();
		if(Index != -1)
		{
			m_SubRebar.GetLBText(Index,strName);
			SetSubRebarCombo(strCode,strName);

			UpdateData(FALSE);
		}
	}
	else	ASSERT(0);
}

void CDgnSrcMatDlg::OnSelchangeDgnSrcRebarcode() 
{
	UpdateData(TRUE);

	CString strCode=_T("");
	int Index = m_RebarCode.GetCurSel();
	if(Index != -1)	m_RebarCode.GetLBText(Index,strCode);
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
#endif
	if(strCode==_T("None") || strCode==_T(""))	ShowRebarMatName(Index);
	else
	{
		ShowRebarMatName(Index);
	  Initial_MainRebarCombo(strCode,_T(""));
	  Initial_SubRebarCombo(strCode,_T(""));
	}

	m_MainRebar2=_T("");
	m_SubRebar2=_T("");

	UpdateData(FALSE);
}

void CDgnSrcMatDlg::OnDgnSrcmatModify() 
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
	if(nItem == -1) 
	{
		AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_MATL));
		return;
	}
	/////////////////////////////////////////////////////////////////////
	UpdateData(TRUE);
	if(ErrorCheck())
	{
		CString strSCode=_T(""), strCCode=_T("");
		CString strSName=_T(""), strCName=_T("");
		CString strID = m_ListCtrl.GetItemText(nItem,0);
		// Modify Material ID.
		int nID = _ttoi(strID);
		T_MATD_D mData;
		mData.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(nID,mData))	return;

		CString strMatName = mData.Name;
		/////////////////////////////////////////////////////////////////////
		//  Steel�� Material�� �����մϴ�.
		int Index=m_sCodeCombo.GetCurSel();
		if(Index != 0)	// Code�� �����մϴ�.
		{
			m_sCodeCombo.GetLBText(Index,strSCode);
#if defined(_RUS)
			m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strSCode);
#endif
			mData.Data1.CodeName = strSCode;
			// Steel Material Name�� ���ɴϴ�.
			Index = m_sMatName.GetCurSel();
			if(Index != -1)
			{
				m_sMatName.GetLBText(Index,strSName);

#if defined(_RUS)
				m_pDoc->m_pEditData->ConvertRusMatlToEngMatlDBName(strSCode, strSName);
#endif

				mData.Data1.CodeMatlName = strSName;
				T_MATL_STEEL sData;
				if(!m_pDoc->m_pMatlDB->GetSteelData(strSCode, strSName, sData))	ASSERT(0);
			}
			else	ASSERT(0);
		}
		else	// Code is None.
		{
			strSName = m_sMatName2;
			mData.Data1.CodeName			= _T("None");
			mData.Data1.CodeMatlName	= m_sMatName2;
		}
		mData.Data1.Analysis.Elast	= m_Es;
		mData.Data1.Design.S_Fu			= m_Fu;
		mData.Data1.Design.S_Fy1		= m_Fy1;
		mData.Data1.Design.S_Fy2		= m_Fy2;
		mData.Data1.Design.S_Fy3		= m_Fy3;
		mData.Data1.Design.S_Fy4		= m_Fy4;
		mData.Data1.Design.S_Fy5		= m_Fy5;  // add by Seungjun (`06.05.30).
		mData.Data1.Design.S_Fy6		= m_Fy6;  // add by Seungjun (`06.05.30).
		mData.Data1.Design.S_Fy7		= m_Fy7;
		mData.Data1.Design.S_Fy8		= m_Fy8;
		mData.Data1.Design.S_Fy9		= m_Fy9;
		mData.Data1.Design.S_Fy10		= m_Fy10;
		/////////////////////////////////////////////////////////////////////
		// Concrete�� Material�� �����մϴ�.
		Index = m_cCodeCombo.GetCurSel();
		if(Index != 0)	// Code�� �����մϴ�.
		{
			m_cCodeCombo.GetLBText(Index,strCCode);
#if defined(_RUS)
			m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCCode);
#endif
			mData.Data2.CodeName = strCCode;
			// Concrete Material Name�� ���ɴϴ�.
			Index = m_cMatName.GetCurSel();
			if(Index != -1)
			{
				m_cMatName.GetLBText(Index,strCName);
#if defined(_RUS)
				m_pDoc->m_pEditData->ConvertRusMatlToEngMatlDBName(strCCode, strCName);
#endif
				mData.Data2.CodeMatlName = strCName;
				T_MATL_CONCRETE cData;
				if(!m_pDoc->m_pMatlDB->GetConcreteData(strCCode,strCName,cData))	ASSERT(0);
			}
			else	ASSERT(0);
		}
		else	// Code�� None�Դϴ�.
		{
			strCName = m_cMatName2;
			mData.Data2.CodeName = _T("None");
			mData.Data2.CodeMatlName = m_cMatName2;
		}
		mData.Data2.Design.C_fc	= m_Fc;
		/////////////////////////////////////////////////////////////////////
		// SRC Rebar�� Material�� �����մϴ�.
		CString strRebarCode=_T(""),strMainRName=_T(""),strSubRName=_T("");
		int ReIndex = m_RebarCode.GetCurSel();
		if(ReIndex!=-1) m_RebarCode.GetLBText(ReIndex,strRebarCode);
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strRebarCode);
#endif
		mData.Rebar_CodeName = strRebarCode;

		if(ReIndex!=0)	// if Code exists.
		{
			T_MATL_REBAR reData;
			// Set Material of Main Rebar.
			Index = m_MainRebar.GetCurSel();
			if(Index!=-1)
			{
				m_MainRebar.GetLBText(Index,strMainRName);
				mData.MainRebar_RebarName = strMainRName;

				reData.Initialize();
				if(!m_pDoc->m_pMatlDB->GetRebarData(strRebarCode,strMainRName,reData))	ASSERT(0);
				mData.MainRebarData.B_Density = reData.B_Density;
				mData.MainRebarData.B_Elast   = reData.B_Elast;
				mData.MainRebarData.B_fy      = reData.B_fy;
			}
			else	ASSERT(0);
			// Set Material of Sub Rebar.
			Index = m_SubRebar.GetCurSel();
			if(Index!=-1)
			{
				m_SubRebar.GetLBText(Index,strSubRName);
				mData.SubRebar_RebarName = strSubRName;

				reData.Initialize();
				if(!m_pDoc->m_pMatlDB->GetRebarData(strRebarCode,strSubRName,reData))	ASSERT(0);
				mData.SubRebarData.B_Density	= reData.B_Density;
				mData.SubRebarData.B_Elast		= reData.B_Elast;
				mData.SubRebarData.B_fy				= reData.B_fy;
			}
		}
		else
		{
			// Set Material of Main Rebar.
			strMainRName = m_MainRebar2;
			mData.MainRebar_RebarName = strMainRName;
			mData.MainRebarData.B_Density = 0.0;
			mData.MainRebarData.B_Elast   = 0.0;
			mData.MainRebarData.B_fy      = m_Fyr;
			// Set Material of Sub Rebar.
			strSubRName = m_SubRebar2;
			mData.SubRebar_RebarName = strSubRName;
			mData.SubRebarData.B_Density	= 0.0;
			mData.SubRebarData.B_Elast		= 0.0;
			mData.SubRebarData.B_fy				= m_Fys;
		}
		/////////////////////////////////////////////////////////////////////
		// Material Data�� �����մϴ�.
		if(m_pDoc->m_pDataCtrl->AddMatd(nID, mData))
		{
			m_ListCtrl.SetItemText(nItem,1,strMatName);		// Name.
			m_ListCtrl.SetItemText(nItem,2,strSName);			// Steel.
			m_ListCtrl.SetItemText(nItem,3,strCName);			// Concrete.
			m_ListCtrl.SetItemText(nItem,4,strMainRName);	// M-Rebar.
			m_ListCtrl.SetItemText(nItem,5,strSubRName);	// S-Rebar.
			m_ListCtrl.SetItemState(nItem, LVIS_FOCUSED | LVIS_SELECTED,LVIS_SELECTED | LVIS_FOCUSED);
		}
	}
}

int CDgnSrcMatDlg::GetChkKind(CString strMatlCode)
{
	CDgnDataCtrl DataCtrl;
	return DataCtrl.GetChkKindStlMatl(strMatlCode);
}

BOOL CDgnSrcMatDlg::ErrorCheck()
{
	BOOL bCheck = TRUE;
	CString strMsg = _LS(IDS_DGN_CHK_TITLE_ERROR);
	// Check Steel.
	if(m_sCodeCombo.GetCurSel()==0)
	{
		if(m_sMatName2==_T("")) {bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_STL_MATL_NAME_ERROR);}
	}
	if(m_Es <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_ES_ERROR);}
	if(m_Fu <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FU_ERROR);}
	// Code�� ���� Error Checking.
	int Index = m_sCodeCombo.GetCurSel();
	CString strSCode=_T("");
	m_sCodeCombo.GetLBText(Index, strSCode);
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strSCode);
#endif
	// Change by ZINU.('02.9.17).
	int iChkKind = GetChkKind(strSCode);
	if(iChkKind==1)
	{
		if(m_Fy1 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY_ERROR);}
		m_Fy2 = m_Fy1;
		m_Fy3 = m_Fy1;
		m_Fy4 = m_Fy1;
		m_Fy5 = m_Fy1;
		m_Fy6 = m_Fy1;
		m_Fy7 = m_Fy1;
		m_Fy8 = m_Fy1;
		m_Fy9 = m_Fy1;
		m_Fy10 = m_Fy1;
	}
	else if(iChkKind==2)
	{
		if(m_Fy1 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY1_ERROR);}
		if(m_Fy2 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY2_ERROR);}
		m_Fy3 = m_Fy2;
		m_Fy4 = m_Fy2;
		m_Fy5 = m_Fy2;
		m_Fy6 = m_Fy2;
		m_Fy7 = m_Fy2;
		m_Fy8 = m_Fy2;
		m_Fy9 = m_Fy2;
		m_Fy10 = m_Fy2;
	}
	else if(iChkKind==3)
	{
		if(m_Fy1 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY1_ERROR);}
		if(m_Fy2 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY2_ERROR);}
		if(m_Fy3 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY3_ERROR);}
		m_Fy4 = m_Fy3;
		m_Fy5 = m_Fy3;
		m_Fy6 = m_Fy3;
		m_Fy7 = m_Fy3;
		m_Fy8 = m_Fy3;
		m_Fy9 = m_Fy3;
		m_Fy10 = m_Fy3;
	}
	else if(iChkKind==4)
	{
		if(m_Fy1 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY1_ERROR);}
		if(m_Fy2 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY2_ERROR);}
		if(m_Fy3 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY3_ERROR);}
		if(m_Fy4 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY4_ERROR);}
		m_Fy5 = m_Fy4;
		m_Fy6 = m_Fy4;
		m_Fy7 = m_Fy4;
		m_Fy8 = m_Fy4;
		m_Fy9 = m_Fy4;
		m_Fy10 = m_Fy4;
	}
	else if(iChkKind==5) // by GAY. PMS:4817. ('14.03.11).
	{
		if(m_Fy1 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY1_ERROR);}
		if(m_Fy2 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY2_ERROR);}
		if(m_Fy3 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY3_ERROR);}
		if(m_Fy4 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY4_ERROR);}
		if(m_Fy5 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY5_ERROR);}
		m_Fy6 = m_Fy5;
		m_Fy7 = m_Fy5;
		m_Fy8 = m_Fy5;
		m_Fy9 = m_Fy5;
		m_Fy10 = m_Fy5;
	}
	else if(iChkKind==6)
	{
		if(m_Fy1 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY1_ERROR);}
		if(m_Fy2 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY2_ERROR);}
		if(m_Fy3 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY3_ERROR);}
		if(m_Fy4 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY4_ERROR);}
		if(m_Fy5 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY5_ERROR);}
		if(m_Fy6 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY6_ERROR);}
		m_Fy7 = m_Fy6;
		m_Fy8 = m_Fy6;
		m_Fy9 = m_Fy6;
		m_Fy10 = m_Fy6;
	}
	else if (iChkKind == 10)
	{
		if (m_Fy1 <= 0.0) { bCheck = FALSE; strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY1_ERROR); }
		if (m_Fy2 <= 0.0) { bCheck = FALSE; strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY2_ERROR); }
		if (m_Fy3 <= 0.0) { bCheck = FALSE; strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY3_ERROR); }
		if (m_Fy4 <= 0.0) { bCheck = FALSE; strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY4_ERROR); }
		if (m_Fy5 <= 0.0) { bCheck = FALSE; strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY5_ERROR); }
		if (m_Fy6 <= 0.0) { bCheck = FALSE; strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY6_ERROR); }
		if (m_Fy7 <= 0.0) { bCheck = FALSE; strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY7_ERROR); }
		if (m_Fy8 <= 0.0) { bCheck = FALSE; strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY8_ERROR); }
		if (m_Fy9 <= 0.0) { bCheck = FALSE; strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY9_ERROR); }
		if (m_Fy10 <= 0.0) { bCheck = FALSE; strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY10_ERROR); }
	}
	else	ASSERT(0);
	// Check Concrete.
	if(m_cCodeCombo.GetCurSel()==0)
	{
		if(m_cMatName2==_T("")) {bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_CON_MATL_NAME_ERROR);}
	}

	// Change by ZINU.('03.01.27). Check Window Enabled.
	BOOL bUseSubRebar = (GetDlgItem(IDC_DGN_SRC_REBARFYS)->IsWindowEnabled() ? TRUE : FALSE);

	if(m_RebarCode.GetCurSel()==0)
	{
		if(bUseSubRebar)
		{
			if(m_MainRebar2==_T("") || m_SubRebar2==_T(""))
			{
		    bCheck = FALSE;
		    strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_STL_MATL_NAME_ERROR);
			}
		}
		else
		{
			if(m_MainRebar2==_T(""))
			{
		    bCheck = FALSE;
		    strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_STL_MATL_NAME_ERROR);
			}
		}
	}
	else
	{
	  if(m_MainRebar.GetCurSel()==-1)
	  {
		  bCheck = FALSE;	
		  strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_MBAR_ERROR);
	  }
	  if(m_SubRebar.GetCurSel()==-1 && bUseSubRebar)
	  {
		  bCheck = FALSE;	
		  strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_SBAR_ERROR);
	  }
	}

	if(m_Fc <= 0.0) {bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FC_ERROR);}

	CDgnDataCtrl DataCtrl;
	T_DSRC_D rData;
	rData.Initialize();
	DataCtrl.Get_DgnSrcDsrc(rData);
	if(rData.DesignCode==_T("AIJ-SRC01"))
	{
		if(m_Fyr<= 0.0) {bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_MFY_ERROR);}
		if(bUseSubRebar && m_Fys<= 0.0)	
			{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_WFY_ERROR);}
	}
	else
	{
		if(m_Fyr<= 0.0) {bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY_ERROR);}
		if(bUseSubRebar && m_Fys<= 0.0)	
			{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FYS_ERROR);}
	}

	if(!bCheck)	AfxMessageBox(strMsg, MB_OK);

	return bCheck;
}
