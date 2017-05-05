<?php
/**
 * @file
 * Legacy mode-related functionality.
 */

/**
 * Allow modules / themes to trigger legacy mode based on custom contexts.
 *
 * @return bool
 *   TRUE for legacy mode.
 */
function fcl_uk_utilities_is_legacy_mode() {
  // Default is NOT legacy. That means it is the site-specific responsibility to
  // inform of legacy contexts.
  $is_legacy_mode = FALSE;
  drupal_alter('is_legacy_mode', $is_legacy_mode);
  return $is_legacy_mode;
}
