<?php
/**
 * @file
 * Airline-related functions.
 */

/**
 * Converts an airline's name (e.g. "British Scareways") to a code such as BS.
 *
 * @param $airline_names
 *
 * @return array|mixed
 */
function fcl_uk_utilities_airline_names_to_airline_codes($airline_names) {
  // If $airline_names is a string, cast it to array.
  $return_string = FALSE;
  if (is_string($airline_names)) {
    $return_string = TRUE;
    $airline_names = [$airline_names];
  }
  
  $airline_codes = [];
  
  // Loop through each airline name, using taxonomy_get_term_by_name() to try
  // and find an appropriate taxonomy term.
  foreach ($airline_names as $airline_name) {
    $airline_terms = taxonomy_get_term_by_name($airline_name, 'fcl_product_supplier');
    
    // We found one or more matching taxonomy terms. Loop through each result
    // until we find one with an airline code; then, we take the first one we
    // come to, which is possibly a bit risky as it could lead to the wrong
    // airline code being selected, but we don't have a better approach just at
    // the moment.
    if (!empty($airline_terms)) {
      foreach ($airline_terms as $airline_term) {
        // If the airline taxonomy term has an airline code, we're in business.
        if (!empty($airline_term->field_airline_code) &&
          !empty($airline_term->field_airline_code[LANGUAGE_NONE][0]['safe_value'])
        ) {
          $airline_codes[] = $airline_term->field_airline_code[LANGUAGE_NONE][0]['safe_value'];
          break;
        }
        // Otherwise, we keep going...
      }
    }
  }
  
  // Return either a single airline code if we were provided a single
  // airline name as a string, or an array of airline names.
  if ($return_string) {
    return reset($airline_codes);
  }
  
  return $airline_codes;
}
