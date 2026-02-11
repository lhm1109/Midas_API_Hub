#if !defined(AFX_CMMVHLITEMSTDBSBD3701HAPAGE_H__A8C86F82_324E_46D8_95AF_EB5FD77A8FD2__INCLUDED_)
#define AFX_CMMVHLITEMSTDBSBD3701HAPAGE_H__A8C86F82_324E_46D8_95AF_EB5FD77A8FD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhlItemStdBSBD3701HAPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdBSBD3701HAPage dialog
#include "..\MIT_frx\MComboBox.h"

#include "..\wg_db\EditUnit.h"
#include "..\wg_db\TextUnit.h"

using namespace mit::frx;

class CCMMvhlItemStdBSDlg;
class CCMMvhlItemStdBSBD3701HAPage : public CInternationalDlg
{
// Construction
public:
	CCMMvhlItemStdBSBD3701HAPage(T_MVHL_D* pData, CWnd* pParent = NULL);   // standard constructor

public:
	T_MVHL_D* m_pData;

// Dialog Data
	//{{AFX_DATA(CCMMvhlItemStdBSBD3701HAPage)
	enum { IDD = IDD_CMD_ML_MVHL_ITEMS_BS_BD3701_HA	};

	int       m_nLaneFact;

	CEditUnit m_edtW1;
	CEditUnit m_edtW1_exp;
	CEditUnit m_edtW2;
	CEditUnit m_edtW2_exp;
	CEditUnit m_edtW3;
	CEditUnit m_edtL1;
	CEditUnit m_edtL21;
	CEditUnit m_edtL22;
	CEditUnit m_edtL3;
	CEditUnit m_edtPa;
	
	BOOL      m_bAddData;
	CEditUnit m_edtaL;
	MComboBox m_cmbCategory;
	MComboBox m_cmbLoadLevel;

	CTextUnit m_untW1;
	CTextUnit m_untW2;
	CTextUnit m_untW3;
	CTextUnit m_untL1;
	CTextUnit m_untL2;
	CTextUnit m_untL3;
	CTextUnit m_untPa;
	CTextUnit m_untaL;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvhlItemStdBSBD3701HAPage)
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
	void InitUnit();
	void InitCombo();

// Implementation
protected:
	CCMMvhlItemStdBSDlg* m_pParent;

	// Generated message map functions
	//{{AFX_MSG(CCMMvhlItemStdBSBD3701HAPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnLaneFactorRdo();
	afx_msg void OnLaneFactorBtn();
	afx_msg void OnAddDataChk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMVHLITEMSTDBSBD3701HAPAGE_H__A8C86F82_324E_46D8_95AF_EB5FD77A8FD2__INCLUDED_)
