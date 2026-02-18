#ifndef __DgnForceStressST_H__
#define __DgnForceStressST_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnStruct.h"

const int cFORC_CON = 18;	// Concurrent Force (Fxmax, Fxmin,...,Mzmax,Mzmin,
							// biMmax, biMmin, STVenant Torsion_max, STVenant Torsion_min, Warping Torsion_max, Warping Torsion_min).
const int cFORC_CON_DGN = 12;
const int cPOSI_CRC =  2;
const int cPOSI_CRC_EX	= 5; // For 5 Position Force for each element, EX = Expansion
const int cDGN_POSNUM	= 5;
#define RATING_LOAD_CASE_NUM 7
#define RATING_LOAD_CASE_STL_NUM 8
const int Mbi_Id = 12;
const int Mst_Id = 14;
const int Mwp_Id  =16;
const int cRAT_LCASE = 8;

#define T_STEP_K unsigned int

//typedef std::pair<unsigned int, UINT> ElemPairK;
//typedef CArray<ElemPairK, ElemPairK&> ArrElemPairKey;


struct _DGN_FORC_POS
{
	double dForce[6];

	void Initialize()
	{
		memset(dForce,0,sizeof(dForce));
	}

	_DGN_FORC_POS() {Initialize();}
};
struct _DGN_FORC_MEMB
{
	_DGN_FORC_POS ForMemb[cDGN_POSNUM];

	void Initialize()
	{
		memset(ForMemb,0,sizeof(ForMemb));
	}
	
	_DGN_FORC_MEMB() {Initialize();}
};


//////////////////////////////////////////////////////////////////////////
// _DGN_FORC_CRC
//////////////////////////////////////////////////////////////////////////

struct _DGN_FORC_CRC
{
	CString strType;
    CString strLCSubType;
	int iRefNo[cPOSI_CRC];	// StagK, DgnLcomK,...
	double dFxx[cPOSI_CRC], dFyy[cPOSI_CRC], dFzz[cPOSI_CRC];
	double dMux[cPOSI_CRC], dMuy[cPOSI_CRC], dMuz[cPOSI_CRC];
	double dMbi[cPOSI_CRC], dTst[cPOSI_CRC], dTwp[cPOSI_CRC]; // Bi moment, STVenant Torsion, Warping Torsion
	void Initialize()
	{
		strType=_T("");
        strLCSubType=_T("");
		for(int i=0; i<cPOSI_CRC; i++)
		{
			iRefNo[i] = 0;
			dFxx[i]=0.0; dFyy[i]=0.0; dFzz[i]=0.0;
			dMux[i]=0.0; dMuy[i]=0.0; dMuz[i]=0.0;
			dMbi[i] = dTst[i] = dTwp[i] = 0.0;
		}
	}
	void Initialize(BOOL bMax)
	{
		strType=_T("");
        strLCSubType=_T("");
		for(int i=0; i<cPOSI_CRC; i++)
		{
			iRefNo[i] = 0;
			if(bMax)
			{
				dFxx[i]=(-1)/pow(cDgn_Zero,40); dFyy[i]=(-1)/pow(cDgn_Zero,40); dFzz[i]=(-1)/pow(cDgn_Zero,40);
				dMux[i]=(-1)/pow(cDgn_Zero,40); dMuy[i]=(-1)/pow(cDgn_Zero,40); dMuz[i]=(-1)/pow(cDgn_Zero,40);
				dMbi[i]=(-1)/pow(cDgn_Zero,40); dTst[i]=(-1)/pow(cDgn_Zero,40); dTwp[i]=(-1)/pow(cDgn_Zero,40);
			}
			else
			{
				dFxx[i]=(+1)/pow(cDgn_Zero,40); dFyy[i]=(+1)/pow(cDgn_Zero,40); dFzz[i]=(+1)/pow(cDgn_Zero,40);
				dMux[i]=(+1)/pow(cDgn_Zero,40); dMuy[i]=(+1)/pow(cDgn_Zero,40); dMuz[i]=(+1)/pow(cDgn_Zero,40);
				dMbi[i]=(+1)/pow(cDgn_Zero,40); dTst[i]=(+1)/pow(cDgn_Zero,40); dTwp[i]=(+1)/pow(cDgn_Zero,40);
			}
		}
	}
	_DGN_FORC_CRC()	{Initialize();}
	_DGN_FORC_CRC(const _DGN_FORC_CRC& rData)	{*this=rData;}
	_DGN_FORC_CRC& operator = (const _DGN_FORC_CRC& rData)
	{
		strType = rData.strType;
        strLCSubType = rData.strLCSubType;
		for(int i=0; i<cPOSI_CRC; i++)
		{
			iRefNo[i] = rData.iRefNo[i];
			dFxx[i]=rData.dFxx[i]; dFyy[i]=rData.dFyy[i]; dFzz[i]=rData.dFzz[i];
			dMux[i]=rData.dMux[i]; dMuy[i]=rData.dMuy[i]; dMuz[i]=rData.dMuz[i];
			dMbi[i]=rData.dMbi[i]; dTst[i]=rData.dTst[i]; dTwp[i]=rData.dTwp[i];
		}
		return *this;
	}
	_DGN_FORC_CRC& operator += (const _DGN_FORC_CRC& rData)
	{
		for(int i=0; i<cPOSI_CRC; i++)
		{
			dFxx[i]+=rData.dFxx[i]; dFyy[i]+=rData.dFyy[i]; dFzz[i]+=rData.dFzz[i];
			dMux[i]+=rData.dMux[i]; dMuy[i]+=rData.dMuy[i]; dMuz[i]+=rData.dMuz[i];
			dMbi[i]+=rData.dMbi[i]; dTst[i]+=rData.dTst[i]; dTwp[i]+=rData.dTwp[i];
		}
		return *this;
	}
	_DGN_FORC_CRC& operator -= (const _DGN_FORC_CRC& rData)
	{
		for(int i=0; i<cPOSI_CRC; i++)
		{
			dFxx[i]-=rData.dFxx[i]; dFyy[i]-=rData.dFyy[i]; dFzz[i]-=rData.dFzz[i];
			dMux[i]-=rData.dMux[i]; dMuy[i]-=rData.dMuy[i]; dMuz[i]-=rData.dMuz[i];
			dMbi[i]-=rData.dMbi[i]; dTst[i]-=rData.dTst[i]; dTwp[i]-=rData.dTwp[i];
		}
		return *this;
	}
	_DGN_FORC_CRC& operator *= (const double dFactor)
	{
		for(int i=0; i<cPOSI_CRC; i++)
		{
			dFxx[i]*=dFactor; dFyy[i]*=dFactor; dFzz[i]*=dFactor;
			dMux[i]*=dFactor; dMuy[i]*=dFactor; dMuz[i]*=dFactor;
			dMbi[i]*=dFactor; dTst[i]*=dFactor; dTwp[i]*=dFactor;
		}
		return *this;
	}
	_DGN_FORC_CRC& operator / (const double dFactor)
	{
		for(int i=0; i<cPOSI_CRC; i++)
		{
			dFxx[i]/=dFactor; dFyy[i]/=dFactor; dFzz[i]/=dFactor;
			dMux[i]/=dFactor; dMuy[i]/=dFactor; dMuz[i]/=dFactor;
			dMbi[i]/=dFactor; dTst[i]/=dFactor; dTwp[i]/=dFactor;
		}
		return *this;
	}  
	_DGN_FORC_CRC& GetData(UINT nIJ)
	{
		int ii=0; // 0에 입력 
		dFxx[ii] = dFxx[nIJ];
		dFyy[ii] = dFyy[nIJ];
		dFzz[ii] = dFzz[nIJ];
		dMux[ii] = dMux[nIJ];
		dMuy[ii] = dMuy[nIJ];
		dMuz[ii] = dMuz[nIJ];
		dMbi[ii] = dMbi[nIJ];
		dTst[ii] = dTst[nIJ];
		dTwp[ii] = dTwp[nIJ];
		//
		ii=1;
		dFxx[ii] = 0.0;
		dFyy[ii] = 0.0;
		dFzz[ii] = 0.0;
		dMux[ii] = 0.0;
		dMuy[ii] = 0.0;
		dMuz[ii] = 0.0;
		dMbi[ii] = 0.0;
		dTst[ii] = 0.0;
		dTwp[ii] = 0.0;
		
		return *this;
	}

	void SetData(UINT nFrom, UINT nTo, const _DGN_FORC_CRC& rData)
	{
		dFxx[nTo] = rData.dFxx[nFrom];
		dFyy[nTo] = rData.dFyy[nFrom];
		dFzz[nTo] = rData.dFzz[nFrom];
		dMux[nTo] = rData.dMux[nFrom];
		dMuy[nTo] = rData.dMuy[nFrom];
		dMuz[nTo] = rData.dMuz[nFrom];
		dMbi[nTo] = rData.dMbi[nFrom];
		dTst[nTo] = rData.dTst[nFrom];
		dTwp[nTo] = rData.dTwp[nFrom];
	}
};

struct _DGN_FORC_CRC_CH
{
	char strType[16];
    char strLCSubType[16];
	int iRefNo[cPOSI_CRC];	// StagK, DgnLcomK,...
	double dFxx[cPOSI_CRC], dFyy[cPOSI_CRC], dFzz[cPOSI_CRC];
	double dMux[cPOSI_CRC], dMuy[cPOSI_CRC], dMuz[cPOSI_CRC];
	double dMbi[cPOSI_CRC], dTst[cPOSI_CRC], dTwp[cPOSI_CRC];
	void ConvertToString(_DGN_FORC_CRC& rData)
	{
		ConvertCharStr(strType, rData.strType, sizeof(strType));
        ConvertCharStr(strLCSubType, rData.strLCSubType, sizeof(strLCSubType));

		for(int i=0; i<cPOSI_CRC; i++)
		{
			rData.iRefNo[i] = iRefNo[i];
			rData.dFxx[i]=dFxx[i]; rData.dFyy[i]=dFyy[i]; rData.dFzz[i]=dFzz[i];
			rData.dMux[i]=dMux[i]; rData.dMuy[i]=dMuy[i]; rData.dMuz[i]=dMuz[i];
			rData.dMbi[i]=dMbi[i]; rData.dTst[i]=dTst[i]; rData.dTwp[i]=dTwp[i];
		}
	}
	void ConvertToChar(_DGN_FORC_CRC& rData)
	{
		ConvertStrChar(rData.strType, strType, sizeof(strType));
        ConvertStrChar(rData.strLCSubType, strLCSubType, sizeof(strLCSubType));

		for(int i=0; i<cPOSI_CRC; i++)
		{
			iRefNo[i] = rData.iRefNo[i];
			dFxx[i]=rData.dFxx[i]; dFyy[i]=rData.dFyy[i]; dFzz[i]=rData.dFzz[i];
			dMux[i]=rData.dMux[i]; dMuy[i]=rData.dMuy[i]; dMuz[i]=rData.dMuz[i];
			dMbi[i]=rData.dMbi[i]; dTst[i]=rData.dTst[i]; dTwp[i]=rData.dTwp[i];
		}
	}

	void Initialize()
	{
		for(int i=0; i<cPOSI_CRC; i++)
		{
			iRefNo[i] = 0;
			dFxx[i]=0.0; dFyy[i]=0.0; dFzz[i]=0.0;
			dMux[i]=0.0; dMuy[i]=0.0; dMuz[i]=0.0;
			dMbi[i] = dTst[i] = dTwp[i] = 0.0;
		}
	}
};

typedef CArray<_DGN_FORC_CRC,_DGN_FORC_CRC&> ADGNFORCE;

struct _DGN_FORC_CRC_ELEM
{
	CArray<_DGN_FORC_CRC,_DGN_FORC_CRC&> aForceLcom;
	void Initialize()
	{
		aForceLcom.RemoveAll();
	}
	_DGN_FORC_CRC_ELEM() {}
	_DGN_FORC_CRC_ELEM(const _DGN_FORC_CRC_ELEM& rData) {*this=rData;}
	_DGN_FORC_CRC_ELEM& operator = (const _DGN_FORC_CRC_ELEM& rData)
	{
		aForceLcom.Copy(rData.aForceLcom);
		return *this;
	}
	_DGN_FORC_CRC_ELEM& operator += (const _DGN_FORC_CRC_ELEM& rData)
	{
		int iForcNumOrg = aForceLcom.GetSize();
		int iForcNumApp = rData.aForceLcom.GetSize();
		aForceLcom.SetSize(iForcNumApp);
		_DGN_FORC_CRC ForcCrcOrg, ForcCrcApp;
		for(int i=0; i<iForcNumApp; i++)
		{
			ForcCrcOrg.Initialize();
			ForcCrcApp.Initialize();
			if(i < iForcNumOrg) ForcCrcOrg = aForceLcom.GetAt(i);
			if(i < iForcNumApp) ForcCrcApp = rData.aForceLcom.GetAt(i);
			ForcCrcOrg += ForcCrcApp;
			aForceLcom.SetAt(i,ForcCrcOrg);
		}
		return *this;
	}
	_DGN_FORC_CRC_ELEM& operator -= (const _DGN_FORC_CRC_ELEM& rData)
	{
		int iForcNumOrg = aForceLcom.GetSize();
		int iForcNumApp = rData.aForceLcom.GetSize();
		aForceLcom.SetSize(iForcNumApp);
		_DGN_FORC_CRC ForcCrcOrg, ForcCrcApp;
		for(int i=0; i<iForcNumApp; i++)
		{
			ForcCrcOrg.Initialize();
			ForcCrcApp.Initialize();
			if(i < iForcNumOrg) ForcCrcOrg = aForceLcom.GetAt(i);
			if(i < iForcNumApp) ForcCrcApp = rData.aForceLcom.GetAt(i);
			ForcCrcOrg -= ForcCrcApp;
			aForceLcom.SetAt(i,ForcCrcOrg);
		}
		return *this;
	}
};

//////////////////////////////////////////////////////////////////////////
// _DGN_FORC_CRC_EX
//////////////////////////////////////////////////////////////////////////
struct _DGN_FORC_CRC_EX
{
	CString strType;
    CString strLCSubType;
	int iRefNo[cPOSI_CRC_EX];	// StagK, DgnLcomK,...
	double dFxx[cPOSI_CRC_EX], dFyy[cPOSI_CRC_EX], dFzz[cPOSI_CRC_EX];
	double dMux[cPOSI_CRC_EX], dMuy[cPOSI_CRC_EX], dMuz[cPOSI_CRC_EX];
	double dMbi[cPOSI_CRC_EX], dTst[cPOSI_CRC_EX], dTwp[cPOSI_CRC_EX]; // Bi moment, STVenant Torsion, Warping Torsion
	void Initialize()
	{
		strType=_T("");
        strLCSubType=_T("");
		for(int i=0; i<cPOSI_CRC_EX; i++)
		{
			iRefNo[i] = 0;
			dFxx[i]=0.0; dFyy[i]=0.0; dFzz[i]=0.0;
			dMux[i]=0.0; dMuy[i]=0.0; dMuz[i]=0.0;
			dMbi[i] = dTst[i] = dTwp[i] = 0.0;
		}
	}
	void Initialize(BOOL bMax)
	{
		strType=_T("");
        strLCSubType=_T("");
		for(int i=0; i<cPOSI_CRC_EX; i++)
		{
			iRefNo[i] = 0;
			if(bMax)
			{
				dFxx[i]=(-1)/pow(cDgn_Zero,40); dFyy[i]=(-1)/pow(cDgn_Zero,40); dFzz[i]=(-1)/pow(cDgn_Zero,40);
				dMux[i]=(-1)/pow(cDgn_Zero,40); dMuy[i]=(-1)/pow(cDgn_Zero,40); dMuz[i]=(-1)/pow(cDgn_Zero,40);
				dMbi[i]=(-1)/pow(cDgn_Zero,40); dTst[i]=(-1)/pow(cDgn_Zero,40); dTwp[i]=(-1)/pow(cDgn_Zero,40);
			}
			else
			{
				dFxx[i]=(+1)/pow(cDgn_Zero,40); dFyy[i]=(+1)/pow(cDgn_Zero,40); dFzz[i]=(+1)/pow(cDgn_Zero,40);
				dMux[i]=(+1)/pow(cDgn_Zero,40); dMuy[i]=(+1)/pow(cDgn_Zero,40); dMuz[i]=(+1)/pow(cDgn_Zero,40);
				dMbi[i]=(+1)/pow(cDgn_Zero,40); dTst[i]=(+1)/pow(cDgn_Zero,40); dTwp[i]=(+1)/pow(cDgn_Zero,40);
			}
		}
	}
	_DGN_FORC_CRC_EX()	{Initialize();}
	_DGN_FORC_CRC_EX(const _DGN_FORC_CRC_EX& rData)	{*this=rData;}
	_DGN_FORC_CRC_EX& operator = (const _DGN_FORC_CRC_EX& rData)
	{
		strType = rData.strType;
        strLCSubType = rData.strLCSubType;
		for(int i=0; i<cPOSI_CRC_EX; i++)
		{
			iRefNo[i] = rData.iRefNo[i];
			dFxx[i]=rData.dFxx[i]; dFyy[i]=rData.dFyy[i]; dFzz[i]=rData.dFzz[i];
			dMux[i]=rData.dMux[i]; dMuy[i]=rData.dMuy[i]; dMuz[i]=rData.dMuz[i];
			dMbi[i]=rData.dMbi[i]; dTst[i]=rData.dTst[i]; dTwp[i]=rData.dTwp[i];
		}
		return *this;
	}
	_DGN_FORC_CRC_EX& operator += (const _DGN_FORC_CRC_EX& rData)
	{
		for(int i=0; i<cPOSI_CRC_EX; i++)
		{
			dFxx[i]+=rData.dFxx[i]; dFyy[i]+=rData.dFyy[i]; dFzz[i]+=rData.dFzz[i];
			dMux[i]+=rData.dMux[i]; dMuy[i]+=rData.dMuy[i]; dMuz[i]+=rData.dMuz[i];
			dMbi[i]+=rData.dMbi[i]; dTst[i]+=rData.dTst[i]; dTwp[i]+=rData.dTwp[i];
		}
		return *this;
	}
	_DGN_FORC_CRC_EX& operator -= (const _DGN_FORC_CRC_EX& rData)
	{
		for(int i=0; i<cPOSI_CRC_EX; i++)
		{
			dFxx[i]-=rData.dFxx[i]; dFyy[i]-=rData.dFyy[i]; dFzz[i]-=rData.dFzz[i];
			dMux[i]-=rData.dMux[i]; dMuy[i]-=rData.dMuy[i]; dMuz[i]-=rData.dMuz[i];
			dMbi[i]-=rData.dMbi[i]; dTst[i]-=rData.dTst[i]; dTwp[i]-=rData.dTwp[i];
		}
		return *this;
	}
	_DGN_FORC_CRC_EX& operator *= (const double dFactor)
	{
		for(int i=0; i<cPOSI_CRC_EX; i++)
		{
			dFxx[i]*=dFactor; dFyy[i]*=dFactor; dFzz[i]*=dFactor;
			dMux[i]*=dFactor; dMuy[i]*=dFactor; dMuz[i]*=dFactor;
			dMbi[i]*=dFactor; dTst[i]*=dFactor; dTwp[i]*=dFactor;
		}
		return *this;
	}
	_DGN_FORC_CRC_EX& operator / (const double dFactor)
	{
		for(int i=0; i<cPOSI_CRC_EX; i++)
		{
			dFxx[i]/=dFactor; dFyy[i]/=dFactor; dFzz[i]/=dFactor;
			dMux[i]/=dFactor; dMuy[i]/=dFactor; dMuz[i]/=dFactor;
			dMbi[i]/=dFactor; dTst[i]/=dFactor; dTwp[i]/=dFactor;
		}
		return *this;
	}  
	_DGN_FORC_CRC_EX& GetData(UINT nIJ)
	{
		int ii=0; // 0에 입력 
		dFxx[ii] = dFxx[nIJ];
		dFyy[ii] = dFyy[nIJ];
		dFzz[ii] = dFzz[nIJ];
		dMux[ii] = dMux[nIJ];
		dMuy[ii] = dMuy[nIJ];
		dMuz[ii] = dMuz[nIJ];
		dMbi[ii] = dMbi[nIJ];
		dTst[ii] = dTst[nIJ];
		dTwp[ii] = dTwp[nIJ];
		//
		ii=1;
		dFxx[ii] = 0.0;
		dFyy[ii] = 0.0;
		dFzz[ii] = 0.0;
		dMux[ii] = 0.0;
		dMuy[ii] = 0.0;
		dMuz[ii] = 0.0;
		dMbi[ii] = 0.0;
		dTst[ii] = 0.0;
		dTwp[ii] = 0.0;

		return *this;
	}

	void SetData(UINT nFrom, UINT nTo, const _DGN_FORC_CRC_EX& rData)
	{
		dFxx[nTo] = rData.dFxx[nFrom];
		dFyy[nTo] = rData.dFyy[nFrom];
		dFzz[nTo] = rData.dFzz[nFrom];
		dMux[nTo] = rData.dMux[nFrom];
		dMuy[nTo] = rData.dMuy[nFrom];
		dMuz[nTo] = rData.dMuz[nFrom];
		dMbi[nTo] = rData.dMbi[nFrom];
		dTst[nTo] = rData.dTst[nFrom];
		dTwp[nTo] = rData.dTwp[nFrom];
	}

    void GetForce(_DGN_FORC_CRC& rData)
    {
        rData.strType = strType;
        rData.strLCSubType = strLCSubType;
        for ( int i=0; i<cPOSI_CRC; ++i )
        {
            int nPos = i==1 ? 4 : 0;
            rData.iRefNo[i] = iRefNo[nPos];
            rData.dFxx[i] = dFxx[nPos];
            rData.dFyy[i] = dFyy[nPos];
            rData.dFzz[i] = dFzz[nPos];
            rData.dMux[i] = dMux[nPos];
            rData.dMuy[i] = dMuy[nPos];
            rData.dMuz[i] = dMuz[nPos];
            rData.dMbi[i] = dMbi[nPos];
            rData.dTst[i] = dTst[nPos];
            rData.dTwp[i] = dTwp[nPos];
        }
        

    }
};

struct _DGN_FORC_CRC_EX_CH
{
	char strType[16];
    char strLCSubType[16];
	int iRefNo[cPOSI_CRC_EX];	// StagK, DgnLcomK,...
	double dFxx[cPOSI_CRC_EX], dFyy[cPOSI_CRC_EX], dFzz[cPOSI_CRC_EX];
	double dMux[cPOSI_CRC_EX], dMuy[cPOSI_CRC_EX], dMuz[cPOSI_CRC_EX];
	double dMbi[cPOSI_CRC_EX], dTst[cPOSI_CRC_EX], dTwp[cPOSI_CRC_EX];
	void ConvertToString(_DGN_FORC_CRC_EX& rData)
	{
		ConvertCharStr(strType, rData.strType, sizeof(strType));
        ConvertCharStr(strLCSubType, rData.strLCSubType, sizeof(strLCSubType));

		for(int i=0; i<cPOSI_CRC_EX; i++)
		{
			rData.iRefNo[i] = iRefNo[i];
			rData.dFxx[i]=dFxx[i]; rData.dFyy[i]=dFyy[i]; rData.dFzz[i]=dFzz[i];
			rData.dMux[i]=dMux[i]; rData.dMuy[i]=dMuy[i]; rData.dMuz[i]=dMuz[i];
			rData.dMbi[i]=dMbi[i]; rData.dTst[i]=dTst[i]; rData.dTwp[i]=dTwp[i];
		}
	}
	void ConvertToChar(_DGN_FORC_CRC_EX& rData)
	{
		ConvertStrChar(rData.strType, strType, sizeof(strType));
        ConvertStrChar(rData.strLCSubType, strLCSubType, sizeof(strLCSubType));

		for(int i=0; i<cPOSI_CRC_EX; i++)
		{
			iRefNo[i] = rData.iRefNo[i];
			dFxx[i]=rData.dFxx[i]; dFyy[i]=rData.dFyy[i]; dFzz[i]=rData.dFzz[i];
			dMux[i]=rData.dMux[i]; dMuy[i]=rData.dMuy[i]; dMuz[i]=rData.dMuz[i];
			dMbi[i]=rData.dMbi[i]; dTst[i]=rData.dTst[i]; dTwp[i]=rData.dTwp[i];
		}
	}

	void Initialize()
	{
		for(int i=0; i<cPOSI_CRC_EX; i++)
		{
			iRefNo[i] = 0;
			dFxx[i]=0.0; dFyy[i]=0.0; dFzz[i]=0.0;
			dMux[i]=0.0; dMuy[i]=0.0; dMuz[i]=0.0;
			dMbi[i] = dTst[i] = dTwp[i] = 0.0;
		}
	}
};

typedef CArray<_DGN_FORC_CRC_EX,_DGN_FORC_CRC_EX&> ADGNFORCE_EX;

struct _DGN_FORC_CRC_EX_ELEM
{
	CArray<_DGN_FORC_CRC_EX,_DGN_FORC_CRC_EX&> aForceLcom;
	void Initialize()
	{
		aForceLcom.RemoveAll();
	}
	_DGN_FORC_CRC_EX_ELEM() {}
	_DGN_FORC_CRC_EX_ELEM(const _DGN_FORC_CRC_EX_ELEM& rData) {*this=rData;}
	_DGN_FORC_CRC_EX_ELEM& operator = (const _DGN_FORC_CRC_EX_ELEM& rData)
	{
		aForceLcom.Copy(rData.aForceLcom);
		return *this;
	}
	_DGN_FORC_CRC_EX_ELEM& operator += (const _DGN_FORC_CRC_EX_ELEM& rData)
	{
		int iForcNumOrg = aForceLcom.GetSize();
		int iForcNumApp = rData.aForceLcom.GetSize();
		aForceLcom.SetSize(iForcNumApp);
		_DGN_FORC_CRC_EX ForcCrcOrg, ForcCrcApp;
		for(int i=0; i<iForcNumApp; i++)
		{
			ForcCrcOrg.Initialize();
			ForcCrcApp.Initialize();
			if(i < iForcNumOrg) ForcCrcOrg = aForceLcom.GetAt(i);
			if(i < iForcNumApp) ForcCrcApp = rData.aForceLcom.GetAt(i);
			ForcCrcOrg += ForcCrcApp;
			aForceLcom.SetAt(i,ForcCrcOrg);
		}
		return *this;
	}
	_DGN_FORC_CRC_EX_ELEM& operator -= (const _DGN_FORC_CRC_EX_ELEM& rData)
	{
		int iForcNumOrg = aForceLcom.GetSize();
		int iForcNumApp = rData.aForceLcom.GetSize();
		aForceLcom.SetSize(iForcNumApp);
		_DGN_FORC_CRC_EX ForcCrcOrg, ForcCrcApp;
		for(int i=0; i<iForcNumApp; i++)
		{
			ForcCrcOrg.Initialize();
			ForcCrcApp.Initialize();
			if(i < iForcNumOrg) ForcCrcOrg = aForceLcom.GetAt(i);
			if(i < iForcNumApp) ForcCrcApp = rData.aForceLcom.GetAt(i);
			ForcCrcOrg -= ForcCrcApp;
			aForceLcom.SetAt(i,ForcCrcOrg);
		}
		return *this;
	}
};

struct _DGN_FORC_UL_MEMB
{
	ADGNFORCE_EX ElemFor;

	void Initialize()
	{
		ElemFor.RemoveAll();
	}

	_DGN_FORC_UL_MEMB() {Initialize();}
	_DGN_FORC_UL_MEMB(const _DGN_FORC_UL_MEMB& rData) {*this = rData;}
	_DGN_FORC_UL_MEMB& operator=(const _DGN_FORC_UL_MEMB& rData)
	{
		ElemFor.Copy(rData.ElemFor);
		return *this;
	}
};

typedef CArray<_DGN_FORC_UL_MEMB,_DGN_FORC_UL_MEMB&> ADGNMEMB_FORCE;

//////////////////////////////////////////////////////////////////////////
// _DGN_STRS_CRC
//////////////////////////////////////////////////////////////////////////

struct _DGN_STRS_CRC
{
	CString strType;
    CString strLCSubType;
	int iRefNo[cPOSI_CRC];	// StagK, DgnLcomK,...
	int iSerialStep[cPOSI_CRC]; // Step
	double dStop[cPOSI_CRC], dSbot[cPOSI_CRC];	// 상단, 하단 응력도.
	double dStl[cPOSI_CRC], dSbl[cPOSI_CRC];		// 상단좌단, 하단좌단 응력도.
	double dStr[cPOSI_CRC], dSbr[cPOSI_CRC];		// 상단우단, 하단우단 응력도.
	double dScom[cPOSI_CRC];										// 최대/최소 응력도.
	double dExtraRat[cPOSI_CRC];								// 할증계수.
	double dSxx[cPOSI_CRC];                     // 축응력
	void Initialize()
	{
		strType=_T("");
        strLCSubType=_T("");
		for(int i=0; i<cPOSI_CRC; i++)
		{
			iRefNo[i]=0;
			iSerialStep[i]=0;
			dStop[i]=0.0; dSbot[i]=0.0;
			dStl[i]=0.0; dSbl[i]=0.0;
			dStr[i]=0.0; dSbr[i]=0.0;
			dScom[i]=0.0;
			dExtraRat[i]=1.0;
			dSxx[i]=0.0;
		}
	}
	void Initialize(BOOL bMax)
	{
		strType=_T("");
        strLCSubType=_T("");
		for(int i=0; i<cPOSI_CRC; i++)
		{
			if(bMax)
			{
				iRefNo[i]=0;
				iSerialStep[i]=0;
				dStop[i]=(-1)/pow(cDgn_Zero,40); dSbot[i]=(-1)/pow(cDgn_Zero,40);
				dStl[i]=(-1)/pow(cDgn_Zero,40); dSbl[i]=(-1)/pow(cDgn_Zero,40);
				dStr[i]=(-1)/pow(cDgn_Zero,40); dSbr[i]=(-1)/pow(cDgn_Zero,40);
				dScom[i]=(-1)/pow(cDgn_Zero,40);
				dExtraRat[i]=1.0;				
				dSxx[i]=(-1)/pow(cDgn_Zero,40);
			}
			else
			{
				iRefNo[i]=0;
				iSerialStep[i]=0;
				dStop[i]=(+1)/pow(cDgn_Zero,40); dSbot[i]=(+1)/pow(cDgn_Zero,40);
				dStl[i]=(+1)/pow(cDgn_Zero,40); dSbl[i]=(+1)/pow(cDgn_Zero,40);
				dStr[i]=(+1)/pow(cDgn_Zero,40); dSbr[i]=(+1)/pow(cDgn_Zero,40);
				dScom[i]=(+1)/pow(cDgn_Zero,40);
				dExtraRat[i]=1.0;
				dSxx[i]=(+1)/pow(cDgn_Zero,40);
			}
		}
	}
	_DGN_STRS_CRC()	{Initialize();}
	_DGN_STRS_CRC(const _DGN_STRS_CRC& rData)	{*this=rData;}
	_DGN_STRS_CRC& operator = (const _DGN_STRS_CRC& rData)
	{
		for(int i=0; i<cPOSI_CRC; i++)
		{
			iRefNo[i]=rData.iRefNo[i];
			iSerialStep[i]=rData.iSerialStep[i];
			dStop[i]=rData.dStop[i]; dSbot[i]=rData.dSbot[i];
			dStl[i]=rData.dStl[i]; dSbl[i]=rData.dSbl[i];
			dStr[i]=rData.dStr[i]; dSbr[i]=rData.dSbr[i];
			dScom[i]=rData.dScom[i];
			dExtraRat[i]=rData.dExtraRat[i];
			dSxx[i]=rData.dSxx[i];
		}
		return *this;
	}
	_DGN_STRS_CRC& operator += (const _DGN_STRS_CRC& rData)
	{
		for(int i=0; i<cPOSI_CRC; i++)
		{
			dStop[i]+=rData.dStop[i]; dSbot[i]+=rData.dSbot[i];
			dStl[i]+=rData.dStl[i]; dSbl[i]+=rData.dSbl[i];
			dStr[i]+=rData.dStr[i]; dSbr[i]+=rData.dSbr[i];
			dScom[i]+=rData.dScom[i];
			dExtraRat[i]+=rData.dExtraRat[i];
			dSxx[i]+=rData.dSxx[i];
		}
		return *this;
	}
	_DGN_STRS_CRC& operator -= (const _DGN_STRS_CRC& rData)
	{
		for(int i=0; i<cPOSI_CRC; i++)
		{
			dStop[i]-=rData.dStop[i]; dSbot[i]-=rData.dSbot[i];
			dStl[i]-=rData.dStl[i]; dSbl[i]-=rData.dSbl[i];
			dStr[i]-=rData.dStr[i]; dSbr[i]-=rData.dSbr[i];
			dScom[i]-=rData.dScom[i];
			dExtraRat[i]-=rData.dExtraRat[i];
			dSxx[i]-=rData.dSxx[i];
		}
		return *this;
	}
	_DGN_STRS_CRC& operator *= (const double dFactor)
	{
		for(int i=0; i<cPOSI_CRC; i++)
		{
			dStop[i]*=dFactor; dSbot[i]*=dFactor;
			dStl[i]*=dFactor;  dSbl[i]*=dFactor;
			dStr[i]*=dFactor;  dSbr[i]*=dFactor;
			dScom[i]*=dFactor;
			dExtraRat[i]*=dFactor;
			dSxx[i]*=dFactor;
		}
		return *this;
	}

	BOOL operator==(const _DGN_STRS_CRC& rhs)
	{
		for (int i = 0; i < cPOSI_CRC; i++)
		{
			//if (lhs.dStop[i] != rhs.dStop[i]) return false;
			//if (lhs.dSbot[i] != rhs.dSbot[i]) return false;
			if (UQ(dStl[i], rhs.dStl[i])) return FALSE;
			if (UQ(dSbl[i], rhs.dSbl[i])) return FALSE;
			if (UQ(dStr[i], rhs.dStr[i])) return FALSE;
			if (UQ(dSbr[i], rhs.dSbr[i])) return FALSE;
		}
		return TRUE;
	}


};

struct _DGN_STRS_CRC_CH
{
	char strType[16];
    char strLCSubType[16];
	int iRefNo[cPOSI_CRC];	// StagK, DgnLcomK,...
	double dStop[cPOSI_CRC], dSbot[cPOSI_CRC];	// 상단, 하단 응력도.
	double dStl[cPOSI_CRC], dSbl[cPOSI_CRC];		// 상단좌단, 하단좌단 응력도.
	double dStr[cPOSI_CRC], dSbr[cPOSI_CRC];		// 상단우단, 하단우단 응력도.
	double dScom[cPOSI_CRC];										// 최대/최소 응력도.
	double dExtraRat[cPOSI_CRC];								// 할증계수.
	double dSxx[cPOSI_CRC];                     // 축응력
	void ConvertToString(_DGN_STRS_CRC& rData)
	{
		ConvertCharStr(strType, rData.strType, sizeof(strType));
        ConvertCharStr(strLCSubType, rData.strLCSubType, sizeof(strLCSubType));

		for(int i=0; i<cPOSI_CRC; i++)
		{
			rData.iRefNo[i]=iRefNo[i];
			rData.dStop[i]=dStop[i]; rData.dSbot[i]=dSbot[i];
			rData.dStl[i]=dStl[i]; rData.dSbl[i]=dSbl[i];
			rData.dStr[i]=dStr[i]; rData.dSbr[i]=dSbr[i];
			rData.dScom[i]=dScom[i];
			rData.dExtraRat[i]=dExtraRat[i];
			rData.dSxx[i]=dSxx[i];
		}
	}
	void ConvertToChar(_DGN_STRS_CRC& rData)
	{
		ConvertStrChar(rData.strType, strType, sizeof(strType));
        ConvertStrChar(rData.strLCSubType, strLCSubType, sizeof(strLCSubType));

		for(int i=0; i<cPOSI_CRC; i++)
		{
			iRefNo[i]=rData.iRefNo[i];
			dStop[i]=rData.dStop[i]; dSbot[i]=rData.dSbot[i];
			dStl[i]=rData.dStl[i]; dSbl[i]=rData.dSbl[i];
			dStr[i]=rData.dStr[i]; dSbr[i]=rData.dSbr[i];
			dScom[i]=rData.dScom[i];
			dExtraRat[i]=rData.dExtraRat[i];
			dSxx[i]=rData.dSxx[i];
		}
	}

	void Initialize()
	{    
		for(int i=0; i<cPOSI_CRC; i++)
		{
			iRefNo[i]=0;
			dStop[i]=0.0; dSbot[i]=0.0;
			dStl[i]=0.0; dSbl[i]=0.0;
			dStr[i]=0.0; dSbr[i]=0.0;
			dScom[i]=0.0;
			dExtraRat[i]=1.0;
			dSxx[i]=0.0;
		}
	}

};

typedef CArray<_DGN_STRS_CRC,_DGN_STRS_CRC&> ADGNSTRESS;

struct _DGN_STRS_CRC_ELEM
{
	CArray<_DGN_STRS_CRC,_DGN_STRS_CRC&> aStressLcom;
	void Initialize()
	{
		aStressLcom.RemoveAll();
	}
	_DGN_STRS_CRC_ELEM() {}
	_DGN_STRS_CRC_ELEM(const _DGN_STRS_CRC_ELEM& rData) {*this=rData;}
	_DGN_STRS_CRC_ELEM& operator = (const _DGN_STRS_CRC_ELEM& rData)
	{
		aStressLcom.Copy(rData.aStressLcom);
		return *this;
	}
	_DGN_STRS_CRC_ELEM& operator += (const _DGN_STRS_CRC_ELEM& rData)
	{
		int iStressNumOrg = aStressLcom.GetSize();
		int iStressNumApp = rData.aStressLcom.GetSize();
		aStressLcom.SetSize(iStressNumApp);
		_DGN_STRS_CRC StressCrcOrg, StressCrcApp;
		for(int i=0; i<iStressNumApp; i++)
		{
			StressCrcOrg.Initialize();
			StressCrcApp.Initialize();
			if(i < iStressNumOrg) StressCrcOrg = aStressLcom.GetAt(i);
			if(i < iStressNumApp) StressCrcApp = rData.aStressLcom.GetAt(i);
			StressCrcOrg += StressCrcApp;
			aStressLcom.SetAt(i,StressCrcOrg);
		}
		return *this;
	}
	_DGN_STRS_CRC_ELEM& operator -= (const _DGN_STRS_CRC_ELEM& rData)
	{
		int iStressNumOrg = aStressLcom.GetSize();
		int iStressNumApp = rData.aStressLcom.GetSize();
		aStressLcom.SetSize(iStressNumApp);
		_DGN_STRS_CRC StressCrcOrg, StressCrcApp;
		for(int i=0; i<iStressNumApp; i++)
		{
			StressCrcOrg.Initialize();
			StressCrcApp.Initialize();
			if(i < iStressNumOrg) StressCrcOrg = aStressLcom.GetAt(i);
			if(i < iStressNumApp) StressCrcApp = rData.aStressLcom.GetAt(i);
			StressCrcOrg -= StressCrcApp;
			aStressLcom.SetAt(i,StressCrcOrg);
		}
		return *this;
	}
};

//////////////////////////////////////////////////////////////////////////
// _DGN_STRS_CRC_EX
//////////////////////////////////////////////////////////////////////////
struct _DGN_STRS_CRC_EX
{
	CString strType;
    CString strLCSubType;
	int iRefNo[cPOSI_CRC_EX];	// StagK, DgnLcomK,...
	int iSerialStep[cPOSI_CRC_EX]; // Step
	double dStop[cPOSI_CRC_EX], dSbot[cPOSI_CRC_EX];	// 상단, 하단 응력도.
	double dStl[cPOSI_CRC_EX], dSbl[cPOSI_CRC_EX];		// 상단좌단, 하단좌단 응력도.
	double dStr[cPOSI_CRC_EX], dSbr[cPOSI_CRC_EX];		// 상단우단, 하단우단 응력도.
	double dScom[cPOSI_CRC_EX];										// 최대/최소 응력도.
	double dExtraRat[cPOSI_CRC_EX];								// 할증계수.
	double dSxx[cPOSI_CRC_EX];                     // 축응력
	void Initialize()
	{
		strType=_T("");
        strLCSubType=_T("");
		for(int i=0; i<cPOSI_CRC_EX; i++)
		{
			iRefNo[i]=0;
			iSerialStep[i]=0;
			dStop[i]=0.0; dSbot[i]=0.0;
			dStl[i]=0.0; dSbl[i]=0.0;
			dStr[i]=0.0; dSbr[i]=0.0;
			dScom[i]=0.0;
			dExtraRat[i]=1.0;
			dSxx[i]=0.0;
		}
	}
	void Initialize(BOOL bMax)
	{
		strType=_T("");
        strLCSubType=_T("");
		for(int i=0; i<cPOSI_CRC_EX; i++)
		{
			if(bMax)
			{
				iRefNo[i]=0;
				iSerialStep[i]=0;
				dStop[i]=(-1)/pow(cDgn_Zero,40); dSbot[i]=(-1)/pow(cDgn_Zero,40);
				dStl[i]=(-1)/pow(cDgn_Zero,40); dSbl[i]=(-1)/pow(cDgn_Zero,40);
				dStr[i]=(-1)/pow(cDgn_Zero,40); dSbr[i]=(-1)/pow(cDgn_Zero,40);
				dScom[i]=(-1)/pow(cDgn_Zero,40);
				dExtraRat[i]=1.0;				
				dSxx[i]=(-1)/pow(cDgn_Zero,40);
			}
			else
			{
				iRefNo[i]=0;
				iSerialStep[i]=0;
				dStop[i]=(+1)/pow(cDgn_Zero,40); dSbot[i]=(+1)/pow(cDgn_Zero,40);
				dStl[i]=(+1)/pow(cDgn_Zero,40); dSbl[i]=(+1)/pow(cDgn_Zero,40);
				dStr[i]=(+1)/pow(cDgn_Zero,40); dSbr[i]=(+1)/pow(cDgn_Zero,40);
				dScom[i]=(+1)/pow(cDgn_Zero,40);
				dExtraRat[i]=1.0;
				dSxx[i]=(+1)/pow(cDgn_Zero,40);
			}
		}
	}
	_DGN_STRS_CRC_EX()	{Initialize();}
	_DGN_STRS_CRC_EX(const _DGN_STRS_CRC_EX& rData)	{*this=rData;}
	_DGN_STRS_CRC_EX& operator = (const _DGN_STRS_CRC_EX& rData)
	{
		for(int i=0; i<cPOSI_CRC_EX; i++)
		{
			iRefNo[i]=rData.iRefNo[i];
			iSerialStep[i]=rData.iSerialStep[i];
			dStop[i]=rData.dStop[i]; dSbot[i]=rData.dSbot[i];
			dStl[i]=rData.dStl[i]; dSbl[i]=rData.dSbl[i];
			dStr[i]=rData.dStr[i]; dSbr[i]=rData.dSbr[i];
			dScom[i]=rData.dScom[i];
			dExtraRat[i]=rData.dExtraRat[i];
			dSxx[i]=rData.dSxx[i];
		}
		return *this;
	}
	_DGN_STRS_CRC_EX& operator += (const _DGN_STRS_CRC_EX& rData)
	{
		for(int i=0; i<cPOSI_CRC_EX; i++)
		{
			dStop[i]+=rData.dStop[i]; dSbot[i]+=rData.dSbot[i];
			dStl[i]+=rData.dStl[i]; dSbl[i]+=rData.dSbl[i];
			dStr[i]+=rData.dStr[i]; dSbr[i]+=rData.dSbr[i];
			dScom[i]+=rData.dScom[i];
			dExtraRat[i]+=rData.dExtraRat[i];
			dSxx[i]+=rData.dSxx[i];
		}
		return *this;
	}
	_DGN_STRS_CRC_EX& operator -= (const _DGN_STRS_CRC_EX& rData)
	{
		for(int i=0; i<cPOSI_CRC_EX; i++)
		{
			dStop[i]-=rData.dStop[i]; dSbot[i]-=rData.dSbot[i];
			dStl[i]-=rData.dStl[i]; dSbl[i]-=rData.dSbl[i];
			dStr[i]-=rData.dStr[i]; dSbr[i]-=rData.dSbr[i];
			dScom[i]-=rData.dScom[i];
			dExtraRat[i]-=rData.dExtraRat[i];
			dSxx[i]-=rData.dSxx[i];
		}
		return *this;
	}
	_DGN_STRS_CRC_EX& operator *= (const double dFactor)
	{
		for(int i=0; i<cPOSI_CRC_EX; i++)
		{
			dStop[i]*=dFactor; dSbot[i]*=dFactor;
			dStl[i]*=dFactor;  dSbl[i]*=dFactor;
			dStr[i]*=dFactor;  dSbr[i]*=dFactor;
			dScom[i]*=dFactor;
			dExtraRat[i]*=dFactor;
			dSxx[i]*=dFactor;
		}
		return *this;
	}

    void GetStress(_DGN_STRS_CRC& rData)
    {
        rData.strType = strType;
        rData.strLCSubType = strLCSubType;
        for ( int i=0; i<cPOSI_CRC; ++i )
        {
            int nPos = i==1 ? 4 : 0;
            rData.iRefNo[i] = iRefNo[nPos];
            rData.iSerialStep[i] = iSerialStep[nPos];
            rData.dStop[i] = dStop[nPos];
            rData.dSbot[i] = dSbot[nPos];
            rData.dStl[i]  = dStl[nPos];
            rData.dSbl[i]  = dSbl[nPos];
            rData.dStr[i]  = dStr[nPos];
            rData.dSbr[i]  = dSbr[nPos];
            rData.dScom[i] = dScom[nPos];
            rData.dExtraRat[i] = dExtraRat[nPos];
            rData.dSxx[i] = dSxx[nPos];
        }
    }

};

struct _DGN_STRS_CRC_EX_CH
{
	char strType[16];
    char strLCSubType[16];
	int iRefNo[cPOSI_CRC_EX];	// StagK, DgnLcomK,...
	double dStop[cPOSI_CRC_EX], dSbot[cPOSI_CRC_EX];	// 상단, 하단 응력도.
	double dStl[cPOSI_CRC_EX], dSbl[cPOSI_CRC_EX];		// 상단좌단, 하단좌단 응력도.
	double dStr[cPOSI_CRC_EX], dSbr[cPOSI_CRC_EX];		// 상단우단, 하단우단 응력도.
	double dScom[cPOSI_CRC_EX];										// 최대/최소 응력도.
	double dExtraRat[cPOSI_CRC_EX];								// 할증계수.
	double dSxx[cPOSI_CRC_EX];                     // 축응력
	void ConvertToString(_DGN_STRS_CRC_EX& rData)
	{
		ConvertCharStr(strType, rData.strType, sizeof(strType));
        ConvertCharStr(strLCSubType, rData.strLCSubType, sizeof(strLCSubType));

		for(int i=0; i<cPOSI_CRC_EX; i++)
		{
			rData.iRefNo[i]=iRefNo[i];
			rData.dStop[i]=dStop[i]; rData.dSbot[i]=dSbot[i];
			rData.dStl[i]=dStl[i]; rData.dSbl[i]=dSbl[i];
			rData.dStr[i]=dStr[i]; rData.dSbr[i]=dSbr[i];
			rData.dScom[i]=dScom[i];
			rData.dExtraRat[i]=dExtraRat[i];
			rData.dSxx[i]=dSxx[i];
		}
	}
	void ConvertToChar(_DGN_STRS_CRC_EX& rData)
	{
		ConvertStrChar(rData.strType, strType, sizeof(strType));
        ConvertStrChar(rData.strLCSubType, strLCSubType, sizeof(strLCSubType));

		for(int i=0; i<cPOSI_CRC_EX; i++)
		{
			iRefNo[i]=rData.iRefNo[i];
			dStop[i]=rData.dStop[i]; dSbot[i]=rData.dSbot[i];
			dStl[i]=rData.dStl[i]; dSbl[i]=rData.dSbl[i];
			dStr[i]=rData.dStr[i]; dSbr[i]=rData.dSbr[i];
			dScom[i]=rData.dScom[i];
			dExtraRat[i]=rData.dExtraRat[i];
			dSxx[i]=rData.dSxx[i];
		}
	}

	void Initialize()
	{    
		for(int i=0; i<cPOSI_CRC_EX; i++)
		{
			iRefNo[i]=0;
			dStop[i]=0.0; dSbot[i]=0.0;
			dStl[i]=0.0; dSbl[i]=0.0;
			dStr[i]=0.0; dSbr[i]=0.0;
			dScom[i]=0.0;
			dExtraRat[i]=1.0;
			dSxx[i]=0.0;
		}
	}

};

typedef CArray<_DGN_STRS_CRC_EX,_DGN_STRS_CRC_EX&> ADGNSTRESS_EX;

struct _DGN_STRS_CRC_EX_ELEM
{
	CArray<_DGN_STRS_CRC_EX,_DGN_STRS_CRC_EX&> aStressLcom;
	void Initialize()
	{
		aStressLcom.RemoveAll();
	}
	_DGN_STRS_CRC_EX_ELEM() {}
	_DGN_STRS_CRC_EX_ELEM(const _DGN_STRS_CRC_EX_ELEM& rData) {*this=rData;}
	_DGN_STRS_CRC_EX_ELEM& operator = (const _DGN_STRS_CRC_EX_ELEM& rData)
	{
		aStressLcom.Copy(rData.aStressLcom);
		return *this;
	}
	_DGN_STRS_CRC_EX_ELEM& operator += (const _DGN_STRS_CRC_EX_ELEM& rData)
	{
		int iStressNumOrg = aStressLcom.GetSize();
		int iStressNumApp = rData.aStressLcom.GetSize();
		aStressLcom.SetSize(iStressNumApp);
		_DGN_STRS_CRC_EX StressCrcOrg, StressCrcApp;
		for(int i=0; i<iStressNumApp; i++)
		{
			StressCrcOrg.Initialize();
			StressCrcApp.Initialize();
			if(i < iStressNumOrg) StressCrcOrg = aStressLcom.GetAt(i);
			if(i < iStressNumApp) StressCrcApp = rData.aStressLcom.GetAt(i);
			StressCrcOrg += StressCrcApp;
			aStressLcom.SetAt(i,StressCrcOrg);
		}
		return *this;
	}
	_DGN_STRS_CRC_EX_ELEM& operator -= (const _DGN_STRS_CRC_EX_ELEM& rData)
	{
		int iStressNumOrg = aStressLcom.GetSize();
		int iStressNumApp = rData.aStressLcom.GetSize();
		aStressLcom.SetSize(iStressNumApp);
		_DGN_STRS_CRC_EX StressCrcOrg, StressCrcApp;
		for(int i=0; i<iStressNumApp; i++)
		{
			StressCrcOrg.Initialize();
			StressCrcApp.Initialize();
			if(i < iStressNumOrg) StressCrcOrg = aStressLcom.GetAt(i);
			if(i < iStressNumApp) StressCrcApp = rData.aStressLcom.GetAt(i);
			StressCrcOrg -= StressCrcApp;
			aStressLcom.SetAt(i,StressCrcOrg);
		}
		return *this;
	}
};

//////////////////////////////////////////////////////////////////////////
// _DGN_SHRS_CRC
//////////////////////////////////////////////////////////////////////////

struct _DGN_SHRS_CRC
{
	int iRefNo[cPOSI_CRC];	// StagK, DgnLcomK,...
	int iSerialStep[cPOSI_CRC]; // Step
	// 평균값.
	double dTsvm[cPOSI_CRC], dTstm[cPOSI_CRC];	// 평균전단응력도 (전단, 전단+비틀림).
	double dSsvm[cPOSI_CRC], dSstm[cPOSI_CRC];	// 가상 경사인장응력도 (전단, 전단+비틀림).
	double dExtraRat[cPOSI_CRC];								// 할증계수.
	// 2nd[] : 2-2, G-G, 3-3 위치.
	// 위치별 계산값.
	double dTsh[cPOSI_CRC][3];	// 전단력에 의한 전단응력도.
	double dTst[cPOSI_CRC][3];	// 비틀림에 의한 전단응력도.
	double dSsh[cPOSI_CRC][3];	// 전단력에 의한 경사인장응력도.
	double dSst[cPOSI_CRC][3];	// 비틀림에 의한 경사인장응력도.
	// 계산시 임시값.
	double dSxi[cPOSI_CRC][3];	// Local-x 방향 응력의 합.
	double dSzi[cPOSI_CRC][3];	// Local-z 방향 응력의 합.
	double dTpi[cPOSI_CRC][3];	// 전단강봉에 의한 전단응력도.
	// 주응력 계산값.
	double dSpi[cPOSI_CRC][3];	// 주응력도.
	double dSsp[cPOSI_CRC][10];	// 검토위치 1~10에서 주응력도.
	double dSspAbs[cPOSI_CRC];	// Max/Min 주응력도.
	void Initialize()
	{
		for(int i=0; i<cPOSI_CRC; i++)
		{
			iRefNo[i]=0;
			iSerialStep[i]=0;
			dTsvm[i]=0.0; dTstm[i]=0.0;
			dSsvm[i]=0.0; dSstm[i]=0.0;
			dExtraRat[i]=1.0;
			for(int j=0; j<3; j++)
			{
				dTsh[i][j]=0.0;
				dTst[i][j]=0.0;
				dSsh[i][j]=0.0;
				dSst[i][j]=0.0;
				dSxi[i][j]=0.0;
				dSzi[i][j]=0.0;
				dTpi[i][j]=0.0;
				dSpi[i][j]=0.0;
			}
			for(int j=0; j<10; j++)	dSsp[i][j]=0.0;
			dSspAbs[i]=0.0;
		}
	}
	_DGN_SHRS_CRC()	{Initialize();}
	_DGN_SHRS_CRC(_DGN_SHRS_CRC& rData)	{*this=rData;}
	_DGN_SHRS_CRC& operator = (const _DGN_SHRS_CRC& rData)
	{
		for(int i=0; i<cPOSI_CRC; i++)
		{
			iRefNo[i]=rData.iRefNo[i];
			iSerialStep[i]=rData.iSerialStep[i];
			dTsvm[i]=rData.dTsvm[i]; dTstm[i]=rData.dTstm[i];
			dSsvm[i]=rData.dSsvm[i]; dSstm[i]=rData.dSstm[i];
			dExtraRat[i]=rData.dExtraRat[i];
			for(int j=0; j<3; j++)
			{
				dTsh[i][j]=rData.dTsh[i][j];
				dTst[i][j]=rData.dTst[i][j];
				dSsh[i][j]=rData.dSsh[i][j];
				dSst[i][j]=rData.dSst[i][j];
				dSxi[i][j]=rData.dSxi[i][j];
				dSzi[i][j]=rData.dSzi[i][j];
				dTpi[i][j]=rData.dTpi[i][j];
				dSpi[i][j]=rData.dSpi[i][j];
			}
			for(int j=0; j<10; j++)	dSsp[i][j]=rData.dSsp[i][j];
			dSspAbs[i]=rData.dSspAbs[i];
		}
		return *this;
	}
	_DGN_SHRS_CRC& operator += (_DGN_SHRS_CRC& rData)
	{
		for(int i=0; i<cPOSI_CRC; i++)
		{
			dTsvm[i]+=rData.dTsvm[i]; dTstm[i]+=rData.dTstm[i];
			dSsvm[i]+=rData.dSsvm[i]; dSstm[i]+=rData.dSstm[i];
			dExtraRat[i]+=rData.dExtraRat[i];
			for(int j=0; j<3; j++)
			{
				dTsh[i][j]+=rData.dTsh[i][j];
				dTst[i][j]+=rData.dTst[i][j];
				dSsh[i][j]+=rData.dSsh[i][j];
				dSst[i][j]+=rData.dSst[i][j];
				dSxi[i][j]+=rData.dSxi[i][j];
				dSzi[i][j]+=rData.dSzi[i][j];
				dTpi[i][j]+=rData.dTpi[i][j];
				dSpi[i][j]+=rData.dSpi[i][j];
			}
			for(int j=0; j<10; j++)	dSsp[i][j]+=rData.dSsp[i][j];
			dSspAbs[i]+=rData.dSspAbs[i];
		}
		return *this;
	}
};

typedef CArray<_DGN_SHRS_CRC,_DGN_SHRS_CRC&> ADGNSHRS;

//////////////////////////////////////////////////////////////////////////
// _DGN_SHRS_CRC_EX
//////////////////////////////////////////////////////////////////////////
struct _DGN_SHRS_CRC_EX
{
	int iRefNo[cPOSI_CRC_EX];	// StagK, DgnLcomK,...
	int iSerialStep[cPOSI_CRC_EX]; // Step
	// 평균값.
	double dTsvm[cPOSI_CRC_EX], dTstm[cPOSI_CRC_EX];	// 평균전단응력도 (전단, 전단+비틀림).
	double dSsvm[cPOSI_CRC_EX], dSstm[cPOSI_CRC_EX];	// 가상 경사인장응력도 (전단, 전단+비틀림).
	double dExtraRat[cPOSI_CRC_EX];								// 할증계수.
	// 2nd[] : 2-2, G-G, 3-3 위치.
	// 위치별 계산값.
	double dTsh[cPOSI_CRC_EX][3];	// 전단력에 의한 전단응력도.
	double dTst[cPOSI_CRC_EX][3];	// 비틀림에 의한 전단응력도.
	double dSsh[cPOSI_CRC_EX][3];	// 전단력에 의한 경사인장응력도.
	double dSst[cPOSI_CRC_EX][3];	// 비틀림에 의한 경사인장응력도.
	// 계산시 임시값.
	double dSxi[cPOSI_CRC_EX][3];	// Local-x 방향 응력의 합.
	double dSzi[cPOSI_CRC_EX][3];	// Local-z 방향 응력의 합.
	double dTpi[cPOSI_CRC_EX][3];	// 전단강봉에 의한 전단응력도.
	// 주응력 계산값.
	double dSpi[cPOSI_CRC_EX][3];	// 주응력도.
	double dSsp[cPOSI_CRC_EX][10];	// 검토위치 1~10에서 주응력도.
	double dSspAbs[cPOSI_CRC_EX];	// Max/Min 주응력도.
	void Initialize()
	{
		for(int i=0; i<cPOSI_CRC_EX; i++)
		{
			iRefNo[i]=0;
			iSerialStep[i]=0;
			dTsvm[i]=0.0; dTstm[i]=0.0;
			dSsvm[i]=0.0; dSstm[i]=0.0;
			dExtraRat[i]=1.0;
			for(int j=0; j<3; j++)
			{
				dTsh[i][j]=0.0;
				dTst[i][j]=0.0;
				dSsh[i][j]=0.0;
				dSst[i][j]=0.0;
				dSxi[i][j]=0.0;
				dSzi[i][j]=0.0;
				dTpi[i][j]=0.0;
				dSpi[i][j]=0.0;
			}
			for(int j=0; j<10; j++)	dSsp[i][j]=0.0;
			dSspAbs[i]=0.0;
		}
	}
	_DGN_SHRS_CRC_EX()	{Initialize();}
	_DGN_SHRS_CRC_EX(_DGN_SHRS_CRC_EX& rData)	{*this=rData;}
	_DGN_SHRS_CRC_EX& operator = (_DGN_SHRS_CRC_EX& rData)
	{
		for(int i=0; i<cPOSI_CRC_EX; i++)
		{
			iRefNo[i]=rData.iRefNo[i];
			iSerialStep[i]=rData.iSerialStep[i];
			dTsvm[i]=rData.dTsvm[i]; dTstm[i]=rData.dTstm[i];
			dSsvm[i]=rData.dSsvm[i]; dSstm[i]=rData.dSstm[i];
			dExtraRat[i]=rData.dExtraRat[i];
			for(int j=0; j<3; j++)
			{
				dTsh[i][j]=rData.dTsh[i][j];
				dTst[i][j]=rData.dTst[i][j];
				dSsh[i][j]=rData.dSsh[i][j];
				dSst[i][j]=rData.dSst[i][j];
				dSxi[i][j]=rData.dSxi[i][j];
				dSzi[i][j]=rData.dSzi[i][j];
				dTpi[i][j]=rData.dTpi[i][j];
				dSpi[i][j]=rData.dSpi[i][j];
			}
			for(int j=0; j<10; j++)	dSsp[i][j]=rData.dSsp[i][j];
			dSspAbs[i]=rData.dSspAbs[i];
		}
		return *this;
	}
	_DGN_SHRS_CRC_EX& operator += (_DGN_SHRS_CRC_EX& rData)
	{
		for(int i=0; i<cPOSI_CRC_EX; i++)
		{
			dTsvm[i]+=rData.dTsvm[i]; dTstm[i]+=rData.dTstm[i];
			dSsvm[i]+=rData.dSsvm[i]; dSstm[i]+=rData.dSstm[i];
			dExtraRat[i]+=rData.dExtraRat[i];
			for(int j=0; j<3; j++)
			{
				dTsh[i][j]+=rData.dTsh[i][j];
				dTst[i][j]+=rData.dTst[i][j];
				dSsh[i][j]+=rData.dSsh[i][j];
				dSst[i][j]+=rData.dSst[i][j];
				dSxi[i][j]+=rData.dSxi[i][j];
				dSzi[i][j]+=rData.dSzi[i][j];
				dTpi[i][j]+=rData.dTpi[i][j];
				dSpi[i][j]+=rData.dSpi[i][j];
			}
			for(int j=0; j<10; j++)	dSsp[i][j]+=rData.dSsp[i][j];
			dSspAbs[i]+=rData.dSspAbs[i];
		}
		return *this;
	}
};

typedef CArray<_DGN_SHRS_CRC_EX,_DGN_SHRS_CRC_EX&> ADGNSHRS_EX;


//////////////////////////////////////////////////////////////////////////
// _DGN_CRCK_JTG
//////////////////////////////////////////////////////////////////////////

struct _DGN_CRCK_JTG
{
	// See JTG D62-04 6.4.3 C2(Variable).
	// 0=영구하중시, 1=FR이없는경우, 2=SF가있는경우, 3=IP가있는경우.
	// Fxx or Muy : 0=(+), 1=(-).
	double dFxxLT[cPOSI_CRC][4][2];	// Long Term.
	double dFxxST[cPOSI_CRC][4][2];	// Short Term.
	double dMuyLT[cPOSI_CRC][4][2];	// Long Term.
	double dMuyST[cPOSI_CRC][4][2];	// Short Term.
	void Initialize()
	{
		for(int i=0; i<cPOSI_CRC; i++)
		{
			for(int j=0; j<4; j++)
			{
				dFxxLT[i][j][0]=0.0; dFxxLT[i][j][1]=0.0;
				dFxxST[i][j][0]=0.0; dFxxST[i][j][1]=0.0;
				dMuyLT[i][j][0]=0.0; dMuyLT[i][j][1]=0.0;
				dMuyST[i][j][0]=0.0; dMuyST[i][j][1]=0.0;
			}
		}
	}
};

struct _DGN_CRCK_JTG_ELEM
{
	CArray<_DGN_CRCK_JTG,_DGN_CRCK_JTG> aCrckForc;
	void Initialize()
	{
		aCrckForc.RemoveAll();
	}
	_DGN_CRCK_JTG_ELEM() {}
	_DGN_CRCK_JTG_ELEM(const _DGN_CRCK_JTG_ELEM& rData) {*this=rData;}
	_DGN_CRCK_JTG_ELEM& operator = (const _DGN_CRCK_JTG_ELEM& rData)
	{
		aCrckForc.Copy(rData.aCrckForc);
		return *this;
	}
};

//////////////////////////////////////////////////////////////////////////
// _DGN_CRCK_JTG_EX
//////////////////////////////////////////////////////////////////////////

struct _DGN_CRCK_JTG_EX
{
	// See JTG D62-04 6.4.3 C2(Variable).
	// 0=영구하중시, 1=FR이없는경우, 2=SF가있는경우, 3=IP가있는경우.
	// Fxx or Muy : 0=(+), 1=(-).
	double dFxxLT[cPOSI_CRC_EX][4][2];	// Long Term.
	double dFxxST[cPOSI_CRC_EX][4][2];	// Short Term.
	double dMuyLT[cPOSI_CRC_EX][4][2];	// Long Term.
	double dMuyST[cPOSI_CRC_EX][4][2];	// Short Term.
	void Initialize()
	{
		for(int i=0; i<cPOSI_CRC_EX; i++)
		{
			for(int j=0; j<4; j++)
			{
				dFxxLT[i][j][0]=0.0; dFxxLT[i][j][1]=0.0;
				dFxxST[i][j][0]=0.0; dFxxST[i][j][1]=0.0;
				dMuyLT[i][j][0]=0.0; dMuyLT[i][j][1]=0.0;
				dMuyST[i][j][0]=0.0; dMuyST[i][j][1]=0.0;
			}
		}
	}
};

struct _DGN_CRCK_JTG_EX_ELEM
{
	CArray<_DGN_CRCK_JTG_EX,_DGN_CRCK_JTG_EX> aCrckForc;
	void Initialize()
	{
		aCrckForc.RemoveAll();
	}
	_DGN_CRCK_JTG_EX_ELEM() {}
	_DGN_CRCK_JTG_EX_ELEM(const _DGN_CRCK_JTG_EX_ELEM& rData) {*this=rData;}
	_DGN_CRCK_JTG_EX_ELEM& operator = (const _DGN_CRCK_JTG_EX_ELEM& rData)
	{
		aCrckForc.Copy(rData.aCrckForc);
		return *this;
	}
};

//////////////////////////////////////////////////////////////////////////
// _DGN_STRS_ADD
//////////////////////////////////////////////////////////////////////////

// Add by ZINU.('05.01.03). For Beam Section Temperature (Btss, Btms).
struct _DGN_STRS_ADD
{
	double dStr[cPOSI_CRC][12]; // 10개 지점 + My에 대한 응력 Top,Bottom 
	void Initialize()
	{
		memset(dStr,0,sizeof(dStr));
	}
	_DGN_STRS_ADD()	{Initialize();}
	_DGN_STRS_ADD(const _DGN_STRS_ADD& rData) {*this=rData;}
	_DGN_STRS_ADD& operator = (const _DGN_STRS_ADD& rData)
	{
		memcpy(dStr,rData.dStr,sizeof(dStr));
		return *this;
	}
	_DGN_STRS_ADD& operator += (const _DGN_STRS_ADD& rData)
	{
		for(int i=0; i<cPOSI_CRC; i++)
		{
			for(int j=0; j<12; j++)	dStr[i][j]+=rData.dStr[i][j];
		}
		return *this;
	}
	_DGN_STRS_ADD& operator -= (const _DGN_STRS_ADD& rData)
	{
		for(int i=0; i<cPOSI_CRC; i++)
		{
			for(int j=0; j<12; j++)	dStr[i][j]-=rData.dStr[i][j];
		}
		return *this;
	}
	_DGN_STRS_ADD& operator *= (const double dFactor)
	{
		for(int i=0; i<cPOSI_CRC; i++)
		{
			for(int j=0; j<12; j++)	dStr[i][j]*=dFactor;
		}
		return *this;
	}
};

typedef CArray<_DGN_STRS_ADD,_DGN_STRS_ADD&> ADGNSTRSADD;

struct _DGN_STRS_ADD_ELEM
{
	CArray<_DGN_STRS_ADD,_DGN_STRS_ADD&> aStressAdd;
	void Initialize()
	{
		aStressAdd.RemoveAll();
	}  
	_DGN_STRS_ADD_ELEM() {}
	_DGN_STRS_ADD_ELEM(const _DGN_STRS_ADD_ELEM& rData) {*this=rData;}
	_DGN_STRS_ADD_ELEM& operator = (const _DGN_STRS_ADD_ELEM& rData)
	{
		aStressAdd.Copy(rData.aStressAdd);
		return *this;
	}
	_DGN_STRS_ADD_ELEM& operator += (const _DGN_STRS_ADD_ELEM& rData)
	{
		int iStrsNumOrg = aStressAdd.GetSize();
		int iStrsNumApp = rData.aStressAdd.GetSize();
		aStressAdd.SetSize(iStrsNumApp);
		_DGN_STRS_ADD StrsCrcOrg, StrsCrcApp;
		for(int i=0; i<iStrsNumApp; i++)
		{
			StrsCrcOrg.Initialize();
			StrsCrcApp.Initialize();
			if(i < iStrsNumOrg) StrsCrcOrg = aStressAdd.GetAt(i);
			if(i < iStrsNumApp) StrsCrcApp = rData.aStressAdd.GetAt(i);
			StrsCrcOrg += StrsCrcApp;
			aStressAdd.SetAt(i,StrsCrcOrg);
		}
		return *this;
	}
	_DGN_STRS_ADD_ELEM& operator -= (const _DGN_STRS_ADD_ELEM& rData)
	{
		int iStrsNumOrg = aStressAdd.GetSize();
		int iStrsNumApp = rData.aStressAdd.GetSize();
		aStressAdd.SetSize(iStrsNumApp);
		_DGN_STRS_ADD StrsCrcOrg, StrsCrcApp;
		for(int i=0; i<iStrsNumApp; i++)
		{
			StrsCrcOrg.Initialize();
			StrsCrcApp.Initialize();
			if(i < iStrsNumOrg) StrsCrcOrg = aStressAdd.GetAt(i);
			if(i < iStrsNumApp) StrsCrcApp = rData.aStressAdd.GetAt(i);
			StrsCrcOrg -= StrsCrcApp;
			aStressAdd.SetAt(i,StrsCrcOrg);
		}
		return *this;
	}  
};

struct DGN_MY_MAX_B
{
	double dMyMax;
	unsigned int ElemK;
	unsigned int SectK;
	int nElemSub;
	int nPosSub;
	int nPos5;
	int nPosIJ;	

	void Initialize()
	{
		dMyMax = 0.0;
		ElemK = 0;
		SectK = 0;
		nElemSub = 0;
		nPosSub = 0;
		nPos5 = 0;
		nPosIJ = 0;
	}

	DGN_MY_MAX_B() { Initialize(); }
};

//////////////////////////////////////////////////////////////////////////
// _DGN_STRS_ADD_EX
//////////////////////////////////////////////////////////////////////////
struct _DGN_STRS_ADD_EX
{
	double dStr[cPOSI_CRC_EX][12]; // 10개 지점 + My에 대한 응력 Top,Bottom 
	void Initialize()
	{
		memset(dStr,0,sizeof(dStr));
	}
	_DGN_STRS_ADD_EX()	{Initialize();}
	_DGN_STRS_ADD_EX(const _DGN_STRS_ADD_EX& rData) {*this=rData;}
	_DGN_STRS_ADD_EX& operator = (const _DGN_STRS_ADD_EX& rData)
	{
		memcpy(dStr,rData.dStr,sizeof(dStr));
		return *this;
	}
	_DGN_STRS_ADD_EX& operator += (const _DGN_STRS_ADD_EX& rData)
	{
		for(int i=0; i<cPOSI_CRC_EX; i++)
		{
			for(int j=0; j<12; j++)	dStr[i][j]+=rData.dStr[i][j];
		}
		return *this;
	}
	_DGN_STRS_ADD_EX& operator -= (const _DGN_STRS_ADD_EX& rData)
	{
		for(int i=0; i<cPOSI_CRC_EX; i++)
		{
			for(int j=0; j<12; j++)	dStr[i][j]-=rData.dStr[i][j];
		}
		return *this;
	}
	_DGN_STRS_ADD_EX& operator *= (const double dFactor)
	{
		for(int i=0; i<cPOSI_CRC_EX; i++)
		{
			for(int j=0; j<12; j++)	dStr[i][j]*=dFactor;
		}
		return *this;
	}
};

typedef CArray<_DGN_STRS_ADD_EX,_DGN_STRS_ADD_EX&> ADGNSTRSADD_EX;

struct _DGN_STRS_ADD_EX_ELEM
{
	CArray<_DGN_STRS_ADD_EX,_DGN_STRS_ADD_EX&> aStressAdd;
	void Initialize()
	{
		aStressAdd.RemoveAll();
	}  
	_DGN_STRS_ADD_EX_ELEM() {}
	_DGN_STRS_ADD_EX_ELEM(const _DGN_STRS_ADD_EX_ELEM& rData) {*this=rData;}
	_DGN_STRS_ADD_EX_ELEM& operator = (const _DGN_STRS_ADD_EX_ELEM& rData)
	{
		aStressAdd.Copy(rData.aStressAdd);
		return *this;
	}
	_DGN_STRS_ADD_EX_ELEM& operator += (const _DGN_STRS_ADD_EX_ELEM& rData)
	{
		int iStrsNumOrg = aStressAdd.GetSize();
		int iStrsNumApp = rData.aStressAdd.GetSize();
		aStressAdd.SetSize(iStrsNumApp);
		_DGN_STRS_ADD_EX StrsCrcOrg, StrsCrcApp;
		for(int i=0; i<iStrsNumApp; i++)
		{
			StrsCrcOrg.Initialize();
			StrsCrcApp.Initialize();
			if(i < iStrsNumOrg) StrsCrcOrg = aStressAdd.GetAt(i);
			if(i < iStrsNumApp) StrsCrcApp = rData.aStressAdd.GetAt(i);
			StrsCrcOrg += StrsCrcApp;
			aStressAdd.SetAt(i,StrsCrcOrg);
		}
		return *this;
	}
	_DGN_STRS_ADD_EX_ELEM& operator -= (const _DGN_STRS_ADD_EX_ELEM& rData)
	{
		int iStrsNumOrg = aStressAdd.GetSize();
		int iStrsNumApp = rData.aStressAdd.GetSize();
		aStressAdd.SetSize(iStrsNumApp);
		_DGN_STRS_ADD_EX StrsCrcOrg, StrsCrcApp;
		for(int i=0; i<iStrsNumApp; i++)
		{
			StrsCrcOrg.Initialize();
			StrsCrcApp.Initialize();
			if(i < iStrsNumOrg) StrsCrcOrg = aStressAdd.GetAt(i);
			if(i < iStrsNumApp) StrsCrcApp = rData.aStressAdd.GetAt(i);
			StrsCrcOrg -= StrsCrcApp;
			aStressAdd.SetAt(i,StrsCrcOrg);
		}
		return *this;
	}  
};



//////////////////////////////////////////////////////////////////////////
// _DGN_SHRS_ADD
//////////////////////////////////////////////////////////////////////////

// Add by ZINU.('05.07.04). For Stress at Stage (Stss).
struct _DGN_SHRS_ADD
{
	// 1.Axial X Normal Stress
	// 2.Moment-y X Normal Stress 
	// 3.Moment-z X Normal Stress       
	// 4.강봉 X Normal Stres
	// 5.Sum
	// 6.강봉 z Normal Stress
	// 7.Shear z Shear Stress
	// 8.Torsion Shear Stress  
	// 9.강봉 Shear Stress
	// 10.비틀림 제외 사인장 응력 
	// 11.비틀림 포함 사인장 응력 / 주응력 최대
	// 12.주응력 최소 
	// 13.warping normal stress
	// 14.pure torsion xy
	// 15.warping torsion xy
	// 16.pure torsion xz
	// 17.warping torsion xz
	// 18.summation shear xy
	// 19.summation shear xz
	double dShr[cPOSI_CRC][10][19];
	void Initialize()
	{
		memset(dShr,0,sizeof(dShr));
	}
	_DGN_SHRS_ADD()	{Initialize();}
	_DGN_SHRS_ADD(const _DGN_SHRS_ADD& rData) {*this=rData;}
	_DGN_SHRS_ADD& operator = (const _DGN_SHRS_ADD& rData)
	{
		memcpy(dShr, rData.dShr, sizeof(dShr));
		return *this;
	}
	_DGN_SHRS_ADD& operator += (const _DGN_SHRS_ADD& rData)
	{
		for(int i=0; i<cPOSI_CRC; i++)
		{
			for(int j=0; j<10; j++)
			{
				for(int k=0; k<19; k++)	dShr[i][j][k]+=rData.dShr[i][j][k];
			}
		}
		return *this;
	}
	_DGN_SHRS_ADD& operator -= (const _DGN_SHRS_ADD& rData)
	{
		for(int i=0; i<cPOSI_CRC; i++)
		{
			for(int j=0; j<10; j++)
			{
				for(int k=0; k<19; k++)	dShr[i][j][k]-=rData.dShr[i][j][k];
			}
		}
		return *this;
	}
	_DGN_SHRS_ADD& operator *= (const double dFactor)
	{
		for(int i=0; i<cPOSI_CRC; i++)
		{
			for(int j=0; j<10; j++)
			{
				for(int k=0; k<19; k++)	dShr[i][j][k]*=dFactor;
			}
		}
		return *this;
	}
};

typedef CArray<_DGN_SHRS_ADD,_DGN_SHRS_ADD&> ADGNSHRSADD;

struct _DGN_SHRS_ADD_ELEM
{  
	CArray<_DGN_SHRS_ADD,_DGN_SHRS_ADD&> aStressAdd;
	
	void Initialize()
	{
		aStressAdd.RemoveAll();
	}
	_DGN_SHRS_ADD_ELEM() {}
	_DGN_SHRS_ADD_ELEM(const _DGN_SHRS_ADD_ELEM& rData) {*this=rData;}
	_DGN_SHRS_ADD_ELEM& operator = (const _DGN_SHRS_ADD_ELEM& rData)
	{
		aStressAdd.Copy(rData.aStressAdd);
		return *this;
	}
	
	_DGN_SHRS_ADD_ELEM& operator += (const _DGN_SHRS_ADD_ELEM& rData)
	{
		int iStrsNumOrg = aStressAdd.GetSize();
		int iStrsNumApp = rData.aStressAdd.GetSize();
		aStressAdd.SetSize(iStrsNumApp);
		_DGN_SHRS_ADD StrsCrcOrg, StrsCrcApp;
		for(int i=0; i<iStrsNumApp; i++)
		{
			StrsCrcOrg.Initialize();
			StrsCrcApp.Initialize();
			if(i < iStrsNumOrg) StrsCrcOrg = aStressAdd.GetAt(i);
			if(i < iStrsNumApp) StrsCrcApp = rData.aStressAdd.GetAt(i);
			StrsCrcOrg += StrsCrcApp;
			aStressAdd.SetAt(i,StrsCrcOrg);
		}
		return *this;
	}
	_DGN_SHRS_ADD_ELEM& operator -= (const _DGN_SHRS_ADD_ELEM& rData)
	{
		int iStrsNumOrg = aStressAdd.GetSize();
		int iStrsNumApp = rData.aStressAdd.GetSize();
		aStressAdd.SetSize(iStrsNumApp);
		_DGN_SHRS_ADD StrsCrcOrg, StrsCrcApp;
		for(int i=0; i<iStrsNumApp; i++)
		{
			StrsCrcOrg.Initialize();
			StrsCrcApp.Initialize();
			if(i < iStrsNumOrg) StrsCrcOrg = aStressAdd.GetAt(i);
			if(i < iStrsNumApp) StrsCrcApp = rData.aStressAdd.GetAt(i);
			StrsCrcOrg -= StrsCrcApp;
			aStressAdd.SetAt(i,StrsCrcOrg);
		}
		return *this;
	}    
};

//////////////////////////////////////////////////////////////////////////
// _DGN_SHRS_ADD_EX
//////////////////////////////////////////////////////////////////////////

// Add by ZINU.('05.07.04). For Stress at Stage (Stss).
struct _DGN_SHRS_ADD_EX
{
	// 1.Axial X Normal Stress
	// 2.Moment-y X Normal Stress 
	// 3.Moment-z X Normal Stress       
	// 4.강봉 X Normal Stres
	// 5.Sum
	// 6.강봉 z Normal Stress
	// 7.Shear z Shear Stress
	// 8.Torsion Shear Stress  
	// 9.강봉 Shear Stress
	// 10.비틀림 제외 사인장 응력 
	// 11.비틀림 포함 사인장 응력 / 주응력 최대
	// 12.주응력 최소 
	// 13.warping normal stress
	// 14.pure torsion xy
	// 15.warping torsion xy
	// 16.pure torsion xz
	// 17.warping torsion xz
	// 18.summation shear xy
	// 19.summation shear xz
	double dShr[cPOSI_CRC_EX][10][19];
	void Initialize()
	{
		memset(dShr,0,sizeof(dShr));
	}
	_DGN_SHRS_ADD_EX()	{Initialize();}
	_DGN_SHRS_ADD_EX(const _DGN_SHRS_ADD_EX& rData) {*this=rData;}
	_DGN_SHRS_ADD_EX& operator = (const _DGN_SHRS_ADD_EX& rData)
	{
		memcpy(dShr, rData.dShr, sizeof(dShr));
		return *this;
	}
	_DGN_SHRS_ADD_EX& operator += (const _DGN_SHRS_ADD_EX& rData)
	{
		for(int i=0; i<cPOSI_CRC_EX; i++)
		{
			for(int j=0; j<10; j++)
			{
				for(int k=0; k<19; k++)	dShr[i][j][k]+=rData.dShr[i][j][k];
			}
		}
		return *this;
	}
	_DGN_SHRS_ADD_EX& operator -= (const _DGN_SHRS_ADD_EX& rData)
	{
		for(int i=0; i<cPOSI_CRC_EX; i++)
		{
			for(int j=0; j<10; j++)
			{
				for(int k=0; k<19; k++)	dShr[i][j][k]-=rData.dShr[i][j][k];
			}
		}
		return *this;
	}
	_DGN_SHRS_ADD_EX& operator *= (const double dFactor)
	{
		for(int i=0; i<cPOSI_CRC_EX; i++)
		{
			for(int j=0; j<10; j++)
			{
				for(int k=0; k<19; k++)	dShr[i][j][k]*=dFactor;
			}
		}
		return *this;
	}
};

typedef CArray<_DGN_SHRS_ADD_EX,_DGN_SHRS_ADD_EX&> ADGNSHRSADD_EX;

struct _DGN_SHRS_ADD_EX_ELEM
{  
	CArray<_DGN_SHRS_ADD_EX,_DGN_SHRS_ADD_EX&> aStressAdd;

	void Initialize()
	{
		aStressAdd.RemoveAll();
	}
	_DGN_SHRS_ADD_EX_ELEM() {}
	_DGN_SHRS_ADD_EX_ELEM(const _DGN_SHRS_ADD_EX_ELEM& rData) {*this=rData;}
	_DGN_SHRS_ADD_EX_ELEM& operator = (const _DGN_SHRS_ADD_EX_ELEM& rData)
	{
		aStressAdd.Copy(rData.aStressAdd);
		return *this;
	}

	_DGN_SHRS_ADD_EX_ELEM& operator += (const _DGN_SHRS_ADD_EX_ELEM& rData)
	{
		int iStrsNumOrg = aStressAdd.GetSize();
		int iStrsNumApp = rData.aStressAdd.GetSize();
		aStressAdd.SetSize(iStrsNumApp);
		_DGN_SHRS_ADD_EX StrsCrcOrg, StrsCrcApp;
		for(int i=0; i<iStrsNumApp; i++)
		{
			StrsCrcOrg.Initialize();
			StrsCrcApp.Initialize();
			if(i < iStrsNumOrg) StrsCrcOrg = aStressAdd.GetAt(i);
			if(i < iStrsNumApp) StrsCrcApp = rData.aStressAdd.GetAt(i);
			StrsCrcOrg += StrsCrcApp;
			aStressAdd.SetAt(i,StrsCrcOrg);
		}
		return *this;
	}
	_DGN_SHRS_ADD_EX_ELEM& operator -= (const _DGN_SHRS_ADD_EX_ELEM& rData)
	{
		int iStrsNumOrg = aStressAdd.GetSize();
		int iStrsNumApp = rData.aStressAdd.GetSize();
		aStressAdd.SetSize(iStrsNumApp);
		_DGN_SHRS_ADD_EX StrsCrcOrg, StrsCrcApp;
		for(int i=0; i<iStrsNumApp; i++)
		{
			StrsCrcOrg.Initialize();
			StrsCrcApp.Initialize();
			if(i < iStrsNumOrg) StrsCrcOrg = aStressAdd.GetAt(i);
			if(i < iStrsNumApp) StrsCrcApp = rData.aStressAdd.GetAt(i);
			StrsCrcOrg -= StrsCrcApp;
			aStressAdd.SetAt(i,StrsCrcOrg);
		}
		return *this;
	}    
};

//////////////////////////////////////////////////////////////////////////
// _DGN_STRBFORC_LCASE
//////////////////////////////////////////////////////////////////////////

struct _DGN_STRBFORC_LCASE
{
	_DGN_FORC_CRC Force;
	_DGN_STRS_CRC Stress;
	_DGN_SHRS_CRC Shrs;
	_DGN_SHRS_ADD Stss;

	void Initialize()
	{
		Force.Initialize();
		Stress.Initialize();
		Shrs.Initialize();
		Stss.Initialize();
	}

	_DGN_STRBFORC_LCASE() {Initialize();}
};

struct _DGN_STRBFORC_LCASE_CH
{
	_DGN_FORC_CRC_CH Force;
	_DGN_STRS_CRC_CH Stress;
	_DGN_SHRS_CRC Shrs;
	_DGN_SHRS_ADD Stss;  

	void Initialize()
	{
		Force.Initialize();
		Stress.Initialize();
		Shrs.Initialize();
		Stss.Initialize();
	}
	
	_DGN_STRBFORC_LCASE_CH() {Initialize();}

	void ConvertToString(_DGN_STRBFORC_LCASE& rData)
	{
		Force.ConvertToString(rData.Force);
		Stress.ConvertToString(rData.Stress);
		rData.Shrs = Shrs;
		rData.Stss = Stss;
	}
	void ConvertToChar(_DGN_STRBFORC_LCASE& rData)
	{
		Force.ConvertToChar(rData.Force);
		Stress.ConvertToChar(rData.Stress);
		Shrs = rData.Shrs;
		Stss = rData.Stss;
	}  
};

//////////////////////////////////////////////////////////////////////////
// _DGN_STRBFORC_LCASE_EX
//////////////////////////////////////////////////////////////////////////

struct _DGN_STRBFORC_LCASE_EX
{
	_DGN_FORC_CRC_EX Force;
	_DGN_STRS_CRC_EX Stress;
	_DGN_SHRS_CRC_EX Shrs;
	_DGN_SHRS_ADD_EX Stss;

	void Initialize()
	{
		Force.Initialize();
		Stress.Initialize();
		Shrs.Initialize();
		Stss.Initialize();
	}

	_DGN_STRBFORC_LCASE_EX() {Initialize();}
};

struct _DGN_STRBFORC_LCASE_EX_CH
{
	_DGN_FORC_CRC_EX_CH Force;
	_DGN_STRS_CRC_EX_CH Stress;
	_DGN_SHRS_CRC_EX Shrs;
	_DGN_SHRS_ADD_EX Stss;  

	void Initialize()
	{
		Force.Initialize();
		Stress.Initialize();
		Shrs.Initialize();
		Stss.Initialize();
	}

	_DGN_STRBFORC_LCASE_EX_CH() {Initialize();}

	void ConvertToString(_DGN_STRBFORC_LCASE_EX& rData)
	{
		Force.ConvertToString(rData.Force);
		Stress.ConvertToString(rData.Stress);
		rData.Shrs = Shrs;
		rData.Stss = Stss;
	}
	void ConvertToChar(_DGN_STRBFORC_LCASE_EX& rData)
	{
		Force.ConvertToChar(rData.Force);
		Stress.ConvertToChar(rData.Stress);
		Shrs = rData.Shrs;
		Stss = rData.Stss;
	}  
};

//////////////////////////////////////////////////////////////////////////
// _DGN_SBCFFORC_LCASE
//////////////////////////////////////////////////////////////////////////

struct _DGN_SBCFFORC_LCASE
{
	_DGN_FORC_CRC Force[cFORC_CON];
	_DGN_STRS_CRC Stress[cFORC_CON];
	_DGN_SHRS_CRC Shrs[cFORC_CON];
	_DGN_SHRS_ADD Stss[cFORC_CON];
	
	void Initialize()
	{
		for (int i=0; i<cFORC_CON; ++i)
		{
			Force[i].Initialize();
			Stress[i].Initialize();
			Shrs[i].Initialize();
			Stss[i].Initialize();
		}
	}
	
	_DGN_SBCFFORC_LCASE() {Initialize();}
};

struct _DGN_SBCFFORC_LCASE_CH
{
	_DGN_FORC_CRC_CH Force[cFORC_CON];
	_DGN_STRS_CRC_CH Stress[cFORC_CON];
	_DGN_SHRS_CRC    Shrs[cFORC_CON];
	_DGN_SHRS_ADD    Stss[cFORC_CON];
	
	void ConvertToString(_DGN_SBCFFORC_LCASE& rData)
	{
		for (int i=0; i<cFORC_CON; ++i)
		{
			Force[i].ConvertToString(rData.Force[i]);
			Stress[i].ConvertToString(rData.Stress[i]);
			rData.Shrs[i] = Shrs[i];
			rData.Stss[i] = Stss[i];
		}
	}
	void ConvertToChar(_DGN_SBCFFORC_LCASE& rData)
	{
		for (int i=0; i<cFORC_CON; ++i)
		{
			Force[i].ConvertToChar(rData.Force[i]);
			Stress[i].ConvertToChar(rData.Stress[i]);
			Shrs[i] = rData.Shrs[i];
			Stss[i] = rData.Stss[i];
		}
	}  

	void Initialize()
	{
		for (int i=0; i<cFORC_CON; ++i)
		{
			Force[i].Initialize();
			Stress[i].Initialize();
			Shrs[i].Initialize();
			Stss[i].Initialize();
		}
	}
	
	_DGN_SBCFFORC_LCASE_CH() {Initialize();}

};

//////////////////////////////////////////////////////////////////////////
// _DGN_SBCFFORC_LCASE_EX
//////////////////////////////////////////////////////////////////////////

struct _DGN_SBCFFORC_LCASE_EX
{
	_DGN_FORC_CRC_EX Force[cFORC_CON];
	_DGN_STRS_CRC_EX Stress[cFORC_CON];
	_DGN_SHRS_CRC_EX Shrs[cFORC_CON];
	_DGN_SHRS_ADD_EX Stss[cFORC_CON];

	void Initialize()
	{
		for (int i=0; i<cFORC_CON; ++i)
		{
			Force[i].Initialize();
			Stress[i].Initialize();
			Shrs[i].Initialize();
			Stss[i].Initialize();
		}
	}

	_DGN_SBCFFORC_LCASE_EX() {Initialize();}
};

struct _DGN_SBCFFORC_LCASE_EX_CH
{
	_DGN_FORC_CRC_EX_CH Force[cFORC_CON];
	_DGN_STRS_CRC_EX_CH Stress[cFORC_CON];
	_DGN_SHRS_CRC_EX    Shrs[cFORC_CON];
	_DGN_SHRS_ADD_EX    Stss[cFORC_CON];

	void ConvertToString(_DGN_SBCFFORC_LCASE_EX& rData)
	{
		for (int i=0; i<cFORC_CON; ++i)
		{
			Force[i].ConvertToString(rData.Force[i]);
			Stress[i].ConvertToString(rData.Stress[i]);
			rData.Shrs[i] = Shrs[i];
			rData.Stss[i] = Stss[i];
		}
	}
	void ConvertToChar(_DGN_SBCFFORC_LCASE_EX& rData)
	{
		for (int i=0; i<cFORC_CON; ++i)
		{
			Force[i].ConvertToChar(rData.Force[i]);
			Stress[i].ConvertToChar(rData.Stress[i]);
			Shrs[i] = rData.Shrs[i];
			Stss[i] = rData.Stss[i];
		}
	}  

	void Initialize()
	{
		for (int i=0; i<cFORC_CON; ++i)
		{
			Force[i].Initialize();
			Stress[i].Initialize();
			Shrs[i].Initialize();
			Stss[i].Initialize();
		}
	}

	_DGN_SBCFFORC_LCASE_EX_CH() {Initialize();}

};

//////////////////////////////////////////////////////////////////////////
// _DGN_STRBFORC_PART_LCASE
//////////////////////////////////////////////////////////////////////////
struct _DGN_STRBFORC_PART_LCASE
{
	_DGN_FORC_CRC ForceP1;
	_DGN_FORC_CRC ForceP2;
	_DGN_STRS_CRC StressP2; // Slab
	
	void Initialize()
	{
		ForceP1.Initialize();
		ForceP2.Initialize();
		StressP2.Initialize();
	}
	
	_DGN_STRBFORC_PART_LCASE() {Initialize();}
};

struct _DGN_STRBFORC_PART_LCASE_CH
{
	_DGN_FORC_CRC_CH ForceP1;
	_DGN_FORC_CRC_CH ForceP2;
	_DGN_STRS_CRC_CH StressP2;
	
	void ConvertToString(_DGN_STRBFORC_PART_LCASE& rData)
	{
		ForceP1.ConvertToString(rData.ForceP1);
		ForceP2.ConvertToString(rData.ForceP2);
		StressP2.ConvertToString(rData.StressP2);
	}
	void ConvertToChar(_DGN_STRBFORC_PART_LCASE& rData)
	{
		ForceP1.ConvertToChar(rData.ForceP1);
		ForceP2.ConvertToChar(rData.ForceP2);
		StressP2.ConvertToChar(rData.StressP2);
	}  

	void Initialize()
	{
		ForceP1.Initialize();
		ForceP2.Initialize();
		StressP2.Initialize();
	}
	
	_DGN_STRBFORC_PART_LCASE_CH() {Initialize();}
};

//////////////////////////////////////////////////////////////////////////
// _DGN_STRBFORC_PART_LCASE_EX
//////////////////////////////////////////////////////////////////////////
struct _DGN_STRBFORC_PART_LCASE_EX
{
	_DGN_FORC_CRC_EX ForceP1;
	_DGN_FORC_CRC_EX ForceP2;
	_DGN_STRS_CRC_EX StressP2; // Slab

	void Initialize()
	{
		ForceP1.Initialize();
		ForceP2.Initialize();
		StressP2.Initialize();
	}

	_DGN_STRBFORC_PART_LCASE_EX() {Initialize();}
};

struct _DGN_STRBFORC_PART_LCASE_EX_CH
{
	_DGN_FORC_CRC_EX_CH ForceP1;
	_DGN_FORC_CRC_EX_CH ForceP2;
	_DGN_STRS_CRC_EX_CH StressP2;

	void ConvertToString(_DGN_STRBFORC_PART_LCASE_EX& rData)
	{
		ForceP1.ConvertToString(rData.ForceP1);
		ForceP2.ConvertToString(rData.ForceP2);
		StressP2.ConvertToString(rData.StressP2);
	}
	void ConvertToChar(_DGN_STRBFORC_PART_LCASE_EX& rData)
	{
		ForceP1.ConvertToChar(rData.ForceP1);
		ForceP2.ConvertToChar(rData.ForceP2);
		StressP2.ConvertToChar(rData.StressP2);
	}  

	void Initialize()
	{
		ForceP1.Initialize();
		ForceP2.Initialize();
		StressP2.Initialize();
	}

	_DGN_STRBFORC_PART_LCASE_EX_CH() {Initialize();}
};


//////////////////////////////////////////////////////////////////////////
// _DGN_SBCFFORC_PART_LCASE
//////////////////////////////////////////////////////////////////////////
struct _DGN_SBCFFORC_PART_LCASE
{
	_DGN_FORC_CRC ForceP1[cFORC_CON];
	_DGN_FORC_CRC ForceP2[cFORC_CON];
	_DGN_STRS_CRC StressP2[cFORC_CON];
	
	void Initialize()
	{
		for (int i=0; i<cFORC_CON; ++i)
		{
			ForceP1[i].Initialize();
			ForceP2[i].Initialize();
			StressP2[i].Initialize();
		}
	}
	
	_DGN_SBCFFORC_PART_LCASE() {Initialize();}
};

struct _DGN_SBCFFORC_PART_LCASE_CH
{
	_DGN_FORC_CRC_CH ForceP1[cFORC_CON];
	_DGN_FORC_CRC_CH ForceP2[cFORC_CON];
	_DGN_STRS_CRC_CH StressP2[cFORC_CON];
	
	void ConvertToString(_DGN_SBCFFORC_PART_LCASE& rData)
	{
		for (int i=0; i<cFORC_CON; ++i)
		{
			ForceP1[i].ConvertToString(rData.ForceP1[i]);
			ForceP2[i].ConvertToString(rData.ForceP2[i]);
			StressP2[i].ConvertToString(rData.StressP2[i]);
		}
	}
	void ConvertToChar(_DGN_SBCFFORC_PART_LCASE& rData)
	{
		for (int i=0; i<cFORC_CON; ++i)
		{
			ForceP1[i].ConvertToChar(rData.ForceP1[i]);
			ForceP2[i].ConvertToChar(rData.ForceP2[i]);
			StressP2[i].ConvertToChar(rData.StressP2[i]);
		}
	}  
	
	void Initialize()
	{
		for (int i=0; i<cFORC_CON; ++i)
		{
			ForceP1[i].Initialize();
			ForceP2[i].Initialize();
			StressP2[i].Initialize();
		}
	}
	
	_DGN_SBCFFORC_PART_LCASE_CH() {Initialize();}
	
};



//////////////////////////////////////////////////////////////////////////
// _DGN_SBCFFORC_PART_LCASE_EX
//////////////////////////////////////////////////////////////////////////
struct _DGN_SBCFFORC_PART_LCASE_EX
{
	_DGN_FORC_CRC_EX ForceP1[cFORC_CON];
	_DGN_FORC_CRC_EX ForceP2[cFORC_CON];
	_DGN_STRS_CRC_EX StressP2[cFORC_CON];

	void Initialize()
	{
		for (int i=0; i<cFORC_CON; ++i)
		{
			ForceP1[i].Initialize();
			ForceP2[i].Initialize();
			StressP2[i].Initialize();
		}
	}

	_DGN_SBCFFORC_PART_LCASE_EX() {Initialize();}
};

struct _DGN_SBCFFORC_PART_LCASE_EX_CH
{
	_DGN_FORC_CRC_EX_CH ForceP1[cFORC_CON];
	_DGN_FORC_CRC_EX_CH ForceP2[cFORC_CON];
	_DGN_STRS_CRC_EX_CH StressP2[cFORC_CON];

	void ConvertToString(_DGN_SBCFFORC_PART_LCASE_EX& rData)
	{
		for (int i=0; i<cFORC_CON; ++i)
		{
			ForceP1[i].ConvertToString(rData.ForceP1[i]);
			ForceP2[i].ConvertToString(rData.ForceP2[i]);
			StressP2[i].ConvertToString(rData.StressP2[i]);
		}
	}
	void ConvertToChar(_DGN_SBCFFORC_PART_LCASE_EX& rData)
	{
		for (int i=0; i<cFORC_CON; ++i)
		{
			ForceP1[i].ConvertToChar(rData.ForceP1[i]);
			ForceP2[i].ConvertToChar(rData.ForceP2[i]);
			StressP2[i].ConvertToChar(rData.StressP2[i]);
		}
	}  

	void Initialize()
	{
		for (int i=0; i<cFORC_CON; ++i)
		{
			ForceP1[i].Initialize();
			ForceP2[i].Initialize();
			StressP2[i].Initialize();
		}
	}

	_DGN_SBCFFORC_PART_LCASE_EX_CH() {Initialize();}

};

//////////////////////////////////////////////////////////////////////////
// _DGN_SCFP_D
//////////////////////////////////////////////////////////////////////////
struct _DGN_SCFP_D
{
	// dbForce	: local force         , 2 part,
	//          component : Axial, Moment-y, Moment-z (3개)
	// dbStress: local stress        , 2 part, 
	//          component : Axial, Moment-y top, Moment-y Bot, Moment-z top, Moment-z Bot, Center, Stress Point 1, Stress Point 2, Stress Point 3, Stress Point 4 (10개) 
	// dbLStress: local stress        , 2 part, 
	//          component : Axial, Moment-y 응력 9개, Moment-z 응력 9개 (19개)

	double dbForce[cPOSI_CRC][3];
	double dbStress[cPOSI_CRC][10];
	double dbLStress[cPOSI_CRC][19];

	void Initialize()
	{
		memset(this, 0, sizeof(_DGN_SCFP_D));
	}

	_DGN_SCFP_D()	{Initialize();}
	_DGN_SCFP_D(const _DGN_SCFP_D& rData) {*this=rData;}
	_DGN_SCFP_D& operator = (const _DGN_SCFP_D& rData)
	{
		memcpy(dbForce,rData.dbForce,sizeof(dbForce));
		memcpy(dbStress,rData.dbStress,sizeof(dbStress));
		memcpy(dbLStress,rData.dbLStress,sizeof(dbLStress));
		return *this;
	}

	_DGN_SCFP_D& operator += (const _DGN_SCFP_D& rData)
	{
		for(int i=0; i<cPOSI_CRC; i++)
		{
			for(int j=0; j<3; j++)	dbForce[i][j]+=rData.dbForce[i][j];
			for(int j=0; j<10; j++)	dbStress[i][j]+=rData.dbStress[i][j];
			for(int j=0; j<19; j++)	dbLStress[i][j]+=rData.dbLStress[i][j];
		}
		return *this;
	}
	_DGN_SCFP_D& operator -= (const _DGN_SCFP_D& rData)
	{
		for(int i=0; i<cPOSI_CRC; i++)
		{
			for(int j=0; j<3; j++)	dbForce[i][j]-=rData.dbForce[i][j];
			for(int j=0; j<10; j++)	dbStress[i][j]-=rData.dbStress[i][j];
			for(int j=0; j<19; j++)	dbLStress[i][j]-=rData.dbLStress[i][j];
		}
		return *this;
	}
	_DGN_SCFP_D& operator *= (const double dFactor)
	{
		for(int i=0; i<cPOSI_CRC; i++)
		{
			for(int j=0; j<3; j++)	dbForce[i][j]*=dFactor;
			for(int j=0; j<10; j++)	dbStress[i][j]*=dFactor;
			for(int j=0; j<19; j++)	dbLStress[i][j]*=dFactor;
		}
		return *this;
	}

};


//////////////////////////////////////////////////////////////////////////
// _DGN_SCFP_EX_D
//////////////////////////////////////////////////////////////////////////
struct _DGN_SCFP_EX_D
{
	// dbForce	: local force         , 5 part,
	//          component : Axial, Moment-y, Moment-z (3개)
	// dbStress: local stress        , 5 part, 
	//          component : Axial, Moment-y top, Moment-y Bot, Moment-z top, Moment-z Bot, Center, Stress Point 1, Stress Point 2, Stress Point 3, Stress Point 4 (10개) 
	// dbLStress: local stress        , 5 part, 
	//          component : Axial, Moment-y 응력 9개, Moment-z 응력 9개 (19개)

	double dbForce[cPOSI_CRC_EX][3];
	double dbStress[cPOSI_CRC_EX][10];
	double dbLStress[cPOSI_CRC_EX][19];

	void Initialize()
	{
		memset(this, 0, sizeof(_DGN_SCFP_EX_D));
	}

	_DGN_SCFP_EX_D()	{Initialize();}
	_DGN_SCFP_EX_D(const _DGN_SCFP_EX_D& rData) {*this=rData;}
	_DGN_SCFP_EX_D& operator = (const _DGN_SCFP_EX_D& rData)
	{
		memcpy(dbForce,rData.dbForce,sizeof(dbForce));
		memcpy(dbStress,rData.dbStress,sizeof(dbStress));
		memcpy(dbLStress,rData.dbLStress,sizeof(dbLStress));
		return *this;
	}

	_DGN_SCFP_EX_D& operator += (const _DGN_SCFP_EX_D& rData)
	{
		for(int i=0; i<cPOSI_CRC_EX; i++)
		{
			for(int j=0; j<3; j++)	dbForce[i][j]+=rData.dbForce[i][j];
			for(int j=0; j<10; j++)	dbStress[i][j]+=rData.dbStress[i][j];
			for(int j=0; j<19; j++)	dbLStress[i][j]+=rData.dbLStress[i][j];
		}
		return *this;
	}
	_DGN_SCFP_EX_D& operator -= (const _DGN_SCFP_EX_D& rData)
	{
		for(int i=0; i<cPOSI_CRC_EX; i++)
		{
			for(int j=0; j<3; j++)	dbForce[i][j]-=rData.dbForce[i][j];
			for(int j=0; j<10; j++)	dbStress[i][j]-=rData.dbStress[i][j];
			for(int j=0; j<19; j++)	dbLStress[i][j]-=rData.dbLStress[i][j];
		}
		return *this;
	}
	_DGN_SCFP_EX_D& operator *= (const double dFactor)
	{
		for(int i=0; i<cPOSI_CRC_EX; i++)
		{
			for(int j=0; j<3; j++)	dbForce[i][j]*=dFactor;
			for(int j=0; j<10; j++)	dbStress[i][j]*=dFactor;
			for(int j=0; j<19; j++)	dbLStress[i][j]*=dFactor;
		}
		return *this;
	}

};

//////////////////////////////////////////////////////////////////////////
// _DGN_SCFP_PART_LCASE
//////////////////////////////////////////////////////////////////////////

struct _DGN_SCFP_PART_LCASE
{
	_DGN_SCFP_D ForceP1;
	_DGN_SCFP_D ForceP2;

	void Initialize()
	{
		ForceP1.Initialize();
		ForceP2.Initialize();    
	}

	_DGN_SCFP_PART_LCASE()	{Initialize();}
	_DGN_SCFP_PART_LCASE(const _DGN_SCFP_PART_LCASE& rData) {*this=rData;}
	_DGN_SCFP_PART_LCASE& operator = (const _DGN_SCFP_PART_LCASE& rData)
	{
		ForceP1 = rData.ForceP1;
		ForceP2 = rData.ForceP2;
		return *this;
	}

	_DGN_SCFP_PART_LCASE& operator += (const _DGN_SCFP_PART_LCASE& rData)
	{
		ForceP1 += rData.ForceP1;
		ForceP2 += rData.ForceP2;
		return *this;
	}
	_DGN_SCFP_PART_LCASE& operator -= (const _DGN_SCFP_PART_LCASE& rData)
	{
		ForceP1 -= rData.ForceP1;
		ForceP2 -= rData.ForceP2;
		return *this;
	}

};

typedef CArray<_DGN_SCFP_PART_LCASE,_DGN_SCFP_PART_LCASE&> ASCFPSTRESS; 

struct _DGN_SCFP_PART_ELEM
{
	ASCFPSTRESS aScfpStress;

	void Initialize()
	{
		aScfpStress.RemoveAll();
	}

	_DGN_SCFP_PART_ELEM() { Initialize(); }
	_DGN_SCFP_PART_ELEM(const _DGN_SCFP_PART_ELEM& rData) { *this = rData; }
	_DGN_SCFP_PART_ELEM& operator = (const _DGN_SCFP_PART_ELEM& rData)
	{
		aScfpStress.Copy(rData.aScfpStress);
		return *this;
	}
};

//////////////////////////////////////////////////////////////////////////
// _DGN_SCFP_PART_LCASE_EX
//////////////////////////////////////////////////////////////////////////

struct _DGN_SCFP_PART_LCASE_EX
{
	_DGN_SCFP_EX_D ForceP1;
	_DGN_SCFP_EX_D ForceP2;

	void Initialize()
	{
		ForceP1.Initialize();
		ForceP2.Initialize();    
	}

	_DGN_SCFP_PART_LCASE_EX()	{Initialize();}
	_DGN_SCFP_PART_LCASE_EX(const _DGN_SCFP_PART_LCASE_EX& rData) {*this=rData;}
	_DGN_SCFP_PART_LCASE_EX& operator = (const _DGN_SCFP_PART_LCASE_EX& rData)
	{
		ForceP1 = rData.ForceP1;
		ForceP2 = rData.ForceP2;
		return *this;
	}

	_DGN_SCFP_PART_LCASE_EX& operator += (const _DGN_SCFP_PART_LCASE_EX& rData)
	{
		ForceP1 += rData.ForceP1;
		ForceP2 += rData.ForceP2;
		return *this;
	}
	_DGN_SCFP_PART_LCASE_EX& operator -= (const _DGN_SCFP_PART_LCASE_EX& rData)
	{
		ForceP1 -= rData.ForceP1;
		ForceP2 -= rData.ForceP2;
		return *this;
	}

};

typedef CArray<_DGN_SCFP_PART_LCASE_EX,_DGN_SCFP_PART_LCASE_EX&> ASCFPSTRESS_EX; 

// Force/Stress
 // STLD
struct STLD_STRB_STSS
{
	// 배열이기 때문에 GetAt으로 죽는 문제 조심.
	CArray<T_STRB_D,T_STRB_D&>	arStldStrb[2]; // StldK 개수 만큼.
	CArray<T_STSS_D,T_STSS_D&>  arStldStss[2]; 
	CArray<T_BTSS_D,T_BTSS_D&>  arStldBtss[2]; 
	void Initialize()
	{
		for(int i=0; i<2; i++)
		{
			arStldStrb[i].RemoveAll();
			arStldStss[i].RemoveAll();
			arStldBtss[i].RemoveAll();
		}
	}
	STLD_STRB_STSS()	{}
	STLD_STRB_STSS(const STLD_STRB_STSS& rData) {*this=rData;}
	STLD_STRB_STSS& operator = (const STLD_STRB_STSS& rData)
	{
		int i;
		int iTotalsize = rData.arStldStrb[0].GetSize();    
		for(i=0; i<2; i++)
		{      
			arStldStrb[i].SetSize(iTotalsize); // STRB
			arStldStss[i].SetSize(iTotalsize); // STSS
			arStldBtss[i].SetSize(iTotalsize);

			arStldStrb[i].Copy(rData.arStldStrb[i]);
			arStldStss[i].Copy(rData.arStldStss[i]);
			arStldBtss[i].Copy(rData.arStldBtss[i]);
		}
		return *this;
	}
};

 // SPLC
struct SPLC_STRB_STSS
{
	// 배열이기 때문에 GetAt으로 죽는 문제 조심.
	CArray<T_STRB_D,T_STRB_D&>	arSplcStrb[2]; // SplcK 개수 만큼.
	CArray<T_STSS_D,T_STSS_D&>  arSplcStss[2]; 
	void Initialize()
	{
		for(int i=0; i<2; i++)
		{
			arSplcStrb[i].RemoveAll();
			arSplcStss[i].RemoveAll();
		}
	}
	SPLC_STRB_STSS()	{}
	SPLC_STRB_STSS(const SPLC_STRB_STSS& rData) {*this=rData;}
	SPLC_STRB_STSS& operator = (const SPLC_STRB_STSS& rData)
	{
		int i;
		int iTotalsize = rData.arSplcStrb[0].GetSize();    
		for(i=0; i<2; i++)
		{      
			arSplcStrb[i].SetSize(iTotalsize); // STRB
			arSplcStss[i].SetSize(iTotalsize); // STSS

			arSplcStrb[i].Copy(rData.arSplcStrb[i]);
			arSplcStss[i].Copy(rData.arSplcStss[i]);
		}
		return *this;
	}
};

 // ESPC
struct ESPC_STRB_STSS
{
	CArray<T_STRB_D,T_STRB_D&>	arEspcStrb[2]; // EspcK 개수 만큼.
	CArray<T_STSS_D,T_STSS_D&>  arEspcStss[2]; 
	void Initialize()
	{
		for(int i=0; i<2; i++)
		{
			arEspcStrb[i].RemoveAll();
			arEspcStss[i].RemoveAll();
		}
	}
	ESPC_STRB_STSS()	{}
	ESPC_STRB_STSS(const ESPC_STRB_STSS& rData) {*this=rData;}
	ESPC_STRB_STSS& operator = (const ESPC_STRB_STSS& rData)
	{
		int i;
		int iTotalsize = rData.arEspcStrb[0].GetSize();    
		for(i=0; i<2; i++)
		{      
			arEspcStrb[i].SetSize(iTotalsize); // STRB
			arEspcStss[i].SetSize(iTotalsize); // STSS

			arEspcStrb[i].Copy(rData.arEspcStrb[i]);
			arEspcStss[i].Copy(rData.arEspcStss[i]);
		}
		return *this;
	}
};

 // THIS
struct THIS_STRB_STSS
{
	CArray<T_STRB_D,T_STRB_D&>	arThisStrb[2]; // EspcK 개수 만큼.
	CArray<T_STSS_D,T_STSS_D&>  arThisStss[2]; 
	void Initialize()
	{
		for(int i=0; i<2; i++)
		{
			arThisStrb[i].RemoveAll();
			arThisStss[i].RemoveAll();
		}
	}
	THIS_STRB_STSS()	{}
	THIS_STRB_STSS(const THIS_STRB_STSS& rData) {*this=rData;}
	THIS_STRB_STSS& operator = (const THIS_STRB_STSS& rData)
	{
		int i;
		int iTotalsize = rData.arThisStrb[0].GetSize();    
		for(i=0; i<2; i++)
		{      
			arThisStrb[i].SetSize(iTotalsize); // STRB
			arThisStss[i].SetSize(iTotalsize); // STSS

			arThisStrb[i].Copy(rData.arThisStrb[i]);
			arThisStss[i].Copy(rData.arThisStss[i]);
		}
		return *this;
	}
};

 // SGLD
struct SGLD_STRB_STSS
{  
	CArray<T_STRB_D,T_STRB_D&>	arSgldStrb[2]; // SgldK 개수 만큼.
	CArray<T_STSS_D,T_STSS_D&>  arSgldStss[2]; 
	void Initialize()
	{
		for(int i=0; i<2; i++)
		{
			arSgldStrb[i].RemoveAll();
			arSgldStss[i].RemoveAll();
		}
	}
	SGLD_STRB_STSS()	{}
	SGLD_STRB_STSS(const SGLD_STRB_STSS& rData) {*this=rData;}
	SGLD_STRB_STSS& operator = (const SGLD_STRB_STSS& rData)
	{
		int i;
		int iTotalsize = rData.arSgldStrb[0].GetSize();    
		for(i=0; i<2; i++)
		{      
			arSgldStrb[i].SetSize(iTotalsize); // STRB
			arSgldStss[i].SetSize(iTotalsize); // STSS

			arSgldStrb[i].Copy(rData.arSgldStrb[i]);
			arSgldStss[i].Copy(rData.arSgldStss[i]);
			
		}
		return *this;
	}
};

// STEP Force/Stress
struct STEP_STRB_STSS
{
	CArray<T_STRB_D,T_STRB_D&>	arStepStrb[2]; // SgldK 개수 만큼.
	CArray<T_STSS_D,T_STSS_D&>  arStepStss[2]; 
	void Initialize()
	{
		for(int i=0; i<2; i++)
		{
			arStepStrb[i].RemoveAll();
			arStepStss[i].RemoveAll();
		}
	}
	STEP_STRB_STSS()	{}
	STEP_STRB_STSS(const STEP_STRB_STSS& rData) {*this=rData;}
	STEP_STRB_STSS& operator = (const STEP_STRB_STSS& rData)
	{
		int i;
		int iTotalsize = rData.arStepStrb[0].GetSize();    
		for(i=0; i<2; i++)
		{      
			arStepStrb[i].SetSize(iTotalsize); //STRB
			arStepStss[i].SetSize(iTotalsize); //STSS

			arStepStrb[i].Copy(rData.arStepStrb[i]);
			arStepStss[i].Copy(rData.arStepStss[i]);
			
		}
		return *this;
	}
};

 // MVLD
struct MVLD_SBCF
{
	CArray<T_SBCF_D,T_SBCF_D&>  arMvldSbcf[2]; // MvldK 개수 만큼.
	void Initialize()
	{
		for(int i=0; i<2; i++)
		{
			arMvldSbcf[i].RemoveAll();
		}
	}
	MVLD_SBCF()	{}
	MVLD_SBCF(const MVLD_SBCF& rData) {*this=rData;}
	MVLD_SBCF& operator = (const MVLD_SBCF& rData)



	{
		int i;
		int iTotalsize = rData.arMvldSbcf[0].GetSize();    
		for(i=0; i<2; i++)
		{      
			arMvldSbcf[i].SetSize(iTotalsize); 
			arMvldSbcf[i].Copy(rData.arMvldSbcf[i]);
		}
		return *this;
	}
};

 // SMLC
struct SMLC_SBCF
{
	CArray<T_SBCF_D,T_SBCF_D&>  arSmlcSbcf[2]; // StldK 개수 만큼.
	void Initialize()
	{
		for(int i=0; i<2; i++)
		{
			arSmlcSbcf[i].RemoveAll();
		}
	}
	SMLC_SBCF()	{}
	SMLC_SBCF(const SMLC_SBCF& rData) {*this=rData;}
	SMLC_SBCF& operator = (const SMLC_SBCF& rData)
	{
		int i;
		int iTotalsize = rData.arSmlcSbcf[0].GetSize();    
		for(i=0; i<2; i++)
		{      
			arSmlcSbcf[i].SetSize(iTotalsize); // BTSS
			arSmlcSbcf[i].Copy(rData.arSmlcSbcf[i]);
		}
		return *this;
	}
};

 // GILC - STRB
struct GILC_STRB_STSS
{  
	CArray<T_STRB_D,T_STRB_D&>	arGilcStrb[2]; // SgldK 개수 만큼.
	CArray<T_STSS_D,T_STSS_D&>  arGilcStss[2]; 
	void Initialize()
	{
		for(int i=0; i<2; i++)
		{
			arGilcStrb[i].RemoveAll();
			arGilcStss[i].RemoveAll();
		}
	}
	GILC_STRB_STSS()	{}
	GILC_STRB_STSS(const GILC_STRB_STSS& rData) {*this=rData;}
	GILC_STRB_STSS& operator = (const GILC_STRB_STSS& rData)
	{
		int i;
		int iTotalsize = rData.arGilcStrb[0].GetSize();    
		for(i=0; i<2; i++)
		{      
			arGilcStrb[i].SetSize(iTotalsize); // STRB
			arGilcStss[i].SetSize(iTotalsize); // STSS
			arGilcStrb[i].Copy(rData.arGilcStrb[i]);
			arGilcStss[i].Copy(rData.arGilcStss[i]);
		}
		return *this;
	}
};

 // GILC - SBCF
struct GILC_SBCF
{
	CArray<T_SBCF_D,T_SBCF_D&>  arGilcSbcf[2]; // StldK 개수 만큼.
	void Initialize()
	{
		for(int i=0; i<2; i++)
		{
			arGilcSbcf[i].RemoveAll();
		}
	}
	GILC_SBCF()	{}
	GILC_SBCF(const GILC_SBCF& rData) {*this=rData;}
	GILC_SBCF& operator = (const GILC_SBCF& rData)
	{
		int i;
		int iTotalsize = rData.arGilcSbcf[0].GetSize();    
		for(i=0; i<2; i++)
		{      
			arGilcSbcf[i].SetSize(iTotalsize); // BTSS
			arGilcSbcf[i].Copy(rData.arGilcSbcf[i]);
		}
		return *this;
	}
};

// Add by GAY. MNET:2903. ('07.07.31). Force Struct for Load Combination.
// LCOM - STRB 
struct LCOM_STRB_STSS
{  
	CArray<T_STRB_D,T_STRB_D&>	arLcomStrb[2]; // LcomK 개수 만큼.
	CArray<T_STSS_D,T_STSS_D&>  arLcomStss[2]; 
	
	void Initialize()
	{
		for(int i=0; i<2; i++)
		{
			arLcomStrb[i].RemoveAll();
			arLcomStss[i].RemoveAll();
		}
	}
	LCOM_STRB_STSS()	{}
	LCOM_STRB_STSS(const LCOM_STRB_STSS& rData) {*this=rData;}
	LCOM_STRB_STSS& operator = (const LCOM_STRB_STSS& rData)
	{
		int i;
		int iTotalsize = rData.arLcomStrb[0].GetSize();    
		for(i=0; i<2; i++)
		{      
			arLcomStrb[i].SetSize(iTotalsize); // STRB
			arLcomStss[i].SetSize(iTotalsize); // STSS

			arLcomStrb[i].Copy(rData.arLcomStrb[i]);
			arLcomStss[i].Copy(rData.arLcomStss[i]);
		}
		return *this;
	}
};

// Add by GAY. MNET:XXXX. ('07.08.24). Concurrent Force Struct for Load Combination.
struct LCOM_SBCF
{
	CArray<T_SBCF_D,T_SBCF_D&>  arLcomSbcf[2]; // LcomK 개수 만큼.
	void Initialize()
	{
		for(int i=0; i<2; i++)
		{
			arLcomSbcf[i].RemoveAll();
		}
	}
	LCOM_SBCF()	{}
	LCOM_SBCF(const LCOM_SBCF& rData) {*this=rData;}
	LCOM_SBCF& operator = (const LCOM_SBCF& rData)
	{
		int i;
		int iTotalsize = rData.arLcomSbcf[0].GetSize();    
		for(i=0; i<2; i++)
		{      
			arLcomSbcf[i].SetSize(iTotalsize); // BTSS
			arLcomSbcf[i].Copy(rData.arLcomSbcf[i]);
		}
		return *this;
	}
};
////////////////////////////////////////
// Bridge Load Rating Design. struct
////////////////////////////////////////

struct FORCE_BLRD_RCASE
{
	_DGN_FORC_CRC_EX ForceDCBe[2]; // Before
	_DGN_FORC_CRC_EX ForceDC[2];    // 0=Max,1=Min  structural components and attachment
	_DGN_FORC_CRC_EX ForceDW[2];    // 0=Max,1=Min  wearing surface and utilites
	_DGN_FORC_CRC_EX ForceTemp[2];  // 0=Max,1=Min  Temperature
	_DGN_FORC_CRC_EX ForceTempG[2]; // 0=Max,1=Min  Temperature gradient. 추가
	_DGN_FORC_CRC_EX ForceSec;      // Secondary
	_DGN_FORC_CRC_EX ForcePerm;     // Permanent loads other than dead loads
	_DGN_FORC_CRC_EX ForceUser;     // User Defined

	_DGN_FORC_CRC_EX ForcePrVehicle[cFORC_CON]; // Primary Vehicle
	_DGN_FORC_CRC_EX ForceAdVehicle[cFORC_CON]; // Adjacent Vehicle

	void Initialize()
	{
		for(int i=0; i<2; i++)
		{
			ForceDCBe[i].Initialize();
			ForceDC[i].Initialize();
			ForceDW[i].Initialize();
			ForceTemp[i].Initialize();
			ForceTempG[i].Initialize();
		}
		ForceSec.Initialize();
		ForcePerm.Initialize();
		ForceUser.Initialize();
		for(int i=0; i<12; i++)
		{
			ForcePrVehicle[i].Initialize(); 
			ForceAdVehicle[i].Initialize(); 
		}
	}
	FORCE_BLRD_RCASE() { Initialize(); }
	FORCE_BLRD_RCASE(const FORCE_BLRD_RCASE& rData) {*this=rData;}
	FORCE_BLRD_RCASE& operator = (const FORCE_BLRD_RCASE& rData)
	{
		for(int i=0; i<2; i++)
		{
			ForceDCBe[i]  = rData.ForceDCBe[i];
			ForceDC[i]    = rData.ForceDC[i];
			ForceDW[i]    = rData.ForceDW[i];
			ForceTemp[i]  = rData.ForceTemp[i];
			ForceTempG[i] = rData.ForceTempG[i];
		}
		ForceSec  = rData.ForceSec ;
		ForcePerm = rData.ForcePerm;
		ForceUser = rData.ForceUser;    
		for(int i=0; i<cFORC_CON; i++)
		{
			ForcePrVehicle[i] = rData.ForcePrVehicle[i]; 
			ForceAdVehicle[i] = rData.ForceAdVehicle[i]; 
		}

		return *this;
	}
	
	FORCE_BLRD_RCASE& operator += (const FORCE_BLRD_RCASE& rData)
	{
		for(int i=0; i<2; i++)
		{
			ForceDCBe[i]  += rData.ForceDCBe[i];
			ForceDC[i]    += rData.ForceDC[i];
			ForceDW[i]    += rData.ForceDW[i];
			ForceTemp[i]  += rData.ForceTemp[i];
			ForceTempG[i] += rData.ForceTempG[i];
		}
		ForceSec  += rData.ForceSec ;
		ForcePerm += rData.ForcePerm;
		ForceUser += rData.ForceUser;    
		for(int i=0; i<cFORC_CON; i++)
		{
			ForcePrVehicle[i] += rData.ForcePrVehicle[i]; 
			ForceAdVehicle[i] += rData.ForceAdVehicle[i]; 
		}
		return *this;
	}

	FORCE_BLRD_RCASE& GetData(UINT nIJ)
	{
		for(int i=0; i<2; i++)
		{
			ForceDCBe[i]  = ForceDCBe[i].GetData(nIJ);
			ForceDC[i]    = ForceDC[i].GetData(nIJ);
			ForceDW[i]    = ForceDW[i].GetData(nIJ);
			ForceTemp[i]  = ForceTemp[i].GetData(nIJ);
			ForceTempG[i] = ForceTempG[i].GetData(nIJ);
		}
		ForceSec  = ForceSec.GetData(nIJ) ;
		ForcePerm = ForcePerm.GetData(nIJ);
		ForceUser = ForceUser.GetData(nIJ);    
		for(int i=0; i<cFORC_CON; i++)
		{
			ForcePrVehicle[i] = ForcePrVehicle[i].GetData(nIJ); 
			ForceAdVehicle[i] = ForceAdVehicle[i].GetData(nIJ); 
		}

		return *this;
	}

	FORCE_BLRD_RCASE& operator / (const double dFactor)
	{
		for(int i=0; i<2; i++)
		{
			ForceDCBe[i] / dFactor;
			ForceDC[i] / dFactor;
			ForceDW[i] / dFactor;
			ForceTemp[i] / dFactor;
			ForceTempG[i] / dFactor;
		}
		ForceSec / dFactor;
		ForcePerm / dFactor;
		ForceUser / dFactor;
		for(int i=0; i<cFORC_CON; i++)
		{
			ForcePrVehicle[i] / dFactor;
			ForceAdVehicle[i] / dFactor;
		}
		return *this;
	}  

	void SetData(UINT nFrom, UINT nTo, const FORCE_BLRD_RCASE& rData)
	{
		for(int i=0; i<2; i++)
		{
			ForceDCBe[i].SetData(nFrom, nTo, rData.ForceDCBe[i]);
			ForceDC[i].SetData(nFrom, nTo, rData.ForceDC[i]);
			ForceDW[i].SetData(nFrom, nTo, rData.ForceDW[i]);
			ForceTemp[i].SetData(nFrom, nTo, rData.ForceTemp[i]);
			ForceTempG[i].SetData(nFrom, nTo, rData.ForceTempG[i]);
		}
		ForceSec.SetData(nFrom, nTo, rData.ForceSec);
		ForcePerm.SetData(nFrom, nTo, rData.ForcePerm);
		ForceUser.SetData(nFrom, nTo, rData.ForceUser); 
		for(int i=0; i<cFORC_CON; i++)
		{
			ForcePrVehicle[i].SetData(nFrom, nTo, rData.ForcePrVehicle[i]);
			ForceAdVehicle[i].SetData(nFrom, nTo, rData.ForceAdVehicle[i]);
		}    
	}
};

struct STRESS_BLRD_RCASE
{
	_DGN_STRS_CRC_EX StressDC[2];    // 0=Max,1=Min  structural components and attachment
	_DGN_STRS_CRC_EX StressDW[2];    // 0=Max,1=Min  wearing surface and utilites
	_DGN_STRS_CRC_EX StressTemp[2];  // 0=Max,1=Min  Temperature
	_DGN_STRS_CRC_EX StressTempG[2]; // 0=Max,1=Min  Temperature gradient. 추가
	_DGN_STRS_CRC_EX StressSec;      // Secondary
	_DGN_STRS_CRC_EX StressPerm;     // Permanent loads other than dead loads
	_DGN_STRS_CRC_EX StressUser;     // User Defined

	_DGN_STRS_CRC_EX StressPrVehicle[cFORC_CON]; // Primary Vehicle
	_DGN_STRS_CRC_EX StressAdVehicle[cFORC_CON]; // Adjacent Vehicle

	void Initialize()
	{
		for(int i=0; i<2; i++)
		{
			StressDC[i].Initialize();
			StressDW[i].Initialize();
			StressTemp[i].Initialize();
			StressTempG[i].Initialize();
		}
		StressSec.Initialize();
		StressPerm.Initialize();
		StressUser.Initialize();
		for(int i=0; i<cFORC_CON; i++)
		{
			StressPrVehicle[i].Initialize(); 
			StressAdVehicle[i].Initialize(); 
		}
	}
	STRESS_BLRD_RCASE() { Initialize(); }
	STRESS_BLRD_RCASE(const STRESS_BLRD_RCASE& rData) {*this=rData;}
	STRESS_BLRD_RCASE& operator = (const STRESS_BLRD_RCASE& rData)
	{
		for(int i=0; i<2; i++)
		{
			StressDC[i]    = rData.StressDC[i];
			StressDW[i]    = rData.StressDW[i];
			StressTemp[i]  = rData.StressTemp[i];
			StressTempG[i] = rData.StressTempG[i];
		}
		StressSec  = rData.StressSec ;
		StressPerm = rData.StressPerm;
		StressUser = rData.StressUser;    
		for(int i=0; i<cFORC_CON; i++)
		{
			StressPrVehicle[i] = rData.StressPrVehicle[i]; 
			StressAdVehicle[i] = rData.StressAdVehicle[i]; 
		}
	
		return *this;
	}
};

struct FORCE_BLRD_ELEM
{
	CArray<FORCE_BLRD_RCASE,FORCE_BLRD_RCASE> arForceRCase;
	void Initialize()
	{
		arForceRCase.RemoveAll();
	}
	FORCE_BLRD_ELEM() {Initialize();}
	FORCE_BLRD_ELEM(const FORCE_BLRD_ELEM& rData) {*this=rData;}
	FORCE_BLRD_ELEM& operator = (const FORCE_BLRD_ELEM& rData)
	{
		arForceRCase.Copy(rData.arForceRCase);
		return *this;
	}
};

struct STRESS_BLRD_ELEM
{
	CArray<STRESS_BLRD_RCASE,STRESS_BLRD_RCASE> arStressRCase;
	void Initialize()
	{
		arStressRCase.RemoveAll();
	}
	STRESS_BLRD_ELEM() {}
	STRESS_BLRD_ELEM(const STRESS_BLRD_ELEM& rData) {*this=rData;}
	STRESS_BLRD_ELEM& operator = (const STRESS_BLRD_ELEM& rData)
	{
		arStressRCase.Copy(rData.arStressRCase);
		return *this;
	}
};

struct PRESTRESS_STLD_KEY
{
	CMap<T_STLD_K,T_STLD_K,T_STLD_K,T_STLD_K> mapStldK;
	void Initialize()
	{
		mapStldK.RemoveAll();
	}
	PRESTRESS_STLD_KEY() {}
	PRESTRESS_STLD_KEY(const PRESTRESS_STLD_KEY& rData) {*this=rData;}
	PRESTRESS_STLD_KEY& operator = (const PRESTRESS_STLD_KEY& rData)
	{
		POSITION Pos = rData.mapStldK.GetStartPosition();
		T_STLD_K TempStldK, StldK;
		while(Pos)
		{			
			rData.mapStldK.GetNextAssoc(Pos,TempStldK,StldK);
			mapStldK.SetAt(TempStldK,StldK);
		}
		return *this;
	}
};

// For Rating, KSCE-USD05
struct FORCE_RCASE_KR
{
	_DGN_FORC_CRC ForceDL;
	_DGN_FORC_CRC ForceST;
	_DGN_FORC_CRC ForceLL;
	_DGN_FORC_CRC ForceG;
	void Initialize()
	{
		ForceDL.Initialize();
		ForceST.Initialize();
		ForceLL.Initialize();
		ForceG.Initialize();
	}

	FORCE_RCASE_KR() {Initialize();}
	FORCE_RCASE_KR(const FORCE_RCASE_KR& rData) {*this=rData;}
	FORCE_RCASE_KR& operator = (const FORCE_RCASE_KR& rData)
	{
		ForceDL = rData.ForceDL;
		ForceST = rData.ForceST;
		ForceLL = rData.ForceLL;
		ForceG  = rData.ForceG;
		return *this;
	}
};

struct STRESS_RCASE_KR
{
	_DGN_STRS_CRC StressDL;
	_DGN_STRS_CRC StressLL;
	_DGN_STRS_CRC StressG;
	void Initialize()
	{
		StressDL.Initialize();
		StressLL.Initialize();
		StressG.Initialize();
	}

	STRESS_RCASE_KR() {Initialize();}
	STRESS_RCASE_KR(const STRESS_RCASE_KR& rData) {*this=rData;}
	STRESS_RCASE_KR& operator = (const STRESS_RCASE_KR& rData)
	{
		StressDL = rData.StressDL;
		StressLL = rData.StressLL;
		StressG  = rData.StressG;
		return *this;
	}
};

struct FORCE_RCASE_PRECOMP
{
	_DGN_FORC_CRC Force;
	_DGN_STRS_CRC Stress;

	void Initialize()
	{
		Force.Initialize();
		Stress.Initialize();
	}
	FORCE_RCASE_PRECOMP() {Initialize();}
	FORCE_RCASE_PRECOMP(const FORCE_RCASE_PRECOMP& rData) {*this=rData;}
	FORCE_RCASE_PRECOMP& operator = (const FORCE_RCASE_PRECOMP& rData)
	{
		Force  = rData.Force;
		Stress = rData.Stress;
		return *this;
	}  

};

struct FORCE_LRKR_ELEM // LRKR = Load Rating KR
{
	CArray<FORCE_RCASE_KR,FORCE_RCASE_KR> arForceRCase;
	void Initialize()
	{
		arForceRCase.RemoveAll();
	}
	FORCE_LRKR_ELEM() {Initialize();}
	FORCE_LRKR_ELEM(const FORCE_LRKR_ELEM& rData) {*this=rData;}
	FORCE_LRKR_ELEM& operator = (const FORCE_LRKR_ELEM& rData)
	{
		arForceRCase.Copy(rData.arForceRCase);
		return *this;
	}
};

struct STRESS_LRKR_ELEM
{
	CArray<STRESS_RCASE_KR,STRESS_RCASE_KR> arStressRCase;
	void Initialize()
	{
		arStressRCase.RemoveAll();
	}
	STRESS_LRKR_ELEM() {Initialize();}
	STRESS_LRKR_ELEM(const STRESS_LRKR_ELEM& rData) {*this=rData;}
	STRESS_LRKR_ELEM& operator = (const STRESS_LRKR_ELEM& rData)
	{
		arStressRCase.Copy(rData.arStressRCase);
		return *this;
	}
};

struct FORCE_RCASE_PRECOMP_ELEM
{
	CArray<FORCE_RCASE_PRECOMP,FORCE_RCASE_PRECOMP> aForceRCase;
	void Initialize()
	{
		aForceRCase.RemoveAll();
	}
	FORCE_RCASE_PRECOMP_ELEM() {Initialize();}
	FORCE_RCASE_PRECOMP_ELEM(const FORCE_RCASE_PRECOMP_ELEM& rData) {*this=rData;}
	FORCE_RCASE_PRECOMP_ELEM& operator = (const FORCE_RCASE_PRECOMP_ELEM& rData)
	{
		aForceRCase.Copy(rData.aForceRCase);
		return *this;
	}
};

struct FORCE_RCASE_COMPO
{
	_DGN_FORC_CRC ForceG;
	_DGN_FORC_CRC ForceDL;
	_DGN_FORC_CRC ForceLL;
	void Initialize()
	{
		ForceG.Initialize();
		ForceDL.Initialize();
		ForceLL.Initialize();
	}
	
	FORCE_RCASE_COMPO() {Initialize();}
	FORCE_RCASE_COMPO(const FORCE_RCASE_COMPO& rData) {*this=rData;}
	FORCE_RCASE_COMPO& operator = (const FORCE_RCASE_COMPO& rData)
	{
		ForceG  = rData.ForceG;
		ForceDL = rData.ForceDL;
		ForceLL = rData.ForceLL;
		return *this;
	}
};

struct STRESS_RCASE_COMPO
{
	_DGN_STRS_CRC StressG;
	_DGN_STRS_CRC StressDL;
	_DGN_STRS_CRC StressLL;
	void Initialize()
	{
		StressG.Initialize();
		StressDL.Initialize();
		StressLL.Initialize();
	}
	
	STRESS_RCASE_COMPO() {Initialize();}
	STRESS_RCASE_COMPO(const STRESS_RCASE_COMPO& rData) {*this=rData;}
	STRESS_RCASE_COMPO& operator = (const STRESS_RCASE_COMPO& rData)
	{
		StressG  = rData.StressG;
		StressDL = rData.StressDL;
		StressLL = rData.StressLL;
		return *this;
	}
};

struct FORCE_LRKR_COMPO_ELEM 
{
	CArray<FORCE_RCASE_COMPO,FORCE_RCASE_COMPO&> aForceRCase;
	void Initialize()
	{
		aForceRCase.RemoveAll();
	}
	FORCE_LRKR_COMPO_ELEM() {Initialize();}
	FORCE_LRKR_COMPO_ELEM(const FORCE_LRKR_COMPO_ELEM& rData) {*this=rData;}
	FORCE_LRKR_COMPO_ELEM& operator = (const FORCE_LRKR_COMPO_ELEM& rData)
	{
		aForceRCase.Copy(rData.aForceRCase);
		return *this;
	}
};

struct STRESS_LRKR_COMPO_ELEM
{
	CArray<STRESS_RCASE_COMPO,STRESS_RCASE_COMPO&> arStressRCase;
	void Initialize()
	{
		arStressRCase.RemoveAll();
	}
	STRESS_LRKR_COMPO_ELEM() {Initialize();}
	STRESS_LRKR_COMPO_ELEM(const STRESS_LRKR_COMPO_ELEM& rData) {*this=rData;}
	STRESS_LRKR_COMPO_ELEM& operator = (const STRESS_LRKR_COMPO_ELEM& rData)
	{
		arStressRCase.Copy(rData.arStressRCase);
		return *this;
	}
};

//++++++++++++++++++++
struct T_SQLC_D
{
	UINT nElemNum; 
	UINT nLCaseNum[14]; // 0=STLD, 1=SPLC, 2=ESPC, 3=THIS, 4=SGLD, 5=STEP, 6=MVLD, 7=SMLC, 8=GILC_STRB, 9=GILC_SBCF, 10=LCOM_STRB, 11=LCOM_SBCF, 12=STLD-SCFP, 13=SGLD-SCFP
	void Initialize()
	{
		nElemNum=0;
		memset(nLCaseNum, 0, sizeof(nLCaseNum));
	}
};

struct T_SQLC_841_D
{
	UINT nElemNum; 
	UINT nLCaseNum[12]; // 0=STLD, 1=SPLC, 2=ESPC, 3=THIS, 4=SGLD, 5=STEP, 6=MVLD, 7=SMLC, 8=GILC_STRB, 9=GILC_SBCF, 10=LCOM_STRB, 11=LCOM_SBCF
	void Initialize()
	{
		nElemNum=0;
		memset(nLCaseNum, 0, sizeof(nLCaseNum));      
	}
};

 // STLD
struct T_STLD_LCASE
{  
	T_STRB_D	StldStrb[6]; // 0,1 : 전체  || 2,3 거더 Max/Min || 4,5 : 슬래브 Max/Min 
	T_STSS_D  StldStss[6]; 
	T_BTSS_D  StldBtss[6]; 
	void Initialize()
	{
		for(int i=0; i<6; i++)
		{
			StldStrb[i].Initialize();
			StldStss[i].Initialize();
			StldBtss[i].Initialize();
		}
	}
	T_STLD_LCASE()	{Initialize();}
	T_STLD_LCASE(const T_STLD_LCASE& rData) {*this=rData;}
	T_STLD_LCASE& operator = (const T_STLD_LCASE& rData)
	{
		for(int i=0; i<6; i++)
		{      
			StldStrb[i] = rData.StldStrb[i]; // STRB
			StldStss[i] = rData.StldStss[i]; // STSS
			StldBtss[i] = rData.StldBtss[i];
		}
		return *this;
	}
};

 // SPLC, ESPC, THIS, SGLD, STEP, GILC-STRB
struct T_STRB_LCASE
{  
	T_STRB_D	StrbLcase[6];  // 0,1 : 전체  || 2,3 거더 Max/Min || 4,5 : 슬래브 Max/Min 
	T_STSS_D  StssLcase[6]; 
	void Initialize()
	{
		for(int i=0; i<6; i++)
		{
			StrbLcase[i].Initialize();
			StssLcase[i].Initialize();
		}
	}
	T_STRB_LCASE()	{Initialize();}
	T_STRB_LCASE(const T_STRB_LCASE& rData) {*this=rData;}
	T_STRB_LCASE& operator = (const T_STRB_LCASE& rData)
	{
		for(int i=0; i<6; i++)
		{      
			StrbLcase[i] = rData.StrbLcase[i]; // STRB
			StssLcase[i] = rData.StssLcase[i]; // STSS
		}
		return *this;
	}
};

 // MVLD, SMLC, GILC-SBCF
struct T_SBCF_LCASE
{
	T_SBCF_D SbcfLcase[6]; // MvldK 개수 만큼.  // 0,1 : 전체  || 2,3 거더 Max/Min || 4,5 : 슬래브 Max/Min 
	void Initialize()
	{
		for(int i=0; i<6; i++)
		{
			SbcfLcase[i].Initialize();
		}
	}
	T_SBCF_LCASE()	{Initialize();}
	T_SBCF_LCASE(const T_SBCF_LCASE& rData) {*this=rData;}
	T_SBCF_LCASE& operator = (const T_SBCF_LCASE& rData)
	{
		for(int i=0; i<6; i++)
		{      
			SbcfLcase[i] = rData.SbcfLcase[i];
		}
		return *this;
	}
};

struct T_DGN_LCOM_ID
{
	CArray<UINT,UINT&> aDgnLcomId;

	void Initialize()
	{
		aDgnLcomId.RemoveAll();
	}
	T_DGN_LCOM_ID()	{Initialize();}
	T_DGN_LCOM_ID(const T_DGN_LCOM_ID& rData) {*this=rData;}
	T_DGN_LCOM_ID& operator = (const T_DGN_LCOM_ID& rData)
	{
		aDgnLcomId.Copy(rData.aDgnLcomId);
		return *this;
	}
};

struct T_STRS_CPT
{
	_DGN_STRS_CRC MaxGr; // 거더
	_DGN_STRS_CRC MinGr;
	_DGN_STRS_CRC MaxSl; // 슬래브
	_DGN_STRS_CRC MinSl;

	void Initialize()
	{
		MaxGr.Initialize();
		MinGr.Initialize();
		MaxSl.Initialize();
		MinSl.Initialize();
	}

	T_STRS_CPT()	{Initialize();}
	T_STRS_CPT(const T_STRS_CPT& rData)	{*this=rData;}
	T_STRS_CPT& operator = (const T_STRS_CPT& rData)
	{
		MaxGr = rData.MaxGr;
		MinGr = rData.MinGr;
		MaxSl = rData.MaxSl;
		MinSl = rData.MinSl;
		return *this;
	}
};

struct T_SHRS_CPT
{
	_DGN_SHRS_CRC MaxGr; // 거더
	_DGN_SHRS_CRC MinGr;
	_DGN_SHRS_CRC MaxSl; // 슬래브
	_DGN_SHRS_CRC MinSl;

	void Initialize()
	{
		MaxGr.Initialize();
		MinGr.Initialize();
		MaxSl.Initialize();
		MinSl.Initialize();
	}

	T_SHRS_CPT()	{Initialize();}
	T_SHRS_CPT(const T_SHRS_CPT& rData)	{*this=rData;}
	T_SHRS_CPT& operator = (const T_SHRS_CPT& rData)
	{
		MaxGr = rData.MaxGr;
		MinGr = rData.MinGr;
		MaxSl = rData.MaxSl;
		MinSl = rData.MinSl;
		return *this;
	}
};

struct T_CSLF_BASE
{
    int    nErIdx;
    double dFactor;
    
    void Initialize()
    {
        nErIdx  = 0;
        dFactor = 1.0;
    }
    T_CSLF_BASE() {Initialize();}
};
struct T_CSLF_CPT
{
    double dFacDL;
    CArray<T_CSLF_BASE,T_CSLF_BASE&> aFacEr;

    void Initialize()
    {
        dFacDL  = 1.0;
        aFacEr.RemoveAll();
    }

    T_CSLF_CPT() {Initialize();}
};

// PMS.5862 BS BD21 Assessment
struct FORCE_RCASE_BD
{
    _DGN_FORC_CRC_EX ForceSd; // Dead and superimposed dead loads
    _DGN_FORC_CRC_EX ForceSst;// Standard Vehicle
    _DGN_FORC_CRC_EX ForceS;  // Special Vehicle
    _DGN_FORC_CRC_EX ForceDLbe; // Dead Load before
    _DGN_FORC_CRC_EX ForceLL;   // Live Load - ForceSd 중 LL로 판단된 것

    void Initialize()
    {
        ForceSd.Initialize();
        ForceSst.Initialize();
        ForceS.Initialize();
        ForceDLbe.Initialize();
    }

    FORCE_RCASE_BD() {Initialize();}
    FORCE_RCASE_BD(const FORCE_RCASE_BD& rData) {*this=rData;}
    FORCE_RCASE_BD& operator = (const FORCE_RCASE_BD& rData)
    {
        ForceSd  = rData.ForceSd ;
        ForceSst = rData.ForceSst;
        ForceS   = rData.ForceS  ;
        ForceDLbe= rData.ForceDLbe;
        ForceLL  = rData.ForceLL;
        return *this;
    }
    void ConvertBlrd(FORCE_BLRD_RCASE& rData)
    {
        for ( int i=0; i<2; ++i )
        {
            rData.ForceDCBe[i] = ForceDLbe;
            rData.ForceDC[i] = ForceSd;
        }
        for ( int i=0; i<cFORC_CON; ++i )
        {
            rData.ForcePrVehicle[i] = ForceS;
            rData.ForceAdVehicle[i] = ForceSst;
        }
    }

    double Get_Fxx(const int& iPos)
    {
        if(iPos >= cPOSI_CRC) { ASSERT(0); return 0; }
        int nPosEx = GetPosEx(iPos);
        return (ForceSd.dFxx[nPosEx] + ForceSst.dFxx[nPosEx] + ForceS.dFxx[nPosEx]);
    }
    double Get_Fyy(const int& iPos)
    {
        if(iPos >= cPOSI_CRC) { ASSERT(0); return 0; }
        int nPosEx = GetPosEx(iPos);
        return (ForceSd.dFyy[nPosEx] + ForceSst.dFyy[nPosEx] + ForceS.dFyy[nPosEx]);
    }
    double Get_Fzz(const int& iPos)
    {
        if(iPos >= cPOSI_CRC) { ASSERT(0); return 0; }
        int nPosEx = GetPosEx(iPos);
        return (ForceSd.dFzz[nPosEx] + ForceSst.dFzz[nPosEx] + ForceS.dFzz[nPosEx]);
    }
    double Get_Mux(const int& iPos)
    {
        if(iPos >= cPOSI_CRC) { ASSERT(0); return 0; }
        int nPosEx = GetPosEx(iPos);
        return (ForceSd.dMux[nPosEx] + ForceSst.dMux[nPosEx] + ForceS.dMux[nPosEx]);
    }
    double Get_Muy(const int& iPos)
    {
        if(iPos >= cPOSI_CRC) { ASSERT(0); return 0; }
        int nPosEx = GetPosEx(iPos);
        return (ForceSd.dMuy[nPosEx] + ForceSst.dMuy[nPosEx] + ForceS.dMuy[nPosEx]);
    }
    double Get_Muz(const int& iPos)
    {
        if(iPos >= cPOSI_CRC) { ASSERT(0); return 0; }
        int nPosEx = GetPosEx(iPos);
        return (ForceSd.dMuz[nPosEx] + ForceSst.dMuz[nPosEx] + ForceS.dMuz[nPosEx]);
    }
    int GetPosEx(const int& nPos)
    {
        return nPos==1 ? cPOSI_CRC_EX-1 : 0;
    }
};

struct STRESS_RCASE_BD
{
    _DGN_STRS_CRC_EX StressSd; // Dead and superimposed dead loads
    _DGN_STRS_CRC_EX StressSst;// Standard Vehicle
    _DGN_STRS_CRC_EX StressS;  // Special Vehicle
    void Initialize()
    {
        StressSd.Initialize();
        StressSst.Initialize();
        StressS.Initialize();
    }

    STRESS_RCASE_BD() {Initialize();}
    STRESS_RCASE_BD(const STRESS_RCASE_BD& rData) {*this=rData;}
    STRESS_RCASE_BD& operator = (const STRESS_RCASE_BD& rData)
    {
        StressSd  = rData.StressSd ;
        StressSst = rData.StressSst;
        StressS   = rData.StressS  ;
        return *this;
    }
    _DGN_STRS_CRC_EX Get_SumEx()
    {
        _DGN_STRS_CRC_EX StressSum;

        StressSum += StressSd;
        StressSum += StressSst;
        StressSum += StressS;

        return StressSum;
    }
    _DGN_STRS_CRC Get_Sum()
    {
        _DGN_STRS_CRC_EX Strs = Get_SumEx();
        _DGN_STRS_CRC StressSum;
        Strs.GetStress(StressSum);

        return StressSum;
    }
 };

struct FORCE_ASBD_ELEM // ASBD = ASSESSMENT BD
{
    CArray<FORCE_RCASE_BD,FORCE_RCASE_BD> arForceRCase;
    void Initialize()
    {
        arForceRCase.RemoveAll();
    }
    FORCE_ASBD_ELEM() {Initialize();}
    FORCE_ASBD_ELEM(const FORCE_ASBD_ELEM& rData) {*this=rData;}
    FORCE_ASBD_ELEM& operator = (const FORCE_ASBD_ELEM& rData)
    {
        arForceRCase.Copy(rData.arForceRCase);
        return *this;
    }
};

struct STRESS_ASBD_ELEM // ASBD = ASSESSMENT BD
{
    CArray<STRESS_RCASE_BD,STRESS_RCASE_BD> arStressRCase;
    void Initialize()
    {
        arStressRCase.RemoveAll();
    }
    STRESS_ASBD_ELEM() {Initialize();}
    STRESS_ASBD_ELEM(const STRESS_ASBD_ELEM& rData) {*this=rData;}
    STRESS_ASBD_ELEM& operator = (const STRESS_ASBD_ELEM& rData)
    {
        arStressRCase.Copy(rData.arStressRCase);
        return *this;
    }
};

#endif
