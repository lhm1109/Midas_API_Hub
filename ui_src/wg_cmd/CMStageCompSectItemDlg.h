#if !defined(__CMSTAGECOMPSECTITEMDLG_H__)
#define __CMSTAGECOMPSECTITEMDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMStageCompSectItemDlg.h : header file
//
#include "..\wg_db\EditSect.h"
#include "..\wg_db\CobxSect.h"
#include "..\wg_db\EditMatl.h"
#include "..\wg_db\CobxMatl.h"

#include "..\wg_base\SpinBtnExCtrl.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;

struct SHAPE_BIT_PART
{
	TCHAR* strShape;
	TCHAR* uBitmapID;
	BOOL bNeedCompType;
};

/////////////////////////////////////////////////////////////////////////////
// CCMStageCompSectItemDlg dialog
class CDBDoc;
class CCMStageCompSectGrid;

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMStageCompSectItemDlg : public CDialogMove
{
// Construction
public:
	CCMStageCompSectItemDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMStageCompSectItemDlg();  

	void SetParam(T_CSCS_K key, T_CSCS_D& rData);
	void SetVirtualType(BOOL bVirtual = FALSE);
	BOOL IsModifyMode();
	BOOL GetAutoCalcH(int nIndex, CArray<double, double>& aH);
	BOOL CalAndGetStiff(int nIndex, T_SECT_STIFFNESS& rStiff_I, T_SECT_STIFFNESS& rStiff_J, T_SECT_STIFFNESS& rStiffAuto_I, T_SECT_STIFFNESS& rStiffAuto_J);
	BOOL CalAndGetCompStiff(int nIndex, T_SECT_STIFFNESS& rStiff_I, T_SECT_STIFFNESS& rStiff_J, CArray<T_SECT_STIFFNESS, T_SECT_STIFFNESS&> *paMultiStiff, BOOL& bHasMulti, UINT& ErrorMsg);
	int  GetPartNumber();

	BOOL Get_H_FromCscs(int nSize, T_CSCS_D CscsData, CArray<double, double>& aH);
	BOOL IsJapanCode(T_CSCS_BASE &Part, const T_CSCS_D& CscsD);
	BOOL IsCompNormalPar2() { return m_bCompNormalPar2; }

public:
// Dialog Data
	//{{AFX_DATA(CCMStageCompSectItemDlg)
	enum { IDD = IDD_CMD_SECT_FOR_CS_ITEM };
	MComboBox	m_cboCompositeType;
	MComboBox	m_cboActiveStage;
	CEditSect	m_editSection;
	CCobxSect	m_cboSection;
	CBCGPStatic	m_ctrlSectType;
	CBCGPStatic	m_ctrlSectShape;
	MEdit	m_editElementList;
	CFormulaEditSpin	m_edtPartNum;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMStageCompSectItemDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	BOOL m_bModify;
	BOOL m_bPropInit;
	T_CSCS_K m_Key;  
	T_CSCS_D m_Data;  
	BOOL m_bVirtual;

	CDBDoc* m_pDoc;
	MillustViewer m_wndPicture;
	static SHAPE_BIT_PART m_aHalfInfo[];
	static SHAPE_BIT_PART m_aShapeInfo[];
	CArray<T_ELEM_K, T_ELEM_K> m_arTargetElemList;

	BOOL m_bCompNormalPar2;

protected:
	void InitActiveStageCombo();
	void InitCompositeTypeCombo(T_SECT_D& data);
	void SetInitGrid(T_SECT_K nKey);
	void SetGridRowNumber(T_SECT_K nKey);
	void SetDefaultValue();
	void SetNullBitmap(int nOption=0);

	void SetData2Dlg();
	BOOL SetDlg2Data(T_CSCS_D& rData, BOOL bErrorChk=FALSE);
	void ChangeElememtList(T_STAG_K nStagKey, T_SECT_K nSectKey);
	void ChangeSection();
	BOOL ChangeBitmap(T_SECT_D& data);
	BOOL IsNeedCompositeType(T_SECT_D& rData);
	int GetCompositeType();
	int GetKindOfCompositeType(T_SECT_D& rData);
	CString ConvStypeToString(int nStype);

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMStageCompSectItemDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCmdSectIdCombo();
	afx_msg void OnSelchangeCmdActiveStageCombo();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnCmdApply();
	afx_msg void OnChangeCmdSectIdEdit();
	afx_msg void OnSelchangeCmdCompositeTypeCombo();
	afx_msg void OnChangeCmdPartNumEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CCMStageCompSectGrid* m_wndGrid;
	CArray<UINT, UINT> m_aPartGroup;
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSTAGECOMPSECTITEMDLG_H__)
