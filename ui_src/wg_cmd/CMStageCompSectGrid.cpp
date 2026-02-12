#include "stdafx.h"
#include "wg_cmd.h"
#include "CMStageCompSectGrid.h"
#include "CMStageCompSectStiffDlg.h"
#include "CMStageCompSectItemDlg.h"

#include "..\wg_db\wg_db_AttrCtrl.h"

#include "..\wg_common\wg_common_TBGrid.h"

// #ifdef _DEBUG
// #define new DEBUG_NEW
// #undef THIS_FILE
// static char THIS_FILE[] = __FILE__;
// #endif

#define ITEM_COLM_NUM             15

#define MATL_TYPE_COL              2
#define MATL_COL                   3
#define AGE_COL                    5
#define H_COL                     10
#define VS_COL                    11
#define M_COL                     12
#define STIFFNESS_COL             13
#define STIFSCALE_COL             14
#define STIF_COMP_COL             15

#define MATL_TYPE_ELEMENT         _LS(IDS_CMD_CSCS__Element)
#define MATL_TYPE_MATERIAL        _LS(IDS_CMD_CSCS__Material)
#define COMPOSITE_ACTIVE_STAGE    _LS(IDS_CMD_CSCS__Active_Stage)

/////////////////////////////////////////////////////////////////////////////
// CCMStageCompSectGrid
CString CCMStageCompSectGrid::m_aCurDefVal[] = {_T(""), _T(""), _T(""), _T(""), _T("0"), _T("0.0"), _T("0.0"), _T("0.0"), _T("0.0"), _T("0.0"), _T("0.0"), _T("0.0")};

void CCMStageCompSectGrid::SetInitData()
{
	// 초기화를 0으로 해줄 것이 아니라 현재 Part Number를 가져올 것.
	//m_nCurRowNumber = 0;
	//m_nPreRowNumber = 0;
	m_nCurRowNumber = m_pParent->GetPartNumber();
	m_nPreRowNumber = m_pParent->GetPartNumber();

	m_nSubType = -1;
	m_bTapered = FALSE;
	m_nSectType = 0;

	m_aScale.RemoveAll();
	m_aStiffAuto_I.RemoveAll();
	m_aStiffAuto_J.RemoveAll();
	m_aStiff_I.RemoveAll();
	m_aStiff_J.RemoveAll();

	CString aCurDefVal[] = {_T(""), MATL_TYPE_ELEMENT, _T(""), COMPOSITE_ACTIVE_STAGE, _T("0"), _T("0.0"), _T("0.0"), _T("0.0"), _T("0.0"), _T("0.0"), _T("0.0"), _T("0.0")};
	for(int i=0; i<sizeof(aCurDefVal)/sizeof(CString); i++)
		m_aCurDefVal[i] = aCurDefVal[i];
}

CCMStageCompSectGrid::CCMStageCompSectGrid(CWnd* pParent)
{
	m_pParent = 0;
	ASSERT(pParent);
	m_pParent = (CCMStageCompSectItemDlg*) pParent;

	m_nCompositType = -2; // Normal
	m_pDoc = 0;
	SetInitData();
}

CCMStageCompSectGrid::~CCMStageCompSectGrid()
{
}

///////////////////////////////////////////////////////////////////////////////
// User Defined Finction

// 디폴트 값을 세팅하는 함수
void CCMStageCompSectGrid::SetDefaultData(ROWCOL nRow)
{
	if(nRow > m_nCurRowNumber) return;

	for(int nCol=1; nCol<=ITEM_COLM_NUM-3; nCol++)
	{
		if(nCol == 1)     // Part Column
		{
			CString str = _T("");
			if(m_nSubType == -1 || m_nSubType == 2)
			{
				str.Format(_T("%d"), nRow);
			}
			else if(m_nSubType == 0)
			{
				if(nRow == 1)      str = _ULS(1+2);
				else if(nRow == 2) str = _ULS(3);
				else ASSERT(0);
			}
			else if(m_nSubType == 1)
			{
				if(nRow == 1)      str = _ULS(1);
				else if(nRow == 2) str = _ULS(2+3);
				else ASSERT(0);
			}
			else 
				ASSERT(0);
				
			SetValueRange(CGXRange(nRow, nCol), str);
		}
		else if(nCol == MATL_TYPE_COL)
		{
			if(m_nSectType == 1)  SetValueRange(CGXRange(nRow, nCol), MATL_TYPE_MATERIAL);
			else                  SetValueRange(CGXRange(nRow, nCol), MATL_TYPE_ELEMENT);
		}
		else 
			SetValueRange(CGXRange(nRow, nCol), m_aCurDefVal[nCol-1]);
	}
}

// 주어진 데이터를 Setting
void CCMStageCompSectGrid::SetData(T_CSCS_BASE& rData)
{
	ROWCOL nRow = GetRowNember(rData.nPart);

	ASSERT(nRow <= m_nCurRowNumber);
	if(nRow > m_nCurRowNumber) return;

	BOOL bOldLock = LockUpdate(TRUE);

	// Material Type
	int nMatlType = (rData.MatlKey == 0) ? 0 : 1;
	CString strMatlType = (rData.MatlKey == 0) ? MATL_TYPE_ELEMENT : MATL_TYPE_MATERIAL;

	SetValueRange(CGXRange(nRow, 2), strMatlType);
	EnableDisableColumns(nRow, nMatlType);
	
	// Material
	if(strMatlType == MATL_TYPE_MATERIAL)
	{
		T_MATL_D data;
		m_pDoc->m_pAttrCtrl->GetMatl(rData.MatlKey, data);
		CString strName = _T("");
		strName.Format(_T("%d: "), rData.MatlKey);
		strName += data.Name;
		SetValueRange(CGXRange(nRow, 3), strName);
	}

	// Composite Stage
	CString strCompositeStage = _T("");
	if(rData.CompStagKey == 0)
	{
		strCompositeStage = COMPOSITE_ACTIVE_STAGE;
	}
	else
	{
		T_STAG_D data;
		m_pDoc->m_pAttrCtrl->GetStag(rData.CompStagKey, data);
		strCompositeStage = data.StageName;
	}
	SetValueRange(CGXRange(nRow, 4), strCompositeStage);
	
	// Age
	SetValueRange(CGXRange(nRow, AGE_COL), double(rData.dAge));

	// Cy, Cz
	SetValueRange(CGXRange(nRow, 6), double(rData.YBar));
	SetValueRange(CGXRange(nRow, 7), double(rData.ZBar));
	SetValueRange(CGXRange(nRow, 8), double(rData.YBarJ));
	SetValueRange(CGXRange(nRow, 9), double(rData.ZBarJ));

	// h
	SetValueRange(CGXRange(nRow, H_COL), double(rData.dH));

	// V/S
	SetValueRange(CGXRange(nRow, VS_COL), double(rData.dVS));

	// M
	SetValueRange(CGXRange(nRow, M_COL), double(rData.dM));

	// Scale & Stiffness
	m_aScale.SetAt(nRow-1, rData.StiffScale);
 
	// General Composite 단면의 파트별 강성은 SectD에 있는 것으로 출력하도록 함
	// General Composite 단면인 경우 part 별 comp.Stiffness 결과는 SectD에 있는 값으로 재계산 한다.
	// ( SetInitStiffnessForGeneral 함수 => SectBefore.SectI.aGeneralPart의 결과로 세팅함.)
	// 그런데 T_CSCS_D에 있는 part 별 Stiffness 결과(이전 버전에 저장된 값)와 SectD에 있는 part별 결과가 다를 경우
	// 테이블 결과가 달라져서 아래처럼 처리함.
	if(m_nCompositType != 2)
	{
		m_aStiff_I.SetAt(nRow-1, rData.Stiffness);
		m_aStiff_J.SetAt(nRow-1, rData.StiffnessJ);
	}
	
	LockUpdate(bOldLock);

	if(!bOldLock) Redraw();
}

// 이 함수는 부모의 void CCMStageCompSectItemDlg::SetInitGrid(T_SECT_K nKey)에서 그리드 초기화마다 호출
// 1) 처음 띄울때, 2) 섹션 바뀔때 호출된다 (섹션변경 -> 타입초기화 -> 그리드초기화 과정을 거침)
void CCMStageCompSectGrid::SetInitH()
{
	int nSize = GetRowCount();
	CArray<double, double> aResult;

	// MNET:2840 20070612 mylee - H 값 자동계산, 한번에 두 Row 받아옴
	if(m_pParent->IsModifyMode() || !m_pParent->GetAutoCalcH(nSize, aResult)) // 섹션에 따른 dH 계산해서 넘겨주는 함수
	{
		aResult.RemoveAll();
		aResult.SetSize(nSize);
	}
	for(int nRow=1; nRow<=GetRowCount(); nRow++)
	{
		double dH = 0.0;
		if(nRow-1 < aResult.GetSize()) dH = aResult[nRow-1];
		SetValueRange(CGXRange(nRow, H_COL), double(dH)); // 초기는 무조건 Elem 이므로 재질 고려 필요 없음
	} 
}

void CCMStageCompSectGrid::SetInitCentroidForGeneral(T_SECT_D& data)
{
	int nSize = GetRowCount();

	if(data.SectBefore.SectI.aGeneralPart.GetSize() < nSize) return;

	for(int nRow=1; nRow<=GetRowCount(); nRow++)
	{
		SetValueRange(CGXRange(nRow, 6), double(data.SectBefore.SectI.aGeneralPart[nRow-1].YBar));
		SetValueRange(CGXRange(nRow, 7), double(data.SectBefore.SectI.aGeneralPart[nRow-1].ZBar));
		if(m_bTapered)
		{
			SetValueRange(CGXRange(nRow, 8), double(data.SectBefore.SectJ.aGeneralPart[nRow-1].YBar));
			SetValueRange(CGXRange(nRow, 9), double(data.SectBefore.SectJ.aGeneralPart[nRow-1].ZBar));
		}
	} 
}

void CCMStageCompSectGrid::SetInitScaleFactorForGeneral(T_SECT_D& data)
{
	// MQC:4060 mylee 20110803 - 1) Factor 계산시에 E, G 값에 의해 보정된 값으로 Factor 가 나오도록 수정, Tapered 인 경우는 평균
	// 2) 여기서 계산 안하고 솔버에 넘길때 계산하는 것으로 수정. 여기서 입력한 값에다 곱해서 솔버로 넘김 (이정우)
	return;

//   int nSize = GetRowCount();
// 
//   if(data.SectBefore.SectI.aGeneralPart.GetSize() < nSize) return;
// 
//   for(int nRow=1; nRow<=GetRowCount(); nRow++)
//   {
//     int idx = nRow-1;
//     if(idx >= 1 && idx < m_aScale.GetSize())
//     {
//       T_GSEC_PART& Pre = data.SectBefore.SectI.aGeneralPart[idx-1];
//       T_GSEC_PART& Cur = data.SectBefore.SectI.aGeneralPart[idx];
// 
//       // 20080626 mylee - 세 개만 계산. 나머지는 솔버에서 계산한다고 함 (이정우 정진상)
//       //if(Cur.Stiffness.Area != 0.) m_aScale[idx].Area = (Cur.StiffnessSum.Area - Pre.StiffnessSum.Area) / Cur.Stiffness.Area;
//       if(Cur.Stiffness.Asy != 0.) m_aScale[idx].Asy = (Cur.StiffnessSum.Asy - Pre.StiffnessSum.Asy) / Cur.Stiffness.Asy;
//       if(Cur.Stiffness.Asz != 0.) m_aScale[idx].Asz = (Cur.StiffnessSum.Asz - Pre.StiffnessSum.Asz) / Cur.Stiffness.Asz;
//       if(Cur.Stiffness.Rxx != 0.) m_aScale[idx].Rxx = (Cur.StiffnessSum.Rxx - Pre.StiffnessSum.Rxx) / Cur.Stiffness.Rxx;
//       //if(Cur.Stiffness.Ryy != 0.) m_aScale[idx].Ryy = (Cur.StiffnessSum.Ryy - Pre.StiffnessSum.Ryy) / Cur.Stiffness.Ryy;
//       //if(Cur.Stiffness.Rzz != 0.) m_aScale[idx].Rzz = (Cur.StiffnessSum.Rzz - Pre.StiffnessSum.Rzz) / Cur.Stiffness.Rzz;
//       //if(Cur.Stiffness.WArea != 0.) m_aScale[idx].WArea = (Cur.StiffnessSum.WArea - Pre.StiffnessSum.WArea) / Cur.Stiffness.WArea;
// 
//     }
//   } 
}

void CCMStageCompSectGrid::SetInitStiffnessForGeneral(T_SECT_D& data)
{
	int nSize = GetRowCount();

	if(data.SectBefore.SectI.aGeneralPart.GetSize() < nSize) return;

	for(int nRow=1; nRow<=GetRowCount(); nRow++)
	{
		int idx = nRow-1;
		if(idx < m_aScale.GetSize())
		{
			T_GSEC_PART& Cur = data.SectBefore.SectI.aGeneralPart[idx];
			m_aStiff_I[idx] = Cur.Stiffness;

			if(m_bTapered)
			{
				T_GSEC_PART& CurJ = data.SectBefore.SectJ.aGeneralPart[idx];
				m_aStiff_J[idx] = CurJ.Stiffness;
			}

		}
	} 
}

// 각 로우를 초기화 
void CCMStageCompSectGrid::SetInitRows(ROWCOL nFromRow, ROWCOL nToRow)
{
	BOOL bOldLock = LockUpdate(TRUE);
	SetStyleRange(CGXRange().SetCols(1), CGXStyle()
		.SetReadOnly(FALSE));

	m_aScale.SetSize(nToRow);
	m_aStiffAuto_I.SetSize(nToRow);
	m_aStiffAuto_J.SetSize(nToRow);
	m_aStiff_I.SetSize(nToRow);
	m_aStiff_J.SetSize(nToRow);

	for(int nRow=nFromRow; nRow<=nToRow; nRow++)
	{
		m_aScale[nRow-1].Initialize(1.0);
// MNET:XXXX-HSSHIM-20100310.  
// SRC 단면 강성 합성시에 0.8을 써야하는 근거가 더이상 없고,
// 사용자에게 혼동을 줄 수 있어서 Civil과 동일하게 1.0으로 변경 
//     #if defined(_MGEN)
//     if(m_bSRC && nRow!=1) m_aScale[nRow-1].Initialize(0.8);
//     #endif

		m_aStiff_I[nRow-1].Initialize(0.0);
		m_aStiff_J[nRow-1].Initialize(0.0);
		m_aStiffAuto_I[nRow - 1].Initialize(0.0);
		m_aStiffAuto_J[nRow - 1].Initialize(0.0);

		SetDefaultData(nRow);
		SetRowHeight(nRow, nRow, globalUtils.ScaleByDPI(18));

		ChangeColumnStatus(nRow, MATL_TYPE_COL);
	} 

	SetStyleRange(CGXRange().SetCols(1), CGXStyle()
		.SetReadOnly(TRUE));
	LockUpdate(bOldLock);
	if(!bOldLock) Redraw();
}

// Type에 따라 바뀌는 Style 정의 
void CCMStageCompSectGrid::SetStyleByType()
{
	BOOL bOldLock = LockUpdate(TRUE);
 
	int nIndex = 0;
	int nColWidth[2][ITEM_COLM_NUM+1] = {
		{15, 35, 67, 67, 85, 37,  0,  0,  0,  0, 35/*h*/, 35/*V/S*/, 35/*M*/, 42, 42, 45},
		{15, 35, 67, 67, 85, 37, 38, 38, 38, 38, 35/*h*/, 35/*V/S*/, 35/*M*/, 42, 42, 45},
	};

	if(m_nSubType == 2) nIndex = 1;
	for(int i=0; i<ITEM_COLM_NUM+1; i++) 
		SetColWidth(i, i, globalUtils.ScaleByDPI(nColWidth[nIndex][i]));

	if(m_nSubType != 2) 
	{
		HideCols(6, 9, TRUE);
		if (m_nSectType == 2)
		{
			HideCols(     MATL_COL,      MATL_COL, TRUE);
			HideCols(STIFFNESS_COL, STIFSCALE_COL, TRUE);
		}
		else
		{
			HideCols(     MATL_COL,      MATL_COL, FALSE);
			HideCols(STIFFNESS_COL, STIFSCALE_COL, FALSE);
		}
	}
	else
	{
		HideCols(6, 7, FALSE);
		if(!m_bTapered)
		{
			HideCols(8, 9, TRUE);
			SetValueRange(CGXRange(0, 6), _LS(IDS_CMD_CSCS__Cy));
			SetValueRange(CGXRange(0, 7), _LS(IDS_CMD_CSCS__Cz));
		}
		else
		{
			HideCols(8, 9, FALSE);
			SetValueRange(CGXRange(0, 6), _LS(IDS_CMD_CSCS__Cy_I));
			SetValueRange(CGXRange(0, 7), _LS(IDS_CMD_CSCS__Cz_I));
		}
	}
	//HideCols(STIFFNESS_COL, STIFFNESS_COL, (m_nSubType != 2));

	CString MaterialType = _T("");
	if(m_nSectType != 1){ MaterialType += MATL_TYPE_ELEMENT;  MaterialType += _T("\n"); }
	if(m_nSectType != 2){ MaterialType += MATL_TYPE_MATERIAL; MaterialType += _T("\n"); }
	SetStyleRange(CGXRange().SetCols(2), CGXStyle()
			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
			.SetChoiceList(MaterialType)
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT)
			.SetValue(_T(""))
			);
	LockUpdate(bOldLock);
	if(!bOldLock) Redraw();
}

void CCMStageCompSectGrid::SetCompoStageColumn(UINT nStagK)
{
	CString csChoiceList = COMPOSITE_ACTIVE_STAGE; 
	csChoiceList += _T("\n");
	
	CArray<T_STAG_K, T_STAG_K> aStagK;
	m_pDoc->m_pAttrCtrl->GetStagKeyList(aStagK);
	
	T_STAG_D StagD;
	int nCount = aStagK.GetSize();
	int i = 0;
	for (i = 0; i < nCount; i++)
		if (aStagK[i]== nStagK) break;

	if (i != nCount)
	{
		int nStart = i;
		for (i = nStart; i < nCount; i++) 
		{
			if (!m_pDoc->m_pAttrCtrl->GetStag(aStagK[i], StagD)) continue;
			csChoiceList += (StagD.StageName+_T("\n"));
		}
	}
	SetStyleRange(CGXRange().SetCols(4), CGXStyle()
			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
			.SetChoiceList(csChoiceList)
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT)
			.SetValue(_T(""))
			);
	ROWCOL ncRow, ncCol;
	GetCurrentCell(ncRow, ncCol);
	if (ncCol == 4) CGXGridWnd::SetCurrentCell(ncRow, ncCol);
}

// 단지 로우개수만 변경 
BOOL CCMStageCompSectGrid::SetRowNumberOnly(UINT nRowNumber)
{
	if(nRowNumber < 0) return FALSE;

	m_nCurRowNumber = nRowNumber;

	SetRowCount(m_nCurRowNumber); 
	if(m_nCurRowNumber > m_nPreRowNumber)
		SetInitRows(m_nPreRowNumber+1, m_nCurRowNumber);

	m_nPreRowNumber = m_nCurRowNumber;
	
	return TRUE;
}

// 로우 개수와 Compostie Type을 Setting
BOOL CCMStageCompSectGrid::SetRowNumber(UINT nRowNumber, int nSubType, BOOL nSectType/*=0*/, BOOL bTapered)
{
	if(nRowNumber < 0) return FALSE;

	SetInitData();

	m_nCurRowNumber = nRowNumber;
	m_nSubType = nSubType;
	m_nSectType = nSectType;
	m_bTapered = bTapered;

	SetStyleByType();

	SetRowCount(m_nCurRowNumber);

	SetInitRows(1, m_nCurRowNumber);

	return TRUE;
}

// Composite Type에 따라 nPart->nRow로 매치 
ROWCOL CCMStageCompSectGrid::GetRowNember(int nPart)
{
	if(m_nSubType == -1 || m_nSubType == 2)
	{
		return nPart;
	}
	else if(m_nSubType == 0) 
	{
		if(nPart == 1 || nPart == 2) return 1;
		else if(nPart == 3)          return 2;
		else ASSERT(0);
	}
	else if(m_nSubType == 1)
	{
		if(nPart == 1)                    return 1;
		else if(nPart == 2 || nPart == 3) return 2;
		else ASSERT(0);
	}
	else 
		ASSERT(0);

	return nPart;
}

// 주어진 셀의 값을 가져오는 함수
CString CCMStageCompSectGrid::GetEditingValue(ROWCOL nRow, ROWCOL nCol)
{
	CString csNum;
	BOOL bGet = FALSE;
	ROWCOL ncRow, ncCol;

	if (GetCurrentCell(ncRow, ncCol))
	{
		if (ncRow == nRow && ncCol == nCol)
		{
			CGXControl* pControl = GetControl(nRow, nCol);
			const CGXStyle style = LookupStyleRowCol(nRow, nCol);
			if (pControl)
			{
				if (pControl->IsActive())
					bGet = pControl->GetValue(csNum);
				else if (style.GetIncludeControl() && style.GetControl() == GX_IDS_CTRL_CHECKBOX3D)
				{
					bGet = pControl->GetValue(csNum);
					if (csNum == _T("0")) csNum = _T("1");
					else csNum = _T("0");
				}
			}
		}
	}

	if (!bGet) csNum = GetValueRowCol(nRow, nCol);
	return csNum;
}

// 그리드에 입력된 데이터를 한줄씩 가져오는 함수
BOOL CCMStageCompSectGrid::GetData(ROWCOL nRow, T_CSCS_BASE& rData)
{
	BOOL bReturn = TRUE;
	CString csValue = _T("");

	csValue = GetEditingValue(nRow, 2);
	if(csValue == MATL_TYPE_ELEMENT) rData.MatlKey = 0;
	else
	{
		csValue = GetEditingValue(nRow, 3);
		int nIndex = csValue.Find(_T(":"));
		csValue = csValue.Left(nIndex);
		rData.MatlKey = _ttoi(csValue);
		if(!m_pDoc->m_pAttrCtrl->ExistMatl(rData.MatlKey)) return FALSE;
	}

	// Composite Stage
	csValue = GetEditingValue(nRow, 4);
	if(csValue == COMPOSITE_ACTIVE_STAGE) rData.CompStagKey = 0;
	else rData.CompStagKey = m_pDoc->m_pAttrCtrl->GetStagKey(csValue);
	
	// Age
	csValue = GetEditingValue(nRow, AGE_COL);
	rData.dAge = _tstof(csValue);

	// Cy, Cz
	csValue = GetEditingValue(nRow, 6);
	rData.YBar = _tstof(csValue);
	csValue = GetEditingValue(nRow, 7);
	rData.ZBar = _tstof(csValue);
	csValue = GetEditingValue(nRow, 8);
	rData.YBarJ = _tstof(csValue);
	csValue = GetEditingValue(nRow, 9);
	rData.ZBarJ = _tstof(csValue);

	// h
	csValue = GetEditingValue(nRow, H_COL);
	if(csValue==_T("")) return FALSE;
	rData.dH = _tstof(csValue);

	// V/S
	csValue = GetEditingValue(nRow, VS_COL);
	if(csValue==_T("")) return FALSE;
	rData.dVS = _tstof(csValue);

	// M
	csValue = GetEditingValue(nRow, M_COL);
	if (csValue == _T("")) return FALSE;
	rData.dM = _tstof(csValue);

	// Scale 
	rData.StiffScale = m_aScale.GetAt(nRow-1);
	rData.Stiffness  = m_aStiff_I.GetAt(nRow-1);
	rData.StiffnessJ = m_aStiff_J.GetAt(nRow-1);

	return bReturn;
}

// 그리드에 입력된 모든 데이터값을 가져오는 함수 
BOOL CCMStageCompSectGrid::GetAllData(CArray<T_CSCS_BASE, T_CSCS_BASE&>& arData)
{
	T_CSCS_BASE data;

	if(m_nSubType == -1 || m_nSubType == 2)
	{
		arData.SetSize(m_nCurRowNumber);
		for(int nRow=1; nRow<=m_nCurRowNumber; nRow++)
		{
			if(!GetData(nRow, data)) return FALSE; 
			data.nPart = nRow; arData[nRow-1] = data;
		}
	}
	else if(m_nSubType == 0)
	{
		arData.SetSize(m_nCurRowNumber+1);
		if(!GetData(1, data)) return FALSE;
		data.nPart = 1; arData[0] = data;
		data.nPart = 2; arData[1] = data;
		
		if(!GetData(2, data)) return FALSE;
		data.nPart = 3; arData[2] = data;
	}
	else if(m_nSubType == 1)
	{
		arData.SetSize(m_nCurRowNumber+1);
		if(!GetData(1, data)) return FALSE;
		data.nPart = 1; arData[0] = data;
		
		if(!GetData(2, data)) return FALSE;
		data.nPart = 2; arData[1] = data;
		data.nPart = 3; arData[2] = data;
	}
	else 
		ASSERT(0);

	return TRUE;
}

// 헤더 Setting 
void CCMStageCompSectGrid::SetHeaderTitle()
{
	CString strHeader[] = {_LS(IDS_CMD_CSCS__Part), _LS(IDS_CMD_CSCS__Material_Type), _LS(IDS_CMD_CSCS__Material), 
												 _LS(IDS_CMD_CSCS__Composite_Stage), _LS(IDS_CMD_CSCS__Age),
												 _LS(IDS_CMD_CSCS__Cy_I), _LS(IDS_CMD_CSCS__Cz_I), _LS(IDS_CMD_CSCS__Cy_J), _LS(IDS_CMD_CSCS__Cz_J), 
												 _LS(IDS_CMD_CSCS__h), _LS(IDS_CMD_CSCS__V_S), _LS(IDS_CMD_CSCS__M),
												 _LS(IDS_CMD_CSCS__Stiffness), _LS(IDS_CMD_CSCS___Stiff_Scale), _LS(IDS_CMD_CSCS__Comp_Stiff)};
	 
	for(int nCol=1; nCol<ITEM_COLM_NUM+1; nCol++)
		SetValueRange(CGXRange(0, nCol), strHeader[nCol-1]);
}

// #define CWnd CGXGridWnd
// BEGIN_MESSAGE_MAP(CCMStageCompSectGrid, CWnd)
// #undef CWnd
// 	//{{AFX_MSG_MAP(CCMStageCompSectGrid)
// 	//}}AFX_MSG_MAP
// END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMStageCompSectGrid message handlers

//--------------------------------------------------------------------------
// Initialize   초기화 
//--------------------------------------------------------------------------
void CCMStageCompSectGrid::Initialize(CDBDoc* pDoc)
{
	m_pDoc = pDoc;
	//m_pParent = pParent;
	CGXGridWnd::Initialize();
	
	// cell 바깥영역 배경색 지정 
	GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND, CTBGrid::m_Color.OuterBackground);
	
	// Header 배경색 지정 
	ChangeColHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg)); 
	ChangeRowHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg)); 
	
	// 기타 Cells 배경색 지정
	ChangeStandardStyle(CGXStyle( ).SetInterior(CTBGrid::m_Color.NormalModeBg));

	GetParam()->EnableUndo(FALSE);

	// Row와  Column의 이동을 금지시킨다.
	GetParam()->EnableMoveRows(FALSE);
	GetParam()->EnableMoveCols(FALSE);

	// MemoryDC를 사용한다.
	SetDrawingTechnique(gxDrawUsingMemDC);
		
	// 헤더의 Current cell을 막는다.
	ColHeaderStyle().SetEnabled(FALSE);
	RowHeaderStyle().SetEnabled(FALSE);

	GetParam()->EnableTrackRowHeight(FALSE);  
	GetParam()->EnableMoveCols(FALSE);        // columns can not be dragged

	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetMarkColHeader(FALSE); // Turn off pressed button effect for column headers
	pProp->SetMarkRowHeader(FALSE);
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL, // 현재 셀의 border 설정 변경
												(CGXStyle) pProp->sInvertNoBorder);

	LockUpdate(TRUE);

	// 기억하라!! 항상 컬럼이나 로우 갯수는 헤더를 빼고 난 나머지다.
	SetColCount(ITEM_COLM_NUM);
	
#ifdef _RUS
	SetRowHeight(0, 0, globalUtils.ScaleByDPI(46));
#else
	SetRowHeight(0, 0, globalUtils.ScaleByDPI(36));
#endif
	
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
	GetParam()->SetNumberedRowHeaders(FALSE);

	// 컬럼 스타일 변경
	// Header
	SetStyleRange(CGXRange().SetCols(0), CGXStyle()
			.SetControl(GX_IDS_CTRL_HEADER)
			);

	SetStyleRange(CGXRange().SetRows(0), CGXStyle()
			.SetControl(GX_IDS_CTRL_HEADER)
			);

	// Part
	SetStyleRange(CGXRange().SetCols(1), CGXStyle()
			.SetHorizontalAlignment(DT_CENTER)
			.SetValueType(GX_VT_STRING)
			.SetEnabled(FALSE)
			);

	// Material
	CString Material = _T("");
	CArray<T_MATL_K, T_MATL_K> arMatlKeyList;
	m_pDoc->m_pAttrCtrl->GetMatlKeyList(arMatlKeyList);
	int nCount = arMatlKeyList.GetSize();
	for(int i=0; i<nCount; i++)
	{
		T_MATL_D data;
		m_pDoc->m_pAttrCtrl->GetMatl(arMatlKeyList[i], data);

		BOOL bChk = FALSE;
		if (m_nSectType == 2) // Dumbbell
		{
			if (data.Type == _ULS(SRC)) bChk = TRUE;
		}
		else
		{
			if (data.Type != _ULS(SRC)) bChk = TRUE;
		}
		if(bChk)
		{
			CString str;
			str.Format(_T("%d: "), arMatlKeyList[i]); str += data.Name;
			Material += (str+_T("\n"));
		}
	}
	SetStyleRange(CGXRange().SetCols(3), CGXStyle()
			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
			.SetChoiceList(Material)
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT)
			.SetValue(_T(""))
			);

	// Composite Stage
	SetCompoStageColumn(1);

	// Age
	SetStyleRange(CGXRange().SetCols(AGE_COL), CGXStyle()
			.SetControl(GX_IDS_CTRL_EDIT)
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC)
			.SetPlaces(2)
			);

	// Cy, Cz, h, V/S, M
	SetStyleRange(CGXRange().SetCols(6, M_COL), CGXStyle()
			.SetControl(GX_IDS_CTRL_EDIT)
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC)
			//.SetPlaces(4)
			);

	// Scale & Stiffness & Comp. Stiff
	SetStyleRange(CGXRange().SetCols(STIFFNESS_COL, STIF_COMP_COL), CGXStyle()
			.SetControl(GX_IDS_CTRL_PUSHBTN)
			.SetInterior(GXSYSCOLOR(COLOR_BTNFACE))
			.SetChoiceList(_T("..."))
			//.SetTextColor(COLORREF(RGB(0,0,0)))
			.SetEnabled(TRUE)
			);

	SetHeaderTitle();

	SetStyleByType();

	LockUpdate(FALSE);
}

//--------------------------------------------------------------------------
// GetStyleRowCol  셀 스타일지정(여기선 Color) 
//--------------------------------------------------------------------------
BOOL CCMStageCompSectGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
{
	BOOL bCurrentRow;
	BOOL bCurrentCell;
	BOOL bEnabled;
	BOOL bHeaderCol;

	ROWCOL ncRow, ncCol;    // 현재 Row, Col
	CGXGridWnd::GetStyleRowCol(nRow, nCol, style, mt, nType);

	// 현재 셀의 그리드 좌표(Row, Col 번호)를 얻어온다.
	if (!GetCurrentCell(ncRow, ncCol)) 
		ncRow = GetRowCount()+1; // 현재 셀이 없는 경우 : ncRow를 범위 밖으로 설정

	// 현재 셀이 이동되면 새로 그려준다.
	m_bRefreshOnSetCurrentCell = TRUE;

	// Row나 Col이 1보다 작으면 리턴한다.(Standard Row or Column Header)
	if (nRow < 1 || nCol < 1) return TRUE;

	// Get Current Table and Cell State
	bCurrentRow = (nRow == ncRow);
	bCurrentCell = (nRow == ncRow && nCol == ncCol);

	// Enable, ReadOnly와 같은 Cell의 상태를 조사한다.
	// ComposeStyleRowCol은 호출해서는 안된다.
	// 그래서 여기서는 직접 조사한다.
	// 단 Cell Style, Column Style만 조사한다.
	if (style.GetIncludeEnabled())  // 셀에 Enable정보 포함 여부 알아냄
	{
		bEnabled = style.GetEnabled();
	}
	else // Column style에서 enable 정보 알아냄
	{
		CGXStyle styleEx;
		GetColStyle(nCol, styleEx);
		bEnabled = (!styleEx.GetIncludeEnabled() || styleEx.GetEnabled());
	}

	bHeaderCol = IsFrozenCol(nCol);

	// Printing 모드이면 헤더 Column이 프린트 되기 위해 다음과 같이 리턴한다.
	if (IsPrinting())
	{
		if (bHeaderCol) style.SetControl(GX_IDS_CTRL_HEADER);
		return TRUE;
	}

	// 일반 셀이 가장 많으므로 빨리 검사하고 바로 리턴할 수 있게 코드를 앞으로 뺀다.
	if (bEnabled && !bCurrentCell && !bCurrentRow && !bHeaderCol)
	{
		style
			.SetInterior(CTBGrid::m_Color.NormalModeBg)
			.SetTextColor(CTBGrid::m_Color.NormalModeFg);

		return TRUE;
	}

	// Disable되었을 때
	if (!bEnabled)  
	{
		if (bHeaderCol) style           // Row or Column Header
			.SetControl(GX_IDS_CTRL_HEADER)
			.SetInterior(CTBGrid::m_Color.RowColHeaderBg)
			.SetTextColor(CTBGrid::m_Color.NormalModeFg);
		else if (bCurrentRow) style     // Disabled Current Row
			.SetInterior(CTBGrid::m_Color.DisabledBg)
			.SetTextColor(CTBGrid::m_Color.ReadOnlyCurrentRowFg);
		else style                      // Disabled Cell
			.SetInterior(CTBGrid::m_Color.DisabledBg)
			.SetTextColor(CTBGrid::m_Color.DisabledFg);

		return TRUE;
	}

	if (bCurrentCell) style
		.SetInterior(CTBGrid::m_Color.NormalCurrentCellBg)
		.SetTextColor(CTBGrid::m_Color.NormalCurrentCellFg);
	else if (bCurrentRow) style
		.SetInterior(CTBGrid::m_Color.NormalModeBg)
		.SetTextColor(CTBGrid::m_Color.NormalModeFg);
	else if (bHeaderCol) style    // Enabled Header Col(Only Frozen)
		.SetInterior(CTBGrid::m_Color.NormalModeBg)
		.SetTextColor(CTBGrid::m_Color.NormalModeFg);

	return TRUE;
}

//--------------------------------------------------------------------------
// OnModifyCell  셀 수정 - Enable / Disable 
//--------------------------------------------------------------------------
void CCMStageCompSectGrid::EnableDisableColumns(ROWCOL nRow, int nMatlType)
{
	BOOL bOldLock = LockUpdate(TRUE);

	ROWCOL nMatlCol = MATL_TYPE_COL + 1;
	BOOL bEnable = (nMatlType == 1) ? TRUE : FALSE;
	SetStyleRange(CGXRange().SetCells(nRow, nMatlCol), CGXStyle().SetEnabled(bEnable));
	if (!bEnable)
		SetValueRange(CGXRange().SetCells(nRow, nMatlCol), _T(""));
	
	if (m_nSectType == 2)
	{
		//SetStyleRange(CGXRange().SetCells(nRow, AGE_COL), CGXStyle().SetEnabled(FALSE));
		SetStyleRange(CGXRange().SetCells(nRow, H_COL  ), CGXStyle().SetEnabled(FALSE));
		SetStyleRange(CGXRange().SetCells(nRow, VS_COL ), CGXStyle().SetEnabled(FALSE));
		SetStyleRange(CGXRange().SetCells(nRow, M_COL  ), CGXStyle().SetEnabled(FALSE));
	}

	// Button이라 Disable 해봤자 소용이 없다.
	// 아예 버튼을 빼고 Static으로 Disable
	/*
	if (bEnable)
		SetStyleRange(CGXRange().SetCells(nRow, STIF_COMP_COL), CGXStyle()
			.SetControl(GX_IDS_CTRL_PUSHBTN));
	else
		SetStyleRange(CGXRange().SetCells(nRow, STIF_COMP_COL), CGXStyle()
			.SetControl(GX_IDS_CTRL_STATIC));
	SetStyleRange(CGXRange().SetCells(nRow, STIF_COMP_COL), CGXStyle().SetEnabled(bEnable));
	*/

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

void CCMStageCompSectGrid::ChangeColumnStatus(ROWCOL nRow, ROWCOL nCol)
{  
	if (nCol != MATL_TYPE_COL) return;

	CString csType;
	csType = GetEditingValue(nRow, nCol);

	int nMatlType;
	if(csType == MATL_TYPE_MATERIAL)     nMatlType = 1;
	else if(csType == MATL_TYPE_ELEMENT) nMatlType = 0;
	else ASSERT(0);

	EnableDisableColumns(nRow, nMatlType);
}

// 재질이 바뀌면 H 값 재계산, Japan Standard 인 경우는 x 0.5
void CCMStageCompSectGrid::SetAutoCalcH(ROWCOL nRow, ROWCOL nCol)
{  
	if ( !(nCol==MATL_TYPE_COL+1 || nCol==MATL_TYPE_COL)) return;

	CString csType;
	csType = GetEditingValue(nRow, MATL_TYPE_COL);

	int nMatlType;
	if(csType == MATL_TYPE_MATERIAL)     nMatlType = 1;
	else if(csType == MATL_TYPE_ELEMENT) nMatlType = 0;
	else ASSERT(0);

	/*
	BOOL bTmatExist = FALSE;
	BOOL bTdmtExist = FALSE;
	BOOL bJapanCode = FALSE;
	CString csValue;
	if(csType == MATL_TYPE_MATERIAL)
	{
		T_MATL_K MatlKey;
		T_MATL_D MatlData;
		T_TMAT_D TmatData;
		T_TDMT_D TdmtData;
		csValue = GetEditingValue(nRow, 3);
		int nIndex = csValue.Find(_T(":"));
		csValue = csValue.Left(nIndex);
		MatlKey = _ttoi(csValue);
		if(!m_pDoc->m_pAttrCtrl->GetMatl(MatlKey, MatlData)) return;
		
		bTmatExist = m_pDoc->m_pAttrCtrl->GetTmat(MatlKey, TmatData);
		if(bTmatExist) bTdmtExist = m_pDoc->m_pAttrCtrl->GetTdmt(TmatData.TdMatlTypeKey, TdmtData);
		if(bTdmtExist) bJapanCode = (TdmtData.COMMON.nCode == 8); // Japan Standard인 경우
	}
	*/
	
	int nSize = GetRowCount();
	CArray<double, double> aResult;
	if(!m_pParent->GetAutoCalcH(nSize, aResult)) // MNET:2840 20070612 mylee - H 값 자동계산, 한번에 두 Row 받아옴
	{
		aResult.RemoveAll();
		aResult.SetSize(nSize);
	}
	double dH = 0.0;
	if(nRow-1 < aResult.GetSize()) dH = aResult[nRow-1];
	
	//if(bTmatExist && bTdmtExist && bJapanCode) dH /= 2.0;
	// -> CCMStageCompSectItemDlg::Get_H_FromCscs 안에서 해주는 것으로 변경
	// MNET:1603 20080221 mylee
	
	SetValueRange(CGXRange(nRow, H_COL), double(dH)); // 초기는 무조건 Elem 이므로 재질 고려 필요 없음
}

void CCMStageCompSectGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	// 부모의 OnModifyCell을 항상 먼저 호출해 주어야 한다.
	CGXGridWnd::OnModifyCell(nRow, nCol);
	ChangeColumnStatus(nRow, nCol);
	SetAutoCalcH(nRow, nCol);
}

BOOL CCMStageCompSectGrid::ReCalcStiff(int nPart)
{
	if(m_nSubType != 2)
	{
		if(!m_pParent->CalAndGetStiff(nPart, m_aStiff_I[nPart], m_aStiff_J[nPart], m_aStiffAuto_I[nPart], m_aStiffAuto_J[nPart]))
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Can_t_calculate_stiffness));
			if (m_nSectType == 2) return FALSE;
			else   			      return TRUE;
		}
	}

	return TRUE;
}

// Scale & Stiffness 버튼을 클릭했을 때 
void CCMStageCompSectGrid::OnClickedButtonRowCol(ROWCOL nRow, ROWCOL nCol)
{
	BOOL bMoreBtn = TRUE;
	if (nCol != STIFSCALE_COL && 
			nCol != STIFFNESS_COL &&
			nCol != STIF_COMP_COL) bMoreBtn = FALSE;

	if (!bMoreBtn)
	{
		CGXGridWnd::OnClickedButtonRowCol(nRow, nCol);
		return;
	}
		
	if(nCol == STIFSCALE_COL) // 1) Stiffness Scale
	{
		CCMStageCompSectStiffDlg dlg(m_pDoc, 1, &m_aScale[nRow-1]);
		dlg.DoModal();
	}
	else if(nCol == STIFFNESS_COL)  // 2) Stiffness
	{
		BOOL bReadOnly = FALSE;

		BOOL bCompNormalPar2 = FALSE;
		if(m_nSubType != 2)
		{
			bReadOnly = TRUE;
			if(!m_pParent->CalAndGetStiff(nRow-1, m_aStiff_I[nRow-1], m_aStiff_J[nRow-1], m_aStiffAuto_I[nRow-1], m_aStiffAuto_J[nRow-1]))
			{
				AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Can_t_calculate_stiffness));
				return;
			}

			if(nRow==2)
			{
				bCompNormalPar2 = m_pParent->IsCompNormalPar2();
			}
		}

		CString strTitle = _LS(IDS_CMD_CSCS_Stiffness_Title1);
		if(m_bTapered)  
		{
			CCMStageCompSectStiffDlg dlg(m_pDoc, 3, &m_aStiff_I[nRow-1], &m_aStiff_J[nRow-1], bReadOnly);
			dlg.SetTitle(strTitle);
			dlg.SetCompNormalPar2(bCompNormalPar2);
			dlg.SetScaleAutoData( &m_aStiffAuto_I[nRow-1], &m_aStiffAuto_J[nRow-1]);
			dlg.DoModal();
		}
		else 
		{
			CCMStageCompSectStiffDlg dlg(m_pDoc, 2, &m_aStiff_I[nRow-1], NULL, bReadOnly);
			dlg.SetTitle(strTitle);
			dlg.SetCompNormalPar2(bCompNormalPar2);
			dlg.SetScaleAutoData( &m_aStiffAuto_I[nRow-1], &m_aStiffAuto_I[nRow-1]);
			dlg.DoModal();
		}
	}
	else if (nCol == STIF_COMP_COL) // 3) Composite Stiffness
	{    
		BOOL bReadOnly = TRUE;
		CArray<T_SECT_STIFFNESS, T_SECT_STIFFNESS> aCompStiff_I, aCompStiff_J;
		CArray<T_SECT_STIFFNESS, T_SECT_STIFFNESS&> *paMultiStiff = new CArray<T_SECT_STIFFNESS, T_SECT_STIFFNESS&>[2];
		paMultiStiff[0].RemoveAll(); // Long Term
		paMultiStiff[1].RemoveAll(); // Shrinkage
		int nTmp = paMultiStiff[0].GetSize();
		aCompStiff_I.SetSize(m_aStiff_I.GetSize());
		aCompStiff_J.SetSize(m_aStiff_J.GetSize());
		
		if(m_nSubType != 2  &&  m_nSectType != 2)
		{
			// MQC 16082 hslee : aCompStiff_I에 강성이 재계산되어야 하는데 CalAndGetCompStiff함수는 재계산된 강성(SectDB::CalcSectCscs()의 1번째 Param)을 안씀..
			//                   CalAndGetStiff 에서는 재계산된 강성을 세팅하기 때문에 그냥 통으로 부르는게 안전할 것 같아서 이렇게 처리 함..
			ReCalcStiff(nRow-1);
		}

		BOOL bHasMulti = FALSE;
		UINT ErrorMsg = 0;
		if(!m_pParent->CalAndGetCompStiff(nRow-1, aCompStiff_I[nRow-1], aCompStiff_J[nRow-1], paMultiStiff, bHasMulti, ErrorMsg))
		{
			if (ErrorMsg > 0)
			{
				CString strError;
				if      (ErrorMsg == IDC_CMD_CSCS__Err___No_element_assigned) strError = _LS(IDS_CMD_CSCS__Err___No_element_assigned);
				else if (ErrorMsg == IDC_CMD_CSCS__Err___Material_not_exist)  strError = _LS(IDS_CMD_CSCS__Err___Material_not_exist);
				else if (ErrorMsg == IDC_CMD_CSCS__Err___Material_not_same)   strError = _LS(IDS_CMD_CSCS__Err___Material_not_same);
				AfxMessageBox(strError);
			}
		
		    delete []paMultiStiff;
			return;
		}

		CString strTitle = _LS(IDS_CMD_CSCS_Stiffness_Title2);
		if(m_bTapered)  
		{
			CCMStageCompSectStiffDlg dlg(m_pDoc, 3, &aCompStiff_I[nRow-1], &aCompStiff_J[nRow-1], bReadOnly);
			dlg.SetTitle(strTitle);
			dlg.HideUnderQzb();
			dlg.DoModal();
		}
		else  
		{
			// 기능 다시 임시로 막음 070208
			// if(m_nCompositType==-2 && nRow==2 && bHasMulti ) // normal type의 part2
			if (FALSE) // blocked
			{
				CCMStageCompSectStiffDlg dlg(m_pDoc, 2, &aCompStiff_I[nRow-1], NULL, bReadOnly);
				dlg.SetMultiData(4, &paMultiStiff[0][nRow-1], &paMultiStiff[1][nRow-1]); // nType = 4
				dlg.SetTitle(strTitle);
				dlg.HideUnderQzb();
				dlg.DoModal();
			}
			else
			{
				CCMStageCompSectStiffDlg dlg(m_pDoc, 2, &aCompStiff_I[nRow-1], NULL, bReadOnly);
				dlg.SetTitle(strTitle);
				dlg.HideUnderQzb();
				if (m_nSectType == 2)
				{
					dlg.GridForDumbAndStub();
				}
				dlg.DoModal();
			}
		}
		delete []paMultiStiff;
	}
	else 
		ASSERT(0);
}

/////////////////////////////////////////////////////////////////////////
// Delete Key 기능 추가를 위해서 
/////////////////////////////////////////////////////////////////////////

/*
BOOL CCMStageCompSectGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	CRowColArray awRows;
	ROWCOL nRow=1, nCol=1;
	int nSize;

	BOOL bCtl = GetKeyState(VK_CONTROL) & 0x8000;
	BOOL bShift = GetKeyState(VK_SHIFT) & 0x8000;

	if (bCtl || bShift) 
		return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
	if (nMessage != WM_KEYDOWN || nChar != VK_DELETE)
		return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);

	GetSelectedRows(awRows, TRUE, FALSE);
	nSize = awRows.GetSize();
		
	// Title Row도 뺀다.
	if (nSize > 0 && awRows.GetAt(0) == 0) { nSize--; awRows.RemoveAt(0); }
	if (nSize == 0) return TRUE;
	if (!OnDeleteRecords(awRows))
		AfxMessageBox(_T("Error : Can't delete data."));

	return TRUE;
}

BOOL CCMStageCompSectGrid::OnDeleteRecords(CRowColArray &awRows)
{
	// 레코드(들)을 DB에서 실제로 지운다. 성공하면 TRUE, 실패하면 ERROR 리턴
	if (!DeleteRecord(awRows))
	{
		SetWarningText(_T("Error : Can't delete data."));
		return FALSE;
	}
	return TRUE;
}

BOOL CCMStageCompSectGrid::DeleteRecord(CRowColArray& awRows)
{
	ROWCOL nRow, nCol;

	// Redraw는 실제 지워진 row가 있을 때만 한다.
	ROWCOL nOldCount = GetRowCount();

	//레코드를 삭제한다. 선택된 레코드들은 항상 정렬되어 있다(오름차순)
	BOOL bOldLock = LockUpdate(TRUE);
	
	// 편집중인 레코드가 제거 대상에 들어 있는지 조사
	ROWCOL row;
	for (int i = awRows.GetSize()-1; i >= 0; i--)
	{
		row = awRows.GetAt(i);
		GetCurrentCell(nRow, nCol);
		RemoveRows(row, row);
	}
	LockUpdate(bOldLock);

	if (GetRowCount() != nOldCount) Redraw();
	
	return TRUE;
}
*/