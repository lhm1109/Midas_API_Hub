// DgnKSCE_USD03.h: interface for the CDgnKSCE_USD03 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGNKSCE_USD03_H__3E897861_48F0_4980_801F_F47764ED3C2E__INCLUDED_)
#define AFX_DGNKSCE_USD03_H__3E897861_48F0_4980_801F_F47764ED3C2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnPscCommon.h"
#include "PrtKSCE_USD03.h"

#define T_ALCR_K unsigned int

class CDgnKSCE_USD03 : public CDgnPscCommon
{
// Member Variables.
protected:
	CPrtKSCE_USD03 m_Print;
	double m_dZero;  

// Member Struct.
protected:

// Member Functions.
public:
	CDgnKSCE_USD03();
	virtual ~CDgnKSCE_USD03();
	void SetDataCtrlPointer(CCRCDataCtrl* pDataCtrl);

	CPrtKSCE_USD03* Get_PrtKscePointer();
	BOOL Get_KsceFysg(ElemPairK ElemK, T_FYSG_D& FysgD);					//  1. 가설중 휨 응력도 (FY_SG)
	BOOL Get_KsceFypc(T_TDNA_K TdnaK, T_FYPC_KSCE& FypcKsce);		//  2. 사용하중시 PC강재 최대응력도 (텐던별)
	BOOL Get_KsceFycm(ElemPairK ElemK, T_FYCM_D& FycmD);					//  3. 합성응력(FY_COM)
	BOOL Get_KsceBdcw(ElemPairK ElemK, T_BDCW_D& BdcwD);					//  4. 휨 균열폭 (BND_CW)
	BOOL Get_KsceTbar(ElemPairK ElemK, T_TBAR_D& TbarD, BOOL bI=TRUE, BOOL bJ=TRUE, int iUseKind=0);	//  5. 인장철근 (RBAR_T)
	BOOL Get_KsceBdcy(ElemPairK ElemK, T_BDCY_KSCE& BdcrKsce, BOOL bI=TRUE, BOOL bJ=TRUE);		//  6.   휨파괴안전도Y (BND_CR)
	BOOL Get_KsceBdcz(ElemPairK ElemK, T_BDCZ_KSCE& BdcrKsce);		//  6-1. 휨파괴안전도Z (BND_CR)
	BOOL Get_KsceCums(ElemPairK ElemK, T_CUMS_KSCE& CumsKsce, BOOL bI=TRUE, BOOL bJ=TRUE);		// 10,11. 계수하중시 전단설계(전단력 최대시)
	BOOL Get_KsceCrmt(ElemPairK ElemK, T_CRMT_KSCE& CrmtKsce, BOOL bI=TRUE, BOOL bJ=TRUE);		// 14,15. 계수하중시 비틀림설계(비틀림 최대시)
	BOOL Get_KsceTndn(ElemPairK ElemK, T_TNDN_KSCE& TndnKsce);
	BOOL Get_KsceFssg(ElemPairK ElemK, T_FPMT_JTG& FpmtKsce);
	BOOL Get_KsceFsms(ElemPairK ElemK, T_FPMS_JTG& FpmsJtg);
	BOOL Get_KsceFsmt(ElemPairK ElemK, T_FSMT_D& FsmtD);
	
	BOOL Get_BeforeAllowbleStressofConc(BOOL bPre, T_MATD_D& MatdD, T_SPSC_SECT& SpscSect, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbar, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndn,
																			T_FYSG_BASE FysgBase , double& dalComf, double& dalTenf);
	BOOL Get_AfterAllowbleStressofConc(BOOL bPre, T_MATD_D& MatdD, T_SPSC_SECT& SpscSect, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbar, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndn,
																		 T_FYCM_BASE FycmBase, double& dalComf, double& dalTenf);
	BOOL Get_AllowbleStressOfConc(BOOL bPre, T_MATD_D& MatdD, T_SPSC_SECT& SpscSect, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbar, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndn, double* dfi, double* dfe);
	BOOL Get_AllowbleStressofPC(T_TDNA_K TdnaK, T_FYPC_KSCE& FypcKsce);
	BOOL Get_CrackCheck(int iCrackType, T_MATD_D& MatdD, T_SPSC_SECT& SpscSect, CArray<T_SPSC_COOR,T_SPSC_COOR>& arOuter, CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*>& arInner, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbar,
											CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi, double dMu, double dftop, double dfbot, double dMu_tp, double& dfs, double& dw, double& dwa);
	BOOL Get_TensionRebar(BOOL bTopCalc,  BOOL bPrint, T_MATD_D MatdD, double dbw, T_SPSC_SECT& SpscgSect, CArray<T_SPSC_COOR,T_SPSC_COOR>& arOuter, CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*>& arInner, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbar, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi,
												double dftop, double dfbot, double& dAreq, double& dAuse, double& dAmin);
	BOOL Get_AllowableStressofLcom(BOOL bStage, ElemPairK ElemK, double* dAlw);
	BOOL Get_AllowblePrincipalStressOfConc(T_MATD_D& MatdD, double& dAFS, double& dAFST);


	double Get_Beta1(double dfc);
	double Get_StressOfTendon(double df_py, double df_pu, double dEp, double dEpsi_ps);
	BOOL Get_FlexureStrength(BOOL bYaxis, BOOL bPosiI, BOOL bIter, BOOL bPositive, double dPhib, _PSC_MATL_ELEM& MatlElem, T_SPSC_SECT& SpscSect, CArray<T_SPSC_COOR,T_SPSC_COOR>& arOuter, CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*>& arInner, T_SPSC_SECT& SpscgSect,
													 double drp, double dbw, double dFlgThk[2], double dpMomPos[2][2], CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi, T_BDCY_KSCE_BASE& BdcrKsce, BOOL bApwM=FALSE);
	BOOL Get_FlexureStrength(BOOL bYaxis, BOOL bPosiI, BOOL bIter, BOOL bPositive, double dPhib, _PSC_MATL_ELEM& MatlElem, T_SPSC_SECT& SpscSect, CArray<T_SPSC_COOR,T_SPSC_COOR>& arOuter, CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*>& arInner, T_SPSC_SECT& SpscgSect,
													 double dbw, double dFlgThk[2], double dpMomPos[2][2], CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi, T_BDCZ_KSCE_BASE& BdcrKsce);
	BOOL Get_ShearDesign(BOOL bPosiI, double dPhiv, T_MATD_D& MatD, T_SPSC_SECT& SpscSect, T_SPSC_SECT& SpscgSect, double dShrThk, double dfdt, double dfdb, double dVd, double dMd, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, 
											 CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi, T_RPSC_SBAR& RpscSbar, T_CUMS_KSCE_BASE& CumsKsce);
	BOOL Get_TorsionDesign(BOOL bYaxis, BOOL bPosiI, double dPhiv, T_MATD_D MatD, T_SPSC_SECT& SpscSect, T_SPSC_SECT& SpscgSect, BOOL bClosedSect, double dAoh, double dPh, double dShrThk, double dTorThk, double dPcp, double dAcp, double dfdt, double dfdb, double dVd, double dMd, 
												 CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, T_RPSC_SBAR& RpscSbar, T_CRMT_KSCE_BASE& CrmtKsce);
	BOOL Get_ReBarInfor(BOOL bYaxis, double dGap, T_SPSC_SECT& SpscSect, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi,
	  								 double& dAs, double& dds, double& dAsc, double& ddc);
													

	BOOL CalctensLength(double dftop, double dfbot, double dh, double& dx);  
	BOOL CalcSectPSC_QA(CArray<T_SPSC_COOR,T_SPSC_COOR>& arOuter, CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*>& arInner, BOOL bDir, BOOL baxis, double dpos, double dlen, double& dCutArea, double& dQ);
	
	BOOL Get_fpeData(BOOL bYaxis, double dMu, double dAg, double dIy, double dyt, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi, double& dAps, double& dPeep, double& dPe, double& dVp, double& dfpe);
};

#endif // !defined(AFX_DGNKSCE_USD03_H__3E897861_48F0_4980_801F_F47764ED3C2E__INCLUDED_)
