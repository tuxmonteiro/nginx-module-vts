
/*
 * Copyright (C) YoungJoo Kim (vozlt)
 */


#ifndef _NGX_HTTP_VTS_NODE_H_INCLUDED_
#define _NGX_HTTP_VTS_NODE_H_INCLUDED_


#define NGX_HTTP_VHOST_TRAFFIC_STATUS_DEFAULT_QUEUE_LEN    64
#define NGX_HTTP_VHOST_TRAFFIC_STATUS_DEFAULT_BUCKET_LEN   32


typedef struct {
    ngx_msec_t                                             time;
    ngx_msec_int_t                                         msec;
} ngx_http_vhost_traffic_status_node_time_t;


typedef struct {
    ngx_http_vhost_traffic_status_node_time_t              times[NGX_HTTP_VHOST_TRAFFIC_STATUS_DEFAULT_QUEUE_LEN];
    ngx_int_t                                              front;
    ngx_int_t                                              rear;
    ngx_int_t                                              len;
} ngx_http_vhost_traffic_status_node_time_queue_t;


typedef struct {
    ngx_msec_int_t                                         msec;
    ngx_atomic_t                                           counter;
} ngx_http_vhost_traffic_status_node_histogram_t;


typedef struct {
    ngx_http_vhost_traffic_status_node_histogram_t         buckets[NGX_HTTP_VHOST_TRAFFIC_STATUS_DEFAULT_BUCKET_LEN];
    ngx_int_t                                              len;
} ngx_http_vhost_traffic_status_node_histogram_bucket_t;


typedef struct {
    /* unsigned type:5 */
    unsigned                                               type;
    ngx_atomic_t                                           response_time_counter;
    ngx_msec_t                                             response_time;
    ngx_http_vhost_traffic_status_node_time_queue_t        response_times;
    ngx_http_vhost_traffic_status_node_histogram_bucket_t  response_buckets;
} ngx_http_vhost_traffic_status_node_upstream_t;


typedef struct {
    u_char                                                 color;
    ngx_atomic_t                                           stat_request_counter;
    ngx_atomic_t                                           stat_in_bytes;
    ngx_atomic_t                                           stat_out_bytes;
    ngx_atomic_t                                           stat_1xx_counter; // Informational
    ngx_atomic_t                                           stat_100_counter; // Continue
    ngx_atomic_t                                           stat_101_counter; // Switching Protocols
    ngx_atomic_t                                           stat_102_counter; // Processing
    ngx_atomic_t                                           stat_2xx_counter; // Success
    ngx_atomic_t                                           stat_200_counter; // OK
    ngx_atomic_t                                           stat_201_counter; // Created
    ngx_atomic_t                                           stat_202_counter; // Accepted
    ngx_atomic_t                                           stat_203_counter; // Non-authoritative Information
    ngx_atomic_t                                           stat_204_counter; // No Content
    ngx_atomic_t                                           stat_205_counter; // Reset Content
    ngx_atomic_t                                           stat_206_counter; // Partial Content
    ngx_atomic_t                                           stat_207_counter; // Multi-Status
    ngx_atomic_t                                           stat_208_counter; // Already Reported
    ngx_atomic_t                                           stat_226_counter; // IM Used
    ngx_atomic_t                                           stat_3xx_counter; // Redirection
    ngx_atomic_t                                           stat_300_counter; // Multiple Choices
    ngx_atomic_t                                           stat_301_counter; // Moved Permanently
    ngx_atomic_t                                           stat_302_counter; // Found
    ngx_atomic_t                                           stat_303_counter; // See Other
    ngx_atomic_t                                           stat_304_counter; // Not Modified
    ngx_atomic_t                                           stat_305_counter; // Use Proxy
    ngx_atomic_t                                           stat_307_counter; // Temporary Redirect
    ngx_atomic_t                                           stat_308_counter; // Permanent Redirect
    ngx_atomic_t                                           stat_4xx_counter; // Client Error
    ngx_atomic_t                                           stat_400_counter; // Bad Request
    ngx_atomic_t                                           stat_401_counter; // Unauthorized
    ngx_atomic_t                                           stat_402_counter; // Payment Required
    ngx_atomic_t                                           stat_403_counter; // Forbidden
    ngx_atomic_t                                           stat_404_counter; // Not Found
    ngx_atomic_t                                           stat_405_counter; // Method Not Allowed
    ngx_atomic_t                                           stat_406_counter; // Not Acceptable
    ngx_atomic_t                                           stat_407_counter; // Proxy Authentication Required
    ngx_atomic_t                                           stat_408_counter; // Request Timeout
    ngx_atomic_t                                           stat_409_counter; // Conflict
    ngx_atomic_t                                           stat_410_counter; // Gone
    ngx_atomic_t                                           stat_411_counter; // Length Required
    ngx_atomic_t                                           stat_412_counter; // Precondition Failed
    ngx_atomic_t                                           stat_413_counter; // Payload Too Large
    ngx_atomic_t                                           stat_414_counter; // Request-URI Too Long
    ngx_atomic_t                                           stat_415_counter; // Unsupported Media Type
    ngx_atomic_t                                           stat_416_counter; // Requested Range Not Satisfiable
    ngx_atomic_t                                           stat_417_counter; // Expectation Failed
    ngx_atomic_t                                           stat_418_counter; // I'm a teapot
    ngx_atomic_t                                           stat_421_counter; // Misdirected Request
    ngx_atomic_t                                           stat_422_counter; // Unprocessable Entity
    ngx_atomic_t                                           stat_423_counter; // Locked
    ngx_atomic_t                                           stat_424_counter; // Failed Dependency
    ngx_atomic_t                                           stat_426_counter; // Upgrade Required
    ngx_atomic_t                                           stat_428_counter; // Precondition Required
    ngx_atomic_t                                           stat_429_counter; // Too Many Requests
    ngx_atomic_t                                           stat_431_counter; // Request Header Fields Too Large
    ngx_atomic_t                                           stat_444_counter; // Connection Closed Without Response
    ngx_atomic_t                                           stat_451_counter; // Unavailable For Legal Reasons
    ngx_atomic_t                                           stat_499_counter; // Client Closed Request
    ngx_atomic_t                                           stat_5xx_counter; // Server Error
    ngx_atomic_t                                           stat_500_counter; // Internal Server Error
    ngx_atomic_t                                           stat_501_counter; // Not Implemented
    ngx_atomic_t                                           stat_502_counter; // Bad Gateway
    ngx_atomic_t                                           stat_503_counter; // Service Unavailable
    ngx_atomic_t                                           stat_504_counter; // Gateway Timeout
    ngx_atomic_t                                           stat_505_counter; // HTTP Version Not Supported
    ngx_atomic_t                                           stat_506_counter; // Variant Also Negotiates
    ngx_atomic_t                                           stat_507_counter; // Insufficient Storage
    ngx_atomic_t                                           stat_508_counter; // Loop Detected
    ngx_atomic_t                                           stat_510_counter; // Not Extended
    ngx_atomic_t                                           stat_511_counter; // Network Authentication Required
    ngx_atomic_t                                           stat_599_counter; // Network Connect Timeout Error

    ngx_atomic_t                                           stat_request_time_counter;
    ngx_msec_t                                             stat_request_time;
    ngx_http_vhost_traffic_status_node_time_queue_t        stat_request_times;
    ngx_http_vhost_traffic_status_node_histogram_bucket_t  stat_request_buckets;

    /* deals with the overflow of variables */
    ngx_atomic_t                                           stat_request_counter_oc;
    ngx_atomic_t                                           stat_in_bytes_oc;
    ngx_atomic_t                                           stat_out_bytes_oc;
    ngx_atomic_t                                           stat_1xx_counter_oc; // Informational
    ngx_atomic_t                                           stat_100_counter_oc; // Continue
    ngx_atomic_t                                           stat_101_counter_oc; // Switching Protocols
    ngx_atomic_t                                           stat_102_counter_oc; // Processing
    ngx_atomic_t                                           stat_2xx_counter_oc; // Success
    ngx_atomic_t                                           stat_200_counter_oc; // OK
    ngx_atomic_t                                           stat_201_counter_oc; // Created
    ngx_atomic_t                                           stat_202_counter_oc; // Accepted
    ngx_atomic_t                                           stat_203_counter_oc; // Non-authoritative Information
    ngx_atomic_t                                           stat_204_counter_oc; // No Content
    ngx_atomic_t                                           stat_205_counter_oc; // Reset Content
    ngx_atomic_t                                           stat_206_counter_oc; // Partial Content
    ngx_atomic_t                                           stat_207_counter_oc; // Multi-Status
    ngx_atomic_t                                           stat_208_counter_oc; // Already Reported
    ngx_atomic_t                                           stat_226_counter_oc; // IM Used
    ngx_atomic_t                                           stat_3xx_counter_oc; // Redirection
    ngx_atomic_t                                           stat_300_counter_oc; // Multiple Choices
    ngx_atomic_t                                           stat_301_counter_oc; // Moved Permanently
    ngx_atomic_t                                           stat_302_counter_oc; // Found
    ngx_atomic_t                                           stat_303_counter_oc; // See Other
    ngx_atomic_t                                           stat_304_counter_oc; // Not Modified
    ngx_atomic_t                                           stat_305_counter_oc; // Use Proxy
    ngx_atomic_t                                           stat_307_counter_oc; // Temporary Redirect
    ngx_atomic_t                                           stat_308_counter_oc; // Permanent Redirect
    ngx_atomic_t                                           stat_4xx_counter_oc; // Client Error
    ngx_atomic_t                                           stat_400_counter_oc; // Bad Request
    ngx_atomic_t                                           stat_401_counter_oc; // Unauthorized
    ngx_atomic_t                                           stat_402_counter_oc; // Payment Required
    ngx_atomic_t                                           stat_403_counter_oc; // Forbidden
    ngx_atomic_t                                           stat_404_counter_oc; // Not Found
    ngx_atomic_t                                           stat_405_counter_oc; // Method Not Allowed
    ngx_atomic_t                                           stat_406_counter_oc; // Not Acceptable
    ngx_atomic_t                                           stat_407_counter_oc; // Proxy Authentication Required
    ngx_atomic_t                                           stat_408_counter_oc; // Request Timeout
    ngx_atomic_t                                           stat_409_counter_oc; // Conflict
    ngx_atomic_t                                           stat_410_counter_oc; // Gone
    ngx_atomic_t                                           stat_411_counter_oc; // Length Required
    ngx_atomic_t                                           stat_412_counter_oc; // Precondition Failed
    ngx_atomic_t                                           stat_413_counter_oc; // Payload Too Large
    ngx_atomic_t                                           stat_414_counter_oc; // Request-URI Too Long
    ngx_atomic_t                                           stat_415_counter_oc; // Unsupported Media Type
    ngx_atomic_t                                           stat_416_counter_oc; // Requested Range Not Satisfiable
    ngx_atomic_t                                           stat_417_counter_oc; // Expectation Failed
    ngx_atomic_t                                           stat_418_counter_oc; // I'm a teapot
    ngx_atomic_t                                           stat_421_counter_oc; // Misdirected Request
    ngx_atomic_t                                           stat_422_counter_oc; // Unprocessable Entity
    ngx_atomic_t                                           stat_423_counter_oc; // Locked
    ngx_atomic_t                                           stat_424_counter_oc; // Failed Dependency
    ngx_atomic_t                                           stat_426_counter_oc; // Upgrade Required
    ngx_atomic_t                                           stat_428_counter_oc; // Precondition Required
    ngx_atomic_t                                           stat_429_counter_oc; // Too Many Requests
    ngx_atomic_t                                           stat_431_counter_oc; // Request Header Fields Too Large
    ngx_atomic_t                                           stat_444_counter_oc; // Connection Closed Without Response
    ngx_atomic_t                                           stat_451_counter_oc; // Unavailable For Legal Reasons
    ngx_atomic_t                                           stat_499_counter_oc; // Client Closed Request
    ngx_atomic_t                                           stat_5xx_counter_oc; // Server Error
    ngx_atomic_t                                           stat_500_counter_oc; // Internal Server Error
    ngx_atomic_t                                           stat_501_counter_oc; // Not Implemented
    ngx_atomic_t                                           stat_502_counter_oc; // Bad Gateway
    ngx_atomic_t                                           stat_503_counter_oc; // Service Unavailable
    ngx_atomic_t                                           stat_504_counter_oc; // Gateway Timeout
    ngx_atomic_t                                           stat_505_counter_oc; // HTTP Version Not Supported
    ngx_atomic_t                                           stat_506_counter_oc; // Variant Also Negotiates
    ngx_atomic_t                                           stat_507_counter_oc; // Insufficient Storage
    ngx_atomic_t                                           stat_508_counter_oc; // Loop Detected
    ngx_atomic_t                                           stat_510_counter_oc; // Not Extended
    ngx_atomic_t                                           stat_511_counter_oc; // Network Authentication Required
    ngx_atomic_t                                           stat_599_counter_oc; // Network Connect Timeout Error
    ngx_atomic_t                                           stat_request_time_counter_oc;
    ngx_atomic_t                                           stat_response_time_counter_oc;

#if (NGX_HTTP_CACHE)
    ngx_atomic_t                                           stat_cache_max_size;
    ngx_atomic_t                                           stat_cache_used_size;
    ngx_atomic_t                                           stat_cache_miss_counter;
    ngx_atomic_t                                           stat_cache_bypass_counter;
    ngx_atomic_t                                           stat_cache_expired_counter;
    ngx_atomic_t                                           stat_cache_stale_counter;
    ngx_atomic_t                                           stat_cache_updating_counter;
    ngx_atomic_t                                           stat_cache_revalidated_counter;
    ngx_atomic_t                                           stat_cache_hit_counter;
    ngx_atomic_t                                           stat_cache_scarce_counter;

    /* deals with the overflow of variables */
    ngx_atomic_t                                           stat_cache_miss_counter_oc;
    ngx_atomic_t                                           stat_cache_bypass_counter_oc;
    ngx_atomic_t                                           stat_cache_expired_counter_oc;
    ngx_atomic_t                                           stat_cache_stale_counter_oc;
    ngx_atomic_t                                           stat_cache_updating_counter_oc;
    ngx_atomic_t                                           stat_cache_revalidated_counter_oc;
    ngx_atomic_t                                           stat_cache_hit_counter_oc;
    ngx_atomic_t                                           stat_cache_scarce_counter_oc;
#endif

    ngx_http_vhost_traffic_status_node_upstream_t          stat_upstream;
    u_short                                                len;
    u_char                                                 data[1];
} ngx_http_vhost_traffic_status_node_t;


ngx_int_t ngx_http_vhost_traffic_status_node_generate_key(ngx_pool_t *pool,
    ngx_str_t *buf, ngx_str_t *dst, unsigned type);
ngx_int_t ngx_http_vhost_traffic_status_node_position_key(ngx_str_t *buf,
    size_t pos);

ngx_rbtree_node_t *ngx_http_vhost_traffic_status_node_lookup(
    ngx_rbtree_t *rbtree, ngx_str_t *key, uint32_t hash);
void ngx_http_vhost_traffic_status_node_zero(
    ngx_http_vhost_traffic_status_node_t *vtsn);
void ngx_http_vhost_traffic_status_node_init(ngx_http_request_t *r,
    ngx_http_vhost_traffic_status_node_t *vtsn);
void ngx_http_vhost_traffic_status_node_set(ngx_http_request_t *r,
    ngx_http_vhost_traffic_status_node_t *vtsn);
void ngx_http_vhost_traffic_status_node_update(ngx_http_request_t *r,
    ngx_http_vhost_traffic_status_node_t *vtsn, ngx_msec_int_t ms);

void ngx_http_vhost_traffic_status_node_time_queue_zero(
    ngx_http_vhost_traffic_status_node_time_queue_t *q);
void ngx_http_vhost_traffic_status_node_time_queue_init(
    ngx_http_vhost_traffic_status_node_time_queue_t *q);
void ngx_http_vhost_traffic_status_node_time_queue_insert(
    ngx_http_vhost_traffic_status_node_time_queue_t *q,
    ngx_msec_int_t x);
ngx_int_t ngx_http_vhost_traffic_status_node_time_queue_push(
    ngx_http_vhost_traffic_status_node_time_queue_t *q,
    ngx_msec_int_t x);
ngx_int_t ngx_http_vhost_traffic_status_node_time_queue_pop(
    ngx_http_vhost_traffic_status_node_time_queue_t *q,
    ngx_http_vhost_traffic_status_node_time_t *x);
ngx_int_t ngx_http_vhost_traffic_status_node_time_queue_rear(
    ngx_http_vhost_traffic_status_node_time_queue_t *q);

ngx_msec_t ngx_http_vhost_traffic_status_node_time_queue_average(
    ngx_http_vhost_traffic_status_node_time_queue_t *q,
    ngx_int_t method, ngx_msec_t period);
ngx_msec_t ngx_http_vhost_traffic_status_node_time_queue_amm(
    ngx_http_vhost_traffic_status_node_time_queue_t *q,
    ngx_msec_t period);
ngx_msec_t ngx_http_vhost_traffic_status_node_time_queue_wma(
    ngx_http_vhost_traffic_status_node_time_queue_t *q,
    ngx_msec_t period);
void ngx_http_vhost_traffic_status_node_time_queue_merge(
    ngx_http_vhost_traffic_status_node_time_queue_t *a,
    ngx_http_vhost_traffic_status_node_time_queue_t *b,
    ngx_msec_t period);

void ngx_http_vhost_traffic_status_node_histogram_bucket_init(
    ngx_http_request_t *r,
    ngx_http_vhost_traffic_status_node_histogram_bucket_t *b);
void ngx_http_vhost_traffic_status_node_histogram_observe(
    ngx_http_vhost_traffic_status_node_histogram_bucket_t *b,
    ngx_msec_int_t x);

void ngx_http_vhost_traffic_status_find_name(ngx_http_request_t *r,
    ngx_str_t *buf);
ngx_rbtree_node_t *ngx_http_vhost_traffic_status_find_node(ngx_http_request_t *r,
    ngx_str_t *key, unsigned type, uint32_t key_hash);

ngx_rbtree_node_t *ngx_http_vhost_traffic_status_find_lru(ngx_http_request_t *r);
ngx_rbtree_node_t *ngx_http_vhost_traffic_status_find_lru_node(ngx_http_request_t *r,
    ngx_rbtree_node_t *a, ngx_rbtree_node_t *b);
ngx_rbtree_node_t *ngx_http_vhost_traffic_status_find_lru_node_cmp(ngx_http_request_t *r,
    ngx_rbtree_node_t *a, ngx_rbtree_node_t *b);

ngx_int_t ngx_http_vhost_traffic_status_node_member_cmp(ngx_str_t *member, const char *name);
ngx_atomic_uint_t ngx_http_vhost_traffic_status_node_member(
    ngx_http_vhost_traffic_status_node_t *vtsn,
    ngx_str_t *member);


#endif /* _NGX_HTTP_VTS_NODE_H_INCLUDED_ */

/* vi:set ft=c ts=4 sw=4 et fdm=marker: */
