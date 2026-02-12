#if !defined(__CMElsPlsConcDmgGraphGrid_H__)
#define __CMElsPlsConcDmgGraphGrid_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMElsPlsConcDmgGraphGrid.h : header file
//

#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_common\wg_common_TBBrowserWnd.h"

struct PMATL_CDMG_BEHAVIOR
{
	double dInelasticStrain;
	double dYieldStress;
	double dDamage;
	
	bool operator<(const PMATL_CDMG_BEHAVIOR &rhs)  const
	{
		return dInelasticStrain < rhs.dInelasticStrain;
	}
};


/////////////////////////////////////////////////////////////////////////////
// CCMElsPlsConcDmgGraphGrid window

class CCMElsPlsConcDmgGraphGrid : public CTBBrowserWnd
{

	// Construction
public:
	CCMElsPlsConcDmgGraphGrid(CWnd* pParent);
	virtual ~CCMElsPlsConcDmgGraphGrid();

	// Operations
public:
	void Initialize();
	void SetData(CArray<PMATL_CDMG_BEHAVIOR, PMATL_CDMG_BEHAVIOR> &aData);
	BOOL GetData(CArray<PMATL_CDMG_BEHAVIOR, PMATL_CDMG_BEHAVIOR> &aData, BOOL bSort = TRUE);

	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags);
	BOOL SetRightValidCell (ROWCOL nRow, ROWCOL nCol);
	BOOL SetLeftValidCell  (ROWCOL nRow, ROWCOL nCol);


public:
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIehpYieldPropGrid)
	//}}AFX_VIRTUAL

protected:
	virtual BOOL GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt = gxCopy, int nType = 0);
	virtual BOOL OnStartEditing(ROWCOL nRow, ROWCOL nCol);
	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnEndEditing(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL CanPaste() { return TRUE; }
	virtual BOOL CanCut() { return FALSE; }
	virtual BOOL OnPasteFromClipboard(const CGXRange &range);
	virtual BOOL CopyRange(const CGXRangeList& selList);
	virtual BOOL OnAddNewRecord(ROWCOL nRow);
	virtual BOOL OnModifyRecord(ROWCOL nRow);
	virtual BOOL OnDeleteRecord(ROWCOL nRow);
	virtual BOOL OnDeleteRecords(CRowColArray &awRows);

protected:
	void SetDataToRow(ROWCOL nRow, CStringArray& raValue);
	BOOL GetRowToData(ROWCOL nRow, CStringArray& raValue);
	void SortGrid();

	CString GetEditingValue(ROWCOL nRow, ROWCOL nCol);
	BOOL IsPasteToAllRange(const CGXRange &range, CString szData); // copy한 Cell이 1개 이고, 동일한 행의 cell 여러개로 붙여넣은 상황일 때 TRUE.. 이 경우엔 붙여넣을 Range의 모든 Cell에 복사한 Cell의 값을 붙여 넣어 줘야 함;
	CWnd* m_pParent;

	// Generated message map functions

	//{{AFX_MSG(CCMElsPlsConcDmgGraphGrid)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMElsPlsConcDmgGraphGrid_H__)
