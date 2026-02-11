#if !defined(AFX_BNDRNSPRCALCDLG_H__66BE7321_B396_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_BNDRNSPRCALCDLG_H__66BE7321_B396_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BndrNsprCalcDlg.h : header file
//

#include "wg_treemenuRes2.h"
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxBngr.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CBndrNsprCalcDlg dialog

class CPileDB;
struct T_PISD_DATA
{
	CString strName;      // Pile Support Define Name
	int     nSupportType; // 1:code, 2:value
	int     nCode;        // CString strCodeName; 을 변경 1:KS specification(1996), 2:JGJ94-94, 3:US
	int     nType;        // 1:PC, 2:PHC, 3:Steel, 4:Cast-in-Place
	int     nConstraint;  // 1:driven, 2:augered, 3:pre-boring
	double  dDiameter; 
	double  dThickness;
	double  dElast;
	double  dArea;
	double  dLength;
	double  dStiffness;   // Support Stiffness
	int     nPileType;    // 1:normal, 2:tension, 3:compression
	int     nShape;       // 1:Round, 2:Square, 3:H  
	int     nHID;         // H단면 번호(PILE DB)
	double  dAlpha;       // Alpha값(미국만 사용)

	void Initialize()
	{
		strName.Empty();
		nSupportType = 1;
		nCode = 1;
		nType = 1;
		nConstraint = 1;
		dDiameter = 0.0;
		dThickness = 0.0;
		dElast = 0.0;
		dArea = 0.0;
		dLength = 0.0;
		dStiffness = 0.0;
		nPileType = 1;
		nShape = 1;    
		nHID = 0;
		dAlpha = 0.0;
	}
};

class CBndrNsprCalcDlg : public CDialogMove
{
	// Construction
public:
	CBndrNsprCalcDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBndrNsprCalcDlg();
//   void SetModifyData(T_PISD_K key, T_PISD_DATA& data);
	BOOL Dlg2Data();
	BOOL Data2Dlg();
	void SetCodeNameCobx();
	void SetPileTypeCobx();
	void SetDiaCobx();
	void SetThickCobx();
	void SetHSectionCobx();
	void SetModulusArea();
	void CalcStiffness();
	void AlignControl();
	void ChangePicture();
	void SectionDefinitionShowHide();  
	void ChangeItemByCode(int nCode);
	void ChangeAlpha();
	void SetPisdData(T_PISD_DATA& data) { m_pData = &data; }

	//  void CtrlController();
	// Dialog Data
	//{{AFX_DATA(CBndrNsprCalcDlg)
	enum { IDD = IDD_TM_BNDR_NSPR_CALC_DLG };
	CEditUnit	m_editAlpha;
	CEditUnit	m_castThick;
	CEditUnit	m_castDia;
	CEditUnit	m_stiffness2;
	CEditUnit	m_dia2;
	CTextUnit	m_thikunit;
	MComboBox	m_thik;
	MComboBox	m_CobxType;
	CTextUnit	m_stiffnessunit;
	CTextUnit	m_lengthunit;
	CTextUnit	m_elasticunit;
	CTextUnit	m_diaunit;
	CTextUnit	m_areaunit;
	CEditUnit	m_stiffness;
	CEditUnit	m_length;
	CEditUnit	m_elastic;
	CEditUnit	m_area;
	MComboBox m_HsectName;
	MComboBox	m_dia;
	MComboBox	m_codename;
	int		m_option; // 0:Normal, 1:Tension Only, 2:Compression Only
	int		m_type;
	int   m_nShapeRdo;
	mit::frx::MillustViewer m_wndPicture;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBndrNsprCalcDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	CArray<UINT, UINT> m_group1, m_group1_extra, m_group2, m_group3, m_group3_1, m_group3_2;
	CArray<UINT, UINT> m_group_jgj_shape_rdo, m_group_jgj_shape_txt, m_ps_dia_cmb;
	CArray<UINT, UINT> m_group_jgj_shape_rdo1, m_group_jgj_shape_rdo2, m_group_jgj_shape_rdo3; 
	CArray<UINT, UINT> m_group_jgj_Hsect, m_group_sect;
	CArray<UINT, UINT> m_group_sect_D, m_group_sect_T;
	CArray<UINT, UINT> m_PileTypeGroupNormal, m_PileTypeGroupCast, m_group_stiffness;
	CArray<UINT, UINT> m_aCtrlCodeName, m_aCtrlAlpha;
	BOOL      m_bInit, m_bElsMod;
	T_PISD_DATA* m_pData;
	CString   m_spiletype, m_sHSectName;
	DWORD     m_nCode;
	double    m_ddia, m_ddim, m_dthik;
	CPileDB*  m_pPileDB;
	CBitmap*  m_pBitmap;

	// Generated message map functions
	//{{AFX_MSG(CBndrNsprCalcDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdSupportType();
	afx_msg void OnSelchangeCmdHSectName();
	afx_msg void OnSelchangeCmdPsPiletype();
	afx_msg void OnSelchangeCmdPsCodename();
	afx_msg void OnSelchangeCmdPsDia();
	afx_msg void OnSelchangeCmdPsThik();
	afx_msg void OnChangeCmdPsElastic();
	afx_msg void OnChangeCmdPsArea();
	afx_msg void OnChangeCmdPsLength();
	afx_msg void OnEditchangeCmdPsDia();
	afx_msg void OnEditchangeCmdPsThik();
	afx_msg void OnSelectPileDetail();
	afx_msg void OnChangeCmdPsCastDia();
	afx_msg void OnChangeCmdPsCastThick();
	afx_msg void OnCmdJgjShapeRdo();
	afx_msg void OnChangeCmdPsAlpha();
	afx_msg void OnCalcualteBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	BOOL CalcSpring(T_PISD_DATA* pData);
public:
	void SetSpringType(int nType);
	double GetCalcValue();
};
/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_BNDRNSPRCALCDLG_H__66BE7321_B396_11D3_92DE_0000C0B0E6B3__INCLUDED_)