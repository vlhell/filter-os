#pragma once

struct __attribute__((__packed__)) spixel {
	unsigned char R, G, B;
};
typedef struct spixel pixe_t;

struct simage {
	unsigned int w, h, size;
	pixe_t *data;
};
typedef struct simage imag_t;

imag_t *get_imag(const char *path);
void put_imag(const char *path, imag_t *imag);
