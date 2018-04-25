#ifndef _SEMC_PARSE_H_
#define _SEMC_PARSE_H_

#ifdef __cpluspluc
extern "C"{
#endif


#include <semaphore.h>
#include "alsaInterface.h"
#include "cmdReqInterface.h" 
#include "cJSON.h"
#include "speex/speex_resampler.h"

#define  MEDIA_URL   "media_url_"
#define  TTS_TEXT    "tts_text_"

//#define  HWappKey		"676fbb7cfba3d231695778c6d8534892"
//#define  HWappSecret 	"7857dcda453f2754466344e52f94ac80"


//#define  HWappKey		"6ea7c2040658730201d4d498bc4dbfe7"
//#define  HWappSecret 	"78662c5d8c74dd2c79f63fcf17360a68"
#define  HWappKey		"9764970ec2c1fb891d869d8c3f4750ca"
#define  HWappSecret 	"cb2d06461c1129ddbc4b7b8bb76ceab2"

sem_t Sem_SmdParse;


#define WITHOUT_PALYING  0
#define MEDIA_PLAYING    1
#define TTS_PLAYING      2
#define CONTROL_CMD		 3



typedef struct 
{
	char *ipAddr;
	char *apiPath;
	int  port;
	char * buff_HTTP;
	char * req_Content;
	char * ctlCmdJSON;
	char * signerName;
	char * songsName;
	int  smdParType;
	char *  quesCmdBuff;						/************quesCmd ********/
	char *  postHttpReq;						/************store  request cmd	 to get 8098 content *****/	
	char *  jsonBaseBuff;						/************for json Dealing process *******/
	char *  jsonTempBuff;						/************for json Dealing process 	too*******/	
}SmdParsingBuffer;

int MP3BoradCast(char *sAddr,SemcParseHandle smcPreHand);
extern SemcParseHandle semcParseHandInit();
extern void robotCmdResponse(char *quesCmd,SemcParseHandle semcPrHand,char **smdRlt);

#ifdef __cplusplus
}
#endif

#endif

