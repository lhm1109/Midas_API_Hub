#if !defined(__MPHI_SECTION_DLG_JP_H__)
#define __MPHI_SECTION_DLG_JP_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMPhiSectionDlg_JP.h : header file
//
#include "MPhiSectDetailGrid_JP.h"
#include "..\wg_base\wg_base_DlgChild.h"


#include "HeaderPre.h"

/////////////////////////////////////////////////////////////////////////////
// CMPhiSectionDlg_JP dialog

class CDBDoc;
class CMPhiSectGrid_JP;
class CMPhiSectDetailGrid_JP;

struct MPhiDataAll;

 class __MY_EXT_CLASS__ CMPhiSectionDlg_JP : public CDlgChild, public CDBUpdateConnector
{
// Construction
public:
	CMPhiSectionDlg_JP(MPhiDataAll* pMPhiDataAll, CWnd* pParent = NULL);   // standard constructor
	~CMPhiSectionDlg_JP();  // destructor

public:
	CDBDoc* m_pDoc;
	CMPhiSectGrid_JP* m_pGrid;
	CMPhiSectDetailGrid_JP* m_pDetailGrid;
	MPhiDataAll* m_pMPhiDataAll;
	CArray<T_SECT_DATA_JP, T_SECT_DATA_JP&> m_aData;
	CArray<UINT, UINT> m_aReinforceCtrl;

// Dialog Data
	//{{AFX_DATA(CMPhiSectionDlg_JP)
	enum { IDD = IDD_CMD_MPHI_SECTION_JP_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMPhiSectionDlg_JP)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	//}}AFX_VIRTUAL

// Implementation
public:
	//virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void MyUpdate(LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer(); 
	BOOL ModifyItem(T_SECT_K Key, T_SECT_D &mData);
	void Dlg2Data();
	BOOL InitData();
	void SetDlgDataToParent();

protected:
	BOOL InitGrid();
	

	void GetDefaultSectData(T_SECT_K SectK, T_SECT_DATA_JP&tempD);
	// Generated message map functions
	//{{AFX_MSG(CMPhiSectionDlg_JP)
	afx_msg void OnReinBtn();
	afx_msg void OnRebarBtn();
	afx_msg void OnInitBtn();

	afx_msg void OnDestroy();
	virtual void OnCancel();
	
	virtual BOOL OnInitDialog();
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL m_bCodeExist[6];
};
#include "HeaderPost.h"

 //{{AFX_INSERT_LOCATION}}
 // Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__MPHI_SECTION_DLG_JP_H__)
