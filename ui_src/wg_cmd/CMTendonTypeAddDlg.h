
#include "..\wg_db\EditMatl.h"
#include "..\wg_db\EditSect.h"
#include "..\wg_db\EditThik.h"
#include "..\wg_db\CobxMatl.h"
#include "..\wg_db\CobxSect.h"
#include "..\wg_db\CobxThik.h"
#include "..\wg_db\CobxTdmf.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\mit_frx\MEdit.h"
#include "..\mit_frx\MComboBox.h" 

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CCMTendonTypeAddDlg dialog
class CCMTendonTypeAddDlg :  public CChildDialogMove 
{
public:
	CCMTendonTypeAddDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_CMD_DEF_TENDON_TYPE_ADD_DLG };

	MComboBox m_wndRelaxJTG04Cmb;
	MEdit	m_wndRelaxEdit;
	MComboBox	m_cobxRelaxMethod;
	CTextUnit m_wndWobbleUnit;
	CEditUnit m_wndWobble;
	CTextUnit m_wndWobbleEUROUnit;
	CEditUnit m_wndWobbleEURO;
	CTextUnit m_wndWobbleAngleEUROUnit;
	CEditUnit m_wndWobbleAngleEURO;
	CEditUnit m_wndWobbleFriction;
	MComboBox 	m_wndRelaxCombo;
	CEditUnit	m_wndAnchoEnd;
	CEditUnit	m_wndAnchoBegin;
	CTextUnit	m_wndLengthUnit2;
	CTextUnit	m_wndLengthUnit1;
	CTextUnit	m_PressUnit1;
	CTextUnit	m_PressUnit2;
	CTextUnit m_PressUnit3;
	CTextUnit m_wndFpkUnit;
	CEditUnit m_wndFpkEdit;
	CEditUnit	m_wndDuctArea;
	MComboBox	m_cmbDgnCode;
	MComboBox	m_cmbDgnDia;
	CEditUnit	m_edtTendonNum;
	CEditUnit	m_wndTArea;
	MComboBox	m_wndTendonTypeCombo;
	CTextUnit	m_wndAUnit2;
	CTextUnit	m_wndAUnit1;
	CEditMatl	m_wndMatEdit;
	CCobxMatl	m_wndMatCombo;
	CEditUnit m_edtXi;
	CString	m_strDuctArea;
	CString	m_strFriction;
	CString	m_strRelCoef;
	CString	m_strTendonName;
	CString	m_strTotalTendonArea;
	CString	m_strUltimate;
	CString	m_strYield;
	CString	m_strExternal;
	BOOL	m_bRelaxation;
	int		m_nBondType;
	CBCGPStatic m_wndDuctDiameter;
	BOOL	m_bOverStreRedFactor;
	CCobxTdmf	m_CobxRelax;
	MComboBox	m_cobxRelaxCase;
	MComboBox	m_cobxABNTRelaxCase;
	BOOL    m_bLowRelax;

public:
	void SetTDNTD(T_TDNT_K TndtK); // (0) Add (1) Modify

protected:
	void SetDefaultUSData();
	void SetData4Euro();
	void SetDesignCode();
	void SetDesignDia();
	void SetControl();
	void SetUnitType();
	void SetMatlCombo();
	void SetTendonTypeCombo();
	void SetTendonAreaCombo();
	void SetRelaxCoeffCombo();
	void SetRelaxEtcCombo();
	void ShowHideControl();

	void AlignControls();
	void Data2Dlg();

	void SetDiameterField();
	BOOL GetFpbWhenAS(double& dFpb);
	void EnableControl();
	void SetFieldData();
	void ChangeMaterial();
	void EnableRelaxUserDefined();
	BOOL ExecuteDlg();
	void RelaxCtrlMan();

	void CalcTendonArea();
	void EnableControlsByDgnCode(const CString& strCode);
	void ReplaceMMRusToEng(CString& str);
	void ReplaceMMEngToRus(CString& str);
protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	afx_msg void OnCmdMatButton();
	afx_msg void OnCmdTareaButton();
	afx_msg void OnCmdApply();
	afx_msg void OnCmdRelaxCheck();
	afx_msg void OnSelchangeMatCombo();
	afx_msg void OnSelchangeCmdRelaxCombo();
	afx_msg void OnSelchangeCmdTendonTypeCombo();
	afx_msg void OnChangeCmdTotalTendonAreaEdit();
	afx_msg void OnSelchangeCmdRelaxCombo2();
	afx_msg void OnCmdSetLongTermStressLossBtn();
	afx_msg void OnSelchangeCmdABNTRelaxTypeCombo();
	void AlignControlsDynamic();

	afx_msg void OnChangeCmdMatEdit();
	afx_msg void OnCmdRelaxUserBtn();
	afx_msg void OnCmdRelaxWobblrBtn();
	afx_msg void OnChangeCmdFrictionEdit();
	afx_msg void OnChangeCmdWobbleEuroEdit();
	afx_msg void OnChangeCmdWobbleAngleEuroEdit();
	afx_msg void OnSelChangeCmdDesignCode();
	afx_msg void OnSelChangeCmdDesignDia();
	afx_msg void OnChangeCmdTendonNum();
	afx_msg void OnDeltaposCmdTendonNumSpin(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

private:
	T_TDNT_D          m_TdntData;
	T_TDNT_K          m_TdntKey;
	int               m_nRelaxation;
	BOOL              m_bIsDBError;
	CString			  m_strAreaNum;

	CArray<UINT, UINT> m_aGroup;
	CArray<UINT, UINT> m_aCtrlRelaxModelCEB;
	CArray<UINT, UINT> m_aCtrlRelaxModelINDIA;
	CArray<UINT, UINT> m_aCtrlTypeInternal;
	CArray<UINT, UINT> m_aWoodle, m_aWoodleEuro;
	CArray<UINT,UINT>  m_IncCtrls1, m_IncCtrls2, m_IncCtrls3, m_IncCtrls4, m_IncCtrls5;
	CArray<UINT, UINT> m_aWobbleType;

};
