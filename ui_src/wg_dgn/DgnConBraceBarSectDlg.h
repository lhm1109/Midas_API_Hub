#if !defined(AFX_DGNCONBRACEBARSECTDLG_I_H__0E038880_1B43_4058_B4ED_8DE2CE7BE12F__INCLUDED_)
#define AFX_DGNCONBRACEBARSECTDLG_I_H__0E038880_1B43_4058_B4ED_8DE2CE7BE12F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConBraceBarSectDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDgnConBraceBarSectDlg dialog
#include "..\MIT_frx\MComboBox.h"

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
class __MY_EXT_CLASS__ CDgnConBraceBarSectDlg : public CChildDialog
{

// Construction
public:
	CDgnConBraceBarSectDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDgnConBraceBarSectDlg();
	
	//virtual void UpdateBuffer();

	void SetCreationType();
// Dialog Data
	//{{AFX_DATA(CDgnConBraceBarSectDlg)
	enum { IDD = IDD_DGN_CON_BRACE_BAR_SECT_DLG };
	mit::frx::MComboBox	m_ctrMainCombo;
	mit::frx::MComboBox	m_ctrTiesCombo;
	mit::frx::MComboBox	m_ctrYCombo;
	mit::frx::MComboBox	m_ctrZCombo;
	CEditUnit	m_ctrDo;
	CTextUnit	m_ctrDoUnit;
	BOOL	m_bApplyAIJ;	
	int		m_nOption;
	int   m_nAddReplace;
	// Add by ZINU.('08.03.21). NO:3292, Option to Apply Spacing Limit.
	BOOL    m_bConsiderSpacingLimitBrace;
	int		  m_nDgnClass;

	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConBraceBarSectDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnUseAIJCheck();
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
		
protected:

	CDgnConBarSectDlg* m_pParent;
	CDBDoc* m_pDoc;

	T_DCBR_D m_CurData;	

	void SetInitUnit();
	BOOL Dlg2Data();
	BOOL Data2Dlg();
	
	void SetAIJControl();

	void SetShowHideControl();

	virtual void OnUpdate(CView* pView,LPARAM lHint,CObject* pHint);
	virtual void UpdateBuffer();
	
	
protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnConBraceBarSectDlg)	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////
#include "HeaderPost.h"     /////////
/////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNCONBRACEBARSECTDLG_I_H__0E038880_1B43_4058_B4ED_8DE2CE7BE12F__INCLUDED_)
