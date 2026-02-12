#if !defined(AFX_CMSTAGECHANGEDLG_H__DB78BA88_3295_47D4_8F46_3EB568DB9C59__INCLUDED_)
#define AFX_CMSTAGECHANGEDLG_H__DB78BA88_3295_47D4_8F46_3EB568DB9C59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMStageChangeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMStageChangeDlg dialog

class CCMStageChangeDlg : public CDialogMove
{
// Construction
public:
	CCMStageChangeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMStageChangeDlg)
	enum { IDD = IDD_CMD_DEF_STAGE_CHANGE_DLG };
	CComboBox	m_cobxStage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMStageChangeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL Apply();

	// Generated message map functions
	//{{AFX_MSG(CCMStageChangeDlg)
	afx_msg void OnCmdOk();
	afx_msg void OnCmdApply();
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSTAGECHANGEDLG_H__DB78BA88_3295_47D4_8F46_3EB568DB9C59__INCLUDED_)
