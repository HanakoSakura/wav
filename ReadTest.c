#include "wav.h"

static unsigned char buffer[256];

int main(){
    FILE *fp = fopen("3.wav","rb"),*out = fopen("3.pcm","wb");

    WAV wav;
    FMT fmt;

    LoadWav(&wav,fp);
    ReadWavFMT(&wav,&fmt);

    printf("Time:%.2lfs\n",wav.subDATA.Size/(double)fmt.SampleRate);
    printf("sr = %u\n",fmt.SampleRate);

    int c;
    fseek(fp,wav.subDATA.Offset+8,SEEK_SET);
    do{
        c = fread(&buffer,1,sizeof(buffer),fp);
        fwrite(buffer,1,c,out);
    }while(c);

    fclose(fp);
    fclose(out);

    return 0;
}