/*
 * File:    task_cmd.h
 * Author:  Alexander130892
 * Date:    6-6-2026
 *
 * Description:
 *   This header file declares the command task function `vCmdTask`,
 *   which is an RTOS task entry point that processes commands within
 *   an embedded system using kernel objects.
 */
#ifndef TASKS_TASK_CMD_H_
#define TASKS_TASK_CMD_H_


#include "rtos/kernel_objects.h"

void vCmdTask(void *pvParameters);

#endif /* TASKS_TASK_CMD_H_ */
