#include <deepstate/DeepState.hpp>
#include "time_series_analyzer.hpp"

// Test initialization with different types of inputs
DEFINE_DEEPSTATE_TEST(TimeSeriesAnalyzerTest_InitializationWithMultipleTypes) {
  uint32_t length = DeepState_UIntInRange(2, 1000); // Fuzz length between 2 and 1000
  std::vector<float> data(length);

  for (auto &value : data) {
    value = DeepState_Float(); // Fuzz each float value
  }

  TimeSeriesAnalyzer analyzer(data);
  ASSERT_EQ(analyzer.getData().size(), length) << "Initialization failed with multiple types.";
};

// Test moving average calculations
DEFINE_DEEPSTATE_TEST(TimeSeriesAnalyzerTest_MovingAverageEdgeCases) {
  uint32_t length = DeepState_UIntInRange(2, 1000);
  uint32_t window = DeepState_UIntInRange(1, length); // Window must be <= length

  std::vector<float> data(length);
  for (auto &value : data) {
    value = DeepState_Float();
  }

  TimeSeriesAnalyzer analyzer(data);
  auto result = analyzer.moving_average(window);

  ASSERT_EQ(result.size(), length - window + 1) << "Incorrect moving average size.";
};

// Test exponential smoothing with various alpha values
DEFINE_DEEPSTATE_TEST(TimeSeriesAnalyzerTest_ExponentialSmoothingVariations) {
  uint32_t length = DeepState_UIntInRange(2, 1000);
  float alpha = DeepState_FloatInRange(0.0, 1.0); // Alpha must be between 0 and 1

  std::vector<float> data(length);
  for (auto &value : data) {
    value = DeepState_Float();
  }

  TimeSeriesAnalyzer analyzer(data);
  auto smoothed = analyzer.exponential_smoothing(alpha);

  ASSERT_EQ(smoothed.size(), length) << "Incorrect smoothing size.";
};

// Test anomaly detection
DEFINE_DEEPSTATE_TEST(TimeSeriesAnalyzerTest_AnomalyDetection) {
  uint32_t length = DeepState_UIntInRange(2, 1000);
  float threshold = DeepState_FloatInRange(0.5, 5.0);

  std::vector<float> data(length);
  for (auto &value : data) {
    value = DeepState_Float();
  }

  TimeSeriesAnalyzer analyzer(data);

  auto z_anomalies = analyzer.detect_anomalies("zscore", threshold);
  ASSERT_EQ(z_anomalies.size(), length) << "Incorrect z-score anomaly size.";

  auto iqr_anomalies = analyzer.detect_anomalies("iqr", threshold);
  ASSERT_EQ(iqr_anomalies.size(), length) << "Incorrect IQR anomaly size.";
};

// Test invalid inputs
DEFINE_DEEPSTATE_TEST(TimeSeriesAnalyzerTest_InvalidInputs) {
  std::vector<float> empty_data;
  try {
    TimeSeriesAnalyzer analyzer(empty_data);
    FAIL() << "Expected exception for empty input.";
  } catch (const std::invalid_argument &e) {
    LOG(INFO) << "Caught expected exception for empty input.";
  }

  uint32_t length = DeepState_UIntInRange(2, 1000);
  std::vector<float> data(length);
  for (auto &value : data) {
    value = DeepState_Float();
  }

  TimeSeriesAnalyzer analyzer(data);
  try {
    analyzer.detect_anomalies("invalid_method", 2.0);
    FAIL() << "Expected exception for invalid anomaly detection method.";
  } catch (const std::invalid_argument &e) {
    LOG(INFO) << "Caught expected exception for invalid anomaly detection method.";
  }
};
