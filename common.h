//+------------------------------------------------------------------+
//|                                       MetaTrader WebRegistration |
//|                 Copyright © 2001-2008, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
int      GetIntParam(LPCSTR string,LPCSTR param,int *data);
int      GetFltParam(LPCSTR string,LPCSTR param,double *data);
int      GetStrParam(LPCSTR string,LPCSTR param,char *buf,const int maxlen);
int      CheckPassword(LPCSTR password);
int      CheckGroup(char* grouplist,const char *group);
char *   insert(void *base,const void *elem,size_t num,const size_t width,
                  int(__cdecl *compare)( const void *elem1,const void *elem2));
//+------------------------------------------------------------------+
