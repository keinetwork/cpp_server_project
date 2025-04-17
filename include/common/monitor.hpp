#pragma once

#include <chrono>
#include <string>
#include <map>
#include <mutex>
#include <atomic>

class PerformanceMonitor {
public:
    static PerformanceMonitor& getInstance();
    
    void startMeasurement(const std::string& name);
    void endMeasurement(const std::string& name);
    void incrementCounter(const std::string& name);
    void setGauge(const std::string& name, double value);
    
    std::map<std::string, double> getMetrics() const;
    void resetMetrics();
    
private:
    PerformanceMonitor();
    ~PerformanceMonitor();
    PerformanceMonitor(const PerformanceMonitor&) = delete;
    PerformanceMonitor& operator=(const PerformanceMonitor&) = delete;
    
    struct Measurement {
        std::chrono::high_resolution_clock::time_point start_time;
        double total_time;
        uint64_t count;
    };
    
    mutable std::mutex mutex_;
    std::map<std::string, Measurement> measurements_;
    std::map<std::string, std::atomic<uint64_t>> counters_;
    std::map<std::string, std::atomic<double>> gauges_;
}; 