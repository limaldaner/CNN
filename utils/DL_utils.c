//
//  DL_utils.c
//  DL_SOC
//
//  Created by Liége Maldaner on 2019-12-16.
//  Copyright © 2019 Liége Maldaner. All rights reserved.
//

#include "DL_utils.h"


//
// memory management
//

// parse the input 1D from csv file into memory
//
float* read_data_layer(char* bias, char*  weights, float* layer, int n, int nb){
    
    FILE *csv_bias;
    csv_bias = fopen(bias,"r");  // r for read
    
    if(csv_bias == NULL) {
        printf("Error: can't open biad file \n");
    }
    
    float buffer;
    fscanf(csv_bias, "%*[^\n]\n"); // to read and discard the first line
    int i = 0;
    for (i=0; i< nb; i++) {
        fscanf(csv_bias, "%*f,"); // to read and discard first integer and comma
        fscanf(csv_bias, "%f", &buffer);
        layer[i] = buffer;
    }
    fclose(csv_bias);
    
    FILE *csv_weights;
    csv_weights = fopen(weights,"r");  // r for read
    
    if(csv_weights == NULL) {
        printf("Error: can't open biad file \n");
    }
    fscanf(csv_weights, "%*[^\n]\n"); // to read and discard the first line
    for (int j= (i); j< n; j++) {
        fscanf(csv_weights, "%*f,"); // to read and discard first integer and comma
        fscanf(csv_weights, "%f", &buffer);
        layer[j] = buffer;
    }
    fclose(csv_weights);
    return 0;
}

float* read_data_1d(char* fn, float* data, int img, int img_size){
    FILE *csv_file;
    csv_file = fopen(fn,"r");  // r for read
    
    if(csv_file == NULL) {
        printf("Error: can't open biad file \n");
    }
    
    float buffer;
    fscanf(csv_file, "%*[^\n]\n"); // to read and discard the first line
    int begin_img = img*img_size;
    
    // This first for loop is used to isconsider the images before the desired one
    for (int i=0; i< begin_img; i++) {
        fscanf(csv_file, "%*f,"); // to read and discard first integer and comma
        fscanf(csv_file, "%f", &buffer);
    }
    int cont = 0;
    for (int i=begin_img; i< begin_img+img_size; i++) {
        fscanf(csv_file, "%*f,"); // to read and discard first integer and comma
        fscanf(csv_file, "%f", &buffer);
        data[cont] = buffer;
        cont ++;
    }
    fclose(csv_file);
    return 0;
}
