#if !defined(__DGNSEISFAILUREMODEDLG_H__)
#define __DGNSEISFAILUREMODEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSeisFailureModeDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\wg_db_DBDoc.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MEdit.h"

#include "BrdgSeisEval.h"
#include "DgnSeisFailureModeEditBtn.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisFailureModeDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CDgnSeisFailureModeDlg : public CDialogMove
{
// Construction
public:
	CDgnSeisFailureModeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDgnSeisFailureModeDlg();

	void MakeEmfAndClose(T_EVGP_K EvgpK, int EvgpColm, int nDir, int nPos, BOOL bBending, BOOL bShear, CString strPath);
	void MakeEmfAndClose(T_EVGP_K EvgpK, int EvgpColm, CString strPath);
public:
// Dialog Data
	//{{AFX_DATA(CDgnSeisFailureModeDlg)
	enum { IDD = IDD_DGN_SEIS_FAILURE_MODE_DLG };

	MComboBox m_cmbEvalGrup;
	MComboBox m_cmbEvgpColm;
	int       m_nDir;
	MEdit     m_edtBehav;
	int       m_nPos;
	MEdit     m_edtFailMode;
	BOOL      m_bBending;
	BOOL      m_bShear;	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSeisFailureModeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitCombo();
	void InitEvgpColmCombo();
	void InitView();
	void UpdateGraph();
	void Data2Dlg();

	void EnableDisableControls();
	int  GetEvgpBehav();

	void SetGraphTempData(int nFailMode); // Test
	void SetGraphData(T_BRDGEVAL_FAILURE_MODE& FModeD, T_BRDGEVAL_CAPACITY& CapaD);
	void MakeEmfFile(CString strPath);  

	CDBDoc* m_pDoc;

	CDgnSeisFailureModeEditBtn m_View;

	BOOL m_bSetEmf;
	CString m_strPath;
	T_EVGP_K m_EvgpKForEmf; // Emf File ¸¸µé ¶§¸¸ ¾¸
	int      m_EvgpColmForEmf; // Emf File ¸¸µé ¶§¸¸ ¾¸

	CBrdgSeisEval* m_pSeisEval;

	// Generated message map functions
	//{{AFX_MSG(CDgnSeisFailureModeDlg)
	virtual BOOL OnInitDialog();    
	afx_msg void OnDgnSeisFailureModeEvalGrupCmb();
	afx_msg void OnDgnSeisFailureModeEvgpColmCmb();
	afx_msg void OnDgnSeisFailureModeDirRdo();
	afx_msg void OnDgnSeisFailureModePosRdo();
	afx_msg void OnDgnSeisFailureModeBendingChk();
	afx_msg void OnDgnSeisFailureModeShearChk();
	afx_msg void OnDgnSeisFailureModeCloaseBtn();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNSEISFAILUREMODEDLG_H__)
