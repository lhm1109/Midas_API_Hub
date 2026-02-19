// PrtKSCE_USD03.h: interface for the CPrtKSCE_USD03 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRTKSCE_USD03_H__9CE011E8_7BC6_4A20_895B_3F38B631ECF1__INCLUDED_)
#define AFX_PRTKSCE_USD03_H__9CE011E8_7BC6_4A20_895B_3F38B631ECF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PrtKSCE.h"

#include "..\wg_base\MSExcel.h"
#include "..\wg_base\XLParser.h"

class CDBDoc;
class CCRCDataCtrl;
class CCRCForceCtrl;

class CPrtKSCE_USD03
{
// Member Variables.
protected:
	CDBDoc* m_pDoc;
	CCRCDataCtrl*  m_pDataCtrl;
	CCRCForceCtrl* m_pForcCtrl;

	int m_iDgnCode;
	BOOL m_bPrintFlag;

	CArray<CString,CString> m_arText;
	int m_iFormula;
	CMap<T_STAG_K,T_STAG_K,CString,LPCTSTR> m_StagTitleMap;
	CMap<T_LCOM_K,T_LCOM_K,CString,LPCTSTR> m_LcomTitleMap;
	ArrElemPairKey m_arElemK;  
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
	
	double m_dZero;
public:
	PRT_BDCY_PARA m_BdcyParam;
	PRT_CUMS_PARA m_CumsParam;
	PRT_CRMT_PARA m_CrmtParam;

// Member Functions.
public:
	CPrtKSCE_USD03();
	virtual ~CPrtKSCE_USD03();

protected:
	BOOL Get_ForcDataByStag();
	BOOL Get_ForcDataByStrs();  // Lcom for Stresses.
	BOOL Get_ForcDataByStrn();  // Lcom for Strength.
	BOOL Get_ForcDataByOrgLcom();
	BOOL Cng_DgnForcCrc(_DGN_FORC_CRC& ForcCrc);
	BOOL Set_Forc24(int iPosiNo, _DGN_FORC_CRC& ForcCrc2, _DGN_FORC_CRC& ForcCrc4);
	// Unit : Curr -> Fixed.
	virtual double Cng_LenFixM(double dLength, int iUp);	// m.
	virtual double Cng_LenFix(double dLength, int iUp);	// cm.
	virtual double Cng_ForFix(double dForce);						// Tonf.
	virtual double Cng_MomFix(double dMoment);					// Tonf*m.
	virtual double Cng_StrFix(double dStress);					// kgf/cm^2.
	virtual double Cng_DegFix(double dRadian);					// Rad -> Deg.
	// Unit : Code -> Fixed.
	virtual double Cng_CodeLenFixM(double dLength, int iUp);	// m.
	virtual double Cng_CodeLenFix(double dLength, int iUp);	// cm.
	virtual double Cng_CodeForFix(double dForce);						// Tonf.
	virtual double Cng_CodeMomFix(double dMoment);					// Tonf*m.
	virtual double Cng_CodeStrFix(double dStress);					// kgf/cm^2.
	virtual double Cng_CodeDegFix(double dRadian);					// Rad -> Deg.

public:
	void SetDataCtrlPointer(CCRCDataCtrl* pDataCtrl, CCRCForceCtrl* pForcCtrl);
	void Set_PrintFlag(BOOL bPrintFlag);
	BOOL Get_PrintFlag();
	void Set_InitData();
	// Force Detail.
	BOOL Get_ForcDetlSect(ElemPairK ElemK, BOOL bChangeM, _PRT_FORC_DETL_SECT& DetlSect);
	BOOL Get_ForcDetlStag(ElemPairK ElemK, _PRT_FORC_DETL_STAG& DetlStag);
	BOOL Get_ForcDetlLcas(ElemPairK ElemK, _PRT_FORC_DETL_LCAS& DetlLcas);
	BOOL Get_ForcDetlLcom(_PRT_FORC_DETL_STAG& DetlStag, _PRT_FORC_DETL_LCAS& DetlLcas, _PRT_FORC_DETL_LCOM& LcomPrev, _PRT_FORC_DETL_LCOM& LcomNext);
	// Force Graphic.
	BOOL Get_ForcGrphStag(T_STAG_K StagK, CString& strText, double& dExtraRat, CArray<_PRT_FORC_GRPH_ELEM,_PRT_FORC_GRPH_ELEM>& arGrphElem);
	BOOL Get_ForcGrphLcom(T_LCOM_K LcomK, CString& strLcomName, CString& strText, double& dExtraRat, CArray<_PRT_FORC_GRPH_ELEM,_PRT_FORC_GRPH_ELEM>& arGrphElemMax, CArray<_PRT_FORC_GRPH_ELEM,_PRT_FORC_GRPH_ELEM>& arGrphElemMin);
	BOOL Get_ForcGrphTotl(CArray<_PRT_FORC_GRPH_TOTL,_PRT_FORC_GRPH_TOTL>& arGrphTotl);
	// Force Diagram.
	BOOL Get_ForcDiagram(CArray<_PRT_FORC_UENV_ELEM,_PRT_FORC_UENV_ELEM>& arUenvElemMax, CArray<_PRT_FORC_UENV_ELEM,_PRT_FORC_UENV_ELEM>& arUenvElemMin);
	// Result Table.
	BOOL Get_ResultBdcyPos(ArrElemPairKey& arElemK, CArray<PRT_BDCY_KSCE,PRT_BDCY_KSCE>& arBdcrKsce);	// My(+).
	BOOL Get_ResultBdcyNeg(ArrElemPairKey& arElemK, CArray<PRT_BDCY_KSCE,PRT_BDCY_KSCE>& arBdcrKsce);	// My(+).
	BOOL Get_ResultCums(	 ArrElemPairKey& arElemK, CArray<PRT_CUMS_KSCE,PRT_CUMS_KSCE>& arCumsKsce);	// Vz.
	BOOL Get_ResultCrmt(	 ArrElemPairKey& arElemK, CArray<PRT_CRMT_KSCE,PRT_CRMT_KSCE>& arCrmtKsce);	// Tx.
	// Result Detail.
	virtual BOOL Get_DetailTbarPos(ElemPairK ElemK);
	virtual BOOL Get_DetailTbarNeg(ElemPairK ElemK);
	virtual BOOL Get_DetailBdcyPos(ElemPairK ElemK);
	virtual BOOL Get_DetailBdcyNeg(ElemPairK ElemK);
	virtual BOOL Get_DetailCums(   ElemPairK ElemK);
	virtual BOOL Get_DetailCrmt(   ElemPairK ElemK);

protected:
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

	CString m_strStresUnit;
	CString m_strForceUnit;
	CString m_strMomentUnit;

	CString m_sTxArea;  
	CString m_sTxAreaM;
	CString m_sTxSecM;
	CString m_sTxSecMM;
	CString m_sTxStif;
	CString m_sTxStifM;
	CString m_sTxStress;

	int m_iVciType; 

public:
//Bdcr..
	void Print_ElementIJ(T_ELEM_K ElemK, BOOL bPosiI);
	void Print_ElementIJ(T_ELEM_K ElemK, BOOL bPosiI, BOOL bPositive);
	void Print_TbarElementIJ(T_ELEM_K ElemK, BOOL bPosiI, BOOL bTopBot);

	virtual void Print_BdcyTheory_Iter(CArray<CString,CString>& aString);
	virtual void Print_BdcyTheory_Code(CArray<CString,CString>& aString);  
	virtual void Print_BdcyDesign0(BOOL bApwM);
	virtual void Print_BdcyDesign1(double dfck, double db, double dAps , double dfy, double ddp, double dAst);
	virtual void Print_BdcyDesign2(double dfpu, double dd, double dAc , double dIy, double dyt, double dPe);
	virtual void Print_BdcyDesign3(double dbeta1, double dep, double dMu , double drp, double dhf);
	virtual void Print_BdcyDesign4(double dc, double dbeta1, double da , double dfck, double dAa, double dCc);
	virtual void Print_BdcyDesign5(CArray<CString,CString>& strTndn, CArray<double,double>* aData, double dTp, double dTpTimesep, double dPeTimesep);
	virtual void Print_BdcyDesign6(double dep_cu, double ddst, double dc , double dep_s, double dfs, double dfs_cal, double dEs, double dfy, double dAst, double dTs);
	virtual void Print_BdcyDesign7(double dTp, double dTs, double dCc , double dDelC, double dTol);
	virtual void Print_BdcyDesign8(double dCc, double dc, double da , double da_cent, double dMps, double dTs, 
																 double ddst, double dMn, double dpMn, double dMuy);
	virtual void Print_BdcyDesign9(double da, double dt, double dRhop, double dfps, double dfck , double dqp, double d036Beta1);
	virtual void Print_BdcyDesign91(double da, double dt, double dApw, double dAp, double dApf, double dAs, double db, double dbw, double ddp, double dfck, double dfps, double dfy, double dqp, double d036beta1);
	virtual void Print_BdcyDesign10(BOOL bIter, double dPe, double dAc, double dPeTimesep , double dyt, double dIy, double dfpe, 
																	double dfck, double dpMn, double d12Mcr, double dMu);

	virtual void Print_BdcyDesign_Code1(double dc, double dbeta1, double da , double dfck);
	virtual void Print_BdcyDesign_Code2(double dfpu, double drp, double dbeta1 , double dRhop, double dfck,
																			double dd, double ddp, double dRho, double dRhoc, double dfy, double dfps);
	virtual void Print_BdcyDesign_Code3(double dfpu, double dfpe_cal, double dfpe, double d05fpu, double dfps);
	virtual void Print_BdcyDesign_Code4(double da, double db, double dbw, double dApw, double dApf,
																			double dAp, double dAs, double dhf, double drp, double dbeta1,
																			double dRhop, double dfpu, double dfck, double dd, double ddp, double dRho, 
																			double dfy, double dfps, double dphi, double dMn, double dpMn, double dMu);
	void Print_BdcyDesign_Code5(BOOL bIter, double dphi, double dfpu, double drp, double dbeta1 , double dRhop, double dfck, double da, double dhf, double db, double dbw,
																			double dd, double ddp, double dqp, double dRho, double dApw, double dAp, double dApf, double dAs, double dfy, double dfps, double dpMn, double dpMn_beta, double d036beta1, double dpMn_Cal);
	virtual void Print_BdcyDesign_Code6(CArray<CString,CString>& strTndn, CArray<double,double>* aData);
	virtual void Print_BdcyDesign_Code7(double dc, double da, double db, double dbw, double dAs, double dhf,
																			double dfck, double dd, double ddp, double dfy, double dphi, double dMn_ps, double dpMn);

	virtual CString Get_fsPrint(double dfs, double depsi, double dEs, double dfy);
	virtual CString Get_DeltaCPrint(double dTp, double dTs, double dCc, double dDelC, double dTol);
	virtual CString Get_pMnPrint(double dpMn, double dMu);
	virtual CString Get_MaxPsPrint(double drhop, double dfps, double dfck, double dqp, double d036beta1);
	virtual CString Get_MinPsPrint(double dpMn, double d12Mcr);
 
	//Cums..
	virtual void Print_CumsTheory(CArray<CString,CString>& aString);
	virtual void Print_CumsDesign0();
	virtual void Print_CumsDesign1(double dfck, double dbw, double ddst , double dfy, double dh, double dIg);
	virtual void Print_CumsDesign2(double dfpu, double ddp, double dAc, double dfpy, double dyt, double dAps);
	virtual void Print_CumsDesign3(double dMu, double dVu, double dMd, double dVd, double dfd, double dPe, double dfpe);
	virtual void Print_CumsDesign4(double dh, double dd, double dd_org, double d08h, double ddp, double ddp_org, double dIy, double dyt, double dfck, double dfpe, 
																 double dfd, double dMcr);
	virtual void Print_CumsDesign5(double dMu, double dMd, double dMmax, double dVu, double dVd, double dVi, double dVci_cal,
																 double dVci, double dVci_incl, double dfck, double dbw, double dd, double dMcr, double dminVci);
	virtual void Print_CumsDesign6(double dPe, double dAc, double dfpc, double dfck, double dbw, 
																 double dd, double dVp, double dVcw, double dVci, double dVc);
	virtual void Print_CumsDesign7(double dVc, double dphiv, double d05pVc, double dpVc, double dVu, double dbw, 
																 double dd, double dVp, double dVcw, double dVci);
	virtual void Print_CumsDesign8(double dVu, double dVc, double dphiv, double dpVc, double d05pVc, double dpVn, double dfck, double dbw, double dd, double dVs_req, double dVs_max,
																 double dVs, double dVs_min, double dsv, double dfy, double dThetaV, double dAv_use, double dAv_req, double dAvmin);

	virtual CString Get_dValuePrint(double dh, double d08h, double dd, double ddp);
	virtual CString Get_ShearReinforcePrint(double dpVc, double d05pVc, double dVu);
	virtual CString Get_StrengthChkPrint(double dpVn, double dVu);

	//Crmt..
	virtual void Print_CrmtTheory(CArray<CString,CString>& aString);
	virtual void Print_CrmtDesign0();
	virtual void Print_CrmtDesign1(double dfck, double dbw, double dt , double dfy, double dh, double dIy);
	virtual void Print_CrmtDesign2(double dfpu, double dd, double ddp, double dAc, double dfpy, double dyt, double dAps, 
											 double dPcp, double dAcp);
	virtual void Print_CrmtDesign3(double dMu, double dTu, double dVu, double dMd, double dVd, double dfd, double dPe, double dfpe);
	virtual void Print_CrmtDesign4(double dfck, double dAcp, double dPcp, double dfpc, double dTcr, double dTu, double dfpe);
	virtual void Print_CrmtDesign5();
	virtual void Print_CrmtDesign6(BOOL bHollow, double dAohOverPh, double dVu,  double dVc, double dbw, double dd, double ddp, double dTu,
																 double dfck, double dTcr, double dPh, double dAoh, double dt, double dvVT, double dvVTmax);
	virtual void Print_CrmtDesign7(double dAoh, double dAo, double dAt, double dfyv, double dst, double dThetaC,
																 double dpTn, double dfyl, double dPh, double dAlmin);
	virtual void Print_CrmtDesign8(double dbw, double dst, double dfyv, double dAv_use, double dAtmin, 
																 double dfck, double dAcp, double dAt, double dPh, double dfyl, double dAlmin, 
																 double dAts_cal, double dbwfyv, double dAts);
	virtual CString Get_TorChkPrint(double dTcr, double dTu);

	CString Check_ValueForm(int iTotal, int iUnder, double dValue);

	virtual void Print_TbarDesign1();
	virtual void Print_TbarDesign2(double dftop, double dfbot, double dh, double dx, double dycent, double dfcent,
												 double dAten, double dTfor, double dfsa, double dFS, double dAsreq);
	virtual void Print_TbarDesign3(double dft, double dfm, double dftop, double dfbot, double dh, double dx, double dycent, double dfcent,
												 double dTfor, double dfsa, double dFS, double dAsreq, double dyten, double dAc);

	virtual int Get_XlLength(int iLength);
	virtual CString Get_TextStringLength(int iMaxLength, CString strText);


//Excel..
	BOOL Get_ChartForceStage(CArray<Prt_Graph_Data,Prt_Graph_Data&>& arGraph_Data);
	BOOL Get_ChartForceLcom(CArray<Prt_Graph_Data,Prt_Graph_Data&>& arGraphData);
	virtual BOOL Get_ForceDetlXLData(CString strTitle, CArray<Prt_ArString,Prt_ArString&>& arStrChk);
	virtual BOOL Get_ForceTotalXLData(CArray<CString,CString>& arStrTotal); 
	BOOL Get_ChartForceDiagram(CArray<Prt_Graph_Data,Prt_Graph_Data&>& arGraphData);
	virtual BOOL Get_UltiLoadXLData(CArray<Prt_ArString,Prt_ArString&>& arStrUltiData);
	virtual BOOL Get_StageKeyList(CArray<T_STAG_K,T_STAG_K>& arStagK);
	virtual BOOL Get_LcomKeyList(CArray<T_LCOM_K,T_LCOM_K>& arLcomK);

	virtual BOOL Get_FlexStrengthCalc(CArray<CString,CString>& arTheory, CArray<Prt_ArString_Detail,Prt_ArString_Detail>& arStrDetail, CMSExcel* pXL=NULL);
	virtual BOOL Get_FlexStrengthTable(CArray<CString,CString>& arPMomTable, CArray<CString,CString>& arNMomTable);
	virtual BOOL Get_FlexStrengthTable_KSCE10(CArray<CString,CString>& arPMomTable, CArray<CString,CString>& arNMomTable, CMSExcel* pXL=NULL);
	BOOL Get_PosiNegaBMD(CArray<CString,CString>& strTitle, CArray<CString,CString>& strCol, CArray<double,double>& dValue, CArray<ChartData,ChartData>& arChaBMD);
	virtual BOOL Get_ShearStrengthCalc(CArray<CString,CString>& arTheory, CArray<Prt_ArString_Detail,Prt_ArString_Detail>& arStrDetail);
	virtual BOOL Get_ShearStrengthTable(CArray<CString,CString>& arShearTable);
	virtual BOOL Get_ShearStrengthTable_KSCE10(CArray<CString,CString>& arShearTable, CMSExcel* pXL=NULL);
	
	virtual BOOL Get_TorsStrengthCalc(CArray<CString,CString>& arTheory, CArray<Prt_ArString_Detail,Prt_ArString_Detail>& arStrDetail);
	virtual BOOL Get_ReinforceCalc(CArray<Prt_ArString_Detail,Prt_ArString_Detail>& arStrDetail);
	
};

#endif // !defined(AFX_PRTKSCE_USD03_H__9CE011E8_7BC6_4A20_895B_3F38B631ECF1__INCLUDED_)


