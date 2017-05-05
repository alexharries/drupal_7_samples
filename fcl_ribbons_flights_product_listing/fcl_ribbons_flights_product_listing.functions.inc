<?php

/**
 * @file
 * Functions which aren't Drupal hooks live in this file.
 */

/**
 * Constant definitions.
 */

// Whether caching is enabled by default or not.
define('FCL_RIBBONS_FLIGHTS_PRODUCT_LISTING_CACHE_ENABLED_DEFAULT', TRUE);

// Cache expiry default time, in seconds.
define('FCL_RIBBONS_FLIGHTS_PRODUCT_LISTING_CACHE_EXPIRY_DEFAULT', 1 * 60 * 15);

// Default Solr server ID.
define('FCL_RIBBONS_FLIGHTS_PRODUCT_LISTING_SOLR_SERVER_ID_DEFAULT', 'acquia_search_server_1');

// This flag indicates that a paragraph's maximum results should come from the
// module's defaults.
define('FCL_RIBBONS_FLIGHTS_PRODUCT_LISTING_SEARCH_RESULTS_DEFAULT', -1);

// Default maximum number of results.
define('FCL_RIBBONS_FLIGHTS_PRODUCT_LISTING_SEARCH_RESULTS_UNLIMITED', 2147483647);

// Default maximum number of results.
define('FCL_RIBBONS_FLIGHTS_PRODUCT_LISTING_SEARCH_RESULTS_DEFAULT_LIMIT', 50);

/**
 * Convert the contents of a textarea into an array of values.
 *
 * The textarea's contents will contain one or more items, separated by either
 * commas or new lines (i.e. one per line, or all on one line and comma-
 * separated).
 *
 * @param string $textarea_safe_value
 *   The textarea's contents, taken from the Field API safe_value array key.
 *   If safe_value isn't available, you should run the text through a
 *   sanitiser function, for example check_plain().
 *
 * @return array
 *   An array containing any items found.
 */
function _fcl_ribbons_flights_product_listing_textarea_to_array($textarea_safe_value) {
  $textarea_safe_value_trimmed = trim($textarea_safe_value);
  
  // If the destinations have been entered one-per-line, explode them into an
  // array.
  if (strstr($textarea_safe_value_trimmed, PHP_EOL)) {
    $textarea_safe_value_array = explode(PHP_EOL, $textarea_safe_value_trimmed);
  }
  // Otherwise, if the destinations are in a comma-separated list, break that
  // up into an array.
  elseif (strstr($textarea_safe_value_trimmed, ',')) {
    $textarea_safe_value_array = explode(',', $textarea_safe_value_trimmed);
  }
  // Otherwise, it might just be a single destination; cast it to an array.
  else {
    $textarea_safe_value_array = (array) $textarea_safe_value_trimmed;
  }
  
  // Trim each value.
  foreach ($textarea_safe_value_array as &$textarea_safe_value_array_item) {
    $textarea_safe_value_array_item = trim($textarea_safe_value_array_item);
  }
  
  // To help us make a cache ID which works no matter what order the
  // items are listed, arrange results alphabetically.
  sort($textarea_safe_value_array);
  
  return $textarea_safe_value_array;
}
