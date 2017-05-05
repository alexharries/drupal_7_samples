<?php
/**
 * @file
 * Provides Solr functionality for fcl_uk_utilities.
 */

/**
 * Converts a PHP array to a Solr query array string.
 *
 * @param array  $values
 *   PHP array of values to be converted.
 *
 * @param string $condition
 *   The search condition, e.g. "OR" or "AND". Defaults to "OR" if not
 *   specified.
 *
 * @return string
 *   A search string in Solr array format. E.g. '("foo" OR "bar" OR "baz")'.
 */
function fcl_uk_utilities_array_to_solr_query(array $values, $condition = "OR") {
  foreach ($values as $key => $value) {
    $value = trim($value, ' "');
    
    if (!is_numeric($value)) {
      $value = '"' . $value . '"';
    }
    
    $values[$key] = $value;
  }
  
  return '(' . implode(" $condition ", $values) . ')';
}
