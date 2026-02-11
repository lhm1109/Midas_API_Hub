#if !defined(__SEISCVLEVALPARAMDLG_H__)
#define __SEISCVLEVALPARAMDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisCvlEvalParamDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\MIT_frx\MComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlEvalParamDlg dialog

class CSeisCvlEvalParamDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
	// Construction
public:
	CSeisCvlEvalParamDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnTmEvalParamApplyBtn(); };
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

	// Dialog Data
	//{{AFX_DATA(CSeisCvlEvalParamDlg)
	enum { IDD = IDD_TM_SEIS_CVL_MISC_EVAL_PARAM };

	int       m_nOption;
	mit::frx::MComboBox m_cmbEvalGrup;
	mit::frx::MComboBox m_cmbEvalColm;
	mit::frx::MComboBox m_cmbBehavL;
	CEditUnit m_edtHe1L;
	CTextUnit m_untHe1L;
	CEditUnit m_edtHe2L;
	CTextUnit m_untHe2L;
	mit::frx::MComboBox m_cmbBehavT;
	CEditUnit m_edtHe1T;
	CTextUnit m_untHe1T;
	CEditUnit m_edtHe2T;
	CTextUnit m_untHe2T;
	CEditUnit m_edtAeRat;
	int       m_nBHMethod;
	mit::frx::MComboBox m_cmbSplice;
	CEditUnit m_edtLP;
	CEditUnit m_edtTs;
	CTextUnit m_untTs;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeisCvlEvalParamDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	void InitUnit();
	void InitCombo();
	void InitCombo_EvgpGrup();
	void InitCombo_EvgpColm();
	BOOL IsEvgpSectSR();
	void EnableDisableControls();
	void GetLp();

	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CSeisCvlEvalParamDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmEvalParamOptionRdo();
	afx_msg void OnTmEvalParamEvalGrpCmb();
	afx_msg void OnTmEvalParamEvalColmCmb();
	afx_msg void OnTmEvalParamBehavLCmb();
	afx_msg void OnTmEvalParamLongiDirBtn();
	afx_msg void OnTmEvalParamBehavTCmb();
	afx_msg void OnTmEvalParamSpliceCmb();  
	afx_msg void OnTmEvalParamTransDirBtn();
	afx_msg void OnTmEvalParamTsBtn();
	afx_msg void OnTmEvalParamApplyBtn();
	afx_msg void OnTmEvalParamCloseBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__SEISCVLEVALPARAMDLG_H__)
