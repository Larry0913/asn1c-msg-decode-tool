#include "MessageFrame.h"
#include <sys/time.h>
#include <time.h>

void uPEREncode()
{
    //BSM编码

    int currentSecMark, i;
    struct timeval tv;
    struct tm* currentTime;
    int BsmCnt = 0;
    int MsgCnt = 0;
    BasicSafetyMessage_t *bsm;
    MessageFrame_t *msgFrame = (MessageFrame_t *)malloc(sizeof(MessageFrame_t));
    memset(msgFrame, 0, sizeof(MessageFrame_t));
    msgFrame->present = MessageFrame_PR_bsmFrame;
    bsm = &msgFrame->choice.bsmFrame;

    bsm->id.buf = (uint8_t *)malloc(8);
    bsm->id.size = 8;
    memcpy(bsm->id.buf ,"12345678", 8);  

    bsm->msgCnt = BsmCnt++;
    BsmCnt %= 128;
    bsm->motionCfd = NULL;
    
    gettimeofday(&tv, NULL);
    currentTime = localtime(&tv.tv_sec);
    currentSecMark = (currentTime->tm_sec * 1000 + tv.tv_usec / 1000);   
    bsm->secMark = currentSecMark;
    bsm->timeConfidence = NULL;


    //填充车身自身信息，值暂时都定为0，后续可从CAN总线上取
    bsm->accelSet.lat = 0;
    bsm->accelSet.Long = 0;
    bsm->accelSet.vert = 0;
    bsm->accelSet.yaw = 0;    
    bsm->angle = NULL;                              //方向盘角度 GB：1.5°， -126~127 127为无效值，左负右正
    bsm->brakes.brakePadel = (BrakePedalStatus_t*)malloc(sizeof(BrakePedalStatus_t));
    *(bsm->brakes.brakePadel) = 0;                  //GB:0->unavailable;1->not pressed;2->pressed
    bsm->brakes.abs = NULL;
    bsm->brakes.auxBrakes = NULL;
    bsm->brakes.brakeBoost = NULL;
    bsm->brakes.scs = NULL;
    bsm->brakes.traction = NULL;
    bsm->brakes.wheelBrakes = NULL;
    bsm->transmission = 0;                          //GB:0->N,1->P,2->D,3->R
    bsm->motionCfd = NULL;

    //填充本车定位相关数据
    bsm->heading = 12000;             //GB:0.0125°
    bsm->pos.lat = 300000000;           //GB:1.0e-7
    bsm->pos.Long =1200000000;          //GB:1.0e-7
    bsm->pos.elevation = (Elevation_t*)malloc(sizeof(Elevation_t));
    *(bsm->pos.elevation) = 210;        //GB:0.1m
    bsm->speed = 100;                 //GB：0.02m/s
    bsm->posAccuracy = NULL;
    bsm->posConfidence = NULL;

    bsm->size.length = 400;                         //GB 1cm
    bsm->size.width = 150;                          //GB 1cm
    bsm->size.height = (VehicleHeight_t*)malloc(sizeof(VehicleHeight_t));
    *(bsm->size.height) = 30;                       //GB 5cm
    bsm->vehicleClass.classification = 10;
    bsm->vehicleClass.fuelType = NULL;

    bsm->safetyExt = NULL;
    bsm->emergencyExt = NULL;

	//xml打印
    xer_fprint(stdout, &asn_DEF_MessageFrame, msgFrame); 
    //===========  对填充数据进行UPER编码并返回编码后的字符串  ==================
    unsigned char *chbuf = (unsigned char *)malloc(260);    
    memset(chbuf, 0, 260);
    int encodeSize = uper_encode_to_new_buffer(&asn_DEF_MessageFrame, 0,msgFrame, (void**)&chbuf);
    if (encodeSize == -1)
    {
        printf("Could not encode bsmFrame \n");
    }
    else
    {
        for ( i = 0; i < encodeSize; i++)
        {
            printf("%02X ", chbuf[i] & 0xff); //发送的编码  
        }
        printf("\n");
    }
}
