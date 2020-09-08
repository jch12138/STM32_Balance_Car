/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-06     SummerGift   first version
 * 2018-11-19     flybreak     add stm32f407-atk-explorer bsp
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

int p ;
int i ;
int d ;

/* defined the LED0 pin: PF9 */
#define LED0_PIN    GET_PIN(C, 13)

int main(void)
{
    int count = 1;
    /* set LED0 pin mode to output */
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);
    fal_init();
    easyflash_init();
    load_parameter();


    while (count++)
    {
        rt_pin_write(LED0_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED0_PIN, PIN_LOW);
        rt_thread_mdelay(500);
    }

    return RT_EOK;
}

void save_parameters(void)
{
    char value_buf[5];

    rt_sprintf(value_buf,"%d",p);
    ef_set_env("p",value_buf);
    rt_memset(value_buf,0,5);

    rt_sprintf(value_buf,"%d",i);
    ef_set_env("i",value_buf);
    rt_memset(value_buf,0,5);

    rt_sprintf(value_buf,"%d",d);
    ef_set_env("d",value_buf);
    rt_memset(value_buf,0,5);

}

void load_parameter(void)
{
    char value_buf[5];

    ef_get_env_blob("p", value_buf, sizeof(value_buf) , NULL);
    p = atoi(value_buf);
    rt_memset(value_buf,0,5);

    ef_get_env_blob("i", value_buf, sizeof(value_buf) , NULL);
    i = atoi(value_buf);
    rt_memset(value_buf,0,5);

    ef_get_env_blob("d", value_buf, sizeof(value_buf) , NULL);
    d = atoi(value_buf);
    rt_memset(value_buf,0,5);

}

void show_parameter(void)
{
    rt_kprintf("P: %5d, I: %5d, D: %5d\n",p,i,d);
}
MSH_CMD_EXPORT(show_parameter,show parameters);

int set(int argc,char ** argv){
    if(argc != 3){
        return 0;
    }

    int value = atoi(argv[2]);
    if (rt_strcmp(argv[1],"p")==0){
        p=value;
    }
    if (rt_strcmp(argv[1],"i")==0){
        i=value;
    }
    if (rt_strcmp(argv[1],"d")==0){
        d=value;
    }
    save_parameters();

}
MSH_CMD_EXPORT(set,set parameters);
//这部分是变量持久化以及动态调参,可以在控制台通过 set p 100 这种方式进行实时调参,调完的参数可以自动保存在 flash 上.重启不丢失.方便进行调参.