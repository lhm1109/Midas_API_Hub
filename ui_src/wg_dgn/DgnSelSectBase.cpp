// DgnDlgSelectBase.cpp: implementation of the CDgnDlgSelectBase class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSelSectBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"

#include "DgnDataCtrl.h"


#include "..\wg_common\wg_common_TBGrid.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgnSelSectBase::CDgnSelSectBase()
{
	m_iMode = 0;
	m_arKey.RemoveAll();
	if(m_arElemData.GetCount()>0) m_arElemData.RemoveAll();

	m_pDoc = CDBDoc::GetDocPoint();

}

CDgnSelSectBase::~CDgnSelSectBase()
{

}

void CDgnSelSectBase::InitSectGrid(CMap<int,int,int,int>& arElemData, int iMode/*=0*/)
{

	m_iMode = iMode;
	// Data Sort & Store
	if(m_arElemData.GetCount()>0) m_arElemData.RemoveAll();

	int Index = 0;
	POSITION Pos = arElemData.GetStartPosition();
	while(Pos)
	{
		int iSectKey=0;
		arElemData.GetNextAssoc(Pos, Index, iSectKey);
		m_arElemData.SetAt(Index, iSectKey);
	}

	m_Grid.Initialize();
	
	// cell 바깥영역 배경색 지정 
	m_Grid.GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND, CTBGrid::m_Color.OuterBackground);
	
	// Header 배경색 지정 
	m_Grid.ChangeColHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg)); 
	m_Grid.ChangeRowHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg)); 
	
	// 기타 Cells 배경색 지정
	m_Grid.ChangeStandardStyle(CGXStyle( ).SetInterior(CTBGrid::m_Color.NormalModeBg));

	m_Grid.GetParam()->EnableUndo(FALSE);

	int iRowNum = m_arElemData.GetCount();
	int iColNum = 3;
	
	m_Grid.SetFrozenCols(0,0);
	m_Grid.SetRowCount(iRowNum);
	m_Grid.SetColCount(iColNum);

	m_Grid.SetRowHeight(0,0, globalUtils.ScaleByDPI(20),NULL,GX_UPDATENOW);
	m_Grid.SetRowHeight(1,iRowNum, globalUtils.ScaleByDPI(18),NULL,GX_UPDATENOW);

	WriteSectGridTitle();

	m_Grid.GetParam()->EnableSelection(FALSE);
	m_Grid.GetParam()->EnableTrackRowHeight(FALSE);
	m_Grid.GetParam()->EnableMoveRows(FALSE);
	m_Grid.GetParam()->EnableTrackColWidth(FALSE);
	m_Grid.GetParam()->EnableMoveCols(FALSE);

	m_Grid.GetParam()->EnableUndo(TRUE);
	if(iRowNum > 4)
		m_Grid.SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);
}

void CDgnSelSectBase::WriteSectGridTitle()
{
	CGXStyle style;

	style.SetHorizontalAlignment(DT_CENTER);
	style.SetVerticalAlignment(DT_VCENTER);
	style.SetFont(CGXFont_GC().SetBold(FALSE).SetSize(9));
	style.SetInterior(GetSysColor(COLOR_3DFACE));

	m_Grid.SetStyleRange(CGXRange().SetTable(), CGXStyle().SetReadOnly(FALSE));
	//////////////////////////////////////////////////////////////
	//  가로 Title를 찍어줍니다.
	//add by cylee, 05.08.17 to display properly when the ID placing No > 99999
	int Index=0;
	int iSectKey=0;
	BOOL IsKeyNo6 = FALSE;

	POSITION Pos = m_arElemData.GetStartPosition();
	while(Pos)
	{
		iSectKey=0;
		m_arElemData.GetNextAssoc(Pos, Index, iSectKey);
		if(iSectKey>99999) {IsKeyNo6 = TRUE; break;}
	}
	//

	CString title  = _LS(IDS_DGN_LIST_ID);
	style.SetValue(title);
	m_Grid.SetStyleRange(CGXRange(0,0),style);
	m_Grid.SetColWidth(0,0,globalUtils.ScaleByDPI(33), nullptr,GX_UPDATENOW);

	title = _LS(IDS_DGN_LIST_SEL);
	style.SetValue(title);
	m_Grid.SetStyleRange(CGXRange(0,1),style);
	m_Grid.SetColWidth(1,1,globalUtils.ScaleByDPI(35), nullptr,GX_UPDATENOW);
	
	title = _LS(IDS_DGN_LIST_NAME);
	style.SetValue(title);
	m_Grid.SetStyleRange(CGXRange(0,2),style);
	m_Grid.SetColWidth(2,2,globalUtils.ScaleByDPI(75), nullptr,GX_UPDATENOW);

	title = _LS(IDS_DGN_LIST_BAR);
	style.SetValue(title);
	m_Grid.SetStyleRange(CGXRange(0,3),style);
	m_Grid.SetColWidth(3,3,globalUtils.ScaleByDPI(35), nullptr,GX_UPDATENOW);

	m_Grid.SetStyleRange(CGXRange().SetTable(), CGXStyle().SetReadOnly(TRUE));
}

void CDgnSelSectBase::ResetSectGridData(BOOL bKeepChk/*FALSE*/)
{
	CString strRebar;
	CGXStyle style;
	style.SetHorizontalAlignment(DT_CENTER);
	style.SetVerticalAlignment(DT_VCENTER);
	style.SetFont(CGXFont_GC().SetBold(FALSE).SetSize(9));
 
	m_Grid.GetParam()->EnableUndo(FALSE);
	BOOL bOldLock = m_Grid.LockUpdate(TRUE);
	m_Grid.GetParam()->SetLockReadOnly(FALSE);

	int Index=0;
	int iSectKey=0;
	int iRow = 1;
	T_SECT_D SectData; 
	POSITION Pos = m_arElemData.GetStartPosition();
	while(Pos)
	{
		iSectKey=0;
		SectData.Initialize();
		m_arElemData.GetNextAssoc(Pos, Index, iSectKey);
		m_pDoc->m_pAttrCtrl->GetSectDesign(iSectKey, SectData);
		style.SetInterior(cDGN_UnSelColor1);
		//SEL
		style.SetControl(GX_IDS_CTRL_CHECKBOX3D);
		if(bKeepChk)
		{
			int iIndex1 = 0;
			UINT iPreSectKey = 0;
			POSITION Pos1 = m_arKey.GetStartPosition();
			while(Pos1)
			{
				iPreSectKey = 0;
				m_arKey.GetNextAssoc(Pos1, iIndex1, iPreSectKey);
				if(iSectKey == iPreSectKey)
				{
					style.SetValue(_T("1"));
					break;
				}
				else style.SetValue(_T("0"));
			}
		}
		else style.SetValue(_T("0"));

		m_Grid.SetStyleRange(CGXRange(iRow, 1), style);
		//ID
		CString strNo = _T("");
		strNo.Format(_T("%d"),iSectKey);
	  style.SetValue(strNo);
		style.SetControl(GX_IDS_CTRL_STATIC);
	  m_Grid.SetStyleRange(CGXRange(iRow, 0), style);
		//Name
		CString strName = _T("");
		strName.Format(_T("%s"),SectData.SName);
	  style.SetValue(strName);
	  m_Grid.SetStyleRange(CGXRange(iRow, 2), style);
		//Bar
		CString strData = _T("");
		if(m_iMode == 0) //Chk
	    style.SetValue((m_pDoc->m_pAttrCtrl->ExistRchk(iSectKey) ? _T("In") : _T("-")));
		else if(m_iMode == 1) //Dgn
			style.SetValue((m_pDoc->m_pAttrCtrl->ExistRdgn(iSectKey) ? _T("In") : _T("-")));
		else if(m_iMode == 2) //SRC Column
			style.SetValue((m_pDoc->m_pAttrCtrl->ExistRebs(iSectKey) ? _T("In") : _T("-")));
		 else if(m_iMode == 3) //Beam
			style.SetValue((m_pDoc->m_pAttrCtrl->ExistRebb(iSectKey) ? _T("In") : _T("-")));
		else if(m_iMode == 4) //Column
			style.SetValue((m_pDoc->m_pAttrCtrl->ExistRebc(iSectKey) ? _T("In") : _T("-")));
		else if(m_iMode == 5) //Brace
			style.SetValue((m_pDoc->m_pAttrCtrl->ExistRebr(iSectKey) ? _T("In") : _T("-")));
		else if(m_iMode == 6) //SRC Beam
			style.SetValue((m_pDoc->m_pAttrCtrl->ExistRbsb(iSectKey) ? _T("In") : _T("-")));
		else ASSERT(0);
	  m_Grid.SetStyleRange(CGXRange(iRow, 3), style);

		iRow++;
	}

	m_Grid.LockUpdate(bOldLock);
	if (!bOldLock) m_Grid.Redraw();

	m_Grid.GetParam()->SetLockReadOnly(TRUE);
	m_Grid.GetParam()->EnableUndo(TRUE);

	CGXSortInfoArray array;
	array.SetSize(1); // 1 key only (you can also have more keys)
	// 1st sorting data.
	array[0].nRC			 = 0;								// row nRow is the key
	array[0].sortType  = CGXSortInfo::numeric;		// will determine if the key is a date, numeric or alphanumeric value
	array[0].sortOrder = CGXSortInfo::ascending;	// sort ascending/descending
	m_Grid.SortRows(CGXRange().SetTable(), array);

	UpdateRchkKeyArray();
}

void CDgnSelSectBase::UpdateRchkKeyArray()
{
	int iSectKey = 0;
	T_SECT_D SectData; 
	m_arKey.RemoveAll();
	
	int iRowIdx = 0;
	int iRowNum = m_Grid.GetRowCount();
	for(int iRow=1; iRow <= iRowNum; iRow++)
	{
		SectData.Initialize();
		int iChk = _ttoi(m_Grid.GetValueRowCol(iRow,1));
		if(iChk==1)
		{
			iSectKey = _ttoi(m_Grid.GetValueRowCol(iRow,0));
			if(!m_pDoc->m_pAttrCtrl->GetSectDesign(iSectKey, SectData)) 
				continue;
			m_arKey.SetAt(iRowIdx, iSectKey);
			iRowIdx++;
		}
	}

}

void CDgnSelSectBase::CheckDiffSections(T_SECT_D OrginSectData, T_SECT_D SectData, int& iResult)
{
	//iResult = 2: Different Section Shape
	//iResult = 4: Different Section Size
	if(SectData.SectBefore.Shape != OrginSectData.SectBefore.Shape)
		iResult |= 2;
/*
	else 
	{
		if(SectData.SectBefore.Shape == _T("SB"))
		{
			if((SectData.SectBefore.SectI.Size[0] != OrginSectData.SectBefore.SectI.Size[0]) ||
				 (SectData.SectBefore.SectI.Size[1] != OrginSectData.SectBefore.SectI.Size[1]))
				 iResult |= 4;
		}
		else if(SectData.SectBefore.Shape == _T("T"))
		{
			if((SectData.SectBefore.SectI.Size[0] != OrginSectData.SectBefore.SectI.Size[0]) ||
				 (SectData.SectBefore.SectI.Size[1] != OrginSectData.SectBefore.SectI.Size[1]) ||
				 (SectData.SectBefore.SectI.Size[2] != OrginSectData.SectBefore.SectI.Size[2]) ||
				 (SectData.SectBefore.SectI.Size[3] != OrginSectData.SectBefore.SectI.Size[3]))
				 iResult |= 4;
		}
		else ASSERT(0);
	}
*/
}

BOOL CDgnSelSectBase::IsExistEnabledSect()
{
	BOOL bSelectSect = FALSE;
	for(int iRow=1; iRow <= m_Grid.GetRowCount(); iRow++)
	{
		int iSectKey = _ttoi(m_Grid.GetValueRowCol(iRow,0));
		int iChk = _ttoi(m_Grid.GetValueRowCol(iRow,1));
		if(iChk!=0)
			bSelectSect = TRUE;
	}

	return bSelectSect;
}

BOOL CDgnSelSectBase::GetCurKeyData(T_RCHK_K& Key,T_SECT_D& SectData)
{
	SectData.Initialize();

	ROWCOL iCurRow=0, iCurCol=0;
	if(m_Grid.GetCurrentCell(iCurRow, iCurCol))
	{
		if(iCurRow!=0)
		{
			int iSectKey = _ttoi(m_Grid.GetValueRowCol(iCurRow,0));
			if(m_pDoc->m_pAttrCtrl->GetSectDesign(iSectKey, SectData)) 
			{
				Key = iSectKey;
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL CDgnSelSectBase::SelectSection()
{
	BOOL bCanbeChked = TRUE;

	CGXStyle style;
	style.SetHorizontalAlignment(DT_CENTER);
	style.SetVerticalAlignment(DT_VCENTER);
	style.SetFont(CGXFont_GC().SetBold(FALSE).SetSize(9));
	style.SetInterior(cDGN_UnSelColor1);
	int iCheckResult = 0;
	T_SECT_D OrginSectData; 
	OrginSectData.Initialize();
	T_SECT_D SectData; 

	int Index = 0;

	for(int iRow=1; iRow <= m_Grid.GetRowCount(); iRow++)
	{
		iCheckResult = 0;
		SectData.Initialize();
		int iSectKey = _ttoi(m_Grid.GetValueRowCol(iRow,0));
		if(!m_pDoc->m_pAttrCtrl->GetSectDesign(iSectKey, SectData)) 
			continue;
		int iChk = _ttoi(m_Grid.GetValueRowCol(iRow,1));
		if(iChk!=0)
		{    
			if(Index == 0)
				OrginSectData = SectData;
			CheckDiffSections(OrginSectData, SectData, iCheckResult);
			if(iCheckResult > 0)
			{
				ROWCOL nRow=0, nCol=0;
				m_Grid.GetCurrentCell(nRow, nCol);
	      m_Grid.GetParam()->EnableUndo(FALSE);
				m_Grid.SetValueRange(CGXRange(nRow,1),_T("0"),gxOverride,0,GX_UPDATENOW);
				m_Grid.SetStyleRange(CGXRange(nRow,0,nRow,m_Grid.GetColCount()), style);
	      m_Grid.GetParam()->EnableUndo(TRUE);

				bCanbeChked = FALSE;
				return bCanbeChked;
			}
			Index++;
		}
	}

	if(bCanbeChked)
		UpdateRchkKeyArray();

	return bCanbeChked;
}

BOOL CDgnSelSectBase::SelectAll()
{
	if(!IsValidSection())
		return FALSE;

	CGXStyle style;
	style.SetHorizontalAlignment(DT_CENTER);
	style.SetVerticalAlignment(DT_VCENTER);
	style.SetFont(CGXFont_GC().SetBold(FALSE).SetSize(9));
	style.SetInterior(cDGN_SelColor1); 

	T_SECT_D SectData; 
	SectData.Initialize();

	m_Grid.GetParam()->EnableUndo(FALSE);
	BOOL bOldLock = m_Grid.LockUpdate(TRUE);
	m_Grid.GetParam()->SetLockReadOnly(FALSE);

	for(int iRow=1; iRow <= m_Grid.GetRowCount(); iRow++)
	{
		int iSectKey = _ttoi(m_Grid.GetValueRowCol(iRow,0));
		if(!m_pDoc->m_pAttrCtrl->GetSectDesign(iSectKey, SectData)) 
			continue;
		m_Grid.SetValueRange(CGXRange(iRow,1),_T("1"),gxOverride,0,GX_UPDATENOW);
		m_Grid.SetStyleRange(CGXRange(iRow,0,iRow,m_Grid.GetColCount()), style);
	}

	m_Grid.LockUpdate(bOldLock);
	if (!bOldLock) m_Grid.Redraw();
	m_Grid.GetParam()->SetLockReadOnly(TRUE);
	m_Grid.GetParam()->EnableUndo(TRUE);

	UpdateRchkKeyArray();

	return TRUE;
}

BOOL CDgnSelSectBase::IsValidSection()
{
	BOOL bIsValid = TRUE;
	int iCheckResult = 0;
	T_SECT_D OrginSectData; 
	OrginSectData.Initialize();
	T_SECT_D SectData; 

	for(int iRow=1; iRow <= m_Grid.GetRowCount(); iRow++)
	{
		iCheckResult = 0;
		SectData.Initialize();
		int iSectKey = _ttoi(m_Grid.GetValueRowCol(iRow,0));
		if(!m_pDoc->m_pAttrCtrl->GetSectDesign(iSectKey, SectData)) 
			continue;
		if(iRow == 1)
			OrginSectData = SectData;
		CheckDiffSections(OrginSectData, SectData, iCheckResult);
		if(iCheckResult > 0)
		{
			bIsValid = FALSE;
			return bIsValid;
		}
	}

	return bIsValid;
}