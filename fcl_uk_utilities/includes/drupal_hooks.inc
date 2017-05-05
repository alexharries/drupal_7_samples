<?php
/**
 * @file
 * Drupal hook implementations.
 */

/**
 * Implements hook_theme().
 */
function fcl_uk_utilities_theme($existing, $type, $theme, $path) {
  $module_templates_prefix = 'fcl-uk-utilities-';
  $templates_directory = drupal_get_path('module', 'fcl_uk_utilities') . '/templates';
  
  return [
    // Displays a from-to link in a destinations table.
    'fcl_uk_utilities_from_to_link' => [
      'variables' => [
        'from' => NULL,
        'to' => NULL,
      ],
      'template' => $module_templates_prefix . 'from-to-link',
      'path' => $templates_directory,
    ],
    
    // Displays the "ask an expert" link.
    'fcl_uk_utilities_ask_an_expert_link' => [
      'variables' => [
        'first_line' => NULL,
        'second_line' => NULL,
        'url' => NULL,
      ],
      'template' => $module_templates_prefix . 'ask-an-expert-link',
      'path' => $templates_directory,
    ],
  ];
}

/**
 * Implements hook_menu_breadcrumb_alter().
 *
 * @param array $active_trail
 *   An array containing breadcrumb links for the current page.
 *
 * @param array $item
 *   The menu router item of the current page.
 */
function fcl_uk_utilities_menu_breadcrumb_alter(&$active_trail, $item) {
  // Get the current path alias.
  $path_alias = drupal_get_path_alias($_GET['q']);
  
  // If we're on the /store-locator page, remove the breadcrumb entirely.
  if ($path_alias == 'store-locator') {
    $active_trail = array();
  }
  
  // If we're on a store node, set the breadcrumb to be <a>Store Locator</a> >
  // Store Name (not linked).
  if ((arg(0) == 'node') && is_numeric(arg(1)) && empty(arg(2))) {
    // Are we on a store node type? Load the node to check.
    if ($node = node_load(arg(1))) {
      // A node loaded successfully, so we can continue.
      if ($node->type == 'store_location') {
        // The active trail is probably just the home link and the current node;
        // we want to add in a link to the store locator. The easiest way to do
        // this is to re-create the active trail.
        
        // Create a pseudo-menu item for the Store Finder (since we can't assume
        // it has its own menu item).
        $store_finder_menu_item = array(
          'href' => 'store-locator',
          'path' => 'store-locator',
          'title' => t('Store Finder'),
          'weight' => 0,
          'options' => array(),
          'localized_options' => array(),
          'access' => TRUE,
        );
        
        // Lastly, get the current store page, with a fallback in case we can't
        // get it to create a pseudo menu item from the node.
        if (!($current_store = menu_link_get_preferred())) {
          $current_store = array(
            'href' => 'node/' . $node->nid,
            'path' => 'node/%',
            'link_path' => 'node/%',
            'router_path' => 'node/%',
            'title' => $node->title,
            'weight' => 0,
            'options' => array(),
            'localized_options' => array(),
            'access' => TRUE,
          );
        }
        
        // Rebuild the active trail array.
        $active_trail = array(
          // Add in the home link.
          $active_trail[0],
          
          // Add in the store finder.
          $store_finder_menu_item,
          
          // Finally, add the current store.
          $current_store,
        );
      }
    }
  }
}

/**
 * Implements hook_libraries_info().
 */
function fcl_uk_utilities_libraries_info() {
  $libraries = [];
  
  $libraries['select2'] = array(
    'name' => 'Select2',
    'vendor url' => 'https://select2.github.io/',
    'download url' => 'https://github.com/select2/select2/releases',
    // @TODO: Detect version automagically.
    'version' => '4.0.2-rc.1',
    // Default list of files of the library to load. Important: Only specify
    // third-party files belonging to the library here, not integration files of
    // your module.
    'files' => array(
      'js' => array(
        'js/select2.min.js',
      ),
      'css' => array(
        'css/select2.min.css',
      ),
    ),
  );
  
  return $libraries;
}

/**
 * Implements hook_xmlsitemap_element_alter().
 */
function fcl_uk_utilities_xmlsitemap_element_alter(array &$element, array $link, $sitemap) {
  // Sitemap entries should display daily on change frequency.
  $element['changefreq'] = 'daily';
}
