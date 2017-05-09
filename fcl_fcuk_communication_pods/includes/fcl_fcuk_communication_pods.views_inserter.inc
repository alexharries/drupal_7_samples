<?php

/**
 * @file
 * Functionality which allows us to inject Communication Pods into Views pages.
 */

/**
 * Implementation of hook_views_api().
 */
function fcl_fcuk_communication_pods_views_api() {
  return array(
    'api' => 3,
    // Tell Views that this module implements Views templates.
    'template path' => drupal_get_path('module', 'fcl_fcuk_communication_pods') . '/templates',
  );
}

/**
 * Implements hook_preprocess_views_view().
 *
 * @param array $variables
 *   Theme variables.
 */
function fcl_fcuk_communication_pods_preprocess_views_view_unformatted(&$variables) {
  // Implement a communication pod after the third search result on holidays
  // search pages. Get the communication pod for this view result, if we have
  // one, and insert it into the view results. This code is quite custom;
  // depending on business logic for other implementations, you might have
  // to hook into another view.

  // If we're being called via Ajax, we need to confirm that the
  // We do this by looking at $_SERVER['REQUEST_URI'] to get the view name and
  // display ID, for example:
  // /views/ajax?_processed=1&view_name=fcluk_mercury_search&view_display_id=holidays&view_args=&view_path=product_search%2Fholidays&view_base_path=&view_dom_id=83770c4888ab4cbfb24d44e3c33ddaa7&pager_element=0&sort_by=price&sort_order=ASC&page=1&_processed=1

  // If it's an ajax call, check the request URI.
  // @see/code stolen from fcl_fcuk_switch_theme.module.
  $request_uri = request_uri();
  $request_uri_parts = drupal_parse_url($request_uri);
  $request_path = $request_uri_parts['path'];
  $request_query = $request_uri_parts['query'];

  $path = current_path();

  if ($request_path == '/views/ajax') {
    if (!empty($request_query['view_name']) && ($request_query['view_name'] == 'fcluk_mercury_search')) {
      if (!empty($request_query['view_display_id']) && in_array($request_query['view_display_id'], [
          'holidays',
        ])
      ) {
        $path = 'holidays/search';
      }
    }
  }

  // Get the page number. Note that the pager starts at zero, not one.
  $page = $variables['view']->query->pager->current_page;

  // Is there a communication pod for this page?
  if ($pod = fcl_fcuk_communication_pods_get_pod_for_path($path, $page)) {
    // We has a pod! Create a render array and insert into the Views rows
    // after the third position.
    $render_array = fcl_fcuk_communication_pods_get_render_array($pod);
    array_splice($variables['rows'], 3, 0, render($render_array));

    // Also add an entry into the classes array.
    array_splice($variables['classes_array'], 3, 0, 'communication-pod-view-item-wrapper');
  }
}
