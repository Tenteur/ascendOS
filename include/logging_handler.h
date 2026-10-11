//
// Created by vianney on 08/10/2026.
//

#ifndef ASCENDOS_LOGGING_HANDLER_H
#define ASCENDOS_LOGGING_HANDLER_H

#include <stdbool.h>

/**
 * @file logging_handler.h
 * @brief Centralized, configurable logging for the whole program.
 * Each log handle has a unique ID and a category, and carries its own
 * visibility flag and ANSI text style.
 * @todo Maybe add settings based logging that are smart based on different situations.
 */

/** @brief Severity of a log handle, in increasing order. */
enum logTypeHandle {
    LOGH_INFO,      /**< General information. */
    LOGH_WARNING,   /**< Something unexpected, but recoverable. */
    LOGH_ERROR,     /**< An operation failed. */
    LOGH_CRITICAL   /**< The program cannot continue safely. */
};

/**
 * @brief Used to store all the log handles.
 */
struct logHandleList {
    unsigned int logHandleID;
    enum logTypeHandle logCategory;
    bool isShown;
    char textProperties[11];
    struct logHandleList *next;
};



/**
 * @brief Init all the variables needed for the logging handler to properly work.
 * @pre Should be run as the first function from the logging_handler.
 * @warning Make sure it was not already launched, otherwise, can have undefined consequences.
 * @return 0 if success, 1 if failed.
 */
bool initLogging_handler(void);

/**
 * @brief Registers a new log handle.
 *
 * New handles are shown by default and use no text styling.
 *
 * @param[in] logHandleID Program-wide unique ID of the handle.
 * @param[in] logCategory Severity of the handle.
 * @warning Handles can't be removed; they live until the program exits. If the logHandleID already exists, the handle will not be added and will fail silently.
 */
void addLogHandle(unsigned int logHandleID, enum logTypeHandle logCategory);

/**
* @brief Sets the ANSI escape sequence used to print a handle's logs.
* @param[in] logHandleID ID of an existing handle.
* @param[in] newTextProperties NUL-terminated sequence of at most 9 characters, copied into the handle (e.g. "\033[1;31m"). Replaces the previous one.
*/
void changeLogTextProperties(unsigned int logHandleID, char *newTextProperties);

/**
 * @brief The primary function used to log text to the STDOUT/STDERR.
 * @param[in] logHandleID ID of an existing handle.
 * @param[in] stringToLog A pointer to the string to log.
 */
void writeNewLog(unsigned int logHandleID, char *stringToLog);


#endif //ASCENDOS_LOGGING_HANDLER_H
