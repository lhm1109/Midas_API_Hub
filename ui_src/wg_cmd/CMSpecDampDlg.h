#if !defined(__CMSPECDAMPDLG_H__)
#define __CMSPECDAMPDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSpecDampDlg.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMSpecDampDlg dialog
class CDampGrid;
class CCMSpecDampDlg : public CDialogMove
{
// Construction
public:
	CCMSpecDampDlg(T_SPLC_D* pData, CWnd* pParent = NULL);   // standard constructor
	~CCMSpecDampDlg();

// Dialog Data
	//{{AFX_DATA(CCMSpecDampDlg)
	enum { IDD = IDD_DBAR_SPLC_DAMP_DLG };
	CEditUnit	m_editStiffDM2;
	CEditUnit	m_editStiffDM1;
	CEditUnit	m_editStiffPM2;
	CEditUnit	m_editStiffPM1;
	CEditUnit	m_editStiffFM2;
	CEditUnit	m_editStiffFM1;
	CEditUnit	m_editStiffSC;
	CEditUnit	m_editStiffMC;
	CEditUnit	m_editStiffSD;
	CEditUnit	m_editStiffMD;
	MButton	  m_chkStiffMass;
	MButton  	m_chkStiffStiff;
	MButton  	m_rdoBtnGroup;
	CListCtrl	m_wndDampList;
	CEditUnit	m_editDampAll;
	//CEditUnit	m_editDampMode;
	//CEditUnit	m_editDampRatio;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSpecDampDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	CDampGrid* m_pGrid;
	void InitControls();
	void AlignControl();
	//CString DataToStr(int i, const T_THIS_BASE &data);

	void Data2Dlg();
	BOOL Dlg2Data();

	void PropData2Dlg();
	BOOL Dlg2PropData();

	void AutoCalcRmRkCoef();
	void DoCtrlManager();
	void ShowDampingRatio();

protected:
	T_SPLC_D*   m_pAllData;
	T_THIS_BASE m_CurData;

	// child dialog backup data
	int    m_nFreqPeriodRdoDlg;
	double m_dFrequencyDlg;
	double m_dPeriodDlg;

	CArray<UINT, UINT> m_aCtrlDampType, m_aCtrlCoefDefn, m_aCtrlFreqPerd;
	CArray<UINT, UINT> m_aCtrlDirect;
	CArray<UINT, UINT> m_aCtrlMassSiff, m_aCtrlCoefCalc;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMSpecDampDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnCmdTypeRadio();
	afx_msg void OnCmdStiffTypeRadio();
	afx_msg void OnCmdStiffModeRadio();
	afx_msg void OnCmdStiffMassChk();
	afx_msg void OnCmdStiffStiffChk();
	afx_msg void OnCmdStiffBtn();
	afx_msg void OnChangeCmdCoefCalcEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSPECDAMPDLG_H__)
