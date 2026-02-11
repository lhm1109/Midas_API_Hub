#if !defined(__CMLOADESEQDLG_H__)
#define      __CMLOADESEQDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMLoadESEQDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"

#include "CMLoadESEQMainGrid.h"
#include "CMLoadESEQSubGrid.h"
#include "CMLoadESEQSpreadGrid.h"
/////////////////////////////////////////////////////////////////////////////
// CCMLoadESEQDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMLoadESEQDlg : public CDialogMove , public CDBUpdateConnector
{
// Construction
public:
	CCMLoadESEQDlg(CWnd* pParent = NULL);   // standard constructor
	~CCMLoadESEQDlg();
// Dialog Data
	//{{AFX_DATA(CCMLoadESEQDlg)
	enum { IDD = IDD_CMD_LOAD_ESEQ_DLG };
	CComboBox	m_cboLoadCase;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMLoadESEQDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

// Implementation
protected:
	CDBDoc* m_pDoc;
	CCMLoadESEQMainGrid* m_wndMainGrid;
	CCMLoadESEQSubGrid* m_wndSubGrid;
	CCMLoadESEQSpreadGrid* m_wndSpreadGrid;

	CArray<UINT, UINT> m_aCtrlNormalStyle;
	CArray<UINT, UINT> m_aCtrlSpreadStyle;

	void AlignControl();
	void InitLoadData();
	void ChangeLoadCase();
	BOOL SaveESEQData();
	BOOL m_bSpreadSheetStyle;
	int IsEmptyESEQData(T_ESEQ_D ESEQData);

	BOOL m_bUpdateCheck;

	T_ESEQ_D    m_ESEQData;
	T_ESEQ_BASE m_ESEQBase;
	T_ESEQ_SEL  m_ESEQSel;

	T_STLD_K m_StldKeyOld;

	// Generated message map functions
	//{{AFX_MSG(CCMLoadESEQDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdLoadEseqRemoveBtn();
	afx_msg void OnCmdLoadEseqOk();
	afx_msg void OnCmdLoadEseqCancel();
	afx_msg void OnCmdEseqLoadcaseBtn();
	afx_msg void OnCmdLoadEseqSpreadBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMLOADESEQDLG_H__353BC8C1_7416_11D4_9AAF_0000C0B9C58C__INCLUDED_)
