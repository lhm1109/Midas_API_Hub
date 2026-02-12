#if !defined(__CMSELECTSECT_H__)
#define __CMSELECTSECT_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSelectSect.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMSelectSect dialog

class CCMSelectSect : public CDialogMove
{
// Construction
public:
	CCMSelectSect(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMSelectSect)
	enum { IDD = IDD_CMD_SELECT_SECT_DLG };
	CListBox m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSelectSect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void SetSectType(const CArray<int, int> &aType);
	void SetPSCShape(const CArray<CString, CString&> &aShape);

	UINT GetSelectedSectK();


protected:
	void ResetList();

protected:
	CMap<int, int, BOOL, BOOL> m_mSectType;
	CArray<CString, CString&> m_aPSCShape;
	UINT m_Key;

// Implementation  
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMSelectSect)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkCmdList();
	afx_msg void OnSelchangeCmdList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSELECTSECT_H__)
