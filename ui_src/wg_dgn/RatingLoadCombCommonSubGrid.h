#if !defined(AFX_RATINGLOADCOMBCOMMONSUBGRID_H__5B5AF003_5156_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_RATINGLOADCOMBCOMMONSUBGRID_H__5B5AF003_5156_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RatingLoadCombCommonSubGrid.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_common\wg_common_TBBrowserWnd.h"

#define D_COL_RAC_LCASE 1
#define D_COL_RAC_CHK_OR_FACTOR 2 // 윗 테이블(m_nGridType == 0) 일 때는 _T("Chk.") 이고 아래 테이블(m_nGirdType == 1) 일 때는 "Factor" 임
#define D_COL_RAC_FACTOR_OR_IMPACTOR 3 // 윗 테이블(m_nGridType == 0) 일 때는 _T("Factor") 이고 아래 테이블(m_nGirdType == 1) 일 때는 "Impact Factor" 임

// 메모 : GetData는 Child에 있음

class CRatingLoadCombCommonMainGrid;

class CRatingLoadCombCommonSubGrid : public CTBBrowserWnd
{
// Construction
public:
	CRatingLoadCombCommonSubGrid(int nType);

// Attributes
public:
	CStringArray m_aTitle;
	CDBDoc* m_pDoc;
	CRatingLoadCombCommonMainGrid* m_pMainGrid;
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
	virtual void SetData(CString sKey) = 0;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRatingLoadCombCommonSubGrid)
	//}}AFX_VIRTUAL
	virtual BOOL OnAddNewRecord(ROWCOL nRow);
	virtual BOOL OnModifyRecord(ROWCOL nRow);
	virtual BOOL OnDeleteRecord(ROWCOL nRow);
	virtual BOOL OnDeleteRecords(CRowColArray &awRows);
	virtual BOOL OnCanceledAddNew();
	virtual BOOL OnCanceledModify();
	virtual BOOL OnInsertEmptyRecord(ROWCOL nRow);
	virtual CString GetDefFactorVal(int nGridType) = 0;

	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnValidateField(ROWCOL nRow, ROWCOL nCol);

	BOOL ValidataFieldDbl( ROWCOL nRow, ROWCOL nCol );

	virtual BOOL OnStartEditing(ROWCOL nRow, ROWCOL nCol);  // 필요에 의해 추가함.

	void OnChangeStld();

// Implementation
public:
	virtual ~CRatingLoadCombCommonSubGrid();
	BOOL IsMainAppendRow();
	BOOL IsMainNewRow();

	// Generated message map functions
protected:
	//{{AFX_MSG(CRatingLoadCombCommonSubGrid)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	int m_nGridType; // 0 : Static Load Case, 1 : Live Load Case
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RATINGLOADCOMBCOMMONSUBGRID_H__5B5AF003_5156_11D3_92DE_0000C0B0E6B3__INCLUDED_)
