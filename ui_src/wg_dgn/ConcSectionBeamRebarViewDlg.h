#if !defined(__CONCSECTIONBEAMREBARVIEWDLG_H__)
#define __CONCSECTIONBEAMREBARVIEWDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// oncSectionRebarViewDlg.h : header file
//

#include "DgnDlgBase.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_base\wg_base_I_Factory_DlgViewPanel.h"
#include "..\wg_db\DB_ST_DN.h"
#include "DgnTabCtrl.h"
#include "..\MIT_frx\MComboBox.h"

#include "DgnStruct.h"

#include "HeaderPre.h"

/////////////////////////////////////////////////////////////////////////////
// CConcSectionBeamRebarViewDlg dialog
using namespace mit::frx;
class I_DlgViewPanel;
class ConcSectionRebarEventHandler;
class CConcSectionMainBeamRebar;
class CConcSectionShearBeamRebar;
class CConcSectionTorsionBeamRebar;

class __MY_EXT_CLASS__ CConcSectionBeamRebarViewDlg : public CDialogMove
{
// Construction
public:
	CConcSectionBeamRebarViewDlg(CWnd* pParent = NULL);   // standard constructor
	~CConcSectionBeamRebarViewDlg();

// Dialog Data
	//{{AFX_DATA(CConcSectionBeamRebarViewDlg)
	enum { IDD = IDD_DGN_CON_SECT_BEAM_REBAR_DLG };
	MComboBox	m_cmdSect;
	CBCGPStatic	m_strTextOut;
	CDlgTabCtrl	m_ctrlTab;
	int m_iCenterType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConcSectionBeamRebarViewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnCancel();
	//}}AFX_VIRTUAL

public:
	T_SECT_D  m_sectData;

	BOOL m_bMainSameIMJ;
	BOOL m_bShearSameIMJ;
	BOOL m_bTorsionSameIMJ;
	CMap<REBAR_KEY,REBAR_KEY,T_RBGC_MABR,T_RBGC_MABR> m_MabrDataListI;
	CMap<REBAR_KEY,REBAR_KEY,T_RBGC_MABR,T_RBGC_MABR> m_MabrDataListM;
	CMap<REBAR_KEY,REBAR_KEY,T_RBGC_MABR,T_RBGC_MABR> m_MabrDataListJ;
	CMap<int,int,int,int> KeyDataListI;
	CMap<int,int,int,int> KeyDataListM;
	CMap<int,int,int,int> KeyDataListJ;
	T_RBGC_SHBR  m_ShearRebarDataI;
	T_RBGC_SHBR  m_ShearRebarDataM;
	T_RBGC_SHBR  m_ShearRebarDataJ;
	T_RBGC_TORBR m_TorsionRebarDataI;
	T_RBGC_TORBR m_TorsionRebarDataM;
	T_RBGC_TORBR m_TorsionRebarDataJ;

	void AddRebarData(REBAR_KEY RebarKey, T_RBGC_MABR MaReberData, int nPosition);
	void DeleteRebarData(REBAR_KEY RebarKey, BOOL bI_End);

	void AddRebarDraw(REBAR_KEY RebarKey, T_RBGC_MABR MaReberData, int nPosition);

	void SetCurDataIMJ(int nPosition);
	void DrawSectionRebarTabChanged();

	int GetMainRebarI_LastNum();
	int GetMainRebarM_LastNum();
	int GetMainRebarJ_LastNum();

	void ReDrawRebar(REBAR_KEY RebarKey, T_RBGC_MABR MaReberData, int nPosition);

protected:
	CDBDoc* m_pDoc;
	CConcSectionMainBeamRebar*    m_pMainRebar;    // MainRebar
	CConcSectionShearBeamRebar*   m_pShearRebar;   // ShearRebar
	CConcSectionTorsionBeamRebar* m_pTorsionRebar; // TorsionRebar
	
	CArray<double, double&> m_arSectionPos;

	CArray<DlgView_Object*,DlgView_Object*> m_arViewGeom;

	T_SECT_K NowSectKey;;

	//Section이 화면에 Draw되어 있는지..
	BOOL bDrawSection;

	CMap<int,int,T_SECT_K,T_SECT_K> m_RandomSectionList;

	//도심의 좌표
	double OffsetI_Y;
	double OffsetI_Z;
	double OffsetJ_Y;
	double OffsetJ_Z;

	int priCenterType;//전에가 도심기준이었는지 좌하단 기준이었는지

protected:
	BOOL ImportSection(int iPosi);
	BOOL ConvertSection(T_SECT_D* pSectD, CArray<double, double&> &arPos, int iPosi);
	void SetToolBarIcons();
	BOOL MakeCirclePoints (double Yc, double Zc, double Radius,CArray<DV_3dp, DV_3dp> &PosArr,double nSeg = 20);
	void InitialRandomSectionData();
	void ChangedCenterType(BOOL bCenType);
	BOOL SaveData();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CConcSectionBeamRebarViewDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnSectImportButton();
	afx_msg void OnDgnSectImportCancelButton();
	afx_msg void OnDgnDefineSectButton();
	afx_msg void OnOk();
	afx_msg void OnSelchangeDgnRandomSectCombo();
	afx_msg void OnSelchangeDgnTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangedCenterType_Center();
	afx_msg void OnChangedCenterType_LeftBottom();
	afx_msg void OnBtnApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	I_DlgViewPanel* m_pIVP;

	ConcSectionRebarEventHandler* m_pEventHandler;
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CONCSECTIONBEAMREBARVIEWDLG_H__)
