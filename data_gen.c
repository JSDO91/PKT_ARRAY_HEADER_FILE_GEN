#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define PKT_LEN 10
#define SEDD_VAL_MASK 0x7FFF
#define PAYLOAD_LEN 1500

struct timeval mytime;
unsigned int len_array[PKT_LEN];

int main()
{
    FILE *Hfile_ptr;
    char* file_name = "pkt_len_array.h";
    char* len_array_name;
    char value[40];
    int idx = 0;
    int temp_pkt_len;
    unsigned int payload_seed_val;
    unsigned int temp_val;
    unsigned int payload_idx            = 0;
    unsigned int _4byte_payload_len     = 0;
    unsigned int _4byte_payload_remain  = 0;
    int array_len = PKT_LEN;
    srand(time(NULL));
// make pkt_len_array.h
    Hfile_ptr = fopen(file_name,"w");
    if(Hfile_ptr < 0)
    {
        printf("failed to fopen\r\n");
        return -1;
    }

    fprintf(Hfile_ptr,"#ifndef __PKT_NUM_LEN_\r\n#define __PKT_NUM_LEN_\r\n\r\n");
    fprintf(Hfile_ptr,"unsigned int data_len[%d] = {\r\n\t", array_len);

    for(idx = 0; idx < array_len; idx++)
    {
        if(idx == array_len - 1)
        {
            temp_pkt_len = rand()%1501;
            len_array[idx] = temp_pkt_len;
            fprintf(Hfile_ptr,"%5d\r\n};\r\n\r\n",temp_pkt_len);
        }
        else
        {
            if( (idx != 0) && (idx % 10 == 0) )
                fprintf(Hfile_ptr,"\r\n\t");

            temp_pkt_len = rand()%1501;
            len_array[idx] = temp_pkt_len;

            fprintf(Hfile_ptr,"%5d, ",temp_pkt_len);
            
        }
    }

    fprintf(Hfile_ptr,"#endif");
    fclose(Hfile_ptr);

// make pkt_payload_array.h   

    file_name = "pkt_payload_array.h";
    Hfile_ptr = fopen(file_name,"w");
    if(Hfile_ptr < 0)
    {
        printf("failed to fopen\r\n");
        return -1;
    }

    fprintf(Hfile_ptr,"#ifndef __PKT_PAYLOAD_\r\n#define __PKT_PAYLOAD\r\n\r\n");
    fprintf(Hfile_ptr,"unsigned int payload[%d][%d] = {\r\n\t", array_len,PAYLOAD_LEN);

    for(idx = 0; idx < array_len; idx++)
    {
        payload_seed_val        = rand()&SEDD_VAL_MASK;
        _4byte_payload_len      = len_array[idx]/4;
        _4byte_payload_remain   = len_array[idx]%4;

        if(_4byte_payload_remain > 0)
            _4byte_payload_len +=1;

        fprintf(Hfile_ptr,"{");

        temp_val = payload_seed_val;

        for(payload_idx = 0; payload_idx < 375; payload_idx++)
        {
            if(payload_idx < _4byte_payload_len)
            {
                if(payload_idx == _4byte_payload_len - 1)
                {
                    if(_4byte_payload_remain == 0)
                        temp_val = temp_val;
                    else if(_4byte_payload_remain == 1) 
                        temp_val = temp_val&0x000F;
                    else if(_4byte_payload_remain == 2) 
                        temp_val = temp_val&0x00FF;
                    else if(_4byte_payload_remain == 3) 
                        temp_val = temp_val&0x0FFF;

                    if(payload_idx == 374)
                        fprintf(Hfile_ptr,"0x%X },\r\n\t",temp_val);
                    else
                    {
                        if( (payload_idx != 0) && (payload_idx % 10 == 0) )
                            fprintf(Hfile_ptr,"\r\n\t");

                        fprintf(Hfile_ptr,"0x%X, ",temp_val);
                    }
                }
                else
                {
                    if( (payload_idx != 0) && (payload_idx % 10 == 0) )
                        fprintf(Hfile_ptr,"\r\n\t");

                    fprintf(Hfile_ptr,"0x%X, ",temp_val);
                    temp_val = (payload_seed_val + temp_val)&SEDD_VAL_MASK;
                }
            }
            else
            {
                if( (payload_idx != 0) && (payload_idx % 10 == 0) )
                    fprintf(Hfile_ptr,"\r\n\t");

                if(payload_idx == 374)
                    fprintf(Hfile_ptr,"0x%X },\r\n\t",0);
                else
                    fprintf(Hfile_ptr,"0x%X, ",0);
            }

        }
        if(idx == array_len - 1)
            fprintf(Hfile_ptr,"};\r\n\r\n");
    }


    fprintf(Hfile_ptr,"#endif");
    fclose(Hfile_ptr);

    return 0;   
    
}