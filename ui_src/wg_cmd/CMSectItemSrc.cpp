// CMSectItemSrc.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemSrc.h"

#include "..\wg_base\wg_base_TestEnvMgr.h"
#include "..\wg_base\wg_base_LocaleLib.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_SectDB.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\SectDBRussiaUtil.h"

#include "CMSecPageStiffDlg.h"
#include "CMConcrSteelMatlDlg.h"
#include "CMSectOffsetDlg.h"

// Default SRC Section Shape
#define D_SECT_SRC_DEF_SHAPE D_SECT_SHAPE_SRC_RBO

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemSrc dialog
#define   CDialog   CCMSectItemBase

namespace SectItemSrcResource
{
	std::vector<UINT> aBitmapID(bool bIncludeExtra)  // Not Used in NX
	{
		std::vector<UINT> vec = {
			IDB_CMD_SEC_SRC_01, IDB_CMD_SEC_SRC_02, IDB_CMD_SEC_SRC_03,
			IDB_CMD_SEC_SRC_04, IDB_CMD_SEC_SRC_05, IDB_CMD_SEC_SRC_06,
			IDB_CMD_SEC_SRC_07, IDB_CMD_SEC_SRC_08, IDB_CMD_SEC_SRC_09,
			IDB_CMD_SEC_SRC_10, IDB_CMD_SEC_SRC_11, IDB_CMD_SEC_SRC_12,
			IDB_CMD_SEC_SRC_13, IDB_CMD_SEC_SRC_15, IDB_CMD_SEC_SRC_16,
			IDB_CMD_SEC_SRC_17
		};

		if (bIncludeExtra)
		{
			vec.push_back(IDB_CMD_SEC_SRC_18); // 18 SRC-Single-Tube
			vec.push_back(IDB_CMD_SEC_SRC_19); // 19 덤벨 단면
		}

		return vec;
	}

	std::vector<CString> aImgPath(bool bIncludeExtra)
	{
		std::vector<CString> vec = {
			_T("SVG\\illustration\\ComboBox\\SecSrcIcon01.svg"),
			_T("SVG\\illustration\\ComboBox\\SecSrcIcon02.svg"),
			_T("SVG\\illustration\\ComboBox\\SecSrcIcon03.svg"),
			_T("SVG\\illustration\\ComboBox\\SecSrcIcon04.svg"),
			_T("SVG\\illustration\\ComboBox\\SecSrcIcon05.svg"),
			_T("SVG\\illustration\\ComboBox\\SecSrcIcon06.svg"),
			_T("SVG\\illustration\\ComboBox\\SecSrcIcon07.svg"),
			_T("SVG\\illustration\\ComboBox\\SecSrcIcon08.svg"),
			_T("SVG\\illustration\\ComboBox\\SecSrcIcon09.svg"),
			_T("SVG\\illustration\\ComboBox\\SecSrcIcon10.svg"),
			_T("SVG\\illustration\\ComboBox\\SecSrcIcon11.svg"),
			_T("SVG\\illustration\\ComboBox\\SecSrcIcon12.svg"),
			_T("SVG\\illustration\\ComboBox\\SecSrcIcon13.svg"),
			_T("SVG\\illustration\\ComboBox\\SecSrcIcon15.svg"),
			_T("SVG\\illustration\\ComboBox\\SecSrcIcon16.svg"),
			_T("SVG\\illustration\\ComboBox\\SecSrcIcon17.svg")
		};

		if (bIncludeExtra)
		{
			vec.push_back(_T("SVG\\illustration\\ComboBox\\SecSrcIcon18.svg")); // 18 SRC-Single-Tube
			vec.push_back(_T("SVG\\illustration\\ComboBox\\SecSrcIcon19.svg"));	// 19 덤벨 단면
		}

		return vec;
	}

	std::vector<CString> aImgPath_Sect(bool bIncludeExtra)
	{
		std::vector<CString> vec = {
			_T("SVG\\illustration\\Dialog\\SecSrc01.svg"),
			_T("SVG\\illustration\\Dialog\\SecSrc02.svg"),
			_T("SVG\\illustration\\Dialog\\SecSrc03.svg"),
			_T("SVG\\illustration\\Dialog\\SecSrc04.svg"),
			_T("SVG\\illustration\\Dialog\\SecSrc05.svg"),
			_T("SVG\\illustration\\Dialog\\SecSrc06.svg"),
			_T("SVG\\illustration\\Dialog\\SecSrc07.svg"),
			_T("SVG\\illustration\\Dialog\\SecSrc08.svg"),
			_T("SVG\\illustration\\Dialog\\SecSrc09.svg"),
			_T("SVG\\illustration\\Dialog\\SecSrc10.svg"),
			_T("SVG\\illustration\\Dialog\\SecSrc11.svg"),
			_T("SVG\\illustration\\Dialog\\SecSrc12.svg"),
			_T("SVG\\illustration\\Dialog\\SecSrc13.svg"),
			_T("SVG\\illustration\\Dialog\\SecSrc15.svg"),
			_T("SVG\\illustration\\Dialog\\SecSrc16.svg"),
			_T("SVG\\illustration\\Dialog\\SecSrc17.svg")
		};

		if (bIncludeExtra)
		{
			vec.push_back(_T("SVG\\illustration\\Dialog\\SecSrc18.svg")); // 18 SRC-Single-Tube
			vec.push_back(_T("SVG\\illustration\\Dialog\\SecSrc19.svg")); // 19 덤벨 단면
		}

		return vec;
	}

	std::vector<CString> aItemName(bool bIncludeExtra)
	{
		std::vector<CString> vec = {
			_LS(IDS_WG_CMD__ADDD__Rect_Box_Open), 
			_LS(IDS_WG_CMD__ADDD__Rect_Box_Close), 
			_LS(IDS_WG_CMD__ADDD__Rect_Pipe_Open),
			_LS(IDS_WG_CMD__ADDD__Rect_Pipe_Close), 
			_LS(IDS_WG_CMD__ADDD__Circle_Box_Open), 
			_LS(IDS_WG_CMD__ADDD__Circle_Box_Close),
			_LS(IDS_WG_CMD__ADDD__Circle_Pipe_Open), 
			_LS(IDS_WG_CMD__ADDD__Circle_Pipe_Close),
			_LS(IDS_WG_CMD__ADDD__SRC_Box),
			_LS(IDS_WG_CMD__ADDD__SRC_Pipe), 
			_LS(IDS_WG_CMD__ADDD__Rect_HBeam), 
			_LS(IDS_WG_CMD__ADDD__Circle_HBeam),
			_LS(IDS_WG_CMD__ADDD__Rect_H2T), 
			_LS(IDS_WG_CMD__ADDD__Rect_TH), 
			_LS(IDS_WG_CMD__ADDD__SRC_Box_Stiffener),
			_LS(IDS_WG_CMD__ADDD__SRC_Pipe_Stiffener)
		};

		if (bIncludeExtra)
		{
			vec.push_back(_LS(IDS_WG_CMD__ADDD__SRC_Single_Tube)); // 18 SRC-Single-Tube
			vec.push_back(_LS(IDS_WG_CMD__ADDD__SRC_Dumbbel    )); // 19 덤벨 단면
		}

		return vec;
	}
}

CCMSectItemSrc::CCMSectItemSrc(CWnd* pParent /*=NULL*/)
	: CDialog(CCMSectItemSrc::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectItemSrc)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bDumb = FALSE;

	m_pBitmap = NULL;
	m_pImageList = NULL;
	m_nCurShapeType = 1;

	m_aCtrlDB.Add(IDC_CMD_SP_ID_USER_RADIO);
	m_aCtrlDB.Add(IDC_CMD_SP_ID_DB_RADIO);

	m_aCtrlReplace.Add(IDC_CMD_SP_ID_FSTEEL);
	m_aCtrlReplace.Add(IDC_CMD_SP_ID_FCONCR);

	// the others except SRC with stiffener
	m_aCtrlType0.Add(IDC_CMD_SP_ID_FRAME_STEEL);
	m_aCtrlType0.Add(IDC_CMD_SP_ID_USER_RADIO);
	m_aCtrlType0.Add(IDC_CMD_SP_ID_DB_RADIO);
	m_aCtrlType0.Add(IDC_CMD_SP_ID_CODE);

	// the others except Rect-H2T, Rect-TH
	m_aCtrlType1.Add(IDC_CMD_SP_ID_DBNAME_TITLE);
	m_aCtrlType1.Add(IDC_CMD_SP_ID_DBNAME);
	m_aCtrlType1.Add(IDC_CMD_SP_ID_BUILTUP);
	m_aCtrlType1.Add(IDC_CMD_SP_ID_TITLE1);
	m_aCtrlType1.Add(IDC_CMD_SP_ID_VALUE1);
	m_aCtrlType1.Add(IDC_CMD_SP_ID_UNIT1);
	m_aCtrlType1.Add(IDC_CMD_SP_ID_TITLE2);
	m_aCtrlType1.Add(IDC_CMD_SP_ID_VALUE2);
	m_aCtrlType1.Add(IDC_CMD_SP_ID_UNIT2);
	m_aCtrlType1.Add(IDC_CMD_SP_ID_TITLE3);
	m_aCtrlType1.Add(IDC_CMD_SP_ID_VALUE3);
	m_aCtrlType1.Add(IDC_CMD_SP_ID_UNIT3);
	m_aCtrlType1.Add(IDC_CMD_SP_ID_TITLE4);
	m_aCtrlType1.Add(IDC_CMD_SP_ID_VALUE4);
	m_aCtrlType1.Add(IDC_CMD_SP_ID_UNIT4);
	m_aCtrlType1.Add(IDC_CMD_SP_ID_TITLE5);
	m_aCtrlType1.Add(IDC_CMD_SP_ID_VALUE5);
	m_aCtrlType1.Add(IDC_CMD_SP_ID_UNIT5);
	m_aCtrlType1.Add(IDC_CMD_SP_ID_TITLE6);
	m_aCtrlType1.Add(IDC_CMD_SP_ID_VALUE6);
	m_aCtrlType1.Add(IDC_CMD_SP_ID_UNIT6);

	// Rect-H2T, Rect-TH
	m_aCtrlType2.Add(IDC_CMD_SP_ID2_DBNAME_TITLE);
	m_aCtrlType2.Add(IDC_CMD_SP_ID2_DBNAME);
	m_aCtrlType2.Add(IDC_CMD_SP_ID2_TITLE1);
	m_aCtrlType2.Add(IDC_CMD_SP_ID2_VALUE1);
	m_aCtrlType2.Add(IDC_CMD_SP_ID2_TITLE2);
	m_aCtrlType2.Add(IDC_CMD_SP_ID2_VALUE2);
	m_aCtrlType2.Add(IDC_CMD_SP_ID2_TITLE3);
	m_aCtrlType2.Add(IDC_CMD_SP_ID2_VALUE3);
	m_aCtrlType2.Add(IDC_CMD_SP_ID2_TITLE4);
	m_aCtrlType2.Add(IDC_CMD_SP_ID2_VALUE4);
	m_aCtrlType2.Add(IDC_CMD_SP_ID2_DBNAME_TITLE2);
	m_aCtrlType2.Add(IDC_CMD_SP_ID2_DBNAME2);
	m_aCtrlType2.Add(IDC_CMD_SP_ID2_TITLE5);
	m_aCtrlType2.Add(IDC_CMD_SP_ID2_VALUE5);
	m_aCtrlType2.Add(IDC_CMD_SP_ID2_TITLE6);
	m_aCtrlType2.Add(IDC_CMD_SP_ID2_VALUE6);
	m_aCtrlType2.Add(IDC_CMD_SP_ID2_TITLE7);
	m_aCtrlType2.Add(IDC_CMD_SP_ID2_VALUE7);
	m_aCtrlType2.Add(IDC_CMD_SP_ID2_TITLE8);
	m_aCtrlType2.Add(IDC_CMD_SP_ID2_VALUE8);
	m_aCtrlType2.Add(IDC_CMD_SP_ID2_UNIT_TITLE);
	m_aCtrlType2.Add(IDC_CMD_SP_ID2_UNIT);

	m_aCtrlType3.Add(IDC_CMD_SP_ID_FRAME_STEEL3);
	m_aCtrlType3.Add(IDC_CMD_SP_ID3_TITLE1);
	m_aCtrlType3.Add(IDC_CMD_SP_ID3_VALUE1);
// 	m_aCtrlType3.Add(IDC_CMD_SP_ID3_SPIN1);
	m_aCtrlType3.Add(IDC_CMD_SP_ID3_TITLE2);
	m_aCtrlType3.Add(IDC_CMD_SP_ID3_VALUE2);
// 	m_aCtrlType3.Add(IDC_CMD_SP_ID3_SPIN2);
	m_aCtrlType3.Add(IDC_CMD_SP_ID3_TITLE3);
	m_aCtrlType3.Add(IDC_CMD_SP_ID3_VALUE3);
	m_aCtrlType3.Add(IDC_CMD_SP_ID3_UNIT3);
	m_aCtrlType3.Add(IDC_CMD_SP_ID3_TITLE4);
	m_aCtrlType3.Add(IDC_CMD_SP_ID3_VALUE4);
	m_aCtrlType3.Add(IDC_CMD_SP_ID3_UNIT4);
	m_aCtrlType3.Add(IDC_CMD_SP_ID3_TITLE5);
	m_aCtrlType3.Add(IDC_CMD_SP_ID3_VALUE5);
	m_aCtrlType3.Add(IDC_CMD_SP_ID3_UNIT5);
	m_aCtrlType3.Add(IDC_CMD_SP_ID3_TITLE6);
	m_aCtrlType3.Add(IDC_CMD_SP_ID3_VALUE6);
	m_aCtrlType3.Add(IDC_CMD_SP_ID3_UNIT6);
	m_aCtrlType3.Add(IDC_CMD_SP_ID3_TITLE7);
	m_aCtrlType3.Add(IDC_CMD_SP_ID3_VALUE7);
	m_aCtrlType3.Add(IDC_CMD_SP_ID3_UNIT7);
	m_aCtrlType3.Add(IDC_CMD_SP_ID3_TITLE8);
	m_aCtrlType3.Add(IDC_CMD_SP_ID3_VALUE8);
	m_aCtrlType3.Add(IDC_CMD_SP_ID3_UNIT8);
	m_aCtrlType3.Add(IDC_CMD_SP_ID3_TITLE9);
	m_aCtrlType3.Add(IDC_CMD_SP_ID3_VALUE9);
	m_aCtrlType3.Add(IDC_CMD_SP_ID3_UNIT9);
	m_aCtrlType3.Add(IDC_CMD_SP_ID3_TITLE10);
	m_aCtrlType3.Add(IDC_CMD_SP_ID3_VALUE10);
	m_aCtrlType3.Add(IDC_CMD_SP_ID3_UNIT10);
	m_aCtrlType3.Add(IDC_CMD_SP_ID3_TITLE11);
	m_aCtrlType3.Add(IDC_CMD_SP_ID3_VALUE11);
	m_aCtrlType3.Add(IDC_CMD_SP_ID3_UNIT11);
	m_aCtrlType3.Add(IDC_CMD_SP_ID3_TITLE12);
	m_aCtrlType3.Add(IDC_CMD_SP_ID3_VALUE12);
	m_aCtrlType3.Add(IDC_CMD_SP_ID3_UNIT12);

	m_aCtrlType4.Add(IDC_CMD_SP_ID_FRAME_STEEL4);
	m_aCtrlType4.Add(IDC_CMD_SP_ID_INFUSION_CHK);
	m_aCtrlType4.Add(IDC_CMD_SP_ID4_TITLE1);
	m_aCtrlType4.Add(IDC_CMD_SP_ID4_VALUE1);
	m_aCtrlType4.Add(IDC_CMD_SP_ID4_UNIT1);
	m_aCtrlType4.Add(IDC_CMD_SP_ID4_TITLE2);
	m_aCtrlType4.Add(IDC_CMD_SP_ID4_VALUE2);
	m_aCtrlType4.Add(IDC_CMD_SP_ID4_UNIT2);
	m_aCtrlType4.Add(IDC_CMD_SP_ID4_TITLE3);
	m_aCtrlType4.Add(IDC_CMD_SP_ID4_VALUE3);
	m_aCtrlType4.Add(IDC_CMD_SP_ID4_UNIT3);
	m_aCtrlType4.Add(IDC_CMD_SP_ID4_TITLE4);
	m_aCtrlType4.Add(IDC_CMD_SP_ID4_VALUE4);
	m_aCtrlType4.Add(IDC_CMD_SP_ID4_UNIT4);
	m_aCtrlType4.Add(IDC_CMD_SP_ID4_TITLE5);
	m_aCtrlType4.Add(IDC_CMD_SP_ID4_VALUE5);
	m_aCtrlType4.Add(IDC_CMD_SP_ID4_UNIT5);
	m_aCtrlType4.Add(IDC_CMD_SP_ID4_TITLE6);
	m_aCtrlType4.Add(IDC_CMD_SP_ID4_VALUE6);
	m_aCtrlType4.Add(IDC_CMD_SP_ID4_UNIT6);
	m_aCtrlType4.Add(IDC_CMD_SP_ID4_TITLE7);
	m_aCtrlType4.Add(IDC_CMD_SP_ID4_VALUE7);
	m_aCtrlType4.Add(IDC_CMD_SP_ID4_UNIT7);

}

CCMSectItemSrc::~CCMSectItemSrc()
{
	if(m_pBitmap != NULL){ delete m_pBitmap; m_pBitmap = NULL; } 
	if(m_pImageList != NULL){ delete m_pImageList; m_pImageList = NULL; }
}

void CCMSectItemSrc::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectItemSrc)
	DDX_Control(pDX, IDC_CMD_SP_ID3_VALUE2, m_editN2);
	DDX_Control(pDX, IDC_CMD_SP_ID3_VALUE1, m_editN1);
	DDX_Control(pDX, IDC_CMD_OFFSET_TEXT, m_txtOffset);
	DDX_Control(pDX, IDC_CMD_SP_ID_MSTIFF, m_wndMStiff);	
	DDX_Control(pDX, IDC_CMD_SP_ID2_DBNAME2, m_wndSteel2Name2);
	DDX_Control(pDX, IDC_CMD_SP_ID2_DBNAME, m_wndSteel2Name1);
	DDX_Control(pDX, IDC_CMD_SP_ID_BUILTUP, m_wndBuiltUp);
	DDX_Control(pDX, IDC_CMD_SP_ID_MVALUE4, m_wndPc);
	DDX_Control(pDX, IDC_CMD_SP_ID_MVALUE3, m_wndPs);
	DDX_Control(pDX, IDC_CMD_SP_ID_MVALUE2, m_wndDsc);
	DDX_Control(pDX, IDC_CMD_SP_ID_MVALUE1, m_wndEsc);
	DDX_Control(pDX, IDC_CMD_SP_ID_DBNAME, m_wndSteelName);
	DDX_Control(pDX, IDC_CMD_SP_ID_CODE, m_wndDB);
	DDX_Control(pDX, IDC_CMD_SP_ID_TYPE, m_cboType);
	DDX_Control(pDX, IDC_CMD_SP_ID_NAME, m_wndName);
	DDX_Control(pDX, IDC_CMD_SP_ID_ID, m_wndID);
	DDX_Control(pDX, IDC_CMD_SHEAR_DEFORM, m_chkShearDeform);
	DDX_Control(pDX, IDC_CMD_WARPING_EFFECT_CHK, m_chkWarpingEffect);
	DDX_Control(pDX, IDC_CMD_SP_ID_INFUSION_CHK, m_chkInfusionConc);
	DDX_Control(pDX, IDC_CMD_SP_ID_BMP, m_ImgSectItem);

	//}}AFX_DATA_MAP

	UINT aConcrID[][2] = {
		{ IDC_CMD_SP_ID_CTITLE1, IDC_CMD_SP_ID_CTITLE2, },
		{ IDC_CMD_SP_ID_CVALUE1, IDC_CMD_SP_ID_CVALUE2, },
		{ IDC_CMD_SP_ID_CUNIT1,  IDC_CMD_SP_ID_CUNIT2, },
	};
	UINT aSteelID[][6] = {
		{ IDC_CMD_SP_ID_TITLE1, IDC_CMD_SP_ID_TITLE2, IDC_CMD_SP_ID_TITLE3, 
			IDC_CMD_SP_ID_TITLE4, IDC_CMD_SP_ID_TITLE5, IDC_CMD_SP_ID_TITLE6 },
		{ IDC_CMD_SP_ID_VALUE1, IDC_CMD_SP_ID_VALUE2, IDC_CMD_SP_ID_VALUE3,
			IDC_CMD_SP_ID_VALUE4, IDC_CMD_SP_ID_VALUE5, IDC_CMD_SP_ID_VALUE6 },
		{ IDC_CMD_SP_ID_UNIT1,  IDC_CMD_SP_ID_UNIT2,  IDC_CMD_SP_ID_UNIT3,
			IDC_CMD_SP_ID_UNIT4,  IDC_CMD_SP_ID_UNIT5,  IDC_CMD_SP_ID_UNIT6 },
	};
	UINT aSteelID2[][8] = {
		{ IDC_CMD_SP_ID2_TITLE1, IDC_CMD_SP_ID2_TITLE2, IDC_CMD_SP_ID2_TITLE3, 
			IDC_CMD_SP_ID2_TITLE4, IDC_CMD_SP_ID2_TITLE5, IDC_CMD_SP_ID2_TITLE6,
			IDC_CMD_SP_ID2_TITLE7, IDC_CMD_SP_ID2_TITLE8 },
		{ IDC_CMD_SP_ID2_VALUE1, IDC_CMD_SP_ID2_VALUE2, IDC_CMD_SP_ID2_VALUE3,
			IDC_CMD_SP_ID2_VALUE4, IDC_CMD_SP_ID2_VALUE5, IDC_CMD_SP_ID2_VALUE6,
			IDC_CMD_SP_ID2_VALUE7, IDC_CMD_SP_ID2_VALUE8 },
	};
	UINT aSteelID3[][10] = {
		{ IDC_CMD_SP_ID3_TITLE3, IDC_CMD_SP_ID3_TITLE4, IDC_CMD_SP_ID3_TITLE5,
			IDC_CMD_SP_ID3_TITLE6, IDC_CMD_SP_ID3_TITLE7, IDC_CMD_SP_ID3_TITLE8,
			IDC_CMD_SP_ID3_TITLE9, IDC_CMD_SP_ID3_TITLE10, IDC_CMD_SP_ID3_TITLE11,
			IDC_CMD_SP_ID3_TITLE12 },
		{ IDC_CMD_SP_ID3_VALUE3, IDC_CMD_SP_ID3_VALUE4, IDC_CMD_SP_ID3_VALUE5,
			IDC_CMD_SP_ID3_VALUE6, IDC_CMD_SP_ID3_VALUE7, IDC_CMD_SP_ID3_VALUE8,
			IDC_CMD_SP_ID3_VALUE9, IDC_CMD_SP_ID3_VALUE10, IDC_CMD_SP_ID3_VALUE11,
			IDC_CMD_SP_ID3_VALUE12 },
		{ IDC_CMD_SP_ID3_UNIT3, IDC_CMD_SP_ID3_UNIT4, IDC_CMD_SP_ID3_UNIT5,
			IDC_CMD_SP_ID3_UNIT6, IDC_CMD_SP_ID3_UNIT7, IDC_CMD_SP_ID3_UNIT8,
			IDC_CMD_SP_ID3_UNIT9, IDC_CMD_SP_ID3_UNIT10, IDC_CMD_SP_ID3_UNIT11,
			IDC_CMD_SP_ID3_UNIT12 },
	};

	UINT aSteelID4[][7] = {
		{ IDC_CMD_SP_ID4_TITLE1, IDC_CMD_SP_ID4_TITLE2, IDC_CMD_SP_ID4_TITLE3,
		  IDC_CMD_SP_ID4_TITLE4, IDC_CMD_SP_ID4_TITLE5, IDC_CMD_SP_ID4_TITLE6,
		  IDC_CMD_SP_ID4_TITLE7 },
		{ IDC_CMD_SP_ID4_VALUE1, IDC_CMD_SP_ID4_VALUE2, IDC_CMD_SP_ID4_VALUE3,
		  IDC_CMD_SP_ID4_VALUE4, IDC_CMD_SP_ID4_VALUE5, IDC_CMD_SP_ID4_VALUE6,
		  IDC_CMD_SP_ID4_VALUE7 },
		{ IDC_CMD_SP_ID4_UNIT1, IDC_CMD_SP_ID4_UNIT2, IDC_CMD_SP_ID4_UNIT3,
		  IDC_CMD_SP_ID4_UNIT4, IDC_CMD_SP_ID4_UNIT5, IDC_CMD_SP_ID4_UNIT6,
		  IDC_CMD_SP_ID4_UNIT7 },
	};

	for (int i = 0; i < 2; i++)
	{
		DDX_Control(pDX, aConcrID[0][i], m_wndConcrTitle[i]);
		DDX_Control(pDX, aConcrID[1][i], m_wndConcrSize[i]);
		DDX_Control(pDX, aConcrID[2][i], m_wndConcrUnit[i]);
	}
	for (int i = 0; i < 6; i++)
	{
		DDX_Control(pDX, aSteelID[0][i], m_wndSteelTitle[i]);
		DDX_Control(pDX, aSteelID[1][i], m_wndSteelSize[i]);
		DDX_Control(pDX, aSteelID[2][i], m_wndSteelUnit[i]);
	}
	for (int i = 0; i < 8; i++)
	{
		DDX_Control(pDX, aSteelID2[0][i], m_wndSteel2Title[i]);
		DDX_Control(pDX, aSteelID2[1][i], m_wndSteel2Size[i]);
	}
	DDX_Control(pDX, IDC_CMD_SP_ID2_UNIT, m_wndSteel2Unit);
	for (int i = 0; i < 10; i++)
	{
		DDX_Control(pDX, aSteelID3[0][i], m_wndSteel3Title[i]);
		DDX_Control(pDX, aSteelID3[1][i], m_wndSteel3Size[i]);
		DDX_Control(pDX, aSteelID3[2][i], m_wndSteel3Unit[i]);
	}

	for (int i = 0; i < 7; i++)
	{
		DDX_Control(pDX, aSteelID4[0][i], m_wndSteel4Title[i]);
		DDX_Control(pDX, aSteelID4[1][i], m_wndSteel4Size[i]);
		DDX_Control(pDX, aSteelID4[2][i], m_wndSteel4Unit[i]);
	}
}

/////////////////////////////////////////////////////////////////////////////
// Interfaces
/////////////////////////////////////////////////////////////////////////////

// 부모 Page에서 OK 버튼 눌러졌을 때 호출
BOOL CCMSectItemSrc::OnOKPublic()
{
	return SaveData();
}

// 부모 페이지에서 Apply 버튼 눌러졌을 때 호출
BOOL CCMSectItemSrc::OnApplyPublic()
{
	if (!SaveData()) return FALSE;
	if (!m_bModify)  // modify 면 그냥 두고 add면 재 초기화 
	{
		// ID만 수정한다.
		m_Key = m_pDoc->m_pAttrCtrl->GetNextNumSect(m_Key);
		CString strID;
		strID.Format(_T("%d"), m_Key);
		m_wndID.SetWindowText(strID);
		//m_Data.SName = _T("");
		//m_wndName.SetWindowText(m_Data.SName);
		//m_wndName.SetFocus();
		m_wndConcrSize[0].SetFocus();
	}
	else
	{
		m_OrgKey = m_Key;  // Change Origin Key
	}
	return TRUE;
}

// 강성 계산 결과를 보여준다.
void CCMSectItemSrc::ShowCalcResult()
{
	if (!Dlg2Data() || !GetPeriAndStiff())
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Can_t_calculate_stiffness));
		return;
	}
	CCMStiffDlg dlg(m_pDoc, &m_Data, NULL, this);
	dlg.DoModal();
}

void CCMSectItemSrc::DisplayOffsetPoint()
{
	m_wndSecView.Invalidate();
}

void CCMSectItemSrc::SetNewID(unsigned nNewID)
{
	m_Key = nNewID;
	ShowKey2Dlg();
}

/////////////////////////////////////////////////////////////////////////////
// Implementations
/////////////////////////////////////////////////////////////////////////////
BOOL CCMSectItemSrc::SaveData()
{
	if (!Dlg2Data()) return FALSE;
	if (m_Data.SName == _T(""))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_section_name_));
		return FALSE;
	}

	if (!GetPeriAndStiff()) return FALSE;

	if (m_bModify)
	{
		if (m_bDesign) return m_pDoc->m_pDataCtrl->AddSecd(m_Key, m_Data, m_bVirtual);
		else return m_pDoc->m_pDataCtrl->ModifySect(m_OrgKey, m_Key, m_Data);
	}
	else return m_pDoc->m_pDataCtrl->AddSect(m_Key, m_Data);
}

BOOL CCMSectItemSrc::Dlg2Data()
{
	// offset data backup
	int nOffsetPoint = m_Data.SectBefore.nOffsetPoint;
	int nOffsetCenter = m_Data.SectBefore.nOffsetCenter;
	int nUserOffsetRef = m_Data.SectBefore.nUserOffsetRef;
	int nHorzOffsetOpt = m_Data.SectBefore.nHorzOffsetOpt;
	int nVertOffsetOpt = m_Data.SectBefore.nVertOffsetOpt;
	double dUserDefOffsetY[2], dUserDefOffsetZ[2];
	for (int i=0; i<2; i++)
	{
		dUserDefOffsetY[i] = m_Data.SectBefore.dUserDefOffsetY[i];
		dUserDefOffsetZ[i] = m_Data.SectBefore.dUserDefOffsetZ[i];
	}

	// save data
	m_Data.Initialize();
	m_Data.nStype = D_SECT_TYPE_SRC;

	// ID
	CString csID;
	m_wndID.GetWindowText(csID);
	m_Key = (T_SECT_K)_tstol(csID);

	// Name
	m_wndName.GetWindowText(m_Data.SName);
	m_Data.SName.TrimLeft(); m_Data.SName.TrimRight();

	// Shape
	int nShapeIndex = m_cboType.GetCurSel();
	if (nShapeIndex == LB_ERR) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Shape_is_not_selected_));
		return FALSE;
	}
	m_Data.SectBefore.Shape = CSectUtil::GetShapeNameFromIndexSrc(nShapeIndex);

	// nSRCtype : replace steel(0, 1), concrete(2)
	int nCheck;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlReplace, nCheck);
	m_Data.SectBefore.nSRCtype = nCheck+1;
	// Matl-Elast
	m_Data.SectBefore.Matl_Elast = m_wndEsc.GetEditValue();
	// Matl-Density
	m_Data.SectBefore.Matl_Density = m_wndDsc.GetEditValue();
	// Matl-Poisson-S
	m_Data.SectBefore.Matl_Poisson_S = m_wndPs.GetEditValue();
	// Matl-Poisson-C
	m_Data.SectBefore.Matl_Poisson_C = m_wndPc.GetEditValue();
	// Matl-Stiffness-Factor
	m_Data.SectBefore.Matl_StiffFactor = m_wndMStiff.GetEditValue();
	// Built-Up : Always 1(built-up)
	m_Data.SectBefore.BuiltUpFlag = 1;
	if (m_wndBuiltUp.GetCheck()) m_Data.SectBefore.SectI.BuiltUpFlag = 1;
	else m_Data.SectBefore.SectI.BuiltUpFlag = 2;

	// Offset
	m_Data.SectBefore.nOffsetPoint = nOffsetPoint;
	m_Data.SectBefore.nOffsetCenter = nOffsetCenter;
	m_Data.SectBefore.nUserOffsetRef = nUserOffsetRef;
	m_Data.SectBefore.nHorzOffsetOpt = nHorzOffsetOpt;
	m_Data.SectBefore.nVertOffsetOpt = nVertOffsetOpt;
	for (int i=0; i<2; i++)
	{
		m_Data.SectBefore.dUserDefOffsetY[i] = dUserDefOffsetY[i];
		m_Data.SectBefore.dUserDefOffsetZ[i] = dUserDefOffsetZ[i];
	}

	// Steel
	T_SECT_SECTBASE_D* pSteel = &(m_Data.SectBefore.SectI);
	{
		// DB
		pSteel->DBName = GetDB();
		// Shape
		pSteel->Shape = CSectUtil::GetSteelShapeFromSRCShape(m_Data.SectBefore.Shape);
		
		if (m_nCurShapeType == 1 || m_nCurShapeType == 5)
		{
			// SName
			if (pSteel->DBName == _T("")) pSteel->SName = _T("");
			else m_wndSteelName.GetWindowText(pSteel->SName);

#if defined(_RUS)
			CSectDBRussiaUtil::ConvertSectDBNameRusToEng(pSteel->DBName, pSteel->SName);
#endif

			// Size
			int nSizeNum = CSectUtil::GetSizeNumFromShapeIndexSrcSteel(nShapeIndex);
			for (int i = 0; i < nSizeNum; i++)
				pSteel->Size[i] = m_wndSteelSize[i].GetEditValue();
		}
		else if (m_nCurShapeType == 2)
		{
			// SName
			if (pSteel->DBName == _T("")) 
			{
				pSteel->SName = _T("");
				pSteel->SName2 = _T("");
			}
			else
			{
				m_wndSteel2Name1.GetWindowText(pSteel->SName);
				m_wndSteel2Name2.GetWindowText(pSteel->SName2);

#if defined(_RUS)
				CSectDBRussiaUtil::ConvertSectDBNameRusToEng(pSteel->DBName, pSteel->SName);
				CSectDBRussiaUtil::ConvertSectDBNameRusToEng(pSteel->DBName, pSteel->SName2);
#endif
			}
			// Size
			int nSizeNum = CSectUtil::GetSizeNumFromShapeIndexSrcSteel(nShapeIndex);
			for (int i = 0; i < nSizeNum; i++)
				pSteel->Size[i] = m_wndSteel2Size[i].GetEditValue();
			pSteel->BuiltUpFlag = 1;    // Always Built-up
		}
		else if (m_nCurShapeType == 3)
		{
			// size
			pSteel->DBName = _T("");

			m_Data.SectBefore.nCellShape = m_editN1.GetEditValueInt();
			m_Data.SectBefore.nCellType = m_editN2.GetEditValueInt();
			int nSizeNum = CSectUtil::GetSizeNumFromShapeIndexSrcSteel(nShapeIndex);
			for (int i = 0; i < nSizeNum; i++)
				pSteel->Size[i] = m_wndSteel3Size[i].GetEditValue();
		}
		else if (m_nCurShapeType == 4)
		{
			// SName
			pSteel->DBName = _T("");

			// Size
			int nSizeNum = CSectUtil::GetSizeNumFromShapeIndexSrcSteel(nShapeIndex);
			for (int i = 0; i < nSizeNum; i++)
				pSteel->Size[i] = m_wndSteel4Size[i].GetEditValue();

			pSteel->BuiltUpFlag = 1;    // Always Built-up
		}
		else ASSERT(0);
	}

	// Concrete (Shape과 Size만)
	T_SECT_SECTBASE_D* pConcr = &(m_Data.SectBefore.SectJ);
	{
		// Shape
		pConcr->Shape = CSectUtil::GetConcreteShapeFromSRCShape(m_Data.SectBefore.Shape);
		// Size
		int nSizeNum = CSectUtil::GetSizeNumFromShapeIndexSrcConcr(nShapeIndex);
		for (int i = 0; i < nSizeNum; i++)
			pConcr->Size[i] = m_wndConcrSize[i].GetEditValue();
	}

	// consider shear deformation
	m_Data.SectBefore.bConsiderShearDeform = m_chkShearDeform.GetCheck();
	m_Data.SectBefore.bConsiderWarpingEffect = m_chkWarpingEffect.GetCheck();
	m_Data.bInfusionChk = m_chkInfusionConc.GetCheck();  // 덤벨 확인! (변수)

	return TRUE;
}

BOOL CCMSectItemSrc::GetPeriAndStiff()
{
	return CSectUtil::CalcSectData(m_Data);
}

BOOL CCMSectItemSrc::SetErr(LPCTSTR msg)
{
	AfxMessageBox(msg);
	return FALSE;
}

void CCMSectItemSrc::AlignControls()
{
	CWnd* pWnd;
	CRect ToMove, ToRef;

	pWnd = GetDlgItem(IDC_CMD_SP_ID_FRAME_STEEL);
	if (!pWnd) { ASSERT(0); return; }
	pWnd->GetWindowRect(&ToRef);

	pWnd = GetDlgItem(IDC_CMD_SP_ID_FRAME_STEEL2);
	if (!pWnd) { ASSERT(0); return; }
	pWnd->GetWindowRect(&ToMove);
	int nDistX = ToMove.left - ToRef.left;
	CDlgUtil::CtrlMoveDistX(this, m_aCtrlType2, -nDistX);

	pWnd = GetDlgItem(IDC_CMD_SP_ID_FRAME_STEEL3);
	if (!pWnd) { ASSERT(0); return; }
	pWnd->GetWindowRect(&ToMove);
	nDistX = ToMove.left - ToRef.left;
	CDlgUtil::CtrlMoveDistX(this, m_aCtrlType3, -nDistX);

	pWnd = GetDlgItem(IDC_CMD_SP_ID_FRAME_STEEL4);
	if (!pWnd) { ASSERT(0); return; }
	pWnd->GetWindowRect(&ToMove);
	nDistX     = ToRef.left - ToMove.left;
	int nDistY = ToRef.top  - ToMove.top;
	CDlgUtil::CtrlMoveDistXY(this, m_aCtrlType4, nDistX, nDistY);

	CDlgUtil::CtrlShowHide(this, m_aCtrlType0, m_nCurShapeType != 3 && m_nCurShapeType != 4);
	CDlgUtil::CtrlShowHide(this, m_aCtrlType1, m_nCurShapeType == 1 || m_nCurShapeType == 5);
	CDlgUtil::CtrlShowHide(this, m_aCtrlType2, m_nCurShapeType == 2);
	CDlgUtil::CtrlShowHide(this, m_aCtrlType3, m_nCurShapeType == 3);
	CDlgUtil::CtrlShowHide(this, m_aCtrlType4, m_nCurShapeType == 4);

	/*
	// resize dialog size
	CRect r;
	GetWindowRect(&r);
	pWnd = GetDlgItem(IDC_CMD_SP_ID_FRAME_STEEL);
	pWnd->GetClientRect(ToMove);
	r.right = ToMove.right + 3;
	MoveWindow(&r);
	*/
}

void CCMSectItemSrc::InitControls()
{
	// Init shape combo
	SetSectionShapeCombo();
	m_cboType.SetCurSel(0);

	// Init DB name combo
	SetDBNameList();
	m_wndDB.SetCurSel(0);

	// Init steel name combo
	m_wndSteelName.SetActionWhenKillFocus(2, -1);

	// preview control
	CWnd* pWnd = GetDlgItem(IDC_CMD_SP_ID_PREVIEW);
	ASSERT(pWnd);
	m_wndSecView.Init(pWnd);

	// Unit control
	int i;
	for (i = 0; i < 6; i++)
	{
		m_wndSteelSize[i].SetUnitType(D_UNITSYS_BASE_LENGTH);
		m_wndSteelUnit[i].SetUnitType(D_UNITSYS_BASE_LENGTH);
	}
	for (i = 0; i < 8; i++)
	{
		m_wndSteel2Size[i].SetUnitType(D_UNITSYS_BASE_LENGTH);
	}
	m_wndSteel2Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	for (i = 0; i < 10; i++)
	{
		m_wndSteel3Size[i].SetUnitType(D_UNITSYS_BASE_LENGTH);
		m_wndSteel3Unit[i].SetUnitType(D_UNITSYS_BASE_LENGTH);
	}
	for (i = 0; i < 7; i++)
	{
		m_wndSteel4Size[i].SetUnitType(D_UNITSYS_BASE_LENGTH);
		m_wndSteel4Unit[i].SetUnitType(D_UNITSYS_BASE_LENGTH);
	}
	for (i = 0; i < 2; i++)
	{
		m_wndConcrSize[i].SetUnitType(D_UNITSYS_BASE_LENGTH);
		m_wndConcrUnit[i].SetUnitType(D_UNITSYS_BASE_LENGTH);
	}
	m_wndEsc.SetUnitType(CUnitCtrl::m_SECT_UNIT.SectBefore.Matl_Elast);
	m_wndDsc.SetUnitType(CUnitCtrl::m_SECT_UNIT.SectBefore.Matl_Density);
	m_wndPs.SetUnitType(CUnitCtrl::m_SECT_UNIT.SectBefore.Matl_Poisson_S);
	m_wndPc.SetUnitType(CUnitCtrl::m_SECT_UNIT.SectBefore.Matl_Poisson_C);    
	m_wndMStiff.SetUnitType(0);  

	m_wndBuiltUp.EnableWindow(FALSE);
	m_editN2.SetRange(1, 100);
	m_editN2.SetInteger(TRUE);
	m_editN1.SetRange(1, 100);
	m_editN1.SetInteger(TRUE);
}

void CCMSectItemSrc::SetSectionShapeCombo()
{
	/*
	//if (CLocaleLib::IsEnglish()) // 영어권만(일본은 H로 씀)
	#if defined(_US)
	{
		aItemName[10] = _LS(IDS_WG_CMD__ADDD__Rect_IBeam);
		aItemName[11] = _LS(IDS_WG_CMD__ADDD__Circle_IBeam);
	}
	#endif
	*/

	// Attach image list to ComboBox
	m_cboType.SetImageList(SectItemSrcResource::aImgPath(m_bDumb));
	m_cboType.SetComboHeight(globalUtils.ScaleByDPI(24));
	m_cboType.SetRedraw(FALSE);
	// Make items
	CString str;
	int nMovingType = CProduct::GetMovingType();
	std::vector<CString> itemNames = SectItemSrcResource::aItemName(m_bDumb);
	int nCount = (int)itemNames.size();

	for (int i = 0; i < nCount; i++)
	{
		CString str = itemNames[i];
		if (nMovingType == D_PRODUCT_MOVING_US || nMovingType == D_PRODUCT_MOVING_RUS) 
		{
			str.Replace('H', 'I');
		}
		m_cboType.AddString(str);
	}
	m_cboType.SetRedraw(TRUE);
}

void CCMSectItemSrc::SetDBNameList()
{
	CArray<CString, CString&> DBNameList;
	m_pDoc->m_pSectDB->GetDBNameList(DBNameList);
	m_wndDB.SetRedraw(FALSE);
	for (int i = 0; i < DBNameList.GetSize(); i++)
	{
#if defined(_RUS)
		CSectDBRussiaUtil::ConvertSectDBEngToRus(DBNameList[i]);
#endif
		m_wndDB.AddString(DBNameList.GetAt(i));
	}
	m_wndDB.SetRedraw(TRUE);
}

void CCMSectItemSrc::ShowKey2Dlg()
{
	if(m_wndID.GetSafeHwnd() == nullptr) return;

	CString csKey;
	csKey.Format(_T("%ld"), m_Key);
	m_wndID.SetWindowText(csKey);
}

// 여기서 Type에 따라 다르게 보여주어야 할 듯...
void CCMSectItemSrc::ShowData2Dlg()
{
	// Type 변화 체크 - Control 배치
	int nShapeType = GetShapeType(m_Data.SectBefore.Shape);
	if (m_nCurShapeType != nShapeType)
		ChangeShapeType(nShapeType);

	// shape
	int nShapeIndex = CSectUtil::GetShapeIndexFromNameSrc(m_Data.SectBefore.Shape);
	m_cboType.SetCurSel(nShapeIndex);
	// db
	SetDB(m_Data.SectBefore.SectI.DBName);
	// name(db보다 나중에 설정 : For AutoChangeName)
	m_wndName.SetWindowText(m_Data.SName);
	// nSRCtype
	int nCheck;
	nCheck = 0;
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlReplace, nCheck); 
/*  int nCheck;
	if (m_Data.SectBefore.nSRCtype == 2) nCheck = 1;
	else nCheck = 0;
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlReplace, nCheck);  */ // Concrete Radio Button 죽임.
																																 // 2000.7.25 BugBoy	
	// Matl
	SetMatlSectData();
	// Built-Up
	int nBuiltUp;
	if (m_Data.SectBefore.SectI.BuiltUpFlag == 2) nBuiltUp = 0;
	else nBuiltUp = 1;
	m_wndBuiltUp.SetCheck(nBuiltUp);

	CDlgUtil::CtrlShowHideByRect(this, IDC_CMD_SP_ID_FRAME_MATL, TRUE, TRUE);

	// Steel/Concrete - depend on type
	if (m_nCurShapeType == 1)
	{
		SetSteelNameCombo();
		SetSteelSectData();   // Title, Value 설정, Show/Hide 수행
	}
	else if (m_nCurShapeType == 5)
	{
		SetSteelNameCombo();
		SetSteelSectData();   // Title, Value 설정, Show/Hide 수행

		CDlgUtil::CtrlShowHideByRect(this, IDC_CMD_SP_ID_FRAME_MATL, FALSE, TRUE); // 덤벨 확인!
	}
	else if (m_nCurShapeType == 2)
	{
		SetSteelFirstNameCombo();
		SetSteelSecondNameCombo();
		SetSteelFirstSectData();
		SetSteelSecondSectData();
	}
	else if (m_nCurShapeType == 3)
	{
		m_editN1.SetValue(m_Data.SectBefore.nCellShape);

		BOOL bUseN2 = (m_Data.SectBefore.Shape == D_SECT_SHAPE_SRC_BSTF);
		GetDlgItem(IDC_CMD_SP_ID3_TITLE2)->ShowWindow(bUseN2);
		GetDlgItem(IDC_CMD_SP_ID3_VALUE2)->ShowWindow(bUseN2);
// 		GetDlgItem(IDC_CMD_SP_ID3_SPIN2)->ShowWindow(bUseN2);
		if (bUseN2)
		{
			m_editN2.SetValue(m_Data.SectBefore.nCellType);
		}
		
		SetSteel3SectData();
	}
	else if (m_nCurShapeType == 4)
	{
		CDlgUtil::CtrlShowHideByRect(this, IDC_CMD_SP_ID_FRAME_MATL, FALSE, TRUE); // 덤벨 확인!

		SetSteel4SectData();
	}
	else ASSERT(0);
	SetConcrSectData();  // Title, Value 설정, Show/Hide 수행

	// offset
	CStringArray arOffsetNameList;
	CSectUtil::GetOffsetNameList(arOffsetNameList);
	if (m_Data.SectBefore.nOffsetPoint >= 1)
	{
		CString csOffset = arOffsetNameList[m_Data.SectBefore.nOffsetPoint-1];
		m_txtOffset.SetWindowText(csOffset);
	}

	// consider shear deformation
	m_chkShearDeform.SetCheck((int)m_Data.SectBefore.bConsiderShearDeform);
	m_chkWarpingEffect.SetCheck((int)m_Data.SectBefore.bConsiderWarpingEffect);
	m_chkInfusionConc.SetCheck((int)m_Data.bInfusionChk);

	if (CSectDB::IsSectDumbAndStub(m_Data)) // Dumbbell
	{
		m_chkWarpingEffect.ShowWindow(SW_HIDE);
	}
	else if(!CSectDB::IsEnableSectShape7thDOF(m_Data.SectBefore.Shape))
	{
		m_Data.SectBefore.bConsiderWarpingEffect = FALSE;
		m_chkWarpingEffect.ShowWindow(SW_HIDE);
	}
	else
	{
		BOOL bHide_7thDOF = FALSE;
		if (!CSectDB::IsEnable7thDOF())
			bHide_7thDOF = TRUE;
		
		m_chkWarpingEffect.ShowWindow(!bHide_7thDOF);
	}

	if (IsEnableSectShape7thDOF_Temp(m_Data.SectBefore.Shape, m_Data.bHSQType, m_Data.bRoundType))
	{
		m_chkWarpingEffect.EnableWindow(TRUE);
	}
	else
	{
		m_chkWarpingEffect.EnableWindow(FALSE);
	}

	// guide bitmap / section preview
	ChangeBitmap(); // Change guide bitmap
	//m_wndSecView.SetCentroidFlag(FALSE);
	m_wndSecView.Invalidate();
}

// Type에 따라 달라진다.
void CCMSectItemSrc::SetSteelNameCombo()
{
	// Name List를 삭제
	m_wndSteelName.ResetContent();

	CString csDBName, csShape;
	T_SECT_SECTBASE_D* pSect = &m_Data.SectBefore.SectI;
	csDBName = pSect->DBName;
	csShape = pSect->Shape;

	// DB가 None이면 Disable하고 Return한다.
	if (csDBName == _T("")) 
	{
		m_wndSteelName.EnableWindow(FALSE);
		return;
	}
	m_wndSteelName.EnableWindow(TRUE);

	// Section name list를 가져온다.
	CArray<CString, CString&> aSectNameList;
	m_pDoc->m_pSectDB->GetSectNameList(csDBName, csShape, aSectNameList);

	// Section name list를 설정한다.
	int nSize = aSectNameList.GetSize();;
	m_wndSteelName.SetRedraw(FALSE);
	for (int i = 0; i < nSize; i++)
	{
#if defined(_RUS)
		CSectDBRussiaUtil::ConvertSectDBNameEngToRus(csDBName, aSectNameList[i]);
#endif
		m_wndSteelName.AddString(aSectNameList.GetAt(i));
	}

	CString SectName = pSect->SName;
#if defined(_RUS)
	CSectDBRussiaUtil::ConvertSectDBNameEngToRus(csDBName, SectName);
#endif

	// 이름을 콤보에서 선택한다.
	int nSel = m_wndSteelName.FindStringExact(-1, SectName);
	if (nSel == CB_ERR) m_wndSteelName.SelectString(-1, SectName);
	else m_wndSteelName.SetCurSel(nSel);
	m_wndSteelName.SetRedraw(TRUE);
}

void CCMSectItemSrc::SetSteelSectData()
{
	int nShapeIndex;
	CStringArray aSizeTitle;
	nShapeIndex = CSectUtil::GetShapeIndexFromNameSrc(m_Data.SectBefore.Shape);
	if (nShapeIndex < 0) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_SRC_section_shape));
		return;
	}
	CSectUtil::GetSizeTitleFromShapeIndexSrcSteel(nShapeIndex, aSizeTitle);

	int nUserOrDB;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlDB, nUserOrDB);
	const BOOL bUser = (nUserOrDB == 0) ? TRUE : FALSE;

	T_SECT_SECTBASE_D* pSect = &m_Data.SectBefore.SectI;
	BOOL bEnable = bUser ? TRUE : FALSE;
	CString csValue;
	int nSizeNum = aSizeTitle.GetSize();
	int i = 0;
	for (i = 0; i < nSizeNum; i++)
	{
		m_wndSteelTitle[i].ShowWindow(SW_SHOW);
		m_wndSteelSize[i].ShowWindow(SW_SHOW);
		m_wndSteelUnit[i].ShowWindow(SW_SHOW);
		m_wndSteelTitle[i].SetWindowText(aSizeTitle[i]);
		csValue.Format(_T("%g"), pSect->Size[i]);
		m_wndSteelSize[i].SetWindowText(csValue);
		m_wndSteelSize[i].EnableWindow(bEnable);
	}
	for (; i < 6; i++)
	{
		m_wndSteelTitle[i].ShowWindow(SW_HIDE);
		m_wndSteelSize[i].ShowWindow(SW_HIDE);
		m_wndSteelUnit[i].ShowWindow(SW_HIDE);
	}
	int nBuiltUp;
	if (pSect->BuiltUpFlag == 2) nBuiltUp = 0;
	else nBuiltUp = 1;
	m_wndBuiltUp.SetCheck(nBuiltUp);
}

void CCMSectItemSrc::SetSteelFirstNameCombo()
{
	// Name List를 삭제
	m_wndSteel2Name1.ResetContent();

	CString csDBName, csShape;
	T_SECT_SECTBASE_D* pSect = &m_Data.SectBefore.SectI;
	csDBName = pSect->DBName;
	csShape = CSectUtil::GetFirstShapeFromComShape(pSect->Shape, TRUE);

	// DB가 None이면 Disable하고 Return한다.
	if (csDBName == _T("")) 
	{
		m_wndSteel2Name1.EnableWindow(FALSE);
		return;
	}
	m_wndSteel2Name1.EnableWindow(TRUE);

	// Section name list를 가져온다.
	CArray<CString, CString&> aSectNameList;
	m_pDoc->m_pSectDB->GetSectNameList(csDBName, csShape, aSectNameList);

	// Section name list를 설정한다.
	int nSize = aSectNameList.GetSize();;
	m_wndSteel2Name2.SetRedraw(FALSE);
	for (int i = 0; i < nSize; i++)
	{
#if defined(_RUS)
		CSectDBRussiaUtil::ConvertSectDBNameEngToRus(csDBName, aSectNameList[i]);
#endif
		m_wndSteel2Name1.AddString(aSectNameList.GetAt(i));
	}

	CString SectName = pSect->SName;
#if defined(_RUS)
	CSectDBRussiaUtil::ConvertSectDBNameEngToRus(csDBName, SectName);
#endif

	// 이름을 콤보에서 선택한다.
	int nSel = m_wndSteel2Name1.FindStringExact(-1, SectName);
	if (nSel == CB_ERR) m_wndSteel2Name1.SelectString(-1, SectName);
	else m_wndSteel2Name1.SetCurSel(nSel);
	m_wndSteel2Name2.SetRedraw(TRUE);
}

void CCMSectItemSrc::SetSteelSecondNameCombo()
{
	// Name List를 삭제
	m_wndSteel2Name2.ResetContent();

	CString csDBName, csShape;
	T_SECT_SECTBASE_D* pSect = &m_Data.SectBefore.SectI;
	csDBName = pSect->DBName;
	csShape = CSectUtil::GetSecondShapeFromComShape(pSect->Shape, TRUE);

	// DB가 None이면 Disable하고 Return한다.
	if (csDBName == _T("")) 
	{
		m_wndSteel2Name2.EnableWindow(FALSE);
		return;
	}
	m_wndSteel2Name2.EnableWindow(TRUE);

	// Section name list를 가져온다.
	CArray<CString, CString&> aSectNameList;
	m_pDoc->m_pSectDB->GetSectNameList(csDBName, csShape, aSectNameList);

	// Section name list를 설정한다.
	int nSize = aSectNameList.GetSize();;
	m_wndSteel2Name2.SetRedraw(FALSE);
	for (int i = 0; i < nSize; i++)
	{
#if defined(_RUS)
		CSectDBRussiaUtil::ConvertSectDBNameEngToRus(csDBName, aSectNameList[i]);
#endif
		m_wndSteel2Name2.AddString(aSectNameList.GetAt(i));
	}

	CString SectName = pSect->SName2;
#if defined(_RUS)
	CSectDBRussiaUtil::ConvertSectDBNameEngToRus(csDBName, SectName);
#endif

	// 이름을 콤보에서 선택한다.
	int nSel = m_wndSteel2Name2.FindStringExact(-1, SectName);
	if (nSel == CB_ERR) m_wndSteel2Name2.SelectString(-1, SectName);
	else m_wndSteel2Name2.SetCurSel(nSel);
	m_wndSteel2Name2.SetRedraw(TRUE);
}

void CCMSectItemSrc::SetSteelFirstSectData()
{
	int nShapeIndex;
	CStringArray aSizeTitle;
	nShapeIndex = CSectUtil::GetShapeIndexFromNameSrc(m_Data.SectBefore.Shape);
	if (nShapeIndex < 0) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_SRC_section_shape));
		return;
	}
	CSectUtil::GetSizeTitleFromShapeIndexSrcSteel(nShapeIndex, aSizeTitle);

	T_SECT_SECTBASE_D* pSect = &m_Data.SectBefore.SectI;
	BOOL bEnable = (pSect->DBName == _T("")) ? TRUE : FALSE;
	CString csValue;
	int nSizeNum = aSizeTitle.GetSize();
	for (int i = 0; i < 4; i++)
	{
		m_wndSteel2Title[i].SetWindowText(aSizeTitle[i]);
		csValue.Format(_T("%g"), pSect->Size[i]);
		m_wndSteel2Size[i].SetWindowText(csValue);
		m_wndSteel2Size[i].EnableWindow(bEnable);
	}
}

void CCMSectItemSrc::SetSteelSecondSectData()
{
	int nShapeIndex;
	CStringArray aSizeTitle;
	nShapeIndex = CSectUtil::GetShapeIndexFromNameSrc(m_Data.SectBefore.Shape);
	if (nShapeIndex < 0) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_SRC_section_shape));
		return;
	}
	CSectUtil::GetSizeTitleFromShapeIndexSrcSteel(nShapeIndex, aSizeTitle);

	T_SECT_SECTBASE_D* pSect = &m_Data.SectBefore.SectI;
	BOOL bEnable = (pSect->DBName == _T("")) ? TRUE : FALSE;
	CString csValue;
	int nSizeNum = aSizeTitle.GetSize();
	for (int i = 0; i < 4; i++)
	{
		m_wndSteel2Title[i+4].SetWindowText(aSizeTitle[i+4]);
		csValue.Format(_T("%g"), pSect->Size[i+4]);
		m_wndSteel2Size[i+4].SetWindowText(csValue);
		m_wndSteel2Size[i+4].EnableWindow(bEnable);
	}
}

void CCMSectItemSrc::SetSteel3SectData()
{
	int nShapeIndex;
	CStringArray aSizeTitle;
	nShapeIndex = CSectUtil::GetShapeIndexFromNameSrc(m_Data.SectBefore.Shape);
	if (nShapeIndex < 0) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_SRC_section_shape));
		return;
	}
	CSectUtil::GetSizeTitleFromShapeIndexSrcSteel(nShapeIndex, aSizeTitle);

	T_SECT_SECTBASE_D* pSect = &m_Data.SectBefore.SectI;
	BOOL bEnable = TRUE;
	CString csValue;
	int nSizeNum = aSizeTitle.GetSize();
	int i = 0;
	for (i = 0; i < nSizeNum; i++)
	{
		m_wndSteel3Title[i].ShowWindow(SW_SHOW);
		m_wndSteel3Size[i].ShowWindow(SW_SHOW);
		m_wndSteel3Unit[i].ShowWindow(SW_SHOW);
		m_wndSteel3Title[i].SetWindowText(aSizeTitle[i]);
		csValue.Format(_T("%g"), pSect->Size[i]);
		m_wndSteel3Size[i].SetWindowText(csValue);
		m_wndSteel3Size[i].EnableWindow(bEnable);
	}
	for (; i < 10; i++)
	{
		m_wndSteel3Title[i].ShowWindow(SW_HIDE);
		m_wndSteel3Size[i].ShowWindow(SW_HIDE);
		m_wndSteel3Unit[i].ShowWindow(SW_HIDE);
	}
	int nBuiltUp;
	if (pSect->BuiltUpFlag == 2) nBuiltUp = 0;
	else nBuiltUp = 1;
	m_wndBuiltUp.SetCheck(nBuiltUp);
}

void CCMSectItemSrc::SetSteel4SectData()
{
	int nShapeIndex;
	CStringArray aSizeTitle;
	nShapeIndex = CSectUtil::GetShapeIndexFromNameSrc(m_Data.SectBefore.Shape);
	if (nShapeIndex < 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_SRC_section_shape));
		return;
	}
	CSectUtil::GetSizeTitleFromShapeIndexSrcSteel(nShapeIndex, aSizeTitle);

	int nUserOrDB;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlDB, nUserOrDB);
	const BOOL bUser = (nUserOrDB == 0) ? TRUE : FALSE;

	T_SECT_SECTBASE_D* pSect = &m_Data.SectBefore.SectI;
	BOOL bEnable = TRUE;
	CString csValue;
	int nSizeNum = aSizeTitle.GetSize();
	int i = 0;
	for (i = 0; i < nSizeNum; i++)
	{
		m_wndSteel4Title[i].ShowWindow(SW_SHOW);
		m_wndSteel4Size[i].ShowWindow(SW_SHOW);
		m_wndSteel4Unit[i].ShowWindow(SW_SHOW);
		m_wndSteel4Title[i].SetWindowText(aSizeTitle[i]);
		csValue.Format(_T("%g"), pSect->Size[i]);
		m_wndSteel4Size[i].SetWindowText(csValue);
		m_wndSteel4Size[i].EnableWindow(bEnable);
	}
	for (; i < 7; i++)
	{
		m_wndSteel4Title[i].ShowWindow(SW_HIDE);
		m_wndSteel4Size[i].ShowWindow(SW_HIDE);
		m_wndSteel4Unit[i].ShowWindow(SW_HIDE);
	}
	int nBuiltUp = 1;  // Always Built-up
	m_wndBuiltUp.SetCheck(nBuiltUp);
}

void CCMSectItemSrc::SetConcrSectData()
{
	int nShapeIndex;
	CStringArray aSizeTitle;
	nShapeIndex = CSectUtil::GetShapeIndexFromNameSrc(m_Data.SectBefore.Shape);
	if (nShapeIndex < 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalide_SRC_section_shap));
		return;
	}
	CSectUtil::GetSizeTitleFromShapeIndexSrcConcr(nShapeIndex, aSizeTitle);

	T_SECT_SECTBASE_D* pSect = &m_Data.SectBefore.SectJ;
	CString csValue;
	int nSizeNum = aSizeTitle.GetSize();
	int i = 0;
	for (i = 0;i < nSizeNum; i++)
	{
		m_wndConcrTitle[i].SetWindowText(aSizeTitle[i]);
		csValue.Format(_T("%g"), pSect->Size[i]);
		m_wndConcrSize[i].SetWindowText(csValue);
		m_wndConcrTitle[i].EnableWindow(TRUE);
		m_wndConcrSize[i].EnableWindow(TRUE);
		m_wndConcrUnit[i].EnableWindow(TRUE);
	}
	for (; i < 2; i++)
	{
		m_wndConcrTitle[i].EnableWindow(FALSE);
		m_wndConcrSize[i].EnableWindow(FALSE);
		m_wndConcrUnit[i].EnableWindow(FALSE);
		m_wndConcrSize[i].SetWindowText(_T(""));
	}
}

void CCMSectItemSrc::SetMatlSectData()
{
	m_wndEsc.SetEditUnit(m_Data.SectBefore.Matl_Elast);
	m_wndDsc.SetEditUnit(m_Data.SectBefore.Matl_Density);
	m_wndPs.SetEditUnit(m_Data.SectBefore.Matl_Poisson_S);
	m_wndPc.SetEditUnit(m_Data.SectBefore.Matl_Poisson_C);  
	m_wndMStiff.SetEditUnit(m_Data.SectBefore.Matl_StiffFactor);
}

void CCMSectItemSrc::ChangeBitmap()
{
	const int nShapeIndex = CSectUtil::GetShapeIndexFromNameSrc(m_Data.SectBefore.Shape);
	if (nShapeIndex < 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_SRC_section_shape));
		return;
	}

	std::vector<CString> ImgPath_Sect = SectItemSrcResource::aImgPath_Sect(m_bDumb);
	m_ImgSectItem.SetImage(ImgPath_Sect[nShapeIndex]);
}

void CCMSectItemSrc::DoActionWhenDBChanged()
{
	CString csDB = GetDB();

	if (!csDB.IsEmpty() && !m_Data.SectBefore.SectI.DBName.IsEmpty() &&
		 csDB == m_Data.SectBefore.SectI.DBName) return;

	T_SECT_SECTBASE_D* pSect = &m_Data.SectBefore.SectI;

	// fisrt 초기화
	if (csDB != _T("")) 
	{
		pSect->Initialize();
		// change new DB name
	  pSect->DBName = csDB;
		pSect->Shape = CSectUtil::GetSteelShapeFromSRCShape(m_Data.SectBefore.Shape);
	}
	else // DB가 None으로 변경되면 Size는 유지
	{
		pSect->DBName = _T("");
		pSect->SName = _T("");
	}
 
	// Dialog에 반영한다.
	if (m_nCurShapeType == 1 || m_nCurShapeType == 5)
	{
		SetSteelNameCombo();
		SetSteelSectData();
	}
	else if (m_nCurShapeType == 2)
	{
		SetSteelFirstNameCombo();
		SetSteelSecondNameCombo();
		SetSteelFirstSectData();
		SetSteelSecondSectData();
	}
	else ASSERT(0);

	// Update preview
	//m_wndSecView.SetCentroidFlag(FALSE);
	m_wndSecView.Invalidate();
}

CString CCMSectItemSrc::GetDB()
{
	CString csDB;
	int nUserOrDB;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlDB, nUserOrDB);
	if (nUserOrDB == 0) csDB = _T("");
	else m_wndDB.GetWindowText(csDB);
#if defined(_RUS)
	CSectDBRussiaUtil::ConvertSectDBRusToEng(csDB);
#endif
	return csDB;
}

void CCMSectItemSrc::SetDB(CString &csDBName)
{
	if (csDBName == _T("")) // DB None -> user
	{
		CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlDB, 0);
		m_wndDB.EnableWindow(FALSE);
	}
	else  // use DB
	{
		CString csDB = csDBName;
#if defined(_RUS)
		CSectDBRussiaUtil::ConvertSectDBEngToRus(csDB);
#endif

		CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlDB, 1);
		m_wndDB.EnableWindow(TRUE);
		int nSel = m_wndDB.FindStringExact(-1, csDB);
		if (nSel == CB_ERR) m_wndDB.SelectString(-1, csDB);
		else m_wndDB.SetCurSel(nSel);
	}
}

void CCMSectItemSrc::ChangeMatlData(T_MATL_CONCRETE &ConcrData, T_MATL_STEEL &SteelData)
{
	// add code here
	T_SECT_SECTION_D* pData = &(m_Data.SectBefore);
	pData->Matl_Elast = SteelData.Elast/ConcrData.Elast;
	pData->Matl_Density = SteelData.Density/ConcrData.Density;
	pData->Matl_Poisson_S = SteelData.Poisson;
	pData->Matl_Poisson_C = ConcrData.Poisson;
	SetMatlSectData();
}


BEGIN_MESSAGE_MAP(CCMSectItemSrc, CDialog)
	//{{AFX_MSG_MAP(CCMSectItemSrc)
	ON_CBN_SELCHANGE(IDC_CMD_SP_ID_TYPE, OnChangeShape)
	ON_CBN_SELCHANGE(IDC_CMD_SP_ID_CODE, OnChangeDB)
	ON_CBN_SELCHANGE(IDC_CMD_SP_ID_DBNAME, OnChangeSteelName)
	ON_BN_CLICKED(IDC_CMD_SP_ID_USER_RADIO, OnChangeUseDB)
	ON_BN_CLICKED(IDC_CMD_SP_ID_BUILTUP, OnChangeBuiltUp)
	ON_EN_CHANGE(IDC_CMD_SP_ID_CVALUE1, OnChangeConcrSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_MVALUE1, OnChangeMatlValue)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE1, OnChangeSteelSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID2_VALUE1, OnChangeSteelSize2)
	ON_BN_CLICKED(IDC_CMD_SP_ID_FSTEEL, OnChangeReplace)
	ON_EN_CHANGE(IDC_CMD_SP_ID_ID, OnChangeID)
	ON_EN_CHANGE(IDC_CMD_SP_ID_NAME, OnChangeName)
	ON_BN_CLICKED(IDC_CMD_SP_ID_BTN_SDB, OnClickBtnSelectMatlFromDB)
	ON_EN_SETFOCUS(IDC_CMD_SP_ID_NAME, OnSetfocusCmdSpIdName)
	ON_EN_UPDATE(IDC_CMD_SP_ID_NAME, OnUpdateCmdSpIdName)
	ON_CBN_SELCHANGE(IDC_CMD_SP_ID2_DBNAME, OnChangeSteel2Name)
	ON_CBN_SELCHANGE(IDC_CMD_SP_ID2_DBNAME2, OnChangeSteel2Name2)
	ON_BN_CLICKED(IDC_CMD_SECT_CENTROID_BTN, OnCmdSectCentroidBtn)
	ON_BN_CLICKED(IDC_CMD_CHANGE_OFFSET_BTN, OnCmdChangeOffsetBtn)
	ON_BN_CLICKED(IDC_CMD_SP_ID_DB_RADIO, OnChangeUseDB)
	ON_EN_CHANGE(IDC_CMD_SP_ID_CVALUE2, OnChangeConcrSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_MVALUE2, OnChangeMatlValue)
	ON_EN_CHANGE(IDC_CMD_SP_ID_MVALUE3, OnChangeMatlValue)
	ON_EN_CHANGE(IDC_CMD_SP_ID_MVALUE4, OnChangeMatlValue)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE2, OnChangeSteelSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE3, OnChangeSteelSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE4, OnChangeSteelSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE5, OnChangeSteelSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE6, OnChangeSteelSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID2_VALUE2, OnChangeSteelSize2)
	ON_EN_CHANGE(IDC_CMD_SP_ID2_VALUE3, OnChangeSteelSize2)
	ON_EN_CHANGE(IDC_CMD_SP_ID2_VALUE4, OnChangeSteelSize2)
	ON_EN_CHANGE(IDC_CMD_SP_ID2_VALUE5, OnChangeSteelSize2)
	ON_EN_CHANGE(IDC_CMD_SP_ID2_VALUE6, OnChangeSteelSize2)
	ON_EN_CHANGE(IDC_CMD_SP_ID2_VALUE7, OnChangeSteelSize2)
	ON_EN_CHANGE(IDC_CMD_SP_ID2_VALUE8, OnChangeSteelSize2)
	ON_EN_CHANGE(IDC_CMD_SP_ID3_VALUE1, OnChangeSteelSize3)
	ON_EN_CHANGE(IDC_CMD_SP_ID3_VALUE2, OnChangeSteelSize3)
	ON_EN_CHANGE(IDC_CMD_SP_ID3_VALUE3, OnChangeSteelSize3)
	ON_EN_CHANGE(IDC_CMD_SP_ID3_VALUE4, OnChangeSteelSize3)
	ON_EN_CHANGE(IDC_CMD_SP_ID3_VALUE5, OnChangeSteelSize3)
	ON_EN_CHANGE(IDC_CMD_SP_ID3_VALUE6, OnChangeSteelSize3)
	ON_EN_CHANGE(IDC_CMD_SP_ID3_VALUE7, OnChangeSteelSize3)
	ON_EN_CHANGE(IDC_CMD_SP_ID3_VALUE8, OnChangeSteelSize3)
	ON_EN_CHANGE(IDC_CMD_SP_ID3_VALUE9, OnChangeSteelSize3)
	ON_EN_CHANGE(IDC_CMD_SP_ID3_VALUE10, OnChangeSteelSize3)
	ON_EN_CHANGE(IDC_CMD_SP_ID3_VALUE11, OnChangeSteelSize3)
	ON_EN_CHANGE(IDC_CMD_SP_ID3_VALUE12, OnChangeSteelSize3)
	ON_EN_CHANGE(IDC_CMD_SP_ID4_VALUE1, OnChangeSteelSize4)
	ON_EN_CHANGE(IDC_CMD_SP_ID4_VALUE2, OnChangeSteelSize4)
	ON_EN_CHANGE(IDC_CMD_SP_ID4_VALUE3, OnChangeSteelSize4)
	ON_EN_CHANGE(IDC_CMD_SP_ID4_VALUE4, OnChangeSteelSize4)
	ON_EN_CHANGE(IDC_CMD_SP_ID4_VALUE5, OnChangeSteelSize4)
	ON_EN_CHANGE(IDC_CMD_SP_ID4_VALUE6, OnChangeSteelSize4)
	ON_EN_CHANGE(IDC_CMD_SP_ID4_VALUE7, OnChangeSteelSize4)
	ON_BN_CLICKED(IDC_CMD_SP_ID_FCONCR, OnChangeReplace)
	ON_BN_CLICKED(IDC_CMD_SP_ID_INFUSION_CHK, OnCmdInfusionCheck)
// 	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_SP_ID3_SPIN1, OnDeltaposCmdSpId3Spin1)
// 	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_SP_ID3_SPIN2, OnDeltaposCmdSpId3Spin2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemSrc message handlers
BOOL CCMSectItemSrc::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetRedraw(FALSE);

	m_bDumb = FALSE;
#if defined(_CIVIL_CH)
	m_bDumb = TRUE;
#endif
	if (CTestEnvMgr::GetTestEnvST(_LSX(Test_DUMB)) == _LSX(yes))
	{
		m_bDumb = TRUE;
	}

	AlignControls();

	InitControls(); // 1번만 초기화 해주면 되는 것 들을 초기화 한다.

	if (!m_bModify && !m_bInitDone)
	{
		// 시작 번호는 현재 번호에서 DB가 생성해 주는 다음 번호로 설정
		if(m_bWorkTab)
			m_Key = m_FixID;
		else
			m_Key = m_pDoc->m_pAttrCtrl->GetStartNumSect();

		m_Data.Initialize();
		m_Data.nStype = D_SECT_TYPE_SRC;
		m_Data.SectBefore.Shape = D_SECT_SRC_DEF_SHAPE;
		T_SECT_SECTBASE_D* pSteel = &m_Data.SectBefore.SectI;
		T_SECT_SECTBASE_D* pConcr = &m_Data.SectBefore.SectJ;

		pSteel->Shape = CSectUtil::GetSteelShapeFromSRCShape(m_Data.SectBefore.Shape);
		pConcr->Shape = CSectUtil::GetConcreteShapeFromSRCShape(m_Data.SectBefore.Shape);

		// init db value
		T_PREFERENCE pref;
		m_pDoc->m_pInitCtrl->GetPreference(pref);
		pSteel->DBName = pref.Property.SectionDBName;
	}
	m_wndSecView.SetDataSource(&m_Data);

	ShowKey2Dlg();
	ShowData2Dlg();

	// 초기화 되었음을 표시
	m_bInitDone = TRUE;

	if (m_bDesign) m_wndID.EnableWindow(FALSE);

	// Added by bugboy 2000.7.25
	// Concrete Radio Button 죽임.  <- 나중에 살리려면 밑에 똑같은 주석이 있는 부분 살려주세요.
	((CButton *)GetDlgItem(IDC_CMD_SP_ID_FCONCR))->EnableWindow(FALSE);

	SetRedraw(TRUE);
	RedrawWindow(NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE	
}

void CCMSectItemSrc::OnChangeShape() 
{
	// TODO: Add your control notification handler code here
	int nShapeIndex = m_cboType.GetCurSel();
	if (nShapeIndex < 0) return;
	int nCurShapeIndex = CSectUtil::GetShapeIndexFromNameSrc(m_Data.SectBefore.Shape);
	if (nShapeIndex == nCurShapeIndex) return;

	// Save new shape in data
	m_Data.SectBefore.Shape = CSectUtil::GetShapeNameFromIndexSrc(nShapeIndex);

	T_SECT_SECTBASE_D *pSteel, *pConcr;
	pSteel = &m_Data.SectBefore.SectI;
	pConcr = &m_Data.SectBefore.SectJ;

	// DB Name은 보존
	CString csDBName = pSteel->DBName;

	// Steel 초기화
	pSteel->Initialize();
	pSteel->DBName = csDBName;
	pSteel->Shape = CSectUtil::GetShapeNameFromIndexSrcSteel(nShapeIndex);

	// Concr 초기화
	pConcr->Initialize();
	pConcr->Shape = CSectUtil::GetShapeNameFromIndexSrcConcr(nShapeIndex);

	// Dialog에 반영
	ShowData2Dlg();	
}

void CCMSectItemSrc::OnChangeDB() 
{
	// TODO: Add your control notification handler code here
	DoActionWhenDBChanged();	
}

void CCMSectItemSrc::OnChangeUseDB() 
{
	// TODO: Add your control notification handler code here
	int nUserOrDB;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlDB, nUserOrDB);
	if (nUserOrDB == 0) 
	{
		m_wndDB.EnableWindow(FALSE);
		m_Data.SectBefore.SectI.BuiltUpFlag = 1;  // user -> always built-up
	}
	else 
	{
		m_wndDB.EnableWindow(TRUE);
	}

	// DB가 변경된 경우와 같다.
	DoActionWhenDBChanged();	
}

void CCMSectItemSrc::OnChangeSteelName() 
{
	// TODO: Add your control notification handler code here
	CString csName;
	m_wndSteelName.GetWindowText(csName);
	T_SECT_SECTBASE_D *pSect = &m_Data.SectBefore.SectI;
	if (csName == pSect->SName) return;
#if defined(_RUS)
	CSectDBRussiaUtil::ConvertSectDBNameRusToEng(pSect->DBName, csName);
#endif

	pSect->SName = csName;

#if defined(_RUS)
	CSectDBRussiaUtil::ConvertSectDBNameEngToRus(pSect->DBName, csName);
#endif

	// 데이타를 초기화 한다.
	T_SECT_SECTBASE_D SectData;
	SectData.Initialize();
	if (csName != _T(""))
		m_pDoc->m_pSectDB->GetSectData(pSect->DBName, pSect->SName, SectData);
	for (int i = 0; i < 6; i++)
		pSect->Size[i] = SectData.Size[i];
	pSect->BuiltUpFlag = SectData.BuiltUpFlag;

	// Dialog에 반영한다.
	SetSteelSectData();

	// Section의 Name을 바꾼다.
	if (m_bAutoChangeName)
	{
		m_Data.SName = csName;
		m_wndName.SetWindowText(csName);
	}

	// Preview Update
	//m_wndSecView.SetCentroidFlag(FALSE);
	m_wndSecView.Invalidate();	
}

void CCMSectItemSrc::OnChangeBuiltUp() 
{
	// TODO: Add your control notification handler code here
	if (m_wndBuiltUp.GetCheck()) m_Data.SectBefore.SectI.BuiltUpFlag = 1;
	else m_Data.SectBefore.SectI.BuiltUpFlag = 2;
}

void CCMSectItemSrc::OnChangeConcrSize() 
{
	const MSG* pMsg = GetCurrentMessage();
	UINT nID = LOWORD(pMsg->wParam);
	int nIx;
	switch(nID)
	{
	case IDC_CMD_SP_ID_CVALUE1 : nIx = 0; break;
	case IDC_CMD_SP_ID_CVALUE2 : nIx = 1; break;
	default: ASSERT(0); break;
	}
	T_SECT_SECTBASE_D* pSect = &m_Data.SectBefore.SectJ;
	pSect->Size[nIx] = m_wndConcrSize[nIx].GetEditValue();
	//m_wndSecView.SetCentroidFlag(FALSE);
	m_wndSecView.Invalidate();
}

void CCMSectItemSrc::OnChangeSteelSize() 
{
	const MSG* pMsg = GetCurrentMessage();
	UINT nID = LOWORD(pMsg->wParam);
	int nIx;
	switch(nID)
	{
	case IDC_CMD_SP_ID_VALUE1 : nIx = 0; break;
	case IDC_CMD_SP_ID_VALUE2 : nIx = 1; break;
	case IDC_CMD_SP_ID_VALUE3 : nIx = 2; break;
	case IDC_CMD_SP_ID_VALUE4 : nIx = 3; break;
	case IDC_CMD_SP_ID_VALUE5 : nIx = 4; break;
	case IDC_CMD_SP_ID_VALUE6 : nIx = 5; break;
	default: ASSERT(0); break;
	}
	T_SECT_SECTBASE_D* pSect = &m_Data.SectBefore.SectI;
	pSect->Size[nIx] = m_wndSteelSize[nIx].GetEditValue();
	//m_wndSecView.SetCentroidFlag(FALSE);
	m_wndSecView.Invalidate();	
}

void CCMSectItemSrc::OnChangeSteelSize2() 
{
	const MSG* pMsg = GetCurrentMessage();
	UINT nID = LOWORD(pMsg->wParam);
	int nIx;
	switch(nID)
	{
	case IDC_CMD_SP_ID2_VALUE1 : nIx = 0; break;
	case IDC_CMD_SP_ID2_VALUE2 : nIx = 1; break;
	case IDC_CMD_SP_ID2_VALUE3 : nIx = 2; break;
	case IDC_CMD_SP_ID2_VALUE4 : nIx = 3; break;
	case IDC_CMD_SP_ID2_VALUE5 : nIx = 4; break;
	case IDC_CMD_SP_ID2_VALUE6 : nIx = 5; break;
	case IDC_CMD_SP_ID2_VALUE7 : nIx = 6; break;
	case IDC_CMD_SP_ID2_VALUE8 : nIx = 7; break;
	default: ASSERT(0); break;
	}
	T_SECT_SECTBASE_D* pSect = &m_Data.SectBefore.SectI;
	pSect->Size[nIx] = m_wndSteel2Size[nIx].GetEditValue();
	//m_wndSecView.SetCentroidFlag(FALSE);
	m_wndSecView.Invalidate();	
}

void CCMSectItemSrc::OnChangeSteelSize3() 
{
	const MSG* pMsg = GetCurrentMessage();
	UINT nID = LOWORD(pMsg->wParam);
	int nIx;
	switch(nID)
	{
	case IDC_CMD_SP_ID3_VALUE1 : nIx =-2; break;
	case IDC_CMD_SP_ID3_VALUE2 : nIx =-1; break;
	case IDC_CMD_SP_ID3_VALUE3 : nIx = 0; break;
	case IDC_CMD_SP_ID3_VALUE4 : nIx = 1; break;
	case IDC_CMD_SP_ID3_VALUE5 : nIx = 2; break;
	case IDC_CMD_SP_ID3_VALUE6 : nIx = 3; break;
	case IDC_CMD_SP_ID3_VALUE7 : nIx = 4; break;
	case IDC_CMD_SP_ID3_VALUE8 : nIx = 5; break;
	case IDC_CMD_SP_ID3_VALUE9 : nIx = 6; break;
	case IDC_CMD_SP_ID3_VALUE10: nIx = 7; break;
	case IDC_CMD_SP_ID3_VALUE11: nIx = 8; break;
	case IDC_CMD_SP_ID3_VALUE12: nIx = 9; break;
	default: ASSERT(0); break;
	}
	if (nIx == -2)
	{
		m_Data.SectBefore.nCellShape = m_editN1.GetEditValueInt();
	}
	else if (nIx == -1)
	{
		m_Data.SectBefore.nCellType  = m_editN2.GetEditValueInt();
	}
	else
	{
		T_SECT_SECTBASE_D* pSect = &m_Data.SectBefore.SectI;
		pSect->Size[nIx] = m_wndSteel3Size[nIx].GetEditValue();
	}
	m_wndSecView.Invalidate();	
}

void CCMSectItemSrc::OnChangeSteelSize4()
{
	const MSG* pMsg = GetCurrentMessage();
	UINT nID = LOWORD(pMsg->wParam);

	int nIx;
	switch (nID)
	{
	case IDC_CMD_SP_ID4_VALUE1: nIx = 0; break;
	case IDC_CMD_SP_ID4_VALUE2: nIx = 1; break;
	case IDC_CMD_SP_ID4_VALUE3: nIx = 2; break;
	case IDC_CMD_SP_ID4_VALUE4: nIx = 3; break;
	case IDC_CMD_SP_ID4_VALUE5: nIx = 4; break;
	case IDC_CMD_SP_ID4_VALUE6: nIx = 5; break;
	case IDC_CMD_SP_ID4_VALUE7: nIx = 6; break;
	default: ASSERT(0); break;
	}
	T_SECT_SECTBASE_D* pSect = &m_Data.SectBefore.SectI;
	pSect->Size[nIx] = m_wndSteel4Size[nIx].GetEditValue();

	m_wndSecView.Invalidate();
}

void CCMSectItemSrc::OnChangeMatlValue() 
{
	const MSG* pMsg = GetCurrentMessage();
	UINT nID = LOWORD(pMsg->wParam);
	T_SECT_SECTION_D* pSect = &m_Data.SectBefore;
	switch(nID)
	{
	case IDC_CMD_SP_ID_MVALUE1 : pSect->Matl_Elast = m_wndEsc.GetEditValue(); break;
	case IDC_CMD_SP_ID_MVALUE2 : pSect->Matl_Density = m_wndDsc.GetEditValue(); break;
	case IDC_CMD_SP_ID_MVALUE3 : pSect->Matl_Poisson_S = m_wndPs.GetEditValue(); break;
	case IDC_CMD_SP_ID_MVALUE4 : pSect->Matl_Poisson_C = m_wndPc.GetEditValue(); break;  
	case IDC_CMD_SP_ID_MSTIFF  : pSect->Matl_StiffFactor = m_wndMStiff.GetEditValue(); break;
	default: ASSERT(0); break;
	}
}

void CCMSectItemSrc::OnChangeReplace() 
{
	// TODO: Add your control notification handler code here
	int nCheck;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlReplace, nCheck);
	m_Data.SectBefore.nSRCtype = nCheck+1;
}

void CCMSectItemSrc::OnCmdInfusionCheck()
{
	m_Data.bInfusionChk = m_chkInfusionConc.GetCheck();
	m_wndSecView.Invalidate();
}

void CCMSectItemSrc::OnChangeID() 
{
	CString csKey;
	m_wndID.GetWindowText(csKey);
	m_Key = _tstol(csKey);
}

void CCMSectItemSrc::OnChangeName() 
{
	m_wndName.GetWindowText(m_Data.SName);
}

void CCMSectItemSrc::OnClickBtnSelectMatlFromDB() 
{
	// TODO: Add your control notification handler code here
	CRect rect;
	GetDlgItem(IDC_CMD_SP_ID_BTN_SDB)->GetWindowRect(rect);
	//rect.top += 20;

	BOOL bDumb = (m_nCurShapeType == 4 || m_nCurShapeType == 5) ? TRUE : FALSE;

	CCMConcrSteelMatlDlg dlg(m_pDoc, rect, IDD_CMD_SECT_ITEM_SRC, this, bDumb);
	CStringArray aData;
	UINT aID[] = {
		IDC_CMD_SP_ID_MVALUE1, IDC_CMD_SP_ID_MVALUE2,
		IDC_CMD_SP_ID_MVALUE3, IDC_CMD_SP_ID_MVALUE4,
	};

	// save current data
	CString data;
	for (int i = 0; i < sizeof(aID)/sizeof(UINT); i++)
	{
		GetDlgItem(aID[i])->GetWindowText(data);
		aData.Add(data);
	}
	if (dlg.DoModal() != IDOK)
	{
		// restore old data
		for (int i = 0; i < sizeof(aID)/sizeof(UINT); i++)
			GetDlgItem(aID[i])->SetWindowText(aData[i]);
	}		
}

void CCMSectItemSrc::OnSetfocusCmdSpIdName() 
{
	::PostMessage(m_wndName.GetSafeHwnd(), EM_SETSEL, 0, -1);
}

void CCMSectItemSrc::OnUpdateCmdSpIdName() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	CString csDB, csName;
	csDB = GetDB();
	m_wndName.GetWindowText(csName);
	if (csName == _T("")) m_bAutoChangeName = TRUE;
	else
	{
		T_SECT_SECTBASE_D SectData;

		if (csDB == _T("")) m_bAutoChangeName = FALSE;  // change by key in
		else if (m_pDoc->m_pSectDB->GetSectData(csDB, csName, SectData))
			m_bAutoChangeName = TRUE; // exist in db
		else m_bAutoChangeName = FALSE; // not exist in db(change by key in)
	}	
}

BOOL CCMSectItemSrc::GetSectData(T_SECT_D& sect_data) //add hjs 03/03/12
{
	if(Dlg2Data()) 
	{
		sect_data = m_Data;
		return TRUE;
	}
	else
		sect_data.Initialize();
	return FALSE;
}

void CCMSectItemSrc::OnChangeSteel2Name() 
{
	// TODO: Add your control notification handler code here
	CString csName;
	m_wndSteel2Name1.GetWindowText(csName);
	T_SECT_SECTBASE_D *pSect = &m_Data.SectBefore.SectI;
	if (csName == pSect->SName) return;

#if defined(_RUS)
	CSectDBRussiaUtil::ConvertSectDBNameRusToEng(pSect->DBName, csName);
#endif

	pSect->SName = csName;

#if defined(_RUS)
	CSectDBRussiaUtil::ConvertSectDBNameEngToRus(pSect->DBName, csName);
#endif

	// 데이타를 초기화 한다.
	T_SECT_SECTBASE_D SectData;
	SectData.Initialize();
	if (csName != _T(""))
		m_pDoc->m_pSectDB->GetSectData(pSect->DBName, pSect->SName, SectData);
	for (int i = 0; i < 4; i++)
		pSect->Size[i] = SectData.Size[i];

	// Dialog에 반영한다.
	SetSteelFirstSectData();

	// Section의 Name을 바꾼다.
	if (m_bAutoChangeName)
	{
		m_Data.SName = csName;
		m_wndName.SetWindowText(csName);
	}

	// Preview Update
	//m_wndSecView.SetCentroidFlag(FALSE);
	m_wndSecView.Invalidate();		
}

void CCMSectItemSrc::OnChangeSteel2Name2() 
{
	// TODO: Add your control notification handler code here
	CString csName2;
	m_wndSteel2Name2.GetWindowText(csName2);
	T_SECT_SECTBASE_D *pSect = &m_Data.SectBefore.SectI;
	if (csName2 == pSect->SName2) return;

#if defined(_RUS)
	CSectDBRussiaUtil::ConvertSectDBNameRusToEng(pSect->DBName, csName2);
#endif

	pSect->SName2 = csName2;

#if defined(_RUS)
	CSectDBRussiaUtil::ConvertSectDBNameEngToRus(pSect->DBName, csName2);
#endif

	// 데이타를 초기화 한다.
	T_SECT_SECTBASE_D SectData;
	SectData.Initialize();
	if (csName2 != _T(""))
		m_pDoc->m_pSectDB->GetSectData(pSect->DBName, pSect->SName2, SectData);
	for (int i = 0; i < 4; i++)
		pSect->Size[i+4] = SectData.Size[i];

	// Dialog에 반영한다.
	SetSteelSecondSectData();

	// Section의 Name을 바꾼다.
	if (m_bAutoChangeName)
	{
		m_Data.SName = csName2;
		m_wndName.SetWindowText(csName2);
	}

	// Preview Update
	//m_wndSecView.SetCentroidFlag(FALSE);
	m_wndSecView.Invalidate();			
}

int CCMSectItemSrc::GetShapeType(CString& csShape)
{
	if (csShape == D_SECT_SHAPE_SRC_RH2T) return 2;
	if (csShape == D_SECT_SHAPE_SRC_RHT)  return 2;
	if (csShape == D_SECT_SHAPE_SRC_BSTF) return 3;
	if (csShape == D_SECT_SHAPE_SRC_PSTF) return 3;
	if (csShape == D_SECT_SHAPE_SRC_DUMB) return 4;
	if (csShape == D_SECT_SHAPE_SRC_STUB) return 5;
	return 1;
}

void CCMSectItemSrc::ChangeShapeType(int nShapeType)
{
	ASSERT(nShapeType == 1 || nShapeType == 2 || nShapeType == 3 || nShapeType == 4 || nShapeType == 5);
	CDlgUtil::CtrlShowHide(this, m_aCtrlType0, nShapeType != 3 && nShapeType != 4);
	CDlgUtil::CtrlShowHide(this, m_aCtrlType1, nShapeType == 1 || nShapeType == 5);
	CDlgUtil::CtrlShowHide(this, m_aCtrlType2, nShapeType == 2);
	CDlgUtil::CtrlShowHide(this, m_aCtrlType3, nShapeType == 3);
	CDlgUtil::CtrlShowHide(this, m_aCtrlType4, nShapeType == 4);

	m_nCurShapeType = nShapeType;
}

void CCMSectItemSrc::OnCmdSectCentroidBtn() 
{
	// TODO: Add your control notification handler code here
	if (!Dlg2Data() || !GetPeriAndStiff())
	{
		AfxMessageBox(_LS(IDS_CMD_SECT_Impossible_Display_Centroid));
		return;
	}
	//m_wndSecView.SetCentroidFlag(TRUE);
	m_wndSecView.Invalidate();		
}

void CCMSectItemSrc::OnCmdChangeOffsetBtn() 
{
	// TODO: Add your control notification handler code here
	T_SECT_SECTION_D OldData;
	OldData = m_Data.SectBefore;

	CCMSectOffsetDlg dlg(&m_Data.SectBefore, FALSE, this);
	if (dlg.DoModal() == IDOK)
	{
		CStringArray arOffsetNameList;
		CSectUtil::GetOffsetNameList(arOffsetNameList);
		if (m_Data.SectBefore.nOffsetPoint >= 1)
		{
			CString csOffset = arOffsetNameList[m_Data.SectBefore.nOffsetPoint-1];
			m_txtOffset.SetWindowText(csOffset);
		}
	}			
	else m_Data.SectBefore = OldData;
	
	m_wndSecView.Invalidate();	
}

// void CCMSectItemSrc::OnDeltaposCmdSpId3Spin1(NMHDR* pNMHDR, LRESULT* pResult) 
// {
// 	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
// 	// TODO: Add your control notification handler code here
// 	CDlgUtil::SetFloatSpinValue(this, IDC_CMD_SP_ID3_VALUE1,_T("1"),_T("100"),1,pNMHDR,pResult);
// 	
// 	CString csN1;
// 	m_editN1.GetWindowText(csN1);
// 	m_Data.SectBefore.nCellShape = _ttoi(csN1);
// 	m_wndSecView.Invalidate();
// 	
// 	*pResult = 0;
// }
// 
// void CCMSectItemSrc::OnDeltaposCmdSpId3Spin2(NMHDR* pNMHDR, LRESULT* pResult) 
// {
// 	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
// 	// TODO: Add your control notification handler code here
// 	CDlgUtil::SetFloatSpinValue(this, IDC_CMD_SP_ID3_VALUE2,_T("1"),_T("100"),1,pNMHDR,pResult);
// 	
// 	CString csN2;
// 	m_editN2.GetWindowText(csN2);
// 	m_Data.SectBefore.nCellType = _ttoi(csN2);
// 	m_wndSecView.Invalidate();
// 
// 	*pResult = 0;
// }
