#if !defined(__CMETCPJST_H__)
#define __CMETCPJST_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMEtcPjst.h : header file
//

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CCMEtcPjst dialog
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMEtcPjst : public CDialogMove
{
//-------------------------------------------------
// Construction
//-------------------------------------------------
public:
	CCMEtcPjst(CWnd* pParent = NULL);   // standard constructor
	~CCMEtcPjst();

//-------------------------------------------------
// Interfaces
//-------------------------------------------------
public:
	void SetAPIMode() { bAPIMode = true; };
	void GetData4API(std::vector<std::tuple<std::string, unsigned int, unsigned int, bool>>& vtplProjStatus);
//-------------------------------------------------
// Implementations
//-------------------------------------------------
protected:
	void AddModelStatus();
	void AddLoadStatus();
	void OutputModelStatus();
	void OutputLoadStatus();

	void AddModelItem(CString csItemName, UINT nCount, UINT nLastNo, int nIgnoreWhen=0);
	void AddLoadItem(CString csItemName, UINT nCount, int nIgnoreWhen=0);

	void InitGridCommonParameter(CGXGridWnd* pGrid);

protected:
	CDBDoc* m_pDoc;

	CGXGridWnd  m_wndModel;
	CGXGridWnd *m_pModel;
	CArray<CString, CString&> m_aModelName;
	CArray<UINT, UINT> m_aModelCount;
	CArray<UINT, UINT> m_aModelLastNo;

	CGXGridWnd  m_wndLoad;
	CGXGridWnd *m_pLoad;
	CArray<CString, CString&> m_aLoadName;
	CArray<UINT, UINT> m_aLoadCount;

// Dialog Data
	//{{AFX_DATA(CCMEtcPjst)
	enum { IDD = IDD_ETC_PJST };
	MButton	m_btnClose;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMEtcPjst)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMEtcPjst)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	bool bAPIMode;
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMETCPJST_H__)
