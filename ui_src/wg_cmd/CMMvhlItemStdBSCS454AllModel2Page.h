#if !defined(AFX_CMMVHLITEMSTDBSCS454ALLMODEL2PAGE_H__A8C86F82_324E_46D8_95AF_EB5FD77A8FD2__INCLUDED_)
#define AFX_CMMVHLITEMSTDBSCS454ALLMODEL2PAGE_H__A8C86F82_324E_46D8_95AF_EB5FD77A8FD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhlItemStdBSCS454AllModel2Page.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdBSCS454AllModel2Page dialog
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MillustViewer.h"

#include "..\wg_db\EditUnit.h"
#include "..\wg_db\TextUnit.h"

using namespace mit::frx;

class CCMMvhlItemStdBSDlg;
class CCMMvhlItemStdBSCS454AllModel2Page : public CInternationalDlg
{
// Construction
public:
	CCMMvhlItemStdBSCS454AllModel2Page(T_MVHL_D* pData, CWnd* pParent = NULL);   // standard constructor

public:
	T_MVHL_D* m_pData;

// Dialog Data
	//{{AFX_DATA(CCMMvhlItemStdBSCS454AllModel2Page)
	enum { IDD = IDD_CMD_ML_MVHL_ITEMS_BS_CS454_ALL_MODEL2 };

	int m_nLaneFact;

	MillustViewer m_wndPicture;

	MComboBox m_cmbCategory;
	MComboBox m_cmbLevel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvhlItemStdBSCS454AllModel2Page)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void SetData2Dlg();
	BOOL SetDlg2Data();

	void EnableDisableControls();

protected:
	void InitCtrl();
	void InitCombo();

// Implementation
protected:
	CCMMvhlItemStdBSDlg* m_pParent;

	// Generated message map functions
	//{{AFX_MSG(CCMMvhlItemStdBSCS454AllModel2Page)
	virtual BOOL OnInitDialog();
	afx_msg void OnLaneFactorRdo();
	afx_msg void OnLaneFactorBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMVHLITEMSTDBSCS454ALLMODEL2PAGE_H__A8C86F82_324E_46D8_95AF_EB5FD77A8FD2__INCLUDED_)
