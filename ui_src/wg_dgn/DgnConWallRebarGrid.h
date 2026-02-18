// DgnConWallRebarGrid.h: interface for the CDgnConWallRebarGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__DGNCONWALLREBARGRID_H__)
#define __DGNCONWALLREBARGRID_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\wg_base_NotifyCtrl.h"

struct T_REBW_D;
class CDgnConWallNewDlg;
class CUnitCtrl;

class CDgnConWallRebarGrid : public CGXGridWnd
{
public:
	CDgnConWallRebarGrid();
//	virtual ~CDgnConWallRebarGrid();

	// Interface
public:
	void Initialize(CWnd* pParent);
	void EnableGrid(int nRow, int nCol, BOOL bEnable=TRUE);
	void SetGridWndSize(int nWidth, int nHeight)  {   m_nWndWidth = nWidth;  m_nWndHeight = nHeight;   }

	void Data2Grid(T_REBW_D* pData, BOOL bSelectedWall=TRUE);
	void Grid2Data(T_REBW_D* pData);

	void UpdateUnit();
	// override 
public:
	virtual BOOL Copy();
	virtual BOOL Paste();
	virtual BOOL OnPasteFromClipboard(const CGXRange &range);
	virtual BOOL GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt = gxCopy, int nType = 0);
	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual void OnCanceledEditing(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnLButtonClickedRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt);
	virtual BOOL OnEndEditing(ROWCOL nRow, ROWCOL nCol);

	virtual ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags);

	// Operation
protected:
	void SetEnableByRcStructWallOpt();

	void SetSizeColumn();
	void SetDataColumn();

	void SetHeaderTitle();
	CString GetEditingValue(ROWCOL nRow, ROWCOL nCol);

	CString Get_ListRebar();
	CString Get_ListSpaceSubBar();

	// Member Var.
protected:
	int m_nWndWidth;
	int m_nWndHeight;

	double m_dSpace[4];     // [Ver, Hor, End, BE Hor]

	CUnitCtrl* m_pUnitCtrl;
	CDgnConWallNewDlg* m_pParent;
};

#endif // !defined(__DGNCONWALLREBARGRID_H__)