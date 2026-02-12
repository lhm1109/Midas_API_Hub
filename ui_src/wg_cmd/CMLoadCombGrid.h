#if !defined(AFX_CMLOADCOMBGRID_H__079B2D41_1B54_11D4_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_CMLOADCOMBGRID_H__079B2D41_1B54_11D4_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMLoadCombGrid.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_common\wg_common_TBBrowserWnd.h"
class CCMLoadCombDlg;

struct SLcomColItem
{
	SLcomColItem() {};
	SLcomColItem(LPCTSTR name, UINT lcType, UINT key)
	{ csName = name; nLCtype = lcType; nKey = key; }
	CString csName;
	UINT    nLCtype;
	UINT    nKey;
};

/////////////////////////////////////////////////////////////////////////////
// CCMLoadCombGrid window
// Browser Style의 윈도우, 가로, 세로 방향으로 늘어난다.
// Column 이동이 금지된다.
class CCMLoadCombGrid : public CTBBrowserWnd
{
// Construction
public:
	CCMLoadCombGrid();

//----------------------------------------------------------
// Overriadable
	virtual BOOL OnValidateField(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnValidateRecord(ROWCOL nRow);
	virtual BOOL OnAddNewRecord(ROWCOL nRow);
	virtual BOOL OnModifyRecord(ROWCOL nRow);
	virtual BOOL OnDeleteRecord(ROWCOL nRow);
	virtual BOOL OnDeleteRecords(CRowColArray &awRows);

	virtual BOOL ValidateField(CString& value, int nColID);
	virtual BOOL ValidateRecord(CStringArray &aValue, CRowColArray &aCols);

	virtual BOOL OnInsertEmptyRecord(ROWCOL nRow);

//----------------------------------------------------------
// Grid's Standard Overriadable Functions
	virtual void OnChangedSelection(const CGXRange *changedRect, BOOL bIsDraggin, BOOL bKey);
	virtual BOOL OnSelDragRowsDrop(ROWCOL nFirstRow, ROWCOL nLastRow, ROWCOL nDestRow);
	virtual BOOL OnPasteFromClipboard(const CGXRange &range);

	BOOL PasteFromClipboard(const CGXRange &range, CString* aDefVal, int nKeyCol, 
													CGXGridWnd &x, int &nPasteMode, int &nPastedCols);
	virtual BOOL Copy();

//----------------------------------------------------------
// Interface
public:
	void Initialize(CDBDoc* pDoc);
	void SetLcomType(int nLcomType);
	void ChangeLcomType(int nLcomType);
	void MakeItemEx();
	void UpdateBuffer();
	void OnStageChanged();
	CString GetKeyString(ROWCOL nRow);

	BOOL  GetColumnNoFor(int nID, ROWCOL &nCol) { nCol = nID+1; return TRUE; }
	BOOL  GetColumnIDFor(ROWCOL nCol, int &nID) { nID = nCol-1; return TRUE; }


//----------------------------------------------------------
// Implementation
public:
	void SetColInfo();
protected:
	void SetValue(ROWCOL nRow, T_LCOM_K key, T_LCOM_D& lcom);
	void SetHeaderTitle();
	void ConvDataToStrArray(T_LCOM_D &lcom, CStringArray &aValue);
	void SetLcomCaseValue(CStringArray &aValue, T_LCOM_BASE &base);

	void UpdateLoadCase(BOOL& bDoMakeItemEx);
	BOOL InsertRecordByKey(T_LCOM_K Key, T_LCOM_D &Data);
	BOOL DeleteRecordByKey(T_LCOM_K Key, T_LCOM_D &Data);
	BOOL InsertColumn(CString &csName, int nLCtype, T_LCOM_K nKey, int nLCid);
	BOOL DeleteColumn(int nLCtype, T_LCOM_K nKey);

	void SelectGridRow(long* aRow, long nSize);

	BOOL GetValue(ROWCOL nRow, T_LCOM_D &lcom);
	BOOL MakeDataArray(CStringArray& value, CRowColArray& awCols, CArray<T_LCOM_D, T_LCOM_D&>& aData);
	BOOL ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, T_LCOM_D &data);
	BOOL ConvStrArrayToData(CStringArray &aValue, T_LCOM_D& data);

	void SetEnableDisableLcomCol();

	CString GetDefValue(int ix);

protected:
	CDBDoc* m_pDoc;
	int m_nLcomType;
	int m_nLCtype;
	CString m_csPostfix;
	CArray<SLcomColItem, SLcomColItem&> m_aColInfo;

	int m_nFixedCol;
	int m_nStldCount;
	int m_nSplcCount;
	int m_nThisCount;
	int m_nMvldCount;
	int m_nSmlcCount;
	int m_nSgldCount;
	int m_nEspcCount;
	int m_nGilcCount;
	int m_nCranCount;
	int m_nNspcCount; // Non-Dissipative 응답 스펙트럼
	int m_nNespCount; // Non-Dissipative 우발편심 응답 스펙트럼
	int m_nLcomCount;

	BOOL m_bElastStag;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMLoadCombGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCMLoadCombGrid();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCMLoadCombGrid)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CCMLoadCombDlg* m_pParentDlg;
	BOOL IsDisplayMode();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMLOADCOMBGRID_H__079B2D41_1B54_11D4_92DE_0000C0B0E6B3__INCLUDED_)
