#pragma once
#include <fstream>
#include <iostream>
#include <string>

#include "NvInfer.h"

// Logger for TensorRT info/warning/errors
class Logger : public nvinfer1::ILogger
{
    std::string log_file = "tensorrt.log";
    std::fstream fs;

    void log_with(std::ostream &os, const char *prefix, const char *msg) const
    {
        os << prefix << msg << std::endl;
    }

  public:
    Logger() : Logger(Severity::kINFO) {}

    Logger(Severity severity)
        : reportableSeverity(severity), fs(log_file, std::ios::out)
    {
    }

    void log(Severity severity, const char *msg) override
    {
        // suppress messages with severity enum value greater than the
        // reportable
        if (severity > reportableSeverity) { return; }

        switch (severity) {
        case Severity::kINTERNAL_ERROR:
            log_with(fs, "INTERNAL_ERROR: ", msg);
            log_with(std::cerr, "INTERNAL_ERROR: ", msg);
            break;
        case Severity::kERROR:
            log_with(fs, "ERROR: ", msg);
            log_with(std::cerr, "ERROR: ", msg);
            break;
        case Severity::kWARNING:
            log_with(fs, "WARNING: ", msg);
            break;
        case Severity::kINFO:
            log_with(fs, "INFO: ", msg);
            break;
        default:
            log_with(fs, "UNKNOWN: ", msg);
            break;
        }
    }

    Severity reportableSeverity{Severity::kWARNING};
};
