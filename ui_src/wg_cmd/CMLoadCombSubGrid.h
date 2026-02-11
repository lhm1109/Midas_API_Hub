#if !defined(AFX_CMLOADCOMBSUBGRID_H__5B5AF003_5156_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_CMLOADCOMBSUBGRID_H__5B5AF003_5156_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMLoadCombSubGrid.h : header file
//

//#include "CMLoadCombDlg.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_common\wg_common_TBBrowserWnd.h"

class CCMLoadCombMainGrid;
enum LCOM_TYPE { LCOM_GENERAL, LCOM_SEISMIC, LCOM_LINEAR };
/////////////////////////////////////////////////////////////////////////////
// CCMLoadCombSubGrid window

class CCMLoadCombSubGrid : public CTBBrowserWnd
{
// Construction
public:
	CCMLoadCombSubGrid();

// Attributes
public:
	CStringArray m_aTitle;
	CDBDoc* m_pDoc;
	CCMLoadCombMainGrid* m_pMainGrid;
	CString m_sKey;
	int m_nLcomType;
	int m_nLCtype;
	CString m_csPostfix;

// Operations
public:
	void Initialize(CDBDoc* pDoc);
	void SetLcomType(int nLcomType);
	void ChangeLcomType(int nLcomType);
	void SetHeaderTitle();
	void SetLoadCaseColumnStyle(ROWCOL nCol);
	CString GetKey() { return m_sKey; }
	void SetData(CString sKey);

	void SetRecordModified();
	void GetCombItem(T_LCOM_D &data);


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
	virtual ~CCMLoadCombSubGrid();
	BOOL IsMainAppendRow();
	BOOL IsMainNewRow();
	void SetSeismicType(LCOM_TYPE nType) { m_nSeismicType=nType; }

	// Generated message map functions
protected:
	//{{AFX_MSG(CCMLoadCombSubGrid)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	LCOM_TYPE m_nSeismicType;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMLOADCOMBSUBGRID_H__5B5AF003_5156_11D3_92DE_0000C0B0E6B3__INCLUDED_)
