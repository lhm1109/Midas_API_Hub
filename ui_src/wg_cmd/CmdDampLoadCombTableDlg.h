#pragma once
#include "..\wg_db\wg_db_DBDoc.h"
#include "CmdIsolatorGBStressLmtGridWnd.h"

class CCmdDampLoadCombGridWnd : public CTBBrowserWndEx
{
	// Construction
public:
	CCmdDampLoadCombGridWnd();
	virtual ~CCmdDampLoadCombGridWnd();
	// Attributes

public:
	void Initialize();
	void Data2Grid();
	//void Grid2Data();
	void SetHeaderTitle();
	void SetTitle();
	void GetTitleUnitName(CString &strUnit_L, CString &strUnit_F);

protected:
	//{{AFX_MSG(ACS_GB17ResultGridWnd)
	CString GetEditingValue(ROWCOL nRow, ROWCOL nCol);
	void Write_Cell(ROWCOL nRow, ROWCOL nCol, CGXStyle& rstyle, const CString& strValue);
	void Write_Cell(ROWCOL nRow, ROWCOL nCol, CGXStyle& rstyle, const TCHAR* Fmt, ...);
	void Write_CellMerge(ROWCOL nRow1, ROWCOL nCol1, ROWCOL nRow2, ROWCOL nCol2, CGXStyle& rstyle, const CString& strValue);
	void Write_CellMerge(ROWCOL nRow1, ROWCOL nCol1, ROWCOL nRow2, ROWCOL nCol2, CGXStyle& rstyle, const TCHAR* Fmt, ...);

	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	//virtual BOOL OnPasteFromClipboard(const CGXRange &range);
	//virtual BOOL OnLButtonDblClkRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt);
public:
	T_DPCI_D *m_pDpciD;
protected:
	CDBDoc*		m_pDoc;
	int m_nRow;
	int m_nCol;
	BOOL m_bCode;
	ROWCOL m_nCurRow;
	CArray<T_LCOM_D, T_LCOM_D &> m_LcomD;
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class CCmdDampLoadCombTableDlg : public CDialogMove
{
	// Construction
public:
	CCmdDampLoadCombTableDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCmdDampLoadCombTableDlg();

	CCmdDampLoadCombGridWnd	m_wndGrid;
	CEdit m_Classify;
	int m_nType;
public:
	BOOL SetParamData(const T_DPCI_D& ParamData);
	BOOL GetParamData(T_DPCI_D& ParamData);

protected:
	CDBDoc* m_pDoc;
	enum { IDD = IDD_CMD_DAMP_LOADCOM_TABLE_DLG };
	CListCtrl	m_List;
	T_DPCI_D m_DpciD;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCmdDampLoadCombTableDlg)
	afx_msg void OnCmdBtnOK();
	afx_msg void OnCmdBtnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
