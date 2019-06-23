
/*
 * Copyright (C) YoungJoo Kim (vozlt)
 */


#ifndef _NGX_HTTP_VTS_MODULE_H_INCLUDED_
#define _NGX_HTTP_VTS_MODULE_H_INCLUDED_


#include <nginx.h>
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

#include "ngx_http_vhost_traffic_status_string.h"
#include "ngx_http_vhost_traffic_status_node.h"

#define NGX_HTTP_VHOST_TRAFFIC_STATUS_UPSTREAM_NO          0
#define NGX_HTTP_VHOST_TRAFFIC_STATUS_UPSTREAM_UA          1
#define NGX_HTTP_VHOST_TRAFFIC_STATUS_UPSTREAM_UG          2
#define NGX_HTTP_VHOST_TRAFFIC_STATUS_UPSTREAM_CC          3
#define NGX_HTTP_VHOST_TRAFFIC_STATUS_UPSTREAM_FG          4

#define NGX_HTTP_VHOST_TRAFFIC_STATUS_UPSTREAMS            (u_char *) "NO\0UA\0UG\0CC\0FG\0"

#define NGX_HTTP_VHOST_TRAFFIC_STATUS_NODE_NONE            0
#define NGX_HTTP_VHOST_TRAFFIC_STATUS_NODE_FIND            1

#define NGX_HTTP_VHOST_TRAFFIC_STATUS_KEY_SEPARATOR        (u_char) 0x1f

#define NGX_HTTP_VHOST_TRAFFIC_STATUS_FORMAT_NONE          0
#define NGX_HTTP_VHOST_TRAFFIC_STATUS_FORMAT_JSON          1
#define NGX_HTTP_VHOST_TRAFFIC_STATUS_FORMAT_HTML          2
#define NGX_HTTP_VHOST_TRAFFIC_STATUS_FORMAT_JSONP         3
#define NGX_HTTP_VHOST_TRAFFIC_STATUS_FORMAT_PROMETHEUS    4

#define NGX_HTTP_VHOST_TRAFFIC_STATUS_AVERAGE_METHOD_AMM   0
#define NGX_HTTP_VHOST_TRAFFIC_STATUS_AVERAGE_METHOD_WMA   1

#define NGX_HTTP_VHOST_TRAFFIC_STATUS_DEFAULT_SHM_NAME     "ngx_http_vhost_traffic_status"
#define NGX_HTTP_VHOST_TRAFFIC_STATUS_DEFAULT_SHM_SIZE     0xfffff
#define NGX_HTTP_VHOST_TRAFFIC_STATUS_DEFAULT_JSONP        "ngx_http_vhost_traffic_status_jsonp_callback"
#define NGX_HTTP_VHOST_TRAFFIC_STATUS_DEFAULT_SUM_KEY      "*"
#define NGX_HTTP_VHOST_TRAFFIC_STATUS_DEFAULT_AVG_PERIOD   60
#define NGX_HTTP_VHOST_TRAFFIC_STATUS_DEFAULT_DUMP_PERIOD  60

#define ngx_http_vhost_traffic_status_add_rc(s, n) {                           \
    if(s < 200) {n->stat_1xx_counter++;                                        \
        if(s == 100) {n->stat_100_counter++;}                                  \
        else if(s == 101) {n->stat_101_counter++;}                             \
        else if(s == 102) {n->stat_102_counter++;}}                            \
    else if(s < 300) {n->stat_2xx_counter++;                                   \
        if(s == 200) {n->stat_200_counter++;}                                  \
        else if(s == 201) {n->stat_201_counter++;}                             \
        else if(s == 202) {n->stat_202_counter++;}                             \
        else if(s == 203) {n->stat_203_counter++;}                             \
        else if(s == 204) {n->stat_204_counter++;}                             \
        else if(s == 205) {n->stat_205_counter++;}                             \
        else if(s == 206) {n->stat_206_counter++;}                             \
        else if(s == 207) {n->stat_207_counter++;}                             \
        else if(s == 208) {n->stat_208_counter++;}                             \
        else if(s == 226) {n->stat_226_counter++;}}                            \
    else if(s < 400) {n->stat_3xx_counter++;                                   \
        if(s == 300) {n->stat_300_counter++;}                                  \
        else if(s == 301) {n->stat_301_counter++;}                             \
        else if(s == 302) {n->stat_302_counter++;}                             \
        else if(s == 303) {n->stat_303_counter++;}                             \
        else if(s == 304) {n->stat_304_counter++;}                             \
        else if(s == 305) {n->stat_305_counter++;}                             \
        else if(s == 307) {n->stat_307_counter++;}                             \
        else if(s == 308) {n->stat_308_counter++;}}                            \
    else if(s < 500) {n->stat_4xx_counter++;                                   \
        if(s == 400) {n->stat_400_counter++;}                                  \
        else if(s == 401) {n->stat_401_counter++;}                             \
        else if(s == 402) {n->stat_402_counter++;}                             \
        else if(s == 403) {n->stat_403_counter++;}                             \
        else if(s == 404) {n->stat_404_counter++;}                             \
        else if(s == 405) {n->stat_405_counter++;}                             \
        else if(s == 406) {n->stat_406_counter++;}                             \
        else if(s == 407) {n->stat_407_counter++;}                             \
        else if(s == 408) {n->stat_408_counter++;}                             \
        else if(s == 409) {n->stat_409_counter++;}                             \
        else if(s == 410) {n->stat_410_counter++;}                             \
        else if(s == 411) {n->stat_411_counter++;}                             \
        else if(s == 412) {n->stat_412_counter++;}                             \
        else if(s == 413) {n->stat_413_counter++;}                             \
        else if(s == 414) {n->stat_414_counter++;}                             \
        else if(s == 415) {n->stat_415_counter++;}                             \
        else if(s == 416) {n->stat_416_counter++;}                             \
        else if(s == 417) {n->stat_417_counter++;}                             \
        else if(s == 418) {n->stat_418_counter++;}                             \
        else if(s == 421) {n->stat_421_counter++;}                             \
        else if(s == 422) {n->stat_422_counter++;}                             \
        else if(s == 423) {n->stat_423_counter++;}                             \
        else if(s == 424) {n->stat_424_counter++;}                             \
        else if(s == 426) {n->stat_426_counter++;}                             \
        else if(s == 428) {n->stat_428_counter++;}                             \
        else if(s == 429) {n->stat_429_counter++;}                             \
        else if(s == 431) {n->stat_431_counter++;}                             \
        else if(s == 444) {n->stat_444_counter++;}                             \
        else if(s == 451) {n->stat_451_counter++;}                             \
        else if(s == 499) {n->stat_499_counter++;}}                            \
    else {n->stat_5xx_counter++;                                               \
        if(s == 500) {n->stat_500_counter++;}                                  \
        else if(s == 501) {n->stat_501_counter++;}                             \
        else if(s == 502) {n->stat_502_counter++;}                             \
        else if(s == 503) {n->stat_503_counter++;}                             \
        else if(s == 504) {n->stat_504_counter++;}                             \
        else if(s == 505) {n->stat_505_counter++;}                             \
        else if(s == 506) {n->stat_506_counter++;}                             \
        else if(s == 507) {n->stat_507_counter++;}                             \
        else if(s == 508) {n->stat_508_counter++;}                             \
        else if(s == 510) {n->stat_510_counter++;}                             \
        else if(s == 511) {n->stat_511_counter++;}                             \
        else if(s == 599) {n->stat_599_counter++;}}                            \
}

#if (NGX_HTTP_CACHE)

#if !defined(nginx_version) || nginx_version < 1005007
#define ngx_http_vhost_traffic_status_add_cc(s, n) {                           \
    if(s == NGX_HTTP_CACHE_MISS) {n->stat_cache_miss_counter++;}               \
    else if(s == NGX_HTTP_CACHE_BYPASS) {n->stat_cache_bypass_counter++;}      \
    else if(s == NGX_HTTP_CACHE_EXPIRED) {n->stat_cache_expired_counter++;}    \
    else if(s == NGX_HTTP_CACHE_STALE) {n->stat_cache_stale_counter++;}        \
    else if(s == NGX_HTTP_CACHE_UPDATING) {n->stat_cache_updating_counter++;}  \
    else if(s == NGX_HTTP_CACHE_HIT) {n->stat_cache_hit_counter++;}            \
    else if(s == NGX_HTTP_CACHE_SCARCE) {n->stat_cache_scarce_counter++;}      \
}
#else
#define ngx_http_vhost_traffic_status_add_cc(s, n) {                           \
    if(s == NGX_HTTP_CACHE_MISS) {                                             \
        n->stat_cache_miss_counter++;                                          \
    }                                                                          \
    else if(s == NGX_HTTP_CACHE_BYPASS) {                                      \
        n->stat_cache_bypass_counter++;                                        \
    }                                                                          \
    else if(s == NGX_HTTP_CACHE_EXPIRED) {                                     \
        n->stat_cache_expired_counter++;                                       \
    }                                                                          \
    else if(s == NGX_HTTP_CACHE_STALE) {                                       \
        n->stat_cache_stale_counter++;                                         \
    }                                                                          \
    else if(s == NGX_HTTP_CACHE_UPDATING) {                                    \
        n->stat_cache_updating_counter++;                                      \
    }                                                                          \
    else if(s == NGX_HTTP_CACHE_REVALIDATED) {                                 \
        n->stat_cache_revalidated_counter++;                                   \
    }                                                                          \
    else if(s == NGX_HTTP_CACHE_HIT) {                                         \
        n->stat_cache_hit_counter++;                                           \
    }                                                                          \
    else if(s == NGX_HTTP_CACHE_SCARCE) {                                      \
        n->stat_cache_scarce_counter++;                                        \
    }                                                                          \
}
#endif

#endif

#if (NGX_HTTP_CACHE)
#define ngx_http_vhost_traffic_status_add_oc(o, c) {                           \
    if (o->stat_request_counter > c->stat_request_counter) {                   \
        c->stat_request_counter_oc++;                                          \
    }                                                                          \
    if (o->stat_in_bytes > c->stat_in_bytes) {                                 \
        c->stat_in_bytes_oc++;                                                 \
    }                                                                          \
    if (o->stat_out_bytes > c->stat_out_bytes) {                               \
        c->stat_out_bytes_oc++;                                                \
    }                                                                          \
    if (o->stat_1xx_counter > c->stat_1xx_counter) {                           \
        c->stat_1xx_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_2xx_counter > c->stat_2xx_counter) {                           \
        c->stat_2xx_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_3xx_counter > c->stat_3xx_counter) {                           \
        c->stat_3xx_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_4xx_counter > c->stat_4xx_counter) {                           \
        c->stat_4xx_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_5xx_counter > c->stat_5xx_counter) {                           \
        c->stat_5xx_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_100_counter > c->stat_100_counter) {                           \
        c->stat_100_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_101_counter > c->stat_101_counter) {                           \
        c->stat_101_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_102_counter > c->stat_102_counter) {                           \
        c->stat_102_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_200_counter > c->stat_200_counter) {                           \
        c->stat_200_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_201_counter > c->stat_201_counter) {                           \
        c->stat_201_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_202_counter > c->stat_202_counter) {                           \
        c->stat_202_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_203_counter > c->stat_203_counter) {                           \
        c->stat_203_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_204_counter > c->stat_204_counter) {                           \
        c->stat_204_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_205_counter > c->stat_205_counter) {                           \
        c->stat_205_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_206_counter > c->stat_206_counter) {                           \
        c->stat_206_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_207_counter > c->stat_207_counter) {                           \
        c->stat_207_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_208_counter > c->stat_208_counter) {                           \
        c->stat_208_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_226_counter > c->stat_226_counter) {                           \
        c->stat_226_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_300_counter > c->stat_300_counter) {                           \
        c->stat_300_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_301_counter > c->stat_301_counter) {                           \
        c->stat_301_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_302_counter > c->stat_302_counter) {                           \
        c->stat_302_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_303_counter > c->stat_303_counter) {                           \
        c->stat_303_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_304_counter > c->stat_304_counter) {                           \
        c->stat_304_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_305_counter > c->stat_305_counter) {                           \
        c->stat_305_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_307_counter > c->stat_307_counter) {                           \
        c->stat_307_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_308_counter > c->stat_308_counter) {                           \
        c->stat_308_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_400_counter > c->stat_400_counter) {                           \
        c->stat_400_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_401_counter > c->stat_401_counter) {                           \
        c->stat_401_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_402_counter > c->stat_402_counter) {                           \
        c->stat_402_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_403_counter > c->stat_403_counter) {                           \
        c->stat_403_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_404_counter > c->stat_404_counter) {                           \
        c->stat_404_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_405_counter > c->stat_405_counter) {                           \
        c->stat_405_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_406_counter > c->stat_406_counter) {                           \
        c->stat_406_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_407_counter > c->stat_407_counter) {                           \
        c->stat_407_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_408_counter > c->stat_408_counter) {                           \
        c->stat_408_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_409_counter > c->stat_409_counter) {                           \
        c->stat_409_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_410_counter > c->stat_410_counter) {                           \
        c->stat_410_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_411_counter > c->stat_411_counter) {                           \
        c->stat_411_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_412_counter > c->stat_412_counter) {                           \
        c->stat_412_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_413_counter > c->stat_413_counter) {                           \
        c->stat_413_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_414_counter > c->stat_414_counter) {                           \
        c->stat_414_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_415_counter > c->stat_415_counter) {                           \
        c->stat_415_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_416_counter > c->stat_416_counter) {                           \
        c->stat_416_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_417_counter > c->stat_417_counter) {                           \
        c->stat_417_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_418_counter > c->stat_418_counter) {                           \
        c->stat_418_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_421_counter > c->stat_421_counter) {                           \
        c->stat_421_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_422_counter > c->stat_422_counter) {                           \
        c->stat_422_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_423_counter > c->stat_423_counter) {                           \
        c->stat_423_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_424_counter > c->stat_424_counter) {                           \
        c->stat_424_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_426_counter > c->stat_426_counter) {                           \
        c->stat_426_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_428_counter > c->stat_428_counter) {                           \
        c->stat_428_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_429_counter > c->stat_429_counter) {                           \
        c->stat_429_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_431_counter > c->stat_431_counter) {                           \
        c->stat_431_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_444_counter > c->stat_444_counter) {                           \
        c->stat_444_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_451_counter > c->stat_451_counter) {                           \
        c->stat_451_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_499_counter > c->stat_499_counter) {                           \
        c->stat_499_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_500_counter > c->stat_500_counter) {                           \
        c->stat_500_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_501_counter > c->stat_501_counter) {                           \
        c->stat_501_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_502_counter > c->stat_502_counter) {                           \
        c->stat_502_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_503_counter > c->stat_503_counter) {                           \
        c->stat_503_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_504_counter > c->stat_504_counter) {                           \
        c->stat_504_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_505_counter > c->stat_505_counter) {                           \
        c->stat_505_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_506_counter > c->stat_506_counter) {                           \
        c->stat_506_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_507_counter > c->stat_507_counter) {                           \
        c->stat_507_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_508_counter > c->stat_508_counter) {                           \
        c->stat_508_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_510_counter > c->stat_510_counter) {                           \
        c->stat_510_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_511_counter > c->stat_511_counter) {                           \
        c->stat_511_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_599_counter > c->stat_599_counter) {                           \
        c->stat_599_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_request_time_counter > c->stat_request_time_counter) {         \
        c->stat_request_time_counter_oc++;                                     \
    }                                                                          \
    if (o->stat_cache_miss_counter > c->stat_cache_miss_counter) {             \
        c->stat_cache_miss_counter_oc++;                                       \
    }                                                                          \
    if (o->stat_cache_bypass_counter > c->stat_cache_bypass_counter) {         \
        c->stat_cache_bypass_counter_oc++;                                     \
    }                                                                          \
    if (o->stat_cache_expired_counter > c->stat_cache_expired_counter) {       \
        c->stat_cache_expired_counter_oc++;                                    \
    }                                                                          \
    if (o->stat_cache_stale_counter > c->stat_cache_stale_counter) {           \
        c->stat_cache_stale_counter_oc++;                                      \
    }                                                                          \
    if (o->stat_cache_updating_counter > c->stat_cache_updating_counter) {     \
        c->stat_cache_updating_counter_oc++;                                   \
    }                                                                          \
    if (o->stat_cache_revalidated_counter > c->stat_cache_revalidated_counter) \
    {                                                                          \
        c->stat_cache_revalidated_counter_oc++;                                \
    }                                                                          \
    if (o->stat_cache_hit_counter > c->stat_cache_hit_counter) {               \
        c->stat_cache_hit_counter_oc++;                                        \
    }                                                                          \
    if (o->stat_cache_scarce_counter > c->stat_cache_scarce_counter) {         \
        c->stat_cache_scarce_counter_oc++;                                     \
    }                                                                          \
}
#else
#define ngx_http_vhost_traffic_status_add_oc(o, c) {                           \
    if (o->stat_request_counter > c->stat_request_counter) {                   \
        c->stat_request_counter_oc++;                                          \
    }                                                                          \
    if (o->stat_in_bytes > c->stat_in_bytes) {                                 \
        c->stat_in_bytes_oc++;                                                 \
    }                                                                          \
    if (o->stat_out_bytes > c->stat_out_bytes) {                               \
        c->stat_out_bytes_oc++;                                                \
    }                                                                          \
    if (o->stat_1xx_counter > c->stat_1xx_counter) {                           \
        c->stat_1xx_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_2xx_counter > c->stat_2xx_counter) {                           \
        c->stat_2xx_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_3xx_counter > c->stat_3xx_counter) {                           \
        c->stat_3xx_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_4xx_counter > c->stat_4xx_counter) {                           \
        c->stat_4xx_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_5xx_counter > c->stat_5xx_counter) {                           \
        c->stat_5xx_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_100_counter > c->stat_100_counter) {                           \
        c->stat_100_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_101_counter > c->stat_101_counter) {                           \
        c->stat_101_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_102_counter > c->stat_102_counter) {                           \
        c->stat_102_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_200_counter > c->stat_200_counter) {                           \
        c->stat_200_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_201_counter > c->stat_201_counter) {                           \
        c->stat_201_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_202_counter > c->stat_202_counter) {                           \
        c->stat_202_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_203_counter > c->stat_203_counter) {                           \
        c->stat_203_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_204_counter > c->stat_204_counter) {                           \
        c->stat_204_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_205_counter > c->stat_205_counter) {                           \
        c->stat_205_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_206_counter > c->stat_206_counter) {                           \
        c->stat_206_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_207_counter > c->stat_207_counter) {                           \
        c->stat_207_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_208_counter > c->stat_208_counter) {                           \
        c->stat_208_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_226_counter > c->stat_226_counter) {                           \
        c->stat_226_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_300_counter > c->stat_300_counter) {                           \
        c->stat_300_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_301_counter > c->stat_301_counter) {                           \
        c->stat_301_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_302_counter > c->stat_302_counter) {                           \
        c->stat_302_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_303_counter > c->stat_303_counter) {                           \
        c->stat_303_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_304_counter > c->stat_304_counter) {                           \
        c->stat_304_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_305_counter > c->stat_305_counter) {                           \
        c->stat_305_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_307_counter > c->stat_307_counter) {                           \
        c->stat_307_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_308_counter > c->stat_308_counter) {                           \
        c->stat_308_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_400_counter > c->stat_400_counter) {                           \
        c->stat_400_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_401_counter > c->stat_401_counter) {                           \
        c->stat_401_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_402_counter > c->stat_402_counter) {                           \
        c->stat_402_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_403_counter > c->stat_403_counter) {                           \
        c->stat_403_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_404_counter > c->stat_404_counter) {                           \
        c->stat_404_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_405_counter > c->stat_405_counter) {                           \
        c->stat_405_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_406_counter > c->stat_406_counter) {                           \
        c->stat_406_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_407_counter > c->stat_407_counter) {                           \
        c->stat_407_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_408_counter > c->stat_408_counter) {                           \
        c->stat_408_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_409_counter > c->stat_409_counter) {                           \
        c->stat_409_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_410_counter > c->stat_410_counter) {                           \
        c->stat_410_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_411_counter > c->stat_411_counter) {                           \
        c->stat_411_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_412_counter > c->stat_412_counter) {                           \
        c->stat_412_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_413_counter > c->stat_413_counter) {                           \
        c->stat_413_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_414_counter > c->stat_414_counter) {                           \
        c->stat_414_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_415_counter > c->stat_415_counter) {                           \
        c->stat_415_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_416_counter > c->stat_416_counter) {                           \
        c->stat_416_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_417_counter > c->stat_417_counter) {                           \
        c->stat_417_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_418_counter > c->stat_418_counter) {                           \
        c->stat_418_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_421_counter > c->stat_421_counter) {                           \
        c->stat_421_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_422_counter > c->stat_422_counter) {                           \
        c->stat_422_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_423_counter > c->stat_423_counter) {                           \
        c->stat_423_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_424_counter > c->stat_424_counter) {                           \
        c->stat_424_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_426_counter > c->stat_426_counter) {                           \
        c->stat_426_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_428_counter > c->stat_428_counter) {                           \
        c->stat_428_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_429_counter > c->stat_429_counter) {                           \
        c->stat_429_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_431_counter > c->stat_431_counter) {                           \
        c->stat_431_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_444_counter > c->stat_444_counter) {                           \
        c->stat_444_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_451_counter > c->stat_451_counter) {                           \
        c->stat_451_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_499_counter > c->stat_499_counter) {                           \
        c->stat_499_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_500_counter > c->stat_500_counter) {                           \
        c->stat_500_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_501_counter > c->stat_501_counter) {                           \
        c->stat_501_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_502_counter > c->stat_502_counter) {                           \
        c->stat_502_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_503_counter > c->stat_503_counter) {                           \
        c->stat_503_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_504_counter > c->stat_504_counter) {                           \
        c->stat_504_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_505_counter > c->stat_505_counter) {                           \
        c->stat_505_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_506_counter > c->stat_506_counter) {                           \
        c->stat_506_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_507_counter > c->stat_507_counter) {                           \
        c->stat_507_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_508_counter > c->stat_508_counter) {                           \
        c->stat_508_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_510_counter > c->stat_510_counter) {                           \
        c->stat_510_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_511_counter > c->stat_511_counter) {                           \
        c->stat_511_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_599_counter > c->stat_599_counter) {                           \
        c->stat_599_counter_oc++;                                              \
    }                                                                          \
    if (o->stat_request_time_counter > c->stat_request_time_counter) {         \
        c->stat_request_time_counter_oc++;                                     \
    }                                                                          \
}
#endif

#define ngx_http_vhost_traffic_status_group_to_string(n) (u_char *) (          \
    (n > 4)                                                                    \
    ? NGX_HTTP_VHOST_TRAFFIC_STATUS_UPSTREAMS                                  \
    : NGX_HTTP_VHOST_TRAFFIC_STATUS_UPSTREAMS + 3 * n                          \
)

#define ngx_http_vhost_traffic_status_string_to_group(s) (unsigned) (          \
{                                                                              \
    unsigned n = NGX_HTTP_VHOST_TRAFFIC_STATUS_UPSTREAM_NO;                    \
    if (*s == 'N' && *(s + 1) == 'O') {                                        \
        n = NGX_HTTP_VHOST_TRAFFIC_STATUS_UPSTREAM_NO;                         \
    } else if (*s == 'U' && *(s + 1) == 'A') {                                 \
        n = NGX_HTTP_VHOST_TRAFFIC_STATUS_UPSTREAM_UA;                         \
    } else if (*s == 'U' && *(s + 1) == 'G') {                                 \
        n = NGX_HTTP_VHOST_TRAFFIC_STATUS_UPSTREAM_UG;                         \
    } else if (*s == 'C' && *(s + 1) == 'C') {                                 \
        n = NGX_HTTP_VHOST_TRAFFIC_STATUS_UPSTREAM_CC;                         \
    } else if (*s == 'F' && *(s + 1) == 'G') {                                 \
        n = NGX_HTTP_VHOST_TRAFFIC_STATUS_UPSTREAM_FG;                         \
    }                                                                          \
    n;                                                                         \
}                                                                              \
)

#define ngx_http_vhost_traffic_status_max_integer (NGX_ATOMIC_T_LEN < 12)      \
    ? "4294967295"                                                             \
    : "18446744073709551615"

#define ngx_http_vhost_traffic_status_boolean_to_string(b) (b) ? "true" : "false"

#define ngx_http_vhost_traffic_status_triangle(n) (unsigned) (                 \
    n * (n + 1) / 2                                                            \
)


typedef struct {
    ngx_rbtree_t                           *rbtree;

    /* array of ngx_http_vhost_traffic_status_filter_t */
    ngx_array_t                            *filter_keys;

    /* array of ngx_http_vhost_traffic_status_limit_t */
    ngx_array_t                            *limit_traffics;

    /* array of ngx_http_vhost_traffic_status_limit_t */
    ngx_array_t                            *limit_filter_traffics;

    /* array of ngx_http_vhost_traffic_status_filter_match_t */
    ngx_array_t                            *filter_max_node_matches;

    ngx_uint_t                              filter_max_node;

    ngx_flag_t                              enable;
    ngx_flag_t                              filter_check_duplicate;
    ngx_flag_t                              limit_check_duplicate;
    ngx_shm_zone_t                         *shm_zone;
    ngx_str_t                               shm_name;
    ssize_t                                 shm_size;

    ngx_flag_t                              dump;
    ngx_str_t                               dump_file;
    ngx_msec_t                              dump_period;
    ngx_event_t                             dump_event;
} ngx_http_vhost_traffic_status_ctx_t;


typedef struct {
    ngx_shm_zone_t                         *shm_zone;
    ngx_str_t                               shm_name;
    ngx_flag_t                              enable;
    ngx_flag_t                              filter;
    ngx_flag_t                              filter_host;
    ngx_flag_t                              filter_check_duplicate;

    /* array of ngx_http_vhost_traffic_status_filter_t */
    ngx_array_t                            *filter_keys;

    /* array of ngx_http_vhost_traffic_status_filter_variable_t */
    ngx_array_t                            *filter_vars;

    ngx_flag_t                              limit;
    ngx_flag_t                              limit_check_duplicate;

    /* array of ngx_http_vhost_traffic_status_limit_t */
    ngx_array_t                            *limit_traffics;

    /* array of ngx_http_vhost_traffic_status_limit_t */
    ngx_array_t                            *limit_filter_traffics;

    ngx_http_vhost_traffic_status_node_t    stats;
    ngx_msec_t                              start_msec;
    ngx_flag_t                              format;
    ngx_str_t                               jsonp;
    ngx_str_t                               sum_key;

    ngx_flag_t                              average_method;
    ngx_msec_t                              average_period;

    /* array of ngx_http_vhost_traffic_status_node_histogram_t */
    ngx_array_t                            *histogram_buckets;

    ngx_flag_t                              bypass_limit;
    ngx_flag_t                              bypass_stats;

    ngx_rbtree_node_t                     **node_caches;
} ngx_http_vhost_traffic_status_loc_conf_t;


ngx_msec_t ngx_http_vhost_traffic_status_current_msec(void);
ngx_msec_int_t ngx_http_vhost_traffic_status_request_time(ngx_http_request_t *r);
ngx_msec_int_t ngx_http_vhost_traffic_status_upstream_response_time(ngx_http_request_t *r);

extern ngx_module_t ngx_http_vhost_traffic_status_module;


#endif /* _NGX_HTTP_VTS_MODULE_H_INCLUDED_ */

/* vi:set ft=c ts=4 sw=4 et fdm=marker: */
