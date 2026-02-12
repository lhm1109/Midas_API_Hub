#if !defined(AFX_CMMVHLITEMSTDBSBD3701PEDESTRIANPAGE_H__A8C86F82_324E_46D8_95AF_EB5FD77A8FD2__INCLUDED_)
#define AFX_CMMVHLITEMSTDBSBD3701PEDESTRIANPAGE_H__A8C86F82_324E_46D8_95AF_EB5FD77A8FD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhlItemStdBSBD3701PedestrianPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdBSBD3701PedestrianPage dialog
#include "..\MIT_frx\MComboBox.h"

#include "..\wg_db\EditUnit.h"
#include "..\wg_db\TextUnit.h"

using namespace mit::frx;

class CCMMvhlItemStdBSDlg;
class CCMMvhlItemStdBSBD3701PedestrianPage : public CInternationalDlg
{
// Construction
public:
	CCMMvhlItemStdBSBD3701PedestrianPage(T_MVHL_D* pData, CWnd* pParent = NULL);   // standard constructor

public:
	T_MVHL_D* m_pData;

// Dialog Data
	//{{AFX_DATA(CCMMvhlItemStdBSBD3701PedestrianPage)
	enum { IDD = IDD_CMD_ML_MVHL_ITEMS_BS_BD3701_PEDESTRIAN	};

	CEditUnit m_edtW1;
	CEditUnit m_edtW2;
	CEditUnit m_edtL1;
	CEditUnit m_edtL2;
	
	CTextUnit m_untW1;
	CTextUnit m_untW2;
	CTextUnit m_untL1;
	CTextUnit m_untL2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvhlItemStdBSBD3701PedestrianPage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void SetData2Dlg();
	BOOL SetDlg2Data();

protected:
	void InitUnit();

// Implementation
protected:
	CCMMvhlItemStdBSDlg* m_pParent;

	// Generated message map functions
	//{{AFX_MSG(CCMMvhlItemStdBSBD3701PedestrianPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMVHLITEMSTDBSBD3701PEDESTRIANPAGE_H__A8C86F82_324E_46D8_95AF_EB5FD77A8FD2__INCLUDED_)
