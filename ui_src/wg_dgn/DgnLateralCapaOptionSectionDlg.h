#if !defined(AFX_DGNLATERALCAPAOPTIONSECTIONDLG_H__EA974AF8_B99D_475E_9BAA_FF653E6950FB__INCLUDED_)
#define AFX_DGNLATERALCAPAOPTIONSECTIONDLG_H__EA974AF8_B99D_475E_9BAA_FF653E6950FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnLateralCapaOptionSectionDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
//#include "..\wg_base\wg_base_ChildDialog.h"
#include "..\wg_base\wg_base_DlgChild.h"
#include "DgnLateralCapaOptionSectGrid.h"
#include "DgnLateralCapaOptionDef.h"
#include "..\wg_base\NotifyCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnLateralCapaOptionSectionDlg dialog
class CDgnLateralCapaOptionDlg;
class CDgnLateralCapaOptionSectionDlg : public CDlgChild, public CDBUpdateConnector
{
// Construction
public:
	CDgnLateralCapaOptionSectionDlg(LateralCapaDataAll* pDataAll, CWnd* pParent = NULL);   // standard constructor
	~CDgnLateralCapaOptionSectionDlg();  // destructor

public:
	CDBDoc* m_pDoc;
	CDgnLateralCapaOptionDlg *m_pParent;
	CDgnLateralCapaOptionSectGrid *m_pGrid;

	LateralCapaDataAll* m_pDataAll;
	CArray<T_DGN_SECT_DATA, T_DGN_SECT_DATA&> m_aData;
	CArray<UINT, UINT> m_aReinforceCtrl;

	BOOL m_bCodeExist[6];

// Dialog Data
	//{{AFX_DATA(CDgnLateralCapaOptionSectionDlg)
	enum { IDD = IDD_DGN_LATERAL_CAPA_OPT_SECTION_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnLateralCapaOptionSectionDlg)
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
	void GetDefaultSectData(T_SECT_K SectK, T_DGN_SECT_DATA &tempD);

	// Generated message map functions
	//{{AFX_MSG(CDgnLateralCapaOptionSectionDlg)
	afx_msg void OnColumnBtn();
	afx_msg void OnColumnGenBtn();
	afx_msg void OnResetBtn();
	afx_msg void OnDestroy();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNLATERALCAPAOPTIONSECTIONDLG_H__EA974AF8_B99D_475E_9BAA_FF653E6950FB__INCLUDED_)
