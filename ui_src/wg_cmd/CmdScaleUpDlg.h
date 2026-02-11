#if !defined(AFX_CMDSCALEUPDLG_H__241946C1_6783_11D4_888F_0000C0F30D4D__INCLUDED_)
#define AFX_CMDSCALEUPDLG_H__241946C1_6783_11D4_888F_0000C0F30D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CmdScaleUpDlg.h : header file
//


class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CCmdScaleUpDlg dialog

class CCmdScaleUpDlg : public CDialogMove
{
// Construction
public:
	CCmdScaleUpDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCmdScaleUpDlg)
	enum { IDD = IDD_CMD_SCALEUP_DLG };
	CListCtrl	m_CtrlList;
	CComboBox	m_SpecLcase;
	double	m_Factor;
	//}}AFX_DATA

public:
	CMap<int,int,double,double> m_ScaleData;

public:
	BOOL ErrorCheck();

	void OverWrite_ListCtrlBox(int Index,double Factor,CString strLoadCase);
	void Write_ListCtrlBox(int Index,double Factor,CString strLoadCase);
	void Show_EditComboBox(BOOL bCheck);
	void Initial_Data();
	void Initial_ComboBox();
	void Initial_ListCtrlBox();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCmdScaleUpDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CCmdScaleUpDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnCmdScaleupAdd();
	afx_msg void OnCmdScaleupModify();
	afx_msg void OnCmdScaleupDel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMDSCALEUPDLG_H__241946C1_6783_11D4_888F_0000C0F30D4D__INCLUDED_)
