#if !defined(AFX_DGNCONWALLBARSECTDLG_I_H__0E038880_1B43_4058_B4ED_8DE2CE7BE12F__INCLUDED_)
#define AFX_DGNCONWALLBARSECTDLG_I_H__0E038880_1B43_4058_B4ED_8DE2CE7BE12F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConWallBarSectDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDgnConWallBarSectDlg dialog
#include "..\MIT_frx\MComboBox.h"

#include "..\wg_base\NotifyCtrl.h"	// for DBUpdate.
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
class __MY_EXT_CLASS__ CDgnConWallBarSectDlg : public CChildDialog, public CDBUpdateConnector
{

// Construction
public:
	CDgnConWallBarSectDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDgnConWallBarSectDlg();
	
	//virtual void UpdateBuffer();

	void SetCreationType();
// Dialog Data
	//{{AFX_DATA(CDgnConWallBarSectDlg)
	enum { IDD = IDD_DGN_CON_WALL_BAR_SECT_DLG };

	mit::frx::MComboBox	m_ctrVerCombo;
	mit::frx::MComboBox	m_ctrHorCombo;
	mit::frx::MComboBox	m_ctrEndCombo;
	mit::frx::MComboBox	m_ctrBEHorCombo;

	CEditUnit	m_ctrBEHorSpaceEdt;
	CEditUnit	m_ctrBEVerSpaceEdt;
	CEditUnit	m_ctrDE;
	CEditUnit	m_ctrDW;

	CTextUnit m_ctrBEHorSpaceUnit;
	CTextUnit m_ctrBEVerSpaceUnit;
	CTextUnit	m_ctrDEUnit;
	CTextUnit	m_ctrDWUnit;

	BOOL	m_bApplyAIJ;	
	int   m_nAddReplace;	
	// Add by sshan (090222) NO:3909
	BOOL    m_bVerEndWall;
	mit::frx::MComboBox    m_ComboVerEndWall;
	mit::frx::MComboBox    m_ComboVerEndFyWall;
	int		  m_nDgnClass;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConWallBarSectDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	// Add by sshan (090222) NO:3909
	afx_msg void OnDgnRcVerEndWall();
	
	//}}AFX_VIRTUAL


// Implementation

public:
	
	void SaveDlgData();
	void Initial_SelectItem();
	void ApplyData();

	void InitDlgData();
	void SetRebarCombo();
		
protected:

	CDgnConBarSectDlg* m_pParent;
	CDBDoc* m_pDoc;

	T_DCBW_D m_CurData;

	void SetInitUnit();
	BOOL Dlg2Data();
	BOOL Data2Dlg();
	
	void SetAIJControl();

	void SetControlEnable();

	void SetBEControlByRCSpecialWall();

	BOOL Is_KCIUSD07_KS_KS01RC();
	void AlignCtls();

	void SetShowHideControl();

	virtual void OnUpdate(CView* pView,LPARAM lHint,CObject* pHint);
	virtual void UpdateBuffer();
		
protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnConWallBarSectDlg)	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////
#include "HeaderPost.h"     /////////
/////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNCONWALLBARSECTDLG_I_H__0E038880_1B43_4058_B4ED_8DE2CE7BE12F__INCLUDED_)
