#if !defined(AFX_EXPOSURECLASSTABDLG_H__)
#define AFX_EXPOSURECLASSTABDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExposureClassTabDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExposureClassTabDlg_RC dialog
#include "..\wg_base\ChildDialog.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\MIT_frx\MComboBox.h"

/////////////////////////////////
#include "HeaderPre.h"   //////////
/////////////////////////////////

class CExposureClassDlg_RC;

class __MY_EXT_CLASS__ CExposureClassTabDlg_RC : public CChildDialog
{
// Construction
public:
	CExposureClassTabDlg_RC(CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CExposureClassTabDlg_RC)
	enum { IDD = IDD_DGN_DESIGN_EXPOSURE_CLASS_TAB_DLG };
	
	BOOL      m_bUserInput;
	mit::frx::MComboBox m_cmbTop;
	mit::frx::MComboBox m_cmbBottom;
	CEditUnit m_edtTop;
	CTextUnit m_untTop;
	CEditUnit m_edtBottom;
	CTextUnit m_untBottom;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExposureClassTabDlg_RC)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void SetData(BOOL bBothIJ, int nOption);
	void GetData(T_RESC_D &RescD);
	void InitUnit();
	void InitComboBox();
	void AlignControls();
	void ShowHideControls();
	void ControlsEnableDisable();

	void Data2Dlg();
	void Dlg2Data();

	CExposureClassDlg_RC* m_pParent;

	T_RESC_D m_Data;

	BOOL m_bBothIJ;
	int  m_nOption;

// Implementation

	// Generated message map functions
	//{{AFX_MSG(CExposureClassTabDlg_RC)
	virtual BOOL DestroyWindow();
	virtual BOOL OnInitDialog();
	afx_msg void OnUserInputChk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////
#include "HeaderPost.h"   //////////
/////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPOSURECLASSTABDLG_H__)
