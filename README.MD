# OpenNeko WAV Library

Useless Library.

See Samples [ReadTest](./ReadTest.c) and [WriteTest](./WriteTest.c)


# Usage

## Read WAV File

```c
FILE *fp = fopen("3.wav","rb");

WAV wav;
FMT fmt;

LoadWav(&wav,fp);
ReadWavFMT(&wav,&fmt);
```

## Write WAV File

```c
WAV wav;
FMT fmt;
FILE *fp = fopen("4.wav","wb");

wav.fp = fp;

// Value of Audio
fmt.BitPerSample=8;
fmt.SampleRate=16000;
fmt.NumChannels=1;
InitFMT(&fmt);

// Init WAV Object
InitWav(&wav);
wav.subFMT.Size=16;
wav.subFMT.Data=&fmt;
wav.subDATA.Size=sizeof(data);
wav.subDATA.Data=data;

WriteWAV(&wav,1);

fclose(fp);
```