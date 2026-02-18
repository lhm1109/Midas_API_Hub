#if !defined(__DGNRATINGCODEDLG_H__)
#define __DGNRATINGCODEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnRatingCodeDlg.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

#include "HeaderPre.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnRatingCodeDlg dialog
class __MY_EXT_CLASS__ CDgnRatingCodeDlg : public CDialogMove
{
// Construction
public:
	CDgnRatingCodeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnRatingCodeDlg)
	enum { IDD = IDD_DGN_RATING_CODE_DLG };
	MComboBox	m_DesignCode;	
	//}}AFX_DATA
	
public:  
	void Initial_Data();	
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnRatingCodeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	void InitDgnCodeCombo();
	void Data2Dlg();
	void Dlg2Data();

	T_RACD_D m_Data;

	// Generated message map functions
	//{{AFX_MSG(CDgnRatingCodeDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNRATINGCODEDLG_H__)
