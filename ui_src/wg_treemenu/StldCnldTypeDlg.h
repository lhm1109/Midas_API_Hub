#pragma once

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxLdgr.h"
#include "..\MIT_frx\MillustViewer.h"
/////////////////////////////////////////////////////////////////////////////
// CStldCnldTypeDlg dialog

class CStldCnldTypeDlg : public CMenuBarChildDlg
{
// Construction
public:
	CStldCnldTypeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CStldCnldTypeDlg();
	virtual void Execute();
// Dialog Data
	//{{AFX_DATA(CStldCnldTypeDlg)
	enum { IDD = IDD_TM_STLD_CNLD_TYPE };
	CSelectLC	m_LoadCase;
	int		m_nOption;
	CCobxLdgr	m_wndLdgrCobx;
	//}}AFX_DATA
	CEditUnit m_Value[6];
	CTextUnit m_Unit[6];
	mit::frx::MillustViewer m_wndPicture;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStldCnldTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	CString m_strTypeName;
	void EnableValueEdit(BOOL bEnable);

	// Generated message map functions
	//{{AFX_MSG(CStldCnldTypeDlg)
	virtual BOOL OnInitDialog();
	virtual void UpdateChildWindow();
	virtual void End();
	afx_msg void OnTmExecute();
	afx_msg void OnTmClose();
	afx_msg void OnClickLoadCaseBtn();
	afx_msg void OnChangeOption();
	afx_msg void OnTmDefineLdgrBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString GetNewTypeName();
};

