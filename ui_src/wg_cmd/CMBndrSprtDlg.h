#if !defined(__CMBNDRSPRTDLG_H__)
#define      __CMBNDRSPRTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMBndrSprtDlg.h : header file
//
#include "CMBndrSprtGrid.h"
#include "../MIT_frx/MTabCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CCMBndrSprtDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMBndrSprtDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CCMBndrSprtDlg(CWnd* pParent = NULL);   // standard constructor
	void OnChangeCurrentGSTP(T_GSTP_K nKey);
	void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
// Dialog Data
	//{{AFX_DATA(CCMBndrSprtDlg)
	enum { IDD = IDD_CMD_BNDR_SPRT_DLG };
	
	BOOL m_bStiffness;
	BOOL m_bMass;
	BOOL m_bDamping;

	mit::frx::MTabCtrl	m_Tab;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMBndrSprtDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString m_csName;

	CDBDoc* m_pDoc;

	CArray<UINT,UINT> m_arSpringType;
	CArray<UINT,UINT> m_arDSpringType;

	double m_dSpring[21];
	double m_dMass[21];
	double m_dDamping[21];

	T_GSTP_K m_GstpKey;
	CArray<T_GSTP_K, T_GSTP_K> m_arGstpKey;
	T_GSTP_D m_GstpData;
	CArray<T_GSTP_D, T_GSTP_D> m_arGstpData;

	BOOL m_bInit;

	CCMBndrSprtGrid m_wndGrid;
//	CGXGridWnd m_wndGrid;
	void InitGrid();
	void SetGridTitle();
	void SetGridValue();
	void UpdateBuffer();
	BOOL GetGstpData(T_GSTP_D& data);
	void ChangeSpringNumber(int nEditNum);
	void ChangeTabState(int &nTabSel);
	void GetData(int nTabIndex);
	void SetData(int nTabIndex);

	// Generated message map functions
	//{{AFX_MSG(CCMBndrSprtDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdSprtChangeMethod();
	afx_msg void OnCmdSprtAdd();
	afx_msg void OnCmdSprtClose();
	afx_msg void OnCmdSprtDelete();
	afx_msg void OnCmdSprtReplace();
	afx_msg void OnChangeCmdSprt7Edit();
	afx_msg void OnChangeCmdSprt8Edit();
	afx_msg void OnChangeCmdSprt9Edit();
	afx_msg void OnChangeCmdSprt10Edit();
	afx_msg void OnChangeCmdSprt11Edit();
	afx_msg void OnChangeCmdSprt12Edit();
	afx_msg void OnChangeCmdSprt13Edit();
	afx_msg void OnChangeCmdSprt14Edit();
	afx_msg void OnChangeCmdSprt15Edit();
	afx_msg void OnChangeCmdSprt16Edit();
	afx_msg void OnChangeCmdSprt17Edit();
	afx_msg void OnChangeCmdSprt18Edit();
	afx_msg void OnChangeCmdSprt19Edit();
	afx_msg void OnChangeCmdSprt20Edit();
	afx_msg void OnChangeCmdSprt21Edit();
	afx_msg void OnSelchangeCmdTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingCmdTab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMBNDRSPRTDLG_H__83336061_6C9B_11D4_9AAF_0000C0B9C58C__INCLUDED_)
