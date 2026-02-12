#pragma once
#include <vector>
#include <map>
#include <set>

#include "../MIT_lib/mitc/xml/ftmXmlFile.h"
#include "../MIT_lib/mitc/xml/ftmXmlTool.h"

class CUnitCtrl;

#pragma pack(push, 1)
#include "HeaderPre.h"
struct __MY_EXT_CLASS__ T_PF_BASE_D
{
public:
	T_PF_BASE_D(void);
	virtual ~T_PF_BASE_D(void);

public:
	virtual void Init(void) = 0;
	virtual void S2D(const ftmXmlNode* pXmlNode) = 0;
	virtual void D2S(ftmXmlNode* pXmlNode) const = 0;
	virtual void ConvertUintIn(CUnitCtrl* pUnitCtrl)=0;
	virtual void ConvertUintOut(CUnitCtrl* pUnitCtrl)=0;
	virtual void ConvertUnitPrev(CUnitCtrl* pUnitCtrl)=0;
};
#include "HeaderPost.h"

#pragma pack(pop)

