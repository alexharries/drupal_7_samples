<?php

/**
 * @file
 * Administration functions for the fcl_ribbons_flights_product_listing module.
 */

/**
 * Configuration form for the module.
 *
 * @return array
 *   A Drupal FAPI-compatible form array.
 */
function fcl_ribbons_flights_product_listing_admin_form() {
  $form = [];
  
  $form['fcl_ribbons_flights_products_listing_solr_server_id'] = [
    '#title' => t('Solr server ID'),
    '#type' => 'textfield',
    '#default_value' => variable_get('fcl_ribbons_flights_products_listing_solr_server_id', FCL_RIBBONS_FLIGHTS_PRODUCT_LISTING_SOLR_SERVER_ID_DEFAULT),
    '#required' => TRUE,
  ];
  
  $search_results_limit_options = [
    10 => 10,
    20 => 20,
    50 => 50,
    100 => 100,
    200 => 200,
    FCL_RIBBONS_FLIGHTS_PRODUCT_LISTING_SEARCH_RESULTS_UNLIMITED => t('Unlimited'),
  ];
  
  $form['fcl_ribbons_flights_products_listing_search_results_limit'] = [
    '#title' => t('Maximum number of results to show per table'),
    '#type' => 'select',
    '#default_value' => variable_get('fcl_ribbons_flights_products_listing_search_results_limit', FCL_RIBBONS_FLIGHTS_PRODUCT_LISTING_SEARCH_RESULTS_DEFAULT_LIMIT),
    '#options' => $search_results_limit_options,
  ];
  
  $form['fcl_ribbons_flights_product_listing_cache_enabled'] = [
    '#title' => t('Cache flights listing results?'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('fcl_ribbons_flights_product_listing_cache_enabled', FCL_RIBBONS_FLIGHTS_PRODUCT_LISTING_CACHE_ENABLED_DEFAULT),
  ];
  
  $cache_expiry_options = [
    1 * 60 * 1 => t('1 minute'),
    1 * 60 * 5 => t('5 minutes'),
    1 * 60 * 15 => t('15 minutes'),
    1 * 60 * 60 => t('1 hour'),
    1 * 60 * 60 * 2 => t('2 hours'),
    1 * 60 * 60 * 6 => t('6 hours'),
    1 * 60 * 60 * 12 => t('12 hours'),
    1 * 60 * 60 * 24 => t("Now you're being silly (1 day)"),
  ];
  
  $form['fcl_ribbons_flights_products_listing_cache_expiry'] = [
    '#title' => t('How long should results be cached before being regenerated?'),
    '#type' => 'select',
    '#default_value' => variable_get('fcl_ribbons_flights_products_listing_cache_expiry', FCL_RIBBONS_FLIGHTS_PRODUCT_LISTING_CACHE_EXPIRY_DEFAULT),
    '#options' => $cache_expiry_options,
    '#states' => [
      'visible' => [
        ':input[name="fcl_ribbons_flights_product_listing_cache_enabled"]' => ['checked' => TRUE],
      ],
    ],
  ];
  
  return system_settings_form($form);
}
