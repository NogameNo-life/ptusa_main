#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <ctime>
#include <numeric>
#include <vector>
#include <memory>

class analog_emulator {
public:
        analog_emulator( float dispersion = 0.238f, float m_expec = 27.f, float min_TE = 20.f, float max_TE = 30.f );
        float get_value(); // возвращает значения температуры
        void set_max(float max_in_range);
        float get_max() const;
        void set_min(float min_in_range);
        float get_min() const;
        float get_st_deviation() const;
private:
        float dispersion;     // дисперсия
        float m_expec;       // мат. ожидание
        float st_deviation; // стандартное отклонение
        float max_TE;
        float min_TE;
        float x;  // случайная величина
        std::unique_ptr<float[]> flptr;
        std::unique_ptr<float[]> iptr;

        float get_random() const;
        bool is_p() const;          // функция расчета вероятности
        unsigned get_index() const;
        void initial_arrays(float min, float max);
};
