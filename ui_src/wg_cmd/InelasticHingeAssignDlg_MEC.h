#if !defined(__INELASTICHINGEASSIGNDLG_MEC_H__)
#define __INELASTICHINGEASSIGNDLG_MEC_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InelasticHingeAssignDlg_MEC.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_CobxIehp.h"
#include "..\wg_db\wg_db_CobxNllp.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\mit_frx\MButton.h"
#include "..\wg_base\MouseEdit.h"

#include "CMChildBarBase.h"

//#include "MenuBarChildDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CInelasticHingeAssignDlg_MEC dialog

class CInelasticHingeAssignDlg_MEC : public CCMChildBarBase, public CDBUpdateConnector
{
public:
	CInelasticHingeAssignDlg_MEC();           // protected constructor used by dynamic creation
	//DECLARE_DYNCREATE(CInelasticHingeAssignDlg_MEC)

	enum ELEM_TYPE { ET_BEAM =0, ET_TRUSS, ET_WALL, ET_NLNK, ET_PSSP};
// Dialog Data
	//{{AFX_DATA(CInelasticHingeAssignDlg_MEC)
	enum { IDD = IDD_INELASTIC_HINGE_ASSIGN_MEC };
	MComboBox	m_cmbHingeProp;
	int		m_nElemType;
	int		m_nAddDelete;
	CString	m_strMembLoc;
	CString	m_strMatl;
	CString	m_strMembType;
	CString	m_strSectThik;
	//
	CMouseEdit	m_edtNlnkInput;
	CBCGPStatic m_stcNlnkInput;
	CEdit		m_edtNlnkNumber;
	mit::frx::MButton m_rdoGlink1;
	mit::frx::MButton m_rdoGlink2;
	mit::frx::MButton m_rdoGlink3;
	CCobxNllp   m_cmbGlink;
	mit::frx::MButton m_btnGlink;

	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInelasticHingeAssignDlg_MEC)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);
	LRESULT OnNodeListEnter(WPARAM wParam, LPARAM lParam);
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
	//{{AFX_MSG(CInelasticHingeAssignDlg_MEC)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeInelasticHingePropCmb();
	afx_msg void OnInelasticHingePropertyBtn();
	afx_msg void OnInelasticElemType();
	afx_msg void OnInelasticAddDelete();
	afx_msg void OnSelectBtn();
	afx_msg void OnImportHingeData();
	afx_msg void OnInelasticGLinkSelRdo();
	afx_msg void OnInelasticGLinkPropBtn();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	bool DlgGlinkCtrlChanges();
	void GetPointSpringSupportNode(CArray<UINT, UINT>& aKey);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__INELASTICHINGEASSIGNDLG_MEC_H__)
