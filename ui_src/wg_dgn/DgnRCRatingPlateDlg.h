#if !defined(__DgnRCRatingPlateDlg_H__)
#define __DgnRCRatingPlateDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnRCRatingPlateDlg.h : header file
//
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnRCRatingPlateDlg dialog
class CDBDoc;

class CDgnRCRatingPlateDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnRCRatingPlateDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDgnRCRatingPlateDlg();

// Dialog Data
	//{{AFX_DATA(CDgnRCRatingPlateDlg)
	enum { IDD = IDD_DGN_RATING_RC_PLATE_DLG };

	int       m_nOption;
	mit::frx::MEdit     m_edtName;
	int       m_nLocalUCS;
	mit::frx::MComboBox m_cmbUCS;
	int       m_nElemAvg;
	BOOL  	  m_bAvgNodal;
	int       m_nXYDir;
	CListCtrl	m_List;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnRCRatingPlateDlg)
protected:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


// Implementation
protected:
	void InitCombo();
	void Data2Dlg();
	BOOL Dlg2Data();
	void ControlsEnableDisable();
	void SetListCtrlHeader();
	void MakeItemEx();
	void InsertItem(T_RRPL_K Key);
	void DeleteItem(T_RRPL_K Key);
	void UpdateBuffer();

	CDBDoc* m_pDoc;
	T_RRPL_D m_Data; 
	
	// Generated message map functions
	//{{AFX_MSG(CDgnRCRatingPlateDlg)
	virtual BOOL OnInitDialog();
	virtual void Execute();
	afx_msg void OnDgnOptionRdo();
	afx_msg void OnDgnLocalUCSRdo();	
	afx_msg void OnDgnRadioElemAvg();
	afx_msg void OnDgnPlateRebarInfo();
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);	
// 	afx_msg void OnDgnApply();
// 	afx_msg void OnDgnClose();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DgnRCRatingPlateDlg_H__)
