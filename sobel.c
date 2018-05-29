#include "sobel.h"
#include "sio.h"
#include "common.h"
#include <math.h>
#include <pthread.h>

#define safe_pthread_create(fi, se, th, fo) \
	assert(pthread_create(fi, se, th, fo) == 0)
#define safe_pthread_join(fi) \
	assert(pthread_join(fi, NULL) == 0)

const char Gx[3][3] = {{-1,  0,  1}, {-2, 0, 2}, {-1, 0, 1} };
const char Gy[3][3] = {{-1, -2, -1}, { 0, 0, 0}, { 1, 2, 1} };

struct s_thr_data {
	unsigned int cnt, num;
	imag_t *imag;
	pixe_t *data;
};
typedef struct s_thr_data thr_t;

void *sobel_thr(void *arg)
{
	thr_t *thd = (thr_t *)arg;
	const int w = thd->imag->w;
	const float h = thd->imag->h / (float)thd->cnt;
	const int start_y = thd->num ? h * thd->num : 1;
	int end_y;

	if (thd->num + 1 < thd->cnt)
		end_y = h * (thd->num + 1);
	else
		end_y = thd->imag->h - 1;

	for (int y = start_y; y < end_y; ++y) {
		pixe_t *row = thd->imag->data + (w * y);

		for (int x = 1; x < w - 1; ++x) {
			int gx = 0, gy = 0;
			pixe_t *px;

			for (int i = 0; i < 3; ++i)
				for (int j = 0; j < 3; ++j) {
					px = thd->data + (x - 1 + i +
						w * (y - 1 + j));

					int d = (px->R + px->G + px->B) / 3;

					gx += Gx[i][j] * d;
					gy += Gy[i][j] * d;
				}

			px = row + x;
			px->R = px->G = px->B =
				MIN(sqrt(gx * gx + gy * gy), 255);
		}
	}
	pthread_exit(0);
}


void sproc(const char *path_i, const char *path_o, int th_num)
{
	imag_t *imag = get_imag(path_i);
	pixe_t *data = imag->data;

	imag->data = safe_calloc(imag->size, 1);

	if (th_num * 3 > imag->h)
		th_num = MAX(imag->h / 3.0, 1);

	thr_t *thr_arr = safe_calloc(th_num, sizeof(thr_t));
	pthread_t *thrs = safe_calloc(th_num, sizeof(pthread_t));

	timespec_t timer;

	safe_push_timer(&timer);
	for (int i = 0; i < th_num; i++) {
		//arr[i]->cnt = th_num;
		//arr[i]->num = i;
		thr_arr[i] = (thr_t){th_num, i, imag, data};
		safe_pthread_create(thrs + i, NULL, sobel_thr, thr_arr + i);
	}

	for (int i = 0; i < th_num; i++)
		safe_pthread_join(thrs[i]);

	printf("total proc. time = %lf\n", safe_pop_timer(&timer));
	free(thr_arr);
	free(thrs);

	put_imag(path_o, imag);
	free(data);
	free(imag->data);
	free(imag);
}





