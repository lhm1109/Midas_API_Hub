#if !defined(AFX_CMBGENTABLEGRID_H__84EB48C5_45BB_11D4_B00F_0000C0B9C58C__INCLUDED_)
#define AFX_CMBGENTABLEGRID_H__84EB48C5_45BB_11D4_B00F_0000C0B9C58C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMBgenTableGrid.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h" // for T_MAKEBUILDING
#include "..\wg_db\wg_db_DataCtrl.h"

#include "..\wg_common\wg_common_TBBrowserWnd.h"


/////////////////////////////////////////////////////////////////////////////
// CCMBgenTableGrid window

class CCMBgenTableGrid : public CTBBrowserWnd
{
// Construction
public:
	CCMBgenTableGrid();

// Attributes
public:
	CStringArray m_aTitle, m_aDefVal;
	CArray<int, int> m_aUnit;
	CDBDoc* m_pDoc;

// Operations
public:
	void Initialize(CDBDoc* pDoc);
	void SetHeaderTitle();
	void MakeItemEx(CArray<T_MAKEBUILDING, T_MAKEBUILDING&> &aMakeBuilding);
	int GetCount();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMBgenTableGrid)
	//}}AFX_VIRTUAL

	virtual BOOL OnValidateField(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnInsertEmptyRecord(ROWCOL nRow);

// Implementation
public:
	virtual ~CCMBgenTableGrid();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCMBgenTableGrid)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMBGENTABLEGRID_H__84EB48C5_45BB_11D4_B00F_0000C0B9C58C__INCLUDED_)
