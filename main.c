#include <stdio.h>
#include <stdlib.h>
#include <string.h> // 引入 memset
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "imlib.h"
#include "collections.h"
#include <setjmp.h>
jmp_buf nlr; 

int main() {
    printf("--- RV1106 OpenMV 最小测试启动 ---\n");
    int width, height, channels;
    unsigned char *img_data = stbi_load("test.jpg", &width, &height, &channels, 1);
    if (!img_data) {
        printf("读取图片失败！请检查 test.jpg 是否存在。\n");
        return 0; 
    }
    printf("成功加载图片！宽度: %d, 高度: %d\n", width, height);
    
    // 【关键修复 1】：用 {0} 把结构体里的垃圾数据彻底清零！
    image_t img = {0}; 
    
    // 【关键修复 2】：使用新版 OpenMV 的像素枚举 PIXFORMAT_GRAYSCALE
    img.w = width; 
    img.h = height; 
    img.bpp = 1; 
    img.pixfmt = PIXFORMAT_GRAYSCALE; // 告诉算法：我是正宗的灰度图！
    img.data = img_data;
    
    list_t out_list;
    list_init(&out_list, sizeof(find_circles_list_lnk_data_t));
    rectangle_t roi = {0, 0, width, height};
    
    // 阈值保持 2000，原版算法在纯白背景找黑圈，梯度极高，2000很容易达到！
    int threshold = 4000;  
    int r_min = 15;      
    int r_max = 80;     
    
    printf("开始找圆... (阈值:%d, 半径:%d-%d)\n", threshold, r_min, r_max);
    // 这里把 x_stride 和 y_stride 还原成 2，提高精度
    imlib_find_circles(&out_list, &img, &roi, 2, 2, threshold, 15, 15, 15, r_min, r_max, 2);
    
    printf("运算结束！共找到 %zu 个圆。\n", list_size(&out_list));
    
    int i = 1;
    while(list_size(&out_list) > 0) {
        find_circles_list_lnk_data_t c;
        list_pop_front(&out_list, &c);
        printf(" -> 识别到图形 %d: 圆心坐标(X:%d, Y:%d), 半径 r=%d\n", i++, c.p.x, c.p.y, c.r);
    }
    
    free(img_data);
    return 0;
}
