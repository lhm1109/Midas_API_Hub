#if !defined(AFX_BNDRELNKGRID_H__2DDC5CB2_2A19_40DD_8FFA_ED291C76D416__INCLUDED_)
#define AFX_BNDRELNKGRID_H__2DDC5CB2_2A19_40DD_8FFA_ED291C76D416__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBndrElnkDlg;

class CBndrElnkGrid : public CGXGridWnd, public CDBUpdateConnector
{
public:
	CBndrElnkGrid(CBndrElnkDlg* pParent);
	//virtual ~CBndrElnkGrid();

// Operations
public:
	void Initialize();
	void SetData2Grid(int *nSymNum);
	BOOL GetGrid2Data(double dDistance[10], double dRadian[10], double dForce[10], double dMoment[10]);
	void OnChangeSymmetric(int nIndex);
	void OnChangeDirecion(int nIndex);

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBndrElnkGrid)
	//}}AFX_VIRTUAL

// Implementation
	virtual	void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void ConvertCoeffUnit();
	virtual ~CBndrElnkGrid();
//   void ConvertCoeffUnit();

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
	int m_nDirection;

	// Generated message map functions
protected:
	//{{AFX_MSG(CBndrElnkGrid)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CBndrElnkDlg* m_pParent;

public:
	void UpdateBearingData();

};

#endif // !defined(AFX_BNDRELNKGRID_H__2DDC5CB2_2A19_40DD_8FFA_ED291C76D416__INCLUDED_)
