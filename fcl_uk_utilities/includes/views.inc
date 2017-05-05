<?php
/**
 * @file
 * Views-related functions.
 */

/**
 * Implements hook_views_post_execute().
 */
function fcl_uk_utilities_views_post_execute(&$view) {
  // If we're on the fc_flights_airline_pages view with current_display
  // destination_by_class, we need to loop through the results and remove
  // duplicate results for each class/destination.
  if (($view->name == 'fc_flights_airline_pages')
    && ($view->current_display == 'destination_by_class')) {
    // This is because the
    // fcl_procat_product_handler_cheapest_product_filter filter isn't returning
    // a single result for each results cell, and we can't fix this problem
    // without fixing core, which isn't possible to do quickly because the core
    // team don't respond to our pull requests :( Maybe we need to send them
    // chocolates or some other form of gentle bribery...? :)
    // Form a two-dimensional array into which we will maintain the cheapest
    // price for each interpolation point of fcl_procat_product_destination_id
    // and fcl_procat_product_selection_id.
    $cheapest_results = $not_the_cheapest_results = $all_results
      = $results_by_destination_and_class = array();
    
    foreach (element_children($view->result) as $result_row_id) {
      // Set up some convenience variables to avoid brain failure...
      $product_id = $view->result[$result_row_id]->pid;
      $class_id = $view->result[$result_row_id]->fcl_procat_product_selection_id;
      $destination_id = $view->result[$result_row_id]->fcl_procat_product_destination_id;
      
      // Check that all the necessary arrays have been initialised.
      if (!array_key_exists($destination_id, $results_by_destination_and_class)) {
        $results_by_destination_and_class[$destination_id] = array();
      }
      
      // If there is no class_id entry, add this result straight in.
      if (!array_key_exists($class_id, $results_by_destination_and_class[$destination_id])) {
        $results_by_destination_and_class[$destination_id][$class_id] = $view->result[$result_row_id];
      }
      // If there is a class_id entry, we only add this result in if its price
      // is lower than the existing one.
      elseif (floatval($view->result[$result_row_id]->fcl_procat_product_pricead_from) < floatval($results_by_destination_and_class[$destination_id][$class_id]->fcl_procat_product_pricead_from)) {
        $results_by_destination_and_class[$destination_id][$class_id] = $view->result[$result_row_id];
      }
      
      $all_results[$product_id] = $result_row_id;
    }
    
    // Now we have the cheapest products, build a list of each of the product
    // IDs which are the cheapest. Perform an array diff between this list and
    // the list of products, and remove any results which are no longer needed
    // (i.e. which are not the cheapest price for a given destination and
    // class).
    foreach ($results_by_destination_and_class as $destination_id => $results_by_class_for_destination) {
      foreach ($results_by_class_for_destination as $class_id => $cheapest_result) {
        $cheapest_results[$cheapest_result->pid] = $all_results[$cheapest_result->pid];
      }
    }
    
    foreach (array_diff($all_results, $cheapest_results) as $product_id => $view_result_row_id) {
      unset($view->result[$view_result_row_id]);
    }
  }
}
