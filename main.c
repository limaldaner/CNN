//
//  main.c
//  DL_SOC
//
//  Created by Liége Maldaner on 2019-12-16.
//  Copyright © 2019 Liége Maldaner. All rights reserved.
//

#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils/DL_utils.h"
#include "model/DL_model.h"

int main()
{
    // load CIFAR-10 test dataset images cutte and normilized, params (wights and bias)
    int length = 24;
    int img_size = length * length * 3;
    
    // This variable is used to choose the image to be processed
    int img = 0;
    
    // allocate space in memory to load csv file
    float* classes = calloc(1, sizeof(float));
    read_data_1d("/Users/liegemaldaner/Downloads/C/DL_SOC/DL_SOC/files/classes.csv", classes, img, 1);
    printf("Real class: %f\n", classes[0]);
    
    float* data = calloc(img_size, sizeof(float));
    read_data_1d("/Users/liegemaldaner/Downloads/C/DL_SOC/DL_SOC/files/test_img.csv", data, img, img_size);
    
    // read data and classes into memory
    // allocate space in memory and load csv layers params (weight and bias)
    int kernel = 3;
    // number of bias in each layer
    int f1 = 64, f2 = 32, f3 = 20, f4 = 180, f5 = 10, input_image_channels=3;
    
    // load layers params (weight and bias) from csv to memory
    int l1 = f1*((kernel*kernel*input_image_channels)+1);
    float* layer1 = calloc(l1, sizeof(float));
    read_data_layer( "/Users/liegemaldaner/Downloads/C/DL_SOC/DL_SOC/files/bias1.csv", "/Users/liegemaldaner/Downloads/C/DL_SOC/DL_SOC/files/weight1.csv", layer1, l1, f1);
    int l2 = f2*((kernel*kernel*f1)+1);
    float* layer2 = calloc(l2, sizeof(float));
    read_data_layer( "/Users/liegemaldaner/Downloads/C/DL_SOC/DL_SOC/files/bias2.csv", "/Users/liegemaldaner/Downloads/C/DL_SOC/DL_SOC/files/weight2.csv", layer2, l2, f2);
    int l3 = f3*((kernel*kernel*f2)+1);
    float* layer3 = calloc(l3, sizeof(float));
    read_data_layer( "/Users/liegemaldaner/Downloads/C/DL_SOC/DL_SOC/files/bias3.csv", "/Users/liegemaldaner/Downloads/C/DL_SOC/DL_SOC/files/weight3.csv", layer3, l3, f3);
    int lfc = f5*((f4)+1);
    float* layerfc = calloc(lfc, sizeof(float));
    read_data_layer( "/Users/liegemaldaner/Downloads/C/DL_SOC/DL_SOC/files/biasfc.csv", "/Users/liegemaldaner/Downloads/C/DL_SOC/DL_SOC/files/weightfc.csv", layerfc, lfc, f5);

    //
    // Teting process
    //
    // start clock for timing
    clock_t begin = clock();
    
    int predicted_class = get_predictions(data,layer1,layer2,layer3,layerfc, f1, f2, f3, f4, f5, input_image_channels, length);
    
    // Validate
    //Compare to the real class: if equal > accuracy = 1, otherwise it is 0
    int accuracy = 0;
    if (predicted_class == classes[0]) {
        accuracy = 1;
    }
    printf("Accuracy = %d\n", accuracy);

    // end clock for timing
    clock_t end = clock();
    printf("\ntime taken: %fs\n",(double)(end - begin) / CLOCKS_PER_SEC);
    
    //
    // free any 1D array
    //
    free(data);
    free(layer1);
    free(layer2);
    free(layer3);
    free(layerfc);
    free(classes);
    
    return 0;
}
