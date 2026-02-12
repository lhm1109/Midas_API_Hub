#if !defined(__CMANALYSISOPTIONDLG_H__)
#define			 __CMANALYSISOPTIONDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMAnalysisOptionDlg.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\SpinBtnExCtrl.h"
#include "..\MIT_frx\MButton.h"
/////////////////////////////////////////////////////////////////////////////
// CCMAnalysisOptionDlg dialog

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMAnalysisOptionDlg : public CDialogMove
{
// Construction
public:
	CCMAnalysisOptionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMAnalysisOptionDlg() {};
// Dialog Data
	//{{AFX_DATA(CCMAnalysisOptionDlg)
	enum { IDD = IDD_ETC_ANALYSIS_OPTION };
	mit::frx::MButton	m_MultiProcessorChk;
	CFormulaEditSpin	m_wndMemoryEditSpin;
	CFormulaEditSpin	m_wndProcessEditSpin;
	int		m_wndMemoryCbo;
	int		m_wndMethodCbo;
	mit::frx::MButton m_ctrlAnaltOpt;
	mit::frx::MButton	m_GPUAccelChk;
	//BOOL m_bUpdateTable;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMAnalysisOptionDlg)
	public:
	virtual BOOL OnInitDialog();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_nMemorySize;  // MB -> GB
	CArray<UINT,UINT> m_arRadio;

	// Generated message map functions
	//{{AFX_MSG(CCMAnalysisOptionDlg)
	afx_msg void OnCmdAnalyoptMemoryMode();
	afx_msg void OnDeltaposCmdAnalyoptUserSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdAnalyoptOk();
	afx_msg void OnCmdAnalyoptCancel();
	afx_msg void OnUpdateCmdAnalyoptUserEdit();
	afx_msg void OnCmdMultiProcessorChk();
	afx_msg void OnCmdAnalysisMethodRdo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMANALYSISOPTIONDLG_H__DE9A3B81_5F27_11D4_9AAF_0000C0B9C58C__INCLUDED_)
