#if !defined(_DGNRCRATINGPLATEOPTIONDLG_H__)
#define _DGNRCRATINGPLATEOPTIONDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnRCRatingPlateOptionDlg.h : header file
//
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\MIT_frx\MComboBox.h"

class CDBDoc;

/////////////////////////////////////////////////////////////////////////////
// CDgnRCRatingPlateOptionDlg dialog

class CDgnRCRatingPlateOptionDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnRCRatingPlateOptionDlg(CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CDgnRCRatingPlateOptionDlg)
	enum { IDD = IDD_DGN_RATING_RC_PLATE_OPTION_DLG };

	int	    	m_nOption;
	mit::frx::MComboBox m_cmbMemb;
	CEditUnit	m_edtDroop;
	CTextUnit	m_untDroop;
	CEditUnit	m_edtStrain;
	CEditUnit	m_edtImpact;
	CEditUnit	m_edtAddResponseF;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnRCRatingPlateOptionDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Controls ID
	CArray<UINT, UINT> m_aDroop;
	CArray<UINT, UINT> m_aStrain;

	void InitUnit();
	void InitCombo();
	void AlignControl();
	void ControlsShowHide();
	void EnableDisableControls();
	BOOL Dlg2Data(T_RROP_K &rKey, T_RROP_D &rData);
	
	// Generated message map functions
	//{{AFX_MSG(CDgnRCRatingPlateOptionDlg)
	virtual BOOL OnInitDialog();
	virtual void Execute();
// 	afx_msg void OnDgnExecute();	
// 	afx_msg void OnDgnClose();	
	afx_msg void OnDgnAddBtn();
	afx_msg void OnDgnDelBtn();
	afx_msg void OnDgnMembCmb();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_DGNRCRATINGPLATEOPTIONDLG_H__)
