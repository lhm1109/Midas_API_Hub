#if !defined(AFX_CMMVHLITEMUSRTRCKDLG_H__0B49704C_3245_48F7_983A_8E0635019E40__INCLUDED_)
#define AFX_CMMVHLITEMUSRTRCKDLG_H__0B49704C_3245_48F7_983A_8E0635019E40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhlItemUsrTrckDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUsrTrckDlg dialog

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\wg_base\SpinBtnExCtrl.h"

using namespace mit::frx;

#include "MvhlItemGrid.h"

class CCMMvhlItemUsrTrckDlg : public CInternationalDlg
{

// data pointer
	T_MVHL_D  *m_pData;
	T_MVHL_D   m_DataBuffForGrid;
	double     m_dDlgH;
// Construction
public:
	CCMMvhlItemUsrTrckDlg(T_MVHL_D  *pData, CWnd* pParent = NULL);   // standard constructor

	void	SetData2Dlg();
	BOOL	SetDlg2Data();
	double  GetDlgH();
	void	SetUnitCtrl(void);
	void  CtrlShowHide();

	CString	m_unitLoad;
	CString	m_unitDist;
	int     m_nTrainType;
	int     m_bChSubway;
// Dialog Data
	//{{AFX_DATA(CCMMvhlItemUsrTrckDlg)
	enum { IDD = IDD_CMD_ML_MVHL_ITEMU_TRAIN };
	CTextUnit	m_stW2UT;
	CTextUnit	m_stW1UT;
	CTextUnit	m_stD2UT;
	CTextUnit	m_stD1UT;
	CString	m_sD1;
	CString	m_sD2;
	CString	m_sW1;
	CString	m_sW2;
	CTextUnit m_untIrcNtDist;
	CTextUnit	m_untIrcD1;
	CTextUnit	m_untIrcD2;
	CTextUnit	m_untIrcP;
	CTextUnit	m_untIrcD;
	CString   m_sIrcNtDist;
	CString	  m_sIrcD1;
	CString	  m_sIrcD2;
	CString	  m_sIrcP;
	CString	  m_sIrcD;
	CEditUnit m_edtP1;
	CEditUnit	m_edtP2;
	CEditUnit	m_edtP3;
	CEditUnit	m_edtP4;
	CEditUnit	m_edtPo;
	CEditUnit m_edtD1;
	CEditUnit	m_edtD2;
	CEditUnit	m_edtD3;
	CEditUnit	m_edtD;
	CTextUnit m_untP1;
	CTextUnit	m_untP2;
	CTextUnit	m_untP3;
	CTextUnit	m_untP4;
	CTextUnit	m_untPo;
	CTextUnit m_untD1;
	CTextUnit	m_untD2;
	CTextUnit	m_untD3;
	CTextUnit	m_untD;
	//CSpinButtonCtrl m_spnCrowd;
	CFormulaEditSpin	m_edtN;
	CString m_strIfr;
	//}}AFX_DATA

	CMvhlItemGrid m_wndGrid;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvhlItemUsrTrckDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void AlignCtrl();
		
	int m_nMovingType;
	CArray<UINT, UINT> m_aCtrlCom;
	CArray<UINT, UINT> m_aCtrlIrc;
	CArray<UINT, UINT> m_aCtrlSub;
	// Generated message map functions
	//{{AFX_MSG(CCMMvhlItemUsrTrckDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMVHLITEMUSRTRCKDLG_H__0B49704C_3245_48F7_983A_8E0635019E40__INCLUDED_)
