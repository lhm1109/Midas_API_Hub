#if !defined(__BNDRMCONDLG_H__)
#define __BNDRMCONDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BndrMconDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_selectctrl.h"
#include "..\wg_db\wg_db_CobxBngr.h"
#include "..\MIT_frx\MButton.h"
/////////////////////////////////////////////////////////////////////////////
// CBndrMconDlg dialog
#include "HeaderPre.h"

class CBndrMconGrid;

class __MY_EXT_CLASS__ CBndrMconDlg : public CMenuBarChildDlg
{
	// Construction
public:
	CBndrMconDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();
	virtual BOOL ExternalInit(UINT key);
	BOOL ExternalInit2(UINT key);

	// Dialog Data
	//{{AFX_DATA(CBndrMconDlg)
	enum { IDD = IDD_TM_BNDR_MCON };
	mit::frx::MButton m_chkReleaseSlaveNode;
	CMouseEdit        m_edtMasterNode;
	int               m_nOption;
	CCobxBngr         m_cbxGroupCombo;
	CSelectEdit       m_edtExpNode;
	CSelectEdit       m_edtWDNode;
	CEditUnit         m_edtCoeff;
	CEditUnit         m_edtWeight;
	int               m_rdoConstaintDOF;
	int               m_rdoEXPDOF;
		
	//}}AFX_DATA
	BOOL    m_bDOF[6];
	BOOL    m_bIndependentDOF[6];
	BOOL    m_bEXP_DOF[6];
	
	CArray<UINT, UINT> m_aCtrlOption;
	CArray<UINT, UINT> m_aCtrlOptionExp;
	CArray<UINT, UINT> m_aCtrlDisable;
	CArray<UINT, UINT> m_aCtrlExp;
	CArray<UINT, UINT> m_aCtrlWD;
	CArray<UINT, UINT> m_aCtrlDOF_RDO;
	CArray<UINT, UINT> m_aCtrlDOF_CHK;
	CArray<UINT, UINT> m_aCtrlShow_EXP;
	CArray<UINT, UINT> m_aCtrlShow_WD;
	CArray<UINT, UINT> m_aCtrlGridApplyGrp;
	CArray<UINT, UINT> m_aCtrlDOF_Exp_RDO;
	
	void AlignControls();
	BOOL GetParameter();
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBndrMconDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	void InitGrid();
	void SetGridTitle();
	
protected:
	CDBDoc* m_pDoc;
	CBndrMconGrid* m_pGrid;
	T_MCON_DT m_aDT;

private:
	BOOL SetGridData();
	void GetGridData();

public:
	int GetMasterDof();
 
// public:
//   void ClearGrid();
//   void SetMode(int nMode);
//   void OnSelect(void*);  
//   void ResetSelection();
	// void Reset();
//   void SetParam(T_MCON_D& data);
//   BOOL GetParam(T_MCON_D& data);
	
// Generated message map functions
//{{AFX_MSG(CBndrMconDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmReleaseSlaveNode();
	afx_msg void OnTmExpAdd();
	afx_msg void OnTmWDAdd();
	afx_msg void OnTmExpDel();
	afx_msg void OnTmWDDel();
	afx_msg void OnChangeOption();
	afx_msg void OnChangeOptionExp();
	afx_msg void OnTmDefineGroupButton();
	afx_msg void OnChangeMasterNodeEdt();
	afx_msg void OnSelChangeGroupCombo();
	afx_msg void OnClickExpDof();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__BNDRMCONDLG_H__)
