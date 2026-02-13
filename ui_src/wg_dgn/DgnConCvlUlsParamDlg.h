/////////////////////////////////////////////////////////////////////////////////////////////////
//(2009.07.10) Add by Unsang :: SPEC변경에 따라 해당 입력창은 EC2-2:05, Colm_Dgn에 적용하지 않음.
/////////////////////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGNCONCVLULSPARAMDLG_H__)
#define AFX_DGNCONCVLULSPARAMDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConCvlUlsParamDlg.h : header file
//
#include "..\wg_base\wg_base_MenuBarChildDlg.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnConCvlUlsParamDlg dialog

class CDgnConCvlUlsParamDlg : public CMenuBarChildDlg
{
// Construction
protected:
	int m_iCode;

public:
	CDgnConCvlUlsParamDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }

// Dialog Data
	//{{AFX_DATA(CDgnConCvlUlsParamDlg)
	enum { IDD = IDD_DGN_CON_ULS_PARAM_DLG };
	int		  m_nOption;
	double  m_dUlsParam1;
	double  m_dUlsParam2;
	double  m_dUlsParam3;
	double  m_dUlsParam4;
	double  m_dUlsParam5;
	//}}AFX_DATA

public:
	void Initial_SelectItem();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);  

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConCvlUlsParamDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	
	void Initial_Data();
	void EnableItems(BOOL bEnable);
	void Update_InitDlgItems();
	void UpdateBuffer();

	void GetDesignCodeNo(CString strCode); 
	
	BOOL ErrorCheck();

	// Generated message map functions
	//{{AFX_MSG(CDgnConCvlUlsParamDlg)
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnCtcAr();
	afx_msg void OnDgnCtcDel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNCONCVLULSPARAMDLG_H__)





















