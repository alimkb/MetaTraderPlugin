//+------------------------------------------------------------------+
//|                                                         MtSrvDev |
//+------------------------------------------------------------------+
#include "stdafx.h"
#include "common.h"

PluginInfo        ExtPluginInfo={ "PluginName",100,"Company Name .",{0} };
CServerInterface *ExtServer=NULL;

BOOL APIENTRY DllMain(HANDLE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
{
	switch(ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			break;
	}

	return(TRUE);
}

void APIENTRY MtSrvAbout(PluginInfo *info)
{
	if(info!=NULL) memcpy(info,&ExtPluginInfo,sizeof(PluginInfo));
}

int APIENTRY MtSrvStartup(CServerInterface *server)
{

	if(server==NULL)                        return(FALSE);
	if(server->Version()!=ServerApiVersion) return(FALSE);
	ExtServer=server;
	ExtServer->LogsOut(CmdOK,"PluginName","initialized");

	return(TRUE);
}

//+------------------------------------------------------------------+
//| Incoming requests                                                |
//+------------------------------------------------------------------+
void APIENTRY MtSrvTradeRequestApply(RequestInfo *request,const int isdemo)
  {
	// Your Code
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
