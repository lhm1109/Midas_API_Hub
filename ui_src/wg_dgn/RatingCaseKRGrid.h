// RatingCaseKRGrid.h: interface for the CRatingCaseKRGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RATINGCASEKRGRID_H__)
#define AFX_RATINGCASEKRGRID_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_common\wg_common_TBBrowserWnd.h"
#include "RatingCaseMainGrid.h"

class CRatingCaseKRGrid : public CTBBrowserWnd  
{
// Construction
public:
	CRatingCaseKRGrid();

// Attributes
public:
	CStringArray m_aTitle;
	CDBDoc* m_pDoc;
	
	CString m_sKey;
	int m_nLcomType;
	int m_nLCtype;
	CString m_csPostfix;

	T_RKLC_D m_Data;

// Operations
public:
	void Initialize(CDBDoc* pDoc);
	void SetHeaderTitle();
	void SetLoadCaseColumnStyle(ROWCOL nCol);
	CString GetKey() { return m_sKey; }
	void SetData(T_RKLC_D &data);

	void GetCombItem(T_RKLC_D &data);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RatingCaseKRGrid)
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
	virtual ~CRatingCaseKRGrid();
	BOOL IsMainAppendRow();
	BOOL IsMainNewRow();

	// Generated message map functions
protected:
	//{{AFX_MSG(RatingCaseKRGrid)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_RATINGCASEKRGRID_H__)
