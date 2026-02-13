// DgnJTG_D62_04_Seismic.h: interface for the CDgnJTG_D62_04_Seismic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGNJTG_D62_04_SEISMIC_H__C9905C02_06CE_4BC9_BEB4_CE6FC1A15089__INCLUDED_)
#define AFX_DGNJTG_D62_04_SEISMIC_H__C9905C02_06CE_4BC9_BEB4_CE6FC1A15089__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <afxtempl.h>
//#include "..\MIT_Lib\rptostream.h"
#include "math.h"
#include "CRCFileBase.h"
#include "..\wg_db\DB_ST_DN_RESULT_CH.H"
#include "RCDataCtrl_CH.h"
#include "RCImportFile_CH.h"

//////////////////////////////////////////////////////////////////////////
//Add by maxiao for china seismic design
struct _DGN_BENTCAP_D//bentCap
{
    UINT nElemK;
    UINT nPart;
    _DGN_RC_MATL_CH RcMatlCH;
	_DGN_RC_SECTION_CH SectionCH;
	double dL0;
    void Initialize()
    {
        nElemK=0;
        nPart =0;
		dL0 = 0;
		RcMatlCH.Initialize();
		SectionCH.Initialize();
    }
};

struct _DGN_FOUND_D//Foundation
{
    UINT nElemK;
    UINT nPart;
    void Initialize()
    {
        nElemK=0;
        nPart =0;
    }
};
struct _PAIR_PIER_D 
{

 double dPierAllowDisp;
 double dPc;
 double dDetd;
 void Initialize()
 {

     dPierAllowDisp =0;
     dPc = 0.0;
     dDetd = 0.0;
    }
};
struct _DGN_PIER_D//Pier
{
    UINT nElemK;
    UINT nPart;
    _DGN_RC_MATL_CH RcMatlCH;
	_DGN_RC_COLUMN_SECTION_CH SectionCH;

    _PAIR_PIER_D PairPierCH;
    void Initialize()
    {
        nElemK=0;
        nPart =0;
		RcMatlCH.Initialize();
		SectionCH.Initialize();
        PairPierCH.Initialize();
    }
};

struct _DGN_RUBBERTHICK_D//支座厚度
{
	CArray<T_SEIS_LINK_DISP,T_SEIS_LINK_DISP> aLinkDispList;
    void Initialize()
    {
		aLinkDispList.RemoveAll();
	}
};


struct _DGN_RUBBERSLIP_D//支座水平位移
{
	CArray<T_SEIS_LINK_FORCE,T_SEIS_LINK_FORCE> aLinkForceList;
	double dFricCoef;
    void Initialize()
    {
		aLinkForceList.RemoveAll();
		dFricCoef = .0;
	}
};

struct _DGN_DISP_D//桥墩位移
{
	double dT, dTg;
    double db, dh, dr, dL0,dDisp;
    double dfkh, dfc, dfy, dDs;
    double dPhiy, dPhiu;
    BOOL bDoublePier;
    void Initialize()
    {
    	dT = dTg = .0;
        db = dh = dr = dL0 = dDisp = .0;
        dfkh = dfc = dfy = dDs = .0;
        dPhiu = dPhiy = .0;
        bDoublePier = FALSE;
    }
};

class CDgnJTG_D62_04_Seismic : public CCRCFileBase  
{
public:
	CDgnJTG_D62_04_Seismic();
	virtual ~CDgnJTG_D62_04_Seismic();
    BOOL Design();
    BOOL Write_ImportData();
    BOOL MakeData();

protected:
    CDBDoc* m_pDoc;
    CRCDataCtrl_CH m_DataCtrl;
    //CCRCDataCtrl*  m_pDataCtrl;	 
	double m_dZero;

public:
    CArray<UINT, UINT> m_PierList;       //桥墩构件list
    CArray<UINT, UINT> m_AbutmentList;      //验算单元list
    CArray<UINT, UINT> m_FoundationList;      //验算单元list
    CArray<UINT, UINT> m_BentCapList;    //盖梁构件list

    CArray<UINT, UINT> m_RubberLinkList;//验算list
    CArray<UINT, UINT> m_ActiveLinkList;//验算list
    CArray<UINT, UINT> m_FixedLinkList;//验算list
    CArray<UINT, UINT> m_MainArchList;//验算list


	CStdioFile m_outfile; 
    int m_iTotLineNum;
    CArray<UINT,UINT> m_E1Pier_Comp_KList;        //E1桥墩、桥台单元强度  Abutment
    CArray<UINT,UINT> m_E1Arch_Cross_KList;       //E1主拱圈强度-抗弯 
    CArray<UINT,UINT> m_E1Arch_Comp_KList;        //E1主拱圈强度-偏压
    CArray<UINT,UINT> m_E1BentCap_Bend_KList;     //E1盖梁强度-抗弯
    CArray<UINT,UINT> m_E1BentCap_Shear_KList;    //E1盖梁强度-抗剪
    CArray<UINT,UINT> m_E1Foundation_KList;       //E1基础强度验算
    CArray<UINT,UINT> m_E1RubberThick_KList;      //E1支座厚度（板式橡胶）
    CArray<UINT,UINT> m_E1RubberSlip_KList;       //E1支座抗滑稳定性（板式橡胶）
    CArray<UINT,UINT> m_E1ActiveHoriDisp_KList;   //E1支座水平位移（盆式活动支座）
    CArray<UINT,UINT> m_E1StatHoriForce_KList;    //E1支座水平承载力（盆式固定支座）
    CArray<UINT,UINT> m_E2Pier_Comp_KList;            //E2桥墩、桥台单元强度
    CArray<UINT,UINT> m_E2Arch_Cross_KList;           //E2主拱圈强度-抗弯 
    CArray<UINT,UINT> m_E2Arch_Comp_KList;            //E2主拱圈强度-偏压
    CArray<UINT,UINT> m_E2BentCap_Bend_KList;         //E2盖梁强度-抗弯
    CArray<UINT,UINT> m_E2BentCap_Shear_KList;        //E2盖梁强度-抗剪
    CArray<UINT,UINT> m_E2Foundation_KList;           //E2基础强度验算
    CArray<UINT,UINT> m_E2Pier_Shear_KList;           //E2桥墩塑性铰区抗剪强度
    CArray<UINT,UINT> m_E2RubberThick_KList;          //E2支座厚度（板式橡胶）
    CArray<UINT,UINT> m_E2RubberSlip_KList;           //E2支座抗滑稳定性（板式橡胶）
    CArray<UINT,UINT> m_E2ActiveHoriDisp_KList;       //E2支座水平位移（盆式活动支座）
    CArray<UINT,UINT> m_E2StatHoriForce_KList;        //E2支座水平承载力（盆式固定支座）
    CArray<UINT,UINT> m_E2FrustaTopDisp_KList;        //E2墩顶位移（规则桥梁）
    CArray<UINT,UINT> m_E2CornerDisp_KList;           //E2塑形转角位移（非规则桥梁）

    CMap<UINT,UINT,UINT,UINT> m_DgnLcomKMap;
    CMap<_CRC_COMF_K, _CRC_COMF_K, _CRC_COMF_D, _CRC_COMF_D&> m_ComfDMap;

    T_RC_D_CH m_Rc_D_CH;
protected:
    HINSTANCE m_hDllCalc;
    CString Get_DllFileName();
	BOOL AfterCheck(BOOL bVersionOK, CString str, BOOL &bPrintTitle, BOOL bPrinti, T_DETAIL_STR DetailStr);
	BOOL GetPrintRpt(int iPrintRpt, int ij);

	//E1E2作用
	BOOL SeismicPierComp(BOOL bE1);          //桥墩单元强度
    BOOL SeismicAbutmentComp(BOOL bE1);      //桥台单元强度
    BOOL SeismicArchCross(BOOL bE1);         //主拱圈强度-抗弯 
    BOOL SeismicArchComp(BOOL bE1);          //主拱圈强度-偏压
    BOOL SeismicBentCapBend(BOOL bE1, BOOL bElastic);       //盖梁强度-抗弯SBBD_JT
    BOOL SeismicBentCapShear(BOOL bE1, BOOL bElastic);      //盖梁强度-抗剪SBSR_JT
    BOOL SeismicFoundation(BOOL bE1);        //基础强度验算SFCK_JT
	BOOL SeismicRubberThick(BOOL bE1);       //支座厚度（板式橡胶）
	BOOL SeismicRubberSlip(BOOL bE1);        //支座抗滑稳定性（板式橡胶）
	BOOL SeismicActiveHoriDisp(BOOL bE1);    //支座水平位移（盆式活动支座）
	BOOL SeismicStatHoriForce(BOOL bE1);     //支座水平承载力（盆式固定支座）
	//E2作用
    BOOL SeismicE2PierShear();               //桥墩塑性铰区抗剪强度//E2作用(弹塑性阶段)SPSR_JT
	BOOL SeismicE2FrustaTopDisp();           //墩顶位移（规则桥梁）SFTD_JT
	BOOL SeismicE2CornerDisp();              //塑形转角位移（非规则桥梁）SCND_JT

    double GetPierVc0(T_RSDL_K iMember, CArray <_DGN_FORC_CRC,_DGN_FORC_CRC>& aForceList, BOOL bLongitudinal, BOOL bOnlyBot, double dHn, int& iLcomK);//求延性柱剪力设计值值
    double GetBentCapVc0(double dL0,_DGN_RC_MATL_CH matli,_DGN_RC_MATL_CH matlj,_DGN_RC_SECTION_CH sectionMemberi,_DGN_RC_SECTION_CH sectionMemberj);
    //求盖梁相连柱Ms (最大值)
    double GetBentCapMs(CArray<UINT,UINT>& BentCapPierList);
    double GetBentCapMs_CJJ(BOOL bMax , CArray<UINT,UINT>& BentCapPierList);
    //求弯矩承载力
    double GetMn(_DGN_RC_MATL_CH RcMatlCH, _DGN_RC_SECTION_CH SectionCH); 
    //Get Phi-y Phi-u of a Pier section. B.0.1~2
//  BOOL Get_PhiyPhiuOfPier(int iElemk,int ij, double& Phiy, double& Phiu);
    //求theta-u 最不利时，对应的截面、Phi-y、Phi-u等信息
    BOOL GetThetauData(CArray<UINT, UINT>& ElemKList, double dH,double&db, double&dh, double&dr,double&dfkh, double&dfc, double&dfy, double&dDs, double&dPhiy, double&dPhiu);
};

#endif // !defined(AFX_DGNJTG_D62_04_SEISMIC_H__C9905C02_06CE_4BC9_BEB4_CE6FC1A15089__INCLUDED_) 
