#if !defined(__CMSPECGRID_H__)
#define __CMSPECGRID_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSpecGrid.h : header file
//

#include "CMGXGridWnd.h"
#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CCMSpecGrid window

class CDBDoc;
class CCMSpecGrid : public CCMGXGridWnd
{
// Construction
public:
	CCMSpecGrid();

// Attributes
public:
	
// Operations
public:
	void Initialize();

	void Data2Grid(CArray<T_SPLC_ECCEN, T_SPLC_ECCEN&>& rSplcEccen, CArray<T_SPLC_ECCEN, T_SPLC_ECCEN&>& rSplcEccen_AL);
	BOOL Grid2Data(CArray<T_SPLC_ECCEN, T_SPLC_ECCEN&>& rSplcEccen, CArray<T_SPLC_ECCEN, T_SPLC_ECCEN&>& rSplcEccen_AL);

	void EnableGrid(BOOL bEnable, BOOL bUseGL);


	/*
	void Data2Grid(CArray<T_SEIS_ECCEN, T_SEIS_ECCEN&>* paSeisEccen);
	BOOL Grid2Data(CArray<T_SEIS_ECCEN, T_SEIS_ECCEN&>* paSeisEccen);    
	*/

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSpecGrid)
	//}}AFX_VIRTUAL

protected:
	//BOOL m_bOnStartEditing;  
	//UINT m_nCurMdulKey;

// Implementation
public:
	virtual ~CCMSpecGrid();

protected:
	virtual BOOL GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt = gxCopy, int nType = 0);
	/*
	virtual BOOL OnStartEditing(ROWCOL nRow, ROWCOL nCol);
	virtual void OnCanceledEditing(ROWCOL nRow, ROWCOL nCol);
	*/

protected:
	void SetHeaderTitle();  
	int  GetColNumber();

	//CString GetEditingValue(ROWCOL nRow, ROWCOL nCol);  
	/*
	void SetDataToRow(ROWCOL nRow, CStringArray& raValue);
	BOOL GetRowToData(ROWCOL nRow, CStringArray& raValue);  
	BOOL GetDefaultValue(ROWCOL nRow, CStringArray& rValue);
	*/

	// Generated message map functions
protected:
	//{{AFX_MSG(CCMSpecGrid)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CDBDoc* m_pDoc;
	BOOL m_bEnable;
	//CMDBCtrl* m_pMDBCtrl;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSPECGRID_H__)
