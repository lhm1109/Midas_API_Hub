#if !defined(AFX_CMLOADESEQTBGRID_H__B9762D61_819F_11D4_A678_00010263A1CE__INCLUDED_)
#define AFX_CMLOADESEQTBGRID_H__B9762D61_819F_11D4_A678_00010263A1CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMLoadESEQTBGrid.h : header file
//

#include <afxtempl.h>
#include "..\wg_common\wg_common_TBGrid.h"

// 종료시 편집중인 레코드의 저장 여부 지정시 사용되는 parameter
#define D_TB_SAVE_NEVER   1     // 저장하지 않는다.
#define D_TB_SAVE_ALWAYS  2     // 항상 저장한다.
#define D_TB_SAVE_IF_YES  3     // 물어보고 Yes를 했을 때 저장한다.

class CTBGrid;
class CCMLoadESEQTBGrid;
/////////////////////////////////////////////////////////////////////////////
// CCMLoadESEQParam
class CCMLoadESEQParam 
{
public:
	CCMLoadESEQParam::CCMLoadESEQParam();
	BOOL    m_bIsBrowseInit;
	ROWCOL  m_nCurrentRow;              // currently edited row
	long    m_nRecordCount;             // can be used by derived classes
};

/////////////////////////////////////////////////////////////////////////////
// CGXBrowseGridArrowRowHeader control
class CCMLoadESEQArrowRowHeader: public CGXControl
{
public:
	CCMLoadESEQArrowRowHeader(CCMLoadESEQTBGrid* pBrowserGrid);

	// event handler
	virtual void Draw(CDC* pDC, CRect rect, ROWCOL nRow, ROWCOL nCol, const CGXStyle& style, const CGXStyle* pStandardStyle);

// Attributes
public:
	CBitmap m_bmPencil;
	CBitmap m_bmStar;
	CBitmap m_bmArrow;
	CCMLoadESEQTBGrid* m_pBrowserGrid;
	BOOL m_bWin95Look;
};

/////////////////////////////////////////////////////////////////////////////
// CCMLoadESEQTBGrid window
class CCMLoadESEQTBGrid : public CTBGrid
{
// static functions
public:
	static BOOL IsNumeric(CString str, int& ecode);
	static int CompareKey(const void* arg1, const void* arg2);

// Construction
public:
	DECLARE_DYNAMIC(CCMLoadESEQTBGrid);
	CCMLoadESEQTBGrid();

// Attributes
public:
	enum EditMode { noMode=0, edit, addnew };

	CCMLoadESEQParam* GetBrowseParam() { return &m_BrowseParam; }
	BOOL        IsEdit() { return (m_nEditMode != noMode); }

	UINT        m_nEditMode;                // current mode
	BOOL        m_bValidField;              // Field validation 결과 저장
	BOOL        m_bValidRecord;             // 레코드 validation 결과 저장

	BOOL        m_bRedrawOnMovedCurrentCell;
	CPoint      m_ptHit;

// Operations
public:
	BOOL IsValidLongNumber(CString value, int &nErrCode);
	BOOL IsValidDoubleNumber(CString value, int &nErrCode);
	BOOL SetRecordCount(ROWCOL nRows, UINT flags = GX_UPDATENOW);
	BOOL InsertRecord(ROWCOL nRow);
	BOOL DeleteRecord(ROWCOL nRow); // 지정된 레코드 삭제
	BOOL DeleteRecord(CRowColArray& awRows);  // 레코드 셋 삭제
	BOOL DeleteAllRecords(); // 모든 레코드 삭제
	BOOL InsertEmptyRecord(ROWCOL nRow);

	BOOL GetColumnNoFor(CString title, ROWCOL &nCol);
	const CString& GetValueRowCol(ROWCOL nRow, ROWCOL nCol);
	CString GetValueRowColBeforeEdit(ROWCOL nRow, ROWCOL nCol);
	BOOL ConvertFormulaToValue(CString& value);
	BOOL IsAppendRow(ROWCOL nRow);

	BOOL SetLeftValidCell(ROWCOL nRow, ROWCOL nCol);
	BOOL SetRightValidCell(ROWCOL nRow, ROWCOL nCol);
	BOOL SetDownValidCell(ROWCOL nRow, ROWCOL nCol);

	BOOL ResizeColWidthsToFitSmart(long nCount);

	// DisplayOnlyMode관련
	void SetModeDisplayOnly(BOOL bReadOnly);
	BOOL IsLockEdit() { return m_bLockEdit; }
	void LockEditMode(BOOL bLock);

	BOOL AddNew(ROWCOL nRow);
	BOOL Edit(ROWCOL nRow);

	// Dialog, View 종료시 저장되지 않은 레코드에 대한 처리 담당
	BOOL OnTerminate(int bMode);

protected:
	BOOL CanAppend();
	BOOL CancelEdit();
	BOOL SetFieldValidation(BOOL bValid) { m_bValidField = bValid; return bValid; }
	BOOL SetRecordValidation(BOOL bValid) { m_bValidRecord = bValid; return bValid; }

// SetModeDisplayOnly 함수를 도와주는 함수와 멤버
protected:
	void SaveCurrentColStyle();
	void RestorePrevColStyle();
	CArray<CGXStyle, CGXStyle&> m_aColStyle;
	BOOL m_bDisplayOnly;  // Flag for Display only mode
	BOOL m_bLockEdit;     // Flag for temporary lock editing

// OnTerminate 함수를 도와주는 함수
protected:
	BOOL SaveCurrentRecord();

// GetStyleRowCol 함수를 도와주는 함수
protected:
	void SetCurrentRowStyle(CGXStyle &style);

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMLoadESEQTBGrid)
	//}}AFX_VIRTUAL
	virtual void Initialize();
	virtual void InitBrowserSettings();
	virtual BOOL SetRowCount(ROWCOL nRows, UINT flags = GX_UPDATENOW);
	virtual BOOL GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt = gxCopy, int nType = 0);
	virtual BOOL SetCurrentCell(ROWCOL nRow, ROWCOL nCol, UINT flags = GX_SCROLLINVIEW | GX_UPDATENOW);
	virtual BOOL OnLeftCell(ROWCOL nRow, ROWCOL nCol, ROWCOL nNewRow, ROWCOL nNewCol);
	virtual void OnMovedCurrentCell(ROWCOL nRow, ROWCOL nCol);

	virtual BOOL OnStartEditing(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnEndEditing(ROWCOL nRow, ROWCOL nCol);

	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL CanSelectCurrentCell(BOOL bSelect, ROWCOL dwSelectRow, ROWCOL dwSelectCol, ROWCOL dwOldRow, ROWCOL dwOldCol);

	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt=1, UINT flags=0);
	virtual BOOL DoRButtonDown(UINT flags, CPoint point);
	virtual BOOL DoLButtonDblClk(UINT flags, CPoint point);

	virtual BOOL OnValidateCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnPasteFromClipboard(const CGXRange &range);
	virtual BOOL CanCut();

	virtual void PrepareClearSelection(BOOL bCreateHint = FALSE);

	// Insert Key를 이용한 삽입을 허용/금지하는 함수 관련
	virtual BOOL CanInsertRowByPressingKey() { return m_bCanInsertRowByPressingKey; }
	virtual BOOL CanDeleteRowByPressingKey() { return m_bCanDeleteRowByPressingKey; }
	BOOL EnableInsertRowByPressingKey(BOOL bEnable);
	BOOL EnableDeleteRowByPressingKey(BOOL bEnable);
	// Append Row Enable/Disable
	BOOL EnableAppendRow(BOOL bEnable);
	ROWCOL GetAppendRowNo();


	// 사용자가 Override해야할 함수 (by Jong-Bock Seon)
	virtual BOOL CanDeleteRecords(CRowColArray& awRows);
	virtual BOOL OnValidateField(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnValidateRecord(ROWCOL nRow);
	virtual BOOL OnAddNewRecord(ROWCOL nRow);
	virtual BOOL OnModifyRecord(ROWCOL nRow);
	virtual BOOL OnDeleteRecord(ROWCOL nRow);
	virtual BOOL OnDeleteRecords(CRowColArray &awRows);
	virtual BOOL OnCanceledAddNew();
	virtual BOOL OnCanceledModify();
	virtual BOOL OnInsertEmptyRecord(ROWCOL nRow);

// Implementation
public:
	virtual ~CCMLoadESEQTBGrid();

// Attributes
protected:
	CCMLoadESEQParam m_BrowseParam;
	BOOL           m_bCanInsertRowByPressingKey;
	BOOL           m_bCanDeleteRowByPressingKey;
	BOOL           m_bEnabledAppend;

	// Generated message map functions
protected:
	//{{AFX_MSG(CCMLoadESEQTBGrid)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CStringArray m_EditRecord;
	CArray <CGXStyle, CGXStyle&> m_EditRecordStyle;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMLOADESEQTBGRID_H__B9762D61_819F_11D4_A678_00010263A1CE__INCLUDED_)
