#include "common/monitor.hpp"
#include "common/logger.hpp"
#include <iomanip>
#include <sstream>

PerformanceMonitor& PerformanceMonitor::getInstance() {
    static PerformanceMonitor instance;
    return instance;
}

PerformanceMonitor::PerformanceMonitor() = default;
PerformanceMonitor::~PerformanceMonitor() = default;

void PerformanceMonitor::startMeasurement(const std::string& name) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto& measurement = measurements_[name];
    measurement.start_time = std::chrono::high_resolution_clock::now();
}

void PerformanceMonitor::endMeasurement(const std::string& name) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = measurements_.find(name);
    if (it != measurements_.end()) {
        auto& measurement = it->second;
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
            end_time - measurement.start_time).count();
        measurement.total_time += duration;
        measurement.count++;
    }
}

void PerformanceMonitor::incrementCounter(const std::string& name) {
    counters_[name]++;
}

void PerformanceMonitor::setGauge(const std::string& name, double value) {
    gauges_[name] = value;
}

std::map<std::string, double> PerformanceMonitor::getMetrics() const {
    std::map<std::string, double> metrics;
    std::lock_guard<std::mutex> lock(mutex_);
    
    // 측정 시간 평균 계산
    for (const auto& [name, measurement] : measurements_) {
        if (measurement.count > 0) {
            metrics[name + "_avg_time"] = measurement.total_time / measurement.count;
        }
        metrics[name + "_total_count"] = measurement.count;
    }
    
    // 카운터 값 추가
    for (const auto& [name, counter] : counters_) {
        metrics[name] = counter.load();
    }
    
    // 게이지 값 추가
    for (const auto& [name, gauge] : gauges_) {
        metrics[name] = gauge.load();
    }
    
    return metrics;
}

void PerformanceMonitor::resetMetrics() {
    std::lock_guard<std::mutex> lock(mutex_);
    
    for (auto& [name, measurement] : measurements_) {
        measurement.total_time = 0;
        measurement.count = 0;
    }
    
    for (auto& [name, counter] : counters_) {
        counter = 0;
    }
    
    for (auto& [name, gauge] : gauges_) {
        gauge = 0;
    }
    
    Logger::info("성능 메트릭이 초기화되었습니다");
} 