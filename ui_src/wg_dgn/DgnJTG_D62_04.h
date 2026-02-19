// DgnJTG_D62_04.h: interface for the CDgnJTG_D62_04 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGNJTG_D62_04_H__D2C256BA_89F4_41BF_8CF2_0D100EBF4C73__INCLUDED_)
#define AFX_DGNJTG_D62_04_H__D2C256BA_89F4_41BF_8CF2_0D100EBF4C73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnPscCommon.h"
#include "DgnStruct.h"

struct _RPT_FLEX_DATA
{
	double dx;
	double dTst, dTps;
	double dCc, dCsc, dCps;
	double dTens, dComp;
	double dTolerance;
	
	void Initialize()
	{
		dx=0.0;
		dTst=dTps=0.0;
		dCc=dCsc=dCps=0.0;
		dTens=dComp=0.0;
		dTolerance=0.0;
		
	}

};

struct _RPT_BDCY_BASE
{
	BOOL bChk;
	double dxib;
	double dfcd;
	double dfsdt, dfsdc;
	double dxCal,dx,dxb;
	double db,dbw,dh,dh0,dhf;
	double dat,dac,dast,dasc;
	double dMd, dr0Md, dMn;
	double dTndnComp;
	double dMnCon;
	double dMnBarC;
	double dMnPsC;
	double dMnPsT;
	double dMnBarT;
	double dMnPsTs;
	double dMnBarTs;
	double dMnPsCs;

	CArray<_RPT_FLEX_DATA, _RPT_FLEX_DATA&> arFlex;

	CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbar[2];
	CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndn[2];
	
	void Initialize()
	{
		bChk=TRUE;
		dxib=0.0;
		dfcd=0.0;
		dfsdt=dfsdc=0.0;
		dxCal=dx=dxb=0.0;
		db=dbw=dh=dh0=dhf=0.0;
		dat=dac=dast=dasc=0.0;
		dMd=dr0Md=dMn=0.0;
		dTndnComp=0.0;
		dMnCon  = 0.0;
		dMnBarC = 0.0;
		dMnPsC  = 0.0;
		dMnPsT  = 0.0;
		dMnBarT = 0.0;
		dMnPsTs = 0.0;
		dMnBarTs= 0.0;
		dMnPsCs = 0.0;

		arFlex.RemoveAll();

		for(int i=0; i<2; i++)
		{
			arRbar[i].RemoveAll();
			arTndn[i].RemoveAll();
		}
	}

	_RPT_BDCY_BASE& operator = (const _RPT_BDCY_BASE& rData)
	{
		bChk  = rData.bChk;
		dxib  = rData.dxib;
		dfcd  = rData.dfcd;
		dfsdt = rData.dfsdt;
		dfsdc = rData.dfsdc;
		dxCal = rData.dxCal;
		dx    = rData.dx;
		dxb   = rData.dxb;
		db    = rData.db;
		dbw   = rData.dbw;
		dh    = rData.dh;
		dh0   = rData.dh0;
		dhf   = rData.dhf;
		dat   = rData.dat;
		dac   = rData.dac;
		dast  = rData.dast;
		dasc  = rData.dasc;
		dMd   = rData.dMd;
		dr0Md = rData.dr0Md;
		dMn   = rData.dMn;
		dTndnComp = rData.dTndnComp;
		dMnCon  = rData.dMnCon ;
		dMnBarC = rData.dMnBarC;
		dMnPsC  = rData.dMnPsC ;
		dMnPsT  = rData.dMnPsT ;
		dMnBarT = rData.dMnBarT;
		dMnPsTs = rData.dMnPsTs;
		dMnBarTs= rData.dMnBarTs;
		dMnPsCs = rData.dMnPsCs;

		arFlex.Copy(rData.arFlex);

		for(int i=0; i<2; i++)
		{
			arRbar[i].Copy(rData.arRbar[i]);
			arTndn[i].Copy(rData.arTndn[i]);
		}

		return *this;
	}  

};

struct _RPT_BDCY_JTG
{
	_RPT_BDCY_BASE BdcyBase[4];

	void Initialize()
	{
		for(int i=0; i<4; i++)
			BdcyBase[i].Initialize();    
	}

	_RPT_BDCY_JTG& operator = (const _RPT_BDCY_JTG& rData)
	{
		for(int i=0; i<4; i++)
			BdcyBase[i] = rData.BdcyBase[i];
		
		return *this;
	}  

};

struct _RPT_TNDN_CUMS
{
	BOOL bSlope;
	CString strTndnGroup;
	double dfpd;
	double dAp;
	double dThetap;  

	void Initialize()
	{
		bSlope = FALSE;
		strTndnGroup.Empty();
		dfpd    = 0.0;
		dAp     = 0.0;
		dThetap = 0.0;
	}  
};

// 전단강도
struct _RPT_CUMS_BASE
{
	BOOL bChk;
	double dfcuk, dftd, dfsdt;
	double dfsv;
	double db,dbw,dh,dh0,dhf;
	double dAsv, dSv_s, dThetas;
	double dAsb, dSv_b;  
	//double dat,dac,dast,dasc;
	double dMd, dVd, dr0Vd, dVn;
	double dalp1, dalp2, dalp3;
	double dVcs, dVsb, dVpb;
	double dAp, dApb, dAst;
	double dP, dPcal, dRho, dRhosv;
	double dm, dm_cal;
	double dC;
	int iAsbN;
	double dShrSect;
	double dShrLoadSkip;

	CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarT;
	CArray<_RPT_TNDN_CUMS,_RPT_TNDN_CUMS&> arTndnCums;
		
	
	void Initialize()
	{
		bChk=TRUE;
		dfcuk=dftd=dfsdt=0.0;
		dfsv=0.0;
		db=dbw=dh=dh0=dhf=0.0;
		dAsv=dSv_s=dThetas=0.0;
		dAsb=dSv_b=0.0;    
		dMd=dVd=dr0Vd=dVn=0.0;
		dalp1=dalp2=dalp3=0.0;
		dVcs=dVsb=dVpb=0.0;
		dAp=dApb=dAst=0.0;
		dP=dPcal=dRho=dRhosv=0.0;
		dm=dm_cal=0.0;
		dC=0.0;
		iAsbN=0;
		dShrSect=0.0;
		dShrLoadSkip=0.0;


		arRbarT.RemoveAll();
		arTndnCums.RemoveAll();
	}

	_RPT_CUMS_BASE& operator = (const _RPT_CUMS_BASE& rData)
	{
		bChk  = rData.bChk;    
		dalp1 = rData.dalp1;
		dalp2 = rData.dalp2;
		dalp3 = rData.dalp3;
		dfcuk = rData.dfcuk;
		dftd  = rData.dftd;
		dfsdt = rData.dfsdt;
		dfsv  = rData.dfsv;
		db    = rData.db;
		dbw   = rData.dbw;
		dh    = rData.dh;
		dh0   = rData.dh0;
		dhf   = rData.dhf;
		dAsv  = rData.dAsv;
		dSv_s = rData.dSv_s;
		dThetas=rData.dThetas;
		dAsb  = rData.dAsb;
		dSv_b = rData.dSv_b;
		dMd   = rData.dMd;
		dVd   = rData.dVd;
		dr0Vd = rData.dr0Vd;
		dVn   = rData.dVn;
		dVcs  = rData.dVcs;
		dVsb  = rData.dVsb;
		dVpb  = rData.dVpb;
		dAp   = rData.dAp;
		dApb  = rData.dApb;
		dAst  = rData.dAst;
		dP    = rData.dP;
		dPcal = rData.dPcal;
		dRho  = rData.dRho;
		dRhosv= rData.dRhosv;
		dm    = rData.dm;
		dm_cal= rData.dm_cal;
		dC    = rData.dC;
		iAsbN = rData.iAsbN;
		dShrSect = rData.dShrSect;
		dShrLoadSkip = rData.dShrLoadSkip;

		for(int i=0; i<2; i++)
		{
			arRbarT.Copy(rData.arRbarT);
			arTndnCums.Copy(rData.arTndnCums);
		}

		return *this;
	}  

};

struct _RPT_CUMS_JTG
{
	_RPT_CUMS_BASE CumsBase[4];

	void Initialize()
	{
		for(int i=0; i<4; i++)
			CumsBase[i].Initialize();    
	}

	_RPT_CUMS_JTG& operator = (const _RPT_CUMS_JTG& rData)
	{
		for(int i=0; i<4; i++)
			CumsBase[i] = rData.CumsBase[i];
		
		return *this;
	}  

};

// 비틀림 강도
struct _RPT_CRMT_BASE
{
	BOOL bChk;
	double dfcuk, dftd, dfsdt;
	double dfsv;
	double db,dbw,dh,dh0,dhf;
	double dAsv, dSv_s, dThetas;
	double dMd, dVd, dr0Vd, dVn;
	double dr0Td,dTn;
	double dalp1, dalp2, dalp3;  
	double dAst;
	double dAp, dApb;
	double dP, dPcal, dRho, dRhosv;
	double dSTstress;
	double dTorSect;
	double dTorLoadSkip;
	double dAsv1;
	double dAslt;
	double dSv_t; 
	double dBetaa;
	double dBetat;
	double dWt;
	double dAcor;
	double dUcor;
	double dNp0;
	double dep0;
	double dA0;
	BOOL bPureTorsion;
	double dZeta;
	double dZetaCal;
	BOOL bSolid;

	//CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarT;
	CArray<_RPT_TNDN_CUMS,_RPT_TNDN_CUMS&> arTndnCums;    
	
	void Initialize()
	{
		bChk=TRUE;
		dfcuk=dftd=dfsdt=0.0;
		dfsv=0.0;
		db=dbw=dh=dh0=dhf=0.0;
		dAsv=dSv_s=dThetas=0.0;
		dMd=dVd=dr0Vd=dVn=0.0;
		dr0Td=dTn=0.0;
		dalp1=dalp2=dalp3=0.0;    
		dAst=0.0;
		dP=dPcal=dRho=dRhosv=0.0;
		dSTstress=0.0;
		dTorSect=0.0;
		dTorLoadSkip=0.0;
		dAsv1=0.0;
		dAslt=0.0;
		dSv_t=0.0; 
		dBetaa=0.0;
		dBetat=0.0;
		dWt   =0.0;
		dAcor =0.0;
		dUcor =0.0;
		dNp0  = 0.0;
		dep0  = 0.0;
		dA0   = 0.0;
		bPureTorsion = FALSE;
		dZeta = 0.0;
		dZetaCal = 0.0;
		bSolid = TRUE;

		//arRbarT.RemoveAll();
		arTndnCums.RemoveAll();
	}

	_RPT_CRMT_BASE& operator = (const _RPT_CRMT_BASE& rData)
	{
		bChk  = rData.bChk;    
		dalp1 = rData.dalp1;
		dalp2 = rData.dalp2;
		dalp3 = rData.dalp3;
		dfcuk = rData.dfcuk;
		dftd  = rData.dftd;
		dfsdt = rData.dfsdt;
		dfsv  = rData.dfsv;
		db    = rData.db;
		dbw   = rData.dbw;
		dh    = rData.dh;
		dh0   = rData.dh0;
		dhf   = rData.dhf;
		dAsv  = rData.dAsv;
		dSv_s = rData.dSv_s;
		dMd   = rData.dMd;
		dVd   = rData.dVd;
		dr0Vd = rData.dr0Vd;
		dVn   = rData.dVn;
		dr0Td = rData.dr0Td;
		dTn   = rData.dTn;    
		dAst  = rData.dAst;
		dAp   = rData.dAp;
		dApb  = rData.dApb;
		dP    = rData.dP;
		dPcal = rData.dPcal;
		dRho  = rData.dRho;
		dRhosv= rData.dRhosv;
		dSTstress = rData.dSTstress;
		dTorSect = rData.dTorSect;
		dTorLoadSkip = rData.dTorLoadSkip;
		dAsv1 = rData.dAsv1;
		dAslt = rData.dAslt;
		dSv_t = rData.dSv_t; 
		dBetaa= rData.dBetaa;
		dBetat= rData.dBetat;
		dWt   = rData.dWt;
		dAcor = rData.dAcor;
		dUcor = rData.dUcor;
		dNp0  = rData.dNp0;
		dep0  = rData.dep0;
		dA0   = rData.dA0;
		bPureTorsion = rData.bPureTorsion;
		dZeta = rData.dZeta;
		dZetaCal = rData.dZetaCal;
		bSolid = rData.bSolid;

		//arRbarT.Copy(rData.arRbarT);
		arTndnCums.Copy(rData.arTndnCums);

		return *this;
	}  

};

struct _RPT_CRMT_JTG
{
	_RPT_CRMT_BASE CrmtBase[6];

	void Initialize()
	{
		for(int i=0; i<6; i++)
			CrmtBase[i].Initialize();    
	}

	_RPT_CRMT_JTG& operator = (const _RPT_CRMT_JTG& rData)
	{
		for(int i=0; i<6; i++)
			CrmtBase[i] = rData.CrmtBase[i];
		
		return *this;
	}  

};

//////////////////////////////////////////////////////////////////////////

struct _DGN_FYSG_D
{
	BOOL bPre;
	int j;
	double dfck_p, dftk_p, dAg;
	T_SPSC_SECT SpscSect;
	CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
	CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnPosi;

	void Initialize()
	{
		bPre = TRUE;
		j = 0;
		dfck_p = dftk_p = dAg =.0;
		SpscSect.Initialize();
		arRbarPosi.RemoveAll();
		arTndnPosi.RemoveAll();
	}
};

struct _DGN_FYPC_D
{
	_PSC_TNDN_TDNA tnsn;
	
	void Initialize()
	{
		tnsn.Initialize();
	}
};

struct _DGN_FYCM_D  
{
	int iBrgType;
	T_ELEM_K ElemK;
	UINT iIJ;
	int j;
	BOOL isServ;
	BOOL isLong;
	double dfck;
	double dftk;
	void Initialize()
	{
		iBrgType = 0;
		ElemK = iIJ = 0;
		j        = 0;
		isServ   = TRUE;
		isLong   = TRUE;
		dfck     = .0;
		dftk     = .0;
	}
};


struct _DGN_FPMS_D  
{
	int iBrgType;
	int iCastType;
	T_ELEM_K ElemK;
	UINT iIJ;
	int j;
	double dfck;
	double dftk;
	double dfskS, dSig_tp, dbw, dAsv, dSv;
	void Initialize()
	{
		iBrgType = 0;
		iCastType= 0;
		ElemK = iIJ = 0;
		j = 0;
		dfck     = .0;
		dftk     = .0;
		dfskS = dSig_tp = dbw = dAsv = dSv =.0;
	}
};

struct _DGN_FPMT_D  
{
	int iBrgType;
	int iCastType;
	T_ELEM_K ElemK;
	UINT iIJ;
	int j;
	double dfck;
	double dftk;
	double dfskS, dSig_tp, dbw, dAsv, dSv;
	void Initialize()
	{
		iBrgType = 0;
		iCastType= 0;
		ElemK = iIJ = 0;
		j = 0;
		dfck     = .0;
		dftk     = .0;
		dfskS = dSig_tp = dbw = dAsv = dSv =.0;
	}
};


struct _DGN_BDCW_D  
{
	BOOL bPrint;
	BOOL m_bTestMode;
	CString m_strTestDgnFile;
	BOOL bSelElemK;
	T_ELEM_K ElemK;
	UINT iTopBot;
	int iCrackType;
	double dMd, dMs, dMl, dMd_extp, dShrThk;
	double dFlgThk[2], dpMomPos[2][2];
	double dC1, dC2, dC3;
	_PSC_MATL_ELEM MatlElem;
	T_SPSC_SECT SpscSect;
	CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
	CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnPosi;
	double dFT;
	double dFB;

	void Initialize()
	{	
		bPrint = FALSE;
		m_bTestMode = FALSE;
		m_strTestDgnFile = _T("");
		bSelElemK = FALSE;
		ElemK = 0;
		iTopBot = 0;
		iCrackType = 0;
		dMd = dMs = dMl = dMd_extp = dShrThk = .0;
		dFlgThk[0] = .0; dFlgThk[1] = .0;
		dpMomPos[0][0] = .0; dpMomPos[0][1];
		dpMomPos[1][0] = .0; dpMomPos[1][1];
		dC1 = dC2 = dC3 = 1.0;
		MatlElem.Initialize();
		SpscSect.Initialize();
		arRbarPosi.RemoveAll();
		arTndnPosi.RemoveAll();
		dFT = dFB = .0;
	}
};

struct _DGN_BDCY_D
{
	BOOL m_bReport;
	BOOL m_bTestMode;
	CString m_strTestDgnFile;
	BOOL bSelElemK;
	T_ELEM_K ElemK;
	UINT iIJ;
	BOOL bYaxis;
	double dgam0, dxib, dfcd, dftd, dbw;
	double dFlgThk[2];
	double dpMomPos[2][2];
	_PSC_MATL_ELEM MatlElem;	
	T_SPSC_SECT SpscSect;
	CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
	CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnPosi;
	_RPT_BDCY_BASE RptBdcy;	

	void Initialize()
	{
		m_bReport = FALSE;
		m_bTestMode = FALSE;
		m_strTestDgnFile = _T("");
		bSelElemK = FALSE;
		ElemK = iIJ = 0;
		bYaxis = TRUE;
		dgam0 = dxib = dfcd = dftd = dbw =.0;
		dFlgThk[0] = .0;
		dFlgThk[1] = .0;
		dpMomPos[0][0] = .0;
		dpMomPos[0][1] = .0;
		dpMomPos[1][0] = .0;
		dpMomPos[1][1] = .0;
		MatlElem.Initialize();	
		SpscSect.Initialize();
		arRbarPosi.RemoveAll();
		arTndnPosi.RemoveAll();
		RptBdcy.Initialize();	
	}
};

struct _DGN_CUMS_D
{
	int iBrgType;
	BOOL m_bReport;
	BOOL m_bTestMode;
	CString m_strTestDgnFile;
	BOOL bSelElemK;
	T_ELEM_K ElemK;
	BOOL bYaxis;
	UINT iIJ;
	double dgam0;
	_PSC_MATL_ELEM MatlElem;
	T_SPSC_SECT SpscSect;
	double dShrThk;
	CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
	CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnPosi;
	T_RPSC_SBAR RpscSbar;
	_RPT_CUMS_BASE RptCums;

	void Initialize()
	{
		iBrgType = 0;
		m_bReport = FALSE;
		m_bTestMode = FALSE;
		m_strTestDgnFile = _T("");
		bSelElemK = FALSE;
		ElemK = 0;
		bYaxis = TRUE;
		iIJ = 0;
		dgam0 = .0;
		MatlElem.Initialize();
		SpscSect.Initialize();
		dShrThk = .0;
		arRbarPosi.RemoveAll();
		arTndnPosi.RemoveAll();
		RpscSbar.Initialize();
		RptCums.Initialize();
	}
};

struct _DGN_CRMT_D 
{
	int iBrgType;
	BOOL m_bReport;
	BOOL m_bTestMode;
	CString m_strTestDgnFile;
	BOOL bSelElemK;
	T_ELEM_K ElemK;
	BOOL bYaxis;
	UINT iIJ;
	double dgam0;
	_PSC_MATL_ELEM MatlElem;
	T_SPSC_SECT SpscSect;
	BOOL bSolidSect;
	double dAoh, dPh, dWt, dShrThk;
	double dFlgThk[2];
	double dTorThk;
	double dpMomPos[2][2];
	CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
	CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnPosi;
	T_RPSC_SBAR RpscSbar;
	_RPT_CRMT_BASE RptCrmt;

	void Initialize()
	{
		iBrgType = 0;
		m_bReport = FALSE;
		m_bTestMode = FALSE;
		m_strTestDgnFile = _T("");
		bSelElemK = FALSE;
		ElemK = 0;
		bYaxis = TRUE;
		iIJ = 0;
		dgam0 = .0;
		MatlElem.Initialize();
		SpscSect.Initialize();
		bSolidSect = TRUE;
		dAoh = dPh = dWt = dShrThk =.0;
		dFlgThk[0] = .0;
		dTorThk = .0;
		dpMomPos[0][0]= .0;
		dpMomPos[0][1]= .0;
		dpMomPos[1][0]= .0;
		dpMomPos[1][1]= .0;
		arRbarPosi.RemoveAll();
		arTndnPosi.RemoveAll();
		RpscSbar.Initialize();
		RptCrmt.Initialize();		
	}
};

struct _DGN_JTCS_D
{
	T_ELEM_K ElemK;
	UINT iIJ;	
	BOOL bYaxis;
	double dgam0, dl0, dfcd, dftd, dbw;
	double dFlgThk[2];
	double dpMomPos[2][2];
	_PSC_MATL_ELEM MatlElem;
	T_SPSC_SECT SpscSect;
	CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
	CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnPosi;

	void Initialize()
	{
		ElemK = iIJ = 0;
		bYaxis = TRUE;
		dgam0 = dl0 = dfcd = dftd = dbw = .0;
		dFlgThk[0] = .0;
		dFlgThk[1] = .0;
		dpMomPos[0][0] = .0;
		dpMomPos[0][1] = .0;
		dpMomPos[1][0] = .0;
		dpMomPos[1][1] = .0;
		MatlElem.Initialize();
		SpscSect.Initialize();
		arRbarPosi.RemoveAll();
		arTndnPosi.RemoveAll();
	}
};

struct _DGN_TBAR_D
{
	BOOL m_bTestMode;
	CString m_strTestDgnFile;
	BOOL bSelElemK;
	T_ELEM_K ElemK;
	UINT iIJ;
	UINT iTopBot;
	_PSC_MATL_ELEM MatlElem;
	double dgam0, dfcd, dftd, dxib, dbw;
	double dFlgThk[2];
	double dpMomPos[2][2];
	T_SPSC_SECT SpscSect;
	CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
	CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnPosi;

	void Initialize()
	{
		m_bTestMode = FALSE;
		m_strTestDgnFile = _T("");
		bSelElemK = FALSE;
		ElemK = 0;
		iIJ = 0;
		iTopBot = 0;
		MatlElem.Initialize();
		dgam0 = dfcd = dftd = dxib = dbw =.0;
		dFlgThk[0] = .0;
		dFlgThk[1] = .0;
		dpMomPos[0][0] = .0;
		dpMomPos[0][1] = .0;
		dpMomPos[1][0] = .0;
		dpMomPos[1][1] = .0;
		SpscSect.Initialize();
		arRbarPosi.RemoveAll();
		arTndnPosi.RemoveAll();
	}
};

struct _DGN_TNDN_D
{
	T_ELEM_K ElemK;
	UINT iIJ;
	UINT iTopBot;	
	double dfck_p, dftk_p, dfck, dftk;
	double dFlgThk[2];
	T_SPSC_SECT SpscgSect;
	T_SPSC_SECT SpscSect;
	CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
	CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnPosi;

	void Initialize()
	{
		ElemK = 0;
		iIJ = 0;
		iTopBot = 0;	
		dfck_p = dftk_p = dfck = dftk = .0;
		dFlgThk[0] = .0;
		dFlgThk[1] = .0;
		SpscgSect.Initialize();
		SpscSect.Initialize();
		arRbarPosi.RemoveAll();
		arTndnPosi.RemoveAll();
	}
};


class CDgnJTG_D62_04 : public CDgnPscCommon
{
// Member Variables.
protected:
	  
	BOOL m_bReport;
	double m_dZero;

// Member Functions.

public:
	CDgnJTG_D62_04();
	virtual ~CDgnJTG_D62_04();
	void SetDataCtrlPointer(CCRCDataCtrl* pDataCtrl);
	//CRptJTG_D62_04* Get_RptJtgPointer();

	BOOL Get_MatlTndn(ElemPairK ElemK, double& dfcpd, double& dftpd);
	BOOL Get_MatlConc(ElemPairK ElemK, double& dfck, double& dftk, double& dfcd, double& dftd, double& dGc);


public:
	BOOL Get_JtgFysg(ElemPairK ElemK, T_FYSG_D& FysgD);				//  1. 가설중 휨 응력도
	// For Fully, Partially A
	BOOL Get_JtgFypc(T_TDNA_K TdnaK, T_FYPC_JTG& FypcJtg);		//  2. 정상사용극한상태 PC강재 최대응력도 (텐던별)
	// For Partially B
	BOOL Get_JtgFypc(CMap<T_TDNA_K,T_TDNA_K,T_FYPC_JTG,T_FYPC_JTG>& mapFypcJtg);		
	BOOL Get_JtgFycm(ElemPairK ElemK, T_FYCM_JTG& FycmJtg,T_DETAIL_STR& DetailStr);		//  3. 정상사용극한상태 합성응력도
	BOOL Get_JtgFpms(ElemPairK ElemK, T_FPMS_JTG& FpmsJtg,T_DETAIL_STR& DetailStr);		//  4. 정상사용극한상태 주응력도 (전단력 최대시)
	BOOL Get_JtgFpmt(ElemPairK ElemK, T_FPMT_JTG& FpmtJtg);		//  5. 정상사용극한상태 주응력도 (비틀림 최대시)
	BOOL Get_JtgBdcw(ElemPairK ElemK, T_BDCW_D& BdcwD,T_DETAIL_STR& DetailStr);				//  6. 정상사용극한상태 휨 균열폭
	BOOL Get_JtgTbar(ElemPairK ElemK, T_TBAR_JTG& TbarJtg, T_DETAIL_STR& DetailStr, BOOL bI=TRUE, BOOL bJ=TRUE, int iUseKind=0);	//  7. 정상사용극한상태 인장철근
	BOOL Get_JtgTndn(ElemPairK ElemK, T_TNDN_JTG& TndnJtg, T_DETAIL_STR& DetailStr);
	BOOL Get_JtgBdcy(ElemPairK ElemK, T_BDCY_JTG& BdcyJtg, T_DETAIL_STR& DetailStr, BOOL bI=TRUE, BOOL bJ=TRUE);		//  9. 재하능력극한상태 휨강도 (강축)
	BOOL Get_JtgCums(ElemPairK ElemK, T_CUMS_JTG& CumsJtg, T_DETAIL_STR& DetailStr, BOOL bI=TRUE, BOOL bJ=TRUE);		// 10. 재하능력극한상태 전단강도
	BOOL Get_JtgCrmt(ElemPairK ElemK, T_CRMT_JTG& CrmtJtg, T_DETAIL_STR& DetailStr, BOOL bI=TRUE, BOOL bJ=TRUE);		// 11. 재하능력극한상태 비틀림강도
	BOOL Get_JtgCscs(ElemPairK ElemK, T_CSTCS_CH& CscsJtg, T_DETAIL_STR& DetailStr, BOOL bI=TRUE, BOOL bJ=TRUE);		// 12.compressive stress check at service stage.예箕駱炬
		
	BOOL Get_BeforeAllowbleStressofConc(BOOL bPre, double dfck_p, double dftk_p, double dAg, T_SPSC_SECT& SpscSect, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi,
																			CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi, T_FYSG_BASE FysgBase , double& dalComf, double& dalTenf, BOOL& bRho);
	BOOL Get_AfterAllowbleStressofConc(T_LCOM_K LcomK, double dfck, double dftk, double& dalComf, double& dalTenf);
	BOOL Get_AllowPrincipalStress(double dfck, double dftk, double dfsk, double dSig_tp, double dbw, double dAsv, double dSv, double& dalSig_cp, double& dalSig_tp);
	BOOL Get_AllowbleStressofPC(T_TDNA_K TdnaK, T_FYPC_JTG& FypcJtg);
	BOOL Get_CrackCheckParam(double dMs, double dMl, double& dC1, double& dC2, double& dC3);
	BOOL Get_Alpha123(double dMu, double dMu_Tnpr, double& dalpha1, double& dalpha2, double& dalpha3);
	BOOL Get_CrackCheck(BOOL bPrint, ElemPairK ElemK, UINT iTopBot, int iCrackType, double dMd, double dMs, double dMl, double dMd_extp, double dShrThk, double dFlgThk[2],
											double dpMomPos[2][2], double dC1, double dC2, double dC3, _PSC_MATL_ELEM& MatlElem, T_SPSC_SECT& SpscSect, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbar,
											CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi, double dFT, double dFB, double& dSigss, double& dWfk, double& dwa);
	BOOL Get_FlexureStrength(ElemPairK ElemK, BOOL bYaxis, double dgam0, double dxib, double dfcd, double dftd, double dbw, double dFlgThk[2], double dpMomPos[2][2], _PSC_MATL_ELEM& MatlElem,
													 T_SPSC_SECT& SpscSect, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi, T_BDCY_JTG_BASE& BdcyJtg, _RPT_BDCY_BASE& RptBdcy);
	BOOL Get_ShearDesign(ElemPairK ElemK, BOOL bYaxis, UINT iIJ, double dgam0, _PSC_MATL_ELEM& MatlElem, T_SPSC_SECT& SpscSect, double dShrThk,
											 CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi, T_RPSC_SBAR& RpscSbar, T_CUMS_JTG_BASE& CumsJtg, _RPT_CUMS_BASE& RptCums);

	BOOL Get_TorsionDesign(ElemPairK ElemK, BOOL bYaxis, UINT iIJ, double dgam0,  _PSC_MATL_ELEM& MatlElem, T_SPSC_SECT& SpscSect, BOOL bSolidSect, double dAoh, double dPh, double dWt, double dShrThk, double dFlgThk[2], double dTorThk, double dpMomPos[2][2],
												 CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi, T_RPSC_SBAR& RpscSbar, T_CRMT_JTG_BASE& CrmtJtg, _RPT_CRMT_BASE& RptCrmt);
//  BOOL Get_CompressiveCheck(T_ELEM_K ElemK, BOOL bYaxis, double dgam0, double dl0, double dfcd, double dftd, double dbw, double dFlgThk[2], double dpMomPos[2][2], _PSC_MATL_ELEM& MatlElem, 
//                           T_SPSC_SECT& SpscSect, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi, T_CSTCS_CH_BASE& CscsJtg);

	BOOL CalcSectPSC_QA(CArray<T_SPSC_COOR,T_SPSC_COOR>& arOuter, CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*>& arInner, BOOL bDir, BOOL baxis, double dpos, double dlen, double& dCutArea, double& dQ);
	BOOL Get_SumForceEccent(BOOL bYaxis, double dMd, double dNueC, T_SPSC_SECT& SpscSect, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi, double& dNp0, double& dep0, double& dhps);

	BOOL Get_PrintPropertyData(ElemPairK ElemK, _PSC_MATL_ELEM& MatlElem, T_SPSC_SECT& SpscSect, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi,
														 CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi);
	BOOL Get_SafetyCoeff(double& drc, double& drs);
	BOOL Get_Alphas(double& dAlpha_s);
	BOOL Get_Alpha1(double& dAlpha1);
	BOOL Get_TensionRebar(ElemPairK ElemK, UINT iIJ, UINT iTopBot, _PSC_MATL_ELEM& MatlElem, double dgam0, double dfcd, double dftd, double dxib, double dbw, double dFlgThk[2], double dpMomPos[2][2],
												T_SPSC_SECT& SpscSect, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi,
												T_TBAR_JTG_BASE& TbarJtg);

	BOOL Get_ReqTendon(ElemPairK ElemK, UINT iIJ, UINT iTopBot, double dfck_p, double dftk_p, double dfck, double dftk, double dFlgThk[2],
										 T_SPSC_SECT& SpscgSect, T_SPSC_SECT& SpscSect, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi, T_TNDN_JTG_BASE& TndnJtg);

	double Get_MatlFactor();
	// For Tendon Stress
	BOOL Get_TendonStress(ElemPairK ElemK, BOOL bPosiMom, double dMuy, double dfcd, double dftd, double dbw, double dFlgThk[2], double dpMomPos[2][2], _PSC_MATL_ELEM& MatlElem, CArray<T_SPSC_COOR,T_SPSC_COOR>& arOuter, CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*>& arInner,
												T_SPSC_SECT& SpscSect, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arCalcTndnPosi);
	BOOL Get_Np0_eN_hps(BOOL bYaxis, double dMd, double dNueC, T_SPSC_SECT& SpscSect, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi, double& dNp0, double& deN, double& dhps);
	double Get_FunctionX(double dA, double dB, double dC, double dD, double dx);
	double Get_FunctionXprim(double dA, double dB, double dC, double dD, double dx);

protected:
	_RPT_BDCY_JTG m_RptBdcy;
	_RPT_CUMS_JTG m_RptCums;
	_RPT_CRMT_JTG m_RptCrmt;

public:
	
	BOOL Get_RptBdcy(_RPT_BDCY_JTG& RptBdcy);
	BOOL Get_RptCums(_RPT_CUMS_JTG& RptCums);
	BOOL Get_RptCrmt(_RPT_CRMT_JTG& RptCrmt);
	void Set_ReportFlag(BOOL bReport);

protected:
	// Coded by Seungjun for Test. ('20060719)
	BOOL m_bTestMode;

	//+++++
	HINSTANCE m_hDllCalc;
	CString Get_DllFileName(int iCode);

	void GetMcrByJTG(ElemPairK ElemK,int iSerialLcom,BOOL bIsEndI,_PSC_SECT_POSI& sectposi,double& dMcr,double dftk);
	
	void Get_Sig_pc(ElemPairK ElemK , _DGN_STRS_CRC& StrsMax,_DGN_STRS_CRC& StrsMin);
	
    BOOL Get_PscTndnTdna(T_TDNA_K TdnaK, _PSC_TNDN_TDNA_W& TndnTdna);

    void ConvertTndnTdna(const _PSC_TNDN_TDNA& InD, _PSC_TNDN_TDNA_W& rData);
};

#endif // !defined(AFX_DGNJTG_D62_04_H__D2C256BA_89F4_41BF_8CF2_0D100EBF4C73__INCLUDED_)
