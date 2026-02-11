#if !defined(AFX_CMLOADESEQMAINGRID_H__353BC8C2_7416_11D4_9AAF_0000C0B9C58C__INCLUDED_)
#define AFX_CMLOADESEQMAINGRID_H__353BC8C2_7416_11D4_9AAF_0000C0B9C58C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMLoadESEQMainGrid.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_common\wg_common_TBBrowserWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CCMLoadESEQMainGrid window
class CCMLoadESEQSubGrid;
class CCMStageSpreadGrid;

class CCMLoadESEQMainGrid : public CTBBrowserWnd
{
// Construction
public:
	CCMLoadESEQMainGrid();
	CCMLoadESEQSubGrid* m_pESEQSubGrid;
	CCMStageSpreadGrid* m_pSpreadGrid;

// Attributes
public:
	T_ESEQ_D m_ESEQData;

private:
	T_ESEQ_BASE m_ESEQBase;
	T_ESEQ_SEL m_ESEQSel;

// Operations
public:
	void Initialize(CDBDoc* pDoc);
	void InitESEQData();
	BOOL CheckField(BOOL bFlag=TRUE);
	BOOL CheckValidRow();
	void AddESEQData(ROWCOL nRow);
	ROWCOL m_nOldRow;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMLoadESEQMainGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCMLoadESEQMainGrid();
	BOOL m_bModified;

protected:
	CDBDoc* m_pDoc;
	CString m_strOldStoryName;

	BOOL m_bRefreshSubGrid;  // Record가 삭제 될때 Sub그리드에 관련 항목이 뿌려진다. 이를 방지하지 위해서..

	void SetHeaderTitle();
	void InitStoryData(ROWCOL nRow);
	void InitStoryData(ROWCOL nRow, T_STOR_K nStorKey);
	void OperateDataMain(ROWCOL nRow, ROWCOL nCol);
	void GridRenumber();
	void ModifyESEQData(int nIndex);
	void InitAllStoryData();
	void DeleteInvalidStepData(ROWCOL nRow);
	CString GetComboStoryName(int nStoryKey);
	BOOL DeleteESEQData(ROWCOL nRow, BOOL bEndStep=TRUE);
	void RefreshESEQData(ROWCOL nRow);

	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual void OnInitCurrentCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL DoLButtonDblClk(UINT flags, CPoint point);
	virtual BOOL OnDeleteRecord(ROWCOL nRow);
	virtual BOOL OnDeleteRecords(CRowColArray &awRows);
	const CString& GetValueRowCol(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnInsertEmptyRecord(ROWCOL nRow);
	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt=1, UINT flags=0);

protected:
	//{{AFX_MSG(CCMLoadESEQMainGrid)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMLOADESEQMAINGRID_H__353BC8C2_7416_11D4_9AAF_0000C0B9C58C__INCLUDED_)
