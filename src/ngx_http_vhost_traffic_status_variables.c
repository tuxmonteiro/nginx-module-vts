
/*
 * Copyright (C) YoungJoo Kim (vozlt)
 */


#include "ngx_http_vhost_traffic_status_module.h"
#include "ngx_http_vhost_traffic_status_variables.h"


static ngx_http_variable_t  ngx_http_vhost_traffic_status_vars[] = {

    { ngx_string("vts_request_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_request_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_in_bytes"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_in_bytes),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_out_bytes"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_out_bytes),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_1xx_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_1xx_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_2xx_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_2xx_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_3xx_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_3xx_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_4xx_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_4xx_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_5xx_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_5xx_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_100_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_100_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_101_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_101_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_102_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_102_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_200_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_200_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_201_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_201_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_202_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_202_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_203_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_203_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_204_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_204_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_205_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_205_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_206_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_206_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_207_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_207_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_208_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_208_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_226_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_226_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_300_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_300_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_301_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_301_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_302_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_302_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_303_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_303_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_304_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_304_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_305_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_305_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_307_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_307_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_308_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_308_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_400_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_400_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_401_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_401_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_402_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_402_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_403_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_403_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_404_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_404_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_405_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_405_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_406_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_406_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_407_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_407_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_408_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_408_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_409_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_409_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_410_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_410_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_411_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_411_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_412_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_412_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_413_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_413_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_414_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_414_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_415_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_415_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_416_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_416_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_417_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_417_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_418_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_418_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_421_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_421_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_422_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_422_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_423_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_423_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_424_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_424_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_426_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_426_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_428_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_428_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_429_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_429_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_431_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_431_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_444_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_444_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_451_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_451_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_499_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_499_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_500_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_500_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_501_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_501_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_502_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_502_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_503_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_503_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_504_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_504_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_505_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_505_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_506_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_506_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_507_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_507_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_508_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_508_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_510_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_510_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_511_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_511_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_599_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_599_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_request_time_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_request_time_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_request_time"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_request_time),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

#if (NGX_HTTP_CACHE)
    { ngx_string("vts_cache_miss_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_cache_miss_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_cache_bypass_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_cache_bypass_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_cache_expired_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_cache_expired_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_cache_stale_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_cache_stale_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_cache_updating_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_cache_updating_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_cache_revalidated_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_cache_revalidated_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_cache_hit_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_cache_hit_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("vts_cache_scarce_counter"), NULL,
      ngx_http_vhost_traffic_status_node_variable,
      offsetof(ngx_http_vhost_traffic_status_node_t, stat_cache_scarce_counter),
      NGX_HTTP_VAR_NOCACHEABLE, 0 },
#endif

    { ngx_null_string, NULL, NULL, 0, 0, 0 }
};


ngx_int_t
ngx_http_vhost_traffic_status_node_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data)
{
    u_char                                    *p;
    unsigned                                   type;
    ngx_int_t                                  rc;
    ngx_str_t                                  key, dst;
    ngx_slab_pool_t                           *shpool;
    ngx_rbtree_node_t                         *node;
    ngx_http_vhost_traffic_status_node_t      *vtsn;
    ngx_http_vhost_traffic_status_loc_conf_t  *vtscf;

    vtscf = ngx_http_get_module_loc_conf(r, ngx_http_vhost_traffic_status_module);

    ngx_http_vhost_traffic_status_find_name(r, &dst);

    type = NGX_HTTP_VHOST_TRAFFIC_STATUS_UPSTREAM_NO;

    rc = ngx_http_vhost_traffic_status_node_generate_key(r->pool, &key, &dst, type);
    if (rc != NGX_OK) {
        return NGX_ERROR;
    }

    if (key.len == 0) {
        return NGX_ERROR;
    }

    shpool = (ngx_slab_pool_t *) vtscf->shm_zone->shm.addr;

    ngx_shmtx_lock(&shpool->mutex);

    node = ngx_http_vhost_traffic_status_find_node(r, &key, type, 0);

    if (node == NULL) {
        goto not_found;
    }

    p = ngx_pnalloc(r->pool, NGX_ATOMIC_T_LEN);
    if (p == NULL) {
        goto not_found;
    }

    vtsn = (ngx_http_vhost_traffic_status_node_t *) &node->color;

    v->len = ngx_sprintf(p, "%uA", *((ngx_atomic_t *) ((char *) vtsn + data))) - p;
    v->valid = 1;
    v->no_cacheable = 0;
    v->not_found = 0;
    v->data = p;

    goto done;

not_found:

    v->not_found = 1;

done:

    vtscf->node_caches[type] = node;

    ngx_shmtx_unlock(&shpool->mutex);

    return NGX_OK;
}


ngx_int_t
ngx_http_vhost_traffic_status_add_variables(ngx_conf_t *cf)
{
    ngx_http_variable_t  *var, *v;

    for (v = ngx_http_vhost_traffic_status_vars; v->name.len; v++) {
        var = ngx_http_add_variable(cf, &v->name, v->flags);
        if (var == NULL) {
            return NGX_ERROR;
        }

        var->get_handler = v->get_handler;
        var->data = v->data;
    }

    return NGX_OK;
}

/* vi:set ft=c ts=4 sw=4 et fdm=marker: */
