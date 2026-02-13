#if !defined(AFX_AssessmentLoadCombSubGridSTL_H__5B5AF003_5156_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_AssessmentLoadCombSubGridSTL_H__5B5AF003_5156_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AssessmentLoadCombSubGridSTL.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_common\wg_common_TBBrowserWnd.h"

#define D_COL_RAC_LCASE 1
#define D_COL_RAC_FACTOR 2


class CAssessmentLoadCombMainGridSTL;

class CAssessmentLoadCombSubGridSTL : public CTBBrowserWnd
{
	// Construction
public:
	CAssessmentLoadCombSubGridSTL();

	// Attributes
public:
	CStringArray m_aTitle;
	CDBDoc* m_pDoc;
	CAssessmentLoadCombMainGridSTL* m_pMainGrid;
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

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CAssessmentLoadCombSubGridSTL)
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

	BOOL ValidataFieldDbl(ROWCOL nRow, ROWCOL nCol);

	virtual BOOL OnStartEditing(ROWCOL nRow, ROWCOL nCol);  // 필요에 의해 추가함.

	void OnChangeStld();

	// Implementation
public:
	virtual ~CAssessmentLoadCombSubGridSTL();
	BOOL IsMainAppendRow();
	BOOL IsMainNewRow();
	virtual void SetData(CString sKey);
	void GetData(T_ALCS_D &data);

	// Generated message map functions
protected:
	//{{AFX_MSG(CAssessmentLoadCombSubGridSTL)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AssessmentLoadCombSubGridSTL_H__5B5AF003_5156_11D3_92DE_0000C0B0E6B3__INCLUDED_)
