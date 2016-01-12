#pragma once
// Global handler to deal with serious engine-wide errors which may arise
class ErrorHandler // Probably worth having a base class which this can inherit from
{
public:
    ErrorHandler();
    ~ErrorHandler();
} GErrorHandler;