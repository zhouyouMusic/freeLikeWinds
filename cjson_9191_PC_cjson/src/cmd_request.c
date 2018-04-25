#include <stdio.h>
#include <fcntl.h>
#include <setjmp.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <ctype.h>
#include <signal.h>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <alsa/asoundlib.h>
#include <linux/soundcard.h>
#include "mad.h"
#include "semcParse.h"
#include "madAlsaPlay.h"
#include "baseModule.h"
#include "JSON_checker.h"
#include "r16_gpio_control.h" 


/************************************************************************

			"POST /semanticPlatform/platfrom HTTP/1.1\n"
			"Accept: application/json\n"
			"Content-Type: application/json\n"
			"Host: 210.22.153.226:9191\n"
			"Connection: keep-alive\n"
			"Content-Length: 117\r\n\r\n"
			"{\n"
			"\"appKey\":\"6ea7c2040658730201d4d498bc4dbfe7\",\n"
			"\"appSecret\":\"78662c5d8c74dd2c79f63fcf17360a68\",\n"
			"\"content\":\"你好\"\n"
			"}\n"
			;
	
*****************************************************************************/


void getPostBuf(char **buff,SemcParseHandle smcPreHand)
{

	SmdParsingBuffer* smcPreHandle = (SmdParsingBuffer*)smcPreHand;
	char post_host[50] = "";
	sprintf(post_host,"%s%s%s%d%s","HOST: ",smcPreHandle->ipAddr,":",smcPreHandle->port,"\n");
	char post_http[100] = "";
	sprintf(post_http,"%s%s%s","POST ",smcPreHandle->apiPath," HTTP/1.1\n");
	char cont_length[100] = "";
	char cont_data[512] = "";
	sprintf(cont_data,"%s%s%s%s%s%s%s","{\n\"appKey\":\"",HWappKey,"\",\n\"appSecret\":\"",HWappSecret,
							"\",\n\"content\":\"",smcPreHandle->quesCmdBuff,"\"\n}\n");
	int dataLen = strlen(cont_data) + 1;
	sprintf(cont_length,"%s%d%s","Content-Length: ",dataLen,"\r\n\r\n");
	
	if(NULL==*buff)
	{
		perror("postBuf  error its Null\n");
		return;
	}
	sprintf(*buff,"%s%s%s%s%s%s%s",
						post_http,
						"Accept: application/json\n",
						"Content-Type: application/json\n",
						post_host,
						"Connection: keep-alive\n",
						cont_length,
						cont_data
						);
	

}

int MP3BoradCast(char *sAddr,SemcParseHandle smcPreHand)
{
	char buff_GET[200] = "";
	char buff_HOST[50] = "";
	char buff_HTTP[50] = "";
	char buff_MP3Post[512] = "";
	int i = 0;
	sAddr = sAddr + 1;
	*(sAddr + strlen(sAddr)-1) = '\0';
	sprintf(buff_GET,"%s%s","GET ",sAddr);
	BASE_INFO_LOG(InfoLogHandle,"%s%s",sAddr,"\n");
	sAddr = sAddr + strlen("http://");
	while((*sAddr != '\n') && (*sAddr != '\0'))
	{
		if(*sAddr == '/')
			break;
		buff_HTTP[i++] = *sAddr++;
	}
	sprintf(buff_HOST,"%s%s%s","HOST: ",buff_HTTP,"\r\n");
	sprintf(buff_MP3Post,"%s%s%s%s%s%s%s",
			buff_GET,
			"",
	 		" HTTP/1.1\r\n",
	 		buff_HOST,
	 		"Connection: close\r\n",
	 		"Accept: */*\r\n",
	 		"\r\n"
			 );
	SmdParsingBuffer* smcPreHandle = (SmdParsingBuffer*)smcPreHand;
	strcpy(smcPreHandle->buff_HTTP,buff_HTTP);
	strcpy(smcPreHandle->req_Content,buff_MP3Post);
printf("%s %s	\n",smcPreHandle->buff_HTTP,smcPreHandle->req_Content);
	if(strlen(sAddr) > 10)
		return 0;
	return -1;
}



void musicResultPlaying(char * quesBuf,SemcParseHandle smcPreHand)
{
	int i = 0,j = 0;
	char httpBuff[50] = "";
	char  sgUrl[200] = "";
	while(quesBuf[i] != '\0')
	{
		if(quesBuf[i] == '\"')
		{
			memset(httpBuff,0,50);
			strncpy(httpBuff,quesBuf+i,50);
			httpBuff[49] = '\0';
			if(strncmp(httpBuff,"\"http://fs.w.kugou.com",strlen("\"http://fs.w.kugou.com"))==0)
			{
				sgUrl[j++] = quesBuf[i++];
				while(quesBuf[i] != '\"')
				{
					sgUrl[j++] = quesBuf[i++];
				}
				sgUrl[j++] = quesBuf[i];
				sgUrl[j] = '\0';
				break;
			}
		}
		i++;
	}	
	SmdParsingBuffer* smcPreHandle = (SmdParsingBuffer*)smcPreHand;
	strcpy(smcPreHandle->ctlCmdJSON,MEDIA_URL);
	strcat(smcPreHandle->ctlCmdJSON,sgUrl);
}


void musicInfoPlaying(cJSON * musicData,SemcParseHandle smcPreHand)
{
	int i = 0;
	cJSON * musicInfo = cJSON_GetObjectItem(musicData,"musicInfo");
	cJSON * ContInfo = cJSON_GetObjectItem(musicInfo,"content");
	if(ContInfo->type == cJSON_Array)
	{
		cJSON *tNode = NULL;
		int size = cJSON_GetArraySize(ContInfo);
		for(i=0;i<size;i++)
		{
			tNode = cJSON_GetArrayItem(ContInfo,i);
			if(tNode->type == cJSON_Object)
			{
				cJSON * songsAddr = cJSON_GetObjectItem(tNode,"songSourceAddr");
				char * sourceAddress = cJSON_Print(songsAddr);
				BASE_INFO_LOG(InfoLogHandle,"%s%s%s","sourceAddress : ",sourceAddress,"\n");		
				SmdParsingBuffer* smcPreHandle = (SmdParsingBuffer*)smcPreHand;
				strcpy(smcPreHandle->ctlCmdJSON,MEDIA_URL);
				strcat(smcPreHandle->ctlCmdJSON,sourceAddress);
				free(sourceAddress);		
				sourceAddress = NULL;
				break;
			}
		}	
	}
}

void jsonDataDealing(char * jsonBuf,SemcParseHandle smcPreHand)
{
	SmdParsingBuffer* smcPreHandle = (SmdParsingBuffer*)smcPreHand;
	memset(smcPreHandle->ctlCmdJSON,0,1024);
	memset(smcPreHandle->buff_HTTP,0,50);
	memset(smcPreHandle->req_Content,0,4096);

	if(0!=json_checker(jsonBuf,20))
	{
		BASE_ERROR_LOG(ErrLogHandle,"%s","json format checking error\n");
		char * elseCmd = "\"您好,本条数据获取失败,请稍后再试\"";
		strcpy(smcPreHandle->ctlCmdJSON,TTS_TEXT);
		strcat(smcPreHandle->ctlCmdJSON,elseCmd);
		return;
	}
	cJSON * cjsRoot = cJSON_Parse(jsonBuf);
	char * cRoot = cJSON_Print(cjsRoot);
printf("%s\n",cRoot);

	BASE_INFO_LOG(InfoLogHandle,"%s%s",cRoot,"\n");
	cJSON * cjsCont = cJSON_GetObjectItem(cjsRoot,"content");
	char * strCont = cJSON_Print(cjsCont);
	cJSON * cjsCmdType = cJSON_GetObjectItem(cjsCont,"cmdType");
	char * cmdType = cJSON_Print(cjsCmdType);
	if(!strncmp(cmdType,"\"chat\"",6))
	{
		cJSON * cjsCmdData = cJSON_GetObjectItem(cjsCont,"answer");
		char * cmdData = cJSON_Print(cjsCmdData);
		strcpy(smcPreHandle->ctlCmdJSON,TTS_TEXT);
		strcat(smcPreHandle->ctlCmdJSON,cmdData);	
		free(cmdData);			
		cmdData = NULL;
	}else if(!strncmp(cmdType,"\"music\"",7)){
		cJSON * cjsCmdData = cJSON_GetObjectItem(cjsCont,"result");
		cJSON * cjsMusicUrl = cJSON_GetObjectItem(cjsCmdData,"songUrl");
		cJSON * cjsSongName = cJSON_GetObjectItem(cjsCmdData,"songName");
		cJSON * cjsSingerName = cJSON_GetObjectItem(cjsCmdData,"singerName");
		char * musicUrl = cJSON_Print(cjsMusicUrl);
		char * songName = cJSON_Print(cjsSongName);
		char * singerName = cJSON_Print(cjsSingerName);

        BASE_INFO_LOG(InfoLogHandle,"%s%s%s%s%s%s%s","songJun|",musicUrl,"|",songName,"|",singerName,"\n");

		strcpy(smcPreHandle->ctlCmdJSON,MEDIA_URL);
		strcat(smcPreHandle->ctlCmdJSON,musicUrl);
		strcpy(smcPreHandle->songsName,songName);
		strcpy(smcPreHandle->signerName,singerName);
            
		free(musicUrl);
		free(songName);
		free(singerName);
		musicUrl = singerName =  songName = NULL;	
		
	}else if(!strncmp(cmdType,"\"mediaControl\"",strlen("\"mediaControl\""))){
		printf("media control	$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
	}else{
			char * elseCmd = "\"您好,本条数据获取失败,请稍后尝试\"";
			strcpy(smcPreHandle->ctlCmdJSON,TTS_TEXT);
			strcat(smcPreHandle->ctlCmdJSON,elseCmd);
	}		
	free(cmdType);
	cmdType = NULL;
	free(strCont);
	cJSON_Delete(cjsRoot);
	strCont = NULL;
}

int dataFilter(char **dataStr,char **dataStr1)
{
	int lyricFlag = 0,cntLeft = 0,cntRight = 0;
	int filterFlag = 0;
	char * ptrMov = *dataStr;
	char * ptrBase = ptrMov;
	char * ptrBaseKeep = ptrMov;
	char * ptr1Mov = *dataStr1;
	char * ptr1Base = ptr1Mov;
	while(*ptrMov!='\0')
	{
		if((*ptrMov=='{')&&(strncmp(ptrMov,"{\"code\":\"200\"",strlen("{\"code\":\"200\""))==0))	
			break;
		if((*ptrMov=='{')&&(strncmp(ptrMov,"{\"code\":\"-1\"",strlen("{\"code\":\"-1\""))==0))
			return -1;
		ptrMov++;
	}
	while(*(ptrMov)!='\0')
	{
		if(*ptrMov=='{')
			cntLeft++;
		if(*ptrMov=='}')
			cntRight++;							
		*ptr1Mov++ = *ptrMov++;
		if((cntLeft >0)&&(cntLeft==cntRight))
		{
			*ptr1Mov = '\0';
			break;
		}
			
	}
	ptrMov = ptr1Base;
	memcpy(*dataStr,ptrMov,strlen(ptrMov)+1);
	return 1;
}


void cmdPlayThread(SemcParseHandle smcPreHand,char **smdOut)
{	
	SmdParsingBuffer* smcPreHandle = (SmdParsingBuffer*)smcPreHand;
#if 1

	if(NULL == (*smdOut))
		return;
	if(strcmp("NoVoiceInput",smcPreHandle->quesCmdBuff)==0)
	{
		strcpy(*smdOut,TTS_TEXT);
		strcat(*smdOut,"\"未检测到声音输入\"");
        return;
	}
	if(strcmp("CLOUDSERVER_ERROR",smcPreHandle->quesCmdBuff)==0)
	{
		strcpy(*smdOut,TTS_TEXT);
		strcat(*smdOut,"\"正在重新连接服务器\"");
        return;
	}
	if(strcmp("RequestOverTime",smcPreHandle->quesCmdBuff)==0)
	{
		strcpy(*smdOut,TTS_TEXT);
		strcat(*smdOut,"\"获取云端数据超时\"");
        return;
	}
#endif

	int PORTS = smcPreHandle->port;
	char * IPADDR = smcPreHandle->ipAddr;
	int retVal = 0,uSocket = 0;
	int flag = 0, i = 0;
	int countLeft = 0,countRight = 0;
	char * postBuf = smcPreHandle->postHttpReq;
	char * jsonBuf = smcPreHandle->jsonBaseBuff;
	char * jsonKeep = smcPreHandle->jsonTempBuff;
	memset(postBuf,0,1600);
	memset(jsonBuf,0,50*1024);
	memset(jsonKeep,0,50*1024);
	char * jsonBufBase = jsonBuf;
	char * jsonKeepBase = jsonKeep;
	char * ptrMov  = jsonBufBase;
	BASE_INFO_LOG(InfoLogHandle,"%s%s%s","quesCmdBuf - ",smcPreHandle->quesCmdBuff,"\n");
	getPostBuf(&postBuf,smcPreHand);

	uSocket = socketCreate(IPADDR,PORTS);
	

		PERROR(uSocket,"\ncreate Socket Error	\n");
	retVal = write(uSocket,postBuf,strlen(postBuf));
		PERROR(retVal,"\nsend question error\n");
	struct timeval timeoutRcv = {5,0};
	retVal = setsockopt(uSocket,SOL_SOCKET,SO_RCVTIMEO,
							(char *)&timeoutRcv,sizeof(struct timeval));
	if(retVal < 0)
		BASE_ERROR_LOG(ErrLogHandle,"%s","setsockopt for timeout error \n");
	while(1)
	{
		i = 0;
		retVal = recv(uSocket,jsonBuf,BASE_BUF,0);
		if(retVal < 0)
		{
			BASE_ERROR_LOG(ErrLogHandle,"%s","read server error\n");
			strcpy(*smdOut,TTS_TEXT);
			strcat(*smdOut,"\"获取数据超时\"");
			return;
		}
		while(jsonBuf[i] != '\0')
		{
			if(jsonBuf[i] == '{')
				countLeft++;
			if(jsonBuf[i] == '}')
				countRight++;
			i++;
		}
		if(retVal == 0)
			break;
		if((countLeft > 0)&&(countLeft==countRight))
			break;
		jsonBuf += retVal;
	}
	close(uSocket);
	jsonBuf = jsonBufBase;

	retVal = dataFilter(&jsonBuf,&jsonKeepBase);
	if(retVal < 0)
	{
		strcpy(*smdOut,TTS_TEXT);
		strcat(*smdOut,smcPreHandle->quesCmdBuff);
		BASE_ERROR_LOG(ErrLogHandle,"%s","response message error,error code -1\n");
		return;
	}
    if(strlen(jsonBuf) < 10)
    {
        return;
    }
	jsonDataDealing(jsonBuf,smcPreHand);
//printf("jsonDataDealing	~~~~~~~~~~~~~~~~~~~~~~~	\n");

	strcpy(*smdOut,smcPreHandle->ctlCmdJSON);
}



extern SemcParseHandle semcParseHandInit()
{
	SmdParsingBuffer * smdParseHandle = (SmdParsingBuffer *)malloc(sizeof(SmdParsingBuffer));

	smdParseHandle->quesCmdBuff = (char *)malloc(1024);
	smdParseHandle->postHttpReq = (char *)malloc(1600);
	smdParseHandle->jsonBaseBuff = (char *)malloc(1024 *50);
	smdParseHandle->jsonTempBuff = (char *)malloc(1024 *50);
	smdParseHandle->ipAddr = (char *)malloc(50);
	smdParseHandle->apiPath = (char *)malloc(100);
	smdParseHandle->songsName = (char*)malloc(50);
	smdParseHandle->signerName = (char*)malloc(50);
	smdParseHandle->port = 9191;
	strcpy(smdParseHandle->ipAddr,"210.22.153.226");
	strcpy(smdParseHandle->apiPath,"/semanticPlatform/platfrom");
	
	smdParseHandle->buff_HTTP = (char *)malloc(50);
	smdParseHandle->req_Content = (char *)malloc(1024 *4);
	smdParseHandle->ctlCmdJSON = (char *)malloc(1024);
	smdParseHandle->smdParType = WITHOUT_PALYING;
		
	sem_init(&Sem_SmdParse,0,0);
	sem_post(&Sem_SmdParse);
	return (SemcParseHandle)smdParseHandle;
}


extern void semcParseResponse(char *quesCmd,SemcParseHandle semcPrHand,char **smdRlt)
{
	sem_wait(&Sem_SmdParse);
	SmdParsingBuffer * smdParseHandle = (SmdParsingBuffer *)semcPrHand;
	memset(smdParseHandle->quesCmdBuff,0,1024);
	strcpy(smdParseHandle->quesCmdBuff,quesCmd);
	cmdPlayThread(semcPrHand,smdRlt);
	sem_post(&Sem_SmdParse);
	BASE_INFO_LOG(InfoLogHandle,"%s%s",*smdRlt,"		&\n");
}	


