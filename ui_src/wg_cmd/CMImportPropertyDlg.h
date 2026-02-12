#if !defined(AFX_CMIMPORTPROPERTYDLG_H__EBAF7782_8575_11D4_A678_00010263A1CE__INCLUDED_)
#define AFX_CMIMPORTPROPERTYDLG_H__EBAF7782_8575_11D4_A678_00010263A1CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMImportPropertyDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
// BnFile 
#include "..\wg_db\BnFile.h"
#include "..\wg_base\SpinBtnExCtrl.h"

#define BN_ER_GOOD    0
#define BN_ER_FORM    1
#define BN_ER_READ    2
#define BN_ER_VRHI    3
#define BN_ER_NONE    4
#define BN_ER_DIFF    5
#define BN_ER_USER    6
#define BN_ER_HEAD    7
#define BN_ER_TYPE    8
#define BN_ER_VRLO    9
#define BN_ER_DATA   10


/*
struct T_MAIN_HEAD
{
	char DESC_HEAD[4];
	int ITYP_FILE;
	float VER_FILE;
	int NUM_HEAD;
	int NEXT_REC;
};

struct T_SUB_HEAD
{
	char ID_MAIN[4];
	int ID_SUB;
	int IST_REC;
	int IEN_REC;
	int LNG_REC;
};
*/

struct T_SECTION
{
	T_SECT_K Key;
	T_SECT_D Data;
};

struct T_MATERIAL
{
	T_MATL_K Key;
	T_MATL_D Data;
};

struct T_MATLDESIGN
{
	T_MATL_K Key;
	T_MATD_D Data;
};

/////////////////////////////////////////////////////////////////////////////
// CCMImportPropertyDlg dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CCMImportPropertyDlg : public CDialogMove
{
// Construction
public:
	CCMImportPropertyDlg(CWnd* pParent = NULL);   // standard constructor
	void InitPropertyData(CString strFileName, int nType, BOOL bIsMgbFile);

// Dialog Data
	//{{AFX_DATA(CCMImportPropertyDlg)
	enum { IDD = IDD_CMD_IMPORTPROPERTY_DLG };
	CBCGPStatic	m_strTitle;
	CFormulaEditSpin	m_edtStartID;
	CListBox	m_lstSelProperty;
	CListBox	m_lstProperty;
	int		m_rdoImportType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMImportPropertyDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:

protected:
	CDBDoc* m_pDoc;
	CFile* m_pFile;
	CBnFile m_BnFile;

	char ID_M[4];
	int ID_S,IST_R,LNG_R,NUM_R;
	int m_nPropertyType;
	T_MAIN_HEAD m_MainHead;
	T_SUB_HEAD m_SubHead;

	BOOL RB_UNIT();
	BOOL RB_SECT();
	BOOL RB_THIK();
	BOOL RB_MATL();
	BOOL RB_MATD();
	BOOL RB_SECD();

	void InitMaterialData(BOOL bSelectUnSelect);
	void InitSectionData(BOOL bSelectUnSelect);
	void InitThicknessData(BOOL bSelectUnSelect);
	BOOL ImportPropertyData();
	void SortData();
	int GetIndexKey(int nIndex);

	CStringArray m_arPropertyData;

	BOOL ImportMaterialData(BOOL bNumType); 
	BOOL ImportSectionData(BOOL bNumType);
	BOOL ImportThicknessData(BOOL bNumType);

	CArray<UINT,UINT> m_arNewID;
	
	CArray<T_SECTION,T_SECTION> m_arSection;	
	CArray<T_MATERIAL,T_MATERIAL> m_arMaterial;
	CArray<T_THIK_UDRD_D,T_THIK_UDRD_D> m_arThickness;
	CArray<T_SECTION,T_SECTION> m_arSectDesign;	
	CArray<T_MATLDESIGN,T_MATLDESIGN> m_arMatlDesign;

	// Generated message map functions
	//{{AFX_MSG(CCMImportPropertyDlg)
	afx_msg void OnCmdImpropSelectBtn();
	afx_msg void OnCmdImpropRemoveBtn();
	afx_msg void OnCmdImpropSelectAll();
	afx_msg void OnCmdImpropRemoveAll();
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdImpropCancel();
	afx_msg void OnCmdImpropOk();
	afx_msg void OnDblclkCmdImpropPropertyList();
	afx_msg void OnDblclkCmdImpropSelpropertyList();
	afx_msg void OnCmdImpropNumKeep();
	afx_msg void OnCmdImpropNumNew();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMIMPORTPROPERTYDLG_H__EBAF7782_8575_11D4_A678_00010263A1CE__INCLUDED_)
