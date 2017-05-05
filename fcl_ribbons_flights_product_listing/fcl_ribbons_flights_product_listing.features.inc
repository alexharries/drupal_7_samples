<?php
/**
 * @file
 * fcl_ribbons_flights_product_listing.features.inc
 */

/**
 * Implements hook_paragraphs_info().
 */
function fcl_ribbons_flights_product_listing_paragraphs_info() {
  $items = array(
    'flights_product_listing' => array(
      'name' => 'Flights Product Listing',
      'bundle' => 'flights_product_listing',
      'locked' => '1',
    ),
  );
  return $items;
}
