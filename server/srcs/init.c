#include "../incs/main.h"

t_data **init()
{
    t_data **datas;
    
    datas = (t_data **)malloc(sizeof(t_data *) * 5);
    if (datas == NULL)
        printf("malloc error\n");
    for (int i = 0; i < 5; i++)
    {
        t_data *tmp = (t_data *)malloc(sizeof(t_data));
        if (tmp == NULL)
            printf("malloc error\n");
        datas[i] = tmp;
    }
    return (datas);
}

void endThings(t_data **datas)
{
    // 스레드 끝내기
    // 할당해제
}