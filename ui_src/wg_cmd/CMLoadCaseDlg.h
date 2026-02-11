#if !defined(__CMLOADCASEDLG_H__)
#define __CMLOADCASEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMLoadCaseDlg.h : header file
//


#include "CMDlgBase.h"
//#include "..\wg_db\wg_db_DlgBase.h"
#include "..\wg_db\wg_db_DBDoc.h"

#include "CMLoadCaseGrid.h"
#include "..\mit_frx\MEdit.h"
#include "..\mit_frx\MComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CCMLoadCaseDlg dialog

/////////////////////////////////////
#include "HeaderPre.h"      /////////
/////////////////////////////////////

class __MY_EXT_CLASS__ CCMLoadCaseDlg : public CCMDlgBase
{
// Construction
public:
	CCMLoadCaseDlg(CWnd* pParent = NULL);   // standard constructor
	~CCMLoadCaseDlg();

// Dialog Data
	//{{AFX_DATA(CCMLoadCaseDlg)
	enum { IDD = IDD_CMD_LOADCASE_DLG };
	mit::frx::MEdit	m_wndDesc;
	mit::frx::MComboBox m_cmbCase;
	mit::frx::MComboBox	m_wndType;
	mit::frx::MEdit	m_wndName;
	//}}AFX_DATA

public:
	void OnChangeCurrentStld(T_STLD_K key);
	CGXGridWnd*  MakeVirtualTB(UINT UtblK); // Word에 insert table시 db에 저장한 대로 grid를 다시 만듬.

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMLoadCaseDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

// Implementation
protected:
	CDBDoc* m_pDoc;
	T_STLD_D m_Data;
	CStringArray m_aStldType;

	void InitCaseCombo();
	void InitTypeCombo();
	void AlignControl();
	void ShowHideControls();
	BOOL Dlg2Data();
	BOOL Data2Dlg();

	BOOL IsMultiInput(const CString &csName, CStringArray &rNames);


	// Generated message map functions
	//{{AFX_MSG(CCMLoadCaseDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDlgClose();
	afx_msg void OnPaint();
	afx_msg void OnCmdLcdAdd();
	afx_msg void OnCmdLcdModify();
	afx_msg void OnCmdLcdDelete();
	afx_msg void OnCmdLcdCaseCmb();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CCMLoadCaseGrid *m_wndGrid;
};
/////////////////////////////////////
#include "HeaderPost.h"     /////////
/////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMLOADCASEDLG_H__)
