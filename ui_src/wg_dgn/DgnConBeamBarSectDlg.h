#if !defined(AFX_DGNCONBEAMBARSECTDLG_I_H__0E038880_1B43_4058_B4ED_8DE2CE7BE12F__INCLUDED_)
#define AFX_DGNCONBEAMBARSECTDLG_I_H__0E038880_1B43_4058_B4ED_8DE2CE7BE12F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConBeamBarSectDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDgnConBeamBarSectDlg dialog
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MEdit.h"

#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_base\ChildDialog.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\DB_ST_DN.h"

//#include "DgnConBarSectDlg.h"

//#include "DgnStruct.h"

/////////////////////////////////////
#include "HeaderPre.h"      /////////
/////////////////////////////////////

class CDgnConBarSectDlg;
class __MY_EXT_CLASS__ CDgnConBeamBarSectDlg : public CChildDialog, public CDBUpdateConnector
{

// Construction
public:
	CDgnConBeamBarSectDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDgnConBeamBarSectDlg();

	//virtual void UpdateBuffer();

	// Dialog Data
	//{{AFX_DATA(CDgnConBeamBarSectDlg)
	enum { IDD = IDD_DGN_CON_BEAM_BAR_SECT_DLG };
	mit::frx::MComboBox	m_ctrMainCombo;
	mit::frx::MComboBox	m_ctrStirCombo;
	mit::frx::MComboBox	m_ctrArraCombo;
	mit::frx::MComboBox	m_ctrSideCombo;
	CEditUnit	m_ctrDT;
	CEditUnit	m_ctrDB;
	CTextUnit	m_ctrDTUnit;
	CTextUnit	m_ctrDBUnit;
	BOOL	  m_bApplyAIJ;
	int		  m_nAIJ;
	int		  m_nOption;
	int     m_nAddReplace;
	// Add by sshan. MNET:2619.('20070126)
	mit::frx::MEdit	  m_DoublyReinEdit;
	BOOL	  m_bDoublyRein;
	double  m_dDoublyRein;
	// Add by ZINU.('08.03.21). NO:3292, Option to Apply Spacing Limit.
	BOOL    m_bConsiderSpacingLimitBeam;
	int		  m_nDgnClass;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConBeamBarSectDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnUseAIJCheck();
	afx_msg void OnDgnDoublyReinCheck();
	// Add by ZINU.('08.03.21). NO:3292, Option to Apply Spacing Limit.
	afx_msg void OnDgnConsiderSpacingLimitCheck();
	//}}AFX_VIRTUAL


// Implementation

public:
	void SaveDlgData();
	void Initial_SelectItem();
	void ApplyData();

	void InitDlgData();	
	void SetRebarCombo();
	void InitData();	

	void ControlMan(BOOL bInit);
	
protected:
	CDgnConBarSectDlg* m_pParent;
	CDBDoc* m_pDoc;

	T_DCBB_D m_CurData;
	
	void SetInitUnit();
	BOOL Dlg2Data();
	BOOL Data2Dlg();

	void SetAIJControl();
	
	virtual void OnUpdate(CView* pView,LPARAM lHint,CObject* pHint);
	virtual void UpdateBuffer();
	
protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnConBeamBarSectDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////
#include "HeaderPost.h"     /////////
/////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNCONBEAMBARSECTDLG_I_H__0E038880_1B43_4058_B4ED_8DE2CE7BE12F__INCLUDED_)
