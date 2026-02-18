#if !defined(AFX_DGNCONLOSSFACTORDLG_H__AD536EA2_7E68_4AA7_A1D0_A02432AF7391__INCLUDED_)
#define AFX_DGNCONLOSSFACTORDLG_H__AD536EA2_7E68_4AA7_A1D0_A02432AF7391__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConLossFactorDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDgnConLossFactorDlg dialog
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgnConLossFactorDlg : public CDialogMove
{
// Construction
public:
	CDgnConLossFactorDlg(CWnd* pParent = NULL);   // standard constructor

	void InitData();
	BOOL ErrorCheck();
    void GetDataFromDlg(T_DCON_D& rData);

// Dialog Data
	//{{AFX_DATA(CDgnConLossFactorDlg)
	enum { IDD = IDD_DGN_CON_LOSS_DLG };
	double m_dTranLoss;
	double m_dServLoss;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConLossFactorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNCONLOSSFACTORDLG_H__AD536EA2_7E68_4AA7_A1D0_A02432AF7391__INCLUDED_)
