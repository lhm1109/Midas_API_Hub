#if !defined(__DGNCONMESHEDSLABWALLDLG_H__)
#define __DGNCONMESHEDSLABWALLDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConMeshedSlabWallDlg.h : header file
//

#include "..\wg_base\wg_base_NotifyCtrl.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectCtrl.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "DgnDlgBase.h"
#include "wg_dgnRes2.h"

#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"
#include "..\wg_base\SpinBtnExCtrl.h"

#include "HeaderPre.h"

using namespace mit::frx;
class CDBDoc;
class CDgnSlabRebarPictureDlg;
/////////////////////////////////////////////////////////////////////////////
// CDgnConMeshedSlabWallDlg dialog

class __MY_EXT_CLASS__ CDgnConMeshedSlabWallDlg : public CDgnDlgBase, public CDBUpdateConnector
{
// Construction
public:
	CDgnConMeshedSlabWallDlg(CWnd* pParent = NULL);
	virtual ~CDgnConMeshedSlabWallDlg();

// Dialog Data
	//{{AFX_DATA(CDgnConMeshedSlabWallDlg)
	enum { IDD = IDD_DGN_CON_MESHED_SLAB_WALL_DLG };
	MEdit m_edtName;
	CSelectEdit m_edtElem;
	CListCtrl m_List;

	// Slab
	MComboBox m_cmbBasicName;
	MComboBox m_cmbBasicSpace;
	MComboBox m_cmbAdd1Name;
	MComboBox m_cmbAdd1Space;
	MComboBox m_cmbAdd2Name;
	MComboBox m_cmbAdd2Space;

	CEditUnit m_edtSlabCover;
	CTextUnit m_untSlabCover;

	// Wall
	MComboBox m_cmbVerName;
	MComboBox m_cmbVerSpace;
	MComboBox m_cmbHorName;
	MComboBox m_cmbHorSpace;

	CEditUnit m_edtDw;
	CTextUnit m_untDw;

	// Slab/Mat Punching Shear
	MComboBox m_cmbPunchRbarName ;
	MComboBox m_cmbPunchRbarSpace;
	MComboBox m_cmbPunchStudName ;
	MComboBox m_cmbPunchStudSpace;

	// Strip
	MComboBox m_cmbStripName_I;
	MComboBox m_cmbStripName_C;
	MComboBox m_cmbStripName_J;
	MComboBox m_cmbStripSpace_I;
	MComboBox m_cmbStripSpace_C;
	MComboBox m_cmbStripSpace_J;
	CEditUnit m_edtStripCover;
	CTextUnit m_untStripCover;

	// Shell
	MComboBox m_cmbShellBasicName; 
	MComboBox m_cmbShellBasicSpace;
	MComboBox m_cmbShellAdd1Name;
	MComboBox m_cmbShellAdd1Space;
	MComboBox m_cmbShellAdd2Name;
	MComboBox m_cmbShellAdd2Space;
	CEditUnit m_edtShellCover;
	CTextUnit m_untShellCover;
	MButton m_chkAuto;	
	CFormulaEditSpin m_edtItr;
	CEditUnit m_edtTop;
	CEditUnit m_edtBot;
	CEditUnit m_edtTol;

	//}}AFX_DATA

protected:
	BOOL m_bInit;

	CArray<UINT,UINT> m_aMembTypeRdo; // 0 : slab,    1 : wall,    2 : strip,   3: Shell
	CArray<UINT,UINT> m_aSlabCtrls;
	CArray<UINT,UINT> m_aWallCtrls;
	CArray<UINT,UINT> m_aSlabLayerRdo;
	CArray<UINT,UINT> m_aSlabPunchCtrls;
	CArray<UINT,UINT> m_aStripCtrls;
	CArray<UINT,UINT> m_aShellCtrls;
	CArray<UINT,UINT> m_aStripLayerRdo;
	CArray<UINT,UINT> m_aPunchRdo;
	CArray<UINT,UINT> m_aPunchRebrCtrls;
	CArray<UINT,UINT> m_aPunchStudCtrls;
	CArray<UINT,UINT> m_aStripRebrCJCtrls; // Center, J 컨트롤
	CArray<UINT,UINT> m_aShellLayerRdo;

	T_RBMW_D m_RbmwD;
	T_RBMS_D m_RbmsD;
	T_RBST_D m_RbstD;
	T_RBSL_D m_RbslD;  

	CDBDoc* m_pDoc;
	CDgnSlabRebarPictureDlg* m_pPictureDlg;
	BOOL m_bPrevUnitType;      // 이전 단위계  0 : m/mm/cm,     1: in/ft
	BOOL m_bOriginMstrDispOpt; // 이전 strip display option
	BOOL m_bStripSameRebarSize;

public:
	void ShowSelectedRbmw(T_RBMW_K RbmwK, BOOL bInit/*=TRUE*/);
	void ShowSelectedRbms(T_RBMS_K RbmsK, BOOL bInit/*=TRUE*/);
	void ShowSelectedRbst(T_RBST_K RbstK, BOOL bInit/*=TRUE*/);
	void ShowSelectedRbsl(T_RBSL_K RbslK, BOOL bInit/*=TRUE*/);
	void SetWallTypeDlg();
	void SetStripTypeDlg();
	void SetShellTypeDlg();

protected:
	void InitCtrls();
	void ChangeCaption();
	void EnableDisableCtrls();

	void InitRebarNameComboBox();
	void InitRebarSpaceComboBox();

	double ConvertSpaceString2Data(CString& strSpace, BOOL bUnitChange=FALSE);
	CString ConvertSpaceData2String(double dSpace);
	void Data2Dlg();
	BOOL Dlg2Data();

	void SetListCtrlHeader();
	BOOL GetSelectedItem(CString& strName);
	int GetSelectedItems(CStringArray& aStrName);
	void MakeItemEx();
	void MakeSlabRebarItem();
	void AlignControl();
	void ShowControl();
	void ShowStripHideWall();
	void SetMstrDispOpt(BOOL bSet);
	void SelectStrip(CArray<T_MSTR_K, T_MSTR_K>& aMstrK);
	void RestoreOriginMstrDispOpt();
	void OnClickStripSameSizeChk();
	void MakeShellRebarItem();
	void SetTitle();

public:
	static unsigned int WM_AFTERINITDIALOG;
	BOOL m_bShowStrip;

	BOOL IsAlivePictureDlg();
	
	void CreatePictureDlg();
	void DestroyPictureDlg();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConMeshedSlabWallDlg)
public:
	virtual BOOL Create(UINT nIDTemplate, CWnd* pParentWnd=NULL);
	virtual BOOL DestroyWindow();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

public:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();
// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnConMeshedSlabWallDlg)
	virtual BOOL OnInitDialog();  
	afx_msg void OnClkListItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMembTypeRdo();
	afx_msg void OnItemChangedList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDgnAddReplace();
	afx_msg void OnDgnDelete();
	afx_msg void OnDgnClose();
	afx_msg void OnSlabLayerRdo();  
	afx_msg void OnStripLayerRdo();  
	afx_msg void OnShellLayerRdo();  	
	afx_msg void OnChkAuto();
	//}}AFX_MSG
	afx_msg LRESULT OnAfterInitDialog(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNCONMESHEDSLABWALLDLG_H__)
