<?php
/**
 * @file
 * Flights tables-related functions.
 */

/**
 * Gets the HTML of a from-to link, typically used in a flights table.
 *
 * This function may return either a plain HTML chunk without a link wrapped
 * around it if $path is NULL or, if $path is provided, will return an HTML
 * link.
 *
 * @param string $from
 *   The plain text title of the origin. If an empty string is provided, the
 *   "From" text will not be rendered.
 *
 * @param string $to
 *   The plain text title of the destination.
 *
 * @param string|NULL $path
 *   If NULL, this function just returns the from and to text without a link; if
 *   a valid internal path is provided (i.e. one which doesn't begin with a
 *   slash), the returned HTML will be wrapped in a link. This value will be
 *   run through the url() function to create a valid URL.
 *
 * @param array $options
 *   A standard options array which follows the $options array specification for
 *   the url() function.
 *
 * @see url()
 *
 * @return string
 *   An HTML link to the destination with inline <span> tags to delineate
 *   between the "from" and "to" text.
 */
function fcl_uk_utilities_from_to_link($from, $to, $path = NULL, array $options = array()) {
  $html = theme('fcl_uk_utilities_from_to_link', [
    'from' => $from,
    'to' => $to,
  ]);
  
  // Do we have a link to build?
  if (!is_null($path)) {
    // Yes. Add the flag to render the link with HTML in it.
    $options['html'] = TRUE;
    
    return l($html, $path, $options);
  }
  else {
    // No; just return the HTML.
    return $html;
  }
}
