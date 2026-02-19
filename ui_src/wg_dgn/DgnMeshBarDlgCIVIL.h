#if !defined(__DgnMeshBarDlgCIVIL_H__)
#define __DgnMeshBarDlgCIVIL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnMeshBarDlgCIVIL.h : header file  :  sshan 090703 Mesh Design
//

#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_base\SpinBtnExCtrl.h"
#include "..\MIT_frx\MButton.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "HeaderPre.h"

using namespace mit::frx;
class CDBDoc;
class CDgnSlabRebarPictureDlg;
/////////////////////////////////////////////////////////////////////////////
// CDgnMeshBarDlgCIVIL dialog

class __MY_EXT_CLASS__ CDgnMeshBarDlgCIVIL : public CDialogMove
{
// Construction
public:
	CDgnMeshBarDlgCIVIL(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnMeshBarDlgCIVIL)
	enum { IDD = IDD_DGN_MESH_BAR_DLG_CIVIL };	
	
	//}}AFX_DATA
public:  
	T_MSRB_D m_Msrb;

	static unsigned int WM_AFTERINITDIALOG;
public:
	void Initial_UnitData();
	CString Add_RebarList(int iMax, CString* strRebar);
	CString Add_RebarSizeList(CString* strRebar);
	CString Add_RebarSpaceList(CString* strRebar);

	BOOL Check_ErrorMsg();
	void Initial_ExistData();
	void Init_DefaultData(const int iRebarSeries);

	BOOL Get_RebarByString(CString strRebar, CArray<CString,CString&>& SelRebarList);

	BOOL IsAlivePictureDlg();
	
	void DestroyPictureDlg();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnMeshBarDlgCIVIL)
public:
	virtual BOOL DestroyWindow();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CTextUnit	m_unitXDbShell;
	CTextUnit	m_unitYDbShell;  
	CEditUnit	m_editXDtShell;
	CEditUnit	m_editXDbShell;
	CEditUnit	m_editYDtShell;
	CEditUnit	m_editYDbShell;	
	CString	m_strBarShell;
	CString	m_strSpaceShell;
	MButton m_chkAuto;	
	CFormulaEditSpin m_edtItr;
	CEditUnit m_edtTop;
	CEditUnit m_edtBot;
	CEditUnit m_edtTol;


	CDBDoc* m_pDoc;	
	CDgnSlabRebarPictureDlg* m_pPictureDlg;

	BOOL m_bSaveBasicRebar;

	BOOL m_bPrevUnitType;     // 이전 단위계  0 : m/mm/cm,     1: in/ft
	
	CArray<UINT,UINT> m_aShellCtrls;
	CArray<UINT,UINT> m_aOkCancelCtrls;  
	CString ConvertSpaceData2String(double dSpace);
	double  ConvertSpaceString2Data(CString& strSpace, BOOL bUnitChange=FALSE);

	
	// Generated message map functions
	//{{AFX_MSG(CDgnMeshBarDlgCIVIL)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnShellbarSize();
	afx_msg void OnDgnShellbarSpace();
	afx_msg void OnDgnBasicRebarChk();
	afx_msg void OnChkAuto();	
	
	//}}AFX_MSG
	afx_msg LRESULT OnAfterInitDialog(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

protected:	
	void EnableDisableCtrls();
	void ChangeCaption();
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DgnMeshBarDlgCIVIL_H__)
