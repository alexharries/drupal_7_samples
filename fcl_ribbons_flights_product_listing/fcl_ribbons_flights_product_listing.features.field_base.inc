<?php
/**
 * @file
 * fcl_ribbons_flights_product_listing.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function fcl_ribbons_flights_product_listing_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_productlist_flts_airlns'.
  $field_bases['field_productlist_flts_airlns'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_productlist_flts_airlns',
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'max_length' => 255,
    ),
    'translatable' => 0,
    'type' => 'text',
  );

  // Exported field_base: 'field_productlist_flts_classes'.
  $field_bases['field_productlist_flts_classes'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_productlist_flts_classes',
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        'Economy' => 'Economy',
        'Premium Economy' => 'Premium Economy',
        'Business' => 'Business',
        'First' => 'First',
      ),
      'allowed_values_function' => '',
    ),
    'translatable' => 0,
    'type' => 'list_text',
  );

  // Exported field_base: 'field_productlist_flts_dest'.
  $field_bases['field_productlist_flts_dest'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_productlist_flts_dest',
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(),
    'translatable' => 0,
    'type' => 'text_long',
  );

  // Exported field_base: 'field_productlist_flts_expts'.
  $field_bases['field_productlist_flts_expts'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_productlist_flts_expts',
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(),
    'translatable' => 0,
    'type' => 'text_long',
  );

  // Exported field_base: 'field_productlist_max_results'.
  $field_bases['field_productlist_max_results'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_productlist_max_results',
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        -1 => 'Default (set by administrator)',
        10 => 10,
        20 => 20,
        50 => 50,
        100 => 100,
        200 => 200,
        2147483647 => 'Unlimited',
      ),
      'allowed_values_function' => '',
    ),
    'translatable' => 0,
    'type' => 'list_integer',
  );

  return $field_bases;
}
