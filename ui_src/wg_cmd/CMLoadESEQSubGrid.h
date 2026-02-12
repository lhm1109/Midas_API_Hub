#if !defined(AFX_CMLOADESEQSUBGRID_H__353BC8C3_7416_11D4_9AAF_0000C0B9C58C__INCLUDED_)
#define AFX_CMLOADESEQSUBGRID_H__353BC8C3_7416_11D4_9AAF_0000C0B9C58C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMLoadESEQSubGrid.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_common\wg_common_TBBrowserWnd.h"
/////////////////////////////////////////////////////////////////////////////
// CCMLoadESEQSubGrid window
class CCMLoadESEQMainGrid;

class CCMLoadESEQSubGrid : public CTBBrowserWnd
{
// Construction
public:
	CCMLoadESEQSubGrid();
	CCMLoadESEQMainGrid* m_pESEQMainGrid;

// Attributes
public:

// Operations
public:
	void Initialize(CDBDoc* pDoc);
	BOOL CheckField(BOOL bFlag=TRUE);
	BOOL CheckAllBlankField(BOOL bFlag=TRUE);
	BOOL CheckExistBlankField();
	void ChangeMaterialData(ROWCOL nRow);
	void InitESEQData(T_ESEQ_BASE& ESEQBase, int nNum);
	void InitESEQData();
	const CString& GetValueRowCol(ROWCOL nRow, ROWCOL nCol);

	BOOL m_bModified;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMLoadESEQSubGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCMLoadESEQSubGrid();

protected:
	CDBDoc* m_pDoc;
	CStringArray m_arTitle;
	CStringArray m_arMatlType;
	ROWCOL m_nOldRow;

	CString m_strMatlType;
	CString m_strMatlName;

	void SetHeaderTitle();
	void InitGridData();
	void InitMatlData();
	void OperateDataSub(ROWCOL nRow, ROWCOL nCol);

	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual void OnInitCurrentCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnDeleteRecord(ROWCOL nRow);
	virtual BOOL OnDeleteRecords(CRowColArray &awRows);
	virtual BOOL OnInsertEmptyRecord(ROWCOL nRow);
	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt=1, UINT flags=0);


	// Generated message map functions
protected:
	//{{AFX_MSG(CCMLoadESEQSubGrid)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMLOADESEQSUBGRID_H__353BC8C3_7416_11D4_9AAF_0000C0B9C58C__INCLUDED_)
