#pragma once

#include "VTable.h"
#include "IBaseClientDLL.h"
#include "IClientEntity.h"
#include "IInputSystem.h"
#include "IVEngineClient.h"
#include "CGlobalVarsBase.h"
#include "ISurface.h"
#include "IPanel.h"

class Interface
{
public:
	void SetupInterfaces();
};

extern Interface sInterface;
