#if !defined(__CMSECTSODDECKSTIFFENERGRID_H__)
#define __CMSECTSODDECKSTIFFENERGRID_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectSODDeckStiffenerGrid.h : header file
//

#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\CMBaseGrid.h"
/////////////////////////////////////////////////////////////////////////////
// CCMSectSODDeckStiffenerGrid window

class CCMSectSODDeckStiffenerDlg;
class CCMSectSODDeckStiffenerGrid : public CCMBaseGrid
{

// Construction
public:
	CCMSectSODDeckStiffenerGrid();
	virtual ~CCMSectSODDeckStiffenerGrid();

// Operations
public:
	void Initialize(T_SECT_D& SectD, CCMSectSODDeckStiffenerDlg *pParent, int nSectType);
	void InitStiffenerCol(CArray<T_STIFF_SHAPE_D, T_STIFF_SHAPE_D&> &aShape);	    
	void InitStiffenerPosCol(T_SECT_D& SectD, int nDeckPos, int nDeckPart, int nDeckPartCnt);
	void InitStiffenerDir(T_SECT_D& SectD, int nDeckPos);
	void SetData2Grid(CArray<T_STIFF_SHAPE_D, T_STIFF_SHAPE_D&> &aShape, CArray<T_SECT_SOD_STIFF_DATA_D, T_SECT_SOD_STIFF_DATA_D&> &aStiffener, int nDeckPos, int nNum);
	BOOL GetGrid2Data(CArray<T_STIFF_SHAPE_D, T_STIFF_SHAPE_D&> &aShape, CArray<T_SECT_SOD_STIFF_DATA_D, T_SECT_SOD_STIFF_DATA_D&> &aStiffener);

	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags);
	BOOL SetRightValidCell (ROWCOL nRow, ROWCOL nCol);
	BOOL SetLeftValidCell  (ROWCOL nRow, ROWCOL nCol);

	CCMSectSODDeckStiffenerDlg* m_pParent;
	
	int  m_nSectType;
	int  m_COL_NUM;
	int  m_nDeckPos;
	CArray<T_STIFF_SHAPE_D, T_STIFF_SHAPE_D&> m_aShape;
	CArray<T_SECT_SOD_STIFF_DATA_D, T_SECT_SOD_STIFF_DATA_D&> m_aStiffener;

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
	virtual BOOL CanPaste() { return FALSE; }
	virtual BOOL CanCut() { return FALSE; }

protected:
	void SetDataToRow(ROWCOL nRow, CStringArray& raValue);
	BOOL GetRowToData(ROWCOL nRow, CStringArray& raValue);
	
	CString GetEditingValue(ROWCOL nRow, ROWCOL nCol);

	// Generated message map functions
	
	//{{AFX_MSG(CCMSectSODDeckStiffenerGrid)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSECTSODDECKSTIFFENERGRID_H__)
