#if !defined(AFX_BNDRNSPRDLG_H__66BE7321_B396_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_BNDRNSPRDLG_H__66BE7321_B396_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BndrNsprDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxBngr.h"
#include "..\MIT_frx\MillustViewer.h"
#include "BndrNsprCalcDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CBndrNsprDlg dialog

class CBndrNsprDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CBndrNsprDlg(CWnd* pParent = NULL);   // standard constructor
	//virtual void Execute();
public:
	virtual void PostNcDestroy() { m_wndPicture.CloseDialog(); CMenuBarChildDlg::PostNcDestroy(); }

	virtual BOOL ExternalInit(UINT key);
	virtual BOOL ExternalInit_by_SelectLabel(UINT key);
	virtual ~CBndrNsprDlg();

// Dialog Data
	//{{AFX_DATA(CBndrNsprDlg)
	enum { IDD = IDD_TM_BNDR_NSPR };
	int		m_nOption;
	CCobxBngr	m_wndGroupCombo;
	mit::frx::MComboBox m_wndStiffCombo;
	CEditUnit m_wndStiffEdit;
	CTextUnit m_wndStiffUnit;
	CMouseEdit m_wndVectorEdit;
	//}}AFX_DATA
	CEditUnit m_Value[6];
	CTextUnit m_Unit[6];

	// MNET:XXXX-JWKWON-20140115 : Damping constant Ãß°¡
	BOOL	m_chkDamping;
	CEditUnit m_DampingValue[6];
	CTextUnit m_DampingUnit[6];

	// MNET:2458 20061110 mylee
	mit::frx::MComboBox m_cmbMultiDir;
	CMouseEdit m_edtMultiVector; 
	mit::frx::MComboBox	m_cmbType;
	mit::frx::MComboBox  m_wndMlfccmbx;

	BOOL m_chkSDx;
	BOOL m_chkSDy;
	BOOL m_chkSDz;
	BOOL m_chkSRx;
	BOOL m_chkSRy;
	BOOL m_chkSRz;

	mit::frx::MillustViewer m_wndPicture;
// Overrides
	virtual	void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBndrNsprDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	CArray<UINT,UINT> m_arGen,m_arCompTens;
	CArray<UINT,UINT> m_arMulti; // MNET:2458 20061110 mylee
	CArray<UINT,UINT> m_aDamping; // MNET:XXXX-JWKWON-20140115
	T_PISD_DATA m_PsidD;
	void EnableValueEdit(BOOL bEnable);
	void ChangeBitmap();
	void SetFlag();
	void RestoreMlfcCbx();

	// Generated message map functions
	//{{AFX_MSG(CBndrNsprDlg)
	virtual BOOL OnInitDialog();
	afx_msg void Execute();  
	afx_msg void OnTmClose();
	afx_msg void OnChangeOption();
	afx_msg void OnSelchangeTmBndrStiffCbo();
	afx_msg void OnTmDefineGroupButton();
	afx_msg void OnSelchangeMultiDir();
	afx_msg void OnSelchangeType();
	afx_msg void OnToggleFlag();
	afx_msg void OnClickDamping();
	afx_msg void OnClickCalcBtn_SDZ();
	afx_msg void OnClickCalcBtn_Stiff();
	afx_msg void OnMlfcButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BNDRNSPRDLG_H__66BE7321_B396_11D3_92DE_0000C0B0E6B3__INCLUDED_)
