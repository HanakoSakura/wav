/*
 * Code by Karyna Sakura(Dark Devil)
 * 
 * A WAV Reader & Writer
 * Useless.
 * 
*/

// ! The 0 return value means SUCCEED, !0 meand FAIL.

#ifndef __SAKURA_WAV_FILE_IO__
#define __SAKURA_WAV_FILE_IO__ 1

// Need Head File
#include <stdio.h>
#include <string.h>

// Chunk
typedef struct _CHUNK_{
    // File Offset
    unsigned Offset;
    // Chunk Size
    unsigned Size;
    // Chunk ID
    char ID[4];
    // Data Buffer (For Write)
    void* Data;
} CHUNK,*rCHUNK;

// WAV Struct
typedef struct _WAV_{
    FILE *fp;
    CHUNK subFMT,subDATA;
} WAV,*rWAV;

// Read Chunk
int __ReadChunk(rCHUNK ck,FILE *fp){
    int ret = 1;
    ck->Offset = ftell(fp);
    ret &= fread(ck->ID,4,1,fp);
    ret &= fread(&ck->Size,4,1,fp);
    return !ret;
}

// Move To Next Chunk
int __NextChunk(rCHUNK buf,rCHUNK last,FILE *fp){
    int ret = 0;
    if(last){
        // Move to the last chunk
        ret |= fseek(fp,last->Offset,SEEK_SET);
        // Move to the next chunk
        ret |= fseek(fp,last->Size+8,SEEK_CUR);
    }
    // read chunk
    return ret | __ReadChunk(buf,fp);
}

int LoadWav(rWAV wav,FILE *fp){
    
    CHUNK tmp;
    tmp.Offset = 0;
    tmp.Size = 4;
    int flag;

    wav->fp = fp;

    // Find FMT Chunk
    flag = 2;
    while(!__NextChunk(&tmp,&tmp,fp)){
        if(!memcmp(tmp.ID,"fmt ",4)){
            flag = 0;
            break;
        }
    }
    if(flag) return flag;

    wav->subFMT = tmp;

    // Find data Chunk
    flag = 2;
    while(!__NextChunk(&tmp,&tmp,fp)){
        if(!memcmp(tmp.ID,"data ",4)){
            flag = 0;
            break;
        }
    }
    if(flag) return flag;

    wav->subDATA = tmp;

    return 0;
}

int JmpWavData(rWAV wav){
    FILE *fp = wav->fp;
    return fseek(fp,wav->subDATA.Offset+8,SEEK_SET);
}

unsigned GetWavDataSize(rWAV wav){
    return wav->subDATA.Size;
}

typedef struct _FMT_{
    unsigned short AudioFormat;
    unsigned short NumChannels;
    unsigned int SampleRate;
    unsigned int ByteRate;
    unsigned short BlockAlign;
    unsigned short BitPerSample;
} FMT,*rFMT;

int ReadWavFMT(rWAV wav,rFMT fmt){
    // Move to FMT Chunk
    if(fseek(wav->fp,wav->subFMT.Offset+8,SEEK_SET)) return 1;
    // Read FMT
    if(!fread(fmt,16,1,wav->fp)) return 2;
    return 0;
}


int WriteWAV(rWAV wav,char flag){
    if(!wav->fp||!wav) return 1;
    unsigned size;
    size = wav->subDATA.Size + wav->subFMT.Size + 20;
    
    fwrite("RIFF",4,1,wav->fp);
    fwrite(&size,4,1,wav->fp);
    fwrite("WAVE",4,1,wav->fp);
    
    fwrite(wav->subFMT.ID,4,1,wav->fp);
    fwrite(&wav->subFMT.Size,4,1,wav->fp);
    fwrite(wav->subFMT.Data,wav->subFMT.Size,1,wav->fp);
    
    fwrite(wav->subDATA.ID,4,1,wav->fp);
    fwrite(&wav->subDATA.Size,4,1,wav->fp);
    // HACK 
    // Usually [data] are very large.
    // So it should copy from file, not memory.
    if(flag)
        fwrite(wav->subDATA.Data,wav->subDATA.Size,1,wav->fp);
    return 0;
}

int InitWav(rWAV wav){
    if(!wav)return 1;
    memcpy(wav->subDATA.ID,"data",4);
    memcpy(wav->subFMT.ID,"fmt ",4);
    return 0;
}

int InitFMT(rFMT fmt){
    if(!fmt)return 1;
    fmt->AudioFormat=1;
    fmt->BlockAlign = (fmt->NumChannels)*(fmt->BitPerSample);
    fmt->ByteRate = (fmt->BlockAlign) * (fmt->SampleRate);
    return 0;
}

#endif