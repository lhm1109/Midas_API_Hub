#if !defined(AFX_CMSECTREGDBLSECT_H__0AA768E6_E44B_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_CMSECTREGDBLSECT_H__0AA768E6_E44B_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectRegDblSect.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMSectRegDblSect dialog
class CDBDoc;
class CCMSectRegDblSect : public CDialogMove
{
// Construction
public:
	CCMSectRegDblSect(CString csShape, int x, int y, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMSectRegDblSect)
	enum { IDD = IDD_CMD_SP_DBL_SECT_DLG };
	CComboBox	m_cboName;
	CComboBox	m_cboDB;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectRegDblSect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	CString m_csShape;
	CWnd* m_pParent;
	int  m_nCurDB;
	int  m_x, m_y; // Dialog Position
	void SetSectNameList();
	void MoveDialog();
	

	// Generated message map functions
	//{{AFX_MSG(CCMSectRegDblSect)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeCurDB();
	afx_msg void OnChangeCurSect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSECTREGDBLSECT_H__0AA768E6_E44B_11D3_92DE_0000C0B0E6B3__INCLUDED_)
