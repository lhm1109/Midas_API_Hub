#if !defined(__DGNMESHBARDLG_H__)
#define __DGNMESHBARDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnMeshBarDlg.h : header file  :  sshan 090703 Mesh Design
//

#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"

#include "..\wg_base\wg_base_DialogMove.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "HeaderPre.h"

const int iConstRebarSizeMax	= 5;
const int iConstRebarSpaceMax	= 50;

class CDBDoc;
class CDgnSlabRebarPictureDlg;
/////////////////////////////////////////////////////////////////////////////
// CDgnMeshBarDlg dialog

class __MY_EXT_CLASS__ CDgnMeshBarDlg : public CDialogMove
{
// Construction
public:
	CDgnMeshBarDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnMeshBarDlg)
	enum { IDD = IDD_DGN_MESH_BAR_DLG };	
	CTextUnit	m_unitXDbSlab;
	CTextUnit	m_unitYDbSlab;
	CTextUnit	m_unitXDbMat;
	CTextUnit	m_unitYDbMat;
	CTextUnit	m_unitDwWall;
	CTextUnit	m_unitMainStrip;
	CTextUnit	m_unitXDbShell;
	CTextUnit	m_unitYDbShell;
	CEditUnit	m_editXDtSlab;
	CEditUnit	m_editXDbSlab;
	CEditUnit	m_editYDtSlab;
	CEditUnit	m_editYDbSlab;
	CEditUnit	m_editXDtMat;
	CEditUnit	m_editXDbMat;
	CEditUnit	m_editYDtMat;
	CEditUnit	m_editYDbMat;
	CEditUnit	m_editDwWall;
	CEditUnit	m_editDtStrip;
	CEditUnit	m_editDbStrip;
	CEditUnit	m_editXDtShell;
	CEditUnit	m_editXDbShell;
	CEditUnit	m_editYDtShell;
	CEditUnit	m_editYDbShell;
	CString	m_strBarSlab;
	CString	m_strSpaceSlab;
	CString	m_strBarMat;
	CString	m_strSpaceMat;
	CString	m_strVerBarWall;
	CString	m_strHorBarWall;
	CString	m_strSpaceWall;
	CString	m_strBarStrip;
	CString	m_strSpaceStrip;
	CString	m_strBarShell;
	CString	m_strSpaceShell;

	BOOL      m_bBasicRebar ;
	mit::frx::MComboBox m_cmbTopXName ;
	mit::frx::MComboBox m_cmbBotXName ;  
	mit::frx::MComboBox m_cmbTopYName ;
	mit::frx::MComboBox m_cmbBotYName ;  
	mit::frx::MComboBox m_cmbTopXSpace;
	mit::frx::MComboBox m_cmbBotXSpace;
	mit::frx::MComboBox m_cmbTopYSpace;
	mit::frx::MComboBox m_cmbBotYSpace;

	mit::frx::MButton m_chkMatlByRbar;

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

	void SetCaption();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnMeshBarDlg)
public:
	virtual BOOL DestroyWindow();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;	
	CDgnSlabRebarPictureDlg* m_pPictureDlg;

	BOOL m_bSaveBasicRebar;

	BOOL m_bPrevUnitType;     // 이전 단위계  0 : m/mm/cm,     1: in/ft
	CArray<UINT,UINT> m_aBasicCtrls;
	CArray<UINT,UINT> m_aWallCtrls;
	CArray<UINT,UINT> m_aStripCtrls;
	CArray<UINT,UINT> m_aShellCtrls;
	CArray<UINT,UINT> m_aMatlCtrls;
	CArray<UINT,UINT> m_aOkCancelCtrls;  
	void InitRebarComboBox();
	CString ConvertSpaceData2String(double dSpace);
	double  ConvertSpaceString2Data(CString& strSpace, BOOL bUnitChange=FALSE);
	
	// Generated message map functions
	//{{AFX_MSG(CDgnMeshBarDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnSlabbarSize();
	afx_msg void OnDgnMatbarSize();
	afx_msg void OnDgnWallVerbarSize();
	afx_msg void OnDgnWallHorbarSize();
	afx_msg void OnDgnStripbarSize();
	afx_msg void OnDgnShellbarSize();
	afx_msg void OnDgnSlabbarSpace();
	afx_msg void OnDgnShellbarSpace();
	afx_msg void OnDgnMatbarSpace();
	afx_msg void OnDgnWallbarSpace();
	afx_msg void OnDgnStripbarSpace();
	afx_msg void OnDgnBasicRebarChk();
	afx_msg void OnDgnShellThk4Sand();
	afx_msg void OnMatlRbarDiamCheck();
	afx_msg void OnMatlRbarDiamBtn();
	//}}AFX_MSG
	afx_msg LRESULT OnAfterInitDialog(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

protected:
	void AlignCtrl();

private:
	void InitRbar_KS();
	void InitRbar_ASTM();
	void InitRbar_EN();
	void InitRbar_GB();
	void InitRbar_CSA();
	void InitRbar_UNI();
	void InitRbar_TW();
	void InitRbar_SS();

	CString GetRebarSize(CString astrBarUser[], const CString& strBarDflt);	
	CString GetRebarSpac(CString astrBarUser[], const CString& strBarDflt);	
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNMESHBARDLG_H__)
