#if !defined(__CMSECTIONSTIFFENERGRID_H__)
#define __CMSECTIONSTIFFENERGRID_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectStiffenerGrid.h : header file
//

#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\CMBaseGrid.h"
/////////////////////////////////////////////////////////////////////////////
// CCMSectStiffenerGrid window

class CCMSectStiffenerDlg;
class CCMSectStiffenerGrid : public CCMBaseGrid
{

// Construction
public:
	CCMSectStiffenerGrid();
	virtual ~CCMSectStiffenerGrid();

// Operations
public:
	void Initialize(CCMSectStiffenerDlg *pParent, int nStiffPos);
	void InitStiffenerCol(CArray<T_STIFF_SHAPE_D, T_STIFF_SHAPE_D&> &aShape);
	void SetData2Grid(CArray<T_STIFF_SHAPE_D, T_STIFF_SHAPE_D&> &aShape, CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&> &aStiffener, int nNum);
	BOOL GetGrid2Data(CArray<T_STIFF_SHAPE_D, T_STIFF_SHAPE_D&> &aShape, CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&> &aStiffener);
	BOOL EnableDiableGrid(BOOL bEnable);

	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags);
	BOOL SetRightValidCell (ROWCOL nRow, ROWCOL nCol);
	BOOL SetLeftValidCell  (ROWCOL nRow, ROWCOL nCol);

	CCMSectStiffenerDlg* m_pParent;
	int m_nStiffPos;
	BOOL m_bSetCalc;
	int  m_nRow;

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIehpYieldPropGrid)
	//}}AFX_VIRTUAL

protected:
	virtual BOOL GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt = gxCopy, int nType = 0);
	virtual BOOL OnStartEditing(ROWCOL nRow, ROWCOL nCol);
	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnEndEditing(ROWCOL nRow, ROWCOL nCol);
// 	virtual BOOL OnLButtonClickedRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt);
// 	virtual BOOL OnLButtonDblClkRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt);
	virtual BOOL CanPaste() { return FALSE; }
	virtual BOOL CanCut() { return FALSE; }

	
protected:
	void SetDataToRow(ROWCOL nRow, CStringArray& raValue);
	BOOL GetRowToData(ROWCOL nRow, CStringArray& raValue);
	
	CString GetEditingValue(ROWCOL nRow, ROWCOL nCol);

	// Generated message map functions
	
	//{{AFX_MSG(CCMSectStiffenerGrid)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSECTIONSTIFFENERGRID_H__)
