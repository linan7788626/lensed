#pragma once

// pixel coordinate system
typedef struct
{
    long rx;    // x reference pixel
    long ry;    // y reference pixel
    double sx;  // x pixel scale
    double sy;  // y pixel scale
} pcsdata;

// read image from file
void read_image(const char* filename, size_t* width, size_t* height, cl_float** image);

// read pixel coordinate system from image file
void read_pcs(const char* filename, pcsdata* pcs);

// read gain from file
void read_gain(const char* filename, size_t width, size_t height, double** gain);

// uniform gain
void make_gain(double value, size_t width, size_t height, double** gain);

// read weights from file
void read_weight(const char* filename, size_t width, size_t height, cl_float** weight);

// generate weights from image, gain and offset
void make_weight(const cl_float* image, const double* gain, double offset, size_t width, size_t height, cl_float** weight);

// read extra weights from file
void read_xweight(const char* filename, size_t width, size_t height, double** weight);

// read mask from file
void read_mask(const char* maskname, const char* imagename, const pcsdata* pcs, size_t width, size_t height, int** mask);

// read PSF from file
void read_psf(const char* filename, size_t* width, size_t* height, cl_float** psf);

// write output to FITS file
void write_output(const char* filename, size_t width, size_t height, size_t noutput, cl_float* output[], const char* names[]);

// write output to in-memory FITS
size_t write_memory(void** mem, size_t width, size_t height, size_t noutput, cl_float* output[], const char* names[]);

// find most common value
void find_mode(size_t nvalues, const cl_float values[], const cl_float mask[], double* mode, double* fwhm);
