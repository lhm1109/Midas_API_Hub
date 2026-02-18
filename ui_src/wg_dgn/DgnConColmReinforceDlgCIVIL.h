#if !defined(__DGNCONCOLMREINFORCEDLGCIVIL_H__)
#define __DGNCONCOLMREINFORCEDLGCIVIL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConBeamReinforceDlgCIVIL.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\ChildDialog.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_cmd\wg_cmd_CMSectViewWnd.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"


/////////////////////////////////////////////////////////////////////////////
// CDgnConColmReinforceDlgCIVIL dialog

#include "DgnColumnGridWndCIVIL.h"
#include "DgnSectionDrawWnd.h"
#include "../MIT_frx/MTabCtrl.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MillustViewer.h"

//class CDgnConColmReinforceDlgRebarCIVIL;

#include "HeaderPre.h"

using namespace mit::frx;
class __MY_EXT_CLASS__ CDgnConColmReinforceDlgCIVIL : public CChildDialog
{
// Construction
public:
	CDgnConColmReinforceDlgCIVIL(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() {}
 
// Dialog Data
	//{{AFX_DATA(CDgnConColmReinforceDlgCIVIL)
	enum { IDD = IDD_DGN_CON_REIN_COLM_DLG };
	//MButton	    m_btnDel;
	//MButton	    m_btnAdd;
	MButton	    m_btnRedraw;
	CListCtrl	  m_wndList;
	mit::frx::MTabCtrl	m_ctrlTab;

	CTextUnit	m_tunit;
	CEditUnit	m_t;
	CTextUnit	m_Hunit;
	CEditUnit	m_H;
	CTextUnit	m_bunit;
	CEditUnit	m_b;
	CTextUnit	m_Bunit;
	CEditUnit	m_B;
	CTextUnit	m_aunit;
	CEditUnit	m_a;

 	CTextUnit	m_AsUT;
	CEditUnit	m_As;
	CEditUnit	m_Num;
	CEditUnit	m_YNum_2nd;
	CEditUnit	m_ZNum_2nd;	
	CTextUnit	m_SpaceUT;
	CEditUnit	m_Space;
	MComboBox	m_SizeCB;
	MComboBox	m_SizeYCB_2nd;
	MComboBox	m_SizeZCB_2nd;
	MComboBox	m_LayerCB;
	int		    m_nType;
	//}}AFX_DATA

public:
	void SetTieSecondType(BOOL bSecond) { m_bTieSecondType = bSecond; }
	void AddDgn();
	void DeleteDgn();

protected:
	//CDgnConColmReinforceDlgRebarCIVIL* m_pParent;
	BOOL    m_bTieSecondType;

	CDBDoc* m_pDoc;
	T_RCHK_K m_RchkKey;
	T_RCHK_D m_RchkData;
	T_SECT_D m_SectD;
	CMap<int,int,int,int> m_arColmData;   // Element 중 Concrete Beam의 Data 저장 
	BOOL m_bModify;
	CString     m_Shape;
	CArray<UINT, UINT>    m_BGroup, m_HGroup, m_tGroup, m_abGroup, m_Tie1stType, m_Tie2ndType, m_TieTypeNum;
		
	void InitGridData();
	void SetInitCtrl_TieRbar();
	void SetInitUnit();
	BOOL SetDlg2Data();
	void SetData2Dlg();
	BOOL CheckInputData();
	void ChangeSectionInfo();
	void SelectElemOnView();
	BOOL SelectSection(CArray<T_RCHK_K, T_RCHK_K>&  arRchKey);
	void CheckDiffSections(T_SECT_D OrginSectData, T_SECT_D SectData, int& iResult);
	void ShowBitmap(CString& colType);
	void ShowHideControl(CString& colType);

	void ChangeItem(int nIndex);
	void MakeItemEx();
	void SetHeaderTitle();
	int  FindInsertionPos(T_SECT_K nID);
	BOOL InsertItem(T_SECT_K Key, T_SECT_D &Data);
	CString	DataToStr(int i, T_SECT_K Key, T_SECT_D &Data, T_RCHK_D &RData);
	 
	void SetControlText(BOOL bBox);
	//void UpdateBuffer();
	//void UpdateUnit();
	//BOOL Is2TypeRebar(int nPos);
	//void ResetSectGridData(BOOL bKeepChk = FALSE);


protected:
	int                      m_nLayerNum;
	T_RCHK_COLM*             m_pColmData;
	CMap<int,int,T_RCHK_K,T_RCHK_K> m_arRchkKey;

	CDgnSectionDrawWnd       m_SectWnd;
	CDgnColumnGridWndCIVIL*  m_ColGrid;

	MillustViewer m_wndPicRect;
	MillustViewer m_wndPicRectSolid;
	MillustViewer m_wndPicCirc;
	MillustViewer m_wndPicCircSolid;
	MillustViewer m_wndPicOcta;
	MillustViewer m_wndPicOctaSolid;
	MillustViewer m_wndPicTracHalf;
	MillustViewer m_wndPicTrac;
	MillustViewer m_wndPicTracSolid;
	MillustViewer m_wndPicH;
	//
	void SetRchkData(T_RCHK_K RchkKey, T_RCHK_COLM* pColmData);
	void UpdateViewWnd();
	void SetInitRebarCB();
	void SetInitLayerCB();
	void SetData2Dlg_Rebar();
	void SetDlg2Data_Rebar();  
	BOOL CheckInputData_Rebar();
	void SetDataToGrid();
	void GetDataFromGrid();
	void UpdateAsInfo(double dAsc);
		

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConColmReinforceDlgCIVIL)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CDgnConColmReinforceDlgCIVIL)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnDgnDesignSection();
	afx_msg void OnDgnDelete();
	afx_msg void OnItemchangedDgnSectList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickDgnSectList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeDgnLayerCb();
	afx_msg void OnRedrawBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNCONCOLMREINFORCEDLGCIVIL_H__)
