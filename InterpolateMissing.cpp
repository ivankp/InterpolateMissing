#include <cmath>

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
goto interpolate_x2:
            t = t1;
            t1 = *++_t1;
            x1 = *_x1;
            ++_x1;
            --n1;

            x2 = *_x2 + flip_if_outside(t2 - t) * (x2 - _x2[i2]) / (t2 - _t2[i2]);

        } else if (t2 < t1) {
            i2 = -1;
goto interpolate_x1:
            t = t2;
            t2 = *++_t2;
            x2 = *_x2;
            ++_x2;
            --n2;

            x1 = *_x1 + flip_if_outside(t1 - t) * (x1 - _x1[i1]) / (t1 - _t1[i1]);

        } else {
            t = t1;

            t1 = *++_t1;
            x1 = *_x1;
            ++_x1;
            --n1;
            i1 = -1;

            t2 = *++_t2;
            x2 = *_x2;
            ++_x2;
            --n2;
            i2 = -1;
        }

        f(t, x1, x2);
    }
}
