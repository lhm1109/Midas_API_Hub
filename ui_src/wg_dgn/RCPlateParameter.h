#if !defined(__CRCPLATEPARAMETER_H__)
#define __CRCPLATEPARAMETER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CRCPlateParameter.h : header file
//

#include "..\wg_base\ChildDialog.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_base\wg_base_ModelessMoveDlg.h"
#include "..\wg_base\wg_baseMouseEdit.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectCtrl.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\SelectLC.h"
#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"

#include "DgnRCPlateGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CRCPlateParameter dialog

class CRCDesignPlateParamDlg;

/////////////////////////////////////
#include "HeaderPre.h"      /////////
/////////////////////////////////////
using namespace mit::frx;
class __MY_EXT_CLASS__ CRCPlateParameter : public CChildDialog
{
// Construction
public:
	//CRCPlateParameter(CCMSecViewWnd* pSectView, CWnd* pParent = NULL);   // standard constructor
	CRCPlateParameter(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRCPlateParameter();

// interface
public:
	//void SetCurData(T_RPSC_D* pData);
	//BOOL GetCurData(T_RPSC_D* pData);
	//void RedrawView();

// Dialog Data
	//{{AFX_DATA(CRCPlateParameter)
	enum { IDD = IDD_DGN_CON_DESIGN_PARAM_PLATE };

	MEdit	m_editName;
	
	MCheckListBox m_ltbxLcomStre; 
	MCheckListBox m_ltbxLcomServ; 
	MButton	m_radioBeam;
	MButton	m_radioSlab;
	MComboBox	m_comboMinShearCheck;
	MButton	m_chkInclude;  
	CTextUnit	m_dDtunit;
	CTextUnit	m_dDbunit;
	CEditUnit	m_Dt;
	CEditUnit	m_Db;

	CString	m_SelectElemString;
	CSelectEdit m_editSelectElem;
	MEdit	m_editEtc;
	MButton	m_radioLocal;
	MButton	m_radioUCS;
	MComboBox	m_comboUCS;
	MButton	m_radioElement;
	MButton	m_radioAvgNodal;
	MButton	m_chkAvgNodal;
	MButton	m_radioXDir;
	MButton	m_radioYDir;
	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRCPlateParameter)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:

	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
	virtual void SetUSCCombo();
	
	void Click_Btn_Apply();

	BOOL Dlg2Data();
	BOOL Data2Dlg();

	void EnableCalcBtn(bool Enable);
	void ChangeLoadCombination();

	int  m_nOperationMode;
	CArray<double,double> m_arXYZ[3]; 

	void OnChangeCurrentRcpl(T_RCPL_K key);

	BOOL   m_bIsComplete;

protected:  
	CRCDesignPlateParamDlg* m_pParent;
	CDBDoc* m_pDoc;

	
	CDgnRCPlateGrid* m_PlateGrid;
	
	BOOL m_bCheckPosi[3];
	T_RCPL_D m_Data;
		
	int m_iAddDelMod; //Add 0, Modify 1, Delete 2

	CString m_strConCode;

protected:

	CArray<T_LCOM_K, T_LCOM_K> m_arLcomKey;
	CMap<T_LCOM_K,T_LCOM_K,T_LCOM_K,T_LCOM_K> m_arStreLcomKey;
	CMap<T_LCOM_K,T_LCOM_K,T_LCOM_K,T_LCOM_K> m_arServLcomKey;
	CMap<T_RCPL_K,T_RCPL_K,T_RCPL_D,T_RCPL_D> m_RcplData;

	CArray<T_LCOM_K, T_LCOM_K> m_StreLcom;
	CArray<T_LCOM_K, T_LCOM_K> m_ServLcom;

	void InitialData();
	void SetInitUnit();  

	void Dlg2Data_LoadCase();

	void LoadDBtoDlg();
	
	BOOL DataCheck();

	void InitEnableColtrol();

	void InitUCSCmb();
	
// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CRCPlateParameter)
	virtual BOOL OnInitDialog();
	afx_msg void OnAddData();
	afx_msg void OnModifyData();
	afx_msg void OnDeleteData();
	afx_msg void OnDgnPlateCalcBtn();
	afx_msg void OnDgnRadioLocalUCS();	
	afx_msg void OnDgnRadioElemAvg();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	BOOL LoadCaseApply();
	void InitialControl();
	void InitialCboMinShearCheck();
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

#endif // !defined(__CRCPLATEPARAMETER_H__)
