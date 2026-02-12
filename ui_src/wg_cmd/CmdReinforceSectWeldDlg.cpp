#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdReinforceSectWeldDlg.h"
#include "CmdIsolatorGBStressLmtTableDlg.h"
#include "ExportFunc.h"
#include "CMLoadCombDlg.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\StageInfo.h"
#include "..\wg_db\DampDgnMgr_CH.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "CmdDampLoadCombTableDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CmdReinforceSectWeldDlg::CmdReinforceSectWeldDlg(T_RFST_WELD_D* pData,CWnd* pParent /*=NULL*/)
	: CDialogMove(CmdReinforceSectWeldDlg::IDD, pParent)
{
	m_nWeldType = 0;
	m_pWeldData = pData;
}

void CmdReinforceSectWeldDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CmdReinforceSectWeldDlg)
	DDX_Radio(pDX, IDC_CMD_RADIO1, m_nWeldType);
	DDX_Control(pDX, IDC_CMD_COMB_STATIC1, m_WeldCountCombox);
	DDX_Control(pDX, IDC_EDIT1, m_HfEdit);
	DDX_Control(pDX, IDC_EDIT2, m_LengthEdit);
	DDX_Control(pDX, IDC_EDIT3, m_SpaceEdit);
	
	DDX_Control(pDX, IDC_CMD_STATIC2, m_wndWeldUnit[0]);
	DDX_Control(pDX, IDC_CMD_STATIC4, m_wndWeldUnit[1]);
	DDX_Control(pDX, IDC_CMD_STATIC6, m_wndWeldUnit[2]);
	//IDC_CMD_STATIC3
}

BEGIN_MESSAGE_MAP(CmdReinforceSectWeldDlg, CDialogMove)
	ON_BN_CLICKED(IDC_CMD_REINFORCE_OK_BTN, OnOK)
	ON_BN_CLICKED(IDC_CMD_REINFORCE_CANCEL_BTN, OnCancel)
	ON_CBN_SELCHANGE(IDC_CMD_COMB_STATIC1, OnChangeWeldCount)
	ON_EN_CHANGE(IDC_EDIT2, OnChangeLength)
	ON_EN_CHANGE(IDC_EDIT3, OnChangeSpace)
END_MESSAGE_MAP()

BOOL CmdReinforceSectWeldDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();
	m_pDoc = CDBDoc::GetDocPoint();

	m_wndWeldUnit[0].SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndWeldUnit[1].SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndWeldUnit[2].SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_WeldCountCombox.ResetContent();
	CString str;
	for(int i = 0; i < 10; i++)
	{
		str.Format(_T("%d"), i + 1);
		m_WeldCountCombox.AddString(str);
	}
	Data2Dlg();
	m_wndGrid.SubclassDlgItem(IDC_CMD_LIST, this);
	InitGrid();
	m_wndGrid.MakeItemEx();
	return TRUE;
}

void CmdReinforceSectWeldDlg::Data2Dlg()
{
	m_nWeldType = m_pWeldData->nType;
	CString str;
	str.Format(_T("%g"), m_pWeldData->dHf);
	m_HfEdit.SetWindowText(str);
	str.Format(_T("%g"), m_pWeldData->dLength);
	m_LengthEdit.SetWindowText(str);
	str.Format(_T("%g"), m_pWeldData->dSpace);
	m_SpaceEdit.SetWindowText(str);
	UpdateData(FALSE);

	int nNum = m_pWeldData->aWeldCoordinate.GetSize();
	if(nNum == 0)
	{
		nNum = 4;
		m_SubWeldData.SetSize(nNum);
		for(int i = 0; i < nNum; i++)
		{
			m_SubWeldData[i].bAuto = TRUE;
			m_SubWeldData[i].dy = 0.0;
			m_SubWeldData[i].dz = 0.0;
			m_SubWeldData[i].dLength = m_pWeldData->dLength;
			m_SubWeldData[i].dSpace = m_pWeldData->dSpace;
		}
	}
	else
	{
		m_SubWeldData.RemoveAll();
		m_SubWeldData.Copy(m_pWeldData->aWeldCoordinate);
	}
	m_WeldCountCombox.SetCurSel(nNum - 1);
}

BOOL CmdReinforceSectWeldDlg::Dlg2Data()
{
	UpdateData(TRUE);
	m_pWeldData->nType = m_nWeldType;
	m_HfEdit.GetEditValue(m_pWeldData->dHf);
	m_LengthEdit.GetEditValue(m_pWeldData->dLength);
	m_SpaceEdit.GetEditValue(m_pWeldData->dSpace);
	m_wndGrid.Grid2Data();
	m_pWeldData->aWeldCoordinate.RemoveAll();
	m_pWeldData->aWeldCoordinate.Copy(m_SubWeldData);
	for (int i = 0; i < m_pWeldData->aWeldCoordinate.GetSize(); i++)
	{
		if (m_pWeldData->aWeldCoordinate[i].bAuto)
		{
			m_pWeldData->aWeldCoordinate[i].dLength = m_pWeldData->dLength;
			m_pWeldData->aWeldCoordinate[i].dSpace = m_pWeldData->dSpace;
		}
	}
	return TRUE;
}

void CmdReinforceSectWeldDlg::OnOK()
{
	if(!Dlg2Data())	
		return;
	CDialogMove::OnOK();
}

void CmdReinforceSectWeldDlg::OnCancel()
{
	CDialogMove::OnCancel();
}

void CmdReinforceSectWeldDlg::OnChangeWeldCount()
{
	int nOldSize = m_SubWeldData.GetSize();
	int nNewSize = m_WeldCountCombox.GetCurSel() + 1;
	if(nNewSize > nOldSize)
	{
		T_RFST_WELD_SUB_D WeldD;
		for(int i = nOldSize; i < nNewSize; i++)
		{
			WeldD.bAuto = TRUE;
			WeldD.dy = WeldD.dz = 0.0;
			WeldD.dLength = m_pWeldData->dLength;
			WeldD.dSpace = m_pWeldData->dSpace;
			m_SubWeldData.Add(WeldD);
		}
		m_wndGrid.MakeItemEx();
	}
	else if(nNewSize < nOldSize)
	{
		for(int i = nOldSize; i > nNewSize; i--)
		{
			m_SubWeldData.RemoveAt(i - 1);
		}
		m_wndGrid.MakeItemEx();
	}
}

void CmdReinforceSectWeldDlg::OnChangeSpace()
{
	m_SpaceEdit.GetEditValue(m_pWeldData->dSpace);
}

void CmdReinforceSectWeldDlg::OnChangeLength()
{
	m_LengthEdit.GetEditValue(m_pWeldData->dLength);
}

void CmdReinforceSectWeldDlg::InitGrid()
{
	m_wndGrid.Initialize(m_pWeldData,&m_SubWeldData);
	m_wndGrid.SetScrollBarMode(SB_VERT, gxnAutomatic);/// gxnEnabled | gxnEnhanced);
}

//-----------------------------CReinfoceSectWeldGrid-----------------------------//

#define EPSILON 1.e-12
#define DATA_FMT _T("%.15g")
#define DATA_INT _T("%d")

CString CRS_WeldGrid::m_aCurDefVal[] = { _T("1"), _T("0.0") ,_T("0.0"),_T("0.0"),_T("0.0") };
CRS_WeldGrid::CRS_WeldGrid()
{
	m_pData = 0;
}

CRS_WeldGrid::~CRS_WeldGrid()
{
}

BEGIN_MESSAGE_MAP(CRS_WeldGrid, CTBCommon)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

void CRS_WeldGrid::Initialize(T_RFST_WELD_D* pWeldData, CArray<T_RFST_WELD_SUB_D, T_RFST_WELD_SUB_D &>* pData)
{
	ASSERT(pData);
	m_pWeldData = pWeldData;
	m_pData = pData;
	T_UNIT_SYSTEM us;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
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


	CString csTitle;
	m_aTitle.RemoveAll();
	m_aType.RemoveAll();
	m_aUnit.RemoveAll();
	int nColCount = 5;
	SetColCount(nColCount);
	AddColInfo(_LS(IDS_CMD_Automatic), t_integer, 0);
	AddColInfo(_T("y"), t_real, 0);
	AddColInfo(_T("z"), t_real, 0);
	AddColInfo(_LS(IDS_WG_CMD_Reinforce_Weld_Length), t_real, 0);
	AddColInfo(_LS(IDS_WG_CMD_Reinforce_Weld_Space), t_real, 0);
	int aSize[] = { 40, 45,45,68,68 };
	for(int i = 0; i < nColCount; i++)
	{
		csTitle = m_aTitle[i];
		SetStyleRange(CGXRange(0, i + 1), CGXStyle()
			.SetValue(csTitle)
			.SetUserAttribute(ID_USER_ATTR_COL_ID, m_aTitle[i]));
		SetColWidth(i + 1, i + 1, aSize[i]);
	}
	SetColWidth(0, 0, 0);

	ChangeStandardStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetSize(9))
		.SetVerticalAlignment(DT_BOTTOM)
		.SetAllowEnter(FALSE));
	ChangeColHeaderStyle(CGXStyle().SetFont(CGXFont_GC().SetBold(FALSE)));

	ChangeRowHeaderStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetBold(FALSE))
		.SetHorizontalAlignment(DT_RIGHT));
		//.SetControl(GX_IDS_CTRL_HEADER));
	GetParam()->SetNumberedRowHeaders(TRUE);

	SetRowHeight(0, 0, 18);

	SetStyleRange(CGXRange().SetCols(1), CGXStyle()
		.SetControl(GX_IDS_CTRL_CHECKBOX3D)
		.SetHorizontalAlignment(DT_CENTER)
		.SetVerticalAlignment(DT_VCENTER)
		.SetValue(_T("")));

	SetRightValidCell(1, 0);
	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);

	m_aSortInfo.SetSize(1);
	m_aSortInfo[0].nRC = 0;
	m_aSortInfo[0].sortType = CGXSortInfo::numeric;
	m_aSortInfo[0].sortOrder = CGXSortInfo::ascending;

	EnableAppendRow(FALSE);

	LockUpdate(FALSE);
}

void CRS_WeldGrid::Data2Grid()
{
	T_UNIT_SYSTEM us;
	SetHeaderTitle(us);
}

void CRS_WeldGrid::Grid2Data()
{
	CString str;
	for(int nRow = 0; nRow < GetCountData(); nRow++)
	{
		BOOL bAuto = TRUE;
		for(int nCol = 0; nCol < GetColCount(); nCol++)
		{
			str = GetValueRowCol(nRow + 1, nCol + 1);
			if (nCol == 0)
			{
				m_pData->GetAt(nRow).bAuto = _tstoi(str) == 1;
				bAuto = _tstoi(str) == 1;
			}
			else if(nCol == 1)
				m_pData->GetAt(nRow).dy = _ttof(str);
			else if(nCol == 2)
				m_pData->GetAt(nRow).dz = _ttof(str);
			else if (nCol == 3)
			{
				if (bAuto)
					m_pData->GetAt(nRow).dLength = m_pWeldData->dLength;
				else
					m_pData->GetAt(nRow).dLength = _ttof(str);
			}
			else if (nCol == 4)
			{
				if (bAuto)
					m_pData->GetAt(nRow).dSpace = m_pWeldData->dSpace;
				else
					m_pData->GetAt(nRow).dSpace = _ttof(str);
			}
		}
	}
}


CString CRS_WeldGrid::GetNewKey()
{
	CString value;
	value.Format(_T("P%d"), m_pData->GetSize() + 1);
	return value;
}

CString CRS_WeldGrid::GetDefValue(int nIndex)
{
	return _T("");
}

CString* CRS_WeldGrid::GetDefValueArray()
{
	return m_aCurDefVal;
}

BOOL CRS_WeldGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols,
	CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	double* pKey;
	double* pData;
	pKey = new double;
	pData = new double;
	aKey.Add((void*)pKey);
	aData.Add((void*)pData);
	if(!ValidateAndMakeRecord(value, awCols, *pKey, *pData)) return FALSE;
	return TRUE;
}

void CRS_WeldGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	double* pKey;
	double* pData;
	ASSERT(aKey.GetSize() == aData.GetSize());
	for(int i = 0; i < aKey.GetSize(); i++)
	{
		pKey = (double*)aKey.GetAt(i);
		pData = (double*)aData.GetAt(i);
		delete pKey;
		delete pData;
	}
}

long CRS_WeldGrid::GetCountData()
{
	return m_pData->GetSize();
}

void CRS_WeldGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	ASSERT(0); // no filtering
}

void CRS_WeldGrid::MakeItemEx(long* aKey, long nKeyNum, BOOL bUnitChanged)
{
	CDBDoc* pDoc = m_pDoc;

	ROWCOL ncRow = 0, ncCol = 0;
	CString curText;

	CArray<long, long> aSelectedKeys;
	GetSelectedKeys(aSelectedKeys);

	BOOL bAddNewSelected = FALSE;
	if(m_nEditMode == addnew)
	{
		CGXRangeList* pSelList = GetParam()->GetRangeList();
		bAddNewSelected = GetInvertStateRowCol(ncRow, ncCol, pSelList);
	}

	BOOL bFilter = FALSE;
	long nFilteredNum, * aFilteredKeys = 0;
	if(nKeyNum == -2)
	{
		if(IsFiltered() && GetRowCount() > 1)
		{
			aFilteredKeys = new long[GetRowCount() - 1];
			nFilteredNum = GetAllKeys(aFilteredKeys);
			bFilter = TRUE;
		}
	}
	else if(nKeyNum == -1) // no filtering : show all
	{
		// do nothing
	}
	else if(nKeyNum == 0)  // filter all : hide all
	{
		nFilteredNum = 0;
		bFilter = TRUE;
	}
	else  // normal filter
	{
		nFilteredNum = nKeyNum;
		aFilteredKeys = new long[nKeyNum];
		for(long i = 0; i < nKeyNum; i++)
			aFilteredKeys[i] = aKey[i];
		bFilter = TRUE;
	}

	CStringArray EditingRecord;
	if(IsEdit())
	{
		CString value;
		ROWCOL nCol;
		for(long i = 0; i < GetColCount(); i++)
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
	if(bFilter) nSize = nFilteredNum;
	else nSize = nNumData;
	if(!SetRecordCount(nSize))   // set record count
	{
		LockUpdate(bOldLock);
		delete[] aFilteredKeys;
		return;
	}

	CGXData* pData = GetParam()->GetData();
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	int nColCount = GetColCount();
	for(int i = 0; i < nColCount; i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));

	if(bFilter)
	{
		MakeItemFiltered(nFilteredNum, aFilteredKeys, awCols);
	}
	else if(nNumData > 0)
	{
		MakeItemNormal(nNumData, awCols);
	}

	if(m_aSortInfo.GetSize() > 1 || (m_aSortInfo.GetSize() == 1 && (m_aSortInfo.GetAt(0).nRC != GetKeyColID() || m_aSortInfo.GetAt(0).sortOrder != CGXSortInfo::ascending)))
	{
		ROWCOL nkCol;
		GetColumnNoFor(m_aSortInfo.GetAt(0).nRC, nkCol);
		Sort(nkCol, m_aSortInfo[0].sortOrder);
	}

	if(nEditMode == addnew)
		InsertRows(ncRow, 1);
	if(GetRowCount() < ncRow) SetCurrentCell(GetRowCount(), ncCol);
	else SetCurrentCell(ncRow, ncCol);

	m_nEditMode = nEditMode;

	if(m_nEditMode == edit) Edit(ncRow);

	if(IsEdit() && bUnitChanged)
	{
		MakeItemEditingRecord(ncRow, EditingRecord);
	}

	if(aSelectedKeys.GetSize() > 0)
	{
		SelectRowByKey((long*)aSelectedKeys.GetData(), aSelectedKeys.GetSize());
	}
	if(bAddNewSelected)
	{
		CGXRange* pRange = new CGXRange();
		pRange->SetRows(ncRow, ncRow);
		GetParam()->GetRangeList()->AddTail(pRange);
	}

	ROWCOL nkCol;
	GetColumnNoFor(GetKeyColID(), nkCol);
	AdjustAppendRowNo(nkCol);
	LockUpdate(bOldLock);
	if(!bOldLock) Redraw();

	if(aFilteredKeys != 0) delete[] aFilteredKeys;
}

void CRS_WeldGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	//long nCount;
	CString str;
	CGXData* pData = GetParam()->GetData();
	BOOL bEnable = awCols.GetSize() != 2;
	for(int nCount = 0; nCount < nNumData; nCount++)
	{
		BOOL bAuto = m_pData->GetAt(nCount).bAuto;
		for (int i = 0; i < awCols.GetSize(); i++)
		{
			if(i == 0)
				str = bAuto ? _T("1") : _T("0");
			else if(i == 1)
				str.Format(DATA_FMT, m_pData->GetAt(nCount).dy);
			else if(i == 2)
				str.Format(DATA_FMT, m_pData->GetAt(nCount).dz);
			else if(i == 3)
				str.Format(DATA_FMT, m_pData->GetAt(nCount).dLength);
			else if(i == 4)
				str.Format(DATA_FMT, m_pData->GetAt(nCount).dSpace);

			pData->StoreValueRowCol(nCount + 1, i + 1, str, gxOverride, GX_VT_NUMERIC);
			if(i == 1 || i == 2)
				SetStyleRange(CGXRange().SetCells(nCount + 1, i + 1), CGXStyle().SetHorizontalAlignment(DT_RIGHT));
			else if(i == 3 || i == 4)
				SetStyleRange(CGXRange().SetCells(nCount + 1, i + 1), CGXStyle().SetEnabled(!bAuto).SetHorizontalAlignment(DT_RIGHT));
		}
	}
}

void CRS_WeldGrid::SetHeaderTitle(T_UNIT_SYSTEM& us)
{
	CString csTitle;
	m_aTitle.RemoveAll();
	m_aType.RemoveAll();
	m_aUnit.RemoveAll();
	int nColCount = 5;
	SetColCount(nColCount);
	AddColInfo(_LS(IDS_CMD_Automatic), t_integer, 0);
	AddColInfo(_T("y"), t_real, 0);
	AddColInfo(_T("z"), t_real, 0);
	AddColInfo(_LS(IDS_WG_CMD_Reinforce_Weld_Length), t_real, 0);
	AddColInfo(_LS(IDS_WG_CMD_Reinforce_Weld_Space), t_real, 0);
	int aSize[] = { 40, 45,45,68,68 };
	for(int i = 0; i < nColCount; i++)
	{
		csTitle = m_aTitle[i];
		SetStyleRange(CGXRange(0, i + 1), CGXStyle()
			.SetValue(csTitle)
			.SetHorizontalAlignment(DT_RIGHT)
			.SetUserAttribute(ID_USER_ATTR_COL_ID, m_aTitle[i]));
		SetColWidth(i + 1, i + 1, aSize[i]);
	}
	SetColWidth(0, 0, 0);
}

void CRS_WeldGrid::OnLButtonUp(UINT nFlags, CPoint point)
{
	CTBBrowserWnd::OnLButtonUp(nFlags, point);

	ROWCOL nRow, nCol;
	int ht = HitTest(point, &nRow, &nCol);

	if(!ht) return;

	BOOL bOldLock = LockUpdate(TRUE);
	ChangeColumnStatus(nRow, nCol);
	LockUpdate(bOldLock);
	if(!bOldLock) Redraw();
}

void CRS_WeldGrid::ChangeColumnStatus(ROWCOL nRow, ROWCOL nCol)
{
	if(nCol == 1)
	{
		CGXData* pData = GetParam()->GetData();
		int nCheckAutoValue = _tstoi(GetValueRowCol(nRow, nCol));
		SetStyleRange(CGXRange(nRow, 4), CGXStyle().SetEnabled(nCheckAutoValue == 0));
		SetStyleRange(CGXRange(nRow, 5), CGXStyle().SetEnabled(nCheckAutoValue == 0));
		CString str = pData->GetValueRowCol(nRow, nCol);
		BOOL bAuto = _tstoi(str);
		if (bAuto)
		{
			str.Format(DATA_FMT, m_pWeldData->dLength);
			pData->StoreValueRowCol(nRow, 4, str, gxOverride, GX_VT_NUMERIC);
			str.Format(DATA_FMT, m_pWeldData->dSpace);
			pData->StoreValueRowCol(nRow, 5, str, gxOverride, GX_VT_NUMERIC);
		}
	}
}

BOOL CRS_WeldGrid::AddToDB(ROWCOL nRow)
{
	//Grid2Data();
	return TRUE;
}
BOOL CRS_WeldGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	//Grid2Data();
	return TRUE;
}
BOOL CRS_WeldGrid::DeleteFromDB(ROWCOL nRow)
{
	//Grid2Data();
	return TRUE;
}
BOOL CRS_WeldGrid::DeleteFromDB(CRowColArray& awRows)
{
	//Grid2Data();
	return TRUE;
}
BOOL CRS_WeldGrid::ModifyToDB(ROWCOL nRow)
{
	//Grid2Data();
	return TRUE;
}
BOOL CRS_WeldGrid::ModifyToDB(CStringArray& raOldKey, CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	//Grid2Data();
	return TRUE;
}

