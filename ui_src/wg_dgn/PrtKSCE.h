#ifndef __PrtKSCE_H__
#define __PrtKSCE_H__

#include "..\wg_base\MSExcel.h"
#include "..\wg_base\XLParser.h"
#include "..\wg_db\DB_ST_DN_RESULT_PSC.H"

#include "DgnForceStressST.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma pack(1)

struct _PRT_SECT_BASE
{
	// 0=I, 1=J.
	double dArea[cPOSI_CRC];
	double dIy[cPOSI_CRC], dIz[cPOSI_CRC];
	double dZyt[cPOSI_CRC], dZyb[cPOSI_CRC];
	void Initialize()
	{
		for(int i=0; i<cPOSI_CRC; i++)
		{
			dArea[i]=0.0;
			dIy[i]=0.0; dIz[i]=0.0;
			dZyt[i]=0.0; dZyb[i]=0.0;
		}
	}
};

struct _PRT_FORC_BASE
{
	// 0=I, 1=J.
	BOOL bData;
	double dFxx[cPOSI_CRC], dMuy[cPOSI_CRC], dMuz[cPOSI_CRC];
	double dStl[cPOSI_CRC], dStr[cPOSI_CRC];
	double dSbl[cPOSI_CRC], dSbr[cPOSI_CRC];
	void Initialize()
	{
		bData = TRUE;
		for(int i=0; i<cPOSI_CRC; i++)
		{
			dFxx[i]=0.0; dMuy[i]=0.0; dMuz[i]=0.0;
			dStl[i]=0.0; dStr[i]=0.0;
			dSbl[i]=0.0; dSbr[i]=0.0;
		}
	}
	_PRT_FORC_BASE()	{}
	_PRT_FORC_BASE(const _PRT_FORC_BASE& rData)	{*this=rData;}
	_PRT_FORC_BASE& operator = (const _PRT_FORC_BASE& rData)
	{
		for(int i=0; i<cPOSI_CRC; i++)
		{
			dFxx[i]=rData.dFxx[i]; dMuy[i]=rData.dMuy[i]; dMuz[i]=rData.dMuz[i];
			dStl[i]=rData.dStl[i]; dStr[i]=rData.dStr[i];
			dSbl[i]=rData.dSbl[i]; dSbr[i]=rData.dSbr[i];
		}
		return *this;
	}
	_PRT_FORC_BASE& operator += (const _PRT_FORC_BASE& rData)
	{
		for(int i=0; i<cPOSI_CRC; i++)
		{
			dFxx[i]+=rData.dFxx[i]; dMuy[i]+=rData.dMuy[i]; dMuz[i]+=rData.dMuz[i];
			dStl[i]+=rData.dStl[i]; dStr[i]+=rData.dStr[i];
			dSbl[i]+=rData.dSbl[i]; dSbr[i]+=rData.dSbr[i];
		}
		return *this;
	}
	_PRT_FORC_BASE& operator -= (const _PRT_FORC_BASE& rData)
	{
		for(int i=0; i<cPOSI_CRC; i++)
		{
			dFxx[i]-=rData.dFxx[i]; dMuy[i]-=rData.dMuy[i]; dMuz[i]-=rData.dMuz[i];
			dStl[i]-=rData.dStl[i]; dStr[i]-=rData.dStr[i];
			dSbl[i]-=rData.dSbl[i]; dSbr[i]-=rData.dSbr[i];
		}
		return *this;
	}
};

struct _PRT_FORC_DETL_SECT
{
	// 단면속성.
	// 0=Gross, 1=Net, 2=CPR.
	_PRT_SECT_BASE SectBase[3];
	void Initialize()
	{
		for(int i=0; i<3; i++)
		{
			SectBase[i].Initialize();
		}
	}
};

struct _PRT_FORC_DETL_STAG
{
	// 최종 시공단계 부재력.
	_PRT_FORC_BASE ForcDL;
	_PRT_FORC_BASE ForcEL;
	_PRT_FORC_BASE ForcTP, ForcTS;
	_PRT_FORC_BASE ForcCSP, ForcCSS;
	_PRT_FORC_BASE ForcSum;
	void Initialize()
	{
		ForcDL.Initialize();
		ForcEL.Initialize();
		ForcTP.Initialize(); ForcTS.Initialize();
		ForcCSP.Initialize(); ForcCSS.Initialize();
		ForcSum.Initialize();
	}
};

struct _PRT_FORC_DETL_LCAS
{
	// 사용하중조건종류별 부재력 (시공단계 부재력 제외).
	_PRT_FORC_BASE ForcDL;									// 고정하중
	_PRT_FORC_BASE ForcLLmax, ForcLLmin;		// 활하중
	_PRT_FORC_BASE ForcADDmax, ForcADDmin;	// 기타하중
	void Initialize()
	{
		ForcDL.Initialize();
		ForcLLmax.Initialize(); ForcLLmin.Initialize();
		ForcADDmax.Initialize(); ForcADDmin.Initialize();
	}
};

struct _PRT_FORC_DETL_LCOM
{
	// 사용하중조합종류별 부재력.
	_PRT_FORC_BASE ForcD;					// PS+DL+C+S.
	_PRT_FORC_BASE ForcDLmax;			// PS+DL+C+S+LLmax.
	_PRT_FORC_BASE ForcDLmin;			// PS+DL+C+S+LLmin.
	_PRT_FORC_BASE ForcDLAmax;		// PS+DL+C+S+(LL+ST)max+W+T.
	_PRT_FORC_BASE ForcDLAmin;		// PS+DL+C+S+(LL+ST)min+W+T.
	void Initialize()
	{
		ForcD.Initialize();
		ForcDLmax.Initialize();
		ForcDLmin.Initialize();
		ForcDLAmax.Initialize();
		ForcDLAmin.Initialize();
	}
};

struct _PRT_FORC_GRPH_ELEM
{
	// 시공단계별, 사용하중조합별 합성응력도.
	T_ELEM_K ElemK;
	double dLeng;
	// 0=I, 1=J.
	double dStl[cPOSI_CRC], dStr[cPOSI_CRC];	  // 발생 Top Left/Right.
	double dSbl[cPOSI_CRC], dSbr[cPOSI_CRC];	  // 발생 Bot Left/Right.
	double dAlc[cPOSI_CRC], dAlt[cPOSI_CRC][2];	// 허용 Comp/Tens. Alt[2] = Top/Bot.
	void Initialize()
	{
		ElemK=0;
		dLeng=0.0;
		for(int i=0; i<cPOSI_CRC; i++)
		{
			dStl[i]=0.0; dStr[i]=0.0;
			dSbl[i]=0.0; dSbr[i]=0.0;
			dAlc[i]=0.0; 
			dAlt[i][0]=0.0, dAlt[i][1]=0.0;;
		}
	}
	void Initialize(BOOL bMax)
	{
		ElemK=0;
		dLeng=0.0;
		for(int i=0; i<cPOSI_CRC; i++)
		{
			if(bMax)
			{ 
				dStl[i]=(-1)/pow(cDGN_Zero,3); dStr[i]=(-1)/pow(cDGN_Zero,3);
				dSbl[i]=(-1)/pow(cDGN_Zero,3); dSbr[i]=(-1)/pow(cDGN_Zero,3);
				dAlc[i]=(-1)/pow(cDGN_Zero,3); 
				dAlt[i][0]=(-1)/pow(cDGN_Zero,3);
				dAlt[i][1]=(-1)/pow(cDGN_Zero,3);
			}
			else
			{
				dStl[i]=(+1)/pow(cDGN_Zero,3); dStr[i]=(+1)/pow(cDGN_Zero,3);
				dSbl[i]=(+1)/pow(cDGN_Zero,3); dSbr[i]=(+1)/pow(cDGN_Zero,3);
				dAlc[i]=(+1)/pow(cDGN_Zero,3); 
				dAlt[i][0]=(+1)/pow(cDGN_Zero,3);
				dAlt[i][1]=(+1)/pow(cDGN_Zero,3);
			}
		}
	}
};

struct _PRT_FORC_GRPH_GRUP
{
	double dExtraRat;
	CArray<_PRT_FORC_GRPH_ELEM,_PRT_FORC_GRPH_ELEM> arGrphElemMax;
	CArray<_PRT_FORC_GRPH_ELEM,_PRT_FORC_GRPH_ELEM> arGrphElemMin;
	void Initialize()
	{
		dExtraRat=0.0;
		arGrphElemMax.RemoveAll();
		arGrphElemMin.RemoveAll();
	}
	_PRT_FORC_GRPH_GRUP() {}
	_PRT_FORC_GRPH_GRUP(_PRT_FORC_GRPH_GRUP& rData)	{*this = rData;}
	_PRT_FORC_GRPH_GRUP& operator = (_PRT_FORC_GRPH_GRUP& rData)
	{
		dExtraRat = rData.dExtraRat;
		arGrphElemMax.Copy(rData.arGrphElemMax);
		arGrphElemMin.Copy(rData.arGrphElemMin);
		return *this;
	}
};

struct _PRT_FORC_GRPH_TOTL
{
	// 시공단계별, 하중조합별 합성응력도.
	CString strTitle;
	// 전체(요소+위치)에서 가장 불리한 합성응력도.
	double dStc, dAltc;	// Top Comp.
	double dStt, dAltt;	// Top Tens.
	double dSbc, dAlbc;	// Bot Comp.
	double dSbt, dAlbt;	// Bot Tens.
	void Initialize()
	{
		dStc=0.0; dAltc=0.0;
		dStt=0.0; dAltt=0.0;
		dSbc=0.0; dAlbc=0.0;
		dSbt=0.0; dAlbt=0.0;
	}
};

struct _PRT_FORC_UENV_ELEM	// Ultimate Envelope.
{
	// 계수하중조합 중 최대/최소 부재력.
	T_ELEM_K ElemK;
	T_LCOM_K LcomKMy[cPOSI_CRC];
	T_LCOM_K LcomKVz[cPOSI_CRC];
	T_LCOM_K LcomKMx[cPOSI_CRC];
	_DGN_FORC_CRC ForcMy;	// Forces if My.
	_DGN_FORC_CRC ForcVz;	// Forces if Vz.
	_DGN_FORC_CRC ForcMx;	// Forces if Mx.
	void Initialize()
	{
		ElemK=0;
		for(int i=0; i<cPOSI_CRC; i++)
		{
			LcomKMy[i]=0;
			LcomKVz[i]=0;
			LcomKMx[i]=0;
		}
		ForcMy.Initialize();
		ForcVz.Initialize();
		ForcMx.Initialize();
	}
	_PRT_FORC_UENV_ELEM() {}
	_PRT_FORC_UENV_ELEM(_PRT_FORC_UENV_ELEM& rData)	{*this = rData;}
	_PRT_FORC_UENV_ELEM& operator = (_PRT_FORC_UENV_ELEM& rData)
	{
		ElemK = rData.ElemK;
		for(int i=0; i<cPOSI_CRC; i++)
		{
			LcomKMy[i]=rData.LcomKMy[i];
			LcomKVz[i]=rData.LcomKVz[i];
			LcomKMx[i]=rData.LcomKMx[i];
		}
		ForcMy=rData.ForcMy;
		ForcVz=rData.ForcVz;
		ForcMx=rData.ForcMx;
		return *this;
	}
};

struct PRT_BDCY_KSCE
{
	// 위치별 최대강도비 결과
	T_BDCY_KSCE_BASE BdcyBase[2];	// 0=I, 1=J.
	void Initialize()
	{
		BdcyBase[0].Initialize();
		BdcyBase[1].Initialize();
	}
};

struct PRT_CUMS_KSCE
{
	// 위치별 최대강도비 결과
	T_CUMS_KSCE_BASE CumsBase[2];	// 0=I, 1=J.
	void Initialize()
	{
		CumsBase[0].Initialize();
		CumsBase[1].Initialize();
	}
};

struct PRT_CRMT_KSCE
{
	// 위치별 최대강도비 결과
	T_CRMT_KSCE_BASE CrmtBase[2];	// 0=I, 1=J.
	void Initialize()
	{
		CrmtBase[0].Initialize();
		CrmtBase[1].Initialize();
	}
};

struct PRT_BDCY_PARA
{
	int iCount;
	double dfck, db, dbw, dAps;
	double dfy, ddp, dAs;
	double dfpu, dd, dAc;
	double dIy, dyb, dPe;
	double dbeta1, dep, dMu;
	double drp, dhf;
	// 도로교 10
	double dBc;
	double dtc;
	double dHh;
	double dH_gr;

	BOOL bIter;
	BOOL bOneType;
	BOOL bBondType;
	BOOL bApwM;    
	double dNeuC;  
	double daArea; 
	double dCc;    
	double dRhop;
	double dfpy_unbond;
	double dfpe_unb_cal;
	double dfpe_unb;
	double d05fpu;
	double dfps_cal;
	double dfps_un_com;
	double dfps_un_max;
	double dMn_ps;
	double dApw; 
	double dApf;
	double dMn_Cal;
	double dphiMn_Cal;
	double dPe_t;
	double dPeep;
	double dMcr;
	double dTp  ;     
	double dTpTimesep;
	double dPeTimesep;
	double dEpsi_cu ;
	double dEpsi_st ;
	double dfst     ;
	double dfst_cal ;
	double dEs      ;
	double dTst     ;
	double daCent;
	double dMps;
	double dphiMn_beta1;
	double dfps_temp_cal;
	double dfps_temp;    

	
	CArray<CString,CString> strTndn;
	CArray<double,double> arfps0;
	CArray<double,double> arfps1;
	CArray<double,double> arfps2;
	CArray<double,double> arfps3;
	CArray<double,double> arfps4;
	CArray<double,double> arfps5;
	CArray<double,double> arfps6;
	CArray<double,double> arfps7;
	CArray<double,double> arfps8;


	void Initialize()
	{
		iCount=0;
		dfck = db = dbw = dAps = 0.0;
		dfy = ddp = dAs = 0.0;
		dfpu = dd = dAc = 0.0;
		dIy = dyb = dPe = 0.0;
		dbeta1 = dep = dMu = 0.0;
		drp = dhf = 0.0;

		dBc= 0.0;
		dtc= 0.0;
		dHh= 0.0;
		dH_gr= 0.0;

		bIter = FALSE;
		bOneType = FALSE;
		bBondType = FALSE;
		bApwM = FALSE;    
		dNeuC = 0.0;  
		daArea= 0.0;   
		dCc= 0.0;      
		dRhop =0.0;
		dfpy_unbond = 0.0;
		dfpe_unb_cal =0.0;
		dfpe_unb = 0.0;
		d05fpu = 0.0;
		dfps_cal = 0.0;
		dfps_un_com = 0.0;
		dfps_un_max = 0.0;
		dMn_ps = 0.0;
		dApw = 0.0;
		dApf = 0.0;
		dMn_Cal = 0.0;
		dphiMn_Cal = 0.0;
		dPe_t = 0.0;
		dPeep = 0.0;
		dMcr = 0.0;
		dTp  = 0.0;     
		dTpTimesep= 0.0;
		dPeTimesep= 0.0;
		dEpsi_cu = 0.0;
		dEpsi_st = 0.0;
		dfst     = 0.0;
		dfst_cal = 0.0;
		dEs      = 0.0;
		dTst     = 0.0;
		daCent= 0.0;
		dMps= 0.0;
		dphiMn_beta1 =0.0;
		dfps_temp_cal=0.0;
		dfps_temp    =0.0; 
		
		strTndn.RemoveAll();
		arfps0.RemoveAll();
		arfps1.RemoveAll();
		arfps2.RemoveAll();
		arfps3.RemoveAll();
		arfps4.RemoveAll();
		arfps5.RemoveAll();
		arfps6.RemoveAll();
		arfps7.RemoveAll();
		arfps8.RemoveAll();
	}
	 
};

struct PRT_CUMS_PARA
{
	double dfck, dbw, ddst;
	double dfy, dh, dIy;
	double dfpu, ddp, dAc;
	double dfpy, dyt, dAps;
	double ddp_org;
	// 도로교10
	double dMu;
	double dPe;
	double dVci_cal;
	double dVci_incl;
	double dVci_min;
	double dphiv;
	double dpVc;
	double d05pVc; 
	double dVs_req;
	double dVsmax; 
	double dVs_min;
	double dVs_cal;
	double dAvmin01; 
	double dAvmin02; 

	void Initialize()
	{
		dfck = dbw = ddst = 0.0;
		dfy = dh = dIy = 0.0;
		dfpu = ddp = dAc = 0.0;
		dfpy = dyt = dAps = 0.0;
		ddp_org =0.0;

		dMu =0.0;
		dPe =0.0;
		dVci_cal = 0.0;
		dVci_incl= 0.0; 
		dVci_min = 0.0;

		dphiv   = 0.0;
		dpVc    = 0.0;
		d05pVc  = 0.0;
		dVs_req = 0.0;
		dVsmax  = 0.0;
		dVs_min = 0.0;
		dVs_cal = 0.0;
		dAvmin01= 0.0; 
		dAvmin02= 0.0; 
	}
};

struct PRT_CRMT_PARA
{
	double dfck, dbw, dt;
	double dfy, dh, dIy;
	double dfpu, dd, ddp;
	double dd_org, ddp_org;
	double dAc, dfpy, dyt;
	double dAps, dPcp, dAcp;
	
	// 도로교10
	double dMu  ;
	double dMd  ;
	double dVd  ;
	double dfd  ;
	double dPe  ;
	double dfpe ;
	double dTcr;
	double dphit;
	BOOL bTorsionChk;
	double dMcr;
	double dMmax;
	double dVi;
	double dVci_cal;
	double dVci;
	double dVci_incl;
	double dVci_min;
	double dVp;
	double dVcw;
	BOOL bClosedSect;
	double dAohOverPh;
	double dA0;
	double dst;
	double dTn;
	double dAlmin_cal;
	double dAv;
	double dAtmin01;  
	double dAtmin02;  
	double dAtmin_cal;
	double dAtst;  
	double dbwfyv; 
	double AtOverS;

 
	void Initialize()
	{
		dfck = dbw = dt = 0.0;
		dfy = dh = dIy = 0.0;
		dfpu = dd = ddp = 0.0;
		dd_org = ddp_org =0.0;
		dAc = dfpy = dyt = 0.0;
		dfpy = dyt = dAps = 0.0;

		dMu  = 0.0;
		dMd  = 0.0;
		dVd  = 0.0;
		dfd  = 0.0;
		dPe  = 0.0;
		dfpe = 0.0;
		dTcr = 0.0;
		dphit = 0.0;
		bTorsionChk = FALSE;
		dMcr = 0.0;
		dMmax= 0.0;
		dVi= 0.0;
		dVci_cal= 0.0;
		dVci= 0.0;
		dVci_incl= 0.0;
		dVci_min= 0.0;
		dVp= 0.0;
		dVcw= 0.0;
		bClosedSect = FALSE;
		dAohOverPh = 0.0;
		dA0 = 0.0;
		dst = 0.0;
		dTn = 0.0;
		dAlmin_cal = 0.0;
		dAv = 0.0;
		dAtmin01 = 0.0;  
		dAtmin02 = 0.0;  
		dAtmin_cal = 0.0;
		dAtst = 0.0;  
		dbwfyv = 0.0; 
		AtOverS = 0.0;

	}
};

struct Prt_ArString
{
	CArray<CString, CString> arString;
	void Initialize()
	{
		arString.RemoveAll();
	}
	Prt_ArString() {}
	Prt_ArString(Prt_ArString& rData)	{*this = rData;}
	Prt_ArString& operator = (Prt_ArString& rData)
	{
		arString.Copy(rData.arString);
		return *this;
	}
};

struct Prt_ArString_Detail
{
	int iFormula;
	Prt_ArString ArString;
	void Initialize()
	{
		iFormula = 0;
		ArString.Initialize();
	}
	Prt_ArString_Detail() {}
	Prt_ArString_Detail(Prt_ArString_Detail& rData)	{*this = rData;}
	Prt_ArString_Detail& operator = (Prt_ArString_Detail& rData)
	{
		iFormula = rData.iFormula;
		ArString = rData.ArString;
		return *this;
	}  
};

struct Prt_Graph_Data
{ 
	int iCurrSheet;
	CArray<CString,CString> arStrTitle;
	CArray<CString,CString> arStrCol;
	CArray<double,double>   arValue;
	CArray<ChartData,ChartData> arChartData;
	void Initialize()
	{    
		iCurrSheet=0;
		arStrTitle.RemoveAll();
		arStrCol.RemoveAll();
		arValue.RemoveAll();
		arChartData.RemoveAll();
	}
	Prt_Graph_Data() {}
	Prt_Graph_Data(Prt_Graph_Data& rData)	{*this = rData;}
	Prt_Graph_Data& operator = (Prt_Graph_Data& rData)
	{
		iCurrSheet = rData.iCurrSheet;
		arStrTitle.Copy(rData.arStrTitle);
		arStrCol.Copy(rData.arStrCol);
		arValue.Copy(rData.arValue);
		arChartData.Copy(rData.arChartData);
		return *this;
	}

};

#pragma pack()

#endif
