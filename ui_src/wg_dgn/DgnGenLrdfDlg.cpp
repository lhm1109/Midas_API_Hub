// DgnGenLrdfDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnGenLrdfDlg.h"

#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"

#include "DgnGenLrdfCaseDlg.h"
#include "DgnDataCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnGenLrdfDlg dialog


CDgnGenLrdfDlg::CDgnGenLrdfDlg(CWnd* pParent /*=NULL*/)
	: CDgnDlgBase(CDgnGenLrdfDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnGenLrdfDlg)
	m_Rmax2 = 0.0;
	m_Rmin2 = 0.0;
	m_strLoadCaseNames = _T("");
	m_strXmax = _T("");
	m_strXmin = _T("");
	m_strYmax = _T("");
	m_strYmin = _T("");
	m_nGenCode = 0;
	m_bReduAll = FALSE;
	m_bReduAxl = FALSE;
	m_bReduMom = FALSE;
	m_bReduShr = FALSE;
	//}}AFX_DATA_INIT
	arRange.Add(IDC_DGN_CTRLDATA_RMAX);
	arRange.Add(IDC_DGN_CTRLDATA_RMIN);
	arRange.Add(IDC_DGN_CTRLDATA_RMAX2);
	arRange.Add(IDC_DGN_CTRLDATA_RMIN2);
}


void CDgnGenLrdfDlg::DoDataExchange(CDataExchange* pDX)
{
	CDgnDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnGenLrdfDlg)
	DDX_Control(pDX, IDC_DGN_NODE_POSITION, m_MousePos);
	DDX_Control(pDX, IDC_DGN_CTRLDATA_STORY, m_StoryCombo);
	DDX_Control(pDX, IDC_DGN_CTRLDATA_RMIN, m_Rmin);
	DDX_Control(pDX, IDC_DGN_CTRLDATA_RMAX, m_Rmax);
	DDX_Control(pDX, IDC_DGN_CTRLDATA_LIST, m_ListCtrl);
	DDX_Text(pDX, IDC_DGN_CTRLDATA_RMAX2, m_Rmax2);
	DDX_Text(pDX, IDC_DGN_CTRLDATA_RMIN2, m_Rmin2);
	DDX_Text(pDX, IDC_DGN_GRECDLIVE_LOADNO, m_strLoadCaseNames);
	DDX_Text(pDX, IDC_DGN_CTRLDATA_XMAX, m_strXmax);
	DDX_Text(pDX, IDC_DGN_CTRLDATA_XMIN, m_strXmin);
	DDX_Text(pDX, IDC_DGN_CTRLDATA_YMAX, m_strYmax);
	DDX_Text(pDX, IDC_DGN_CTRLDATA_YMIN, m_strYmin);
	DDX_Radio(pDX, IDC_DGN_CTRLDATA_GEN, m_nGenCode);
	DDX_Check(pDX, IDC_DGN_REDU_ALL_CHECK, m_bReduAll);
	DDX_Check(pDX, IDC_DGN_REDU_AXL_CHECK, m_bReduAxl);
	DDX_Check(pDX, IDC_DGN_REDU_MOM_CHECK, m_bReduMom);
	DDX_Check(pDX, IDC_DGN_REDU_SHR_CHECK, m_bReduShr);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnGenLrdfDlg, CDgnDlgBase)
	//{{AFX_MSG_MAP(CDgnGenLrdfDlg)
	ON_BN_CLICKED(IDC_DGN_CTRLDATA_ADD, OnDgnCtrldataAdd)
	ON_BN_CLICKED(IDC_DGN_CTRLDATA_DELETE, OnDgnCtrldataDelete)
	ON_BN_CLICKED(IDC_DGN_CTRLDATA_MODIFY, OnDgnCtrldataModify)
	ON_CBN_SELCHANGE(IDC_DGN_CTRLDATA_RMIN, OnSelchangeDgnCtrldataRmin)
	ON_BN_CLICKED(IDC_DGN_CTRLDATA_LOADCOMB, OnDgnCtrldataLoadcomb)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DGN_CTRLDATA_LIST, OnItemchangedDgnCtrldataList)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_DGN_CTRLDATA_RMAX, OnSelchangeDgnCtrldataRmax)
	ON_BN_CLICKED(IDC_DGN_CTRLDATA_CHN, OnDgnCtrldataChn)
	ON_BN_CLICKED(IDC_DGN_CTRLDATA_GEN, OnDgnCtrldataGen)
	ON_BN_CLICKED(IDC_DGN_REDU_ALL_CHECK, OnDgnCtrlReduKind)
	ON_BN_CLICKED(IDC_DGN_REDU_AXL_CHECK, OnDgnCtrlReduKind)
	ON_BN_CLICKED(IDC_DGN_REDU_MOM_CHECK, OnDgnCtrlReduKind)
	ON_BN_CLICKED(IDC_DGN_REDU_SHR_CHECK, OnDgnCtrlReduKind)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_MOUSEEDITENTER,         OnNodeListEnter)
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_MOUSEEDITESCAPE,        OnNodeListEscape)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnGenLrdfDlg message handlers

BOOL CDgnGenLrdfDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDgnDlgBase::OnInitDialog();
	// Change by ZINU.('03.04.15). Agree with Dae-keun, Lee.
	#if defined(_US) || defined(_RUS)
	{
		GetDlgItem(IDC_DGN_CTRLDATA_GEN)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CTRLDATA_CHN)->EnableWindow(FALSE);
	}
	#else
	{
		GetDlgItem(IDC_DGN_CTRLDATA_GEN)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CTRLDATA_CHN)->EnableWindow(TRUE);
	}
	#endif

	m_Rmin.SetCurSel(10);
	m_Rmax.SetCurSel(0);

	Initial_ListCtrlBox();
	Initial_Data();
	Initial_MouseEdit();

	return TRUE;
}

void CDgnGenLrdfDlg::Initial_MouseEdit()
{
	m_MousePos.SetAttNodeList();
	m_MousePos.SetLButtonDownNotifyWindow(this);
	m_MousePos.SetEnterNotifyWindow(this);
	m_MousePos.SetEscNotifyWindow(this);
 	m_MousePos.SetModeToUse(MOUSEEDIT_USE_GET_NODE_LIST);
	m_MousePos.SetMaxNodeKeyNum(2);
}

void CDgnGenLrdfDlg::Add_LiveData(T_STLD_D rData)
{
	_DGN_REDU reData;
	reData.Initialize();

	reData.iLcasNo	 = rData.LoadCaseId;
	reData.sDescript = rData.Description;
	reData.sLcasNa	 = rData.LoadCaseName;
	reData.sLcasType = rData.LoadCaseType;
	m_LiveData.Add(reData);
}

void CDgnGenLrdfDlg::Modify_LiveData(T_STLD_D rData)
{
	int iCount = m_LiveData.GetSize();
	for(int i=0; i<iCount; i++)
	{
		_DGN_REDU reData;
		reData.Initialize();
		reData = m_LiveData.GetAt(i);
		if(reData.iLcasNo==rData.LoadCaseId)  
		{
			reData.iLcasNo	 = rData.LoadCaseId;
			reData.sDescript = rData.Description;
			reData.sLcasNa   = rData.LoadCaseName;
			reData.sLcasType = rData.LoadCaseType;
			m_LiveData.SetAt(i,reData);
		}
	}
}

void CDgnGenLrdfDlg::Del_LiveData(T_STLD_D rData)
{
	CArray<int,int> DelNo;
	int iCount = m_LiveData.GetSize();
	for(int i=0; i<iCount; i++)
	{
		_DGN_REDU reData;
		reData.Initialize();

		reData = m_LiveData.GetAt(i);
		if(reData.iLcasNo==rData.LoadCaseId)  DelNo.Add(i);
	}

	iCount = DelNo.GetSize();
	for(int i=0; i<iCount; i++)
	{
		int N = DelNo.GetAt(i);
		m_LiveData.RemoveAt(N);
	}
}

void CDgnGenLrdfDlg::Initial_Data()
{
	if(m_LiveData.GetSize() > 0)		m_LiveData.RemoveAll();
	if(m_StoryData.GetSize() > 0)		m_StoryData.RemoveAll();
	if(m_StoryCombo.GetCount() > 0)	m_StoryCombo.ResetContent();

	//  Story Name을 초기화합니다.
	CArray<CString, CString&> strStoryList;
	m_pDoc->m_pAttrCtrl->GetStorList(strStoryList);
	int iCount = strStoryList.GetSize();
	for(int i=0; i<iCount; i++)
	{
		CString tempStoryList=_T("");
		tempStoryList = strStoryList.GetAt(i);
		m_StoryCombo.AddString(tempStoryList);
	}

	// 저장되어있는지를 체크하여, Data를 초기화합니다.
	if(m_pDoc->m_pAttrCtrl->ExistDctl())
	{
		T_DCTL_D rData;
		rData.Initialize();
		m_pDoc->m_pAttrCtrl->GetDctl(rData);
		m_nGenCode = rData.nReductType; // 0:General Design Code, 1:Chinese Standard
		DlgCtrl(m_nGenCode);
		// Add by ZINU.('03.02.11). For Applied Components.
		m_bReduAxl = rData.bReduAxl;
		m_bReduMom = rData.bReduMom;
		m_bReduShr = rData.bReduShr;
		m_bReduAll = (m_bReduAxl && m_bReduMom && m_bReduShr ? TRUE : FALSE);
		UpdateReduKindCtrl(m_bReduAll);

		// Change by ZINU.('04.11.24). 인영근DR 오류 (KeyStld는 StldKey가 아니고, Id임).
		T_STLD_D StldD;
		m_strLoadCaseNames = _T("");
		CArray<T_STLD_K,T_STLD_K> arStldK;
		m_pDoc->m_pAttrCtrl->GetStldKeyList(arStldK);
		for(int j=0; j<5; j++)
		{
			for(int k=0; k<arStldK.GetSize(); k++)
			{
				T_STLD_K StldK = arStldK.GetAt(k);
				StldD.Initialize();
				m_pDoc->m_pAttrCtrl->GetStld(StldK,StldD);
				if(rData.KeyStld[j]==StldD.LoadCaseId)
				{
					Add_LiveData(StldD);
					CString strComma = (m_strLoadCaseNames==_T("") ? _T("") : _T(","));
					m_strLoadCaseNames = m_strLoadCaseNames + strComma + StldD.LoadCaseName;
					break;
				}
			}
		}

		int iCount = rData.arReduction.GetSize();
		for(int i=0; i<iCount; i++)
		{
			T_DCTL_BASE bData;
			bData.Initialize();
			bData = rData.arReduction.GetAt(i);

	    m_Xmax  = bData.dMaxX;
			m_Xmin  = bData.dMinX;
	    m_Ymax  = bData.dMaxY;
	    m_Ymin  = bData.dMinY;
	    m_Rmax2 = bData.dRmax;
	    m_Rmin2 = bData.dRmin;
			if(m_nGenCode) {m_Rmax2 = 1.0; m_Rmin2 = 0.55;}

			T_STOR_D sData;
			sData.Initialize();
			m_pDoc->m_pAttrCtrl->GetStor(bData.KeyStory,sData);
			CString Story = sData.StoryName;

			m_StoryKey = bData.KeyStory;
			m_strXmax.Format(_T("%g"),m_Xmax);//bData.dMaxX);
			m_strXmin.Format(_T("%g"),m_Xmin);//bData.dMinX);
			m_strYmax.Format(_T("%g"),m_Ymax);//bData.dMaxY);
			m_strYmin.Format(_T("%g"),m_Ymin);//bData.dMinY);
			CString Rmax=_T(""); Rmax.Format(_T("%.2f"),m_Rmax2);//bData.dRmax);
			CString Rmin=_T(""); Rmin.Format(_T("%.2f"),m_Rmin2);//bData.dRmin);

			AddStoryData();
			int ListLineNo = m_ListCtrl.GetItemCount();
			Write_ListCtrlBox(m_nGenCode,ListLineNo,Story,m_strXmin,m_strXmax,m_strYmin,m_strYmax,Rmax,Rmin);
		}
	}
	m_strXmax = _T("0.00");
	m_strXmin = _T("0.00");
	m_strYmax = _T("0.00");
	m_strYmin = _T("0.00");
	m_Rmax2 = 1.0;
	m_Rmin2 = 0.5;

	m_Rmin.SetCurSel(10);
	m_Rmax.SetCurSel(0);
	if(m_nGenCode) {m_Rmin2 = 0.55; m_Rmin.SetCurSel(9);}

	UpdateData(FALSE);
}

void CDgnGenLrdfDlg::Initial_ListCtrlBox()
{
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_ListCtrl.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_ListCtrl.GetSafeHwnd(),dwStyle);

	LV_COLUMN lvcolumn;
	TCHAR *list[6]	= {(TCHAR*)(LPCTSTR)_LS(IDS_DGN_LIST_STORY),
									 _T("Xmin"),
									 _T("Xmax"),
									 _T("Ymin"),
									 _T("Ymax"),
									 (TCHAR*)(LPCTSTR)_LS(IDS_DGN_LIST_RANGE)};
	int width[6]	= {50,65,65,65,65,95};
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

void CDgnGenLrdfDlg::Write_ListCtrlBox(int nCH, int Index,CString strStory,CString strXmin,
																			CString strXmax,CString strYmin,CString strYmax,
																			CString strRmax,CString strRmin)
{
	CString strRange=_T("");
	if(nCH) strRange = _T("Auto");//0: General Design Code, 1: Chinese Standard
	else strRange = strRmax+_T("-")+strRmin;

	LV_ITEM lvitem;

	//  ListCtrl Box에 값을 입력합니다.
	lvitem.mask		  = LVIF_TEXT;
	lvitem.iItem  	= Index;
	lvitem.iSubItem = 0;
	lvitem.pszText	= (LPTSTR)(LPCTSTR)strStory;
	m_ListCtrl.InsertItem(&lvitem);

	lvitem.mask 		= LVIF_TEXT;
	lvitem.iItem  	= Index;
	lvitem.iSubItem = 1;
	lvitem.pszText	= (LPTSTR)(LPCTSTR)strXmin;
	m_ListCtrl.SetItem(&lvitem);

	lvitem.mask		  = LVIF_TEXT;
	lvitem.iItem	  = Index;
	lvitem.iSubItem = 2;
	lvitem.pszText	= (LPTSTR)(LPCTSTR)strXmax;
	m_ListCtrl.SetItem(&lvitem);

	lvitem.mask 		= LVIF_TEXT;
	lvitem.iItem  	= Index;
	lvitem.iSubItem = 3;
	lvitem.pszText	= (LPTSTR)(LPCTSTR)strYmin;
	m_ListCtrl.SetItem(&lvitem);

	lvitem.mask		  = LVIF_TEXT;
	lvitem.iItem	  = Index;
	lvitem.iSubItem = 4;
	lvitem.pszText	= (LPTSTR)(LPCTSTR)strYmax;
	m_ListCtrl.SetItem(&lvitem);

	lvitem.mask 		= LVIF_TEXT;
	lvitem.iItem  	= Index;
	lvitem.iSubItem = 5;
	lvitem.pszText	= (LPTSTR)(LPCTSTR)strRange;
	m_ListCtrl.SetItem(&lvitem);
}

void CDgnGenLrdfDlg::AddStoryData()
{
	_DGN_STOR rData;
	rData.Initialize();

	rData.KeyStory = m_StoryKey;
	rData.dMinX    = m_Xmin;
	rData.dMaxX    = m_Xmax;
	rData.dMinY    = m_Ymin;
	rData.dMaxY    = m_Ymax;
	rData.dMinR    = m_Rmin2;
	rData.dMaxR    = m_Rmax2;

	m_StoryData.Add(rData);
}

void CDgnGenLrdfDlg::DeleteStoryData(int Index)
{
	if(m_pDoc->m_pAttrCtrl->ExistDctl())
	{
		T_DCTL_D rData;
		rData.Initialize();
		m_pDoc->m_pAttrCtrl->GetDctl(rData);
		rData.nReductType = m_nGenCode;
		rData.bReduAxl		= (m_bReduAll ? TRUE : m_bReduAxl);
		rData.bReduMom		= (m_bReduAll ? TRUE : m_bReduMom);
		rData.bReduShr		= (m_bReduAll ? TRUE : m_bReduShr);

		int iCount = rData.arReduction.GetSize();
		int DeleteIndex=-1;
		for(int i=0; i<iCount; i++)
		{
			T_DCTL_BASE bData;
			bData.Initialize();
			bData = rData.arReduction.GetAt(i);

			_DGN_STOR sData;
			sData.Initialize();
			sData = m_StoryData.GetAt(Index);

			if(sData.KeyStory == bData.KeyStory)  DeleteIndex = i;
		}
		if(DeleteIndex != -1) rData.arReduction.RemoveAt(DeleteIndex);
		m_pDoc->m_pDataCtrl->AddDctl(rData);
		m_StoryData.RemoveAt(Index);
	}
	else	AfxMessageBox(_LS(IDS_DGN_CHK_SEL_ITEM),MB_OK);
}

void CDgnGenLrdfDlg::ModifyStoryData(int Index)
{
	// Change by ZINU.('02.9.26).
	_DGN_STOR rData;
	rData.Initialize();

	int Index2 = m_StoryCombo.GetCurSel();
	if(Index2 != -1)
	{
		CString StoryName=_T("");
		m_StoryCombo.GetLBText(Index2,StoryName);

		rData.KeyStory = m_pDoc->m_pAttrCtrl->GetStorKey(StoryName);
		rData.dMinX    = m_Xmin;
		rData.dMaxX    = m_Xmax;
		rData.dMinY    = m_Ymin;
		rData.dMaxY    = m_Ymax;
		rData.dMinR    = m_Rmin2;
		rData.dMaxR    = m_Rmax2;
		// Modify Data.
		m_StoryData.SetAt(Index,rData);

		if(m_pDoc->m_pAttrCtrl->ExistDctl())
		{
			T_DCTL_D cData;
			cData.Initialize();
			m_pDoc->m_pAttrCtrl->GetDctl(cData);
			cData.nReductType = m_nGenCode;
			cData.bReduAxl		= (m_bReduAll ? TRUE : m_bReduAxl);
			cData.bReduMom		= (m_bReduAll ? TRUE : m_bReduMom);
			cData.bReduShr		= (m_bReduAll ? TRUE : m_bReduShr);
			// Set Modified Data at DB.
			T_DCTL_BASE bData;
			bData.Initialize();
			bData = cData.arReduction.GetAt(Index);
			bData.KeyStory = rData.KeyStory;
			bData.dMinX    = rData.dMinX;
			bData.dMaxX    = rData.dMaxX;
			bData.dMinY    = rData.dMinY;
			bData.dMaxY    = rData.dMaxY;
			bData.dRmin    = rData.dMinR;
			bData.dRmax    = rData.dMaxR;
			
			cData.arReduction.SetAt(Index,bData);
			
			m_pDoc->m_pDataCtrl->AddDctl(cData);
		}
		else	AfxMessageBox(_LS(IDS_DGN_CHK_SEL_ITEM),MB_OK);
	}
	else	AfxMessageBox(_LS(IDS_DGN_CHK_FIND),MB_OK);
}

void CDgnGenLrdfDlg::OnDgnCtrldataAdd() 
{
	int ListLineNo = m_ListCtrl.GetItemCount();

	UpdateData(TRUE);
	int Index = m_StoryCombo.GetCurSel();
	CString Story=_T("");
	m_StoryCombo.GetLBText(Index,Story);

	m_StoryKey = m_pDoc->m_pAttrCtrl->GetStorKey(Story);

	if(m_strXmax==_T(""))	m_strXmax = _T("0.00");
	if(m_strXmin==_T(""))	m_strXmin = _T("0.00");
	if(m_strYmax==_T(""))	m_strYmax = _T("0.00");
	if(m_strYmin==_T(""))	m_strYmin = _T("0.00");

	m_Xmin = _tstof(m_strXmin);
	m_Xmax = _tstof(m_strXmax);
	m_Ymin = _tstof(m_strYmin);
	m_Ymax = _tstof(m_strYmax);
	CString Rmin=_T(""); Rmin.Format(_T("%.2f"),m_Rmin2);
	CString Rmax=_T(""); Rmax.Format(_T("%.2f"),m_Rmax2);
	////////////////////////////////////////////////////////////////////
	//  DB에 저장합니다.
	T_DCTL_D rData;
	rData.Initialize();
	if(m_pDoc->m_pAttrCtrl->ExistDctl())  
	{
		m_pDoc->m_pAttrCtrl->GetDctl(rData);
	}
	rData.nReductType = m_nGenCode;
	rData.bReduAxl		= (m_bReduAll ? TRUE : m_bReduAxl);
	rData.bReduMom		= (m_bReduAll ? TRUE : m_bReduMom);
	rData.bReduShr		= (m_bReduAll ? TRUE : m_bReduShr);

	T_DCTL_BASE bData;
	bData.Initialize();
	bData.KeyStory = m_StoryKey;
	bData.dMaxX    = m_Xmax;
	bData.dMaxY    = m_Ymax;
	bData.dMinX    = m_Xmin;
	bData.dMinY    = m_Ymin;
	bData.dRmax    = m_Rmax2;
	bData.dRmin    = m_Rmin2;

	rData.arReduction.Add(bData);

	if(m_pDoc->m_pDataCtrl->AddDctl(rData))
	{
		Write_ListCtrlBox(m_nGenCode,ListLineNo,Story,m_strXmin,m_strXmax,m_strYmin,m_strYmax,Rmax,Rmin);
		AddStoryData();
	}
}

void CDgnGenLrdfDlg::OnDgnCtrldataDelete() 
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
	if (nItem == -1) 
	{
		AfxMessageBox(_LS(IDS_DGN_CHK_SEL_ITEM));
		return;
	}
	m_ListCtrl.DeleteItem(nItem);
	DeleteStoryData(nItem);

	int nCount = m_ListCtrl.GetItemCount();
	if (nCount <= nItem) nItem -= 1;
	if (nItem >= 0) 
		m_ListCtrl.SetItemState(nItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CDgnGenLrdfDlg::OnDgnCtrldataModify() 
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
	if (nItem == -1) 
	{
		AfxMessageBox(_LS(IDS_DGN_CHK_SEL_ITEM));
		return;
	}

	UpdateData(TRUE);
	int Index = m_StoryCombo.GetCurSel();
	CString Story=_T("");
	m_StoryCombo.GetLBText(Index,Story);

	if(m_strXmax==_T(""))  m_strXmax = _T("0.00");
	if(m_strXmin==_T(""))  m_strXmin = _T("0.00");
	if(m_strYmax==_T(""))  m_strYmax = _T("0.00");
	if(m_strYmin==_T(""))  m_strYmin = _T("0.00");

	m_Xmin = _tstof(m_strXmin);
	m_Xmax = _tstof(m_strXmax);
	m_Ymin = _tstof(m_strYmin);
	m_Ymax = _tstof(m_strYmax);
	CString Rmin=_T(""); Rmin.Format(_T("%.2f"),m_Rmin2);
	CString Rmax=_T(""); Rmax.Format(_T("%.2f"),m_Rmax2);

	ModifyStoryData(nItem);
	//m_ListCtrl.DeleteItem(nItem);

	CString strRange;
	if(m_nGenCode) strRange = _T("Auto");
	else           strRange = Rmax + _T("-") + Rmin;

	m_ListCtrl.SetItemText(nItem,0,Story);
	m_ListCtrl.SetItemText(nItem,1,m_strXmin);
	m_ListCtrl.SetItemText(nItem,2,m_strXmax);
	m_ListCtrl.SetItemText(nItem,3,m_strYmin);
	m_ListCtrl.SetItemText(nItem,4,m_strYmax);
	m_ListCtrl.SetItemText(nItem,5,strRange);

	//Write_ListCtrlBox(nItem,Story,m_strXmin,m_strXmax,m_strYmin,m_strYmax,Rmax,Rmin);
	m_ListCtrl.SetItemState(nItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CDgnGenLrdfDlg::OnDestroy() 
{
	CDgnDlgBase::OnDestroy();
	
	// TODO: Add your message handler code here
	I_GENModelBase::SetModelingModeST(IG_STATE_INITALL);
}

void CDgnGenLrdfDlg::OnOK() 
{
	// Add by ZINU.('04.11.25). OnDgnCtrldataAdd() -> OnOK().
	T_DCTL_D DctlD;
	DctlD.Initialize();
	m_pDoc->m_pAttrCtrl->GetDctl(DctlD);
	for(int i=0; i<m_LiveData.GetSize(); i++)
	{
		_DGN_REDU DgnRedu = m_LiveData.GetAt(i);
		DctlD.KeyStld[i] = DgnRedu.iLcasNo;
	}
	m_pDoc->m_pDataCtrl->AddDctl(DctlD);

	DestroyWindow();
	//CDgnDlgBase::OnOK();
}

void CDgnGenLrdfDlg::OnCancel() 
{
	DestroyWindow();
	//CDgnDlgBase::OnCancel();
}

BOOL CDgnGenLrdfDlg::ErrorCheck()
{
	BOOL bCheck=TRUE;
	CString str=_T("");
	int Index = m_Rmin.GetCurSel();
	if(Index != -1 ) m_Rmin.GetLBText(Index,str);
	double Rmin = _tstof(str);

	Index = m_Rmax.GetCurSel();
	if(Index != -1 ) m_Rmax.GetLBText(Index,str);
	double Rmax = _tstof(str);

	CString strCombo=_T("");
	Index = m_StoryCombo.GetCurSel();
	if(Index != -1 ) m_StoryCombo.GetLBText(Index,strCombo);

	if(strCombo == _T(""))            bCheck = FALSE;
	if(Rmin > 1. || Rmin < 0.5)   bCheck = FALSE;
	if(Rmax > 1. || Rmax < 0.5)   bCheck = FALSE;
	if(m_strLoadCaseNames == _T(""))  bCheck = FALSE;

	if(!bCheck)	AfxMessageBox(_LS(IDS_DGN_CHK_REDUCT_DATA),MB_OK);

	return bCheck;
}

void CDgnGenLrdfDlg::OnSelchangeDgnCtrldataRmin() 
{
	int Index = m_Rmin.GetCurSel();
	if(Index != -1)
	{
		UpdateData(TRUE);

		CString strRmin=_T("");
		m_Rmin.GetLBText(Index,strRmin);
		m_Rmin2 = _tstof(strRmin);

		UpdateData(FALSE);
	}
}

void CDgnGenLrdfDlg::OnItemchangedDgnCtrldataList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
 	int ItemNo = pNMListView->iItem;

	Select_ListCtrlItem(ItemNo);

	*pResult = 0;
}

void CDgnGenLrdfDlg::Select_ListCtrlItem(int nItem)
{
	if(nItem != -1)
	{
		_DGN_STOR sData;
		sData.Initialize();

		sData = m_StoryData.GetAt(nItem);

		T_STOR_D rData;
		rData.Initialize();
		m_pDoc->m_pAttrCtrl->GetStor(sData.KeyStory,rData);
		int Index = m_StoryCombo.FindStringExact(-1,rData.StoryName);
		m_StoryCombo.SetCurSel(Index);

		m_Xmin  = sData.dMinX;
		m_Xmax  = sData.dMaxX;
		m_Ymin  = sData.dMinY;
		m_Ymax  = sData.dMaxY;
		m_Rmax2 = sData.dMaxR;
		m_Rmin2 = sData.dMinR;
		if(m_nGenCode) {m_Rmax2 = 1.0; m_Rmin2 = 0.55;}

		m_strXmax.Format(_T("%g"),sData.dMaxX);
		m_strXmin.Format(_T("%g"),sData.dMinX);
		m_strYmax.Format(_T("%g"),sData.dMaxY);
		m_strYmin.Format(_T("%g"),sData.dMinY);

		Index=0;
		CString str=_T("");

		str.Format(_T("%.2f"),m_Rmax2);
		Index = m_Rmax.FindStringExact(-1,str);
		m_Rmax.SetCurSel(Index);

		str.Format(_T("%.2f"),m_Rmin2);
		Index = m_Rmin.FindStringExact(-1,str);
		m_Rmin.SetCurSel(Index);

		UpdateData(FALSE);
	}
}

void CDgnGenLrdfDlg::OnDgnCtrldataLoadcomb() 
{
	UpdateData(TRUE);

	CDgnGenLrdfCaseDlg dlg;
	dlg.m_LoadCaseNames = m_strLoadCaseNames;
	if(dlg.DoModal()==IDOK)
	{
		m_strLoadCaseNames = dlg.m_LoadCaseNames;

		int iCount = dlg.m_SelLiveData.GetSize();
		m_LiveData.RemoveAll();
		for(int i=0; i<iCount; i++)
		{
			_DGN_REDU rData = dlg.m_SelLiveData.GetAt(i);
			m_LiveData.Add(rData);
		}
		UpdateData(FALSE);
	}
}

LRESULT CDgnGenLrdfDlg::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
{
	if(m_MousePos.GetSafeHwnd() == (HWND)lParam)
	{
		CString str;
		m_MousePos.GetWindowText(str);
		CArray<unsigned int, unsigned int> m_NodeNoList;
		if(CStrParser::GetNodeList(str,m_NodeNoList))
	  {
	    if(m_NodeNoList.GetSize() == 2)
			{
				int NodeNo1 = m_NodeNoList.GetAt(0);
				int NodeNo2 = m_NodeNoList.GetAt(1);

				Search_NodeMaxMinCoord(NodeNo1,NodeNo2);
			}
		}
	}
	return 0L;
}

LRESULT CDgnGenLrdfDlg::OnNodeListEnter(WPARAM wParam, LPARAM lParam)
{
	if(m_MousePos.GetSafeHwnd() == (HWND)lParam)
	{
		//CString str;
		//m_MousePos.GetWindowText(str);     
		//CArray<unsigned int, unsigned int> m_iaJointList;
		//if(CStrParser::GetNodeList(str,m_iaJointList))
		//{
		//  GSaveHistoryFormatNF(_T("Node Key iCount: %d"),m_iaJointList.GetSize());
		//}
		//else
		//{
		//  GSaveHistoryNF(_T("Node List Error !!!"));
		//}
	}
	return 0L;
}

LRESULT CDgnGenLrdfDlg::OnNodeListEscape(WPARAM wParam, LPARAM lParam)
{
	if(m_MousePos.GetSafeHwnd() == (HWND)lParam)
	{
		//GSaveHistoryNF(_T("Escape !!!"));          
		//m_MousePos.SetWindowText(_T(""));
	}
	return 0L;
}

void CDgnGenLrdfDlg::Search_NodeMaxMinCoord(int nNo1,int nNo2)
{
	T_NODE_D nData1,nData2;
	nData1.Initialize();
	nData2.Initialize();

	m_pDoc->m_pAttrCtrl->GetNode(nNo1,nData1);
	m_pDoc->m_pAttrCtrl->GetNode(nNo2,nData2);

	m_Xmax = max(nData1.x,nData2.x);
	m_Xmin = min(nData1.x,nData2.x);
	m_Ymax = max(nData1.y,nData2.y);
	m_Ymin = min(nData1.y,nData2.y);

	m_strXmax.Format(_T("%g"),m_Xmax);
	m_strXmin.Format(_T("%g"),m_Xmin);
	m_strYmax.Format(_T("%g"),m_Ymax);
	m_strYmin.Format(_T("%g"),m_Ymin);

	UpdateData(FALSE);
}

void CDgnGenLrdfDlg::ConvertUnitListCtrlData()
{
	int iCount = m_ListCtrl.GetItemCount();
	for(int i=0; i<iCount; i++)
	{
		CString Story      = m_ListCtrl.GetItemText(i,0);
		CString Xmin       = m_ListCtrl.GetItemText(i,1);
		CString Xmax       = m_ListCtrl.GetItemText(i,2);
		CString Ymin       = m_ListCtrl.GetItemText(i,3);
		CString Ymax       = m_ListCtrl.GetItemText(i,4);
		CString TotalRatio = m_ListCtrl.GetItemText(i,5);

		m_Xmin  = _tstof(Xmin);
		m_Xmax  = _tstof(Xmax);
		m_Ymin  = _tstof(Ymin);
		m_Ymax  = _tstof(Ymax);

		m_Xmin = CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertUnitLengthPrevious(m_Xmin);
		m_Xmax = CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertUnitLengthPrevious(m_Xmax);
		m_Ymin = CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertUnitLengthPrevious(m_Ymin);
		m_Ymax = CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertUnitLengthPrevious(m_Ymax);

		m_Rmin2 = _tstof(TotalRatio.Right(4));
		m_Rmax2 = _tstof(TotalRatio.Left(4));

		m_strXmin.Format(_T("%g"),m_Xmin);
		m_strXmax.Format(_T("%g"),m_Xmax);
		m_strYmin.Format(_T("%g"),m_Ymin);
		m_strYmax.Format(_T("%g"),m_Ymax);
		CString Rmin=_T(""); Rmin.Format(_T("%.2f"),m_Rmin2);
		CString Rmax=_T(""); Rmax.Format(_T("%.2f"),m_Rmax2);

		ModifyStoryData(i);
		m_ListCtrl.DeleteItem(i);
		Write_ListCtrlBox(m_nGenCode,i,Story,m_strXmin,m_strXmax,m_strYmin,m_strYmax,Rmax,Rmin);
	}
	m_strXmax = _T("0.00");
	m_strXmin = _T("0.00");
	m_strYmax = _T("0.00");
	m_strYmin = _T("0.00");
	m_Rmax2 = 1.0;
	m_Rmin2 = 0.5;

	m_Rmin.SetCurSel(10);
	m_Rmax.SetCurSel(0);

	UpdateData(FALSE);
}

void CDgnGenLrdfDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
		case D_UPDATE_DEFAULT:
			// do something...
			break;
		case D_UPDATE_BUFFER_BEFORE:
			// do something...
			break;
		case D_UPDATE_UNIT:
		{
			ConvertUnitListCtrlData();
		}break;
		case D_UPDATE_SEL_ADD:
			// do something...
			break;
		case D_UPDATE_SEL_DEL:
			// do something...
			break;
		case D_UPDATE_BUFFER_AFTER:
			UpdateBuffer();
			break;
		default:
			//ASSERT(FALSE);
			break;
	  }
}

void CDgnGenLrdfDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if(nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_STLD_K Key;
	T_STLD_D Data;
	Data.Initialize();

	CArray<int,int> arKey;
	arKey.RemoveAll();
	BOOL bUR_STOR_MFS = FALSE;
	BOOL bUR_STLD_DEL = FALSE;
	BOOL bUR_STLD_MFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		arKey.Add(nKey);		

		switch(nCmd)
		{
			case(UR_STOR_ADD):
			case(UR_STOR_DEL):
			case(UR_STOR_MFD):
			case(UR_STOR_MFS):	{bUR_STOR_MFS = TRUE;	break;}
			case(UR_STLD_ADD):
			case(UR_STLD_DEL):	{bUR_STLD_DEL = TRUE;	break;}
			case(UR_STLD_MFD):	{bUR_STLD_MFD = TRUE;	break;}
			case(UR_STLD_MFS):	// 바꾸기전 Data를 얻어올 수 있습니다.
			default:	break;
		}
	}
	if(bUR_STOR_MFS)	
	{
		Initial_StoryCombo();
		Initial_ListBox();
	}
	if(bUR_STLD_DEL || bUR_STLD_MFD)
	{
		for(int i=0; i<arKey.GetSize(); i++)	pViewBuff->GetStld(arKey.GetAt(i), Key, Data);
	}
	if(bUR_STLD_DEL)	Del_SelLiveLoad(Key, Data);
	if(bUR_STLD_MFD)	Modify_SelLiveLoad(Key, Data);
}

void CDgnGenLrdfDlg::Modify_SelLiveLoad(T_STLD_K key,T_STLD_D data)
{
	int iCount,i;

	UpdateData(TRUE);
	//  Select된 놈의 키를 알아옵니다.
	CArray<int,int> NoList;
	Cutting_SelLiveLoad(m_strLoadCaseNames,NoList);
	m_strLoadCaseNames=_T("");

	if(data.LoadCaseType == _T("L"))
	{
		iCount = NoList.GetSize();
		for(i=0; i<iCount; i++)
		{
			int N = NoList.GetAt(i);
			if(data.LoadCaseId == N)  Modify_LiveData(data);
		}
	}
	else
	{
		iCount = NoList.GetSize();
		for(i=0; i<iCount; i++)
		{
			int N = NoList.GetAt(i);
			if(data.LoadCaseId == N)  Del_LiveData(data);
		}
	}

	//  Select된 놈의 이름을 작성합니다.
	iCount = m_LiveData.GetSize();
	for(i=0; i<iCount; i++)
	{
		_DGN_REDU reData;
		reData.Initialize();

		reData = m_LiveData.GetAt(i);
		if(m_strLoadCaseNames==_T(""))	m_strLoadCaseNames = reData.sLcasNa;
		else                        m_strLoadCaseNames = m_strLoadCaseNames+_T(",")+reData.sLcasNa;
	}
	GetDlgItem(IDC_DGN_GRECDLIVE_LOADNO)->SetWindowText(m_strLoadCaseNames);
}

void CDgnGenLrdfDlg::Del_SelLiveLoad(T_STLD_K key,T_STLD_D data)
{
	UpdateData(TRUE);
	CArray<CString,CString> NameList;
	Cutting_SelLiveLoad(m_strLoadCaseNames,NameList);
	m_strLoadCaseNames=_T("");

	if(data.LoadCaseType == _T("L"))
	{
		int iCount = NameList.GetSize();
		for(int i=0; i<iCount; i++)
		{
			CString strName=_T("");
			strName = NameList.GetAt(i);
			if(data.LoadCaseName == strName)
			{
				Del_LiveData(data);
			}
			else
			{
				if(m_strLoadCaseNames == _T(""))  m_strLoadCaseNames = strName;
				else                          m_strLoadCaseNames = m_strLoadCaseNames+_T(",")+strName;
			}
		}
		GetDlgItem(IDC_DGN_GRECDLIVE_LOADNO)->SetWindowText(m_strLoadCaseNames);
	}
}

void CDgnGenLrdfDlg::Cutting_SelLiveLoad(CString strSelLoadCases,CArray<int,int>& List)
{
	if(List.GetSize() != 0) List.RemoveAll();

	int iCount = m_LiveData.GetSize();
	for(int i=0; i<iCount; i++)
	{
		_DGN_REDU reData;
		reData.Initialize();
		reData = m_LiveData.GetAt(i);
		List.Add(reData.iLcasNo);
	}
}

void CDgnGenLrdfDlg::Cutting_SelLiveLoad(CString strSelLoadCases,CArray<CString,CString>& List)
{
	if(List.GetSize() != 0) List.RemoveAll();

	CString temp=_T("");
	int Len = strSelLoadCases.GetLength();
	for(int i=0; i<Len; i++)
	{
		TCHAR ch = strSelLoadCases.GetAt(i);
		if(ch == ',')
		{
			List.Add(temp);
			temp = _T("");
		}
		else
		{
			temp=temp+ch;
			if(i+1 == Len)
			{
				List.Add(temp);
				temp = _T("");
			}
		}
	}
}

void CDgnGenLrdfDlg::Initial_StoryCombo()
{
	m_StoryCombo.ResetContent();

	//  Story Name을 초기화합니다.
	CArray<CString, CString&> strStoryList;
	m_pDoc->m_pAttrCtrl->GetStorList(strStoryList);
	int iCount = strStoryList.GetSize();
	for(int i=0; i<iCount; i++)
	{
		CString tempStoryList=_T("");
		tempStoryList = strStoryList.GetAt(i);
		m_StoryCombo.AddString(tempStoryList);
	}
}

void CDgnGenLrdfDlg::Initial_ListBox()
{
	if(m_StoryData.GetSize() > 0)			m_StoryData.RemoveAll();
	if(m_ListCtrl.GetItemCount() > 0)	m_ListCtrl.DeleteAllItems();

	T_DCTL_D rData;
	rData.Initialize();
	m_pDoc->m_pAttrCtrl->GetDctl(rData);

	int iCount = rData.arReduction.GetSize();
	for(int i=0; i<iCount; i++)
	{
		T_DCTL_BASE bData;
		bData.Initialize();
		bData = rData.arReduction.GetAt(i);

	  m_Xmax  = bData.dMaxX;
	  m_Xmin  = bData.dMinX;
	  m_Ymax  = bData.dMaxY;
	  m_Ymin  = bData.dMinY;
	  m_Rmax2 = bData.dRmax;
	  m_Rmin2 = bData.dRmin;
		if(m_nGenCode) {m_Rmax2 = 1.0; m_Rmin2 = 0.55;}

		CString Story=_T(""), Xmin=_T(""), Rmin=_T(""), Rmax=_T("");
		T_STOR_D sData;
		sData.Initialize();
		m_pDoc->m_pAttrCtrl->GetStor(bData.KeyStory,sData);

		Story = sData.StoryName;
		m_StoryKey = bData.KeyStory;
		m_strXmax.Format(_T("%g"),m_Xmax);//bData.dMaxX);
		m_strXmin.Format(_T("%g"),m_Xmin);//bData.dMinX);
		m_strYmax.Format(_T("%g"),m_Ymax);//bData.dMaxY);
		m_strYmin.Format(_T("%g"),m_Ymin);//bData.dMinY);
		Rmax.Format(_T("%.2f"),m_Rmax2);//bData.dRmax);
		Rmin.Format(_T("%.2f"),m_Rmin2);//bData.dRmin);

		AddStoryData();
		int ListLineNo = m_ListCtrl.GetItemCount();
		Write_ListCtrlBox(m_nGenCode,ListLineNo,Story,m_strXmin,m_strXmax,m_strYmin,m_strYmax,Rmax,Rmin);
	}
	m_strXmax = _T("0.00");
	m_strXmin = _T("0.00");
	m_strYmax = _T("0.00");
	m_strYmin = _T("0.00");
	m_Rmax2 = 1.0;
	m_Rmin2 = 0.5;

	m_Rmin.SetCurSel(10);
	m_Rmax.SetCurSel(0);
	if(m_nGenCode){m_Rmin2 = 0.55; m_Rmin.SetCurSel(9);}

	UpdateData(FALSE);
}

void CDgnGenLrdfDlg::OnSelchangeDgnCtrldataRmax() 
{
	// TODO: Add your control notification handler code here
	int Index = m_Rmax.GetCurSel();
	if(Index != -1)
	{
		UpdateData(TRUE);

		CString strRmax=_T("");
		m_Rmax.GetLBText(Index,strRmax);
		m_Rmax2 = _tstof(strRmax);

		UpdateData(FALSE);
	}
}

void CDgnGenLrdfDlg::DlgCtrl(int nCtrl)
{
	if(nCtrl) CDlgUtil::CtrlEnableDisable(this, arRange, FALSE);
	else      CDlgUtil::CtrlEnableDisable(this, arRange, TRUE);
}

void CDgnGenLrdfDlg::OnDgnCtrldataChn() 
{
	// TODO: Add your control notification handler code here
	m_nGenCode = 1;
	CDlgUtil::CtrlEnableDisable(this, arRange, FALSE);

	CString strRange = _T("Auto");//0: General Design Code, 1: Chinese Standard
	
	m_Rmax2 = 1.0;
	m_Rmin2 = 0.55;

	m_Rmin.SetCurSel(9);
	m_Rmax.SetCurSel(0);

	int Index = m_ListCtrl.GetItemCount();
	LV_ITEM lvitem;
	for (int i=0; i<Index ; i++)
	{
		lvitem.mask 		= LVIF_TEXT;
	  lvitem.iItem  	= i;
	  lvitem.iSubItem = 5;
	  lvitem.pszText	= (LPTSTR)(LPCTSTR)strRange;
	  m_ListCtrl.SetItem(&lvitem);
	}
	UpdateData(FALSE);
}

void CDgnGenLrdfDlg::OnDgnCtrldataGen() 
{
	// TODO: Add your control notification handler code here
	m_nGenCode = 0;
	CDlgUtil::CtrlEnableDisable(this, arRange, TRUE);

	m_Rmax2 = 1.0;
	m_Rmin2 = 0.5;
	CString strRmax; strRmax.Format(_T("%.2f"),m_Rmax2);
	CString strRmin; strRmin.Format(_T("%.2f"),m_Rmin2);
	CString strRange = strRmax+_T("-")+strRmin;

	m_Rmin.SetCurSel(10);
	m_Rmax.SetCurSel(0);
	int Index = m_ListCtrl.GetItemCount();
	LV_ITEM lvitem;
	for (int i=0; i<Index ; i++)
	{
		lvitem.mask 		= LVIF_TEXT;
	  lvitem.iItem  	= i;
	  lvitem.iSubItem = 5;
	  lvitem.pszText	= (LPTSTR)(LPCTSTR)strRange;
	  m_ListCtrl.SetItem(&lvitem);
	}
	UpdateData(FALSE);
}

void CDgnGenLrdfDlg::OnDgnCtrlReduKind()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	UpdateReduKindCtrl(m_bReduAll);
	UpdateData(FALSE);
}

BOOL CDgnGenLrdfDlg::UpdateReduKindCtrl(BOOL bReduAll)
{
	if(bReduAll)
	{
		m_bReduAxl = TRUE;
		m_bReduMom = TRUE;
		m_bReduShr = TRUE;
		GetDlgItem(IDC_DGN_REDU_AXL_CHECK)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_REDU_MOM_CHECK)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_REDU_SHR_CHECK)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_DGN_REDU_AXL_CHECK)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_REDU_MOM_CHECK)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_REDU_SHR_CHECK)->EnableWindow(TRUE);
	}
	return TRUE;
}
