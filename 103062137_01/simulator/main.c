#include <stdio.h>
#include <string.h>



int reg[32];
int PC;
unsigned char ii[1024];
unsigned char di[1024];

int iim[256];
unsigned char dim[256];


int combine(unsigned char a,unsigned char b,unsigned char c,unsigned char d)
{

int back=0;
back |= a;
back <<= 8;
back |= b;
back <<= 8;
back |= c;
back <<= 8;
back |= d;
return back;

}

int main(void)
{
FILE *iimage = fopen("../../archiTA/testcase/open_testcase/func/iimage.bin","rb");
FILE *dimage = fopen("../../archiTA/testcase/open_testcase/func/dimage.bin","rb");
//FILE *error = fopen("../../archiTA/testcase/open_testcase/func/errormessage.rpt","w");
//FILE *snapshot = fopen("../../archiTA/testcase/open_testcase/func/snap.rpt","w");


long sdimage=0,siimage=0;
int sdata=0,sins=0;

int i,j;

memset(reg,0,sizeof(reg));
memset(ii,0,sizeof(ii));
memset(di,0,sizeof(di));
memset(iim,0,sizeof(iim));
memset(dim,0,sizeof(dim));
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
for(i=0;i<siimage;i++)
printf("%x\n",ii[i]);
*/
reg[29]=combine(di[0],di[1],di[2],di[3]);
sdata=combine(di[4],di[5],di[6],di[7]);

for(i=0;i<sdata*4;i++)
{
    dim[i]=di[8+i];
}

PC=combine(ii[0],ii[1],ii[2],ii[3]);
sins=combine(ii[4],ii[5],ii[6],ii[7]);

for(i=0;i<sins;i++)
{
    iim[i]=combine(ii[8+i*4],ii[9+i*4],ii[10+i*4],ii[11+i*4]);
}
/*
for(i=0;i<sins;i++)
printf("%x\n",iim[i]);
*/

/**
ii : string of all the data int iimage.bin
di : string of all the data int dimage.bin
PC : current PC
reg : int array of all the register
iim : int array of all the instruction
dim : unsigned char array of all the data in memory
sins: int of the number of instruction
sdata : int of the number of memory (word)
**/

return 0;
}
