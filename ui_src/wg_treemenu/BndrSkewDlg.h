#if !defined(AFX_BNDRSKEWDLG_H__863A3A81_6AA8_11D4_92DF_0000C0B0E6B3__INCLUDED_)
#define AFX_BNDRSKEWDLG_H__863A3A81_6AA8_11D4_92DF_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BndrSkewDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\mit_frx\MillustViewer.h"
#include "..\mit_frx\MComboBox.h"
/////////////////////////////////////////////////////////////////////////////
// CBndrSkewDlg dialog

class CBndrSkewDlg : public CMenuBarChildDlg
{
// Construction
public:
	CBndrSkewDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBndrSkewDlg();
	virtual void Execute();
	virtual BOOL ExternalInit(UINT key);
											 //(T_BNGR_K *)

public:
	void ChangeMethod(int nMethod);

// Dialog Data
	//{{AFX_DATA(CBndrSkewDlg)
	enum { IDD = IDD_TM_BNDR_SKEW };
	//}}AFX_DATA
	mit::frx::MComboBox  AngleCobx[3];

	mit::frx::MComboBox  m_CmbInputMethod;
	mit::frx::MComboBox  m_CmbLineVectorDirection;
	mit::frx::MComboBox  m_CmbRefLineP1;

	CMouseEdit m_LineVectorEdt[2];
	CTextUnit  m_LineVectorUnit[2];

	CMouseEdit Pt3Edit[3];
	CTextUnit  Pt3Unit[3];
	CMouseEdit VtEdit[2];
	

	CArray<UINT, UINT> m_aCtrlOption;
	CArray<UINT, UINT> m_aCtrlDisable;  //Option Delete로 설정 됬을때..
	CArray<UINT, UINT> m_aCtrlAngle;
	CArray<UINT, UINT> m_aCtrl3Points;
	CArray<UINT, UINT> m_aCtrlVector;
	CArray<UINT, UINT> m_aCtrlLineVectorDirection;
	CArray<UINT, UINT> m_aCtrlLineVector;

	CArray<UINT, UINT> m_aCtrlRefLineRdo;
	CArray<UINT, UINT> m_aGlobalDirectionGroup; 
	CArray<UINT, UINT> m_aRefPointGroup; 
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBndrSkewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void AlignControl(int nMethod);
	void ChangeBitmap();
	void InitInputMethodCmb();
	void InitDirectionCmb();
	void InitGlobalDirectionCmb();
	mit::frx::MillustViewer m_wndPicture;
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CBndrSkewDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmClose();
	afx_msg void OnChangeOption();
	afx_msg void OnChangeMethod();
	afx_msg void OnChangeRefOption();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BNDRSKEWDLG_H__863A3A81_6AA8_11D4_92DF_0000C0B0E6B3__INCLUDED_)
