#if !defined(__CRCBEAMPARAMETER_H__)
#define __CRCBEAMPARAMETER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CRCBeamParameter.h : header file
//

#include "..\wg_base\ChildDialog.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectCtrl.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\MIT_frx\MComboBox.h"

#include "DgnRCBeamGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CRCBeamParameter dialog

class CRCDesignParamDlg;

/////////////////////////////////////
#include "HeaderPre.h"      /////////
/////////////////////////////////////
using namespace mit::frx;
class __MY_EXT_CLASS__ CRCBeamParameter : public CChildDialog
{
	// Construction
public:
	//CRCBeamParameter(CCMSecViewWnd* pSectView, CWnd* pParent = NULL);   // standard constructor
	CRCBeamParameter(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRCBeamParameter();

	// interface
public:
	//void SetCurData(T_RPSC_D* pData);
	//BOOL GetCurData(T_RPSC_D* pData);
	//void RedrawView();

	// Dialog Data
	//{{AFX_DATA(CRCBeamParameter)
	enum { IDD = IDD_DGN_CON_DESIGN_PARAM_BEAM };
	int m_iRbarRatio; 
	MComboBox	m_cboCrackWidth; 
	BOOL m_bIncludeSbar; 
	MCheckListBox m_ltbxLcomStre; 
	MCheckListBox m_ltbxLcomServ; 

	BOOL	m_bIEnd;
	BOOL	m_bJEnd;
	BOOL	m_bMid;

	CString	m_strRemark;
	CTextUnit	m_dDtunit;
	CTextUnit	m_dDbunit;
	CEditUnit	m_Dt;
	CEditUnit	m_Db;
	CString	m_SelectElemString;
	//}}AFX_DATA

	CSelectEdit m_editSelectElem;


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRCBeamParameter)
public:
	virtual BOOL DestroyWindow();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:

	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

	void OnChangeCurrentRcbe(T_ELEM_K key);

	void Click_Btn_Apply();

	BOOL Dlg2Data();
	BOOL Data2Dlg();

	void EnableCalcBtn(bool Enable);
	void ChangeLoadCombination();

protected:  

	CRCDesignParamDlg* m_pParent;
	CDBDoc* m_pDoc;


	CDgnRCBeamGrid* m_BeamGrid;

	CArray<T_ELEM_K,T_ELEM_K> m_arElemK;  
	BOOL m_bCheckPosi[3];
	T_RCBE_D m_Data;

	BOOL GetMaterialData(T_ELEM_K ElemK, double& dfck, double& dfy, double& dEratio);

	int addDelMod;

	// Add, Jaeoh. (06.12.07)
	CString m_strConCode;

protected:

	CArray<T_LCOM_K, T_LCOM_K> m_arLcomKey;
	CMap<T_LCOM_K,T_LCOM_K,T_LCOM_K,T_LCOM_K> m_arStreLcomKey;
	CMap<T_LCOM_K,T_LCOM_K,T_LCOM_K,T_LCOM_K> m_arServLcomKey;
	CMap<T_ELEM_K,T_ELEM_K,T_RCBE_D,T_RCBE_D> m_RcbeData;

	CArray<T_LCOM_K, T_LCOM_K> m_StreLcom;
	CArray<T_LCOM_K, T_LCOM_K> m_ServLcom;

	CArray<T_ELEM_K, T_ELEM_K&> m_pBadKeyList;


	void InitialData();
	void SetInitUnit();  

	void Dlg2Data_LoadCase();

	void LoadDBtoDlg();
	void UpdateDgnData();

	BOOL DataCheck();

	void InitEnableColtrol();

	// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CRCBeamParameter)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelChangeCrackWidth();
	afx_msg void OnLcomStrengthIn();
	afx_msg void OnLcomStrengthOut();
	afx_msg void OnLcomServiceIn();
	afx_msg void OnLcomServiceOut();
	afx_msg void OnLcomSeismicIn();
	afx_msg void OnLcomSeismicOut();
	afx_msg void OnAddData();
	afx_msg void OnModifyData();
	afx_msg void OnDeleteData();
	afx_msg void OnBtnApply();
	BOOL m_bOnBtnApplyResult;
	afx_msg void OnDgnBeamCalcBtn();
	afx_msg void OnDgnBeamModifyConcreteMaterialBtn();
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

#endif // !defined(__CRCBEAMPARAMETER_H__)
