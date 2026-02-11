#if !defined(AFX_IEHPPROPSTLDLG_H_INCLUDED_)
#define AFX_IEHPPROPSTLDLG_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IehpPropStlDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\MIT_frx\MillustViewer.h"
/////////////////////////////////////////////////////////////////////////////
// CIehpPropStlDlg dialog

using namespace mit::frx;
class CIehpPropStlDlg : public CDialogMove
{
	// Construction
public:
	CIehpPropStlDlg(CWnd* pParent = NULL);   // standard constructor
	~CIehpPropStlDlg();

	// Dialog Data
	//{{AFX_DATA(CIehpPropStlDlg)
	enum { IDD = IDD_ETC_IEHP_PROP_STL };
	CEditUnit	m_edtCrkPlus;
	CEditUnit	m_edtCrkMnus;
	CTextUnit	m_untCrkPlus;
	CTextUnit	m_untCrkMnus;
	CEditUnit	m_edtSlednerRatio;
	CEditUnit	m_edtSlednerEffRatio;
	CEditUnit	m_edtSlednerEffKRatio;
	CButton m_chkSlednerEffKRatio;
	CString	m_str1Plus;
	CString	m_str1Mnus;
	CEditUnit	m_edtStiffUser;
	CTextUnit	m_untStiffUser;
	int		m_nUserAuto;
	int		m_nBuckling;
	int		m_nStiffType;
	//}}AFX_DATA

public:
	T_IEHP_D m_Data;
	T_ELEM_K m_ElemK;
	void SetIehpPropData(T_IEHP_D iehpD, T_IEHG_K IehgKey, T_IEHP_BUCKL data,T_IEHP_BUCKL subdata, int nFg1, int nFg2, int nSectType=0, BOOL bAssignProperties=FALSE);
	void SetControlStateData(int nOptType);
	void ChangeBitmap();
	bool SyncDialogData();

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIehpPropStlDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	CDBDoc*   m_pDoc;
	int m_nModelType;
	int m_nDof;
	int m_nIntrType;  // Interaction Type(IehpItemDlg)
	int m_nHingeType; // Hinge Type(IehpItemDlg)
	int m_nDefinition;
	double m_dZero;
	int m_nSectType;  // 0: 적용할수 없는 단면, 1: 일반, 2: value type
	BOOL m_bDisableAutoCalc;    // Auto 계산을 할 수 없는 경우

	MillustViewer m_wndPicture;

	// Assign된 Iehp인 경우, 편집할 수 없게 대화상자 보여줌
	BOOL m_bAssignProperties;
	CArray<UINT, UINT> m_aAssignHinge_Disabled; 
	CArray<UINT, UINT> m_aAssignHinge_ReadOnly;

	void Data2Dlg();
	BOOL Dlg2Data();
	BOOL CheckErrorNameDataVar(LPCTSTR lpszData, LPCTSTR lpszName, LPCTSTR lpszVar, BOOL bMsg);
	BOOL CheckData(BOOL bMsg=TRUE);
	void UpdateLambdaEff();
	void CtrlManager();
	void SetUnitAndTitle();

	// Generated message map functions
	//{{AFX_MSG(CIehpPropStlDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	afx_msg void OnUserAutoRdo();
	afx_msg void OnBuckStrengthRdo();
	afx_msg void OnCmdLambdaEffKChk();
	afx_msg void OnStiffTypeRdo();
	afx_msg void OnChangeLambda();
	afx_msg void OnChangeLambdaEffKEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IEHPPROPSTLDLG_H_INCLUDED_)