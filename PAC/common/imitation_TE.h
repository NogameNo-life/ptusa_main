#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <ctime>
#include <numeric>
#include <vector>
#include <memory>

class imitation_TE {
public:
        imitation_TE();
        imitation_TE(float dispersion, float m_expec,float max_TE,float min_TE);
        float get_TE(); // возвращает значения температуры
        void set_max(float max);
        float get_max() const;
        void set_min(float min);
        float get_min() const;
        float get_st_deviation() const;
private:
        float dispersion;     // дисперсия
        float m_expec;       // мат. ожидание
        float st_deviation; // стандартное отклонение
        float max_TE;
        float min_TE;
        float x;  // случайная величина
        std::unique_ptr<float[]> iarr;
        std::unique_ptr<float[]> flarr;

        float get_random();
        bool is_p() const;          // функция расчета вероятности
        unsigned get_index() const;
        void initial_arrays(float min_TE,float max_TE);
};
