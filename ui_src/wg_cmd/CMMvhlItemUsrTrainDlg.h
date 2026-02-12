#if !defined(AFX_CMMVHLITEMUSRTRAINDLG_H__9DB250B1_C326_4DD1_B287_100DF9026AEF__INCLUDED_)
#define AFX_CMMVHLITEMUSRTRAINDLG_H__9DB250B1_C326_4DD1_B287_100DF9026AEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhlItemUsrTrainDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUsrTrainDlg dialog

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "MvhlItemGrid.h"

class CCMMvhlItemUsrTrainDlg : public CInternationalDlg
{
// Construction
public:
	CCMMvhlItemUsrTrainDlg(T_MVHL_D  *pData, CWnd* pParent = NULL);   // standard constructor

	void	SetData2Dlg();
	BOOL	SetDlg2Data();

// Dialog Data
	//{{AFX_DATA(CCMMvhlItemUsrTrainDlg)
	enum { IDD = IDD_CMD_ML_MVHL_ITEM_TRAIN_CH };
	CTextUnit	m_unitFD;
	CTextUnit	m_unitDD;
	CTextUnit	m_unitBD;
	CEditUnit	m_editFD;
	CEditUnit	m_editDD;
	CEditUnit	m_editBD;
	//}}AFX_DATA

	T_MVHL_D  *m_pData;
	T_MVHL_D  m_pMainData;
	T_MVHL_D  m_pHeavyData;

	CMvhlItemGrid m_wndMainGrid;
	CMvhlItemGrid m_wndHeavyGrid;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvhlItemUsrTrainDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMMvhlItemUsrTrainDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMVHLITEMUSRTRAINDLG_H__9DB250B1_C326_4DD1_B287_100DF9026AEF__INCLUDED_)
