// DgnConBeamRebarGrid.h: interface for the CDgnConBeamRebarGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__DGNCONBEAMREBARGRID_H__)
#define __DGNCONBEAMREBARGRID_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\wg_base_NotifyCtrl.h"

struct T_REBB_D;
struct T_REBB_BASE;
class CDgnConBeamNewDlg;
class CUnitCtrl;

class CDgnConBeamRebarGrid : public CGXGridWnd
{
public:
	CDgnConBeamRebarGrid();
//	virtual ~CDgnConBeamRebarGrid();

	// Interface
public:
	void Initialize(CWnd* pParent, int nArrangeType=2);
	void EnableGrid(int nRow, int nCol, BOOL bEnable=TRUE);
	void SetGridWndSize(int nWidth, int nHeight)  {   m_nWndWidth = nWidth;  m_nWndHeight = nHeight;   }
	void SetSameSizeSetting(BOOL bSameTopBot, BOOL bSameIMJ, BOOL bSameLayer);

	void Data2Grid(T_REBB_D* pData);
	void Grid2Data(T_REBB_D* pData);

		BOOL UseSkin();

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
	void SetSizeColumn(int nArrangeType);
	void SetDataColumn(int nArrangeType);
	void SetDataColumnSection(ROWCOL nCol);

	int GetColumnNum();
	void SetHeaderTitle(int nArrangeType);
	void ChangeStatusColumn(ROWCOL nRow, ROWCOL nCol);
	CString GetEditingValue(ROWCOL nRow, ROWCOL nCol);

	void Data2GridSection(ROWCOL nCol, T_REBB_BASE* pTop, T_REBB_BASE* pBot);
	void Grid2Data_Section(ROWCOL nCol, T_REBB_BASE* pTop, T_REBB_BASE* pBot);

	CString Get_ListRebar();
	CString Get_ListSpaceSubBar();

	// Member Var.
protected:
	BOOL m_bSameTopBot;
	BOOL m_bSameIMJ;
	BOOL m_bSameLayer; // Add/Modify by Seungjun PMS:4496  '20120613 RC 보/기둥 철근종류 여러 개 지정 (코너철근 등, 설계없이 검토만)
	int m_nArrangeType;
	int m_nWndWidth;
	int m_nWndHeight;
	
	double m_dSpace[3];     // [I,M,J]

	CUnitCtrl* m_pUnitCtrl;
	CDgnConBeamNewDlg* m_pParent;
};

#endif // !defined(__DGNCONBEAMREBARGRID_H__)