#if !defined(AFX_DGNSRCSECTDLG_H__5D7AB3A7_D4E8_11D3_888F_0000C0F30D4D__INCLUDED_)
#define AFX_DGNSRCSECTDLG_H__5D7AB3A7_D4E8_11D3_888F_0000C0F30D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSrcSectDlg.h : header file
//
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_base\SpinBtnExCtrl.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_EditSect.h"
#include "..\MIT_frx\MillustViewer.h"
#include "..\MIT_frx\MComboBox.h"

#include "DgnSelSectBase.h"

class CDBDoc;
class CDgnSrcSectDlg;

//////////////////////////////////////////////////////////////////////////
// MSRCSectRowSpin
class MSRCSectRowSpin : public CFormulaEditSpin
{
public:
	MSRCSectRowSpin();
	virtual ~MSRCSectRowSpin();

protected:
	virtual BOOL DoScroll(int nScrollSteps);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};

//////////////////////////////////////////////////////////////////////////
// MSRCSectRebarSpin
class MSRCSectRebarSpin : public CFormulaEditSpin
{
public:
	MSRCSectRebarSpin();
	virtual ~MSRCSectRebarSpin();

protected:
	virtual BOOL DoScroll(int nScrollSteps);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};

/////////////////////////////////////////////////////////////////////////////
// CDgnSrcSectDlg dialog
class CDgnSrcSectDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnSrcSectDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();

// Dialog Data
	//{{AFX_DATA(CDgnSrcSectDlg)
	enum { IDD = IDD_DGN_SRC_SECT_DLG };
	mit::frx::MComboBox    m_RebarSize;
	mit::frx::MComboBox    m_SubbarSize;
	MSRCSectRowSpin	m_RowSpin;
	MSRCSectRebarSpin m_RebarSpin;
	/////////////////////////////////
	CEditUnit	m_RebarSpace;
	CEditUnit	m_SubbarSpace;
	CEditUnit	m_D1Ctrl;
	CEditUnit	m_D2Ctrl;
	CEditUnit	m_D3Ctrl;
	CEditUnit	m_D4Ctrl;
	CEditUnit	m_D5Ctrl;
	CEditUnit	m_D6Ctrl;
	CEditUnit	m_D7Ctrl;
	CEditUnit	m_D8Ctrl;
	CEditUnit	m_HcCtrl;
	CEditUnit	m_BcCtrl;
	CEditUnit	m_D0Ctrl;
	/////////////////////////////////
	CTextUnit	m_D1Unit;
	CTextUnit	m_D2Unit;
	CTextUnit	m_D3Unit;
	CTextUnit	m_D4Unit;
	CTextUnit	m_D5Unit;
	CTextUnit	m_D6Unit;
	CTextUnit	m_D7Unit;
	CTextUnit	m_D8Unit;
	CTextUnit	m_HcUnit;
	CTextUnit	m_BcUnit;
	CTextUnit	m_D0Unit;
	CTextUnit	m_RebarSpaceUnit;
	CTextUnit	m_SubbarSpaceUnit;
	/////////////////////////////////
	int		m_nOption;
	int		m_RebarNo;
	int		m_RowNo;
	BOOL	m_nAutoCal;
	mit::frx::MillustViewer m_wndPicture;
	//}}AFX_DATA

public:
	T_RCHK_K m_RchkKey;
	T_REBS_D m_RchkData;
	CMap<int,int,int,int> m_arElemData;   // SRC Element Data ¿˙¿Â 
	CDgnSelSectBase m_SelectGrid;

	BOOL m_bUpdate;
	int m_SpinNo,m_SpinRebarNo;
	CString m_ConcType,m_StelType,m_SrcType;

public:
	void Set_RebarType(CString Type);
	void Set_SRCMaterialData(int nSrcType);
	void Set_SRCSteelDimension(T_SECT_D rData);
	void Set_SRCConcreteDimension(T_SECT_D rData);
	void Set_SRCBitmap(CString Shape);
	void Set_SRCShape4RebarCtrl();
	void Set_RebarSpaceStatus(int nCheck);

	void Initial_RebarData();
	void Initial_EditBoxStatus();
	void Initial_UnitData();
	void Initial_Data();
	void Initial_SelectItem();

	void Show_Stl_B(int iBuiltUpFlag);
	void Show_Stl_H();
	void Show_Stl_P();
	void Show_Stl_H2T();
	void Show_Stl_HT();
	void Show_Con_None();
	void Show_Con_Rect();
	void Show_Con_Cir();

	void UpdateBuffer();

	double Get_ShapeMinLength();

	BOOL ErrorCheck();

	void SetData2Dlg();
	BOOL SetDlg2Data();
	void SelectElemOnView();
	void ChangeSectionInfo();

	CString Set_SRCStlType(CString strSrcShp, int iBuiltupFlag);
	CString Set_SRCConType(CString strConShp);

	void InitSectGrid();
	void ResetSectGridData(BOOL bKeepChk = FALSE);

	virtual void OnUpdate(CView* pView, LPARAM lHint, CObject* pHint);

	BOOL OnNotifyRowSpin(WPARAM wParam, LPARAM& lParam, LRESULT* pResult);
	BOOL OnNotifyRebarSpin(WPARAM wParam, LPARAM& lParam, LRESULT* pResult);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSrcSectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnSrcSectDlg)
	afx_msg void OnDgnSrcAr();
	afx_msg void OnDgnSrcDel();
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposDgnSrcRebarspin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDgnSrcSect();
	afx_msg void OnDeltaposDgnSrcRebarrowspin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDgnSrcAutocalrebar();
	afx_msg void OnDgnSRCSelectAll();
	afx_msg void OnDgnSRCUnSelectAll();
	//}}AFX_MSG
	LRESULT OnSelectCheckSectionGrid(WPARAM wParam, LPARAM lParam);
	LRESULT OnChangeFocusSectionGrid(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNSRCSECTDLG_H__5D7AB3A7_D4E8_11D3_888F_0000C0F30D4D__INCLUDED_)
