//
//  DL_model.h
//  DL_SOC
//
//  Created by Liége Maldaner on 2019-12-16.
//  Copyright © 2019 Liége Maldaner. All rights reserved.
//

#ifndef DL_model_h
#define DL_model_h

#include "../utils/DL_utils.h"


float get_predictions(float* data, float* layer1, float* layer2, float*layer3, float* layerfc, int f1, int f2, int f3, int f4, int f5, int input_image_channels, int img_size);
float* convolution2d(float* data, float* layer, int filter, int img_size, int channels);//, float***** layer, int strides, int filter, int input_kernel);
float* relu(float* data, int length);
float* maxpooling(float* data, int strides, int img_size, int channels);
float* Dense(float* data, float* layer);

#endif /* DL_model_h */
