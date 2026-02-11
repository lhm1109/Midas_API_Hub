#if !defined(__CMGILCLISTDLG_H__)
#define __CMGILCLISTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMGilcListDlg.h : header file
//

#include "CMGXGridList.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

class CCMGXGilcList : public CCMGXGridList
{
public:
	CCMGXGilcList() {}
	virtual ~CCMGXGilcList() {}

public:
	virtual void MakeItemList();
	virtual void ModifySerialID(CArray<CString, CString&> &aName, CArray<UINT, UINT> &aSerial);
	virtual BOOL OnLButtonDblClkRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt);

	void ConvertDataToStr(T_GILC_D data, CStringArray &aValue);
};

/////////////////////////////////////////////////////////////////////////////
// CCMGilcListDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMGilcListDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CCMGilcListDlg(CWnd* pParent = NULL);   // standard constructor
	void DoModalLoadDlg(T_GILC_K Key);

// Dialog Data
	//{{AFX_DATA(CCMGilcListDlg)
	enum { IDD = IDD_CMD_GRID_GILC_LIST };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMGilcListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();

protected:
	CDBDoc* m_pDoc;
	CCMGXGilcList m_wndGrid;

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMGilcListDlg)
	afx_msg void OnCmdAdd();
	afx_msg void OnCmdModify();
	afx_msg void OnCmdDelete();
	afx_msg void OnCmdLoads();
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMGILCLISTDLG_H__)
