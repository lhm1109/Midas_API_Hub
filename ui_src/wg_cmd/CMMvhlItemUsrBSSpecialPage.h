#if !defined(AFX_CMMVHLITEMUSRBSSPECIALPAGE_H__A8C86F82_324E_46D8_95AF_EB5FD77A8FD2__INCLUDED_)
#define AFX_CMMVHLITEMUSRBSSPECIALPAGE_H__A8C86F82_324E_46D8_95AF_EB5FD77A8FD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhlItemUsrBSSpecialPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUsrBSSpecialPage dialog
#include "..\wg_db\TextUnit.h"
#include "..\wg_db\EditUnit.h"

struct T_MVHL_D;
class CCMMvhlItemUsrBSSpecialPage : public CInternationalDlg
{
// Construction
public:
	CCMMvhlItemUsrBSSpecialPage(T_MVHL_D* pData, CWnd* pParent = NULL);   // standard constructor

public:
	T_MVHL_D* m_pData;

// Dialog Data
	//{{AFX_DATA(CCMMvhlItemUsrBSSpecialPage)
	enum { IDD = IDD_CMD_ML_MVHL_ITEMU_BS_SPECIAL_PAGE };

	CEditUnit m_edtP;
	CTextUnit m_untP;
	CEditUnit m_edtD;
	CTextUnit m_untD;
	CListCtrl	m_lstTruckL;
	BOOL      m_bAxleSpacing;
	CEditUnit m_edtAxleNum;
	CEditUnit m_edtMinSpacing;
	CTextUnit m_untMinSpacing;
	CEditUnit m_edtMaxSpacing;
	CTextUnit m_untMaxSpacing;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvhlItemUsrBSSpecialPage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void SetData2Dlg();
	BOOL SetDlg2Data();

protected:
	void InitUnit();
	void SetHeaderTitle();
	void MakeItemEx();
	
	BOOL InsertItem(int nPos, int nNo, double dLoad, double dDist);
	BOOL DeleteItem(int nPos);
	BOOL ModifyItem(int nPos, int nNo, double dLoad, double dDist);

	BOOL GetLoadAndDistValue(double &dLoad, double &dDist);
	int GetTruckLoadCount();
	int GetTruckDistanceCount();


// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMMvhlItemUsrBSSpecialPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnIns();
	afx_msg void OnCmdBtnMod();
	afx_msg void OnCmdBtnDel();
	afx_msg void OnItemchangedCmdList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdAxleSpacingChk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMVHLITEMUSRBSSPECIALPAGE_H__A8C86F82_324E_46D8_95AF_EB5FD77A8FD2__INCLUDED_)
