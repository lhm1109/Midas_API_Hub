#if !defined(AFX_RATINGLOADCOMBSUBGRID_H__5B5AF003_5156_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_RATINGLOADCOMBSUBGRID_H__5B5AF003_5156_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RatingLoadCombSubGrid.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_common\wg_common_TBBrowserWnd.h"

class CRatingLoadCombMainGrid;

class CRatingLoadCombSubGrid : public CTBBrowserWnd
{
// Construction
public:
	CRatingLoadCombSubGrid();

// Attributes
public:
	CStringArray m_aTitle;
	CDBDoc* m_pDoc;
	CRatingLoadCombMainGrid* m_pMainGrid;
	CString m_sKey;
	
	int m_nLcomType;
	int m_nLCtype;
	CString m_csPostfix;

// Operations
public:
	void Initialize(CDBDoc* pDoc);
	void SetHeaderTitle();
	void SetLoadCaseColumnStyle(ROWCOL nCol);
	CString GetKey() { return m_sKey; }
	void SetData(CString sKey);

	void GetCombItem(T_RLCS_D &data);
	void SetMVOnly(BOOL bMVOnly){m_bMoveingLoadOnly = bMVOnly;}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRatingLoadCombSubGrid)
	//}}AFX_VIRTUAL
	virtual BOOL OnAddNewRecord(ROWCOL nRow);
	virtual BOOL OnModifyRecord(ROWCOL nRow);
	virtual BOOL OnDeleteRecord(ROWCOL nRow);
	virtual BOOL OnDeleteRecords(CRowColArray &awRows);
	virtual BOOL OnCanceledAddNew();
	virtual BOOL OnCanceledModify();
	virtual BOOL OnInsertEmptyRecord(ROWCOL nRow);

	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnValidateField(ROWCOL nRow, ROWCOL nCol);

	void OnChangeStld();

// Implementation
public:
	virtual ~CRatingLoadCombSubGrid();
	BOOL IsMainAppendRow();
	BOOL IsMainNewRow();

	// Generated message map functions
protected:
	//{{AFX_MSG(CRatingLoadCombSubGrid)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL m_bMoveingLoadOnly;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RATINGLOADCOMBSUBGRID_H__5B5AF003_5156_11D3_92DE_0000C0B0E6B3__INCLUDED_)
