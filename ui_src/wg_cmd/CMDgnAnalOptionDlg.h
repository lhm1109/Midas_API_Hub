#if !defined(__CMDgnAnalOptionDlg_H__)
#define			 __CMDgnAnalOptionDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMDgnAnalOptionDlg.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"
/////////////////////////////////////////////////////////////////////////////
// CCMDgnAnalOptionDlg dialog

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMDgnAnalOptionDlg : public CDialogMove
{
// Construction
public:
	CCMDgnAnalOptionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMDgnAnalOptionDlg() {};
// Dialog Data
	//{{AFX_DATA(CCMDgnAnalOptionDlg)
	enum { IDD = IDD_ETC_DGNANAL_OPTION };

	BOOL m_bConsiderStiff; // Consider the Stiffness of Beam or Plate
	int	 m_nStiffType    ; // 0:Beam, 1:Plate
	//BOOL m_bRelieveSettle; // Relieve Differential Settlement
	//}}AFX_DATA
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMDgnAnalOptionDlg)
	public:
	virtual BOOL OnInitDialog();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
	void EnableDisableCtrls();

	// Generated message map functions
	//{{AFX_MSG(CCMDgnAnalOptionDlg)
	virtual void OnOK();
	afx_msg void OnConsiderStiffChk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMDgnAnalOptionDlg_H__DE9A3B81_5F27_11D4_9AAF_0000C0B9C58C__INCLUDED_)
