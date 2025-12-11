#include <cstddef>

template <typename T, typename X1, typename X2, typename F>
void InterpolateMissing(
    size_t n1, const T* _t1, const X1* _x1,
    size_t n2, const T* _t2, const X2* _x2,
    F&& f
) {
    T t, t1 = *_t1, t2 = *_t2;
    X1 x1;
    X2 x2;
    int i1 = 1, i2 = 1;

    for (;;) {
        if (!n1) {
            if (!n2)
                return;
            goto interpolate_x1;
        }
        if (!n2)
            goto interpolate_x2;

        if (t1 < t2) {
            i1 = -1;
interpolate_x2:
            t = t1;
            t1 = *++_t1;
            x1 = *_x1;
            ++_x1;
            --n1;

            x2 = *_x2;
            x2 += (t - t2) * (x2 - _x2[i2]) / (t2 - _t2[i2]);

        } else if (t2 < t1) {
            i2 = -1;
interpolate_x1:
            t = t2;
            t2 = *++_t2;
            x2 = *_x2;
            ++_x2;
            --n2;

            x1 = *_x1;
            x1 += (t - t1) * (x1 - _x1[i1]) / (t1 - _t1[i1]);

        } else {
            t = t1;

            x1 = *_x1;
            if (--n1) {
                ++_x1;
                t1 = *++_t1;
                i1 = -1;
            }

            x2 = *_x2;
            if (--n2) {
                ++_x2;
                t2 = *++_t2;
                i2 = -1;
            }
        }

        f(t, x1, x2);
    }
}

#include <cstdio>

int main(int argc, char** argv) {
    double
        time1[] { 1, 2, 4, 6, 8 },
        time2[] { 1, 3, 5, 7, 8 },
        values1[] { 10, 20, 40, 60, 80 },
        values2[] { 10, 30, 50, 70, 80 };

    InterpolateMissing(
        5, time1, values1,
        5, time2, values2,
        [](double t, double v1, double v2) {
            std::printf("%f %f %f\n", t, v1, v2);
        }
    );
}
