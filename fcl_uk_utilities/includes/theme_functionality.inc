<?php
/**
 * @file
 * Theme-related functionality.
 */

/**
 * If the element isn't empty, print it with *fixes.
 *
 * This is a helper function for cleaner code in the .tpl files.
 *
 * @param array  $element
 *   The element to render.
 * @param string $prefix
 *   Prefix, e.g. '<p>'.
 * @param string $suffix
 *   Suffix, e.g. '</p>'.
 */
function fcl_uk_utilities_print($element, $prefix = '', $suffix = '') {
  $markup = is_array($element) ? render($element) : $element;
  if (!empty($markup)) {
    echo $prefix . $markup . $suffix;
  }
}

/**
 * Initialise variables to avoid undefined variable notices in .tpl files.
 */
function fcl_uk_utilities_initialise_expected_vars(&$variables, $expected_variables) {
  foreach ($expected_variables as $expected_variable) {
    if (!isset($variables[$expected_variable])) {
      $variables[$expected_variable] = NULL;
    }
  }
}
