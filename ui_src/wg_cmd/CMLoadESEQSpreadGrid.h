#if !defined(AFX_CMLOADESEQSPREADGRID_H__07FC5981_7F40_11D4_A678_00010263A1CE__INCLUDED_)
#define AFX_CMLOADESEQSPREADGRID_H__07FC5981_7F40_11D4_A678_00010263A1CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMLoadESEQSpreadGrid.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\DB_ST_DT.h"
#include "CMLoadESEQTBGrid.h"
/////////////////////////////////////////////////////////////////////////////
// CCMLoadESEQSpreadGrid window

class CCMLoadESEQMainGrid;

class CCMLoadESEQSpreadGrid : public CCMLoadESEQTBGrid
{
// Construction
public:
	CCMLoadESEQSpreadGrid();
	CCMLoadESEQMainGrid* m_pESEQMainGrid;

// Attributes
public:
	T_ESEQ_D m_ESEQData;

private:
	T_ESEQ_BASE m_ESEQBase;
	T_ESEQ_SEL m_ESEQSel;

// Operations
public:
	void Initialize(CDBDoc* pDoc);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMLoadESEQSpreadGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL m_bModified;
	void InitESEQData();
	BOOL SaveESEQData();

	virtual ~CCMLoadESEQSpreadGrid();

	// Generated message map functions
protected:
	CDBDoc* m_pDoc;
	CString m_strMatlName;
	CString m_strMatlType;
//	int m_nMatlCount;

	CArray<int,int> m_arMatlSteelKey;
	CArray<int,int> m_arMatlConcKey;
	CArray<int,int> m_arMatlSRCKey;
	CArray<int,int> m_arMatlUserKey;   

	ROWCOL m_nOldRow;
	BOOL m_bInsertMode;
	BOOL m_bStepModified;
	BOOL m_bPureCellChange;
	BOOL m_bSameStepData;

	void SetHeaderTitle();
	void InitStoryData();
	void InitStoryData(ROWCOL nRow);
	void InitStoryData(ROWCOL nRow, int nStoryKey);
	void ChangeStoryData(ROWCOL nRow, ROWCOL nCol);
	void InitMatlData();
	void ChangeMaterialData(ROWCOL nRow);
	void ChangeStepData(ROWCOL nRow, ROWCOL nCol);
	void ChangeEnableDisableLoadStory(ROWCOL nRow);
	void CheckStepData(ROWCOL nRow);
	void OperateDataSpread(ROWCOL nRow, ROWCOL nCol);
	BOOL CheckField();
	BOOL CheckCellEnable(ROWCOL nRow);
	BOOL CheckValidRow(ROWCOL nRow);
	BOOL CheckAllBlankRow(ROWCOL nRow);
//	void SortRowContent(ROWCOL nRow);
	void StepRenumber(ROWCOL nRow);
	void StepRenumberAll(ROWCOL nRow);
	void GetModelData(ROWCOL nRow, T_ESEQ_SEL& ESEQSel);
	CString GetComboStoryName(int nStoryKey);
	void SetStartStoryName(ROWCOL nRow);
	void DeleteInvalidStepData(ROWCOL nRow);

	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt=1, UINT flags=0);
	virtual void OnInitCurrentCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnDeleteRecords(CRowColArray &awRows);
	virtual BOOL OnDeleteRecord(ROWCOL nRow);
	virtual BOOL OnInsertEmptyRecord(ROWCOL nRow);

	const CString& GetValueRowCol(ROWCOL nRow, ROWCOL nCol);


	//{{AFX_MSG(CCMLoadESEQSpreadGrid)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMLOADESEQSPREADGRID_H__07FC5981_7F40_11D4_A678_00010263A1CE__INCLUDED_)
