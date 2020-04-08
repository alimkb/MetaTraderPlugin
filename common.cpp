//+------------------------------------------------------------------+
//|                                       MetaTrader WebRegistration |
//|                 Copyright © 2001-2008, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#include "stdafx.h"
//+------------------------------------------------------------------+
//| Reading number parameter                                         |
//+------------------------------------------------------------------+
int GetIntParam(LPCSTR string,LPCSTR param,int *data)
  {
//---- checks
   if(string==NULL || param==NULL || data==NULL) return(FALSE);
//---- find
   if((string=strstr(string,param))==NULL)       return(FALSE);
//---- all right
   *data=atoi(&string[strlen(param)]);
   return(TRUE);
  }
//+------------------------------------------------------------------+
//| Reading floating parameter                                       |
//+------------------------------------------------------------------+
int GetFltParam(LPCSTR string,LPCSTR param,double *data)
  {
//---- checks
   if(string==NULL || param==NULL || data==NULL) return(FALSE);
//---- find
   if((string=strstr(string,param))==NULL)       return(FALSE);
//---- all right
   *data=atof(&string[strlen(param)]);
   return(TRUE);
  }
//+------------------------------------------------------------------+
//| Reading string parameter                                         |
//+------------------------------------------------------------------+
int GetStrParam(LPCSTR string,LPCSTR param,char *buf,const int maxlen)
  {
   int i=0;
//---- checks
   if(string==NULL || param==NULL || buf==NULL)  return(FALSE);
//---- find
   if((string=strstr(string,param))==NULL)       return(FALSE);
//---- receive result
   string+=strlen(param);
   while(*string!=0 && *string!='|' && i<maxlen) { *buf++=*string++; i++; }
   *buf=0;
//----
   return(TRUE);
  }
//+------------------------------------------------------------------+
//| Check complexity of password                                     |
//+------------------------------------------------------------------+
int CheckPassword(LPCSTR password)
  {
   char   tmp[256];
   int    len,num=0,upper=0,lower=0;
   USHORT type[256];
//----
   if(password==NULL) return(FALSE);
//---- check len
   if((len=strlen(password))<5) return(FALSE);
//---- must Upper case,lower case and digits
   strcpy(tmp,password);
   if(GetStringTypeA(LOCALE_SYSTEM_DEFAULT,CT_CTYPE1,tmp,len,(USHORT*)type))
     {
      for(int i=0;i<len;i++)
        {
         if(type[i]&C1_DIGIT)  { num=1;   continue; }
         if(type[i]&C1_UPPER)  { upper=1; continue; }
         if(type[i]&C1_LOWER)  { lower=1; continue; }
         if(!(type[i] & (C1_ALPHA | C1_DIGIT) )) { num=2; break; }
        }
     }
//---- compute complexity
   return((num+upper+lower)>=2);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
char* insert(void *base,const void *elem,size_t num,const size_t width,int(__cdecl *compare)( const void *elem1,const void *elem2 ))
  {
//---- проверки
   if(base==NULL || elem==NULL || compare==NULL) return(NULL);
//---- первый элемент?
   if(num<1) { memcpy(base,elem,width); return(char*)(base); }
//---- 
   register char *lo=(char *)base;
   register char *hi=(char *)base+(num-1) * width, *end=hi;
   register char *mid;
   unsigned int   half;
   int            result;
//----
   while(num>0)
     {
      half=num/2;
      mid=lo+half*width;
      //---- compare
      if((result=compare(elem,mid))>0) // data[mid]<elem
        {
         lo  =mid+width;
         num =num-half-1;
        }
      else if(result<0)                // data[mid]>elem
        {
         num=half;
        }
      else                             // data[mid]==elem
        return(NULL);
     }
//---- вставляем
   memmove(lo+width,lo,end-lo+width);
   memcpy(lo,elem,width);
//----
   return(lo);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
int CheckTemplate(char* expr,char* tok_end,const char* group,char* prev,int* deep)
  {
   char  tmp=0;
   char *lastwc,*prev_tok;
   const char *cp;
//---- проверим глубину рекурсии
   if((*deep)++>=10) return(FALSE);
//---- пропускаем повторы *
   while(*expr=='*' && expr!=tok_end) expr++;
   if(expr==tok_end) return(TRUE);
//---- ищем следующую звездочку или конец
   lastwc=expr;
   while(*lastwc!='*' && *lastwc!=0) lastwc++;
//---- временно ограничиваем строку
   if((tmp=*(lastwc))!=0) // токен не последний в строке?
     {
      tmp=*(lastwc);*(lastwc)=0;
      if((prev_tok=const_cast<char *>(strstr(group,expr)))==NULL) { if(tmp!=0) *(lastwc)=tmp; return(FALSE); }
	  //if((prev_tok=strstr(group,expr))==NULL) { if(tmp!=0) *(lastwc)=tmp; return(FALSE); }
      *(lastwc)=tmp;
     }
   else // токен последний...
     {
      //---- проверяем
      cp=group+strlen(group);
      for(;cp>=group;cp--)
        if(*cp==expr[0] && strcmp(cp,expr)==0)
          return(TRUE);
      return(FALSE);
     }
//---- нарушен порядок?
   if(prev!=NULL &&  prev_tok<=prev) return(FALSE);
   prev=prev_tok;
//----
   group=prev_tok+(lastwc-expr-1);
//---- дошли до конца?
   if(lastwc!=tok_end) return CheckTemplate(lastwc,tok_end,group,prev,deep);
//----
   return(TRUE);
  }
//+------------------------------------------------------------------+
//|  Удовлетворяет ли группа одному из шаблонов?                     |
//+------------------------------------------------------------------+
int CheckGroup(char* grouplist,const char *group)
  {
//---- проверки
   if(grouplist==NULL || group==NULL) return(FALSE);
//---- проходимся по всем группам
   char *tok_start=grouplist,end;
   int  res=TRUE,deep=0,normal_mode;
   while(*tok_start!=0)
     {
      //---- пропустим запятые
      while(*tok_start!=0 && *tok_start==',') tok_start++;
      //----
      if(*tok_start=='!') { tok_start++; normal_mode=FALSE; }
      else                 normal_mode=TRUE;
      //---- найдем границы токена
      char *tok_end=tok_start;
      while(*tok_end!=',' && *tok_end!=0) tok_end++;
      end=*tok_end; *tok_end=NULL;
      //----
      char *tp=tok_start;
      const char *gp=group;
      char *prev=NULL;
      //---- проходим по токену
      res=TRUE;
      while(tp!=tok_end && *gp!=NULL)
        {
         //---- нашли звёздочку? проверяем как регэксп
         if(*tp=='*')
           {
            if((res=CheckTemplate(tp,tok_end,gp,prev,&deep))==TRUE)
              {
               *tok_end=end;
               return(normal_mode);
              }
            break;
           }
         //---- просто проверяем
         if(*tp!=*gp) { *tok_end=end; res=FALSE; break; }
         tp++; gp++;
        }
      //---- восстанавливаем
      *tok_end=end;
      //---- проверяем, мы нашли точную цитату и всё хорошо?
      if(*gp==NULL && (tp==tok_end || *tp=='*') && res==TRUE) return(normal_mode);
      //---- переход к следующему токену
      if(*tok_end==0) break;
      tok_start=tok_end+1;
     }
//----
   return(FALSE);
  }
//+------------------------------------------------------------------+