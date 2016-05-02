/*
 * Copyright (c) 2012-2013, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * *    * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of The Linux Foundation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#define LOG_NIDEBUG 0

#include <dlfcn.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"
#include "list.h"
#include "hint-data.h"
#include "power-common.h"

#define LOG_TAG "QCOM PowerHAL"
#include <utils/Log.h>

char scaling_gov_path[4][80] ={
    "sys/devices/system/cpu/cpu0/cpufreq/scaling_governor",
    "sys/devices/system/cpu/cpu1/cpufreq/scaling_governor",
    "sys/devices/system/cpu/cpu2/cpufreq/scaling_governor",
    "sys/devices/system/cpu/cpu3/cpufreq/scaling_governor"
};

static void *qcopt_handle;
static struct list_node active_hint_list_head;

static void *get_qcopt_handle()
{
    char qcopt_lib_path[PATH_MAX] = {0};
    void *handle = NULL;

    dlerror();

    if (property_get("ro.vendor.extension_library", qcopt_lib_path,
                NULL)) {
        handle = dlopen(qcopt_lib_path, RTLD_NOW);
        if (!handle) {
            ALOGE("Unable to open %s: %s\n", qcopt_lib_path,
                    dlerror());
        }
    }

    return handle;
}

static void __attribute__ ((constructor)) initialize(void)
{
    qcopt_handle = get_qcopt_handle();

    if (!qcopt_handle) {
        ALOGE("Failed to get qcopt handle.\n");
    } else {

    }
}

static void __attribute__ ((destructor)) cleanup(void)
{
    if (qcopt_handle) {
        if (dlclose(qcopt_handle))
            ALOGE("Error occurred while closing qc-opt library.");
    }
}

int sysfs_read(char *path, char *s, int num_bytes)
{
    char buf[80];
    int count;
    int ret = 0;
    int fd = open(path, O_RDONLY);

    if (fd < 0) {
        strerror_r(errno, buf, sizeof(buf));
        ALOGE("Error opening %s: %s\n", path, buf);

        return -1;
    }

    if ((count = read(fd, s, num_bytes - 1)) < 0) {
        strerror_r(errno, buf, sizeof(buf));
        ALOGE("Error writing to %s: %s\n", path, buf);

        ret = -1;
    } else {
        s[count] = '\0';
    }

    close(fd);

    return ret;
}

int sysfs_write(char *path, char *s)
{
    char buf[80];
    int len;
    int ret = 0;
    int fd = open(path, O_WRONLY);

    if (fd < 0) {
        strerror_r(errno, buf, sizeof(buf));
        ALOGE("Error opening %s: %s\n", path, buf);
        return -1 ;
    }

    len = write(fd, s, strlen(s));
    if (len < 0) {
        strerror_r(errno, buf, sizeof(buf));
        ALOGE("Error writing to %s: %s\n", path, buf);

        ret = -1;
    }

    close(fd);

    return ret;
}

int get_scaling_governor(char governor[], int size)
{
    if (sysfs_read(SCALING_GOVERNOR_PATH, governor,
                size) == -1) {
        // Can't obtain the scaling governor. Return.
        return -1;
    } else {
        // Strip newline at the end.
        int len = strlen(governor);

        len--;

        while (len >= 0 && (governor[len] == '\n' || governor[len] == '\r'))
            governor[len--] = '\0';
    }

    return 0;
}

int get_scaling_governor_check_cores(char governor[], int size,int core_num)
{
   if (sysfs_read(scaling_gov_path[core_num], governor,
               size) == -1) {
      // Can't obtain the scaling governor. Return.
      return -1;
   }

   // Strip newline at the end.
   int len = strlen(governor);
   len--;
   while (len >= 0 && (governor[len] == '\n' || governor[len] == '\r'))
        governor[len--] = '\0';
   return 0;
}

void interaction(int duration, int num_args, int opt_list[])
{
    static int lock_handle = 0;

    if (duration <= 0 || num_args < 1 || opt_list[0] == 0)
        return;

    if (qcopt_handle) {
    }
}

void perform_hint_action(int hint_id, int resource_values[], int num_resources)
{
    if (qcopt_handle) {
    }
}

void undo_hint_action(int hint_id)
{
    if (qcopt_handle) {
    }
}

/*
 * Used to release initial lock holding
 * two cores online when the display is on
 */
void undo_initial_hint_action()
{
    if (qcopt_handle) {
    }
}
