#if !defined(AFX_DGNGENMEMBERTYPEDLG_H__1A710801_274C_11D4_888F_0000C0F30D4D__INCLUDED_)
#define AFX_DGNGENMEMBERTYPEDLG_H__1A710801_274C_11D4_888F_0000C0F30D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnGenMemberTypeDlg.h : header file
//
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnGenMemberTypeDlg dialog

class CDgnGenMemberTypeDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnGenMemberTypeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnGenMemberTypeDlg)
	enum { IDD = IDD_DGN_GEN_MTYP_DLG };
	int		m_nOption;
	int		m_nElemType;
	int		m_nMType_Ch_Rad;
	int		m_nCol_Rad;
	int		m_nBeam_Rad;
	int		m_nWall_Rad;
	int		m_nBrace_Rad;//add by maxiao 2015-10-14
	//}}AFX_DATA

public:
	BOOL ErrorCheck();
	void Initial_SelectItem();
	void Initial_Data(BOOL bUseSubType); //Ch Hong,js 03/01/02
	virtual void OnUpdate(CView* pView,LPARAM lHint,CObject* pHint);
	virtual void UpdateBuffer();
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnGenMemberTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	//ADD Hong,js 03/01/02
	BOOL m_bUseSubType;
	int m_CurSubType_bottom; //현재 활성화된 아이템의 바닥 좌표.
	BOOL bFirstDlg;
	CArray<UINT, UINT> m_arMCtrl_MType;
	CArray<UINT, UINT> m_arMCtrl_MType_Ch;
	CArray<UINT, UINT> m_arMCtrl_SubType[4];// 0:Beam, 1:Column, 2:Wall, 3:Brace 
	CArray<UINT, UINT> m_arBtn;

	void UpdateData_SubType();
	void UpdateData_NonSubType();
	void Show_MainItem(BOOL bUseSubType);
	void Show_SubItem(BOOL bBeam, BOOL bCol, BOOL bWall, BOOL bBrace=FALSE);
	void AlignItems(int iOption); 
	void EnableItems(BOOL bEnable);

	// Generated message map functions
	//{{AFX_MSG(CDgnGenMemberTypeDlg)
	virtual void Execute();
	afx_msg void OnDgnEtypeAr();
	afx_msg void OnDgnEtypeDel();
	afx_msg void OnDgnEtypeBeam();
	afx_msg void OnDgnEtypeBrace();
	afx_msg void OnDgnEtypeColumn();
	afx_msg void OnDgnEtypeTruss();//add by maxiao 2015-10-14
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnEtypeBeamCh();
	afx_msg void OnDgnEtypeBraceCh();
	afx_msg void OnDgnEtypeColumnCh();
	afx_msg void OnDgnEtypeWallCh();
	afx_msg void OnDgnEtypeTrussCh();//add by maxiao 2015-10-14
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNGENMEMBERTYPEDLG_H__1A710801_274C_11D4_888F_0000C0F30D4D__INCLUDED_)
