//CMWindItemChina2019CrossDlg .h : header file
// Add by maxiao 2011-5-10  For china GB 50009-2011


#if !defined(AFX_CMWindItemChina2019CrossDlg_H__04C9E1E2_3D38_11D5_880F_00010263A1F4__INCLUDED_)
#define AFX_CMWindItemChina2019CrossDlg_H__04C9E1E2_3D38_11D5_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\DB_ST_DT_WIND.h"

/////////////////////////////////////////////////////
////////////////////////
// CMWindItemChina2019CrossDlg  dialog
class CMWindItemChina2019CrossDlg  : public CDialogMove
{
// Construction
public:
	CMWindItemChina2019CrossDlg (CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMWindItemChina2019CrossDlg )
	enum { IDD = IDD_ETC_WIND_ITEM_CHINA2019_VIBRATION_DLG };
	
	//CTextUnit	m_WindForceUnit;
	//}}AFX_DATA
	CEditUnit	m_editCrossPeriodX;
	CEditUnit	m_editCrossPeriodY;
	CEditUnit	m_editPeriodTorsion;

	int m_nRoundOrRectRidio;
	int m_nCrossCheck;
	int m_nTorsionCheck;
	
	double m_dCrossPeriodX;
	double m_dCrossPeriodY;
	double m_dPeriodTorsion;

	void SetData(T_WIND_CH2019& mydata);

	//BOOL OnInitDialog(T_CHINA_WIND_TRANS TransWind);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMWindItemChina2019CrossDlg )
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMWindItemChina2019CrossDlg )
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckCross();	
	afx_msg void OnCheckRadio();	
	afx_msg void OnCheckTorsion();	
	virtual void OnCancel();
	virtual void OnOk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMWindItemChina2019CrossDlg_H__04C9E1E2_3D38_11D5_880F_00010263A1F4__INCLUDED_)
