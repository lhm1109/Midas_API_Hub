#if !defined(__CRCWALLPARAMETER_H__)
#define __CRCWALLPARAMETER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CRCWallParameter.h : header file
//

#include "..\wg_base\ChildDialog.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectCtrl.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MComboBox.h"

#include "DgnRCWallGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CRCWallParameter dialog

class CRCDesignParamDlg;
class CMembCtrl;

/////////////////////////////////////
#include "HeaderPre.h"      /////////
/////////////////////////////////////
using namespace mit::frx;
class __MY_EXT_CLASS__ CRCWallParameter : public CChildDialog
{
// Construction
public:
	//CRCWallParameter(CCMSecViewWnd* pSectView, CWnd* pParent = NULL);   // standard constructor
	CRCWallParameter(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRCWallParameter();

// interface
public:
	//void SetCurData(T_RPSC_D* pData);
	//BOOL GetCurData(T_RPSC_D* pData);
	//void RedrawView();

// Dialog Data
	//{{AFX_DATA(CRCWallParameter)
	enum { IDD = IDD_DGN_CON_DESIGN_PARAM_WALL };
	MButton m_radioShort;
	MButton m_radioLongShort;
	MButton m_chkAutoSway;
	MButton m_radioSway;
	MButton m_radioNonSway;
	MButton m_chkAutoMomentEnlarge;
	MButton m_chkAutoMomentEquivalent;
	MButton m_chkAutoMomentExpansion;
	MEdit	m_editDeltaNs;
	MEdit	m_editCm;
	MEdit	m_editDeltaS;
	MComboBox	m_comboCrackWidth;
	MButton m_chkIncludeShear;
	MButton m_chkAutoRebarPosi;
	MEdit   m_editTopRebarRatio;
	MButton m_chkMaxRebarRatio;
	MEdit	  m_editPMax;
	CEditUnit	m_editDt;
	CEditUnit	m_editDb;
	CEditUnit	m_editHeightI;
	CEditUnit	m_editHeightJ;
	CTextUnit	m_unitDt;
	CTextUnit	m_unitDb;
	CTextUnit	m_unitHeightI;
	CTextUnit	m_unitHeightJ;
	MButton m_chkIEnd;
	MButton m_chkMid;
	MButton m_chkJEnd;
	MEdit	m_editEtc;
	CSelectEdit m_editSelectElem;
	MCheckListBox	m_ltbxLcomServ;
	MCheckListBox	m_ltbxLcomStre;
	MCheckListBox	m_ltbxGroup;	
	CString	m_SelectElemString;
	MButton	m_chkCroseBd;
	MButton	m_chkNoCroseBd;
	MEdit	m_editBetaDCrose;
	MEdit	m_editBetaDNoCrose;
	//}}AFX_DATA

	


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRCWallParameter)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:

	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
	
	void OnChangeCurrentRcwa(T_ELEM_K key);

	void Click_Btn_Apply();

	BOOL Dlg2Data();
	BOOL Data2Dlg();

	void EnableCalcBtn(bool Enable);
	void ChangeGroupandLoadCombination();

	CMembCtrl* m_pMembCtrl;
	

protected:
	CRCDesignParamDlg* m_pParent;
	CDBDoc* m_pDoc;
	
	CDgnRCWallGrid* m_WallGrid;
	
	CArray<T_ELEM_K,T_ELEM_K> m_arElemK;  
	T_RCWA_D m_Data;
		
	int addDelMod;

	CString m_strConCode;

protected:

	CArray<T_LCOM_K, T_LCOM_K> m_arLcomKey;
	CArray<T_GRUP_K, T_GRUP_K> m_arGrupKey;
	CMap<T_LCOM_K,T_LCOM_K,T_LCOM_K,T_LCOM_K> m_arStreLcomKey;
	CMap<T_LCOM_K,T_LCOM_K,T_LCOM_K,T_LCOM_K> m_arServLcomKey;
	CMap<T_GRUP_K,T_GRUP_K,T_GRUP_K,T_GRUP_K> m_arGroupMapKey;
	CMap<T_ELEM_K,T_ELEM_K,T_RCWA_D,T_RCWA_D> m_RcwaData;

	CArray<T_LCOM_K, T_LCOM_K> m_StreLcom;
	CArray<T_LCOM_K, T_LCOM_K> m_ServLcom;

	CArray<T_ELEM_K, T_ELEM_K&> m_pBadKeyList;
	CArray<T_ELEM_K, T_ELEM_K&> m_pBadMemberKeyList;
	
	void InitialData();

	void SetInitUnit();

	void Dlg2Data_LoadCase();
	void LoadDBtoDlg();
	void UpdateDgnData();

	BOOL DataCheck();

	void InitEnableColtrol();
	
	void YZDirectionAllSwayEnableControl();

// Implementation
protected:
	// Generated message map functions`
	//{{AFX_MSG(CRCWallParameter)
	virtual BOOL OnInitDialog();
	afx_msg void OnAddData();
	afx_msg void OnModifyData();
	afx_msg void OnDeleteData();
	afx_msg void OnBtnApply();  
	BOOL m_bOnBtnApplyResult;
	afx_msg void OnDgnWallCalcBtn();		
	afx_msg void OnDgnUnbracedLengthBtn();	
	afx_msg void OnDgnEffectiveLengthBtn();	
	afx_msg void OnDgnChkClossBinding();	
	afx_msg void OnDgnChkAutoMomentEnlarge();	
	afx_msg void OnDgnChkAutoEquivalentMoment();	
	afx_msg void OnDgnChkAutoMomentEnlargeNocress();	
	afx_msg void OnDgnRadioLongShort();
	afx_msg void OnDgnRadioSwayNonsway();	
	afx_msg void OnDgnChkAutoRebarPosi();
	afx_msg void OnDgnChkRhoMax();
	afx_msg void OnClickChkCroseBd();
	afx_msg void OnClickChkNoCroseBd();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


protected:
	void InitialCboCrackWidth();
	void InitialLcomDataForDgn();  

	CString ReturnDgnConCode();
	
private:
	ROWCOL m_nCurrentRow;

};

/////////////////////////////////////
#include "HeaderPost.h"     /////////
/////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CRCWALLPARAMETER_H__)
