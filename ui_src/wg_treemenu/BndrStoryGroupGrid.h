#if !defined(AFX_BNDRSTORYGROUPGRID_H__2270F7D8_DF2D_400C_8145_2759964C80E3__INCLUDED_)
#define AFX_BNDRSTORYGROUPGRID_H__2270F7D8_DF2D_400C_8145_2759964C80E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BndrStoryGroupGrid.h : header file
//

#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CBndrStoryGroupGrid window

class CBndrStoryGroupGrid : public CGXGridWnd, public CDBUpdateConnector
{
// Construction
public:
	CBndrStoryGroupGrid();
	virtual ~CBndrStoryGroupGrid();

// Attributes
public:

// Operations
public:
	void Initialize();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBndrStoryGroupGrid)
	//}}AFX_VIRTUAL

// Implementation
protected:
	void ShowAllData();
	void SetBngrColumn(ROWCOL nCol);
	void UpdateBuffer();
	void AddStdg(T_STDG_K key, T_STDG_D& rData);
	void DelStdg(T_STDG_K key);
	int FindPos(T_STDG_K key);

protected:
	virtual BOOL GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt = gxCopy, int nType = 0);
	virtual BOOL OnEndEditing(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnPasteFromClipboard(const CGXRange &range);

	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
protected:
	//{{AFX_MSG(CBndrStoryGroupGrid)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BNDRSTORYGROUPGRID_H__2270F7D8_DF2D_400C_8145_2759964C80E3__INCLUDED_)
