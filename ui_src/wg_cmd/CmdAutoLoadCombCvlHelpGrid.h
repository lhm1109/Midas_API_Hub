#if !defined(AFX_CMDAUTOLOADCOMBCVLHELPGRID_H__2DDC5CB2_2A19_40DD_8FFA_ED291C76D416__INCLUDED_)
#define AFX_CMDAUTOLOADCOMBCVLHELPGRID_H__2DDC5CB2_2A19_40DD_8FFA_ED291C76D416__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCmdAutoLoadCombCvlHelpDlg;

class CCmdAutoLoadCombCvlHelpGrid : public CGXGridWnd 
{
public:
	CCmdAutoLoadCombCvlHelpGrid(CCmdAutoLoadCombCvlHelpDlg* pParent);
	virtual ~CCmdAutoLoadCombCvlHelpGrid();

// Operations
public:
	void Initialize();
	void SetText();

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCmdAutoLoadCombCvlHelpGrid)
	//}}AFX_VIRTUAL

protected:
	virtual BOOL GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt = gxCopy, int nType = 0);
	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnEndEditing(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL CanPaste() { return FALSE; }
	virtual BOOL CanCut() { return FALSE; }

protected:
	void SetHeaderTitle();
	void SetDataToRow(ROWCOL nRow, CStringArray& raValue);
	BOOL GetRowToData(ROWCOL nRow, CStringArray& raValue);
	
	CString GetEditingValue(ROWCOL nRow, ROWCOL nCol);

protected:
	BOOL m_bOnEditing;

	// Generated message map functions
protected:
	//{{AFX_MSG(CCmdAutoLoadCombCvlHelpGrid)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CCmdAutoLoadCombCvlHelpDlg* m_pParent;

public:
	void UpdateBearingData();

};

#endif // !defined(AFX_CMDAUTOLOADCOMBCVLHELPGRID_H__2DDC5CB2_2A19_40DD_8FFA_ED291C76D416__INCLUDED_)
