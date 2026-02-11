#if !defined(AFX_CMMVHLITEMSTDBSCS48PAGE_H__A8C86F82_324E_46D8_95AF_EB5FD77A8FD2__INCLUDED_)
#define AFX_CMMVHLITEMSTDBSCS48PAGE_H__A8C86F82_324E_46D8_95AF_EB5FD77A8FD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhlItemStdBSCS458Page.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdBSCS458Page dialog
#include "..\MIT_frx\MComboBox.h"

#include "CMMvhlItemStdLoadGrid.h"

#include "..\wg_db\EditUnit.h"
#include "..\wg_db\TextUnit.h"

using namespace mit::frx;

class CCMMvhlItemStdBSDlg;
class CCMMvhlItemStdBSCS458Page : public CInternationalDlg
{
// Construction
public:
	CCMMvhlItemStdBSCS458Page(T_MVHL_D* pData, CWnd* pParent = NULL);   // standard constructor

public:
	T_MVHL_D* m_pData;

	int m_nLoadType;

// Dialog Data
	//{{AFX_DATA(CCMMvhlItemStdBSCS458Page)
	enum { IDD = IDD_CMD_ML_MVHL_ITEMS_BS_CS458 };

	CCMMvhlItemStdLoadGrid m_wndGrid1;
	CCMMvhlItemStdLoadGrid m_wndGrid2;

	BOOL m_bDynamic;
	int  m_nDynaAutoUser;
	CEditUnit m_edtDynaPhi;

	BOOL m_bOverload;
	int  m_nOverAutoUser;
	CEditUnit m_edtOverCriAxle;
	CEditUnit m_edtOverOtherAxle;

	int  m_nDistVehl;

	BOOL m_bPatch;
	CEditUnit m_edtPatchW;
	CTextUnit m_untPatchW;
	CEditUnit m_edtPatchL;
	CTextUnit m_untPatchL;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvhlItemStdBSCS458Page)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void SetData2Dlg();
	BOOL SetDlg2Data();

	void EnableDisableControls();

	void ChangeText();

	void SetLoadType(int nLoadType);

protected:
	void InitUnit();
	void InitGrid();

// Implementation
protected:
	CCMMvhlItemStdBSDlg* m_pParent;

	// Generated message map functions
	//{{AFX_MSG(CCMMvhlItemStdBSCS458Page)
	virtual BOOL OnInitDialog();
	afx_msg void OnDynamicChk();
	afx_msg void OnDynaAutoUserRdo();
	afx_msg void OnOverloadChk();
	afx_msg void OnOverAutoUserRdo();
	afx_msg void OnPatchBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMVHLITEMSTDBSCS48PAGE_H__A8C86F82_324E_46D8_95AF_EB5FD77A8FD2__INCLUDED_)
