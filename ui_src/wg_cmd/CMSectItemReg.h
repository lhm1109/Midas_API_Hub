#if !defined(AFX_CMSECTITEMREG_H__673754E4_FBDD_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_CMSECTITEMREG_H__673754E4_FBDD_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemReg.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "../wg_db/SectShapeUtil.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\SpinBtnExCtrl.h"

#include "..\mit_frx\MEdit.h"
#include "..\MIT_frx\MButton.h"	
#include "..\mit_frx\MComboBox.h"

#include "CMSectItemBase.h"

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemReg dialog
using namespace mit::frx;
namespace SectItemReg
{
	constexpr const int firstSectNum = 11;
}

class CCMSectItemReg : public CCMSectItemBase
{
	struct SectItemRegResource
	{
		const int MaxByte = 100;
		std::vector<UINT> aBitmapID;
		std::vector<CString> aImgPath;
		std::vector<CString> aItemName;
		std::vector <DWORD> nIndex;

		std::vector<UINT> aBitmapID_JUD;
		std::vector<CString> aImgPath_Sect;
		std::vector<CString> aItemName_JUD;
		std::vector <DWORD> nIndex_JUD;

		SectItemRegResource()
		{
			const int nMovingType = CProduct::GetMovingType();

		#if defined(_MGEN)
			// JWKWON
			// EC3 압축을 받는 조립부재 설계시 Cross Angle 추가하기 위함(2014년 11월 출시..) 
			//nMovingType == D_PRODUCT_MOVING_US ? 19 : 17;

			if (nMovingType == D_PRODUCT_MOVING_US)
			{
				aBitmapID = {
					IDB_CMD_SEC_REG_01, IDB_CMD_SEC_REG_02, IDB_CMD_SEC_REG_03,
					IDB_CMD_SEC_REG_04, IDB_CMD_SEC_REG_05, IDB_CMD_SEC_REG_06,
					IDB_CMD_SEC_REG_07, IDB_CMD_SEC_REG_18, IDB_CMD_SEC_REG_08,
					IDB_CMD_SEC_REG_30, IDB_CMD_SEC_REG_09, IDB_CMD_SEC_REG_10,
					IDB_CMD_SEC_REG_13, IDB_CMD_SEC_REG_11, IDB_CMD_SEC_REG_31,
					IDB_CMD_SEC_REG_29, IDB_CMD_SEC_REG_28, IDB_CMD_SEC_REG_12,
					IDB_CMD_SEC_REG_26
				};
				aImgPath = {
					_T("SVG\\illustration\\ComboBox\\SecRegIcon01.svg"),
					_T("SVG\\illustration\\ComboBox\\SecRegIcon02.svg"),
					_T("SVG\\illustration\\ComboBox\\SecRegIcon03.svg"),
					_T("SVG\\illustration\\ComboBox\\SecRegIcon04.svg"),
					_T("SVG\\illustration\\ComboBox\\SecRegIcon05.svg"),
					_T("SVG\\illustration\\ComboBox\\SecRegIcon06.svg"),
					_T("SVG\\illustration\\ComboBox\\SecRegIcon07.svg"),
					_T("SVG\\illustration\\ComboBox\\SecRegIcon18.svg"),
					_T("SVG\\illustration\\ComboBox\\SecRegIcon08.svg"),
					_T("SVG\\illustration\\ComboBox\\SecRegIcon08B.svg"),
					_T("SVG\\illustration\\ComboBox\\SecRegIcon09.svg"),
					_T("SVG\\illustration\\ComboBox\\SecRegIcon10.svg"),
					_T("SVG\\illustration\\ComboBox\\SecRegIcon13.svg"),
					_T("SVG\\illustration\\ComboBox\\SecRegIcon11.svg"),
					_T("SVG\\illustration\\ComboBox\\SecRegIcon11H.svg"),
					_T("SVG\\illustration\\ComboBox\\SecRegIcon29.svg"),
					_T("SVG\\illustration\\ComboBox\\SecRegIcon28.svg"),
					_T("SVG\\illustration\\ComboBox\\SecRegIcon12.svg"),
					_T("SVG\\illustration\\ComboBox\\SecRegIcon26.svg")
				};
				aItemName = CSectShapeUtil::GetShapeNameList();
				nIndex = CSectShapeUtil::GetShapeIndexList();
			}
			else
			{
				aBitmapID = {
					IDB_CMD_SEC_REG_01, IDB_CMD_SEC_REG_02, IDB_CMD_SEC_REG_03,
					IDB_CMD_SEC_REG_04, IDB_CMD_SEC_REG_05, IDB_CMD_SEC_REG_06,
					IDB_CMD_SEC_REG_07, IDB_CMD_SEC_REG_18, IDB_CMD_SEC_REG_08,
					IDB_CMD_SEC_REG_09, IDB_CMD_SEC_REG_10,
					IDB_CMD_SEC_REG_13, IDB_CMD_SEC_REG_11,
					IDB_CMD_SEC_REG_29, IDB_CMD_SEC_REG_28, IDB_CMD_SEC_REG_12,
					IDB_CMD_SEC_REG_26
				};
				aImgPath = {
					_T("SVG\\illustration\\ComboBox\\SecRegIcon01.svg"),
					_T("SVG\\illustration\\ComboBox\\SecRegIcon02.svg"),
					_T("SVG\\illustration\\ComboBox\\SecRegIcon03.svg"),
					_T("SVG\\illustration\\ComboBox\\SecRegIcon04.svg"),
					_T("SVG\\illustration\\ComboBox\\SecRegIcon05.svg"),
					_T("SVG\\illustration\\ComboBox\\SecRegIcon06.svg"),
					_T("SVG\\illustration\\ComboBox\\SecRegIcon07.svg"),
					_T("SVG\\illustration\\ComboBox\\SecRegIcon18.svg"),
					_T("SVG\\illustration\\ComboBox\\SecRegIcon08.svg"),
					_T("SVG\\illustration\\ComboBox\\SecRegIcon09.svg"),
					_T("SVG\\illustration\\ComboBox\\SecRegIcon10.svg"),
					_T("SVG\\illustration\\ComboBox\\SecRegIcon13.svg"),
					_T("SVG\\illustration\\ComboBox\\SecRegIcon11.svg"),
					_T("SVG\\illustration\\ComboBox\\SecRegIcon29.svg"),
					_T("SVG\\illustration\\ComboBox\\SecRegIcon28.svg"),
					_T("SVG\\illustration\\ComboBox\\SecRegIcon12.svg"),
					_T("SVG\\illustration\\ComboBox\\SecRegIcon26.svg")
				};
				aItemName = CSectShapeUtil::GetShapeNameList();
				nIndex = CSectShapeUtil::GetShapeIndexList();
			}
		#elif defined(_CIVIL)
			aBitmapID = {
				IDB_CMD_SEC_REG_01, IDB_CMD_SEC_REG_02, IDB_CMD_SEC_REG_03,
				IDB_CMD_SEC_REG_04, IDB_CMD_SEC_REG_05, IDB_CMD_SEC_REG_06,
				IDB_CMD_SEC_REG_07, IDB_CMD_SEC_REG_08, IDB_CMD_SEC_REG_09,
				IDB_CMD_SEC_REG_10,
				IDB_CMD_SEC_REG_13, IDB_CMD_SEC_REG_14, IDB_CMD_SEC_REG_21,
				IDB_CMD_SEC_REG_15, IDB_CMD_SEC_REG_16, IDB_CMD_SEC_REG_17,
				IDB_CMD_SEC_REG_11, IDB_CMD_SEC_REG_28, IDB_CMD_SEC_REG_12,
				IDB_CMD_SEC_REG_22, IDB_CMD_SEC_REG_23,
				IDB_CMD_SEC_REG_26,
			};
			aImgPath = {
				_T("SVG\\illustration\\ComboBox\\SecRegIcon01.svg"),
				_T("SVG\\illustration\\ComboBox\\SecRegIcon02.svg"),
				_T("SVG\\illustration\\ComboBox\\SecRegIcon03.svg"),
				_T("SVG\\illustration\\ComboBox\\SecRegIcon04.svg"),
				_T("SVG\\illustration\\ComboBox\\SecRegIcon05.svg"),
				_T("SVG\\illustration\\ComboBox\\SecRegIcon06.svg"),
				_T("SVG\\illustration\\ComboBox\\SecRegIcon07.svg"),
				_T("SVG\\illustration\\ComboBox\\SecRegIcon08.svg"),
				_T("SVG\\illustration\\ComboBox\\SecRegIcon09.svg"),
				_T("SVG\\illustration\\ComboBox\\SecRegIcon10.svg"),
				_T("SVG\\illustration\\ComboBox\\SecRegIcon13.svg"),
				_T("SVG\\illustration\\ComboBox\\SecRegIcon14.svg"),
				_T("SVG\\illustration\\ComboBox\\SecRegIcon21.svg"),
				_T("SVG\\illustration\\ComboBox\\SecRegIcon15.svg"),
				_T("SVG\\illustration\\ComboBox\\SecRegIcon16.svg"),
				_T("SVG\\illustration\\ComboBox\\SecRegIcon17.svg"),
				_T("SVG\\illustration\\ComboBox\\SecRegIcon11.svg"),
				_T("SVG\\illustration\\ComboBox\\SecRegIcon28.svg"),
				_T("SVG\\illustration\\ComboBox\\SecRegIcon12.svg"),
				_T("SVG\\illustration\\ComboBox\\SecRegIcon22.svg"),
				_T("SVG\\illustration\\ComboBox\\SecRegIcon23.svg"),
				_T("SVG\\illustration\\ComboBox\\SecRegIcon26.svg")
			};
			aItemName = CSectShapeUtil::GetShapeNameList();
			nIndex = CSectShapeUtil::GetShapeIndexList();
			/*
			#if defined(_US)
				aItemName[2] = _LS(IDS_WG_CMD__ADDD__I_Section);
			#endif
			*/

		#elif defined(_TOWER)
			aBitmapID = {
				IDB_CMD_SEC_REG_01, IDB_CMD_SEC_REG_06,
				IDB_CMD_SEC_REG_07, IDB_CMD_SEC_REG_18,
			};
			aItemName = CSectShapeUtil::GetShapeNameList();
			nIndex = CSectShapeUtil::GetShapeIndexList();
		#else
		#error _LS(IDS_CMD0417__Unknown_Product_Definition)
		#endif

			// 일본 2차 설계에 Cold Formed Box 가 들어감
			aBitmapID_JUD = {
					IDB_CMD_SEC_REG_01, IDB_CMD_SEC_REG_02, IDB_CMD_SEC_REG_03,
					IDB_CMD_SEC_REG_04, IDB_CMD_SEC_REG_05, IDB_CMD_SEC_REG_06,
					IDB_CMD_SEC_REG_07, IDB_CMD_SEC_REG_08, IDB_CMD_SEC_REG_09,
					IDB_CMD_SEC_REG_10,
					IDB_CMD_SEC_REG_11, IDB_CMD_SEC_REG_27, IDB_CMD_SEC_REG_29, IDB_CMD_SEC_REG_28,
					IDB_CMD_SEC_REG_12, IDB_CMD_SEC_REG_26
			};
			aItemName_JUD = {
					_LS(IDS_WG_CMD__ADDD__Angle),		_LS(IDS_WG_CMD__ADDD__Channel),			_LS(IDS_WG_CMD__ADDD__H_Section),
					_LS(IDS_WG_CMD__ADDD__T_Section),	_LS(IDS_WG_CMD__ADDD__Box),				_LS(IDS_WG_CMD__ADDD__Pipe),
					_LS(IDS_WG_CMD__ADDD__Double_Angle), _LS(IDS_WG_CMD__ADDD__Double_Channel), _LS(IDS_WG_CMD__ADDD__Solid_Rectangle),
					_LS(IDS_WG_CMD__ADDD__Solid_Round),
					_LS(IDS_WG_CMD__ADDD__Cold_Formed_Channel), _LS(IDS_WG_CMD__ADDD__Cold_Formed_Box), _LS(IDS_WG_CMD__ADDD__Z_Section), _LS(IDS_WG_CMD__ADDD__Upright),
					_LS(IDS_WG_CMD__ADD2__U_RIB),_LS(IDS_WG_CMD__ADDD__Upside_Down_T_Section),
			};
			nIndex_JUD = {
					0,  1,  2,
					3,  4,  5,
					6,  7,  8,
					9,
					10, 23, 25, 24,
					11, 22,
			};

			aImgPath_Sect = {
			_T("SVG\\illustration\\Dialog\\Sec01.svg"),
			_T("SVG\\illustration\\Dialog\\Sec19.svg"),
			_T("SVG\\illustration\\Dialog\\Sec20.svg"),
			_T("SVG\\illustration\\Dialog\\Sec04.svg"),
			_T("SVG\\illustration\\Dialog\\Sec05.svg"),
			_T("SVG\\illustration\\Dialog\\Sec06.svg"),
			_T("SVG\\illustration\\Dialog\\Sec07.svg"),
			_T("SVG\\illustration\\Dialog\\Sec08.svg"),
			_T("SVG\\illustration\\Dialog\\Sec09.svg"),
			_T("SVG\\illustration\\Dialog\\Sec10.svg"),
			_T("SVG\\illustration\\Dialog\\Sec11.svg"),
			_T("SVG\\illustration\\Dialog\\Sec12.svg"),
			_T("SVG\\illustration\\Dialog\\Sec13.svg"),
			_T("SVG\\illustration\\Dialog\\Sec14.svg"),
			_T("SVG\\illustration\\Dialog\\Sec15.svg"),
			_T("SVG\\illustration\\Dialog\\Sec16.svg"),
			_T("SVG\\illustration\\Dialog\\Sec17.svg"),
			_T("SVG\\illustration\\Dialog\\Sec18.svg"),
			_T("SVG\\illustration\\Dialog\\Sec21.svg"),
			_T("SVG\\illustration\\Dialog\\Sec23.svg"),
			_T("SVG\\illustration\\Dialog\\Sec24.svg"),
			_T("SVG\\illustration\\Dialog\\Sec25.svg"),
			_T("SVG\\illustration\\Dialog\\Sec26.svg"),
			_T("SVG\\illustration\\Dialog\\Sec27.svg"),
			_T("SVG\\illustration\\Dialog\\Sec28.svg"),
			_T("SVG\\illustration\\Dialog\\Sec29.svg"),
			_T("SVG\\illustration\\Dialog\\Sec30.svg"),
			_T("SVG\\illustration\\Dialog\\Sec08B.svg"),
			};
		}
	};
// Construction
public:
	CCMSectItemReg(CWnd* pParent = NULL);   // standard constructor
	~CCMSectItemReg();
//---------------------------------------------------
// Overrides
//---------------------------------------------------
public:
	BOOL OnOKPublic() override;
	BOOL OnApplyPublic() override;
	void ShowCalcResult() override;
	void DisplayOffsetPoint() override;
	void SetNewID(T_SECT_K nNewID) override;
	enum EN_DB_CASE
	{
		EN_USER=0, EN_DB, EN_COMMERCIAL,
	};

	enum EN_SECT_SHAPE_REG_CC
	{
		IS=0,
		IW,
		BS,
		BW,
	};
//---------------------------------------------------
// Interface
//---------------------------------------------------
public:
	BOOL GetSectData(T_SECT_D& sect_data); //add hjs 03/03/12
//---------------------------------------------------
// Implementation
//---------------------------------------------------
protected:
	BOOL SaveData();
	BOOL Dlg2Data();
	BOOL GetPeriAndStiff();

	void InitControls();
	void ShowKey2Dlg();
	void ShowData2Dlg();
	void SetSectionShapeCombo();
	void SetDBNameList();
	void SetCommercialDBNameList();
	void SetFirstNameCombo();
	void SetFirstSectData();
	void ChangeBitmap();
	void SetDoubleTypeSectCtrl();
	void SetBuiltUp();
	void DoActionWhenDBChanged();
	void ChangeCellNumber(int nCellNum);

	CString GetDB();
	void    SetDB(CString& csDBName);

	int GetCurShapeIndex();
	void SetCurShapeByIndex(int nShapeIndex);
	void ShowHideControlsByShape(CString &Shape);

	// cc 관련
	void SetCCShapeCobx(int nCCShape);
	CString GetCurCCShape();

	BOOL IsEnableCommercialDB();
	void AlignCommercialDB();

protected:
	CArray<UINT, UINT> m_aCtrlDblType;
	CArray<UINT, UINT> m_aCtrlDB;
	CArray<UINT, UINT> m_aCtrlROct;   
	CArray<UINT, UINT> m_aCtrlStiff1, m_aCtrlStiff2;  // stiffener number
	CArray<UINT, UINT> m_aCtrlHSQ;  
	CArray<UINT, UINT> m_aCtrlRoundType;
	CArray<UINT, UINT> m_aCtrlPrincipalAxis;

	// cc 관련
	CArray<UINT, UINT> m_aCtrlColdForm;

	// Commercial DB Enable Disable로 인해 move할 control들
	CArray<UINT, UINT> m_aCtrlMoveByCommDB;
	CArray<UINT, UINT> m_aCtrlCommDB;

	// For changing section name by automatically
	BOOL    m_bAutoChangeName;
	BOOL	m_bShowCommercialDB;
	int     m_nDistCommercial;
	SectItemRegResource m_SectItemRes;

public:
// Dialog Data
	//{{AFX_DATA(CCMSectItemReg)
	enum { IDD = IDD_CMD_SECT_ITEM_REG };
	CFormulaEditSpin	m_editN2;
	CFormulaEditSpin	m_editN1;
	CBCGPStatic	m_txtOffset;
	CFormulaEditSpin	m_editCellNum;
	CBCGPStatic	m_wndDblTitle;
	CCobxAutoFill	m_wndDblSect;
	MComboBox	m_wndDblDB;
	MButton	m_wndBuiltUp;
	CCobxAutoFill	m_wndFirstName;
	MComboBox	m_wndDB;
	MComboBox	m_wndCommecialDB;
	MComboBox	m_cboType;
	MEdit	m_wndName;
	MEdit	m_wndID;
	MComboBox m_wndCCNum;
	MComboBox m_wndCCShape;
	MButton m_chkShearDeform;
	MButton m_chkWarpingEffect;
	//}}AFX_DATA
	CBCGPStatic   m_wndFirstTitle[SectItemReg::firstSectNum];
	CEditUnit	m_wndFirstSize[SectItemReg::firstSectNum];
	CTextUnit m_wndFirstUnit[SectItemReg::firstSectNum];
	MButton	m_wndHSQType;
	MButton m_wndRoundType;
	MButton	m_wndPrincipalAxis;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemReg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMSectItemReg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeShape();
	afx_msg void OnChangeDB();
	afx_msg void OnChangeFirstName();
	afx_msg void OnChangeDblDB();
	afx_msg void OnChangeDblSectName();
	afx_msg void OnChangeFirstSize();
	afx_msg void OnChangeBuiltUp();
	afx_msg void OnChangeUseDB();
	afx_msg void OnSetfocusCmdSpIdName();
	afx_msg void OnUpdateCmdSpIdName();
	afx_msg void OnSelchangeCmdSpIdCcNumCobx();
	afx_msg void OnSelchangeCmdSpIdCcShapeCobx();
	afx_msg void OnCmdSectCentroidBtn();
	afx_msg void OnChangeCmdEditCellNum();
// 	afx_msg void OnDeltaposCmdSpinCellNum(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdChangeOffsetBtn();
	afx_msg void OnChangeCmdSpIdValueN1();
	afx_msg void OnChangeCmdSpIdValueN2();
// 	afx_msg void OnDeltaposCmdSpIdSpinN1(NMHDR* pNMHDR, LRESULT* pResult);
// 	afx_msg void OnDeltaposCmdSpIdSpinN2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeHSQType();
	afx_msg void OnChangeRoundType();
	afx_msg void OnChangePrincipalAxis();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSECTITEMREG_H__673754E4_FBDD_11D3_92DE_0000C0B0E6B3__INCLUDED_)
