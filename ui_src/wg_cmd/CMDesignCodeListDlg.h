#if !defined(AFX_CMDESIGNCODELISTDLG_H__917A7EE1_2572_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_CMDESIGNCODELISTDLG_H__917A7EE1_2572_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMDesignCodeListDlg.h : header file
//

class CCMMatPageItemDlg;
class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CCMDesignCodeListDlg dialog

class CCMDesignCodeListDlg : public CDialogMove
{
// Construction
public:
	CCMDesignCodeListDlg(CDBDoc* pDoc, CString& strType, 
		CRect& rect, CWnd* pParent = NULL);   // standard constructor

// Attributes
public:
	CDBDoc* m_pDoc;
	CString m_type;
	CRect m_rect;
	CCMMatPageItemDlg *m_pParent;

	CString m_strCurCode;
	CString m_strCurName;

// Dialog Data
	//{{AFX_DATA(CCMDesignCodeListDlg)
	enum { IDD = IDD_CMD_MP_CODE_DLG };
	CComboBox	m_cboName;
	CComboBox	m_cboCode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMDesignCodeListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int ChangeDesignCodeList();
	int ChangeMaterialNameList();
	void ChangeValueInParentDialog();

	// Generated message map functions
	//{{AFX_MSG(CCMDesignCodeListDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeDesignCode();
	afx_msg void OnChangeMatName();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMDESIGNCODELISTDLG_H__917A7EE1_2572_11D3_92DE_0000C0B0E6B3__INCLUDED_)
