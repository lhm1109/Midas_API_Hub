#if !defined(AFX_DGNCONPSCMATDLG_H__89011DED_0792_4057_8625_CF74E77CF623__INCLUDED_)
#define AFX_DGNCONPSCMATDLG_H__89011DED_0792_4057_8625_CF74E77CF623__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\wg_db_TextUnit.h"

const int CONST_DGN_PSC_iGRIDCOLM = 4;

#include "HeaderPre.h"

// DgnConPscMatDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DgnConPscMatDlg dialog

class __MY_EXT_CLASS__ CDgnConPscMatDlg : public CDialogMove
{
// Construction
public:
	CDgnConPscMatDlg(CWnd* pParent = NULL);   // standard constructor

	double m_Fck;
	CString m_strDgnCode;
// Dialog Data
	//{{AFX_DATA(DgnConPscMatDlg)
	enum { IDD = IDD_DGN_CON_PSCMAT_DLG };

	CTextUnit	m_FciUnit;
	CListCtrl	m_ListCtrl;
	double	m_Fci;
	double  m_Age;
	BOOL    m_bCalc;
	int     m_iCement;
	//}}AFX_DATA

public:
	BOOL ErrorCheck();

	void Init_Data();
	void Init_GridData();
	void Init_Unit();
	void Init_MatlCode();
	void EnableEditBox(BOOL nCheck);
	void Write_ListBox(int iIndex, CString strID, CString strNa, CString strFc, CString strFci);
	void ChangeDlgItemInfo();
	double CalcInitConcStrength(double dFck, CString strDgnCode);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DgnConPscMatDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	// Generated message map functions
	//{{AFX_MSG(DgnConPscMatDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnClickDgnConcPscMatList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemChangedDgnConcPscMatList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDgnConcPscMatModify();
	afx_msg void OnDgnConcPscMatCalculate();
	afx_msg void OnDgnConcPscMatCheck();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNCONPSCMATDLG_H__89011DED_0792_4057_8625_CF74E77CF623__INCLUDED_)
