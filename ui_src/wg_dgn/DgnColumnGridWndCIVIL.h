#if !defined(AFX_DGNCOLUMNGRIDWNDCIVIL_H__3E50C3FE_1757_4BCA_AA56_E64B1E735FEF__INCLUDED_)
#define AFX_DGNCOLUMNGRIDWNDCIVIL_H__3E50C3FE_1757_4BCA_AA56_E64B1E735FEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnColumnGridWndCIVIL.h : header file
//

#include "..\wg_common\wg_common_TBBrowserWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnColumnGridWndCIVIL window
//struct T_RCHK_BEAM;
class CDBDoc;
class CDgnDrawWnd;

class CDgnColumnGridWndCIVIL : public CTBBrowserWnd
{
// Construction
public:
	CDgnColumnGridWndCIVIL();
	virtual ~CDgnColumnGridWndCIVIL();

	void Initialize();
	void SetLayerNum(UINT RchkKey, UINT nLayer);

	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnPasteFromClipboard(const CGXRange &range);
	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags);

	virtual BOOL GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType);

	virtual BOOL OnValidateField(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL ValidateField(CString& value, int nColID);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnColumnGridWndCIVIL)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	int   m_nLayer;

	void SetHeaderTitle();
	void InitSizeComBoBox();

	//{{AFX_MSG(CDgnColumnGridWndCIVIL)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNCOLUMNGRIDWNDCIVIL_H__3E50C3FE_1757_4BCA_AA56_E64B1E735FEF__INCLUDED_)
