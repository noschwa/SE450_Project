#ifndef TIME_SERIES_ANALYZER_HPP
#define TIME_SERIES_ANALYZER_HPP

#include <vector>
#include <stdexcept>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <iostream>

class TimeSeriesAnalyzer {
public:
  
 public:
    const std::vector<float>& get_data() const {
        return data;
    }

 explicit TimeSeriesAnalyzer(const std::vector<float>& data) {
        if (data.empty()) {
            throw std::invalid_argument("Input data cannot be empty");
        }
        this->data = data;
    }

    std::vector<float> moving_average(size_t window) {
        if (window <= 0) {
            throw std::invalid_argument("Window size must be positive");
        }
        if (window > data.size()) {
            throw std::invalid_argument("Window size cannot exceed data length");
        }

        std::vector<float> result(data.size() - window + 1);
        float sum = std::accumulate(data.begin(), data.begin() + window, 0.0f);
        result[0] = sum / window;

        for (size_t i = 1; i < result.size(); ++i) {
            sum += data[i + window - 1] - data[i - 1];
            result[i] = sum / window;
        }
        return result;
    }

    std::vector<float> exponential_smoothing(float alpha) {
        if (alpha < 0.0f || alpha > 1.0f) {
            throw std::invalid_argument("Alpha must be between 0 and 1");
        }

        std::vector<float> smoothed(data.size());
        smoothed[0] = data[0];

        for (size_t t = 1; t < data.size(); ++t) {
            smoothed[t] = alpha * data[t] + (1 - alpha) * smoothed[t - 1];
        }
        return smoothed;
    }

    std::vector<bool> detect_anomalies(const std::string& method = "zscore", float threshold = 3.0) {
        if (method == "zscore") {
            float mean = std::accumulate(data.begin(), data.end(), 0.0f) / data.size();
            float sq_sum = std::inner_product(data.begin(), data.end(), data.begin(), 0.0f);
            float std_dev = std::sqrt(sq_sum / data.size() - mean * mean);

            std::vector<bool> anomalies(data.size());
            for (size_t i = 0; i < data.size(); ++i) {
                anomalies[i] = std::abs(data[i] - mean) / std_dev > threshold;
            }
            return anomalies;

        } else if (method == "iqr") {
            std::vector<float> sorted_data = data;
            std::sort(sorted_data.begin(), sorted_data.end());

            size_t q1_index = sorted_data.size() / 4;
            size_t q3_index = 3 * sorted_data.size() / 4;
            float Q1 = sorted_data[q1_index];
            float Q3 = sorted_data[q3_index];
            float IQR = Q3 - Q1;

            float lower_bound = Q1 - threshold * IQR;
            float upper_bound = Q3 + threshold * IQR;

            std::vector<bool> anomalies(data.size());
            for (size_t i = 0; i < data.size(); ++i) {
                anomalies[i] = data[i] < lower_bound || data[i] > upper_bound;
            }
            return anomalies;

        } else {
            throw std::invalid_argument("Invalid anomaly detection method");
        }
    }

private:
    std::vector<float> data;
};

#endif // TIME_SERIES_ANALYZER_HPP
