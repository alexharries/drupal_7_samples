<?php
/**
 * @file
 * Workbench-related functions.
 */

/**
 * Fetch the node reference when we're using workbench and draft versions.
 */
function fcl_uk_utilities_get_page_node() {
  $node = &drupal_static(__FUNCTION__);
  if (!isset($node)) {
    $node = menu_get_object();
    if (!empty($node)) {
      if (module_exists('workbench_moderation')) {
        $request_path = request_path();
        $current_paths = array('/^node\/(\d+)\/draft/', '/^node\/(\d+)\/current-revision/');
        foreach ($current_paths as $path) {
          if (preg_match($path, $request_path)) {
            $node = workbench_moderation_node_current_load($node);
          }
        }
      }
    }
    else {
      $node = FALSE;
    }
  }
  return $node ? $node : NULL;
}
