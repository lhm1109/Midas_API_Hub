#if !defined(__CMGXGRIDLIST_H__)
#define __CMGXGRIDLIST_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMGXGridList.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_common\wg_common_TBBrowserWnd.h"

struct SListColItem
{
	SListColItem() {};
	SListColItem(LPCTSTR name, int width, UINT type, UINT place)
	{ csName = name; nWidth = width; nType = type; nPlace = place; }
	CString csName;
	int  nWidth;
	UINT nType;
	UINT nPlace;
};

/////////////////////////////////////////////////////////////////////////////
// CCMGXGridList window
// Browser Style의 윈도우, 가로, 세로 방향으로 늘어난다.
// Column 이동이 금지된다. 
// 단, Row 이동은 가능하다. -> Row 이동시 change serial key

class CCMGXGridList : public CTBBrowserWnd
{
// Construction
public:
	CCMGXGridList();

//----------------------------------------------------------
// interface
	void AddColInfo(LPCTSTR name, int width, UINT type, UINT place);
	BOOL InsertRecordByKey(UINT key, UINT serial, CStringArray &aValue);
	BOOL DeleteRecordByKey(UINT key);
	//int  GetSelectedNameList(CArray<CString, CString&> &raName);
	int  GetSelectedKeyList(CArray<UINT, UINT> &aKey);

//----------------------------------------------------------
// pure virtual function
	virtual void MakeItemList() = 0;
	virtual void ModifySerialID(CArray<CString, CString&> &aName, CArray<UINT, UINT> &aSerial) = 0;
	//virtual void DoLButtonDblClick(ROWCOL nRow, ROWCOL nCol, UINT key) = 0;

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
	//virtual void OnChangedSelection(const CGXRange *changedRect, BOOL bIsDraggin, BOOL bKey);
	virtual BOOL OnSelDragRowsDrop(ROWCOL nFirstRow, ROWCOL nLastRow, ROWCOL nDestRow);
	virtual BOOL CanPaste() { return FALSE; }   // Copy 불가 

	//virtual BOOL OnLButtonDblClkRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt);

//----------------------------------------------------------
// Interface
public:
	void Initialize(CDBDoc* pDoc);
	void MakeItemEx();
	//CString GetKeyString(ROWCOL nRow);

	BOOL  GetColumnNoFor(int nID, ROWCOL &nCol) { nCol = nID+1; return TRUE; }
	BOOL  GetColumnIDFor(ROWCOL nCol, int &nID) { nID = nCol-1; return TRUE; }

//----------------------------------------------------------
// Implementation
public:
	void SetColInfo();
protected:
	void SetValue(ROWCOL nRow, UINT key, UINT serial, CStringArray &aValue);
	void SetHeaderTitle();

protected:
	CDBDoc* m_pDoc;
	CArray<SListColItem, SListColItem&> m_aColInfo;

	int m_nDefaultCount;
	int m_nUserDefCount;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMGXGridList)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCMGXGridList();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCMGXGridList)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMGXGRIDLIST_H__)
