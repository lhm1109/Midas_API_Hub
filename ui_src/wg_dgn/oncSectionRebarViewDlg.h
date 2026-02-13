// (060123) sshan Create

#if !defined(__ONCSECTIONREBARVIEWDLG_H__)
#define __ONCSECTIONREBARVIEWDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// oncSectionRebarViewDlg.h : header file
//

#include "DgnDlgBase.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_CobxSect.h"
#include "..\wg_db\wg_db_CobxFimp.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_base\wg_base_I_Factory_DlgViewPanel.h"
#include "..\wg_db\DB_ST_DN.h"
#include "..\MIT_frx\MComboBox.h"
#include "DgnTabCtrl.h"

//#include "ConcSectionCreateRebar.h"

#include "DgnStruct.h"

#include "HeaderPre.h"

/////////////////////////////////////////////////////////////////////////////
// ConcSectionRebarViewDlg dialog



class I_DlgViewPanel;
class ConcSectionRebarEventHandler;
class CConcSectionMainRebar;
class CConcSectionShearRebar;
using namespace mit::frx;

class __MY_EXT_CLASS__ ConcSectionRebarViewDlg : public CDialogMove
{
// Construction
public:
	ConcSectionRebarViewDlg(CWnd* pParent = NULL);   // standard constructor
	~ConcSectionRebarViewDlg();

// Dialog Data
	//{{AFX_DATA(ConcSectionRebarViewDlg)
	enum { IDD = IDD_DGN_CON_SECT_REBAR_DLG };
	MComboBox	m_RandomSectionCombo;
	CBCGPStatic	m_wndTextOut;
	//CDGNTabCtrl	m_ctrlTab;
	CDlgTabCtrl	m_ctrlTab;
	CCobxSect	m_wndSectCombo;
	int m_iCenterType;
	CString	m_Ctrl_ElemNumber_Edit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ConcSectionRebarViewDlg)

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnCancel();

 
	//}}AFX_VIRTUAL

public:
	T_SECT_D  m_sectData;
	T_REBT_D  m_RebtData;
	T_REBT_SHBR m_ShearRebarDataI;
	T_REBT_SHBR m_ShearRebarDataJ;
	BOOL bMainSameIJ;
	BOOL bShearSameIJ;
	CMap<REBAR_KEY,REBAR_KEY,T_REBT_MABR,T_REBT_MABR> m_MabrDataListI;
	CMap<REBAR_KEY,REBAR_KEY,T_REBT_MABR,T_REBT_MABR> m_MabrDataListJ;
	CMap<int,int,int,int> KeyDataListI;
	CMap<int,int,int,int> KeyDataListJ;

	void AddRebarData(REBAR_KEY RebarKey, T_REBT_MABR MaReberData, BOOL bI_End);
	void DeleteRebarData(REBAR_KEY RebarKey, BOOL bI_End);

	void AddRebarDraw(REBAR_KEY RebarKey, T_REBT_MABR MaReberData, BOOL bI_End);

	void SetCurDataIJ(BOOL bI_End);
	void DrawSectionRebarTabChanged();

	void SetSectionInputType(int nSectiontype, BOOL bIsShearRebar=FALSE, BOOL bIsScaleTaperedGroup=FALSE);

	// RC_UMD_COLUMN :: Code By Unsang!!
	void SetSectionInputType(int nSectiontype, CString strName, T_SECT_D SectData, T_REBT_D RebtData);
	void SetElemKey(T_ELEM_K key);

	int GetMainRebarI_LastNum();
	int GetMainRebarJ_LastNum();

	void ReDrawRebar(REBAR_KEY RebarKey, T_REBT_MABR MaReberData, BOOL bI_End);

	// RC_UMD_COLUMN :: Code By Unsang!!
	CString m_strName;

	//(2010.02.05) Add by Unsang :: Add DgnCode. for Eurocode2-2:05, Gentype!
	CString m_strDgnCode;

	//BOOL Get_ConvertRebtData(T_REBT_D DBInputData, T_REBT_D RebtData);

protected:
	CDBDoc* m_pDoc;
	CConcSectionMainRebar* m_pMainRebar; //MainRebar
	CConcSectionShearRebar* m_pShearRebar; //ShearRebar
	
	CArray<double, double&> m_arSectionPos;

	CArray<DlgView_Object*,DlgView_Object*> m_arViewGeom;

	T_SECT_K NowSectKey;;

	//Section이 화면에 Draw되어 있는지..
	BOOL bDrawSection;

	CMap<int,int,T_SECT_K,T_SECT_K> m_RandomSectionList;

	
	BOOL m_bIsShearRebar;        // 전단철근 입력창을 보여줄지 여부
	BOOL m_bIsScaleTaperedGroup; // Scale만 다른 동일형상의 Tapered Section Group을 허용할지 여부
	int nSectionType;//1 : Section별 철근입력,   0 : Elem별 철근 입력, 2:UMD에서 호출
	T_ELEM_K Elemkey;

	void Data2Dlg();
	BOOL Dlg2Data();

	//도심의 좌표
	double OffsetI_Y;
	double OffsetI_Z;
	double OffsetJ_Y;
	double OffsetJ_Z;

	int priCenterType;//전에가 도심기준이었는지 좌하단 기준이었는지



protected:
	void LayoutChildDlg();

	BOOL ImportSection(int iPosi);

	BOOL ConvertSection(T_SECT_D* pSectD, CArray<double, double&> &arPos, int iPosi);

	void AddObjects();

	void SetToolBarIcons();

	BOOL MakeCirclePoints (double Yc, double Zc, double Radius,CArray<DV_3dp, DV_3dp> &PosArr,double nSeg = 20);

	void InitialRandomSectionData();

	void SetRandomSectionComboHide();

	void SectionImportByElem();
	void SectionImportByElemUsedUMD();

	void ChangedCenterType(BOOL bCenType);

	BOOL SaveData();

	// T_REBT_D 형식의 정보를 T_RBTE_D형식으로 변경시켜줌
	//   rebtData : 변경할 T_REBT_D 정보 
	//   rbteData : 변경된 T_RBTE_D 정보를 넘겨 받을 변수
	BOOL Cng_From_REBT_To_RBTE(T_REBT_D rebtData, T_RBTE_D & rbteData);
	BOOL Cng_From_RBTE_To_REBT(T_RBTE_D rbteData, T_REBT_D & rebtData);


	

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ConcSectionRebarViewDlg)
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

//public:
	//CConcSectionRebarView *m_pView;

public:
	I_DlgViewPanel* m_pIVP;

	ConcSectionRebarEventHandler* m_pEventHandler;

};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__ONCSECTIONREBARVIEWDLG_H__)
