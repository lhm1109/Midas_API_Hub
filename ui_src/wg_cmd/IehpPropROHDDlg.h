#if !defined(AFX_IEHPPROPROHDDLG_H__5B71E286_BB26_48CC_8A9C_46A35491E9B1__INCLUDED_)
#define AFX_IEHPPROPROHDDLG_H__5B71E286_BB26_48CC_8A9C_46A35491E9B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IehpPropROHDDlg.h : header file
//

#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CIehpPropROHDDlg dialog
struct T_IEHP_ROHD;
class CIehpPropROHDDlg : public CDialogMove
{
// Construction
public:
	CIehpPropROHDDlg(T_IEHP_ROHD *pData, CWnd* pParent = NULL);   // standard constructor

	void SetROHDType(int nType, int nDof) { m_nType = nType; m_nDof = nDof; }
	bool SyncDialogData();

// Dialog Data
	//{{AFX_DATA(CIehpPropROHDDlg)
	enum { IDD = IDD_ETC_IEHP_ROHD };
	CEditUnit	m_edtCriticalDispl;
	CTextUnit	m_untCriticalDispl;
	CEditUnit	m_edtMaxDamp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIehpPropROHDDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	T_IEHP_ROHD *m_pData;
	int          m_nType; // D_IEHP_RO, D_IEHP_HD
	int          m_nDof;  // 0:displacement, 1:rotation
	

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CIehpPropROHDDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IEHPPROPROHDDLG_H__5B71E286_BB26_48CC_8A9C_46A35491E9B1__INCLUDED_)
