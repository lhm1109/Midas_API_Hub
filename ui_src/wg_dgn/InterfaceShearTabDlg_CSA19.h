#if !defined(AFX_INTERFACESHEARTABIDLG_CSA19_H__)
#define AFX_INTERFACESHEARTABIDLG_CSA19_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InterfaceShearTabDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInterfaceShearTabDlg dialog
#include "..\wg_base\ChildDialog.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectCtrl.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

/////////////////////////////////
#include "HeaderPre.h"   //////////
/////////////////////////////////

class CInterfaceShearDlg;

class __MY_EXT_CLASS__ CInterfaceShearTabDlg_CSA19 : public CChildDialog
{
	// Construction
public:
	CInterfaceShearTabDlg_CSA19(int nIJ, CWnd* pParent = NULL/*T_SECT_D& SectD*/);   // standard constructor


	enum { IDD = IDD_DGN_DESIGN_INTERFACE_SHEAR_TAB_CSA19_DLG };
	CEditUnit m_edtBvi;
	CTextUnit m_untBvi;
	CEditUnit m_edtAvf;
	CTextUnit m_untAvf;
	CTextUnit m_untAvf2;
	CEditUnit m_edtFy;
	CTextUnit m_untFy;
	CEditUnit m_edtAf;
	//}}AFX_DATA


	// Overrides

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	CArray<UINT, UINT> m_aFace;

public:
	void SetData(BOOL bBothIJ, int nOption);
	void GetData(T_PSCS_D &PscsD);
	void InitUnit();
	void ControlsEnableDisable();

	void Data2Dlg();
	void Dlg2Data();

	CInterfaceShearDlg* m_pParent;

	T_PSCS_D m_Data;
	BOOL m_bAuto;
	BOOL m_bBothIJ;
	int  m_nOption;

	virtual BOOL DestroyWindow();
	virtual BOOL OnInitDialog();
	afx_msg void OnInterfaceShearAvfBtn();
	afx_msg void OnInterfaceShearFyBtn();
	//afx_msg void OnUserInputChk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	int m_nIJ;

};

/////////////////////////////////
#include "HeaderPost.h"   //////////
/////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INTERFACESHEARTABIDLGIDLG_H__)
