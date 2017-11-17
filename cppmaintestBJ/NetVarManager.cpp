#include "NetVarManager.h"
#include "IBaseClientDLL.h"
#include <cstdio>
#include <vector>

void INetVarManager::Init()
{
	m_pHead = g_pClient->GetAllClasses();
}

DWORD INetVarManager::FindOffset(const char* tablename, const char* var)
{
	if (!m_pHead)
		return 0;

	ClientClass* Class = m_pHead;
	RecvTable* Table = 0;

	while (Class)
	{
		RecvTable* table = Class->m_pRecvTable;
		if (table)
		{
			if (strcmp(tablename, table->m_pNetTableName) == 0)
			{
				Table = table;
				break;
			}
		}
		Class = Class->m_pNext;
	}

	if (!Table)
		return 0;

	for (int i = 0; i < Table->m_nProps; i++)
	{
		RecvProp* prop = &Table->m_pProps[i];
		if (!prop)
			continue;

		if (strcmp(var, prop->m_pVarName) == 0)
			return prop->m_Offset;
	}

	return 0;
}

void INetVarManager::DumpTables()
{
	if (!m_pHead)
		return;

	ClientClass* Class = m_pHead;

	while (Class)
	{
		RecvTable* table = Class->m_pRecvTable;
		if (table)
		{
			printf("%s\n", table->m_pNetTableName);
			for (int i = 0; i < table->m_nProps; i++)
			{
				RecvProp* prop = &table->m_pProps[i];
				if (!prop)
					continue;

				printf("\t%s\t->\t0x%x\n", prop->m_pVarName, prop->m_Offset);
			}
		}

		Class = Class->m_pNext;
	}
}

INetVarManager netvars;