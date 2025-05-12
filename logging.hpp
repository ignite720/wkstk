#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/dist_sink.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace logging {
    inline std::shared_ptr<spdlog::sinks::dist_sink_mt> __logger_sinks;
    inline std::shared_ptr<spdlog::logger> logger;

    inline auto init(
        std::string base_filename = "logs/log.txt",
        int rotation_hour = 0,
        int rotation_minute = 5,
        std::string file_pattern = "[%Y-%m-%d %H:%M:%S.%e][%l] %v",
        std::string console_pattern = "[%H:%M:%S.%e][%L] %v",
        std::chrono::seconds interval = std::chrono::seconds(5)
    ) -> void {
        assert(!__logger_sinks && !logger);

        __logger_sinks = std::make_shared<spdlog::sinks::dist_sink_mt>();
        logger = std::make_shared<spdlog::logger>("logger", __logger_sinks);

        #if !TARGET_PLATFORM_WEB && !TARGET_PLATFORM_UWP
        auto daily_file_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(
            std::move(base_filename),
            rotation_hour,
            rotation_minute
        );
        daily_file_sink->set_level(spdlog::level::trace);
        daily_file_sink->set_pattern(std::move(file_pattern));
        __logger_sinks->add_sink(daily_file_sink);

        auto console_sink = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();
        console_sink->set_level(spdlog::level::debug);
        console_sink->set_pattern(std::move(console_pattern));
        __logger_sinks->add_sink(console_sink);

        logger->flush_on(spdlog::level::info);
        logger->set_level(spdlog::level::trace);
        spdlog::flush_every(interval);
        #endif
    }
}

#define LOG_TRACE(...)                                          logging::logger->trace(__VA_ARGS__)
#define LOG_DEBUG(...)                                          logging::logger->debug(__VA_ARGS__)
#define LOG_INFO(...)                                           logging::logger->info(__VA_ARGS__)
#define LOG_WARN(...)                                           logging::logger->warn(__VA_ARGS__)
#define LOG_ERROR(...)                                          logging::logger->error(__VA_ARGS__)
#define LOG_CRITICAL(...)                                       logging::logger->critical(__VA_ARGS__)