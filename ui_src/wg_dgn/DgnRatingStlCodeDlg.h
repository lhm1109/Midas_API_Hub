#if !defined(__DGNRATINGSTLCODEDLG_H__)
#define __DGNRATINGSTLCODEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnRatingStlCodeDlg.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\MIT_frx\MComboBox.h"

#include "HeaderPre.h"

using namespace mit::frx;

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnRatingStlCodeDlg dialog
class __MY_EXT_CLASS__ CDgnRatingStlCodeDlg : public CDialogMove
{
	// Construction
public:
	CDgnRatingStlCodeDlg(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(CDgnRatingStlCodeDlg)
	enum { IDD = IDD_DGN_RATING_STL_CODE_DLG };
	MComboBox	m_DesignCode;
	//}}AFX_DATA

public:
	void Initial_Data();

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnRatingStlCodeDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	CDBDoc* m_pDoc;

	void InitDgnCodeCombo();
	void Data2Dlg();
	void Dlg2Data();

	T_RCDS_D m_Data;

	// Generated message map functions
	//{{AFX_MSG(CDgnRatingStlCodeDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNRATINGSTLCODEDLG_H__)
