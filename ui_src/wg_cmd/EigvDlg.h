#if !defined(__EIGVDLG_H__)
#define __EIGVDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EigvDlg.h : header file
//
#include "..\wg_base\wg_base_NotifyListCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_base\SpinBtnExCtrl.h"
#include "..\MIT_frx\MComboBox.h"
/////////////////////////////////////////////////////////////////////////////
// CEigvDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CEigvDlg : public CDialogMove
{
// Construction
	CArray<UINT , UINT> m_EigvCtrls;
	CArray<UINT , UINT> m_RitzCtrls;
	CArray<UINT , UINT> m_EigvSub;
	int m_nSTLC; // static load case °¹¼ö 
public:
	CEigvDlg(CWnd* pParent = NULL);   // standard constructor
	void CtrlMan();
	void NLLVCtrlMan();
	void MakeCaseListHeader();
	void MakeCaseList();
	int  FindEndLCIndex();
	void ShowCurLoadCaseData();
	void MakeLCCombo();
	void DisplayRitzVectorNumber();
	
// Dialog Data
	//{{AFX_DATA(CEigvDlg)
	enum { IDD = IDD_ETC_EIGV };
	mit::frx::MEdit	m_wndTotalNumOfRitz2;
	mit::frx::MEdit	m_wndTotalNumOfRitz;
	mit::frx::MEdit	m_wndNLFVector;
	CFormulaEditSpin	m_wndENLForceVectors;
	CFormulaEditSpin	m_wndNOGSpin;
	CListCtrl	m_wndLCList;
	mit::frx::MComboBox	m_wndLCCombo;
	CFormulaEditSpin	m_wndNOISpin;
	CFormulaEditSpin	m_wndNOFSpin;
	CFormulaEditSpin	m_wndSubDimSpin;
	CTextUnit	m_wndFreqShiftUnit;
	CTextUnit	m_wndUpperFreqUnit;
	CTextUnit	m_wndLowerFreqUnit;
	double	m_dblLowerFreq;
	double	m_dblUpperFreq;
	double	m_dblConvTol;
	double	m_dblFreqShift;
	BOOL	m_bNLLinkVect;
	BOOL  m_bUseMinMax;
	BOOL  m_bSTRUM;
	int		m_nEigenRitz;
	int		m_nSubLanc;
	int		m_nSubComp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEigvDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void CtrlComplex(BOOL bShow);
	void ChangeComplexBtnText(int nType);
	void OpenComplexMassDlg();
	void OpenComplexElemDlg();
// Implementation
protected:
	CDBDoc* m_pDoc;
	T_EIGV_D m_EigvData;
	void DlgVar2Eigv();
	void Eigv2DlgVar();

	int m_nAnalType;
	BOOL m_bComp;
	// Generated message map functions
	//{{AFX_MSG(CEigvDlg)
	virtual void OnOK();
	afx_msg void OnEigvRemoveBtn();
	virtual BOOL OnInitDialog();
	afx_msg void OnLcAddButton();
	afx_msg void OnLcDeleteButton();
	afx_msg void OnLcModifyButton();
	afx_msg void OnItemchangedLcList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickLcList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnIncNlLinkFvectorCheck();
	afx_msg void OnChangeNlLinkVnumberEdit();
	afx_msg void OnAnaltypeRadio();
	afx_msg void OnUseFRICheck();
	afx_msg void OnEIGVComplexRadio();
	afx_msg void OnEigvComplexBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__EIGVDLG_H__)
