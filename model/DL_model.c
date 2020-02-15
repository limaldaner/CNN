//
//  DL_model.c
//  DL_SOC
//
//  Created by Liége Maldaner on 2019-12-16.
//  Copyright © 2019 Liége Maldaner. All rights reserved.
//

#include "DL_model.h"
#include <math.h>

//
// get predictions processing the iamge throught the 3 CNN layers
//
float get_predictions(float* data, float* layer1, float* layer2, float*layer3, float* layerfc, int f1, int f2, int f3, int f4, int f5, int input_image_channels, int img_size)
{
    float maximum = 0;
    int class = 0;
    
    // Layer 1
    int filter = f1;
    int channels = input_image_channels;
    int strides = 2;
    float* output1 = convolution2d(data, layer1, filter, img_size, channels);
    float* relu_out = relu(output1, filter*img_size*img_size);
    float* out_maxpool1 = maxpooling(relu_out, strides, img_size, filter);
    
    // Layer 2
    filter = f2;
    img_size = 12;
    channels = f1;
    float*  output2 = convolution2d(out_maxpool1, layer2, filter, img_size, channels);
    float* relu_out2 = relu(output2, filter*img_size*img_size);
    float* out_maxpool2 = maxpooling(relu_out2, strides, img_size, filter);
    
    // Layer 3
    filter = f3;
    img_size = 6;
    channels = f2;
    float* output3 = convolution2d(out_maxpool2, layer3, filter, img_size, channels);
    float* relu_out3 = relu(output3, filter*img_size*img_size);
    float* out_maxpool3 = maxpooling(relu_out3, strides, img_size, filter);
    
    // Fully connected layer
    float* out_dense = Dense(out_maxpool3, layerfc);
    
    // Predictions: get max value of the dense array
    maximum = out_dense[0];
    for(int c = 1; c < 10; c++){
        if (out_dense[c] > maximum){
            maximum  = out_dense[c];
            class = c;
        }
    }
    printf("Probability of class %d: %f\n", class, maximum);
    return class;
}

//
// Padding of image before convolution
// Each image are contained into an array of one dimension
//
float* image_padding(float* padded, float* img, int rows, int channels)
{
    int size = rows*channels;
    int columns = (size)*rows;
    for(int j=0 ; j < size; j++){
        // top and bottom
        padded[j] = 0;
        padded[j+ (columns-(size))] = 0;
    }
    //complete the rest of the padded image with the real image
    int i = 0;
    for (int y=1; y< rows-1; y++) { // exclude first and last column already filled with zeros. Run for all y columns to fill it
        for (int row = 0; row < channels; row++) { // fill with zeros all channels, at first and last rows
            padded[row+(size)*y] = 0;
            padded[row+((size)*(y+1))-channels] = 0;
        }
        for (int rows1 = 0; rows1 < (rows-2)*channels; rows1++) { // fill the rest 3 channels rows with image
            padded[rows1+channels+((size)*y)] = img[i];
            i++;
        }
    }
    return padded;
}

//
// Convolution between Kernel and Image
// At 1st layer, we have 64 different kernels with shape 3x3x3. And the image have the shape 24x24x3.
// It means that the output image will have the shape 24x24x64.
//
float* convolution2d(float* data, float* layer, int filter, int img_size, int channels)
{
    float* output = malloc((((img_size)*(img_size)*filter)*2) * sizeof(float));
    int newSize = img_size + 2;
    float* padded = malloc((newSize*channels)*newSize * sizeof(float));
    
    // Image must be padded before convolution
    padded  = image_padding(padded, data, newSize, channels);
    
    //concolutional loop: image is multiplied by a kernel with strides
    int cont = 0;
    float mult = 0;
    int size_channel = channels*channels;
    int size_kernel = (size_channel)*channels;
    float* kernel = malloc((size_kernel) * sizeof(float));
    float* image_stride = malloc((size_kernel) * sizeof(float));
    
    for (int ch = 0; ch < filter; ch++) {
        // create kernel 3x3x3 to be multiplied by the image
        for(int j=0; j < 3*3*channels; j++){
            kernel[j] = layer[j*filter+ch+filter];
        }
        
        // mult(padded image, kernel 3x3x3)
        for (int x=0; x < img_size ; x++) { //cols = x
            for (int row=0; row < img_size; row++) { //rows = y
                for (int y=0; y < channels; y++) { // each 3 y
                    for (int k=0; k<3; k++) {
                        for(int j=0; j < channels; j++){ //3 in each y
                            image_stride[cont] = padded[k*channels + j + (y+row)*(newSize*channels) + x*channels];
                            cont++;
                        }
                    }
                }
                cont =0;
                
                // multiplication after we get the image and kernel with the same shape
                for (int c=0; c<size_channel*channels; c++) {
                    mult = kernel[c]*image_stride[c] + mult;
                }
                
                // for this kernel running trought all the image,
                // each stride creates a new pixel, and so we have 24x24 pixels
                output[(row*img_size +(x))*(filter)+ch] = mult + layer[ch];
                mult = 0;
            }
        }
    }
    free(kernel);
    free(image_stride);
    free(padded);
    return output;
}

//
// RELU make negative values 0 and keep positive ones
//
float* relu(float* data, int length){
    float* output = malloc((length) * sizeof(float));
    for (int i=0; i<length; i++) {
        if(data[i]<0){
            output[i]=0;
        }
        else{
            output[i]=data[i];
        }
    }
    return output;
}

//
// MAXPOOLING gets the max value of the kernel being shifted on the image according to the strides
//
float* maxpooling(float* data, int strides, int img_size, int channels){
    int new_size = img_size/strides;
    float* output = malloc((new_size*new_size*channels) * sizeof(float));
    
    // image padding
    int size_pad = img_size + 2;
    float* padded_max = malloc((size_pad*channels)*size_pad * sizeof(float));
    padded_max  = image_padding(padded_max, data, size_pad, channels);

    // select 3x3 pixel from padded image, it will runs for each 64 channels
    float* image_kernel = malloc(3*3 * sizeof(float));
    int cont = 0;
    for (int chan=0 ; chan < channels; chan++) {
        for (int x=0; x < new_size ; x++) { //cols = x
            for (int row=0; row < new_size; row++) { //rows = y
                for (int y=0; y < 3; y++) { //each 64 y channels
                    for(int j=0; j < 3; j++){ //9 in each y
                        image_kernel[cont] = padded_max[(j)*channels + (y)*(size_pad*channels) + row*(strides*size_pad*channels) + x*(strides*channels)+ chan];
                        cont++;
                    }
                }
                cont =0;
                
                //after we get the img with shape 3x3, we get the max value
                float maximum = image_kernel[0];
                for(int c = 1; c < 9; c++){
                    if (image_kernel[c] > maximum)
                    {
                        maximum  = image_kernel[c];
                    }
                }
                output[(row)*(new_size*channels) + x*(channels) + chan] = maximum;
            }
        }
    }
    
    free(padded_max);
    free(image_kernel);
    return output;
}

//
// Dense layer performs the Softmax function and returns 10 probabilistic values, correponding to the probability
// that the tested image fits to each 10 classes
//
float* Dense(float* data1, float* layer){
    // This function performs the last weights and bias dot, and the Softmax opetration
    float* output1 = malloc((10) * sizeof(float));
    float* kernel1 = malloc((180) * sizeof(float));
    int count = 0;
    float mult = 0;
    float sum = 0;
    for (int ch = 0; ch < 10; ch++) {
        for(int j=0; j < 180; j++){
            kernel1[j] = layer[j*10 + ch + 10]; // last filter = bias
        }
        //printf("\n");
        for (int i=0; i<180; i++) {
            mult = data1[i] * kernel1[i] + mult;
            
        }
        output1[count] = exp(mult + layer[ch]);
        sum = output1[count] + sum;
        mult = 0;
        count++;
    }
    
    for (int x=0; x<10; x++) {
        output1[x] = output1[x]/sum;
    }
    
    free(kernel1);
    return output1;
}
