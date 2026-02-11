#if !defined(__CMSECTRIVETANGLEGRID_H__)
#define __CMSECTRIVETANGLEGRID_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectRivetAngleGrid.h : header file
//

#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\CMBaseGrid.h"
/////////////////////////////////////////////////////////////////////////////
// CCMSectRivetAngleGrid window

class CCMSectRivetAngleGrid : public CCMBaseGrid
{

	// Construction
public:
	CCMSectRivetAngleGrid();
	virtual ~CCMSectRivetAngleGrid();

	// Operations
public:
	void Initialize();

	void SetData2Grid(CArray<T_SECT_RIVET_POS_D, T_SECT_RIVET_POS_D&>& aRivetPos, BOOL bSymmetric, CArray<int, int>& aAngleIndex);
	BOOL GetGrid2Data(CArray<T_SECT_RIVET_POS_D, T_SECT_RIVET_POS_D&>& aRivetPos);

	virtual BOOL ProcessKeys(CWnd* pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags);
	BOOL SetRightValidCell(ROWCOL nRow, ROWCOL nCol);
	BOOL SetLeftValidCell(ROWCOL nRow, ROWCOL nCol);

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
	BOOL m_bSymmetric;
	CMap<int , int , T_SECT_RIVET_POS_D, T_SECT_RIVET_POS_D> m_mapRowdata;
	void SetDataToRow(ROWCOL nRow, BOOL bSymRow, CStringArray& raValue);
	BOOL GetRowToData(ROWCOL nRow, CStringArray& raValue);

	void StringToData(IN CStringArray& raValue,        OUT T_SECT_RIVET_POS_D& RivetPos);
	void DataToString(BOOL bSymmetric, std::map<int, T_SECT_RIVET_POS_D>& mapUseRevet, std::map<int, T_SECT_RIVET_POS_D>& mapUseRevetW, IN T_SECT_RIVET_POS_D& RivetPos,
					  BOOL& bRightPart, OUT CStringArray& aValue);

	CString GetEditingValue(ROWCOL nRow, ROWCOL nCol);

	void SortRivetArray(CArray<T_SECT_RIVET_POS_D, T_SECT_RIVET_POS_D>& aArray);
	void ChangeColumnStatus(ROWCOL nRow, ROWCOL nCol);
	// Generated message map functions

	//{{AFX_MSG(CCMSectRivetAngleGrid)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSECTRIVETANGLEGRID_H__)
