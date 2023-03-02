#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include<numeric>
#include <ctime>
class imitation_TE
    {
    public:
        imitation_TE() = default;
        imitation_TE(float D,float m_expec);
        float get_TE(); // возвращает значения температуры
        void set_max(float max);
        float get_max() const;
        void set_min(float min);
        float get_min() const;
        float get_st_deviation() const;

    private:
        float D = 0.234f;   // дисперсия
        float m_expec = 27.f; // мат. ожидание
        float st_deviation = 0.48f; // стандартное отклонение
        float old_value = 25.7f;
        float x = get_random(); // случайная величина
        float max = 30.f;
        float min = 20.f;

        float get_random();
        bool is_p() const;   // функция расчета вероятности
        unsigned get_index() const;
    };


