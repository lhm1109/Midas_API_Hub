#if !defined(AFX_CMSECTITEMDLG_H__673754E3_FBDD_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_CMSECTITEMDLG_H__673754E3_FBDD_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\MIT_frx\MTabCtrl.h"
#include "..\MIT_frx\MButton.h"	

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemDlg dialog
#include "HeaderPre.h"

using namespace mit::frx;
class __MY_EXT_CLASS__ CCMSectItemDlg : public CDialogMove
{
//---------------------------------------------------
// Construction
//---------------------------------------------------
public:
	CCMSectItemDlg(CWnd* pParent = NULL);   // standard constructor

//---------------------------------------------------
// Interface
//---------------------------------------------------
public:
	bool		m_bWorkTab;
	int			m_FixID;
	int m_nActiveTab;

	void SetMode(BOOL bDesign) { m_bDesign = bDesign; }
	void SetVirual(BOOL bVirtual) { m_bVirtual = bVirtual; }
	void SetModifyData(T_SECT_K Key, T_SECT_D &Data);
	void SetSectionID(UINT key);
	//Add hong,js 03/03/12 for CODS_Stfdgn_IndiSect_Grid
	void SetRetunData(void* &pData);
	void ExternalSetCalcBtn(BOOL bSet);
	void *m_pVoidData;

	void SetActiveTab(int selectTab);
//---------------------------------------------------
// Implementation
//---------------------------------------------------
protected:
	BOOL SetDialogPageList();
	int  GetDialogPagePos();
	BOOL ShowDialogPage(int nPagePos);
	void ShowErrorAndClose();
	void EnableDisableCalcBtn();

protected:
	CDBDoc*           m_pDoc;
	BOOL              m_bModify;
	T_SECT_K          m_Key;
	T_SECT_D          m_Data;

private:
	CArray<int, int>  m_aSectType;
	BOOL              m_bDesign;
	BOOL              m_bVirtual;
	BOOL              m_bDestory;

// Dialog Data
	//{{AFX_DATA(CCMSectItemDlg)
	enum { IDD = IDD_CMD_SECT_DLG };
	MButton	m_wndCalcBtn;
 	MButton	m_wndBtnApply;
	CDlgTabCtrl	m_wndTabCtrl;
	//mit::frx::MTabCtrl m_wndTabCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMSectItemDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnCalculate();
	afx_msg void OnBtnOK();
	afx_msg void OnBtnApply();
	afx_msg void OnChangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeforeChangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSECTITEMDLG_H__673754E3_FBDD_11D3_92DE_0000C0B0E6B3__INCLUDED_)
