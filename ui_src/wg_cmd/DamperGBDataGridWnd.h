////////////// CODE BY ZINU

#if !defined(__ACS_DAMPERGBDATAGRIDWND_H__)
#define __ACS_DAMPERGBDATAGRIDWND_H__

#if _MSC_VER > 1000
#pragma once
#endif 

#define D_START_ROW 1
#define IsSHIFTpressed() ( (GetKeyState(VK_SHIFT) & (1 << (sizeof(SHORT)*8-1))) != 0 ) 

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\DamperGBDBMgr.h"
#include "..\wg_common\TBBrowserWndEx.h"
/////////////////////////////////////////////////////////////////////////////
// DamperGBDataGridWnd window

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDamperGBDataGridWnd : public CTBBrowserWndEx
{
	// Construction
public:
	CDamperGBDataGridWnd();
	virtual ~CDamperGBDataGridWnd();
	// Attributes

public:
	void Initialize(int nCode, int nPropType, int nCategory, CString strInitName, int nRow);
	void Data2Grid();
	void Grid2Data(T_DMGB_D &DmgbD);
	void SetHeaderTitle_Code(int nPropType);
	void SetHeaderTitle_User(int nPropType);
	void SetTitle_Code(int nPropType);
	void SetTitle_User(int nPropType);
	void GetTitleUnitName(CString &strUnit_L, CString &strUnit_F);
	BOOL SaveUserDefineData2Ini(CString strClassify);

protected:
	//{{AFX_MSG(ACS_GB17ResultGridWnd)
	BOOL SaveData(CString strClassify);
	void GetItemNameKey();
	void SetNewLineNo(ROWCOL nRow, ROWCOL nCol);
	CString GetEditingValue(ROWCOL nRow, ROWCOL nCol);
	void Write_Cell(ROWCOL nRow, ROWCOL nCol, CGXStyle& rstyle, const CString& strValue);
	void Write_Cell(ROWCOL nRow, ROWCOL nCol, CGXStyle& rstyle, const TCHAR* Fmt, ...);
	void Write_CellMerge(ROWCOL nRow1, ROWCOL nCol1, ROWCOL nRow2, ROWCOL nCol2, CGXStyle& rstyle, const CString& strValue);
	void Write_CellMerge(ROWCOL nRow1, ROWCOL nCol1, ROWCOL nRow2, ROWCOL nCol2, CGXStyle& rstyle, const TCHAR* Fmt, ...);

	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnPasteFromClipboard(const CGXRange& range);
	virtual BOOL OnDeleteRecords(CRowColArray& awRows);

	virtual BOOL OnLButtonDblClkRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt);

protected:
	CDBDoc*		m_pDoc;
	int m_nRow;
	int m_nCol;
	int m_nPropType;
	int m_nCategory;
	int m_nCode;
	BOOL m_bCode;
	T_DMGB_D DmgbD;
	T_DMGB_CODE m_Code;	//code
	T_DMGB_CODE m_User; //user
	ROWCOL m_nCurRow;
	CString m_strClassify;
	CArray<CString, CString&> m_aNameKey;
	CStringArray m_astrInputType;
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////


#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // 

