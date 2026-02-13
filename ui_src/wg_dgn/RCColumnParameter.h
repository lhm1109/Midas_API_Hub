#if !defined(__CRCCOLUMNPARAMETER_H__)
#define __CRCCOLUMNPARAMETER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CRCColumnParameter.h : header file
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

#include "DgnRCColumnGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CRCColumnParameter dialog

class CRCDesignParamDlg;
class CMembCtrl;

/////////////////////////////////////
#include "HeaderPre.h"      /////////
/////////////////////////////////////
using namespace mit::frx;
class __MY_EXT_CLASS__ CRCColumnParameter : public CChildDialog
{
// Construction
public:
	//CRCColumnParameter(CCMSecViewWnd* pSectView, CWnd* pParent = NULL);   // standard constructor
	CRCColumnParameter(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRCColumnParameter();

// interface
public:
	//void SetCurData(T_RPSC_D* pData);
	//BOOL GetCurData(T_RPSC_D* pData);
	//void RedrawView();

// Dialog Data
	//{{AFX_DATA(CRCColumnParameter)
	enum { IDD = IDD_DGN_CON_DESIGN_PARAM_COLUMN };
	MButton	m_Ctrl_Auto_EquivalentMoment_Check;
	MEdit	m_Ctrl_Delta_Nsz_Edit;
	MEdit	m_Ctrl_Delta_Nsy_Edit;
	MEdit	m_Ctrl_Delta_Sz_Edit;
	MEdit	m_Ctrl_Delta_Sy_Edit;
	MEdit	m_Ctrl_CmzEdit;
	MEdit	m_Ctrl_CmyEdit;
	CBCGPStatic	m_Ctrl_Cmz;
	CBCGPStatic	m_Ctrl_Cmy;
	CBCGPStatic	m_Ctrl_Delta_Sz;
	CBCGPStatic	m_Ctrl_Delta_Sy;
	CBCGPStatic	m_Ctrl_Delta_Nsz;
	CBCGPStatic	m_Ctrl_Delta_Nsy;
	MButton	m_Ctrl_iYDirBd_Braced;
	MButton	m_Ctrl_iXDirBd_Braced;
	MButton	m_Ctrl_iYDirBd;
	MButton	m_Ctrl_iXDirBd;
	CBCGPStatic	m_Ctrl_EnableCtrl2;
	CBCGPStatic	m_Ctrl_EnableCtrl1;
	MCheckListBox	m_ltbxLcomServ;
	MCheckListBox	m_ltbxLcomStre;
	MCheckListBox	m_ltbxGroup;
	BOOL	m_bShearChk;
	double	m_dRhomax;
	double	m_dCmy;
	double	m_dCmz;
	BOOL	m_bAutoBoundary;
	int		m_iXDirBd;
	int		m_iYDirBd;
	BOOL	m_bIEnd;
	BOOL	m_bJEnd;
	BOOL	m_bMid;
	CString	m_strRemark;
	CTextUnit	m_dDCunit;
	CEditUnit	m_Dc;
	CString	m_SelectElemString;
	double	m_dDeltla_Nsy;
	double	m_dDeltla_Nsz;
	double	m_dDeltla_Sy;
	double	m_dDeltla_Sz;
	BOOL	m_bEquivalMoment;
	BOOL	m_bMomentEnlarge;
	BOOL	m_bMomentEnlargeNocross;
	MButton	m_chkCTC;
	MButton	m_chkCroseBd;
	MButton	m_chkNoCroseBd;
	CTextUnit	m_unitCTC;
	CEditUnit	m_editCTC;
	MEdit	m_editBetaDCroseY;
	MEdit	m_editBetaDCroseZ;
	MEdit	m_editBetaDNoCroseY;
	MEdit	m_editBetaDNoCroseZ;
	MButton	m_chkRfactor;
	CEditUnit	m_editRfactorY;
	CEditUnit	m_editRfactorZ;
	//}}AFX_DATA

	CSelectEdit m_editSelectElem;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRCColumnParameter)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:

	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
	
	void OnChangeCurrentRcco(T_ELEM_K key);

	void Click_Btn_Apply();

	BOOL Dlg2Data();
	BOOL Data2Dlg();

	void EnableCalcBtn(bool Enable);
	void ChangeGroupandLoadCombination();

	CMembCtrl* m_pMembCtrl;
	

protected:
	
	CRCDesignParamDlg* m_pParent;
	CDBDoc* m_pDoc;
	
	CDgnRCColumnGrid* m_ColumnGrid;
	
	CArray<T_ELEM_K,T_ELEM_K> m_arElemK;  
	BOOL m_bCheckPosi[3];
	T_RCCO_D m_Data;
	
	BOOL GetMaterialData(T_ELEM_K ElemK, double& dfck, double& dfy, double& dEratio);

	int addDelMod;

protected:

	CArray<T_LCOM_K, T_LCOM_K> m_arLcomKey;
	CArray<T_GRUP_K, T_GRUP_K> m_arGrupKey;
	CMap<T_LCOM_K,T_LCOM_K,T_LCOM_K,T_LCOM_K> m_arStreLcomKey;
	CMap<T_LCOM_K,T_LCOM_K,T_LCOM_K,T_LCOM_K> m_arServLcomKey;
	CMap<T_GRUP_K,T_GRUP_K,T_GRUP_K,T_GRUP_K> m_arGroupMapKey;
	CMap<T_ELEM_K,T_ELEM_K,T_RCCO_D,T_RCCO_D> m_RccoData;

	CArray<T_LCOM_K, T_LCOM_K> m_StreLcom;
	CArray<T_LCOM_K, T_LCOM_K> m_ServLcom;

	CArray<T_ELEM_K, T_ELEM_K&> m_pBadKeyList;
	CArray<T_ELEM_K, T_ELEM_K&> m_pNoExistREBTKeyList;
	CArray<T_ELEM_K, T_ELEM_K&> m_pBadMemberKeyList;
	
	void InitialData();

	void SetInitUnit();

	void Dlg2Data_LoadCase();
	void LoadDBtoDlg();
	void UpdateDgnData();
	
	BOOL DataCheck();
	BOOL ExistSectREBTByElem(T_ELEM_K eKey);

	void YZDirectionAllSwayEnableControl();

// Implementation
protected:
	// Generated message map functions`
	//{{AFX_MSG(CRCColumnParameter)
	virtual BOOL OnInitDialog();
	afx_msg void OnAddData();
	afx_msg void OnModifyData();
	afx_msg void OnDeleteData();
	afx_msg void OnCheck2();
	afx_msg void OnBtnApply();  
	BOOL m_bOnBtnApplyResult;
	afx_msg void OnDgnColumnCalcBtn();
	afx_msg void OnAutoEquivalentMomentChk();
	afx_msg void OnAutoMomentEnlargeNocrossChk();
	afx_msg void OnAutoMomentEnlargeChk();
	afx_msg void OnDgnColumnModifyConcreteMaterialBtn();
	afx_msg void OnDgnColumnUnbracedLengthBtn();
	afx_msg void OnDgnColumnEffectiveLengthBtn();
	afx_msg void OnClickYDirectionRadioBtn();
	afx_msg void OnClickZDirectionRadioBtn();
	afx_msg void OnClickChkCTC();
	afx_msg void OnClickChkCroseBd();
	afx_msg void OnClickChkNoCroseBd();
	afx_msg void OnClickRfactor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


protected:
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

#endif // !defined(__CRCCOLUMNPARAMETER_H__)
