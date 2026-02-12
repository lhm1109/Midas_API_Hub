#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdSFRLoadDlg.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UndoCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\ViewCtrl.h"
#include "..\wg_db\PostCtrl.h"
#include "..\wg_base\DlgUtil.h"
#include "..\wg_base\I_GENModelBase.h"
#include "..\wg_base\NumericOptimizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define EPSILON 1.e-12
#define DATA_FMT _T("%.15g")
#define DATA_INT _T("%d")

CString CSFR_RDFGrid::m_aCurDefVal[] = { _T("0.0"), _T("0.0") ,_T("0.0"),_T("0.0"),};
CSFR_RDFGrid::CSFR_RDFGrid()
{
	m_pData = 0;
	m_nCurMode = 0;
}

CSFR_RDFGrid::~CSFR_RDFGrid()
{
}

BEGIN_MESSAGE_MAP(CSFR_RDFGrid, CTBCommon)
END_MESSAGE_MAP()

void CSFR_RDFGrid::Initialize(CArray<SFRGridData, SFRGridData &>* pData,int* nCurMode)
{
	ASSERT(pData);
	m_pData = pData;
	m_nCurMode = nCurMode;
	T_UNIT_SYSTEM us;
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	CTBCommon::Initialize(pDoc, us);


	GetParam()->EnableUndo(FALSE);
	//EnableScrollTips();
	//EnableHints();
	GetParam()->EnableTrackRowHeight(FALSE);
	GetParam()->EnableMoveCols(FALSE);        // columns can not be dragged

	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetMarkColHeader(FALSE); // Turn off pressed button effect for column headers
	pProp->SetMarkRowHeader(FALSE);
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL,
		(CGXStyle)pProp->sInvertNoBorder);

	LockUpdate(TRUE);

	
	SetHeaderTitle(us, TRUE);

	ChangeStandardStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetSize(9))
		.SetVerticalAlignment(DT_BOTTOM)
		.SetAllowEnter(FALSE));
	ChangeColHeaderStyle(CGXStyle().SetFont(CGXFont_GC().SetBold(FALSE)));
	ChangeRowHeaderStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetBold(FALSE))
		.SetHorizontalAlignment(DT_RIGHT)
		.SetControl(GX_IDS_CTRL_HEADER));
	GetParam()->SetNumberedRowHeaders(TRUE);

	//SetStyleRange(CGXRange().SetCols(1, m_nColCount), CGXStyle()
	//	.SetHorizontalAlignment(DT_RIGHT)
	//	.SetValueType(GX_VT_NUMERIC)
	//	.SetFormat(GX_FMT_FIXED)
	//	.SetPlaces(4));

	SetRowHeightByDPI(0, 0, 18);

	SetStyleRange(CGXRange().SetCols(2), CGXStyle()
		.SetPlaces(4));

	SetRightValidCell(1, 0);
	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);

	m_aSortInfo.SetSize(1);
	m_aSortInfo[0].nRC = 0;
	m_aSortInfo[0].sortType = CGXSortInfo::numeric;
	m_aSortInfo[0].sortOrder = CGXSortInfo::ascending;

	LockUpdate(FALSE);
}

void CSFR_RDFGrid::Data2Grid()
{
	T_UNIT_SYSTEM us;
	SetHeaderTitle(us, TRUE);
}

void CSFR_RDFGrid::Grid2Data()
{
	CString str;
	for (int nRow = 0; nRow < GetCountData(); nRow++)
	{
		for (int nCol = 1; nCol < GetColCount(); nCol++)
		{
			str = GetValueRowCol(nRow+1, nCol+1);
			m_pData->GetAt(nRow).dF[nCol-1] = _ttof(str);
		}
	}
}


CString CSFR_RDFGrid::GetNewKey()
{
	CString value;
	value.Format(_T("P%d"), m_pData->GetSize()+1);
	return value;
}

CString CSFR_RDFGrid::GetDefValue(int nIndex)
{
	return _T("");
	//if (nIndex == 0)
	//	return _T("");
	//else if(nIndex == 1)
	//	return m_pData->GetAt(m_pData->GetSize() - 1).strName;
	//CString str;
	//str.Format(DATA_FMT, m_pData->GetAt(m_pData->GetSize() - 1).dF[nIndex-2]);
	//return str;
}

CString* CSFR_RDFGrid::GetDefValueArray()
{
	return m_aCurDefVal;
}

BOOL CSFR_RDFGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols,
	CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	double* pKey;
	double* pData;
	pKey = new double;
	pData = new double;
	aKey.Add((void*)pKey);
	aData.Add((void*)pData);
	if (!ValidateAndMakeRecord(value, awCols, *pKey, *pData)) return FALSE;
	return TRUE;
}

void CSFR_RDFGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	double* pKey;
	double* pData;
	ASSERT(aKey.GetSize() == aData.GetSize());
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		pKey = (double*)aKey.GetAt(i);
		pData = (double*)aData.GetAt(i);
		delete pKey;
		delete pData;
	}
}

long CSFR_RDFGrid::GetCountData()
{
	return m_pData->GetSize();
}

void CSFR_RDFGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	ASSERT(0); // no filtering
}

void CSFR_RDFGrid::MakeItemEx(long* aKey, long nKeyNum, BOOL bUnitChanged)
{
	CDBDoc* pDoc = m_pDoc;

	ROWCOL ncRow = 0, ncCol = 0;
	CString curText;

	CArray<long, long> aSelectedKeys;
	GetSelectedKeys(aSelectedKeys);

	BOOL bAddNewSelected = FALSE;
	if (m_nEditMode == addnew)
	{
		CGXRangeList* pSelList = GetParam()->GetRangeList();
		bAddNewSelected = GetInvertStateRowCol(ncRow, ncCol, pSelList);
	}

	BOOL bFilter = FALSE;
	long nFilteredNum, *aFilteredKeys = 0;
	if (nKeyNum == -2)
	{
		if (IsFiltered() && GetRowCount() > 1)
		{
			aFilteredKeys = new long[GetRowCount() - 1];
			nFilteredNum = GetAllKeys(aFilteredKeys);
			bFilter = TRUE;
		}
	}
	else if (nKeyNum == -1) // no filtering : show all
	{
		// do nothing
	}
	else if (nKeyNum == 0)  // filter all : hide all
	{
		nFilteredNum = 0;
		bFilter = TRUE;
	}
	else  // normal filter
	{
		nFilteredNum = nKeyNum;
		aFilteredKeys = new long[nKeyNum];
		for (long i = 0; i < nKeyNum; i++)
			aFilteredKeys[i] = aKey[i];
		bFilter = TRUE;
	}

	CStringArray EditingRecord;
	if (IsEdit())
	{
		CString value;
		ROWCOL nCol;
		for (long i = 0; i < GetColCount(); i++)
		{
			VERIFY(GetColumnNoFor(i, nCol));
			value = CGXGridWnd::GetValueRowCol(ncRow, nCol);
			EditingRecord.Add(value);
		}
	}

	int nEditMode = m_nEditMode;

	long nNumData = GetCountData();

	BOOL bOldLock = LockUpdate(TRUE);

	SetRowCount(0); // row count reset
	long nSize;
	if (bFilter) nSize = nFilteredNum;
	else nSize = nNumData;
	if (!SetRecordCount(nSize))   // set record count
	{
		LockUpdate(bOldLock);
		delete[] aFilteredKeys;
		return;
	}

	CGXData* pData = GetParam()->GetData();
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	int nColCount = GetColCount();
	for (int i = 0; i < nColCount; i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));

	if (bFilter)
	{
		MakeItemFiltered(nFilteredNum, aFilteredKeys, awCols);
	}
	else if (nNumData > 0)
	{
		MakeItemNormal(nNumData, awCols);
	}

	if (m_aSortInfo.GetSize() > 1 ||
		(m_aSortInfo.GetSize() == 1 &&
		(m_aSortInfo.GetAt(0).nRC != GetKeyColID() ||
			m_aSortInfo.GetAt(0).sortOrder != CGXSortInfo::ascending)))
	{
		ROWCOL nkCol;
		GetColumnNoFor(m_aSortInfo.GetAt(0).nRC, nkCol);
		Sort(nkCol, m_aSortInfo[0].sortOrder);
	}

	if (nEditMode == addnew)
		InsertRows(ncRow, 1);
	if (GetRowCount() < ncRow) SetCurrentCell(GetRowCount(), ncCol);
	else SetCurrentCell(ncRow, ncCol);

	m_nEditMode = nEditMode;

	if (m_nEditMode == edit) Edit(ncRow);

	if (IsEdit() && bUnitChanged)
	{
		MakeItemEditingRecord(ncRow, EditingRecord);
	}

	if (aSelectedKeys.GetSize() > 0)
	{
		SelectRowByKey((long*)aSelectedKeys.GetData(), aSelectedKeys.GetSize());
	}
	if (bAddNewSelected)
	{
		CGXRange* pRange = new CGXRange();
		pRange->SetRows(ncRow, ncRow);
		GetParam()->GetRangeList()->AddTail(pRange);
	}

	ROWCOL nkCol;
	GetColumnNoFor(GetKeyColID(), nkCol);
	AdjustAppendRowNo(nkCol);
	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();

	if (aFilteredKeys != 0) delete[] aFilteredKeys;
}

void CSFR_RDFGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	//long nCount;
	CString str;
	CGXData* pData = GetParam()->GetData();
	BOOL bEnable = awCols.GetSize() != 2;
	for (int nCount = 0; nCount < nNumData; nCount++)
	{
		for (int i = 0; i < awCols.GetSize(); i++)
		{
			if (i == 0)
				str = m_pData->GetAt(nCount).strName;
			else
				str.Format(DATA_FMT, m_pData->GetAt(nCount).dF[i - 1]);
			pData->StoreValueRowCol(nCount+1, i+1, str, gxOverride, GX_VT_NUMERIC);
		}
		SetStyleRange(CGXRange().SetCells(nCount+1, 2), CGXStyle().SetEnabled(bEnable));
	}
}

void CSFR_RDFGrid::SetHeaderTitle(T_UNIT_SYSTEM &us, BOOL bInit)
{
	CString csTitle;
	m_aTitle.RemoveAll();
	m_aType.RemoveAll();
	m_aUnit.RemoveAll();
	if (*m_nCurMode == 3)
	{
		int nColCount = 4;
		SetColCount(nColCount);
		AddColInfo(_LS(IDS_CMD_LOAD_SFR_ID), t_real, 0);
		AddColInfo(_T("FX"), t_real, 0);
		AddColInfo(_T("FY"), t_real, 0);
		AddColInfo(_T("FZ"), t_real, 0);
		int aSize[] = { 60, 60,60,60};
		for (int i = 0; i < nColCount; i++)
		{
			csTitle = m_aTitle[i];
			SetStyleRange(CGXRange(0, i + 1), CGXStyle()
				.SetValue(csTitle)
				.SetUserAttribute(ID_USER_ATTR_COL_ID, m_aTitle[i]));
			if (bInit) SetColWidthByDPI(i + 1, i + 1, aSize[i]);
		}
	}
	else
	{
		int nColCount = 2;
		SetColCount(nColCount);
		AddColInfo(_LS(IDS_CMD_LOAD_SFR_ID), t_real, 0);
		AddColInfo(_T("F"), t_real, 0);
		int aSize[] = { 60, 180 };
		for (int i = 0; i < nColCount; i++)
		{
			csTitle = m_aTitle[i];
			SetStyleRange(CGXRange(0, i + 1), CGXStyle()
				.SetValue(csTitle)
				.SetUserAttribute(ID_USER_ATTR_COL_ID, m_aTitle[i]));
			if (bInit) SetColWidthByDPI(i + 1, i + 1, aSize[i]);
		}
	}
	SetColWidthByDPI(0, 0, 0);
}

BOOL CSFR_RDFGrid::AddToDB(ROWCOL nRow)
{
	Grid2Data();
	return TRUE;
}
BOOL CSFR_RDFGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	Grid2Data();
	return TRUE;
}
BOOL CSFR_RDFGrid::DeleteFromDB(ROWCOL nRow) 
{
	Grid2Data();
	return TRUE;
}
BOOL CSFR_RDFGrid::DeleteFromDB(CRowColArray& awRows)
{
	Grid2Data();
	return TRUE;
}
BOOL CSFR_RDFGrid::ModifyToDB(ROWCOL nRow) 
{
	Grid2Data();
	return TRUE;
}
BOOL CSFR_RDFGrid::ModifyToDB(CStringArray& raOldKey,
	CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	Grid2Data();
	return TRUE;
}

//-----------------------------CCmdSFRLoadDlg-----------------------------//

CCmdSFRLoadDlg::CCmdSFRLoadDlg(CWnd* pParent /*=NULL*/)
	: CCMDlgBase(CCmdSFRLoadDlg::IDD, pParent)
{
	m_nEndTime = 0;
	m_nIncrementTime = 0;
	m_nCurSelIndex = -1;
}

CCmdSFRLoadDlg::~CCmdSFRLoadDlg()
{
}

void CCmdSFRLoadDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCmdSFRLoadDlg)
	//DDX_Control(pDX, IDC_THFC_LIST, m_List);
	//DDX_Text(pDX,IDC_CMD_SFR_RATED_FREQUENCY_EDIT,

	//DDX_Control(pDX, IDC_CMD_SFR_RATED_FREQUENCY_EDIT, m_RFEdit);
	DDX_Text(pDX, IDC_CMD_SFR_RATED_FREQUENCY_EDIT, m_RFEdit);
	DDX_Text(pDX, IDC_CMD_SFR_NUMBER_RF_EDIT, m_NODFEdit);
	DDX_Text(pDX, IDC_CMD_SFR_RDF_1_EDIT, m_KEdit);
	DDX_Text(pDX, IDC_CMD_SFR_RDF_2_EDIT, m_WEdit);
	DDX_Control(pDX, IDC_CMD_SFR_RDF_3_EDIT, m_eEdit);
	DDX_Text(pDX, IDC_CMD_SFR_CFR_FREQRANGE_EDIT1, m_CFRRangeEdit1);
	DDX_Text(pDX, IDC_CMD_SFR_CFR_FREQRANGE_EDIT2, m_CFRRangeEdit2);
	DDX_Text(pDX, IDC_CMD_SFR_CFR_FREQRANGE_EDIT3, m_CFRPerEdit);
	DDX_Text(pDX, IDC_CMD_SFR_THLC_DAMPING_EDIT, m_CFRDampingEdit);
	DDX_Text(pDX, IDC_CMD_SFR_MSEL_X_FACTOR_EDIT, m_DirEdit[0]);
	DDX_Text(pDX, IDC_CMD_SFR_MSEL_Y_FACTOR_EDIT, m_DirEdit[1]);
	DDX_Text(pDX, IDC_CMD_SFR_MSEL_Z_FACTOR_EDIT, m_DirEdit[2]);

	DDX_Control(pDX, IDC_CMD_SFR_MSEL_X_FACTOR_COMBOX, m_DirCombox[0]);
	DDX_Control(pDX, IDC_CMD_SFR_MSEL_Y_FACTOR_COMBOX, m_DirCombox[1]);
	DDX_Control(pDX, IDC_CMD_SFR_MSEL_Z_FACTOR_COMBOX, m_DirCombox[2]);
	DDX_Control(pDX, IDC_CMD_SFR_ASSIGN_COMBOX, m_AssignCombox);
	

	DDX_Text(pDX, IDC_CMD_SFR_THLC_ENDTIME_EDIT, m_nEndTime);
	DDX_Text(pDX, IDC_CMD_SFR_THLC_INCREMENT_EDIT, m_nIncrementTime);
	
	DDX_Radio(pDX, IDC_CMD_SFR_RDF_1_RADIO, m_nRDFMode);

	//DDX_Control(pDX, IDC_CMD_SFR_RDF_LIST,m_RDFList);
	DDX_Control(pDX, IDC_CMD_SFR_LOAD_LIST, m_SFRList);
	DDX_Control(pDX, IDC_CMD_SFR_ASSIGN_SELECT_EDIT, m_wndNodeNumber);
	DDX_Control(pDX, IDC_CMD_SFR_RDF_LIST, m_wndGrid);
	//}}AFX_DATA_MAP
}
//
//BOOL CCmdSFRLoadDlg::InsertItem(T_THFC_K Key, T_THFC_D &Data)
//{
//	LVITEM lvitem;
//	CString str;
//	int nItem = m_List.GetItemCount();
//	for(int i = 0; i < COLCOUNT; i++)
//	{
//		lvitem.iItem = nItem;
//		lvitem.iSubItem=i;
//		str = DataToStr(i, Key, Data);
//		lvitem.pszText=str.GetBuffer(0);
//		lvitem.mask=LVIF_TEXT;
//
//		if( i == 0 ) 
//		{
//			lvitem.mask |= LVIF_PARAM;
//			lvitem.lParam = (LPARAM)Key;
//			nItem = m_List.InsertItem(&lvitem);
//		}
//		else m_List.SetItem(&lvitem);
//		str.ReleaseBuffer();
//	}
//	return TRUE;
//}
//
//BOOL CCmdSFRLoadDlg::DeleteItem(T_THFC_K Key, T_THFC_D &Data)
//{
//	LVFINDINFO FindInfo;
//	FindInfo.flags = LVFI_PARAM;
//	FindInfo.lParam = (LPARAM)Key;
//	int nItem = m_List.FindItem(&FindInfo);
//	if (nItem != -1) m_List.DeleteItem(nItem);
//	return TRUE;
//}
//
//BOOL CCmdSFRLoadDlg::ModifyItem(T_THFC_K KeyOld, T_THFC_K Key, T_THFC_D &Data)
//{
//	LVFINDINFO FindInfo;
//	FindInfo.flags = LVFI_PARAM;
//	FindInfo.lParam = (LPARAM)Key;
//	int nItem = m_List.FindItem(&FindInfo);
//	if (nItem != -1)
//	{
//		CString str;
//		for(int i = 0; i < COLCOUNT; i++)
//		{
//			str = DataToStr(i, Key, Data);
//			m_List.SetItemText(nItem, i, str);
//		}
//		// key Àç ¼³Á¤ 
//		LVITEM lvitem;
//		lvitem.iItem = nItem;
//		lvitem.iSubItem=0;
//		lvitem.mask = LVIF_PARAM;
//		lvitem.lParam = (LPARAM)Key;
//		m_List.SetItem(&lvitem);
//	}
//	return TRUE;
//}
//
//CString CCmdSFRLoadDlg::DataToStr(int i, T_THFC_K Key, T_THFC_D &Data)
//{
//	CString str;
//	CString csFuncType[] = {_LS(IDS_WG_CMD__ADDD__Time), _LS(IDS_WG_CMD__ADDD__Sinusoidal)};
//	CString csDataType[] = {_LS(IDS_WG_CMD__ADDD__NormAcc), _LS(IDS_WG_CMD__ADDD__Accel), 
//													_LS(IDS_WG_CMD__ADDD__Force), _LS(IDS_WG_CMD__ADDD__Moment),
//													_LS(IDS_WG_CMD__ADDD__Normal)};
//
//	ASSERT(Data.nThisFuncType >= 1 && Data.nThisFuncType <= 2);
//	ASSERT(Data.nThisDataType >= 1 && Data.nThisDataType <= 5);
//
//	if(i==0) str = Data.ThisFuncName;
//	else if(i==1) str = csFuncType[Data.nThisFuncType-1];
//	else if(i==2) str = csDataType[Data.nThisDataType-1];
//	else str = _LS(IDS_WG_CMD__ADDD__Error);
//
//	return str;
//}


BEGIN_MESSAGE_MAP(CCmdSFRLoadDlg, CCMDlgBase)

	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_SFR_NUMBER_RF_SPIN, OnNODFNumSpin)
	ON_BN_CLICKED(IDC_CMD_SFR_ASSIGN_APPALY_BUT, OnApply)
	ON_BN_CLICKED(IDC_CMD_SFR_ASSIGN_CANCEL_BUT, OnCmdSFRClose)
	ON_BN_CLICKED(IDC_CMD_SFR_RDF_1_RADIO, ChangeRDFMode)
	ON_BN_CLICKED(IDC_CMD_SFR_RDF_2_RADIO, ChangeRDFMode)
	ON_BN_CLICKED(IDC_CMD_SFR_RDF_3_RADIO, ChangeRDFMode)
	ON_BN_CLICKED(IDC_CMD_SFR_RDF_USER_RADIO, ChangeRDFMode)
	ON_BN_CLICKED(IDC_CMD_SFR_ASSIGN_ADD_BTN, OnAdd)
	ON_BN_CLICKED(IDC_CMD_SFR_ASSIGN_MODIFY_BTN, OnModify)
	ON_BN_CLICKED(IDC_CMD_SFR_ASSIGN_DEL_BTN, OnDel)
	ON_BN_CLICKED(IDC_CMD_SFR_CFR_FREQRANGE_CHECK, CheckFreqRange)
	ON_BN_CLICKED(IDC_CMD_SFR_CFR_EIGE_CHECK, CheckFreqEige)

	ON_BN_CLICKED(IDC_CMD_SFR_MSEL_XDIR_CHECK, CheckXDir)
	ON_BN_CLICKED(IDC_CMD_SFR_MSEL_YDIR_CHECK, CheckYDir)
	ON_BN_CLICKED(IDC_CMD_SFR_MSEL_ZDIR_CHECK, CheckZDir)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_SFR_LOAD_LIST, OnItemchangedList)

	ON_WM_DESTROY()
	//ON_EN_SETFOCUS(IDC_CMD_SFR_ASSIGN_SELECT_EDIT, OnSetfocusGrQueryNodeNo)
	//ON_REGISTERED_MESSAGE(CMouseEdit::WM_MOUSEEDITENTER, OnMouseEditEnter)
	//ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)

END_MESSAGE_MAP()

BOOL CCmdSFRLoadDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN && (GetFocus() == GetDlgItem(IDC_CMD_SFR_RDF_1_EDIT) ||
		GetFocus() == GetDlgItem(IDC_CMD_SFR_RDF_2_EDIT) || GetFocus() == GetDlgItem(IDC_CMD_SFR_RDF_3_EDIT) ||
		GetFocus() == GetDlgItem(IDC_CMD_SFR_RATED_FREQUENCY_EDIT)))
	{
		if (GetFocus() == GetDlgItem(IDC_CMD_SFR_RATED_FREQUENCY_EDIT))
		{
			UpdateData(TRUE);
			double	Pi = 3.141592;
			CString str;
			str.Format(_T("%.9f"), 0.0063 / (2.0 * Pi * m_RFEdit));
			m_eEdit.SetWindowText(str);
			UpdateData(FALSE);
		}
		ChangeRDFMode();
		return TRUE;
	}
	else if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN && (GetFocus() == GetDlgItem(IDC_CMD_SFR_NUMBER_RF_EDIT)))
	{
		ChangeRFNum();
		return TRUE;
	}
	
	return __super::PreTranslateMessage(pMsg);
}

BOOL CCmdSFRLoadDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
	CCMDlgBase::OnInitDialog();
	
	m_wndNodeNumber.SetAttNodeList();
	m_wndNodeNumber.SetLButtonDownNotifyWindow(this);
	m_wndNodeNumber.SetEnterNotifyWindow(this);
	m_wndNodeNumber.SetModeToUse(MOUSEEDIT_USE_GET_NODE_LIST);
	m_wndNodeNumber.SetMaxNodeKeyNum(1);

	InitVibratingMachineData();
	InitSFRAnalysisSetting();
	m_nRDFMode = 0;
	UpdateData(FALSE);
	CWnd *pWnd = GetDlgItem(IDC_CMD_SFR_RDF_LIST);
	InitGrid();
	//m_wndGrid.SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);
	GetDlgItem(IDC_CMD_SFR_RDF_CHECK)->EnableWindow(FALSE);
	ChangeRDFMode();
	ChangeRFNum();
	InitSFRList();
	CheckDlgButton(IDC_CMD_SFR_CFR_FREQRANGE_CHECK, TRUE);
	CheckDlgButton(IDC_CMD_SFR_MSEL_XDIR_CHECK, TRUE);
	CheckDlgButton(IDC_CMD_SFR_MSEL_YDIR_CHECK, TRUE);
	CheckDlgButton(IDC_CMD_SFR_MSEL_ZDIR_CHECK, TRUE);

	int nModeNum = m_pDoc->m_pPostCtrl->GetNumEigvPost();
	BOOL bEnable = nModeNum > 0 && m_pDoc->m_pPostCtrl->IsAnalysisResultEnable();
	GetDlgItem(IDC_CMD_SFR_CFR_EIGE_CHECK)->EnableWindow(bEnable);
	return TRUE;
}

void CCmdSFRLoadDlg::OnCmdSFRClose()
{
	DestroyWindow();
}

void CCmdSFRLoadDlg::InitVibratingMachineData()
{
	double	Pi = 3.141592;
	m_RFEdit = 50;
	m_NODFEdit = 2;
	m_KEdit = 0.25;
	m_WEdit = 100;
	CString str;
	str.Format(_T("%.9f"), 0.0063 / (2. * Pi * m_RFEdit));
	m_eEdit.SetWindowText(str);
}

void CCmdSFRLoadDlg::InitSFRAnalysisSetting()
{
	m_CFRRangeEdit1 = 0;
	m_CFRRangeEdit2 = 62.5;
	m_CFRPerEdit = 1;
	m_nEndTime = 20;
	m_nIncrementTime = 20;
	m_CFRDampingEdit = 0.0625;
	m_DirEdit[0] = 1;
	m_DirEdit[1] = 0.5;
	m_DirEdit[2] = 1;

	CString str[] = { _T("0"),_T("T/8"), _T("T/4"),_T("3T/8"),_T("T/2"),_T("5T/8"),_T("3T/4"),_T("7T/8"),_T("T") };
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			m_DirCombox[i].AddString(str[j]);
		}
		m_DirCombox[i].SetCurSel(0);
	}

}

void CCmdSFRLoadDlg::InitSFRList()
{
	CString strTitle[] = { _LS(IDS_CMD_LOAD_SFR_ID),_LS(IDS_CMD_LOAD_SFR_NODE) };
	int nColWidth[] = { 50,250 };

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	m_SFRList.DeleteAllItems();
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_SFRList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(m_SFRList.GetSafeHwnd(), dwStyle);
	//Set Title
	for (i = 0; i < 2; i++)
	{
		title = strTitle[i];
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_CENTER;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_SFRList.InsertColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCmdSFRLoadDlg::OnDestroy()
{
	for (int i = 0; i < m_SFRList.GetItemCount(); i++)
	{
		SFRListData* pData = (SFRListData*)m_SFRList.GetItemData(i);
		delete pData;
		pData = 0;
	}
}

void CCmdSFRLoadDlg::InitGrid()
{
	UpdateData(TRUE);

	double dF = 0.0;
	if (m_nRDFMode == 0 || m_nRDFMode == 3)
	{
		dF = m_KEdit * m_WEdit;
	}
	else if (m_nRDFMode == 1)
	{
		CString str;
		m_eEdit.GetWindowText(str);
		double eEdit = _ttof(str);
		dF =  m_WEdit/9.8 * eEdit * m_RFEdit * m_RFEdit;
	}
	else if (m_nRDFMode == 2)
	{
		dF = 0.25 * m_WEdit * pow(m_RFEdit * 60 / 3000., 3 / 2.);
	}
	m_SFRGridData.SetSize(m_NODFEdit);
	for (int j = 0; j < m_NODFEdit; j++)
	{
		m_SFRGridData[j].strName.Format(_T("P%d"), j + 1);
		for (int i = 0; i < 3; i++)
		{
			m_SFRGridData[j].dF[i] = dF;
		}
	}
	m_wndGrid.Initialize(&m_SFRGridData,&m_nRDFMode);
	m_wndGrid.SetScrollBarMode(SB_VERT, gxnAutomatic);/// gxnEnabled | gxnEnhanced);
	m_wndGrid.MakeItemEx();
}
void CCmdSFRLoadDlg::UpdateGrid()
{
	double dF = 0.0;
	if (m_nRDFMode == 0 || m_nRDFMode == 3)
	{
		dF = m_KEdit * m_WEdit;
	}
	else if (m_nRDFMode == 1)
	{
		double	Pi = 3.141592;
		CString str;
		m_eEdit.GetWindowText(str);
		double eEdit = _ttof(str);
		dF = m_WEdit / 9.8 * eEdit * (2 * Pi * m_RFEdit) * (2 * Pi * m_RFEdit);
	}
	else if (m_nRDFMode == 2)
	{
		dF = 0.25 * m_WEdit * pow(m_RFEdit * 60 / 3000., 3 / 2.);
	}
	dF /= m_NODFEdit;
	CArray<SFRGridData, SFRGridData&> TmpSFRGridData;
	TmpSFRGridData.Copy(m_SFRGridData);
	m_SFRGridData.RemoveAll();
	m_SFRGridData.SetSize(m_NODFEdit);
	for (int j = 0; j < m_NODFEdit; j++)
	{
		m_SFRGridData[j].strName.Format(_T("P%d"), j + 1);
		for (int i = 0; i < 3; i++)
		{
			if (m_nRDFMode == 3 && TmpSFRGridData.GetSize() > j)
			{
				m_SFRGridData[j].dF[i] = TmpSFRGridData[j].dF[i];
			}
			else
				m_SFRGridData[j].dF[i] = dF;
		}
	}
	m_wndGrid.Data2Grid();
	m_wndGrid.MakeItemEx();
	//m_wndGrid.SetRowCount(m_NODFEdit);
}

void CCmdSFRLoadDlg::ChangeRDFMode()
{
	UpdateData(TRUE);
	BOOL bShowRDF = TRUE, bShowRDF1 = TRUE, bShowRDF3 = TRUE;

	if (m_nRDFMode == 0 || m_nRDFMode == 2)
		bShowRDF3 = FALSE;
	if (m_nRDFMode == 1 || m_nRDFMode == 2)
		bShowRDF1 = FALSE;
	if (m_nRDFMode == 3)
	{ 
		bShowRDF1 = FALSE;
		bShowRDF = FALSE;
		bShowRDF3 = FALSE;
	}
		

	CheckDlgButton(IDC_CMD_SFR_RDF_CHECK,bShowRDF);
	GetDlgItem(IDC_CMD_SFR_RDF_1_STATIC)->EnableWindow(bShowRDF1);
	GetDlgItem(IDC_CMD_SFR_RDF_2_STATIC)->EnableWindow(bShowRDF);
	GetDlgItem(IDC_CMD_SFR_RDF_3_STATIC)->EnableWindow(bShowRDF3);
	GetDlgItem(IDC_CMD_SFR_RDF_1_EDIT)->EnableWindow(bShowRDF1);
	GetDlgItem(IDC_CMD_SFR_RDF_2_EDIT)->EnableWindow(bShowRDF);
	GetDlgItem(IDC_CMD_SFR_RDF_3_EDIT)->EnableWindow(bShowRDF3);
	GetDlgItem(IDC_CMD_SFR_RDF_2_UNIT)->EnableWindow(bShowRDF);
	GetDlgItem(IDC_CMD_SFR_RDF_3_UNIT)->EnableWindow(bShowRDF3);
	m_wndGrid.CancelEdit();
	UpdateGrid();
}
void CCmdSFRLoadDlg::ChangeRFNum()
{
	UpdateData(TRUE);
	m_AssignCombox.ResetContent();
	CString str;
	for (int i = 0; i < m_NODFEdit; i++)
	{
		str.Format(_T("P%d"), i + 1);
		m_AssignCombox.SetItemData(m_AssignCombox.AddString(str), i);
	}
	m_AssignCombox.SetCurSel(0);
	UpdateGrid();
	m_aNodeK.SetSize(m_NODFEdit);
}

void CCmdSFRLoadDlg::OnAdd()
{
	//InsertList
	if (!UpdateData(TRUE))
		return;
	//m_pDoc->m_pViewCtrl->GetCurrentNode(m_aNodeK[m_AssignCombox.GetCurSel()]);
	//if (m_aNodeK[m_AssignCombox.GetCurSel()].IsEmpty())
	//	return;
	//Insert Item
	CString str,strNumber;
	m_wndNodeNumber.GetWindowText(strNumber);
	if (strNumber.IsEmpty())
		return;
	int nId = m_AssignCombox.GetItemData(m_AssignCombox.GetCurSel());
	
	T_NODE_K CurNode = _ttoi(strNumber);
	T_NODE_K_LIST& aNode = m_aNodeK[nId];
	for (int i = 0; i < aNode.GetSize(); i++)
	{
		if (aNode[i] == CurNode)
			return;
	}
	aNode.Add(CurNode);
	CNumericOptimizer opt;
	strNumber = opt.OptimizeSortedArray(aNode);

	for (int i = 0; i < m_SFRList.GetItemCount(); i++)
	{
		SFRListData* pUserData = (SFRListData*)m_SFRList.GetItemData(i);
		if (pUserData->nId == nId)
		{
			//Modify 
			SetSFRMSELData(pUserData);
			m_SFRList.SetItemText(i, 1, strNumber);
			return;
		}
	}

	LVITEM lvitem;
	for (int j = 0; j < 2; j++)
	{
		lvitem.iSubItem = j;
		lvitem.mask = LVIF_TEXT;
		if (j == 0)
		{
			str.Format(_T("P%d"), nId+1);
			lvitem.pszText = str.GetBuffer(0);
			lvitem.iItem = m_SFRList.GetItemCount();
			SFRListData* pUserData = new SFRListData;
			pUserData->nId = nId;
			SetSFRMSELData(pUserData);
			m_SFRList.SetItemData(m_SFRList.InsertItem(&lvitem), (DWORD_PTR)pUserData);
		}
		else
		{
			lvitem.pszText = strNumber.GetBuffer(0);
			m_SFRList.SetItem(&lvitem);
		}
	}
}
void CCmdSFRLoadDlg::OnModify()
{
	if (!UpdateData(TRUE))
		return;
	int iItem = m_SFRList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
		return;
	CString str, strNumber;
	m_wndNodeNumber.GetWindowText(strNumber);
	if (strNumber.IsEmpty())
		return;
	int nId = m_AssignCombox.GetItemData(m_AssignCombox.GetCurSel());
	for (int i = 0; i < m_SFRList.GetItemCount(); i++)
	{
		SFRListData* pData = (SFRListData*)m_SFRList.GetItemData(i);
		if (pData->nId == nId)
		{
			//Modify 
			m_SFRList.SetItemText(i, 1, strNumber);
			SetSFRMSELData(pData);
			T_NODE_K CurNode = _ttoi(strNumber);
			m_aNodeK[nId].RemoveAll();
			m_aNodeK[nId].Add(CurNode);
			return;
		}
	}
}
void CCmdSFRLoadDlg::OnDel()
{
	int iItem = m_SFRList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
		return;
	SFRListData* pData = (SFRListData*)m_SFRList.GetItemData(iItem);
	if (pData)
	{
		delete pData;
		pData = 0;
	}
	m_aNodeK[iItem].RemoveAll();
	m_SFRList.DeleteItem(iItem);
}

void CCmdSFRLoadDlg::SetSFRMSELData(SFRListData* pUserData)
{
	pUserData->bCheck[0] = ((CButton*)GetDlgItem(IDC_CMD_SFR_MSEL_XDIR_CHECK))->GetCheck();
	pUserData->bCheck[1] = ((CButton*)GetDlgItem(IDC_CMD_SFR_MSEL_YDIR_CHECK))->GetCheck();
	pUserData->bCheck[2] = ((CButton*)GetDlgItem(IDC_CMD_SFR_MSEL_ZDIR_CHECK))->GetCheck();
	for(int i = 0;i < 3;i++)
	{
		pUserData->dScale[i] = m_DirEdit[i];
		pUserData->nTimeIndex[i] = m_DirCombox[i].GetCurSel();
	}
}
void CCmdSFRLoadDlg::UpdateSFRMSELData(const SFRListData* pUserData)
{
	CheckDlgButton(IDC_CMD_SFR_MSEL_XDIR_CHECK, pUserData->bCheck[0]);
	GetDlgItem(IDC_CMD_SFR_MSEL_X_FACTOR_COMBOX)->EnableWindow(pUserData->bCheck[0]);
	GetDlgItem(IDC_CMD_SFR_MSEL_X_FACTOR_STATIC)->EnableWindow(pUserData->bCheck[0]);
	GetDlgItem(IDC_CMD_SFR_MSEL_X_FACTOR_EDIT)->EnableWindow(pUserData->bCheck[0]);
	CheckDlgButton(IDC_CMD_SFR_MSEL_YDIR_CHECK, pUserData->bCheck[1]);
	GetDlgItem(IDC_CMD_SFR_MSEL_Y_FACTOR_COMBOX)->EnableWindow(pUserData->bCheck[1]);
	GetDlgItem(IDC_CMD_SFR_MSEL_Y_FACTOR_STATIC)->EnableWindow(pUserData->bCheck[1]);
	GetDlgItem(IDC_CMD_SFR_MSEL_Y_FACTOR_EDIT)->EnableWindow(pUserData->bCheck[0]);
	CheckDlgButton(IDC_CMD_SFR_MSEL_ZDIR_CHECK, pUserData->bCheck[2]);
	GetDlgItem(IDC_CMD_SFR_MSEL_Z_FACTOR_COMBOX)->EnableWindow(pUserData->bCheck[2]);
	GetDlgItem(IDC_CMD_SFR_MSEL_Z_FACTOR_STATIC)->EnableWindow(pUserData->bCheck[2]);
	GetDlgItem(IDC_CMD_SFR_MSEL_Z_FACTOR_EDIT)->EnableWindow(pUserData->bCheck[0]);
	for (int i = 0; i < 3; i++)
	{
		m_DirEdit[i] = pUserData->dScale[i];
		m_DirCombox[i].SetCurSel(pUserData->nTimeIndex[i]);
	}
	UpdateData(FALSE);
}


void CCmdSFRLoadDlg::CheckFreqRange()
{
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_CMD_SFR_CFR_FREQRANGE_CHECK))->GetCheck();
	CheckDlgButton(IDC_CMD_SFR_CFR_EIGE_CHECK, !bCheck);
}

void CCmdSFRLoadDlg::CheckFreqEige()
{
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_CMD_SFR_CFR_EIGE_CHECK))->GetCheck();
	CheckDlgButton(IDC_CMD_SFR_CFR_FREQRANGE_CHECK, !bCheck);
}

void CCmdSFRLoadDlg::CheckXDir()
{
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_CMD_SFR_MSEL_XDIR_CHECK))->GetCheck();
	GetDlgItem(IDC_CMD_SFR_MSEL_X_FACTOR_EDIT)->EnableWindow(bCheck);
	GetDlgItem(IDC_CMD_SFR_MSEL_X_FACTOR_COMBOX)->EnableWindow(bCheck);
	GetDlgItem(IDC_CMD_SFR_MSEL_X_FACTOR_STATIC)->EnableWindow(bCheck);
}
void CCmdSFRLoadDlg::CheckYDir()
{
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_CMD_SFR_MSEL_YDIR_CHECK))->GetCheck();
	GetDlgItem(IDC_CMD_SFR_MSEL_Y_FACTOR_EDIT)->EnableWindow(bCheck);
	GetDlgItem(IDC_CMD_SFR_MSEL_Y_FACTOR_COMBOX)->EnableWindow(bCheck);
	GetDlgItem(IDC_CMD_SFR_MSEL_Y_FACTOR_STATIC)->EnableWindow(bCheck);
}
void CCmdSFRLoadDlg::CheckZDir()
{
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_CMD_SFR_MSEL_ZDIR_CHECK))->GetCheck();
	GetDlgItem(IDC_CMD_SFR_MSEL_Z_FACTOR_EDIT)->EnableWindow(bCheck);
	GetDlgItem(IDC_CMD_SFR_MSEL_Z_FACTOR_COMBOX)->EnableWindow(bCheck);
	GetDlgItem(IDC_CMD_SFR_MSEL_Z_FACTOR_STATIC)->EnableWindow(bCheck);
}

void CCmdSFRLoadDlg::OnApply()
{
	if (!UpdateData(TRUE))
		return;
	if (m_SFRList.GetItemCount() < 1 && m_CFRRangeEdit2 < m_CFRRangeEdit1)
		return;
	m_wndGrid.Grid2Data();
	CArray<double, double> aFreq;
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_CMD_SFR_CFR_EIGE_CHECK))->GetCheck();
	CString str;
	T_THIS_K ThisK;
	T_THIS_D ThisD;
	T_THFC_D ThfcD;

	struct MSELD
	{
		T_THIS_K	keyThis;
		T_THFC_K	keyThfc[3];
	};
	typedef CArray<MSELD, MSELD&> SFRD;
	CArray<SFRD, SFRD&> aSFRD;
	
	double dF = 0.0;
	BOOL bCheckExist = FALSE;
	CString strError;
	CString strThfcName;
	CArray<T_THIK_K, T_THIK_K> aThisK;
	if (bCheck)
	{
		int nModeNum = m_pDoc->m_pPostCtrl->GetNumEigvPost();
		if (nModeNum == 0)
		{
			ASSERT(FALSE);
			return;
		}
		//CString strSuffix[] = { _T("X"),_T("Y"),_T("Z") };
		CArray<T_FREQ_D, T_FREQ_D&> aFreqD;
		aFreqD.SetSize(nModeNum);
		aSFRD.SetSize(nModeNum);
		
		for (int i = 0; i < nModeNum; i++)
		{
			m_pDoc->m_pPostCtrl->GetEiFreq(i + 1, aFreqD[i]);
			if(bCheckExist)
				continue;
			str.Format(_LS(IDS_DB_LOAD_CASE_MODE) + _T(" %d"), i + 1);
			double frequency = aFreqD[i].dblFreq[1];
			if (m_pDoc->m_pAttrCtrl->ExistThis(str))
			{
				strError.Format(_LS(IDS_CMD_SWEPT_FREQUENCY_RESPONSE_THISNAME_EXIST_ERROR), str);
				bCheckExist = TRUE;
			}
			if (m_pDoc->m_pAttrCtrl->ExistThfc(str))
			{
				strError.Format(_LS(IDS_CMD_SWEPT_FREQUENCY_RESPONSE_THFCNAME_EXIST_ERROR), str);
				bCheckExist = TRUE;
			}
		}
		if (bCheckExist && AfxMessageBox(strError, MB_OKCANCEL) != MB_OKCANCEL)
			return;
		if (!m_pDoc->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_CMD_SWEPT_FREQUENCY_RESPONSE_SETTING)), CMDTYPE_REMOVE_ANALYSIS))return;
		if (bCheckExist)
		{
			m_pDoc->m_pEditData->DelAllThfc();
			m_pDoc->m_pEditData->DelAllThis();
		}
		for (int i = 0; i < nModeNum; i++)
		{
			str.Format(_LS(IDS_DB_LOAD_CASE_MODE) + _T(" %d"), i + 1);
			double frequency = aFreqD[i].dblFreq[1];
			if (frequency < 1e-9)
			{
				ASSERT(FALSE);
				m_pDoc->m_pUndoCtrl->CancelEditDB();
				return;
			}
			//This
			ThisD.Initialize();
			ThisD.LoadCaseName = str;
			ThisD.dEndTime = m_nEndTime / frequency;
			ThisD.dDelta = 1. / (frequency*m_nIncrementTime);
			ThisD.dDampAll = m_CFRDampingEdit;
			ThisD.nTimeHistoryType = 2;
			ThisD.Description.Format(_T("%d"), m_nEndTime);
			ThisK = m_pDoc->m_pAttrCtrl->GetStartNumThis();
			if (!m_pDoc->m_pEditData->AddThis(ThisD))
			{
				m_pDoc->m_pUndoCtrl->CancelEditDB();
				return;
			}
			aThisK.Add(ThisK);
			//Thfc
			aSFRD[i].SetSize(m_NODFEdit);
			for (int j = 0; j < m_NODFEdit; j++)
			{
				aSFRD[i][j].keyThis = ThisK;
				ThfcD.Initialize();
				ThfcD.nThisFuncType = 2;
				ThfcD.nThisDataType = 3;
				CString strSuffix[] = { _T("X"),_T("Y"),_T("Z") };
				for (int k = 0; k < 3; k++)
				{
					if (k == 1 && m_nRDFMode != 3)
						break;
					dF = m_SFRGridData[j].dF[k] * pow((frequency / m_RFEdit), 2);
					ThfcD.ThisFuncName = ThisD.LoadCaseName + _T("_") + m_SFRGridData[j].strName;
					if (m_nRDFMode == 3)
						ThfcD.ThisFuncName += strSuffix[k];
					ThfcD.SinuA = dF;
					ThfcD.SinuFrequency = frequency;
					CalcSinu(frequency, dF, ThfcD.arThisFuncData);
					if (!m_pDoc->m_pEditData->AddThfc(ThfcD))
					{
						m_pDoc->m_pUndoCtrl->CancelEditDB();
						return;
					}
					aSFRD[i][j].keyThfc[k] = ThfcD.nThisFuncId;
					aSFRD[i][j].keyThis = ThisK;
					if (m_nRDFMode != 3)
					{
						aSFRD[i][j].keyThfc[k + 1] = ThfcD.nThisFuncId;
						aSFRD[i][j].keyThfc[k + 2] = ThfcD.nThisFuncId;
					}
				}

			}
		}
	}
	else
	{
		int nCount = (m_CFRRangeEdit2 - m_CFRRangeEdit1) / m_CFRPerEdit + 1;
		aSFRD.SetSize(nCount);
		for (int i = 0; i < nCount; i++)
		{
			double frequency = m_CFRRangeEdit1 + i * m_CFRPerEdit;
			if(frequency < 0.00001)
				continue;
			str.Format(_T("%.4f"), frequency);
			if (m_pDoc->m_pAttrCtrl->ExistThis(str))
			{
				strError.Format(_LS(IDS_CMD_SWEPT_FREQUENCY_RESPONSE_THISNAME_EXIST_ERROR), str);
				bCheckExist = TRUE;
				break;
			}
			for (int j = 0; j < m_NODFEdit; j++)
			{
				CString strSuffix[] = { _T("X"),_T("Y"),_T("Z") };
				for (int k = 0; k < 3; k++)
				{
					if (k == 1 && m_nRDFMode != 3)
						break;
					if (m_nRDFMode == 3)
						ThfcD.ThisFuncName += strSuffix[k];
					strThfcName = str + _T("_") + m_SFRGridData[j].strName;
					if (m_pDoc->m_pAttrCtrl->ExistThfc(str))
					{
						strError.Format(_LS(IDS_CMD_SWEPT_FREQUENCY_RESPONSE_THFCNAME_EXIST_ERROR), strThfcName);
						bCheckExist = TRUE;
						break;
					}
				}
				if (bCheckExist)
					break;
			}
		}
		if (bCheckExist && AfxMessageBox(strError, MB_OKCANCEL) != MB_OKCANCEL)
			return;
		if (!m_pDoc->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_CMD_SWEPT_FREQUENCY_RESPONSE_SETTING)), CMDTYPE_REMOVE_ANALYSIS))return;
		if (bCheckExist)
		{
			m_pDoc->m_pEditData->DelAllThfc();
			m_pDoc->m_pEditData->DelAllThis();
		}
			
		for (int i = 0; i < nCount; i++)
		{
			double frequency = m_CFRRangeEdit1 + i * m_CFRPerEdit;
			if (frequency < 0.00001)
				continue;
			str.Format(_T("%.4f"), frequency);
			//This
			ThisD.Initialize();
			ThisD.LoadCaseName = str;
			ThisD.dEndTime = m_nEndTime / frequency;
			ThisD.dDelta = 1. / (frequency*m_nIncrementTime);
			ThisD.dDampAll = m_CFRDampingEdit;
			ThisD.nTimeHistoryType = 2;
			ThisD.Description.Format(_T("%d"), m_nEndTime);
			ThisK = m_pDoc->m_pAttrCtrl->GetStartNumThis();
			if (!m_pDoc->m_pEditData->AddThis(ThisD))
			{
				m_pDoc->m_pUndoCtrl->CancelEditDB();
				return;
			}
			aThisK.Add(ThisK);
			aSFRD[i].SetSize(m_NODFEdit);
			//Thfc
			for (int j = 0; j < m_NODFEdit; j++)
			{
				if(m_aNodeK[j].IsEmpty())
					continue;
				ThfcD.Initialize();
				ThfcD.nThisFuncType = 2;
				ThfcD.nThisDataType = 3;
				CString strSuffix[] = { _T("X"),_T("Y"),_T("Z") };
				for (int k = 0; k < 3; k++)
				{
					if (k == 1 && m_nRDFMode != 3)
						break;
					dF = m_SFRGridData[j].dF[k] * pow((frequency / m_RFEdit), 2);
					ThfcD.ThisFuncName = ThisD.LoadCaseName + _T("_") + m_SFRGridData[j].strName;
					if (m_nRDFMode == 3)
						ThfcD.ThisFuncName += strSuffix[k];
					ThfcD.SinuA = dF;
					ThfcD.SinuFrequency = frequency;
					CalcSinu(frequency, dF, ThfcD.arThisFuncData);
					if (!m_pDoc->m_pEditData->AddThfc(ThfcD))
					{
						m_pDoc->m_pUndoCtrl->CancelEditDB();
						return;
					}
					aSFRD[i][j].keyThfc[k] = ThfcD.nThisFuncId;
					aSFRD[i][j].keyThis = ThisK;
					if (m_nRDFMode != 3)
					{
						aSFRD[i][j].keyThfc[k + 1] = ThfcD.nThisFuncId;
						aSFRD[i][j].keyThfc[k + 2] = ThfcD.nThisFuncId;
					}
				}
			}
		}
	}

	//Thnl
	T_THNL_D ThnlD;
	T_THNL_K KeyThnl;
	ThnlD.Initialize();
	for (int i = 0; i < m_NODFEdit; i++)
	{
		if(m_aNodeK[i].IsEmpty())
			continue;

		SFRListData* pUserData = 0;// (SFRListData*)m_SFRList.GetItemData(i);
		for (int j = 0; j < m_SFRList.GetItemCount(); j++)
		{
			pUserData = (SFRListData*)m_SFRList.GetItemData(j);
			if (pUserData->nId == i)
			{
				break;
			}
		}
		if (!pUserData)
		{
			ASSERT(FALSE);
			continue;
		}
		for (int j = 0; j < aSFRD.GetSize(); j++)
		{
			if(aSFRD[j].IsEmpty())
				continue;
			ThnlD.nLoadType = 1;
			ThnlD.LoadCaseKey = aSFRD[j][i].keyThis;
			for (int k = 0; k < 3; k++)
			{
				if(!pUserData->bCheck[k])
					continue;
				ThnlD.nThisFuncKey = aSFRD[j][i].keyThfc[k];
				ThnlD.nDirection = k + 1;
				ThnlD.dFactor = pUserData->dScale[k];// m_DirEdit[k];
				ThnlD.dArrivalTime = 0.125 * pUserData->nTimeIndex[k] / m_RFEdit;//0.25 * m_DirCombox[k].GetCurSel() / m_RFEdit;
				for (int m = 0; m < m_aNodeK[i].GetSize(); m++)
				{
					m_pDoc->m_pAttrCtrl->MakeKeyThnl(m_aNodeK[i][m], ThnlD.LoadCaseKey, KeyThnl);
					if (!m_pDoc->m_pEditData->AddThnl(KeyThnl, ThnlD, FALSE))
					{
						m_pDoc->m_pUndoCtrl->CancelEditDB();
						return;
					}
				}
			}
		}
	}
	//LCOM
	CString strSFRLoadName = _T("gSFR");
	bool bExistLoadComb = false;
	T_LCOM_D LcomData;
	if (m_pDoc->m_pAttrCtrl->ExistLcom(D_LCOMTYPE_GENERAL, strSFRLoadName))
	{
		bExistLoadComb = true;
		m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_GENERAL, strSFRLoadName, LcomData);
		//m_pDoc->m_pEditData->DelLcom(D_LCOMTYPE_GENERAL, strSFRLoadName);
	}
	LcomData.LoadCombName = strSFRLoadName;
	LcomData.LoadCombType = 1;
	LcomData.Description = _T("Swept Frequency Response LoadCombo");
	T_LCOM_BASE LcomBase;
	LcomBase.AnalType = D_LCOM_HISTORY;
	LcomBase.Factor = 1.0;
	for (int i = 0; i < aThisK.GetSize(); i++)
	{
		LcomBase.LoadCaseKey = aThisK[i];
		LcomData.aCombination.Add(LcomBase);
	}
	if(bExistLoadComb)
		m_pDoc->m_pEditData->ModifyLcom(D_LCOMTYPE_GENERAL, LcomData.LoadCombId,LcomData);
	else
		m_pDoc->m_pEditData->AddLcom(D_LCOMTYPE_GENERAL,LcomData);

	//aThisK.Add(ThisD.LoadCaseId);
	m_pDoc->m_pUndoCtrl->CloseEditDB(TRUE);
}

void CCmdSFRLoadDlg::OnNODFNumSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	UpdateData(TRUE);
	int nTemp = m_NODFEdit;
	if (pNMUpDown->iDelta < 0)
	{
		m_NODFEdit += 1;
	}
	else
	{
		m_NODFEdit -= 1;
	}
	if (m_NODFEdit == 0)
	{
		m_NODFEdit = nTemp;
		return;
	}
	UpdateData(FALSE);
	*pResult = 0;
	ChangeRFNum();
}


BOOL CCmdSFRLoadDlg::CalcSinu(double dFreq,double dF,CArray<T_THFC_BASE, T_THFC_BASE&> &aThisFuncData)
{
	double A, C, f, D, PA;
	A = dF;
	C = 0.0;
	f = dFreq;
	D = 0.0;
	PA = 0.0;

	aThisFuncData.RemoveAll();
	if (D < 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Damping_factor_must_be_gr));
		return FALSE;
	}
	if (f == 0.0)
	{
		return TRUE;
	}

#define D_THFC_MAX_COUNT 5000
#define D_THFC_DEF_CYCLE 10
	double tInc, tDraw;
	tInc = 1.0 / (36.0*f);     // sampling every 10 deg.
	tDraw = D_THFC_DEF_CYCLE / f;
	if ((int)(tDraw / tInc) > D_THFC_MAX_COUNT)
	{
		tDraw = tInc * D_THFC_MAX_COUNT;
	}

	double t = 0;
	double Ft;
	double eVal, sinVal;
	T_THFC_BASE data;

	for (int i = 0; i < D_THFC_MAX_COUNT && t <= tDraw; i++)
	{
		eVal = (A + C * t)*exp(-D * t);
		sinVal = sin(2 * PI*f*t + PA * PI / 180.0);
		Ft = eVal * sinVal;
		if (Ft == HUGE_VAL || Ft == -HUGE_VAL)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___F_t__value_is_out_of_rang));
			return FALSE;
		}

		data.dblTime = t;
		data.dblValue = Ft;
		aThisFuncData.Add(data);

		t += tInc;
	}
	return TRUE;
}

void CCmdSFRLoadDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch (lHint)
	{
	case D_UPDATE_DBALL:
		//Del Eigv Result
		CheckDlgButton(IDC_CMD_SFR_CFR_FREQRANGE_CHECK, TRUE);
		CheckDlgButton(IDC_CMD_SFR_CFR_EIGE_CHECK, FALSE);
		GetDlgItem(IDC_CMD_SFR_CFR_EIGE_CHECK)->EnableWindow(FALSE);
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CCmdSFRLoadDlg::OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->uNewState == 0 || pNMListView->uNewState == 1)
	{
		*pResult = 0;
		return;
	}
	int nItem = pNMListView->iItem;
	if (nItem == -1)
	{
		*pResult = 0;
		return;
	}
	m_AssignCombox.SetCurSel(nItem);
	SFRListData* pData = (SFRListData*)m_SFRList.GetItemData(nItem);
	UpdateSFRMSELData(pData);
}

