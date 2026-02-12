#if !defined(__CMPRESTYPEDLG_H__)
#define __CMPRESTYPEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMPsltDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\mit_frx\MComboBox.h"
#include "..\MIT_frx\MColorButton.h"

#include "PsltGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CCMPsltDlg dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CCMPsltDlg : public CDialogMove, public CDBUpdateConnector
{
	// Construction
public:
	CCMPsltDlg(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(CCMPsltDlg)
	enum { IDD = IDD_CMD_PRES_TYPE_DLG };
	CString	m_csName;
	CString	m_csDesc;
	//}}AFX_DATA
	CSelectLC	m_cmbLCase [PSLT_NUM];
	mit::frx::MComboBox	m_cmbLType [PSLT_NUM];
	CEditUnit	m_wndLoadP1[PSLT_NUM];
	CEditUnit	m_wndLoadP2[PSLT_NUM];
	CEditUnit	m_wndLoadP3[PSLT_NUM];
	CEditUnit	m_wndLoadP4[PSLT_NUM];
	CTextUnit	m_wndUnit  [PSLT_NUM];
	mit::frx::MComboBox	m_cmbElemType;
public:
	void OnChangeCurrentPslt(T_PSLT_K nKey);
	void UpdateBuffer();
	void UnitChanged();
	void SetStartPsltK(T_PSLT_K PsltK);
	void SetPresUnit();

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMPsltDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

	// Implementation
protected:
	BOOL GetPsltData(T_PSLT_D& data,BOOL bModify = FALSE);

protected:
	CDBDoc* m_pDoc;
	CPsltGrid m_wndGrid;

	// Generated message map functions
	//{{AFX_MSG(CCMPsltDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDlgClose();
	afx_msg void OnCmdPsltAdd();
	afx_msg void OnCmdPsltDelete();
	afx_msg void OnCmdPsltModify();
	afx_msg void OnChangeLoadCase();
	afx_msg void OnChangeLoadType();
	afx_msg void OnCmdLoadcaseBtn();
	afx_msg void OnCmdElemTypeImageBtn();
	afx_msg void OnChangeETypeCmb();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	T_PSLT_K m_StartPsltK; // 외부에서 선택한 키
	void EnableDisableCtrls();
private:
	mit::frx::MColorButton		m_WellButtonColor;
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMPRESTYPEDLG_H__)
