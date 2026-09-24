#pragma once

class RAM {
    float total;
    float cache;
    float available;
    float swap_total;
    float swap_used;

public:
    RAM();

    float getTotal() const;
    void setTotal(float value);

    float getCache() const;
    void setCache(float value);

    float getAvailable() const;
    void setAvailable(float value);

    float getSwapTotal() const;
    void setSwapTotal(float value);

    float getSwapUsed() const;
    void setSwapUsed(float value);
};