#if !defined(__CMRESULTLCTBGRID_H__)
#define __CMRESULTLCTBGRID_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMResultLctbGrid.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_common\wg_common_TBBrowserWnd.h"


struct SLcomColItem
{
	SLcomColItem() {};
	SLcomColItem(LPCTSTR name, UINT lcType, UINT key)
	{ csName = name; nLCtype = lcType; nKey = key; }
	CString csName;
	UINT    nLCtype;
	UINT    nKey;
};

/////////////////////////////////////////////////////////////////////////////
// CCMResultLctbGrid window
// Browser Style의 윈도우, 가로, 세로 방향으로 늘어난다.
// Column 이동이 금지된다.
class CCMResultLctbGrid : public CTBBrowserWnd
{
// Construction
public:
	CCMResultLctbGrid();
	int GetSelectedNameList(CArray<CString, CString&> &raName);

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
	/*
	virtual void OnChangedSelection(const CGXRange *changedRect, BOOL bIsDraggin, BOOL bKey);
	virtual BOOL OnSelDragRowsDrop(ROWCOL nFirstRow, ROWCOL nLastRow, ROWCOL nDestRow);
	virtual BOOL OnPasteFromClipboard(const CGXRange &range);

	BOOL PasteFromClipboard(const CGXRange &range, CString* aDefVal, int nKeyCol, 
													CGXGridWnd &x, int &nPasteMode, int &nPastedCols);
													*/

//----------------------------------------------------------
// Interface
public:
	void Initialize(CDBDoc* pDoc);
	void MakeItemEx();
	void UpdateBuffer();
	CString GetKeyString(ROWCOL nRow);

	BOOL  GetColumnNoFor(int nID, ROWCOL &nCol) { nCol = nID+1; return TRUE; }
	BOOL  GetColumnIDFor(ROWCOL nCol, int &nID) { nID = nCol-1; return TRUE; }


//----------------------------------------------------------
// Implementation
public:
	void SetColInfo();
protected:
	void SetValue(ROWCOL nRow, T_LCTB_K key, T_LCTB_D& data);
	void SetHeaderTitle();
	void ConvDataToStrArray(T_LCTB_D &data, CStringArray &aValue);
	void SetLcomCaseValue(CStringArray &aValue, T_LCTB_BASE &base);

	BOOL InsertRecordByKey(T_LCTB_K Key, T_LCTB_D &Data);
	BOOL DeleteRecordByKey(T_LCTB_K Key, T_LCTB_D &Data);

protected:
	CDBDoc* m_pDoc;
	CArray<SLcomColItem, SLcomColItem&> m_aColInfo;

	int m_nFixedCol;
	int m_nStldCount;
	int m_nSplcCount;
	int m_nThisCount;
	int m_nMvldCount;
	int m_nSmlcCount;
	int m_nSgldCount;
	int m_nEspcCount;
	int m_nNspcCount; // Non-Dissipative 응답 스펙트럼
	int m_nNespCount; // Non-Dissipative 우발편심 응답 스펙트럼
	//int m_nLcomCount;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMResultLctbGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCMResultLctbGrid();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCMResultLctbGrid)
	//afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMRESULTLCTBGRID_H__)
