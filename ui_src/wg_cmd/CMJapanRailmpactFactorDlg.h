#if !defined(AFX_CMJapanRailmpactFactorDlg_H__E4262136_E38A_47CE_879A_AD5993E059E5__INCLUDED_)
#define AFX_CMJapanRailmpactFactorDlg_H__E4262136_E38A_47CE_879A_AD5993E059E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMJapanRailmpactFactorDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_selectctrl.h"

/////////////////////////////////////////////////////////////////////////////
// CCMJapanRailmpactFactorDlg dialog
#include "CMChildBarBase.h"
class CCMJapanRailmpactFactorDlg : public CCMChildBarBase, public CDBUpdateConnector
{
// Construction
public:
	CCMJapanRailmpactFactorDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();
	virtual BOOL ExternalInit(UINT key);
	afx_msg void OnTmSetfocusLinkEdt();

// Dialog Data
	//{{AFX_DATA(CCMJapanRailmpactFactorDlg)
	enum { IDD = IDD_CMD_JPRAIL_IMPACT_FACTOR_DLG };
	int		m_nOption;
	int		m_nTargetType;
	CEditUnit	m_dImpactFactor1;
	CEditUnit	m_dImpactFactor2;
	CSelectEdit m_edtLink;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMJapanRailmpactFactorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void RemoveNodeSelect();
	void Data2Dlg(T_JIMP_D& data);
	void Dlg2Data(T_JIMP_D& data);

	// Generated message map functions
	//{{AFX_MSG(CCMJapanRailmpactFactorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmExecute();
	afx_msg void OnTmClose();
	afx_msg void OnTmConsAddDelRdo();
	afx_msg void OnTmJimpType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMJapanRailmpactFactorDlg_H__E4262136_E38A_47CE_879A_AD5993E059E5__INCLUDED_)
