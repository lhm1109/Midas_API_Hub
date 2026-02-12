#if !defined(__CMGXLDLNGRIDLIST_H__)
#define __CMGXLDLNGRIDLIST_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMGXLdlnGridList.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_common\wg_common_TBBrowserWnd.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CCMGXLdlnGridList window
// Browser Style의 윈도우, 가로, 세로 방향으로 늘어난다.
// Column 이동이 금지된다. 
// 단, Row 이동은 가능하다. -> Row 이동시 change serial key

class CCMGXLdlnGridList : public CTBBrowserWnd, public CDBUpdateConnector
{
// Construction
public:
	CCMGXLdlnGridList();

//----------------------------------------------------------
// interface
	//void AddColInfo(LPCTSTR name, int width, UINT type, UINT place);
	BOOL InsertRecordByKey(UINT key, UINT serial, CStringArray &aValue);
	BOOL DeleteRecordByKey(UINT key);
	//int  GetSelectedNameList(CArray<CString, CString&> &raName);
	int  GetSelectedKeyList(CArray<UINT, UINT> &aKey);

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
	
	virtual BOOL GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt = gxCopy, int nType = 0);
	virtual BOOL OnLButtonDblClkRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt);

//----------------------------------------------------------
// Grid's Standard Overriadable Functions
	//virtual void OnChangedSelection(const CGXRange *changedRect, BOOL bIsDraggin, BOOL bKey);
	virtual BOOL OnSelDragRowsDrop(ROWCOL nFirstRow, ROWCOL nLastRow, ROWCOL nDestRow);
	virtual BOOL CanPaste() { return FALSE; }   // Copy 불가 

//----------------------------------------------------------
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
	void Initialize(CDBDoc* pDoc);
	void MakeItemEx();

	BOOL  GetColumnNoFor(int nID, ROWCOL &nCol) { nCol = nID+1; return TRUE; }
	BOOL  GetColumnIDFor(ROWCOL nCol, int &nID) { nID = nCol-1; return TRUE; }

//----------------------------------------------------------
// Implementation
public:
	void SetColInfo();
	void UpdateUnit();
protected:
	void SetHeaderTitle();

	void SetValue(ROWCOL nRow, UINT key, UINT serial, CStringArray &aValue);
	void ConvertDataToStr(T_LDLN_D &data, CStringArray &aValue);
	void ModifySerialID(CArray<CString, CString&> &aName, CArray<UINT, UINT> &aSerial);

protected:
	CDBDoc* m_pDoc;
	int m_nColCount;
	int m_nColHeader;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMGXLdlnGridList)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCMGXLdlnGridList();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCMGXLdlnGridList)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMGXGRIDLIST_H__)
