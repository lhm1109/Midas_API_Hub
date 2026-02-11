#if !defined(__CMGILDBASEGRID_H__)
#define __CMGILDBASEGRID_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMLoadCombGrid.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_common\wg_common_TBBrowserWnd.h"


// default로 7개의 item을 보여주고 필요한 item만 show 한다.

#define D_GILD_LOAD_TYPE_COL  1
#define D_GILD_CENT_LINE_COL  2
#define D_GILD_GIRD_LINE_COL  3
#define D_GILD_CROS_LINE_COL  4
#define D_GILD_MEDN_STRP_COL  5
#define D_GILD_LOAD_STED_COL  6
#define D_GILD_LOAD_VALU_COL  7
#define D_GILD_LOAD_UNIT_COL  8
#define D_GILD_MAIN_LOAD_COL  9

/////////////////////////////////////////////////////////////////////////////
// CCMGildBaseGrid window
// Browser Style의 윈도우, 가로, 세로 방향으로 늘어난다.
// Row, Column 이동이 금지.

// 기본적으로 두줄의 컬럼헤더를 가진다.

class CCMGildBaseGrid : public CTBBrowserWnd
{
// Construction
public:
	CCMGildBaseGrid();

	void AddColInfo(UINT nColInfo) { m_aColInfo.Add(nColInfo); }

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

	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual void OnCanceledEditing(ROWCOL nRow, ROWCOL nCol);
	void ChangeColumnStatus(ROWCOL nRow, ROWCOL nCol);
	void ChangeEndColEnable(ROWCOL nRow, int nLoadType);
	void ChangeEndColEnable2(ROWCOL nRow, int nMedialStrp);
	void ChangeEndColEnable3(ROWCOL nRow, BOOL bEnable);
	void ChangeEndColEnableLoad(ROWCOL nRow);

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
	void Initialize(CDBDoc* pDoc, T_GILD_D *pData);
	void MakeItemEx();
	void UpdateBuffer(int nCmd, CArray<int, int> &aOldIndex, CArray<int, int> &aIndex, CArray<T_GILD_BASE, T_GILD_BASE&> &aData);
	void Setdata(int nComboBoxIndex);

	BOOL  GetColumnNoFor(int nID, ROWCOL &nCol) { nCol = nID+1; return TRUE; }
	BOOL  GetColumnIDFor(ROWCOL nCol, int &nID) { nID = nCol-1; return TRUE; }


//----------------------------------------------------------
// Implementation
public:
	void SetColInfo();

protected:
	void SetValue(ROWCOL nRow, int index, T_GILD_BASE &data);
	void SetHeaderTitle();
	void ConvDataToStrArray(T_GILD_BASE &data, CStringArray &aValue);

	BOOL InsertRecordByKey(int index, T_GILD_BASE &data);
	BOOL DeleteRecordByKey(int index, T_GILD_BASE &data);

	void  SelectGridRow(long* aRow, long nSize);

	BOOL GetValue(ROWCOL nRow, T_GILD_BASE &data);
	BOOL MakeDataArray(CStringArray& value, CRowColArray& awCols, CArray<T_GILD_BASE, T_GILD_BASE&>& aData);
	BOOL ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, T_GILD_BASE &data);
	BOOL ConvStrArrayToData(CStringArray &aValue, T_GILD_BASE& data);

	BOOL AddToDB(ROWCOL nRow);
	BOOL AddData(int nIndex, T_GILD_BASE &data);
	BOOL ModifyToDB(ROWCOL nRow);
	BOOL ModifyData(int nOldIndex, int nIndex, T_GILD_BASE &data);
	BOOL DeleteFromDB(CRowColArray &awRow);
	BOOL IsUsedColumn(ROWCOL nCol);

	CString GetDefValue(int ix);

	void SetLoadTypeColumn(ROWCOL nCol);
	void SetLoadLineColumn(ROWCOL nCol);
	void SetGirdLineColumn(ROWCOL nCol);
	void SetCrosLineColumn(ROWCOL nCol);
	void SetMedlStrpColumn(ROWCOL nCol, BOOL bNone);

	int     GetLoadTypeData(CString value);
	CString GetLoadTypeStr(int data);
	UINT    GetLoadLineData(CString value);
	CString GetLoadLineStr(int data);
	UINT    GetGirdLineData(CString value, BOOL &bGirder);
	CString GetGirdLineStr(int data, BOOL bGirder);
	UINT    GetCrosLineData(CString value);
	CString GetCrosLineStr(int data);
	UINT    GetMedlStrpData(CString value);
	CString GetMedlStrpStr(int data);

protected:
	CDBDoc* m_pDoc;
	T_GILD_D* m_pData;

	int m_nColHeader;
	int m_nColCount;
	CArray<UINT, UINT> m_aColInfo;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMGildBaseGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCMGildBaseGrid();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCMGildBaseGrid)
	//afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMGILDBASEGRID_H__)
