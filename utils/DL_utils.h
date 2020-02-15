//
//  DL_utils.h
//  DL_SOC
//
//  Created by Liége Maldaner on 2019-12-16.
//  Copyright © 2019 Liége Maldaner. All rights reserved.
//

#ifndef DL_utils_h
#define DL_utils_h

#include <stdio.h>
#include <stdlib.h> 

float* read_data_layer(char* weights, char*  bias, float* layer, int n, int nb);
float* read_data_1d(char* fn, float* data, int img, int img_size);

#endif /* DL_utils_h */
