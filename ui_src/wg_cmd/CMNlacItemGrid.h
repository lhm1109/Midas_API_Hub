// CMNlacItemGrid.h: interface for the CCMNlacItemGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__CMNLACITEMGRID_H__)
#define __CMNLACITEMGRID_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCMNlacItemGrid : public CGXGridWnd
{
public:
	CCMNlacItemGrid();
	virtual ~CCMNlacItemGrid();

public:
	void Initialize();
	void SetHeaderTitle();
	void SetRowCountUser(int nRowCount);
	void SetDefaultValue(ROWCOL nFromRow, ROWCOL nToRow);
	void SetValueRow(ROWCOL nRow, double dValue);
	double GetValueRow(ROWCOL nRow);
	void GenerateValue(double dValue);

	void SetOption(int nOption) { m_nOption = nOption; } // 0 : Load Factor, 1 : Master Node Displacement  
	void SetMaxDispValue(double dMaxDisp) { m_dMaxDisp = dMaxDisp; }

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMNlacItemGrid)
	//}}AFX_VIRTUAL

protected:
	int m_nOption;
	double m_dMaxDisp;
	int m_nRowCount;
	int m_nOldRowCount;

protected:
	virtual BOOL GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt = gxCopy, int nType = 0);
	virtual BOOL CanPaste() { return FALSE; }
	virtual BOOL CanCut() { return FALSE; }

// Generated message map functions
protected:
	//{{AFX_MSG(CCMNlacItemGrid)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

#endif // !defined(__CMNLACITEMGRID_H__)
