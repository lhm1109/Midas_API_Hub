// DgnConBarSizeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConBarSizeDlg.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_InitCtrl.h"

#include "..\wg_base\wg_base_LocaleLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConBarSizeDlg dialog


CDgnConBarSizeDlg::CDgnConBarSizeDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnConBarSizeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConBarSizeDlg)
	//}}AFX_DATA_INIT
	m_TotalReBarSize = _T("");
	m_CheckCount = 0;
}

#define IDRBAR_11(n)\
	arRbar11.Add(IDC_DGN_CHECK_11##n);
#define IDRBAR_12(n)\
	arRbar12.Add(IDC_DGN_CHECK_12##n);
#define IDRBAR_13(n)\
	arRbar13.Add(IDC_DGN_CHECK_13##n);
#define IDRBAR_14(n)\
	arRbar14.Add(IDC_DGN_CHECK_14##n);
#define IDRBAR_21(n)\
	arRbar21.Add(IDC_DGN_CHECK_21##n);
#define IDRBAR_31(n)\
	arRbar31.Add(IDC_DGN_CHECK_31##n);
#define IDRBAR_32(n)\
	arRbar32.Add(IDC_DGN_CHECK_32##n);
#define IDRBAR_33(n)\
	arRbar33.Add(IDC_DGN_CHECK_33##n);
#define IDRBAR_34(n)\
	arRbar34.Add(IDC_DGN_CHECK_34##n);
#define IDRBAR_41(n)\
	arRbar41.Add(IDC_DGN_CHECK_41##n);
#define IDRBAR_51(n)\
	arRbar51.Add(IDC_DGN_CHECK_51##n);
#define IDRBAR_61(n)\
	arRbar61.Add(IDC_DGN_CHECK_61##n);
#define IDRBAR_71(n)\
	arRbar71.Add(IDC_DGN_CHECK_71##n);

void CDgnConBarSizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConBarSizeDlg)
	//}}AFX_DATA_MAP
	CArray<UINT,UINT> arRbar11;  IDRBAR_11(01)IDRBAR_11(02)IDRBAR_11(03)IDRBAR_11(04)IDRBAR_11(05)IDRBAR_11(06)IDRBAR_11(07)IDRBAR_11(08)IDRBAR_11(09)IDRBAR_11(10)IDRBAR_11(11)IDRBAR_11(12)IDRBAR_11(13)IDRBAR_11(14)IDRBAR_11(15);
	CArray<UINT,UINT> arRbar12;  IDRBAR_12(01)IDRBAR_12(02)IDRBAR_12(03)IDRBAR_12(04)IDRBAR_12(05)IDRBAR_12(06)IDRBAR_12(07)IDRBAR_12(08)IDRBAR_12(09)IDRBAR_12(10)IDRBAR_12(11)IDRBAR_12(12)IDRBAR_12(13);
	CArray<UINT,UINT> arRbar13;  IDRBAR_13(01)IDRBAR_13(02)IDRBAR_13(03)IDRBAR_13(04)IDRBAR_13(05)IDRBAR_13(06)IDRBAR_13(07)IDRBAR_13(08)IDRBAR_13(09)IDRBAR_13(10)IDRBAR_13(11)IDRBAR_13(12)IDRBAR_13(13);
	CArray<UINT,UINT> arRbar14;  IDRBAR_14(01)IDRBAR_14(02)IDRBAR_14(03)IDRBAR_14(04)IDRBAR_14(05)IDRBAR_14(06)IDRBAR_14(07)IDRBAR_14(08)IDRBAR_14(09)IDRBAR_14(10);
	CArray<UINT,UINT> arRbar21;  IDRBAR_21(01)IDRBAR_21(02)IDRBAR_21(03)IDRBAR_21(04)IDRBAR_21(05)IDRBAR_21(06)IDRBAR_21(07)IDRBAR_21(08)IDRBAR_21(09)IDRBAR_21(10)IDRBAR_21(11);
	CArray<UINT,UINT> arRbar31;  IDRBAR_31(01)IDRBAR_31(02)IDRBAR_31(03)IDRBAR_31(04)IDRBAR_31(05)IDRBAR_31(06)IDRBAR_31(07)IDRBAR_31(08)IDRBAR_31(09)IDRBAR_31(10)IDRBAR_31(11)IDRBAR_31(12)IDRBAR_31(13)IDRBAR_31(14);
	CArray<UINT,UINT> arRbar32;  IDRBAR_32(01)IDRBAR_32(02)IDRBAR_32(03)IDRBAR_32(04)IDRBAR_32(05)IDRBAR_32(06)IDRBAR_32(07)IDRBAR_32(08)IDRBAR_32(09)IDRBAR_32(10)IDRBAR_32(11)IDRBAR_32(12)IDRBAR_32(13);
	CArray<UINT,UINT> arRbar33;  IDRBAR_33(01)IDRBAR_33(02)IDRBAR_33(03)IDRBAR_33(04)IDRBAR_33(05)IDRBAR_33(06)IDRBAR_33(07)IDRBAR_33(08)IDRBAR_33(09)IDRBAR_33(10)IDRBAR_33(11)IDRBAR_33(12)IDRBAR_33(13)IDRBAR_33(14)IDRBAR_33(15)IDRBAR_33(16)IDRBAR_33(17);
	CArray<UINT,UINT> arRbar34;  IDRBAR_34(01)IDRBAR_34(02)IDRBAR_34(03)IDRBAR_34(04)IDRBAR_34(05)IDRBAR_34(06)IDRBAR_34(07)IDRBAR_34(08)IDRBAR_34(09)IDRBAR_34(10)IDRBAR_34(11)IDRBAR_34(12)IDRBAR_34(13)IDRBAR_34(14);
	CArray<UINT,UINT> arRbar41;  IDRBAR_41(01)IDRBAR_41(02)IDRBAR_41(03)IDRBAR_41(04)IDRBAR_41(05)IDRBAR_41(06)IDRBAR_41(07)IDRBAR_41(08)IDRBAR_41(09)IDRBAR_41(10)IDRBAR_41(11)IDRBAR_41(12)IDRBAR_41(13)IDRBAR_41(14)IDRBAR_41(15)IDRBAR_41(16);
	CArray<UINT,UINT> arRbar51;  IDRBAR_51(01)IDRBAR_51(02)IDRBAR_51(03)IDRBAR_51(04)IDRBAR_51(05)IDRBAR_51(06)IDRBAR_51(07)IDRBAR_51(08);
	CArray<UINT,UINT> arRbar61;  IDRBAR_61(01)IDRBAR_61(02)IDRBAR_61(03)IDRBAR_61(04)IDRBAR_61(05)IDRBAR_61(06)IDRBAR_61(07)IDRBAR_61(08);IDRBAR_61(09);IDRBAR_61(10);IDRBAR_61(11);IDRBAR_61(12);IDRBAR_61(13);IDRBAR_61(14);
	CArray<UINT,UINT> arRbar71;  IDRBAR_71(01)IDRBAR_71(02)IDRBAR_71(03)IDRBAR_71(04)IDRBAR_71(05)IDRBAR_71(06)IDRBAR_71(07)IDRBAR_71(08);IDRBAR_71(09);IDRBAR_71(10);IDRBAR_71(11);IDRBAR_71(12);IDRBAR_71(13);


	for(int i=0; i<CONST_BAR_KS  ; i++)	    DDX_Check(pDX, arRbar11[i], m_bRbar11[i]);
	for(int i=0; i<CONST_BAR_CNS ; i++)	    DDX_Check(pDX, arRbar12[i], m_bRbar12[i]);
	for(int i=0; i<CONST_BAR_JIS ; i++)	    DDX_Check(pDX, arRbar13[i], m_bRbar13[i]);
	for(int i=0; i<CONST_BAR_PNS49;i++)	    DDX_Check(pDX, arRbar14[i], m_bRbar14[i]);
	for(int i=0; i<CONST_BAR_ASTM; i++)	    DDX_Check(pDX, arRbar21[i], m_bRbar21[i]);
	for(int i=0; i<CONST_BAR_BSEN; i++)	    DDX_Check(pDX, arRbar31[i], m_bRbar31[i]);
	for(int i=0; i<CONST_BAR_IS  ; i++)	    DDX_Check(pDX, arRbar32[i], m_bRbar32[i]);
	for(int i=0; i<CONST_BAR_UNI ; i++)	    DDX_Check(pDX, arRbar33[i], m_bRbar33[i]);
	for(int i=0; i<CONST_BAR_SS  ; i++)	    DDX_Check(pDX, arRbar34[i], m_bRbar34[i]);
	for(int i=0; i<CONST_BAR_GB  ; i++)	    DDX_Check(pDX, arRbar41[i], m_bRbar41[i]);
	for(int i=0; i<CONST_BAR_CSA ; i++)	    DDX_Check(pDX, arRbar51[i], m_bRbar51[i]);
	for(int i=0; i<CONST_BAR_GOST_SP ; i++)	DDX_Check(pDX, arRbar61[i], m_bRbar61[i]);
	for(int i=0; i<CONST_BAR_AS ;	 i++)		  DDX_Check(pDX, arRbar71[i], m_bRbar71[i]);
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnConBarSizeDlg, CInternationalDlg)
#undef CDialog
	//{{AFX_MSG_MAP(CDgnConBarSizeDlg)
	ON_BN_CLICKED(IDC_DGN_CHECK_1101,  OnDgnCheck1101)
	ON_BN_CLICKED(IDC_DGN_CHECK_1102,  OnDgnCheck1102)
	ON_BN_CLICKED(IDC_DGN_CHECK_1103,  OnDgnCheck1103)
	ON_BN_CLICKED(IDC_DGN_CHECK_1104,  OnDgnCheck1104)
	ON_BN_CLICKED(IDC_DGN_CHECK_1105,  OnDgnCheck1105)
	ON_BN_CLICKED(IDC_DGN_CHECK_1106,  OnDgnCheck1106)
	ON_BN_CLICKED(IDC_DGN_CHECK_1107,  OnDgnCheck1107)
	ON_BN_CLICKED(IDC_DGN_CHECK_1108,  OnDgnCheck1108)
	ON_BN_CLICKED(IDC_DGN_CHECK_1109,  OnDgnCheck1109)
	ON_BN_CLICKED(IDC_DGN_CHECK_1110,  OnDgnCheck1110)
	ON_BN_CLICKED(IDC_DGN_CHECK_1111,  OnDgnCheck1111)
	ON_BN_CLICKED(IDC_DGN_CHECK_1112,  OnDgnCheck1112)
	ON_BN_CLICKED(IDC_DGN_CHECK_1113,  OnDgnCheck1113)
	ON_BN_CLICKED(IDC_DGN_CHECK_1114,  OnDgnCheck1114)
	ON_BN_CLICKED(IDC_DGN_CHECK_1115,  OnDgnCheck1115)

	ON_BN_CLICKED(IDC_DGN_CHECK_1201,  OnDgnCheck1201)
	ON_BN_CLICKED(IDC_DGN_CHECK_1202,  OnDgnCheck1202)
	ON_BN_CLICKED(IDC_DGN_CHECK_1203,  OnDgnCheck1203)
	ON_BN_CLICKED(IDC_DGN_CHECK_1204,  OnDgnCheck1204)
	ON_BN_CLICKED(IDC_DGN_CHECK_1205,  OnDgnCheck1205)
	ON_BN_CLICKED(IDC_DGN_CHECK_1206,  OnDgnCheck1206)
	ON_BN_CLICKED(IDC_DGN_CHECK_1207,  OnDgnCheck1207)
	ON_BN_CLICKED(IDC_DGN_CHECK_1208,  OnDgnCheck1208)
	ON_BN_CLICKED(IDC_DGN_CHECK_1209,  OnDgnCheck1209)
	ON_BN_CLICKED(IDC_DGN_CHECK_1210,  OnDgnCheck1210)
	ON_BN_CLICKED(IDC_DGN_CHECK_1211,  OnDgnCheck1211)
	ON_BN_CLICKED(IDC_DGN_CHECK_1212,  OnDgnCheck1212)
	ON_BN_CLICKED(IDC_DGN_CHECK_1213,  OnDgnCheck1213)

	ON_BN_CLICKED(IDC_DGN_CHECK_1301,  OnDgnCheck1301)
	ON_BN_CLICKED(IDC_DGN_CHECK_1302,  OnDgnCheck1302)
	ON_BN_CLICKED(IDC_DGN_CHECK_1303,  OnDgnCheck1303)
	ON_BN_CLICKED(IDC_DGN_CHECK_1304,  OnDgnCheck1304)
	ON_BN_CLICKED(IDC_DGN_CHECK_1305,  OnDgnCheck1305)
	ON_BN_CLICKED(IDC_DGN_CHECK_1306,  OnDgnCheck1306)
	ON_BN_CLICKED(IDC_DGN_CHECK_1307,  OnDgnCheck1307)
	ON_BN_CLICKED(IDC_DGN_CHECK_1308,  OnDgnCheck1308)
	ON_BN_CLICKED(IDC_DGN_CHECK_1309,  OnDgnCheck1309)
	ON_BN_CLICKED(IDC_DGN_CHECK_1310,  OnDgnCheck1310)
	ON_BN_CLICKED(IDC_DGN_CHECK_1311,  OnDgnCheck1311)
	ON_BN_CLICKED(IDC_DGN_CHECK_1312,  OnDgnCheck1312)
	ON_BN_CLICKED(IDC_DGN_CHECK_1313,  OnDgnCheck1313)

	ON_BN_CLICKED(IDC_DGN_CHECK_1401, OnDgnCheck1401)
	ON_BN_CLICKED(IDC_DGN_CHECK_1402, OnDgnCheck1402)
	ON_BN_CLICKED(IDC_DGN_CHECK_1403, OnDgnCheck1403)
	ON_BN_CLICKED(IDC_DGN_CHECK_1404, OnDgnCheck1404)
	ON_BN_CLICKED(IDC_DGN_CHECK_1405, OnDgnCheck1405)
	ON_BN_CLICKED(IDC_DGN_CHECK_1406, OnDgnCheck1406)
	ON_BN_CLICKED(IDC_DGN_CHECK_1407, OnDgnCheck1407)
	ON_BN_CLICKED(IDC_DGN_CHECK_1408, OnDgnCheck1408)
	ON_BN_CLICKED(IDC_DGN_CHECK_1409, OnDgnCheck1409)
	ON_BN_CLICKED(IDC_DGN_CHECK_1410, OnDgnCheck1410)

	ON_BN_CLICKED(IDC_DGN_CHECK_2101,  OnDgnCheck2101)
	ON_BN_CLICKED(IDC_DGN_CHECK_2102,  OnDgnCheck2102)
	ON_BN_CLICKED(IDC_DGN_CHECK_2103,  OnDgnCheck2103)
	ON_BN_CLICKED(IDC_DGN_CHECK_2104,  OnDgnCheck2104)
	ON_BN_CLICKED(IDC_DGN_CHECK_2105,  OnDgnCheck2105)
	ON_BN_CLICKED(IDC_DGN_CHECK_2106,  OnDgnCheck2106)
	ON_BN_CLICKED(IDC_DGN_CHECK_2107,  OnDgnCheck2107)
	ON_BN_CLICKED(IDC_DGN_CHECK_2108,  OnDgnCheck2108)
	ON_BN_CLICKED(IDC_DGN_CHECK_2109,  OnDgnCheck2109)
	ON_BN_CLICKED(IDC_DGN_CHECK_2110,  OnDgnCheck2110)
	ON_BN_CLICKED(IDC_DGN_CHECK_2111,  OnDgnCheck2111)
	
	ON_BN_CLICKED(IDC_DGN_CHECK_3101,  OnDgnCheck3101)
	ON_BN_CLICKED(IDC_DGN_CHECK_3102,  OnDgnCheck3102)
	ON_BN_CLICKED(IDC_DGN_CHECK_3103,  OnDgnCheck3103)
	ON_BN_CLICKED(IDC_DGN_CHECK_3104,  OnDgnCheck3104)
	ON_BN_CLICKED(IDC_DGN_CHECK_3105,  OnDgnCheck3105)
	ON_BN_CLICKED(IDC_DGN_CHECK_3106,  OnDgnCheck3106)
	ON_BN_CLICKED(IDC_DGN_CHECK_3107,  OnDgnCheck3107)
	ON_BN_CLICKED(IDC_DGN_CHECK_3108,  OnDgnCheck3108)
	ON_BN_CLICKED(IDC_DGN_CHECK_3109,  OnDgnCheck3109)
	ON_BN_CLICKED(IDC_DGN_CHECK_3110,  OnDgnCheck3110)
	ON_BN_CLICKED(IDC_DGN_CHECK_3111,  OnDgnCheck3111)
	ON_BN_CLICKED(IDC_DGN_CHECK_3112,  OnDgnCheck3112)
	ON_BN_CLICKED(IDC_DGN_CHECK_3113,  OnDgnCheck3113)
	ON_BN_CLICKED(IDC_DGN_CHECK_3114,  OnDgnCheck3114)
	
	ON_BN_CLICKED(IDC_DGN_CHECK_3201,  OnDgnCheck3201)
	ON_BN_CLICKED(IDC_DGN_CHECK_3202,  OnDgnCheck3202)
	ON_BN_CLICKED(IDC_DGN_CHECK_3203,  OnDgnCheck3203)
	ON_BN_CLICKED(IDC_DGN_CHECK_3204,  OnDgnCheck3204)
	ON_BN_CLICKED(IDC_DGN_CHECK_3205,  OnDgnCheck3205)
	ON_BN_CLICKED(IDC_DGN_CHECK_3206,  OnDgnCheck3206)
	ON_BN_CLICKED(IDC_DGN_CHECK_3207,  OnDgnCheck3207)
	ON_BN_CLICKED(IDC_DGN_CHECK_3208,  OnDgnCheck3208)
	ON_BN_CLICKED(IDC_DGN_CHECK_3209,  OnDgnCheck3209)
	ON_BN_CLICKED(IDC_DGN_CHECK_3210,  OnDgnCheck3210)
	ON_BN_CLICKED(IDC_DGN_CHECK_3211,  OnDgnCheck3211)
	ON_BN_CLICKED(IDC_DGN_CHECK_3212,  OnDgnCheck3212)
	ON_BN_CLICKED(IDC_DGN_CHECK_3213,  OnDgnCheck3213)

	ON_BN_CLICKED(IDC_DGN_CHECK_3301,  OnDgnCheck3301)
	ON_BN_CLICKED(IDC_DGN_CHECK_3302,  OnDgnCheck3302)
	ON_BN_CLICKED(IDC_DGN_CHECK_3303,  OnDgnCheck3303)
	ON_BN_CLICKED(IDC_DGN_CHECK_3304,  OnDgnCheck3304)
	ON_BN_CLICKED(IDC_DGN_CHECK_3305,  OnDgnCheck3305)
	ON_BN_CLICKED(IDC_DGN_CHECK_3306,  OnDgnCheck3306)
	ON_BN_CLICKED(IDC_DGN_CHECK_3307,  OnDgnCheck3307)
	ON_BN_CLICKED(IDC_DGN_CHECK_3308,  OnDgnCheck3308)
	ON_BN_CLICKED(IDC_DGN_CHECK_3309,  OnDgnCheck3309)
	ON_BN_CLICKED(IDC_DGN_CHECK_3310,  OnDgnCheck3310)
	ON_BN_CLICKED(IDC_DGN_CHECK_3311,  OnDgnCheck3311)
	ON_BN_CLICKED(IDC_DGN_CHECK_3312,  OnDgnCheck3312)
	ON_BN_CLICKED(IDC_DGN_CHECK_3313,  OnDgnCheck3313)
	ON_BN_CLICKED(IDC_DGN_CHECK_3314,  OnDgnCheck3314)
	ON_BN_CLICKED(IDC_DGN_CHECK_3315,  OnDgnCheck3315)
	ON_BN_CLICKED(IDC_DGN_CHECK_3316,  OnDgnCheck3316)
	ON_BN_CLICKED(IDC_DGN_CHECK_3317,  OnDgnCheck3317)

	ON_BN_CLICKED(IDC_DGN_CHECK_3401,  OnDgnCheck3401)
	ON_BN_CLICKED(IDC_DGN_CHECK_3402,  OnDgnCheck3402)
	ON_BN_CLICKED(IDC_DGN_CHECK_3403,  OnDgnCheck3403)
	ON_BN_CLICKED(IDC_DGN_CHECK_3404,  OnDgnCheck3404)
	ON_BN_CLICKED(IDC_DGN_CHECK_3405,  OnDgnCheck3405)
	ON_BN_CLICKED(IDC_DGN_CHECK_3406,  OnDgnCheck3406)
	ON_BN_CLICKED(IDC_DGN_CHECK_3407,  OnDgnCheck3407)
	ON_BN_CLICKED(IDC_DGN_CHECK_3408,  OnDgnCheck3408)
	ON_BN_CLICKED(IDC_DGN_CHECK_3409,  OnDgnCheck3409)
	ON_BN_CLICKED(IDC_DGN_CHECK_3410,  OnDgnCheck3410)
	ON_BN_CLICKED(IDC_DGN_CHECK_3411,  OnDgnCheck3411)
	ON_BN_CLICKED(IDC_DGN_CHECK_3412,  OnDgnCheck3412)
	ON_BN_CLICKED(IDC_DGN_CHECK_3413,  OnDgnCheck3413)
	ON_BN_CLICKED(IDC_DGN_CHECK_3414,  OnDgnCheck3414)

	ON_BN_CLICKED(IDC_DGN_CHECK_4101,  OnDgnCheck4101)
	ON_BN_CLICKED(IDC_DGN_CHECK_4102,  OnDgnCheck4102)
	ON_BN_CLICKED(IDC_DGN_CHECK_4103,  OnDgnCheck4103)
	ON_BN_CLICKED(IDC_DGN_CHECK_4104,  OnDgnCheck4104)
	ON_BN_CLICKED(IDC_DGN_CHECK_4105,  OnDgnCheck4105)
	ON_BN_CLICKED(IDC_DGN_CHECK_4106,  OnDgnCheck4106)
	ON_BN_CLICKED(IDC_DGN_CHECK_4107,  OnDgnCheck4107)
	ON_BN_CLICKED(IDC_DGN_CHECK_4108,  OnDgnCheck4108)
	ON_BN_CLICKED(IDC_DGN_CHECK_4109,  OnDgnCheck4109)
	ON_BN_CLICKED(IDC_DGN_CHECK_4110,  OnDgnCheck4110)
	ON_BN_CLICKED(IDC_DGN_CHECK_4111,  OnDgnCheck4111)
	ON_BN_CLICKED(IDC_DGN_CHECK_4112,  OnDgnCheck4112)
	ON_BN_CLICKED(IDC_DGN_CHECK_4113,  OnDgnCheck4113)
	ON_BN_CLICKED(IDC_DGN_CHECK_4114,  OnDgnCheck4114)
	ON_BN_CLICKED(IDC_DGN_CHECK_4115,  OnDgnCheck4115)
	ON_BN_CLICKED(IDC_DGN_CHECK_4116,  OnDgnCheck4116)

	ON_BN_CLICKED(IDC_DGN_CHECK_5101,  OnDgnCheck5101)
	ON_BN_CLICKED(IDC_DGN_CHECK_5102,  OnDgnCheck5102)
	ON_BN_CLICKED(IDC_DGN_CHECK_5103,  OnDgnCheck5103)
	ON_BN_CLICKED(IDC_DGN_CHECK_5104,  OnDgnCheck5104)
	ON_BN_CLICKED(IDC_DGN_CHECK_5105,  OnDgnCheck5105)
	ON_BN_CLICKED(IDC_DGN_CHECK_5106,  OnDgnCheck5106)
	ON_BN_CLICKED(IDC_DGN_CHECK_5107,  OnDgnCheck5107)
	ON_BN_CLICKED(IDC_DGN_CHECK_5108,  OnDgnCheck5108)

	ON_BN_CLICKED(IDC_DGN_CHECK_6101,  OnDgnCheck6101)
	ON_BN_CLICKED(IDC_DGN_CHECK_6102,  OnDgnCheck6102)
	ON_BN_CLICKED(IDC_DGN_CHECK_6103,  OnDgnCheck6103)
	ON_BN_CLICKED(IDC_DGN_CHECK_6104,  OnDgnCheck6104)
	ON_BN_CLICKED(IDC_DGN_CHECK_6105,  OnDgnCheck6105)
	ON_BN_CLICKED(IDC_DGN_CHECK_6106,  OnDgnCheck6106)
	ON_BN_CLICKED(IDC_DGN_CHECK_6107,  OnDgnCheck6107)
	ON_BN_CLICKED(IDC_DGN_CHECK_6108,  OnDgnCheck6108)
	ON_BN_CLICKED(IDC_DGN_CHECK_6109,  OnDgnCheck6109)
	ON_BN_CLICKED(IDC_DGN_CHECK_6110,  OnDgnCheck6110)
	ON_BN_CLICKED(IDC_DGN_CHECK_6111,  OnDgnCheck6111)
	ON_BN_CLICKED(IDC_DGN_CHECK_6112,  OnDgnCheck6112)
	ON_BN_CLICKED(IDC_DGN_CHECK_6113,  OnDgnCheck6113)
	ON_BN_CLICKED(IDC_DGN_CHECK_6114,  OnDgnCheck6114)

	ON_BN_CLICKED(IDC_DGN_CHECK_7101,  OnDgnCheck7101)
	ON_BN_CLICKED(IDC_DGN_CHECK_7102,  OnDgnCheck7102)
	ON_BN_CLICKED(IDC_DGN_CHECK_7103,  OnDgnCheck7103)
	ON_BN_CLICKED(IDC_DGN_CHECK_7104,  OnDgnCheck7104)
	ON_BN_CLICKED(IDC_DGN_CHECK_7105,  OnDgnCheck7105)
	ON_BN_CLICKED(IDC_DGN_CHECK_7106,  OnDgnCheck7106)
	ON_BN_CLICKED(IDC_DGN_CHECK_7107,  OnDgnCheck7107)
	ON_BN_CLICKED(IDC_DGN_CHECK_7108,  OnDgnCheck7108)
	ON_BN_CLICKED(IDC_DGN_CHECK_7109,  OnDgnCheck7109)
	ON_BN_CLICKED(IDC_DGN_CHECK_7110,  OnDgnCheck7110)
	ON_BN_CLICKED(IDC_DGN_CHECK_7111,  OnDgnCheck7111)
	ON_BN_CLICKED(IDC_DGN_CHECK_7112,  OnDgnCheck7112)
	ON_BN_CLICKED(IDC_DGN_CHECK_7113,  OnDgnCheck7113)

	//}}AFX_MSG_MAP
	END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConBarSizeDlg message handlers

void CDgnConBarSizeDlg::OnOK() 
{
	// 선택된 철근직경을 문자로 만듭니다.
	m_TotalReBarSize = Get_SelectReBarSize();
	// Add by ZINU.('05.01.04). Check No Selected Item.
	if(m_TotalReBarSize==_T(""))
	{
		AfxMessageBox(_LS(IDS_DGN_CHK_SEL_ITEM), MB_OK);
		return;
	}
	CDialogMove::OnOK();
}

void CDgnConBarSizeDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}

CString CDgnConBarSizeDlg::Get_SelectReBarSize()
{
	UpdateData(TRUE);

    CMatlDB* pMatlDB = CDBDoc::GetDocPoint()->m_pMatlDB;

	CStringArray arRbar11; pMatlDB->GetRebarNameList(EN_BAR_KS,      arRbar11);
	CStringArray arRbar12; pMatlDB->GetRebarNameList(EN_BAR_CNS,     arRbar12);
	CStringArray arRbar13; pMatlDB->GetRebarNameList(EN_BAR_JIS,     arRbar13);
	CStringArray arRbar14; pMatlDB->GetRebarNameList(EN_BAR_PNS49,   arRbar14);
	CStringArray arRbar21; pMatlDB->GetRebarNameList(EN_BAR_ASTM,    arRbar21);
	CStringArray arRbar31; pMatlDB->GetRebarNameList(EN_BAR_BSEN,    arRbar31);
	CStringArray arRbar32; pMatlDB->GetRebarNameList(EN_BAR_IS,      arRbar32);
	CStringArray arRbar33; pMatlDB->GetRebarNameList(EN_BAR_UNI,     arRbar33);
	CStringArray arRbar34; pMatlDB->GetRebarNameList(EN_BAR_SS,      arRbar34);
	CStringArray arRbar41; pMatlDB->GetRebarNameList(EN_BAR_GB,      arRbar41);
	CStringArray arRbar51; pMatlDB->GetRebarNameList(EN_BAR_CSA,     arRbar51);
	CStringArray arRbar61; pMatlDB->GetRebarNameList(EN_BAR_GOST_SP, arRbar61);
	CStringArray arRbar71; pMatlDB->GetRebarNameList(EN_BAR_AS,			arRbar71);

	CString strSize=_T("");	
	for(int i=0; i<CONST_BAR_KS  ;   i++)	strSize = strSize + (m_bRbar11[i] ? arRbar11[i]+_T(",") : _T(""));
	for(int i=0; i<CONST_BAR_CNS ;   i++)	strSize = strSize + (m_bRbar12[i] ? arRbar12[i]+_T(",") : _T(""));
	for(int i=0; i<CONST_BAR_JIS ;   i++)	strSize = strSize + (m_bRbar13[i] ? arRbar13[i]+_T(",") : _T(""));
	for(int i=0; i<CONST_BAR_PNS49;  i++)	strSize = strSize + (m_bRbar14[i] ? arRbar14[i]+_T(",") : _T(""));
	for(int i=0; i<CONST_BAR_ASTM;   i++)	strSize = strSize + (m_bRbar21[i] ? arRbar21[i]+_T(",") : _T(""));
	for(int i=0; i<CONST_BAR_BSEN;   i++)	strSize = strSize + (m_bRbar31[i] ? arRbar31[i]+_T(",") : _T(""));
	for(int i=0; i<CONST_BAR_IS  ;   i++)	strSize = strSize + (m_bRbar32[i] ? arRbar32[i]+_T(",") : _T(""));
	for(int i=0; i<CONST_BAR_UNI ;   i++)	strSize = strSize + (m_bRbar33[i] ? arRbar33[i]+_T(",") : _T(""));
	for(int i=0; i<CONST_BAR_SS  ;   i++)	strSize = strSize + (m_bRbar34[i] ? arRbar34[i]+_T(",") : _T(""));
	for(int i=0; i<CONST_BAR_GB  ;   i++)	strSize = strSize + (m_bRbar41[i] ? arRbar41[i]+_T(",") : _T(""));
	for(int i=0; i<CONST_BAR_CSA ;   i++)	strSize = strSize + (m_bRbar51[i] ? arRbar51[i]+_T(",") : _T(""));
	for(int i=0; i<CONST_BAR_GOST_SP;i++)	strSize = strSize + (m_bRbar61[i] ? arRbar61[i]+_T(",") : _T(""));
	for(int i=0; i<CONST_BAR_AS;		 i++)	strSize = strSize + (m_bRbar71[i] ? arRbar71[i]+_T(",") : _T(""));

	int iLen = strSize.GetLength();
	CString strTotSize=_T("");
	if(iLen > 0)	strTotSize = strSize.Left(iLen-1);
	else					strTotSize = _T("");

	return strTotSize;
}

BOOL CDgnConBarSizeDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	for(int j=0; j<CONST_BAR_KS;      j++)	m_bRbar11[j] = FALSE;
	for(int j=0; j<CONST_BAR_CNS;     j++)	m_bRbar12[j] = FALSE;
	for(int j=0; j<CONST_BAR_JIS;     j++)	m_bRbar13[j] = FALSE;
	for(int j=0; j<CONST_BAR_PNS49;   j++)	m_bRbar14[j] = FALSE;
	for(int j=0; j<CONST_BAR_ASTM;    j++)	m_bRbar21[j] = FALSE;
	for(int j=0; j<CONST_BAR_BSEN;    j++)	m_bRbar31[j] = FALSE;
	for(int j=0; j<CONST_BAR_IS;      j++)	m_bRbar32[j] = FALSE;
	for(int j=0; j<CONST_BAR_UNI;     j++)	m_bRbar33[j] = FALSE;
	for(int j=0; j<CONST_BAR_SS;      j++)	m_bRbar34[j] = FALSE;
	for(int j=0; j<CONST_BAR_GB;      j++)	m_bRbar41[j] = FALSE;
	for(int j=0; j<CONST_BAR_CSA;     j++)	m_bRbar51[j] = FALSE;
	for(int j=0; j<CONST_BAR_GOST_SP; j++)	m_bRbar61[j] = FALSE;
	for(int j=0; j<CONST_BAR_AS;			j++)	m_bRbar71[j] = FALSE;
	
    CMatlDB* pMatlDB = CDBDoc::GetDocPoint()->m_pMatlDB;

	// TODO: Add extra initialization here
	if(m_TotalReBarSize!=_T(""))
	{
		CStringArray arRbar11; pMatlDB->GetRebarNameList(EN_BAR_KS,   arRbar11);
		CStringArray arRbar12; pMatlDB->GetRebarNameList(EN_BAR_CNS,  arRbar12);
		CStringArray arRbar13; pMatlDB->GetRebarNameList(EN_BAR_JIS,  arRbar13);
		CStringArray arRbar14; pMatlDB->GetRebarNameList(EN_BAR_PNS49,arRbar14);
		CStringArray arRbar21; pMatlDB->GetRebarNameList(EN_BAR_ASTM, arRbar21);
		CStringArray arRbar31; pMatlDB->GetRebarNameList(EN_BAR_BSEN, arRbar31);
		CStringArray arRbar32; pMatlDB->GetRebarNameList(EN_BAR_IS,   arRbar32);
		CStringArray arRbar33; pMatlDB->GetRebarNameList(EN_BAR_UNI,  arRbar33);
		CStringArray arRbar34; pMatlDB->GetRebarNameList(EN_BAR_SS,   arRbar34);
		CStringArray arRbar41; pMatlDB->GetRebarNameList(EN_BAR_GB,   arRbar41);
		CStringArray arRbar51; pMatlDB->GetRebarNameList(EN_BAR_CSA,  arRbar51);
		CStringArray arRbar61; pMatlDB->GetRebarNameList(EN_BAR_GOST_SP,  arRbar61);
		CStringArray arRbar71; pMatlDB->GetRebarNameList(EN_BAR_AS,  arRbar71);

		CArray<CString,CString> ReBarSize;
		BOOL bCheck = Cutting_RebarSizeString(m_TotalReBarSize, ReBarSize);
		ASSERT(bCheck);
		m_CheckCount = ReBarSize.GetSize();
		for(int i=0; i<m_CheckCount; i++)
		{
			CString strSize = ReBarSize.GetAt(i);
			
			for(int j=0; j<CONST_BAR_KS;      j++)	m_bRbar11[j] = ((strSize==arRbar11[j] || m_bRbar11[j]==TRUE) ? TRUE : FALSE);
			for(int j=0; j<CONST_BAR_CNS;     j++)	m_bRbar12[j] = ((strSize==arRbar12[j] || m_bRbar12[j]==TRUE) ? TRUE : FALSE);
			for(int j=0; j<CONST_BAR_JIS;     j++)	m_bRbar13[j] = ((strSize==arRbar13[j] || m_bRbar13[j]==TRUE) ? TRUE : FALSE);
			for(int j=0; j<CONST_BAR_PNS49;   j++)	m_bRbar14[j] = ((strSize==arRbar14[j] || m_bRbar14[j]==TRUE) ? TRUE : FALSE);
			for(int j=0; j<CONST_BAR_ASTM;    j++)	m_bRbar21[j] = ((strSize==arRbar21[j] || m_bRbar21[j]==TRUE) ? TRUE : FALSE);
			for(int j=0; j<CONST_BAR_BSEN;    j++)	m_bRbar31[j] = ((strSize==arRbar31[j] || m_bRbar31[j]==TRUE) ? TRUE : FALSE);
			for(int j=0; j<CONST_BAR_IS;      j++)	m_bRbar32[j] = ((strSize==arRbar32[j] || m_bRbar32[j]==TRUE) ? TRUE : FALSE);
			for(int j=0; j<CONST_BAR_UNI;     j++)	m_bRbar33[j] = ((strSize==arRbar33[j] || m_bRbar33[j]==TRUE) ? TRUE : FALSE);
			for(int j=0; j<CONST_BAR_SS;      j++)	m_bRbar34[j] = ((strSize==arRbar34[j] || m_bRbar34[j]==TRUE) ? TRUE : FALSE);
			for(int j=0; j<CONST_BAR_GB;      j++)	m_bRbar41[j] = ((strSize==arRbar41[j] || m_bRbar41[j]==TRUE) ? TRUE : FALSE);
			for(int j=0; j<CONST_BAR_CSA;     j++)	m_bRbar51[j] = ((strSize==arRbar51[j] || m_bRbar51[j]==TRUE) ? TRUE : FALSE);
			for(int j=0; j<CONST_BAR_GOST_SP; j++)	m_bRbar61[j] = ((strSize==arRbar61[j] || m_bRbar61[j]==TRUE) ? TRUE : FALSE);
			for(int j=0; j<CONST_BAR_AS;	  j++)	m_bRbar71[j] = ((strSize==arRbar71[j] || m_bRbar71[j]==TRUE) ? TRUE : FALSE);

		}
		Show_RebarKind();
		UpdateData(FALSE);
	}
	return TRUE;
}

BOOL CDgnConBarSizeDlg::Check_SizeData(BOOL& bSize)
{
	UpdateData(TRUE);
	BOOL bChkOK = FALSE;
	if(bSize)	m_CheckCount++;
	else			m_CheckCount--;
	if(m_CheckCount > 5) 
	{
		m_CheckCount--;
		bSize = FALSE;
		UpdateData(FALSE);
		AfxMessageBox(_LS(IDS_DGN_CHK_SEL_OVER_ITEM), MB_OK);
	}
	else bChkOK = TRUE;
	return bChkOK;
}

void CDgnConBarSizeDlg::OnDgnCheck1101()	{Check_SizeData(m_bRbar11[ 0]);}
void CDgnConBarSizeDlg::OnDgnCheck1102()	{Check_SizeData(m_bRbar11[ 1]);}
void CDgnConBarSizeDlg::OnDgnCheck1103()	{Check_SizeData(m_bRbar11[ 2]);}
void CDgnConBarSizeDlg::OnDgnCheck1104()	{Check_SizeData(m_bRbar11[ 3]);}
void CDgnConBarSizeDlg::OnDgnCheck1105()	{Check_SizeData(m_bRbar11[ 4]);}
void CDgnConBarSizeDlg::OnDgnCheck1106()	{Check_SizeData(m_bRbar11[ 5]);}
void CDgnConBarSizeDlg::OnDgnCheck1107()	{Check_SizeData(m_bRbar11[ 6]);}
void CDgnConBarSizeDlg::OnDgnCheck1108()	{Check_SizeData(m_bRbar11[ 7]);}
void CDgnConBarSizeDlg::OnDgnCheck1109()	{Check_SizeData(m_bRbar11[ 8]);}
void CDgnConBarSizeDlg::OnDgnCheck1110()	{Check_SizeData(m_bRbar11[ 9]);}
void CDgnConBarSizeDlg::OnDgnCheck1111()	{Check_SizeData(m_bRbar11[10]);}
void CDgnConBarSizeDlg::OnDgnCheck1112()	{Check_SizeData(m_bRbar11[11]);}
void CDgnConBarSizeDlg::OnDgnCheck1113()	{Check_SizeData(m_bRbar11[12]);}
void CDgnConBarSizeDlg::OnDgnCheck1114()	{Check_SizeData(m_bRbar11[13]);}
void CDgnConBarSizeDlg::OnDgnCheck1115()	{Check_SizeData(m_bRbar11[14]);}

void CDgnConBarSizeDlg::OnDgnCheck1201()	{Check_SizeData(m_bRbar12[ 0]);}
void CDgnConBarSizeDlg::OnDgnCheck1202()	{Check_SizeData(m_bRbar12[ 1]);}
void CDgnConBarSizeDlg::OnDgnCheck1203()	{Check_SizeData(m_bRbar12[ 2]);}
void CDgnConBarSizeDlg::OnDgnCheck1204()	{Check_SizeData(m_bRbar12[ 3]);}
void CDgnConBarSizeDlg::OnDgnCheck1205()	{Check_SizeData(m_bRbar12[ 4]);}
void CDgnConBarSizeDlg::OnDgnCheck1206()	{Check_SizeData(m_bRbar12[ 5]);}
void CDgnConBarSizeDlg::OnDgnCheck1207()	{Check_SizeData(m_bRbar12[ 6]);}
void CDgnConBarSizeDlg::OnDgnCheck1208()	{Check_SizeData(m_bRbar12[ 7]);}
void CDgnConBarSizeDlg::OnDgnCheck1209()	{Check_SizeData(m_bRbar12[ 8]);}
void CDgnConBarSizeDlg::OnDgnCheck1210()	{Check_SizeData(m_bRbar12[ 9]);}
void CDgnConBarSizeDlg::OnDgnCheck1211()	{Check_SizeData(m_bRbar12[10]);}
void CDgnConBarSizeDlg::OnDgnCheck1212()	{Check_SizeData(m_bRbar12[11]);}
void CDgnConBarSizeDlg::OnDgnCheck1213()	{Check_SizeData(m_bRbar12[12]);}

void CDgnConBarSizeDlg::OnDgnCheck1301()	{Check_SizeData(m_bRbar13[ 0]);}
void CDgnConBarSizeDlg::OnDgnCheck1302()	{Check_SizeData(m_bRbar13[ 1]);}
void CDgnConBarSizeDlg::OnDgnCheck1303()	{Check_SizeData(m_bRbar13[ 2]);}
void CDgnConBarSizeDlg::OnDgnCheck1304()	{Check_SizeData(m_bRbar13[ 3]);}
void CDgnConBarSizeDlg::OnDgnCheck1305()	{Check_SizeData(m_bRbar13[ 4]);}
void CDgnConBarSizeDlg::OnDgnCheck1306()	{Check_SizeData(m_bRbar13[ 5]);}
void CDgnConBarSizeDlg::OnDgnCheck1307()	{Check_SizeData(m_bRbar13[ 6]);}
void CDgnConBarSizeDlg::OnDgnCheck1308()	{Check_SizeData(m_bRbar13[ 7]);}
void CDgnConBarSizeDlg::OnDgnCheck1309()	{Check_SizeData(m_bRbar13[ 8]);}
void CDgnConBarSizeDlg::OnDgnCheck1310()	{Check_SizeData(m_bRbar13[ 9]);}
void CDgnConBarSizeDlg::OnDgnCheck1311()	{Check_SizeData(m_bRbar13[10]);}
void CDgnConBarSizeDlg::OnDgnCheck1312()	{Check_SizeData(m_bRbar13[11]);}
void CDgnConBarSizeDlg::OnDgnCheck1313()	{Check_SizeData(m_bRbar13[12]);}

void CDgnConBarSizeDlg::OnDgnCheck1401()    {Check_SizeData(m_bRbar14[0]);}
void CDgnConBarSizeDlg::OnDgnCheck1402()    {Check_SizeData(m_bRbar14[1]);}
void CDgnConBarSizeDlg::OnDgnCheck1403()    {Check_SizeData(m_bRbar14[2]);}
void CDgnConBarSizeDlg::OnDgnCheck1404()    {Check_SizeData(m_bRbar14[3]);}
void CDgnConBarSizeDlg::OnDgnCheck1405()    {Check_SizeData(m_bRbar14[4]);}
void CDgnConBarSizeDlg::OnDgnCheck1406()    {Check_SizeData(m_bRbar14[5]);}
void CDgnConBarSizeDlg::OnDgnCheck1407()    {Check_SizeData(m_bRbar14[6]);}
void CDgnConBarSizeDlg::OnDgnCheck1408()    {Check_SizeData(m_bRbar14[7]);}
void CDgnConBarSizeDlg::OnDgnCheck1409()    {Check_SizeData(m_bRbar14[8]);}
void CDgnConBarSizeDlg::OnDgnCheck1410()    {Check_SizeData(m_bRbar14[9]);}

void CDgnConBarSizeDlg::OnDgnCheck2101()	{Check_SizeData(m_bRbar21[ 0]);}
void CDgnConBarSizeDlg::OnDgnCheck2102()	{Check_SizeData(m_bRbar21[ 1]);}
void CDgnConBarSizeDlg::OnDgnCheck2103()	{Check_SizeData(m_bRbar21[ 2]);}
void CDgnConBarSizeDlg::OnDgnCheck2104()	{Check_SizeData(m_bRbar21[ 3]);}
void CDgnConBarSizeDlg::OnDgnCheck2105()	{Check_SizeData(m_bRbar21[ 4]);}
void CDgnConBarSizeDlg::OnDgnCheck2106()	{Check_SizeData(m_bRbar21[ 5]);}
void CDgnConBarSizeDlg::OnDgnCheck2107()	{Check_SizeData(m_bRbar21[ 6]);}
void CDgnConBarSizeDlg::OnDgnCheck2108()	{Check_SizeData(m_bRbar21[ 7]);}
void CDgnConBarSizeDlg::OnDgnCheck2109()	{Check_SizeData(m_bRbar21[ 8]);}
void CDgnConBarSizeDlg::OnDgnCheck2110()	{Check_SizeData(m_bRbar21[ 9]);}
void CDgnConBarSizeDlg::OnDgnCheck2111()	{Check_SizeData(m_bRbar21[10]);}

void CDgnConBarSizeDlg::OnDgnCheck3101()	{Check_SizeData(m_bRbar31[ 0]);}
void CDgnConBarSizeDlg::OnDgnCheck3102()	{Check_SizeData(m_bRbar31[ 1]);}
void CDgnConBarSizeDlg::OnDgnCheck3103()	{Check_SizeData(m_bRbar31[ 2]);}
void CDgnConBarSizeDlg::OnDgnCheck3104()	{Check_SizeData(m_bRbar31[ 3]);}
void CDgnConBarSizeDlg::OnDgnCheck3105()	{Check_SizeData(m_bRbar31[ 4]);}
void CDgnConBarSizeDlg::OnDgnCheck3106()	{Check_SizeData(m_bRbar31[ 5]);}
void CDgnConBarSizeDlg::OnDgnCheck3107()	{Check_SizeData(m_bRbar31[ 6]);}
void CDgnConBarSizeDlg::OnDgnCheck3108()	{Check_SizeData(m_bRbar31[ 7]);}
void CDgnConBarSizeDlg::OnDgnCheck3109()	{Check_SizeData(m_bRbar31[ 8]);}
void CDgnConBarSizeDlg::OnDgnCheck3110()	{Check_SizeData(m_bRbar31[ 9]);}
void CDgnConBarSizeDlg::OnDgnCheck3111()	{Check_SizeData(m_bRbar31[10]);}
void CDgnConBarSizeDlg::OnDgnCheck3112()	{Check_SizeData(m_bRbar31[11]);}
void CDgnConBarSizeDlg::OnDgnCheck3113()	{Check_SizeData(m_bRbar31[12]);}
void CDgnConBarSizeDlg::OnDgnCheck3114()	{Check_SizeData(m_bRbar31[13]);}

void CDgnConBarSizeDlg::OnDgnCheck3201()	{Check_SizeData(m_bRbar32[ 0]);}
void CDgnConBarSizeDlg::OnDgnCheck3202()	{Check_SizeData(m_bRbar32[ 1]);}
void CDgnConBarSizeDlg::OnDgnCheck3203()	{Check_SizeData(m_bRbar32[ 2]);}
void CDgnConBarSizeDlg::OnDgnCheck3204()	{Check_SizeData(m_bRbar32[ 3]);}
void CDgnConBarSizeDlg::OnDgnCheck3205()	{Check_SizeData(m_bRbar32[ 4]);}
void CDgnConBarSizeDlg::OnDgnCheck3206()	{Check_SizeData(m_bRbar32[ 5]);}
void CDgnConBarSizeDlg::OnDgnCheck3207()	{Check_SizeData(m_bRbar32[ 6]);}
void CDgnConBarSizeDlg::OnDgnCheck3208()	{Check_SizeData(m_bRbar32[ 7]);}
void CDgnConBarSizeDlg::OnDgnCheck3209()	{Check_SizeData(m_bRbar32[ 8]);}
void CDgnConBarSizeDlg::OnDgnCheck3210()	{Check_SizeData(m_bRbar32[ 9]);}
void CDgnConBarSizeDlg::OnDgnCheck3211()	{Check_SizeData(m_bRbar32[10]);}
void CDgnConBarSizeDlg::OnDgnCheck3212()	{Check_SizeData(m_bRbar32[11]);}
void CDgnConBarSizeDlg::OnDgnCheck3213()	{Check_SizeData(m_bRbar32[12]);}

void CDgnConBarSizeDlg::OnDgnCheck3301()	{Check_SizeData(m_bRbar33[ 0]);}
void CDgnConBarSizeDlg::OnDgnCheck3302()	{Check_SizeData(m_bRbar33[ 1]);}
void CDgnConBarSizeDlg::OnDgnCheck3303()	{Check_SizeData(m_bRbar33[ 2]);}
void CDgnConBarSizeDlg::OnDgnCheck3304()	{Check_SizeData(m_bRbar33[ 3]);}
void CDgnConBarSizeDlg::OnDgnCheck3305()	{Check_SizeData(m_bRbar33[ 4]);}
void CDgnConBarSizeDlg::OnDgnCheck3306()	{Check_SizeData(m_bRbar33[ 5]);}
void CDgnConBarSizeDlg::OnDgnCheck3307()	{Check_SizeData(m_bRbar33[ 6]);}
void CDgnConBarSizeDlg::OnDgnCheck3308()	{Check_SizeData(m_bRbar33[ 7]);}
void CDgnConBarSizeDlg::OnDgnCheck3309()	{Check_SizeData(m_bRbar33[ 8]);}
void CDgnConBarSizeDlg::OnDgnCheck3310()	{Check_SizeData(m_bRbar33[ 9]);}
void CDgnConBarSizeDlg::OnDgnCheck3311()	{Check_SizeData(m_bRbar33[10]);}
void CDgnConBarSizeDlg::OnDgnCheck3312()	{Check_SizeData(m_bRbar33[11]);}
void CDgnConBarSizeDlg::OnDgnCheck3313()	{Check_SizeData(m_bRbar33[12]);}
void CDgnConBarSizeDlg::OnDgnCheck3314()	{Check_SizeData(m_bRbar33[13]);}
void CDgnConBarSizeDlg::OnDgnCheck3315()	{Check_SizeData(m_bRbar33[14]);}
void CDgnConBarSizeDlg::OnDgnCheck3316()	{Check_SizeData(m_bRbar33[15]);}
void CDgnConBarSizeDlg::OnDgnCheck3317()	{Check_SizeData(m_bRbar33[16]);}

void CDgnConBarSizeDlg::OnDgnCheck3401()	{Check_SizeData(m_bRbar34[ 0]);}
void CDgnConBarSizeDlg::OnDgnCheck3402()	{Check_SizeData(m_bRbar34[ 1]);}
void CDgnConBarSizeDlg::OnDgnCheck3403()	{Check_SizeData(m_bRbar34[ 2]);}
void CDgnConBarSizeDlg::OnDgnCheck3404()	{Check_SizeData(m_bRbar34[ 3]);}
void CDgnConBarSizeDlg::OnDgnCheck3405()	{Check_SizeData(m_bRbar34[ 4]);}
void CDgnConBarSizeDlg::OnDgnCheck3406()	{Check_SizeData(m_bRbar34[ 5]);}
void CDgnConBarSizeDlg::OnDgnCheck3407()	{Check_SizeData(m_bRbar34[ 6]);}
void CDgnConBarSizeDlg::OnDgnCheck3408()	{Check_SizeData(m_bRbar34[ 7]);}
void CDgnConBarSizeDlg::OnDgnCheck3409()	{Check_SizeData(m_bRbar34[ 8]);}
void CDgnConBarSizeDlg::OnDgnCheck3410()	{Check_SizeData(m_bRbar34[ 9]);}
void CDgnConBarSizeDlg::OnDgnCheck3411()	{Check_SizeData(m_bRbar34[10]);}
void CDgnConBarSizeDlg::OnDgnCheck3412()	{Check_SizeData(m_bRbar34[11]);}
void CDgnConBarSizeDlg::OnDgnCheck3413()	{Check_SizeData(m_bRbar34[12]);}
void CDgnConBarSizeDlg::OnDgnCheck3414()	{Check_SizeData(m_bRbar34[13]);}

void CDgnConBarSizeDlg::OnDgnCheck4101()	{Check_SizeData(m_bRbar41[ 0]);}
void CDgnConBarSizeDlg::OnDgnCheck4102()	{Check_SizeData(m_bRbar41[ 1]);}
void CDgnConBarSizeDlg::OnDgnCheck4103()	{Check_SizeData(m_bRbar41[ 2]);}
void CDgnConBarSizeDlg::OnDgnCheck4104()	{Check_SizeData(m_bRbar41[ 3]);}
void CDgnConBarSizeDlg::OnDgnCheck4105()	{Check_SizeData(m_bRbar41[ 4]);}
void CDgnConBarSizeDlg::OnDgnCheck4106()	{Check_SizeData(m_bRbar41[ 5]);}
void CDgnConBarSizeDlg::OnDgnCheck4107()	{Check_SizeData(m_bRbar41[ 6]);}
void CDgnConBarSizeDlg::OnDgnCheck4108()	{Check_SizeData(m_bRbar41[ 7]);}
void CDgnConBarSizeDlg::OnDgnCheck4109()	{Check_SizeData(m_bRbar41[ 8]);}
void CDgnConBarSizeDlg::OnDgnCheck4110()	{Check_SizeData(m_bRbar41[ 9]);}
void CDgnConBarSizeDlg::OnDgnCheck4111()	{Check_SizeData(m_bRbar41[10]);}
void CDgnConBarSizeDlg::OnDgnCheck4112()	{Check_SizeData(m_bRbar41[11]);}
void CDgnConBarSizeDlg::OnDgnCheck4113()	{Check_SizeData(m_bRbar41[12]);}
void CDgnConBarSizeDlg::OnDgnCheck4114()	{Check_SizeData(m_bRbar41[13]);}
void CDgnConBarSizeDlg::OnDgnCheck4115()	{Check_SizeData(m_bRbar41[14]);}
void CDgnConBarSizeDlg::OnDgnCheck4116()	{Check_SizeData(m_bRbar41[15]);}

void CDgnConBarSizeDlg::OnDgnCheck5101()	{Check_SizeData(m_bRbar51[ 0]);}
void CDgnConBarSizeDlg::OnDgnCheck5102()	{Check_SizeData(m_bRbar51[ 1]);}
void CDgnConBarSizeDlg::OnDgnCheck5103()	{Check_SizeData(m_bRbar51[ 2]);}
void CDgnConBarSizeDlg::OnDgnCheck5104()	{Check_SizeData(m_bRbar51[ 3]);}
void CDgnConBarSizeDlg::OnDgnCheck5105()	{Check_SizeData(m_bRbar51[ 4]);}
void CDgnConBarSizeDlg::OnDgnCheck5106()	{Check_SizeData(m_bRbar51[ 5]);}
void CDgnConBarSizeDlg::OnDgnCheck5107()	{Check_SizeData(m_bRbar51[ 6]);}
void CDgnConBarSizeDlg::OnDgnCheck5108()	{Check_SizeData(m_bRbar51[ 7]);}

void CDgnConBarSizeDlg::OnDgnCheck6101()	{Check_SizeData(m_bRbar61[ 0]);}
void CDgnConBarSizeDlg::OnDgnCheck6102()	{Check_SizeData(m_bRbar61[ 1]);}
void CDgnConBarSizeDlg::OnDgnCheck6103()	{Check_SizeData(m_bRbar61[ 2]);}
void CDgnConBarSizeDlg::OnDgnCheck6104()	{Check_SizeData(m_bRbar61[ 3]);}
void CDgnConBarSizeDlg::OnDgnCheck6105()	{Check_SizeData(m_bRbar61[ 4]);}
void CDgnConBarSizeDlg::OnDgnCheck6106()	{Check_SizeData(m_bRbar61[ 5]);}
void CDgnConBarSizeDlg::OnDgnCheck6107()	{Check_SizeData(m_bRbar61[ 6]);}
void CDgnConBarSizeDlg::OnDgnCheck6108()	{Check_SizeData(m_bRbar61[ 7]);}
void CDgnConBarSizeDlg::OnDgnCheck6109()	{Check_SizeData(m_bRbar61[ 8]);}
void CDgnConBarSizeDlg::OnDgnCheck6110()	{Check_SizeData(m_bRbar61[ 9]);}
void CDgnConBarSizeDlg::OnDgnCheck6111()	{Check_SizeData(m_bRbar61[10]);}
void CDgnConBarSizeDlg::OnDgnCheck6112()	{Check_SizeData(m_bRbar61[11]);}
void CDgnConBarSizeDlg::OnDgnCheck6113()	{Check_SizeData(m_bRbar61[12]);}
void CDgnConBarSizeDlg::OnDgnCheck6114()	{Check_SizeData(m_bRbar61[13]);}

void CDgnConBarSizeDlg::OnDgnCheck7101()	{Check_SizeData(m_bRbar71[ 0]);}
void CDgnConBarSizeDlg::OnDgnCheck7102()	{Check_SizeData(m_bRbar71[ 1]);}
void CDgnConBarSizeDlg::OnDgnCheck7103()	{Check_SizeData(m_bRbar71[ 2]);}
void CDgnConBarSizeDlg::OnDgnCheck7104()	{Check_SizeData(m_bRbar71[ 3]);}
void CDgnConBarSizeDlg::OnDgnCheck7105()	{Check_SizeData(m_bRbar71[ 4]);}
void CDgnConBarSizeDlg::OnDgnCheck7106()	{Check_SizeData(m_bRbar71[ 5]);}
void CDgnConBarSizeDlg::OnDgnCheck7107()	{Check_SizeData(m_bRbar71[ 6]);}
void CDgnConBarSizeDlg::OnDgnCheck7108()	{Check_SizeData(m_bRbar71[ 7]);}
void CDgnConBarSizeDlg::OnDgnCheck7109()	{Check_SizeData(m_bRbar71[ 8]);}
void CDgnConBarSizeDlg::OnDgnCheck7110()	{Check_SizeData(m_bRbar71[ 9]);}
void CDgnConBarSizeDlg::OnDgnCheck7111()	{Check_SizeData(m_bRbar71[10]);}
void CDgnConBarSizeDlg::OnDgnCheck7112()	{Check_SizeData(m_bRbar71[11]);}
void CDgnConBarSizeDlg::OnDgnCheck7113()	{Check_SizeData(m_bRbar71[12]);}

BOOL CDgnConBarSizeDlg::Cutting_RebarSizeString(CString str, CArray<CString,CString>& BarSize)
{
	BOOL bCheck=TRUE;

	TCHAR ch;
	CString temp=_T("");
	int iLength = str.GetLength();
	for(int i=0; i<iLength; i++)
	{
		ch = str.GetAt(i);
		if(ch==',' || i+1==iLength)
		{
			if(ch==',') BarSize.Add(temp);
			else			  BarSize.Add(temp+ch);
			temp=_T("");
		}
		else	temp = temp+ch;
	}
	return bCheck;
}

BOOL CDgnConBarSizeDlg::Show_RebarKind()
{
	int iME = CDBDoc::GetDocPoint()->m_pMatlDB->Get_RbarKindCon();
	if(iME < 11 || iME > 71)	return FALSE;

	CArray<UINT,UINT> arRbar11;  IDRBAR_11(01)IDRBAR_11(02)IDRBAR_11(03)IDRBAR_11(04)IDRBAR_11(05)IDRBAR_11(06)IDRBAR_11(07)IDRBAR_11(08)IDRBAR_11(09)IDRBAR_11(10)IDRBAR_11(11)IDRBAR_11(12)IDRBAR_11(13)IDRBAR_11(14)IDRBAR_11(15);
	CArray<UINT,UINT> arRbar12;  IDRBAR_12(01)IDRBAR_12(02)IDRBAR_12(03)IDRBAR_12(04)IDRBAR_12(05)IDRBAR_12(06)IDRBAR_12(07)IDRBAR_12(08)IDRBAR_12(09)IDRBAR_12(10)IDRBAR_12(11)IDRBAR_12(12)IDRBAR_12(13);
	CArray<UINT,UINT> arRbar13;  IDRBAR_13(01)IDRBAR_13(02)IDRBAR_13(03)IDRBAR_13(04)IDRBAR_13(05)IDRBAR_13(06)IDRBAR_13(07)IDRBAR_13(08)IDRBAR_13(09)IDRBAR_13(10)IDRBAR_13(11)IDRBAR_13(12)IDRBAR_13(13);
	CArray<UINT,UINT> arRbar14;  IDRBAR_14(01)IDRBAR_14(02)IDRBAR_14(03)IDRBAR_14(04)IDRBAR_14(05)IDRBAR_14(06)IDRBAR_14(07)IDRBAR_14(08)IDRBAR_14(09)IDRBAR_14(10);
	CArray<UINT,UINT> arRbar21;  IDRBAR_21(01)IDRBAR_21(02)IDRBAR_21(03)IDRBAR_21(04)IDRBAR_21(05)IDRBAR_21(06)IDRBAR_21(07)IDRBAR_21(08)IDRBAR_21(09)IDRBAR_21(10)IDRBAR_21(11);
	CArray<UINT,UINT> arRbar31;  IDRBAR_31(01)IDRBAR_31(02)IDRBAR_31(03)IDRBAR_31(04)IDRBAR_31(05)IDRBAR_31(06)IDRBAR_31(07)IDRBAR_31(08)IDRBAR_31(09)IDRBAR_31(10)IDRBAR_31(11)IDRBAR_31(12)IDRBAR_31(13)IDRBAR_31(14);
	CArray<UINT,UINT> arRbar32;  IDRBAR_32(01)IDRBAR_32(02)IDRBAR_32(03)IDRBAR_32(04)IDRBAR_32(05)IDRBAR_32(06)IDRBAR_32(07)IDRBAR_32(08)IDRBAR_32(09)IDRBAR_32(10)IDRBAR_32(11)IDRBAR_32(12)IDRBAR_32(13);
	CArray<UINT,UINT> arRbar33;  IDRBAR_33(01)IDRBAR_33(02)IDRBAR_33(03)IDRBAR_33(04)IDRBAR_33(05)IDRBAR_33(06)IDRBAR_33(07)IDRBAR_33(08)IDRBAR_33(09)IDRBAR_33(10)IDRBAR_33(11)IDRBAR_33(12)IDRBAR_33(13)IDRBAR_33(14)IDRBAR_33(15)IDRBAR_33(16)IDRBAR_33(17);
	CArray<UINT,UINT> arRbar34;  IDRBAR_34(01)IDRBAR_34(02)IDRBAR_34(03)IDRBAR_34(04)IDRBAR_34(05)IDRBAR_34(06)IDRBAR_34(07)IDRBAR_34(08)IDRBAR_34(09)IDRBAR_34(10)IDRBAR_34(11)IDRBAR_34(12)IDRBAR_34(13)IDRBAR_34(14);
	CArray<UINT,UINT> arRbar41;  IDRBAR_41(01)IDRBAR_41(02)IDRBAR_41(03)IDRBAR_41(04)IDRBAR_41(05)IDRBAR_41(06)IDRBAR_41(07)IDRBAR_41(08)IDRBAR_41(09)IDRBAR_41(10)IDRBAR_41(11)IDRBAR_41(12)IDRBAR_41(13)IDRBAR_41(14)IDRBAR_41(15)IDRBAR_41(16);
	CArray<UINT,UINT> arRbar51;  IDRBAR_51(01)IDRBAR_51(02)IDRBAR_51(03)IDRBAR_51(04)IDRBAR_51(05)IDRBAR_51(06)IDRBAR_51(07)IDRBAR_51(08);
	CArray<UINT,UINT> arRbar61;  IDRBAR_61(01)IDRBAR_61(02)IDRBAR_61(03)IDRBAR_61(04)IDRBAR_61(05)IDRBAR_61(06)IDRBAR_61(07)IDRBAR_61(08);IDRBAR_61(09);IDRBAR_61(10);IDRBAR_61(11);IDRBAR_61(12);IDRBAR_61(13);IDRBAR_61(14);
	CArray<UINT,UINT> arRbar71;  IDRBAR_71(01)IDRBAR_71(02)IDRBAR_71(03)IDRBAR_71(04)IDRBAR_71(05)IDRBAR_71(06)IDRBAR_71(07)IDRBAR_71(08);IDRBAR_71(09);IDRBAR_71(10);IDRBAR_71(11);IDRBAR_71(12);IDRBAR_71(13);
	// Enable.	
	for(int i=0; i<CONST_BAR_KS;      i++)	GetDlgItem(arRbar11[i])->EnableWindow(TRUE);
	for(int i=0; i<CONST_BAR_CNS;     i++)	GetDlgItem(arRbar12[i])->EnableWindow(TRUE);
	for(int i=0; i<CONST_BAR_JIS;     i++)	GetDlgItem(arRbar13[i])->EnableWindow(TRUE);
	for(int i=0; i<CONST_BAR_PNS49;   i++)	GetDlgItem(arRbar14[i])->EnableWindow(TRUE);
	for(int i=0; i<CONST_BAR_ASTM;    i++)	GetDlgItem(arRbar21[i])->EnableWindow(TRUE);
	for(int i=0; i<CONST_BAR_BSEN;    i++)	GetDlgItem(arRbar31[i])->EnableWindow(TRUE);
	for(int i=0; i<CONST_BAR_IS;      i++)	GetDlgItem(arRbar32[i])->EnableWindow(TRUE);
	for(int i=0; i<CONST_BAR_UNI;     i++)	GetDlgItem(arRbar33[i])->EnableWindow(TRUE);
	for(int i=0; i<CONST_BAR_SS;      i++)	GetDlgItem(arRbar34[i])->EnableWindow(TRUE);
	for(int i=0; i<CONST_BAR_GB;      i++)	GetDlgItem(arRbar41[i])->EnableWindow(TRUE);
	for(int i=0; i<CONST_BAR_CSA;     i++)	GetDlgItem(arRbar51[i])->EnableWindow(TRUE);
	for(int i=0; i<CONST_BAR_GOST_SP; i++)	GetDlgItem(arRbar61[i])->EnableWindow(TRUE);
	for(int i=0; i<CONST_BAR_AS;	  i++)	GetDlgItem(arRbar71[i])->EnableWindow(TRUE);
	
	// Disable.
	if(iME!=EN_BAR_KS  )     { SetDisableCtrl(CONST_BAR_KS     , arRbar11, m_bRbar11); }
	if(iME!=EN_BAR_CNS && iME!=EN_BAR_CNS560_18 )
                             { SetDisableCtrl(CONST_BAR_CNS    , arRbar12, m_bRbar12); }	
	if(iME!=EN_BAR_JIS )     { SetDisableCtrl(CONST_BAR_JIS    , arRbar13, m_bRbar13); }
	if(iME!=EN_BAR_PNS49)    { SetDisableCtrl(CONST_BAR_PNS49  , arRbar14, m_bRbar14); }
	if(iME!=EN_BAR_ASTM)     { SetDisableCtrl(CONST_BAR_ASTM   , arRbar21, m_bRbar21); }
	if(iME!=EN_BAR_BSEN)     { SetDisableCtrl(CONST_BAR_BSEN   , arRbar31, m_bRbar31); }
	if(iME!=EN_BAR_IS  )     { SetDisableCtrl(CONST_BAR_IS     , arRbar32, m_bRbar32); }
	if(iME!=EN_BAR_UNI )     { SetDisableCtrl(CONST_BAR_UNI    , arRbar33, m_bRbar33); }
	if(iME!=EN_BAR_SS  )     { SetDisableCtrl(CONST_BAR_SS     , arRbar34, m_bRbar34); }
	if(iME!=EN_BAR_GB  )     { SetDisableCtrl(CONST_BAR_GB     , arRbar41, m_bRbar41); }
	if(iME!=EN_BAR_CSA )     { SetDisableCtrl(CONST_BAR_CSA    , arRbar51, m_bRbar51); }
	if(iME!=EN_BAR_GOST_SP && iME!=EN_BAR_GOST_SNIP )
							 { SetDisableCtrl(CONST_BAR_GOST_SP, arRbar61, m_bRbar61); }
	if(iME!=EN_BAR_AS )		 { SetDisableCtrl(CONST_BAR_AS		 , arRbar71, m_bRbar71); }

	return TRUE;
}

void CDgnConBarSizeDlg::SetDisableCtrl(int nSize, const CArray<UINT,UINT>& aRbarID, BOOL abRbar[])
{
	for(int i=0; i<nSize; ++i)
	{
		GetDlgItem(aRbarID[i])->EnableWindow(FALSE);
		abRbar[i] = FALSE;
	}
}

