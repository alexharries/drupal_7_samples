<?php

/**
 * @file
 * Autoloader(-ish) functionality.
 */

/**
 * Auto-load any .inc files found in a module's ./includes directory.
 *
 * @param string $module_name
 *   The name of the module to load include files for.
 *
 * @param string $includes_directory
 *   The directory name in which include files (with the extension .inc) can be
 *   found. Defaults to "includes".
 */
function fcl_uk_utilities_autoload_includes($module_name, $includes_directory = 'includes') {
  $module_path = DRUPAL_ROOT . '/' . drupal_get_path('module', $module_name);
  $include_files = (array)file_scan_directory($module_path . '/' . $includes_directory, '/\.inc/');
  
  foreach ($include_files as $include_file_info) {
    require_once $include_file_info->uri;
  }
}
