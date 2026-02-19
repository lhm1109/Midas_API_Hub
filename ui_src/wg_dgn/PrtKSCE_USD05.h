// PrtKSCE_USD05.h: interface for the CPrtKSCE_USD05 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRTKSCE_USD05_H__INCLUDED_)
#define AFX_PRTKSCE_USD05_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PrtKSCE_USD03.h"

#include "..\wg_base\MSExcel.h"
#include "..\wg_base\XLParser.h"


class CPrtKSCE_USD05 : public CPrtKSCE_USD03
{
// Member Variables.
protected:

//  BOOL m_bPrintFlag;
/*
	CArray<CString,CString> m_arText;
	CMap<T_STAG_K,T_STAG_K,CString,LPCTSTR> m_StagTitleMap;
	CMap<T_LCOM_K,T_LCOM_K,CString,LPCTSTR> m_LcomTitleMap;
	CArray<T_ELEM_K,T_ELEM_K> m_arElemK;
	CArray<T_STAG_K,T_STAG_K> m_arStagK;
	CArray<T_LCOM_K,T_LCOM_K> m_arLcomK;
	
	CMap<T_STAG_K,T_STAG_K,_PRT_FORC_GRPH_GRUP,_PRT_FORC_GRPH_GRUP&> m_amForcStag;
	CMap<T_LCOM_K,T_LCOM_K,_PRT_FORC_GRPH_GRUP,_PRT_FORC_GRPH_GRUP&> m_amForcOrgLcom;
	CArray<_DGN_FORC_CRC,_DGN_FORC_CRC&> m_arElemMy[2];	// 0=Max,1=Min.
	CArray<_DGN_FORC_CRC,_DGN_FORC_CRC&> m_arElemVz[2];	// 0=Max,1=Min.
	CArray<_DGN_FORC_CRC,_DGN_FORC_CRC&> m_arElemMx[2];	// 0=Max,1=Min.
	CArray<T_LCOM_K,T_LCOM_K> m_arElemLcomMy[2][2];	// 1st(0=Max,1=Min), 2nd(0=I,1=J).
	CArray<T_LCOM_K,T_LCOM_K> m_arElemLcomVz[2][2];	// 1st(0=Max,1=Min), 2nd(0=I,1=J).
	CArray<T_LCOM_K,T_LCOM_K> m_arElemLcomMx[2][2];	// 1st(0=Max,1=Min), 2nd(0=I,1=J).
*/

// Member Functions.
public:
	CPrtKSCE_USD05();
	virtual ~CPrtKSCE_USD05();

protected:
	/*
	BOOL Get_ForcDataByStag();
	BOOL Get_ForcDataByStrs();  // Lcom for Stresses.
	BOOL Get_ForcDataByStrn();  // Lcom for Strength.
	BOOL Get_ForcDataByOrgLcom();
	BOOL Cng_DgnForcCrc(_DGN_FORC_CRC& ForcCrc);
	BOOL Set_Forc24(int iPosiNo, _DGN_FORC_CRC& ForcCrc2, _DGN_FORC_CRC& ForcCrc4);
	double Get_ElemLength(T_ELEM_K ElemK);
	*/

public:
/*	
	void SetDataCtrlPointer(CCRCDataCtrl* pDataCtrl, CCRCForceCtrl* pForcCtrl);
	void Set_PrintFlag(BOOL bPrintFlag);
	BOOL Get_PrintFlag();
	void Set_InitData();
	// Force Detail.
	BOOL Get_ForcDetlSect(T_ELEM_K ElemK, _PRT_FORC_DETL_SECT& DetlSect);
	BOOL Get_ForcDetlStag(T_ELEM_K ElemK, _PRT_FORC_DETL_STAG& DetlStag);
	BOOL Get_ForcDetlLcas(T_ELEM_K ElemK, _PRT_FORC_DETL_LCAS& DetlLcas);
	BOOL Get_ForcDetlLcom(_PRT_FORC_DETL_STAG& DetlStag, _PRT_FORC_DETL_LCAS& DetlLcas, _PRT_FORC_DETL_LCOM& LcomPrev, _PRT_FORC_DETL_LCOM& LcomNext);
	// Force Graphic.
	BOOL Get_ForcGrphStag(T_STAG_K StagK, CString& strText, double& dExtraRat, CArray<_PRT_FORC_GRPH_ELEM,_PRT_FORC_GRPH_ELEM>& arGrphElem);
	BOOL Get_ForcGrphLcom(T_LCOM_K LcomK, CString& strText, double& dExtraRat, CArray<_PRT_FORC_GRPH_ELEM,_PRT_FORC_GRPH_ELEM>& arGrphElemMax, CArray<_PRT_FORC_GRPH_ELEM,_PRT_FORC_GRPH_ELEM>& arGrphElemMin);
	BOOL Get_ForcGrphTotl(CArray<_PRT_FORC_GRPH_TOTL,_PRT_FORC_GRPH_TOTL>& arGrphTotl);
	// Force Diagram.
	BOOL Get_ForcDiagram(CArray<_PRT_FORC_UENV_ELEM,_PRT_FORC_UENV_ELEM>& arUenvElemMax, CArray<_PRT_FORC_UENV_ELEM,_PRT_FORC_UENV_ELEM>& arUenvElemMin);
	// Result Table.
	BOOL Get_ResultBdcyPos(CArray<T_ELEM_K,T_ELEM_K>& arElemK, CArray<PRT_BDCY_KSCE,PRT_BDCY_KSCE>& arBdcrKsce);	// My(+).
	BOOL Get_ResultBdcyNeg(CArray<T_ELEM_K,T_ELEM_K>& arElemK, CArray<PRT_BDCY_KSCE,PRT_BDCY_KSCE>& arBdcrKsce);	// My(+).
	BOOL Get_ResultCums(	 CArray<T_ELEM_K,T_ELEM_K>& arElemK, CArray<PRT_CUMS_KSCE,PRT_CUMS_KSCE>& arCumsKsce);	// Vz.
	BOOL Get_ResultCrmt(	 CArray<T_ELEM_K,T_ELEM_K>& arElemK, CArray<PRT_CRMT_KSCE,PRT_CRMT_KSCE>& arCrmtKsce);	// Tx.
*/  
	// Result Detail.
	BOOL Get_DetailTbarPos(ElemPairK ElemK);
	BOOL Get_DetailTbarNeg(ElemPairK ElemK);
	BOOL Get_DetailBdcyPos(ElemPairK ElemK, CMSExcel* pXL=NULL);
	BOOL Get_DetailBdcyNeg(ElemPairK ElemK, CMSExcel* pXL=NULL);
	BOOL Get_DetailCums(   ElemPairK ElemK);
	BOOL Get_DetailCrmt(   ElemPairK ElemK);

protected:
	/*
	int m_TotalNum;
	int m_decimal;

	CString m_strLength;
	CString m_strForce;
	CString m_strDegree;
	CString m_strRadian;
	
	CString m_strArea;
	CString m_strAreaM;
	CString m_strStif;
	CString m_strStifM;
	CString m_strMoment;
	CString m_strStress;
	*/
	CString m_strMpa;
/*  
	CString m_sTxArea;  
	CString m_sTxAreaM;
	CString m_sTxSecM;
	CString m_sTxSecMM;
	CString m_sTxStif;
	CString m_sTxStifM;
	CString m_sTxStress;
*/
public:
//Bdcr..
	//void Print_ElementIJ(T_ELEM_K ElemK, BOOL bPosiI);

	void Print_BdcyTheory_Iter(CArray<CString,CString>& aString, int iCode);
	void Print_BdcyTheory_Code_KSCE05(CArray<CString,CString>& aString);
	void Print_BdcyTheory_Code_RAIL04(CArray<CString,CString>& aString);
	void Print_BdcyDesign0(BOOL bApwM);
	void Print_BdcyDesign1(double dfck, double db, double dAps , double dfy, double ddp, double dAst);
	void Print_BdcyDesign2(double dfpu, double dd, double dAc , double dIy, double dyt, double dPe);
	void Print_BdcyDesign3(double dbeta1, double dep, double dMu , double drp, double dhf);
	void Print_BdcyDesign4(double dc, double dbeta1, double da , double dfck, double dAa, double dCc);
	void Print_BdcyDesign5(CArray<CString,CString>& strTndn, CArray<double,double>* aData, double dTp, double dTpTimesep, double dPeTimesep);
	void Print_BdcyDesign6(double dep_cu, double ddst, double dc , double dep_s, double dfs, double dfs_cal, double dEs, double dfy, double dAst, double dTs);
	void Print_BdcyDesign7(double dTp, double dTs, double dCc , double dDelC, double dTol);
	void Print_BdcyDesign8(double dCc, double dc, double da , double da_cent, double dMps, double dTs, 
												 double ddst, double dMn, double dpMn, double dMuy);
	void Print_BdcyDesign9(double da, double dt, double dRhop, double dfps, double dfck , double dqp, double d036Beta1);
	void Print_BdcyDesign91(double da, double dt, double dApw, double dAp, double dApf, double dAs, double db, double dbw, double ddp, double dfck, double dfps, double dfy, double dqp, double d036beta1);
	void Print_BdcyDesign10(BOOL bIter, double dPe, double dAc, double dPeTimesep , double dyt, double dIy, double dfpe, 
													double dfck, double dpMn, double d12Mcr, double dMu);

	void Print_BdcyDesign_Code1(double dc, double dbeta1, double da , double dfck);
	void Print_BdcyDesign_Code2(double dfpu, double drp, double dbeta1 , double dRhop, double dfck,
															double dd, double ddp, double dRho, double dRhoc, double dfy, double dfps);
	void Print_BdcyDesign_Code3(double dfck, double dfpu, double dfpy, double dfpe_cal, double dfpe, double d05fpu, double dfps, double dfps_cal, double dRhop, double dfps_unbCom, double dfps_unbond_Compare);
	void Print_BdcyDesign_Code4(double da, double db, double dbw, double dApw, double dApf,
															double dAp, double dAs, double dhf, double drp, double dbeta1,
															double dRhop, double dfpu, double dfck, double dd, double ddp, double dRho, 
															double dfy, double dfps, double dphi, double dMn, double dpMn, double dMu);
	void Print_BdcyDesign_Code5(BOOL bIter, double dphi, double dfpu, double drp, double dbeta1 , double dRhop, double dfck, double da, double dhf, double db, double dbw,
															double dd, double ddp, double dqp, double dRho, double dApw, double dAp, double dApf, double dAs, double dfy, double dfps, double dpMn, double dpMn_beta, double d036beta1, double dpMn_Cal);
	void Print_BdcyDesign_Code6(CArray<CString,CString>& strTndn, CArray<double,double>* aData);
	void Print_BdcyDesign_Code7(double dc, double da, double db, double dbw, double dAs, double dhf,
															double dfck, double dd, double ddp, double dfy, double dphi, double dMn_ps, double dpMn, double dMu);


	CString Get_fsPrint(double dfs, double depsi, double dEs, double dfy);
	CString Get_DeltaCPrint(double dTp, double dTs, double dCc, double dDelC, double dTol);
	CString Get_pMnPrint(double dpMn, double dMu);
	CString Get_MaxPsPrint(double drhop, double dfps, double dfck, double dqp, double d036beta1);
	CString Get_MinPsPrint(double dpMn, double d12Mcr);
 
	//Cums..
	void Print_CumsTheory(CArray<CString,CString>& aString);
	void Print_CumsDesign0();
	void Print_CumsDesign1(double dfck, double dbw, double ddst , double dfy, double dh, double dIg);
	void Print_CumsDesign2(double dfpu, double ddp, double dAc, double dfpy, double dyt, double dAps);
	void Print_CumsDesign3(double dMu, double dVu, double dMd, double dVd, double dfd, double dPe, double dfpe);
	void Print_CumsDesign4(double dh, double dd, double dd_org, double d08h, double ddp, double ddp_org, double dIy, double dyt, double dfck, double dfpe, 
												 double dfd, double dMcr);
	void Print_CumsDesign5(double dMu, double dMd, double dMmax, double dVu, double dVd, double dVi, double dVci_cal,
												 double dVci, double dVci_incl, double dfck, double dbw, double dd, double ddp, double dMcr, double dminVci);
	void Print_CumsDesign6(double dPe, double dAc, double dfpc, double dfck, double dbw, 
												 double dd, double ddp, double dVp, double dVcw, double dVci, double dVc);
	void Print_CumsDesign7(double dVc, double dphiv, double d05pVc, double dpVc, double dVu, double dbw, 
												 double dd, double dVp, double dVcw, double dVci);
	void Print_CumsDesign8(double dVu, double dVc, double dphiv, double dpVc, double d05pVc, double dpVn, double dfck, double dbw, double dd, double dVs_req, double dVs_max,
												 double dVs, double dVs_min, double dsv, double dfy, double dThetaV, double dAv_use, double dAv_req, double dAvmin);

	CString Get_dValuePrint(double dh, double d08h, double dd, double ddp);
	CString Get_ShearReinforcePrint(double dpVc, double d05pVc, double dVu);
	CString Get_StrengthChkPrint(double dpVn, double dVu);

	//Crmt..
	void Print_CrmtTheory(CArray<CString,CString>& aString);
	void Print_CrmtDesign0();
	void Print_CrmtDesign1(double dfck, double dbw, double dt , double dfy, double dh, double dIy);
	void Print_CrmtDesign2(double dfpu, double dd, double ddp, double dAc, double dfpy, double dyt, double dAps, 
											 double dPcp, double dAcp);
	void Print_CrmtDesign3(double dMu, double dTu, double dVu, double dMd, double dVd, double dfd, double dPe, double dfpe);
	void Print_CrmtDesign4(double dfck, double dAcp, double dPcp, double dfpc, double dTcr, double dTu, double dfpe);
	void Print_CrmtDesign5();
	void Print_CrmtDesign6(BOOL bHollow, double dAohOverPh, double dVu,  double dVc, double dbw, double dd, double ddp, double dTu,
												 double dfck, double dTcr, double dPh, double dAoh, double dt, double dvVT, double dvVTmax);
	void Print_CrmtDesign7(double dAoh, double dAo, double dAt, double dfyv, double dst, double dThetaC,
											 double dpTn, double dfyl, double dPh, double dAlmin);
	void Print_CrmtDesign8(double dbw, double dst, double dfyv, double dAv_use, double dAtmin, 
												 double dfck, double dAcp, double dAt, double dPh, double dfyl, double dAlmin, 
												 double dAts_cal, double dbwfyv, double dAts);
	CString Get_TorChkPrint(double dTcr, double dTu);

//  CString Check_ValueForm(int iTotal, int iUnder, double dValue);

	void Print_TbarDesign1();
	void Print_TbarDesign2(double dftop, double dfbot, double dh, double dx, double dycent, double dfcent,
												 double dAten, double dTfor, double dfsa, double dFS, double dAsreq);
	void Print_TbarDesign3(double dft, double dfm, double dftop, double dfbot, double dh, double dx, double dycent, double dfcent,
												 double dTfor, double dfsa, double dFS, double dAsreq, double dyten, double dAc);

// 
	int Get_XlLength(int iLength);
	CString Get_TextStringLength(int iMaxLength, CString strText);
	

//Excel..
//  BOOL Get_ChartForceStage(CArray<CString,CString>& strTitle, CArray<CString,CString>& strCol, CArray<double,double>& dValue, CArray<ChartData,ChartData>& StageData);
//  BOOL Get_ChartForceLcom(CArray<CString,CString>& strTitle, CArray<CString,CString>& strCol, CArray<double,double>& dValue, CArray<ChartData,ChartData>& LcomData);
	BOOL Get_ForceDetlXLData(CString strTitle, CArray<Prt_ArString,Prt_ArString&>& arStrChk);
	BOOL Get_ForceTotalXLData(CArray<CString,CString>& arStrTotal); 
//  BOOL Get_ChartForceDiagram(CArray<CString,CString>& strTitle, CArray<CString,CString>& strCol, CArray<double,double>& dValue, CArray<ChartData,ChartData>& arForcDiag);
	BOOL Get_UltiLoadXLData(CArray<Prt_ArString,Prt_ArString&>& arStrUltiData);
	BOOL Get_StageKeyList(CArray<T_STAG_K,T_STAG_K>& arStagK);
	BOOL Get_LcomKeyList(CArray<T_LCOM_K,T_LCOM_K>& arLcomK);

	BOOL Get_FlexStrengthCalc(CArray<CString,CString>& arTheory, CArray<Prt_ArString_Detail,Prt_ArString_Detail>& arStrDetail, CMSExcel* pXL=NULL);
	//BOOL Get_FlexStrengthCalc_KSCE10(CMSExcel* pXL=NULL);
	
	BOOL Get_FlexStrengthTable(CArray<CString,CString>& arPMomTable, CArray<CString,CString>& arNMomTable);
	BOOL Get_FlexStrengthTable_KSCE10(CArray<CString,CString>& arPMomTable, CArray<CString,CString>& arNMomTable, CMSExcel* pXL=NULL);
//  BOOL Get_PosiNegaBMD(CArray<CString,CString>& strTitle, CArray<CString,CString>& strCol, CArray<double,double>& dValue, CArray<ChartData,ChartData>& arChaBMD);
	BOOL Get_ShearStrengthCalc(CArray<CString,CString>& arTheory, CArray<Prt_ArString_Detail,Prt_ArString_Detail>& arStrDetail);
	BOOL Get_ShearStrengthTable(CArray<CString,CString>& arShearTable);
	BOOL Get_ShearStrengthTable_KSCE10(CArray<CString,CString>& arShearTable, CMSExcel* pXL=NULL);
	BOOL Get_TorsStrengthCalc(CArray<CString,CString>& arTheory, CArray<Prt_ArString_Detail,Prt_ArString_Detail>& arStrDetail);
	BOOL Get_ReinforceCalc(CArray<Prt_ArString_Detail,Prt_ArString_Detail>& arStrDetail);
	
};

#endif // !defined(AFX_PRTKSCE_USD05_H__INCLUDED_)


