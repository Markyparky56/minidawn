#pragma once
// Global handler to deal with serious engine-wide errors which may arise
class ErrorHandler // Probably worth having a base class which this can inherit from, 
{                  // or this is the base class and have platform specific variations
public:
    ErrorHandler();
    ~ErrorHandler();
} GErrorHandler;