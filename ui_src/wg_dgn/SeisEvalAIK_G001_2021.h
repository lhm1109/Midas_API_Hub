#pragma once

#include "seiseval.h"

struct T_PDHG_CONBEAM_FLEX
{
	double dbi;  /// 주철근의 지름. ([CHECK] layer 별 직경이 다른 경우, 최외곽 철근 ?)
	double dAs1; /// 1단 철근 단면적.
	double dd1;  /// 1단 철근 거리 (압축연단 ~ 도심)
	double dAs2; /// 2단 철근 단면적.
	double dd2;  /// 2단 철근 거리 (압축연단 ~ 도심)
	double dAs;  /// 철근 단면적.
	double dd;   /// 유효 춤.
	double dRho; /// 철근비.	
	double da;
    double dMe;  /// 기대휨강도.
    double dMy;  /// 항복모멘트

	void Initialize()
	{
		dbi  = 0.0;
		dAs1 = 0.0;
		dd1  = 0.0;
		dAs2 = 0.0;
		dd2  = 0.0;
		dAs  = 0.0;
		dd   = 0.0;
		dRho = 0.0;
		da   = 0.0;
		dMe  = 0.0;
        dMy  = 0.0;
	}

	T_PDHG_CONBEAM_FLEX()
	{
		Initialize();
	}
};

struct T_PDHG_CONBEAM_INFO
{
	double dB; /// 보 폭.
	double dH; /// 보 깊이.
	double dIe;  /// 유효단면2차모멘트.

	double dfck; /// 콘크리트 압축강도.
	double dEc;  /// 콘크리트 탄성계수.
	
	double dfy;  /// 주철근 항복강도.
	double dfys; /// 전단철근 항복강도.		
	double dEs;  /// 철근 탄성계수.

	double dVu;
	double dMu;
	double dVc;
	double dVs;
	double dVp;

	T_PDHG_CONBEAM_FLEX FlexTop; /// 상부철근 정보.
	T_PDHG_CONBEAM_FLEX FlexBot; /// 하부철근 정보.
	double dThetayN;
	double dThetayP;

	UINT unDetail;     /// 내진상세.
	double dShearSpan; /// 전단경간	
	double dAsv;
	double dS;         /// 전단철근 간격.

	double dRatioMuMy; /// M_ultimate / M_yield.
			
	void Initialize()
	{
		dB = 0.0;
		dH = 0.0;
		dIe = 0.0;

		dfck = 0.0;
		dEc  = 0.0;
		
		dfy  = 0.0;
		dfys = 0.0;
		dEs  = 0.0;

		dVu = 0.0;
		dMu = 0.0;
		dVc = 0.0;
		dVs = 0.0;
		dVp = 0.0;

		FlexTop.Initialize();
		FlexBot.Initialize();
		dThetayN = 0.0;
		dThetayP = 0.0;

		unDetail = EN_SEBE_DETL_NONE;
		dShearSpan = 0.0;
		dAsv = 0.0;
		dS   = 0.0;

		dRatioMuMy = 1.0;
	}

	T_PDHG_CONBEAM_INFO()
	{
		Initialize();
	}

	T_PDHG_CONBEAM_FLEX GetFlexByMu() const
	{
		return dMu < 0.0 ? FlexTop : FlexBot;
	}

	double GetThetayByMu() const 
	{
		return dMu < 0.0 ? dThetayN : dThetayP;
	}
};

struct T_PDHG_CONBEAM_PARAM
{
	UINT enCtrlType;
	bool bSeisDetail;
	double dEspUL;
	double dEspCF;
	double dEspBL;
	double dEspSC;
	double dcu;  /// [L]
	double dPhiy;
	double dlh;  /// [L]

	void Initialize()
	{
		enCtrlType = EN_EVALCTRL_FLEX;
		bSeisDetail = false;
		dEspUL = 0.0;
		dEspCF = 0.0;
		dEspBL = 0.0;
		dEspSC = 0.0;
		dcu = 0.0;
		dPhiy = 0.0;
		dlh = 0.0;		
	}

	T_PDHG_CONBEAM_PARAM()
	{
		Initialize();
	}
};

struct T_PDHG_CONCOLM_INFO
{
	double dfck;  /// 콘크리트 압축강도.
	double dEc;   /// 콘크리트 탄성계수.

	double dfy;   /// 주철근 항복강도.
	double dfys;  /// 전단철근 항복강도.
	double dEs;   /// 철근 탄성계수.

	bool bCircle;	
	double dAg;   /// 콘크리트 전체 단면적.
	double dAst;  /// 주철근 단면적.

	UINT unDetail; /// 내진상세.
	double dS;     /// 전단철근 간격.
	double ddo;    /// 콘크리트 연단에서 주철근 중심까지의 거리.

	void Initialize()
	{
		dfck = 0.0;
		dEc  = 0.0;

		dfy  = 0.0;
		dfys = 0.0;
		dEs  = 0.0;

		bCircle = false;
		dAg  = 0.0;
		dAst = 0.0;

		unDetail = EN_SECO_DETL_NONE;
		dS  = 0.0;
		ddo = 0.0;
	}

	T_PDHG_CONCOLM_INFO()
	{
		Initialize();
	}
};

struct T_PDHG_CONCOLM_DIR
{
	double dPu; /// 압축력.
	double dVu; /// 전단력.
	double dMu; /// 모멘트.

	double dB;
	double dH;

	double dBv;
	double ddv;
	double dAv;         /// 전단철근 단면적.
	double dShearSpan;  /// 전단경간.

	double dMn0;/// P-M 상관도 상 순수 휨강도.
	double dMn; /// 동일 축력에 대한 모멘트 강도.
	double dVy; /// 휨항복에 의한 전단력.
	double dVc;
	double dVs;
	double dVn;
	double dmu; /// 변형연성도.
	
	double dIg;
	double dIe;

	double dThetay;  /// 항복변형각.
	double dThetau;  /// 한계변형각.
	double dThetaa;  /// 파괴변형각.

	void Initialize()
	{
		dPu = 0.0;
		dVu = 0.0;
		dMu = 0.0;

		dB = 0.0;
		dH = 0.0;

		dBv = 0.0;
		ddv = 0.0;
		dAv = 0.0;
		dShearSpan = 0.0;

		dMn0 = 0.0;
		dMn = 0.0;
		dVy = 0.0;
		dVc = 0.0;
		dVs = 0.0;
		dVn = 0.0;
		dmu = 0.0;

		dIg = 0.0;
		dIe = 0.0;

		dThetay = 0.0;
		dThetau = 0.0;
		dThetaa = 0.0;
	}

	T_PDHG_CONCOLM_DIR()
	{
		Initialize();
	}

	bool IsDeformationControlled() const
	{
		return dgn::LT(dVc + dVs, dVy) ? false : true;
	}
};

class CSeisEvalAIK_G001_2021 : public CSeisEval
{
public:
	CSeisEvalAIK_G001_2021(void);
	virtual ~CSeisEvalAIK_G001_2021(void) override;

private:
    struct T_CU final
    {
        double dcu;
        double dcu1;
        double dcu2;

        T_CU(double dH) { Init(dH); }
        void Init(double dH)
        {
            dcu1 = dH;
            dcu2 = 0.0;
            dcu = (dcu1 + dcu2) * 0.5;
        }
    };

protected:	
	virtual void SetInitPhgtPropComponentCon(UINT unDOF, BOOL bDOF, int nLoc, int nSkcvType, BOOL bSymIJ, int nInitStiff, OUT T_PHGT_D& rPhgtD) override final;
	void GetInitialHingePropCon(UINT unDOF, double dC, OUT T_PHGT_FEMA& rPhgtFEMA);

	virtual BOOL CalcFzHingePropConBeam(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, const EN_NLANALTYPE& enNLType, double dRatioMuMy, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhgtFemaJ) override final;
	virtual double GetDistLayerConBeam() { return 25.0; }  /// 기준에 따라 다르면 해당 기준에서 override.

	virtual BOOL CalcMyHingePropConBeam(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, const EN_NLANALTYPE& enNLType, double dRatioMuMy, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhgtFemaJ) override final;
	virtual double CalcBeta1(double dfck) { ASSERT(0); return 0.0; }

	virtual BOOL CalcFxHingePropConColm(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhghFemaJ);
	virtual BOOL CalcFyzHingePropConColm(T_ELEM_K ElemK, EN_DIR enDir, const T_SEIS_PO_ANAL_RESULT_D& PORslt, const EN_NLANALTYPE& enNLType, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhghFemaJ);
	virtual BOOL CalcMyzHingePropConColm(T_ELEM_K ElemK, EN_DIR enDir, const T_SEIS_PO_ANAL_RESULT_D& PORslt, const EN_NLANALTYPE& enNLType, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhghFemaJ);
	
	virtual double GetAlphaConColm(const T_SECT_D& SectD) { ASSERT(0); return 0.0; }

	virtual void SetAccetanceCriteriaForcCtrl(OUT T_PHGT_FEMA& rPhgtD);
	virtual void SetAccetanceCriteriaConAxial(OUT T_PHGT_FEMA& rPhgtD);

	virtual BOOL CalcSeisEval2ndRsltConBeam(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_BEAM& raEval2nd) override final;
	virtual BOOL CalcSeisEval2ndRsltConBeam(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_BEAM& raEval2nd) override final;

	virtual BOOL CalcSeisEval2ndRsltConColmAgbp(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, int nSerialAgxp, OUT T_EVAL2ND_COLM& raEval2nd) override final;
	virtual BOOL CalcSeisEval2ndRsltConColmAgbp(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nSerialAgxp, OUT T_EVAL2ND_COLM& raEval2nd) override final;

	virtual BOOL CalcSeisEval2ndRsltConWallAgwp(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, int nSerialAgxp, OUT T_EVAL2ND_WALL& raEval2nd) override final;
	virtual BOOL CalcSeisEval2ndRsltConWallAgwp(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nSerialAgxp, OUT T_EVAL2ND_WALL& raEval2nd) override final;

	virtual BOOL CalcSeisEval2ndRsltConColmFiber(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, int nSerialAgxp, OUT T_EVAL2ND_COLM& raEval2nd) override final;
	virtual BOOL CalcSeisEval2ndRsltConColmFiber(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nSerialAgxp, OUT T_EVAL2ND_COLM& raEval2nd) override final;

	virtual BOOL CalcSeisEval2ndRsltConWallFiber(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, int nSerialAgxp, OUT T_EVAL2ND_WALL& raEval2nd) override final;
	virtual BOOL CalcSeisEval2ndRsltConWallFiber(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nSerialAgxp, OUT T_EVAL2ND_WALL& raEval2nd) override final;

	virtual BOOL CalcSeisEval2ndRsltConBeamTH(T_ELEM_K ElemK, T_THIS_K ThisK, int nHingeTimeIndex, OUT T_EVAL2ND_BEAM& raEval2nd) override final;
	
	virtual BOOL CalcSeisEval2ndRsltConColmTH(T_ELEM_K ElemK, T_THIS_K ThisK, int nHingeTimeIndex, int nSerialAgxt, OUT T_EVAL2ND_COLM& raEval2nd) override final;
	virtual BOOL CalcSeisEval2ndRsltConColmFiberTH(T_ELEM_K ElemK, T_THIS_K ThisK, int nHingeTimeIndex, int nSerialAgxt, OUT T_EVAL2ND_COLM& raEval2nd) override final;

	virtual bool ReCalcSeisEval2ndRsltConBeamLumpedTH(const T_EVAL2ND_BEAM_POS& InData, OUT T_EVAL2ND_BEAM_POS& rData) override final;
	virtual bool ReCalcSeisEval2ndRsltConColmLumpedTH(const int nDOF, const T_EVAL2ND_COLM_POS& InData, OUT T_EVAL2ND_COLM_POS& rData) override final;
	virtual bool ReCalcSeisEval2ndRsltConColmFiberTHRAngle(const T_EVAL2ND_COLM_POS& InData, OUT T_EVAL2ND_COLM_POS& rData) override final;
	virtual bool ReCalcSeisEval2ndRsltConColmFiberTHShear(const T_EVAL2ND_COLM_POS& InData, OUT T_EVAL2ND_COLM_POS& rData) override final;

	virtual BOOL CalcSeisEval2ndRsltConWallFiberTH(T_ELEM_K ElemK, T_THIS_K ThisK, int nHingeTimeIndex, int nSerialAgxt, OUT T_EVAL2ND_WALL& raEval2nd) override final;

	virtual bool ReCalcSeisEval2ndRsltConWallFiberTHRAngle(const T_EVAL2ND_WALL_POS& InData, OUT T_EVAL2ND_WALL_POS& rData) override final;
	virtual bool ReCalcSeisEval2ndRsltConWallFiberTHShear(const T_EVAL2ND_WALL_POS& InData, OUT T_EVAL2ND_WALL_POS& rData) override final;

	virtual BOOL GetLevelSeisEval2ndConFramTH(T_ELEM_K ElemK, T_THIS_K ThisK, int nHingeTimeIndex, int nDof, OUT UINT& runLevelI, OUT UINT& runLevelJ) override final;
    
    virtual BOOL IsIgnoreMomentHingeProp() override final;

	virtual BOOL GetSeisEval2ndRsltSubConFramTHFiberShear(T_ELEM_K ElemK, T_THIS_K ThisK, int nTimeIndex, int nHingeType, int nDof, OUT T_EVAL2ND_PHNE& rPhneDI, OUT T_EVAL2ND_PHNE& rPhneDJ) override final;
	virtual BOOL GetSeisEval2ndRsltSubConWallTHFiberShear(T_ELEM_K ElemK, T_THIS_K ThisK, int nTimeIndex, int nHingeType, int nDof, OUT T_EVAL2ND_WALL_POS& rPosI, OUT T_EVAL2ND_WALL_POS& rPosJ) override final;

	virtual double CalcVsmaxFram(const T_CON_SHR_PARAM& Shr) override;
	
private:
    double GetEc(double dEcAnal, double dfck);
    double CalcEs();
    double Calcks(double dd) const;
    double Calccu(const T_PDHG_CONBEAM_INFO& Info, bool bPositive) const;
    bool Findcu(const T_PDHG_CONBEAM_INFO& Info, bool bPositive, T_CU& stCu) const;
    double CalcNextcu(double dA, double dB, double dcu, double& dcu1, double& dcu2) const;
    double GetBeta1(double dfck) const;
    std::pair<double, double> CalcPlasticDeformationFactors(const T_PDHG_CONBEAM_INFO& Info, bool bPositive, OUT T_PDHG_CONBEAM_PARAM& rParam);

	UINT GetSeisDetailConBeam(T_ELEM_K ElemK);
	UINT GetSeisDetailConColm(T_ELEM_K ElemK);
	UINT GetSeisDetailConWall(T_ELEM_K ElemK);
	double GetLpRatioConWall(T_ELEM_K ElemK);

	bool IsSeisDetailConBeam(const T_PDHG_CONBEAM_INFO& PdghInfo);

	void GetShearSpanConBeam(T_ELEM_K ElemK, OUT double radLs[EN_MEMBEND_NUM]);
	void GetShearSpanConColm(T_ELEM_K ElemK, int nDof, OUT double radLs[EN_MEMBEND_NUM]);

	double GetAppSectionStiffnessFactor(T_ELEM_K ElemK, int nDOF);
	double GetFactorIeConBeam(T_ELEM_K ElemK);
	double GetFactorIeConColm(T_ELEM_K ElemK, EN_DIR enDir);
	double GetFactorIeConWall(T_ELEM_K ElemK);
		
	bool MakeBeamShearParam(T_ELEM_K ElemK, const EN_NLANALTYPE& enAnalType, OUT T_BEAM_SHR_PARAM raShrPos[EN_BEAM_POS_NUM]);
	bool MakeBeamInfo(T_ELEM_K ElemK, const EN_NLANALTYPE& enAnalType, double adRatioMuMy[EN_MEMBEND_NUM], double adV[EN_MEMBEND_NUM], double adM[EN_MEMBEND_NUM], OUT T_PDHG_CONBEAM_INFO raInfo[EN_MEMBEND_NUM]);
	
	bool MakeWallShearParam(T_ELEM_K ElemK, EN_DIR enDir, OUT T_WALL_SHR_PARAM raShr[EN_MEMBEND_NUM]);

	double CalcPnTensCon(double dfy, double dAs);
	double CalcPnCompCon(double dfck, double dAg, double dfy, double dAs, int nHoopType);
	
	int GetShearStrengthMethodConBeam(const EN_NLANALTYPE& enAnalType);

	int GetMomentMethodVcConBeam();
	double CalcSigmact(double dMu, double dbw, double dcu, double djdu, double dfck);

	double CalcVcConBeam(const T_PDHG_CONBEAM_INFO& Info, const T_CON_SHR_PARAM& Shr, int nShrMethod);
	double CalcVnConBeam(const T_PDHG_CONBEAM_INFO& Info, const T_CON_SHR_PARAM& Shr, int nShrMethod);
		
	double CalcVcConColm(const T_COLM_SHR_PARAM& Shr, double dPu);
	double CalcVnConColm(const T_COLM_SHR_PARAM& Shr, double dPu);

	double CalcVnConWall(const T_WALL_SHR_PARAM& Shr);
	
	double CalcmuConCom(UINT nSeisDetail, double dVc, double dVs, double dVy);

	BOOL CalcMyHingePropConBeamPos(const T_PDHG_CONBEAM_INFO& Info, double dCBRatio, double dc, OUT T_PHGT_FEMA& rPhgt);
	BOOL CalcMyzHingePropConColmPos(const T_PDHG_CONCOLM_INFO& PdghInfo, const T_PDHG_CONCOLM_DIR& PdhgDir, double dCBRatio, double dc, OUT T_PHGT_FEMA& rPhgt);
		
	void SetCurveFromTableConBeam(double daN, double dbN, double dThetayN, double daP, double dbP, double dThetayP, double dCBRatio, double dc, OUT double radCurv[2][9]);
	void SetCurveFromTableConColm(double da, double db, double dThetay, double dCBRatio, double dc, OUT double radCurv[2][9]);

	double CalcEnergyDissipationFactorConBeam(const T_PDHG_CONBEAM_INFO& Info);
	double CalcEnergyDissipationFactorConColm(const T_PDHG_CONCOLM_INFO& Info, const T_PDHG_CONCOLM_DIR& Dir);
	void SetPhgtEnergyDissipationOption(double dFactor, OUT T_PHGT_FEMA_ENRG_DISSIPATION& rPhgtEnrg);

	UINT GetPOThetayConColmAnalType();
	UINT GetTHThetayConColmAnalType();

	double CalcThetayConColm(UINT unAnalRsltType, double dAnalDy, double dMn, double dls, double dE, double dIe);
	double CalcThetaaConColm(double dNu, double dVs, double dThetay);
	double CalcThetauConColm(double dmu, double dThetay, double dThetaa);
	double CalcThetapConColm(double dThetamax, double dThetay);

	void CalcSeisEval2ndRsltConColmDir(UINT unAnalRsltType, const T_PDHG_CONCOLM_INFO& PdhgInfo, const T_COLM_SHR_PARAM& ShrParam, bool bFiber, double dAnalThetay, double dThetamax,
		const T_PDHG_CONCOLM_DIR& PdhgDir, OUT T_EVAL2ND_COLM_DIR& rDir, OUT T_EVAL2ND_PHNE& rPhneD);

	double CalcAcceptableCriteriaCP(double dLS, double dCP);
	void CalcAcceptablePlasticHingeRotationBeam(UINT unCtrlType, bool dsGTd2, double da, double db, double dThetay, double dIOmin, OUT double& rdIO, OUT double& rdLS, OUT double& rdCP);
    void CalcAcceptablePlasticHingeRotationColm(double da, double db, double dThetay, OUT double& rdIO, OUT double& rdLS, OUT double& rdCP);
	void CalcAcceptablePlasticHingeRotationColmFiber(double da, double db, double dThetay, OUT double& rdIO, OUT double& rdLS, OUT double& rdCP);
	void CalcAcceptablePlasticHingeRotationWall(UINT unSeisDetail, double dAxialRatio, double dShearRatio, OUT double& rdIO, OUT double& rdLS, OUT double& rdCP);

	UINT GetPerformanceLevel(double dIndex, double dIO, double dLS, double dCP);
	UINT GetPerformanceLevel(double dIndex);

#pragma region /// NOT SUPPORET THIS CODE

	virtual BOOL InitializeHingeConWall(const BOOL abDof[6], OUT T_PHGT_D& rPhgtD) { return FALSE; }
	virtual BOOL CalcFxHingePropConWall(T_ELEM_K ElemK, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhgtFemaJ) { return FALSE; }
	virtual BOOL CalcFyHingePropConWall(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhghFemaJ) { return FALSE; }
	virtual BOOL CalcFzHingePropConWall(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhghFemaJ) { return FALSE; }
	virtual BOOL CalcMyHingePropConWall(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhghFemaJ) { return FALSE; }
	virtual BOOL CalcMzHingePropConWall(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhghFemaJ) { return FALSE; }
	
	virtual double GetMFactForc(const EN_LEVEL enLevel) { ASSERT(0); return 0.0; }
	virtual double CalcK1(const T_COLM_SHR_PARAM& ColmShr) { ASSERT(0); return 0.0; }

	virtual BOOL CalcMyHingePropStlBeam(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhghFemaJ) { ASSERT(0); return FALSE; }

	virtual BOOL CalcFxHingePropStlColm(T_ELEM_K ElemK, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhgtFemaJ) { ASSERT(0); return FALSE; }
	virtual BOOL CalcMyzHingePropStlColm(T_ELEM_K ElemK, UINT unDir, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhghFemaJ) { ASSERT(0); return FALSE; }

	virtual BOOL CalcFxHingePropStlBrac(T_ELEM_K ElemK, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhgtFemaJ) { ASSERT(0); return FALSE; }

	virtual BOOL CalcMyHingePropSrcBeam(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhghFemaJ) { ASSERT(0); return FALSE; }

	virtual BOOL CalcFxHingePropSrcColm(T_ELEM_K ElemK, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhgtFemaJ) { ASSERT(0); return FALSE; }
	virtual BOOL CalcMyzHingePropSrcColm(T_ELEM_K ElemK, UINT unDir, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhghFemaJ) { ASSERT(0); return FALSE; }

#pragma endregion

private:
    BOOL m_bIgnoreMomentHingeProp;
    void SetIgnoreMomentHingeProp(BOOL bIgnoreMomentHingeProp) { m_bIgnoreMomentHingeProp = bIgnoreMomentHingeProp; }
    BOOL GetIgnoreMomentHingeProp() const { return m_bIgnoreMomentHingeProp; }
		
	bool IsIgnoreThetayFiberWall();
	bool IsSeismicDetail_1(const double dSpace, const double dd) const;
	bool IsSeismicDetail_2(const double dVs, const double dVu) const;

	T_THIS_K GetInitialLoadThisK(T_THIS_K ThisK);	

	double GetRatioMuMy(double dM, const double adForceMomentRatio[2][9]);
	bool GetRatioMuMyPhgt(T_ELEM_K ElemK, const double adM[2], OUT double radRatioMuMy[2]);
	bool GetRatioMuMyIehp(T_ELEM_K ElemK, const double adM[2], OUT double radRatioMuMy[2]);
	
	BOOL GetSeisEval2ndRsltSubConColmPOFiberShear(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, int nDof, OUT T_EVAL2ND_PHNE& rPhneDI, OUT T_EVAL2ND_PHNE& rPhneDJ);
	BOOL GetSeisEval2ndRsltSubConWallPOFiberShear(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, int nDof, OUT T_EVAL2ND_WALL_POS& rPosI, OUT T_EVAL2ND_WALL_POS& rPosJ);
};

