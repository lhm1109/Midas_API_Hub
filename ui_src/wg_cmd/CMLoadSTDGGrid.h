#if !defined(AFX_CMLOADSTDGGRID_H__D6CD7293_D17F_4297_95A3_A32E74672DB0__INCLUDED_)
#define AFX_CMLOADSTDGGRID_H__D6CD7293_D17F_4297_95A3_A32E74672DB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMLoadSTDGGrid.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_common\wg_common_TBBrowserWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CCMLoadSTDGGrid window

class CCMLoadSTDGGrid : public CTBBrowserWnd
{
// Construction
public:
	CCMLoadSTDGGrid();

// Attributes
public:
	T_ESQW_ADD_LOAD      m_ESQWData;
	T_ESQW_ADD_STOR_BASE m_ESQWBase;
 
	BOOL   m_bModified;
	ROWCOL m_nOldRow;

// Operations
public:
	void Initialize(CDBDoc* pDoc);
	void InitESQWData();
	void AddESQWData(ROWCOL nRow);
	void EnableGrid(BOOL bFlag);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMLoadSTDGGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCMLoadSTDGGrid();

	// Generated message map functions
protected:
	CDBDoc* m_pDoc;
	CString m_strOldStoryName;
	BOOL    m_bRefreshSubGrid;

	void ModifyESQWData(int nIndex);
	BOOL DeleteESQWData(ROWCOL nRow,BOOL bEndStep=TRUE);

	void SetHeaderTitle();
	void InitStoryData(ROWCOL nRow);  
	void InitStoryData(ROWCOL nRow, T_STOR_K nStorKey);
	void InitAllStoryData();
	void OperateDataMain(ROWCOL nRow, ROWCOL nCol);
	BOOL CheckValidRow();
	BOOL CheckField(BOOL bFlag);
	BOOL OnDeleteRecord(ROWCOL nRow); 
	BOOL OnDeleteRecords(CRowColArray &awRows);
	CString GetComboStoryName(int nStoryKey);
	const CString& GetValueRowCol(ROWCOL nRow, ROWCOL nCol);

	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual void OnInitCurrentCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnInsertEmptyRecord(ROWCOL nRow);
	virtual BOOL DoLButtonDblClk(UINT flags, CPoint point);
	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt=1, UINT flags=0);

	//{{AFX_MSG(CCMLoadSTDGGrid)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMLOADSTDGGRID_H__D6CD7293_D17F_4297_95A3_A32E74672DB0__INCLUDED_)
