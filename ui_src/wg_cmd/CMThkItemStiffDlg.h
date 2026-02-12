#if !defined(AFX_CMTHKITEMSTIFFDLG_H__6F715C21_64C6_11D5_880F_00010263A1F4__INCLUDED_)
#define AFX_CMTHKITEMSTIFFDLG_H__6F715C21_64C6_11D5_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMThkItemStiffDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\wg_base_DlgChild.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"

#include "CMSecViewWnd.h"

#include "..\MIT_frx\McomboBox.h"
#include "..\MIT_frx\MillustViewer.h"

// Data Struct for Dlg...
struct Stiff
{
	int ID;
	int nType;
	double dPlateThk;
	double dWeightThk;
	int		nRibPos;
	CString DBName;

	// value
	BOOL	 YZValue_Exist;
	CString YZValue_Shape;
	double YZValue_ThickIn;
	double YZValue_ThickOut;
	double YZValue_Hu;
	double YZValue_Hl;
	BOOL	 XZValue_Exist;
	CString XZValue_Shape;
	double XZValue_ThickIn;
	double XZValue_ThickOut;
	double XZValue_Hu;
	double XZValue_Hl;

	// sect
	BOOL	 YZSect_Exist;
	CString YZSect_Shape;
	CString YZSect_SName;
	double YZSect_Dist;
	double YZSect_Size[10];
	BOOL	 XZSect_Exist;
	CString XZSect_Shape;
	CString XZSect_SName;
	double XZSect_Dist;
	double XZSect_Size[10];

	void Initialize()
	{
		ID = 0;
		nType = 0;
		DBName.Empty();

		// value
		YZValue_Shape = D_SECT_SHAPE_REG_URIB;
		YZValue_ThickIn = 0.0;
		YZValue_ThickOut = 0.0;
		YZValue_Hu = 0.0;
		YZValue_Hl = 0.0;
		XZValue_Shape = D_SECT_SHAPE_REG_URIB;
		XZValue_ThickIn = 0.0;
		XZValue_ThickOut = 0.0;
		XZValue_Hu = 0.0;
		XZValue_Hl = 0.0;

		// sect
		YZSect_Shape = D_SECT_SHAPE_REG_URIB;
		YZSect_SName = D_SECT_SHAPE_REG_URIB;
		YZSect_Dist = 0.0;
		for (int i = 0; i < 10; i++) YZSect_Size[i] = 0.0;
		XZSect_Shape = D_SECT_SHAPE_REG_URIB;
		XZSect_SName = D_SECT_SHAPE_REG_URIB;
		XZSect_Dist = 0.0;
		for (int i = 0; i < 10; i++) XZSect_Size[i] = 0.0;
	}
};


class CCMThkPageItemDlg;
/////////////////////////////////////////////////////////////////////////////
// CCMThkItemStiffDlg dialog

class CCMThkItemStiffDlg : public CDlgChild
{
	// Construction
public:
	CCMThkItemStiffDlg(CWnd* pParent = NULL);   // standard constructor
	~CCMThkItemStiffDlg();

public:
	CCMThkPageItemDlg* m_pParent;
	CDBDoc*			m_pDoc;
	BOOL				m_bInit;
	CDlgChild*	m_pCurYZDlg;
	CDlgChild*	m_pCurXZDlg;
	Stiff				m_sDlgData;
	BOOL				m_bModify;
	mit::frx::MillustViewer m_wndPictureTop;
	mit::frx::MillustViewer m_wndPictureMid;
	mit::frx::MillustViewer m_wndPictureBottom;

	mit::frx::MillustViewer m_wndPictureMidValue;

	CCMSecViewWnd m_wndSecView;

	T_THIK_K		m_Key;
	T_THIK_D		m_Data;
	T_SECT_D		m_SectData;


public:
	void	SetTabDlg(int Type, BOOL flg = FALSE);
	void	DelAllTabs();
	void	InitialDBData(BOOL bOnInitDialog);
	void	ImportDBData();
	void	MakeUnitSystem();
	BOOL	OnApplyPublic();
	void	SetDBNameList();
	int		GetCurStiffType();
	int		GetCurStiffSectionType();
	BOOL	GetCurCalcData(T_THIK_D& thik);
	void	AlignControls();
	void	ChangeBitmap(CString shape);
	void	ChangeBitmap1(int nNum);
	void	InvalidateSecView();

	// Dialog Data
	//{{AFX_DATA(CCMThkItemStiffDlg)
	enum { IDD = IDD_CMD_TP_ITEM_STIFFENED };
	mit::frx::MComboBox 	m_wndDB;
	CTextUnit	m_UnitWeight;
	CTextUnit	m_UnitThkofPlate;
	CEditUnit	m_dWeight;
	CEditUnit	m_dThkofPlate;
	CDlgTabCtrl	m_wndTab;
	int		m_StiffType;
	int		m_nID;
	int		m_RibPos;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMThkItemStiffDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	CArray<UINT, UINT> m_aThikPlate, m_aThikWeight;
	CArray<UINT, UINT> m_aValuePic, m_aUserPic;

	// Generated message map functions
	//{{AFX_MSG(CCMThkItemStiffDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeStiffType();
	afx_msg void OnChangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeforeChangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeCmdTpStiffCode();
	afx_msg void OnSelChangeRibPos();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMTHKITEMSTIFFDLG_H__6F715C21_64C6_11D5_880F_00010263A1F4__INCLUDED_)

