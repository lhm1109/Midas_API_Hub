// DgnConBraceRebarGrid.h: interface for the CDgnConBraceRebarGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__DGNCONBRACEREBARGRID_H__)
#define __DGNCONBRACEREBARGRID_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\wg_base_NotifyCtrl.h"

struct T_REBR_D;
class CDgnConBraceNewDlg;
class CUnitCtrl;

class CDgnConBraceRebarGrid : public CGXGridWnd
{
public:
	CDgnConBraceRebarGrid();
//	virtual ~CDgnConBraceRebarGrid();

	// Interface
public:
	void Initialize(CWnd* pParent);
	void EnableGrid(int nRow, int nCol, BOOL bEnable=TRUE);
	void SetGridWndSize(int nWidth, int nHeight)  {   m_nWndWidth = nWidth;  m_nWndHeight = nHeight;   }

	void Data2Grid(T_REBR_D* pData, CString* pStrShape=NULL);
	void Grid2Data(T_REBR_D* pData);

	void UpdateUnit();
	// override 
public:
	virtual BOOL Copy();
	virtual BOOL Paste();
	virtual BOOL OnPasteFromClipboard(const CGXRange &range);
	virtual BOOL GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt = gxCopy, int nType = 0);
	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual void OnCanceledEditing(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnEndEditing(ROWCOL nRow, ROWCOL nCol);
	virtual ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags);

	// Operation
protected:
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

	CString m_strShape;

	double m_dSpace[2];     // [End, Center]

	CUnitCtrl* m_pUnitCtrl;
	CDgnConBraceNewDlg* m_pParent;
};

#endif // !defined(__DGNCONBRACEREBARGRID_H__)