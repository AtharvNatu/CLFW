__kernel void vec_add_gpu(__global float *in1, __global float* in2, __global float *out, int len) 
{ 
    int i = get_global_id(0); 
    if (i < len) 
    { 
        out[i] = in1[i] + in2[i]; 
    } 
};
