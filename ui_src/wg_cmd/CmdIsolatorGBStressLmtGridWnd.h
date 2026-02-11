
#if _MSC_VER > 1000
#pragma once
#endif 

#define D_START_ROW 1
#define IsSHIFTpressed() ( (GetKeyState(VK_SHIFT) & (1 << (sizeof(SHORT)*8-1))) != 0 ) 

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_common\TBBrowserWndEx.h"
/////////////////////////////////////////////////////////////////////////////
// AntivibrationSupportGBDataGridWnd window

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CCmdIsolatorGBStressLmtGridWnd : public CTBBrowserWndEx
{
	// Construction
public:
	CCmdIsolatorGBStressLmtGridWnd();
	virtual ~CCmdIsolatorGBStressLmtGridWnd();
	// Attributes

public:
	void Initialize();
	void SetData(T_ISGB_D  *pdata);
	void Data2Grid();
	void Grid2Data();
	void SetHeaderTitle();
	void SetTitle();
	void GetTitleUnitName(CString &strUnit_L, CString &strUnit_F);

protected:
	//{{AFX_MSG(ACS_GB17ResultGridWnd)
	CString GetEditingValue(ROWCOL nRow, ROWCOL nCol);
	void Write_Cell(ROWCOL nRow, ROWCOL nCol, CGXStyle& rstyle, const CString& strValue);
	void Write_Cell(ROWCOL nRow, ROWCOL nCol, CGXStyle& rstyle, const TCHAR* Fmt, ...);
	void Write_CellMerge(ROWCOL nRow1, ROWCOL nCol1, ROWCOL nRow2, ROWCOL nCol2, CGXStyle& rstyle, const CString& strValue);
	void Write_CellMerge(ROWCOL nRow1, ROWCOL nCol1, ROWCOL nRow2, ROWCOL nCol2, CGXStyle& rstyle, const TCHAR* Fmt, ...);

	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	//virtual BOOL OnPasteFromClipboard(const CGXRange &range);
	//virtual BOOL OnLButtonDblClkRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt);
public:
	T_ISGB_D *pIsgbD;
protected:
	CDBDoc*		m_pDoc;
	int m_nRow;
	int m_nCol;
	BOOL m_bCode;
	ROWCOL m_nCurRow;
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////


#include "HeaderPost.h"


