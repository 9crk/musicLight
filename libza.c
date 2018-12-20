
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "portaudio.h"
PaStream *stream;
int za_init(int sr, int chn)
{
    PaError err;
    
    PaStreamParameters inputParameters;
    err = Pa_Initialize();
    inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
    if (inputParameters.device == paNoDevice) {
        printf("Error: No default input device.\n");
	return -1;
    }
    inputParameters.channelCount = chn;
    inputParameters.sampleFormat = paInt16;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream(
              &stream,
              &inputParameters,
              NULL,                  /* &outputParameters, */
              sr,
              1024,//FRAMES_PER_BUFFER,
              paClipOff,      /* we won't output out of range samples so don't bother clipping them */
              NULL, /* no callback, use blocking API */
              NULL ); /* no callback, so no callback userData */
    if( err != paNoError )return -1;;

    err = Pa_StartStream( stream );
    if( err != paNoError )return -1;;
    printf("recording..."); fflush(stdout);
    return 0;    
}
int za_read(char*buff,int frames)
{
    PaError err;
    err = Pa_ReadStream( stream, buff, frames);
    if( err != paNoError ) return -1;;
    return 0;
}
int za_exit()
{
    PaError err;
    err = Pa_CloseStream( stream );
    if( err != paNoError )return -1;
    return 0;
}
typedef struct WAV_HEADER_S  
{  
    char            riffType[4];    //4byte,资源交换文件标志:RIFF     
    unsigned int    riffSize;       //4byte,从下个地址到文件结尾的总字节数   
    char            waveType[4];    //4byte,wav文件标志:WAVE      
    char            formatType[4];  //4byte,波形文件标志:FMT(最后一位空格符)   
    unsigned int    formatSize;     //4byte,音频属性(compressionCode,numChannels,sampleRate,bytesPerSecond,blockAlign,bitsPerSample)所占字节数  
    unsigned short  compressionCode;//2byte,格式种类(1-线性pcm-WAVE_FORMAT_PCM,WAVEFORMAT_ADPCM)  
    unsigned short  numChannels;    //2byte,通道数  
    unsigned int    sampleRate;     //4byte,采样率  
    unsigned int    bytesPerSecond; //4byte,传输速率  
    unsigned short  blockAlign;     //2byte,数据块的对齐，即DATA数据块长度  
    unsigned short  bitsPerSample;  //2byte,采样精度-PCM位宽  
    char            dataType[4];    //4byte,数据标志:data  
    unsigned int    dataSize;       //4byte,从下个地址到文件结尾的总字节数，即除了wav header以外的pcm data length  
}WAV_HEADER;
static int za_gen_header(char* buff)
{
    WAV_HEADER hd;
    strncpy(hd.riffType,"RIFF",4);
    hd.riffSize=16000+sizeof(WAV_HEADER)-8;
    strncpy(hd.waveType,"WAVE",4);
    strncpy(hd.formatType,"fmt ",4);
    hd.formatSize=2;
    hd.compressionCode=1;
    hd.numChannels = 1;
    hd.sampleRate=8000;
    hd.bytesPerSecond=16000;
    hd.blockAlign=4;
    hd.bitsPerSample=16;
    strncpy(hd.dataType,"data",4);
    hd.dataSize=0;
    memcpy(buff,&hd,sizeof(WAV_HEADER));
    return sizeof(WAV_HEADER);
} 
int record(char* filename,int samples)
{
    int len;
    FILE*fp = fopen(filename,"wb");
    char *buf = (char*)malloc(samples*2);    
    char hd[sizeof(WAV_HEADER)];
    za_init(8000,1);
    za_read(buf,samples);
    za_exit();   
    len = za_gen_header(hd);
    fwrite(hd,len,1,fp);
    fwrite(buf,1,samples*2,fp);
    fclose(fp);
    free(buf);
    return 0;
}
int record2buf(char* buf,int samples)
{
    za_init(8000,1);
    za_read(buf,samples);
    za_exit();   
    return 0;
}
int buf2file(char*buf,int samples,char*filename)
{
    int len;
    FILE*fp = fopen(filename,"wb");
    char hd[sizeof(WAV_HEADER)];
    len = za_gen_header(hd);
    fwrite(hd,len,1,fp);
    fwrite(buf,1,samples*2,fp);
    fclose(fp);
    return 0;
}

#if 0
#include "math.h"
#define PI 3.1415926535897932384626433832795028841971
int main(int argc,char* argv[])
{
    int i,j;
    short buf[1024];
    za_init(8000,1);

    for(i=0;i<1024;i++){
        buf[i] = 512*sin(11*2*PI*i/1024)+sin(13*2*PI*i/1024); 
        for(j=0;j<(buf[i])/10;j++)printf("*");
        printf("\n");
    }
    fix_fftr(buf,9,0);
    
    for(i=0;i<512;i++){
        for(j=0;j<(buf[i])/10;j++)printf("*");
        printf("\n");
    }
    
    /*while(1){
        za_read((char*)buf,1024);
        fix_fftr(buf,9,0);
        for(i=0;i<128;i+=8){
            for(j=0;j<(buf[i])/100;j++)printf("*");
            printf("\n");
        }
        printf("--------------------------------------\n");
    }*/
    za_exit();   
}
#endif



