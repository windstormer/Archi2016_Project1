#include <stdio.h>
#include <string.h>



char reg[128];
int PC;
unsigned char ii[1024];
unsigned char di[1024];

/*
int combine(unsigned char a,unsigned char b,unsigned char c,unsigned char d)
{


}*/


int main(void)
{
FILE *iimage = fopen("../../archiTA/testcase/open_testcase/func/iimage.bin","rb");
FILE *dimage = fopen("../../archiTA/testcase/open_testcase/func/dimage.bin","rb");
//FILE *error = fopen("../../archiTA/testcase/open_testcase/func/errormessage.rpt","w");
//FILE *snapshot = fopen("../../archiTA/testcase/open_testcase/func/snap.rpt","w");


long sdimage=0,siimage=0;

int i,j;

memset(reg,0,sizeof(reg));
memset(ii,0,sizeof(ii));
memset(di,0,sizeof(di));
PC=0;

fseek(iimage , 0 , SEEK_END);

  siimage = ftell (iimage);
  rewind (iimage);

fread(ii,sizeof(unsigned char),siimage,iimage);

fseek(dimage , 0 , SEEK_END);

  sdimage = ftell (dimage);
  rewind (dimage);

fread(di,sizeof(unsigned char),sdimage,dimage);

/*
for(i=0;i<sdimage;i++)
printf("%x\n",di[i]);
*/

/*
fread(di[1],1,1,dimage);
fread(di[2],1,1,dimage);
fread(di[3],1,1,dimage);

printf("%c %c %c %c\n",di[0],di[1],di[2],di[3]);

reg[29]=di[0];
fscanf(dimage,"%d",&di[1]);
for(int i=0;i<di[1];i++)
{
    fscanf(dimage,"%d",&di[i+2]);
}
fscanf(iimage,"%d",&ii[0]);
PC=ii[0];
fscanf(iimage,"%d",&ii[1]);
for(int i=0;i<ii[1];i++)
{
    fscanf(iimage,"%d",&ii[i+2]);
}

printf("ii[0]= %d\n",ii[0]);
*/


return 0;
}
