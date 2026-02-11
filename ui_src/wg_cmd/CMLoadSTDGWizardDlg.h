#if !defined(__CMLOADSTDGWIZARDDLG_H__)
#define      __CMLOADSTDGWIZARDDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMLoadSTDGWizardDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"

#include "CMLoadESEQMainGrid.h"
#include "CMLoadESEQSubGrid.h"
#include "CMLoadESEQSpreadGrid.h"
#include "CMLoadSTDGGrid.h"


struct T_ADDL_D
{
	T_STLD_K StldKey;
	double   dSotryIncr;
	double   dStartDay;
	double   dDayIncr;

	void Initialize()
	{
		StldKey    = 0;
		dSotryIncr = 0.;
		dStartDay  = 0.;
		dDayIncr   = 0.;
	}
};

struct T_ATGN_D
{
	BOOL     bCheck;
	T_STLD_K StldKey;
	double   dStoryIncr;
	double   dStageDurt;
	double   dMemberAge;
	CArray<T_ADDL_D, T_ADDL_D&>arAddDeadLoad;

	void Initialize()
	{
		bCheck = TRUE;
		StldKey    = 0;
		dStoryIncr = 0.;
		dStageDurt = 0.;
		dMemberAge = 0.;
		arAddDeadLoad.RemoveAll();
	} 
};

/////////////////////////////////////////////////////////////////////////////
// CCMLoadSTDGWizardDlg dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CCMLoadSTDGWizardDlg : public CDialogMove , public CDBUpdateConnector
{
// Construction
public:
	CCMLoadSTDGWizardDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMLoadSTDGWizardDlg();   

public:
	void InitAutoGenerationData();

// Dialog Data
	//{{AFX_DATA(CCMLoadSTDGWizardDlg)
	enum { IDD = IDD_CMD_CONS_STAGE_WIZARD };
	CComboBox	m_cboLoadCase;
	double	m_dStageDur;
	double	m_dMembrAge;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMLoadSTDGWizardDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

protected:
	CDBDoc* m_pDoc;
	CCMLoadESEQMainGrid*   m_wndMainGrid;
	CCMLoadESEQSubGrid*    m_wndSubGrid;
	CCMLoadESEQSpreadGrid* m_wndSpreadGrid;
	CCMLoadSTDGGrid*       m_wndLCaseGrid[3];
	CComboBox              m_ESQWLoadCb[3];

	CArray<UINT, UINT> m_aCtrlNormalStyle;
	CArray<UINT, UINT> m_aCtrlSpreadStyle;

	void InitLoadData();
	void AlignControl();
	void ChangeLoadCase();
	BOOL SaveESQWData();
	BOOL m_bSpreadSheetStyle;
	BOOL m_bUpdateCheck;
	int  IsEmptyESQWData(T_ESQW_D ESQWData);
	void CheckGridCondition(UINT nComboID);
	void  SaveSubGridData(UINT nGridID, T_ESQW_D& rESQWData);

	T_ATGN_D    m_ATGNData;
	T_ESQW_D    m_ESQWData;
	T_ESEQ_BASE m_ESEQBase;
	T_ESEQ_SEL  m_ESEQSel;
	T_STLD_K    m_StldKeyOld;
	T_STLD_K    m_SubStldKeyOld[3];//1, m_StldKeyOld2, m_StldKeyOld3;

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMLoadSTDGWizardDlg)
	afx_msg void OnCmdLoadEseqSpreadBtn();
	afx_msg void OnCmdLoadEseqOk();
	afx_msg void OnCmdLoadEseqCancel();
	afx_msg void OnCmdEseqLoadcaseBtn();
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdLoadEseqRemoveBtn();
	afx_msg void OnDeltaposMemberAgeSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposStageDurationSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdLoadEsqwAutogenBtn();
	afx_msg void OnSelchangeCmdLoadEsqwLoadcase1Cbo();
	afx_msg void OnSelchangeCmdLoadEsqwLoadcase2Cbo();
	afx_msg void OnSelchangeCmdLoadEsqwLoadcase3Cbo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMLOADSTDGWIZARDDLG_H__27F69CFE_FAF9_43D8_9C4C_7DBAFF826F85__INCLUDED_)



