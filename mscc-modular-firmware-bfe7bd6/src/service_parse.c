// Copyright (c) 2016 Microsemi Corporation "Microsemi".

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

#include "service.h"

int attr_validate_appearence(const AttrConf_t *conf, struct Attr *attr)
{
    struct Attr *elt;
    int cnt;
    DL_COUNT(attr, elt, cnt);
    switch (conf[attr->attr_type].appearence) {
        case ATTR_REQUIRED: {
            if (cnt != 1) {
                printf("%s appeared %d time(s), expect %d!\n",
                       conf[attr->attr_type].key_name,
                       cnt, 1);
                return 0;
            }
            break;
        }
        case ATTR_OPTIONAL: {
            if (cnt > 1) {
                printf("%s appeared %d time(s), expect 0 or 1!\n",
                       conf[attr->attr_type].key_name,
                       cnt);
                return 0;
            }
            break;
        }
        case ATTR_DEFAULT: {
            if (cnt == 0) {
                printf("%s not defined, use default value instead!\n",
                       conf[attr->attr_type].key_name);
                DL_APPEND(attr, conf[attr->attr_type].create(conf[attr->attr_type].default_value));
            } else if (cnt == 1) {
                // do nothing
            } else {
                printf("%s appeared %d time(s), expect 0 or 1!\n",
                       conf[attr->attr_type].key_name,
                       cnt);
                return 0;
            }
            break;
        }
        case ATTR_SEQUENCE: {
            if (cnt < 0) {
                printf("%s appeared %d times(s), expect >= 0!\n",
                       conf[attr->attr_type].key_name,
                       cnt);
                return 0;
            }
            break;
        }
    }
    return 1;
}

int service_validate(const AttrConf_t *conf, Service_t *service)
{
    int i, rc = 1;

    for (i = 0; i < SERVICE_CNT; i++) {
        if (service->attr[i] &&
            attr_validate_appearence(conf, service->attr[i]) == 0) {
            rc = 0;
            break;
        }
    }
    return rc;
}

int config_parse(char *data,
                 size_t data_len,
                 const AttrConf_t *conf,
                 int conf_size,
                 struct Attr **result)
{
    char *key, *val;
    struct Attr *attr;

    while (data_len) {
        size_t l = attr_get_line(data, data_len);
        if (attr_split_into_key_val(data, &key, &val)) {
            attr = attr_construct(conf, conf_size, key, val);
            if (attr) {
                DL_APPEND(result[attr->attr_type], attr);
            }
        }
        data += l;
        data_len -= l;
    }
    return 1;
}
