#pragma once

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgnSymbol final
{
public:
	CDgnSymbol();
	virtual ~CDgnSymbol();

public:
	static CString GetFc(const int nDgnCode, bool bFixWidth = false);
	static CString GetFy(const int nDgnCode, bool bFixWidth = false);
	static CString GetFys(const int nDgnCode, bool bFixWidth = false);

	static CString GetPu(const int nDgnCode, bool bFixWidth = false);
	static CString GetPuy(const int nDgnCode, bool bFixWidth = false);
	static CString GetPuz(const int nDgnCode, bool bFixWidth = false);
	static CString GetMu(const int nDgnCode, bool bFixWidth = false);
	static CString GetMc(const int nDgnCode, bool bFixWidth = false);
	static CString GetMcy(const int nDgnCode, bool bFixWidth = false);
	static CString GetMcz(const int nDgnCode, bool bFixWidth = false);
	static CString GetVu(const int nDgnCode, bool bFixWidth = false);
	static CString GetVuy(const int nDgnCode, bool bFixWidth = false);
	static CString GetVuz(const int nDgnCode, bool bFixWidth = false);
	static CString GetTu(const int nDgnCode, bool bFixWidth = false);
	static CString Getxd(const int nDgnCode, bool bFixWidth = false);

	static CString GetpPnmax(const int nDgnCode, bool bFixWidth = false);
	static CString GetpPn(const int nDgnCode, bool bFixWidth = false);
	static CString GetpMn(const int nDgnCode, bool bFixWidth = false);
	static CString GetpMny(const int nDgnCode, bool bFixWidth = false);
	static CString GetpMnz(const int nDgnCode, bool bFixWidth = false);
	static CString GetpV(const int nDgnCode, bool bFixWidth = false);
	static CString GetpTn(const int nDgnCode, bool bFixWidth = false);

	static CString GetRatP(const int nDgnCode, bool bFixWidth = false);
	static CString GetRatPy(const int nDgnCode, bool bFixWidth = false);
	static CString GetRatPz(const int nDgnCode, bool bFixWidth = false);
	static CString GetRatV(const int nDgnCode, bool bFixWidth = false);
	static CString GetRatT(const int nDgnCode, bool bFixWidth = false);
	static CString GetRatF(const int nDgnCode, bool bFixWidth = false);
	static CString GetRatS(const int nDgnCode, bool bFixWidth = false);

	static CString GetAst(const int nDgnCode, bool bFixWidth = false);
	static CString GetAsV(const int nDgnCode, bool bFixWidth = false);
	static CString GetAsH(const int nDgnCode, bool bFixWidth = false);
	static CString GetAsy(const int nDgnCode, bool bFixWidth = false);
	static CString GetAsz(const int nDgnCode, bool bFixWidth = false);

	static CString GetMFy(const int nDgnCode, bool bFixWidth = false);
	static CString GetMFz(const int nDgnCode, bool bFixWidth = false);

	static CString GetCrack(const int nDgnCode, bool bFixWidth = false);
	static CString GetCracka(const int nDgnCode, bool bFixWidth = false);

	static CString GetStress(const int nDgnCode, bool bFixWidth = false);
	static CString GetStressa(const int nDgnCode, bool bFixWidth = false);

	static CString Getffs(const int nDgnCode, bool bFixWidth = false);
	static CString Getfrs(const int nDgnCode, bool bFixWidth = false);

public: // For Graphic Output.
	static CString GetpPnmax(const int nDgnCode, CString strPi);
	static CString GetpPn(const int nDgnCode, CString strPi);
	static CString GetpPny(const int nDgnCode, CString strPi);
	static CString GetpPnz(const int nDgnCode, CString strPi);
	static CString GetpMn(const int nDgnCode, CString strPi);
	static CString GetpMny(const int nDgnCode, CString strPi);
	static CString GetpMnz(const int nDgnCode, CString strPi);
	static CString GetpVc(const int nDgnCode, CString strPi);
	static CString GetpVs(const int nDgnCode, CString strPi);
	static CString GetpVn(const int nDgnCode, CString strPi);
	static CString GetpVcy(const int nDgnCode, CString strPi);
	static CString GetpVcz(const int nDgnCode, CString strPi);
	static CString GetpVsy(const int nDgnCode, CString strPi);
	static CString GetpVsz(const int nDgnCode, CString strPi);

public:
	static CString JoinString(const std::vector<CString>& vStr);
	static CString GetStar(const long nCount);
};

#include "HeaderPost.h"
