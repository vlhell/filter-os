#include "sio.h"
#include "common.h"

imag_t *get_imag(const char *path)
{
	FILE *fd = fopen(path, "r");
	char format[3];

	fscanf(fd, "%s", format);

	assert(strncmp(format, "P6", 2) == 0);

	imag_t *imag = safe_calloc(sizeof(imag_t), 1);

	fscanf(fd, "%u", &imag->w);
	fscanf(fd, "%u", &imag->h);

	int temp;

	fscanf(fd, "%d", &temp);
	assert(temp == 255);

	imag->size = imag->w * imag->h * sizeof(pixe_t);

	imag->data = safe_calloc(imag->size, 1);

	fread(imag->data, 1, imag->size, fd);

	fclose(fd);
	return imag;
}

void put_imag(const char *path, imag_t *imag)
{
	FILE *fd = fopen(path, "w");

	fprintf(fd, "P6\n%u %u\n255\n", imag->w, imag->h);

	fwrite(imag->data, 1, imag->size, fd);

	fclose(fd);
}

