// DgnDataCtrl.h: interface for the CDgnDataCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__DGNDATACTRL_H__)
#define __DGNDATACTRL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnStruct.h"
#include "..\wg_db\wg_db_qfactor.h"

#include "HeaderPre.h"

enum EN_FY_THK
{
	EN_FY_THK_1 = 0,
	EN_FY_THK_2,
	EN_FY_THK_3,
	EN_FY_THK_4,
	EN_FY_THK_5,
	EN_FY_THK_6,
	EN_FY_THK_7,
	EN_FY_THK_8,
	EN_FY_THK_9,
	EN_FY_THK_10,
	EN_FY_THK_NUM,
};

enum EN_LCOM
{
	EN_LCOM_CHAR = 0,
	EN_LCOM_FREQ,
	EN_LCOM_QUAS,
	EM_LCOM_NUM
};

enum EN_EC2_EXPOSURE_CLASS
{
	EN_X0 = 0,
	EN_XC1,
	EN_XC2,
	EN_XC3,
	EN_XC4,
	EN_XD1,
	EN_XD2,
	EN_XD3,
	EN_XS1,
	EN_XS2,
	EN_XS3,
	EN_XF1,
	EN_XF2,
	EN_XF3,
	EN_XF4,
	EN_XA1,
	EN_XA2,
	EN_XA3
};

enum EN_EC2_SWE_SERV_LIFE
{
	EN_SWE_L100 = 0,
	EN_SWE_L50,
	EN_SWE_L20
};

struct T_FY_UNITPARAM
{
	double dLenUnit;
	double dStrUnit;
	double dTolZero;

	void Initialize()
	{
		dLenUnit = 0.0;
		dStrUnit = 0.0;
		dTolZero = 0.0;
	}

	T_FY_UNITPARAM(double dLen, double dStr, double dTol)
	{
		dLenUnit = dLen;
		dStrUnit = dStr;
		dTolZero = dTol;
	}

	/// 계속 이 형태로 호출되니, 이렇게 모아두고 다시 refactoring 하자!	
	BOOL IsLE(double dThk, double dThkLim) { return dThk < (dThkLim + dTolZero) / dLenUnit ? TRUE : FALSE; }
	BOOL IsLT(double dThk, double dThkLim) { return dThk < (dThkLim - dTolZero) / dLenUnit ? TRUE : FALSE; }
	BOOL IsGE(double dThk, double dThkLim) { return dThk > (dThkLim - dTolZero) / dLenUnit ? TRUE : FALSE; }
	BOOL IsGT(double dThk, double dThkLim) { return dThk > (dThkLim + dTolZero) / dLenUnit ? TRUE : FALSE; }
	double GetCurZeroStress() { return dTolZero / dStrUnit; }
};

struct T_CRACK_EC2
{
	bool   bActivate[EM_LCOM_NUM];
	bool   bChkFlag[EM_LCOM_NUM];
	double dCrkLim[EM_LCOM_NUM];

	T_CRACK_EC2()
	{
		Initialize();
	}

	void Initialize()
	{
		for(int i=0; i<EM_LCOM_NUM; ++i)
		{
			bActivate[i]  = false;
			bChkFlag[i] = false;
			dCrkLim[i]  = 0.0;
		}
	}

	void SetActivate(const bool& bChar, const bool& bFreq, const bool& bQuas)
	{
		bActivate[EN_LCOM_CHAR] = bChar;
		bActivate[EN_LCOM_FREQ] = bFreq;
		bActivate[EN_LCOM_QUAS] = bQuas;
	};

	void SetFlag(const bool& bChar, const bool& bFreq, const bool& bQuas)
	{
		bChkFlag[EN_LCOM_CHAR] = bChar;
		bChkFlag[EN_LCOM_FREQ] = bFreq;
		bChkFlag[EN_LCOM_QUAS] = bQuas;
	};
	void SetLimit(const double& dChar, const double& dFreq, const double& dQuas)
	{
		dCrkLim[EN_LCOM_CHAR] = dChar;
		dCrkLim[EN_LCOM_FREQ] = dFreq;
		dCrkLim[EN_LCOM_QUAS] = dQuas;
	};
};

class CDBDoc;
class CStoryData;
class CMembCtrl;

class __MY_EXT_CLASS__ CDgnDataCtrl
{
public:
	CDgnDataCtrl();
	virtual ~CDgnDataCtrl();

	// Member Variables.
protected:
	CDBDoc*	m_pDoc;
	BOOL m_bHighRised;	// For GBJ17-88, GB50017-03. (TRUE : Height >= 10F or 28m).
	// Add by ParkBong.('07.06.28) 
	BOOL m_bOver12F;    // For GB50011-01 (TRUE : Height >= 12F).
	BOOL m_IsTestMode;  // For Debugging.
	CString m_strTestFile;

	int m_iDgnStatus; // 0:None, RC_BEAM, RC_COLUMN, RC_BRACE, RC_WALL, STEEL, SRC_BEAM, SRC_COLUMN
public:
	CMembCtrl* m_pMembCtrl;

	// Member Functions.
protected:
	BOOL Chk_DgnConRebb(CString strFirst, CString strSecond, T_REBB_D& rData);
	BOOL Chk_DgnConRebb(T_REBB_D& rData);
	BOOL Chk_DgnSrcRbsb(CString strFirst, CString strSecond, T_RBSB_D& rData);
	BOOL Chk_DgnSrcRbsb(T_RBSB_D& rData);
	void Cal_DgnGenRedu(int nCH, T_DCTL_BASE Data, CStoryData* pStoryData,
		BOOL bReduAxl, BOOL bReduMom, BOOL bReduShr);
	void Set_DgnGenRedu(T_DCTL_BASE rData, CStoryData* pStoryData, CString StoryName,
		double dFactor, BOOL bReduAxl, BOOL bReduMom, BOOL bReduShr);

public:
	// General.
	void Create_DgnGenRedu();												// Create Live Load Reduction Factor.
	void Create_DgnGenKfac(BOOL bCount = FALSE);							// Create K-Factor.
	void Create_DgnHighRised(const CString& strCode);						// Create High-rised Building Information.
	void Create_DgnGenQfac(int iDgnNo = D_RC_COLUMN, CString strCodeIn = _T(""));                       // Create Q-Factor. ('06.03.20). Add by GAY.
	void Create_DgnGenQfac_LRFD(int iDgnNo = D_RC_COLUMN, CString strCodeIn = _T(""));

	BOOL Get_DgnStlMatd(T_MATD_D& mData);
	BOOL Get_DgnConMatd(T_MATD_D& mData);
	BOOL Get_DgnSrcMatd(T_MATD_D& mData);
	BOOL Get_DgnGenMatd(int key, T_MATD_D& rData);						// Material.
	BOOL Get_DgnGenMatd4Rpt(T_MATL_K MatlK, T_MATD_D& rData);           // Add, Jaeoh [1/14/2009]


	BOOL Get_DgnGenDctl(T_DCTL_D& rData);								// Definition of Frame.
	BOOL Get_DgnGenCmft(int key, T_CMFT_D& rData);						// Cm-Factor.
	BOOL IsAutoDgnGenCmft(int key);
	BOOL Get_DgnGenFmag(int key, T_FMAG_D& rData);						// Magnified Factor.
	BOOL Get_DgnGenLtsr(int key, double dFy1, T_LTSR_D& rData);			// Limit Slender Ratio.
	BOOL Get_DgnGenSueq(int key, T_SUEQ_D& rData);						// Scale Up Factor for Earthquake.
	BOOL Get_DgnGenRedu(int key, T_REDU_D& rData);						// Live Load Reduction Factor.
	BOOL Get_DgnGenKfac(int key, T_KFAC_D& rData, T_KFAC_D& aData);	// K-Factor. (rData:User, aData:Auto).
	BOOL Get_DgnGenMbtp(int key, T_MBTP_D& rData);						// Member Type.
	BOOL Get_DgnGenGreq(int key, int& iDgnClass);						// Grade of Earthquake in China.
	int Get_DgnGenMbtp(int key);										// Member Type.
	int Get_DgnGenSpva(int key);										// Special Provision for AREMA
	int Get_DgnGenMbtpTower(int key);									// Slenderness Checking Type for Tower.
	BOOL Get_DgnGenQfac(int elemK, int lcomK, Q_FACTOR& qFactor);   // Add by GAY.('06.02.24).Get Stability index.

	BOOL Get_DgnGenLeng(int key, int iPosiNo, T_LENG_D& rData, double& dLen);	// Length.
	BOOL Get_DgnGenLeng(int key, int iPosiNo, T_LENG_D& rData, double& dLen, double& dNetLy, double& dNetLz);	// Length, Net-Length.
	BOOL Get_DgnGenLeng(int key, int iPosiNo, T_LENG_D& rData, double& dLen, double& dNetLy, double& dNetLz, double* dOffSetLy, double* dOffSetLz, int nLengTypei = 0, int nLengTypej = 0, int nOffSetOutType = 0);	// Length, Net-Length.

	// Coded by Seungjun MNet:No.2393 ('20060816)
	// Serviceability Limit State
	BOOL Get_DgnGenDisp(T_ELEM_K ElemKey, int iLcomKind, T_LCOM_K LcomKey, _DGN_DISP& Disp, int iCantilever, int nCode_Length, BOOL bServ = TRUE);			// Displacement.  
	BOOL Get_DgnGenBeamDisp(T_ELEM_K ElemKey, int iLcomKind, T_LCOM_K LcomKey, _DGN_DISP& Disp, int iCantilever, BOOL bServ = TRUE);
	BOOL Get_DgnGenColmDisp(T_ELEM_K ElemKey, int iLcomKind, T_LCOM_K LcomKey, _DGN_DISP& Disp, int iCantilever);
	void Get_DgnGenLocalDispCantilever(double dDisp1, double dDisp2, const CArray<double, double>& aDistRatio, OUT CArray<double, double>& raLocalDisp);
	void Get_DgnGenLocalDisp(double dDisp1, double dDisp2, const CArray<double, double>& aDistRatio, OUT CArray<double, double>& raLocalDisp);
	void Get_DistRatioAlongMemberCantilever(bool bSupportEndI, const T_ELEM_K_LIST& aElemK, int nDiv4Elem, OUT CArray<double, double>& raDistRatio);
	void Get_DistRatioAlongMember(const T_ELEM_K_LIST& aElemK, int nDiv4Elem, OUT CArray<double, double>& raDistRatio);

	// Cold Formed Steel.
	BOOL Get_DgnCfsDcfs(T_DCFS_D& sData);														// Cold Formed Steel Control Data.
	BOOL Get_DgnCfsCsms(int key, T_CSMS_D& rData);									// Add by tss
	BOOL Get_DgnCfsCsos(int key, T_CSOS_D& rData);									// Add by tss
	BOOL Get_DgnCfsPlin(int key, T_PLIN_D& rData);
	// Steel.
	BOOL Get_DgnStlDstl(T_DSTL_D& sData);														// Steel Control Data.
	BOOL Get_DgnStlSect(int key, T_SECT_D& rData, BOOL bDgnMemb = FALSE);// Steel Section.
	BOOL Get_DgnStlDalw(int key, T_DALW_D& rData);									// Define Allowable Stress.
	BOOL Get_DgnStlCbft(int key, T_CBFT_D& rData);									// Cb-Factor.
	BOOL Get_DgnStlCvft(int key, T_CVFT_D& rData);									// Cv-Factor.
	BOOL Get_DgnStlUtcf(int key, T_UTCF_D& rData);									// Unity Control Factor.
	BOOL Get_DgnStlStfn(int key, BOOL& bFlag, T_STFN_D& rData);			// Stiffener.
	BOOL Get_DgnStlSlrs(int Key, T_SLRS_D& rData);                  // Seismic Load Resisting System. Modify by GAY. PMS:4035. ('09.08.01).
	BOOL Get_DgnStlBcif(int Key, T_BCIF_D& rData);					// Eurocode Buckliung curve imperfection factor GEN-6936

	// Concrete.
	BOOL Get_DgnConDcon(T_DCON_D& rData);														// Concrete Control Data.
	// Change by ZINU.('06.11.28). NO:2484, Get Rebar Data for Design by Global -> Elem.
	BOOL Get_DgnConDcrb(int key, T_DCRB_D& rData);									// Rebar Control Data. For Member.
	BOOL Get_DgnConDcrb(T_DCRB_D& rData);                           // Rebar Control Data. For Global. // Add by GAY. MNET:XXXX. ('08.06.30). 
	BOOL Get_DgnConRebb(int key, BOOL& bFlag, T_REBB_D& rData);			// Beam Section.
	BOOL Get_DgnConRebc(int key, BOOL& bFlag, T_REBC_D& rData);			// Column Section.
	BOOL Get_DgnConRebr(int key, BOOL& bFlag, T_REBR_D& rData);			// Brace Section.
	BOOL Get_DgnConMrft(int key, T_MRFT_D& rData);									// Moment Redistribution Factor.
	BOOL Get_DgnConTrft(int key, T_TRFT_D& rData);                  // Add by GAY. MNET:3780. ('09.03.20). Torsion Reduction Factor.
	BOOL Get_DgnConUccf(int key, T_UCCF_D& rData);                  // Add by GAY. PMS:4267. ('11.12.28). 중국신기준-불확정성 분항계수.

	BOOL Get_DgnConLamb(int key, double& rdLambda, BOOL& rbLightCon);// SIAM / 2013.01.23 / KCI-USD12 : 경량콘크리트계수
	BOOL Get_DgnConCEnv(int key, T_REXC_D& rData);                   // SIAM / 2013.01.23 / KCI-USD12 : 균열환경
	double Get_DgnConLamb(int key);                                  // SIAM / 2013.01.23 / KCI-USD12 : 경량콘크리트계수
	UINT   Get_DgnConCEnv(int key);                                  // SIAM / 2013.01.23 / KCI-USD12 : 균열환경
	double Get_DgnConStan(int key);                                  // RC frame 부재별 strut angle. EC2에서 전단강도 계산을 위한 변수.
	int  Get_DgnConPmdm(T_ELEM_K ElemK);
	int  Get_DgnConPmdm_SeisEval();

	bool IsConSeisColmType(T_ELEM_K ElemK);
	int GetConSeisColmType(T_ELEM_K ElemK);

	// Wall.
	BOOL Get_DgnWalKfac(int key, T_KFAC_D& rData, T_KFAC_D& aData);	// K-Factor for Wall (rData:User, aData:Auto).
	BOOL Get_DgnWalCmft(int key, T_CMFT_D& rData);									// Cm-Factor for Wall.
	BOOL Get_DgnWalFmag(int key, T_FMAG_D& rData);									// Magnified Factor for Wall.
	BOOL Get_DgnWalSueq(int key, T_SUEQ_D& rData);									// Scale Up Factor for Earthquake.
	BOOL Get_DgnWalMbtp(int key, T_MBTP_D& rData);									// Element Type.
	BOOL Get_DgnWalRedu(int key, T_REDU_D& rData);									// Live Load Reduction Factor for Wall.
	BOOL Get_DgnWalGreq(int key, int& iDgnClass);										// Grade of Earthquake in China.
	BOOL Get_DgnWalMatd(int key, int& iMatlNo, T_MATD_D& rData);		// Material for Wall.
	BOOL Get_DgnWalWmak(int key, T_WMAK_D& rData);									// Wall mark.
	BOOL Get_DgnWalRebw(int key, BOOL& bFlag, T_REBW_D& rData);			// Wall Section.
	// Add by ZINU.('06.11.28). NO:2484, Get Rebar Data for Design by Global -> Elem.
	BOOL Get_DgnWalDcrb(int key, T_DCRB_D& rData);									// Rebar Control Data.
	int  Get_DgnWalBndrElemMethod(int iWallID);                     // Add by GAY. PMS:4076. ('10.04.19). Boundary Element Method for Wall ID.    
	int Get_DgnBEEBarDesignMethod(int key);							// Added by Maharshi
	BOOL Get_DgnWalUccf(int key, T_UCCF_D& rData);                  // Add by GAY. PMS:4267. ('11.12.28). 중국신기준-불확정성 분항계수.

	BOOL Get_DgnWalLamb(int key, double& rdLambda, BOOL& rbLightCon);// SIAM / 2013.01.23 / KCI-USD12 : 경량콘크리트계수
	BOOL Get_DgnWalCEnv(int key, T_REXC_D& rData);                   // SIAM / 2013.01.23 / KCI-USD12 : 균열환경
	double Get_DgnWalLamb(int key);                                  // SIAM / 2013.01.23 / KCI-USD12 : 경량콘크리트계수
	UINT   Get_DgnWalCEnv(int key);                                  // SIAM / 2013.01.23 / KCI-USD12 : 균열환경
	int  Get_DgnWalPmdm(T_WALL_KEY WallK);
	int  Get_DgnWalPmdm_SeisEval();

protected:
	// Add by ZINU.('06.11.28). NO:2484, Get Rebar Data for Design by Elem.
	BOOL Get_DgnConDcbb(int key, T_DCRB_D& rData);
	BOOL Get_DgnConDcbc(int key, T_DCRB_D& rData);
	BOOL Get_DgnConDcbr(int key, T_DCRB_D& rData);
	BOOL Get_DgnWalDcbw(int key, T_DCRB_D& rData);
	BOOL Get_DgnWallEbmw(int key, T_DCRB_D& rData);

public:
	// SRC.
	BOOL Get_DgnSrcDsrc(T_DSRC_D& rData);														// SRC Design Code.
	BOOL Get_DgnSrcRebs(int key, BOOL& bFlag, T_REBS_D& rData);			// SRC Section.
	BOOL Get_DgnSrcRbsb(int key, BOOL& bFlag, T_RBSB_D& rData);			// SRC-Beam Section.

	BOOL Get_DgnHcbmSect(int PartK, int iPartNo, T_SECT_D& rData);  // RC-Haunched Beam
	BOOL Get_DgnHcbmTaperedSectSize(int PartK, int iPartNo, double& dHc, double& dBc);

	// PSC Design.
	BOOL Get_DgnPscPscd(T_PSCD_D& rData);

	// Pre-stressed, Tendon.
	// Add by ZINU.('02.12.04). For Tendon Positions.
	BOOL Get_DgnConTndn(int key, int iStepNo, CArray<_DGN_TNDN, _DGN_TNDN> aTndnList[]);
	BOOL Get_DgnConPsas(int key, T_PSAS_D& rData);									// Pre-stressed Allowable Stress.
	BOOL Get_DgnConJsft(int key, T_JSFT_D& rData);									// Jacking Stress Factor.

	// Add by ZINU.('04.01.17). For Display Option. Auto-calculation Data.
	BOOL Exist_DgnGenKfacAuto();
	BOOL Exist_DgnGenReduAuto();
	BOOL Get_DgnGenKfacAuto(CMap<T_ELEM_K, T_ELEM_K, T_KFAC_D, T_KFAC_D>& arKfacDataAuto);
	BOOL Get_DgnGenReduAuto(CMap<T_ELEM_K, T_ELEM_K, T_REDU_D, T_REDU_D>& arReduDataAuto);

	// PMS:4020-Seungjun-20090914 EC2:04 Slab/Wall Design.
	BOOL Get_DgnConServ(T_ELEM_K iElemNo, T_SERV_D& ServD);
	BOOL Get_DgnStlServ(T_ELEM_K iElemNo, T_SERV_D& ServD);

	void Get_DgnConCrackDef_EC2_05(int nNationalAnnex, int nExposure, int nLife, OUT double& rdWmaxQ, OUT double& rdWmaxF);

	// For Setting Default Data by Code.
protected:
	BOOL Get_DgnConDcrbDef(T_DCRB_D& rData);		
	
	void Get_DgnConCrackDef_EC2_05_NA_REC(int nExposure, OUT double& rdWmaxQ, OUT double& rdWmaxF);
	void Get_DgnConCrackDef_EC2_05_NA_AUS(int nExposure, OUT double& rdWmaxQ, OUT double& rdWmaxF);
	void Get_DgnConCrackDef_EC2_05_NA_CZE(int nExposure, OUT double& rdWmaxQ, OUT double& rdWmaxF);
	void Get_DgnConCrackDef_EC2_05_NA_NLD(int nExposure, OUT double& rdWmaxQ, OUT double& rdWmaxF);
	void Get_DgnConCrackDef_EC2_05_NA_FIN(int nExposure, OUT double& rdWmaxQ, OUT double& rdWmaxF);
	void Get_DgnConCrackDef_EC2_05_NA_IRL(int nExposure, OUT double& rdWmaxQ, OUT double& rdWmaxF);
	void Get_DgnConCrackDef_EC2_05_NA_LUX(int nExposure, OUT double& rdWmaxQ, OUT double& rdWmaxF);
	void Get_DgnConCrackDef_EC2_05_NA_SVK(int nExposure, OUT double& rdWmaxQ, OUT double& rdWmaxF);
	void Get_DgnConCrackDef_EC2_05_NA_ESP(int nExposure, OUT double& rdWmaxQ, OUT double& rdWmaxF);
	void Get_DgnConCrackDef_EC2_05_NA_SWE(int nExposure, int nLife, OUT double& rdWmaxQ, OUT double& rdWmaxF);

	// Coded by Seungjun MNet:No.2393 ('20061121)
public:
	double Change_Fstr(int iLenID, int iForID);
	double Change_Flen(int iLenID);
	double Change_Ffor(int iForID);
	double Change_Fln2(int iLenID);
public:
	// Add by ZINU.('03.11.18). To Set Default Strength Reduction Factor.
	BOOL Get_DgnCfsPhiDef(CString strCode, double* pdPhi, int nNatioalAnnex);
	BOOL Get_DgnStlPhiDef(CString strCode, double* pdPhi, int nNatioalAnnex);
	BOOL Get_DgnConPhiDef(CString strCode, double* pdPhi);
	// Add by ZINU.('03.11.19). To Set Default Moment Redistribution Factor.
	BOOL Get_DgnConMrftDef(CString strCode, double& dMrft);
	// Add by GAY. MNET:3780. ('09.03.20). To Set Default Torsion Reduction Factor.
	BOOL Get_DgnConTrftDef(CString strCode, double& dTrft);
	// Add by ZINU.('05.05.25). To Set Default Safety Level, Eq Grade (GB50017-03, GBJ17-88, GB50010-02, JGJ138-01).
	BOOL Get_DgnStlChinaDef(CString strCode, int& iSafeLevel, int& iEqGrade);
	BOOL Get_DgnConChinaDef(CString strCode, int& iSafeLevel, int& iEqGrade);
	BOOL Get_DgnSrcChinaDef(CString strCode, int& iSafeLevel, int& iEqGrade);
	// Add by ZINU.('04.02.04). To Set Default Scale Up Factor for Shear.
	BOOL Get_DgnConSufsDef(CString strCode, int iClass, int& iAlphaType, double& dAlpha1, double& dAlpha2);
	BOOL Get_DgnSrcSufsDef(CString strCode, int iClass, int& iAlphaType, double& dAlpha1, double& dAlpha2);
	// Add by ZINU.('02.12F.24). To Set Default Lcom for Transfer Stage.
	BOOL Get_DgnConLcomDef(int iLcomKind, int iLcomKeyTran);
	// Add by ZINU.('03.01.03). To Set Default PS-Allowable Stress.
	BOOL Get_DgnConPsasDef(CString strCode, double& dRtsc, double& dRtsts, double& dRtstm, double& dRsscs, double& dRssct, double& dRsst);
	// Coded by Seungjun MNet:No.2393 ('20061121)
	// Crack Width Default Value
	BOOL Get_DgnConCrackDef(const CString &strCode, int nSubCode, double* pdPhi);
	// Add by ZINU.('03.02.15). For GBJ17-88, GB50017-03 Seismic Design.
	BOOL Is_HighRiseForGBJ();
	// Add by ParkBong.('07.06.28) 
	BOOL Is_Over12FloorGBJ();
	// Add by ZINU.('03.08.27). For Column Under Ground by ElemKey.
	// Change by ZINU.('06.08.30). NO:2338, Check Conditions, Member SubType.
	//double Get_ConColmEffLengFactorForGB(int key, double dAutoKFactor);
	BOOL Get_ConColmEffLengFactorForGB(int key, double& dKy, double& dKz);
	// Coded by Seungjun MNet:No.2393 ('20061110)
	double Get_DgnConEc(double dfc, CString strMatlDB = _T(""), CString strMatlNa = _T(""));

	// Coded by Seungjun MNet:No.2393 ('20060831)
	double GetDeflectLimByCode(int iMembType, int& iDeflectType);
	double GetDeflectLimByCode4ColdFormed(int iMembType, int& iDeflectType);

	// Coded by Seungjun MNet:No.2591 ('20061211)	For SCWB Design.
	BOOL IsSCWBDesign();
	BOOL IsRBeamOrColumn(T_ELEM_K ElemK, int& iMBType);
	double Get_SCWBfactor();
	BOOL IsConsiderTopStorForConSCWB();
	BOOL IsConsiderTopStorForStlSCWB();

	//Add By maxiao
	CMap<T_ELEM_K, T_ELEM_K, double, double> m_mapK1y;
	CMap<T_ELEM_K, T_ELEM_K, double, double> m_mapK2y;
	CMap<T_ELEM_K, T_ELEM_K, double, double> m_mapK1z;
	CMap<T_ELEM_K, T_ELEM_K, double, double> m_mapK2z;

	BOOL IsMeshedSlab(const int& iDgnCode);
	BOOL IsMeshedWall(const int& iDgnCode);
	int  GetCodeID4Meshed(const CString& strCode, const BOOL& bPlateForce);

	void SetDatabyNationalAnnex(int nNationalAnnex, double* dPhi, double& dAlpha_cc, double& dAlphacc4Shear);
	int GetHorizontalElem_On_Node(T_ELEM_K ElemK, CArray<T_ELEM_K, T_ELEM_K>& arTop, CArray<T_ELEM_K, T_ELEM_K>& arBottom, T_NODE_K &TopNode, T_NODE_K &BottomNode, CArray<double, double>& TAyzJunction, CArray<double, double> & BAyzJunction, CArray<T_MATD_D, T_MATD_D> &MatlArrayTop, CArray<T_MATD_D, T_MATD_D> &MatlArrayBottom, CArray<CString, CString> &SectTypeTop, CArray<CString, CString> &SectTypeBottom);
	int GetSectionAttachedtoElem(T_ELEM_K ElemK, CArray<T_ELEM_K, T_ELEM_K>& arTop, CArray<T_ELEM_K, T_ELEM_K>& arBottom, T_NODE_K &TopNode, T_NODE_K &BottomNode, CArray<CString, CString> &SectTypeTop, CArray<CString, CString> &SectTypeBottom, CArray<T_SECT_SECTION_D, T_SECT_SECTION_D> &SectTop, CArray<T_SECT_SECTION_D, T_SECT_SECTION_D> &SectBottom, CArray<T_MATD_D, T_MATD_D> &MatlArrayTop, CArray<T_MATD_D, T_MATD_D> &MatlArrayBottom);
	// Add by ZINU.('02.5.2). For Civil Design.
protected:
	double Get_ClearHDistRbar(double dDistFactor, double dBarDia, CString DgnCode, int iPourigType = 0, int nLayer = 0);
	double Get_BeamVDistRbar(double dBarDia, CString DgnCode);
	double Get_BeamDistRbar(double dBarDia, CString DgnCode, int iPourigType = 0, int nLayer = 0);
	double Get_ColmDistRbar(double dBarDia, CString DgnCode, int nLayer = 0);
	BOOL Check_BeamUseDist(int iSectShp, int iSite, double dD[], int iBarNum, double dBarDia, double dDc, double dDc1, CString DgnCode, int iPourigType = 0);
	BOOL Check_ColmUseDist(int iSectShp, int iSite, double dD[], int iBarNum, double dBarDia, double dDc, CString DgnCode, int nLayer = 0);

public:
	int Check_RbarCivil(int iKind, _DGNC_RBAR& RbarData);
	int Check_RbarCivil4PlateRating(int iKind, int iThik, _DGNC_RBAR& RbarData);

	double Get_UsedAs(int iSect, int iGrup, double dAsBar);
	BOOL Get_SufcCivil(int iElem, T_SUFC_D& rData);	// Scale up Factor for Column.
	BOOL Get_UlecCivil(int iElem, T_ULEC_D& rData); // Design Parameter for ULS.
	// KSCE-USD05 (Civil)
	int Check_RbarCivilRC(int iKind, int iPosi, _DGNC_RBAR& RbarData);
	double Lengthfor1Meter(int iLenID);
	BOOL Get_CTCfactor4RCcolumnCivil(int iElem, int iCode_Length, double& dfactor);

	// 기존 Civil-MODS RC. 설계에서 철근의 존재 여부 판다. 
	BOOL IsCPBEExsit(int nPosi, T_CPBE_D& CpbeD);
	BOOL IsCPBEMbarExist(int nPosi, T_CPBE_D& CpbeD);
	BOOL IsCPBESbarExist(int nPosi, T_CPBE_D& CpbeD);

	BOOL IsCPCOExsit(int nPosi, T_CPCO_D& CpcoD);
	BOOL IsCPCOMbarExist(int nPosi, T_CPCO_D& CpcoD);
	BOOL IsCPCOSbarExist(int nPosi, T_CPCO_D& CpcoD);

	BOOL IsRCHKCOExsit(T_RCHK_COLM& RchkD);
	BOOL IsRCHKCOMbarExist(T_RCHK_COLM& RchkD);
	BOOL IsRCHKCOSbarExist(T_RCHK_COLM& RchkD);

	BOOL IsCPWAExsit(int nPosi, T_CPWA_D& CpwaD);
	BOOL IsCPWAMbarExist(int nPosi, T_CPWA_D& CpwaD);
	BOOL IsCPWASbarExist(int nPosi, T_CPWA_D& CpwaD);


public:
	// PMS:4020-Seungjun-20090713 EC2:04 Slab/Wall Deisng
	int  Get_Node_in_Elem(T_ELEM_D& ElemD);

	BOOL Get_DgnConRbms(T_ELEM_K ElemK, T_RBMS_D& RbmsD, int nCode_Length);
	BOOL Get_DgnConRbms(T_ELEM_K ElemK, CArray<T_RBMS_D, T_RBMS_D&>& aRbmsD, int nCode_Length);
	BOOL Get_DgnConRbmw(T_ELEM_K ElemK, T_RBMW_D& RbmwD, int nCode_Length);
	BOOL Get_DgnConRbst(T_ELEM_K ElemK, CArray<T_RBST_D, T_RBST_D&>& aRbstD, int nCode_Length);
	BOOL Get_DgnConRbsl(T_ELEM_K ElemK, OUT CArray<T_RBSL_D, T_RBSL_D&>& raRbslD, int nCode_Length);

	int  Get_DgnConSbdoKList(int iType, std::vector<T_SBDO_K>& aSbdoKeys); // 0:all, 1:Slab, 2:Mat, 3:wall, 4:slab/mat
	int  GetPlateList(CArray<T_ELEM_K, T_ELEM_K>& aElemList, BOOL bWall = FALSE);

	int GetDgnStatus();
	BOOL SetDgnStatus(int iDgnStatus);
	// Seungjun-20110411 일본 2차설계 여부 판단.
	BOOL IsJUD();
	BOOL ConsiderJUDWebBending(int nMembType);

	//BOOL Get_DgnMlcb(T_MLCB_D& rMlcbD);
	// Add, Jaeoh. [1/14/2009]
	//BOOL Get_StlMatd4RPT(T_MATD_RPT& MatdRptD);
	//BOOL Get_ConMatd4RPT(T_MATD_RPT& MatdRptD);
	//BOOL Get_SrcMatd4RPT(T_MATD_RPT& MatdRptD);

	BOOL IsLongTerm(int nLcomType, T_LCOM_K LcomK);
	BOOL IsSeismicLcom(int nLcomType, T_LCOM_K LcomK);
	bool IsAccidentalLoad(int nLoadCaseK);

	BOOL   IsSame3DPoint(T_NODE_D &p1, T_NODE_D &p2);
	BOOL   IsSameLine(double dOCx[2], double dOCy[2], double dNCx[2], double dNCy[2]);
	double GetMaxDistFormPnt(UINT NodeK, UINT ElemK);
	BOOL   IsNodeOnElemPlane(T_NODE_D& NodeD, T_ELEM_D& ElemD, double dTol = 1e-4);
	int    Get_SamePlaneList(T_KEY NodeK, CArray<UINT, UINT> &arElemK, CArray<UINT, UINT> &arSamePlaneElemK); // 동일 평면에 있는 요소리스트를 찾음.
	CString Get_OptimizedListString(CArray<T_ELEM_K, T_ELEM_K>& aKey);

public:
	void Get_UnitFactorForThick(double& rdLenUnit, double& rdStrUnit, int nCodeUnitLen, int nCodeUnitFor);
	double Get_FyByThick_Code(const CString& strMatlDB, const CString& strMatlNa, int nShapeClass, BOOL bHollowSect, double dThkMax, T_FY_UNITPARAM& UnitParam, double adFy[EN_FY_THK_NUM]);

	double Get_FyByThick_KS16(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3);
	double Get_FyByThick_KS18_KDS41(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4);
	double Get_FyByThick_KS18_KDS14(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4, double dFy5);
	double Get_FyByThick_KS22(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4, double dFy5);
	double Get_FyByThick_GB50018_02(const CString& strMatlNa, double dFy1);
	double Get_FyByThick_GB50018_25(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4);
	double Get_FyByThick_JGJ2015(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4, double dFy5);
	double Get_FyByThick_GB50017_17(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4, double dFy5);
	double Get_FyByThick_GB50429_07(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2);
	double Get_FyByThick_TB10091_17(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2);
	double Get_FyByThick_TB10092_17(const CString& strMatlNa, double dFy1);
	double Get_FyByThick_JTG3362_18(const CString& strMatlNa, double dFy1);
	double Get_FyByThick_EN10326(const CString& strMatlNa, double dFy1);
	double Get_FyByThick_EN10149_2(const CString& strMatlNa, double dFy1);
	double Get_FyByThick_EN10149_3(const CString& strMatlNa, double dFy1);
	double Get_FyByThick_GB_50917_13(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4, double dFy5);
	//double Get_FyByThick_GOST_SNiP(const CString& strMatlNa, double dThkMax, double dLenUnit, double dStrUnit, double dTolZero, double dFy1, double dFy2, double dFy3);
	//double Get_FyByThick_GOST_SP(const CString& strMatlNa, double dThkMax, double dLenUnit, double dStrUnit, double dTolZero, double dFy1, double dFy2, double dFy3);

	double Get_FyByThick_ASNZS3678_17(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4, double dFy5, double dFy6);
	double Get_FyByThick_ASNZS3679_17(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3);
	double Get_FyByThick_ASNZS4672_17(const CString& strMatlNa, double dFy1);

	double Get_FyByThick_KS(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2);
	double Get_FyByThick_KS08(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3);
	double Get_FyByThick_KS09(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2);
	double Get_FyByThick_KS_Civil(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3);

	double Get_FyByThick_JIS(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3);
	double Get_FyByThick_JIS_Civil(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4);

	double Get_FyByThick_DIN(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3);
	double Get_FyByThick_BS(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4);
	double Get_FyByThick_BS04(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4, double dFy5, double dFy6);
	double Get_FyByThick_EN05(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2);
	double Get_FyByThick_EN05P(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4, double dFy5, double dFy6);
	double Get_FyByThick_EN05SW(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1);
	double Get_FyByThick_EN(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2);
	double Get_FyByThick_UNI(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2);

	double Get_FyByThick_GB12(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4, double dFy5);
	double Get_FyByThick_GB03(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4);
	double Get_FyByThick_GB(const CString& strMatlNa, int nShapeClass, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3);
	double Get_FyByThick_JGJ(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4);
	double Get_FyByThick_JTJ(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4);
	double Get_FyByThick_JTJ023_85(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4);

	double Get_FyByThick_CSA(const CString& strMatlNa, BOOL bHollowSect, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3);
	double Get_FyByThick_IS(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3);
	double Get_FyByThick_CNS(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4);
	double Get_FyByThick_CNS06(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4);

	double Get_FyByThick_BS1_12ASTM(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4, double dFy5);
	double Get_FyByThick_BS1_12BSEN(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4, double dFy5, double dFy6);
	double Get_FyByThick_BS1_12JIS(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4, double dFy5, double dFy6);
	double Get_FyByThick_BS1_12GB(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4, double dFy5);

	double Get_FyByThick_TIS1228_2018(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1);

	double Get_FyByThick_SP16_2017_tB3(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double adFy[EN_FY_THK_NUM]);
	double Get_FyByThick_SP16_2017_tB4(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double adFy[EN_FY_THK_NUM]);
	double Get_FyByThick_SP16_2017_tB5(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double adFy[EN_FY_THK_NUM]);

	double Get_FyByThick_NR_GN_CIV_025(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double adFy[EN_FY_THK_NUM]);

	int GetChkKindStlMatl(const CString& strStlMatlCode);

protected:
	double GetDeflectLimByCode_EC3(int nMembType, int& rnDeflType);
	double GetDeflectLimByCode_TWN96(int nMembType, int& rnDeflType);
	double GetDeflectLimByCode_KSSC09(int nMembType, int& rnDeflType);
	double GetDeflectLimByCode_AISC05(int nMembType, int& rnDeflType);
	double GetDeflectLimByCode_GB17(int nMembType, int& rnDeflType);
	double GetDeflectLimByCode_KDS2022(int nMembType, int& rnDeflType);
	double GetDeflectLimByCode_IS800(int nMembType, int& rnDeflType);
	double GetDeflectLimByCode_IRC24(int nMembType, int& rnDeflType);
public:
	bool IsExistDgnLcomULS(int nLcomType);
	bool IsExistDgnLcomSLS(int nLcomType);
	bool IsExistDgnLcomELS(int nLcomType);

	UINT GetConCodeEnum(const CString& strCode);
	UINT GetSRCCodeEnum(const CString& strCode);
	UINT GetStlCodeEnum(const CString& strCode);

	bool Is_UngerGround(const UINT& iMemb);

public:
	UINT   GetBrdgSeisEvalCode();

	double GetHcbmMaxHc(const T_ELEM_K& iElem);

	int GetBeamSupportType(const UINT& iElemNo, const int& iCantilever);

	void Get_CrackData_EN04(const int& nNationalAnnex, const EN_EC2_EXPOSURE_CLASS& enClass, const EN_EC2_SWE_SERV_LIFE& enServLife, const bool& bCorrosionHigh, T_CRACK_EC2& CrackD);
	void Get_CrackData_EN04_REC(const EN_EC2_EXPOSURE_CLASS& enClass, T_CRACK_EC2& CrackD);	// Recommended
	void Get_CrackData_EN04_DNK(const EN_EC2_EXPOSURE_CLASS& enClass, T_CRACK_EC2& CrackD); // Denmark
	void Get_CrackData_EN04_NLD(const EN_EC2_EXPOSURE_CLASS& enClass, T_CRACK_EC2& CrackD); // Netherlands
	void Get_CrackData_EN04_FIN(const EN_EC2_EXPOSURE_CLASS& enClass, T_CRACK_EC2& CrackD); // Finland
	void Get_CrackData_EN04_FRA(const EN_EC2_EXPOSURE_CLASS& enClass, T_CRACK_EC2& CrackD); // France
	void Get_CrackData_EN04_GBR(const EN_EC2_EXPOSURE_CLASS& enClass, T_CRACK_EC2& CrackD); // United Kingdom
	void Get_CrackData_EN04_NOR(const EN_EC2_EXPOSURE_CLASS& enClass, const EN_EC2_SWE_SERV_LIFE& enServLife, T_CRACK_EC2& CrackD); // Norway
	void Get_CrackData_EN04_ESP(const EN_EC2_EXPOSURE_CLASS& enClass, T_CRACK_EC2& CrackD); // Spain
	void Get_CrackData_EN04_SWE_C_L100(const EN_EC2_EXPOSURE_CLASS& enClass, T_CRACK_EC2& CrackD); // Corrosion sensitivity, Sweden L100
	void Get_CrackData_EN04_SWE_C_L50(const EN_EC2_EXPOSURE_CLASS& enClass, T_CRACK_EC2& CrackD); // Corrosion sensitivity, Sweden L50
	void Get_CrackData_EN04_SWE_C_L20(const EN_EC2_EXPOSURE_CLASS& enClass, T_CRACK_EC2& CrackD); // Corrosion sensitivity, Sweden L20
	void Get_CrackData_EN04_SWE_L_L100(const EN_EC2_EXPOSURE_CLASS& enClass, T_CRACK_EC2& CrackD); // Little corrosion sensitivity, Sweden L100
	void Get_CrackData_EN04_SWE_L_L50(const EN_EC2_EXPOSURE_CLASS& enClass, T_CRACK_EC2& CrackD); // Little corrosion sensitivity, Sweden L50
	void Get_CrackData_EN04_SWE_L_L20(const EN_EC2_EXPOSURE_CLASS& enClass, T_CRACK_EC2& CrackD); // Little corrosion sensitivity, Sweden L20
};


class __MY_EXT_CLASS__ CDgnDataCtrlStatusSaver
{
public:
	CDgnDataCtrlStatusSaver(CDgnDataCtrl* pDgnDataCtrl, int nType = 1);
	virtual ~CDgnDataCtrlStatusSaver();
public:
	//void Save();
	//void Restore();
	void Reset() { m_nType = 0; }
private:
	CDgnDataCtrl* m_pDgnDataCtrl;
	int m_iDgnStatus;
	int m_nType; // 0:동작안함 1:Save된 값으로 복구 2:초기화
};

#include "HeaderPost.h"

#endif // !defined(__DGNDATACTRL_H__)
