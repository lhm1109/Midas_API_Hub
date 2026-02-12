#if !defined(__CMThroDlg_H__)
#define __CMThroDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMThroDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\EditUnit.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMThroDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMThroDlg : public CDialogMove
{
	// Construction
public:
	CCMThroDlg(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(CCMThroDlg)
	enum { IDD = IDD_ETC_THRO_DLG };
	BOOL m_bEnergyResult;
	BOOL m_bSdvi        ;
	BOOL m_bSdve        ;
	BOOL m_bSdst        ;
	BOOL m_bSdhy        ;
	BOOL m_bSdis        ;
	BOOL m_bMSSStatus   ;
	int m_nHingeOutput;
	BOOL m_bFiberSameHinge;
	int m_nFiberOutput;
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMThroDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	T_THRO_D m_ThroD;

	void Data2Dlg();
	void Dlg2Data();

	// Generated message map functions
	//{{AFX_MSG(CCMThroDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void UpdateFiberOutputCtrl();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMThroDlg_H__)
