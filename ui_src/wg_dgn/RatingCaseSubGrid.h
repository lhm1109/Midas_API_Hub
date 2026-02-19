// RatingCaseSubGrid.h: interface for the CRatingCaseSubGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RATINGCASESUBGRID_H__C7767E0F_2844_4DB1_A1B3_398D8F3411E9__INCLUDED_)
#define AFX_RATINGCASESUBGRID_H__C7767E0F_2844_4DB1_A1B3_398D8F3411E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_common\wg_common_TBBrowserWnd.h"
#include "RatingCaseMainGrid.h"

class CRatingCaseMainGrid;

class CRatingCaseSubGrid : public CTBBrowserWnd  
{
// Construction
public:
	CRatingCaseSubGrid();

// Attributes
public:
	CStringArray m_aTitle;
	CDBDoc* m_pDoc;
	//CRatingCaseMainGrid* m_pMainGrid;
	CString m_sKey;
	int m_nLcomType;
	int m_nLCtype;
	CString m_csPostfix;

	T_RTLD_D m_Data;

// Operations
public:
	void Initialize(CDBDoc* pDoc);
	void SetHeaderTitle();
	void SetLoadCaseColumnStyle(ROWCOL nCol);
	CString GetKey() { return m_sKey; }
	void SetData(T_RTLD_D &Data);

	//void SetRecordModified();
	void GetCombItem(T_RTLD_D &data);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMLoadCombSubGrid)
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
	virtual ~CRatingCaseSubGrid();
	BOOL IsMainAppendRow();
	BOOL IsMainNewRow();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCMLoadCombSubGrid)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_RATINGCASESUBGRID_H__C7767E0F_2844_4DB1_A1B3_398D8F3411E9__INCLUDED_)
