#if !defined(AFX_DGNRCDESIGNPARAMPARAMDLG_H__D6401187_048E_4F4B_A55B_82641E5E335D__INCLUDED_)
#define AFX_DGNRCDESIGNPARAMPARAMDLG_H__D6401187_048E_4F4B_A55B_82641E5E335D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnRCDesignParamParamDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DgnRCDesignParamParamDlg dialog
#include "..\wg_base\ChildDialog.h"

class DgnRCDesignParamBaseDlg;

#include "HeaderPre.h"   


const int iIDPRINT     = 12;//
const int iRAILIDPRINT = 12;//
const int iSEISIDPRINT = 12;//
const int iCJJ166SEISIDPRINT = 12;//

class __MY_EXT_CLASS__ DgnRCDesignParamParamDlg : public CChildDialog
{
// Construction
public:
	DgnRCDesignParamParamDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DgnRCDesignParamParamDlg)
	enum { IDD = IDD_DGN_RC_DESIGN_PARAM_PARAM };
		// NOTE: the ClassWizard will add data members here
	//add 
	CComboBox m_DesignCode;	
	int m_iDimType;
	int m_iSafeType;
	int m_iCrackCode;
	int m_iCrackType;
	BOOL m_bDesignPrint[iIDPRINT];

	int  m_iRailCrackCode;
	BOOL m_bRailDesignPrint[iRAILIDPRINT];
	BOOL m_bSeismic;

	int  m_iEarthquakeType;
	int  m_iBridgeType;
	BOOL m_bSeisDesignPrint[iSEISIDPRINT];
	BOOL m_IDSeisDesignPrint[iSEISIDPRINT];
	CButton   m_RadioEarthE1;
	CButton   m_RadioEarthE2_E;
	CButton   m_RadioEarthE2_EP;	
	CButton   m_RadioBridgeType_BC;
	CButton   m_RadioBridgeType_D;
	CButton   m_ChkCopingDis;         
	CButton   m_ChkPlasticTorsion;    

	int  m_iCjjEarthquakeType;
	int  m_iCjjBridgeType;
	BOOL m_bCjjSeisDesignPrint[iCJJ166SEISIDPRINT];
	BOOL m_IDCjjSeisDesignPrint[iCJJ166SEISIDPRINT];
	CButton   m_RadioCjjEarthE1;
	CButton   m_RadioCjjEarthE2_E;
	CButton   m_RadioCjjEarthE2_EP;	
	CButton   m_RadioCjjBridgeType_A;
	CButton   m_RadioCjjBridgeType_B;
	CButton   m_ChkCjjCopingDis;         
	CButton   m_ChkCjjPlasticTorsion;    

	//}}AFX_DATA
	CArray<UINT, UINT> m_aRoad_OptID;
	CArray<UINT, UINT> m_aRoad_OutID;
	CArray<UINT, UINT> m_aRail_OptID;
	CArray<UINT, UINT> m_aRail_OutID;
	CArray<UINT, UINT> m_aSeis_OptID;
	CArray<UINT, UINT> m_aSeis_OutID;

	CArray<UINT, UINT> m_aCjjSeis_OptID;
	CArray<UINT, UINT> m_aCjjSeis_OutID;


	void InitCtrl();
	void Initial_ExistData();
	void Initial_Data();
	void ChangeDesignCode();
	
	void Init_BridgeCtrl();
	void Init_CheckCtrl();	
	void Init_CheckCtrl_E2BC();

	void Init_CJJBridgeCtrl();
	void Init_CJJCheckCtrl();	
	void Init_CJJCheckCtrl_E2BC();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DgnRCDesignParamParamDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	public:
		void SaveOrUpdate();
		void EnableCtrls();

// Implementation
protected:
	DgnRCDesignParamBaseDlg* m_pParent;
		int m_iDgnCode;
	// Generated message map functions
	//{{AFX_MSG(DgnRCDesignParamParamDlg)
		// NOTE: the ClassWizard will add member functions here	
	virtual BOOL OnInitDialog();
 	//virtual void OnCancel();
	afx_msg void OnSelchangeDgnCtrldataDesigncode();
	afx_msg void OnClickSelectAll();
	afx_msg void OnClickUnSelectAll();
	afx_msg void OnClickRailSelectAll();
	afx_msg void OnClickRailUnSelectAll();
	afx_msg void OnChangeEarthquakeType();
	afx_msg void OnChangeBridgeType();
	afx_msg void OnCheck_E2BC_Ctrl();
	afx_msg void OnClickSeisSelectAll();
	afx_msg void OnClickSeisUnSelectAll();
	afx_msg void OnClickSpecialLoad();
	afx_msg void OnClickCjjSeisSelectAll();
	afx_msg void OnClickCjjSeisUnSelectAll();
	afx_msg void OnClickCjjBridgeType();
	afx_msg void OnClickCjjEarthquakeType();
	afx_msg void OnCheck_CjjE2BC_Ctrl();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////
#include "HeaderPost.h"     /////////
/////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNRCDESIGNPARAMPARAMDLG_H__D6401187_048E_4F4B_A55B_82641E5E335D__INCLUDED_)
