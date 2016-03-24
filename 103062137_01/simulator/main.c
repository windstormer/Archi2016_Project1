#include <stdio.h>
#include <string.h>
#include "function.h"




int reg[32];
int PC;
int PC_start;
unsigned char ii[1024];
unsigned char di[1024];
int iim[256];
unsigned char dim[1024];

int main(void)
{
    FILE *iimage = fopen("../../archiTA/testcase/open_testcase/error2/iimage.bin","rb");
    FILE *dimage = fopen("../../archiTA/testcase/open_testcase/error2/dimage.bin","rb");
    FILE *error = fopen("./error_dump.rpt","w");
    FILE *snapshot = fopen("./snapshot.rpt","w");



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
    short immediate=0;
    unsigned int address=0;

    int read=0;
    unsigned char *getting;
    getting = (unsigned char*)malloc(sizeof(unsigned char)*4);
    int flag=0;
    i=0;
    int cycle=0;
    while(i<sins)
    {
        //printf("i:%d   ",i);
        op=(unsigned)iim[i]>>26;
        //printf("%x ",iim[i]);
        //printf("%x ",op);
        fprintf(snapshot,"cycle %d\n",cycle);
         for(j=0;j<32;j++)
        {
            fprintf(snapshot,"$%02d: 0x%08X\n",j,reg[j]);
        }
        fprintf(snapshot,"PC: 0x%08X\n",PC);

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
                if(rd==0)
                    fprintf(error,"In cycle %d: Write $0 Error\n",cycle+1);
                else
                reg[rd]=reg[rs]+reg[rt];    ///need overflow detect
                if(overflow_detect(reg[rd],reg[rs],reg[rt]))
                    fprintf(error,"In cycle %d: Number Overflow\n",cycle+1);
                PC+=4;
                break;
            }
            case 0x21:
            {
                rs=cut_rs(iim[i]);
                rt=cut_rt(iim[i]);
                rd=cut_rd(iim[i]);
                if(rd==0)
                    fprintf(error,"In cycle %d: Write $0 Error\n",cycle+1);
                else
                reg[rd]=reg[rs]+reg[rt];
                PC+=4;
                break;
            }
            case 0x22:
            {
                rs=cut_rs(iim[i]);
                rt=cut_rt(iim[i]);
                rd=cut_rd(iim[i]);
                if(rd==0)
                    fprintf(error,"In cycle %d: Write $0 Error\n",cycle+1);
                else
                reg[rd]=reg[rs]-reg[rt];
                if(overflow_detect(reg[rd],reg[rs],reg[rt]))
                    fprintf(error,"In cycle %d: Number Overflow\n",cycle+1);
                PC+=4;
                break;
            }
            case 0x24:
            {
                rs=cut_rs(iim[i]);
                rt=cut_rt(iim[i]);
                rd=cut_rd(iim[i]);
                if(rd==0)
                    fprintf(error,"In cycle %d: Write $0 Error\n",cycle+1);
                else
                reg[rd]=reg[rs]&reg[rt];
                PC+=4;
                break;
            }
            case 0x25:
            {
                rs=cut_rs(iim[i]);
                rt=cut_rt(iim[i]);
                rd=cut_rd(iim[i]);
                if(rd==0)
                    fprintf(error,"In cycle %d: Write $0 Error\n",cycle+1);
                else
                reg[rd]=reg[rs]|reg[rt];
                PC+=4;
                break;
            }
            case 0x26:
            {
                rs=cut_rs(iim[i]);
                rt=cut_rt(iim[i]);
                rd=cut_rd(iim[i]);
                if(rd==0)
                    fprintf(error,"In cycle %d: Write $0 Error\n",cycle+1);
                else
                reg[rd]=reg[rs]^reg[rt];
                PC+=4;
                break;
            }
            case 0x27:
            {
                rs=cut_rs(iim[i]);
                rt=cut_rt(iim[i]);
                rd=cut_rd(iim[i]);
                if(rd==0)
                    fprintf(error,"In cycle %d: Write $0 Error\n",cycle+1);
                else
                reg[rd]=~(reg[rs]|reg[rt]);
                PC+=4;
                break;
            }
            case 0x28:
            {
                rs=cut_rs(iim[i]);
                rt=cut_rt(iim[i]);
                rd=cut_rd(iim[i]);
                if(rd==0)
                    fprintf(error,"In cycle %d: Write $0 Error\n",cycle+1);
                else
                reg[rd]=~(reg[rs]&reg[rt]);
                PC+=4;
                break;
            }
            case 0x2A:
            {
                rs=cut_rs(iim[i]);
                rt=cut_rt(iim[i]);
                rd=cut_rd(iim[i]);
                if(rd==0)
                    fprintf(error,"In cycle %d: Write $0 Error\n",cycle+1);
                else
                {
                     if(reg[rs]<reg[rt])reg[rd]=1;
                else reg[rd]=0;
                }

                PC+=4;
                break;
            }
            case 0x00:
            {
                rt=cut_rt(iim[i]);
                rd=cut_rd(iim[i]);
                shamt=cut_shamt(iim[i]);
                if(rd==0)
                    fprintf(error,"In cycle %d: Write $0 Error\n",cycle+1);
                else
                reg[rd]=reg[rt]<<shamt;
                PC+=4;
                break;
            }
            case 0x02:
            {
                rt=cut_rt(iim[i]);
                rd=cut_rd(iim[i]);
                shamt=cut_shamt(iim[i]);
                if(rd==0)
                    fprintf(error,"In cycle %d: Write $0 Error\n",cycle+1);
                else
                reg[rd]=(unsigned)reg[rt]>>shamt;
                PC+=4;
                break;
            }
            case 0x03:
            {
                rt=cut_rt(iim[i]);
                rd=cut_rd(iim[i]);
                shamt=cut_shamt(iim[i]);
                if(rd==0)
                    fprintf(error,"In cycle %d: Write $0 Error\n",cycle+1);
                else
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
            if(rt==0)
                fprintf(error,"In cycle %d: Write $0 Error\n",cycle+1);
            else
            reg[rt]=reg[rs]+immediate;      ///need overflow detect
            if(overflow_detect(reg[rt],reg[rs],(int)immediate))
                    fprintf(error,"In cycle %d: Number Overflow\n",cycle+1);
            PC+=4;
            break;
        }
        case 0x09:
        {
            rs=cut_rs(iim[i]);
            rt=cut_rt(iim[i]);
            immediate=cut_immediate(iim[i]);
            if(rt==0)
                fprintf(error,"In cycle %d: Write $0 Error\n",cycle+1);
            else
            reg[rt]=reg[rs]+(unsigned)immediate;
            PC+=4;
            break;
        }
        case 0x23:
        {

            rs=cut_rs(iim[i]);
            rt=cut_rt(iim[i]);
            immediate=cut_immediate(iim[i]);
            if(rt==0)
                fprintf(error,"In cycle %d: Write $0 Error\n",cycle+1);
            else
            {
                 read= reg[rs]+immediate;  ///need overflow detect && data misaligned
            if(overflow_detect(read,reg[rs],(int)immediate))
                    fprintf(error,"In cycle %d: Number Overflow\n",cycle+1);
            if(read>=1021)
            {
                    fprintf(error,"In cycle %d: Address Overflow\n",cycle+1);
                    flag=1;
                    break;
            }
            if(read%4!=0)
            {
                fprintf(error,"In cycle %d: Misalignment Error\n",cycle+1);
                flag=1;
                break;
            }
            reg[rt]=(short)combine(dim[read],dim[read+1],dim[read+2],dim[read+3]);
            }


            PC+=4;
            break;
        }
        case 0x21:
        {
            rs=cut_rs(iim[i]);
            rt=cut_rt(iim[i]);
            immediate=cut_immediate(iim[i]);
            if(rt==0)
                fprintf(error,"In cycle %d: Write $0 Error\n",cycle+1);
            else
            {
              read= reg[rs]+immediate;  ///need overflow detect && data misaligned
            if(overflow_detect(read,reg[rs],(int)immediate))
                    fprintf(error,"In cycle %d: Number Overflow\n",cycle+1);
            if(read>=1023)
            {
                    fprintf(error,"In cycle %d: Address Overflow\n",cycle+1);
                    flag=1;
                    break;
            }
            if(read%2!=0)
            {
                fprintf(error,"In cycle %d: Misalignment Error\n",cycle+1);
                flag=1;
                break;
            }
            reg[rt]=(char)combine_two(dim[read],dim[read+1]);
            }

            PC+=4;
            break;
        }
        case 0x25:
        {
            rs=cut_rs(iim[i]);
            rt=cut_rt(iim[i]);
            immediate=cut_immediate(iim[i]);
            if(rt==0)
                fprintf(error,"In cycle %d: Write $0 Error\n",cycle+1);
            else
            {
               read= reg[rs]+immediate;  ///need overflow detect && data misaligned
            if(overflow_detect(read,reg[rs],(int)immediate))
                fprintf(error,"In cycle %d: Number Overflow\n",cycle+1);
            if(read>=1023)
            {
                    fprintf(error,"In cycle %d: Address Overflow\n",cycle+1);
                    flag=1;
                    break;
            }
            if(read%2!=0)
            {
                fprintf(error,"In cycle %d: Misalignment Error\n",cycle+1);
                flag=1;
                break;
            }
            reg[rt]=combine_two(dim[read],dim[read+1]);
            }

            PC+=4;
            break;
        }
        case 0x20:
        {
            rs=cut_rs(iim[i]);
            rt=cut_rt(iim[i]);
            immediate=cut_immediate(iim[i]);
            if(rt==0)
                fprintf(error,"In cycle %d: Write $0 Error\n",cycle+1);
            else
            {
               read= reg[rs]+immediate;  ///need overflow detect && data misaligned
            if(overflow_detect(read,reg[rs],(int)immediate))
                fprintf(error,"In cycle %d: Number Overflow\n",cycle+1);
            if(read>=1024)
            {
                    fprintf(error,"In cycle %d: Address Overflow\n",cycle+1);
                    flag=1;
                    break;
            }
            reg[rt]=(char)dim[read];
            }

            PC+=4;
            break;
        }
        case 0x24:
        {
            rs=cut_rs(iim[i]);
            rt=cut_rt(iim[i]);
            immediate=cut_immediate(iim[i]);
            if(rt==0)
                fprintf(error,"In cycle %d: Write $0 Error\n",cycle+1);
            else
            {
              read= reg[rs]+immediate;  ///need overflow detect && data misaligned
            if(overflow_detect(read,reg[rs],(int)immediate))
                fprintf(error,"In cycle %d: Number Overflow\n",cycle+1);
            if(read>=1024)
            {
                    fprintf(error,"In cycle %d: Address Overflow\n",cycle+1);
                    flag=1;
                    break;
            }
            reg[rt]=(unsigned)dim[read];
            }

            PC+=4;
            break;
        }
        case 0x2B:
        {
            rs=cut_rs(iim[i]);
            rt=cut_rt(iim[i]);
            immediate=cut_immediate(iim[i]);

                getting=seperate(reg[rt]);
                read= reg[rs]+immediate;   ///need overflow detect && data misaligned
                if(overflow_detect(read,reg[rs],(int)immediate))
                    fprintf(error,"In cycle %d: Number Overflow\n",cycle+1);
                    if(read>=1021)
            {
                    fprintf(error,"In cycle %d: Address Overflow\n",cycle+1);
                    flag=1;
                    break;
            }
            if(read%4!=0)
            {
                fprintf(error,"In cycle %d: Misalignment Error\n",cycle+1);
                flag=1;
                break;
            }
                dim[read]=getting[0];
                dim[read+1]=getting[1];
                dim[read+2]=getting[2];
                dim[read+3]=getting[3];

            PC+=4;
            break;
        }
        case 0x29:
        {
            rs=cut_rs(iim[i]);
            rt=cut_rt(iim[i]);
            immediate=cut_immediate(iim[i]);
            getting=seperate_two(reg[rt]);
            read= reg[rs]+immediate;   ///need overflow detect && data misaligned
            if(overflow_detect(read,reg[rs],(int)immediate))
                fprintf(error,"In cycle %d: Number Overflow\n",cycle+1);
                if(read>=1023)
            {
                    fprintf(error,"In cycle %d: Address Overflow\n",cycle+1);
                    flag=1;
                    break;
            }
            if(read%2!=0)
            {
                fprintf(error,"In cycle %d: Misalignment Error\n",cycle+1);
                flag=1;
                break;
            }
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
            read= reg[rs]+immediate;   ///need overflow detect && data misaligned
            if(overflow_detect(read,reg[rs],(int)immediate))
                fprintf(error,"In cycle %d: Number Overflow\n",cycle+1);
                if(read>=1024)
            {
                    fprintf(error,"In cycle %d: Address Overflow\n",cycle+1);
                    flag=1;
                    break;
            }
            dim[read]=getting[0];
            PC+=4;
            break;
        }
        case 0x0F:
        {
            rt=cut_rt(iim[i]);
            immediate=cut_immediate(iim[i]);
            if(rt==0)
                fprintf(error,"In cycle %d: Write $0 Error\n",cycle+1);
            else
            reg[rt]=immediate<<16;
            PC+=4;
            break;
        }
        case 0x0C:
        {
            rs=cut_rs(iim[i]);
            rt=cut_rt(iim[i]);
            immediate=cut_immediate(iim[i]);
            if(rt==0)
                fprintf(error,"In cycle %d: Write $0 Error\n",cycle+1);
            else
            reg[rt]=reg[rs]&(unsigned)immediate;
            PC+=4;
            break;
        }
        case 0x0D:
        {
            rs=cut_rs(iim[i]);
            rt=cut_rt(iim[i]);
            immediate=cut_immediate(iim[i]);
            if(rt==0)
                fprintf(error,"In cycle %d: Write $0 Error\n",cycle+1);
            else
            reg[rt]=reg[rs]|(unsigned)immediate;
            PC+=4;
            break;
        }
        case 0x0E:
        {
            rs=cut_rs(iim[i]);
            rt=cut_rt(iim[i]);
            immediate=cut_immediate(iim[i]);
            if(rt==0)
                fprintf(error,"In cycle %d: Write $0 Error\n",cycle+1);
            else
            reg[rt]=~(reg[rs]|(unsigned)immediate);
            PC+=4;
            break;
        }
        case 0x0A:
        {
            rs=cut_rs(iim[i]);
            rt=cut_rt(iim[i]);
            immediate=cut_immediate(iim[i]);
            if(rt==0)
                fprintf(error,"In cycle %d: Write $0 Error\n",cycle+1);
            else
            {
              if(reg[rs]<immediate) reg[rt]=1;
            else reg[rt]=0;
            }

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
                read=immediate*4+4;        ///need overflow detect
                if(overflow_detect(read,immediate*4,4))
                    fprintf(error,"In cycle %d: Number Overflow\n",cycle+1);
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
                read=immediate*4+4;        ///need overflow detect
                if(overflow_detect(read,immediate*4,4))
                    fprintf(error,"In cycle %d: Number Overflow\n",cycle+1);
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
                read=immediate*4+4;        ///need overflow detect
                if(overflow_detect(read,immediate*4,4))
                    fprintf(error,"In cycle %d: Number Overflow\n",cycle+1);
                PC+=read;
            }
            else PC+=4;
            break;
        }
        case 0x02:
        {
            address=cut_address(iim[i]);
            address=address<<2;
            PC+=4;
            PC=(unsigned)PC>>28;
            PC=PC<<28;
            PC=(unsigned)PC|address;
            break;
        }
        case 0x03:
        {
            address=cut_address(iim[i]);
            address=address<<2;
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

        cycle++;

        fprintf(snapshot,"\n\n");

      
        if(flag==1) break;
    }

    return 0;
}
