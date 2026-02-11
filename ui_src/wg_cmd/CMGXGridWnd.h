#if !defined(__CMGXGridWnd_H__)
#define __CMGXGridWnd_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMGXGridWnd.h : header file
//

#include "HeaderPre.h"
/////////////////////////////////////////////////////////////////////////////
// CCMGXGridWnd dialog
// CGXGridWnd의 기본적인 기능에 Copy 기능 추가 + Insert/Delete Key 기능 막음.

class  __MY_EXT_CLASS__ CCMGXGridWnd : public CGXGridWnd
{
public:
	CCMGXGridWnd() {}
	virtual ~CCMGXGridWnd() {}

public:
	void Initialize();

	virtual BOOL Copy();
	virtual BOOL OnPasteFromClipboard(const CGXRange &range);

	virtual ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags);

	void SetColWidthByDPI(ROWCOL nColSt, ROWCOL nColEd, int nWidth, int* anWidthArray = NULL, UINT flags = GX_UPDATENOW, GXCmdType ctCmd = gxDo);
	void SetRowHeightByDPI(ROWCOL nRowSt, ROWCOL nRowEd, int nHeight, int* anHeightArray = NULL, UINT flags = GX_UPDATENOW, GXCmdType ctCmd = gxDo);
};

/////////////////////////////////////////////////////////////////////////////
// CCMGXGridWnd2 dialog
// CGXGridWnd의 기본적인 기능에 Copy + Color + Utility 

class  __MY_EXT_CLASS__ CCMGXGridWnd2 : public CCMGXGridWnd
{
public:
	CCMGXGridWnd2() {}
	virtual ~CCMGXGridWnd2() {}

public:
	virtual BOOL GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt = gxCopy, int nType = 0);

	void SetColWidthByDPI(ROWCOL nColSt, ROWCOL nColEd, int nWidth, int* anWidthArray = NULL, UINT flags = GX_UPDATENOW, GXCmdType ctCmd = gxDo);
	void SetRowHeightByDPI(ROWCOL nRowSt, ROWCOL nRowEd, int nHeight, int* anHeightArray = NULL, UINT flags = GX_UPDATENOW, GXCmdType ctCmd = gxDo);

	// utility - check data 
	BOOL IsValidLongNumber(CString value, int &nErrCode);
	BOOL IsValidDoubleNumber(CString value, int &nErrCode);
	BOOL ConvertFormulaToValue(CString& value);
	BOOL IsNumeric(CString str, int& ecode);
	BOOL ValidateError(LPCTSTR lpszError);
	BOOL InvalidLongNumber(int nErrCode);
	BOOL InvalidDoubleNumber(int nErrCode);
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMGXGridWnd_H__)
