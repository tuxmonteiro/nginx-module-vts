
/*
 * Copyright (C) YoungJoo Kim (vozlt)
 */


#include "ngx_http_vhost_traffic_status_module.h"
#include "ngx_http_vhost_traffic_status_filter.h"
#include "ngx_http_vhost_traffic_status_shm.h"
#include "ngx_http_vhost_traffic_status_node.h"


ngx_int_t
ngx_http_vhost_traffic_status_node_generate_key(ngx_pool_t *pool,
    ngx_str_t *buf, ngx_str_t *dst, unsigned type)
{
    size_t   len;
    u_char  *p;

    len = ngx_strlen(ngx_http_vhost_traffic_status_group_to_string(type));

    buf->len = len + sizeof("@") - 1 + dst->len;
    buf->data = ngx_pcalloc(pool, buf->len);
    if (buf->data == NULL) {
        *buf = *dst;
        return NGX_ERROR;
    }

    p = buf->data;

    p = ngx_cpymem(p, ngx_http_vhost_traffic_status_group_to_string(type), len);
    *p++ = NGX_HTTP_VHOST_TRAFFIC_STATUS_KEY_SEPARATOR;
    p = ngx_cpymem(p, dst->data, dst->len);

    return NGX_OK;
}


ngx_int_t
ngx_http_vhost_traffic_status_node_position_key(ngx_str_t *buf, size_t pos)
{
    size_t   n, c, len;
    u_char  *p, *s;

    n = buf->len + 1;
    c = len = 0;
    p = s = buf->data;

    while (--n) {
        if (*p == NGX_HTTP_VHOST_TRAFFIC_STATUS_KEY_SEPARATOR) {
            if (pos == c) {
                break;
            }
            s = (p + 1);
            c++;
        }
        p++;
        len = (p - s);
    }

    if (pos > c || len == 0) {
        return NGX_ERROR;
    }

    buf->data = s;
    buf->len = len;

    return NGX_OK;
}


void
ngx_http_vhost_traffic_status_find_name(ngx_http_request_t *r,
    ngx_str_t *buf)
{
    ngx_http_core_srv_conf_t                  *cscf;
    ngx_http_vhost_traffic_status_loc_conf_t  *vtscf;

    cscf = ngx_http_get_module_srv_conf(r, ngx_http_core_module);
    vtscf = ngx_http_get_module_loc_conf(r, ngx_http_vhost_traffic_status_module);

    if (vtscf->filter && vtscf->filter_host && r->headers_in.server.len) {
        /* set the key by host header */
        *buf = r->headers_in.server;

    } else {
        /* set the key by server_name variable */
        *buf = cscf->server_name;

        if (buf->len == 0) {
            buf->len = 1;
            buf->data = (u_char *) "_";
        }
    }
}


ngx_rbtree_node_t *
ngx_http_vhost_traffic_status_find_node(ngx_http_request_t *r,
    ngx_str_t *key, unsigned type, uint32_t key_hash)
{
    uint32_t                                   hash;
    ngx_rbtree_node_t                         *node;
    ngx_http_vhost_traffic_status_ctx_t       *ctx;
    ngx_http_vhost_traffic_status_loc_conf_t  *vtscf;

    ctx = ngx_http_get_module_main_conf(r, ngx_http_vhost_traffic_status_module);
    vtscf = ngx_http_get_module_loc_conf(r, ngx_http_vhost_traffic_status_module);

    hash = key_hash;

    if (hash == 0) {
        hash = ngx_crc32_short(key->data, key->len);
    }

    if (vtscf->node_caches[type] != NULL) {
        if (vtscf->node_caches[type]->key == hash) {
            node = vtscf->node_caches[type];
            goto found;
        }
    }

    node = ngx_http_vhost_traffic_status_node_lookup(ctx->rbtree, key, hash);

found:

    return node;
}


ngx_rbtree_node_t *
ngx_http_vhost_traffic_status_find_lru(ngx_http_request_t *r)
{
    ngx_rbtree_node_t                         *node;
    ngx_http_vhost_traffic_status_ctx_t       *ctx;
    ngx_http_vhost_traffic_status_shm_info_t  *shm_info;

    ctx = ngx_http_get_module_main_conf(r, ngx_http_vhost_traffic_status_module);
    node = NULL;

    /* disabled */
    if (ctx->filter_max_node == 0) {
        return NULL;
    }

    shm_info = ngx_pcalloc(r->pool, sizeof(ngx_http_vhost_traffic_status_shm_info_t));

    if (shm_info == NULL) { 
        return NULL;
    }

    ngx_http_vhost_traffic_status_shm_info(r, shm_info);

    /* find */
    if (shm_info->filter_used_node >= ctx->filter_max_node) {
        node = ngx_http_vhost_traffic_status_find_lru_node(r, NULL, ctx->rbtree->root);
    }

    return node;
}


ngx_rbtree_node_t *
ngx_http_vhost_traffic_status_find_lru_node(ngx_http_request_t *r,
    ngx_rbtree_node_t *a, ngx_rbtree_node_t *b)
{
    ngx_str_t                              filter;
    ngx_http_vhost_traffic_status_ctx_t   *ctx;
    ngx_http_vhost_traffic_status_node_t  *vtsn;

    ctx = ngx_http_get_module_main_conf(r, ngx_http_vhost_traffic_status_module);

    if (b != ctx->rbtree->sentinel) {
        vtsn = (ngx_http_vhost_traffic_status_node_t *) &b->color;

        if (vtsn->stat_upstream.type == NGX_HTTP_VHOST_TRAFFIC_STATUS_UPSTREAM_FG) {
            filter.data = vtsn->data;
            filter.len = vtsn->len;

            (void) ngx_http_vhost_traffic_status_node_position_key(&filter, 1);

            if (ngx_http_vhost_traffic_status_filter_max_node_match(r, &filter) == NGX_OK) {
                a = ngx_http_vhost_traffic_status_find_lru_node_cmp(r, a, b);
            }
        }

        a = ngx_http_vhost_traffic_status_find_lru_node(r, a, b->left);
        a = ngx_http_vhost_traffic_status_find_lru_node(r, a, b->right);
    }

    return a;
}


ngx_rbtree_node_t *
ngx_http_vhost_traffic_status_find_lru_node_cmp(ngx_http_request_t *r,
    ngx_rbtree_node_t *a, ngx_rbtree_node_t *b)
{
    ngx_int_t                                         ai, bi;
    ngx_http_vhost_traffic_status_node_t             *avtsn, *bvtsn;
    ngx_http_vhost_traffic_status_node_time_queue_t  *aq, *bq;

    if (a == NULL) {
        return b;
    }

    avtsn = (ngx_http_vhost_traffic_status_node_t *) &a->color;
    bvtsn = (ngx_http_vhost_traffic_status_node_t *) &b->color;

    aq = &avtsn->stat_request_times;
    bq = &bvtsn->stat_request_times;

    if (aq->front == aq->rear) {
        return a;
    }

    if (bq->front == bq->rear) {
        return b;
    }

    ai = ngx_http_vhost_traffic_status_node_time_queue_rear(aq);
    bi = ngx_http_vhost_traffic_status_node_time_queue_rear(bq);

    return (aq->times[ai].time < bq->times[bi].time) ? a : b;
}


ngx_rbtree_node_t *
ngx_http_vhost_traffic_status_node_lookup(ngx_rbtree_t *rbtree, ngx_str_t *key,
    uint32_t hash)
{
    ngx_int_t                              rc;
    ngx_rbtree_node_t                     *node, *sentinel;
    ngx_http_vhost_traffic_status_node_t  *vtsn;

    node = rbtree->root;
    sentinel = rbtree->sentinel;

    while (node != sentinel) {

        if (hash < node->key) {
            node = node->left;
            continue;
        }

        if (hash > node->key) {
            node = node->right;
            continue;
        }

        /* hash == node->key */

        vtsn = (ngx_http_vhost_traffic_status_node_t *) &node->color;

        rc = ngx_memn2cmp(key->data, vtsn->data, key->len, (size_t) vtsn->len);
        if (rc == 0) {
            return node;
        }

        node = (rc < 0) ? node->left : node->right;
    }

    return NULL;
}


void
ngx_http_vhost_traffic_status_node_zero(ngx_http_vhost_traffic_status_node_t *vtsn)
{
    vtsn->stat_request_counter = 0;
    vtsn->stat_in_bytes = 0;
    vtsn->stat_out_bytes = 0;
    vtsn->stat_1xx_counter = 0;
    vtsn->stat_100_counter = 0;
    vtsn->stat_101_counter = 0;
    vtsn->stat_102_counter = 0;
    vtsn->stat_2xx_counter = 0;
    vtsn->stat_200_counter = 0;
    vtsn->stat_201_counter = 0;
    vtsn->stat_202_counter = 0;
    vtsn->stat_203_counter = 0;
    vtsn->stat_204_counter = 0;
    vtsn->stat_205_counter = 0;
    vtsn->stat_206_counter = 0;
    vtsn->stat_207_counter = 0;
    vtsn->stat_208_counter = 0;
    vtsn->stat_226_counter = 0;
    vtsn->stat_3xx_counter = 0;
    vtsn->stat_300_counter = 0;
    vtsn->stat_301_counter = 0;
    vtsn->stat_302_counter = 0;
    vtsn->stat_303_counter = 0;
    vtsn->stat_304_counter = 0;
    vtsn->stat_305_counter = 0;
    vtsn->stat_307_counter = 0;
    vtsn->stat_308_counter = 0;
    vtsn->stat_4xx_counter = 0;
    vtsn->stat_400_counter = 0;
    vtsn->stat_401_counter = 0;
    vtsn->stat_402_counter = 0;
    vtsn->stat_403_counter = 0;
    vtsn->stat_404_counter = 0;
    vtsn->stat_405_counter = 0;
    vtsn->stat_406_counter = 0;
    vtsn->stat_407_counter = 0;
    vtsn->stat_408_counter = 0;
    vtsn->stat_409_counter = 0;
    vtsn->stat_410_counter = 0;
    vtsn->stat_411_counter = 0;
    vtsn->stat_412_counter = 0;
    vtsn->stat_413_counter = 0;
    vtsn->stat_414_counter = 0;
    vtsn->stat_415_counter = 0;
    vtsn->stat_416_counter = 0;
    vtsn->stat_417_counter = 0;
    vtsn->stat_418_counter = 0;
    vtsn->stat_421_counter = 0;
    vtsn->stat_422_counter = 0;
    vtsn->stat_423_counter = 0;
    vtsn->stat_424_counter = 0;
    vtsn->stat_426_counter = 0;
    vtsn->stat_428_counter = 0;
    vtsn->stat_429_counter = 0;
    vtsn->stat_431_counter = 0;
    vtsn->stat_444_counter = 0;
    vtsn->stat_451_counter = 0;
    vtsn->stat_499_counter = 0;
    vtsn->stat_5xx_counter = 0;
    vtsn->stat_500_counter = 0;
    vtsn->stat_501_counter = 0;
    vtsn->stat_502_counter = 0;
    vtsn->stat_503_counter = 0;
    vtsn->stat_504_counter = 0;
    vtsn->stat_505_counter = 0;
    vtsn->stat_506_counter = 0;
    vtsn->stat_507_counter = 0;
    vtsn->stat_508_counter = 0;
    vtsn->stat_510_counter = 0;
    vtsn->stat_511_counter = 0;
    vtsn->stat_599_counter = 0;

    vtsn->stat_request_time_counter = 0;
    vtsn->stat_request_time = 0;

    vtsn->stat_request_counter_oc = 0;
    vtsn->stat_in_bytes_oc = 0;
    vtsn->stat_out_bytes_oc = 0;
    vtsn->stat_1xx_counter_oc = 0;
    vtsn->stat_100_counter_oc = 0;
    vtsn->stat_101_counter_oc = 0;
    vtsn->stat_102_counter_oc = 0;
    vtsn->stat_2xx_counter_oc = 0;
    vtsn->stat_200_counter_oc = 0;
    vtsn->stat_201_counter_oc = 0;
    vtsn->stat_202_counter_oc = 0;
    vtsn->stat_203_counter_oc = 0;
    vtsn->stat_204_counter_oc = 0;
    vtsn->stat_205_counter_oc = 0;
    vtsn->stat_206_counter_oc = 0;
    vtsn->stat_207_counter_oc = 0;
    vtsn->stat_208_counter_oc = 0;
    vtsn->stat_226_counter_oc = 0;
    vtsn->stat_3xx_counter_oc = 0;
    vtsn->stat_300_counter_oc = 0;
    vtsn->stat_301_counter_oc = 0;
    vtsn->stat_302_counter_oc = 0;
    vtsn->stat_303_counter_oc = 0;
    vtsn->stat_304_counter_oc = 0;
    vtsn->stat_305_counter_oc = 0;
    vtsn->stat_307_counter_oc = 0;
    vtsn->stat_308_counter_oc = 0;
    vtsn->stat_4xx_counter_oc = 0;
    vtsn->stat_400_counter_oc = 0;
    vtsn->stat_401_counter_oc = 0;
    vtsn->stat_402_counter_oc = 0;
    vtsn->stat_403_counter_oc = 0;
    vtsn->stat_404_counter_oc = 0;
    vtsn->stat_405_counter_oc = 0;
    vtsn->stat_406_counter_oc = 0;
    vtsn->stat_407_counter_oc = 0;
    vtsn->stat_408_counter_oc = 0;
    vtsn->stat_409_counter_oc = 0;
    vtsn->stat_410_counter_oc = 0;
    vtsn->stat_411_counter_oc = 0;
    vtsn->stat_412_counter_oc = 0;
    vtsn->stat_413_counter_oc = 0;
    vtsn->stat_414_counter_oc = 0;
    vtsn->stat_415_counter_oc = 0;
    vtsn->stat_416_counter_oc = 0;
    vtsn->stat_417_counter_oc = 0;
    vtsn->stat_418_counter_oc = 0;
    vtsn->stat_421_counter_oc = 0;
    vtsn->stat_422_counter_oc = 0;
    vtsn->stat_423_counter_oc = 0;
    vtsn->stat_424_counter_oc = 0;
    vtsn->stat_426_counter_oc = 0;
    vtsn->stat_428_counter_oc = 0;
    vtsn->stat_429_counter_oc = 0;
    vtsn->stat_431_counter_oc = 0;
    vtsn->stat_444_counter_oc = 0;
    vtsn->stat_451_counter_oc = 0;
    vtsn->stat_499_counter_oc = 0;
    vtsn->stat_5xx_counter_oc = 0;
    vtsn->stat_500_counter_oc = 0;
    vtsn->stat_501_counter_oc = 0;
    vtsn->stat_502_counter_oc = 0;
    vtsn->stat_503_counter_oc = 0;
    vtsn->stat_504_counter_oc = 0;
    vtsn->stat_505_counter_oc = 0;
    vtsn->stat_506_counter_oc = 0;
    vtsn->stat_507_counter_oc = 0;
    vtsn->stat_508_counter_oc = 0;
    vtsn->stat_510_counter_oc = 0;
    vtsn->stat_511_counter_oc = 0;
    vtsn->stat_599_counter_oc = 0;
    vtsn->stat_request_time_counter_oc = 0;
    vtsn->stat_response_time_counter_oc = 0;

#if (NGX_HTTP_CACHE)
    vtsn->stat_cache_miss_counter = 0;
    vtsn->stat_cache_bypass_counter = 0;
    vtsn->stat_cache_expired_counter = 0;
    vtsn->stat_cache_stale_counter = 0;
    vtsn->stat_cache_updating_counter = 0;
    vtsn->stat_cache_revalidated_counter = 0;
    vtsn->stat_cache_hit_counter = 0;
    vtsn->stat_cache_scarce_counter = 0;

    vtsn->stat_cache_miss_counter_oc = 0;
    vtsn->stat_cache_bypass_counter_oc = 0;
    vtsn->stat_cache_expired_counter_oc = 0;
    vtsn->stat_cache_stale_counter_oc = 0;
    vtsn->stat_cache_updating_counter_oc = 0;
    vtsn->stat_cache_revalidated_counter_oc = 0;
    vtsn->stat_cache_hit_counter_oc = 0;
    vtsn->stat_cache_scarce_counter_oc = 0;
#endif

}

/*
   Initialize the node and update it with the first request.
   Set the `stat_request_time` to the time of the first request.
*/
void
ngx_http_vhost_traffic_status_node_init(ngx_http_request_t *r,
    ngx_http_vhost_traffic_status_node_t *vtsn)
{
    ngx_msec_int_t                             ms;

    /* init serverZone */
    ngx_http_vhost_traffic_status_node_zero(vtsn);
    ngx_http_vhost_traffic_status_node_time_queue_init(&vtsn->stat_request_times);
    ngx_http_vhost_traffic_status_node_histogram_bucket_init(r, &vtsn->stat_request_buckets);

    /* init upstreamZone */
    vtsn->stat_upstream.type = NGX_HTTP_VHOST_TRAFFIC_STATUS_UPSTREAM_NO;
    vtsn->stat_upstream.response_time_counter = 0;
    vtsn->stat_upstream.response_time = 0;
    ngx_http_vhost_traffic_status_node_time_queue_init(&vtsn->stat_upstream.response_times);
    ngx_http_vhost_traffic_status_node_histogram_bucket_init(r,
        &vtsn->stat_upstream.response_buckets);

    /* set serverZone */
    ms = ngx_http_vhost_traffic_status_request_time(r);
    vtsn->stat_request_time = (ngx_msec_t) ms;

    ngx_http_vhost_traffic_status_node_update(r, vtsn, ms);
}

/*
   Update the node from a subsequent request. Now there is more than one request,
   calculate the average request time.
*/
void
ngx_http_vhost_traffic_status_node_set(ngx_http_request_t *r,
    ngx_http_vhost_traffic_status_node_t *vtsn)
{
    ngx_msec_int_t                             ms;
    ngx_http_vhost_traffic_status_node_t       ovtsn;
    ngx_http_vhost_traffic_status_loc_conf_t  *vtscf;

    vtscf = ngx_http_get_module_loc_conf(r, ngx_http_vhost_traffic_status_module);

    ovtsn = *vtsn;

    ms = ngx_http_vhost_traffic_status_request_time(r);
    ngx_http_vhost_traffic_status_node_update(r, vtsn, ms);

    vtsn->stat_request_time = ngx_http_vhost_traffic_status_node_time_queue_average(
                                  &vtsn->stat_request_times, vtscf->average_method,
                                  vtscf->average_period);

    ngx_http_vhost_traffic_status_add_oc((&ovtsn), vtsn);
}

void
ngx_http_vhost_traffic_status_node_update(ngx_http_request_t *r,
    ngx_http_vhost_traffic_status_node_t *vtsn, ngx_msec_int_t ms)
{
    ngx_uint_t status = r->headers_out.status;

    vtsn->stat_request_counter++;
    vtsn->stat_in_bytes += (ngx_atomic_uint_t) r->request_length;
    vtsn->stat_out_bytes += (ngx_atomic_uint_t) r->connection->sent;

    ngx_http_vhost_traffic_status_add_rc(status, vtsn);

    vtsn->stat_request_time_counter += (ngx_atomic_uint_t) ms;

    ngx_http_vhost_traffic_status_node_time_queue_insert(&vtsn->stat_request_times,
                                                         ms);

    ngx_http_vhost_traffic_status_node_histogram_observe(&vtsn->stat_request_buckets,
                                                         ms);

#if (NGX_HTTP_CACHE)
    if (r->upstream != NULL && r->upstream->cache_status != 0) {
        ngx_http_vhost_traffic_status_add_cc(r->upstream->cache_status, vtsn);
    }
#endif
}

void
ngx_http_vhost_traffic_status_node_time_queue_zero(
    ngx_http_vhost_traffic_status_node_time_queue_t *q)
{
    ngx_memzero(q, sizeof(ngx_http_vhost_traffic_status_node_time_queue_t));
}


void
ngx_http_vhost_traffic_status_node_time_queue_init(
    ngx_http_vhost_traffic_status_node_time_queue_t *q)
{
    ngx_http_vhost_traffic_status_node_time_queue_zero(q);
    q->rear = NGX_HTTP_VHOST_TRAFFIC_STATUS_DEFAULT_QUEUE_LEN - 1;
    q->len = NGX_HTTP_VHOST_TRAFFIC_STATUS_DEFAULT_QUEUE_LEN;
}


ngx_int_t
ngx_http_vhost_traffic_status_node_time_queue_push(
    ngx_http_vhost_traffic_status_node_time_queue_t *q,
    ngx_msec_int_t x)
{
    if ((q->rear + 1) % q->len == q->front) {
        return NGX_ERROR;
    }

    q->times[q->rear].time = ngx_http_vhost_traffic_status_current_msec();
    q->times[q->rear].msec = x;
    q->rear = (q->rear + 1) % q->len;

    return NGX_OK;
}


ngx_int_t
ngx_http_vhost_traffic_status_node_time_queue_pop(
    ngx_http_vhost_traffic_status_node_time_queue_t *q,
    ngx_http_vhost_traffic_status_node_time_t *x)
{
    if (q->front == q->rear) {
        return NGX_ERROR;
    }

    *x = q->times[q->front];
    q->front = (q->front + 1) % q->len;

    return NGX_OK;
}


ngx_int_t
ngx_http_vhost_traffic_status_node_time_queue_rear(
    ngx_http_vhost_traffic_status_node_time_queue_t *q)
{
    return (q->rear > 0) ? (q->rear - 1) : (NGX_HTTP_VHOST_TRAFFIC_STATUS_DEFAULT_QUEUE_LEN - 1);
}


void
ngx_http_vhost_traffic_status_node_time_queue_insert(
    ngx_http_vhost_traffic_status_node_time_queue_t *q,
    ngx_msec_int_t x)
{
    ngx_int_t                                  rc;
    ngx_http_vhost_traffic_status_node_time_t  rx;
    rc = ngx_http_vhost_traffic_status_node_time_queue_pop(q, &rx)
         | ngx_http_vhost_traffic_status_node_time_queue_push(q, x);

    if (rc != NGX_OK) {
        ngx_http_vhost_traffic_status_node_time_queue_init(q);
    }
}


ngx_msec_t
ngx_http_vhost_traffic_status_node_time_queue_average(
    ngx_http_vhost_traffic_status_node_time_queue_t *q,
    ngx_int_t method, ngx_msec_t period)
{
    ngx_msec_t  avg;

    if (method == NGX_HTTP_VHOST_TRAFFIC_STATUS_AVERAGE_METHOD_AMM) {
        avg = ngx_http_vhost_traffic_status_node_time_queue_amm(q, period);
    } else {
        avg = ngx_http_vhost_traffic_status_node_time_queue_wma(q, period);
    }

    return avg;
}


ngx_msec_t
ngx_http_vhost_traffic_status_node_time_queue_amm(
    ngx_http_vhost_traffic_status_node_time_queue_t *q,
    ngx_msec_t period)
{
    ngx_int_t   i, j, k;
    ngx_msec_t  x, current_msec;

    ngx_int_t   count = 0;

    current_msec = ngx_http_vhost_traffic_status_current_msec();

    x = period ? (current_msec - period) : 0;

    for (i = q->front, j = 1, k = 0; i != q->rear; i = (i + 1) % q->len, j++) {
        if (x < q->times[i].time) {
            k += (ngx_int_t) q->times[i].msec;
            count++;
        }
    }

    if (j != q->len) {
        ngx_http_vhost_traffic_status_node_time_queue_init(q);
    }

    if (count == 0)
        return (ngx_msec_t)0;
    else
        return (ngx_msec_t)(k / count);
}


ngx_msec_t
ngx_http_vhost_traffic_status_node_time_queue_wma(
    ngx_http_vhost_traffic_status_node_time_queue_t *q,
    ngx_msec_t period)
{
    ngx_int_t   i, j, k;
    ngx_msec_t  x, current_msec;

    ngx_int_t   count = 0;

    current_msec = ngx_http_vhost_traffic_status_current_msec();

    x = period ? (current_msec - period) : 0;

    for (i = q->front, j = 1, k = 0; i != q->rear; i = (i + 1) % q->len, j++) {
        if (x < q->times[i].time) {
            count++;
            k += (ngx_int_t) q->times[i].msec * count;
        }
    }

    if (j != q->len) {
        ngx_http_vhost_traffic_status_node_time_queue_init(q);
    }

    if (count == 0)
        return (ngx_msec_t)0;
    else
        return (ngx_msec_t)
               (k / (ngx_int_t) ngx_http_vhost_traffic_status_triangle(count));
}


void
ngx_http_vhost_traffic_status_node_time_queue_merge(
    ngx_http_vhost_traffic_status_node_time_queue_t *a,
    ngx_http_vhost_traffic_status_node_time_queue_t *b,
    ngx_msec_t period)
{
    ngx_int_t   i;
    ngx_msec_t  x, current_msec;

    current_msec = ngx_http_vhost_traffic_status_current_msec();

    x = period ? (current_msec - period) : 0;

    for (i = a->front; i != a->rear; i = (i + 1) % a->len) {
            a->times[i].time = (a->times[i].time > b->times[i].time)
                               ? a->times[i].time
                               : b->times[i].time;

            if (x < a->times[i].time) {
                a->times[i].msec = (a->times[i].msec + b->times[i].msec) / 2
                                   + (a->times[i].msec + b->times[i].msec) % 2;

            } else {
                a->times[i].msec = 0;
            }
    }
}


void
ngx_http_vhost_traffic_status_node_histogram_bucket_init(ngx_http_request_t *r,
    ngx_http_vhost_traffic_status_node_histogram_bucket_t *b)
{
    ngx_uint_t                                       i, n;
    ngx_http_vhost_traffic_status_loc_conf_t        *vtscf;
    ngx_http_vhost_traffic_status_node_histogram_t  *buckets;

    vtscf = ngx_http_get_module_loc_conf(r, ngx_http_vhost_traffic_status_module);

    if (vtscf->histogram_buckets == NULL) {
        b->len = 0;
        return;
    }

    buckets = vtscf->histogram_buckets->elts;
    n = vtscf->histogram_buckets->nelts;
    b->len = n;

    for (i = 0; i < n; i++) {
        b->buckets[i].msec = buckets[i].msec;
        b->buckets[i].counter = 0;
    }
}


void
ngx_http_vhost_traffic_status_node_histogram_observe(
    ngx_http_vhost_traffic_status_node_histogram_bucket_t *b,
    ngx_msec_int_t x)
{
    ngx_uint_t  i, n;

    n = b->len;

    for (i = 0; i < n; i++) {
        if (x <= b->buckets[i].msec) {
            b->buckets[i].counter++;
        }
    }
}


ngx_int_t
ngx_http_vhost_traffic_status_node_member_cmp(ngx_str_t *member, const char *name)
{
    if (member->len == ngx_strlen(name) && ngx_strncmp(name, member->data, member->len) == 0) {
        return 0;
    }

    return 1;
}


ngx_atomic_uint_t
ngx_http_vhost_traffic_status_node_member(ngx_http_vhost_traffic_status_node_t *vtsn,
    ngx_str_t *member)
{
    if (ngx_http_vhost_traffic_status_node_member_cmp(member, "request") == 0)
    {
        return vtsn->stat_request_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "in") == 0)
    {
        return vtsn->stat_in_bytes;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "out") == 0)
    {
        return vtsn->stat_out_bytes;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "1xx") == 0)
    {
        return vtsn->stat_1xx_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "2xx") == 0)
    {
        return vtsn->stat_2xx_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "3xx") == 0)
    {
        return vtsn->stat_3xx_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "4xx") == 0)
    {
        return vtsn->stat_4xx_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "5xx") == 0)
    {
        return vtsn->stat_5xx_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "100") == 0)
    {
        return vtsn->stat_100_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "101") == 0)
    {
        return vtsn->stat_101_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "102") == 0)
    {
        return vtsn->stat_102_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "200") == 0)
    {
        return vtsn->stat_200_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "201") == 0)
    {
        return vtsn->stat_201_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "202") == 0)
    {
        return vtsn->stat_202_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "203") == 0)
    {
        return vtsn->stat_203_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "204") == 0)
    {
        return vtsn->stat_204_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "205") == 0)
    {
        return vtsn->stat_205_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "206") == 0)
    {
        return vtsn->stat_206_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "207") == 0)
    {
        return vtsn->stat_207_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "208") == 0)
    {
        return vtsn->stat_208_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "226") == 0)
    {
        return vtsn->stat_226_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "300") == 0)
    {
        return vtsn->stat_300_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "301") == 0)
    {
        return vtsn->stat_301_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "302") == 0)
    {
        return vtsn->stat_302_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "303") == 0)
    {
        return vtsn->stat_303_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "304") == 0)
    {
        return vtsn->stat_304_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "305") == 0)
    {
        return vtsn->stat_305_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "307") == 0)
    {
        return vtsn->stat_307_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "308") == 0)
    {
        return vtsn->stat_308_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "400") == 0)
    {
        return vtsn->stat_400_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "401") == 0)
    {
        return vtsn->stat_401_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "402") == 0)
    {
        return vtsn->stat_402_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "403") == 0)
    {
        return vtsn->stat_403_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "404") == 0)
    {
        return vtsn->stat_404_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "405") == 0)
    {
        return vtsn->stat_405_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "406") == 0)
    {
        return vtsn->stat_406_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "407") == 0)
    {
        return vtsn->stat_407_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "408") == 0)
    {
        return vtsn->stat_408_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "409") == 0)
    {
        return vtsn->stat_409_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "410") == 0)
    {
        return vtsn->stat_410_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "411") == 0)
    {
        return vtsn->stat_411_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "412") == 0)
    {
        return vtsn->stat_412_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "413") == 0)
    {
        return vtsn->stat_413_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "414") == 0)
    {
        return vtsn->stat_414_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "415") == 0)
    {
        return vtsn->stat_415_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "416") == 0)
    {
        return vtsn->stat_416_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "417") == 0)
    {
        return vtsn->stat_417_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "418") == 0)
    {
        return vtsn->stat_418_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "421") == 0)
    {
        return vtsn->stat_421_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "422") == 0)
    {
        return vtsn->stat_422_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "423") == 0)
    {
        return vtsn->stat_423_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "424") == 0)
    {
        return vtsn->stat_424_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "426") == 0)
    {
        return vtsn->stat_426_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "428") == 0)
    {
        return vtsn->stat_428_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "429") == 0)
    {
        return vtsn->stat_429_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "431") == 0)
    {
        return vtsn->stat_431_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "444") == 0)
    {
        return vtsn->stat_444_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "451") == 0)
    {
        return vtsn->stat_451_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "499") == 0)
    {
        return vtsn->stat_499_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "500") == 0)
    {
        return vtsn->stat_500_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "501") == 0)
    {
        return vtsn->stat_501_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "502") == 0)
    {
        return vtsn->stat_502_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "503") == 0)
    {
        return vtsn->stat_503_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "504") == 0)
    {
        return vtsn->stat_504_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "505") == 0)
    {
        return vtsn->stat_505_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "506") == 0)
    {
        return vtsn->stat_506_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "507") == 0)
    {
        return vtsn->stat_507_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "508") == 0)
    {
        return vtsn->stat_508_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "510") == 0)
    {
        return vtsn->stat_510_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "511") == 0)
    {
        return vtsn->stat_511_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "599") == 0)
    {
        return vtsn->stat_599_counter;
    }

#if (NGX_HTTP_CACHE)
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "cache_miss") == 0)
    {
        return vtsn->stat_cache_miss_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "cache_bypass") == 0)
    {
        return vtsn->stat_cache_bypass_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "cache_expired") == 0)
    {
        return vtsn->stat_cache_expired_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "cache_stale") == 0)
    {
        return vtsn->stat_cache_stale_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "cache_updating") == 0)
    {
        return vtsn->stat_cache_updating_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "cache_revalidated") == 0)
    {
        return vtsn->stat_cache_revalidated_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "cache_hit") == 0)
    {
        return vtsn->stat_cache_hit_counter;
    }
    else if (ngx_http_vhost_traffic_status_node_member_cmp(member, "cache_scarce") == 0)
    {
        return vtsn->stat_cache_scarce_counter;
    }
#endif

    return 0;
}

/* vi:set ft=c ts=4 sw=4 et fdm=marker: */
