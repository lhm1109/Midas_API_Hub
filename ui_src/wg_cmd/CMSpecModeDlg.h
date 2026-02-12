#if !defined(AFX_CMSPECMODEDLG_H__4C058F17_37AC_491D_94DD_F1456977B9E5__INCLUDED_)
#define AFX_CMSPECMODEDLG_H__4C058F17_37AC_491D_94DD_F1456977B9E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSpecModeDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

#include "CMSpecModeGrid.h"
//#include "CMSpecModeGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CCMSpecModeDlg dialog

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMSpecModeDlg : public CDialogMove
{
// Construction
public:
	CCMSpecModeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMSpecModeDlg)
	enum { IDD = IDD_CMD_SPEC_DLG };	
	MButton	m_chkResultSign;
	MButton m_chkSelectUse;
	int		m_nSignType;	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSpecModeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc *m_pDoc;
	CArray<UINT, UINT> m_aCtrlRadio;
	CArray<UINT, UINT> m_arCtrlSignType;  
	CArray<UINT, UINT> m_arCtrlHide;
	CArray<UINT, UINT> m_arCtrlMove;
	CCMSpecModeGrid m_wndGrid;
	T_SPLC_D* m_pData;
	BOOL m_bShifted;
//  CCMSpecModeGrid m_wndGrid;
protected:
	void IsExistEigvComplex();

public:
	void SetDataSource(T_SPLC_D* pData);
	void InitControls();
	// Generated message map functions
	//{{AFX_MSG(CCMSpecModeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickRadio();
	virtual void OnOK();
	afx_msg void OnCmdSpecTypeResultsign();	
	afx_msg void OnCmdSpecSelectModeChk();
	afx_msg void OnCmdSpecAllBtn();
	afx_msg void OnCmdSpecNoneBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSPECMODEDLG_H__4C058F17_37AC_491D_94DD_F1456977B9E5__INCLUDED_)
