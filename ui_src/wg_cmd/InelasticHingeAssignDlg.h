#if !defined(__INELASTICHINGEASSIGNDLG_H__)
#define __INELASTICHINGEASSIGNDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InelasticHingeAssignDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_CobxIehp.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MEdit.h"
#include "CMChildBarBase.h"

using namespace mit::frx;

//#include "MenuBarChildDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CInelasticHingeAssignDlg dialog

class CInelasticHingeAssignDlg : public CCMChildBarBase, public CDBUpdateConnector
{
public:
	CInelasticHingeAssignDlg();           // protected constructor used by dynamic creation
	~CInelasticHingeAssignDlg();
	//DECLARE_DYNCREATE(CInelasticHingeAssignDlg)

	enum ELEM_TYPE { ET_TRUSS=0, ET_BEAM, ET_WALL };
// Dialog Data
	//{{AFX_DATA(CInelasticHingeAssignDlg)
	enum { IDD = IDD_INELASTIC_HINGE_ASSIGN };
	MComboBox	m_cmbHingeProp;
	int		m_nElemType;
	int		m_nAddDelete;
	CString	m_strMembLoc;
	CString	m_strMatl;
	CString	m_strMembType;
	MEdit m_edtSectThik;
	//}}AFX_DATA

	HBRUSH m_hEdtBrush;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInelasticHingeAssignDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
//	virtual void OnInitialUpdate();

public:
	virtual BOOL ExternalInit(T_IEHG_K key);
	virtual void Execute();

	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

// Implementation
protected:	
	CDBDoc* m_pDoc;
	T_IEHP_D m_Data;

	int  TransMembType(int& nType);
	void GetElementByKind(CArray<UINT,UINT>& aKey, int nElemType);
	void MakeIehpCombo(int nElemType=-1);
	void ChangeIehpComboSel(UINT key);
	void UpdateIehpCombo();

	// Generated message map functions
	//{{AFX_MSG(CInelasticHingeAssignDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeInelasticHingePropCmb();
	afx_msg void OnInelasticHingePropertyBtn();
	afx_msg void OnInelasticElemType();
	afx_msg void OnInelasticAddDelete();
	afx_msg void OnSelectBtn();
	afx_msg void OnImportHingeData();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__INELASTICHINGEASSIGNDLG_H__)
