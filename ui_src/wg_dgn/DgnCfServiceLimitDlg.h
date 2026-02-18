#if !defined(AFX_DGNCFSERVICELIMITDLG_H__67582C46_0EAF_4B78_97E6_42345627350F__INCLUDED_)
#define AFX_DGNCFSERVICELIMITDLG_H__67582C46_0EAF_4B78_97E6_42345627350F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnCfServiceLimitDlg.h : header file
//
// Coded by Seungjun MNet:No.2393 ('20061011)
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnCfServiceLimitDlg dialog

class CDgnCfServiceLimitDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnCfServiceLimitDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }

// Dialog Data
	//{{AFX_DATA(CDgnCfServiceLimitDlg)
	enum { IDD = IDD_DGN_CF_SERVICE_DLG };
	int     m_iVertical;
	double  m_dVerticalUser;
	double  m_dDAF;
	int     m_nOption;
	int     m_nSeletType;
	int		m_iHorizontal;
	double	m_dHorizontalUser;
	//}}AFX_DATA

public:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void Initial_SelectItem();
	void Initial_Data();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnCfServiceLimitDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:
	//void EnableItems(BOOL bEnable);
	CDBDoc* m_pDoc;

	int  m_iCode;
	CArray<UINT,UINT> m_arIDBeam;
	CArray<UINT,UINT> m_arIDColm;
	CArray<UINT,UINT> m_arIDEtc;

	void Update_InitDataByCode();
	void UpdateBuffer();
	void EnableItems(BOOL bEnable);

	void GetDesignCodeNo(CString strCode);
	
	void MoveCtrl();

	// Generated message map functions
	//{{AFX_MSG(CDgnCfServiceLimitDlg)
	afx_msg void OnDgnExecute();
	afx_msg void OnDgnClose();
	afx_msg void OnDgnConServAr();
	afx_msg void OnDgnConServDel();
	afx_msg void OnHorizontalUser();
	afx_msg void OnVerticalUser();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNCFSERVICELIMITDLG_H__67582C46_0EAF_4B78_97E6_42345627350F__INCLUDED_)
