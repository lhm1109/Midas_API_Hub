#if !defined(__SEISCVLJPSETINVESTMETHODTIMEGRID_H__)
#define __SEISCVLJPSETINVESTMETHODTIMEGRID_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MeshRatioList.h : header file
//

#include "..\wg_common\wg_common_TBBrowserWnd.h"

/////////////////////////////////////////////////////////////////////////////

// CSeisCvlJpSetInvestMethodTimeGrid window
#include "HeaderPre.h"

class CSeisCvlJpSetInvestMethodTimeGrid :  public CTBBrowserWnd
{
public:
// Construction
public:
	CSeisCvlJpSetInvestMethodTimeGrid();
	virtual ~CSeisCvlJpSetInvestMethodTimeGrid();

public:
	void Initialize();
	void SetRow(const CArray< CString, CString>& aCase);
	void GetRow(CArray< CString, CString>& aCase, CArray<double, double>& aStart, CArray<double, double>& aEnd);
	void EnableControl(const BOOL& bEnable);
private:
	//virtual BOOL GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt = gxCopy, int nType = 0);
	//virtual BOOL OnStartEditing(ROWCOL nRow, ROWCOL nCol);
	//virtual BOOL OnEndEditing(ROWCOL nRow, ROWCOL nCol);
	////virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	//virtual BOOL ProcessKeys(CWnd* pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags);
	//virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()

};
#include "HeaderPost.h"

#endif // !defined(__CMGRIDLDLNGRID_H__)
