#if !defined(AFX_CMTHKPAGEITEMDLG_H__50132E62_1E8E_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_CMTHKPAGEITEMDLG_H__50132E62_1E8E_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMThkPageItemDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\wg_base_DlgChild.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\MIT_frx\MButton.h"

class CCMThkItemValueDlg;
class CCMThkItemStiffDlg;
class CCMThkItemStlWallDlg;
/////////////////////////////////////////////////////////////////////////////
// CCMThkPageItemDlg dialog

/////////////////////////////////////
#include "HeaderPre.h"      /////////
/////////////////////////////////////

class __MY_EXT_CLASS__ CCMThkPageItemDlg : public CDialogMove
{
	// Construction
public:
	CCMThkPageItemDlg(CDBDoc* pDoc, CWnd* pParent = NULL);   // standard constructor

	// Attributes
public:
	CDBDoc*		m_pDoc;
	BOOL			m_bModify;
	BOOL			m_bWorkTab;
	int				m_FixID;
	CCMThkItemValueDlg*	m_pDlgValue;
	CCMThkItemStiffDlg*	m_pDlgStiff;
	CCMThkItemStlWallDlg*	m_pDlgStlWall;

	T_THIK_K	m_Key;
	T_THIK_D	m_data;

	// Operations
public:
	void	SetModifyData(T_THIK_K Key, T_THIK_D &Data);
	void	SetThicknessID(UINT key);
	void	SetDlg2Data(T_THIK_K &Key, T_THIK_D &Data);
	void	SetData2Dlg();
	void	EnableDisableCalcBtn(BOOL flg);
	BOOL	SaveData();

	// Dialog Data
	//{{AFX_DATA(CCMThkPageItemDlg)
	enum { IDD = IDD_CMD_TP_ITEM_DLG };
	CDlgTabCtrl	m_wndTabCtrl;
	mit::frx::MButton 	m_wndBtnApply;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMThkPageItemDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMThkPageItemDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnApply();
	afx_msg void OnCmdTpItemBtnCalc();
	afx_msg void OnSelchangeCmdTabCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////
#include "HeaderPost.h"      ////////
/////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMTHKPAGEITEMDLG_H__50132E62_1E8E_11D3_92DE_0000C0B0E6B3__INCLUDED_)


