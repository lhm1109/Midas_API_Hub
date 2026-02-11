// SoilParamAddLoadGrid.cpp: implementation of the CSoilParamAddLoadGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "SoilParamAddLoadGrid.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_common\wg_common_TBGrid.h"
#include "..\wg_base\wg_base_CompFunc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define D_MAX_COL 10

enum ColumnIndex
{
	COLINDEX_LEVEL_START,
	COLINDEX_LEVEL_TILDE,
	COLINDEX_LEVEL_END,
	COLINDEX_HEIGHT,
	COLINDEX_ANGLE,
	COLINDEX_DENSITY,
	COLINDEX_POISSON,
	COLINDEX_VS,
	COLINDEX_KH,
	COLINDEX_DISP
};

CString CSoilParamAddLoadGrid::m_aCurDefVal[] = { _T("1.0"), _T("~"), _T("2.0"), _T("1.0"), _T("30"), _T("18.0"),_T("0.3"), _T("100.0"), _T("4082.0"), _T("0.001") };
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSoilParamAddLoadGrid::CSoilParamAddLoadGrid()
{
#if defined(_CIVIL)
	m_bCivil = TRUE;
#else
	m_bCivil = FALSE;
#endif
	m_nWndWidth = 0;
	m_dGroundLevel = 0.0;
	m_bUseN = FALSE;
	m_pDoc = CDBDoc::GetDocPoint();
	m_pUnitCtrl = m_pDoc->m_pUnitCtrl;

	AddColInfo(_LS(IDS_CMD_LOAD_SOIL_PROP_TB_LEVEL), t_real, D_UNITSYS_BASE_LENGTH);
	AddColInfo(_LS(IDS_CMD_LOAD_SOIL_PROP_TB_LEVEL), t_real, D_UNITSYS_NONE);
	AddColInfo(_LS(IDS_CMD_LOAD_SOIL_PROP_TB_LEVEL), t_real, D_UNITSYS_BASE_LENGTH);
	AddColInfo(_LS(IDS_CMD_LOAD_SOIL_PROP_TB_HEIGHT), t_real, CUnitCtrl::m_POSP_UNIT.dHeight);
	AddColInfo(_LS(IDS_CMD_LOAD_SOIL_PROP_TB_ANGLE), t_real, CUnitCtrl::m_POSP_UNIT.dAngleOrN);
	AddColInfo(_LS(IDS_CMD_LOAD_SOIL_PROP_TB_DENSITY), t_real, CUnitCtrl::m_POSP_UNIT.dDensity);
	AddColInfo(_LS(IDS_CMD_LOAD_SOIL_PROP_TB_POISSON), t_real, CUnitCtrl::m_POSP_UNIT.dPoissonRatio);
	AddColInfo(_LS(IDS_CMD_LOAD_SOIL_PROP_TB_VS), t_real, CUnitCtrl::m_POSP_UNIT.dVs);
	AddColInfo(_LS(IDS_CMD_LOAD_SOIL_PROP_TB_KH), t_real, CUnitCtrl::m_POSP_UNIT.dKh);
	AddColInfo(_LS(IDS_CMD_LOAD_SOIL_PROP_TB_DISP), t_real, CUnitCtrl::m_POSP_UNIT.dDisp);
	

	m_aPrimaryKey.Add(0);



}

CSoilParamAddLoadGrid::~CSoilParamAddLoadGrid()
{

}

//////////////////////////////////////////////////////////////////////
// Interface
void CSoilParamAddLoadGrid::Initialize(CWnd* pParent, int nWidth)
{	

	m_nWndWidth = nWidth;

	SetTitle();

	T_UNIT_SYSTEM us;
	CTBCommon::Initialize(m_pDoc, us);

	SetRowHeightByDPI(0, 0, 36);
	SetColWidthByDPI(0, 0, 25);

	GetParam()->EnableUndo(FALSE);
	GetParam()->EnableTrackRowHeight(FALSE);
	GetParam()->EnableMoveCols(FALSE);        // columns can not be dragged
	GetParam()->EnableMoveRows(FALSE);
	GetParam()->SetNumberedRowHeaders(TRUE);

	EnableAppendRow(FALSE);
	EnableInsertRowByPressingKey(FALSE);

	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetMarkColHeader(FALSE); // Turn off pressed button effect for column headers
	pProp->SetMarkRowHeader(FALSE);
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL, // 현재 셀의 border 설정 변경
		(CGXStyle)pProp->sInvertNoBorder);

	LockUpdate(TRUE);

	SetStyleRange(CGXRange().SetCols(COLINDEX_LEVEL_START + 1, COLINDEX_LEVEL_END + 1), CGXStyle().SetEnabled(FALSE));

	SetStyleRange(CGXRange().SetCols(COLINDEX_LEVEL_START+1, COLINDEX_KH +1), CGXStyle()
		.SetHorizontalAlignment(DT_CENTER)
		.SetVerticalAlignment(DT_VCENTER)
		.SetValueType(GX_VT_NUMERIC)
		.SetFormat(GX_FMT_FIXED)
		.SetPlaces(2));

	SetStyleRange(CGXRange().SetCols(COLINDEX_DISP + 1, COLINDEX_DISP + 1), CGXStyle()
		.SetHorizontalAlignment(DT_CENTER)
		.SetVerticalAlignment(DT_VCENTER)
		.SetValueType(GX_VT_NUMERIC)
		.SetFormat(GX_FMT_FIXED)
		.SetPlaces(4));

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetSize(9))
		.SetVerticalAlignment(DT_BOTTOM)
		.SetAllowEnter(FALSE));
	ChangeColHeaderStyle(CGXStyle().SetFont(CGXFont_GC().SetBold(FALSE)));
	ChangeRowHeaderStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetBold(FALSE))
		.SetHorizontalAlignment(DT_RIGHT)
		.SetControl(GX_IDS_CTRL_HEADER));

	SetScrollBarMode(SB_HORZ, gxnEnabled);
	SetScrollBarMode(SB_VERT, gxnEnabled);

	// Sorting 정보 초기화
	m_aSortInfo.SetSize(1);
	m_aSortInfo[0].nRC = 0;
	m_aSortInfo[0].sortType = CGXSortInfo::numeric;
	m_aSortInfo[0].sortOrder = CGXSortInfo::ascending;

	LockUpdate(FALSE);
	Redraw();
}

void CSoilParamAddLoadGrid::SetTitle()
{
	m_aTitle.RemoveAll();
	m_aTitle.Add(_LS(IDS_CMD_LOAD_SOIL_PROP_TB_LEVEL));
	m_aTitle.Add(_LSX(2));
	m_aTitle.Add(_LSX(3));
	m_aTitle.Add(_LS(IDS_CMD_LOAD_SOIL_PROP_TB_HEIGHT));
	if (m_bUseN)	m_aTitle.Add(_LS(IDS_CMD_LOAD_SOIL_PROP_TB_N));
	else m_aTitle.Add(_LS(IDS_CMD_LOAD_SOIL_PROP_TB_ANGLE));
	m_aTitle.Add(_LS(IDS_CMD_LOAD_SOIL_PROP_TB_DENSITY));
	m_aTitle.Add(_LS(IDS_CMD_LOAD_SOIL_PROP_TB_POISSON));
	m_aTitle.Add(_LS(IDS_CMD_LOAD_SOIL_PROP_TB_VS));
	m_aTitle.Add(_LS(IDS_CMD_LOAD_SOIL_PROP_TB_KH));
	m_aTitle.Add(_LS(IDS_CMD_LOAD_SOIL_PROP_TB_DISP));
}

void CSoilParamAddLoadGrid::SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit)
{
	BOOL bEnableEarthPressureSeis = CDBDoc::IsEnableEarthPressureSeis();
	int* aSize = new int[D_MAX_COL] {};
	aSize[COLINDEX_LEVEL_START] = 25;
	aSize[COLINDEX_LEVEL_TILDE] = 10;
	aSize[COLINDEX_LEVEL_END] = 25;
	aSize[COLINDEX_HEIGHT] = 25;
	if (m_bCivil) aSize[COLINDEX_ANGLE] = 0;
	else aSize[COLINDEX_ANGLE] = 25;
	aSize[COLINDEX_DENSITY] = 35;
	aSize[COLINDEX_VS] = bEnableEarthPressureSeis ? 33 : 0;
	aSize[COLINDEX_KH] = bEnableEarthPressureSeis ? 33 : 0;
	aSize[COLINDEX_DISP] = bEnableEarthPressureSeis ? 35 : 0;

	if (m_bCivil) aSize[COLINDEX_POISSON] = 30;
	else aSize[COLINDEX_POISSON] = 0;


	int nSumSize = 0;
	for (int i = 0; i < D_MAX_COL; i++)
	{
		nSumSize += aSize[i];
	}
	int nVerBarSize = 25;
	nSumSize += nVerBarSize;

	double dRatio = (double)m_nWndWidth / nSumSize;
	for (int i = 0; i < D_MAX_COL; i++)
	{
		aSize[i] *= dRatio;
	}

	// (0,0)에 No 추가
	SetStyleRange(CGXRange(0, 0), CGXStyle()
		.SetValue(_LS(IDS_CMD_LOAD_SOIL_PROP_TB_NO))
		.SetUserAttribute(ID_USER_ATTR_COL_ID, _LS(IDS_CMD_LOAD_SOIL_PROP_TB_NO)));
	if (bInit) SetColWidth(0, 0, 20 * dRatio);

	SetCoveredCellsRowCol(0, COLINDEX_LEVEL_START + 1, 0, COLINDEX_LEVEL_END + 1);

	CString title, csUnit;
	ROWCOL nCol;
	int nColCount = GetColCount();
	if (nColCount != m_aTitle.GetCount()) { ASSERT(0); return; }
	if (nColCount != m_aUnit.GetCount()) { ASSERT(0); return; }
	for (int i = 0; i < nColCount; i++)
	{
		nCol = i + 1;
		BOOL isNCol = (m_bUseN && i == COLINDEX_ANGLE);

		CUnitCtrl::GetUnitSystem(m_aUnit[i], csUnit);
		if (!csUnit.IsEmpty() && !isNCol) title = m_aTitle[i] + _T("\n(") + csUnit + _T(")");
		else title = m_aTitle[i];

		if (i < COLINDEX_HEIGHT)
		{
			if (i == COLINDEX_LEVEL_START)
			{
				SetStyleRange(CGXRange(0, nCol), CGXStyle()
					.SetValue(title)
					.SetUserAttribute(ID_USER_ATTR_COL_ID, m_aTitle[i]));
			}
			else
			{
				SetStyleRange(CGXRange(0, nCol), CGXStyle()
					.SetUserAttribute(ID_USER_ATTR_COL_ID, m_aTitle[i]));
			}
		}
		else
		{
			// 컬럼 헤더 이름 및 ID를 설정
			SetStyleRange(CGXRange(0, nCol), CGXStyle()
				.SetValue(title)
				.SetUserAttribute(ID_USER_ATTR_COL_ID, m_aTitle[i]));
		}
		if (bInit) SetColWidth(nCol, nCol, aSize[i]);
	}
	delete aSize;
}

void CSoilParamAddLoadGrid::AddRow(double dHeight, int nRowCnt, BOOL bInsert /*= FALSE*/)
{
	ROWCOL nRow, nCol;
	if (bInsert)
		GetCurrentCell(nRow, nCol);
	else
		nRow = GetAppendRowNo();

	int idx = nRow - 1;

	if (idx > m_Data.GetCount() - 1)
		idx = m_Data.GetCount() - 1;
	if (idx < 0)
		idx = 0;

	T_POSP_TB_DATA Base;
	Base.dDensity	= CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_DENSITY, Base.dDensity);
	Base.dVs		= CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_VELOCITY, Base.dVs);
	Base.dKh		= CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_DENSITY, Base.dKh);
	Base.dDisp      = CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, Base.dDisp);

	if (m_Data.GetCount() > 0)
		Base = m_Data[idx];
	Base.dHeight = dHeight;
	for (int i = 0; i < nRowCnt; i++)
	{
		if (bInsert)
			m_Data.InsertAt(idx, Base);
		else
			m_Data.Add(Base);
	}
	MakeItemEx();
}

void CSoilParamAddLoadGrid::DelRow()
{
	CRowColArray awRows;
	GetSelectedRows(awRows);
	DeleteFromDB(awRows);
}

void CSoilParamAddLoadGrid::SetGroundLevel(double dGroundLevel, BOOL bMakeItem /*= TRUE*/)
{
	m_dGroundLevel = dGroundLevel;
	if(bMakeItem)
		MakeItemEx();
}

double CSoilParamAddLoadGrid::GetStartLevel(ROWCOL nRow)
{
	int nCurrentIdx = nRow - 1;
	if (nCurrentIdx < 0 || nCurrentIdx > m_Data.GetCount() - 1) { ASSERT(0); return  0.; }
	double dLevel = m_dGroundLevel;

	for (int i = 0; i < nCurrentIdx; i++)
		dLevel -= m_Data[i].dHeight;

	return dLevel;
}

ROWCOL CSoilParamAddLoadGrid::GetCurrentRowByLevel(double dLevelKey)
{
	//ground Level이랑 같으면 첫번째 행
	if (m_dGroundLevel == dLevelKey)
		return 1;
	
	ROWCOL nRow;
	double dLevel = m_dGroundLevel;
	for (int i = 0; i < m_Data.GetCount() - 1; i++)
	{
		dLevel -= m_Data[i].dHeight;
		if (dLevel == dLevelKey)
		{
			nRow = i + 2;
			break;
		}
	}

	return nRow;
}

void CSoilParamAddLoadGrid::SetUseN(BOOL bUseN, BOOL bMakeItem /*= TRUE*/)
{
	m_bUseN = bUseN;
	SetTitle();
	T_UNIT_SYSTEM UnitSystem;
	SetHeaderTitle(UnitSystem, FALSE);

	if (bMakeItem)
		MakeItemEx();
}

void CSoilParamAddLoadGrid::SetAngleorN(T_POSP_TB_DATA &rData, const double val)
{
	if (m_bUseN)
		rData.N = val;
	else
		rData.dAngle = val;
}

double CSoilParamAddLoadGrid::GetAngleorN(const T_POSP_TB_DATA &rData)
{
	double dVal;

	if (m_bUseN)
		dVal = rData.N;
	else
		dVal = rData.dAngle;

	return dVal;
}

BOOL CSoilParamAddLoadGrid::InsertRecordByRow(ROWCOL nRow, const T_POSP_TB_DATA &data)
{
	// 삽입 위치를 찾는다.
	VERIFY(InsertRecord(nRow));
	return SetValue(nRow, data);
}

BOOL CSoilParamAddLoadGrid::DeleteRecordByRow(ROWCOL nRow)
{
	// 삭제한다.
	return DeleteRecord(nRow);
}

void CSoilParamAddLoadGrid::SetData(CArray<T_POSP_BASE_DATA, T_POSP_BASE_DATA&>& aPospBase, BOOL bMakeItem /*= TRUE*/)
{
	int nSize = aPospBase.GetSize();
	m_Data.SetSize(nSize);

	for (int i = 0; i < nSize; i++)
	{
		m_Data[i].Initialize();
		m_Data[i].dHeight	= aPospBase[i].dHeight;
		if (m_bUseN) m_Data[i].N		= aPospBase[i].dAngleOrN;
		else		 m_Data[i].dAngle	= aPospBase[i].dAngleOrN;
		m_Data[i].dDensity	= aPospBase[i].dDensity;
		m_Data[i].dPoissonRatio = aPospBase[i].dPoissonRatio;
		m_Data[i].dVs		= aPospBase[i].dVs;
		m_Data[i].dKh		= aPospBase[i].dKh;
		m_Data[i].dDisp     = aPospBase[i].dDisp;
	}
	if(bMakeItem)
		MakeItemEx();
}

void CSoilParamAddLoadGrid::GetData(CArray<T_POSP_BASE_DATA, T_POSP_BASE_DATA&>& aPospBase)
{
	OnTerminate(D_TB_SAVE_ALWAYS);
	int nSize = m_Data.GetSize();
	aPospBase.SetSize(nSize);

	for (int i = 0; i < nSize; i++)
	{
		aPospBase[i].dHeight	= m_Data[i].dHeight;
		if (m_bUseN) aPospBase[i].dAngleOrN = m_Data[i].N;
		else		 aPospBase[i].dAngleOrN = m_Data[i].dAngle;
		aPospBase[i].dDensity	= m_Data[i].dDensity;
		aPospBase[i].dPoissonRatio = m_Data[i].dPoissonRatio;
		aPospBase[i].dVs		= m_Data[i].dVs;
		aPospBase[i].dKh		= m_Data[i].dKh;
		aPospBase[i].dDisp      = m_Data[i].dDisp;
	}				
}

//////////////////////////////////////////////////////////////////////
// Operation

//////////////////////////////////////////////////////////////////////
// Override

BOOL CSoilParamAddLoadGrid::ValidateField(CString value, int nColID)
{
	if (nColID > GetColCount()) return FALSE;

	int nErrCode;
	if (nColID >= COLINDEX_HEIGHT)
	{
		if (nColID == COLINDEX_ANGLE && m_bUseN)
		{
			if (!IsValidLongNumber(value, nErrCode))
			{
				return InvalidLongNumber(nErrCode);
			}
		}
		else if (!IsValidDoubleNumber(value, nErrCode))
		{
			return InvalidDoubleNumber(nErrCode);
		}

		double dVal = _tstof(value);
		CString strMsg = _T(""), strCondition;
		switch (nColID)
		{
		case COLINDEX_HEIGHT:
			if (dVal <= 0.0)
			{
				strCondition.Format(_LS(IDS_CMD_ERR_SOIL_PORP_GREATER_THAN_0_CONDTION), _LS(IDS_CMD_LOAD_SOIL_PROP_TB_HEIGHT));
				strMsg.Format(_LS(IDS_CMD_ERR_SOIL_PROP_GRID_INVALID), strCondition);
				SetWarningText(strMsg);
				return FALSE;
			}
			break;
		case COLINDEX_ANGLE:
			if (m_bUseN)
			{
				int nVal = static_cast<int>(dVal); //N은 자연수임
				if (nVal < 1 || nVal > 50)
				{
					strMsg.Format(_LS(IDS_CMD_ERR_SOIL_PROP_GRID_INVALID), _LS(IDS_CMD_ERR_SOIL_PORP_N_CONDTION));
					SetWarningText(strMsg);
					return FALSE;
				}
			}
			else
			{
				if (dVal < 0.0 || dVal > 60.0)
				{
					strMsg.Format(_LS(IDS_CMD_ERR_SOIL_PROP_GRID_INVALID), _LS(IDS_CMD_ERR_SOIL_PORP_ANGLE_CONDTION));
					SetWarningText(strMsg);
					return FALSE;
				}
			}
			break;
		case COLINDEX_DENSITY:
			if (dVal <= 0.0)
			{
				strCondition.Format(_LS(IDS_CMD_ERR_SOIL_PORP_GREATER_THAN_0_CONDTION), _LS(IDS_CMD_LOAD_SOIL_PROP_TB_DENSITY));
				strMsg.Format(_LS(IDS_CMD_ERR_SOIL_PROP_GRID_INVALID), strCondition);
				SetWarningText(strMsg);
				return FALSE;
			}
			break;
		case COLINDEX_VS:
			if (dVal <= 0.0)
			{
				strCondition.Format(_LS(IDS_CMD_ERR_SOIL_PORP_GREATER_THAN_0_CONDTION), _LS(IDS_CMD_LOAD_SOIL_PROP_TB_VS));
				strMsg.Format(_LS(IDS_CMD_ERR_SOIL_PROP_GRID_INVALID), strCondition);
				SetWarningText(strMsg);
				return FALSE;
			}
			break;
		case COLINDEX_KH:
			if (dVal <= 0.0)
			{
				strCondition.Format(_LS(IDS_CMD_ERR_SOIL_PORP_GREATER_THAN_0_CONDTION), _LS(IDS_CMD_LOAD_SOIL_PROP_TB_KH));
				strMsg.Format(_LS(IDS_CMD_ERR_SOIL_PROP_GRID_INVALID), strCondition);
				SetWarningText(strMsg);
				return FALSE;
			}
			break;
		}
	}

	return TRUE;
}

// 삽입이나 수정전에 Record의 Validation을 검사한다.
BOOL CSoilParamAddLoadGrid::ValidateRecord(CStringArray &value, CRowColArray &aCols)
{
	CString msg = _T("");
	int nPos;

	int nColCount = GetColCount();
	if (nColCount > m_aTitle.GetCount())	{ ASSERT(0); return FALSE; }
	if (nColCount > value.GetCount())		{ ASSERT(0); return FALSE; }
	if (nColCount > aCols.GetCount())		{ ASSERT(0); return FALSE; }
	for (int i = 0; i < nColCount; i++)
	{
		if(i == COLINDEX_LEVEL_START || i == COLINDEX_LEVEL_TILDE || i == COLINDEX_LEVEL_END) continue;
		nPos = aCols[i] - 1;
		if (value[i] == _T(""))
			msg += ((msg != _T("")) ? _T("\n") : _T("")) + m_aTitle[i] + _LS(IDS_WG_CMD__ADDD___has_not_been_entered_);
		else if (!ValidateField(value[i], nPos)) return FALSE;
	}
	if (msg == _T("")) return TRUE;
	else SetWarningText(msg);
	return FALSE;
}

// 이 함수는 Paste할 때 호출되므로 False를 리턴할 때는 ValidateField와 
// Validate Record에서 설정된 Warning Text를 없애주고 리턴한다.
BOOL CSoilParamAddLoadGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, int &key, T_POSP_TB_DATA &data)
{
	int nValCount = value.GetCount();
	int nColCount = aCols.GetCount();
	if (nValCount > nColCount) { ASSERT(0); return FALSE; }
	// 각 필드에 대해 Validation을 검사한다.
	for (int i = 0; i < nValCount; i++)
	{
		if (aCols[i] > nValCount) { ASSERT(0); return FALSE; }
		if (!ValidateField(value[aCols[i]-1], i))
		{
			//SetWarningText(_T(""));
			return FALSE;
		}
	}
	
	// 레코드에 대해 Validation을 검사한다.
	if (!ValidateRecord(value, aCols))
	{
		//SetWarningText(_T(""));
		return FALSE;
	}

	// 레코드를 만들어서 리턴한다.
	if (GetColCount() > nColCount) { ASSERT(0); return FALSE; }
	int nPos;
	for (int i = 0; i < GetColCount(); i++)
	{
		nPos = aCols[i] - 1; // col id i에 해당하는 value가 저장된 위치
		if (nPos > nValCount - 1)	{ ASSERT(0); return FALSE; }
		ConvStrToData(i, value[nPos], data);
	}

	return TRUE;
}

BOOL CSoilParamAddLoadGrid::AddToDB(ROWCOL nRow)
{
	// Record를 만들어서 DB에 입력한다.
	T_POSP_TB_DATA data;

	GetValue(nRow, data);

	if (!AddData(nRow, data)) return FALSE;

	MakeItemEx();

	return TRUE;
}

BOOL CSoilParamAddLoadGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	CArray<T_POSP_TB_DATA, T_POSP_TB_DATA&> aData;
	CRowColArray awRows;
	awRows.RemoveAll();
	GetSelectedRows(awRows);
	for (int i = 0; i <awRows.GetSize(); i++)
	{
		if (IsAppendRow(awRows[i]))
			return FALSE;
	}

	for (int i = 0; i < raData.GetSize(); i++)
	{
		aData.Add(*((T_POSP_TB_DATA*)raData.GetAt(i)));
	}
	if (!AddData(awRows, aData)) return FALSE;

	MakeItemEx();

	return TRUE;
}

BOOL CSoilParamAddLoadGrid::DeleteFromDB(ROWCOL nRow)
{
	if(nRow < 0) return TRUE;
	m_Data.RemoveAt(nRow-1);
	if (!DeleteRecord(nRow)) return FALSE;

	MakeItemEx();

	return TRUE;
}

BOOL CSoilParamAddLoadGrid::DeleteFromDB(CRowColArray& awRows)
{
	if (awRows.GetSize() <= 0) return TRUE;

	CRowColArray awDelRows;
	ROWCOL nHeaderRow = GetHeaderRows();
	for (int i = awRows.GetSize()-1 ; i >= 0; i--)
	{
		if (nHeaderRow==awRows[i]) continue;
		if (IsAppendRow(awRows[i])) continue;
		if (awRows[i] > m_Data.GetCount()) break;
		m_Data.RemoveAt(awRows[i] - 1);
 		awDelRows.Add(awRows[i]);
	}

	if (!DeleteRecord(awDelRows)) return FALSE;

	MakeItemEx();

	return TRUE;
}

BOOL CSoilParamAddLoadGrid::ModifyToDB(ROWCOL nRow)
{
	T_POSP_TB_DATA data;

	GetValue(nRow, data);

	// 수정한다. 실패하면 에러 리턴
	if (!ModifyData(nRow, data)) return FALSE;
	MakeItemEx();

	return TRUE;
}

BOOL CSoilParamAddLoadGrid::ModifyToDB(CStringArray& raOldKey,
	CArray<void*, void*>& raKey,
	CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	ASSERT(raKey.GetSize() == raOldKey.GetSize());
	CArray<T_POSP_TB_DATA, T_POSP_TB_DATA&> aData;
	int nSize = raData.GetSize();
	CRowColArray awRows;
	for (int i = 0; i < nSize; i++)
	{
		double OldKey = _tstof(raOldKey.GetAt(i));
		awRows.Add(GetCurrentRowByLevel(OldKey));
		aData.Add(*((T_POSP_TB_DATA*)raData.GetAt(i)));
	}
	if (!ModifyData(awRows, aData)) return FALSE;
	MakeItemEx();

	return TRUE;
}

CString CSoilParamAddLoadGrid::GetNewKey()
{
	CString value(_T("0"));
	return value;
}

CString CSoilParamAddLoadGrid::GetDefValue(int nIndex)
{
	return m_aCurDefVal[nIndex];
}

CString* CSoilParamAddLoadGrid::GetDefValueArray()
{
	return m_aCurDefVal;
}

BOOL CSoilParamAddLoadGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols,
	CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	int* pKey;
	T_POSP_TB_DATA* pData;
	pKey = new int;
	pData = new T_POSP_TB_DATA;
	aKey.Add((void*)pKey);
	aData.Add((void*)pData);
	if (!ValidateAndMakeRecord(value, awCols, *pKey, *pData)) return FALSE;
	return TRUE;
}

void CSoilParamAddLoadGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	double* pKey;
	T_POSP_TB_DATA* pData;
	ASSERT(aKey.GetSize() == aData.GetSize());
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		pKey = (double*)aKey.GetAt(i);
		pData = (T_POSP_TB_DATA*)aData.GetAt(i);
		delete pKey;
		delete pData;
	}
}

long CSoilParamAddLoadGrid::GetCountData()
{
	return m_Data.GetSize();
}

void CSoilParamAddLoadGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	ASSERT(0); // no filtering
}

void CSoilParamAddLoadGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	T_POSP_TB_DATA Data;

	if (nNumData > m_Data.GetCount()) { ASSERT(0); return; }
	long nCount;
	for (nCount = 0; nCount < nNumData; nCount++)
	{
		Data = m_Data[nCount];

		SetValue(GetParam()->GetData(), nCount + 1, Data, awCols);
	} // end of loop nCount

}

void CSoilParamAddLoadGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{

	CDBDoc* pDoc = m_pDoc;
	T_POSP_TB_DATA Data;
	int nColCount = GetColCount();
	if (nColCount > EditingRecord.GetCount()) { ASSERT(0); return; }
	for (int i = 0; i < nColCount; i++)
	{
		ConvStrToData(i, EditingRecord[i], Data);
	}
	// 현재 값을 설정한다.
	SetValue(ncRow, Data);
}

/////////////////////////////////////////////////////////////////////////////
// CLD_CTRL_WIND_ShapeFactGrid Implementation
BOOL CSoilParamAddLoadGrid::GetValue(ROWCOL nRow, T_POSP_TB_DATA &data)
{
	ROWCOL nCol;
	CString str;
	int nColCount = GetColCount();
	if (nColCount > m_aTitle.GetCount()) { ASSERT(0); return FALSE; }
	for (int i = 0; i < nColCount; i++)
	{
		VERIFY(GetColumnNoFor(m_aTitle[i], nCol));
		str = GetValueRowCol(nRow, nCol);
		// 수식인 경우 값으로 변환한다.
		// Validation이 OK인 Field이므로 항상 TRUE 리턴
		VERIFY(ConvertFormulaToValue(str));

		ConvStrToData(i, str, data);
	}
	return TRUE;
}

BOOL CSoilParamAddLoadGrid::SetValue(ROWCOL nRow, const T_POSP_TB_DATA &data)
{
	int nColCount = GetColCount();
	CRowColArray awCols;
	awCols.SetSize(nColCount);
	if (nColCount > m_aTitle.GetCount()) { ASSERT(0); return FALSE; }
	for (int i = 0;i < nColCount; i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, data, awCols);
}

BOOL CSoilParamAddLoadGrid::SetValue(CGXData* pData, ROWCOL nRow, const T_POSP_TB_DATA &data, CRowColArray &awCols)
{
	CString str;
	double dStartLevel = GetStartLevel(nRow);
	int nColCount = GetColCount();
	if (nColCount > awCols.GetCount()) { ASSERT(0); return FALSE; }
	for (int i = 0; i < nColCount; i++)
	{
		if (i == COLINDEX_LEVEL_START)
			str.Format(_T("%g"), dStartLevel);
		else if( i == COLINDEX_LEVEL_END)
			str.Format(_T("%g"), dStartLevel - data.dHeight);
		else 
			ConvDataToStr(i, data, str);
		pData->StoreValueRowCol(nRow, awCols[i], str, gxOverride, GX_VT_NUMERIC);
	}

	// ReadOnly 이기 때문에 마지막 Row 는 Hide
	int nRowCount = GetRowCount();
	HideRows(nRowCount, nRowCount);

	return TRUE;
}

void CSoilParamAddLoadGrid::ConvStrToData(int i, CString& value, T_POSP_TB_DATA& data)
{
	switch (i)
	{
	case COLINDEX_HEIGHT:
		data.dHeight = _tstof(value);
		break;
	case COLINDEX_ANGLE:
		SetAngleorN(data, _tstof(value));
		break;
	case COLINDEX_DENSITY:
		data.dDensity = _tstof(value);
		break;
	case COLINDEX_VS:
		data.dVs = _tstof(value);
		break;
	case COLINDEX_KH:
		data.dKh = _tstof(value);
	case COLINDEX_DISP:
		data.dDisp = _tstof(value);
		break;
	case COLINDEX_POISSON:
		data.dPoissonRatio = _tstof(value);
		break;
	}
}

void CSoilParamAddLoadGrid::ConvDataToStr(int i, const T_POSP_TB_DATA& data, CString& value)
{
	switch (i)
	{
	case COLINDEX_LEVEL_TILDE:
		value = _T("~");
		break;
	case COLINDEX_HEIGHT:
		value.Format(_T("%g"), data.dHeight);
		break;
	case COLINDEX_ANGLE:
		value.Format(_T("%g"), GetAngleorN(data));
		break;
	case COLINDEX_DENSITY:
		value.Format(_T("%g"), data.dDensity);
		break;
	case COLINDEX_VS:
		value.Format(_T("%g"), data.dVs);
		break;
	case COLINDEX_KH:
		value.Format(_T("%g"), data.dKh);
		break;
	case COLINDEX_DISP:
		value.Format(_T("%g"), data.dDisp);
		break;
	case COLINDEX_POISSON:
		value.Format(_T("%g"), data.dPoissonRatio);
		break;
	}
}

BOOL CSoilParamAddLoadGrid::AddData(ROWCOL nRow, T_POSP_TB_DATA& data)
{
	CRowColArray awRows;
	CArray<T_POSP_TB_DATA, T_POSP_TB_DATA&> aData;
	awRows.Add(nRow);
	m_Data.InsertAt(nRow-1, data);
	UpdateBuffer(0, awRows, aData);

	return TRUE;
}

BOOL CSoilParamAddLoadGrid::AddData(CRowColArray& awRows, CArray<T_POSP_TB_DATA, T_POSP_TB_DATA&> &aData, BOOL bNotify)
{
	CArray<int, int> aOldKey;

	// 동일한 key값 중복 검토
	int nSize = awRows.GetSize();
	if (nSize > aData.GetCount()) { ASSERT(0); return FALSE; }
	for (int i = 0; i < nSize; i++)
	{
		m_Data.InsertAt(awRows[i] - 1, aData[i]);
	}
	UpdateBuffer(0, awRows, aData);

	return TRUE;
}

BOOL CSoilParamAddLoadGrid::ModifyData(ROWCOL nRow, T_POSP_TB_DATA data)
{
	CArray<T_POSP_TB_DATA, T_POSP_TB_DATA&> aData;

	if(nRow > m_Data.GetCount()) { ASSERT(0); return FALSE; }
	m_Data[nRow - 1] = data;
	CRowColArray awRows;
	awRows.Add(nRow);
	aData.Add(data);
	UpdateBuffer(2, awRows, aData);

	return TRUE;
}

BOOL CSoilParamAddLoadGrid::ModifyData(CRowColArray& awRows, CArray<T_POSP_TB_DATA, T_POSP_TB_DATA&> &aData)
{

	CArray<T_POSP_TB_DATA, T_POSP_TB_DATA&> aOrgData;
	aOrgData.Copy(m_Data);

	if (!DeleteFromDB(awRows))
		return FALSE;
	if (!AddData(awRows, aData, FALSE))
	{
		m_Data.Copy(aOrgData);
		return FALSE;
	}

	return TRUE;
}

void CSoilParamAddLoadGrid::UpdateBuffer(int nCmd, CRowColArray& awRows, CArray<T_POSP_TB_DATA, T_POSP_TB_DATA&> &aData)
{
	// 현재 Row, Col을 저장 : 나중에 복구하기 위함
	ROWCOL ncRow, ncCol;
	GetCurrentCell(ncRow, ncCol);

	BOOL bOldLock = LockUpdate(TRUE);

	int nSize = awRows.GetSize();
	if (nSize > aData.GetCount()) { ASSERT(0); return; }
	for (int i = 0; i < nSize; i++)
	{
		switch (nCmd)
		{
		case(0):
			// 삽입될 위치(nRow)를 찾아 넣는다.
			InsertRecordByRow(awRows[i], aData[i]);
			break;
		case(1):
			// 삭제될 위치(nRow)를 찾아서 지운다.
			//DeleteRecordByRow(aKey[i], aData[i]);
			DeleteRecordByRow(awRows[i]);
			break;
		case(2):
			//DeleteRecordByRow(aOldKey[i], aData[i]);
			DeleteRecordByRow(awRows[i]);
			InsertRecordByRow(awRows[i], aData[i]);
			break;
		default:
			break;
		}
	} // end of while

	// 원래 row, col을 복구하려고 시도

	unsigned int nOldMode = m_nEditMode;
	if (nOldMode != CTBBrowserWnd::addnew)
	{ // addnew 상태에서 복구하면 추가되는 Row가 삭제(CancelEdit에서)된다.
		m_nEditMode = CTBBrowserWnd::noMode;
		if (GetRowCount() < ncRow) ncRow = GetRowCount();
		BOOL bSet = SetCurrentCell(ncRow, ncCol);
		if (!bSet) bSet = SetRightValidCell(ncRow, ncCol);
		if (!bSet) bSet = SetLeftValidCell(ncRow, ncCol);
		m_nEditMode = nOldMode;
	}

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

BOOL CSoilParamAddLoadGrid::OnPasteFromClipboard(const CGXRange &range)
{
	return CTBCommon::OnPasteFromClipboard(range);
}

BOOL CSoilParamAddLoadGrid::Copy()
{
	return CTBCommon::Copy();
}

