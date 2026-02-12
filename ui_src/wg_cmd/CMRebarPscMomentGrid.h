#if !defined(__CMREBARPSCMOMENTGRID_H__)
#define __CMREBARPSCMOMENTGRID_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMLoadCombGrid.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_common\wg_common_TBBrowserWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CCMRebarPscMomentGrid window
// Browser Style의 윈도우, 가로, 세로 방향으로 늘어난다.
// Row, Column 이동이 금지.
class CCMRebarPscMoment;
class CCMRebarPscMomentGrid : public CTBBrowserWnd
{
// Construction
public:
	CCMRebarPscMomentGrid(CCMRebarPscMoment* pParent);

//----------------------------------------------------------
// Overriadable
	virtual BOOL OnValidateField(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnValidateRecord(ROWCOL nRow);
	virtual BOOL OnAddNewRecord(ROWCOL nRow);
	virtual BOOL OnModifyRecord(ROWCOL nRow);
	virtual BOOL OnDeleteRecord(ROWCOL nRow);
	virtual BOOL OnDeleteRecords(CRowColArray &awRows);

	virtual BOOL ValidateField(CString& value, int nColID);
	virtual BOOL ValidateRecord(CStringArray &aValue, CRowColArray &aCols);

	virtual BOOL OnInsertEmptyRecord(ROWCOL nRow);


//----------------------------------------------------------
// Grid's Standard Overriadable Functions
	virtual void OnChangedSelection(const CGXRange *changedRect, BOOL bIsDraggin, BOOL bKey);
	//virtual BOOL OnSelDragRowsDrop(ROWCOL nFirstRow, ROWCOL nLastRow, ROWCOL nDestRow);
	virtual BOOL OnPasteFromClipboard(const CGXRange &range);
	BOOL PasteFromClipboard(const CGXRange &range, CString* aDefVal, int nKeyCol, 
													CGXGridWnd &x, int &nPasteMode, int &nPastedCols);
	virtual BOOL GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt = gxCopy, int nType = 0);

// CTBVisibleCtrl::HideColsByIntend()지원 함수
	BOOL HideColsByIntend(int nFromCol, int nToCol, BOOL bHide);
	virtual BOOL IsHiddenByIntend(int nCol);
	virtual int HitTest(CPoint& pt, ROWCOL* Row = NULL, ROWCOL* Col = NULL, CRect* rectHit = NULL);
	virtual BOOL HideCols(ROWCOL nFromCol, ROWCOL nToCol, BOOL bHide = TRUE, 
												BOOL* abHideArray = NULL, UINT flags = GX_UPDATENOW,
												GXCmdType ctCmd = gxDo);
	virtual BOOL OnTrackColWidth(ROWCOL nCol);
	virtual BOOL Copy();


//----------------------------------------------------------
// Interface
public:
	void Initialize(CArray<T_RPSC_MBAR, T_RPSC_MBAR&> *pData, BOOL bSym=FALSE);
	void MakeItemEx();
	void ChangeSymmetry(BOOL bSym);
	void UpdateBuffer(int nCmd, CArray<int, int> &aOldIndex, CArray<int, int> &aIndex, CArray<T_RPSC_MBAR, T_RPSC_MBAR&> &aData);

	BOOL  GetColumnNoFor(int nID, ROWCOL &nCol) { nCol = nID+1; return TRUE; }
	BOOL  GetColumnIDFor(ROWCOL nCol, int &nID) { nID = nCol-1; return TRUE; }

//----------------------------------------------------------
// Implementation
public:
	void SetColInfo();

protected:
	void SetValue(ROWCOL nRow, int index, T_RPSC_MBAR &data);
	void SetHeaderTitle();
	void ConvDataToStrArray(T_RPSC_MBAR &data, CStringArray &aValue);

	BOOL InsertRecordByKey(int index, T_RPSC_MBAR &data);
	BOOL DeleteRecordByKey(int index, T_RPSC_MBAR &data);

	void  SelectGridRow(long* aRow, long nSize);

	BOOL GetValue(ROWCOL nRow, T_RPSC_MBAR &data);
	BOOL MakeDataArray(CStringArray& value, CRowColArray& awCols, CArray<T_RPSC_MBAR, T_RPSC_MBAR&>& aData);
	BOOL ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, T_RPSC_MBAR &data);
	BOOL ConvStrArrayToData(CStringArray &aValue, T_RPSC_MBAR& data);

	BOOL AddToDB(ROWCOL nRow);
	BOOL AddData(int nIndex, T_RPSC_MBAR &data);
	BOOL ModifyToDB(ROWCOL nRow);
	BOOL ModifyData(int nOldIndex, int nIndex, T_RPSC_MBAR &data);
	BOOL DeleteFromDB(CRowColArray &awRow);

	CString GetDefValue(int ix);

	CString GetArrangeStr(int data);
	int GetArrangeData(CString &value);
	CString GetRefPosStr(int data);
	int GetRefPosData(CString &value);

protected:
	BOOL m_bSymmetry;
	CArray<T_RPSC_MBAR, T_RPSC_MBAR&> *m_pData;
	CCMRebarPscMoment* m_pParent;

	int m_nColHeader;
	int m_nColCount;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMRebarPscMomentGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCMRebarPscMomentGrid();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCMRebarPscMomentGrid)
	//afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMREBARPSCMOMENTGRID_H__)
