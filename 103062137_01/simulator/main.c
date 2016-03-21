#include <stdio.h>
#include <string.h>



int reg[32];
int PC;
int PC_start;
unsigned char ii[1024];
unsigned char di[1024];
int sp[256];
int spn;
int iim[256];
unsigned char dim[256];

unsigned char cut_rs(int a)
{
    unsigned char back;
    a<<=6;
    back=(unsigned)a>>27;

   // printf("%x ",back);
    return back;
}
unsigned char cut_rt(int a)
{
    unsigned char back;
    a<<=11;
    back=(unsigned)a>>27;

   // printf("%x ",back);
    return back;
}
unsigned char cut_rd(int a)
{
    unsigned char back;
    a<<=16;
    back=(unsigned)a>>27;

   // printf("%x ",back);
    return back;
}
unsigned char cut_shamt(int a)
{
    unsigned char back;
    a<<=21;
    back=(unsigned)a>>27;

    //printf("%x ",back);
    return back;
}
unsigned char cut_func(int a)
{
    unsigned char back;
    a<<=26;
    back=(unsigned)a>>26;

   // printf("%x ",back);
    return back;
}
unsigned short cut_immediate(int a)
{
    unsigned short back;
    a<<=16;
    back=(unsigned)a>>16;

    //printf("%x ",back);
    return back;
}
unsigned int cut_address(int a)
{
    unsigned int back;
    a<<=6;
    back=(unsigned)a>>6;

    //printf("%x ",back);
    return back;
}

int extend_immediate(unsigned short a)
{
    int x = (int)a;
    x<<=16;
    (unsigned)x>>16;

    return (int)x;
}


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
short combine_two(unsigned char a, unsigned char b)
{
    short back=0;
    back |= a;
    back <<= 8;
    back |= b;
    return back;
}

unsigned char* seperate(int in)
{
    unsigned char* back;
    back=(unsigned char*)malloc(sizeof(unsigned char)*4);
    int a,b,c,d;
    a=in,b=in,c=in,d=in;
    a>>24;
    b<<8;
    (unsigned)b>>24;
    c<<16;
    (unsigned)c>>24;
    d<<24;
    (unsigned)d>>24;
    back[0]=(unsigned char)a;
    back[1]=(unsigned char)b;
    back[2]=(unsigned char)c;
    back[3]=(unsigned char)d;

    return back;
}
unsigned char* seperate_two(int in)
{
    unsigned char* back;
    back=(unsigned char*)malloc(sizeof(char)*2);

    in&=0x0000FFFF;

    int c,d;
    c=in,d=in;

    c<<16;
    (unsigned)c>>24;
    d<<24;
    (unsigned)d>>24;
    back[0]=(unsigned char)c;
    back[1]=(unsigned char)d;

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
    int spn=0;
    int i,j;

    memset(reg,0,sizeof(reg));
    memset(ii,0,sizeof(ii));
    memset(di,0,sizeof(di));
    memset(iim,0,sizeof(iim));
    memset(dim,0,sizeof(dim));
    memset(sp,0,sizeof(sp));
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

    for(i=0; i<sdata*4; i++)
    {
        dim[i]=di[8+i];
    }

    PC=combine(ii[0],ii[1],ii[2],ii[3]);
    PC_start=PC;
    sins=combine(ii[4],ii[5],ii[6],ii[7]);

    for(i=0; i<sins; i++)
    {
        iim[i]=combine(ii[8+i*4],ii[9+i*4],ii[10+i*4],ii[11+i*4]);
      //  printf("i=%d %x\n",i,iim[i]);
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

    unsigned char op=0;
    unsigned char rs=0;
    unsigned char rt=0;
    unsigned char rd=0;
    unsigned char shamt=0;
    unsigned char funct=0;
    unsigned short immediate=0;
    unsigned int address=0;

    int read=0;
    unsigned char *getting;
    getting = (unsigned char*)malloc(sizeof(unsigned char)*4);
    int flag=0;
    i=1;
    while(i<sins)
    {
        //printf("i:%d   ",i);
        op=(unsigned)iim[i]>>26;
        //printf("%x ",iim[i]);
        //printf("%x ",op);
        printf("  PC:%x",PC);
        switch(op)
        {
        case 0x00:
        {
            funct=cut_func(iim[i]);
            switch(funct)
            {
            case 0x20:
            {
                rs=cut_rs(iim[i]);
                rt=cut_rt(iim[i]);
                rd=cut_rd(iim[i]);
                reg[rd]=reg[rs]+reg[rt];    ///need overflow detect
                PC+=4;
                break;
            }
            case 0x21:
            {
                rs=cut_rs(iim[i]);
                rt=cut_rt(iim[i]);
                rd=cut_rd(iim[i]);
                reg[rd]=reg[rs]+reg[rt];
                PC+=4;
                break;
            }
            case 0x22:
            {
                rs=cut_rs(iim[i]);
                rt=cut_rt(iim[i]);
                rd=cut_rd(iim[i]);
                reg[rd]=reg[rs]-reg[rt];
                PC+=4;
                break;
            }
            case 0x24:
            {
                rs=cut_rs(iim[i]);
                rt=cut_rt(iim[i]);
                rd=cut_rd(iim[i]);
                reg[rd]=reg[rs]&reg[rt];
                PC+=4;
                break;
            }
            case 0x25:
            {
                rs=cut_rs(iim[i]);
                rt=cut_rt(iim[i]);
                rd=cut_rd(iim[i]);
                reg[rd]=reg[rs]|reg[rt];
                PC+=4;
                break;
            }
            case 0x26:
            {
                rs=cut_rs(iim[i]);
                rt=cut_rt(iim[i]);
                rd=cut_rd(iim[i]);
                reg[rd]=reg[rs]^reg[rt];
                PC+=4;
                break;
            }
            case 0x27:
            {
                rs=cut_rs(iim[i]);
                rt=cut_rt(iim[i]);
                rd=cut_rd(iim[i]);
                reg[rd]=~(reg[rs]|reg[rt]);
                PC+=4;
                break;
            }
            case 0x28:
            {
                rs=cut_rs(iim[i]);
                rt=cut_rt(iim[i]);
                rd=cut_rd(iim[i]);
                reg[rd]=~(reg[rs]&reg[rt]);
                PC+=4;
                break;
            }
            case 0x2A:
            {
                rs=cut_rs(iim[i]);
                rt=cut_rt(iim[i]);
                rd=cut_rd(iim[i]);
                if(reg[rs]<reg[rt])reg[rd]=1;
                else reg[rd]=0;
                PC+=4;
                break;
            }
            case 0x00:
            {
                rt=cut_rt(iim[i]);
                rd=cut_rd(iim[i]);
                shamt=cut_shamt(iim[i]);
                reg[rd]=reg[rt]<<shamt;
                PC+=4;
                break;
            }
            case 0x02:
            {
                rt=cut_rt(iim[i]);
                rd=cut_rd(iim[i]);
                shamt=cut_shamt(iim[i]);
                reg[rd]=(unsigned)reg[rt]>>shamt;
                PC+=4;
                break;
            }
            case 0x03:
            {
                rt=cut_rt(iim[i]);
                rd=cut_rd(iim[i]);
                shamt=cut_shamt(iim[i]);
                reg[rd]=reg[rt]>>shamt;
                PC+=4;
                break;
            }
            case 0x08:
            {
                rs=cut_rs(iim[i]);
                PC=reg[rs];
                break;
            }

            }
            break;
        }
        case 0x08:
        {
            rs=cut_rs(iim[i]);
            rt=cut_rt(iim[i]);
            immediate=cut_immediate(iim[i]);
            reg[rt]=reg[rs]+(int)immediate;      ///need overflow detect
            PC+=4;
            break;
        }
        case 0x09:
        {
            rs=cut_rs(iim[i]);
            rt=cut_rt(iim[i]);
            immediate=cut_immediate(iim[i]);
            reg[rt]=reg[rs]+(int)immediate;
            PC+=4;
            break;
        }
        case 0x23:
        {

            rs=cut_rs(iim[i]);
            rt=cut_rt(iim[i]);
            immediate=cut_immediate(iim[i]);
            if(rs!=0x1D)                    ///if rs == sp then take from sp[]
            {
            read= reg[rs]+(int)immediate;  ///need overflow detect && data misaligned
            reg[rt]=combine(dim[read],dim[read+1],dim[read+2],dim[read+3]);
            }else
            {
                reg[rt]=sp[spn];
                sp[spn]=0;
                spn--;
            }
            PC+=4;
            break;
        }
        case 0x21:
        {
            rs=cut_rs(iim[i]);
            rt=cut_rt(iim[i]);
            immediate=cut_immediate(iim[i]);
            read= reg[rs]+(int)immediate;  ///need overflow detect && data misaligned
            reg[rt]=combine_two(dim[read],dim[read+1]);
            PC+=4;
            break;
        }
        case 0x25:
        {
            rs=cut_rs(iim[i]);
            rt=cut_rt(iim[i]);
            immediate=cut_immediate(iim[i]);
            read= reg[rs]+(int)immediate;  ///need overflow detect && data misaligned
            reg[rt]=(unsigned)combine_two(dim[read],dim[read+1]);
            PC+=4;
            break;
        }
        case 0x20:
        {
            rs=cut_rs(iim[i]);
            rt=cut_rt(iim[i]);
            immediate=cut_immediate(iim[i]);
            read= reg[rs]+(int)immediate;  ///need overflow detect && data misaligned
            reg[rt]=dim[read];
            PC+=4;
            break;
        }
        case 0x24:
        {
            rs=cut_rs(iim[i]);
            rt=cut_rt(iim[i]);
            immediate=cut_immediate(iim[i]);
            read= reg[rs]+(int)immediate;  ///need overflow detect && data misaligned
            reg[rt]=(unsigned)dim[read];
            PC+=4;
            break;
        }
        case 0x2B:
        {
            rs=cut_rs(iim[i]);
            rt=cut_rt(iim[i]);
            immediate=cut_immediate(iim[i]);
            if(rs!=0x1D)
            {
                getting=seperate(reg[rt]);
                read= reg[rs]+(int)immediate;   ///need overflow detect && data misaligned
                dim[read]=getting[0];
                dim[read+1]=getting[1];
                dim[read+2]=getting[2];
                dim[read+3]=getting[3];
            }else
            {
                sp[spn]=reg[rt];
                spn++;
            }
            PC+=4;
            break;
        }
        case 0x29:
        {
            rs=cut_rs(iim[i]);
            rt=cut_rt(iim[i]);
            immediate=cut_immediate(iim[i]);
            getting=seperate_two(reg[rt]);
            read= reg[rs]+(int)immediate;   ///need overflow detect && data misaligned
            dim[read]=getting[0];
            dim[read+1]=getting[1];
            PC+=4;
            break;
        }
        case 0x28:
        {
            rs=cut_rs(iim[i]);
            rt=cut_rt(iim[i]);
            immediate=cut_immediate(iim[i]);
            getting[0]=(unsigned char)(reg[rt]&0x000000FF);
            read= reg[rs]+(int)immediate;   ///need overflow detect && data misaligned
            dim[read]=getting[0];
            PC+=4;
            break;
        }
        case 0x0F:
        {
            rt=cut_rt(iim[i]);
            immediate=cut_immediate(iim[i]);
            reg[rt]=immediate<<16;
            PC+=4;
            break;
        }
        case 0x0C:
        {
            rs=cut_rs(iim[i]);
            rt=cut_rt(iim[i]);
            immediate=cut_immediate(iim[i]);
            reg[rt]=reg[rs]&immediate;
            PC+=4;
            break;
        }
        case 0x0D:
        {
            rs=cut_rs(iim[i]);
            rt=cut_rt(iim[i]);
            immediate=cut_immediate(iim[i]);
            reg[rt]=reg[rs]|immediate;
            PC+=4;
            break;
        }
        case 0x0E:
        {
            rs=cut_rs(iim[i]);
            rt=cut_rt(iim[i]);
            immediate=cut_immediate(iim[i]);
            reg[rt]=~(reg[rs]|immediate);
            PC+=4;
            break;
        }
        case 0x0A:
        {
            rs=cut_rs(iim[i]);
            rt=cut_rt(iim[i]);
            immediate=cut_immediate(iim[i]);
            if(reg[rs]<(short)immediate) reg[rt]=1;
            else reg[rt]=0;
            PC+=4;
            break;
        }
        case 0x04:
        {
            rs=cut_rs(iim[i]);
            rt=cut_rt(iim[i]);
            immediate=cut_immediate(iim[i]);
            if(reg[rs]==reg[rt])
            {
                read=(int)immediate*4+4;        ///need overflow detect
                PC+=read;
            }
            else PC+=4;
            break;
        }
        case 0x05:
        {
            rs=cut_rs(iim[i]);
            rt=cut_rt(iim[i]);
            immediate=cut_immediate(iim[i]);
            if(reg[rs]!=reg[rt])
            {
                read=(int)immediate*4+4;        ///need overflow detect
                PC+=read;
            }
            else PC+=4;
            break;
        }
        case 0x07:
        {
            rs=cut_rs(iim[i]);
            immediate=cut_immediate(iim[i]);
            if(reg[rs]>0)
            {
                read=(int)immediate*4+4;        ///need overflow detect
                PC+=read;
            }
            else PC+=4;
            break;
        }
        case 0x02:
        {
            address=cut_address(iim[i]);
            address<<=2;
            PC+=4;
            PC=(unsigned)PC>>28;
            PC=PC<<28;
            PC=(unsigned)PC|address;
            break;
        }
        case 0x03:
        {
            address=cut_address(iim[i]);
            address<<=2;
            PC+=4;
            reg[31]=PC;

            PC=(unsigned)PC>>28;
            PC=PC<<28;
            PC=(unsigned)PC|address;
            break;
        }
        case 0x3F:
        {
            printf("halt\n");
            flag=1;
            break;
        }


        }

        i=(PC-PC_start)/4;


        printf("\n");

        free(getting);
        if(flag==1) break;
    }

    return 0;
}
