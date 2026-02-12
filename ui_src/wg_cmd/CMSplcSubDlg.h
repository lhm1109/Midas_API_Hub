#if !defined(__CMSPLCSUBDLG_H__)
#define __CMSPLCSUBDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSplcSubDlg.h : header file
//
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MEdit.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_DialogMove.h"

/////////////////////////////////////////////////////////////////////////////
// CCMSplcSubDlg dialog

struct T_SPLC_D;
class CDBDoc;
class CCMSpecGrid;
class CCMSplcSubDlg : public CDialogMove
{
// Construction
public:
	CCMSplcSubDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMSplcSubDlg();

// Interface
	void SetEccenData(T_SPLC_D* pData);
	BOOL GetEccenData(T_SPLC_D& rData);
	
// Dialog Data
	//{{AFX_DATA(CCMSplcSubDlg)
	enum { IDD = IDD_DBAR_SPLC_SUB_DLG };
	mit::frx::MEdit	m_editRate;
	int		m_nMothod;
	int		m_nAuto;
	mit::frx::MButton m_chkUseGL;
	mit::frx::MButton m_chkCnsMnt;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSplcSubDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	CDBDoc* m_pDoc;
	CCMSpecGrid* m_wndGrid;
	T_SPLC_D m_Data;

protected:
	void Data2Dlg();
	BOOL Dlg2Data();

	void CheckEccnCalcBtnEnable();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMSplcSubDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnCmdSplcDataRd();
	afx_msg void OnCmdSplcCalcEccentricity();
	afx_msg void OnChangeCmdSplcDataEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSPLCSUBDLG_H__)
