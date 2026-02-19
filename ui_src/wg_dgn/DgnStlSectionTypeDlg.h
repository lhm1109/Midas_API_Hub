#pragma once

#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CDgnStlSectionTypeDlg dialog

class CDBDoc;

class CDgnStlSectionTypeDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnStlSectionTypeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }

// Dialog Data
	//{{AFX_DATA(CDgnStlSectionTypeDlg)
	enum { IDD = IDD_DGN_STL_SECT_TYPE_DLG };
	int		m_nOption;
	int		m_nTypeY;
	int		m_nTypeZ;
	CComboBox m_cmbTypeY;
	CComboBox m_cmbTypeZ;
	//}}AFX_DATA

public:
	void Initial_SelectItem();
	void Initial_Data();
	void Inital_SectTypeCombo_Y();
	void Inital_SectTypeCombo_Z();
	BOOL ErrorCheck();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void Update_InitDataByCode();
	void SectPropertyTest();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnStlSectionTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	void EnableItems(BOOL bEnable);

// Implementation
protected:
	CDBDoc* m_pDoc;
	// Generated message map functions
	//{{AFX_MSG(CDgnStlSectionTypeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	afx_msg void OnDgnSectionTypeAr();
	afx_msg void OnDgnSectionTypeDel();
	afx_msg void OnDgnSectionTypeSel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

